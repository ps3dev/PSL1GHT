#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/lv2errno.h>

#include <net/net.h>
#include <sysmodule/sysmodule.h>

#define LIBNET_MEMORY_SIZE			0x20000 /* 128KB */

void* __netMemory = NULL;

s32 netInitialize()
{
	if(__netMemory) return 0;

	s32 ret;
	netInitParam params;
	
	ret = sysModuleLoad(SYSMODULE_NET);
	if(ret<0) return lv2errno(ret);

	memset(&params,0,sizeof(netInitParam));
	__netMemory = malloc(LIBNET_MEMORY_SIZE);
	
	params.memory = (u32)((u64)__netMemory);
	params.memory_size = LIBNET_MEMORY_SIZE;
	params.flags = 0;
	ret = netInitializeNetworkEx(&params);
	if(ret) {
		free(__netMemory);
		__netMemory = NULL;
	}
	return ret;
}

s32 netDeinitialize()
{
	netFinalizeNetwork();

	if(__netMemory) free(__netMemory);
	__netMemory = NULL;

	sysModuleUnload(SYSMODULE_NET);
	return 0;
}
