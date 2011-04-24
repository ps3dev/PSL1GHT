/*! \file rsx_program.h
\brief RSX shader programming management.
*/

#ifndef __RSX_PROGRAM_H__
#define __RSX_PROGRAM_H__

#include <ppu-types.h>

#define PARAM_FLOAT				0
#define PARAM_FLOAT2			1
#define PARAM_FLOAT3			2
#define PARAM_FLOAT4			3
#define PARAM_FLOAT4x4			4
#define PARAM_SAMPLER1D			5
#define PARAM_SAMPLER2D			6
#define PARAM_SAMPLER3D			7
#define PARAM_SAMPLERCUBE		8
#define PARAM_SAMPLERRECT		9
#define PARAM_UNKNOWN			0xff

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Vertex program data structure.

This data structure is filled by cgcomp, the offline compiler for shader programs. */
typedef struct rsx_vp
{
	u16 magic;			/*!< \brief magic identifier */
	u16 num_attrib;		/*!< \brief number of used input attributes in the vertex program */
	u32 attrib_off;		/*!< \brief offset to the attribute name table */

	u32 input_mask;		/*!< \brief mask of input attributes in the vertex shader */
	u32 output_mask;	/*!< \brief mask of result attributes passed to the fragment shader */

	u16 const_start;	/*!< \brief start address in vertex shader's constant block memory */
	u16 num_const;		/*!< \brief number of used constants in the vertex program */
	u32 const_off;		/*!< \brief offset to the constant name table */

	u16 start_insn;		/*!< \brief start address to load the vertex program to */
	u16 num_insn;		/*!< \brief number of vertex shader instructions */
	u32 ucode_off;		/*!< \brief offset to the shader's micro code */
} rsxVertexProgram;

/*! \brief Fragment program data structure.

This data structure is filled by cgcomp, the offline compiler for shader programs. */
typedef struct rsx_fp
{
	u16 magic;			/*!< \brief magic identifier */ 
	u16 num_attrib;		/*!< \brief number of used input attributes in the fragment program */
	u32 attrib_off;		/*!< \brief offset to the attribute name table */

	u32 num_regs;		/*!< \brief number of used registers in the fragment program */
	u32 fp_control;		/*!< \brief fragment program control mask */

	u16 texcoords;		/*!< \brief bit mask of all used texture coords in the fragment program */
	u16 texcoord2D;		/*!< \brief bit mask of used 2D texture coords in the fragment program */
	u16 texcoord3D;		/*!< \brief bit mask of used 3D texture coords in the fragment program */

	u16 _pad0;			/*!< \brief unused padding word. most be 0 */

	u16 num_const;		/*!< \brief number of used constants in the fragment program */
	u32 const_off;		/*!< \brief offset to the constant name table */

	u16 num_insn;		/*!< \brief number of fragment program instructions */
	u32 ucode_off;		/*!< \brief offset to the shaders's micro code */
} rsxFragmentProgram;

/*! \brief Program const data structure. */
typedef struct rsx_const
{
	u32 name_off;		/*!< \brief offset of name. */
	u32 index;			/*!< \brief program const id. */
	u8 type;			/*!< \brief program const type. */
	u8 is_internal;		/*!< \brief internal flag. */
	u8 count;			/*!< \brief number of elements in the const. */

	u8 _pad0;			/*!< \brief unused padding byte, most be 0. */

	union {
		u32 u;			/*!< \brief integer value. */
		f32 f;			/*!< \brief float value. */
	} values[4];		/*!< \brief array of element values.
	
	  Number of elements is in the \p count field.
	*/


} rsxProgramConst;

/*! \brief Table of const offsets. */
typedef struct rsx_co_table
{
	u32 num;		/*!< \brief number of elements in the array. */
	u32 offset[];	/*!< \brief array of const offsets. */
} rsxConstOffsetTable;

/*! \brief Table of program attributes. */
typedef struct rsx_attrib
{
	u32 name_off;	/*!< \brief offset of attribute name. */
	u32 index;		/*!< \brief attribute index. */
	u8 type;		/*!< \brief attribute type. */
	u8 _pad0[3];
} rsxProgramAttrib;

/*! \brief Get Ucode from RSX vertex program.
\param vp Pointer the to vertex program structure.
\return Pointer to the ucode.
*/
void* rsxVertexProgramGetUCode(rsxVertexProgram *vp);

/*! \brief Get the list of vertex program consts.
\param vp Pointer the to vertex program structure.
\return Pointer to the list of program const structures.
*/
rsxProgramConst* rsxVertexProgramGetConsts(rsxVertexProgram *vp);

/*! \brief Get id of vertex program const from its name.
\param vp Pointer the to vertex program structure.
\param name Name of the vertex program const.
\return The requested vertex program const id.
*/
s32 rsxVertexProgramGetConst(rsxVertexProgram *vp,const char *name);

/*! \brief Get the list of vertex program attributes.
\param vp Pointer the to vertex program structure.
\return Pointer to the list of program attribute structures.
*/
rsxProgramAttrib* rsxVertexProgramGetAttribs(rsxVertexProgram *vp);

/*! \brief Get id of vertex program attribute from its name.
\param vp Pointer the to vertex program structure.
\param name Name of the vertex program attribute.
\return The requested vertex program attribute id.
*/
s32 rsxVertexProgramGetAttrib(rsxVertexProgram *vp,const char *name);

/*! \brief Get Ucode from RSX fragment program.
\param fp Pointer the to fragment program structure.
\return Pointer to the ucode.
*/
void* rsxFragmentProgramGetUCode(rsxFragmentProgram *fp,u32 *size);

/*! \brief Get the list of fragment program consts.
\param fp Pointer the to fragment program structure.
\return Pointer to the list of program const structures.
*/
rsxProgramConst* rsxFragmentProgramGetConsts(rsxFragmentProgram *fp);

/*! \brief Get id of fragment program const from its name.
\param fp Pointer the to fragment program structure.
\param name Name of the fragment program const.
\return The requested fragment program const id.
*/
s32 rsxFragmentProgramGetConst(rsxFragmentProgram *fp,const char *name);

/*! \brief Get the list of fragment program attributes.
\param fp Pointer the to fragment program structure.
\return Pointer to the list of program attribute structures.
*/
rsxProgramAttrib* rsxFragmentProgramGetAttribs(rsxFragmentProgram *fp);

/*! \brief Get id of fragment program attribute from its name.
\param fp Pointer the to fragment program structure.
\param name Name of the fragment program attribute.
\return The requested fragment program attribute id.
*/
s32 rsxFragmentProgramGetAttrib(rsxFragmentProgram *fp,const char *name);

/*! \brief Get const offset table from a fragment program.
\param fp Pointer the to fragment program structure.
\param table_off Offset of the const offset table.
\return Pointer to the requested const offset table.
*/
rsxConstOffsetTable* rsxFragmentProgramGetConstOffsetTable(rsxFragmentProgram *fp,u32 table_off);

#ifdef __cplusplus
	}
#endif

#endif
