/*! \file commands.h
 \brief RSX commands.

These are functions to enqueue commands into the RSX's command buffer.
*/

#ifndef __RSX_COMMANDS_H__
#define __RSX_COMMANDS_H__

#include <rsx/gcm_sys.h>
#include <rsx/rsx_program.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Set drawing direction of front face.
\param context Pointer to the context object.
\param dir Drawing direction of front face. Possible values are:
- \ref GCM_FRONTFACE_CW
- \ref GCM_FRONTFACE_CCW
*/
void rsxSetFrontFace(gcmContextData *context,u32 dir);

/*! \brief Set culling mode.
\param context Pointer to context object.
\param cull Type of cull mode. Possible values are:
- \ref GCM_CULL_FRONT
- \ref GCM_CULL_BACK
- \ref GCM_CULL_ALL
*/
void rsxSetCullFace(gcmContextData *context,u32 cull);

/*! \brief Enable/Disable face culling.
\param context Pointer to the context object.
\param enable Enable flag. Possible values are:
 - \ref GCM_TRUE
 - \ref GCM_FALSE
 */
void rsxSetCullFaceEnable(gcmContextData *context,u32 enable);

/*! \brief Enable/Disable write to depth buffer.
\param context Pointer to the context object.
\param enable Enable flag. Possible values are:
- \ref GCM_TRUE
- \ref GCM_FALSE
*/
void rsxSetDepthWriteEnable(gcmContextData *context,u32 enable);

/*! \brief Stop the render sequence.

Stops the rendering for a primitive, started by \ref rsxDrawVertexBegin.
\param context Pointer to the context object.
*/
void rsxDrawVertexEnd(gcmContextData *context);

/*! \brief Set the shading model for the render sequence.
\param context Pointer to the context object.
\param shadeModel Type of shading model. Possible values are:
- \ref GCM_SHADE_MODEL_FLAT
- \ref GCM_SHADE_MODEL_SMOOTH
*/
void rsxSetShadeModel(gcmContextData *context,u32 shadeModel);

/*! \brief Start the render sequence.

Starts the rendering for a primitive.
\param context Pointer to the context object.
\param type Type of primitive to render. Possible values are:
- \ref GCM_TYPE_POINTS
- \ref GCM_TYPE_LINES
- \ref GCM_TYPE_LINE_LOOP
- \ref GCM_TYPE_LINE_STRIP
- \ref GCM_TYPE_TRIANGLES
- \ref GCM_TYPE_TRIANGLE_STRIP
- \ref GCM_TYPE_TRIANGLE_FAN
- \ref GCM_TYPE_QUADS
- \ref GCM_TYPE_QUAD_STRIP
- \ref GCM_TYPE_POLYGON
*/
void rsxDrawVertexBegin(gcmContextData *context,u32 type);

void rsxDrawVertex2f(gcmContextData *context,u8 idx,f32 x,f32 y);
void rsxDrawVertex3f(gcmContextData *context,u8 idx,f32 x,f32 y,f32 z);
void rsxDrawVertex4f(gcmContextData *context,u8 idx,f32 x,f32 y,f32 z,f32 w);
void rsxSetScissor(gcmContextData *context,u16 x,u16 y,u16 w,u16 h);
void rsxSetDepthFunc(gcmContextData *context,u32 func);
void rsxSetDepthTestEnable(gcmContextData *context,u32 enable);
void rsxClearSurface(gcmContextData *context,u32 clear_mask);
void rsxSetClearDepthValue(gcmContextData *context,u32 value);
void rsxSetReturnCommand(gcmContextData *context);
void rsxSetCallCommand(gcmContextData *context,u32 offset);
void rsxSetJumpCommand(gcmContextData *context,u32 offset);
void rsxSetNopCommand(gcmContextData *context,u32 count);
void rsxSetClearColor(gcmContextData *context,u32 color);
void rsxSetColorMask(gcmContextData *context,u32 mask);
void rsxSetColorMaskMRT(gcmContextData *context,u32 mask);

/*! \brief Setup the render surface.

This function is used to setup the render target where RSX should render the frame into.
\param context Pointer to the context object.
\param surface Pointer to the surface object.
*/
void rsxSetSurface(gcmContextData *context,gcmSurface *surface);
void rsxSetReferenceCommand(gcmContextData *context,u32 ref_value);

/*! \brief Enqueues a Wait for label command.
\param context Pointer to the context object.
\param index Label index
\param valuie Label value
*/
void rsxSetWaitLabel(gcmContextData *context,u8 index,u32 value);

/*! \brief Enqueues a Write Command label command.
\param context Pointer to the context object.
\param index Label index
\param value Label value
*/
void rsxSetWriteCommandLabel(gcmContextData *context,u8 index,u32 value);

/*! \brief Enqueues a Write Backend label command.
\param context Pointer to the context object.
\param index Label index
\param value Label value
*/
void rsxSetWriteBackendLabel(gcmContextData *context,u8 index,u32 value);

void rsxSetViewportClip(gcmContextData *context,u8 sel,u16 width,u16 height);

/*! \brief Set viewport.

This function sets the viewport.<br/>
The origin (0,0) of the normalized device coordinate points to the center of the screen.<br/> 
Performing viewport conversion, where the upper left corner is the origin is as follows:
\code
	x = X;
	y = Y;
	width = WIDTH;
	height = HEIGHT;
	min = 0.0f;
	max = 1.0f;
	scale[0] = width * 0.5f;
	scale[1] = height * 0.5f;
	scale[2] = (max - min) * 0.5f;
	offset[0] = x + width * 0.5f;
	offset[1] = y + height * 0.5f;
	offset[2] = (max + min) * 0.5f;
\endcode
<br/><br/>
Performing viewport conversion, where the lower left corner is the origin is as follows (this is equivalent to glViewport):
\code
	x = X;
	y = WINDOW_HEIGHT - Y - HEIGHT;
	width = WIDTH;
	height = HEIGHT;
	min = 0.0f;
	max = 1.0f;
	scale[0] = width * 0.5f;
	scale[1] = height * -0.5f;
	scale[2] = (max - min) * 0.5f;
	offset[0] = x + width * 0.5f;
	offset[1] = y + height * 0.5f;
	offset[2] = (max + min) * 0.5f;
\endcode
\param context Pointer to the context object.
\param x Origin of the viewport rectangle in pixels (0 - 4095). Initial value is (0,0).
\param y Origin of the viewport rectangle in pixels (0 - 4095). Initial value is (0,0).
\param width Width of the viewport (0 - 4096). Initial value is 4096.
\param height Height of the viewport (0 - 4096). Initial value is 4096.
\param min Minimum Z clip value. Initial value is 0.0.
\param max Maximum Z clip value. Initial value is 1.0.
\param scale Scale values to be used for viewport conversion. Initial values are (2048.0,2048.0,0.5,0.0).
\param offset Offset values to be used for viewport conversion. Initial values are (2048.0,2048.0,0.5,0.0).
*/
void rsxSetViewport(gcmContextData *context,u16 x,u16 y,u16 width,u16 height,f32 min,f32 max,const f32 scale[4],const f32 offset[4]);

/*! \brief Invalidates a texture cache.
\param context Pointer to the context object.
\param type Type of texture cache to be invalidated. Possible values are:
 - \ref GCM_INVALIDATE_TEXTURE
 - \ref GCM_INVALIDATE_VERTEX_TEXTURE
*/
void rsxInvalidateTextureCache(gcmContextData *context,u32 type);

/*! \brief Loads a texture.
\param context Pointer to the context object.
\param index Texture index.
\param texture Pointer to the texture data.
*/
void rsxLoadTexture(gcmContextData *context,u8 index,const gcmTexture *texture);

/*! \brief Set texture control parameters.
\param context Pointer to the context object.
\param index Texture index.
\param enable Enable flag. Possible values are:
 - \ref GCM_TRUE
 - \ref GCM_FALSE
\param minlod minimum level of detail.
\param maxlod maximum level of detail.
\param maxaniso sample level of the anisotropic filter. Possible values are:
 - \ref GCM_TEXTURE_MAX_ANISO_1
 - \ref GCM_TEXTURE_MAX_ANISO_2
 - \ref GCM_TEXTURE_MAX_ANISO_4
 - \ref GCM_TEXTURE_MAX_ANISO_6
 - \ref GCM_TEXTURE_MAX_ANISO_8
 - \ref GCM_TEXTURE_MAX_ANISO_10
 - \ref GCM_TEXTURE_MAX_ANISO_12
 - \ref GCM_TEXTURE_MAX_ANISO_16
\todo finish args documentation.
*/
void rsxTextureControl(gcmContextData *context,u8 index,u32 enable,u16 minlod,u16 maxlod,u8 maxaniso);
void rsxTextureFilter(gcmContextData *context,u8 index,u8 min,u8 mag,u8 conv);
void rsxTextureWrapMode(gcmContextData *context,u8 index,u8 wraps,u8 wrapt,u8 wrapr,u8 unsignedRemap,u8 zfunc,u8 gamma);

/*! \brief Load a compiled vertex shader program.
\param context Pointer to the context object
\param program Pointer to the vertex program configuration
\param ucode Pointer to the shader micro code
*/
void rsxLoadVertexProgram(gcmContextData *context,rsxVertexProgram *program,const void *ucode);

/*! \brief Load a compiled fragment shader program.
\param context Pointer to the context object
\param program Pointer to the fragment program configuration
\param offset Memory offset of fragment program
\param location Memory location type where the program relies. Possible values are:
- \ref GCM_LOCATION_RSX
- \ref GCM_LOCATION_CELL
*/
void rsxLoadFragmentProgramLocation(gcmContextData *context,rsxFragmentProgram *program,u32 offset,u32 location);
void rsxZControl(gcmContextData *context,u8 cullNearFar,u8 zClampEnable,u8 cullIgnoreW);
void rsxLoadVertexProgramBlock(gcmContextData *context,rsxVertexProgram *program,const void *ucode);
void rsxLoadVertexProgramParameterBlock(gcmContextData *context,u32 base_const,u32 const_cnt,const f32 *value); 
void rsxSetVertexProgramParameter(gcmContextData *context,rsxVertexProgram *program,s32 index,const f32 *value);
void rsxSetFragmentProgramParameter(gcmContextData *context,rsxFragmentProgram *program,s32 index,const f32 *value,u32 offset);
void rsxDrawVertexArray(gcmContextData *context,u32 type,u32 start,u32 count);
void rsxBindVertexArrayAttrib(gcmContextData *context,u8 attr,u32 offset,u8 stride,u8 elems,u8 dtype,u8 location);
void rsxDrawIndexArray(gcmContextData *context,u32 type,u32 offset,u32 count,u32 data_type,u32 location);
void rsxInlineTransfer(gcmContextData *context,const u32 dstOffset,const void *srcAddress,const u32 sizeInWords,const u8 location);
void rsxSetUserClipPlaneControl(gcmContextData *context,u32 plane0,u32 plane1,u32 plane2,u32 plane3,u32 plane4,u32 plane5);
void rsxSetBlendFunc(gcmContextData *context,u16 sfcolor,u16 dfcolor,u16 sfalpha,u16 dfalpha);
void rsxSetBlendEquation(gcmContextData *context,u16 color,u16 alpha);
void rsxSetBlendColor(gcmContextData *context,u16 color0,u16 color1);
void rsxSetBlendEnable(gcmContextData *context,u32 enable);
void rsxSetTransformBranchBits(gcmContextData *context,u32 branchBits);

/*! \brief Configuration the mode for an upcoming asynchronous RSX DMA transfer.
\param context Pointer to the context object
\param mode Specify source and destination memory areas. Possible values are:
- \ref GCM_TRANSFER_LOCAL_TO_LOCAL
- \ref GCM_TRANSFER_MAIN_TO_LOCAL
- \ref GCM_TRANSFER_LOCAL_TO_MAIN
- \ref GCM_TRANSFER_MAIN_TO_MAIN
*/
void rsxSetTransferDataMode(gcmContextData *context,u8 mode);

/*! \brief Specify the memory locations for an RSX DMA transfer. This function should be called after rsxSetTransferDataMode() and rsxSetTransferDataFormat().
\param context Pointer to the context object
\param dst Destination memory offset, e.g., a value returned by gcmAddressToOffset() or gcmMapMainMemory().
\param src Source memory offset, e.g., a value returned by gcmAddressToOffset() or gcmMapMainMemory().
*/
void rsxSetTransferDataOffset(gcmContextData *context,u32 dst,u32 src);

/*! \brief Format an upcoming asynchronous RSX DMA transfer.
\param context Pointer to the context object
\param inpitch Pitch size, in bytes, of the source buffer (e.g., for a buffer that represents a rectangular image, this would be the width multiplied by the number of bytes in each pixel).
\param outpitch Pitch size, in bytes, of the destination buffer (e.g., for a buffer that represents a rectangular image, this would be the width multiplied by the number of bytes in each pixel).
\param linelength Size, in bytes, of each line of data that will be transfered.
\param linecount Number of lines of data to transfer.
\param inbytes Number of bytes for each block (e.g., pixel) of data to be transfered: 1, 2, or 4. Will perform scatter-gather transfer if different from outbytes.
\param outbytes Number of bytes for each block (e.g., pixel) of data to be transfered: 1, 2, or 4. Will perform scatter-gather transfer if different from inbytes.
*/
void rsxSetTransferDataFormat(gcmContextData *context,s32 inpitch,s32 outpitch,u32 linelength,u32 linecount,u8 inbytes,u8 outbytes);

/*! \brief Initiate an asynchronous RSX DMA transfer.
\param context Pointer to the context object
\param mode Specify source and destination memory areas. Possible values are:
- \ref GCM_TRANSFER_LOCAL_TO_LOCAL
- \ref GCM_TRANSFER_MAIN_TO_LOCAL
- \ref GCM_TRANSFER_LOCAL_TO_MAIN
- \ref GCM_TRANSFER_MAIN_TO_MAIN
\param dst Destination memory offset, e.g., a value returned by gcmAddressToOffset() or gcmMapMainMemory().
\param outpitch Pitch size, in bytes, of the destination buffer (e.g., for a buffer that represents a rectangular image, this would be the width multiplied by the number of bytes in each pixel).
\param src Source memory offset, e.g., a value returned by gcmAddressToOffset() or gcmMapMainMemory().
\param inpitch Pitch size, in bytes, of the source buffer (e.g., for a buffer that represents a rectangular image, this would be the width multiplied by the number of bytes in each pixel).
\param linelength Size, in bytes, of each line of data that will be transfered.
\param linecount Number of lines of data to transfer.
*/
void rsxSetTransferData(gcmContextData *context,u8 mode,u32 dst,u32 outpitch,u32 src,u32 inpitch,u32 linelength,u32 linecount);

/*! \brief Configure an upcoming asynchronous RSX blit.
\param context Pointer to the context object
\param mode Specify source and destination memory areas. Possible values are:
- \ref GCM_TRANSFER_LOCAL_TO_LOCAL
- \ref GCM_TRANSFER_MAIN_TO_LOCAL
- \ref GCM_TRANSFER_LOCAL_TO_MAIN
- \ref GCM_TRANSFER_MAIN_TO_MAIN
\param surface Transfer surface mode. Possible values are:
- \ref GCM_TRANSFER_SURFACE
- \ref GCM_TRANSFER_SWIZZLE
*/
void rsxSetTransferScaleMode(gcmContextData *context,const u8 mode,const u8 surface);

/*! \brief Initiate an asynchronous RSX blit.
\param context Pointer to the context object
\param scale Specify the transfer geometry & parameters.
\param surface Specify the surface to blit to.
*/
void rsxSetTransferScaleSurface(gcmContextData *context,const gcmTransferScale *scale,const gcmTransferSurface *surface);

/*! \brief Initialiate an asynchronous transfer of a rectangular image from one area of memory to another.
\param context Pointer to the context object
\param mode Specify source and destination memory areas. Possible values are:
- \ref GCM_TRANSFER_LOCAL_TO_LOCAL
- \ref GCM_TRANSFER_MAIN_TO_LOCAL
- \ref GCM_TRANSFER_LOCAL_TO_MAIN
- \ref GCM_TRANSFER_MAIN_TO_MAIN
\param dstOffset Destination memory offset, e.g., a value returned by gcmAddressToOffset() or gcmMapMainMemory().
\param dstPitch Pitch size, in bytes, of the destination image data (width multiplied by the number of bytes in each pixel).
\param dstX Origin of the destination data, relative to the beginning of the destination buffer.
\param dstY Origin of the destination data, relative to the beginning of the destination buffer.
\param srcOffset Source memory offset, e.g., a value returned by gcmAddressToOffset() or gcmMapMainMemory().
\param srcPitch Pitch size, in bytes, of the source image data (width multiplied by the number of bytes in each pixel).
\param srcX Origin of the source rectangle, relative to the beginning of the source buffer.
\param srcY Origin of the source rectangle, relative to the beginning of the source buffer.
\param width Width of the transfer rectangle.
\param height Height of the transfer rectangle.
\param bytesPerPixel Number of bytes per pixel to transfer: 2 or 4.
*/
void rsxSetTransferImage(gcmContextData *context,const u8 mode,const u32 dstOffset,const u32 dstPitch,const u32 dstX,const u32 dstY,const u32 srcOffset,const u32 srcPitch,const u32 srcX,const u32 srcY,const u32 width,const u32 height,const u32 bytesPerPixel);
void rsxSetTimeStamp(gcmContextData *context,u32 index);

#if 0
/*! \brief Unfinished 
*/
void rsxSetTransferScaleSwizzle(gcmContextData *context,const gcmTransferScale *scale,const gcmTransferSwizzle *swizzle);
#endif

#ifdef __cplusplus
	}
#endif

#endif
