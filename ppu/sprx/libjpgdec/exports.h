#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(cellJpgDecCreate, 0xa7978f59);
EXPORT(cellJpgDecDestroy, 0xd8ea91f8);
EXPORT(cellJpgDecOpen, 0x976ca5c2);
EXPORT(cellJpgDecClose, 0x9338a07a);
EXPORT(cellJpgDecReadHeader, 0x6d9ebccf);
EXPORT(cellJpgDecDecodeData, 0xaf8bb012);
EXPORT(cellJpgDecSetParameter, 0xe08f3910);

EXPORT(cellJpgDecExtSetParameter, 0x65cbbb16);
EXPORT(cellJpgDecExtDecodeData, 0x716f8792);
EXPORT(cellJpgDecExtOpen, 0xa9f703e3);
EXPORT(cellJpgDecExtReadHeader, 0xb91eb3d2);

#endif
