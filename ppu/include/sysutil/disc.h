#ifndef __LV2_SYSUTIL_DISC_H__
#define __LV2_SYSUTIL_DISC_H__

#include <ppu-types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	SYS_DISCTYPE_OTHER = 0,
	SYS_DISCTYPE_PS3
} sysDiscType;

typedef void (*sysDiscEjectCallback)(void);
typedef void (*sysDiscInsertCallback)(u32 discType,char *title);

s32 sysDiscRegisterDiscChangeCallback(sysDiscEjectCallback cbEject,sysDiscInsertCallback cbInsert);
s32 sysDiscUnregisterDiscChangeCallback();

#ifdef __cplusplus
	}
#endif

#endif
