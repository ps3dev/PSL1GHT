/*! \file cellGcm_sys.h
\brief RSX low level management.
*/

#ifndef __CELL_GCM_SYS_H__
#define __CELL_GCM_SYS_H__

#include <ppu-types.h>

/*! \brief true boolean value */
#define CELL_GCM_TRUE									1
/*! \brief false boolean value */
#define CELL_GCM_FALSE									0

/*! \brief Traditional FIFO mode. Checks the <i>get</i> pointer in the traditional 
           way for any opening in the next segment. May sleep-wait if the <i>get</i>
  		   pointer is pointing to the next segment.
*/
#define CELL_GCM_DEFAULT_FIFO_MODE_TRADITIONAL			0

/* \brief Optimized FIFO mode. Uses the \ref rsxSetWriteTextureLable() to manage the
          command buffer. It handles each segment buffer as an index and monitors
          states of the segment buffers by reading the label value executed
          by \ref rsxSetWriteTextureLabel().
*/
#define CELL_GCM_DEFAULT_FIFO_MODE_OPTIMIZE				1

/* \brief Conditional FIFO mode. Uses the \ref rsxSetWriteCommandLabel() to manage the
  		  the command buffer. The management of the command buffer is carried out in
  		  the same manner as \ref CELL_GCM_DEFAULT_FIFO_MODE_OPTIMIZE.
*/ 
#define CELL_GCM_DEFAULT_FIFO_MODE_CONDITIONAL			2

/*! \brief flip on horizontal sync, accurate mode */
#define CELL_GCM_FLIP_HSYNC								1
/*! \brief flip on vertical sync */
#define CELL_GCM_FLIP_VSYNC								2
/*! \brief flip on horizontal sync, inaccurate mode */
#define CELL_GCM_FLIP_HSYNC_AND_BREAK_EVERYTHING			3

/*! \brief maximum count of multiple render targets */
#define CELL_GCM_MAX_MRT_COUNT							4

#define CELL_GCM_DMA_MEMORY_FRAME_BUFFER					(0xFEED0000)
#define CELL_GCM_DMA_MEMORY_HOST_BUFFER					(0xFEED0001)

#define CELL_GCM_CONTEXT_SURFACE2D						(0x313371C3)
#define CELL_GCM_CONTEXT_SWIZZLE2D						(0x31337A73)

#define CELL_GCM_TRANSFER_LOCAL_TO_LOCAL					0
#define CELL_GCM_TRANSFER_MAIN_TO_LOCAL					1
#define CELL_GCM_TRANSFER_LOCAL_TO_MAIN					2
#define CELL_GCM_TRANSFER_MAIN_TO_MAIN					3

/*! \brief Memory buffer is located in RSX memory. */
#define CELL_GCM_LOCATION_RSX							0
/*! \brief Memory buffer is located in main memory. */
#define CELL_GCM_LOCATION_CELL							1
/*! \brief Memory buffer is located in report memory. */
#define CELL_GCM_LOCATION_REPORT							2

#define CELL_GCM_SURFACE_X1R5G5B5_Z1R5G5B5				1
#define CELL_GCM_SURFACE_X1R5G5B5_O1R5G5B5				2
#define CELL_GCM_SURFACE_R5G5B5							3
#define CELL_GCM_SURFACE_X8R8G8B8_Z8R8G8B8				4
#define CELL_GCM_SURFACE_X8R8G8B8						5
#define CELL_GCM_SURFACE_A8R8G8B8						8
#define CELL_GCM_SURFACE_B8								9
#define CELL_GCM_SURFACE_G8B8							10
#define CELL_GCM_SURFACE_F_W16Z16Y16X16					11
#define CELL_GCM_SURFACE_F_W32Z32Y32X32					12
#define CELL_GCM_SURFACE_F_X32							13
#define CELL_GCM_SURFACE_X8B8G8R8_Z8B8G8R8				14
#define CELL_GCM_SURFACE_X8B8G8R8_O8B8G8R8				15
#define CELL_GCM_SURFACE_A8B8G8R8						16

/*! \brief 16-bit depth buffer */
#define CELL_GCM_SURFACE_ZETA_Z16						1
/*! \brief 24-bit depth buffer and 8-bit stencil buffer. */
#define CELL_GCM_SURFACE_ZETA_Z24S8						2

/*! \brief Render target is linear */
#define CELL_GCM_SURFACE_TYPE_LINEAR						1
/*! \brief Render target is swizzled */
#define CELL_GCM_SURFACE_TYPE_SWIZZLE					2

/*! \brief Do not use render target */
#define CELL_GCM_SURFACE_TARGET_NONE						0
/*! \brief Render target 0 */
#define CELL_GCM_SURFACE_TARGET_0						1
/*! \brief Render target 1 */
#define CELL_GCM_SURFACE_TARGET_1						2
/*! \brief Render target 0 and 1 */
#define CELL_GCM_SURFACE_TARGET_MRT1						0x13
/*! \brief Render target 0,1 and 2 */
#define CELL_GCM_SURFACE_TARGET_MRT2						0x17
/*! \brief Render target 0,1,2 and 3 */
#define CELL_GCM_SURFACE_TARGET_MRT3						0x1f

/*! \brief Do not use multiple samples. */
#define CELL_GCM_SURFACE_CENTER_1						0
#define CELL_GCM_SURFACE_DIAGONAL_CENTERED_2				3
#define CELL_GCM_SURFACE_SQUARE_CENTERED_4				4
#define CELL_GCM_SURFACE_SQUARE_ROTATED_4				5

/*! \brief blue color component */
#define	CELL_GCM_COLOR_MASK_B							0x00000001
/*! \brief green color component */
#define	CELL_GCM_COLOR_MASK_G							0x00000100
/*! \brief red color component */
#define	CELL_GCM_COLOR_MASK_R							0x00010000
/*! \brief alpha component */
#define	CELL_GCM_COLOR_MASK_A							0x01000000

#define CELL_GCM_COLOR_MASK_MRT1_A						0x00000010
#define CELL_GCM_COLOR_MASK_MRT1_R						0x00000020
#define CELL_GCM_COLOR_MASK_MRT1_G						0x00000040
#define CELL_GCM_COLOR_MASK_MRT1_B						0x00000080
#define CELL_GCM_COLOR_MASK_MRT2_A						0x00000100
#define CELL_GCM_COLOR_MASK_MRT2_R						0x00000200
#define CELL_GCM_COLOR_MASK_MRT2_G						0x00000400
#define CELL_GCM_COLOR_MASK_MRT2_B						0x00000800
#define CELL_GCM_COLOR_MASK_MRT3_A						0x00001000
#define CELL_GCM_COLOR_MASK_MRT3_R						0x00002000
#define CELL_GCM_COLOR_MASK_MRT3_G						0x00004000
#define CELL_GCM_COLOR_MASK_MRT3_B						0x00008000

/*! \brief clear the Z buffer (depth buffer) */
#define CELL_GCM_CLEAR_Z									0x01
/*! \brief clear the stencil buffer */
#define CELL_GCM_CLEAR_S									0x02
/*! \brief clear the red components */
#define CELL_GCM_CLEAR_R									0x10
/*! \brief clear the green components */
#define CELL_GCM_CLEAR_G									0x20
/*! \brief clear the blue components */
#define CELL_GCM_CLEAR_B									0x40
/*! \brief clear the alpha components */
#define CELL_GCM_CLEAR_A									0x80
/*! \brief clear all RGBA components, Z buffer and stencil buffer */
#define CELL_GCM_CLEAR_M									0xf3

/*! \brief depth test never passes. */
#define CELL_GCM_NEVER									0x0200
/*! \brief depth test passes if the incoming depth value is less than the stored depth value. */
#define CELL_GCM_LESS									0x0201
/*! \brief depth test passes if the incoming depth value is equal to the stored depth value. */
#define CELL_GCM_EQUAL									0x0202
/*! \brief depth test passes if the incoming depth value is less than or equal to the stored depth value. */
#define CELL_GCM_LEQUAL									0x0203
/*! \brief depth test passes if the incoming depth value is greater than the stored depth value. */
#define CELL_GCM_GREATER									0x0204
/*! \brief depth test passes if the incoming depth value is not equal to the stored depth value. */
#define CELL_GCM_NOTEQUAL								0x0205
/*! \brief depth test passes if the incoming depth value is greater than or equal to the stored depth value. */
#define CELL_GCM_GEQUAL									0x0206
/*! \brief depth test always passes. */
#define CELL_GCM_ALWAYS									0x0207

/*! \brief culling of front face */
#define CELL_GCM_CULL_FRONT								0x0404
/*! \brief culling of back face */
#define CELL_GCM_CULL_BACK								0x0405
/*! \brief culling of front and back faces */
#define CELL_GCM_CULL_ALL								0x0408

/*! \brief render the polygon by points */
#define CELL_GCM_POLYGON_MODE_POINT						0x1b00
/*! \brief render the polygon by lines */
#define CELL_GCM_POLYGON_MODE_LINE						0x1b01
/*! \brief render the polygon by filling */
#define CELL_GCM_POLYGON_MODE_FILL						0x1b02

/*! \brief front face is to be drawn clock wise */
#define CELL_GCM_FRONTFACE_CW							0x0900
/*! \brief front face is to be drawn counter clock wise */
#define CELL_GCM_FRONTFACE_CCW							0x0901

#define CELL_GCM_CLEAR									0x1500
#define CELL_GCM_AND										0x1501
#define CELL_GCM_AND_REVERSE								0x1502
#define CELL_GCM_COPY									0x1503
#define CELL_GCM_AND_INVERTED							0x1504
#define CELL_GCM_NOOP									0x1505
#define CELL_GCM_XOR										0x1506
#define CELL_GCM_OR										0x1507
#define CELL_GCM_NOR										0x1508
#define CELL_GCM_EQUIV									0x1509
#define CELL_GCM_INVERT									0x150A
#define CELL_GCM_OR_REVERSE								0x150B
#define CELL_GCM_COPY_INVERTED							0x150C
#define CELL_GCM_OR_INVERTED								0x150D
#define CELL_GCM_NAND									0x150E
#define CELL_GCM_SET										0x150F

/*! \brief keep current stencil buffer value */
#define CELL_GCM_KEEP									0x1E00
/*! \brief set stencil buffer value to 0 */
#define CELL_GCM_REPLACE									0x1E01
/*! \brief increment current stencil buffer value. clamp to 255 */
#define CELL_GCM_INCR									0x1E02
/*! \brief decrement current stencil buffer value. clamp to 0 */
#define CELL_GCM_DECR									0x1E03
/*! \brief increment current stencil buffer value. when incrementing a stencil value of 255, wrap around to 0 */
#define CELL_GCM_INCR_WRAP								0x8507
/*! \brief decrement current stencil buffer value. when decrementing a stencil value of 0, wrap around to 255 */
#define CELL_GCM_DECR_WRAP								0x8508

/*! \brief render POINTS primitive */
#define CELL_GCM_TYPE_POINTS								1			
/*! \brief render LINES primitive */
#define CELL_GCM_TYPE_LINES								2			
/*! \brief render LINE_LOOP primitive */
#define CELL_GCM_TYPE_LINE_LOOP							3		
/*! \brief render LINE_STRIP primitive */
#define CELL_GCM_TYPE_LINE_STRIP							4		
/*! \brief render TRIANGLES primitive */
#define CELL_GCM_TYPE_TRIANGLES							5		
/*! \brief render TRIANGLE_STRIP primitive */
#define CELL_GCM_TYPE_TRIANGLE_STRIP						6	
/*! \brief render TRIANGLE_FAN primitive */
#define CELL_GCM_TYPE_TRIANGLE_FAN						7		
/*! \brief render QUADS primitive */
#define CELL_GCM_TYPE_QUADS								8			
/*! \brief render QUAD_STRIP primitive */
#define CELL_GCM_TYPE_QUAD_STRIP							9		
/*! \brief render POLYGON primitive */
#define CELL_GCM_TYPE_POLYGON							10			

/*! \brief invalidate texture cache for fragment programs */
#define CELL_GCM_INVALIDATE_TEXTURE						1
/*! \brief invalidate texture cache for vertex programs */
#define CELL_GCM_INVALIDATE_VERTEX_TEXTURE				2

/*! \brief texture is 1D. */
#define CELL_GCM_TEXTURE_DIMS_1D							1
/*! \brief texture is 2D. */
#define CELL_GCM_TEXTURE_DIMS_2D							2
/*! \brief texture is 3D. */
#define CELL_GCM_TEXTURE_DIMS_3D							3

/*! \brief texture uses swizzle format */
#define CELL_GCM_TEXTURE_FORMAT_SWZ						0x00
/*! \brief texture uses linear format */
#define CELL_GCM_TEXTURE_FORMAT_LIN						0x20
/*! \brief texture uses normalized texture coordinates */
#define CELL_GCM_TEXTURE_FORMAT_NRM						0x00
/*! \brief texture uses unnormalized texture coordinates */
#define CELL_GCM_TEXTURE_FORMAT_UNRM						0x40

/*! \brief texture color format is 8-bit unsigned integer */
#define CELL_GCM_TEXTURE_FORMAT_B8						1
/*! \brief texture color format is 1-bit alpha and three 5-bit unsigned integers */
#define CELL_GCM_TEXTURE_FORMAT_A1R5G5B5					2
/*! \brief texture color format is four 4-bit unsigned values */
#define CELL_GCM_TEXTURE_FORMAT_A4R4G4B4					3
/*! \brief texture color format is 5-bit, 6-bit and 5-bit unsigned integers */
#define CELL_GCM_TEXTURE_FORMAT_R5G6B5					4
/*! \brief texture color format is four 8-bit unsigned integers */
#define CELL_GCM_TEXTURE_FORMAT_A8R8G8B8					5
/*! \brief texture color format is 4x4 pixels compressed to 8 bytes */
#define CELL_GCM_TEXTURE_FORMAT_DXT1						6
/*! \brief texture color format is 4x4 pixels compressed to 16 bytes */
#define CELL_GCM_TEXTURE_FORMAT_DXT23					7
/*! \brief texture color format is 4x4 pixels compressed to 16 bytes */
#define CELL_GCM_TEXTURE_FORMAT_DXT45					8
/*! \brief texture color format is two 8-bit unsigned integers */
#define CELL_GCM_TEXTURE_FORMAT_G8B8						11
/*! \brief texture color format is 6-bit, 5-bit and 5-bit unsigned integers */
#define CELL_GCM_TEXTURE_FORMAT_R6G5B5					15
/*! \brief texture color format is 24-bit fixed and 8-bit dummy data */
#define CELL_GCM_TEXTURE_FORMAT_DEPTH24_D8				16
/*! \brief texture color format is 24-bit float and 8-bit dummy data */
#define CELL_GCM_TEXTURE_FORMAT_DEPTH24_D8_FLOAT			17
/*! \brief texture color format is 16-bit fixed */
#define CELL_GCM_TEXTURE_FORMAT_DEPTH16					18
/*! \brief texture color format is 16-bit float */
#define CELL_GCM_TEXTURE_FORMAT_DEPTH16_FLOAT			19
/*! \brief texture color format is 16-bit integer */
#define CELL_GCM_TEXTURE_FORMAT_X16						20
/*! \brief texture color format is two 16-bit integers */
#define CELL_GCM_TEXTURE_FORMAT_Y16_X16					21
/*! \brief texture color format is three 5-bit unsigned integers and one 1-bit value */
#define CELL_GCM_TEXTURE_FORMAT_R5G6B5A1					23
/*! \brief texture color format is two 16-bit unsigned values compressed to two 8-bit values */
#define CELL_GCM_TEXTURE_FORMAT_COMPRESSED_HILO8			24
/*! \brief texture color format is two 16-bit signed values compressed to two 8-bit values */
#define CELL_GCM_TEXTURE_FORMAT_COMPRESSED_HILO8_S8		25
/*! \brief texture color format is four 16-bit float values */
#define CELL_GCM_TEXTURE_FORMAT_W16_Z16_Y16_X16_FLOAT	26
/*! \brief texture color format is four 32-bit float values */
#define CELL_GCM_TEXTURE_FORMAT_W32_Z32_Y32_X32_FLOAT	27
/*! \brief texture color format is one 32-bit float value */
#define CELL_GCM_TEXTURE_FORMAT_X32_FLOAT				28
/*! \brief texture color format is 1-bit dummy data and 5-bit unsigned integers */
#define CELL_GCM_TEXTURE_FORMAT_D1R5G5B5					29
/*! \brief texture color format is 8-bit dummy data and 8-bit unsigned integers */
#define CELL_GCM_TEXTURE_FORMAT_D8R8G8B8					30
/*! \brief texture color format is two 16-bit float values */
#define CELL_GCM_TEXTURE_FORMAT_Y16_X16_FLOAT			31
/*! \brief texture color format is two pixels compressed to 32 bits in YUV format */
#define CELL_GCM_TEXTURE_FORMAT_COMPRESSED_B8R8_G8R8		45
/*! \brief texture color format is two pixels compressed to 32 bits in YUV format */
#define CELL_GCM_TEXTURE_FORMAT_COMPRESSED_R8B8_R8G8		46


/*! \brief shift value for texture remapping type corresponding to the blue component */
#define CELL_GCM_TEXTURE_REMAP_TYPE_B_SHIFT				14
/*! \brief shift value for texture remapping type corresponding to the green component */
#define CELL_GCM_TEXTURE_REMAP_TYPE_G_SHIFT				12
/*! \brief shift value for texture remapping type corresponding to the red component */
#define CELL_GCM_TEXTURE_REMAP_TYPE_R_SHIFT				10
/*! \brief shift value for texture remapping type corresponding to the alpha component */
#define CELL_GCM_TEXTURE_REMAP_TYPE_A_SHIFT				8

/*! \brief shift value for texture remapping component color corresponding to the blue component */
#define CELL_GCM_TEXTURE_REMAP_COLOR_B_SHIFT				6
/*! \brief shift value for texture remapping component color corresponding to the green component */
#define CELL_GCM_TEXTURE_REMAP_COLOR_G_SHIFT				4
/*! \brief shift value for texture remapping component color corresponding to the red component */
#define CELL_GCM_TEXTURE_REMAP_COLOR_R_SHIFT				2
/*! \brief shift value for texture remapping component color corresponding to the alpha component */
#define CELL_GCM_TEXTURE_REMAP_COLOR_A_SHIFT				0

/*! \brief remap component to all zero bits */
#define CELL_GCM_TEXTURE_REMAP_TYPE_ZERO					0
/*! \brief remap component to all one bits */
#define CELL_GCM_TEXTURE_REMAP_TYPE_ONE					1
/*! \brief remap component to specified component */
#define CELL_GCM_TEXTURE_REMAP_TYPE_REMAP				2

/*! \brief map the two input elements XY to XYZW as XYXY */
#define CELL_GCM_TEXTURE_REMAP_ORDER_XYXY				0
/*! \brief map the two input elements XY to XYZW as XXXY */
#define CELL_GCM_TEXTURE_REMAP_ORDER_XXXY				1
/*! \brief remap component to alpha component */
#define CELL_GCM_TEXTURE_REMAP_COLOR_A					0
/*! \brief remap component to red component */
#define CELL_GCM_TEXTURE_REMAP_COLOR_R					1
/*! \brief remap component to green component */
#define CELL_GCM_TEXTURE_REMAP_COLOR_G					2
/*! \brief remap component to blue component */
#define CELL_GCM_TEXTURE_REMAP_COLOR_B					3

/*! \brief x1 sample */
#define CELL_GCM_TEXTURE_MAX_ANISO_1						0
/*! \brief x2 sample */
#define CELL_GCM_TEXTURE_MAX_ANISO_2						1
/*! \brief x4 sample */
#define CELL_GCM_TEXTURE_MAX_ANISO_4						2
/*! \brief x6 sample */
#define CELL_GCM_TEXTURE_MAX_ANISO_6						3
/*! \brief x8 sample */
#define CELL_GCM_TEXTURE_MAX_ANISO_8						4
/*! \brief x10 sample */
#define CELL_GCM_TEXTURE_MAX_ANISO_10					5
/*! \brief x12 sample */
#define CELL_GCM_TEXTURE_MAX_ANISO_12					6
/*! \brief x16 sample */
#define CELL_GCM_TEXTURE_MAX_ANISO_16					7

#define CELL_GCM_FOG_MODE_LINEAR							0x2601
#define CELL_GCM_FOG_MODE_EXP							0x0800
#define CELL_GCM_FOG_MODE_EXP2							0x0801
#define CELL_GCM_FOG_MODE_EXP_ABS						0x0802
#define CELL_GCM_FOG_MODE_EXP2_ABS						0x0803
#define CELL_GCM_FOG_MODE_LINEAR_ABS						0x0804

#define CELL_GCM_POINT_SPRITE_TEX0						(1<<8)
#define CELL_GCM_POINT_SPRITE_TEX1						(1<<9)
#define CELL_GCM_POINT_SPRITE_TEX2						(1<<10)
#define CELL_GCM_POINT_SPRITE_TEX3						(1<<11)
#define CELL_GCM_POINT_SPRITE_TEX4						(1<<12)
#define CELL_GCM_POINT_SPRITE_TEX5						(1<<13)
#define CELL_GCM_POINT_SPRITE_TEX6						(1<<14)
#define CELL_GCM_POINT_SPRITE_TEX7						(1<<15)
#define CELL_GCM_POINT_SPRITE_TEX8						(1<<16)
#define CELL_GCM_POINT_SPRITE_TEX9						(1<<17)

#define CELL_GCM_POINT_SPRITE_RMODE_ZERO					0
#define CELL_GCM_POINT_SPRITE_RMODE_FROM_R				1
#define CELL_GCM_POINT_SPRITE_RMODE_FROM_S				2

#define CELL_GCM_TEXTURE_NEAREST							1
#define CELL_GCM_TEXTURE_LINEAR							2
#define CELL_GCM_TEXTURE_NEAREST_MIPMAP_NEAREST			3
#define CELL_GCM_TEXTURE_LINEAR_MIPMAP_NEAREST			4
#define CELL_GCM_TEXTURE_NEAREST_MIPMAP_LINEAR			5
#define CELL_GCM_TEXTURE_LINEAR_MIPMAP_LINEAR			6
#define CELL_GCM_TEXTURE_CONVOLUTION_MIN					7
#define CELL_GCM_TEXTURE_CONVOLUTION_MAG					4

#define CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX			1
#define CELL_GCM_TEXTURE_CONVOLUTION_GAUSSIAN			2
#define CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX_ALT		3

#define CELL_GCM_TEXTURE_REPEAT							1
#define CELL_GCM_TEXTURE_MIRRORED_REPEAT					2
#define CELL_GCM_TEXTURE_CLAMP_TO_EDGE					3
#define CELL_GCM_TEXTURE_BORDER							4
#define CELL_GCM_TEXTURE_CLAMP							5
#define CELL_GCM_TEXTURE_MIRROR_ONCE_CLAMP_TO_EDGE		6
#define CELL_GCM_TEXTURE_MIRROR_ONCE_CLAMP_TO_BORDER		7
#define CELL_GCM_TEXTURE_MIRROR_ONCE_CLAMP				8

#define CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL			0
#define CELL_GCM_TEXTURE_UNSIGNED_REMAP_BIASED			1

#define CELL_GCM_TEXTURE_GAMMA_R							(1<<0)
#define CELL_GCM_TEXTURE_GAMMA_G							(1<<1)
#define CELL_GCM_TEXTURE_GAMMA_B							(1<<2)
#define CELL_GCM_TEXTURE_GAMMA_A							(1<<3)

#define CELL_GCM_TEXTURE_ZFUNC_NEVER						0
#define CELL_GCM_TEXTURE_ZFUNC_LESS						1
#define CELL_GCM_TEXTURE_ZFUNC_EQUAL						2
#define CELL_GCM_TEXTURE_ZFUNC_LEQUAL					3
#define CELL_GCM_TEXTURE_ZFUNC_GREATER					4
#define CELL_GCM_TEXTURE_ZFUNC_NOTEQUAL					5
#define CELL_GCM_TEXTURE_ZFUNC_GEQUAL					6
#define CELL_GCM_TEXTURE_ZFUNC_ALWAYS					7

#define CELL_GCM_TEXTURE_ISO_LOW							0
#define CELL_GCM_TEXTURE_ISO_HIGH						1

#define CELL_GCM_TEXTURE_ANISO_LOW						0
#define CELL_GCM_TEXTURE_ANISO_HIGH						1

#define CELL_GCM_DEPTH_FORMAT_FIXED						0
#define CELL_GCM_DEPTH_FORMAT_FLOAT						1

#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX0_U	(1<<0)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX0_V	(1<<1)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX0_P	(1<<2)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX0_Q	(1<<3)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX1_U	(1<<4)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX1_V	(1<<5)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX1_P	(1<<6)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX1_Q	(1<<7)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX2_U	(1<<8)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX2_V	(1<<9)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX2_P	(1<<10)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX2_Q	(1<<11)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX3_U	(1<<12)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX3_V	(1<<13)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX3_P	(1<<14)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX3_Q	(1<<15)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX4_U	(1<<16)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX4_V	(1<<17)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX4_P	(1<<18)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX4_Q	(1<<19)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX5_U	(1<<20)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX5_V	(1<<21)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX5_P	(1<<22)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX5_Q	(1<<23)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX6_U	(1<<24)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX6_V	(1<<25)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX6_P	(1<<26)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX6_Q	(1<<27)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX7_U	(1<<28)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX7_V	(1<<29)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX7_P	(1<<30)
#define CELL_GCM_TEXTURE_CYLINDRICAL_WRAP_ENABLE_TEX7_Q	(1<<31)

#define CELL_GCM_VERTEX_ATTRIB_POS						0
#define CELL_GCM_VERTEX_ATTRIB_WEIGHT					1
#define CELL_GCM_VERTEX_ATTRIB_NORMAL					2
#define CELL_GCM_VERTEX_ATTRIB_COLOR0					3
#define CELL_GCM_VERTEX_ATTRIB_COLOR1					4
#define CELL_GCM_VERTEX_ATTRIB_FOG						5
#define CELL_GCM_VERTEX_ATTRIB_COLOR_INDEX				6
#define CELL_GCM_VERTEX_ATTRIB_POINT_SIZE				6	/*alias*/
#define CELL_GCM_VERTEX_ATTRIB_EDGEFLAG					7
#define CELL_GCM_VERTEX_ATTRIB_TEX0						8
#define CELL_GCM_VERTEX_ATTRIB_TEX1						9
#define CELL_GCM_VERTEX_ATTRIB_TEX2						10
#define CELL_GCM_VERTEX_ATTRIB_TEX3						11
#define CELL_GCM_VERTEX_ATTRIB_TEX4						12
#define CELL_GCM_VERTEX_ATTRIB_TEX5						13
#define CELL_GCM_VERTEX_ATTRIB_TEX6						14
#define CELL_GCM_VERTEX_ATTRIB_TANGENT					14	/*alias*/
#define CELL_GCM_VERTEX_ATTRIB_TEX7						15
#define CELL_GCM_VERTEX_ATTRIB_BINORMAL					15	/*alias*/

#define CELL_GCM_VERTEX_DATA_TYPE_F32					2
#define CELL_GCM_VERTEX_DATA_TYPE_U8						4

#define CELL_GCM_INDEX_TYPE_32B							0
#define CELL_GCM_INDEX_TYPE_16B							1

#define CELL_GCM_USER_CLIP_PLANE_DISABLE					0
#define CELL_GCM_USER_CLIP_PLANE_LT						1
#define CELL_GCM_USER_CLIP_PLANE_GE						2

#define CELL_GCM_ATTRIB_OUTPUT_MASK_FRONTDIFFUSE			(1<< 0)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_FRONTSPECULAR		(1<< 1)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_BACKDIFFUSE			(1<< 2)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_BACKSPECULAR			(1<< 3)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_FOG					(1<< 4)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_POINTSIZE			(1<< 5)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_UC0					(1<< 6)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_UC1					(1<< 7)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_UC2					(1<< 8)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_UC3					(1<< 9)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_UC4					(1<<10)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_UC5					(1<<11)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX8					(1<<12)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX9					(1<<13)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX0					(1<<14)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX1					(1<<15)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX2					(1<<16)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX3					(1<<17)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX4					(1<<18)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX5					(1<<19)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX6					(1<<20)
#define CELL_GCM_ATTRIB_OUTPUT_MASK_TEX7					(1<<21)

/*! \brief Flat shading */
#define CELL_GCM_SHADE_MODEL_FLAT						0x1D00
/*! \brief Smooth shading */
#define CELL_GCM_SHADE_MODEL_SMOOTH						0x1D01

/*! \brief blend factors are zero */
#define CELL_GCM_ZERO									0
/*! \brief blend factors are one */
#define CELL_GCM_ONE										1
/*! \brief blend factors are the source color components */
#define CELL_GCM_SRC_COLOR								0x0300
/*! \brief blend factors are one minus source color components */
#define CELL_GCM_ONE_MINUS_SRC_COLOR						0x0301
/*! \brief blend factors are the source alpha component */
#define CELL_GCM_SRC_ALPHA								0x0302
/*! \brief blend factors are one minus the source alpha component */
#define CELL_GCM_ONE_MINUS_SRC_ALPHA						0x0303
/*! \brief blend factors are the destination alpha component */
#define CELL_GCM_DST_ALPHA								0x0304
/*! \brief blend factors are one minus the destination alpha component */
#define CELL_GCM_ONE_MINUS_DST_ALPHA						0x0305
/*! \brief blend factors are the destination color components */
#define CELL_GCM_DST_COLOR								0x0306
/*! \brief blend factors are one minus the destination color components */
#define CELL_GCM_ONE_MINUS_DST_COLOR						0x0307
/*! \brief blend factors are set to saturate the output */
#define CELL_GCM_SRC_ALPHA_SATURATE						0x0308
/*! \brief blend factors are the constant color components */
#define CELL_GCM_CONSTANT_COLOR							0x8001
/*! \brief blend factors are one minus the constant color components */
#define CELL_GCM_ONE_MINUS_CONSTANT_COLOR				0x8002
/*! \brief blend factors are the constant color alpha component */
#define CELL_GCM_CONSTANT_ALPHA							0x8003
/*! \brief blend factors are one minus the constant color alpha component */
#define CELL_GCM_ONE_MINUS_CONSTANT_ALPHA				0x8004

/*! \brief use scaled source plus destination */
#define CELL_GCM_FUNC_ADD								0x8006
/*! \brief use the minimum of source and destination color components */
#define CELL_GCM_FUNC_MIN								0x8007
/*! \brief use the maximum of source and destination color components */
#define CELL_GCM_FUNC_MAX								0x8008
/*! \brief use scaled source minus destination */
#define CELL_GCM_FUNC_SUBTRACT							0x800a
/*! \brief use scaled destination minus source */
#define CELL_GCM_FUNC_REVERSE_SUBTRACT					0x800b
/*! \brief use scaled destination minus source (signed) */
#define CELL_GCM_FUNC_REVERSE_SUBTRACT_SIGNED			0xf005
/*! \brief use scaled source plus destination (signed) */
#define CELL_GCM_FUNC_ADD_SIGNED							0xf006
/*! \brief use scaled destination plus source (signed) */
#define CELL_GCM_FUNC_REVERSE_ADD_SIGNED					0xf007

#define CELL_GCM_TRANSFER_SURFACE						0
#define CELL_GCM_TRANSFER_SWIZZLE						1

/*! \brief Convert pixel component values using dithering. */
#define CELL_GCM_TRANSFER_CONVERSION_DITHER				0
/*! \brief Convert pixel component values by truncation (\em ie, ignore lower bits). */
#define CELL_GCM_TRANSFER_CONVERSION_TRUNCATE			1
/*! \brief Convert pixel component values by subtraction/truncation. */
#define CELL_GCM_TRANSFER_CONVERSION_SUBTRACT_TRUNCATE	2

/*! \brief Pixel format is 1-bit alpha, 5-bit red, 5-bit green, 5-bit blue */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_A1R5G5B5			1
/*! \brief Pixel format is 1-bit unused, 5-bit red, 5-bit green, 5-bit blue */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_X1R5G5B5			2
/*! \brief Pixel format is 8-bit alpha, 8-bit red, 8-bit green, 8-bit blue */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8			3
/*! \brief Pixel format is 8-bit unused, 8-bit red, 8-bit green, 8-bit blue */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_X8R8G8B8			4
/*! \brief Pixel format is 8-bit red chrominance, 8-bit luminance, 8-bit blue chrominance, 8-bit alpha */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_CR8YB8CB8YA8		5
/*! \brief Pixel format is 8-bit luminance, 8-bit red chrominance, 8-bit alpha, 8-bit blue chrominance */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_YB8CR8YA8CB8		6
/*! \brief Pixel format is 5-bit red, 6-bit green, 5-bit blue */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_R5G6B5			7
/*! \brief Pixel format is 8-bit grayscale */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_Y8				8
/*! \brief Pixel format is 8-bit alpha */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_AY8				9
/*! \brief Pixel format is EYB8ECR8EYA8ECB8 */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_EYB8ECR8EYA8ECB8	0xa
/*! \brief Pixel format is ECR8EYB8ECB8EYA8 */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_ECR8EYB8ECB8EYA8	0xb
/*! \brief Pixel format is 8-bit alpha, 8-bit blue, 8-bit green, 8-bit red */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_A8B8G8R8			0xc
/*! \brief Pixel format is 8-bit unused, 8-bit blue, 8-bit green, 8-bit red */
#define CELL_GCM_TRANSFER_SCALE_FORMAT_X8B8G8R8			0xd

/*! \brief Copy source image, perform logical \c AND with destination. */
#define CELL_GCM_TRANSFER_OPERATION_SRCCOPY_AND			0
/*! \brief Perform ROP (raster operation), and logical \c AND with destination. */
#define CELL_GCM_TRANSFER_OPERATION_ROP_AND				1
/*! \brief Perform blending, and logical \c AND with destination. */
#define CELL_GCM_TRANSFER_OPERATION_BLEND_AND			2
/*! \brief Copy source image. */
#define CELL_GCM_TRANSFER_OPERATION_SRCCOPY				3
/*! \brief Copy pre-multiplied source image. */
#define CELL_GCM_TRANSFER_OPERATION_SRCCOPY_PREMULT		4
/*! \brief Blend pre-multiplied source image. */
#define CELL_GCM_TRANSFER_OPERATION_BLEND_PREMULT		5

/*! \brief Origin is the center of the source image. */
#define CELL_GCM_TRANSFER_ORIGIN_CENTER					1
/*! \brief Origin is the topleft cornet of the source image. */
#define CELL_GCM_TRANSFER_ORIGIN_CORNER					2

/*! \brief Use point sampling interpolation. */
#define CELL_GCM_TRANSFER_INTERPOLATOR_NEAREST			0 // point sampling
/*! \brief Use point linear interpolation. */
#define CELL_GCM_TRANSFER_INTERPOLATOR_LINEAR			1 // bilinear interpolation

/*! \brief Source surface pixel format is 5-bit red, 6-bit green, 5-bit blue. */
#define CELL_GCM_TRANSFER_SURFACE_FORMAT_R5G6B5			4
/*! \brief Source surface pixel format is 8-bit alpha, 8-bit red, 8-bit green, 8-bit blue. */
#define CELL_GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8		0xa
/*! \brief Source surface pixel format is Y32. */
#define CELL_GCM_TRANSFER_SURFACE_FORMAT_Y32				0xb

#define CELL_GCM_FREQUENCY_MODULO						1
#define CELL_GCM_FREQUENCY_DIVIDE						0

#define CELL_GCM_COMPMODE_DISABLED						0
#define CELL_GCM_COMPMODE_C32_2X1						7
#define CELL_GCM_COMPMODE_C32_2X2						8
#define CELL_GCM_COMPMODE_Z32_SEPSTENCIL					9
#define CELL_GCM_COMPMODE_Z32_SEPSTENCIL_REG				10
#define CELL_GCM_COMPMODE_Z32_SEPSTENCIL_REGULAR			10
#define CELL_GCM_COMPMODE_Z32_SEPSTENCIL_DIAGONAL		11
#define CELL_GCM_COMPMODE_Z32_SEPSTENCIL_ROTATED			12

#define CELL_GCM_ZCULL_Z16								1
#define CELL_GCM_ZCULL_Z24S8								2

#define CELL_GCM_ZCULL_MSB								0
#define CELL_GCM_ZCULL_LONES								1

#define CELL_GCM_ZCULL_LESS								0
#define CELL_GCM_ZCULL_GREATER							1

#define CELL_GCM_SCULL_SFUNC_NEVER						0
#define CELL_GCM_SCULL_SFUNC_LESS						1
#define CELL_GCM_SCULL_SFUNC_EQUAL						2
#define CELL_GCM_SCULL_SFUNC_LEQUAL						3
#define CELL_GCM_SCULL_SFUNC_GREATER						4
#define CELL_GCM_SCULL_SFUNC_NOTEQUAL					5
#define CELL_GCM_SCULL_SFUNC_GEQUAL						6
#define CELL_GCM_SCULL_SFUNC_ALWAYS						7

#define CELL_GCM_CONDITIONAL								2

#define CELL_GCM_ZPASS_PIXEL_CNT							1
#define CELL_GCM_ZCULL_STATS								2
#define CELL_GCM_ZCULL_STATS1							3
#define CELL_GCM_ZCULL_STATS2							4
#define CELL_GCM_ZCULL_STATS3							5

#define CELL_GCM_ZCULL_ALIGN_OFFSET						4096
#define CELL_GCM_ZCULL_ALIGN_WIDTH						64
#define CELL_GCM_ZCULL_ALIGN_HEIGHT						64
#define CELL_GCM_ZCULL_ALIGN_CULLSTART					4096
#define CELL_GCM_ZCULL_COMPRESSION_TAG_BASE_MAX			0x7FF
#define CELL_GCM_ZCULL_RAM_SIZE_MAX						0x00300000

#define CELL_GCM_TILE_ALIGN_OFFSET						0x00010000
#define CELL_GCM_TILE_ALIGN_SIZE							0x00010000
#define CELL_GCM_TILE_LOCAL_ALIGN_HEIGHT					32
#define CELL_GCM_TILE_MAIN_ALIGN_HEIGHT					64

#define CELL_GCM_TILE_ALIGN_BUFFER_START_BOUNDARY		8

#define CELL_GCM_FRAGMENT_UCODE_LOCAL_ALIGN_OFFSET		64
#define CELL_GCM_FRAGMENT_UCODE_MAIN_ALIGN_OFFSET		128

#define CELL_GCM_SURFACE_LINEAR_ALIGN_OFFSET				64
#define CELL_GCM_SURFACE_SWIZZLE_ALIGN_OFFSET			128

#define CELL_GCM_TEXTURE_SWIZZLE_ALIGN_OFFSET			128
#define CELL_GCM_TEXTURE_CUBEMAP_ALIGN_OFFSET			128
#define CELL_GCM_TEXTURE_SWIZZLED_CUBEMAP_FACE_ALIGN_OFFSET 	128

#define CELL_GCM_VERTEX_TEXTURE_CACHE_LINE_SIZE			32
#define CELL_GCM_L2_TEXTURE_CACHE_LOCAL_LINE_SIZE		64
#define CELL_GCM_L2_TEXTURE_CACHE_MAIN_LINE_SIZE			128

#define CELL_GCM_TEXTURE_REMAP_MODE(order, inputA, inputR, inputG, inputB, outputA, outputR, outputG, outputB) \
	(((order) << 16) | \
	 ((inputA) << CELL_GCM_TEXTURE_REMAP_COLOR_A_SHIFT) | ((inputR) << CELL_GCM_TEXTURE_REMAP_COLOR_R_SHIFT) | \
	 ((inputG) << CELL_GCM_TEXTURE_REMAP_COLOR_G_SHIFT) | ((inputB) << CELL_GCM_TEXTURE_REMAP_COLOR_B_SHIFT) | \
	 ((outputA) << CELL_GCM_TEXTURE_REMAP_TYPE_A_SHIFT) | ((outputR) << CELL_GCM_TEXTURE_REMAP_TYPE_R_SHIFT) | \
	 ((outputG) << CELL_GCM_TEXTURE_REMAP_TYPE_G_SHIFT) | ((outputB) << CELL_GCM_TEXTURE_REMAP_TYPE_B_SHIFT))

#ifdef __cplusplus
extern "C" {
#endif

struct _cellGcmCtxData;
typedef s32 (*CellGcmContextCallback)(struct _cellGcmCtxData *context,u32 count);

/*! \brief RSX Context data structure.

This structure is used for managing and controlling the command buffer.
*/
typedef struct _cellGcmCtxData
{
	u32 *__restrict begin ATTRIBUTE_PRXPTR;					/*!< \brief Start address of command buffer */
	u32 *__restrict end ATTRIBUTE_PRXPTR;					/*!< \brief End address of command buffer */
	u32 *__restrict current ATTRIBUTE_PRXPTR;				/*!< \brief Current address of command buffer */
	CellGcmContextCallback callback ATTRIBUTE_PRXPTR;			/*!< \brief Callback function that is called when <i>current</i> reaches <i>end</i> */
} CellGcmContextData;

/*! \brief RSX control data structure.

This structure is used to control the command buffer.
*/
typedef struct _cellGcmCtrlRegister
{
	vu32 put;					/*!< \brief member for accessing the PUT register */
	vu32 get;					/*!< \brief member for accessing the GET register */
	vu32 ref;					/*!< \brief member for accessing the REF register. Initial value is 0xFFFFFFFF */
} CellGcmControlRegister;

/*! \brief RSX Configuration structure.

This structure holds system informations of RSX.
*/
typedef struct _cellGcmCfg
{
	void *localAddress ATTRIBUTE_PRXPTR;			/*!< \brief effective start address of RSX memory */
	void *ioAddress ATTRIBUTE_PRXPTR;				/*!< \brief effective start address of I/O mapped main memory to be used by RSX */
	u32 localSize;									/*!< \brief maximum available size of RSX memory */
	u32 ioSize;										/*!< \brief maximum available size of I/O mapped main memory to be used by RSX */
	u32 memoryFreq;									/*!< \brief RSX memory clock frequency. */
	u32 coreFreq;									/*!< \brief Core clock frequency of RSX */
} CellGcmConfiguration;

/*! \brief RSX target surface data structure.

This structure holds settings of the render target that is to be the render buffer.
Set the buffer to use for rendering by passing this structure as the argument when calling \ref rsxSetSurface. */
typedef struct _cellGcmSurface
{
	/*! \brief Type of render target.

	Possible values are:
	- \ref CELL_GCM_SURFACE_TYPE_LINEAR
	- \ref CELL_GCM_SURFACE_TYPE_SWIZZLE
	*/
	u8 type;

	/*! \brief Antialiasing format type.

	Specifies the mode of multiple samples. Possible values are:
	- \ref CELL_GCM_SURFACE_CENTER_1			
	- \ref CELL_GCM_SURFACE_DIAGONAL_CENTERED_2	
	- \ref CELL_GCM_SURFACE_SQUARE_CENTERED_4	
	- \ref CELL_GCM_SURFACE_SQUARE_ROTATED_4	
	*/
	u8 antiAlias;

	/*! \brief Format of the color buffer.

	Possible values are:
	-\ ref CELL_GCM_SURFACE_X1R5G5B5_Z1R5G5B5	
	-\ ref CELL_GCM_SURFACE_X1R5G5B5_O1R5G5B5	
	-\ ref CELL_GCM_SURFACE_R5G5B5				
	-\ ref CELL_GCM_SURFACE_X8R8G8B8_Z8R8G8B8	
	-\ ref CELL_GCM_SURFACE_X8R8G8B8			
	-\ ref CELL_GCM_SURFACE_A8R8G8B8			
	-\ ref CELL_GCM_SURFACE_B8					
	-\ ref CELL_GCM_SURFACE_G8B8				
	-\ ref CELL_GCM_SURFACE_F_W16Z16Y16X16		
	-\ ref CELL_GCM_SURFACE_F_W32Z32Y32X32		
	-\ ref CELL_GCM_SURFACE_F_X32				
	-\ ref CELL_GCM_SURFACE_X8B8G8R8_Z8B8G8R8	
	-\ ref CELL_GCM_SURFACE_X8B8G8R8_O8B8G8R8	
	-\ ref CELL_GCM_SURFACE_A8B8G8R8			
	*/
	u8 colorFormat;

	/*! \brief Target of the color buffer.

	Specifies the render target to use as a surface. Possible values are:
	- \ref CELL_GCM_SURFACE_TARGET_NONE
	- \ref CELL_GCM_SURFACE_TARGET_0
	- \ref CELL_GCM_SURFACE_TARGET_1
	- \ref CELL_GCM_SURFACE_TARGET_MRT1
	- \ref CELL_GCM_SURFACE_TARGET_MRT2
	- \ref CELL_GCM_SURFACE_TARGET_MRT3
	*/
	u8 colorTarget;

	/*! \brief Location of the color buffer.

	When using multiple render targets, set as many locations as the number of color buffers enabled in <i>colorTarget</i>.
	In this system, up to 4 color buffers can be specified for multiple render targets, and the location of each individual color buffer can be specified independently.
	Possible values are:
	- \ref CELL_GCM_LOCATION_RSX
	- \ref CELL_GCM_LOCATION_CELL
	*/
	u8 colorLocation[CELL_GCM_MAX_MRT_COUNT];

	/*! \brief Offset from the base address of the color buffer.

	When using multiple render targets, set as many addresses as the number of color buffers specified in <i>colorTarget</i>.
	Use \ref rsxAddressToOffset to convert the effective addresses into offset values when specifying the buffer offset. <i>colorOffset</i> should be aligned on a 64 bytes boundery.
	*/
	u32 colorOffset[CELL_GCM_MAX_MRT_COUNT];

	/*! \brief Size of a color buffer line in bytes.

	When using multiple render targets, specify as many pitch sizes as the number of color buffers specified in <i>colorTarget</i>.
	The pitch size should be 64 when rendering in the swizzle format. For all others, the pitch size should be a multiple of 64.
	*/
	u32 colorPitch[CELL_GCM_MAX_MRT_COUNT];

	/*! \brief Format of the depth buffer.

	Possible values are:
	- \ref CELL_GCM_SURFACE_ZETA_Z16
	- \ref CELL_GCM_SURFACE_ZETA_Z24S8
	*/
	u8 depthFormat;

	/*! \brief Location of the depth buffer.

	Possible values are:
	- \ref CELL_GCM_LOCATION_RSX
	- \ref CELL_GCM_LOCATION_CELL
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
} CellGcmSurface;

/*! \brief RSX Texture data structure. */
typedef struct _cellGcmTexture
{
	/*! \brief Texture format.
	
	This is an OR-ed combination of the following values:
	- \ref CELL_GCM_TEXTURE_FORMAT_SWZ
	- \ref CELL_GCM_TEXTURE_FORMAT_LIN
	- \ref CELL_GCM_TEXTURE_FORMAT_NRM
	- \ref CELL_GCM_TEXTURE_FORMAT_B8
	- \ref CELL_GCM_TEXTURE_FORMAT_A1R5G5B5
	- \ref CELL_GCM_TEXTURE_FORMAT_A4R4G4B4
	- \ref CELL_GCM_TEXTURE_FORMAT_R5G6B5
	- \ref CELL_GCM_TEXTURE_FORMAT_A8R8G8B8
	- \ref CELL_GCM_TEXTURE_FORMAT_DXT1
	- \ref CELL_GCM_TEXTURE_FORMAT_DXT3
	- \ref CELL_GCM_TEXTURE_FORMAT_DXT5
	- \ref CELL_GCM_TEXTURE_FORMAT_A8L8
	*/
	u8 format;

	/*! \brief Indicates if this is a mip-mapped texture.
	
	Possible values are:
	- \ref CELL_GCM_TRUE
	- \ref CELL_GCM_FALSE
	*/
	u8 mipmap;

	/*! \brief Texture dimension.
	
	Possible values are:
	- \ref CELL_GCM_TEXTURE_DIMS_1D
	- \ref CELL_GCM_TEXTURE_DIMS_2D
	- \ref CELL_GCM_TEXTURE_DIMS_3D
	*/
	u8 dimension;

	/*! \brief Indicates if this is a cube-mapped texture.
	
	Possible values are:
	- \ref CELL_GCM_TRUE
	- \ref CELL_GCM_FALSE
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

    (\ref CELL_GCM_TEXTURE_REMAP_TYPE_ZERO << \ref CELL_GCM_TEXTURE_REMAP_TYPE_A_SHIFT)\n
	| (\ref CELL_GCM_TEXTURE_REMAP_TYPE_REMAP << \ref CELL_GCM_TEXTURE_REMAP_TYPE_R_SHIFT)\n
	| (\ref CELL_GCM_TEXTURE_REMAP_COLOR_B << \ref CELL_GCM_TEXTURE_REMAP_COLOR_R_SHIFT)\n
    | (\ref CELL_GCM_TEXTURE_REMAP_TYPE_REMAP << \ref CELL_GCM_TEXTURE_REMAP_TYPE_G_SHIFT)\n
	| (\ref CELL_GCM_TEXTURE_REMAP_COLOR_G << \ref CELL_GCM_TEXTURE_REMAP_COLOR_G_SHIFT)\n
    | (\ref CELL_GCM_TEXTURE_REMAP_TYPE_REMAP << \ref CELL_GCM_TEXTURE_REMAP_TYPE_B_SHIFT)\n
	| (\ref CELL_GCM_TEXTURE_REMAP_COLOR_R << \ref CELL_GCM_TEXTURE_REMAP_COLOR_B_SHIFT)
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
	- \ref CELL_GCM_LOCATION_RSX
	- \ref CELL_GCM_LOCATION_CELL
	*/
	u8 location;
	/*! \brief unused padding byte. */
	u8 _pad;
	/*! \brief Size of a texture line in bytes. */
	u32 pitch;
	/*! \brief Offset of texture data. */
	u32 offset;
} CellGcmTexture;

/*! \brief Specify scaled image blit geometry and format for rsxSetTransferImage() */
typedef struct _CellGcmTransferScale
{
	/*! \brief Not sure what this dones. Possible values:
      - \ref CELL_GCM_TRANSFER_CONVERSION_DITHER
      - \ref CELL_GCM_TRANSFER_CONVERSION_TRUNCATE
      - \ref CELL_GCM_TRANSFER_CONVERSION_SUBTRACT_TRUNCATE
	 */	
	u32 conversion;

	/*! \brief Format of image data. Possible values:
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_A1R5G5B5
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_X1R5G5B5
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_X8R8G8B8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_CR8YB8CB8YA8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_YB8CR8YA8CB8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_R5G6B5
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_Y8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_AY8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_EYB8ECR8EYA8ECB8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_ECR8EYB8ECB8EYA8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_A8B8G8R8
	  - \ref CELL_GCM_TRANSFER_SCALE_FORMAT_X8B8G8R8
	 */
	u32 format;

	/*! \brief Blit operation. Possible values:
	  - \ref CELL_GCM_TRANSFER_OPERATION_SRCCOPY_AND
	  - \ref CELL_GCM_TRANSFER_OPERATION_ROP_AND
	  - \ref CELL_GCM_TRANSFER_OPERATION_BLEND_AND
	  - \ref CELL_GCM_TRANSFER_OPERATION_SRCCOPY
	  - \ref CELL_GCM_TRANSFER_OPERATION_SRCCOPY_PREMULT
	  - \ref CELL_GCM_TRANSFER_OPERATION_BLEND_PREMULT
	 */
	u32 operation;

	/*! \brief X origin of clipping rectangle, within the destination surface. */
	s16 clipX;

	/*! \brief Y origin of clipping rectangle, within the destination surface. */
	s16 clipY;

	/*! \brief Width of clipping rectangle, within the destination surface. */
	u16 clipW;

	/*! \brief Height of clipping rectangle, within the destination surface. */
	u16 clipH;

	/*! \brief X origin of destination rectangle. */
	s16 outX;

	/*! \brief Y origin of destination rectangle. */
	s16 outY;

	/*! \brief Width of the destination rectangle. */
	u16 outW;

	/*! \brief Height of the destination rectangle. */
	u16 outH;

	/*! \brief Ratio in X direction of the source rectangle size to the destination rectangle size, encoded as a 32-bit signed fixed-point number. Such a value can be obtained from a floating point number by rsxGetFixedSint32(). */
	s32 ratioX;

	/*! \brief Ratio in Y direction of the source rectangle size to the destination rectangle size, encoded as a 32-bit signed fixed-point number. Such a value can be obtained from a floating point number by rsxGetFixedSint32(). */
	s32 ratioY;

	/*! \brief Width of the source rectangle. */
	u16 inW;

	/*! \brief Height of the source rectangle. */
	u16 inH;

	/*! \brief Pitch size, in bytes, of the source image data (width multiplied by the number of bytes in each pixel). */
	u16 pitch;

	/*! \brief How the origin of each pixel is determined. Possible values:
	 - \ref CELL_GCM_TRANSFER_ORIGIN_CENTER
	 - \ref CELL_GCM_TRANSFER_ORIGIN_CORNER
	*/
	u8 origin;

	/*! \brief Sampling for scaled blits. Possible values:
	 - \ref CELL_GCM_TRANSFER_INTERPOLATOR_NEAREST: no interpolation
	 - \ref CELL_GCM_TRANSFER_INTERPOLATOR_LINEAR: bilinear interpolation
	*/
	u8 interp;

	/*! \brief Image data offset, e.g., a value returned by cellGcmAddressToOffset() or cellGcmMapMainMemory(). */
	u32 offset;

	/*! \brief X origin of destination rectangle. */
	u16 inX;

	/*! \brief Y origin of destination rectangle. */
	u16 inY;
} CellGcmTransferScale;

/*! \brief Specify destination surface characteristics for rsxSetTransferImage(). */
typedef struct _cellGcmTransferSurface
{
	/*! \brief Format of destination surface. Possible values are:
	 - \ref CELL_GCM_TRANSFER_SURFACE_FORMAT_R5G6B5
	 - \ref CELL_GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8
	 - \ref CELL_GCM_TRANSFER_SURFACE_FORMAT_Y32
	*/
	u32 format;

	/*! \brief Pitch for destination surface (width multipied by the number of bytes per pixel). */
	u16 pitch;

	/*! \brief unused padding bytes. most be 0. */
	u8 _pad0[2];

	/*! \brief Destination suface memory offset, e.g., a value returned by cellGcmAddressToOffset() or cellGcmMapMainMemory(). */
	u32 offset;
} CellGcmTransferSurface;

typedef struct _cellGcmTransferSwizzle
{
	u16 format;
	u8 width;
	u8 height;
	u32 offset;
} CellGcmTransferSwizzle;

typedef struct _cellGcmTileInfo
{
    u32 tile;
    u32 limit;
    u32 pitch;
    u32 format;
} CellGcmTileInfo;

typedef struct _cellGcmZcullInfo
{
    u32 region;
    u32 size;
    u32 start;
    u32 offset;
    u32 stat0;
    u32 stat1;
} CellGcmZcullInfo;

/*! \brief Display information */
typedef struct _cellGcmDisplayInfo
{
    /*! \brief */
    u32 offset;
    /*! \brief Pitch for display (width multipied by the number of bytes per pixel). */
    u32 pitch;
    /*! \brief Width for display (in pixels) */
    u32 width;
    /*! \brief Height for display (in pixels) */
    u32 height;

} CellGcmDisplayInfo;

typedef struct _cellGcmOffsetTable
{
    u16 *io; /* ea -> io */
    u16 *ea; /* io -> ea */
} CellGcmOffsetTable;

typedef struct _reportData
{
    u64 timer;
    u32 value;
    u32 zero;
} CellGcmReportData;

typedef struct _notifyData
{
    u64 timer;
    u64 zero;
} CellGcmNotifyData;

/*! \brief Initialize the RSX context.

\param ctx Pointer to where the effective address of the allocated context
 structure will be stored.
\param cmdSize The command buffer size.
\param ioSize The allocated IO buffer size.
\param ioAddress Pointer to an allocated buffer of \p ioSize bytes.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellGcmInitBody(CellGcmContextData **ctx,const u32 cmdSize,const u32 ioSize,const void *ioAddress);

/*! \brief Converts an effective address in RSX memory to an offset.
\param address The effective address to be converted.
\param offset A pointer to the returned offset value.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellGcmAddressToOffset(const void *address,u32 *offset);

/*! \brief Converts an offset to an effective address in RSX memory.
\param offset The offset to be converted.
\param address A pointer to the returned effective address.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellGcmIoOffsetToAddress(u32 offset,void **address);

/*! \brief Retrieves the RSX configuration.
\param config A pointer to the configuration structure to be updated.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellGcmGetConfiguration(CellGcmConfiguration *config);

/*! \brief Gets the flip status.

Once a flip occurred, querying for a subsequent flip requires the flip status
to be reset using \ref cellGcmResetFlipStatus.
\return zero if no flip occured, nonzero otherwise.
*/
u32 cellGcmGetFlipStatus(void);

/*! \brief Enqueues a flip command into the command buffer.
\param context Pointer to the context object.
\param bufferId Framebuffer id to flip to (as configured with
   \ref cellGcmSetDisplayBuffer).
\return zero if no error occured, nonzero otherwise.
*/
s32 cellGcmSetFlip(CellGcmContextData *context,const u8 bufferId);

/*! \brief Configures a display framebuffer.
\param bufferId The buffer id (0-7).
\param offset The offset of the allocated memory block (see \ref rsxAddressToOffset).
\param pitch The size of a buffer line in bytes.
\param width The buffer width in pixels.
\param height The buffer height in pixels.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellGcmSetDisplayBuffer(const u8 bufferId,const u32 offset,const u32 pitch,const u32 width,const u32 height);

/*! \brief Maps a memory block in main memory for RSX to access it.
\param address Pointer to the block to be mapped.
\param size Size of the block in bytes.
\param offset A pointer to the returned mapped offset value.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellGcmMapMainMemory(const void *address,const u32 size,u32 *offset);

/*! \brief Get address of specified label.
\param index The label index whose address is to be obtained.
\return Pointer to the label address.
*/
u32* cellGcmGetLabelAddress(const u8 index);

/*! \brief Reset the flip status. */
void cellGcmResetFlipStatus(void);

/*! \brief Set flip mode.
\param mode The specified flip mode. Possible vales are:
 - \ref CELL_GCM_FLIP_HSYNC
 - \ref CELL_GCM_FLIP_VSYNC
 - \ref CELL_GCM_FLIP_HSYNC_AND_BREAK_EVERYTHING
*/
void cellGcmSetFlipMode(const u32 mode);

/*! \brief Wait for a flip to be completed.
\param context Pointer to the context object.
*/
void cellGcmSetWaitFlip(CellGcmContextData *context);

/*! \brief Perform preprocessing for display output
\param context Pointer to context object
\param id buffer id
*/
u32 cellGcmSetPrepareFlip(CellGcmContextData *context,const u8 id);

/*! \brief Register a callback function to be called upon VBlank.

This function registers a callback function to be called upon VBlank and after VSync. The registered callback can be canceled if NULL is specified for <i>handler</i>.
\param handler Callback function to register
*/
void cellGcmSetVBlankHandler(void (*handler)(const u32 head));

/*! \brief Register a callback function for when a flip is executed.

This function registers a callback function to be called when RSX actually carries out a flip. The registered callback can be canceled if NULL is specified for <i>handler</i>.
\param handler Callback function to register
*/
void cellGcmSetFlipHandler(void (*handler)(const u32 head));

/*! \brief Register a graphics callback function.

This function registers a callback function to be executed when an exception occurs in the graphics pipeline. The registered callback can be canceled if NULL is specified for <i>handler</i>.
\param handler Callback function to register
*/
void cellGcmSetGraphicsHandler(void (*handler)(const u32 val));
void cellGcmSetDefaultCommandBuffer(void);

/*! \brief Obtain registers for controlling the command buffer.
\return Pointer to a \ref CellGcmControlRegister structure
*/
CellGcmControlRegister* cellGcmGetControlRegister(void);

s32 cellGcmInitSystemMode(const u64 mode);
u32 cellGcmGetTiledPitchSize(const u32 size);
void cellGcmSetSecondVHandler(void (*handler)(const u32 head));
void cellGcmSetQueueHandler(void (*handler)(const u32 head));
void cellGcmSetUserHandler(void (*handler)(const u32 cause));
void cellGcmSetDebugOutputLevel(s32 level);
void cellGcmGetOffsetTable(CellGcmOffsetTable *table);
s32 cellGcmSortRemapEaIoAddress(void);
s32 cellGcmMapEaIoAddress(const void *ea, const u32 io, const u32 size);
s32 cellGcmUnmapIoAddress(const u32 io);
s32 cellGcmUnmapEaIoAddress(const void *ea);
s32 cellGcmMapEaIoAddressWithFlags(const void *ea, const u32 io, const u32 size, u32 userflags);
s32 cellGcmMapLocalMemory(void **address, u32 *size);
u32 cellGcmGetMaxIoMapSize(void);
s32 cellGcmReserveIoMapSize(const u32 size);
s32 cellGcmUnreserveIoMapSize(const u32 size);

/*
 * display functions
 */
s64 cellGcmGetLastFlipTime(void);
s32 cellGcmSetFlipImmediate(const u8 id);
void cellGcmSetFlipStatus(void);
void cellGcmSetSecondVFrequency(const u32 freq);
s32 cellGcmGetDisplayBufferByFlipIndex(const u32 qid);
u64 cellGcmGetVBlankCount(void);
s64 cellGcmGetLastSecondVTime(void);
u32 cellGcmGetCurrentField(void);
void cellGcmSetVBlankFrequency(const u32 freq);
s32 cellGcmGetCurrentDisplayBufferId(u8 *id);


/*
 * hwcursor functions
 */
s32 cellGcmInitCursor(void);
s32 cellGcmSetCursorEnable(void);
s32 cellGcmSetCursorDisable(void);
s32 cellGcmSetCursorImageOffset(const u32 offset);
s32 cellGcmSetCursorPosition(const s32 x, const s32 y);
s32 cellGcmUpdateCursor(void);

/*
 * flow control functions
 */
s32 cellGcmSetDefaultCommandBufferAndSegmentWordSize(const u32 bufferSize, const u32 segmentSize);
u32 cellGcmGetDefaultCommandWordSize(void);
u32 cellGcmGetDefaultSegmentWordSize(void);
s32 cellGcmInitDefaultFifoMode(s32 mode);
s32 cellGcmSetDefaultFifoSize(const u32 bufferSize, const u32 segmentSize);

/*
 * render functions
 */
u64 cellGcmGetTimeStamp(const u32 index);
u32 cellGcmGetReport(const u32 type, const u32 index);
void cellGcmSetTile(const u8 index, const u8 location, const u32 offset, const u32 size, const u32 pitch, const u8 comp, const u16 base, const u8 bank);
void cellGcmSetInvalidateTile(const u8 index);
void cellGcmSetZcull(const u8 index, const u32 offset, const u32 width, const u32 height, const u32 cullStart, const u32 zFormat, const u32 aaFormat, const u32 zCullDir, const u32 zCullFormat, const u32 sFunc, const u32 sRef, const u32 sMask);
s32 cellGcmSetTileInfo(const u8 index, const u8 location, const u32 offset, const u32 size, const u32 pitch, const u8 comp, const u16 base, const u8 bank);
s32 cellGcmBindTile(const u8 index);
s32 cellGcmUnbindTile(const u8 index);
s32 cellGcmBindZcull(const u8 index, const u32 offset, const u32 width, const u32 height, const u32 cullStart, const u32 zFormat, const u32 aaFormat, const u32 zCullDir, const u32 zCullFormat, const u32 sFunc, const u32 sRef, const u32 sMask);
s32 cellGcmUnbindZcull(const u8 index);
CellGcmReportData* cellGcmGetReportDataAddress(const u32 index);


/*
 * report functions
 */
CellGcmReportData* cellGcmGetReportDataAddressLocation(const u32 index, const u32 location);
u64 cellGcmGetTimeStampLocation(const u32 index, const u32 location);
u32 cellGcmGetReportDataLocation(const u32 index, const u32 location);

/*
 * notify functions
 */
CellGcmNotifyData* cellGcmGetNotifyDataAddress(const u32 index);


/*
 * tile / zcull / display functions
 */
const CellGcmTileInfo* cellGcmGetTileInfo(void);
const CellGcmZcullInfo* cellGcmGetZcullInfo(void);
const CellGcmDisplayInfo* cellGcmGetDisplayInfo(void);

/*
 * debug functions
 */
s32 cellGcmDumpGraphicsError(void);

void cellGcmSetUserCommand(void (*handler)(const u32 cause));

#ifdef __cplusplus
	}
#endif

#endif
