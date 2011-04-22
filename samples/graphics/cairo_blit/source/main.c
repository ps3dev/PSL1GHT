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

/*************************************************************************
 * 22 April 2011
 *
 * This modification to the cairo example demonstrates how to use the DMA
 * capabilities of the RSX to blit the rendered image to the display.
 * Instead of rendering directly to the RSX framebuffer memory, cairo 
 * renders to a buffer in main memory that's then blitted to the RSX.
 *
 * This transfer is asynchronous with respect to the PPU, and I'd thought
 * would be faster than having cairo perform lots of small memory transfers
 * by rendering directly to the RSX framebuffer mapped into main memory.
 * For this example, though, the reported frame rate when using the DMA 
 * operations are about the same as when DMA blitting is not used.
 *
 * There are three different ways to invoke the transfer - rsxSetTransferData,
 * rsxSetTransferImage, rsxSetTransferScaleMode/rsxSetTransferScaleSurface.
 * TransferData and TransferImage basically achieve the same thing;
 * TransferData presents an interface suitable for generic blocks of data,
 * while TransferImage's parameters are convenient for specifying 
 * rectangle-shaped arrays of data (such as... images!).
 *
 * The SetTransferScale functions can scale an image as it's blitted, and
 * can also change its pixel format along the way. This can be used to
 * get Cairo to render to the temporary buffer using the smaller RGB16_565
 * format.
 * 
 */

#define BLIT_TRANSFER_DATA 1
#define BLIT_TRANSFER_IMAGE 2
#define BLIT_TRANSFER_SCALE 3

// Set the BLIT symbol to one of BLIT_TRANSFER_DATA, BLIT_TRANSFER_IMAGE, or 
// BLIT_TRANSFER_SCALE to choose which function is used for the transfer.
// Or set it to 0 to maintain the original behavior of the sample.
#define BLIT BLIT_TRANSFER_SCALE
#define BLIT_CAIRO_FORMAT CAIRO_FORMAT_RGB24 /*CAIRO_FORMAT_RGB16_565*/

// Specify the size of the surface that cairo renders to. I've only tested
// this on an HD display. The width & height will get clamped to the display
// size as reported by
#define BLIT_CAIRO_WIDTH 1280
#define BLIT_CAIRO_HEIGHT 720

// If we're not using BLIT_TRANSFER_SCALE, force cairo to render to the same
// pixel format as the video display surface, as these blit functions do
// not convert between pixel formats.
#if (BLIT == BLIT_TRANSFER_DATA) || (BLIT == BLIT_TRANSFER_IMAGE)
#undef BLIT_CAIRO_FORMAT
#define BLIT_CAIRO_FORMAT CAIRO_FORMAT_RGB24
#endif

#define DEBUG(...)

static inline uintptr_t
align_up(uintptr_t addr,size_t n) {
  if(addr % n != 0) {
    return addr + n - addr % n;
  }
  else {
    return addr;
  }
}

static inline uintptr_t
align_down(uintptr_t addr,size_t n) {
  if(addr % n != 0) {
    return addr - addr % n;
  }
  else {
    return addr;
  }
}

#if BLIT > 0
u8 * image = 0;
u32 image_rsx = 0;
cairo_t *cr = NULL;
cairo_surface_t *surface = NULL;
const int cairo_format = BLIT_CAIRO_FORMAT;

void
initCairo(int width,int height)
{
  // 
  static const size_t depth = (cairo_format == CAIRO_FORMAT_RGB24) ? sizeof(u32) : ((cairo_format == CAIRO_FORMAT_RGB16_565) ? sizeof(u16) : sizeof(u32));
  static const size_t align = 1024 * 1024;

  // main memory that gets mapped to the address space of the RSX must be aligned on a 1024*1024 boundary.
  // gcmMapMainMemory maps blocks in 1024*1024 increments, so, we make the buffer at least that size.
  size_t size = align_up(width * height * depth,align);
  image = memalign(align,size);
  
  // map it into the RSX's address space:
  gcmMapMainMemory(image,size,&image_rsx);

  surface = cairo_image_surface_create_for_data ((u8 *) image,
						 cairo_format, width, height, width * depth);
  cr = cairo_create (surface);
}
#endif

// Draw a single frame, do all your drawing/animation from in here.
void
drawFrame (rsxBuffer *buffer,int width, int height, int pitch, int frame)
{
#if BLIT == 0
  cairo_t *cr;
  cairo_surface_t *surface = NULL;
#endif

  static time_t starttime = 0;
  double fps = 0;

  if (starttime == 0)
    starttime = time (NULL);
  else
    fps = frame / difftime (time (NULL), starttime);

  DEBUG ("starttime is %lu - now is %lu - diff is %f\n", starttime, time (NULL),
      difftime (time (NULL), starttime));
  DEBUG ("Drawing at %f FPS\n", fps);
#if BLIT == 0
  surface = cairo_image_surface_create_for_data ((u8 *) buffer->ptr,
						 CAIRO_FORMAT_RGB24, width, height, pitch);
#endif

  if (surface != NULL) {
#if BLIT == 0
    cr = cairo_create (surface);
#endif

    if (cr != NULL) {
      // Lets start by clearing everything
      cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); // White
      cairo_paint (cr);

      /* Draw what needs tobe drawn */
      {
        int size = 100;
        float r, g;
        float rel;
        char buf[100];

        cairo_save(cr);
        cairo_new_path(cr);

        cairo_translate(cr, width / 2, height / 2);
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

#if BLIT == 0
      cairo_destroy (cr); // Realease Surface
#endif
    }

#if BLIT == 0
    cairo_surface_finish (surface);
    cairo_surface_destroy (surface); // Flush and destroy the cairo surface
#endif
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
  const size_t pitch = width * sizeof(u32);
  for (i = 0; i < MAX_BUFFERS; i++)
    makeBuffer (&buffers[i], width, height, i);

#if BLIT == 0
  const size_t cairo_width = width, cairo_height = height;
#else
  const size_t cairo_width = (BLIT_CAIRO_WIDTH < width) ? BLIT_CAIRO_WIDTH : width, cairo_height = (BLIT_CAIRO_HEIGHT < height) ? BLIT_CAIRO_HEIGHT : height;
  const size_t cairo_pitch = cairo_width * ((cairo_format == CAIRO_FORMAT_RGB24) ? sizeof(u32) : ((cairo_format == CAIRO_FORMAT_RGB16_565) ? sizeof(u16) : sizeof(u32)));
#endif

#if BLIT > 0
  initCairo(cairo_width,cairo_height);
#endif

#if BLIT == BLIT_TRANSFER_SCALE
  gcmTransferScale transfer_settings = {
    .conversion = GCM_TRANSFER_CONVERSION_TRUNCATE,
    .format = (cairo_format == CAIRO_FORMAT_RGB16_565) ? GCM_TRANSFER_SCALE_FORMAT_R5G6B5 : GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8,
    .operation = GCM_TRANSFER_OPERATION_SRCCOPY,
    .clipX = 0,
    .clipY = 0,
    .clipW = width,
    .clipH = height,
    .outX = 0,
    .outY = 0,
    .outW = width,
    .outH = height,
    .ratioX = rsxGetFixedSint32((float)cairo_width / (float)width),
    .ratioY = rsxGetFixedSint32((float)cairo_height / (float)height),
    .inW = cairo_width,
    .inH = cairo_height,
    .pitch = cairo_pitch,
    .origin = GCM_TRANSFER_ORIGIN_CORNER,
    .interp = GCM_TRANSFER_INTERPOLATOR_LINEAR,
    .offset = image_rsx,
    .inX = 0,
    .inY = 0
  };

  gcmTransferSurface transfer_surface[MAX_BUFFERS] = {
    {
      .format = GCM_TRANSFER_SURFACE_FMT_A8R8G8B8,
      .pitch = pitch,
      .offset = buffers[0].offset
    },
    {
      .format = GCM_TRANSFER_SURFACE_FMT_A8R8G8B8,
      .pitch = pitch,
      .offset = buffers[1].offset
    }
  };
#endif

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
    drawFrame (&buffers[currentBuffer],cairo_width,cairo_height,pitch,frame++); // Draw into the unused buffer:

    // Blizzle that shizzle:
#if BLIT == BLIT_TRANSFER_DATA
    rsxSetTransferData(context,GCM_TRANSFER_MAIN_TO_LOCAL,
		       buffers[currentBuffer].offset,pitch,
		       image_rsx,cairo_pitch,
		       cairo_pitch,
		       cairo_height);
#elif BLIT == BLIT_TRANSFER_IMAGE
    rsxSetTransferImage(context,GCM_TRANSFER_MAIN_TO_LOCAL,
			buffers[currentBuffer].offset,pitch,0,0,
			image_rsx,cairo_pitch,0,0,cairo_width,cairo_height,4);
#elif BLIT == BLIT_TRANSFER_SCALE
    rsxSetTransferScaleMode(context,GCM_TRANSFER_MAIN_TO_LOCAL,GCM_TRANSFER_SURFACE);
    rsxSetTransferScaleSurface(context,&transfer_settings,transfer_surface + currentBuffer);
#endif

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

