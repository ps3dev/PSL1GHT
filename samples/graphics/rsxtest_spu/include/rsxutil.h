#ifndef __RSXUTIL_H__
#define __RSXUTIL_H__

#include <ppu-types.h>
#include <rsx/rsx.h>

#define CB_SIZE		0x100000
#define HOST_SIZE	(32*1024*1024)

#define FRAME_BUFFER_COUNT					2

extern CellGcmContextData *context;

extern u32 curr_fb;

extern u32 display_width;
extern u32 display_height;

extern u32 depth_pitch;
extern u32 depth_offset;
extern u32 *depth_buffer;

extern u32 color_pitch;
extern u32 color_offset[FRAME_BUFFER_COUNT];
extern u32 *color_buffer[FRAME_BUFFER_COUNT];

extern f32 aspect_ratio;

void setRenderTarget(u32 index);
void init_screen(void *host_addr,u32 size);
void waitflip();
void flip();

#endif
