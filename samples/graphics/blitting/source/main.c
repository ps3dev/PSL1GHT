/*
 * Usage of the blit functions.
 */

#include <io/pad.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>

#include <sysutil/sysutil.h>
#include <sysutil/video.h>
#include <rsx/rsx.h>
#include <rsx/gcm_sys.h>

#include "bitmap.h"
#include "psl1ght.xpm"

typedef struct {
  gcmContextData *context;
  u32 curr_fb;
  u32 framecnt;
  u32 pitch;
  u32 *buffer[2];
  u32 offset[2];
  u32 depth_pitch;
  u16 *depth_buffer;
  u32 depth_offset;
  videoResolution res;
} displayData;

void setRenderTarget(displayData *vdat)
{
  gcmSurface sf;

  sf.colorFormat = GCM_TF_COLOR_X8R8G8B8;
  sf.colorTarget = GCM_TF_TARGET_0;
  sf.colorLocation[0] = GCM_LOCATION_RSX;
  sf.colorOffset[0] = vdat->offset[vdat->curr_fb];
  sf.colorPitch[0] = vdat->pitch;

  sf.colorLocation[1] = GCM_LOCATION_RSX;
  sf.colorLocation[2] = GCM_LOCATION_RSX;
  sf.colorLocation[3] = GCM_LOCATION_RSX;
  sf.colorOffset[1] = 0;
  sf.colorOffset[2] = 0;
  sf.colorOffset[3] = 0;
  sf.colorPitch[1] = 64;
  sf.colorPitch[2] = 64;
  sf.colorPitch[3] = 64;

  sf.depthFormat = GCM_TF_ZETA_Z16;
  sf.depthLocation = GCM_LOCATION_RSX;
  sf.depthOffset = vdat->depth_offset;
  sf.depthPitch = vdat->depth_pitch;

  sf.type = GCM_TF_TYPE_LINEAR;
  sf.antiAlias = GCM_TF_CENTER_1;

  sf.width = vdat->res.width;
  sf.height = vdat->res.height;
  sf.x = 0;
  sf.y = 0;

  rsxSetSurface(vdat->context, &sf);
}

static void eventHandle(u64 status, u64 param, void * userdata) {
  (void)param;
  (void)userdata;
  if(status == SYSUTIL_EXIT_GAME){
    printf("Quit game requested\n");
    exit(0);
  }else if(status == SYSUTIL_MENU_OPEN){
    /* xmb opened, should prob pause game or something :P */
    printf("XMB opened\n");
  }else if(status == SYSUTIL_MENU_CLOSE){
    /* xmb closed, and then resume */
    printf("XMB closed\n");
  }else if(status == SYSUTIL_DRAW_BEGIN){
  }else if(status == SYSUTIL_DRAW_END){
  }else{
    printf("Unhandled event: %08llX\n", (unsigned long long int)status);
  }
}

void appCleanup(){
  sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
  printf("Exiting for real.\n");
}

/* Block the PPU thread untill the previous flip operation has finished. */
void waitFlip() {
  while(gcmGetFlipStatus() != 0)
    usleep(200);
  gcmResetFlipStatus();
}

/* Enqueue a flip command in RSX command buffer.
   Setup next screen to be drawn to. */
void flip(displayData *vdat) {
  s32 status = gcmSetFlip(vdat->context, vdat->curr_fb);
  assert(status == 0);
  rsxFlushBuffer(vdat->context);
  gcmSetWaitFlip(vdat->context);
  vdat->curr_fb = !vdat->curr_fb;
  ++vdat->framecnt;
  setRenderTarget(vdat);
}

/* Initilize everything. */
void init_screen(displayData *vdat) {
  int i;

  /* Allocate a 1Mb buffer, alligned to a 1Mb boundary to be our shared IO memory with the RSX. */
  void *host_addr = memalign(1024*1024, 1024*1024);
  assert(host_addr != NULL);

  /* Initilise libRSX, which sets up the command buffer and shared IO memory */
  vdat->context = rsxInit(0x10000, 1024*1024, host_addr);
  assert(vdat->context != NULL);

  videoState state;
  s32 status = videoGetState(0, 0, &state); /* Get the state of the display */
  assert(status == 0);
  assert(state.state == 0); /* Make sure display is enabled */

  /* Get the current resolution */
  status = videoGetResolution(state.displayMode.resolution, &vdat->res);
  assert(status == 0);

  /* Configure the buffer format to xRGB */
  videoConfiguration vconfig;
  memset(&vconfig, 0, sizeof(videoConfiguration));
  vconfig.resolution = state.displayMode.resolution;
  vconfig.format = VIDEO_BUFFER_FORMAT_XRGB;
  vconfig.pitch = vdat->res.width * 4;
  vconfig.aspect=state.displayMode.aspect;

  status = videoConfigure(0, &vconfig, NULL, 0);
  assert(status == 0);
  status = videoGetState(0, 0, &state);
  assert(status == 0);

  gcmSetFlipMode(GCM_FLIP_VSYNC); /* Wait for VSYNC to flip */

  /* Allocate and setup two buffers for the RSX to draw to the screen (double buffering) */
  vdat->pitch = vdat->res.width*sizeof(u32);
  for (i=0; i<2; ++i) {
    vdat->buffer[i] = (u32*)rsxMemalign(64, vdat->pitch * vdat->res.height);
    assert(vdat->buffer[i] != NULL);
    status = rsxAddressToOffset(vdat->buffer[i], &vdat->offset[i]);
    assert(status==0);
    status = gcmSetDisplayBuffer(i, vdat->offset[i], vdat->pitch, vdat->res.width, vdat->res.height);
    assert(status==0);
  }

  /* Allocate the depth buffer */
  vdat->depth_pitch = vdat->res.width * sizeof(u16);
  vdat->depth_buffer = (u16*)rsxMemalign(64, vdat->depth_pitch * vdat->res.height);
  assert(vdat->depth_buffer != NULL);
  status = rsxAddressToOffset(vdat->depth_buffer, &vdat->depth_offset);
  assert(status==0);

  gcmResetFlipStatus();
  vdat->curr_fb = 0;
  vdat->framecnt = 0;
  flip(vdat);
}

void blit_simple(displayData *vdat, Bitmap *bitmap,
  u32 dstX, u32 dstY, u32 srcX, u32 srcY, u32 w, u32 h)
{
  rsxSetTransferImage(vdat->context, GCM_TRANSFER_LOCAL_TO_LOCAL,
    vdat->offset[vdat->curr_fb], vdat->pitch, dstX-w/2, dstY-h/2,
    bitmap->offset, bitmap->width*4, rsxGetFixedUint16((float)srcX),
    rsxGetFixedUint16((float)srcY), w, h, 4);
}

void blit_data(displayData *vdat, Bitmap *bitmap,
  u32 dstX, u32 dstY, u32 srcX, u32 srcY, u32 w, u32 h)
{
  rsxSetTransferData(vdat->context, GCM_TRANSFER_LOCAL_TO_LOCAL,
    vdat->offset[vdat->curr_fb], vdat->pitch, bitmap->offset, bitmap->width*4,
    w*4, h);
}

void blit_scale(displayData *vdat, Bitmap *bitmap,
  u32 dstX, u32 dstY, u32 srcX, u32 srcY, u32 w, u32 h, float zoom)
{
  gcmTransferScale scale;
  gcmTransferSurface surface;

  scale.conversion = GCM_TRANSFER_CONVERSION_TRUNCATE;
  scale.format = GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
  scale.origin = GCM_TRANSFER_ORIGIN_CORNER;
  scale.operation = GCM_TRANSFER_OPERATION_SRCCOPY_AND;
  scale.interp = GCM_TRANSFER_INTERPOLATOR_NEAREST;
  scale.clipX = 0;
  scale.clipY = 0;
  scale.clipW = vdat->res.width;
  scale.clipH = vdat->res.height;
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

  surface.format = GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
  surface.pitch = vdat->pitch;
  surface.offset = vdat->offset[vdat->curr_fb];

  rsxSetTransferScaleMode(vdat->context, GCM_TRANSFER_LOCAL_TO_LOCAL,
    GCM_TRANSFER_SURFACE);

  rsxSetTransferScaleSurface(vdat->context, &scale, &surface);
}

int main(int argc, const char* argv[])
{
  padInfo padinfo;
  padData paddata;
  Bitmap bitmap;

  displayData vdat;
  int quit = 0;
  int i;
  int k = 0;

  atexit(appCleanup);
  sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, eventHandle, NULL);

  init_screen(&vdat);
  printf("screen res: %dx%d buffers: %p %p\n",
       vdat.res.width, vdat.res.height, vdat.buffer[0], vdat.buffer[1]);
  ioPadInit(7);

  bitmapSetXpm(&bitmap, psl1ght_xpm);

  while (!quit) {
    /* Check the pads. */
    ioPadGetInfo(&padinfo);
    for (i=0; i<MAX_PADS; i++) {
      if (padinfo.status[i]) {
        ioPadGetData(i, &paddata);
        if (paddata.BTN_CROSS)
          quit = 1;
      }
    }

    /* Wait for the last flip to finish, so we can draw to the old buffer */
    waitFlip();

    /* Display some stuff on the screen */
    rsxSetClearColor(vdat.context, 0x200030);
    rsxSetClearDepthValue(vdat.context, 0xffff);
    rsxClearSurface(vdat.context,GCM_CLEAR_R |
                                 GCM_CLEAR_G |
                                 GCM_CLEAR_B |
                                 GCM_CLEAR_A |
                                 GCM_CLEAR_S |
                                 GCM_CLEAR_Z);

    /* Enable blending (for rsxSetTransferScaleSurface) */
    rsxSetBlendFunc(vdat.context, GCM_SRC_ALPHA, GCM_ONE_MINUS_SRC_ALPHA, GCM_SRC_ALPHA, GCM_ONE_MINUS_SRC_ALPHA);
    rsxSetBlendEquation(vdat.context, GCM_FUNC_ADD, GCM_FUNC_ADD);
    rsxSetBlendEnable(vdat.context, GCM_TRUE);

    /* Display the whole PSL1GHT image */
    blit_simple(&vdat, &bitmap, vdat.res.width/4, 100, 0, 0, bitmap.width, bitmap.height);

    /* Distort the PSL1GHT image by displaying lines with different X coords */
    for (i=0; i<bitmap.height; ++i) {
      int x = vdat.res.width*3/4 + 50.f*sinf(((i+k) & 0x7f) * (2.f*M_PI/0x80));
      blit_simple(&vdat, &bitmap, x, 100+i, 0, i, bitmap.width, 1);
    }
    k = (k+1) & 0x7f;

    /* Animate all letters */
    for (i=6; i>=0; --i) {
      int x = vdat.res.width * (0.1f + 0.8f * (0.5f+0.5f*sinf((vdat.framecnt - 10*i) * .01f)));
      int y = 150 + 400 * (0.5f+0.5f*sinf((vdat.framecnt - 10*i) * .02f));
      blit_simple(&vdat, &bitmap, x, y, i*32, 0, 32, bitmap.height);
    }

    /* Animate all letters, with zoom */
    for (i=6; i>=0; --i) {
      int x = vdat.res.width * (0.1f + 0.8f * (0.5f+0.5f*sinf(M_PI + (vdat.framecnt - 10*i) * .01f)));
      int y = 150 + 400 * (0.5f+0.5f*sinf((vdat.framecnt - 10*i) * .02f));
      blit_scale(&vdat, &bitmap, x, y, i*32, 0, 32, bitmap.height, 2.f);
    }

    /* Flip buffer onto screen */
    flip(&vdat);

    sysUtilCheckCallback();
  }
  waitFlip();
  bitmapDestroy(&bitmap);

  return 0;
}
