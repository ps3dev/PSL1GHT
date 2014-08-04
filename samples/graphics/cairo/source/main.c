#include <ppu-lv2.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <io/pad.h>
#include <time.h>
#include <cairo/cairo.h>
#include <math.h>
#include <time.h>

#include "rsxutil.h"

#define MAX_BUFFERS 2

#define DEBUG(...)

/* Draw a single frame, do all your drawing/animation from in here. */
void
drawFrame (rsxBuffer *buffer, int frame)
{
  cairo_t *cr;
  cairo_surface_t *surface = NULL;
  static time_t starttime = 0;
  double fps = 0;

  if (starttime == 0)
    starttime = time (NULL);
  else
    fps = frame / difftime (time (NULL), starttime);

  DEBUG ("starttime is %lu - now is %lu - diff is %f\n", starttime, time (NULL),
      difftime (time (NULL), starttime));
  DEBUG ("Drawing at %f FPS\n", fps);
  surface = cairo_image_surface_create_for_data ((u8 *) buffer->ptr,
      CAIRO_FORMAT_RGB24, buffer->width, buffer->height, buffer->width * 4);

  if (surface != NULL) {
    cr = cairo_create (surface);
    if (cr != NULL) {
      /* Lets start by clearing everything */
      cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* White */
      cairo_paint (cr);

      /* Draw what needs tobe drawn */
      {
        int size = 100;
        float r, g;
        float rel;
        char buf[100];

        cairo_save(cr);
        cairo_new_path(cr);

        cairo_translate(cr, buffer->width / 2, buffer->height / 2);
        cairo_rotate (cr, frame * M_PI / 180);
        cairo_set_antialias(cr, CAIRO_ANTIALIAS_SUBPIXEL);
	cairo_line_to(cr, - (size / 2), - (size / 2));
	cairo_line_to(cr, - (size / 2), + (size / 2));
	cairo_line_to(cr, + (size / 2), + (size / 2));
	cairo_line_to(cr, + (size / 2), - (size / 2));
        cairo_close_path(cr);

        rel = sin ((frame % 180) * M_PI / 180);
        if (rel < 0.5) {
          r = 0.85;
          g = 2 * rel;
        } else {
          g = 0.85;
          r = 2 * (1.0 - rel);
        }

        cairo_set_source_rgb(cr, r, g, 0);
	cairo_fill_preserve(cr);

        r = g;
        g = 0.85;
        cairo_set_source_rgb(cr, r, g, 0);
        cairo_stroke(cr);

        cairo_identity_matrix (cr);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_select_font_face(cr, "Purisa",
            CAIRO_FONT_SLANT_NORMAL,
            CAIRO_FONT_WEIGHT_BOLD);

        cairo_set_font_size(cr, 13);

        cairo_move_to(cr, 50, 30);
        snprintf (buf, 100, "FPS : %f", fps);
        cairo_show_text (cr, buf);
      }

      cairo_destroy (cr); /* Realease Surface */
    }

    cairo_surface_finish (surface);
    cairo_surface_destroy (surface); /* Flush and destroy the cairo surface */
  }

}

int
main (s32 argc, const char* argv[])
{
  gcmContextData *context;
  void *host_addr = NULL;
  rsxBuffer buffers[MAX_BUFFERS];
  int currentBuffer = 0;
  padInfo padinfo;
  padData paddata;
  u16 width;
  u16 height;
  int frame = 0;
  int i;

  /* Allocate a 1Mb buffer, alligned to a 1Mb boundary
   * to be our shared IO memory with the RSX. */
  host_addr = memalign (1024*1024, HOST_SIZE);
  context = initScreen (host_addr, HOST_SIZE);
  ioPadInit (7);

  getResolution(&width, &height);
  for (i = 0; i < MAX_BUFFERS; i++)
    makeBuffer (&buffers[i], width, height, i);

  flip(context, MAX_BUFFERS - 1);

  DEBUG ("Starting Cairo test\n");

  while (1) {
    ioPadGetInfo (&padinfo);
    for(i = 0; i < MAX_PADS; i++) {
        if(padinfo.status[i]) {
            ioPadGetData (i, &paddata);
            if(paddata.BTN_START) {
              goto end;
            }
        }
    }

    setRenderTarget(context, &buffers[currentBuffer]);

    DEBUG ("Drawing frame %d\n", frame);
    waitFlip ();
    drawFrame (&buffers[currentBuffer], frame++); /* Draw into the unused buffer */
    flip (context, buffers[currentBuffer].id); /* Flip buffer onto screen */

    currentBuffer++;
    if (currentBuffer >= MAX_BUFFERS)
      currentBuffer = 0;
  }

 end:

  gcmSetWaitFlip(context);
  for (i = 0; i < MAX_BUFFERS; i++)
    rsxFree (buffers[i].ptr);

  rsxFinish (context, 1);
  free (host_addr);
  ioPadEnd();

  return 0;
}

