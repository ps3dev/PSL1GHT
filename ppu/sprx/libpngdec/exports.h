#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(pngDecCreate, 0x157d30c5);
EXPORT(pngDecDestroy, 0x820dae1a);
EXPORT(pngDecOpen, 0xd2bc5bfd);
EXPORT(pngDecClose, 0x5b3d1ff1);
EXPORT(pngDecReadHeader, 0x9ccdcc95);
EXPORT(pngDecDecodeData, 0x2310f155);
EXPORT(pngDecSetParameter, 0xe97c9bd4);

EXPORT(pngDecExtCreate, 0x48436b2d);
EXPORT(pngDecExtOpen, 0x0c515302);
EXPORT(pngDecExtReadHeader, 0x8b33f863);
EXPORT(pngDecExtDecodeData, 0x726fc1d0);
EXPORT(pngDecExtSetParameter, 0x9e9d7d42);
EXPORT(pngDecGetbKGD, 0x7585a275);
EXPORT(pngDecGetcHRM, 0x7a062d26);
EXPORT(pngDecGetgAMA, 0xb153629c);
EXPORT(pngDecGethIST, 0xb905ebb7);
EXPORT(pngDecGetiCCP, 0xf44b6c30);
EXPORT(pngDecGetoFFs, 0x27c921b5);
EXPORT(pngDecGetpCAL, 0xb4fe75e1);
EXPORT(pngDecGetpHYs, 0x3d50016a);
EXPORT(pngDecGetsBIT, 0x30cb334a);
EXPORT(pngDecGetsCAL, 0xc41e1198);
EXPORT(pngDecGetsPLT, 0xa5cdf57e);
EXPORT(pngDecGetsRGB, 0xe4416e82);
EXPORT(pngDecGettIME, 0x35a6846c);
EXPORT(pngDecGettRNS, 0xb96fb26e);
EXPORT(pngDecGetPLTE, 0xe163977f);
EXPORT(pngDecUnknownChunks, 0x609ec7d5);
EXPORT(pngDecGetTextChunk, 0xb40ca175);

#endif
