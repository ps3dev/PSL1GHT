#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(cellPngDecCreate, 0x157d30c5);
EXPORT(cellPngDecDestroy, 0x820dae1a);
EXPORT(cellPngDecOpen, 0xd2bc5bfd);
EXPORT(cellPngDecClose, 0x5b3d1ff1);
EXPORT(cellPngDecReadHeader, 0x9ccdcc95);
EXPORT(cellPngDecDecodeData, 0x2310f155);
EXPORT(cellPngDecSetParameter, 0xe97c9bd4);

EXPORT(cellPngDecExtCreate, 0x48436b2d);
EXPORT(cellPngDecExtOpen, 0x0c515302);
EXPORT(cellPngDecExtReadHeader, 0x8b33f863);
EXPORT(cellPngDecExtDecodeData, 0x726fc1d0);
EXPORT(cellPngDecExtSetParameter, 0x9e9d7d42);
EXPORT(cellPngDecGetbKGD, 0x7585a275);
EXPORT(cellPngDecGetcHRM, 0x7a062d26);
EXPORT(cellPngDecGetgAMA, 0xb153629c);
EXPORT(cellPngDecGethIST, 0xb905ebb7);
EXPORT(cellPngDecGetiCCP, 0xf44b6c30);
EXPORT(cellPngDecGetoFFs, 0x27c921b5);
EXPORT(cellPngDecGetpCAL, 0xb4fe75e1);
EXPORT(cellPngDecGetpHYs, 0x3d50016a);
EXPORT(cellPngDecGetsBIT, 0x30cb334a);
EXPORT(cellPngDecGetsCAL, 0xc41e1198);
EXPORT(cellPngDecGetsPLT, 0xa5cdf57e);
EXPORT(cellPngDecGetsRGB, 0xe4416e82);
EXPORT(cellPngDecGettIME, 0x35a6846c);
EXPORT(cellPngDecGettRNS, 0xb96fb26e);
EXPORT(cellPngDecGetPLTE, 0xe163977f);
EXPORT(cellPngDecUnknownChunks, 0x609ec7d5);
EXPORT(cellPngDecGetTextChunk, 0xb40ca175);

#endif
