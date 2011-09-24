#include <stdio.h>
#include <errno.h>
#include <ppu-asm.h>
#include <lv2/spu.h>


extern s32 sysSpuPrintfInitializeEx(int prio, opd32 *opdentry);

s32 sysSpuPrintfInitialize(int prio,void (*entry)(void*))
{
	return sysSpuPrintfInitializeEx(prio,(opd32*)__get_opd32(entry));
}
