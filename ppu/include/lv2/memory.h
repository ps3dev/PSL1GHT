/*! \file lv2/memory.h
 \brief Memory management functions.
*/

#ifndef __LV2_MEMORY_H__
#define __LV2_MEMORY_H__

#include <ppu-types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u32 mem_id_t;
typedef u32 mem_addr_t;
typedef u32 mem_container_t;

s32 sysMMapperAllocateMemory(size_t size,u64 flags,mem_id_t *mem_id);
s32 sysMMapperAllocateMemoryFromContainer(size_t size,mem_container_t container,u64 flags,mem_id_t *mem_id);
s32 sysMMapperFreeMemory(mem_id_t mem_id);
s32 sysMMapperMapMemory(mem_addr_t start_addr,mem_id_t mem_id,u64 flags);
s32 sysMMapperUnmapMemory(mem_addr_t start_addr,mem_id_t *mem_id);

#ifdef __cplusplus
	}
#endif

#endif
