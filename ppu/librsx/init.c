#include <rsx/mm.h>
#include <rsx/gcm_sys.h>
#include <ppu-asm.h>

gcmContextData *gGcmContext ATTRIBUTE_PRXPTR = NULL;

static gcmContextData sUserContext =
{
	NULL,
	NULL,
	NULL,
	NULL
};

extern s32 gcmInitBodyEx(gcmContextData* ATTRIBUTE_PRXPTR *ctx,const u32 cmdSize,const u32 ioSize,const void *ioAddress);

s32 rsxInit(gcmContextData **context,u32 cmdSize,u32 ioSize,const void *ioAddress)
{
	s32 ret = -1;

	ret = gcmInitBodyEx(&gGcmContext,cmdSize,ioSize,ioAddress);
	if(ret==0) {
		u32 maxCmdSize = (ioSize > 0x1000) ? (ioSize - 0x1000) : 0;
		if(cmdSize > 0x7000 && cmdSize <= maxCmdSize && gGcmContext != NULL) {
			gGcmContext->end = (u32*)((uintptr_t)(void*)gGcmContext->begin + (cmdSize & ~0x3) - 4);
		}

		rsxHeapInit();

		if (context)
			*context = gGcmContext;
	}
	return ret;
}

/**
 * @brief Configures GCM context buffer pointers and registers wrap callback.
 *
 * @param context Pointer to destination gcmContextData structure.
 * @param addr Start address of the mapped command FIFO buffer.
 * @param size Total allocated byte size of the command buffer.
 * @param cb User-defined callback invoked when buffer reaches end.
 */
void rsxSetupContextData(gcmContextData *context,const u32 *addr,u32 size,gcmContextCallback cb)
{
	u32 alignedSize = size&~0x3;

	context->begin = (u32*)addr;
	context->current = (u32*)addr;
	/*
	 * Explicitly cast addr to uintptr_t (or u64) before adding the alignedSize byte
	 * count. Because addr is typed as 'const u32*', standard C pointer arithmetic
	 * (addr + alignedSize) would scale alignedSize by sizeof(u32) (4x multiplication),
	 * overflowing 4x beyond the allocated buffer and bypassing boundary checks.
	 */
	context->end = (u32*)((uintptr_t)addr + alignedSize - 4);
	context->callback = (gcmContextCallback)__get_opd32(cb);
}

void rsxSetCurrentBuffer(gcmContextData **context,const u32 *addr,u32 size)
{
	u32 alignedSize = size&~0x3;
	
	gGcmContext = &sUserContext;

	sUserContext.begin = (u32*)addr;
	sUserContext.current = (u32*)addr;
	sUserContext.end = (u32*)((u64)addr + alignedSize - 4);

	if (context)
		*context = gGcmContext;
}

void rsxSetDefaultCommandBuffer(gcmContextData **context)
{
	gcmSetDefaultCommandBuffer();
	if (context)
		*context = gGcmContext;
}

/**
 * @brief Sets user context callback handler for GCM command buffer wrap.
 *
 * In PowerPC 64-bit ELF ABI, C function pointers reference 24-byte 64-bit Procedure
 * Descriptors (OPD). However, rsxContextCallback in rsx_function_macros.h uses 32-bit
 * word loads (lwz 0, 0(%3); lwz 2, 4(%3)) expecting a 32-bit OPD descriptor layout.
 * We must explicitly transform the handler pointer via __get_opd32 to ensure the entry
 * point and TOC register are properly populated, preventing CTR=0 crashes on ring wrap.
 *
 * @param cb Function pointer to user-defined gcmContextCallback handler.
 */
void rsxSetUserCallback(gcmContextCallback cb)
{
	gcmContextCallback cb32 = (gcmContextCallback)__get_opd32(cb);
	sUserContext.callback = cb32;
	if(gGcmContext != NULL)
		gGcmContext->callback = cb32;
}

u32* rsxGetCurrentBuffer()
{
	return gGcmContext->current;
}

