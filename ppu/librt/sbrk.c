#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>

#include <sys/heap.h>
#include <sys/memory.h>
#include <sys/lv2errno.h>

#include <lv2/spinlock.h>

#define MEM_NUM_PAGES			256
#define MEM_PAGE_SIZE			(1024*1024)
#define SPACE_SIZE				(MEM_NUM_PAGES*MEM_PAGE_SIZE)

#define CEIL(size,to)			(((size)+(to)-1)&~((to)-1))
#define FLOOR(size,to)			((size)&~((to)-1))

typedef struct _region_list_entry
{
	void *top_allocated;
	void *top_reserved;
	u32 reserve_size;
	sys_mem_id_t mem_id;
	struct _region_list_entry *previous;
} region_list_entry;

static s32 __sbrk_spinlock = 0;
static heap_cntrl __region_heap;
static sys_mem_addr_t __sbrk_baseaddr = 0;
static region_list_entry *__sbrk_curr_regionentry = NULL;
static char __region_list_entries[sizeof(region_list_entry)*(MEM_NUM_PAGES + 1)] __attribute__((aligned(64)));

extern void dbg_printf(const char *fmt,...);

static int region_list_append(region_list_entry **last,void *base_reserved,u32 reserve_size,sys_mem_id_t mem_id)
{
	region_list_entry *next = (region_list_entry*)heapAllocate(&__region_heap,sizeof(region_list_entry));
	if(!next) return 0;

	next->top_allocated = (char*)base_reserved;
	next->top_reserved = (char*)base_reserved + reserve_size;
	next->reserve_size = reserve_size;
	next->mem_id = mem_id;
	next->previous = *last;
	*last = next;

	return 1;
}

static int region_list_remove(region_list_entry **last)
{
	region_list_entry *previous = (*last)->previous;

	if(!heapFree(&__region_heap,*last))
		return 0;

	*last = previous;
	
	return 1;
}

static void sbrk_init() __attribute__((constructor(103)));
static void sbrk_init()
{
	s32 ret;

	sysSpinlockInitialize(&__sbrk_spinlock);

	ret = sysMMapperAllocateAddress(SPACE_SIZE,(SYS_MEMORY_PAGE_SIZE_1M | SYS_MEMORY_ACCESS_RIGHT_ANY),0,&__sbrk_baseaddr);
	if(ret) return;

	heapInit(&__region_heap,__region_list_entries,sizeof(region_list_entry)*MEM_NUM_PAGES);
	if(!region_list_append(&__sbrk_curr_regionentry,0,0,0)) return;
}

static void sbrk_deinit() __attribute__((destructor(103)));
static void sbrk_deinit()
{
	while(__sbrk_curr_regionentry) {
		sys_mem_id_t ret_id;
		s32 release_size = __sbrk_curr_regionentry->reserve_size;
		sys_mem_addr_t base_reserved = (sys_mem_addr_t)((u64)__sbrk_curr_regionentry->top_reserved - release_size);
		
		sysMMapperUnmapMemory(base_reserved,&ret_id);
		sysMMapperFreeMemory(ret_id);

		region_list_remove(&__sbrk_curr_regionentry);
	}
	sysMMapperFreeAddress(__sbrk_baseaddr);
}

caddr_t _DEFUN(__librt_sbrk_r, (ptr,incr),
			   struct _reent *ptr _AND
			   ptrdiff_t incr)
{
	s32 ret;
	void* result = (void*)-1;

	sysSpinlockLock(&__sbrk_spinlock);

	if(incr>=0) {
		s32 allocate_size = incr;
		s32 to_reserve = (char*)__sbrk_curr_regionentry->top_allocated + allocate_size - (char*)__sbrk_curr_regionentry->top_reserved;

		if(to_reserve>0) {
			sys_mem_id_t mem_id;
			sys_mem_addr_t base_reserved;
			s32 reserve_size = CEIL(to_reserve,MEM_PAGE_SIZE);

			ret = sysMMapperAllocateMemory(reserve_size,SYS_MEMORY_PAGE_SIZE_1M,&mem_id);
			if(ret) {
				lv2errno_r(ptr,ret);
				goto sbrk_exit;
			}

			ret = sysMMapperSearchAndMap(__sbrk_baseaddr,mem_id,SYS_MEMORY_PROT_READ_WRITE,&base_reserved);
			if(ret) {
				lv2errno_r(ptr,ret);
				goto sbrk_exit;
			}

			if((u64)__sbrk_curr_regionentry->top_reserved==(u64)base_reserved) {
				s32 start_size = (u64)__sbrk_curr_regionentry->top_reserved - (u64)__sbrk_curr_regionentry->top_allocated;

				allocate_size -= start_size;

				__sbrk_curr_regionentry->top_allocated = __sbrk_curr_regionentry->top_reserved;
			}

			if(!region_list_append(&__sbrk_curr_regionentry,(void*)((u64)base_reserved),reserve_size,mem_id)) {
				lv2errno_r(ptr,ENOMEM);
				goto sbrk_exit;
			}
		}
		__sbrk_curr_regionentry->top_allocated = (char*)__sbrk_curr_regionentry->top_allocated + allocate_size;
		result = (char*)__sbrk_curr_regionentry->top_allocated - incr;
	} else if(incr<0) {
		s32 deallocate_size = -incr;

		while((char*)__sbrk_curr_regionentry->top_allocated - deallocate_size < (char*)__sbrk_curr_regionentry->top_reserved - __sbrk_curr_regionentry->reserve_size) {
			sys_mem_id_t ret_id;
			s32 release_size = __sbrk_curr_regionentry->reserve_size;
			sys_mem_addr_t base_reserved = (sys_mem_addr_t)((u64)__sbrk_curr_regionentry->top_reserved - release_size);

			ret = sysMMapperUnmapMemory(base_reserved,&ret_id);
			if(ret) {
				lv2errno_r(ptr,ret);
				goto sbrk_exit;
			}

			ret = sysMMapperFreeMemory(ret_id);
			if(ret) {
				lv2errno_r(ptr,ret);
				goto sbrk_exit;
			}

			deallocate_size -= (u64)__sbrk_curr_regionentry->top_allocated - (u64)base_reserved;

			if(!region_list_remove(&__sbrk_curr_regionentry))
				goto sbrk_exit;
		}

		__sbrk_curr_regionentry->top_allocated = (char*)__sbrk_curr_regionentry->top_allocated - deallocate_size;
		if((char*)__sbrk_curr_regionentry->top_reserved - __sbrk_curr_regionentry->reserve_size > (char*)__sbrk_curr_regionentry->top_allocated ||
			__sbrk_curr_regionentry->top_allocated > __sbrk_curr_regionentry->top_reserved)
		{
			__sbrk_curr_regionentry->top_allocated = (char*)__sbrk_curr_regionentry->top_reserved - __sbrk_curr_regionentry->reserve_size;
			lv2errno_r(ptr,ENOMEM);
			goto sbrk_exit;
		}
		result = __sbrk_curr_regionentry->top_allocated;
	}

sbrk_exit:
	sysSpinlockUnlock(&__sbrk_spinlock);

	return result;
}

/* old version - still here for historical reasons */ 
/*
#define PAGE_SIZE		(1024*1024)

#define ROUND_UP(p, round) ((p + round - 1) & ~(round - 1))

static char *memend = NULL;
static char *pageend = NULL;

caddr_t _DEFUN(__librt_sbrk_r, (ptr,incr),
			   struct _reent *ptr _AND
				   ptrdiff_t incr)
{
	if(incr==0) return (caddr_t)memend;
	if(incr<0) return (caddr_t)memend;
	if((memend + incr)<=pageend) {
		void *ret = memend;
		memend += incr;
		return ret;
	}

	mem_addr_t taddr;
	size_t allocsize = ROUND_UP(incr, PAGE_SIZE);

	if(sysMemoryAllocate(allocsize,SYS_MEMORY_PAGE_SIZE_1M,&taddr)) {
		ptr->_errno = ENOMEM;
		return (caddr_t)-1;
	}

	char *addr = (char*)((u64)taddr);
	if(pageend!=addr) memend = addr;

	pageend = addr + allocsize;
	void *ret = memend;
	memend += incr;

	return ret;
}
*/