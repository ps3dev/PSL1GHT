#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-types.h>

#include <sysutil/video.h>

#include <rsxdebugfontrenderer.h>

#include "rsxutil.h"

#define GCM_LABEL_INDEX		255

CellVideoOutResolution vResolution;
CellGcmContextData *context = NULL;

u32 curr_fb = 0;
u32 first_fb = 1;

u32 display_width;
u32 display_height;

u32 depth_pitch;
u32 depth_offset;
u32 *depth_buffer;

u32 color_pitch;
u32 color_offset[FRAME_BUFFER_COUNT];
u32 *color_buffer[FRAME_BUFFER_COUNT];

f32 aspect_ratio;

static u32 sResolutionIds[] = {
    CELL_VIDEO_OUT_RESOLUTION_960x1080,
    CELL_VIDEO_OUT_RESOLUTION_720,
    CELL_VIDEO_OUT_RESOLUTION_480,
    CELL_VIDEO_OUT_RESOLUTION_576
};
static size_t RESOLUTION_ID_COUNT = sizeof(sResolutionIds)/sizeof(u32);

static u32 sLabelVal = 1;

static RSXDebugFontRenderer *debugFontRenderer;

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

void initVideoConfiguration()
{
    s32 rval = 0;
    s32 resId = 0;

    for (size_t i=0;i < RESOLUTION_ID_COUNT;i++) {
        rval = cellVideoOutGetResolutionAvailability(CELL_VIDEO_OUT_PRIMARY, sResolutionIds[i], CELL_VIDEO_OUT_ASPECT_AUTO, 0);
        if (rval != 1) continue;

        resId = sResolutionIds[i];
        rval = cellVideoOutGetResolution(resId, &vResolution);
        if(!rval) break;
    }

    if(rval) {
        printf("Error: cellVideoOutGetResolutionAvailability failed. No usable resolution.\n");
        exit(1);
    }

    CellVideoOutConfiguration config = {
        (u8)resId,
        CELL_VIDEO_OUT_BUFFER_FORMAT_XRGB,
        CELL_VIDEO_OUT_ASPECT_AUTO,
        {0,0,0,0,0,0,0,0,0},
        (u32)vResolution.width*4
    };

    rval = cellVideoOutConfigure(CELL_VIDEO_OUT_PRIMARY, &config, NULL, 0);
    if(rval) {
        printf("Error: cellVideoOutConfigure failed.\n");
        exit(1);
    }

    CellVideoOutState state;

    rval = cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &state);
    switch(state.displayMode.aspect) {
        case CELL_VIDEO_OUT_ASPECT_4_3:
            aspect_ratio = 4.0f/3.0f;
            break;
        case CELL_VIDEO_OUT_ASPECT_16_9:
            aspect_ratio = 16.0f/9.0f;
            break;
        default:
            printf("unknown aspect ratio %x\n", state.displayMode.aspect);
            aspect_ratio = 16.0f/9.0f;
            break;
    }

    display_height = vResolution.height;
    display_width = vResolution.width;
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

	sf.depthFormat		= CELL_GCM_SURFACE_ZETA_Z24S8;
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
    u32 zs_depth = 4;
    u32 color_depth = 4;

	rsxInit(&context,DEFUALT_CB_SIZE,size,host_addr);

	initVideoConfiguration();

	waitRSXIdle();

	cellGcmSetFlipMode(CELL_GCM_FLIP_VSYNC);

	color_pitch = display_width*color_depth;
	depth_pitch = display_width*zs_depth;

	for (u32 i=0;i < FRAME_BUFFER_COUNT;i++) {
		color_buffer[i] = (u32*)rsxMemalign(64,(display_height*color_pitch));
		rsxAddressToOffset(color_buffer[i],&color_offset[i]);
		cellGcmSetDisplayBuffer(i,color_offset[i],color_pitch,display_width,display_height);
	}

	depth_buffer = (u32*)rsxMemalign(64, display_height*depth_pitch);
	rsxAddressToOffset(depth_buffer,&depth_offset);

	debugFontRenderer = new RSXDebugFontRenderer(context);
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
