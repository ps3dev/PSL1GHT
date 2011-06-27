#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(rescInit, 0x516ee89e);
EXPORT(rescExit, 0x2ea3061e);
EXPORT(rescSetDsts, 0x10db5b1a);
EXPORT(rescSetDisplayMode, 0x23134710);
EXPORT(rescGetNumColorBuffers, 0x0a2069c7);
EXPORT(rescGetBufferSize, 0x5a338cdb);
EXPORT(rescSetBufferAddress, 0x8107277c);
EXPORT(rescSetSrc, 0x6cd0f95f);
EXPORT(rescSetConvertAndFlip, 0x25c107e6);
EXPORT(rescSetWaitFlip, 0x0d3c22ce);
EXPORT(rescGetLastFlipTime, 0x66f5e388);
EXPORT(rescResetFlipStatus, 0x129922a0);
EXPORT(rescGetFlipStatus, 0xc47c5c22);
EXPORT(rescGetRegisterCount, 0x1dd3c4cd);
EXPORT(rescSetRegisterCount, 0x7af8a37f);
EXPORT(rescSetPalInterpolateDropFlexRatio, 0x19a2a967);
EXPORT(rescCreateInterlaceTable, 0xe0cef79e);
EXPORT(rescAdjustAspectRatio, 0x22ae06d8);
EXPORT(rescSetVBlankHandlerEx, 0xd3758645);
EXPORT(rescSetFlipHandlerEx, 0x2ea94661);
EXPORT(rescGcmSurface2RescSrc, 0x01220224);
EXPORT(rescVideoResolutionId2RescBufferMode, 0xd1ca0503);

#endif
