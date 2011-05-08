#include <nv40.h>
#include <rsx.h>
#include <gcm_sys.h>
#include <ppu_intrinsics.h>

#include "commands.h"
#include "rsx_internal.h"

#ifndef RSX_MEMCPY
#define RSX_MEMCPY	__builtin_memcpy
#endif

static __inline__ f32 swapF32_16(f32 v)
{
	ieee32 d;
	d.f = v;
	d.u = ( ( ( d.u >> 16 ) & 0xffff ) << 0 ) | ( ( ( d.u >> 0 ) & 0xffff ) << 16 );
	return d.f;
}

s32 __attribute__((noinline)) rsxContextCallback(gcmContextData *context,u32 count)
{
	register s32 result asm("3");
	asm volatile (
		"stdu	1,-128(1)\n"
		"mr		31,2\n"
		"lwz	0,0(%0)\n"
		"lwz	2,4(%0)\n"
		"mtctr	0\n"
		"bctrl\n"
		"mr		2,31\n"
		"addi	1,1,128\n"
		: : "b"(context->callback)
		: "r31", "r0", "r1", "r2", "lr"
	);
	return result;
}

void rsxSetReturnCommand(gcmContextData *context)
{
	RSX_CONTEXT_CURRENT_BEGIN(1);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_FLAG_RETURN;
	RSX_CONTEXT_CURRENT_END(1);
}

void rsxSetCallCommand(gcmContextData *context,u32 offset)
{
	RSX_CONTEXT_CURRENT_BEGIN(1);
	RSX_CONTEXT_CURRENTP[0] = (offset | RSX_METHOD_FLAG_CALL);
	RSX_CONTEXT_CURRENT_END(1);
}

void rsxSetJumpCommand(gcmContextData *context,u32 offset)
{
	RSX_CONTEXT_CURRENT_BEGIN(1);
	RSX_CONTEXT_CURRENTP[0] = (offset | RSX_METHOD_FLAG_JUMP);
	RSX_CONTEXT_CURRENT_END(1);
}

void rsxSetNopCommand(gcmContextData *context,u32 count)
{
	u32 i;

	RSX_CONTEXT_CURRENT_BEGIN(count);
	for(i=0;i<count;i++)
		RSX_CONTEXT_CURRENTP[i] = 0;
	RSX_CONTEXT_CURRENT_END(count);
}

void rsxSetClearColor(gcmContextData *context,u32 color)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLEAR_VALUE_COLOR,1);
	RSX_CONTEXT_CURRENTP[1] = color;
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetClearDepthValue(gcmContextData *context,u32 value)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLEAR_VALUE_DEPTH,1);
	RSX_CONTEXT_CURRENTP[1] = value;
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetReferenceCommand(gcmContextData *context,u32 ref_value)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV406ETCL_SET_REF,1);
	RSX_CONTEXT_CURRENTP[1] = ref_value;
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetWriteBackendLabel(gcmContextData *context,u8 index,u32 value)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);

	u32 offset = 0x10*index;
	u32 wvalue = (value&0xff00ff00) | ((value>>16)&0xff) | ((value&0xff)<<16);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_SEMAPHORE_OFFSET,1);
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_SEMAPHORE_BACKENDWRITE_RELEASE,1);
	RSX_CONTEXT_CURRENTP[3] = wvalue;
	RSX_CONTEXT_CURRENT_END(4);
}

void rsxSetWaitLabel(gcmContextData *context,u8 index,u32 value)
{
	u32 offset = 0x10*index;

	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV406ETCL_SEMAPHORE_OFFSET,1);
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV406ETCL_SEMAPHORE_ACQUIRE,1);
	RSX_CONTEXT_CURRENTP[3] = value;
	RSX_CONTEXT_CURRENT_END(4);
}

void rsxSetWriteCommandLabel(gcmContextData *context,u8 index,u32 value)
{
	u32 offset = 0x10*index;

	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV406ETCL_SEMAPHORE_OFFSET,1);
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV406ETCL_SEMAPHORE_RELEASE,1);
	RSX_CONTEXT_CURRENTP[3] = value;
	RSX_CONTEXT_CURRENT_END(4);
}

void rsxSetSurface(gcmContextData *context,gcmSurface *surface)
{
	RSX_CONTEXT_CURRENT_BEGIN(32);

	u32 log2Width = 31 - __cntlzw(surface->width);
	u32 log2Height = 31 - __cntlzw(surface->height);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DMA_COLOR0,1);
	RSX_CONTEXT_CURRENTP[1] = GCM_DMA_MEMORY_FRAME_BUFFER + surface->colorLocation[0];

	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_DMA_COLOR1,1);
	RSX_CONTEXT_CURRENTP[3] = GCM_DMA_MEMORY_FRAME_BUFFER + surface->colorLocation[1];

	RSX_CONTEXT_CURRENTP[4] = RSX_METHOD(NV40TCL_DMA_COLOR2,2);
	RSX_CONTEXT_CURRENTP[5] = GCM_DMA_MEMORY_FRAME_BUFFER + surface->colorLocation[2];
	RSX_CONTEXT_CURRENTP[6] = GCM_DMA_MEMORY_FRAME_BUFFER + surface->colorLocation[3];

	RSX_CONTEXT_CURRENTP[7] = RSX_METHOD(NV40TCL_DMA_ZETA,1);
	RSX_CONTEXT_CURRENTP[8] = GCM_DMA_MEMORY_FRAME_BUFFER + surface->depthLocation;

	RSX_CONTEXT_CURRENTP[9] = RSX_METHOD(NV40TCL_RT_FORMAT,6);
	RSX_CONTEXT_CURRENTP[10] = ((log2Height<<NV40TCL_RT_FORMAT_LOG2_HEIGHT_SHIFT) | 
							    (log2Width<<NV40TCL_RT_FORMAT_LOG2_WIDTH_SHIFT) | 
							    (surface->antiAlias<<NV40TCL_RT_FORMAT_ANTIALIAS_SHIFT) | 
							    (surface->type<<NV40TCL_RT_FORMAT_TYPE_SHIFT) | 
							    (surface->depthFormat<<NV40TCL_RT_FORMAT_ZETA_SHIFT) | 
							    (surface->colorFormat<<NV40TCL_RT_FORMAT_COLOR_SHIFT));
	RSX_CONTEXT_CURRENTP[11] = surface->colorPitch[0];
	RSX_CONTEXT_CURRENTP[12] = surface->colorOffset[0];
	RSX_CONTEXT_CURRENTP[13] = surface->depthOffset;
	RSX_CONTEXT_CURRENTP[14] = surface->colorOffset[1];
	RSX_CONTEXT_CURRENTP[15] = surface->colorPitch[1];

	RSX_CONTEXT_CURRENTP[16] = RSX_METHOD(NV40TCL_ZETA_PITCH,1);
	RSX_CONTEXT_CURRENTP[17] = surface->depthPitch;

	RSX_CONTEXT_CURRENTP[18] = RSX_METHOD(NV40TCL_COLOR2_PITCH,4);
	RSX_CONTEXT_CURRENTP[19] = surface->colorPitch[2];
	RSX_CONTEXT_CURRENTP[20] = surface->colorPitch[3];
	RSX_CONTEXT_CURRENTP[21] = surface->colorOffset[2];
	RSX_CONTEXT_CURRENTP[22] = surface->colorOffset[3];

	RSX_CONTEXT_CURRENTP[23] = RSX_METHOD(NV40TCL_RT_ENABLE,1);
	RSX_CONTEXT_CURRENTP[24] = surface->colorTarget;

	RSX_CONTEXT_CURRENTP[25] = RSX_METHOD(NV40TCL_WINDOW_OFFSET,1);
	RSX_CONTEXT_CURRENTP[26] = ((surface->y<<16) | surface->x);

	RSX_CONTEXT_CURRENTP[27] = RSX_METHOD(NV40TCL_RT_HORIZ,2);
	RSX_CONTEXT_CURRENTP[28] = ((surface->width<<16) | surface->x);
	RSX_CONTEXT_CURRENTP[29] = ((surface->height<<16) | surface->y);

	RSX_CONTEXT_CURRENTP[30] = RSX_METHOD(NV40TCL_SHADER_WINDOW,1);
	RSX_CONTEXT_CURRENTP[31] = ((0<<16) | (1<<12) | surface->height);

	RSX_CONTEXT_CURRENT_END(32);
}

void rsxSetColorMask(gcmContextData *context,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_COLOR_MASK,1);
	RSX_CONTEXT_CURRENTP[1] = mask;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetColorMaskMRT(gcmContextData *context,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_MRT_COLOR_MASK,1);
	RSX_CONTEXT_CURRENTP[1] = mask;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetShadeModel(gcmContextData *context,u32 shadeModel)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_SHADE_MODEL,1);
	RSX_CONTEXT_CURRENTP[1] = shadeModel;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetViewport(gcmContextData *context,u16 x,u16 y,u16 width,u16 height,f32 min,f32 max,const f32 scale[4],const f32 offset[4])
{
	ieee32 _min,_max;
	ieee32 _offset[4],_scale[4];

	_min.f = min;
	_max.f = max;

	_scale[0].f = scale[0];
	_scale[1].f = scale[1];
	_scale[2].f = scale[2];
	_scale[3].f = scale[3];

	_offset[0].f = offset[0];
	_offset[1].f = offset[1];
	_offset[2].f = offset[2];
	_offset[3].f = offset[3];

	RSX_CONTEXT_CURRENT_BEGIN(24);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VIEWPORT_HORIZ,2);
	RSX_CONTEXT_CURRENTP[1] = ((width<<16) | x);
	RSX_CONTEXT_CURRENTP[2] = ((height<<16) | y);

	RSX_CONTEXT_CURRENTP[3] = RSX_METHOD(NV40TCL_DEPTH_RANGE,2);
	RSX_CONTEXT_CURRENTP[4] = _min.u;
	RSX_CONTEXT_CURRENTP[5] = _max.u;

	RSX_CONTEXT_CURRENTP[6] = RSX_METHOD(NV40TCL_VIEWPORT_OFFSET,8);
	RSX_CONTEXT_CURRENTP[7] = _offset[0].u;
	RSX_CONTEXT_CURRENTP[8] = _offset[1].u;
	RSX_CONTEXT_CURRENTP[9] = _offset[2].u;
	RSX_CONTEXT_CURRENTP[10] = _offset[3].u;
	RSX_CONTEXT_CURRENTP[11] = _scale[0].u;
	RSX_CONTEXT_CURRENTP[12] = _scale[1].u;
	RSX_CONTEXT_CURRENTP[13] = _scale[2].u;
	RSX_CONTEXT_CURRENTP[14] = _scale[3].u;

	RSX_CONTEXT_CURRENTP[15] = RSX_METHOD(NV40TCL_VIEWPORT_OFFSET,8);
	RSX_CONTEXT_CURRENTP[16] = _offset[0].u;
	RSX_CONTEXT_CURRENTP[17] = _offset[1].u;
	RSX_CONTEXT_CURRENTP[18] = _offset[2].u;
	RSX_CONTEXT_CURRENTP[19] = _offset[3].u;
	RSX_CONTEXT_CURRENTP[20] = _scale[0].u;
	RSX_CONTEXT_CURRENTP[21] = _scale[1].u;
	RSX_CONTEXT_CURRENTP[22] = _scale[2].u;
	RSX_CONTEXT_CURRENTP[23] = _scale[3].u;

	RSX_CONTEXT_CURRENT_END(24);
}

void rsxSetViewportClip(gcmContextData *context,u8 sel,u16 width,u16 height)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VIEWPORT_CLIP_HORIZ(sel),2);
	RSX_CONTEXT_CURRENTP[1] = ((width-1) << 16);
	RSX_CONTEXT_CURRENTP[2] = ((height-1) << 16);
	
	RSX_CONTEXT_CURRENT_END(3);
}

void rsxSetUserClipPlaneControl(gcmContextData *context,u32 plane0,u32 plane1,u32 plane2,u32 plane3,u32 plane4,u32 plane5)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLIP_PLANE_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = ((plane5 << 20) | (plane4 << 16) | (plane3 << 12) | (plane2 << 8) | (plane1 << 4) | plane0);
	
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetDepthTestEnable(gcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_TEST_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetDepthFunc(gcmContextData *context,u32 func)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_FUNC,1);
	RSX_CONTEXT_CURRENTP[1] = func;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetDepthWriteEnable(gcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_WRITE_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetCullFaceEnable(gcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CULL_FACE_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetCullFace(gcmContextData *context,u32 cull)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CULL_FACE,1);
	RSX_CONTEXT_CURRENTP[1] = cull;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetFrontFace(gcmContextData *context,u32 dir)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FRONT_FACE,1);
	RSX_CONTEXT_CURRENTP[1] = dir;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxClearSurface(gcmContextData *context,u32 clear_mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLEAR_BUFFERS,1);
	RSX_CONTEXT_CURRENTP[1] = clear_mask;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_NOP,1);
	RSX_CONTEXT_CURRENTP[3] = 0;

	RSX_CONTEXT_CURRENT_END(4);
}

void rsxLoadVertexProgramBlock(gcmContextData *context,rsxVertexProgram *program,const void *ucode)
{
	u32 pos = 0;
	const u32 *data = (const u32*)ucode;
	u32 startIndex = program->start_insn;
	u32 i,instCount = program->num_insn;

	RSX_CONTEXT_CURRENT_BEGIN(6+5*instCount);

	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV40TCL_VP_UPLOAD_FROM_ID,2);
	RSX_CONTEXT_CURRENTP[pos++] = startIndex;
	RSX_CONTEXT_CURRENTP[pos++] = startIndex;

	for(i=0;i<instCount;i++) {
		RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV40TCL_VP_UPLOAD_INST(0),4);	
		RSX_CONTEXT_CURRENTP[pos++] = data[0];
		RSX_CONTEXT_CURRENTP[pos++] = data[1];
		RSX_CONTEXT_CURRENTP[pos++] = data[2];
		RSX_CONTEXT_CURRENTP[pos++] = data[3];
		data += 4;
	}
	
	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV40TCL_VP_ATTRIB_EN,2);
	RSX_CONTEXT_CURRENTP[pos++] = program->input_mask;
	RSX_CONTEXT_CURRENTP[pos++] = program->output_mask;

	RSX_CONTEXT_CURRENT_END(6+5*instCount);
}

void rsxLoadFragmentProgramLocation(gcmContextData *context,rsxFragmentProgram *program,u32 offset,u32 location)
{
	u32 i,fpcontrol;
	u32 texcoords,texcoord2D,texcoord3D;

	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FP_ADDRESS,1);
	RSX_CONTEXT_CURRENTP[1] = ((location + 1) | offset);
	RSX_CONTEXT_CURRENT_END(2);

	texcoords = program->texcoords;
	texcoord2D = program->texcoord2D;
	texcoord3D = program->texcoord3D;
	for(i=0;texcoords;i++) {
		if(texcoords&1) {
			RSX_CONTEXT_CURRENT_BEGIN(2);
			RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_COORD_CONTROL(i),1);
			RSX_CONTEXT_CURRENTP[1] = (((texcoord3D&1) << 4) | (texcoord2D&1));
			RSX_CONTEXT_CURRENT_END(2);
		}
		texcoords >>= 1;
		texcoord2D >>= 1;
		texcoord3D >>= 1;
	}

	{
		RSX_CONTEXT_CURRENT_BEGIN(2);

		fpcontrol = program->fp_control | (program->num_regs << NV40TCL_FP_CONTROL_TEMP_COUNT_SHIFT);
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FP_CONTROL,1);
		RSX_CONTEXT_CURRENTP[1] = fpcontrol;

		RSX_CONTEXT_CURRENT_END(2);
	}
}

void rsxLoadVertexProgramParameterBlock(gcmContextData *context,u32 base_const,u32 const_cnt,const f32 *value)
{
	u32 i,curr = 0;

	RSX_CONTEXT_CURRENT_BEGIN(const_cnt*6);

	for(i=0;i<const_cnt;i++) {
		RSX_CONTEXT_CURRENTP[curr+0] = RSX_METHOD(NV40TCL_VP_UPLOAD_CONST_ID,5);
		RSX_CONTEXT_CURRENTP[curr+1] = base_const + i;
		
		RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[curr+2],value,sizeof(f32)*4);

		value += 4;
		curr += 6;
	}
	RSX_CONTEXT_CURRENT_END(const_cnt*6);
}

void rsxLoadVertexProgram(gcmContextData *context,rsxVertexProgram *program,const void *ucode)
{
	u32 i;
	u32 base_const = program->const_start;
	rsxProgramConst *consts = rsxVertexProgramGetConsts(program);

	rsxLoadVertexProgramBlock(context,program,ucode);

	for(i=0;i<program->num_const;i++) {
		if(consts[i].is_internal)
			rsxLoadVertexProgramParameterBlock(context,consts[i].index + base_const,1,(f32*)consts[i].values);
	}
}

void rsxSetVertexProgramParameter(gcmContextData *context,rsxVertexProgram *program,s32 index,const f32 *value)
{
	u32 base_const = program->const_start;
	rsxProgramConst *consts = rsxVertexProgramGetConsts(program);

	rsxLoadVertexProgramParameterBlock(context,consts[index].index + base_const,consts[index].count,value);
}

void rsxSetFragmentProgramParameter(gcmContextData *context,rsxFragmentProgram *program,s32 index,const f32 *value,u32 offset)
{
	s32 i;
	f32 params[4] = {0.0f,0.0f,0.0f,0.0f};
	rsxProgramConst *consts = rsxFragmentProgramGetConsts(program);

	switch(consts[index].type) {
		case PARAM_FLOAT4x4:
		{
			s32 j,cnt = consts[index].count;
			for(j=0;j<cnt;j++) {
				if(consts[index + j].index!=0xffffffff) {
					rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(program,consts[index + j].index);

					params[0] = swapF32_16(value[(j*4) + 0]);
					params[1] = swapF32_16(value[(j*4) + 1]);
					params[2] = swapF32_16(value[(j*4) + 2]);
					params[3] = swapF32_16(value[(j*4) + 3]);

					for(i=0;i<co_table->num;i++)
						rsxInlineTransfer(context,offset + co_table->offset[i],params,4,GCM_LOCATION_RSX);
				}
			}
			return;
		}

		case PARAM_FLOAT4:
			params[3] = swapF32_16(value[3]);
		case PARAM_FLOAT3:
			params[2] = swapF32_16(value[2]);
		case PARAM_FLOAT2:
			params[1] = swapF32_16(value[1]);
		case PARAM_FLOAT:
			params[0] = swapF32_16(value[0]);
			break;
	}

	if(consts[index].index!=0xffffffff) {
		rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(program,consts[index].index);

		for(i=0;i<co_table->num;i++)
			rsxInlineTransfer(context,offset + co_table->offset[i],params,4,GCM_LOCATION_RSX);
	}
}

void rsxDrawVertexBegin(gcmContextData *context,u32 type)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[1] = type;
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxDrawVertexEnd(gcmContextData *context)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxDrawVertex2f(gcmContextData *context,u8 idx,f32 x,f32 y)
{
	ieee32 d[2];
	d[0].f = x; d[1].f = y;

	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_2F_X(idx),2);
	RSX_CONTEXT_CURRENTP[1] = d[0].u;
	RSX_CONTEXT_CURRENTP[2] = d[1].u;

	RSX_CONTEXT_CURRENT_END(3);
}

void rsxDrawVertex3f(gcmContextData *context,u8 idx,f32 x,f32 y,f32 z)
{
	ieee32 d[3];
	d[0].f = x; d[1].f = y; d[2].f = z;

	RSX_CONTEXT_CURRENT_BEGIN(4);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_3F_X(idx),3);
	RSX_CONTEXT_CURRENTP[1] = d[0].u;
	RSX_CONTEXT_CURRENTP[2] = d[1].u;
	RSX_CONTEXT_CURRENTP[3] = d[2].u;

	RSX_CONTEXT_CURRENT_END(4);
}

void rsxDrawVertex4f(gcmContextData *context,u8 idx,f32 x,f32 y,f32 z,f32 w)
{
	ieee32 d[4];
	d[0].f = x; d[1].f = y; d[2].f = z; d[3].f = w;

	RSX_CONTEXT_CURRENT_BEGIN(5);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_4F_X(idx),4);
	RSX_CONTEXT_CURRENTP[1] = d[0].u;
	RSX_CONTEXT_CURRENTP[2] = d[1].u;
	RSX_CONTEXT_CURRENTP[3] = d[2].u;
	RSX_CONTEXT_CURRENTP[4] = d[3].u;

	RSX_CONTEXT_CURRENT_END(5);
}

void rsxInvalidateTextureCache(gcmContextData *context,u32 type)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_CACHE_CTL,1);
	RSX_CONTEXT_CURRENTP[1] = type;
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxLoadTexture(gcmContextData *context,u8 index,const gcmTexture *texture)
{
	u32 format,offset,swizzle,size0,size1;

	RSX_CONTEXT_CURRENT_BEGIN(9);

	offset = texture->offset;
	format = ((texture->location + 1) | (texture->cubemap << 2) | 
			  (texture->dimension << NV40TCL_TEX_FORMAT_DIMS_SHIFT) |
			  (texture->format << NV40TCL_TEX_FORMAT_FORMAT_SHIFT) |
			  (texture->mipmap << NV40TCL_TEX_FORMAT_MIPMAP_COUNT_SHIFT) |
			  NV40TCL_TEX_FORMAT_NO_BORDER | 0x8000);
	swizzle = texture->remap;
	size0 = (texture->width << NV40TCL_TEX_SIZE0_W_SHIFT) | texture->height;
	size1 = (texture->depth << NV40TCL_TEX_SIZE1_DEPTH_SHIFT) | texture->pitch;

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_OFFSET(index),2);		// set offset and format for texture at once
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = format;

	RSX_CONTEXT_CURRENTP[3] = RSX_METHOD(NV40TCL_TEX_SWIZZLE(index),1);		// set remap order or swizzle respectively for texture
	RSX_CONTEXT_CURRENTP[4] = swizzle;

	RSX_CONTEXT_CURRENTP[5] = RSX_METHOD(NV40TCL_TEX_SIZE0(index),1);		// set width and height for texture
	RSX_CONTEXT_CURRENTP[6] = size0;

	RSX_CONTEXT_CURRENTP[7] = RSX_METHOD(NV40TCL_TEX_SIZE1(index),1);		// set pitch and depth for texture
	RSX_CONTEXT_CURRENTP[8] = size1;

	RSX_CONTEXT_CURRENT_END(9);
}

void rsxTextureControl(gcmContextData *context,u8 index,u32 enable,u16 minlod,u16 maxlod,u8 maxaniso)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_ENABLE(index),1);
	RSX_CONTEXT_CURRENTP[1] = ((enable << NV40TCL_TEX_ENABLE_SHIFT) | (minlod << NV40TCL_TEX_MINLOD_SHIFT) | (maxlod << NV40TCL_TEX_MAXLOD_SHIFT) | (maxaniso << NV40TCL_TEX_MAXANISO_SHIFT));
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxTextureFilter(gcmContextData *context,u8 index,u8 min,u8 mag,u8 conv)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_FILTER(index),1);
	RSX_CONTEXT_CURRENTP[1] = ((mag << NV40TCL_TEX_FILTER_MAG_SHIFT) | (min << NV40TCL_TEX_FILTER_MIN_SHIFT) | (conv << NV40TCL_TEX_FILTER_CONV_SHIFT));
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxTextureWrapMode(gcmContextData *context,u8 index,u8 wraps,u8 wrapt,u8 wrapr,u8 unsignedRemap,u8 zfunc,u8 gamma)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_WRAP(index),1);
	RSX_CONTEXT_CURRENTP[1] = ((wraps << NV40TCL_TEX_WRAP_S_SHIFT) |
							   (wrapt << NV40TCL_TEX_WRAP_T_SHIFT) |
							   (wrapr << NV40TCL_TEX_WRAP_R_SHIFT) |
							   (unsignedRemap << NV40TCL_TEX_UREMAP_SHIFT) |
							   (gamma << NV40TCL_TEX_GAMMA_SHIFT) |
							   (zfunc << NV40TCL_TEX_ZFUNC_SHIFT));
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxZControl(gcmContextData *context,u8 cullNearFar,u8 zClampEnable,u8 cullIgnoreW)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_CONTROL,1);
	RSX_CONTEXT_CURRENTP[1] = (cullNearFar | (zClampEnable<<4) | (cullIgnoreW<<8));
	RSX_CONTEXT_CURRENT_END(2);
}

void rsxBindVertexArrayAttrib(gcmContextData *context,u8 attr,u32 offset,u8 stride,u8 elems,u8 dtype,u8 location)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTXBUF_ADDRESS(attr),1);
	RSX_CONTEXT_CURRENTP[1] = ((location << 31) | offset);

	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_VTXFMT(attr),1);
	RSX_CONTEXT_CURRENTP[3] = ((stride << NV40TCL_VTXFMT_STRIDE_SHIFT) | (elems << NV40TCL_VTXFMT_SIZE_SHIFT) | dtype);

	RSX_CONTEXT_CURRENT_END(4);
}

void rsxDrawVertexArray(gcmContextData *context,u32 type,u32 start,u32 count)
{
	RSX_CONTEXT_CURRENT_BEGIN(6);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_CACHE_INVALIDATE,3);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENTP[2] = 0;
	RSX_CONTEXT_CURRENTP[3] = 0;
	
	RSX_CONTEXT_CURRENTP[4] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[5] = type;

	RSX_CONTEXT_CURRENT_END(6);

	while(count) {
		u32 rem = count;

		if(rem>256) rem = 256;

		RSX_CONTEXT_CURRENT_BEGIN(2);
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VB_VERTEX_BATCH,1);
		RSX_CONTEXT_CURRENTP[1] = (((rem - 1) << 24) | start);
		RSX_CONTEXT_CURRENT_END(2);

		count -= rem;
		start += rem;
	}

	{
		RSX_CONTEXT_CURRENT_BEGIN(2);

		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END,1);
		RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;

		RSX_CONTEXT_CURRENT_END(2);
	}
}

void rsxDrawIndexArray(gcmContextData *context,u32 type,u32 offset,u32 count,u32 data_type,u32 location)
{
	u32 start;

	RSX_CONTEXT_CURRENT_BEGIN(7);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_CACHE_INVALIDATE,1);
	RSX_CONTEXT_CURRENTP[1] = 0;

	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_VB_INDEX_BATCH_OFFSET,2);
	RSX_CONTEXT_CURRENTP[3] = offset;
	RSX_CONTEXT_CURRENTP[4] = (((data_type) << 4) | location);

	RSX_CONTEXT_CURRENTP[5] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[6] = type;

	RSX_CONTEXT_CURRENT_END(7);

	start = 0;
	while(count) {
		u32 rem = count;

		if(rem>256) rem = 256;

		RSX_CONTEXT_CURRENT_BEGIN(2);
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VB_INDEX_BATCH_DRAW,1);
		RSX_CONTEXT_CURRENTP[1] = (((rem - 1) << 24) | start);
		RSX_CONTEXT_CURRENT_END(2);

		count -= rem;
		start += rem;
	}

	{
		RSX_CONTEXT_CURRENT_BEGIN(2);

		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END,1);
		RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;

		RSX_CONTEXT_CURRENT_END(2);
	}
}

void rsxSetScissor(gcmContextData *context,u16 x,u16 y,u16 w,u16 h)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_SCISSOR_HORIZ,2);
	RSX_CONTEXT_CURRENTP[1] = ((w<<16) | x);
	RSX_CONTEXT_CURRENTP[2] = ((h<<16) | y);

	RSX_CONTEXT_CURRENT_END(3);
}

void rsxInlineTransfer(gcmContextData *context,const u32 dstOffset,const void *srcAddress,const u32 sizeInWords,const u8 location)
{
	u32 *src;
	u32 pos,cnt;
	u32 padSizeInWords;
	u32 alignedVideoOffset;
	u32 pixelShift;

	alignedVideoOffset = dstOffset&~0x3f;
	pixelShift = (dstOffset&0x3f)>>2;

	padSizeInWords = (sizeInWords + 1)&~0x01;

	RSX_CONTEXT_CURRENT_BEGIN(12 + padSizeInWords);

	pos = 0;
	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV3062TCL_SET_CONTEXT_DMA_IMAGE_DEST,1);
	RSX_CONTEXT_CURRENTP[pos++] = GCM_DMA_MEMORY_FRAME_BUFFER + location;

	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV3062TCL_SET_OFFSET_DEST,1);
	RSX_CONTEXT_CURRENTP[pos++] = alignedVideoOffset;

	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV3062TCL_SET_COLOR_FORMAT,2);
	RSX_CONTEXT_CURRENTP[pos++] = GCM_TRANSFER_SURFACE_FORMAT_Y32;
	RSX_CONTEXT_CURRENTP[pos++] = ((0x1000 << 16) | 0x1000);

	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV308ATCL_POINT,3);
	RSX_CONTEXT_CURRENTP[pos++] = ((0 << 16) | pixelShift);
	RSX_CONTEXT_CURRENTP[pos++] = ((1 << 16) | sizeInWords);
	RSX_CONTEXT_CURRENTP[pos++] = ((1 << 16) | sizeInWords);

	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV308ATCL_COLOR,padSizeInWords);

	cnt = 0;
	src = (u32*)srcAddress;
	while(cnt<sizeInWords) {
		RSX_CONTEXT_CURRENTP[pos++] = src[cnt];
		cnt++;
	}
	if(padSizeInWords!=sizeInWords)
		RSX_CONTEXT_CURRENTP[pos++] = 0;

	RSX_CONTEXT_CURRENT_END(12 + padSizeInWords);
}

void rsxSetBlendFunc(gcmContextData *context,u16 sfcolor,u16 dfcolor,u16 sfalpha,u16 dfalpha)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_FUNC_SRC,2);
	RSX_CONTEXT_CURRENTP[1] = ((sfalpha<<16) | sfcolor);
	RSX_CONTEXT_CURRENTP[2] = ((dfalpha<<16) | dfcolor);

	RSX_CONTEXT_CURRENT_END(3);
}

void rsxSetBlendEquation(gcmContextData *context,u16 color,u16 alpha)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_EQUATION,1);
	RSX_CONTEXT_CURRENTP[1] = ((alpha<<16) | color);

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetBlendColor(gcmContextData *context,u16 color0,u16 color1)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_COLOR,1);
	RSX_CONTEXT_CURRENTP[1] = color0;

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_COLOR2,1);
	RSX_CONTEXT_CURRENTP[1] = color1;

	RSX_CONTEXT_CURRENT_END(4);
}

void rsxSetBlendEnable(gcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetTransformBranchBits(gcmContextData *context,u32 branchBits)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_TRANSFORM_BRANCH_BITS,1);
	RSX_CONTEXT_CURRENTP[1] = branchBits;

	RSX_CONTEXT_CURRENT_END(2);
}

void rsxSetTransferData(gcmContextData *context,u8 mode,u32 dst,u32 outpitch,u32 src,u32 inpitch,u32 linelength,u32 linecount)
{
	RSX_CONTEXT_CURRENT_BEGIN(12);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,2);
	RSX_CONTEXT_CURRENTP[1] = (mode&0x01) ? GCM_DMA_MEMORY_HOST_BUFFER : GCM_DMA_MEMORY_FRAME_BUFFER;
	RSX_CONTEXT_CURRENTP[2] = (mode&0x02) ? GCM_DMA_MEMORY_HOST_BUFFER : GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENTP[3] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_IN,8);
	RSX_CONTEXT_CURRENTP[4] = src;
	RSX_CONTEXT_CURRENTP[5] = dst;
	RSX_CONTEXT_CURRENTP[6] = inpitch;
	RSX_CONTEXT_CURRENTP[7] = outpitch;
	RSX_CONTEXT_CURRENTP[8] = linelength;
	RSX_CONTEXT_CURRENTP[9] = linecount;
	RSX_CONTEXT_CURRENTP[10] = (((u32)1<<8) | 1);
	RSX_CONTEXT_CURRENTP[11] = 0;

	RSX_CONTEXT_CURRENT_END(12);
}

void rsxSetTransferDataMode(gcmContextData *context,u8 mode)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,2);
	RSX_CONTEXT_CURRENTP[1] = (mode&0x01) ? GCM_DMA_MEMORY_HOST_BUFFER : GCM_DMA_MEMORY_FRAME_BUFFER;
	RSX_CONTEXT_CURRENTP[2] = (mode&0x02) ? GCM_DMA_MEMORY_HOST_BUFFER : GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENT_END(3);
}

void rsxSetTransferDataOffset(gcmContextData *context,u32 dst,u32 src)
{
	RSX_CONTEXT_CURRENT_BEGIN(5);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_IN,2);
	RSX_CONTEXT_CURRENTP[1] = src;
	RSX_CONTEXT_CURRENTP[2] = dst;

	RSX_CONTEXT_CURRENTP[3] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_BUF_NOTIFY,1);
	RSX_CONTEXT_CURRENTP[4] = 0;

	RSX_CONTEXT_CURRENT_END(5);
}

void rsxSetTransferDataFormat(gcmContextData *context,s32 inpitch,s32 outpitch,u32 linelength,u32 linecount,u8 inbytes,u8 outbytes)
{
	RSX_CONTEXT_CURRENT_BEGIN(6);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_PITCH_IN,5);
	RSX_CONTEXT_CURRENTP[1] = inpitch;
	RSX_CONTEXT_CURRENTP[2] = outpitch;
	RSX_CONTEXT_CURRENTP[3] = linelength;
	RSX_CONTEXT_CURRENTP[4] = linecount;
	RSX_CONTEXT_CURRENTP[5] = (((u32)outbytes<<8) | inbytes);

	RSX_CONTEXT_CURRENT_END(6);
}

void rsxSetTransferImage(gcmContextData *context,const u8 mode,const u32 dstOffset,const u32 dstPitch,const u32 dstX,const u32 dstY,const u32 srcOffset,const u32 srcPitch,const u32 srcX,const u32 srcY,const u32 width,const u32 height,const u32 bytesPerPixel)
{
	RSX_CONTEXT_CURRENT_BEGIN(26);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(6,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,1);
	RSX_CONTEXT_CURRENTP[1] = (mode&0x01) ? GCM_DMA_MEMORY_HOST_BUFFER : GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENTP[2] = RSX_SUBCHANNEL_METHOD(3,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_OUT,1);
	RSX_CONTEXT_CURRENTP[3] = (mode&0x02) ? GCM_DMA_MEMORY_HOST_BUFFER : GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENTP[4] = RSX_SUBCHANNEL_METHOD(6,NV01_IMAGE_FROM_CPU_SURFACE,1);
	RSX_CONTEXT_CURRENTP[5] = GCM_CONTEXT_SURFACE2D;

	RSX_CONTEXT_CURRENTP[6] = RSX_SUBCHANNEL_METHOD(3,NV04_CONTEXT_SURFACES_2D_OFFSET_DESTIN,1);
	RSX_CONTEXT_CURRENTP[7] = dstOffset;

	RSX_CONTEXT_CURRENTP[8] = RSX_SUBCHANNEL_METHOD(3,NV04_CONTEXT_SURFACES_2D_FORMAT,2);
	RSX_CONTEXT_CURRENTP[9] = (bytesPerPixel==4) ? GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8 : ((bytesPerPixel==2) ? GCM_TRANSFER_SURFACE_FORMAT_R5G5B5 : 0);
	RSX_CONTEXT_CURRENTP[10] = ((dstPitch << 16) | dstPitch);

	RSX_CONTEXT_CURRENTP[11] = RSX_SUBCHANNEL_METHOD(6,NV01_IMAGE_FROM_CPU_OPERATION,9);
	RSX_CONTEXT_CURRENTP[12] = GCM_TRANSFER_CONVERSION_TRUNCATE;
	RSX_CONTEXT_CURRENTP[13] = (bytesPerPixel==4) ? GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8 : ((bytesPerPixel==2) ? GCM_TRANSFER_SURFACE_FORMAT_R5G5B5 : 0);
	RSX_CONTEXT_CURRENTP[14] = GCM_TRANSFER_OPERATION_SRCCOPY;
	RSX_CONTEXT_CURRENTP[15] = ((dstY << 16) | dstX);
	RSX_CONTEXT_CURRENTP[16] = ((height << 16) | width);
	RSX_CONTEXT_CURRENTP[17] = ((dstY << 16) | dstX);
	RSX_CONTEXT_CURRENTP[18] = ((height << 16) | width);
	RSX_CONTEXT_CURRENTP[19] = (16 << 16);
	RSX_CONTEXT_CURRENTP[20] = (16 << 16);

	RSX_CONTEXT_CURRENTP[21] = RSX_SUBCHANNEL_METHOD(6,NV03_SCALED_IMAGE_FROM_MEMORY_IMAGE_IN_SIZE,4);
	RSX_CONTEXT_CURRENTP[22] = ((height << 16) | width);
	RSX_CONTEXT_CURRENTP[23] = (srcPitch | (GCM_TRANSFER_ORIGIN_CORNER << 16) | (GCM_TRANSFER_INTERPOLATOR_NEAREST << 24));
	RSX_CONTEXT_CURRENTP[24] = srcOffset;
	RSX_CONTEXT_CURRENTP[25] = 0;

	RSX_CONTEXT_CURRENT_END(26);
}

void rsxSetTransferScaleMode(gcmContextData *context,const u8 mode,const u8 surface)
{
	RSX_CONTEXT_CURRENT_BEGIN(6);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(6,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,1);
	RSX_CONTEXT_CURRENTP[1] = (mode&0x01) ? GCM_DMA_MEMORY_HOST_BUFFER : GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENTP[2] = (surface==GCM_TRANSFER_SWIZZLE) ? RSX_SUBCHANNEL_METHOD(4,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,1) : RSX_SUBCHANNEL_METHOD(3,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_OUT,1);
	RSX_CONTEXT_CURRENTP[3] = (mode&0x02) ? GCM_DMA_MEMORY_HOST_BUFFER : GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENTP[4] = RSX_SUBCHANNEL_METHOD(6,NV01_IMAGE_FROM_CPU_SURFACE,1);
	RSX_CONTEXT_CURRENTP[5] = (surface==GCM_TRANSFER_SWIZZLE) ? GCM_CONTEXT_SWIZZLE2D : GCM_CONTEXT_SURFACE2D;

	RSX_CONTEXT_CURRENT_END(6);
}

void rsxSetTransferScaleSurface(gcmContextData *context,const gcmTransferScale *scale,const gcmTransferSurface *surface)
{
	RSX_CONTEXT_CURRENT_BEGIN(20);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(3,NV04_CONTEXT_SURFACES_2D_FORMAT,4);
	RSX_CONTEXT_CURRENTP[1] = surface->format;
	RSX_CONTEXT_CURRENTP[2] = ((surface->pitch << 16) | 0x40);	// or'ing with 64 - why?
	RSX_CONTEXT_CURRENTP[3] = 0;
	RSX_CONTEXT_CURRENTP[4] = surface->offset;

	RSX_CONTEXT_CURRENTP[5] = RSX_SUBCHANNEL_METHOD(6,NV03_STRETCHED_IMAGE_FROM_CPU_OPERATION,9);
	RSX_CONTEXT_CURRENTP[6] = scale->conversion;
	RSX_CONTEXT_CURRENTP[7] = scale->format;
	RSX_CONTEXT_CURRENTP[8] = scale->operation;
	RSX_CONTEXT_CURRENTP[9] = ((scale->clipY << 16) | scale->clipX);
	RSX_CONTEXT_CURRENTP[10] = ((scale->clipH << 16) | scale->clipW);
	RSX_CONTEXT_CURRENTP[11] = ((scale->outY << 16) | scale->outX);
	RSX_CONTEXT_CURRENTP[12] = ((scale->outH << 16) | scale->outW);
	RSX_CONTEXT_CURRENTP[13] = scale->ratioX;
	RSX_CONTEXT_CURRENTP[14] = scale->ratioY;

	RSX_CONTEXT_CURRENTP[15] = RSX_SUBCHANNEL_METHOD(6,NV03_SCALED_IMAGE_FROM_MEMORY_IMAGE_IN_SIZE,4);
	RSX_CONTEXT_CURRENTP[16] = ((scale->inH << 16) | scale->inW);
	RSX_CONTEXT_CURRENTP[17] = ((scale->pitch) | (scale->origin << 16) | (scale->interp << 24));
	RSX_CONTEXT_CURRENTP[18] = scale->offset;
	RSX_CONTEXT_CURRENTP[19] = ((scale->inY << 16) | scale->inX);

	RSX_CONTEXT_CURRENT_END(20);
}

#if 0
// This is unfinished
void rsxSetTransferScaleSwizzle(gcmContextData *context,const gcmTransferScale *scale,const gcmTransferSwizzle *swizzle)
{
	RSX_CONTEXT_CURRENT_BEGIN(20);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(4,0x300,2);
	RSX_CONTEXT_CURRENTP[1] = (swizzle->format | (swizzle->width << 16) | (swizzle->height << 24));
	RSX_CONTEXT_CURRENTP[2] = swizzle->offset;

	RSX_CONTEXT_CURRENTP[3] = RSX_SUBCHANNEL_METHOD(6,0x2fc,9);;
	RSX_CONTEXT_CURRENTP[4] = GCM_TRANSFER_CONVERSION_TRUNCATE;
	RSX_CONTEXT_CURRENTP[5] = scale->format;
	RSX_CONTEXT_CURRENTP[6] = GCM_TRANSFER_OPERATION_SRCCOPY;
	RSX_CONTEXT_CURRENTP[7] = ((scale->clipY << 16) | scale->clipX);
	RSX_CONTEXT_CURRENTP[8] = ((scale->clipH << 16) | scale->clipW);
	RSX_CONTEXT_CURRENTP[9] = ((scale->outY << 16) | scale->outX);
	RSX_CONTEXT_CURRENTP[10] = ((scale->outH << 16) | scale->outW);
	RSX_CONTEXT_CURRENTP[11] = scale->ratioX;
	RSX_CONTEXT_CURRENTP[12] = scale->ratioY;

	RSX_CONTEXT_CURRENTP[13] = RSX_SUBCHANNEL_METHOD(6,0x400,4);
	RSX_CONTEXT_CURRENTP[14] = ((scale->inH << 16) | scale->inW);
	RSX_CONTEXT_CURRENTP[15] = ((scale->pitch) | (scale->origin << 16) | (scale->interp << 24));
	RSX_CONTEXT_CURRENTP[16] = scale->offset;
	RSX_CONTEXT_CURRENTP[17] = ((scale->inY << 16) | scale->inX);

	RSX_CONTEXT_CURRENT_END(20);
}
#endif

void rsxSetTimeStamp(gcmContextData *context,u32 index)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_QUERY_GET,1);
	RSX_CONTEXT_CURRENTP[2] = (((index << 4)&0x0fffffff) | 0x10000000);

	RSX_CONTEXT_CURRENT_END(2);
}
