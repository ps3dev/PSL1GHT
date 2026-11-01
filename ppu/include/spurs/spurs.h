#ifndef __SPURS_H__
#define __SPURS_H__

#include <ppu-types.h>
#include <spurs/types.h>
#ifdef __cplusplus
	extern "C" {
#endif
	
		

s32 cellSpursAttributeInitialize(CellSpursAttribute *attr, u8 nSpus,s32 spuPriority,s32 ppuPriority,bool exitIfNoWork);
s32	cellSpursInitializeWithAttribute (CellSpurs*, const CellSpursAttribute*);
s32	cellSpursInitializeWithAttribute2 (CellSpurs2*, const CellSpursAttribute*);
s32 cellSpursInitialize(CellSpurs* spurs,unsigned nSpus,int spuPriority,int ppuPriority,bool exitIfNoWork);
s32 cellSpursFinalize(CellSpurs*);
s32 cellSpursWakeUp(CellSpurs*);
s32 cellSpursGetNumSpuThread(CellSpurs*, unsigned *nThreads);
s32 cellSpursSetMaxContention(CellSpurs*,unsigned,unsigned int maxContention);
s32 cellSpursSetPriorities(CellSpurs*,unsigned,const u8 priorities[CELL_SPURS_MAX_SPU]);
s32 cellSpursGetSpuThreadId(CellSpurs*, sys_spu_thread_t*, unsigned *nThreads);
s32 cellSpursGetSpuThreadGroupId(CellSpurs*, sys_spu_group_t*);
s32 cellSpursGetInfo(CellSpurs*, CellSpursInfo *info);
s32 cellSpursGetSpuGuid(const void* pSpuGUID, uint64_t *dest);
s32 cellSpursAttributeSetNamePrefix(CellSpursAttribute* attr,const char* name,size_t size);
s32 cellSpursAttributeSetSpuThreadGroupType(CellSpursAttribute* attr, int type);
s32 cellSpursAttributeEnableSpuPrintfIfAvailable(CellSpursAttribute* attr);
s32	cellSpursAttributeSetMemoryContainerForSpuThread(CellSpursAttribute* attr,sys_mem_container_t container);

#ifdef __cplusplus
	}
#endif

#endif
