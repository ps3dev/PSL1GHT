/*! \file rsx.h
 \brief RSX library.

This library contains the necessary functions for handling the RSX, the
accelerated 3D chip of the PS3.

*/

/*! \page rsxqk Quick guide to RSX programming

RSX is the 3D accelerator processor of the PS3. Here are the basic steps to get
it handle your 3D objects in your application.

You may also use basic RSX management for simple frame buffer management with
double buffering .


\section rsxqk_display_management Display management

The basic steps for managing the display are the following.

\subsection rsxqk_init Initialization

 - Create a RSX context (see \ref rsxInit).
 - Configure the video (video mode, color depth, aspect ratio). See
    \ref cellVideoOutConfigure for doing this. Default values can be obtained using
    \ref cellVideoOutGetState.
 - Set flip mode (see \ref cellGcmSetFlipMode). To prevent unpleasant flickering,
    you usually want to synchronize screen flipping with vertical refresh (use
    the \ref CELL_GCM_FLIP_VSYNC value).
 - Allocate buffers in video memory. If you want to perform double buffering,
    you need to allocate two buffers. In the most commonly used color depth mode
    (32-bit = 4 bytes), each buffer has a size of
    <code>screen_width*screen_height*4</code>.
    For each buffer, the steps are the following:
    - Allocate a 64-byte aligned buffer in RSX memory with \ref rsxMemalign.
    - Generate an offset for the buffer address using \ref rsxAddressToOffset.
    - Setup the buffer using \ref cellGcmSetDisplayBuffer, providing the buffer
      number (starting from 0) and the buffer offset generated from the previous
      step.
 - Allocate a depth buffer, aligned to 64-byte boundary, which size can be
    <code>screen_width*screen_height*2</code> if using 16-bit depth mode. Use
    \ref rsxMemalign for this. Generate an offset to that depth buffer with
    \ref rsxAddressToOffset.
 - Reset the flip status using \ref cellGcmResetFlipStatus.

\subsection rsxqk_flip Setting the render target and flipping

For each frame to be drawn, the steps are the following:

 - Write the pixel data to the buffer which is not being displayed. For
    instance, if buffer 0 is being displayed, write to buffer 1.
 - Push a flip command using \ref, cellGcmSetFlip specifying the buffer to be
    displayed (the one you just updated).
 - Flush the RSX buffer (\ref rsxFlushBuffer).
 - Force the RSX to wait for next flip (\ref cellGcmSetWaitFlip).
 - Set the new render target using \ref rsxSetSurface. You'll have to provide
    that function a pointer to a \ref CellGcmSurface structure you filled with
    proper color depth mode, buffer offset, depth buffer mode, etc. You can
    borrow proper valid values from the various graphics samples in the samples
    directory.


Then, before modifying the next buffer, ensure the flip actually has occurred
by querying for the flip status (\ref cellGcmGetFlipStatus) in a loop. Add timing
after each negative query in order not to take too much CPU time. For this,
a <code>usleep(200)</code> call should be fine.



\todo Finish that page.


*/


#ifndef __RSX_H__
#define __RSX_H__

#include <rsx/mm.h>
#include <rsx/gcm_sys.h>
#include <rsx/rsx_program.h>
#include <rsx/commands.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Pointer to default command buffer context */
extern CellGcmContextData *gGcmContext ATTRIBUTE_PRXPTR;

/*! \brief Initialize the RSX context and the RSX memory manager.

The user must provide a 1 MB-aligned IO buffer allocated in main memory, which
size is a multiple of 1 megabyte.

This function initializes a heap structure in RSX memory space, allowing
dynamic memory allocation using \ref rsxMalloc, \ref rsxMemalign and
\ref rsxFree.
\param context Pointer to receive the address of the default command buffer
\param cmdSize The command buffer size.
\param ioSize The allocated IO buffer size.
\param ioAddress Pointer to an allocated buffer of \p ioSize bytes.
\return zero if no error occured, nonzero otherwise.
*/
s32 rsxInit(CellGcmContextData **context,u32 cmdSize,u32 ioSize,const void *ioAddress);

void rsxSetupContextData(CellGcmContextData *context,const u32 *addr,u32 size,CellGcmContextCallback cb);
void rsxSetCurrentBuffer(CellGcmContextData **context,const u32 *addr,u32 size);
void rsxSetDefaultCommandBuffer(CellGcmContextData **context);
void rsxSetUserCallback(CellGcmContextCallback cb);
void rsxSetupContextData(CellGcmContextData *context,const u32 *addr,u32 size,CellGcmContextCallback cb);

u32* rsxGetCurrentBuffer();

/*! \brief Converts a pointer value in RSX memory to an offset.
\param ptr The pointer whose value is to be converted.
\param offset A pointer to the returned offset value.
\return zero if no error occured, nonzero otherwise.
*/
static inline s32 rsxAddressToOffset(const void *ptr,u32 *offset)
{
	return cellGcmAddressToOffset(ptr,offset);
}

/*! \brief Convert a floating point coordinate into 32-bit signed fixed point format. */
static inline s32 rsxGetFixedSint32(f32 f)
{
	return (s32)(f*1048576.0f);
}

/*! \brief Convert a floating point coordinate into 16-bit unsigned fixed point format. */
static inline u16 rsxGetFixedUint16(f32 f)
{
	return (u16)(f*16.0f);
}

static inline u32 rsxAlign(u32 alignment, u32 value)
{
	return (alignment==0 ? value : (value==0 ? 0 : (((u32)((value - 1)/alignment) + 1)*alignment)));
}

#ifdef __cplusplus
	}
#endif

#endif
