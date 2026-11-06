#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <math.h>
#include <ppu-types.h>

#include <sys/process.h>

#include <io/pad.h>
#include <rsx/rsx.h>
#include <sysutil/sysutil.h>

#include "rsxutil.h"
#include "bitmap.h"
#include "psl1ght.xpm"

u32 running = 0;
u32 frame_cnt = 0;

SYS_PROCESS_PARAM(1001, 0x100000);

extern "C" {
static void program_exit_callback()
{
    cellSysutilUnregisterCallback(CELL_SYSUTIL_EVENT_SLOT0);
	cellGcmSetWaitFlip(context);
	rsxFinish(context,1);
}

static void sysutil_exit_callback(u64 status,u64 param,void *usrdata)
{
	switch(status) {
		case CELL_SYSUTIL_EXIT_GAME:
			running = 0;
			break;
		case CELL_SYSUTIL_DRAW_BEGIN:
		case CELL_SYSUTIL_DRAW_END:
			break;
		default:
			break;
	}
}
}

void blit_simple(Bitmap *bitmap, u32 dstX, u32 dstY, u32 srcX, u32 srcY, u32 w, u32 h)
{
  rsxSetTransferImage(context, CELL_GCM_TRANSFER_LOCAL_TO_LOCAL,
    color_offset[curr_fb], color_pitch, dstX-w/2, dstY-h/2,
    bitmap->offset, bitmap->width*4, rsxGetFixedUint16((float)srcX),
    rsxGetFixedUint16((float)srcY), w, h, 4);
}

void blit_data(Bitmap *bitmap, u32 dstX, u32 dstY, u32 srcX, u32 srcY, u32 w, u32 h)
{
  rsxSetTransferData(context, CELL_GCM_TRANSFER_LOCAL_TO_LOCAL,
    color_offset[curr_fb], color_pitch, bitmap->offset, bitmap->width*4,
    w*4, h);
}

void blit_scale(Bitmap *bitmap, u32 dstX, u32 dstY, u32 srcX, u32 srcY, u32 w, u32 h, float zoom)
{
  CellGcmTransferScale scale;
  CellGcmTransferSurface surface;

  scale.conversion = CELL_GCM_TRANSFER_CONVERSION_TRUNCATE;
  scale.format = CELL_GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
  scale.origin = CELL_GCM_TRANSFER_ORIGIN_CORNER;
  scale.operation = CELL_GCM_TRANSFER_OPERATION_SRCCOPY_AND;
  scale.interp = CELL_GCM_TRANSFER_INTERPOLATOR_NEAREST;
  scale.clipX = 0;
  scale.clipY = 0;
  scale.clipW = display_width;
  scale.clipH = display_height;
  scale.outX = dstX - w*zoom*.5f;
  scale.outY = dstY - h*zoom*.5f;
  scale.outW = w * zoom;
  scale.outH = h * zoom;
  scale.ratioX = rsxGetFixedSint32(1.f / zoom);
  scale.ratioY = rsxGetFixedSint32(1.f / zoom);
  scale.inX = rsxGetFixedUint16(srcX);
  scale.inY = rsxGetFixedUint16(srcY);
  scale.inW = bitmap->width;
  scale.inH = bitmap->height;
  scale.offset = bitmap->offset;
  scale.pitch = sizeof(u32) * bitmap->width;

  surface.format = CELL_GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
  surface.pitch = color_pitch;
  surface.offset = color_offset[curr_fb];

  rsxSetTransferScaleMode(context, CELL_GCM_TRANSFER_LOCAL_TO_LOCAL, CELL_GCM_TRANSFER_SURFACE);
  rsxSetTransferScaleSurface(context, &scale, &surface);
}

int main(int argc,const char *argv[])
{
    int k = 0;
	CellPadInfo padinfo;
	CellPadData paddata;
    Bitmap bitmap;
	void *host_addr = memalign(1024*1024,HOST_SIZE);

	printf("blitting started...\n");

	init_screen(host_addr,HOST_SIZE);
	cellPadInit(7);

	atexit(program_exit_callback);
	cellSysutilRegisterCallback(CELL_SYSUTIL_EVENT_SLOT0,sysutil_exit_callback,NULL);

    bitmapSetXpm(&bitmap, psl1ght_xpm);
	setRenderTarget(curr_fb);

	running = 1;
	while(running) {
		cellSysutilCheckCallback();

		cellPadGetInfo(&padinfo);
		for(int i=0; i < CELL_MAX_PADS; i++){
			if(padinfo.status[i]){
				cellPadGetData(i, &paddata);

				if(paddata.BTN_CROSS)
					goto done;
			}

		}
		
        /* Display some stuff on the screen */
        rsxSetClearColor(context, 0x200030);
        rsxSetClearDepthStencil(context, 0xffff);
        rsxClearSurface(context,CELL_GCM_CLEAR_R |
                                        CELL_GCM_CLEAR_G |
                                        CELL_GCM_CLEAR_B |
                                        CELL_GCM_CLEAR_A |
                                        CELL_GCM_CLEAR_S |
                                        CELL_GCM_CLEAR_Z);

        /* Enable blending (for rsxSetTransferScaleSurface) */
        rsxSetBlendFunc(context, CELL_GCM_SRC_ALPHA, CELL_GCM_ONE_MINUS_SRC_ALPHA, CELL_GCM_SRC_ALPHA, CELL_GCM_ONE_MINUS_SRC_ALPHA);
        rsxSetBlendEquation(context, CELL_GCM_FUNC_ADD, CELL_GCM_FUNC_ADD);
        rsxSetBlendEnable(context, CELL_GCM_TRUE);

        /* Display the whole PSL1GHT image */
        blit_simple(&bitmap, display_width/4, 100, 0, 0, bitmap.width, bitmap.height);

        /* Distort the PSL1GHT image by displaying lines with different X coords */
        for (u32 i=0; i<bitmap.height; ++i) {
            int x = display_width*3/4 + 50.f*sinf(((i+k) & 0x7f) * (2.f*M_PI/0x80));
            blit_simple(&bitmap, x, 100+i, 0, i, bitmap.width, 1);
        }
        k = (k+1) & 0x7f;

        /* Animate all letters */
        for (int i=6; i>=0; --i) {
            int x = display_width * (0.1f + 0.8f * (0.5f+0.5f*sinf((frame_cnt - 10*i) * .01f)));
            int y = 150 + 400 * (0.5f+0.5f*sinf((frame_cnt - 10*i) * .02f));
            blit_simple(&bitmap, x, y, i*32, 0, 32, bitmap.height);
        }

        /* Animate all letters, with zoom */
        for (int i=6; i>=0; --i) {
            int x = display_width * (0.1f + 0.8f * (0.5f+0.5f*sinf(M_PI + (frame_cnt - 10*i) * .01f)));
            int y = 150 + 400 * (0.5f+0.5f*sinf((frame_cnt - 10*i) * .02f));
            blit_scale(&bitmap, x, y, i*32, 0, 32, bitmap.height, 2.f);
        }

		flip();
        frame_cnt++;
	}

done:
    printf("blitting finished...\n");
    
    bitmapDestroy(&bitmap);
    return 0;
}
