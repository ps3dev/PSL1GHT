#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-types.h>

#include <sys/event_queue.h>
#include <sysutil/video.h>

#include <rsxdebugfontrenderer.h>

#include "rsxutil.h"

CellVideoOutResolution vResolution;

u32 curr_fb = 0;

u32 display_width;
u32 display_height;

u32 depth_pitch;
u32 depth_offset;
void *depth_buffer;

u32 color_pitch;
u32 color_offset[FRAME_BUFFER_COUNT];
void *color_buffer[FRAME_BUFFER_COUNT];

void *state_buffer;
u32 state_offset;

f32 aspect_ratio;

u32 fbOnDisplay = 0;
u32 fbFlipped = 0;
bool fbOnFlip = false;
sys_event_queue_t flipEventQueue;
sys_event_port_t flipEventPort;

CellGcmSurface surface;

static u32 sLabelVal = 1;
static RSXDebugFontRenderer *fontRenderer;

static u32 sResolutionIds[] = {
    CELL_VIDEO_OUT_RESOLUTION_1600x1080,
    CELL_VIDEO_OUT_RESOLUTION_1440x1080,
    CELL_VIDEO_OUT_RESOLUTION_1280x1080,
    CELL_VIDEO_OUT_RESOLUTION_960x1080,
    CELL_VIDEO_OUT_RESOLUTION_720,
    CELL_VIDEO_OUT_RESOLUTION_480,
    CELL_VIDEO_OUT_RESOLUTION_576
};
static size_t RESOLUTION_ID_COUNT = sizeof(sResolutionIds)/sizeof(u32);

extern "C" {
static void flipHandler(const u32 head)
{
    (void)head;
    u32 v = fbFlipped;

    for (u32 i = fbOnDisplay; i != v; i=(i + 1)%FRAME_BUFFER_COUNT) {
        *((vu32*) cellGcmGetLabelAddress(CELL_GCM_BUFFER_STATUS_INDEX + i)) = BUFFER_IDLE;
    }
    fbOnDisplay = v;
    fbOnFlip = false;

    sysEventPortSend(flipEventPort, 0, 0, 0);
}

static void vblankHandler(const u32 head)
{
    (void)head;
    u32 data;
    u32 bufferToFlip;
    u32 indexToFlip;

    data = *((vu32*) cellGcmGetLabelAddress(CELL_GCM_PREPARED_BUFFER_INDEX));
    bufferToFlip = (data >> 8);
    indexToFlip = (data & 0x07);

    if (!fbOnFlip) {
        if (bufferToFlip != fbOnDisplay) {
            s32 ret = cellGcmSetFlipImmediate(indexToFlip);
            if (ret != 0) {
                printf("flip immediate failed\n");
                return;
            }
            fbFlipped = bufferToFlip;
            fbOnFlip = true;
        }
    }
}
}

static void syncPPUGPU()
{
    vu32 *label = (vu32*) cellGcmGetLabelAddress(CELL_GCM_PREPARED_BUFFER_INDEX);
    while(((curr_fb + FRAME_BUFFER_COUNT - ((*label)>>8))%FRAME_BUFFER_COUNT) > MAX_BUFFER_QUEUE_SIZE) {
        sys_event_t event;

        sysEventQueueReceive(flipEventQueue, &event, 0);
        sysEventQueueDrain(flipEventQueue);
    }
}

static void waitRSXFinish()
{
	rsxSetWriteBackendLabel(gGcmContext,CELL_GCM_WAIT_LABEL_INDEX,sLabelVal);

	rsxFlushBuffer(gGcmContext);

	while(*(vu32*)cellGcmGetLabelAddress(CELL_GCM_WAIT_LABEL_INDEX)!=sLabelVal)
		usleep(30);

	++sLabelVal;
}

static void waitRSXIdle()
{
	rsxSetWriteBackendLabel(gGcmContext,CELL_GCM_WAIT_LABEL_INDEX,sLabelVal);
	rsxSetWaitLabel(gGcmContext,CELL_GCM_WAIT_LABEL_INDEX,sLabelVal);

	++sLabelVal;

	waitRSXFinish();
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
        cellGcmGetTiledPitchSize(vResolution.width*4)
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

void initFlipEvent()
{
    sys_event_queue_attr_t queueAttr = { SYS_EVENT_QUEUE_PRIO, SYS_EVENT_QUEUE_PPU, "\0" };

    sysEventQueueCreate(&flipEventQueue, &queueAttr, SYS_EVENT_QUEUE_KEY_LOCAL, 32);
    sysEventPortCreate(&flipEventPort, SYS_EVENT_PORT_LOCAL, SYS_EVENT_PORT_NO_NAME);
    sysEventPortConnectLocal(flipEventPort, flipEventQueue);

    cellGcmSetFlipHandler(flipHandler);
    cellGcmSetVBlankHandler(vblankHandler);
}

void initRenderTarget()
{
    memset(&surface, 0, sizeof(CellGcmSurface));

	surface.colorFormat		= CELL_GCM_SURFACE_X8R8G8B8;
	surface.colorTarget		= CELL_GCM_SURFACE_TARGET_0;
	surface.colorLocation[0]	= CELL_GCM_LOCATION_RSX;
	surface.colorOffset[0]	= color_offset[curr_fb];
	surface.colorPitch[0]	= color_pitch;

    for(u32 i=1; i< CELL_GCM_MAX_MRT_COUNT;i++) {
        surface.colorLocation[i]	= CELL_GCM_LOCATION_RSX;
        surface.colorOffset[i]		= color_offset[curr_fb];
        surface.colorPitch[i]		= 64;
    }

	surface.depthFormat		= CELL_GCM_SURFACE_ZETA_Z24S8;
	surface.depthLocation	= CELL_GCM_LOCATION_RSX;
	surface.depthOffset		= depth_offset;
	surface.depthPitch		= depth_pitch;

	surface.type			= CELL_GCM_SURFACE_TYPE_LINEAR;
	surface.antiAlias		= CELL_GCM_SURFACE_CENTER_1;

	surface.width			= display_width;
	surface.height			= display_height;
	surface.x				= 0;
	surface.y				= 0;
}

void setRenderTarget(u32 index)
{
	surface.colorOffset[0]	= color_offset[index];
	rsxSetSurface(gGcmContext,&surface);
}

void initDefaultStateCommands()
{
    rsxSetCurrentBuffer(nullptr, (u32*)state_buffer, HOST_STATE_CB_SIZE);
    {
        rsxSetBlendEnable(gGcmContext, CELL_GCM_FALSE);
        rsxSetBlendFunc(gGcmContext, CELL_GCM_ONE, CELL_GCM_ZERO, CELL_GCM_ONE, CELL_GCM_ZERO);
        rsxSetBlendEquation(gGcmContext, CELL_GCM_FUNC_ADD, CELL_GCM_FUNC_ADD);
        rsxSetDepthWriteEnable(gGcmContext, CELL_GCM_TRUE);
        rsxSetDepthFunc(gGcmContext, CELL_GCM_LESS);
        rsxSetDepthTestEnable(gGcmContext, CELL_GCM_TRUE);
        rsxSetClearDepthStencil(gGcmContext,0xffffff00);
  	    rsxSetShadeModel(gGcmContext,CELL_GCM_SHADE_MODEL_SMOOTH);
        rsxSetFrontFace(gGcmContext, CELL_GCM_FRONTFACE_CCW);
        rsxSetClearReport(gGcmContext, CELL_GCM_ZPASS_PIXEL_CNT);
        rsxSetZMinMaxControl(gGcmContext, CELL_GCM_TRUE, CELL_GCM_FALSE, CELL_GCM_FALSE);
        rsxSetZCullControl(gGcmContext, CELL_GCM_ZCULL_LESS, CELL_GCM_ZCULL_LONES);
        rsxSetSCullControl(gGcmContext, CELL_GCM_SCULL_SFUNC_LESS, 1, 0xff);
        rsxSetColorMaskMrt(gGcmContext, 0);
    	rsxSetColorMask(gGcmContext,CELL_GCM_COLOR_MASK_B |
							CELL_GCM_COLOR_MASK_G |
							CELL_GCM_COLOR_MASK_R |
							CELL_GCM_COLOR_MASK_A);
        rsxSetReturnCommand(gGcmContext);
    }
    rsxSetDefaultCommandBuffer(nullptr);
}

void initScreen(u32 hostBufferSize)
{
    u32 zs_depth = 4;
    u32 color_depth = 4;
    u32 bufferSize = rsxAlign(HOST_ADDR_ALIGNMENT, (DEFAULT_CB_SIZE + HOST_STATE_CB_SIZE + hostBufferSize));

    cellGcmInitDefaultFifoMode(CELL_GCM_DEFAULT_FIFO_MODE_CONDITIONAL);

    void *hostAddr = memalign(HOST_ADDR_ALIGNMENT, bufferSize);
    rsxInit(nullptr, DEFAULT_CB_SIZE, bufferSize, hostAddr);

    state_buffer = (void*)((intptr_t)hostAddr + DEFAULT_CB_SIZE);
    rsxAddressToOffset(state_buffer, &state_offset);
    printf("state_cmd: %p [%08x]\n", state_buffer, state_offset);

    initDefaultStateCommands();
    initVideoConfiguration();

    fontRenderer = new RSXDebugFontRenderer(gGcmContext);

    waitRSXIdle();

    cellGcmSetFlipMode(CELL_GCM_FLIP_HSYNC);

	color_pitch = cellGcmGetTiledPitchSize(display_width*color_depth);
    depth_pitch = cellGcmGetTiledPitchSize(display_width*zs_depth);

    u32 tileIndex = 0;
    u32 bufferHeight = rsxAlign(CELL_GCM_TILE_LOCAL_ALIGN_HEIGHT, display_height);
    u32 colorBufferSize = bufferHeight*color_pitch;
    u32 depthBufferSize = bufferHeight*depth_pitch;
    for (u32 i=0; i < FRAME_BUFFER_COUNT;i++, tileIndex++) {
       bufferSize = rsxAlign(CELL_GCM_TILE_ALIGN_OFFSET, colorBufferSize);
       color_buffer[i] = rsxMemalign(CELL_GCM_TILE_ALIGN_SIZE, bufferSize);
       rsxAddressToOffset(color_buffer[i], &color_offset[i]);
       cellGcmSetDisplayBuffer(i, color_offset[i], color_pitch, display_width, display_height);
       cellGcmSetTileInfo(tileIndex, CELL_GCM_LOCATION_RSX, color_offset[i], bufferSize, color_pitch, CELL_GCM_COMPMODE_DISABLED, 0, 0);
       cellGcmBindTile(tileIndex);
       printf("fb[%d]: %p (%08x) [%dx%d] %d\n", i, color_buffer[i], color_offset[i], display_width, display_height, color_pitch);
    }

    bufferSize = rsxAlign(CELL_GCM_TILE_ALIGN_OFFSET, depthBufferSize);
    depth_buffer = rsxMemalign(CELL_GCM_TILE_ALIGN_SIZE, bufferSize);
    rsxAddressToOffset(depth_buffer, &depth_offset);
    cellGcmSetTileInfo(tileIndex, CELL_GCM_LOCATION_RSX, depth_offset, bufferSize, depth_pitch, CELL_GCM_COMPMODE_Z32_SEPSTENCIL, 0, 2);
    cellGcmBindTile(tileIndex);
    
    cellGcmSetZcull(0, depth_offset, rsxAlign(64, display_width), rsxAlign(64, display_height), 0, CELL_GCM_ZCULL_Z24S8, CELL_GCM_SURFACE_CENTER_1, CELL_GCM_ZCULL_LESS, CELL_GCM_ZCULL_LONES, CELL_GCM_SCULL_SFUNC_LESS, 1, 0xff);

    for (u32 i=0;i < FRAME_BUFFER_COUNT;i++) {
        *((vu32*) cellGcmGetLabelAddress(CELL_GCM_BUFFER_STATUS_INDEX + i)) = BUFFER_IDLE;
    }
    *((vu32*) cellGcmGetLabelAddress(CELL_GCM_PREPARED_BUFFER_INDEX)) = (fbOnDisplay << 8);
    *((vu32*) cellGcmGetLabelAddress(CELL_GCM_BUFFER_STATUS_INDEX + fbOnDisplay)) = BUFFER_BUSY;

    curr_fb = (fbOnDisplay + 1)%FRAME_BUFFER_COUNT;

    initFlipEvent();
    initRenderTarget();

    rsxSetWriteCommandLabel(gGcmContext, CELL_GCM_BUFFER_STATUS_INDEX + curr_fb, BUFFER_BUSY);
}

void flip()
{
    s32 qid = cellGcmSetPrepareFlip(gGcmContext, curr_fb);
    while (qid < 0) {
        usleep(100);
        qid = cellGcmSetPrepareFlip(gGcmContext, curr_fb);
    }

    rsxSetWriteBackendLabel(gGcmContext, CELL_GCM_PREPARED_BUFFER_INDEX, ((curr_fb << 8) | qid));
    rsxFlushBuffer(gGcmContext);

    syncPPUGPU();

    curr_fb = (curr_fb + 1)%FRAME_BUFFER_COUNT;

    rsxSetWaitLabel(gGcmContext, CELL_GCM_BUFFER_STATUS_INDEX + curr_fb, BUFFER_IDLE);
    rsxSetWriteCommandLabel(gGcmContext, CELL_GCM_BUFFER_STATUS_INDEX + curr_fb, BUFFER_BUSY);

    setRenderTarget(curr_fb);
}

void finish()
{
	rsxFinish(gGcmContext,1);

    u32 data = *((vu32*) cellGcmGetLabelAddress(CELL_GCM_PREPARED_BUFFER_INDEX));
    u32 lastBuffer = (data >> 8);
    while (lastBuffer != fbOnDisplay)
        usleep(100);
}
