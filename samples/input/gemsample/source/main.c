/* Sample showing use of libgem. PlayStation Eye and PlayStation Move are
 * needed. Author: bigboss Date: September 2011. Minimun Firmware needed:
 * 3.41
 *
 * You need attached PlayStation Eye and PlayStation Move device before to run
 * the sample After load gemsample, you will see move sphere iluminated Point
 * to PlayStation Eye with your Move device and press move button, device will
 * be calibrated After calibration: X pressed on move will show X,Y,Z position
 * on mm and acelerometer position on device on console log Trigger pressed on
 * move will show frame number and sphere center position on console log Square
 * pressed on move will show inertial sensor values on console log Triangle
 * pressed on move will show image state values( u v and projection) X pressed
 * on sixaxis pad will go to xmb
 *
 * TODO improve video output with rsx texture features
 *
 * Finally libgem has full support on PSL1GHT let "Move" the Scene and share
 * your samples
 *
 */

#include <ppu-lv2.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <sysutil/video.h>
#include <rsx/gcm_sys.h>
#include <rsx/rsx.h>

#include <io/pad.h>
#include "rsxutil.h"
#include <sysmodule/sysmodule.h>
#include <sysutil/sysutil.h>
#include <sys/memory.h>
#include <io/camera.h>

// PAD
padInfo padinfo;
padData paddata;

// main bucle running
int running = 1;
int gem_flag = 0;
extern gcmContextData *context;
extern rsxBuffer buffers[MAX_BUFFERS];
extern int currentBuffer;

extern cameraInfoEx camInf;
extern cameraReadInfo camread;
extern u8 video_frame[640*480*4]; //added to get video output, transforamtion in spus give us a RGBA frame
int calibrate_flag = 1;
int tracking = 0;
int pos_x = 0;
int pos_y = 0;
int flag_video=0;
int
readPad ()
{
  int ret = 1;

  int i;

  ioPadGetInfo (&padinfo);
  for (i = 0; i < 6; i++) {	// 7 is our Move device
    if (padinfo.status[i]) {
      ioPadGetData (i, &paddata);

      if (paddata.BTN_CROSS) {

        ret = 0;		// To exit it will go to XMB
      }
    }

  }
  return ret;

}

void
loadModules ()
{
  int ret;

  printf ("Loading modules\n");
  ret = sysModuleLoad (SYSMODULE_CAMERA);
  printf ("laod camera module return %X\n", ret);
  ret = sysModuleLoad (SYSMODULE_GEM);
  printf ("load gem module return %X\n", ret);

}

void
unLoadModules ()
{
  int ret;

  printf ("Unloading modules\n");

  ret = sysModuleUnload (SYSMODULE_GEM);
  printf ("unload gem module return %X\n", ret);
  ret = sysModuleUnload (SYSMODULE_CAMERA);
  printf ("unload camera module return %X\n", ret);

}

static void
eventHandler (u64 status, u64 param, void *userdata)
{
  static struct XMBEvent {
    int drawing;
    int opened;
    int closed;
    int exit;
  } xmb;

  printf ("Received event %lX\n", status);
  if (status == SYSUTIL_EXIT_GAME) {
    xmb.exit = 1;
    running = 0;
  } else if (status == SYSUTIL_MENU_OPEN) {
    xmb.opened = 1;
    xmb.closed = 0;
  } else if (status == SYSUTIL_MENU_CLOSE) {
    xmb.opened = 0;
    xmb.closed = 1;
  } else if (status == SYSUTIL_DRAW_BEGIN) {
    /* We must start drawing, to avoid the app freezing */
    xmb.drawing = 1;
  } else if (status == SYSUTIL_DRAW_END) {
    xmb.drawing = 0;
  }
}

// Finalize stuff

void
endGame ()
{

  endGem ();

  // sysMemContainerDestroy(container1);
  endCamera ();
  unLoadModules ();

  sysUtilUnregisterCallback (SYSUTIL_EVENT_SLOT0);

  ioPadEnd ();

}

int
initGame ()
{
  int ret;

  // First load modules needed
  loadModules ();
  // When we finalize this method is called
  atexit (endGame);
  // register callback
  sysUtilRegisterCallback (SYSUTIL_EVENT_SLOT0, eventHandler, NULL);
  // Init screen
  ret = startScreen ();
  // Init pad
  ioPadInit (6);
  // Init camera
  // printf("cameraInit() returned %d\n", cameraInit());
  initGem ();
  ret = initCamera ();
  return ret;

}
void
displayFrame(s32 posx,s32 posy)
{
  int i,j;
  u32 pixel1;
  int w=640;
  int h=480;
  u8 *pixels=(u8*)video_frame;
  for(j=0;j<h;j++){
    for(i=0;i<w;i++){
	  u32 pixel1=0x80000000|pixels[j*4*640+4*i]<<16|pixels[j*4*640+4*i+1]<<8|pixels[j*4*640+4*i+2];
      buffers[currentBuffer].ptr[(j+posy)*buffers[currentBuffer].width+i+posx]=pixel1;
	}

  }
}
int
main (s32 argc, const char *argv[])
{
  int ret;

  long frame = 0;		// To keep track of how many frames we have
				// rendered.

  running = initGame ();

  // Ok, everything is setup. Now for the main loop.
  while (running) {
    // Check the pads.
    running = readPad ();

    waitFlip ();		// Wait for the last flip to finish, so we can
				// draw to the old buffer

    ret = readCamera ();
    if (ret != 0) {
		
      readGem ();	
	  displayFrame(0,0);

    }

    flip (context, buffers[currentBuffer].id);	// Flip buffer onto screen

    currentBuffer++;
    if (currentBuffer >= MAX_BUFFERS)
      currentBuffer = 0;

    sysUtilCheckCallback ();
  }

  endScreen ();

  return 0;
}
