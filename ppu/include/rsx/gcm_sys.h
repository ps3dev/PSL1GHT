/*! \file gcm_sys.h
\brief RSX low level management.
*/

#ifndef __GCM_SYS_H__
#define __GCM_SYS_H__

#include <ppu-types.h>

/*! \brief true boolean value */
#define GCM_TRUE								1
/*! \brief false boolean value */
#define GCM_FALSE								0

/*! \brief flip on horizontal sync, accurate mode */
#define GCM_FLIP_HSYNC							1
/*! \brief flip on vertical sync */
#define GCM_FLIP_VSYNC							2
/*! \brief flip on horizontal sync, inaccurate mode */
#define GCM_FLIP_HSYNC_AND_BREAK_EVERYTHING		3

/*! \brief maximum count of multiple render targets */
#define GCM_MAX_MRT_COUNT						4

#define GCM_DMA_MEMORY_FRAME_BUFFER				(0xFEED0000)
#define GCM_DMA_MEMORY_HOST_BUFFER				(0xFEED0001)

#define GCM_TF_COLOR_R5G5B5						3
#define GCM_TF_COLOR_X8R8G8B8					5
#define GCM_TF_COLOR_A8R8G8B8					8

/*! \brief 16-bit depth buffer */
#define GCM_TF_ZETA_Z16							1
/*! \brief 24-bit depth buffer and 8-bit stencil buffer. */
#define GCM_TF_ZETA_Z24S8						2

/*! \brief Render target is linear */
#define GCM_TF_TYPE_LINEAR						1
/*! \brief Render target is swizzled */
#define GCM_TF_TYPE_SWIZZLE						2

/*! \brief Memory buffer is located in RSX memory. */
#define GCM_LOCATION_RSX						0
/*! \brief Memory buffer is located in main memory. */
#define GCM_LOCATION_CELL						1

/*! \brief Do not use render target */
#define GCM_TF_TARGET_NONE						0
/*! \brief Render target 0 */
#define GCM_TF_TARGET_0							1
/*! \brief Render target 1 */
#define GCM_TF_TARGET_1							2
/*! \brief Render target 0 and 1 */
#define GCM_TF_TARGET_MRT1						0x13
/*! \brief Render target 0,1 and 2 */
#define GCM_TF_TARGET_MRT2						0x17
/*! \brief Render target 0,1,2 and 3 */
#define GCM_TF_TARGET_MRT3						0x1f

/*! \brief Do not use multiple samples. */
#define GCM_TF_CENTER_1							0

#define	GCM_COLOR_MASK_B						0x00000001
#define	GCM_COLOR_MASK_G						0x00000100
#define	GCM_COLOR_MASK_R						0x00010000
#define	GCM_COLOR_MASK_A						0x01000000

#define GCM_CLEAR_Z								0x01
#define GCM_CLEAR_S								0x02
#define GCM_CLEAR_R								0x10
#define GCM_CLEAR_G								0x20
#define GCM_CLEAR_B								0x40
#define GCM_CLEAR_A								0x80
#define GCM_CLEAR_M								0xf3

#define GCM_NEVER								0x0200
#define GCM_LESS								0x0201
#define GCM_EQUAL								0x0202
#define GCM_LEQUAL								0x0203
#define GCM_GREATER								0x0204
#define GCM_NOTEQUAL							0x0205
#define GCM_GEQUAL								0x0206
#define GCM_ALWAYS								0x0207

/*! \brief culling of front face */
#define GCM_CULL_FRONT							0x0404
/*! \brief culling of back face */
#define GCM_CULL_BACK							0x0405
/*! \brief culling of front and back faces */
#define GCM_CULL_ALL							0x0408

/*! \brief front face is to be drawn clock wise */
#define GCM_FRONTFACE_CW						0x0900
/*! \brief front face is to be drawn counter clock wise */
#define GCM_FRONTFACE_CCW						0x0901

/*! \brief render POINTS primitive */
#define GCM_TYPE_POINTS							1			
/*! \brief render LINES primitive */
#define GCM_TYPE_LINES							2			
/*! \brief render LINE_LOOP primitive */
#define GCM_TYPE_LINE_LOOP						3		
/*! \brief render LINE_STRIP primitive */
#define GCM_TYPE_LINE_STRIP						4		
/*! \brief render TRIANGLES primitive */
#define GCM_TYPE_TRIANGLES						5		
/*! \brief render TRIANGLE_STRIP primitive */
#define GCM_TYPE_TRIANGLE_STRIP					6	
/*! \brief render TRIANGLE_FAN primitive */
#define GCM_TYPE_TRIANGLE_FAN					7		
/*! \brief render QUADS primitive */
#define GCM_TYPE_QUADS							8			
/*! \brief render QUAD_STRIP primitive */
#define GCM_TYPE_QUAD_STRIP						9		
/*! \brief render POLYGON primitive */
#define GCM_TYPE_POLYGON						10			

/*! \brief invalidate texture cache for fragment programs */
#define GCM_INVALIDATE_TEXTURE					1
/*! \brief invalidate texture cache for vertex programs */
#define GCM_INVALIDATE_VERTEX_TEXTURE			2

/*! \brief texture is 1D. */
#define GCM_TEXTURE_DIMS_1D						1
/*! \brief texture is 2D. */
#define GCM_TEXTURE_DIMS_2D						2
/*! \brief texture is 3D. */
#define GCM_TEXTURE_DIMS_3D						3

#define GCM_TEXTURE_FORMAT_SWZ					0x00
#define GCM_TEXTURE_FORMAT_LIN					0x20
#define GCM_TEXTURE_FORMAT_NRM					0x40

#define GCM_TEXTURE_FORMAT_L8					1
#define GCM_TEXTURE_FORMAT_A1R5G5B5				2
#define GCM_TEXTURE_FORMAT_A4R4G4B4				3
#define GCM_TEXTURE_FORMAT_R5G6B5				4
#define GCM_TEXTURE_FORMAT_A8R8G8B8				5
#define GCM_TEXTURE_FORMAT_DXT1					6
#define GCM_TEXTURE_FORMAT_DXT3					7
#define GCM_TEXTURE_FORMAT_DXT5					8
#define GCM_TEXTURE_FORMAT_A8L8					24

/*! \brief shift value for texture remapping type corresponding to the blue component */
#define GCM_TEXTURE_REMAP_TYPE_B_SHIFT			14
/*! \brief shift value for texture remapping type corresponding to the green component */
#define GCM_TEXTURE_REMAP_TYPE_G_SHIFT			12
/*! \brief shift value for texture remapping type corresponding to the red component */
#define GCM_TEXTURE_REMAP_TYPE_R_SHIFT			10
/*! \brief shift value for texture remapping type corresponding to the alpha component */
#define GCM_TEXTURE_REMAP_TYPE_A_SHIFT			8

/*! \brief shift value for texture remapping component color corresponding to the blue component */
#define GCM_TEXTURE_REMAP_COLOR_B_SHIFT			6
/*! \brief shift value for texture remapping component color corresponding to the green component */
#define GCM_TEXTURE_REMAP_COLOR_G_SHIFT			4
/*! \brief shift value for texture remapping component color corresponding to the red component */
#define GCM_TEXTURE_REMAP_COLOR_R_SHIFT			2
/*! \brief shift value for texture remapping component color corresponding to the alpha component */
#define GCM_TEXTURE_REMAP_COLOR_A_SHIFT			0

/*! \brief remap component to all zero bits */
#define GCM_TEXTURE_REMAP_TYPE_ZERO				0
/*! \brief remap component to all one bits */
#define GCM_TEXTURE_REMAP_TYPE_ONE				1
/*! \brief remap component to specified component */
#define GCM_TEXTURE_REMAP_TYPE_REMAP			2

/*! \brief remap component to alpha component */
#define GCM_TEXTURE_REMAP_COLOR_A				0
/*! \brief remap component to red component */
#define GCM_TEXTURE_REMAP_COLOR_R				1
/*! \brief remap component to green component */
#define GCM_TEXTURE_REMAP_COLOR_G				2
/*! \brief remap component to blue component */
#define GCM_TEXTURE_REMAP_COLOR_B				3

/*! \brief x1 sample */
#define GCM_TEXTURE_MAX_ANISO_1					0
/*! \brief x2 sample */
#define GCM_TEXTURE_MAX_ANISO_2					1
/*! \brief x4 sample */
#define GCM_TEXTURE_MAX_ANISO_4					2
/*! \brief x6 sample */
#define GCM_TEXTURE_MAX_ANISO_6					3
/*! \brief x8 sample */
#define GCM_TEXTURE_MAX_ANISO_8					4
/*! \brief x10 sample */
#define GCM_TEXTURE_MAX_ANISO_10				5
/*! \brief x12 sample */
#define GCM_TEXTURE_MAX_ANISO_12				6
/*! \brief x16 sample */
#define GCM_TEXTURE_MAX_ANISO_16				7

#define GCM_TEXTURE_NEAREST						1
#define GCM_TEXTURE_LINEAR						2
#define GCM_TEXTURE_NEAREST_MIPMAP_NEAREST		3
#define GCM_TEXTURE_LINEAR_MIPMAP_NEAREST		4
#define GCM_TEXTURE_NEAREST_MIPMAP_LINEAR		5
#define GCM_TEXTURE_LINEAR_MIPMAP_LINEAR		6

#define GCM_TEXTURE_CONVOLUTION_QUINCUNX		1
#define GCM_TEXTURE_CONVOLUTION_GAUSSIAN		2
#define GCM_TEXTURE_CONVOLUTION_QUINCUNX_ALT	3

#define GCM_TEXTURE_REPEAT						1
#define GCM_TEXTURE_MIRRORED_REPEAT				2
#define GCM_TEXTURE_CLAMP_TO_EDGE				3
#define GCM_TEXTURE_CLAMP_TO_BORDER				4
#define GCM_TEXTURE_CLAMP						5
#define GCM_TEXTURE_MIRROR_CLAMP_TO_EDGE		6
#define GCM_TEXTURE_MIRROR_CLAMP_TO_BORDER		7
#define GCM_TEXTURE_MIRROR_CLAMP				8

#define GCM_TEXTURE_ZFUNC_NEVER					0
#define GCM_TEXTURE_ZFUNC_LESS					1
#define GCM_TEXTURE_ZFUNC_EQUAL					2
#define GCM_TEXTURE_ZFUNC_LEQUAL				3
#define GCM_TEXTURE_ZFUNC_GREATER				4
#define GCM_TEXTURE_ZFUNC_NOTEQUAL				5
#define GCM_TEXTURE_ZFUNC_GEQUAL				6
#define GCM_TEXTURE_ZFUNC_ALWAYS				7

#define GCM_VERTEX_ATTRIB_POS					0
#define GCM_VERTEX_ATTRIB_WEIGHT				1
#define GCM_VERTEX_ATTRIB_NORMAL				2
#define GCM_VERTEX_ATTRIB_COLOR0				3
#define GCM_VERTEX_ATTRIB_COLOR1				4
#define GCM_VERTEX_ATTRIB_FOG					5
#define GCM_VERTEX_ATTRIB_COLOR_INDEX			6
#define GCM_VERTEX_ATTRIB_POINT_SIZE			6	/*alias*/
#define GCM_VERTEX_ATTRIB_EDGEFLAG				7
#define GCM_VERTEX_ATTRIB_TEX0					8
#define GCM_VERTEX_ATTRIB_TEX1					9
#define GCM_VERTEX_ATTRIB_TEX2					10
#define GCM_VERTEX_ATTRIB_TEX3					11
#define GCM_VERTEX_ATTRIB_TEX4					12
#define GCM_VERTEX_ATTRIB_TEX5					13
#define GCM_VERTEX_ATTRIB_TEX6					14
#define GCM_VERTEX_ATTRIB_TEX7					15

#define GCM_VERTEX_DATA_TYPE_F32				2
#define GCM_VERTEX_DATA_TYPE_U8					4

#define GCM_INDEX_TYPE_32B						0
#define GCM_INDEX_TYPE_16B						1

#define GCM_USER_CLIP_PLANE_DISABLE				0
#define GCM_USER_CLIP_PLANE_LT					1
#define GCM_USER_CLIP_PLANE_GE					2

#define GCM_TRANSFER_SURFACE_FMT_R5G5B5			4
#define GCM_TRANSFER_SURFACE_FMT_A8R8G8B8		0xa
#define GCM_TRANSFER_SURFACE_FMT_Y32			0xb

/*! \brief Flat shading */
#define GCM_SHADE_MODEL_FLAT					0x1D00
/*! \brief Smooth shading */
#define GCM_SHADE_MODEL_SMOOTH					0x1D01

#define GCM_ZERO								0
#define GCM_ONE									1
#define GCM_SRC_COLOR							0x0300
#define GCM_ONE_MINUS_SRC_COLOR					0x0301
#define GCM_SRC_ALPHA							0x0302
#define GCM_ONE_MINUS_SRC_ALPHA					0x0303
#define GCM_DST_ALPHA							0x0304
#define GCM_ONE_MINUS_DST_ALPHA					0x0305
#define GCM_DST_COLOR							0x0306
#define GCM_ONE_MINUS_DST_COLOR					0x0307
#define GCM_SRC_ALPHA_SATURATE					0x0308
#define GCM_CONSTANT_COLOR						0x8001
#define GCM_ONE_MINUS_CONSTANT_COLOR			0x8002
#define GCM_CONSTANT_ALPHA						0x8003
#define GCM_ONE_MINUS_CONSTANT_ALPHA			0x8004

#define GCM_FUNC_ADD							0x8006
#define GCM_MIN									0x8007
#define GCM_MAX									0x8008
#define GCM_FUNC_SUBTRACT						0x800a
#define GCM_FUNC_REVERSE_SUBTRACT				0x800b
#define GCM_FUNC_REVERSE_SUBTRACT_SIGNED		0xf005
#define GCM_FUNC_ADD_SIGNED						0xf006
#define GCM_FUNC_REVERSE_ADD_SIGNED				0xf007

#ifdef __cplusplus
extern "C" {
#endif

struct _gcmCtxData;
typedef s32 (*gcmContextCallback)(struct _gcmCtxData *context,u32 count);

/*! \brief RSX Context data structure.

This structure is used for managing and controlling the command buffer.
*/
typedef struct _gcmCtxData
{
	u32 *__restrict begin ATTRIBUTE_PRXPTR;					/*!< \brief Start address of command buffer */
	u32 *__restrict end ATTRIBUTE_PRXPTR;					/*!< \brief End address of command buffer */
	u32 *__restrict current ATTRIBUTE_PRXPTR;				/*!< \brief Current address of command buffer */
	gcmContextCallback callback ATTRIBUTE_PRXPTR;			/*!< \brief Callback function that is called when <i>current</i> reaches <i>end</i> */
} gcmContextData;

/*! \brief RSX control data structure.

This structure is used to control the command buffer.
*/
typedef struct _gcmCtrlRegister
{
	vu32 put;					/*!< \brief member for accessing the PUT register */
	vu32 get;					/*!< \brief member for accessing the GET register */
	vu32 ref;					/*!< \brief member for accessing the REF register. Initial value is 0xFFFFFFFF */
} gcmControlRegister;

/*! \brief RSX Configuration structure.

This structure holds system informations of RSX.
*/
typedef struct _gcmCfg
{
	void *localAddress ATTRIBUTE_PRXPTR;			/*!< \brief effective start address of RSX memory */
	void *ioAddress ATTRIBUTE_PRXPTR;				/*!< \brief effective start address of I/O mapped main memory to be used by RSX */
	u32 localSize;									/*!< \brief maximum available size of RSX memory */
	u32 ioSize;										/*!< \brief maximum available size of I/O mapped main memory to be used by RSX */
	u32 memoryFreq;									/*!< \brief RSX memory clock frequency. */
	u32 coreFreq;									/*!< \brief Core clock frequency of RSX */
} gcmConfiguration;

/*! \brief RSX target surface data structure.

This structure holds settings of the render target that is to be the render buffer.
Set the buffer to use for rendering by passing this structure as the argument when calling \ref rsxSetSurface. */
typedef struct _gcmSurface
{
	/*! \brief Type of render target.

	Possible values are:
	- \ref GCM_TF_TYPE_LINEAR
	- \ref GCM_TF_TYPE_SWIZZLE
	*/
	u8 type;

	/*! \brief Antialiasing format type.

	Specifies the mode of multiple samples. Possible values are:
	- \ref GCM_TF_CENTER_1
	*/
	u8 antiAlias;

	/*! \brief Format of the color buffer.

	Possible values are:
	- \ref GCM_TF_COLOR_R5G5B5
	*/
	u8 colorFormat;

	/*! \brief Target of the color buffer.

	Specifies the render target to use as a surface. Possible values are:
	- \ref GCM_TF_TARGET_NONE
	- \ref GCM_TF_TARGET_0
	- \ref GCM_TF_TARGET_1
	- \ref GCM_TF_TARGET_MRT1
	- \ref GCM_TF_TARGET_MRT2
	- \ref GCM_TF_TARGET_MRT3
	*/
	u8 colorTarget;

	/*! \brief Location of the color buffer.

	When using multiple render targets, set as many locations as the number of color buffers enabled in <i>colorTarget</i>.
	In this system, up to 4 color buffers can be specified for multiple render targets, and the location of each individual color buffer can be specified independently.
	Possible values are:
	- \ref GCM_LOCATION_RSX
	- \ref GCM_LOCATION_CELL
	*/
	u8 colorLocation[GCM_MAX_MRT_COUNT];

	/*! \brief Offset from the base address of the color buffer.

	When using multiple render targets, set as many addresses as the number of color buffers specified in <i>colorTarget</i>.
	Use \ref rsxAddressToOffset to convert the effective addresses into offset values when specifying the buffer offset. <i>colorOffset</i> should be aligned on a 64 bytes boundery.
	*/
	u32 colorOffset[GCM_MAX_MRT_COUNT];

	/*! \brief Size of a color buffer line in bytes.

	When using multiple render targets, specify as many pitch sizes as the number of color buffers specified in <i>colorTarget</i>.
	The pitch size should be 64 when rendering in the swizzle format. For all others, the pitch size should be a multiple of 64.
	*/
	u32 colorPitch[GCM_MAX_MRT_COUNT];

	/*! \brief Format of the depth buffer.

	Possible values are:
	- \ref GCM_TF_ZETA_Z16
	- \ref GCM_TF_ZETA_Z24S8
	*/
	u8 depthFormat;

	/*! \brief Location of the depth buffer.

	Possible values are:
	- \ref GCM_LOCATION_RSX
	- \ref GCM_LOCATION_CELL
	*/
	u8 depthLocation;

	/*! \brief unused padding bytes. most be 0. */
	u8 _pad[2];

	/*! \brief Offset from the base address of the depth buffer.

	As in <i>colorOffset</i> use \ref rsxAddressToOffset to convert effective addresses into offset values. <i>depthOffset</i> should be aligned on a 64 bytes boundery.
	*/
	u32 depthOffset;

	/*! \brief Size of a depth buffer line in bytes. */
	u32 depthPitch;

	/*! \brief Width of the render buffer (1 - 4096). */
	u16 width;

	/*! \brief Height of the render buffer (1 - 4096). */
	u16 height;

	/*! \brief Window offset in x direction (0 - 4095). */
	u16 x;

	/*! \brief Window offset in y direction (0 - 4095). */
	u16 y;
} gcmSurface;

/*! \brief RSX Texture data structure. */
typedef struct _gcmTexture
{
	/*! \brief Texture format.
	
	This is an OR-ed combination of the following values:
	- \ref GCM_TEXTURE_FORMAT_SWZ
	- \ref GCM_TEXTURE_FORMAT_LIN
	- \ref GCM_TEXTURE_FORMAT_NRM
	- \ref GCM_TEXTURE_FORMAT_L8
	- \ref GCM_TEXTURE_FORMAT_A1R5G5B5
	- \ref GCM_TEXTURE_FORMAT_A4R4G4B4
	- \ref GCM_TEXTURE_FORMAT_R5G6B5
	- \ref GCM_TEXTURE_FORMAT_A8R8G8B8
	- \ref GCM_TEXTURE_FORMAT_DXT1
	- \ref GCM_TEXTURE_FORMAT_DXT3
	- \ref GCM_TEXTURE_FORMAT_DXT5
	- \ref GCM_TEXTURE_FORMAT_A8L8
	*/
	u8 format;

	/*! \brief Indicates if this is a mip-mapped texture.
	
	Possible values are:
	- \ref GCM_TRUE
	- \ref GCM_FALSE
	*/
	u8 mipmap;

	/*! \brief Texture dimension.
	
	Possible values are:
	- \ref GCM_TEXTURE_DIMS_1D
	- \ref GCM_TEXTURE_DIMS_2D
	- \ref GCM_TEXTURE_DIMS_3D
	*/
	u8 dimension;

	/*! \brief Indicates if this is a cube-mapped texture.
	
	Possible values are:
	- \ref GCM_TRUE
	- \ref GCM_FALSE
	*/
	u8 cubemap;

	/*! \brief Color remapping bitfield.
	
	Each of the texture color components (red, green, blue, alpha) can be
	remapped according to a specified remapping type. The type specifies
	that the component is either set to zero, all one bits, or takes value
	of one of the source components. All remapping types and values are to
	be OR-ed together.

    For instance, to have the alpha component set to zero, the red and blue
	components swapped and the green component kept as-is, set the following
	value:

    (\ref GCM_TEXTURE_REMAP_TYPE_ZERO << \ref GCM_TEXTURE_REMAP_TYPE_A_SHIFT)\n
	| (\ref GCM_TEXTURE_REMAP_TYPE_REMAP << \ref GCM_TEXTURE_REMAP_TYPE_R_SHIFT)\n
	| (\ref GCM_TEXTURE_REMAP_COLOR_B << \ref GCM_TEXTURE_REMAP_COLOR_R_SHIFT)\n
    | (\ref GCM_TEXTURE_REMAP_TYPE_REMAP << \ref GCM_TEXTURE_REMAP_TYPE_G_SHIFT)\n
	| (\ref GCM_TEXTURE_REMAP_COLOR_G << \ref GCM_TEXTURE_REMAP_COLOR_G_SHIFT)\n
    | (\ref GCM_TEXTURE_REMAP_TYPE_REMAP << \ref GCM_TEXTURE_REMAP_TYPE_B_SHIFT)\n
	| (\ref GCM_TEXTURE_REMAP_COLOR_R << \ref GCM_TEXTURE_REMAP_COLOR_B_SHIFT)
	*/
	u32 remap;

	/*! \brief Texture width in pixels. */
	u16 width;
	/*! \brief Texture height in pixels. */
	u16 height;
	/*! \brief Texture depth. */
	u16 depth;
	/*! \brief Location of texture.

	Possible values are:
	- \ref GCM_LOCATION_RSX
	- \ref GCM_LOCATION_CELL
	*/
	u8 location;
	/*! \brief unused padding byte. */
	u8 _pad;
	/*! \brief Size of a texture line in bytes. */
	u32 pitch;
	/*! \brief Offset of texture data. */
	u32 offset;
} gcmTexture;

/*! \brief Initialize the RSX context.

\param ctx Pointer to where the effective address of the allocated context
 structure will be stored.
\param cmdSize The command buffer size.
\param ioSize The allocated IO buffer size.
\param ioAddress Pointer to an allocated buffer of \p ioSize bytes.
\return zero if no error occured, nonzero otherwise.
*/
s32 gcmInitBody(gcmContextData* ATTRIBUTE_PRXPTR *ctx,const u32 cmdSize,const u32 ioSize,const void *ioAddress);

/*! \brief Converts an effective address in RSX memory to an offset.
\param address The effective address to be converted.
\param offset A pointer to the returned offset value.
\return zero if no error occured, nonzero otherwise.
*/
s32 gcmAddressToOffset(void *address,u32 *offset);

/*! \brief Converts an offset to an effective address in RSX memory.
\param offset The offset to be converted.
\param address A pointer to the returned effective address.
\return zero if no error occured, nonzero otherwise.
*/
s32 gcmIoOffsetToAddress(u32 offset,u32 *address);

/*! \brief Retrieves the RSX configuration.
\param config A pointer to the configuration structure to be updated.
\return zero if no error occured, nonzero otherwise.
*/
s32 gcmGetConfiguration(gcmConfiguration *config);

/*! \brief Gets the flip status.

Once a flip occurred, querying for a subsequent flip requires the flip status
to be reset using \ref gcmResetFlipStatus.
\return zero if a flip occured, nonzero otherwise.
*/
s32 gcmGetFlipStatus();

/*! \brief Enqueues a flip command into the command buffer.
\param context Pointer to the context object.
\param bufferId Framebuffer id to flip to (as configured with
   \ref gcmSetDisplayBuffer).
\return zero if no error occured, nonzero otherwise.
*/
s32 gcmSetFlip(gcmContextData *context,u32 bufferId);

/*! \brief Configures a display framebuffer.
\param bufferId The buffer id (0-7).
\param offset The offset of the allocated memory block (see \ref rsxAddressToOffset).
\param pitch The size of a buffer line in bytes.
\param width The buffer width in pixels.
\param height The buffer height in pixels.
\return zero if no error occured, nonzero otherwise.
*/
s32 gcmSetDisplayBuffer(u32 bufferId,u32 offset,u32 pitch,u32 width,u32 height);

/*! \brief Maps a memory block in main memory for RSX to access it.
\param address Pointer to the block to be mapped.
\param size Size of the block in bytes.
\param offset A pointer to the returned mapped offset value.
\return zero if no error occured, nonzero otherwise.
*/
s32 gcmMapMainMemory(const void *address,const u32 size,u32 *offset);

/*! \brief Get address of specified label.
\param index The label index whose address is to be obtained.
\return Pointer to the label address.
*/
u32* gcmGetLabelAddress(const u8 index);

/*! \brief Reset the flip status. */
void gcmResetFlipStatus();

/*! \brief Set flip mode.
\param mode The specified flip mode. Possible vales are:
 - \ref GCM_FLIP_HSYNC
 - \ref GCM_FLIP_VSYNC
 - \ref GCM_FLIP_HSYNC_AND_BREAK_EVERYTHING
*/
void gcmSetFlipMode(s32 mode);

/*! \brief Wait for a flip to be completed.
\param context Pointer to the context object.
*/
void gcmSetWaitFlip(gcmContextData *context);
void gcmSetVBlankHandler(void (*handler)(const u32 head));
void gcmSetFlipHandler(void (*handler)(const u32 head));
void gcmSetGraphicsHandler(void (*handler)(const u32 val));
void gcmSetDefaultCommandBuffer();
gcmControlRegister* gcmGetControlRegister();

#ifdef __cplusplus
	}
#endif

#endif
