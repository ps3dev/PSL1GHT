#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-types.h>

#include <io/pad.h>
#include <rsx/rsx.h>
#include <sys/heap.h>
#include <sysutil/sysutil.h>

#include "spu.h"
#include "rsxutil.h"
#include "geometry.h"
#include "memory.h"

#include "acid.h"

#include "diffuse_specular_shader_vpo.h"
#include "diffuse_specular_shader_fpo.h"

#define DEGTORAD(a)			( (a) *  0.01745329252f )
#define RADTODEG(a)			( (a) * 57.29577951f )

u32 running = 0;

u32 fp_offset;
u32 *fp_buffer;

u32 *texture_buffer;
u32 texture_offset;

// vertex shader
rsxProgramConst *projMatrix;
rsxProgramConst *mvMatrix;

// fragment shader
rsxProgramAttrib *textureUnit;
rsxProgramConst *eyePosition;
rsxProgramConst *globalAmbient;
rsxProgramConst *litPosition;
rsxProgramConst *litColor;
rsxProgramConst *Kd;
rsxProgramConst *Ks;
rsxProgramConst *spec;

Point3 eye_pos = Point3(0.0f,0.0f,20.0f);
Point3 eye_dir = Point3(0.0f,0.0f,0.0f);
Vector3 up_vec = Vector3(0.0f,1.0f,0.0f);

void *vp_ucode = NULL;
rsxVertexProgram *vpo = (rsxVertexProgram*)diffuse_specular_shader_vpo;

void *fp_ucode = NULL;
rsxFragmentProgram *fpo = (rsxFragmentProgram*)diffuse_specular_shader_fpo;

static Matrix4 P;
static SMeshBuffer *sphere = NULL;
static SMeshBuffer *donut = NULL;
static SMeshBuffer *cube = NULL;

extern "C" {
static void program_exit_callback()
{
	gcmSetWaitFlip(context);
	rsxFinish(context,1);

	shutdown_spu();
}

static void sysutil_exit_callback(u64 status,u64 param,void *usrdata)
{
	switch(status) {
		case SYSUTIL_EXIT_GAME:
			running = 0;
			break;
		case SYSUTIL_DRAW_BEGIN:
		case SYSUTIL_DRAW_END:
			break;
		default:
			break;
	}
}
}
static void init_texture()
{
	u32 i;
	u8 *buffer;
	const u8 *data = acid.pixel_data;

	texture_buffer = (u32*)rsxMemalign(128,(acid.width*acid.height*4));
	if(!texture_buffer) return;

	rsxAddressToOffset(texture_buffer,&texture_offset);

	buffer = (u8*)texture_buffer;
	for(i=0;i<acid.width*acid.height*4;i+=4) {
		buffer[i + 1] = *data++;
		buffer[i + 2] = *data++;
		buffer[i + 3] = *data++;
		buffer[i + 0] = *data++;
	}
}

static void setTexture(u8 textureUnit)
{
	u32 width = 128;
	u32 height = 128;
	u32 pitch = (width*4);
	gcmTexture texture;

	if(!texture_buffer) return;

	rsxInvalidateTextureCache(context,GCM_INVALIDATE_TEXTURE);

	texture.format		= (GCM_TEXTURE_FORMAT_A8R8G8B8 | GCM_TEXTURE_FORMAT_LIN);
	texture.mipmap		= 1;
	texture.dimension	= GCM_TEXTURE_DIMS_2D;
	texture.cubemap		= GCM_FALSE;
	texture.remap		= ((GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_B_SHIFT) |
						   (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_G_SHIFT) |
						   (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_R_SHIFT) |
						   (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_A_SHIFT) |
						   (GCM_TEXTURE_REMAP_COLOR_B << GCM_TEXTURE_REMAP_COLOR_B_SHIFT) |
						   (GCM_TEXTURE_REMAP_COLOR_G << GCM_TEXTURE_REMAP_COLOR_G_SHIFT) |
						   (GCM_TEXTURE_REMAP_COLOR_R << GCM_TEXTURE_REMAP_COLOR_R_SHIFT) |
						   (GCM_TEXTURE_REMAP_COLOR_A << GCM_TEXTURE_REMAP_COLOR_A_SHIFT));
	texture.width		= width;
	texture.height		= height;
	texture.depth		= 1;
	texture.location	= GCM_LOCATION_RSX;
	texture.pitch		= pitch;
	texture.offset		= texture_offset;
	rsxLoadTexture(context,textureUnit,&texture);
	rsxTextureControl(context,textureUnit,GCM_TRUE,0<<8,12<<8,GCM_TEXTURE_MAX_ANISO_1);
	rsxTextureFilter(context,textureUnit,0,GCM_TEXTURE_LINEAR,GCM_TEXTURE_LINEAR,GCM_TEXTURE_CONVOLUTION_QUINCUNX);
	rsxTextureWrapMode(context,textureUnit,GCM_TEXTURE_CLAMP_TO_EDGE,GCM_TEXTURE_CLAMP_TO_EDGE,GCM_TEXTURE_CLAMP_TO_EDGE,0,GCM_TEXTURE_ZFUNC_LESS,0);
}

static void setDrawEnv()
{
	rsxSetColorMask(context,GCM_COLOR_MASK_B |
							GCM_COLOR_MASK_G |
							GCM_COLOR_MASK_R |
							GCM_COLOR_MASK_A);

	rsxSetColorMaskMrt(context,0);

	u16 x,y,w,h;
	f32 min, max;
	f32 scale[4],offset[4];

	x = 0;
	y = 0;
	w = display_width;
	h = display_height;
	min = 0.0f;
	max = 1.0f;
	scale[0] = w*0.5f;
	scale[1] = h*-0.5f;
	scale[2] = (max - min)*0.5f;
	scale[3] = 0.0f;
	offset[0] = x + w*0.5f;
	offset[1] = y + h*0.5f;
	offset[2] = (max + min)*0.5f;
	offset[3] = 0.0f;

	rsxSetViewport(context,x, y, w, h, min, max, scale, offset);
	rsxSetScissor(context,x,y,w,h);

	rsxSetDepthTestEnable(context,GCM_TRUE);
	rsxSetDepthFunc(context,GCM_LESS);
	rsxSetShadeModel(context,GCM_SHADE_MODEL_SMOOTH);
	rsxSetDepthWriteEnable(context,1);
	rsxSetFrontFace(context,GCM_FRONTFACE_CCW);
}


void init_shader()
{
	u32 fpsize = 0;
	u32 vpsize = 0;

	rsxVertexProgramGetUCode(vpo, &vp_ucode, &vpsize);
	printf("vpsize: %d\n", vpsize);

	projMatrix = rsxVertexProgramGetConst(vpo,"projMatrix");
	mvMatrix = rsxVertexProgramGetConst(vpo,"modelViewMatrix");

	rsxFragmentProgramGetUCode(fpo,&fp_ucode,&fpsize);
	printf("fpsize: %d\n", fpsize);

	fp_buffer = (u32*)rsxMemalign(128,fpsize);
	memcpy(fp_buffer,fp_ucode,fpsize);
	rsxAddressToOffset(fp_buffer,&fp_offset);

	textureUnit = rsxFragmentProgramGetAttrib(fpo,"texture");
	eyePosition = rsxFragmentProgramGetConst(fpo,"eyePosition");
	globalAmbient = rsxFragmentProgramGetConst(fpo,"globalAmbient");
	litPosition = rsxFragmentProgramGetConst(fpo,"lightPosition");
	litColor = rsxFragmentProgramGetConst(fpo,"lightColor");
	spec = rsxFragmentProgramGetConst(fpo,"shininess");
	Ks = rsxFragmentProgramGetConst(fpo,"Ks");
	Kd = rsxFragmentProgramGetConst(fpo,"Kd");
}

void initialize()
{
	void *host_addr = memalign(1024*1024,HOST_SIZE);
	void *main_addr = (void*)((u64)host_addr + CB_SIZE);

	init_spu();
	init_screen(host_addr,HOST_SIZE);
	init_shader();
	init_texture();
	init_memory(main_addr,(HOST_SIZE - CB_SIZE));
}

void drawFrame()
{
	u32 i,color = 0;
	Matrix4 rotX,rotY;
	Vector4 objEyePos,objLightPos;
	Matrix4 viewMatrix,modelMatrix,modelMatrixIT,modelViewMatrix;
	Point3 lightPos = Point3(250.0f,150.0f,150.0f);
	f32 globalAmbientColor[3] = {0.1f,0.1f,0.1f};
	f32 lightColor[3] = {0.95f,0.95f,0.95f};
	f32 materialColorDiffuse[3] = {0.5f,0.0f,0.0f};
	f32 materialColorSpecular[3] = {0.7f,0.6f,0.6f};
	f32 shininess = 17.8954f;
	static f32 rot = 0.0f;
	SMeshBuffer *mesh = NULL;

	setDrawEnv();
	setTexture(textureUnit->index);

	rsxSetClearColor(context,color);
	rsxSetClearDepthStencil(context,0xffff);
	rsxClearSurface(context,GCM_CLEAR_R |
							GCM_CLEAR_G |
							GCM_CLEAR_B |
							GCM_CLEAR_A |
							GCM_CLEAR_S |
							GCM_CLEAR_Z);

	rsxSetZMinMaxControl(context,GCM_FALSE, GCM_TRUE, GCM_FALSE);

	for(i=0;i<8;i++)
		rsxSetViewportClip(context,i,display_width,display_height);

	viewMatrix = Matrix4::lookAt(eye_pos,eye_dir,up_vec);

	mesh = sphere;
	rotX = Matrix4::rotationX(DEGTORAD(30.0f));
	rotY = Matrix4::rotationY(DEGTORAD(rot));
	modelMatrix = rotX*rotY;
	modelMatrixIT = inverse(modelMatrix);
	modelViewMatrix = transpose(viewMatrix*modelMatrix);

	objEyePos = modelMatrixIT*eye_pos;
	objLightPos = modelMatrixIT*lightPos;

	wait_signal_spu();

	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_POS,0,mesh->pos_off,sizeof(S3DVertex),3,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);
	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_NORMAL,0,mesh->nrm_off,sizeof(S3DVertex),3,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);
	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_TEX0,0,mesh->uv_off,sizeof(S3DVertex),2,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);

	rsxLoadVertexProgram(context,vpo,vp_ucode);
	rsxSetVertexProgramParameter(context,vpo,projMatrix,(float*)&P);
	rsxSetVertexProgramParameter(context,vpo,mvMatrix,(float*)&modelViewMatrix);

	rsxSetFragmentProgramParameter(context,fpo,eyePosition,(float*)&objEyePos,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,globalAmbient,globalAmbientColor,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,litPosition,(float*)&objLightPos,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,litColor,lightColor,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,spec,&shininess,fp_offset,GCM_LOCATION_RSX);

	rsxSetFragmentProgramParameter(context,fpo,Kd,materialColorDiffuse,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,Ks,materialColorSpecular,fp_offset,GCM_LOCATION_RSX);

	rsxLoadFragmentProgramLocation(context,fpo,fp_offset,GCM_LOCATION_RSX);

	rsxSetUserClipPlaneControl(context,GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE);

	rsxDrawIndexArray(context,GCM_TYPE_TRIANGLES,mesh->ind_off,mesh->getIndexCount(),GCM_INDEX_TYPE_32B,GCM_LOCATION_RSX);

	mesh = donut;
	rotX = Matrix4::rotationX(DEGTORAD(rot));
	rotY = Matrix4::rotationY(DEGTORAD(30.0f));
	modelMatrix = rotX*rotY;
	modelMatrix.setTranslation(Vector3(3.0f,5.0f,-8.0f));

	modelMatrixIT = inverse(modelMatrix);
	modelViewMatrix = transpose(viewMatrix*modelMatrix);

	objEyePos = modelMatrixIT*eye_pos;
	objLightPos = modelMatrixIT*lightPos;

	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_POS,0,mesh->pos_off,sizeof(S3DVertex),3,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);
	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_NORMAL,0,mesh->nrm_off,sizeof(S3DVertex),3,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);
	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_TEX0,0,mesh->uv_off,sizeof(S3DVertex),2,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);

	rsxLoadVertexProgram(context,vpo,vp_ucode);
	rsxSetVertexProgramParameter(context,vpo,projMatrix,(float*)&P);
	rsxSetVertexProgramParameter(context,vpo,mvMatrix,(float*)&modelViewMatrix);

	rsxSetFragmentProgramParameter(context,fpo,eyePosition,(float*)&objEyePos,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,globalAmbient,globalAmbientColor,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,litPosition,(float*)&objLightPos,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,litColor,lightColor,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,spec,&shininess,fp_offset,GCM_LOCATION_RSX);

	rsxSetFragmentProgramParameter(context,fpo,Kd,materialColorDiffuse,fp_offset,GCM_LOCATION_RSX);
	rsxSetFragmentProgramParameter(context,fpo,Ks,materialColorSpecular,fp_offset,GCM_LOCATION_RSX);

	rsxLoadFragmentProgramLocation(context,fpo,fp_offset,GCM_LOCATION_RSX);

	rsxSetUserClipPlaneControl(context,GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE);

	rsxDrawIndexArray(context,GCM_TYPE_TRIANGLES,mesh->ind_off,mesh->getIndexCount(),GCM_INDEX_TYPE_32B,GCM_LOCATION_RSX);

	clear_signal_spu();
	signal_spu_rsx();

	rot += 4.0f;
	if(rot>=360.0f) rot = 0.0f;
}

int main(int argc,const char *argv[])
{
	padInfo padinfo;
	padData paddata;

	printf("rsxtest_spu started...\n");

	initialize();
	ioPadInit(7);

	sphere = createSphere(3.0f,32,32);
	donut = createDonut(3.0f,1.5f,32,32);
	cube = createCube(5.0f);

	rsxConstOffsetTable *co_table = rsxFragmentProgramGetConstOffsetTable(fpo,litColor->index);
	u32 const_addr = (u32)((u64)fp_buffer + co_table->offset[0]);
	setup_shared_buffer(const_addr,(u32)(u64)mem_align(128,128),(u32)(u64)gcmGetLabelAddress(64));
	signal_spu_ppu();
	signal_spu_rsx();

	P = transpose(Matrix4::perspective(DEGTORAD(45.0f),aspect_ratio,1.0f,3000.0f));

	setRenderTarget(curr_fb);
	rsxFinish(context,0);

	atexit(program_exit_callback);
	sysUtilRegisterCallback(0,sysutil_exit_callback,NULL);

	delete cube;

	running = 1;
	while(running) {
		sysUtilCheckCallback();

		ioPadGetInfo(&padinfo);
		for(int i=0; i < MAX_PADS; i++){
			if(padinfo.status[i]){
				ioPadGetData(i, &paddata);

				if(paddata.BTN_CROSS)
					goto done;
			}

		}

		drawFrame();
		flip();
	}

done:
    printf("rsxtest_spu done...\n");
    program_exit_callback();
    return 0;
}
