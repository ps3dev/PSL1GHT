#include <rsx/mm.h>
#include <rsx/gcm_sys.h>
#include <ppu-asm.h>

gcmContextData* rsxInit(const u32 cmdSize,const u32 ioSize,const void *ioAddress)
{
	s32 ret = -1;
	gcmContextData *context ATTRIBUTE_PRXPTR;

	ret = gcmInitBody(&context,cmdSize,ioSize,ioAddress);
	if(ret==0) {
		rsxHeapInit();
		return context;
	}
	return NULL;
}

void rsxSetupContextData(gcmContextData *context,const u32 *addr,const u32 size,gcmContextCallback cb)
{
	u32 alignedSize = size&~0x3;

	context->begin = (u32*)addr;
	context->current = (u32*)addr;
	context->end = (u32*)(addr + alignedSize - 4);
	context->callback = (gcmContextCallback)__get_opd32(cb);
}
