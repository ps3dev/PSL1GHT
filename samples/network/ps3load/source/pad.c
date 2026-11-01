#include <sysutil/sysutil.h>

#include "pad.h"

static u32 btn_pressed = 0;
static u32 btn_pressed_old = 0;

u32 pad_read()
{
	u32 i,btn;
	u32 pad_alive;
	CellPadInfo padinfo;
	CellPadData paddata;

	cellSysutilCheckCallback();

	btn = 0;
	pad_alive = 0;
	cellPadGetInfo(&padinfo);
	for(i=0;i<CELL_MAX_PADS;i++) {
		if(padinfo.status[i]) {
			pad_alive = 1;
			cellPadGetData(i, &paddata);
			btn = ((paddata.button[2]<<8) | (paddata.button[3]&0xff));
			break;
		}
	}

	if(!pad_alive) btn = 0;

	btn_pressed = btn&(~btn_pressed_old);
	btn_pressed_old = btn;

	return btn_pressed;
}
