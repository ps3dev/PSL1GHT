#include <stdio.h>
#include <ppu-asm.h>
#include <rsx/resc.h>


extern void cellRescSetVBlankHandlerEx(opd32 *opd);
extern void cellRescSetFlipHandlerEx(opd32 *opd);


void cellRescSetVBlankHandler(void (*handler)(const u32 head))
{
	cellRescSetVBlankHandlerEx((opd32*)__get_opd32(handler));
}

void cellRescSetFlipHandler(void (*handler)(const u32 head))
{
	cellRescSetFlipHandlerEx((opd32*)__get_opd32(handler));
}

