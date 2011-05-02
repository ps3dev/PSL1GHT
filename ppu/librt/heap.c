#include <stdlib.h>
#include <ppu-asm.h>
#include <lv2/spinlock.h>

#include "heap.h"
#include "heap.inl"

static u32 __heap_calc_block_size(u32 size,u32 page_size,u32 min_size)
{
	u32 block_size = size + HEAP_BLOCK_USED_OVERHEAD;

	__heap_align_up(&block_size,page_size);
	if(block_size<min_size) block_size = min_size;

	return (block_size>size) ? block_size : 0;
}

static u32 __heap_block_allocate(heap_cntrl *theheap,heap_block *block,u32 alloc_size)
{
	u32 const block_size = __heap_block_size(block);
	u32 const rest = block_size - alloc_size;

	if(rest>=theheap->min_block_size) {
		heap_block *next_block = __heap_block_at(block,alloc_size);
	
		__heap_block_replace(block,next_block);

		block->size = alloc_size|HEAP_PREV_USED;
		next_block->size = rest|HEAP_PREV_USED;

		__heap_block_at(next_block,rest)->prev_size = rest;
	} else {
		__heap_block_remove(block);

		alloc_size = block_size;
		__heap_block_at(block,alloc_size)->size |= HEAP_PREV_USED;
	}

	return alloc_size;
}

static heap_block* __block_allocate(heap_cntrl *theheap,heap_block *block,u32 alloc_size)
{
	u32 const block_size = __heap_block_size(block);
	u32 const rest = block_size - alloc_size;

	if(rest>=theheap->min_block_size) {
		block->size = rest|HEAP_PREV_USED;

		block = __heap_block_at(block,rest);
		block->prev_size = rest;
		block->size = alloc_size;
	} else {
		__heap_block_remove(block);

		alloc_size = block_size;
	}

	__heap_block_at(block,alloc_size)->size |= HEAP_PREV_USED;

	return block;
}

u32 heapInit(heap_cntrl *theheap,void *start_addr,u32 size)
{
	u32 page_size;
	heap_block *block;
	u32 dsize,overhead;
	u64 start,aligned_start;

	page_size = PPU_ALIGNMENT;

	start = (u64)start_addr;
	aligned_start = start + HEAP_BLOCK_USER_OFFSET;
	__heap_align_up_ptr(&aligned_start,page_size);
	aligned_start -= HEAP_BLOCK_USER_OFFSET;

	theheap->min_block_size = HEAP_MIN_BLOCK_SIZE;
	__heap_align_up(&theheap->min_block_size,page_size);

	overhead = HEAP_OVERHEAD + (aligned_start - start);
	if(size<overhead) return 0;

	dsize = size - overhead;
	__heap_align_down(&dsize,page_size);
	if(dsize==0) return 0;

	theheap->page_size = page_size;
	theheap->begin = start_addr;
	theheap->end = start_addr + size;

	block = (heap_block*)aligned_start;
	block->prev_size = page_size;
	block->size = dsize|HEAP_PREV_USED;
	block->next = __heap_tail(theheap);
	block->prev = __heap_head(theheap);
	__heap_head(theheap)->next = block;
	__heap_tail(theheap)->prev = block;

	theheap->start = block;

	block = __heap_block_at(block,dsize);
	block->prev_size = dsize;
	block->size = page_size;

	theheap->final = block;

	sysSpinlockInitialize(&theheap->lock);

	return (dsize - HEAP_BLOCK_USED_OVERHEAD);
}

void* heapAllocate(heap_cntrl *theheap,u32 size)
{
	u32 dsize;
	void *ptr = NULL;
	heap_block *block;
	heap_block *const tail = __heap_tail(theheap);

	dsize = __heap_calc_block_size(size,theheap->page_size,theheap->min_block_size);
	if(dsize==0) return NULL;

	sysSpinlockLock(&theheap->lock);
	for(block=__heap_first(theheap);block!=tail;block=block->next) {
		u32 const block_size = __heap_block_size(block);

		if(__heap_prev_used(block) && block_size>=dsize) {
			__heap_block_allocate(theheap,block,dsize);

			ptr = __heap_block_user(block);
			break;
		}
	}
	sysSpinlockUnlock(&theheap->lock);
	
	return ptr;
}

void* heapAllocateAligned(heap_cntrl *theheap,u32 size,u32 alignment)
{
	void *ptr = NULL;
	heap_block *block;
	u32 const page_size = theheap->page_size;
	heap_block *const tail = __heap_tail(theheap);
	u32 const end_of_user_offs = size - HEAP_BLOCK_HEADER_OFFSET;
	u32 const dsize = __heap_calc_block_size(size,page_size,theheap->min_block_size);

	if(dsize==0) return NULL;
	if(alignment==0) alignment = PPU_ALIGNMENT;

	sysSpinlockLock(&theheap->lock);
	for(block=__heap_first(theheap);block!=tail;block=block->next) {
		u32 const block_size = __heap_block_size(block);

		if(__heap_prev_used(block) && block_size>=dsize) {
			u64 user_addr,aligned_user_addr;
			u64 const user_area = (u64)__heap_block_user(block);
			u64 const block_end = (u64)block + block_size;

			aligned_user_addr = block_end - end_of_user_offs;
			__heap_align_down_ptr(&aligned_user_addr,alignment);

			user_addr = aligned_user_addr;
			__heap_align_down_ptr(&user_addr,page_size);

			if(user_addr>=user_area) {
				if((user_addr - user_area)<theheap->min_block_size) {
					user_addr = user_area;

					if((aligned_user_addr - user_addr)>=page_size) {
						aligned_user_addr = user_addr;

						__heap_align_up_ptr(&aligned_user_addr,alignment);
						if((aligned_user_addr - user_addr)>=page_size) aligned_user_addr = 0;
					}
				}

				if(aligned_user_addr) {
					u32 const alloc_size = block_end - user_addr + HEAP_BLOCK_USER_OFFSET;

					block = __block_allocate(theheap,block,alloc_size);

					ptr = (void*)aligned_user_addr;
					break;
				}
			}
		}
	}
	sysSpinlockUnlock(&theheap->lock);

	return ptr;
}

bool heapFree(heap_cntrl *theheap,void *ptr)
{
	heap_block *block;
	heap_block *next_block;
	u32 dsize,next_size;
	bool next_is_free;

	if(!((u64)ptr>=(u64)theheap->start && (u64)ptr<=(u64)theheap->final)) return false;

	sysSpinlockLock(&theheap->lock);

	__heap_block_start(theheap,ptr,&block);

	if(!__heap_block_in(theheap,block)) {
		sysSpinlockUnlock(&theheap->lock);
		return false;
	}

	dsize = __heap_block_size(block);
	next_block = __heap_block_at(block,dsize);

	if(!__heap_block_in(theheap,next_block) || !__heap_prev_used(next_block)) {
		sysSpinlockUnlock(&theheap->lock);
		return false;
	}

	next_size = __heap_block_size(next_block);
	next_is_free = next_block<theheap->final && !__heap_prev_used(__heap_block_at(next_block,next_size));

	if(!__heap_prev_used(block)) {
		u32 const prev_size = block->prev_size;
		heap_block *const prev_block = __heap_block_at(block,-prev_size);

		if(!__heap_block_in(theheap,prev_block) || !__heap_prev_used(prev_block)) {
			sysSpinlockUnlock(&theheap->lock);
			return false;
		}

		if(next_is_free) {
			u32 const size = dsize + prev_size + next_size;

			__heap_block_remove(next_block);

			prev_block->size = size|HEAP_PREV_USED;

			next_block = __heap_block_at(prev_block,size);
			next_block->prev_size = size;
		} else {
			u32 const size = dsize + prev_size;

			prev_block->size = size|HEAP_PREV_USED;
			next_block->size &= ~HEAP_PREV_USED;
			next_block->prev_size = size;
		}
	} else if(next_is_free) {
		u32 const size = dsize + next_size;

		__heap_block_replace(next_block,block);

		block->size = size|HEAP_PREV_USED;

		next_block = __heap_block_at(block,size);
		next_block->prev_size = size;
	} else {
		__heap_block_insert_after(__heap_head(theheap),block);

		block->size = dsize|HEAP_PREV_USED;
		next_block->size &= ~HEAP_PREV_USED;
		next_block->prev_size = dsize;
	}
	sysSpinlockUnlock(&theheap->lock);

	return true;
}
