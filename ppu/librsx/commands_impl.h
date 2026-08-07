void RSX_FUNC(SetReturnCommand)(CellGcmContextData *context)
{
	RSX_CONTEXT_CURRENT_BEGIN(1);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_FLAG_RETURN;
	RSX_CONTEXT_CURRENT_END(1);
}

void RSX_FUNC(SetCallCommand)(CellGcmContextData *context,u32 offset)
{
	RSX_CONTEXT_CURRENT_BEGIN(1);
	RSX_CONTEXT_CURRENTP[0] = (offset | RSX_METHOD_FLAG_CALL);
	RSX_CONTEXT_CURRENT_END(1);
}

void RSX_FUNC(SetJumpCommand)(CellGcmContextData *context,u32 offset)
{
	RSX_CONTEXT_CURRENT_BEGIN(1);
	RSX_CONTEXT_CURRENTP[0] = (offset | RSX_METHOD_FLAG_JUMP);
	RSX_CONTEXT_CURRENT_END(1);
}

void RSX_FUNC(SetNopCommand)(CellGcmContextData *context,u32 count)
{
	u32 i;

	RSX_CONTEXT_CURRENT_BEGIN(count);
	for(i=0;i<count;i++)
		RSX_CONTEXT_CURRENTP[i] = 0;
	RSX_CONTEXT_CURRENT_END(count);
}
 
void RSX_FUNC(SetSkipNop)(CellGcmContextData *context,u32 count)
{
	RSX_CONTEXT_CURRENT_BEGIN(1 + count);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_NOP, count);
	RSX_CONTEXT_CURRENT_END(1 + count);
}

void RSX_FUNC(SetClearColor)(CellGcmContextData *context,u32 color)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLEAR_VALUE_COLOR,1);
	RSX_CONTEXT_CURRENTP[1] = color;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetClearDepthStencil)(CellGcmContextData *context,u32 value)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLEAR_VALUE_DEPTH,1);
	RSX_CONTEXT_CURRENTP[1] = value;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetReferenceCommand)(CellGcmContextData *context,u32 ref_value)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV406ETCL_SET_REF,1);
	RSX_CONTEXT_CURRENTP[1] = ref_value;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetWriteBackendLabel)(CellGcmContextData *context,u8 index,u32 value)
{
	u32 offset = 0x10*index;
	u32 wvalue = (value&0xff00ff00) | ((value>>16)&0xff) | ((value&0xff)<<16);

	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_SEMAPHORE_OFFSET,1);
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_SEMAPHORE_BACKENDWRITE_RELEASE,1);
	RSX_CONTEXT_CURRENTP[3] = wvalue;
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetWriteTextureLabel)(CellGcmContextData *context,u8 index,u32 value)
{
	u32 offset = 0x10*index;

	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_SEMAPHORE_OFFSET,1);
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_SEMAPHORE_TEXTUREREAD_RELEASE,1);
	RSX_CONTEXT_CURRENTP[3] = value;
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetWaitLabel)(CellGcmContextData *context,u8 index,u32 value)
{
	u32 offset = 0x10*index;

	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV406ETCL_SEMAPHORE_OFFSET,1);
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV406ETCL_SEMAPHORE_ACQUIRE,1);
	RSX_CONTEXT_CURRENTP[3] = value;
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetWriteCommandLabel)(CellGcmContextData *context,u8 index,u32 value)
{
	u32 offset = 0x10*index;

	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV406ETCL_SEMAPHORE_OFFSET,1);
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV406ETCL_SEMAPHORE_RELEASE,1);
	RSX_CONTEXT_CURRENTP[3] = value;
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetSurface)(CellGcmContextData *context,const CellGcmSurface *surface)
{
	u32 log2Width = 31 - __cntlzw(surface->width);
	u32 log2Height = 31 - __cntlzw(surface->height);

	RSX_CONTEXT_CURRENT_BEGIN(32);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DMA_COLOR0,1);
	RSX_CONTEXT_CURRENTP[1] = CELL_GCM_DMA_MEMORY_FRAME_BUFFER + surface->colorLocation[0];

	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_DMA_COLOR1,1);
	RSX_CONTEXT_CURRENTP[3] = CELL_GCM_DMA_MEMORY_FRAME_BUFFER + surface->colorLocation[1];

	RSX_CONTEXT_CURRENTP[4] = RSX_METHOD(NV40TCL_DMA_COLOR2,2);
	RSX_CONTEXT_CURRENTP[5] = CELL_GCM_DMA_MEMORY_FRAME_BUFFER + surface->colorLocation[2];
	RSX_CONTEXT_CURRENTP[6] = CELL_GCM_DMA_MEMORY_FRAME_BUFFER + surface->colorLocation[3];

	RSX_CONTEXT_CURRENTP[7] = RSX_METHOD(NV40TCL_DMA_ZETA,1);
	RSX_CONTEXT_CURRENTP[8] = CELL_GCM_DMA_MEMORY_FRAME_BUFFER + surface->depthLocation;

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

void RSX_FUNC(SetColorMask)(CellGcmContextData *context,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_COLOR_MASK,1);
	RSX_CONTEXT_CURRENTP[1] = mask;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetColorMaskMrt)(CellGcmContextData *context,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_MRT_COLOR_MASK,1);
	RSX_CONTEXT_CURRENTP[1] = mask;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetShadeModel)(CellGcmContextData *context,u32 shadeModel)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_SHADE_MODEL,1);
	RSX_CONTEXT_CURRENTP[1] = shadeModel;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetViewport)(CellGcmContextData *context,u16 x,u16 y,u16 width,u16 height,f32 min,f32 max,const f32 scale[4],const f32 offset[4])
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

void RSX_FUNC(SetViewportClip)(CellGcmContextData *context,u8 sel,u16 width,u16 height)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VIEWPORT_CLIP_HORIZ(sel),2);
	RSX_CONTEXT_CURRENTP[1] = ((width-1) << 16);
	RSX_CONTEXT_CURRENTP[2] = ((height-1) << 16);
	
	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(SetUserClipPlaneControl)(CellGcmContextData *context,u32 plane0,u32 plane1,u32 plane2,u32 plane3,u32 plane4,u32 plane5)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLIP_PLANE_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = ((plane5 << 20) | (plane4 << 16) | (plane3 << 12) | (plane2 << 8) | (plane1 << 4) | plane0);
	
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetDepthTestEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_TEST_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetDepthFunc)(CellGcmContextData *context,u32 func)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_FUNC,1);
	RSX_CONTEXT_CURRENTP[1] = func;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetDepthWriteEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_WRITE_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetDepthFormat)(CellGcmContextData *context,u32 format)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CONTROL0,1);
	RSX_CONTEXT_CURRENTP[1] = ((format&1)<<12) | 0x00100000;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetCullFaceEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CULL_FACE_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetCullFace)(CellGcmContextData *context,u32 cull)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CULL_FACE,1);
	RSX_CONTEXT_CURRENTP[1] = cull;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetFrontFace)(CellGcmContextData *context,u32 dir)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FRONT_FACE,1);
	RSX_CONTEXT_CURRENTP[1] = dir;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetFrontPolygonMode)(CellGcmContextData *context,u32 mode)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_POLYGON_MODE_FRONT,1);
	RSX_CONTEXT_CURRENTP[1] = mode;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetBackPolygonMode)(CellGcmContextData *context,u32 mode)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_POLYGON_MODE_BACK,1);
	RSX_CONTEXT_CURRENTP[1] = mode;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetPolygonOffsetFillEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_POLYGON_OFFSET_FILL_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetPolygonOffset)(CellGcmContextData *context,f32 factor,f32 units)
{
	ieee32 d0,d1;

	d0.f = factor;
	d1.f = units;

	RSX_CONTEXT_CURRENT_BEGIN(3);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_POLYGON_OFFSET_FACTOR,2);
	RSX_CONTEXT_CURRENTP[1] = d0.u;
	RSX_CONTEXT_CURRENTP[2] = d1.u;
	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(SetPolygonOffsetLineEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_POLYGON_OFFSET_LINE_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(ClearSurface)(CellGcmContextData *context,u32 clear_mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLEAR_BUFFERS,1);
	RSX_CONTEXT_CURRENTP[1] = clear_mask;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_NOP,1);
	RSX_CONTEXT_CURRENTP[3] = 0;

	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetCylindricalWrap)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CYLINDRICAL_WRAP,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetTwoSideLightEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TWO_SIDE_LIGHT_EN,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetStencilFunc)(CellGcmContextData *context,u32 func,u32 ref,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_STENCIL_BACK_FUNC_FUNC,3);
	RSX_CONTEXT_CURRENTP[1] = func;
	RSX_CONTEXT_CURRENTP[2] = ref;
	RSX_CONTEXT_CURRENTP[3] = mask;
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetStencilMask)(CellGcmContextData *context,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_STENCIL_BACK_MASK,1);
	RSX_CONTEXT_CURRENTP[1] = mask;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetStencilOp)(CellGcmContextData *context,u32 fail,u32 depthFail,u32 depthPass)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_STENCIL_BACK_OP_FAIL,3);
	RSX_CONTEXT_CURRENTP[1] = fail;
	RSX_CONTEXT_CURRENTP[2] = depthFail;
	RSX_CONTEXT_CURRENTP[3] = depthPass;
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetStencilTestEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_STENCIL_BACK_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetBackStencilFunc)(CellGcmContextData *context,u32 func,u32 ref,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_STENCIL_FRONT_FUNC_FUNC,3);
	RSX_CONTEXT_CURRENTP[1] = func;
	RSX_CONTEXT_CURRENTP[2] = ref;
	RSX_CONTEXT_CURRENTP[3] = mask;
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetBackStencilMask)(CellGcmContextData *context,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_STENCIL_FRONT_MASK,1);
	RSX_CONTEXT_CURRENTP[1] = mask;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetBackStencilOp)(CellGcmContextData *context,u32 fail,u32 depthFail,u32 depthPass)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_STENCIL_FRONT_OP_FAIL,3);
	RSX_CONTEXT_CURRENTP[1] = fail;
	RSX_CONTEXT_CURRENTP[2] = depthFail;
	RSX_CONTEXT_CURRENTP[3] = depthPass;
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetTwoSidedStencilTestEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_STENCIL_FRONT_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetRenderEnable)(CellGcmContextData *context,u8 mode,u32 index)
{
	u32 offset = 0x10*index;
	
	if(mode == CELL_GCM_CONDITIONAL) {
		RSX_CONTEXT_CURRENT_BEGIN(4);
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_NOP,1);
		RSX_CONTEXT_CURRENTP[1] = 0;
		RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_RENDER_ENABLE,1);
		RSX_CONTEXT_CURRENTP[3] = (0x2000000 | offset);
		RSX_CONTEXT_CURRENT_END(4);
	} else {
		RSX_CONTEXT_CURRENT_BEGIN(2);
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_RENDER_ENABLE,1);
		RSX_CONTEXT_CURRENTP[1] = 0x1000000;
		RSX_CONTEXT_CURRENT_END(2);
	}
}

void RSX_FUNC(SetReport)(CellGcmContextData *context,u32 type,u32 index)
{
	u32 offset = 0x10*index;

	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_QUERY_GET,1);
	RSX_CONTEXT_CURRENTP[1] = ((type<<24) | offset);
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetClearReport)(CellGcmContextData *context,u32 type)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_QUERY_RESET,1);
	RSX_CONTEXT_CURRENTP[1] = type;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetZPixelCountEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_QUERY_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetSCullControl)(CellGcmContextData *context,u8 sFunc,u8 sRef,u8 sMask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_SCULL_CONTROL,1);
	RSX_CONTEXT_CURRENTP[1] = ((sMask<<24) | (sRef<<16) | sFunc);
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetZCullLimit)(CellGcmContextData *context,u16 moveforwardlimit,u16 pushbacklimit)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_ZCULL_CONTROL1,1);
	RSX_CONTEXT_CURRENTP[1] = ((moveforwardlimit<<16) | pushbacklimit);
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetZCullStatsEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_ZCULL_STATS_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetZCullControl)(CellGcmContextData *context,u8 zculldir,u8 zcullformat)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_ZCULL_CONTROL0,1);
	RSX_CONTEXT_CURRENTP[1] = ((zcullformat<<4) | zculldir);
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetClearZCullSurface)(CellGcmContextData *context,u32 depth,u32 stencil)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_CLEAR_ZCULL_SURFACE,1);
	RSX_CONTEXT_CURRENTP[1] = ((stencil<<1) | depth);
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetZCullEnable)(CellGcmContextData *context,u32 depth,u32 stencil)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_ZCULL_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = ((stencil<<1) | depth);
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetZCullInvalidate)(CellGcmContextData *context)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_INVALIDATE_ZCULL,1);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetPolygonSmoothEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_POLYGON_SMOOTH_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetDitherEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DITHER_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(LoadVertexProgramBlock)(CellGcmContextData *context,const rsxVertexProgram *program,const void *ucode)
{
	u32 pos = 0;
	u32 loop, rest;
	const u32 *data = (const u32*)ucode;
	u32 startIndex = program->insn_start;
	u32 i,j,instCount = program->num_insn;
	
	loop = instCount/8;
	rest = (instCount%8)*4;
	
	RSX_CONTEXT_CURRENT_BEGIN(9 + loop*33 + (rest!=0 ? rest + 1 : 0));
	
	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV40TCL_VP_UPLOAD_FROM_ID,2);
	RSX_CONTEXT_CURRENTP[pos++] = startIndex;
	RSX_CONTEXT_CURRENTP[pos++] = startIndex;
	
	for(i=0;i<loop;i++) {
		RSX_CONTEXT_CURRENTP[pos] = RSX_METHOD(NV40TCL_VP_UPLOAD_INST(0),32);
		RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[pos+1],&data[0],sizeof(u32)*16);
		RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[pos+17],&data[16],sizeof(u32)*16);
		pos += (1+32);
		data += 32;
	}

	if(rest>0) {
		RSX_CONTEXT_CURRENTP[pos] = RSX_METHOD(NV40TCL_VP_UPLOAD_INST(0),rest);
		for(j=0;j<rest;j++)
			RSX_CONTEXT_CURRENTP[pos+j+1] = data[j];
		pos += (1+rest);
	}
	
	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV40TCL_VP_ATTRIB_EN,1);
	RSX_CONTEXT_CURRENTP[pos++] = program->input_mask;
	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV40TCL_VP_RESULT_EN,1);
	RSX_CONTEXT_CURRENTP[pos++] = (program->output_mask | CELL_GCM_ATTRIB_OUTPUT_MASK_POINTSIZE);

	RSX_CONTEXT_CURRENTP[pos++] = RSX_METHOD(NV40TCL_TRANSFORM_TIMEOUT,1);
	if(program->num_regs<=32)
		RSX_CONTEXT_CURRENTP[pos++] = 0x0020FFFF;
	else
		RSX_CONTEXT_CURRENTP[pos++] = 0x0030FFFF;

	RSX_CONTEXT_CURRENT_END(9 + loop*33 + (rest!=0 ? rest + 1 : 0));
}

void RSX_FUNC(LoadFragmentProgramLocation)(CellGcmContextData *context,const rsxFragmentProgram *program,u32 offset,u32 location)
{
	u32 i;
	u32 texcoords,texcoord2D,texcoord3D;

	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FP_ADDRESS,1);
	RSX_CONTEXT_CURRENTP[1] = ((location + 1) | (offset&0x1fffffff));
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
		u32 num_regs = program->num_regs > 2 ? program->num_regs : 2;
		u32 fpcontrol = program->fp_control | (num_regs << NV40TCL_FP_CONTROL_TEMP_COUNT_SHIFT) | (1<<10);

		RSX_CONTEXT_CURRENT_BEGIN(2);
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FP_CONTROL,1);
		RSX_CONTEXT_CURRENTP[1] = fpcontrol;
		RSX_CONTEXT_CURRENT_END(2);
	}
}

void RSX_FUNC(UpdateFragmentProgramLocation)(CellGcmContextData *context,u32 offset,u32 location)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FP_ADDRESS,1);
	RSX_CONTEXT_CURRENTP[1] = ((location + 1) | (offset&0x1fffffff));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetFragmentProgramControl)(CellGcmContextData *context,const rsxFragmentProgram *program,u8 allowTex,u8 reserved0,u8 reserved1)
{
	u32 num_regs = program->num_regs > 2 ? program->num_regs : 2;
	u32 fpcontrol = program->fp_control | ((allowTex<<15)&0x00008000) | (num_regs << NV40TCL_FP_CONTROL_TEMP_COUNT_SHIFT) | (1<<10);

	(void)reserved0;
	(void)reserved1;

	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FP_CONTROL,1);
	RSX_CONTEXT_CURRENTP[1] = fpcontrol;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(LoadVertexProgramParameterBlock)(CellGcmContextData *context,u32 base_const,u32 const_cnt,const f32 *value)
{
	u32 i, curr = 0;
	u32 block_cnt = ((const_cnt*4)>>5);
	u32 block_rem = ((const_cnt*4)&0x1f);
	u32 reserve = block_cnt*34 + (block_rem!=0 ? 2 + block_rem : 0);

	RSX_CONTEXT_CURRENT_BEGIN(reserve);

	for(i=0;i<block_cnt;i++) {
		u32 loadAt = base_const + i*8;

		RSX_CONTEXT_CURRENTP[curr+0] = RSX_METHOD(NV40TCL_VP_UPLOAD_CONST_ID,33);
		RSX_CONTEXT_CURRENTP[curr+1] = loadAt;

		RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[curr+2],value,sizeof(f32)*16);
		RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[curr+18],&value[16],sizeof(f32)*16);
		curr += 34;
		value += 32;
	}

	if (block_rem) {
		RSX_CONTEXT_CURRENTP[curr++] = RSX_METHOD(NV40TCL_VP_UPLOAD_CONST_ID,block_rem + 1);
		RSX_CONTEXT_CURRENTP[curr++] = base_const + block_cnt*8;

		block_rem >>= 2;
		for(i=0;i < block_rem;i++) {
			RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[curr],value,sizeof(f32)*4);
			curr += 4;
			value += 4;
		}
	}

	RSX_CONTEXT_CURRENT_END(reserve);
}

void RSX_FUNC(LoadVertexProgram)(CellGcmContextData *context,const rsxVertexProgram *program,const void *ucode)
{
	u32 i;
	u32 base_const = program->const_start;
	rsxProgramConst *consts = rsxVertexProgramGetConsts(program);

	RSX_FUNC(LoadVertexProgramBlock)(context,program,ucode);

	for(i=0;i<program->num_const;i++) {
		if(consts[i].is_internal)
			RSX_FUNC(LoadVertexProgramParameterBlock)(context,consts[i].index + base_const,1,(f32*)consts[i].values);
	}
}

static inline __attribute__((always_inline)) void RSX_FUNC_INTERNAL(SetVertexProgramParameter)(CellGcmContextData *context,const rsxVertexProgram *program,const rsxProgramConst *param,const f32 *value)
{
	u32 base_const = program->const_start;
	f32 params[4] = {0.0f,0.0f,0.0f,0.0f};

	switch(param->type) {
		case PARAM_FLOAT3x4:
		case PARAM_FLOAT4x4:
			RSX_FUNC(LoadVertexProgramParameterBlock)(context,param->index + base_const,param->count,value);
			return;
		case PARAM_FLOAT3x3:
		case PARAM_FLOAT4x3:
		{
			u32 i;
			
			for(i=0;i<param->count;i++,value+=3) {
				params[0] = value[0];
				params[1] = value[1];
				params[2] = value[2];
				RSX_FUNC(LoadVertexProgramParameterBlock)(context,param->index + base_const + i,1,params);
			}
			return;
		}
		
		case PARAM_FLOAT4:
			RSX_FUNC(LoadVertexProgramParameterBlock)(context,param->index + base_const,1,value);
			return;
		case PARAM_FLOAT3:
			params[2] = value[2];
		case PARAM_FLOAT2:
			params[1] = value[1];
		case PARAM_FLOAT:
		case PARAM_FLOAT1:
			params[0] = value[0];
	}
	RSX_FUNC(LoadVertexProgramParameterBlock)(context,param->index + base_const,1,params);
}

void RSX_FUNC(SetVertexProgramParameter)(CellGcmContextData *context,const rsxVertexProgram *program,const rsxProgramConst *param,const f32 *value)
{
	RSX_FUNC_INTERNAL(SetVertexProgramParameter)(context, program, param, value);
}

void RSX_FUNC(SetVertexProgramConstants)(CellGcmContextData *context,u32 start,u32 count,const f32 *data)
{
	u32 i;
	u32 loop = count>>5;
	u32 rest = count&0x1f;
	const f32* __restrict value = data;
	u32 reserve = loop*34 + (rest!=0 ? 2 + rest : 0);

	RSX_CONTEXT_CURRENT_BEGIN(reserve);

	for(i=0;i < loop;i++) {
		u32 loadAt = start + i*8;

		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_UPLOAD_CONST_ID,33);
		RSX_CONTEXT_CURRENTP[1] = loadAt;

		RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[2],value,sizeof(f32)*16);
		RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[18],value,sizeof(f32)*16);
		RSX_CONTEXT_CURRENTP += 34;
		value += 32;
	}

	if (rest) {
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_UPLOAD_CONST_ID,rest + 1);
		RSX_CONTEXT_CURRENTP[1] = start + (loop<<3);
		RSX_CONTEXT_CURRENTP += 2;

		for(i=0;i < rest;i++) {
			RSX_CONTEXT_CURRENTP[0] = *value;
			RSX_CONTEXT_CURRENTP++;
			value++;
		}
	}

	RSX_CONTEXT_CURRENT_END(reserve);
}

void RSX_FUNC(SetVertexAttribOutputMask)(CellGcmContextData *context,u32 mask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_RESULT_EN,1);
	RSX_CONTEXT_CURRENTP[1] = mask;
	RSX_CONTEXT_CURRENT_END(2);
}

static inline __attribute__((always_inline)) void RSX_FUNC_INTERNAL(SetFragmentProgramParameter)(CellGcmContextData *context,const rsxFragmentProgram *program,const rsxProgramConst *param,const f32 *value,u32 offset,u32 location)
{
	s32 i;
	f32 params[4] = {0.0f,0.0f,0.0f,0.0f};

	switch(param->type) {
		case PARAM_FLOAT:
		case PARAM_FLOAT1:
		case PARAM_BOOL:
		case PARAM_BOOL1:
		{
			if(param->index!=0xffffffff) {
				rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(program,param->index);

				params[0] = swapF32_16(value[0]);

				for(i=0;i<co_table->num;i++)
					RSX_FUNC(InlineTransfer)(context,offset + co_table->offset[i],params,1,location);
			}
			return;
		}

		case PARAM_FLOAT2:
		case PARAM_BOOL2:
		{
			if(param->index!=0xffffffff) {
				rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(program,param->index);

				params[0] = swapF32_16(value[0]);
				params[1] = swapF32_16(value[1]);

				for(i=0;i<co_table->num;i++)
					RSX_FUNC(InlineTransfer)(context,offset + co_table->offset[i],params,2,location);
			}
			return;
		}

		case PARAM_FLOAT3:
		case PARAM_BOOL3:
		{
			if(param->index!=0xffffffff) {
				rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(program,param->index);

				params[0] = swapF32_16(value[0]);
				params[1] = swapF32_16(value[1]);
				params[2] = swapF32_16(value[2]);

				for(i=0;i<co_table->num;i++)
					RSX_FUNC(InlineTransfer)(context,offset + co_table->offset[i],params,3,location);
			}
			return;
		}

		case PARAM_FLOAT4:
		case PARAM_BOOL4:
		{
			if(param->index!=0xffffffff) {
				rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(program,param->index);

				params[0] = swapF32_16(value[0]);
				params[1] = swapF32_16(value[1]);
				params[2] = swapF32_16(value[2]);
				params[3] = swapF32_16(value[3]);

				for(i=0;i<co_table->num;i++)
					RSX_FUNC(InlineTransfer)(context,offset + co_table->offset[i],params,4,location);
			}
			return;
		}

		case PARAM_FLOAT3x3:
		case PARAM_FLOAT4x3:
		{
			s32 j,cnt = param->count;
			for(j=0;j<cnt;j++,value+=3) {
				if(param[j].index!=0xffffffff) {
					rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(program,param[j].index);

					params[0] = swapF32_16(value[0]);
					params[1] = swapF32_16(value[1]);
					params[2] = swapF32_16(value[2]);

					for(i=0;i<co_table->num;i++)
						RSX_FUNC(InlineTransfer)(context,offset + co_table->offset[i],params,3,location);
				}
			}
			return;
		}

		case PARAM_FLOAT3x4:
		case PARAM_FLOAT4x4:
		{
			s32 j,cnt = param->count;
			for(j=0;j<cnt;j++,value+=4) {
				if(param[j].index!=0xffffffff) {
					rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(program,param[j].index);

					params[0] = swapF32_16(value[0]);
					params[1] = swapF32_16(value[1]);
					params[2] = swapF32_16(value[2]);
					params[3] = swapF32_16(value[3]);

					for(i=0;i<co_table->num;i++)
						RSX_FUNC(InlineTransfer)(context,offset + co_table->offset[i],params,4,location);
				}
			}
			return;
		}
	}
}

void RSX_FUNC(SetFragmentProgramParameter)(CellGcmContextData *context,const rsxFragmentProgram *program,const rsxProgramConst *param,const f32 *value,u32 offset,u32 location)
{
	RSX_FUNC_INTERNAL(SetFragmentProgramParameter)(context, program, param, value, offset, location);
}

void RSX_FUNC(SetFragmentProgramGammaEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FP_PACKER,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(DrawVertexBegin)(CellGcmContextData *context,u32 type)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[1] = type;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(DrawVertexEnd)(CellGcmContextData *context)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(DrawVertex1f)(CellGcmContextData *context,u8 idx,f32 v)
{
	ieee32 d;
	d.f = v;

	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_1F_X(idx),1);
	RSX_CONTEXT_CURRENTP[1] = d.u;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(DrawVertex2f)(CellGcmContextData *context,u8 idx,const f32 v[2])
{
	RSX_CONTEXT_CURRENT_BEGIN(3);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_2F_X(idx),2);
	RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[1],v,sizeof(f32)*2);
	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(DrawVertex3f)(CellGcmContextData *context,u8 idx,const f32 v[3])
{
	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_3F_X(idx),3);
	RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[1],v,sizeof(f32)*3);
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(DrawVertex4f)(CellGcmContextData *context,u8 idx,const f32 v[4])
{
	RSX_CONTEXT_CURRENT_BEGIN(5);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_4F_X(idx),4);
	RSX_MEMCPY(&RSX_CONTEXT_CURRENTP[1],v,sizeof(f32)*4);
	RSX_CONTEXT_CURRENT_END(5);
}

void RSX_FUNC(DrawVertex4s)(CellGcmContextData *context,u8 idx,const s16 v[4])
{
	RSX_CONTEXT_CURRENT_BEGIN(3);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_4I_0(idx),2);
	RSX_CONTEXT_CURRENTP[1] = (v[0] | (v[1]<<16));
	RSX_CONTEXT_CURRENTP[2] = (v[2] | (v[3]<<16));
	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(DrawVertexScaled4s)(CellGcmContextData *context,u8 idx,const s16 v[4])
{
	RSX_CONTEXT_CURRENT_BEGIN(3);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_4I_SCALED_0(idx),2);
	RSX_CONTEXT_CURRENTP[1] = (v[0] | (v[1]<<16));
	RSX_CONTEXT_CURRENTP[2] = (v[2] | (v[3]<<16));
	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(DrawVertex2s)(CellGcmContextData *context,u8 idx,const s16 v[2])
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_2I(idx),1);
	RSX_CONTEXT_CURRENTP[1] = (v[0] | (v[1]<<16));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(DrawVertex4ub)(CellGcmContextData *context,u8 idx,const u8 v[4])
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_ATTR_4UB(idx),1);
	RSX_CONTEXT_CURRENTP[1] = (v[0] | (v[1]<<8) | (v[2]<<16) | (v[3]<<24));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetFrequencyDividerOperation)(CellGcmContextData *context,u16 operation)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DIVIDER_FREQUENCY_OP,1);
	RSX_CONTEXT_CURRENTP[1] = operation;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(InvalidateVertexCache)(CellGcmContextData *context)
{
	RSX_CONTEXT_CURRENT_BEGIN(8);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_CACHE_INVALIDATE2,1);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_VTX_CACHE_INVALIDATE,1);
	RSX_CONTEXT_CURRENTP[3] = 0;
	RSX_CONTEXT_CURRENTP[4] = RSX_METHOD(NV40TCL_VTX_CACHE_INVALIDATE,1);
	RSX_CONTEXT_CURRENTP[5] = 0;
	RSX_CONTEXT_CURRENTP[6] = RSX_METHOD(NV40TCL_VTX_CACHE_INVALIDATE,1);
	RSX_CONTEXT_CURRENTP[7] = 0;
	RSX_CONTEXT_CURRENT_END(8);
}

void RSX_FUNC(InvalidateTextureCache)(CellGcmContextData *context,u32 type)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_CACHE_CTL,1);
	RSX_CONTEXT_CURRENTP[1] = type;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(LoadTexture)(CellGcmContextData *context,u8 index,const CellGcmTexture *texture)
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

void RSX_FUNC(LoadVertexTexture)(CellGcmContextData *context,u8 index,const CellGcmTexture *texture)
{
	u32 format,offset,control,imagerect;
		
	offset = texture->offset;
	format = (texture->location + 1) | (texture->dimension << 4) |
			 (texture->format << 8) | (texture->mipmap << 16);
	imagerect = texture->height | (texture->width << 16);
	control   = texture->pitch;

	RSX_CONTEXT_CURRENT_BEGIN(7);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_TEXTURE_OFFSET(index),2);	// set offset and format for texture at once
	RSX_CONTEXT_CURRENTP[1] = offset;
	RSX_CONTEXT_CURRENTP[2] = format;
	
	RSX_CONTEXT_CURRENTP[3] = RSX_METHOD(NV40TCL_VP_TEXTURE_CONTROL3(index),1);	// set pitch for texture
	RSX_CONTEXT_CURRENTP[4] = control;
	
	RSX_CONTEXT_CURRENTP[5] = RSX_METHOD(NV40TCL_VP_TEXTURE_IMAGE_RECT(index),1);	// set width and height for texture
	RSX_CONTEXT_CURRENTP[6] = imagerect;
	
	RSX_CONTEXT_CURRENT_END(7);
}

void RSX_FUNC(TextureControl)(CellGcmContextData *context,u8 index,u32 enable,u16 minlod,u16 maxlod,u8 maxaniso)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_ENABLE(index),1);
	RSX_CONTEXT_CURRENTP[1] = ((enable << NV40TCL_TEX_ENABLE_SHIFT) | (minlod << NV40TCL_TEX_MINLOD_SHIFT) | (maxlod << NV40TCL_TEX_MAXLOD_SHIFT) | (maxaniso << NV40TCL_TEX_MAXANISO_SHIFT));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(VertexTextureControl)(CellGcmContextData *context,u8 index,u32 enable,u16 minlod,u16 maxlod)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_TEXTURE_CONTROL0(index),1);
	RSX_CONTEXT_CURRENTP[1] = ((enable << NV40TCL_TEX_ENABLE_SHIFT) | ((minlod&0xfff) << NV40TCL_TEX_MINLOD_SHIFT) | ((maxlod&0xfff) << NV40TCL_TEX_MAXLOD_SHIFT));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(TextureFilter)(CellGcmContextData *context,u8 index,u16 bias,u8 min,u8 mag,u8 conv)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_FILTER(index),1);
	RSX_CONTEXT_CURRENTP[1] = ((mag << NV40TCL_TEX_FILTER_MAG_SHIFT) | (min << NV40TCL_TEX_FILTER_MIN_SHIFT) | (conv << NV40TCL_TEX_FILTER_CONV_SHIFT) | (bias&0x1fff));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(VertexTextureFilter)(CellGcmContextData *context,u8 index,u16 bias)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_TEXTURE_FILTER(index),1);
	RSX_CONTEXT_CURRENTP[1] = bias;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(TextureWrapMode)(CellGcmContextData *context,u8 index,u8 wraps,u8 wrapt,u8 wrapr,u8 unsignedRemap,u8 zfunc,u8 gamma)
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

void RSX_FUNC(VertexTextureWrapMode)(CellGcmContextData *context,u8 index,u8 wraps,u8 wrapt)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_TEXTURE_ADDRESS(index),1);
	RSX_CONTEXT_CURRENTP[1] = (wraps | (wrapt<<8));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(TextureBorderColor)(CellGcmContextData *context,u8 index,u32 color)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_BORDER_COLOR(index),1);
	RSX_CONTEXT_CURRENTP[1] = color;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(VertexTextureBorderColor)(CellGcmContextData *context,u8 index,u32 color)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_TEXTURE_BORDER_COLOR(index),1);
	RSX_CONTEXT_CURRENTP[1] = color;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(TextureOptimization)(CellGcmContextData *context,u8 index,u8 slope,u8 iso,u8 aniso)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_CONTROL2(index),1);
	RSX_CONTEXT_CURRENTP[1] = ((0x2d<<8) | (aniso<<7) | (iso<<6) | slope);
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(TextureAnisoSpread)(CellGcmContextData *context,u8 index,u8 reduceSamplesEnable,u8 hReduceSamplesEnable,u8 vReduceSamplesEnable,u8 spacingSelect,u8 hSpacingSelect,u8 vSpacingSelect)
{
	u32 val = ((spacingSelect&0x7)<<0) | (( reduceSamplesEnable&0x1)<<4) |
			  ((hSpacingSelect&0x7)<<8) | ((hReduceSamplesEnable&0x1)<<12) |
			  ((vSpacingSelect&0x7)<<16) | ((vReduceSamplesEnable&0x1)<<20);
			  
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_TEX_ANISO_SPREAD(index),1);
	RSX_CONTEXT_CURRENTP[1] = val;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetZMinMaxControl)(CellGcmContextData *context,u8 cullNearFar,u8 zClampEnable,u8 cullIgnoreW)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_CONTROL,1);
	RSX_CONTEXT_CURRENTP[1] = (cullNearFar | (zClampEnable<<4) | (cullIgnoreW<<8));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetDepthBoundsTestEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_BOUNDS_TEST_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetDepthBounds)(CellGcmContextData *context,f32 zMin,f32 zMax)
{
	ieee32 d0,d1;

	d0.f = zMin;
	d1.f = zMax;
	RSX_CONTEXT_CURRENT_BEGIN(3);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DEPTH_BOUNDS_MIN,2);
	RSX_CONTEXT_CURRENTP[1] = d0.u;
	RSX_CONTEXT_CURRENTP[2] = d1.u;
	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(BindVertexArrayAttrib)(CellGcmContextData *context,u8 attr,u16 frequency,u32 offset,u8 stride,u8 elems,u8 dtype,u8 location)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTXFMT(attr),1);
	RSX_CONTEXT_CURRENTP[1] = ((frequency << 16) | (stride << NV40TCL_VTXFMT_STRIDE_SHIFT) | (elems << NV40TCL_VTXFMT_SIZE_SHIFT) | dtype);
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_VTXBUF_ADDRESS(attr),1);
	RSX_CONTEXT_CURRENTP[3] = ((location << 31) | offset);
	RSX_CONTEXT_CURRENT_END(4);
}

static inline __attribute__((always_inline)) void RSX_FUNC_INTERNAL(DrawVertexArray)(CellGcmContextData *context,u32 type,u32 start,u32 count)
{
	u32 i,j,lcount,loop,rest;
	
	--count;
	lcount = count&0xff;
	count >>= 8;
	
	loop = count/RSX_MAX_METHOD_COUNT;
	rest = count%RSX_MAX_METHOD_COUNT;
	
	RSX_CONTEXT_CURRENT_BEGIN(8);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VTX_CACHE_INVALIDATE,3);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENTP[2] = 0;
	RSX_CONTEXT_CURRENTP[3] = 0;
	
	RSX_CONTEXT_CURRENTP[4] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[5] = type;

	RSX_CONTEXT_CURRENTP[6] = RSX_METHOD(NV40TCL_VB_VERTEX_BATCH,1);
	RSX_CONTEXT_CURRENTP[7] = ((lcount<<24) | start);
	
	RSX_CONTEXT_CURRENT_END(8);
	
	start += (lcount + 1);
	for(i=0;i < loop;i++) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + RSX_MAX_METHOD_COUNT);
		
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VB_VERTEX_BATCH, RSX_MAX_METHOD_COUNT);
		RSX_CONTEXT_CURRENTP++;
		
		for(j=0;j<RSX_MAX_METHOD_COUNT;j++) {
			RSX_CONTEXT_CURRENTP[0] = (0xff000000 | start);
			RSX_CONTEXT_CURRENTP++;
			start += 0x100;
		}
		
	}	

	if(rest) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + rest);

		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VB_VERTEX_BATCH, rest);
		RSX_CONTEXT_CURRENTP++;
		
		for(j=0;j < rest;j++) {
			RSX_CONTEXT_CURRENTP[0] = (0xff000000 | start);
			RSX_CONTEXT_CURRENTP++;
			start += 0x100;
		}
	}
		
	RSX_CONTEXT_CURRENT_BEGIN(2);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END, 1);
	RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(DrawVertexArray)(CellGcmContextData *context,u32 type,u32 start,u32 count)
{
	RSX_FUNC_INTERNAL(DrawVertexArray)(context,type,start,count);
}

void RSX_FUNC(DrawInlineVertexArray)(CellGcmContextData *context,u8 type,u32 count,const void *data)
{
	u32 i,j,loop,rest;
	u32 *value = (u32*)data;
	
	loop = count/RSX_MAX_METHOD_COUNT;
	rest = count%RSX_MAX_METHOD_COUNT;
	
	RSX_CONTEXT_CURRENT_BEGIN(6);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VTX_CACHE_INVALIDATE,3);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENTP[2] = 0;
	RSX_CONTEXT_CURRENTP[3] = 0;

	RSX_CONTEXT_CURRENTP[4] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[5] = type;
	
	RSX_CONTEXT_CURRENT_END(6);

	for(i=0;i < loop;i++) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + RSX_MAX_METHOD_COUNT);

		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VERTEX_DATA, RSX_MAX_METHOD_COUNT);
		RSX_CONTEXT_CURRENTP++;
		
		for(j=0;j < RSX_MAX_METHOD_COUNT;j++) {
			RSX_CONTEXT_CURRENTP[0] = *value;
			RSX_CONTEXT_CURRENTP++;
			value++;
		}
	}
	
	if(rest) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + rest);

		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VERTEX_DATA, rest);
		RSX_CONTEXT_CURRENTP++;
		
		for(j=0;j < rest;j++) {
			RSX_CONTEXT_CURRENTP[0] = *value;
			RSX_CONTEXT_CURRENTP++;
			value++;
		}
	}

	RSX_CONTEXT_CURRENT_BEGIN(2);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END,1);
	RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;

	RSX_CONTEXT_CURRENT_END(2);
}

static inline __attribute__((always_inline)) void RSX_FUNC_INTERNAL(DrawIndexArray)(CellGcmContextData *context,u8 type,u32 offset,u32 count,u8 data_type,u8 location)
{
	u32 i,start,mcount;
	u32 misalignedcount,odd;

	if(data_type == CELL_GCM_INDEX_TYPE_32B)
		misalignedcount = (((offset + 127)&~127) - offset)>>2;
	else
		misalignedcount = (((offset + 127)&~127) - offset)>>1;

	odd = (misalignedcount && misalignedcount < count) ? 1 : 0;
	
	RSX_CONTEXT_CURRENT_BEGIN(7 + odd*2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VTX_CACHE_INVALIDATE, 1);
	RSX_CONTEXT_CURRENTP[1] = 0;

	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_VB_INDEX_BATCH_OFFSET, 2);
	RSX_CONTEXT_CURRENTP[3] = offset;
	RSX_CONTEXT_CURRENTP[4] = (((data_type) << 4) | location);

	RSX_CONTEXT_CURRENTP[5] = RSX_METHOD(NV40TCL_BEGIN_END, 1);
	RSX_CONTEXT_CURRENTP[6] = type;
	
	start = 0;
	if(odd) {
		u32 tmp = misalignedcount - 1;
		
		RSX_CONTEXT_CURRENTP[7] = RSX_METHOD(NV40TCL_VB_INDEX_BATCH_DRAW, 1);
		RSX_CONTEXT_CURRENTP[8] = ((tmp<<24) | start);

		start += misalignedcount;
		count -= misalignedcount;
	}
	
	RSX_CONTEXT_CURRENT_END(7 + odd*2);

	while(count > 0x7ff00) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + RSX_MAX_METHOD_COUNT);
		
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VB_INDEX_BATCH_DRAW, RSX_MAX_METHOD_COUNT);
		RSX_CONTEXT_CURRENTP++;
		
		for(i=0;i < RSX_MAX_METHOD_COUNT;i++) {
			RSX_CONTEXT_CURRENTP[0] = (0xff000000 | start);
			RSX_CONTEXT_CURRENTP++;
			start += 0x100;
		}

		count -= 0x7ff00;
	}
	
	mcount = (count + 0xff)>>8;
	
	RSX_CONTEXT_CURRENT_BEGIN(1 + mcount);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VB_INDEX_BATCH_DRAW, mcount);
	RSX_CONTEXT_CURRENTP++;
	
	while(count > 0x100) {
		RSX_CONTEXT_CURRENTP[0] = (0xff000000 | start);
		RSX_CONTEXT_CURRENTP++;
		start += 0x100;
		count -= 0x100;
	}
	
	if(count) {
		--count;	
		RSX_CONTEXT_CURRENTP[0] = ((count<<24) | start);
		RSX_CONTEXT_CURRENTP++;
	}	

	RSX_CONTEXT_CURRENT_BEGIN(2);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END, 1);
	RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;
 	
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(DrawIndexArray)(CellGcmContextData *context,u8 type,u32 offset,u32 count,u8 data_type,u8 location)
{
	RSX_FUNC_INTERNAL(DrawIndexArray)(context,type,offset,count,data_type,location);
}

void RSX_FUNC(DrawInlineIndexArray16)(CellGcmContextData *context,u8 type,u32 start,u32 count,const u16 *data)
{
	u32 odd,lcount;
	u32 loop,rest,i,j;
	
	if(count&1) {
		odd = 1;
		lcount = count - 1;
	} else {
		odd = 0;
		lcount = count;
	}
	
	data = data + start;
	loop = (lcount>>1)/RSX_MAX_METHOD_COUNT;
	rest = (lcount>>1)%RSX_MAX_METHOD_COUNT;
	
	RSX_CONTEXT_CURRENT_BEGIN(6 + odd*2);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VTX_CACHE_INVALIDATE, 3);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENTP[2] = 0;
	RSX_CONTEXT_CURRENTP[3] = 0;
	
	RSX_CONTEXT_CURRENTP[4] = RSX_METHOD(NV40TCL_BEGIN_END, 1);
	RSX_CONTEXT_CURRENTP[5] = type;
	
	if(odd) {
		RSX_CONTEXT_CURRENTP[6] = RSX_METHOD_NI(NV40TCL_VB_ELEMENT_U32, 1);
		RSX_CONTEXT_CURRENTP[7] = data[0];
		data++;
	}
	
	RSX_CONTEXT_CURRENT_END(6 + odd*2);
	
	for(i=0;i < loop;i++) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + RSX_MAX_METHOD_COUNT);

		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VB_ELEMENT_U16, RSX_MAX_METHOD_COUNT);
		RSX_CONTEXT_CURRENTP++;

		for(j=0;j < RSX_MAX_METHOD_COUNT;j++) {
			RSX_CONTEXT_CURRENTP[0] = (data[0] | (data[1]<<16));
			RSX_CONTEXT_CURRENTP++;
			data += 2;
		}
	}
	
	if(rest) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + rest);
	
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VB_ELEMENT_U16, rest);
		RSX_CONTEXT_CURRENTP++;
		
		for(j=0;j < rest;j++) {
			RSX_CONTEXT_CURRENTP[0] = (data[0] | (data[1]<<16));
			RSX_CONTEXT_CURRENTP++;
			data += 2;
		}
	}
	
	RSX_CONTEXT_CURRENT_BEGIN(2);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END, 1);
	RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;
	
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(DrawInlineIndexArray32)(CellGcmContextData *context,u8 type,u32 start,u32 count,const u32 *data)
{
	u32 i,j;
	u32 loop,rest;
	
	data = data + start;
	loop = count/RSX_MAX_METHOD_COUNT;
	rest = count%RSX_MAX_METHOD_COUNT;
	
	RSX_CONTEXT_CURRENT_BEGIN(6);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VTX_CACHE_INVALIDATE, 3);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENTP[2] = 0;
	RSX_CONTEXT_CURRENTP[3] = 0;
	
	RSX_CONTEXT_CURRENTP[4] = RSX_METHOD(NV40TCL_BEGIN_END, 1);
	RSX_CONTEXT_CURRENTP[5] = type;
	
	RSX_CONTEXT_CURRENT_END(6);
	
	for(i=0;i < loop;i++) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + RSX_MAX_METHOD_COUNT);
		
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VB_ELEMENT_U32, RSX_MAX_METHOD_COUNT);
		RSX_CONTEXT_CURRENTP++;
		
		for(j=0;j < RSX_MAX_METHOD_COUNT;j++) {
			RSX_CONTEXT_CURRENTP[0] = *data;
			RSX_CONTEXT_CURRENTP++;
			data++;
		}
	}
	
	if(rest) {
		RSX_CONTEXT_CURRENT_BEGIN(1 + rest);
		
		RSX_CONTEXT_CURRENTP[0] = RSX_METHOD_NI(NV40TCL_VB_ELEMENT_U32, rest);
		RSX_CONTEXT_CURRENTP++;
		
		for(j=0;j < rest;j++) {
			RSX_CONTEXT_CURRENTP[0] = *data;
			RSX_CONTEXT_CURRENTP++;
			data++;
		}
	}

	RSX_CONTEXT_CURRENT_BEGIN(2);
	
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BEGIN_END, 1);
	RSX_CONTEXT_CURRENTP[1] = NV40TCL_BEGIN_END_STOP;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetRestartIndexEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_RESTART_INDEX_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetRestartIndex)(CellGcmContextData *context,u32 index)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_RESTART_INDEX,1);
	RSX_CONTEXT_CURRENTP[1] = index;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetScissor)(CellGcmContextData *context,u16 x,u16 y,u16 w,u16 h)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_SCISSOR_HORIZ,2);
	RSX_CONTEXT_CURRENTP[1] = ((w<<16) | x);
	RSX_CONTEXT_CURRENTP[2] = ((h<<16) | y);

	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(SetAntialiasingControl)(CellGcmContextData *context,u32 enable,u32 alphaToCoverage,u32 alphaToOne,u32 sampleMask)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_ANTI_ALIASING_CONTROL,1);
	RSX_CONTEXT_CURRENTP[1] = ((sampleMask<<16) | (alphaToOne<<8) | (alphaToCoverage<<4) | enable);
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(InlineTransfer)(CellGcmContextData *context,u32 dstOffset,const void *srcAddress,u32 sizeInWords,u8 location)
{
	u32 *src;
	u32 pixelShift;
	u32 cnt,pos = 0;
	u32 padSizeInWords;
	u32 alignedVideoOffset;

	alignedVideoOffset = dstOffset&~0x3f;
	pixelShift = (dstOffset&0x3f)>>2;

	padSizeInWords = (sizeInWords + 1)&~0x01;

	RSX_CONTEXT_CURRENT_BEGIN(12 + padSizeInWords);

	RSX_CONTEXT_CURRENTP[pos++] = RSX_SUBCHANNEL_METHOD(3,NV04_CONTEXT_SURFACES_2D_DMA_IMAGE_DESTIN,1);
	RSX_CONTEXT_CURRENTP[pos++] = CELL_GCM_DMA_MEMORY_FRAME_BUFFER + location;

	RSX_CONTEXT_CURRENTP[pos++] = RSX_SUBCHANNEL_METHOD(3,NV04_CONTEXT_SURFACES_2D_OFFSET_DESTIN,1);
	RSX_CONTEXT_CURRENTP[pos++] = alignedVideoOffset;

	RSX_CONTEXT_CURRENTP[pos++] = RSX_SUBCHANNEL_METHOD(3,NV04_CONTEXT_SURFACES_2D_FORMAT,2);
	RSX_CONTEXT_CURRENTP[pos++] = NV04_CONTEXT_SURFACES_2D_FORMAT_Y32;
	RSX_CONTEXT_CURRENTP[pos++] = ((0x1000 << 16) | 0x1000);

	RSX_CONTEXT_CURRENTP[pos++] = RSX_SUBCHANNEL_METHOD(5,NV01_IMAGE_FROM_CPU_POINT,3);
	RSX_CONTEXT_CURRENTP[pos++] = ((0 << 16) | pixelShift);
	RSX_CONTEXT_CURRENTP[pos++] = ((1 << 16) | sizeInWords);
	RSX_CONTEXT_CURRENTP[pos++] = ((1 << 16) | sizeInWords);

	RSX_CONTEXT_CURRENTP[pos++] = RSX_SUBCHANNEL_METHOD(5,NV01_IMAGE_FROM_CPU_COLOR(0),padSizeInWords);

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

void RSX_FUNC(SetAlphaFunc)(CellGcmContextData *context,u32 alphaFunc,u32 ref)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_ALPHA_TEST_FUNC,2);
	RSX_CONTEXT_CURRENTP[1] = alphaFunc;
	RSX_CONTEXT_CURRENTP[2] = ref;
	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(SetAlphaTestEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_ALPHA_TEST_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetBlendFunc)(CellGcmContextData *context,u16 sfcolor,u16 dfcolor,u16 sfalpha,u16 dfalpha)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_FUNC_SRC,2);
	RSX_CONTEXT_CURRENTP[1] = ((sfalpha<<16) | sfcolor);
	RSX_CONTEXT_CURRENTP[2] = ((dfalpha<<16) | dfcolor);

	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(SetBlendEquation)(CellGcmContextData *context,u16 color,u16 alpha)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_EQUATION,1);
	RSX_CONTEXT_CURRENTP[1] = ((alpha<<16) | color);

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetBlendColor)(CellGcmContextData *context,u32 color0,u32 color1)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_COLOR,1);
	RSX_CONTEXT_CURRENTP[1] = color0;

	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_BLEND_COLOR2,1);
	RSX_CONTEXT_CURRENTP[3] = color1;

	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetBlendEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetBlendOptimization)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_DST_COL_REDUCE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetBlendEnableMrt)(CellGcmContextData *context, u32 mrt1, u32 mrt2, u32 mrt3)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_BLEND_ENABLE_MRT,1);
	RSX_CONTEXT_CURRENTP[1] = ((mrt3<<3) | (mrt2<<2) | (mrt1<<1));
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetLogicOp)(CellGcmContextData *context,u32 op)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_COLOR_LOGIC_OP,1);
	RSX_CONTEXT_CURRENTP[1] = op;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetLogicOpEnable)(CellGcmContextData *context,u32 enable)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_COLOR_LOGIC_OP_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetFogMode)(CellGcmContextData *context,u32 mode)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FOG_MODE,1);
	RSX_CONTEXT_CURRENTP[1] = mode;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetFogParams)(CellGcmContextData *context,f32 p0,f32 p1)
{
	ieee32 d0,d1;
	d0.f = p0;
	d1.f = p1;

	RSX_CONTEXT_CURRENT_BEGIN(3);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_FOG_EQUATION_CONSTANT,2);
	RSX_CONTEXT_CURRENTP[1] = d0.u;
	RSX_CONTEXT_CURRENTP[2] = d1.u;
	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(SetTransformBranchBits)(CellGcmContextData *context,u32 branchBits)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_VP_TRANSFORM_BRANCH_BITS,1);
	RSX_CONTEXT_CURRENTP[1] = branchBits;

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetPointSpriteControl)(CellGcmContextData *context,u32 enable,u32 rmode,u32 texcoordMask)
{
	RSX_CONTEXT_CURRENT_BEGIN(4);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_POINT_PARAMS_ENABLE,1);
	RSX_CONTEXT_CURRENTP[1] = enable;
	RSX_CONTEXT_CURRENTP[2] = RSX_METHOD(NV40TCL_POINT_SPRITE_CONTROL,1);
	RSX_CONTEXT_CURRENTP[3] = (texcoordMask | (rmode<<1) | enable);
	RSX_CONTEXT_CURRENT_END(4);
}

void RSX_FUNC(SetPointSize)(CellGcmContextData *context,f32 size)
{
	ieee32 d;
	
	d.f = size;
	
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_POINT_SIZE,1);
	RSX_CONTEXT_CURRENTP[1] = d.u;
	RSX_CONTEXT_CURRENT_END(2);
}

static inline __attribute__((always_inline)) void RSX_FUNC_INTERNAL(SetTransferData)(CellGcmContextData *context,u32 dstOffset,u32 dstPitch,u32 srcOffset,u32 srcPitch,u32 bytesPerRow,u32 rowCount)
{
	const s32 MIN_PITCH = -32768;
	const s32 MAX_PITCH =  32768;
	const s32 MAX_ROWS = 0x7ff;
	const u32 MAX_LINES = 0x3fffff;
	u32 colCount, rows, cols;

	if (srcPitch == bytesPerRow && dstPitch == bytesPerRow) {
		bytesPerRow *= rowCount;
		rowCount = 1;
		srcPitch = 0;
		dstPitch = 0;
	}

	if (srcPitch < MIN_PITCH || srcPitch > MAX_PITCH || dstPitch < MIN_PITCH || dstPitch > MAX_PITCH) {
		while (--rowCount >= 0) {
			for (colCount=bytesPerRow;colCount > 0;colCount-=cols) {
				cols = (colCount > MAX_LINES) ? MAX_LINES : colCount;

				RSX_CONTEXT_CURRENT_BEGIN(9);
				RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_IN,8);
				RSX_CONTEXT_CURRENTP[1] = srcOffset + (bytesPerRow - colCount);
				RSX_CONTEXT_CURRENTP[2] = dstOffset + (bytesPerRow - colCount);
				RSX_CONTEXT_CURRENTP[3] = 0;
				RSX_CONTEXT_CURRENTP[4] = 0;
				RSX_CONTEXT_CURRENTP[5] = cols;
				RSX_CONTEXT_CURRENTP[6] = 1;
				RSX_CONTEXT_CURRENTP[7] = (((u32)1<<8) | 1);
				RSX_CONTEXT_CURRENTP[8] = 0;
				RSX_CONTEXT_CURRENT_END(9);
			}
		}
	} else {
		for (;rowCount > 0;rowCount-=rows) {
			rows = (rowCount > MAX_ROWS) ? MAX_ROWS : rowCount;

			for (colCount=bytesPerRow;colCount > 0;colCount-=cols) {
				cols = (colCount > MAX_LINES) ? MAX_LINES : colCount;

				RSX_CONTEXT_CURRENT_BEGIN(9);
				RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_IN,8);
				RSX_CONTEXT_CURRENTP[1] = srcOffset + (bytesPerRow - colCount);
				RSX_CONTEXT_CURRENTP[2] = dstOffset + (bytesPerRow - colCount);
				RSX_CONTEXT_CURRENTP[3] = srcPitch;
				RSX_CONTEXT_CURRENTP[4] = dstPitch;
				RSX_CONTEXT_CURRENTP[5] = cols;
				RSX_CONTEXT_CURRENTP[6] = rows;
				RSX_CONTEXT_CURRENTP[7] = (((u32)1<<8) | 1);
				RSX_CONTEXT_CURRENTP[8] = 0;
				RSX_CONTEXT_CURRENT_END(9);
			}

			srcOffset += (rows*srcPitch);
			dstOffset += (rows*dstPitch);
		}
	}


}

void RSX_FUNC(SetTransferData)(CellGcmContextData *context,u8 mode,u32 dst,u32 outpitch,u32 src,u32 inpitch,u32 linelength,u32 linecount)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);
	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,2);
	RSX_CONTEXT_CURRENTP[1] = (mode&0x01) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;
	RSX_CONTEXT_CURRENTP[2] = (mode&0x02) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;
	RSX_CONTEXT_CURRENT_END(3);

	RSX_FUNC_INTERNAL(SetTransferData)(context, dst, outpitch, src, inpitch, linelength, linecount);
}

void RSX_FUNC(SetTransferDataMode)(CellGcmContextData *context,u8 mode)
{
	RSX_CONTEXT_CURRENT_BEGIN(3);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,2);
	RSX_CONTEXT_CURRENTP[1] = (mode&0x01) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;
	RSX_CONTEXT_CURRENTP[2] = (mode&0x02) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENT_END(3);
}

void RSX_FUNC(SetTransferDataOffset)(CellGcmContextData *context,u32 dst,u32 src)
{
	RSX_CONTEXT_CURRENT_BEGIN(6);
	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_IN,1);
	RSX_CONTEXT_CURRENTP[1] = src;
	RSX_CONTEXT_CURRENTP[2] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_OUT,1);
	RSX_CONTEXT_CURRENTP[3] = dst;
	RSX_CONTEXT_CURRENTP[4] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_BUF_NOTIFY,1);
	RSX_CONTEXT_CURRENTP[5] = 0;
	RSX_CONTEXT_CURRENT_END(6);

	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(1,NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_OUT,1);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetTransferDataFormat)(CellGcmContextData *context,s32 inpitch,s32 outpitch,u32 linelength,u32 linecount,u8 inbytes,u8 outbytes)
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

void RSX_FUNC(SetTransferImage)(CellGcmContextData *context,u8 mode,u32 dstOffset,u32 dstPitch,u32 dstX,u32 dstY,u32 srcOffset,u32 srcPitch,u32 srcX,u32 srcY,u32 width,u32 height,u32 bytesPerPixel)
{
	const u32 SURFACE_MAX_DIM = 10;
	const u32 BLOCK_SIZE = 1<<SURFACE_MAX_DIM;
	u32 finalDstX, finalDstY, x, y;

	RSX_CONTEXT_CURRENT_BEGIN(6);
	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(6,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,1);
	RSX_CONTEXT_CURRENTP[1] = (mode&0x01) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;
	RSX_CONTEXT_CURRENTP[2] = RSX_SUBCHANNEL_METHOD(3,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_OUT,1);
	RSX_CONTEXT_CURRENTP[3] = (mode&0x02) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;
	RSX_CONTEXT_CURRENTP[4] = RSX_SUBCHANNEL_METHOD(6,NV01_IMAGE_FROM_CPU_SURFACE,1);
	RSX_CONTEXT_CURRENTP[5] = CELL_GCM_CONTEXT_SURFACE2D;
	RSX_CONTEXT_CURRENT_END(6);

	finalDstX = dstX + width;
	finalDstY = dstY + height;
	for (y=dstY;y < finalDstY;) {
		u32 dstTop = y&~(BLOCK_SIZE - 1);
		u32 dstBot = dstTop + BLOCK_SIZE;
		u32 dstBltHeight = ((dstBot < finalDstY) ? dstBot : finalDstY) - y;

		for (x=dstX;x < finalDstX;) {
			u32 dstLeft = x&~(BLOCK_SIZE - 1);
			u32 dstRight = dstLeft + BLOCK_SIZE;
			u32 dstBltWidth = ((dstRight < finalDstX) ? dstRight : finalDstX) - x;
			u32 dstBlockOffset = bytesPerPixel*(dstLeft&~(BLOCK_SIZE - 1)) + dstPitch*dstTop;
			u32 srcBlockOffset = bytesPerPixel*(srcX + x - dstX) + srcPitch*(srcY + y - dstY);
			u32 safeDstBltWidth = (dstBltWidth < 16) ? 16 : (dstBltWidth + 1)&~1;
			u32 destinOffset = dstOffset + dstBlockOffset;

			RSX_CONTEXT_CURRENT_BEGIN(20);
			RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(3,NV04_CONTEXT_SURFACES_2D_OFFSET_DESTIN,1);
			RSX_CONTEXT_CURRENTP[1] = destinOffset;
			RSX_CONTEXT_CURRENTP[2] = RSX_SUBCHANNEL_METHOD(3,NV04_CONTEXT_SURFACES_2D_FORMAT,2);
			RSX_CONTEXT_CURRENTP[3] = (bytesPerPixel==4) ? NV04_CONTEXT_SURFACES_2D_FORMAT_A8R8G8B8 : ((bytesPerPixel==2) ? NV04_CONTEXT_SURFACES_2D_FORMAT_X1R5G5B5_X1R5G5B5 : 0);
			RSX_CONTEXT_CURRENTP[4] = ((dstPitch << 16) | dstPitch);
			RSX_CONTEXT_CURRENTP[5] = RSX_SUBCHANNEL_METHOD(6,NV01_IMAGE_FROM_CPU_OPERATION,9);
			RSX_CONTEXT_CURRENTP[6] = CELL_GCM_TRANSFER_CONVERSION_TRUNCATE;
			RSX_CONTEXT_CURRENTP[7] = (bytesPerPixel==4) ? NV01_IMAGE_FROM_CPU_COLOR_FORMAT_A8R8G8B8 : ((bytesPerPixel==2) ? NV01_IMAGE_FROM_CPU_COLOR_FORMAT_A1R5G5B5 : 0);
			RSX_CONTEXT_CURRENTP[8] = CELL_GCM_TRANSFER_OPERATION_SRCCOPY;
			RSX_CONTEXT_CURRENTP[9] = (((y - dstTop) << 16) | (x - dstLeft));
			RSX_CONTEXT_CURRENTP[10] = ((dstBltHeight << 16) | dstBltWidth);
			RSX_CONTEXT_CURRENTP[11] = (((y - dstTop) << 16) | (x - dstLeft));
			RSX_CONTEXT_CURRENTP[12] = ((dstBltHeight << 16) | dstBltWidth);
			RSX_CONTEXT_CURRENTP[13] = (1 << 20);
			RSX_CONTEXT_CURRENTP[14] = (1 << 20);
			RSX_CONTEXT_CURRENTP[15] = RSX_SUBCHANNEL_METHOD(6,NV03_SCALED_IMAGE_FROM_MEMORY_IMAGE_IN_SIZE,4);
			RSX_CONTEXT_CURRENTP[16] = ((dstBltHeight << 16) | safeDstBltWidth);
			RSX_CONTEXT_CURRENTP[17] = (srcPitch | (CELL_GCM_TRANSFER_ORIGIN_CORNER << 16) | (CELL_GCM_TRANSFER_INTERPOLATOR_NEAREST << 24));
			RSX_CONTEXT_CURRENTP[18] = srcOffset + srcBlockOffset;
			RSX_CONTEXT_CURRENTP[19] = 0;
			RSX_CONTEXT_CURRENT_END(20);

			x += dstBltWidth;
		}
		y += dstBltHeight;
	}
}

void RSX_FUNC(SetTransferScaleMode)(CellGcmContextData *context,u8 mode,u8 surface)
{
	RSX_CONTEXT_CURRENT_BEGIN(6);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(6,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,1);
	RSX_CONTEXT_CURRENTP[1] = (mode&0x01) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENTP[2] = (surface==CELL_GCM_TRANSFER_SWIZZLE) ? RSX_SUBCHANNEL_METHOD(4,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,1) : RSX_SUBCHANNEL_METHOD(3,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_OUT,1);
	RSX_CONTEXT_CURRENTP[3] = (mode&0x02) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;

	RSX_CONTEXT_CURRENTP[4] = RSX_SUBCHANNEL_METHOD(6,NV01_IMAGE_FROM_CPU_SURFACE,1);
	RSX_CONTEXT_CURRENTP[5] = (surface==CELL_GCM_TRANSFER_SWIZZLE) ? CELL_GCM_CONTEXT_SWIZZLE2D : CELL_GCM_CONTEXT_SURFACE2D;

	RSX_CONTEXT_CURRENT_END(6);
}

void RSX_FUNC(SetTransferScaleSurface)(CellGcmContextData *context,const CellGcmTransferScale *scale,const CellGcmTransferSurface *surface)
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

void RSX_FUNC(SetTransferScaleSwizzle)(CellGcmContextData *context,const CellGcmTransferScale *scale,const CellGcmTransferSwizzle *swizzle)
{
	RSX_CONTEXT_CURRENT_BEGIN(18);

	RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(4,NV04_SWIZZLED_SURFACE_FORMAT,2);
	RSX_CONTEXT_CURRENTP[1] = ((swizzle->height << 24) | (swizzle->width << 16) | swizzle->format);
	RSX_CONTEXT_CURRENTP[2] = swizzle->offset;

	RSX_CONTEXT_CURRENTP[3] = RSX_SUBCHANNEL_METHOD(6,NV04_SCALED_IMAGE_FROM_MEMORY_COLOR_CONVERSION,9);;
	RSX_CONTEXT_CURRENTP[4] = CELL_GCM_TRANSFER_CONVERSION_TRUNCATE;
	RSX_CONTEXT_CURRENTP[5] = scale->format;
	RSX_CONTEXT_CURRENTP[6] = CELL_GCM_TRANSFER_OPERATION_SRCCOPY;
	RSX_CONTEXT_CURRENTP[7] = ((scale->clipY << 16) | scale->clipX);
	RSX_CONTEXT_CURRENTP[8] = ((scale->clipH << 16) | scale->clipW);
	RSX_CONTEXT_CURRENTP[9] = ((scale->outY << 16) | scale->outX);
	RSX_CONTEXT_CURRENTP[10] = ((scale->outH << 16) | scale->outW);
	RSX_CONTEXT_CURRENTP[11] = scale->ratioX;
	RSX_CONTEXT_CURRENTP[12] = scale->ratioY;

	RSX_CONTEXT_CURRENTP[13] = RSX_SUBCHANNEL_METHOD(6,NV04_SCALED_IMAGE_FROM_MEMORY_SIZE,4);
	RSX_CONTEXT_CURRENTP[14] = ((scale->inH << 16) | scale->inW);
	RSX_CONTEXT_CURRENTP[15] = ((scale->interp << 24) | (scale->origin << 16) | (scale->pitch));
	RSX_CONTEXT_CURRENTP[16] = scale->offset;
	RSX_CONTEXT_CURRENTP[17] = ((scale->inY << 16) | scale->inX);

	RSX_CONTEXT_CURRENT_END(18);
}

static inline __attribute__((always_inline)) void RSX_FUNC_INTERNAL(SetConvertSwizzleFormat)(CellGcmContextData *context,u32 dstOffset,u32 dstWidth,u32 dstHeight,u32 dstX,u32 dstY,u32 srcOffset,u32 srcPitch,u32 srcX,u32 srcY,u32 width,u32 height,u32 bytesPerPixel,u32 mode)
{
    const u32 NV_MEM2MEM_MAX_HEIGHT_VALUE = 2047;
    const u32 NV_SURFACE_SWIZZLED_MAX_DIM = 10;
	u32 dstwlog2 = 31 - __cntlzw(dstWidth);
	u32 dsthlog2 = 31 - __cntlzw(dstHeight);
	
	switch(bytesPerPixel) {
        case 2:
        case 4:
            break;
        case 8:
            dstWidth <<= 1;
            dstX <<= 1;
            srcX <<= 1;
            width <<= 1;
            bytesPerPixel >>= 1;
            dstwlog2 += 1;
            break;
        case 16:
            dstWidth <<= 2;
            dstX <<= 2;
            srcX <<= 2;
            width <<= 2;
            bytesPerPixel >>= 2;
            dstwlog2 += 2;
            break;
        default:
        	return;
	}
	
    if((dstwlog2 <= 1) || (dsthlog2 == 0)) {
        u32 dstPitch;
        u32 linesLeft;

		dstPitch = bytesPerPixel<<dstwlog2;
		srcOffset = srcOffset + srcX*bytesPerPixel + srcY*srcPitch;
		dstOffset = dstOffset + dstX*bytesPerPixel + dstY*dstPitch;

        for(linesLeft=height;linesLeft;) {
            u32 actualHeight = (linesLeft > NV_MEM2MEM_MAX_HEIGHT_VALUE) ?  NV_MEM2MEM_MAX_HEIGHT_VALUE :  linesLeft;

			// todo: this is incorrect for the vid->vid case
			rsxSetTransferData(context,mode,dstOffset,dstPitch,srcOffset,srcPitch,width*bytesPerPixel,actualHeight);

            srcOffset = srcOffset + actualHeight*srcPitch;
            dstOffset = dstOffset + actualHeight*dstPitch;
            linesLeft -= actualHeight;
        }
        return;
	} else {
		u32 yTop,xEnd,yEnd,x,y;
        u32 srcFormat,dstFormat,logWidthLimit,logHeightLimit;
		u32 srcHandle = (mode&0x01) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;
		u32 dstHandle = (mode&0x02) ? CELL_GCM_DMA_MEMORY_HOST_BUFFER : CELL_GCM_DMA_MEMORY_FRAME_BUFFER;

		RSX_CONTEXT_CURRENT_BEGIN(6);
		RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(4,NV_MEMORY_TO_MEMORY_FORMAT_DMA_BUFFER_IN,1);
		RSX_CONTEXT_CURRENTP[1] = dstHandle;
		RSX_CONTEXT_CURRENTP[2] = RSX_SUBCHANNEL_METHOD(6,NV03_SCALED_IMAGE_FROM_MEMORY_DMA_IMAGE,1);
		RSX_CONTEXT_CURRENTP[3] = srcHandle;
		RSX_CONTEXT_CURRENTP[4] = RSX_SUBCHANNEL_METHOD(6,NV01_IMAGE_FROM_CPU_SURFACE,1);
		RSX_CONTEXT_CURRENTP[5] = CELL_GCM_CONTEXT_SWIZZLE2D;		
		RSX_CONTEXT_CURRENT_END(6);

		switch(bytesPerPixel)
		{
		case 2:
			srcFormat = CELL_GCM_TRANSFER_SCALE_FORMAT_R5G6B5;
			dstFormat = CELL_GCM_TRANSFER_SURFACE_FORMAT_R5G6B5;
			break;
		case 4:
			srcFormat = CELL_GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
			dstFormat = CELL_GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
			break;
		case 1:
		default:
			return;
		}

        logWidthLimit  = (dstwlog2 > NV_SURFACE_SWIZZLED_MAX_DIM) ? NV_SURFACE_SWIZZLED_MAX_DIM : dstwlog2;
        logHeightLimit = (dsthlog2 > NV_SURFACE_SWIZZLED_MAX_DIM) ? NV_SURFACE_SWIZZLED_MAX_DIM : dsthlog2;

        srcOffset += (srcX - dstX)*bytesPerPixel + (srcY - dstY)*srcPitch;

        xEnd = dstX + width;
        yEnd = dstY + height;

        yTop = dstY&~((1<< NV_SURFACE_SWIZZLED_MAX_DIM) - 1);
        for(y=dstY;y < yEnd;) {
            u32 xLeft;
            u32 yBottom;
            u32 bltHeight;

            yBottom = yTop + (1<<NV_SURFACE_SWIZZLED_MAX_DIM);
            if(yBottom > (1ul << dsthlog2)) {
                yBottom = (1<<dsthlog2);
            }
            bltHeight = (yBottom > yEnd) ? yEnd - y : yBottom - y;

            xLeft = dstX&~((1<<NV_SURFACE_SWIZZLED_MAX_DIM) - 1);
            for(x=dstX; x < xEnd;) {
                u32 xRight;
                u32 bltWidth;
                u32 blockSrcOffset;
                u32 blockDstOffset;
                u32 blockX;
                u32 blockY;
                u32 srcWidth;

                xRight = xLeft + (1<<NV_SURFACE_SWIZZLED_MAX_DIM);
                bltWidth = (xRight > xEnd ) ? xEnd - x : xRight - x;

				if(!dstwlog2)
					blockDstOffset = dstOffset + yTop*bytesPerPixel;
				else if(!dsthlog2)
					blockDstOffset = dstOffset + xLeft*bytesPerPixel;
				else {
					u32 log = (dstwlog2 < dsthlog2) ? dstwlog2 : dsthlog2;  
					u32 doubleLog = log<<1;                     
					u32 upperMask = ~((1<<doubleLog) - 1);      
					u32 lowerMask = ~upperMask;                   

					uint32_t upperU = (xLeft<<log)&upperMask;
					uint32_t upperV = (yTop << log)&upperMask;
					uint32_t lower  = ((xLeft&0x001)<<0) | ((yTop&0x001)<<1) | 
									  ((xLeft&0x002)<<1) | ((yTop&0x002)<<2) |
									  ((xLeft&0x004)<<2) | ((yTop&0x004)<<3) |
									  ((xLeft&0x008)<<3) | ((yTop&0x008)<<4) |
									  ((xLeft&0x010)<<4) | ((yTop&0x010)<<5) |
									  ((xLeft&0x020)<<5) | ((yTop&0x020)<<6) |
									  ((xLeft&0x040)<<6) | ((yTop&0x040)<<7) |
									  ((xLeft&0x080)<<7) | ((yTop&0x080)<<8) |
									  ((xLeft&0x100)<<8) | ((yTop&0x100)<<9) |
									  ((xLeft&0x200)<<9) | ((yTop&0x200)<<10) |
									  ((xLeft&0x400)<<10) | ((yTop&0x400)<<11) |
									  ((xLeft&0x800)<<11) | ((yTop&0x800)<<12);

					blockDstOffset = dstOffset + ((lower&lowerMask) | upperU | upperV)*bytesPerPixel;
				}
				
                blockX = x&((1<<NV_SURFACE_SWIZZLED_MAX_DIM) - 1);
                blockY = y&((1<<NV_SURFACE_SWIZZLED_MAX_DIM) - 1);

                blockSrcOffset = srcOffset + x*bytesPerPixel + y*srcPitch;

                srcWidth = (bltWidth < 16) ? 16 : (bltWidth + 1)&~1;

				RSX_CONTEXT_CURRENT_BEGIN(18);
				
				RSX_CONTEXT_CURRENTP[0] = RSX_SUBCHANNEL_METHOD(4,NV04_SWIZZLED_SURFACE_FORMAT,2);
				RSX_CONTEXT_CURRENTP[1] = ((logHeightLimit<<24) | (logWidthLimit<<16) |(dstFormat));
				RSX_CONTEXT_CURRENTP[2] = blockDstOffset;
				
				RSX_CONTEXT_CURRENTP[3] = RSX_SUBCHANNEL_METHOD(6,NV04_SCALED_IMAGE_FROM_MEMORY_COLOR_CONVERSION,9);
				RSX_CONTEXT_CURRENTP[4] = CELL_GCM_TRANSFER_CONVERSION_TRUNCATE;
				RSX_CONTEXT_CURRENTP[5] = srcFormat;
				RSX_CONTEXT_CURRENTP[6] = CELL_GCM_TRANSFER_OPERATION_SRCCOPY;
				RSX_CONTEXT_CURRENTP[7] = ((blockY<<16) | blockX);
				RSX_CONTEXT_CURRENTP[8] = ((bltHeight<<16) | bltWidth);
				RSX_CONTEXT_CURRENTP[9] = ((blockY<<16) | blockX);
				RSX_CONTEXT_CURRENTP[10] = ((bltHeight<<16) | bltWidth);
				RSX_CONTEXT_CURRENTP[11] = 0x00100000;
				RSX_CONTEXT_CURRENTP[12] = 0x00100000;
				
				RSX_CONTEXT_CURRENTP[13] = RSX_SUBCHANNEL_METHOD(6,NV04_SCALED_IMAGE_FROM_MEMORY_SIZE,4);
				RSX_CONTEXT_CURRENTP[14] = ((bltHeight<<16) | srcWidth);
				RSX_CONTEXT_CURRENTP[15] = ((CELL_GCM_TRANSFER_INTERPOLATOR_NEAREST<<24) | (CELL_GCM_TRANSFER_ORIGIN_CORNER<<16) | srcPitch);
				RSX_CONTEXT_CURRENTP[16] = blockSrcOffset;
				RSX_CONTEXT_CURRENTP[17] = 0;
				
				RSX_CONTEXT_CURRENT_END(18);
				
                x = xLeft = xRight;
            }
            y = yTop = yBottom;
        }
	}
}

void RSX_FUNC(SetConvertSwizzleFormat)(CellGcmContextData *context,u32 dstOffset,u32 dstWidth,u32 dstHeight,u32 dstX,u32 dstY,u32 srcOffset,u32 srcPitch,u32 srcX,u32 srcY,u32 width,u32 height,u32 bytesPerPixel,u32 mode)
{
	RSX_FUNC_INTERNAL(SetConvertSwizzleFormat)(context,dstOffset,dstWidth,dstHeight,dstX,dstY,srcOffset,srcPitch,srcX,srcY,width,height,bytesPerPixel,mode);
}

void RSX_FUNC(SetTimeStamp)(CellGcmContextData *context,u32 index)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);

	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_QUERY_GET,1);
	RSX_CONTEXT_CURRENTP[2] = (((index << 4)&0x0fffffff) | 0x10000000);

	RSX_CONTEXT_CURRENT_END(2);
}

void RSX_FUNC(SetWaitForIdle)(CellGcmContextData *context)
{
	RSX_CONTEXT_CURRENT_BEGIN(2);
	RSX_CONTEXT_CURRENTP[0] = RSX_METHOD(NV40TCL_WAIT_FOR_IDLE, 1);
	RSX_CONTEXT_CURRENTP[1] = 0;
	RSX_CONTEXT_CURRENT_END(2);
}
