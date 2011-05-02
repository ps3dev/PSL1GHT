/*! \file heap.h
 \brief Heap management functions.
*/

#ifndef __SYS_HEAP_H__
#define __SYS_HEAP_H__

#include <ppu-types.h>

#define HEAP_PREV_USED				1
#define HEAP_MIN_BLOCK_SIZE			(sizeof(heap_block))
#define HEAP_BLOCK_HEADER_OFFSET	(sizeof(u32))
#define HEAP_BLOCK_USER_OFFSET		(sizeof(u32)*2)
#define HEAP_BLOCK_USED_OVERHEAD	(HEAP_BLOCK_USER_OFFSET - HEAP_BLOCK_HEADER_OFFSET)
#define HEAP_OVERHEAD				HEAP_BLOCK_USER_OFFSET


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _heap_block_t heap_block;
struct _heap_block_t
{
	u32 prev_size;
	u32 size;
	heap_block *next;
	heap_block *prev;
};

typedef struct _heap_cntrl_t
{
	heap_block free_list;
	
	u32 page_size;
	u32 min_block_size;

	void *begin;
	void *end;

	heap_block *start;
	heap_block *final;

	s32 lock;
} heap_cntrl;

u32 heapInit(heap_cntrl *theheap,void *start_addr,u32 size);
void* heapAllocate(heap_cntrl *theheap,u32 size);
void* heapAllocateAligned(heap_cntrl *theheap,u32 size,u32 alignment);
bool heapFree(heap_cntrl *theheap,void *ptr);

#ifdef __cplusplus
	}
#endif

#endif
