/*
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#include <ppu-lv2.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <sysutil/video.h>
#include <rsx/gcm_sys.h>
#include <rsx/rsx.h>
#include <io/pad.h>
#include <time.h>
#include <cairo/cairo.h>
#include <math.h>

#include "rsxutil.h"

#define GCM_LABEL_INDEX		255

static void waitRSXIdle(gcmContextData *context);

static u32 depth_pitch;
static u32 depth_offset;
static u32 *depth_buffer;

void
waitFlip ()
{
  while (gcmGetFlipStatus () != 0)
    usleep (200);  /* Sleep, to not stress the cpu. */
  gcmResetFlipStatus ();
}

int
flip (gcmContextData *context, s32 buffer)
{
  if (gcmSetFlip (context, buffer) == 0) {
    rsxFlushBuffer (context);
    // Prevent the RSX from continuing until the flip has finished.
    gcmSetWaitFlip (context);

    return TRUE;
  }
  return FALSE;
}


int
makeBuffer (rsxBuffer * buffer, u16 width, u16 height, int id)
{
  int depth = sizeof(u32);
  int pitch = depth * width;
  int size = depth * width * height;

  buffer->ptr = (uint32_t*) rsxMemalign (64, size);

  if (buffer->ptr == NULL)
    goto error;

  if (rsxAddressToOffset (buffer->ptr, &buffer->offset) != 0)
    goto error;

  /* Register the display buffer with the RSX */
  if (gcmSetDisplayBuffer (id, buffer->offset, pitch, width, height) != 0)
    goto error;

  buffer->width = width;
  buffer->height = height;
  buffer->id = id;

  return TRUE;

 error:
  if (buffer->ptr != NULL)
    rsxFree (buffer->ptr);

  return FALSE;
}

int
getResolution (u16 *width, u16 *height)
{
  videoState state;
  videoResolution resolution;

  /* Get the state of the display */
  if (videoGetState (0, 0, &state) == 0 &&
      videoGetResolution (state.displayMode.resolution, &resolution) == 0) {
    if (width)
      *width = resolution.width;
    if (height)
      *height = resolution.height;

    return TRUE;
  }
  return FALSE;
}

gcmContextData *
initScreen (void *host_addr, u32 size)
{
  gcmContextData *context = NULL; /* Context to keep track of the RSX buffer. */
  videoState state;
  videoConfiguration vconfig;
  videoResolution res; /* Screen Resolution */

  /* Initilise Reality, which sets up the command buffer and shared IO memory */
  context = rsxInit (CB_SIZE, size, host_addr);
  if (context == NULL)
    goto error;

  /* Get the state of the display */
  if (videoGetState (0, 0, &state) != 0)
    goto error;

  /* Make sure display is enabled */
  if (state.state != 0)
    goto error;

  /* Get the current resolution */
  if (videoGetResolution (state.displayMode.resolution, &res) != 0)
    goto error;

  /* Configure the buffer format to xRGB */
  memset (&vconfig, 0, sizeof(videoConfiguration));
  vconfig.resolution = state.displayMode.resolution;
  vconfig.format = VIDEO_BUFFER_FORMAT_XRGB;
  vconfig.pitch = res.width * sizeof(u32);
  vconfig.aspect = state.displayMode.aspect;

  waitRSXIdle(context);

  if (videoConfigure (0, &vconfig, NULL, 0) != 0)
    goto error;

  if (videoGetState (0, 0, &state) != 0)
    goto error;

  gcmSetFlipMode (GCM_FLIP_VSYNC); // Wait for VSYNC to flip

  depth_pitch = res.width * sizeof(u32);
  depth_buffer = (u32 *) rsxMemalign (64, (res.height * depth_pitch)* 2);
  rsxAddressToOffset (depth_buffer, &depth_offset);

  gcmResetFlipStatus();

  return context;

 error:
  if (context)
    rsxFinish (context, 0);

  if (host_addr)
    free (host_addr);

  return NULL;
}


static void
waitFinish(gcmContextData *context, u32 sLabelVal)
{
  rsxSetWriteBackendLabel (context, GCM_LABEL_INDEX, sLabelVal);

  rsxFlushBuffer (context);

  while(*(vu32 *) gcmGetLabelAddress (GCM_LABEL_INDEX) != sLabelVal)
    usleep(30);

  sLabelVal++;
}

static void
waitRSXIdle(gcmContextData *context)
{
  u32 sLabelVal = 1;

  rsxSetWriteBackendLabel (context, GCM_LABEL_INDEX, sLabelVal);
  rsxSetWaitLabel (context, GCM_LABEL_INDEX, sLabelVal);

  sLabelVal++;

  waitFinish(context, sLabelVal);
}

void
setRenderTarget(gcmContextData *context, rsxBuffer *buffer)
{
  gcmSurface sf;

  sf.colorFormat = GCM_TF_COLOR_X8R8G8B8;
  sf.colorTarget = GCM_TF_TARGET_0;
  sf.colorLocation[0] = GCM_LOCATION_RSX;
  sf.colorOffset[0] = buffer->offset;
  sf.colorPitch[0] = depth_pitch;

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
  sf.depthOffset = depth_offset;
  sf.depthPitch = depth_pitch;

  sf.type = GCM_TF_TYPE_LINEAR;
  sf.antiAlias 	= GCM_TF_CENTER_1;

  sf.width = buffer->width;
  sf.height = buffer->height;
  sf.x = 0;
  sf.y = 0;

  rsxSetSurface (context, &sf);
}
