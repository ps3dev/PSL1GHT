#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <io/pad.h>

#include <sysutil/msg.h>
#include <sysutil/sysutil.h>

#include "rsxutil.h"

static vs32 dialog_action = 0;

extern "C" {
static void dialog_handler(CellMsgDialogButton button,void *usrData)
{
	switch(button) {
		case CELL_MSG_DIALOG_BTN_OK:
			dialog_action = 1;
			break;
		case CELL_MSG_DIALOG_BTN_NO:
		case CELL_MSG_DIALOG_BTN_ESCAPE:
			dialog_action = 2;
			break;
		case CELL_MSG_DIALOG_BTN_NONE:
			dialog_action = -1;
			break;
		default:
			break;
	}
}

static void program_exit_callback()
{
	cellGcmSetWaitFlip(context);
	rsxFinish(context,1);
}

static void sysutil_exit_callback(u64 status,u64 param,void *usrdata)
{
	switch(status) {
		case CELL_SYSUTIL_EXIT_GAME:
			break;
		case CELL_SYSUTIL_DRAW_BEGIN:
		case CELL_SYSUTIL_DRAW_END:
			break;
		default:
			break;
	}
}
}

static void do_flip()
{
	cellSysutilCheckCallback();
	flip();
}

int main(int argc,char *argv[])
{
    CellMsgDialogType dialogType;
 	void *host_addr = memalign(1024*1024,HOST_SIZE);

    printf("msgdialog test...\n");

	init_screen(host_addr,HOST_SIZE);
	cellPadInit(7);

	atexit(program_exit_callback);
	cellSysutilRegisterCallback(CELL_SYSUTIL_EVENT_SLOT0,sysutil_exit_callback,NULL);

	cellMsgDialogOpenErrorCode(0xBADC0FFE,dialog_handler,NULL,NULL);
	cellMsgDialogClose(3000.0f);

	dialog_action = 0;
	while(dialog_action!=-1)
		do_flip();

	cellMsgDialogAbort();

	// yes/no dialog type
	dialogType = (CellMsgDialogType)(CELL_MSG_DIALOG_NORMAL | CELL_MSG_DIALOG_BTN_TYPE_YESNO | CELL_MSG_DIALOG_DISABLE_CANCEL_ON | CELL_MSG_DIALOG_DEFAULT_CURSOR_NO);
	cellMsgDialogOpen2(dialogType,"Do you want to continue?",dialog_handler,NULL,NULL);

	dialog_action = 0;
	while(!dialog_action)
		do_flip();

	cellMsgDialogAbort();

	// OK dialog type
	dialogType = (CellMsgDialogType)(CELL_MSG_DIALOG_NORMAL | CELL_MSG_DIALOG_BTN_OK);
	if(dialog_action==1)
		cellMsgDialogOpen2(dialogType,"Your answer was YES",dialog_handler,NULL,NULL);
	else
		cellMsgDialogOpen2(dialogType,"Your answer was NO",dialog_handler,NULL,NULL);

	dialog_action = 0;
	while(!dialog_action)
		do_flip();

	cellMsgDialogAbort();

    printf("msgdialog test done...\n");
    return 0;
}
