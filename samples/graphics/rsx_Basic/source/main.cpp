#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-types.h>

#include <sys/process.h>

#include <io/pad.h>
#include <rsx/rsx.h>
#include <sysutil/sysutil.h>

#include "acid.h"
#include "mesh.h"
#include "rsxutil.h"

#include "diffuse_specular_shader_vpo.h"
#include "diffuse_specular_shader_fpo.h"

typedef struct
{
	float x, y, z;
	u32 rgba;
} Vertex_t;

Vertex_t* vertex_buffer;
u32 VertexBufferOffset;

u32 running = 0;

u32 fp_offset;
u32 *fp_buffer;



// vertex shader
rsxProgramConst  *projMatrix;

rsxProgramAttrib* mPosIndex = NULL;

rsxProgramAttrib* mColIndex = NULL;

u32 color_index;
u32 position_index;


void *vp_ucode = NULL;
rsxVertexProgram *vpo = (rsxVertexProgram*)diffuse_specular_shader_vpo;

void *fp_ucode = NULL;
rsxFragmentProgram *fpo = (rsxFragmentProgram*)diffuse_specular_shader_fpo;


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

	projMatrix = rsxVertexProgramGetConst(vpo, "modelViewProj");
		
	mPosIndex = rsxVertexProgramGetAttrib(vpo, "position");
		
	mColIndex = rsxVertexProgramGetAttrib(vpo, "color");
	
	rsxFragmentProgramGetUCode(fpo, &fp_ucode, &fpsize);

	fp_buffer = (u32*)rsxMemalign(64,fpsize);
	memcpy(fp_buffer,fp_ucode,fpsize);

	position_index = mPosIndex->index;
	color_index = mColIndex->index;

	rsxAddressToOffset(fp_buffer,&fp_offset);

}

void drawFrame()
{
	u32 i, color;
	
	setDrawEnv();

	// Screen clear color between red and blue
	static float count = 0;
	count += 0.1f;
	unsigned char red = ((int)count) % 255;
	unsigned char green = 32;
	unsigned char blue = (255 - (int)count) % 255;
	color = (blue << 0) | (green << 8) | (red << 16) | (255 << 24);
	// Otherwise
	//color =  0; // -> Black;

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

	Matrix4 tempMatrix = transpose(Matrix4::identity());
	
	rsxAddressToOffset((void*)vertex_buffer, &VertexBufferOffset);
	
	rsxBindVertexArrayAttrib(context, position_index, 0, VertexBufferOffset, sizeof(Vertex_t), 3, GCM_VERTEX_DATA_TYPE_F32, GCM_LOCATION_RSX);
	rsxBindVertexArrayAttrib(context, color_index, 0, VertexBufferOffset + sizeof(float) * 3, sizeof(Vertex_t), 3, GCM_VERTEX_DATA_TYPE_U8, GCM_LOCATION_RSX);

	rsxLoadVertexProgram(context, vpo, vp_ucode);
	rsxSetVertexProgramParameter(context, vpo, projMatrix, (float*)&tempMatrix);
	rsxLoadFragmentProgramLocation(context, fpo, fp_offset, GCM_LOCATION_RSX);
	rsxDrawVertexArray(context, GCM_TYPE_TRIANGLES, 0, 3);
	
}

int main(int argc,const char *argv[])
{
	padInfo padinfo;
	padData paddata;
	void *host_addr = memalign(HOST_ADDR_ALIGNMENT,HOSTBUFFER_SIZE);

	printf("rsxtest started...\n");

	init_screen(host_addr,HOSTBUFFER_SIZE);
	ioPadInit(7);

	//Create Triangle
	void* ret = rsxMemalign(128, sizeof(Vertex_t) * 3);
	vertex_buffer = (Vertex_t*)ret;
	vertex_buffer[0].x = -1.0f;
	vertex_buffer[0].y = -1.0f;
	vertex_buffer[0].z = -1.0f;
	vertex_buffer[0].rgba = 0x00ff0000;

	vertex_buffer[1].x = 1.0f;
	vertex_buffer[1].y = -1.0f;
	vertex_buffer[1].z = -1.0f;
	vertex_buffer[1].rgba = 0x0000ff00;

	vertex_buffer[2].x = 0.0f;
	vertex_buffer[2].y = 1.0f;
	vertex_buffer[2].z = -1.0f;
	vertex_buffer[2].rgba = 0xff000000;

	init_shader();
	

	atexit(program_exit_callback);
	sysUtilRegisterCallback(0,sysutil_exit_callback,NULL);

	setDrawEnv();
	setRenderTarget(curr_fb);

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
    printf("rsxtest done...\n");
	program_exit_callback();
    return 0;
}
