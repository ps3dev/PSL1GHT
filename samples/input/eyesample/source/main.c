#include <tiny3d.h>
#include <io/pad.h>
#include <io/camera.h>
#include <sys/memory.h>
#include <sysutil/sysutil.h>
#include <sysutil/video.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sysmodule/sysmodule.h>

#define CAMERA_WIDTH  640
#define CAMERA_HEIGHT 480

static u32 *camera_texture = NULL;
static u32 camera_texture_offset = 0;
static int camera_texture_ready = 0;

static int g_running = 1;
static u32 g_device_type = 0xffffffffu;
static int g_container_created = 0;
static int g_camera_initialized = 0;
static sys_mem_container_t g_container;
static cameraType g_camera_type;
static cameraInfoEx camInfo;

u32 *texture_pointer = NULL;

void LoadTexture()
{
    u32 * texture_mem = tiny3d_AllocTexture(4*1024*1024);
    if(!texture_mem) return;
    texture_pointer = texture_mem;
	
	camera_texture = texture_mem;
	
	camera_texture_offset = tiny3d_TextureOffset(camera_texture);
	camera_texture_ready = 1;

	texture_pointer += (CAMERA_WIDTH * CAMERA_HEIGHT);
}

static void on_system_event(u64 status, u64 param, void *userdata)
{
    (void)param;
    (void)userdata;
    if(status == SYSUTIL_EXIT_GAME) g_running = 0;
}

static void draw_rect(float x, float y, float w, float h, u32 color, float z)
{
    tiny3d_SetPolygon(TINY3D_QUADS);
    tiny3d_VertexPos(x, y, z); tiny3d_VertexColor(color);
    tiny3d_VertexPos(x + w, y, z);
    tiny3d_VertexPos(x + w, y + h, z);
    tiny3d_VertexPos(x, y + h, z);
    tiny3d_End();
}

void DrawFull(float x, float y, int wid, int hei, float layer, u32 color)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(0, 0);
    tiny3d_VertexPos(x+wid, y, layer);
    tiny3d_VertexTexture(0.99f, 0);
    tiny3d_VertexPos(x+wid, y+hei, layer);
    tiny3d_VertexTexture(0.99f, 0.99f);
    tiny3d_VertexPos(x, y+hei, layer);
    tiny3d_VertexTexture(0, 0.99f);

    tiny3d_End();
}

static void draw_status(void)
{
    tiny3d_Project2D();
	
	if(cameraIsAttached(0) && cameraIsAvailable(0))
	{
		tiny3d_SetTexture(0, camera_texture_offset, CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_WIDTH * 4, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_NEAREST);
		DrawFull(0, 0, 916, 582, 0, 0xffffffff);
	}
	else draw_rect(0, 0, 916, 582, 0x006700ff, 1.0f);
}

static void initCamera(void)
{
    s32 ret;
	
    sysMemContainerCreate(&g_container, 2 * 1024 * 1024);
    g_container_created = 1;
    cameraInit();
    g_camera_initialized = 1;
	g_device_type = CAM_TYPE_PLAYSTATION_EYE;
	ret = cameraGetType(0, &g_camera_type);
	if(ret == 0) g_device_type = (u32)g_camera_type;
	
	memset(&camInfo, 0, sizeof(camInfo));

	camInfo.format = CAM_FORM_YUV422;
	camInfo.framerate = 30;
	camInfo.resolution = CAM_RESO_VGA;
	camInfo.info_ver = 0x101;
	camInfo.container = g_container;

	cameraOpenEx(0, &camInfo);

	cameraReset(0);
	cameraStart(0);
}

static void cleanup(void)
{
    if(g_camera_initialized)
	{
		cameraStop(0);
		cameraClose(0);
		cameraEnd();
	}
    if(g_container_created) sysMemContainerDestroy(g_container);
}

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

    return (r << 16) | (g << 8) | b;
}

static void Convert422(u8 *yuv, u32 *rgb1, u32 *rgb2)
{
    int y1 = yuv[0];
    int v  = yuv[1];
    int y2 = yuv[2];
    int u  = yuv[3];

    *rgb1 = YUV_to_RGB(y1, u, v);
    *rgb2 = YUV_to_RGB(y2, u, v);
}

void UpdateCameraTexture(const u8 *yuv)
{
    int x, y;
    u32 *dst;

    if (!camera_texture_ready || !yuv) return;

    dst = camera_texture;

    for (y = 0; y < CAMERA_HEIGHT; y++)
    {
        for (x = 0; x < CAMERA_WIDTH; x += 2)
        {
            u32 pixel1, pixel2;

            Convert422((u8 *)yuv, &pixel1, &pixel2);

            dst[0] = 0xff000000 | pixel1;
            dst[1] = 0xff000000 | pixel2;

            dst += 2;
            yuv += 4;
        }
    }
}

int main(int argc, const char *argv[])
{
    padInfo padinfo;
    padData paddata;
    int i;
    (void)argc;
    (void)argv;

    tiny3d_Init(6 * 1024 * 1024);
    tiny3d_UserViewport(1, 0, 0,
        (float)(Video_Resolution.width / 916.0f),
        (float)(Video_Resolution.height / 582.0f),
        (float)(Video_Resolution.width / 1920.0f),
        (float)(Video_Resolution.height / 1080.0f));
	
	LoadTexture();
	
	sysModuleLoad(SYSMODULE_CAMERA);
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, on_system_event, NULL);
	
	initCamera();
	
    ioPadInit(7);

    while(g_running)
	{
        sysUtilCheckCallback();
        tiny3d_Clear(0xff000000, TINY3D_CLEAR_ALL);
		tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);
		tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
            TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
            TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);

        if(ioPadGetInfo(&padinfo) == 0)
		{
            for (i = 0; i < MAX_PADS; ++i)
			{
                if(padinfo.status[i] && ioPadGetData(i, &paddata) == 0)
				{
                    if(paddata.BTN_START) g_running = 0;
                }
            }
        }

		if(cameraIsAttached(0) && cameraIsAvailable(0))
		{
			u32 frame = 0;
			u32 readcount = 0;

			s32 ret = cameraRead(0, &frame, &readcount);
			if(ret==0) UpdateCameraTexture((const u8 *)(u64)camInfo.buffer);
		}
		
        draw_status();
        tiny3d_Flip();
    }

    cleanup();
    ioPadEnd();
    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    return 0;
}