#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#ifdef WIN32
#include <memory.h>
#include <windows.h>
#endif

/**
 * Instruction precision for GL_NV_fragment_program
 */
/*@{*/
#define FLOAT32  0x0
#define FLOAT16  0x1
#define FIXED12  0x2
/*@}*/

typedef signed char					s8;
typedef signed short				s16;
typedef signed int					s32;

#if _MSC_VER>1300
typedef signed __int64				s64;
#else
typedef signed long long			s64;
#endif

typedef unsigned char				u8;
typedef unsigned short				u16;
typedef unsigned int				u32;

#if _MSC_VER>1300
typedef unsigned __int64			u64;
#else
typedef unsigned long long			u64;
#endif

typedef float						f32;
typedef double						f64;

#ifndef FALSE
#define FALSE						0
#endif

#ifndef TRUE
#define TRUE						1
#endif

#ifndef INLINE
#define INLINE						__inline
#endif

#ifndef boolean
#define boolean						char
#endif

#define MIN2(a,b)		((a)<(b) ? (a) : (b))
#define MAX2(a,b)		((a)>(b) ? (a) : (b))

#if _MSC_VER>1300
#define strnicmp		_strnicmp
#define stricmp			_stricmp
#endif

#if defined(_MSC_VER)
#define strcasecmp		stricmp
#define strncasecmp		strnicmp
#elif defined(__GNUC__)
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif

#ifdef __CYGWIN__
#include <strings.h>
#endif

typedef struct rsx_vp
{
	u16 magic;
	u16 _pad0;

	u16 num_regs;
	u16 num_attr;
	u16 num_const;
	u16 num_insn;

	u32 attr_off;
	u32 const_off;
	u32 ucode_off;

	u32 input_mask;
	u32 output_mask;

	u16 const_start;
	u16 insn_start;
} rsxVertexProgram;

typedef struct rsx_fp
{
	u16 magic;
	u16 _pad0;

	u16 num_regs;
	u16 num_attr;
	u16 num_const;
	u16 num_insn;

	u32 attr_off;
	u32 const_off;
	u32 ucode_off;

	u32 fp_control;

	u16 texcoords;
	u16 texcoord2D;
	u16 texcoord3D;
	u16 _pad1;
} rsxFragmentProgram;

typedef struct rsx_const
{
	u32 name_off;
	u32 index;
	u8 type;
	u8 is_internal;
	u8 count;
	
	u8 _pad0;

	union {
		u32 u;
		f32 f;
	} values[4];

} rsxProgramConst;

typedef struct rsx_constoffset_table
{
	u32 num;
	u32 offset[];
} rsxConstOffsetTable;

typedef struct rsx_attrib
{
	u32 name_off;
	u32 index;
	u8 type;
	u8 _pad0[3];
} rsxProgramAttrib;

#endif
