#include <ppu-asm.h>
#include <rsx/gcm_sys.h>

extern void cellGcmSetVBlankHandlerEx(opd32 *opd);
extern void cellGcmSetFlipHandlerEx(opd32 *opd);
extern void cellGcmSetGraphicsHandlerEx(opd32 *opd);
extern void cellGcmSetSecondVHandlerEx(opd32 *opd);
extern void cellGcmSetUserHandlerEx(opd32 *opd);
extern void cellGcmSetQueueHandlerEx(opd32 *opd);
extern void cellGcmSetUserCommandEx(opd32 *opd);
extern s32 cellGcmIoOffsetToAddressEx(u32 offset,void* ATTRIBUTE_PRXPTR *address);
extern s32 _cellGcmInitBodyEx(CellGcmContextData* ATTRIBUTE_PRXPTR *ctx,const u32 cmdSize,const u32 ioSize,const void *ioAddress);

s32 _cellGcmInitBody(CellGcmContextData **ctx,const u32 cmdSize,const u32 ioSize,const void *ioAddress)
{
	s32 ret;
	CellGcmContextData *context ATTRIBUTE_PRXPTR;
	
	if(ctx == NULL) return -1;
	
	ret = _cellGcmInitBodyEx(&context,cmdSize,ioSize,ioAddress);
	*ctx = ret == 0 ? context : NULL;
	
	return ret;
}

s32 cellGcmIoOffsetToAddress(u32 offset,void **address)
{
	s32 ret;
	void *addr ATTRIBUTE_PRXPTR;
	
	if(address == NULL) return -1;
	
	ret = cellGcmIoOffsetToAddressEx(offset,&addr);
	*address = ret == 0 ? addr : NULL;
	
	return ret;
}

void cellGcmSetVBlankHandler(void (*handler)(const u32 head))
{
	cellGcmSetVBlankHandlerEx((opd32*)__get_opd32(handler));
}

void cellGcmSetFlipHandler(void (*handler)(const u32 head))
{
	cellGcmSetFlipHandlerEx((opd32*)__get_opd32(handler));
}

void cellGcmSetGraphicsHandler(void (*handler)(const u32 val))
{
	cellGcmSetGraphicsHandlerEx((opd32*)__get_opd32(handler));
}

void cellGcmSetSecondVHandler(void (*handler)(const u32 head))
{
	cellGcmSetSecondVHandlerEx((opd32*)__get_opd32(handler));
}

void cellGcmSetUserHandler(void (*handler)(const u32 cause))
{
	cellGcmSetUserHandlerEx((opd32*)__get_opd32(handler));
}

void cellGcmSetQueueHandler(void (*handler)(const u32 head))
{
	cellGcmSetQueueHandlerEx((opd32*)__get_opd32(handler));
}

void cellGcmSetUserCommand(void (*handler)(const u32 cause))
{
	cellGcmSetUserCommandEx((opd32*)__get_opd32(handler));
}
