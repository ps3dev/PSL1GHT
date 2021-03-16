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

videoResolution vResolution;

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

gcmSurface surface;

static u32 sLabelVal = 1;
static RSXDebugFontRenderer *fontRenderer;

static u32 sResolutionIds[] = {
    VIDEO_RESOLUTION_1600x1080,
    VIDEO_RESOLUTION_1440x1080,
    VIDEO_RESOLUTION_1280x1080,
    VIDEO_RESOLUTION_960x1080,
    VIDEO_RESOLUTION_720,
    VIDEO_RESOLUTION_480,
    VIDEO_RESOLUTION_576
};
static size_t RESOLUTION_ID_COUNT = sizeof(sResolutionIds)/sizeof(u32);

extern "C" {
static void flipHandler(const u32 head)
{
    (void)head;
    u32 v = fbFlipped;

    for (u32 i = fbOnDisplay; i != v; i=(i + 1)%FRAME_BUFFER_COUNT) {
        *((vu32*) gcmGetLabelAddress(GCM_BUFFER_STATUS_INDEX + i)) = BUFFER_IDLE;
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

    data = *((vu32*) gcmGetLabelAddress(GCM_PREPARED_BUFFER_INDEX));
    bufferToFlip = (data >> 8);
    indexToFlip = (data & 0x07);

    if (!fbOnFlip) {
        if (bufferToFlip != fbOnDisplay) {
            s32 ret = gcmSetFlipImmediate(indexToFlip);
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
    vu32 *label = (vu32*) gcmGetLabelAddress(GCM_PREPARED_BUFFER_INDEX);
    while(((curr_fb + FRAME_BUFFER_COUNT - ((*label)>>8))%FRAME_BUFFER_COUNT) > MAX_BUFFER_QUEUE_SIZE) {
        sys_event_t event;

        sysEventQueueReceive(flipEventQueue, &event, 0);
        sysEventQueueDrain(flipEventQueue);
    }
}

static void waitRSXFinish()
{
	rsxSetWriteBackendLabel(gGcmContext,GCM_WAIT_LABEL_INDEX,sLabelVal);

	rsxFlushBuffer(gGcmContext);

	while(*(vu32*)gcmGetLabelAddress(GCM_WAIT_LABEL_INDEX)!=sLabelVal)
		usleep(30);

	++sLabelVal;
}

static void waitRSXIdle()
{
	rsxSetWriteBackendLabel(gGcmContext,GCM_WAIT_LABEL_INDEX,sLabelVal);
	rsxSetWaitLabel(gGcmContext,GCM_WAIT_LABEL_INDEX,sLabelVal);

	++sLabelVal;

	waitRSXFinish();
}

void initVideoConfiguration()
{
    s32 rval = 0;
    s32 resId = 0;

    for (size_t i=0;i < RESOLUTION_ID_COUNT;i++) {
        rval = videoGetResolutionAvailability(VIDEO_PRIMARY, sResolutionIds[i], VIDEO_ASPECT_AUTO, 0);
        if (rval != 1) continue;

        resId = sResolutionIds[i];
        rval = videoGetResolution(resId, &vResolution);
        if(!rval) break;
    }

    if(rval) {
        printf("Error: videoGetResolutionAvailability failed. No usable resolution.\n");
        exit(1);
    }

    videoConfiguration config = {
        (u8)resId,
        VIDEO_BUFFER_FORMAT_XRGB,
        VIDEO_ASPECT_AUTO,
        {0,0,0,0,0,0,0,0,0},
        gcmGetTiledPitchSize(vResolution.width*4)
    };

    rval = videoConfigure(VIDEO_PRIMARY, &config, NULL, 0);
    if(rval) {
        printf("Error: videoConfigure failed.\n");
        exit(1);
    }

    videoState state;

    rval = videoGetState(VIDEO_PRIMARY, 0, &state);
    switch(state.displayMode.aspect) {
        case VIDEO_ASPECT_4_3:
            aspect_ratio = 4.0f/3.0f;
            break;
        case VIDEO_ASPECT_16_9:
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

    gcmSetFlipHandler(flipHandler);
    gcmSetVBlankHandler(vblankHandler);
}

void initRenderTarget()
{
    memset(&surface, 0, sizeof(gcmSurface));

	surface.colorFormat		= GCM_SURFACE_X8R8G8B8;
	surface.colorTarget		= GCM_SURFACE_TARGET_0;
	surface.colorLocation[0]	= GCM_LOCATION_RSX;
	surface.colorOffset[0]	= color_offset[curr_fb];
	surface.colorPitch[0]	= color_pitch;

    for(u32 i=1; i< GCM_MAX_MRT_COUNT;i++) {
        surface.colorLocation[i]	= GCM_LOCATION_RSX;
        surface.colorOffset[i]		= color_offset[curr_fb];
        surface.colorPitch[i]		= 64;
    }

	surface.depthFormat		= GCM_SURFACE_ZETA_Z24S8;
	surface.depthLocation	= GCM_LOCATION_RSX;
	surface.depthOffset		= depth_offset;
	surface.depthPitch		= depth_pitch;

	surface.type			= GCM_SURFACE_TYPE_LINEAR;
	surface.antiAlias		= GCM_SURFACE_CENTER_1;

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
        rsxSetBlendEnable(gGcmContext, GCM_FALSE);
        rsxSetBlendFunc(gGcmContext, GCM_ONE, GCM_ZERO, GCM_ONE, GCM_ZERO);
        rsxSetBlendEquation(gGcmContext, GCM_FUNC_ADD, GCM_FUNC_ADD);
        rsxSetDepthWriteEnable(gGcmContext, GCM_TRUE);
        rsxSetDepthFunc(gGcmContext, GCM_LESS);
        rsxSetDepthTestEnable(gGcmContext, GCM_TRUE);
        rsxSetClearDepthStencil(gGcmContext,0xffffff00);
  	    rsxSetShadeModel(gGcmContext,GCM_SHADE_MODEL_SMOOTH);
        rsxSetFrontFace(gGcmContext, GCM_FRONTFACE_CCW);
        rsxSetClearReport(gGcmContext, GCM_ZPASS_PIXEL_CNT);
        rsxSetZControl(gGcmContext, GCM_TRUE, GCM_FALSE, GCM_FALSE);
        rsxSetZCullControl(gGcmContext, GCM_ZCULL_LESS, GCM_ZCULL_LONES);
        rsxSetSCullControl(gGcmContext, GCM_SCULL_SFUNC_LESS, 1, 0xff);
        rsxSetColorMaskMrt(gGcmContext, 0);
    	rsxSetColorMask(gGcmContext,GCM_COLOR_MASK_B |
							GCM_COLOR_MASK_G |
							GCM_COLOR_MASK_R |
							GCM_COLOR_MASK_A);
        rsxSetReturnCommand(gGcmContext);
    }
    rsxSetDefaultCommandBuffer(nullptr);
}

void initScreen(u32 hostBufferSize)
{
    u32 zs_depth = 4;
    u32 color_depth = 4;
    u32 bufferSize = rsxAlign(HOST_ADDR_ALIGNMENT, (DEFAULT_CB_SIZE + HOST_STATE_CB_SIZE + hostBufferSize));

    gcmInitDefaultFifoMode(GCM_DEFAULT_FIFO_MODE_CONDITIONAL);

    void *hostAddr = memalign(HOST_ADDR_ALIGNMENT, bufferSize);
    rsxInit(nullptr, DEFAULT_CB_SIZE, bufferSize, hostAddr);

    state_buffer = (void*)((intptr_t)hostAddr + DEFAULT_CB_SIZE);
    rsxAddressToOffset(state_buffer, &state_offset);
    printf("state_cmd: %p [%08x]\n", state_buffer, state_offset);

    initDefaultStateCommands();
    initVideoConfiguration();

    fontRenderer = new RSXDebugFontRenderer(gGcmContext);

    waitRSXIdle();

    gcmSetFlipMode(GCM_FLIP_HSYNC);

	color_pitch = gcmGetTiledPitchSize(display_width*color_depth);
    depth_pitch = gcmGetTiledPitchSize(display_width*zs_depth);

    u32 tileIndex = 0;
    u32 bufferHeight = rsxAlign(GCM_TILE_LOCAL_ALIGN_HEIGHT, display_height);
    u32 colorBufferSize = bufferHeight*color_pitch;
    u32 depthBufferSize = bufferHeight*depth_pitch;
    for (u32 i=0; i < FRAME_BUFFER_COUNT;i++, tileIndex++) {
       bufferSize = rsxAlign(GCM_TILE_ALIGN_OFFSET, colorBufferSize);
       color_buffer[i] = rsxMemalign(GCM_TILE_ALIGN_SIZE, bufferSize);
       rsxAddressToOffset(color_buffer[i], &color_offset[i]);
       gcmSetDisplayBuffer(i, color_offset[i], color_pitch, display_width, display_height);
       gcmSetTileInfo(tileIndex, GCM_LOCATION_RSX, color_offset[i], bufferSize, color_pitch, GCM_COMPMODE_DISABLED, 0, 0);
       gcmBindTile(tileIndex);
       printf("fb[%d]: %p (%08x) [%dx%d] %d\n", i, color_buffer[i], color_offset[i], display_width, display_height, color_pitch);
    }

    bufferSize = rsxAlign(GCM_TILE_ALIGN_OFFSET, depthBufferSize);
    depth_buffer = rsxMemalign(GCM_TILE_ALIGN_SIZE, bufferSize);
    rsxAddressToOffset(depth_buffer, &depth_offset);
    gcmSetTileInfo(tileIndex, GCM_LOCATION_RSX, depth_offset, bufferSize, depth_pitch, GCM_COMPMODE_Z32_SEPSTENCIL, 0, 2);
    gcmBindTile(tileIndex);
    
    gcmSetZcull(0, depth_offset, rsxAlign(64, display_width), rsxAlign(64, display_height), 0, GCM_ZCULL_Z24S8, GCM_SURFACE_CENTER_1, GCM_ZCULL_LESS, GCM_ZCULL_LONES, GCM_SCULL_SFUNC_LESS, 1, 0xff);

    for (u32 i=0;i < FRAME_BUFFER_COUNT;i++) {
        *((vu32*) gcmGetLabelAddress(GCM_BUFFER_STATUS_INDEX + i)) = BUFFER_IDLE;
    }
    *((vu32*) gcmGetLabelAddress(GCM_PREPARED_BUFFER_INDEX)) = (fbOnDisplay << 8);
    *((vu32*) gcmGetLabelAddress(GCM_BUFFER_STATUS_INDEX + fbOnDisplay)) = BUFFER_BUSY;

    curr_fb = (fbOnDisplay + 1)%FRAME_BUFFER_COUNT;

    initFlipEvent();
    initRenderTarget();

    rsxSetWriteCommandLabel(gGcmContext, GCM_BUFFER_STATUS_INDEX + curr_fb, BUFFER_BUSY);
}

void flip()
{
    s32 qid = gcmSetPrepareFlip(gGcmContext, curr_fb);
    while (qid < 0) {
        usleep(100);
        qid = gcmSetPrepareFlip(gGcmContext, curr_fb);
    }

    rsxSetWriteBackendLabel(gGcmContext, GCM_PREPARED_BUFFER_INDEX, ((curr_fb << 8) | qid));
    rsxFlushBuffer(gGcmContext);

    syncPPUGPU();

    curr_fb = (curr_fb + 1)%FRAME_BUFFER_COUNT;

    rsxSetWaitLabel(gGcmContext, GCM_BUFFER_STATUS_INDEX + curr_fb, BUFFER_IDLE);
    rsxSetWriteCommandLabel(gGcmContext, GCM_BUFFER_STATUS_INDEX + curr_fb, BUFFER_BUSY);

    setRenderTarget(curr_fb);
}

void finish()
{
	rsxFinish(gGcmContext,1);

    u32 data = *((vu32*) gcmGetLabelAddress(GCM_PREPARED_BUFFER_INDEX));
    u32 lastBuffer = (data >> 8);
    while (lastBuffer != fbOnDisplay)
        usleep(100);
}
