#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(sysFsOpen, 0x718BF5F8);
EXPORT(sysFsClose, 0x2CB51F0D);
EXPORT(sysFsRead, 0x4D5FF8E2);
EXPORT(sysFsWrite, 0xECDCF2AB);
EXPORT(sysFsLseek, 0xA397D042);
EXPORT(sysFsStat, 0x7DE6DCED);
EXPORT(sysFsFstat, 0xEF3EFA34);
EXPORT(sysFsChmod, 0x99406D0B);
EXPORT(sysFsMkdir, 0xBA901FE6);
EXPORT(sysFsRmdir, 0x2796FDF3);
EXPORT(sysFsUnlink, 0x7F4677A8);
EXPORT(sysFsOpendir, 0x3F61245C);
EXPORT(sysFsClosedir, 0xFF42DCC3);
EXPORT(sysFsReaddir, 0x5C74903D);
EXPORT(sysFsAioInit, 0xDB869F20);
EXPORT(sysFsAioReadEx, 0xC1C507E7);
EXPORT(sysFsAioWriteEx, 0x4CEF342E);
EXPORT(sysFsAioCancel, 0x7F13FC8C);
EXPORT(sysFsAioFinish, 0x9F951810);
EXPORT(sysFsGetFreeSize, 0xAA3B4BCD);
EXPORT(sysFsSetIoBuffer, 0x3140F6E1);
EXPORT(sysFsSetDefaultContainer, 0x02671310);
EXPORT(sysFsSetIoBufferFromDefaultContainer, 0x75F16DC5);

#endif
