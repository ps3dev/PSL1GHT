#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(cellFsOpen, 0x718BF5F8);
EXPORT(cellFsClose, 0x2CB51F0D);
EXPORT(cellFsRead, 0x4D5FF8E2);
EXPORT(cellFsWrite, 0xECDCF2AB);
EXPORT(cellFsLseek, 0xA397D042);
EXPORT(cellFsStat, 0x7DE6DCED);
EXPORT(cellFsFstat, 0xEF3EFA34);
EXPORT(cellFsChmod, 0x99406D0B);
EXPORT(cellFsMkdir, 0xBA901FE6);
EXPORT(cellFsRmdir, 0x2796FDF3);
EXPORT(cellFsUnlink, 0x7F4677A8);
EXPORT(cellFsOpendir, 0x3F61245C);
EXPORT(cellFsClosedir, 0xFF42DCC3);
EXPORT(cellFsReaddir, 0x5C74903D);
EXPORT(cellFsAccess, 0x06E681ED);
EXPORT(cellFsUtime, 0xBEF554A4);
EXPORT(cellFsAioInit, 0xDB869F20);
EXPORT(cellFsAioReadEx, 0xC1C507E7);
EXPORT(cellFsAioWriteEx, 0x4CEF342E);
EXPORT(cellFsAioCancel, 0x7F13FC8C);
EXPORT(cellFsAioFinish, 0x9F951810);
EXPORT(cellFsGetFreeSize, 0xAA3B4BCD);
EXPORT(cellFsGetDirectoryEntries, 0x9b882495);
EXPORT(cellFsSetIoBuffer, 0x3140F6E1);
EXPORT(cellFsSetDefaultContainer, 0x02671310);
EXPORT(cellFsSetIoBufferFromDefaultContainer, 0x75F16DC5);

#endif
