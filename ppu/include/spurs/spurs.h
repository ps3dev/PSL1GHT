#ifndef __SPURS_H__
#define __SPURS_H__

#include <ppu-types.h>
#include <spurs/types.h>
#ifdef __cplusplus
	extern "C" {
#endif
	
		

s32 spursAttributeInitialize(SpursAttribute *attr, u8 nSpus,s32 spuPriority,s32 ppuPriority,bool exitIfNoWork);
s32	spursInitializeWithAttribute (Spurs*, const SpursAttribute*);
s32	spursInitializeWithAttribute2 (Spurs2*, const SpursAttribute*);
s32 spursInitialize(Spurs* spurs,unsigned nSpus,int spuPriority,int ppuPriority,bool exitIfNoWork);
s32 spursFinalize(Spurs*);
s32 spursWakeUp(Spurs*);
s32 spursGetNumSpuThread(Spurs*, unsigned *nThreads);
s32 spursSetMaxContention(Spurs*,unsigned,unsigned int maxContention);
s32 spursSetPriorities(Spurs*,unsigned,const u8 priorities[SPURS_MAX_SPU]);
s32 spursGetSpuThreadId(Spurs*, sys_spu_thread_t*, unsigned *nThreads);
s32 spursGetSpuThreadGroupId(Spurs*, sys_spu_group_t*);
s32 spursGetInfo(Spurs*, SpursInfo *info);
s32 spursGetSpuGuid(const void* pSpuGUID, uint64_t *dest);
s32 spursAttributeSetNamePrefix(SpursAttribute* attr,const char* name,size_t size);
s32 spursAttributeSetSpuThreadGroupType(SpursAttribute* attr, int type);
s32 spursAttributeEnableSpuPrintfIfAvailable(SpursAttribute* attr);
s32	spursAttributeSetMemoryContainerForSpuThread(SpursAttribute* attr,sys_mem_container_t container);

#ifdef __cplusplus
	}
#endif

#endif
