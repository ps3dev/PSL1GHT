#ifndef __LV2_SYSUTIL_H__
#define __LV2_SYSUTIL_H__

#include <ppu-types.h>

#define SYSUTIL_EVENT_SLOT0			0
#define SYSUTIL_EVENT_SLOT1			1
#define SYSUTIL_EVENT_SLOT2			2
#define SYSUTIL_EVENT_SLOT3			3

#define SYSUTIL_EXIT_GAME			0x0101
#define SYSUTIL_DRAW_BEGIN			0x0121
#define SYSUTIL_DRAW_END			0x0122
#define SYSUTIL_MENU_OPEN			0x0131
#define SYSUTIL_MENU_CLOSE			0x0132
#define SYSUTIL_OSK_LOADED                      0x0502
#define SYSUTIL_OSK_DONE                        0x0503
#define SYSUTIL_OSK_UNLOADED                    0x0504

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*sysutilCallback)(u64 status,u64 param,void *usrdata);

s32 sysUtilCheckCallback();
s32 sysUtilUnregisterCallback(s32 slot);
s32 sysUtilRegisterCallback(s32 slot,sysutilCallback cb,void *usrdata);

#ifdef __cplusplus
	}
#endif

#endif
