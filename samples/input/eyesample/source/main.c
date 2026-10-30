#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <malloc.h>
#include <assert.h>

#include <sysutil/video.h>
#include <rsx/rsx.h>
#include <rsx/gcm_sys.h>

#include <io/pad.h>
#include <io/camera.h>

#include <sysmodule/sysmodule.h>
#include <sysutil/sysutil.h>

#define CAMERA_WIDTH  640
#define CAMERA_HEIGHT 480

__attribute__((aligned(128)))

#define CAMERA_RGB_SIZE (640 * 480 * 4)
#define CAMERA_MAP_SIZE ((CAMERA_RGB_SIZE + 0xFFFFF) & ~0xFFFFF)

static u32 *converted;
static u32 camera_offset;

static int init_camera_texture(void)
{
    converted = memalign(1024 * 1024, CAMERA_MAP_SIZE);
    if (!converted) return -1;

    memset(converted, 0, CAMERA_MAP_SIZE);

    if (gcmMapMainMemory(converted, CAMERA_MAP_SIZE, &camera_offset) != 0)
    {
        free(converted);
        converted = NULL;
        return -1;
    }

    return 0;
}

static gcmContextData *context;

static int currentBuffer = 0;
static u32 *buffer[2];
static u32 buffer_offset[2];

static int g_running = 1;
static int g_container_created = 0;
static int g_camera_initialized = 0;
static int g_camera_opened = 0;
static int g_camera_started = 0;

static sys_mem_container_t g_container;
static cameraType g_camera_type;
static cameraInfoEx camInfo;
static videoResolution res;

/* Wait until the previous buffer flip has completed before reusing it. */
static void waitFlip(void)
{
    while (gcmGetFlipStatus() != 0) usleep(200);
    gcmResetFlipStatus();
}

static void flip(s32 buffer_id)
{
    assert(gcmSetFlip(context, buffer_id) == 0);
    rsxFlushBuffer(context);
    gcmSetWaitFlip(context);
}

/* Initialize the display and two RSX framebuffers. */
static int init_screen(void)
{
    void *host_addr;
    videoState state;
	videoConfiguration vconfig;
    u32 buffer_size;
    int i;

    host_addr = memalign(1024 * 1024, 1024 * 1024);
    if (!host_addr) return -1;

    if (rsxInit(&context, 0x10000, 1024 * 1024, host_addr) != 0) return -1;
    if (videoGetState(0, 0, &state) != 0 || state.state != 0) return -1;
    if (videoGetResolution(state.displayMode.resolution, &res) != 0) return -1;

    memset(&vconfig, 0, sizeof(vconfig));
    vconfig.resolution = state.displayMode.resolution;
    vconfig.format = VIDEO_BUFFER_FORMAT_XRGB;
    vconfig.pitch = res.width * 4;
    vconfig.aspect = state.displayMode.aspect;

    if (videoConfigure(0, &vconfig, NULL, 0) != 0) return -1;
    if (videoGetState(0, 0, &state) != 0) return -1;

    buffer_size = 4 * res.width * res.height;

    gcmSetFlipMode(GCM_FLIP_VSYNC);

    for (i = 0; i < 2; i++)
    {
        buffer[i] = rsxMemalign(64, buffer_size);

		if (!buffer[i]) return -1;
		if (rsxAddressToOffset(buffer[i], &buffer_offset[i]) != 0) return -1;
        if (gcmSetDisplayBuffer(i, buffer_offset[i], res.width * 4, res.width, res.height) != 0) return -1;
    }

    gcmResetFlipStatus();

    /* Start by displaying buffer 1; the first frame will be drawn into 0. */
    flip(1);
    currentBuffer = 0;

    return 0;
}

static void on_system_event(u64 status, u64 param, void *userdata)
{
    (void)param;
    (void)userdata;

    if (status == SYSUTIL_EXIT_GAME)
        g_running = 0;
}

/* Original sample's YUV conversion retained as-is. */
static u32 YUV_to_RGB(int y, int u, int v)
{
    int r, g, b;

    v -= 128;
    u -= 128;

    r = y + u;
    g = y - u / 2 - v / 8;
    b = y + v;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    return ((u32)r << 16) | ((u32)g << 8) | (u32)b;
}

static void Convert422(const u8 *yuv, u32 *rgb1, u32 *rgb2)
{
    int y1 = yuv[0];
    int v  = yuv[1];
    int y2 = yuv[2];
    int u  = yuv[3];

    *rgb1 = YUV_to_RGB(y1, u, v);
    *rgb2 = YUV_to_RGB(y2, u, v);
}

static void convertCameraFrame(const u8 *yuv)
{
    int x, y;

    for (y = 0; y < CAMERA_HEIGHT; y++)
    {
        const u8 *src = yuv + y * CAMERA_WIDTH * 2;
        u32 *dst = converted + y * CAMERA_WIDTH;

        for (x = 0; x < CAMERA_WIDTH; x += 2)
        {
            Convert422(src, &dst[x], &dst[x + 1]);
            src += 4;
        }
    }
}

static void drawCameraFrame(u32 *dst)
{
    int x, y;
    int src_x, src_y;

    for (y = 0; y < res.height; y++)
    {
        src_y = (y * CAMERA_HEIGHT) / res.height;
        for (x = 0; x < res.width; x++)
        {
            src_x = (x * CAMERA_WIDTH) / res.width;
            dst[y * res.width + x] = converted[src_y * CAMERA_WIDTH + src_x];
        }
    }
}

static int initCamera(void)
{
    s32 ret;

    ret = sysMemContainerCreate(&g_container, 2 * 1024 * 1024);
    if (ret != 0) return ret;
    g_container_created = 1;

    /* Required on this setup: load the camera module before cameraInit(). */
    ret = sysModuleLoad(SYSMODULE_CAMERA);
    if (ret != 0) return ret;

    ret = cameraInit();
    if (ret != 0) return ret;
    g_camera_initialized = 1;

    g_camera_type = CAM_TYPE_PLAYSTATION_EYE;
    cameraGetType(0, &g_camera_type);

    memset(&camInfo, 0, sizeof(camInfo));
    camInfo.format = CAM_FORM_YUV422;
    camInfo.framerate = 30;
    camInfo.resolution = CAM_RESO_VGA;
    camInfo.info_ver = 0x101;
    camInfo.container = g_container;

    ret = cameraOpenEx(0, &camInfo);
    if (ret != 0) return ret;
    g_camera_opened = 1;

    ret = cameraReset(0);
    if (ret != 0) return ret;

    ret = cameraStart(0);
    if (ret != 0) return ret;

    g_camera_started = 1;
    return 0;
}

static void cleanup(void)
{
    if (g_camera_started) cameraStop(0);
    if (g_camera_opened) cameraClose(0);
    if (g_camera_initialized) cameraEnd();
    if (g_container_created) sysMemContainerDestroy(g_container);
}

static void scale_camera_frame(u32 framebuffer_offset, u32 framebuffer_pitch, u32 screen_w, u32 screen_h)
{
    gcmTransferScale scale;
    gcmTransferSurface surface;

    memset(&scale, 0, sizeof(scale));
    memset(&surface, 0, sizeof(surface));

    scale.conversion = GCM_TRANSFER_CONVERSION_TRUNCATE;
    scale.format = GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
    scale.operation = GCM_TRANSFER_OPERATION_SRCCOPY;

    scale.clipX = 0;
    scale.clipY = 0;
    scale.clipW = screen_w;
    scale.clipH = screen_h;

    scale.outX = 0;
    scale.outY = 0;
    scale.outW = screen_w;
    scale.outH = screen_h;

    scale.ratioX = rsxGetFixedSint32(640.0f / screen_w);
    scale.ratioY = rsxGetFixedSint32(480.0f / screen_h);

    scale.inW = 640;
    scale.inH = 480;
    scale.pitch = 640 * 4;

    scale.origin = GCM_TRANSFER_ORIGIN_CORNER;
    scale.interp = GCM_TRANSFER_INTERPOLATOR_LINEAR;

    scale.offset = camera_offset;
    scale.inX = 0;
    scale.inY = 0;

    surface.format = GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
    surface.pitch = framebuffer_pitch;
    surface.offset = framebuffer_offset;

    rsxSetTransferScaleMode(gGcmContext, GCM_TRANSFER_MAIN_TO_LOCAL, GCM_TRANSFER_SURFACE);

    rsxSetTransferScaleSurface(gGcmContext, &scale, &surface);
}

int main(int argc, const char *argv[])
{
    padInfo padinfo;
    padData paddata;
    int i;
    s32 camera_error;

    (void)argc;
    (void)argv;

    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, on_system_event, NULL);

    if (init_screen() != 0)
    {
        printf("RSX/display initialization failed\n");
        return 1;
    }
	
	if (init_camera_texture() != 0)
	{
		printf("Camera texture mapping failed\n");
		return 1;
	}
	
    ioPadInit(7);

    camera_error = initCamera();
    if (camera_error != 0) printf("Camera initialization failed: 0x%08x\n", (unsigned int)camera_error);

    while (g_running)
    {
        u32 frame = 0;
        u32 readcount = 0;
        s32 ret;

        sysUtilCheckCallback();

        if (ioPadGetInfo(&padinfo) == 0)
        {
            for (i = 0; i < MAX_PADS; i++)
            {
                if (padinfo.status[i] && ioPadGetData(i, &paddata) == 0)
                {
                    if (paddata.BTN_START) g_running = 0;
                }
            }
        }

        /* Do not write to a framebuffer until its previous flip is done. */
        waitFlip();

        if (g_camera_started && cameraIsAttached(0) && cameraIsAvailable(0))
        {
            ret = cameraRead(0, &frame, &readcount);

            if (ret == 0 && readcount != 0)
			{
				convertCameraFrame((const u8 *)(u64)camInfo.buffer);
				scale_camera_frame(buffer_offset[currentBuffer], res.width * 4, res.width, res.height);
			}
            else if (ret == CAMERA_ERRO_NEED_START)
            {
                cameraReset(0);
                cameraStart(0);
                g_camera_started = 1;
            }
        }

        flip(currentBuffer);
        currentBuffer = !currentBuffer;
    }

    cleanup();
    ioPadEnd();
    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    return 0;
}
