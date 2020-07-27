// Cairo clock sample by CrystalCT (crystal@unict.it)

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

u16 width;
u16 height;

// Draw a single frame, do all your drawing/animation from in here.
void
drawFrame (rsxBuffer *buffer, int frame)
{
  cairo_t *cr;
  cairo_surface_t *surface = NULL;
  
  

  

  
  surface = cairo_image_surface_create_for_data ((u8 *) buffer->ptr,
      CAIRO_FORMAT_RGB24, buffer->width, buffer->height, buffer->width * 4);

  if (surface != NULL) {
    cr = cairo_create (surface);
    if (cr != NULL) {
      // Lets start by clearing everything
      cairo_scale(cr, height, height);
	  cairo_translate(cr, 0.5 * width/height, 0.5);
	  cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
      cairo_paint (cr);

      /* Draw what needs tobe drawn */
      {
        // ARC
		double xc = 0.0;
		double yc = 0.0;
		double m_radius = 0.42;
		double m_line_width = 0.05;
		cairo_set_source_rgb (cr, 0.0, 0.0, 0.0); // Black
		
		cairo_set_line_width (cr, m_line_width);
		
		cairo_arc (cr, xc, yc, m_radius, 0, 2 * M_PI);
		cairo_stroke(cr);
		
		//clock ticks
		  int i;
		  for (i = 0; i < 12; i++)
		  {
			double inset = 0.05;

			cairo_save(cr);
			cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

			if(i % 3 != 0)
			{
			  inset *= 0.8;
			  cairo_set_line_width(cr, 0.03);
			}

			cairo_move_to(cr, 
			  (m_radius - inset) * cos (i * M_PI / 6),
			  (m_radius - inset) * sin (i * M_PI / 6));
			cairo_line_to (cr, 
			  m_radius * cos (i * M_PI / 6),
			  m_radius * sin (i * M_PI / 6));
			cairo_stroke(cr);
			cairo_restore(cr); /* stack-pen-size */
		  }
		
		// store the current time
		  time_t rawtime;
		  time(&rawtime);
		  struct tm * timeinfo = localtime (&rawtime);

		  // compute the angles of the indicators of our clock
		  double minutes = timeinfo->tm_min * M_PI / 30;
		  double hours = timeinfo->tm_hour * M_PI / 6;
		  double seconds= timeinfo->tm_sec * M_PI / 30;

		  cairo_save(cr);
		  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

		  // draw the seconds hand
		  cairo_save(cr);
		  cairo_set_line_width(cr, m_line_width / 3);
		  cairo_set_source_rgb(cr, 0.7, 0.7, 0.7); // gray
		  cairo_move_to(cr, 0, 0);
		  cairo_line_to(cr, sin(seconds) * (m_radius * 0.9),
			-cos(seconds) * (m_radius * 0.9));
		  cairo_stroke(cr);
		  cairo_restore(cr);
		  
		  // draw the minutes hand
		  cairo_set_source_rgb(cr, 0.117, 0.337, 0.612);   // blue
		  cairo_move_to(cr, 0, 0);
		  cairo_line_to(cr, sin(minutes + seconds / 60) * (m_radius * 0.8),
			-cos(minutes + seconds / 60) * (m_radius * 0.8));
		  cairo_stroke(cr);

		  // draw the hours hand
		  cairo_set_source_rgb(cr, 0.337, 0.612, 0.117);   // green
		  cairo_move_to(cr, 0, 0);
		  cairo_line_to(cr, sin(hours + minutes / 12.0) * (m_radius * 0.5),
			-cos(hours + minutes / 12.0) * (m_radius * 0.5));
		  cairo_stroke(cr);
		  cairo_restore(cr);

		  // draw a little dot in the middle
		  cairo_arc(cr, 0, 0, m_line_width / 3.0, 0, 2 * M_PI);
		  cairo_fill(cr);
				
      }

      cairo_destroy (cr); // Realease Surface
    }

    cairo_surface_finish (surface);
    cairo_surface_destroy (surface); // Flush and destroy the cairo surface
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
    drawFrame (&buffers[currentBuffer], frame++); // Draw into the unused buffer
    flip (context, buffers[currentBuffer].id); // Flip buffer onto screen

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

