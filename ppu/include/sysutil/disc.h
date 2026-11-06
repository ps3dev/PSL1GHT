#ifndef __LV2_SYSUTIL_DISC_H__
#define __LV2_SYSUTIL_DISC_H__

#include <ppu-types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	CELL_DISCTYPE_OTHER = 0,
	CELL_DISCTYPE_PS3,
	CELL_DISCTYPE_PS2
} CellDiscType;

typedef void (*CellDiscGameEjectCallback)(void);
typedef void (*CellDiscGameInsertCallback)(u32 discType,char *title);

s32 cellDiscGameRegisterDiscChangeCallback(CellDiscGameEjectCallback cbEject,CellDiscGameInsertCallback cbInsert);
s32 cellDiscGameUnregisterDiscChangeCallback();

#ifdef __cplusplus
	}
#endif

#endif
