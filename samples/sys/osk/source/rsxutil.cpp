#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-types.h>

#include <rsx/rsx.h>
#include <sysutil/video.h>

#include "rsxutil.h"

#define GCM_LABEL_INDEX		255

CellVideoOutResolution res;
CellGcmContextData *context = NULL;

u32 curr_fb = 0;
u32 first_fb = 1;

u32 display_width;
u32 display_height;

u32 depth_pitch;
u32 depth_offset;
u32 *depth_buffer;

u32 color_pitch;
u32 color_offset[2];
u32 *color_buffer[2];

static u32 sLabelVal = 1;

static void waitFinish()
{
	rsxSetWriteBackendLabel(context,GCM_LABEL_INDEX,sLabelVal);

	rsxFlushBuffer(context);

	while(*(vu32*)cellGcmGetLabelAddress(GCM_LABEL_INDEX)!=sLabelVal)
		usleep(30);

	++sLabelVal;
}

static void waitRSXIdle()
{
	rsxSetWriteBackendLabel(context,GCM_LABEL_INDEX,sLabelVal);
	rsxSetWaitLabel(context,GCM_LABEL_INDEX,sLabelVal);

	++sLabelVal;

	waitFinish();
}

void setRenderTarget(u32 index)
{
	CellGcmSurface sf;

	sf.colorFormat		= CELL_GCM_SURFACE_X8R8G8B8;
	sf.colorTarget		= CELL_GCM_SURFACE_TARGET_0;
	sf.colorLocation[0]	= CELL_GCM_LOCATION_RSX;
	sf.colorOffset[0]	= color_offset[index];
	sf.colorPitch[0]	= color_pitch;

	sf.colorLocation[1]	= CELL_GCM_LOCATION_RSX;
	sf.colorLocation[2]	= CELL_GCM_LOCATION_RSX;
	sf.colorLocation[3]	= CELL_GCM_LOCATION_RSX;
	sf.colorOffset[1]	= 0;
	sf.colorOffset[2]	= 0;
	sf.colorOffset[3]	= 0;
	sf.colorPitch[1]	= 64;
	sf.colorPitch[2]	= 64;
	sf.colorPitch[3]	= 64;

	sf.depthFormat		= CELL_GCM_SURFACE_ZETA_Z16;
	sf.depthLocation	= CELL_GCM_LOCATION_RSX;
	sf.depthOffset		= depth_offset;
	sf.depthPitch		= depth_pitch;

	sf.type				= CELL_GCM_SURFACE_TYPE_LINEAR;
	sf.antiAlias		= CELL_GCM_SURFACE_CENTER_1;

	sf.width			= display_width;
	sf.height			= display_height;
	sf.x				= 0;
	sf.y				= 0;

	rsxSetSurface(context,&sf);
}

void init_screen(void *host_addr,u32 size)
{
	rsxInit(&context,CB_SIZE,size,host_addr);

	CellVideoOutState state;
	cellVideoOutGetState(0,0,&state);

	cellVideoOutGetResolution(state.displayMode.resolution,&res);

	CellVideoOutConfiguration vconfig;
	memset(&vconfig,0,sizeof(CellVideoOutConfiguration));

	vconfig.resolution = state.displayMode.resolution;
	vconfig.format = CELL_VIDEO_OUT_BUFFER_FORMAT_XRGB;
	vconfig.pitch = res.width*sizeof(u32);

	waitRSXIdle();

	cellVideoOutConfigure(0,&vconfig,NULL,0);
	cellVideoOutGetState(0,0,&state);

	cellGcmSetFlipMode(CELL_GCM_FLIP_VSYNC);

	display_width = res.width;
	display_height = res.height;

	color_pitch = display_width*sizeof(u32);
	color_buffer[0] = (u32*)rsxMemalign(64,(display_height*color_pitch));
	color_buffer[1] = (u32*)rsxMemalign(64,(display_height*color_pitch));

	rsxAddressToOffset(color_buffer[0],&color_offset[0]);
	rsxAddressToOffset(color_buffer[1],&color_offset[1]);

	cellGcmSetDisplayBuffer(0,color_offset[0],color_pitch,display_width,display_height);
	cellGcmSetDisplayBuffer(1,color_offset[1],color_pitch,display_width,display_height);

	depth_pitch = display_width*sizeof(u32);
	depth_buffer = (u32*)rsxMemalign(64,(display_height*depth_pitch)*2);
	rsxAddressToOffset(depth_buffer,&depth_offset);
}

void waitflip()
{
	while(cellGcmGetFlipStatus()!=0)
		usleep(200);
	cellGcmResetFlipStatus();
}

void flip()
{
	if(!first_fb) waitflip();
	else cellGcmResetFlipStatus();

	cellGcmSetFlip(context,curr_fb);
	rsxFlushBuffer(context);

	cellGcmSetWaitFlip(context);

	curr_fb ^= 1;
	setRenderTarget(curr_fb);

	first_fb = 0;
}
