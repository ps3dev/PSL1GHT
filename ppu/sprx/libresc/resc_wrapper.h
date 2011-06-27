#include <ppu-asm.h>
#include <rsx/resc.h>

extern void rescSetVBlankHandlerEx(opd32 *opd);
extern void rescSetFlipHandlerEx(opd32 *opd);

void rescSetVBlankHandler(void (*handler)(const u32 head))
{
	rescSetVBlankHandlerEx((opd32*)__get_opd32(handler));
}

void rescSetFlipHandler(void (*handler)(const u32 head))
{
	rescSetFlipHandlerEx((opd32*)__get_opd32(handler));
}

