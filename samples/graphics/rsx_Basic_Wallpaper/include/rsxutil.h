#ifndef __RSXUTIL_H__
#define __RSXUTIL_H__

#include <rsx/rsx.h>

#include <debugfont.h>

#define DEFAULT_CB_SIZE						0x80000		// 512Kb default command buffer size
#define HOST_STATE_CB_SIZE					0x10000		// 64Kb state command buffer size (used for resetting certain default states)
#define HOST_ADDR_ALIGNMENT					(1024*1024)

#define GCM_PREPARED_BUFFER_INDEX			65
#define GCM_BUFFER_STATUS_INDEX				66
#define GCM_WAIT_LABEL_INDEX				255

#define MAX_BUFFER_QUEUE_SIZE				1

#define BUFFER_IDLE							0
#define BUFFER_BUSY							1

#define FRAME_BUFFER_COUNT					4

extern u32 curr_fb;

extern u32 display_width;
extern u32 display_height;

extern u32 depth_pitch;
extern u32 depth_offset;
extern void *depth_buffer;

extern u32 color_pitch;
extern u32 color_offset[FRAME_BUFFER_COUNT];
extern void *color_buffer[FRAME_BUFFER_COUNT];

extern void *state_buffer;
extern u32 state_offset;


extern f32 aspect_ratio;

void initScreen(u32 hostBufferSize);
void flip();
void finish();

void setRenderTarget(u32 index);

#endif // __RSXUTIL_H__
