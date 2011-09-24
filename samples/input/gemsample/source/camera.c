#include <stdio.h>
#include <sys/memory.h>
#include <io/camera.h>
#include <ppu-types.h>
#include <../include/rsxutil.h>

sys_mem_container_t container;
//extern
 u8 *buf;
extern u16 width;
extern u16 height;
extern rsxBuffer buffers[MAX_BUFFERS];
extern int currentBuffer; 
cameraType type;

cameraInfoEx camInf;
cameraReadInfo camread;


void endCamera()
{
	cameraStop(0);
	
	cameraClose(0);
	cameraEnd();
  
	sysMemContainerDestroy(container);
	
}
int setupCamera()
{

	int ret;
	int flag1=0;
	cameraGetType(0, &type);
	if (type == CAM_TYPE_PLAYSTATION_EYE){
		flag1 = 1;
		camInf.format=CAM_FORM_RAW8;
		camInf.framerate=60;
		camInf.resolution=CAM_RESO_VGA;
		camInf.info_ver=0x0101;
		camInf.container=container;
		printf("Buffer at %08X\n", camInf.buffer);
		printf("Buffer at %08X\n", camInf.pbuf[0]);
		printf("Buffer at %08X\n", camInf.pbuf[1]);
		
		ret = cameraOpenEx(0, &camInf);
		switch(ret)
		{
			case CAMERA_ERRO_DOUBLE_OPEN:
				cameraClose(0);
				flag1=0;
				break;
			case CAMERA_ERRO_NO_DEVICE_FOUND:
				printf("This sample need a PlayStation Eye device\n");
				flag1=0;
				break;
			case 0:
				printf("Found me an eye, arrr!\n");
				printf("cameraOpenEx returned %08X\n", ret);
				printf("Video dimensions: %dx%d\n", camInf.width, camInf.height);
				printf("Buffer at %08X\n", camInf.buffer);
				printf("pbuf0 Buffer at %08X\n", camInf.pbuf[0]);
				printf("pbuf0 Buffer at %08X\n", camInf.pbuf[1]);
				printf("camInf.info_ver %X\n",camInf.info_ver);
				
				camread.buffer=camInf.buffer; 
				camread.version=0x0100;
				buf=(u8 *)(u64)camread.buffer;
				printf("Setting CameraReadEx %08X buffer to cameraInfoex buffer \n",camread.buffer);
				break;
			default:
				printf("Error %X detected opening PlayStation Eye\n",ret);
				flag1=0;
			
		}
		
	
	}
	else
	{
		printf("Device detected is not a PlayStation Eye and this sample need it\n");
		flag1=0;
		
	}
	return flag1;
}
int initCamera()
{
	int ret;
	ret=sysMemContainerCreate(&container, 0x200000);
	printf("sysMemContainerCreate() for camera container returned %d\n", ret);
	ret=cameraInit();
	printf("cameraInit() returned %d\n", ret);
	if(ret==0)
	{
		ret=setupCamera();
	}
	return ret;
	
	
}
int readCamera()
{	int ret;
	
	ret = cameraReadEx(0,&camread);
	switch(ret)
	{	
		
		case CAMERA_ERRO_NEED_START:
			cameraReset(0);
			ret=gemPrepareCamera(128, 0.5);
			printf("GemPrepareCamera return %d exposure set to 128 and quality to 0.5 before cameraStart\n",ret);
			printf("lets go!! It's time to look your face in Sony Bravia :P\n");
			ret=cameraStart(0);
			printf("cameraStart return %d \n",ret);
			break;
		case 0:
			break;
		default:
			printf("error %08X ",ret);
			ret=1;
			break;	
	}
//	printf("despues de start return %d \n",ret);
	if(ret==0 && camread.readcount!=0)
	{
		return camread.readcount;
	}
	else
	{
		return 0;
	}
	
	
}
u32 YUV_to_RGB(int y,int u,int v)
{
   int r,g,b;
   v -= 128;
   u -= 128;
   // Conversion
   r = y + u;
   g = y-u/2-v/8;
   b = y+v;

   // Clamp to 0..1
   if (r < 0) r = 0;
   if (g < 0) g = 0;
   if (b < 0) b = 0;
   if (r > 255) r = 255;
   if (g > 255) g = 255;
   if (b > 255) b = 255;


   return r << 16 | g << 8 | b;
}

void Convert422(u8* yuv, u32 *rgb1, u32 *rgb2)
{
	int y1,y2,u,v;

	// Extract yuv components
	y1 = yuv[0];
	v  = yuv[1];
	y2 = yuv[2];
	u  = yuv[3];

	// yuv to rgb
	*rgb1 = YUV_to_RGB(y1,u,v);
	*rgb2 = YUV_to_RGB(y2,u,v);
}
void displayCameraFrame(s32 posx,s32 posy)
{
		int i,j;
		u32 pixel1, pixel2;
		u8 *img = buf;
		int w=camInf.width;
		int h=camInf.height;
		
		if((readCamera())!=0){
		
			for(j=0;j<h;j++)
			{
				for(i=0;i<w;i+=2)
				{
					
					Convert422(img, &pixel1, &pixel2);
				
				
					
					img += 4;
					buffers[currentBuffer].ptr[(j+posy)*buffers[currentBuffer].width+i+posx]=pixel1;	
			
					buffers[currentBuffer].ptr[(j+posy)*buffers[currentBuffer].width+i+1+posx]=pixel2;
				
				}

			}
		}
				

}