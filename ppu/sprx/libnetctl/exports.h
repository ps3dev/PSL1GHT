#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(cellNetCtlInit, 0xbd5a59fc);
EXPORT(cellNetCtlTerm, 0x105ee2cb);
EXPORT(cellNetCtlGetInfo, 0x1e585b5d);
EXPORT(cellNetCtlGetNatInfo, 0x3a12865f);
EXPORT(cellNetCtlGetState, 0x8b3eba69);
EXPORT(cellNetCtlAddHandler, 0x0ce13c6b);
EXPORT(cellNetCtlDelHandler, 0x901815c3);

EXPORT(cellNetCtlNetStartDialogLoadAsync, 0x04459230);
EXPORT(cellNetCtlNetStartDialogUnloadAsync, 0x0f1f13d3);
EXPORT(cellNetCtlNetStartDialogAbortAsync, 0x71d53210);

EXPORT(cellGameUpdateInit, 0x99ab1a26);
EXPORT(cellGameUpdateTerm, 0x10dae56d);
EXPORT(cellGameUpdateCheckAbort, 0x3e359ab6);
EXPORT(cellGameUpdateCheckStartAsync, 0xd0a5d727);
EXPORT(cellGameUpdateCheckStartAsyncEx, 0x0791015f);
EXPORT(cellGameUpdateCheckStartWithoutDialogAsync, 0xa5e1fa60);
EXPORT(cellGameUpdateCheckStartWithoutDialogAsyncEx, 0x558700f6);
EXPORT(cellGameUpdateCheckFinishAsync, 0xffa3d791);
EXPORT(cellGameUpdateCheckFinishAsyncEx, 0xf463981c);

#endif
