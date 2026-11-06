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
#include <math.h>

#include "rsxutil.h"

#define GCM_LABEL_INDEX		255
// graphics
CellGcmContextData *context;
void *host_addr = NULL;
rsxBuffer buffers[MAX_BUFFERS];
int currentBuffer = 0;
u16 width;
u16 height;

// extern CellGcmContextData *context;
// extern void *host_addr;
// extern rsxBuffer buffers[MAX_BUFFERS];
// extern int currentBuffer;
// extern u16 width;
// extern u16 height;
static void waitRSXIdle (CellGcmContextData * context);

static u32 depth_pitch;
static u32 depth_offset;
static u32 *depth_buffer;

void
waitFlip ()
{
  while (cellGcmGetFlipStatus () != 0)
    usleep (200);		/* Sleep, to not stress the cpu. */
  cellGcmResetFlipStatus ();
}

int
flip (CellGcmContextData * context, s32 buffer)
{
  if (cellGcmSetFlip (context, buffer) == 0) {
    rsxFlushBuffer (context);
    // Prevent the RSX from continuing until the flip has finished.
    cellGcmSetWaitFlip (context);

    return TRUE;
  }
  return FALSE;
}

int
makeBuffer (rsxBuffer * buffer, u16 width, u16 height, int id)
{
  int depth = sizeof (u32);
  int pitch = depth * width;
  int size = depth * width * height;

  buffer->ptr = (uint32_t *) rsxMemalign (64, size);

  if (buffer->ptr == NULL)
    goto error;

  if (rsxAddressToOffset (buffer->ptr, &buffer->offset) != 0)
    goto error;

  /* Register the display buffer with the RSX */
  if (cellGcmSetDisplayBuffer (id, buffer->offset, pitch, width, height) != 0)
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
getResolution (u16 * width, u16 * height)
{
  CellVideoOutState state;
  CellVideoOutResolution resolution;

  /* Get the state of the display */
  if (cellVideoOutGetState (0, 0, &state) == 0 &&
      cellVideoOutGetResolution (state.displayMode.resolution, &resolution) == 0) {
    if (width)
      *width = resolution.width;
    if (height)
      *height = resolution.height;

    return TRUE;
  }
  return FALSE;
}

int
startScreen ()
{
  int i;
  int ret;

  /* Allocate a 1Mb buffer, alligned to a 1Mb boundary * to be our shared IO
   * memory with the RSX. */
  host_addr = memalign (1024 * 1024, HOST_SIZE);
  ret = initScreen (host_addr, HOST_SIZE);
  if (ret) {
    return 0;
  }

  getResolution (&width, &height);
  for (i = 0; i < MAX_BUFFERS; i++)
    makeBuffer (&buffers[i], width, height, i);

  flip (context, MAX_BUFFERS - 1);
  return 1;

}

void
endScreen ()
{
  int i;

  cellGcmSetWaitFlip (context);
  for (i = 0; i < MAX_BUFFERS; i++)
    rsxFree (buffers[i].ptr);

  rsxFinish (context, 1);
  free (host_addr);
}

int
initScreen (void *host_addr, u32 size)
{
  // CellGcmContextData *context = NULL; /* Context to keep track of the RSX
  // buffer. */
  CellVideoOutState state;
  CellVideoOutConfiguration vconfig;
  CellVideoOutResolution res;		/* Screen Resolution */

  /* Initilise Reality, which sets up the command buffer and shared IO memory */
  rsxInit (&context, CB_SIZE, size, host_addr);
  if (context == NULL)
    goto error;

  /* Get the state of the display */
  if (cellVideoOutGetState (0, 0, &state) != 0)
    goto error;

  /* Make sure display is enabled */
  if (state.state != 0)
    goto error;

  /* Get the current resolution */
  if (cellVideoOutGetResolution (state.displayMode.resolution, &res) != 0)
    goto error;

  /* Configure the buffer format to xRGB */
  memset (&vconfig, 0, sizeof (CellVideoOutConfiguration));
  vconfig.resolution = state.displayMode.resolution;
  vconfig.format = CELL_VIDEO_OUT_BUFFER_FORMAT_XRGB;
  vconfig.pitch = res.width * sizeof (u32);
  vconfig.aspect = state.displayMode.aspect;

  waitRSXIdle (context);

  if (cellVideoOutConfigure (0, &vconfig, NULL, 0) != 0)
    goto error;

  if (cellVideoOutGetState (0, 0, &state) != 0)
    goto error;

  cellGcmSetFlipMode (CELL_GCM_FLIP_VSYNC);	// Wait for VSYNC to flip

  depth_pitch = res.width * sizeof (u32);
  depth_buffer = (u32 *) rsxMemalign (64, (res.height * depth_pitch) * 2);
  rsxAddressToOffset (depth_buffer, &depth_offset);

  cellGcmResetFlipStatus ();

  return 0;

  error:
  if (context)
    rsxFinish (context, 0);

  if (host_addr)
    free (host_addr);

  return 1;
}

static void
waitFinish (CellGcmContextData * context, u32 sLabelVal)
{
  rsxSetWriteBackendLabel (context, GCM_LABEL_INDEX, sLabelVal);

  rsxFlushBuffer (context);

  while (*(vu32 *) cellGcmGetLabelAddress (GCM_LABEL_INDEX) != sLabelVal)
    usleep (30);

  sLabelVal++;
}

static void
waitRSXIdle (CellGcmContextData * context)
{
  u32 sLabelVal = 1;

  rsxSetWriteBackendLabel (context, GCM_LABEL_INDEX, sLabelVal);
  rsxSetWaitLabel (context, GCM_LABEL_INDEX, sLabelVal);

  sLabelVal++;

  waitFinish (context, sLabelVal);
}

void
setRenderTarget (CellGcmContextData * context, rsxBuffer * buffer)
{
  CellGcmSurface sf;

  sf.colorFormat = CELL_GCM_SURFACE_X8R8G8B8;
  sf.colorTarget = CELL_GCM_SURFACE_TARGET_0;
  sf.colorLocation[0] = CELL_GCM_LOCATION_RSX;
  sf.colorOffset[0] = buffer->offset;
  sf.colorPitch[0] = depth_pitch;

  sf.colorLocation[1] = CELL_GCM_LOCATION_RSX;
  sf.colorLocation[2] = CELL_GCM_LOCATION_RSX;
  sf.colorLocation[3] = CELL_GCM_LOCATION_RSX;
  sf.colorOffset[1] = 0;
  sf.colorOffset[2] = 0;
  sf.colorOffset[3] = 0;
  sf.colorPitch[1] = 64;
  sf.colorPitch[2] = 64;
  sf.colorPitch[3] = 64;

  sf.depthFormat = CELL_GCM_SURFACE_ZETA_Z16;
  sf.depthLocation = CELL_GCM_LOCATION_RSX;
  sf.depthOffset = depth_offset;
  sf.depthPitch = depth_pitch;

  sf.type = CELL_GCM_TEXTURE_LINEAR;
  sf.antiAlias = CELL_GCM_SURFACE_CENTER_1;

  sf.width = buffer->width;
  sf.height = buffer->height;
  sf.x = 0;
  sf.y = 0;

  rsxSetSurface (context, &sf);
}
