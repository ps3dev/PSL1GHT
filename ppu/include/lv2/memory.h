/*! \file lv2/memory.h
 \brief Memory management functions.
*/

#ifndef __LV2_MEMORY_H__
#define __LV2_MEMORY_H__

#include <ppu-types.h>

#ifdef __cplusplus
extern "C" {
#endif

s32 sysMMapperAllocateMemory(size_t size,u64 flags,sys_mem_id_t *mem_id);
s32 sysMMapperAllocateMemoryFromContainer(size_t size,sys_mem_container_t container,u64 flags,sys_mem_id_t *mem_id);
s32 sysMMapperFreeMemory(sys_mem_id_t mem_id);
s32 sysMMapperMapMemory(sys_mem_addr_t start_addr,sys_mem_id_t mem_id,u64 flags);
s32 sysMMapperUnmapMemory(sys_mem_addr_t start_addr,sys_mem_id_t *mem_id);

#ifdef __cplusplus
	}
#endif

#endif
