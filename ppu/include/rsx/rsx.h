/*! \file rsx.h
 \brief RSX library.

This library contains the necessary functions for handling the RSX, the
accelerated 3D chip of the PS3.

*/

/*! \page rsxqk Quick guide to RSX programming

RSX is the 3D accelerated of the PS3. Here are the basic steps to get it handle
your 3D objects in your application.

\section Initialization

First of all, you need to setup the display.

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

/*! \brief Initialize the RSX context and the RSX memory manager.

The user must provide a 1 MB-aligned IO buffer allocated in main memory, which
size is a multiple of 1 megabyte.

This function initializes a heap structure in RSX memory space, allowing
dynamic memory allocation using \ref rsxMalloc, \ref rsxMemalign and
\ref rsxFree.
\param cmdSize The command buffer size.
\param ioSize The allocated IO buffer size.
\param ioAddress Pointer to an allocated buffer of \p ioSize bytes.
\return Pointer to the allocated context structure.
*/
gcmContextData* rsxInit(const u32 cmdSize,const u32 ioSize,const void *ioAddress);

/*! \brief Flushes the RSX command buffer.

This ensures all remaining commands in the command buffer are executed, and
that the buffer is empty when that function returns.
\param context Pointer to the context object.
*/
void rsxFlushBuffer(gcmContextData *context);
void rsxResetCommandBuffer(gcmContextData *context);
void rsxFinish(gcmContextData *context,u32 ref_value);
void rsxSetupContextData(gcmContextData *context,const u32 *addr,const u32 size,gcmContextCallback cb);

/*! \brief Converts a pointer value in RSX memory to an offset.
\param ptr The pointer whose value is to be converted.
\param offset A pointer to the returned offset value.
\return zero if no error occured, nonzero otherwise.
*/
static inline s32 rsxAddressToOffset(void *ptr,u32 *offset)
{
	return gcmAddressToOffset(ptr,offset);
}

static inline s32 rsxGetFixedSint32(const f32 f)
{
	return (s32)(f*1048576.0f);
}

static inline u16 rsxGetFixedUint16(const f32 f)
{
	return (u16)(f*16.0f);
}

#ifdef __cplusplus
	}
#endif

#endif
