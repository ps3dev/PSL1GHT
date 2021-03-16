#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-types.h>
#include <sysmodule/sysmodule.h>
#include <pngdec/pngdec.h>
#include "Stone_Texture_png_bin.h"
#include <sys/process.h>

#include <io/pad.h>
#include <rsx/rsx.h>
#include <sysutil/sysutil.h>

#include "acid.h"
#include "mesh.h"
#include "rsxutil.h"

#include "diffuse_specular_shader_vpo.h"
#include "diffuse_specular_shader_fpo.h"

#define DEGTORAD(a)			( (a) *  0.01745329252f )
#define RADTODEG(a)			( (a) * 57.29577951f )
#define TEXTURE_SIZE 128
float perlin2d(float x, float y, float freq, int depth);

u32 running = 0;

u32 fp_offset;
u32 *fp_buffer;

u32 *texture_buffer[3];
u32 texture_buffer_idx = 0;
u32 texture_offset[3];
pngData *png;
f32 rotx = 0.0f;
f32 roty = 0.0f;

// vertex shader
rsxProgramConst *projMatrix;
rsxProgramConst *mvMatrix;

// fragment shader
rsxProgramAttrib *textureUnit;

Point3 eye_pos = Point3(0.0f,0.0f,20.0f);
Point3 eye_dir = Point3(0.0f,0.0f,0.0f);
Vector3 up_vec = Vector3(0.0f,1.0f,0.0f);

void *vp_ucode = NULL;
rsxVertexProgram *vpo = (rsxVertexProgram*)diffuse_specular_shader_vpo;

void *fp_ucode = NULL;
rsxFragmentProgram *fpo = (rsxFragmentProgram*)diffuse_specular_shader_fpo;

static Matrix4 P;
static SMeshBuffer *cube = NULL;

SYS_PROCESS_PARAM(1001, 0x100000);

extern "C" {
static void program_exit_callback()
{
	gcmSetWaitFlip(context);
	rsxFinish(context,1);
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
	
	//Init png texture
	const u8* data = (u8*)png->bmp_out;
	texture_buffer[0] = (u32*)rsxMemalign(128, (png->height * png->pitch));

	if(!texture_buffer[0]) return;

	rsxAddressToOffset(texture_buffer[0],&texture_offset[0]);

	buffer = (u8*)texture_buffer[0];
	for (i = 0; i < png->height * png->pitch; i += 4) {
		buffer[i + 0] = *data++;
		buffer[i + 1] = *data++;
		buffer[i + 2] = *data++;
		buffer[i + 3] = *data++;
	}

	//Init acid pixel texture
	data = acid.pixel_data;
	texture_buffer[1] = (u32*)rsxMemalign(128,(acid.width*acid.height*4));

	if (!texture_buffer[1]) return;

	rsxAddressToOffset(texture_buffer[1], &texture_offset[1]);

	buffer = (u8*)texture_buffer[1];
	for(i=0;i<acid.width*acid.height*4;i+=4) {
		buffer[i + 1] = *data++;
		buffer[i + 2] = *data++;
		buffer[i + 3] = *data++;
		buffer[i + 0] = *data++;
	}

	//Init perlin noise texture
	texture_buffer[2] = (u32*)rsxMemalign(128, (TEXTURE_SIZE * TEXTURE_SIZE * 4));

	if (!texture_buffer[2]) return;

	rsxAddressToOffset(texture_buffer[2], &texture_offset[2]);

	u32* buffer32 = (u32*)texture_buffer[2];
	for (u32 i = 0; i < TEXTURE_SIZE; i++) {
		for (u32 j = 0; j < TEXTURE_SIZE; j++) {
			buffer32[j + TEXTURE_SIZE * i] = 255 << 24 | (u8)(255.0 * perlin2d((float)j, (float)i, 0.1, 10.0)) << 16 | 255 << 8 | 255 ;
		}
	}
}

static SMeshBuffer* createCube(f32 size)
{
	u32 i;
	SMeshBuffer *buffer = new SMeshBuffer();
	const u16 u[36] = {   0,1,2,   0,2,3,   1,4,5,   1,5,2,   4,7,6,	 4,6,5, 
						  7,0,3,   7,3,6,   9,2,5,   9,5,8,   0,10,11,   0,7,10};

	buffer->cnt_indices = 36;
	buffer->indices = (u16*)rsxMemalign(128,buffer->cnt_indices*sizeof(u16));

	for(i=0;i<36;i++) buffer->indices[i] = u[i];

	buffer->cnt_vertices = 12;
	buffer->vertices = (S3DVertex*)rsxMemalign(128,buffer->cnt_vertices*sizeof(S3DVertex));

	buffer->vertices[0] = S3DVertex(0,0,0, -1,-1,-1, 1, 0);
	buffer->vertices[1] = S3DVertex(1,0,0,  1,-1,-1, 1, 1);
	buffer->vertices[2] = S3DVertex(1,1,0,  1, 1,-1, 0, 1);
	buffer->vertices[3] = S3DVertex(0,1,0, -1, 1,-1, 0, 0);
	buffer->vertices[4] = S3DVertex(1,0,1,  1,-1, 1, 1, 0);
	buffer->vertices[5] = S3DVertex(1,1,1,  1, 1, 1, 0, 0);
	buffer->vertices[6] = S3DVertex(0,1,1, -1, 1, 1, 0, 1);
	buffer->vertices[7] = S3DVertex(0,0,1, -1,-1, 1, 1, 1);
	buffer->vertices[8] = S3DVertex(0,1,1, -1, 1, 1, 1, 0);
	buffer->vertices[9] = S3DVertex(0,1,0, -1, 1,-1, 1, 1);
	buffer->vertices[10] = S3DVertex(1,0,1,  1,-1, 1, 0, 1);
	buffer->vertices[11] = S3DVertex(1,0,0,  1,-1,-1, 0, 0);

	for(i=0;i<12;i++) {
		buffer->vertices[i].pos -= Vector3(0.5f,0.5f,0.5f);
		buffer->vertices[i].pos *= size;
	}

	return buffer;
}

static void setTexture(u8 textureUnit, u32 numTex)
{
	u32 width = 128;
	u32 height = 128;
	u32 pitch = (width*4);
	gcmTexture texture;

	if(!texture_buffer[0]) return;

	rsxInvalidateTextureCache(context,GCM_INVALIDATE_TEXTURE);

	//texture.format		= (GCM_TEXTURE_FORMAT_A8R8G8B8 | GCM_TEXTURE_FORMAT_LIN);
	texture.format = (GCM_TEXTURE_FORMAT_A8R8G8B8 | GCM_TEXTURE_FORMAT_LIN | GCM_TEXTURE_FORMAT_NRM);

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
	texture.offset		= texture_offset[numTex];
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
	if (projMatrix)
		printf("projMatrix OK\n");
	mvMatrix = rsxVertexProgramGetConst(vpo,"modelViewMatrix");
	if (mvMatrix)
		printf("mvMatrix OK\n");
	rsxFragmentProgramGetUCode(fpo, &fp_ucode, &fpsize);
	printf("fpsize: %d\n", fpsize);

	fp_buffer = (u32*)rsxMemalign(64,fpsize);
	memcpy(fp_buffer,fp_ucode,fpsize);
	rsxAddressToOffset(fp_buffer,&fp_offset);

	textureUnit = rsxFragmentProgramGetAttrib(fpo,"texture");
	if (textureUnit)
		printf("textureUnit OK\n");
}

void drawFrame()
{
	u32 i,offset,color = 0;
	Matrix4 rotX,rotY;
	Matrix4 viewMatrix,modelMatrix,modelMatrixIT,modelViewMatrix;
	
	SMeshBuffer *mesh = NULL;

	setDrawEnv();
	setTexture(textureUnit->index, texture_buffer_idx);

	rsxSetClearColor(context,color);
	rsxSetClearDepthStencil(context,0xffffff00);
	rsxClearSurface(context,GCM_CLEAR_R |
							GCM_CLEAR_G |
							GCM_CLEAR_B |
							GCM_CLEAR_A |
							GCM_CLEAR_S |
							GCM_CLEAR_Z);

	rsxSetZControl(context,0,1,1);

	for(i=0;i<8;i++)
		rsxSetViewportClip(context,i,display_width,display_height);

	viewMatrix = Matrix4::lookAt(eye_pos,eye_dir,up_vec);

	mesh = cube;
	rotX = Matrix4::rotationX(DEGTORAD(rotx));
	rotY = Matrix4::rotationY(DEGTORAD(roty));
	modelMatrix = rotX*rotY;
	modelMatrixIT = Matrix4::identity();// inverse(modelMatrix);
	modelViewMatrix = transpose(viewMatrix*modelMatrix);

	rsxAddressToOffset(&mesh->vertices[0].pos,&offset);
	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_POS,0,offset,sizeof(S3DVertex),3,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);

	rsxAddressToOffset(&mesh->vertices[0].nrm,&offset);
	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_NORMAL,0,offset,sizeof(S3DVertex),3,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);

	rsxAddressToOffset(&mesh->vertices[0].u,&offset);
	rsxBindVertexArrayAttrib(context,GCM_VERTEX_ATTRIB_TEX0,0,offset,sizeof(S3DVertex),2,GCM_VERTEX_DATA_TYPE_F32,GCM_LOCATION_RSX);

	rsxLoadVertexProgram(context,vpo,vp_ucode);
	rsxSetVertexProgramParameter(context,vpo,projMatrix,(float*)&P);
	rsxSetVertexProgramParameter(context,vpo,mvMatrix,(float*)&modelViewMatrix);
	rsxLoadFragmentProgramLocation(context,fpo,fp_offset,GCM_LOCATION_RSX);

	rsxSetUserClipPlaneControl(context,GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE,
									   GCM_USER_CLIP_PLANE_DISABLE);

	rsxAddressToOffset(&mesh->indices[0],&offset);
	rsxDrawIndexArray(context,GCM_TYPE_TRIANGLES,offset,mesh->cnt_indices,GCM_INDEX_TYPE_16B,GCM_LOCATION_RSX);

}

int main(int argc,const char *argv[])
{
	padInfo padinfo;
	padData paddata;
	
	if (sysModuleLoad(SYSMODULE_PNGDEC) != 0) exit(0);

	png = new pngData;
	pngLoadFromBuffer(Stone_Texture_png_bin, Stone_Texture_png_bin_size, png);
	
	void *host_addr = memalign(HOST_ADDR_ALIGNMENT,HOSTBUFFER_SIZE);

	printf("rsxtest started...\n");

	init_screen(host_addr,HOSTBUFFER_SIZE);
	ioPadInit(7);
	init_shader();
	init_texture();

	DebugFont::init();
	DebugFont::setScreenRes(display_width, display_height);

	cube = createCube(5.0f);

	atexit(program_exit_callback);
	sysUtilRegisterCallback(0,sysutil_exit_callback,NULL);

	P = transpose(Matrix4::perspective(DEGTORAD(45.0f),aspect_ratio,1.0f,3000.0f));

	setDrawEnv();
	setRenderTarget(curr_fb);

	running = 1;
	while(running) {
		sysUtilCheckCallback();

		ioPadGetInfo(&padinfo);
		for(int i=0; i < MAX_PADS; i++){
			if(padinfo.status[i]){
				ioPadGetData(i, &paddata);

				if (paddata.BTN_DOWN) {
					rotx += 0.5f;				
				}
				if (paddata.BTN_UP) {
					rotx -= 0.5f;
				}
				if (rotx >= 360.0f || rotx <= 0.0f) rotx = fmodf(rotx, 360.0f);
				if (paddata.BTN_RIGHT) {
					roty += 0.5f;
				}
				if (paddata.BTN_LEFT) {
					roty -= 0.5f;
				}
				if (roty >= 360.0f || roty <= 0.0f) roty = fmodf(roty, 360.0f);
				if (paddata.BTN_CROSS)
					texture_buffer_idx++;
				if (texture_buffer_idx == 3)
					texture_buffer_idx = 0;
				if(paddata.BTN_CIRCLE)
					goto done;
			}

		}
		
		drawFrame();
		int ypos = 10;
		int xpos = 10;
		DebugFont::setPosition(xpos, ypos);
		DebugFont::setColor(1.0f, 1.0f, 1.0f, 1.0f);

		DebugFont::print("Left/Right Pad to Left/Right rotation");
		ypos += 12;
		DebugFont::setPosition(xpos, ypos);
		DebugFont::print("Up/Down Pad to Up/Down rotation");
		ypos += 12;
		DebugFont::setPosition(xpos, ypos);
		DebugFont::print("CROSS Button to change texture");
		ypos += 12;
		DebugFont::setPosition(xpos, ypos);
		DebugFont::print("CIRCLE Button to exit");

		flip();
	}

done:
    printf("rsxtest done...\n");
	DebugFont::shutdown();
    program_exit_callback();
    return 0;
}
