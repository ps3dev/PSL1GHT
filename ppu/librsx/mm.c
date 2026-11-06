#include <stdlib.h>

#include <gcm_sys.h>
#include <sys/heap.h>

#define SAFE_AREA			4096

static CellGcmConfiguration __rsx_config;

static heap_cntrl __rsx_heap;
static u32 __rsxheap_initialized = 0;

s64 rsxHeapInit()
{
	if(!__rsxheap_initialized) {
		void *heapBuffer;
		u32 heapBufferSize;

		cellGcmGetConfiguration(&__rsx_config);
		
		heapBuffer = __rsx_config.localAddress;
		heapBufferSize = __rsx_config.localSize - SAFE_AREA;
		heapInit(&__rsx_heap,heapBuffer,heapBufferSize);
		__rsxheap_initialized = 1;
	}

	return 1;
}

void* rsxMemalign(u32 alignment,u32 size)
{
	if(!__rsxheap_initialized) return NULL;
	return heapAllocateAligned(&__rsx_heap,size,alignment);
}

void* rsxMalloc(u32 size)
{
	if(!__rsxheap_initialized) return NULL;
	return heapAllocate(&__rsx_heap,size);
}

void rsxFree(void *ptr)
{
	if(!__rsxheap_initialized) return;
	heapFree(&__rsx_heap,ptr);
}
