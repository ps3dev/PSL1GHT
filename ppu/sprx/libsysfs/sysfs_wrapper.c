#include <stdio.h>
#include <stdlib.h>
#include <ppu-asm.h>

#include <lv2/sysfs.h>

extern s32 cellFsAioReadEx(void *aio,s32 *id,opd32 *opd);
extern s32 cellFsAioWriteEx(void *aio,s32 *id,opd32 *opd);

s32 cellFsAioRead(CellFsAio *aio,s32 *id,CellFsAioCallback cb)
{
	return cellFsAioReadEx(aio,id,(opd32*)__get_opd32(cb));
}

s32 cellFsAioWrite(CellFsAio *aio,s32 *id,CellFsAioCallback cb)
{
	return cellFsAioWriteEx(aio,id,(opd32*)__get_opd32(cb));
}
