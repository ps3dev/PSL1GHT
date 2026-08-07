#ifndef __LV2_MSG_H__
#define __LV2_MSG_H__

#include <ppu-types.h>

#define MSG_PROGRESSBAR_INDEX0			0
#define MSG_PROGRESSBAR_INDEX1			1

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	CELL_MSG_DIALOG_ERROR				= 0,
	CELL_MSG_DIALOG_NORMAL				= 1,
	CELL_MSG_DIALOG_MUTE_ON				= 2,
	CELL_MSG_DIALOG_BKG_INVISIBLE		= 4,
	CELL_MSG_DIALOG_BTN_TYPE_YESNO		= 16,
	CELL_MSG_DIALOG_BTN_TYPE_OK			= 32,
	CELL_MSG_DIALOG_DISABLE_CANCEL_ON	= 128,
	CELL_MSG_DIALOG_DEFAULT_CURSOR_NO	= 256,

	CELL_MSG_DIALOG_SINGLE_PROGRESSBAR	= 4096,
	CELL_MSG_DIALOG_DOUBLE_PROGRESSBAR	= 8192
} CellMsgDialogType;

typedef enum
{
	CELL_MSG_DIALOG_BTN_NONE				= -1,
	CELL_MSG_DIALOG_BTN_INVALID,
	CELL_MSG_DIALOG_BTN_OK,
	CELL_MSG_DIALOG_BTN_YES				= 1,
	CELL_MSG_DIALOG_BTN_NO,
	CELL_MSG_DIALOG_BTN_ESCAPE
} CellMsgDialogButton;

typedef void (*CellMsgDialogCallback)(CellMsgDialogButton button,void *usrData);

s32 cellMsgDialogAbort();
s32 cellMsgDialogClose(f32 waitMs);
s32 cellMsgDialogOpen(CellMsgDialogType type,const char *msg,CellMsgDialogCallback cb,void *usrData,void *unused);
s32 cellMsgDialogOpen2(CellMsgDialogType type,const char *msg,CellMsgDialogCallback cb,void *usrData,void *unused);
s32 cellMsgDialogOpenErrorCode(u32 errorCode,CellMsgDialogCallback cb,void *usrData,void *unused);

s32 cellMsgDialogProgressBarSetMsg(u32 index,const char *msg);
s32 cellMsgDialogProgressBarReset(u32 index);
s32 cellMsgDialogProgressBarInc(u32 index,u32 percent);

#ifdef __cplusplus
	}
#endif

#endif
