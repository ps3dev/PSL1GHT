/*! \file sys/memory.h
 \brief Memory management functions.
*/

#ifndef __SYS_MEMORY_H__
#define __SYS_MEMORY_H__

#include <ppu-lv2.h>
#include <lv2/memory.h>

#define SYS_MEMORY_PAGE_SIZE_1M				0x0000000000000400ULL
#define SYS_MEMORY_PAGE_SIZE_64K			0x0000000000000200ULL

#define SYS_MEMORY_ACCESS_RIGHT_PPU_THR		0x0000000000000008ULL
#define SYS_MEMORY_ACCESS_RIGHT_HANDLER		0x0000000000000004ULL
#define SYS_MEMORY_ACCESS_RIGHT_SPU_THR		0x0000000000000002ULL
#define SYS_MEMORY_ACCESS_RIGHT_RAW_SPU		0x0000000000000001ULL
#define SYS_MEMORY_ACCESS_RIGHT_ANY			(SYS_MEMORY_ACCESS_RIGHT_PPU_THR | \
											 SYS_MEMORY_ACCESS_RIGHT_HANDLER | \
											 SYS_MEMORY_ACCESS_RIGHT_SPU_THR | \
											 SYS_MEMORY_ACCESS_RIGHT_RAW_SPU)
#define SYS_MEMORY_ACCESS_RIGHT_NONE		0x00000000000000f0ULL

#define SYS_MEMORY_PROT_READ_ONLY			0x0000000000080000ULL
#define SYS_MEMORY_PROT_READ_WRITE			0x0000000000040000ULL

#ifdef __cplusplus
	extern "C" {
#endif

LV2_SYSCALL sysMemoryAllocate(size_t size,u64 flags,sys_mem_addr_t *alloc_addr)
{
	lv2syscall3(348,size,flags,(u64)alloc_addr);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysMemoryFree(sys_mem_addr_t start_addr)
{
	lv2syscall1(349,start_addr);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysMMapperAllocateAddress(size_t size,u64 flags,size_t alignment,sys_mem_addr_t *alloc_addr)
{
	lv2syscall4(330,size,flags,alignment,(u64)alloc_addr);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysMMapperFreeAddress(sys_mem_addr_t start_addr)
{
	lv2syscall1(331,start_addr);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysMMapperSearchAndMap(sys_mem_addr_t start_addr,sys_mem_id_t mem_id,u64 flags,sys_mem_addr_t *alloc_addr)
{
	lv2syscall4(337,start_addr,mem_id,flags,(u64)alloc_addr);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysMemContainerCreate(sys_mem_container_t *container,size_t size)
{
	lv2syscall2(324,(u64)container,size);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysMemContainerDestroy(sys_mem_container_t container)
{
	lv2syscall1(325,container);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysMemAllocateFromContainer(size_t size,sys_mem_container_t container,u64 flags,sys_mem_addr_t *alloc_addr)
{
	lv2syscall4(350,size,container,flags,(u64)alloc_addr);
	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
