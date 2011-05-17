/* Sample showing use of libgem. PlayStation Eye and PlayStation Move are needed. 
Author: bigboss 
Date: december 2010.
Minimun Firmware needed: 3.41  supported in 3.55 also.
 */
#include <psl1ght/lv2.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <sysutil/video.h>
#include <rsx/gcm.h>
#include <rsx/reality.h>

#include <io/pad.h>
#include <io/cam.h>
#include <io/move.h>
#include <sys/thread.h>
//#include <libgem/libgem.h>

#include <spurs/types.h>
#include <sysmodule/sysmodule.h>

#include <stdarg.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/net.h>
#include <arpa/inet.h>
#include <sysutil/events.h>

typedef struct {
	int height;
	int width;
	uint32_t *ptr;
	// Internal stuff
	uint32_t offset;
} buffer;

buffer *buffers[2];// The buffer we will be drawing into
gcmContextData *context; // Context to keep track of the RSX buffer.

VideoResolution res; // Screen Resolution

int currentBuffer = 0;

CameraReadEx readex;
CameraType type;
CameraInfoEx cameraInfo;

sys_mem_container_t container;
sys_mem_container_t container1;


GemAttribute gem_attr;
GemInfo gem_info;
GemVideoConvertAttribute gem_video_convert;
GemState gem_state;
GemInertialState gem_inertial_state;
u16 oldGemPad=0;
u16 newGemPad=0;
u16 newGemAnalogT=0;
int tracking=0;
float rx,ry;
int pos_x=0;
int pos_y=0;
int dx=200; //default step x
int dy=100; //default step y
GemImageState image_state;
void *buffer_mem;
void *video_out;


PadInfo padinfo;
PadData paddata;






static int SocketFD;
#define DEBUG_IP "192.168.0.1"    //change to your PC workstation or Mac  ip
#define DEBUG_PORT 18194
#define printf debugPrintf //i use debugPrintf on 3.55

void debugPrintf(const char* fmt, ...)
{
	char buffer[0x800];
	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg);
	va_end(arg);
	netSend(SocketFD, buffer, strlen(buffer), 0);
}
void debugInit()
{

	struct sockaddr_in stSockAddr;
	SocketFD = netSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	memset(&stSockAddr, 0, sizeof stSockAddr);

	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(DEBUG_PORT);
	inet_pton(AF_INET, DEBUG_IP, &stSockAddr.sin_addr);

	netConnect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr);
	

	
	debugPrintf("network debug module initialized\n") ;
	debugPrintf("ready to have a lot of fun\n") ;
	
	
}

void makeBuffer(int id, int size) {
	buffer *buf = malloc(sizeof(buffer));
	buf->ptr = rsxMemAlign(16, size);
	assert(buf->ptr != NULL);

	assert(realityAddressToOffset(buf->ptr, &buf->offset) == 0);
	// Register the display buffer with the RSX
	assert(gcmSetDisplayBuffer(id, buf->offset, res.width * 4, res.width, res.height) == 0);
	
	buf->width = res.width;
	buf->height = res.height;
	buffers[id] = buf;
}

void waitFlip() { // Block the PPU thread untill the previous flip operation has finished.
	while(gcmGetFlipStatus() != 0) 
		usleep(200);
	gcmResetFlipStatus();
}

void flip(s32 buffer) {
	assert(gcmSetFlip(context, buffer) == 0);
	realityFlushBuffer(context);
	gcmSetWaitFlip(context); // Prevent the RSX from continuing until the flip has finished.
}

// Initilize everything. You can probally skip over this function.
void init_screen() {
	// Allocate a 1Mb buffer, alligned to a 1Mb boundary to be our shared IO memory with the RSX.
	void *host_addr = memalign(1024*1024, 1024*1024);
	assert(host_addr != NULL);

	// Initilise Reality, which sets up the command buffer and shared IO memory
	context = realityInit(0x10000, 1024*1024, host_addr); 
	assert(context != NULL);

	VideoState state;
	assert(videoGetState(0, 0, &state) == 0); // Get the state of the display
	assert(state.state == 0); // Make sure display is enabled

	// Get the current resolution
	assert(videoGetResolution(state.displayMode.resolution, &res) == 0);
	
	// Configure the buffer format to xRGB
	VideoConfiguration vconfig;
	memset(&vconfig, 0, sizeof(VideoConfiguration));
	vconfig.resolution = state.displayMode.resolution;
	vconfig.format = VIDEO_BUFFER_FORMAT_XRGB;
	vconfig.pitch = res.width * 4;

	assert(videoConfigure(0, &vconfig, NULL, 0) == 0);
	assert(videoGetState(0, 0, &state) == 0); 

	s32 buffer_size = 4 * res.width * res.height; // each pixel is 4 bytes
	
	gcmSetFlipMode(GCM_FLIP_VSYNC); // Wait for VSYNC to flip

	gcmSetFlipMode(GCM_FLIP_VSYNC); // Wait for VSYNC to flip

	// Allocate two buffers for the RSX to draw to the screen (double buffering)
	makeBuffer(0, buffer_size);
	makeBuffer(1, buffer_size);

	gcmResetFlipStatus();
	flip(1);

}

void fillFrame(u32 *b, u32 color) {
	s32 i, j;
	for(i = 0; i < res.height; i++) {
		for(j = 0; j < res.width; j++)
			b[i* res.width + j] = color;
	}

}
void fillVideoOut(u32 *b, u32 color) {
	s32 i, j;
	for(i = 0; i < 480; i++) {
		for(j = 0; j < 640; j++)
			b[i* 640 + j] = color;
	}

}
void appCleanup(){
	gemEnd();
	cameraStop(0);
	
	cameraClose(0);
	cameraEnd();
  
	lv2MemContinerDestroy(container);
	lv2MemContinerDestroy(container1);
	SysUnloadModule(SYSMODULE_MOVE);
	SysUnloadModule(SYSMODULE_CAM);
	sysUnregisterCallback(EVENT_SLOT0);
	printf("Exiting for real.\n");
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
static void eventHandle(u64 status, u64 param, void * userdata) {
    (void)param;
    (void)userdata;
	if(status == EVENT_REQUEST_EXITAPP){
		printf("Quit game requested\n");
		exit(0);
	}else{
		printf("Unhandled event: %08llX\n", (unsigned long long int)status);
	}
}

int readPad()
{
	int ret=1;

	int i;
	ioPadGetInfo(&padinfo);
	for(i=0; i<6; i++){  //7 is our Move device
		if(padinfo.status[i]){
			ioPadGetData(i, &paddata);
			
			if(paddata.BTN_CROSS){
			
				ret=0; //To exit it will go to XMB
			}
			
		}
		
	}
	return ret;
	
}
int proccessGem(int t)
{
	int ret;
	switch(t)
	{
		
			
		case 0:
			ret=gemUpdateStart((void*)(u64)readex.buffer, readex.timestamp);
			if(ret!=0)
			{
				printf("Return from gemUpdateStart %X\n",ret);
			}
		break;
		case 1:
		
			ret=gemConvertVideoStart((void*)(u64)readex.buffer);
			if(ret!=0)
			{
				printf("Return from gemConvertVideoStart %X\n",ret);
			}
			break;
		case 2:

			ret=gemUpdateFinish();
			if(ret!=0)
			{
				printf("Return from gemUpdateFinish %X\n",ret);
			}
			break;
		case 3:
			ret=gemConvertVideoFinish();
			if(ret!=0)
			{
				printf("Return from gemConvertVideoFinish %X\n",ret);
			}
			break;
		default:
			ret= -1;
			break;
		
	}
	return ret;
	
	
}
void displayFrame(buffer *buffer,u32 * img,s32 posx,s32 posy,s32 w, s32 h)
{
		int i,j;
		for(j=0;j<h;j++)
		{
			for(i=0;i<w;i++)
			{		
				(buffer->ptr)[(j+posy)*res.width + i+posx] = img[j*w+i];
				
			}

		}
}

void displayCameraFrame(buffer *buffer,u8* img,s32 posx,s32 posy,s32 w, s32 h)
{
		int i,j;
		u32 pixel1, pixel2;
	
		
			for(j=0;j<h;j++)
			{
				for(i=0;i<w;i+=2)
				{
					
					Convert422(img, &pixel1, &pixel2);
				
				
					
					img += 4;
					(buffer->ptr)[(j+posy)*res.width + i+posx] = pixel1;
				//	framex[j*640+i]=pixel1;
					(buffer->ptr)[(j+posy)*res.width + i+1+posx]= pixel2;
				//	framex[j*640+i+1]=pixel2;
				}

			}
				

}
int readCamera()
{	int ret;
	
	ret = cameraReadEx(0,&readex);
	switch(ret)
	{	
		
		case CAMERA_ERRO_NEED_START:
			cameraReset(0);
			ret=gemPrepareCamera(500, 0.5);
			printf("GemPrepareCamera return %d exposure set to 500 and quality to 0.5 before cameraStart\n",ret);
			printf("lets go!! It's time to look your face in Sony Bravia :P\n");
			cameraStart(0);
			break;
		case 0:
			break;
		default:
			ret=1;
			break;	
	}
	if(ret==0 && readex.readcount!=0)
	{
		return readex.readcount;
	}
	else
	{
		return 0;
	}
	
	
}
int setupCamera()
{
	int ret;
	int flag1=0;
	cameraGetType(0, &type);
	if (type == CAM_TYPE_PLAYSTATION_EYE){
		flag1 = 1;
		cameraInfo.format=CAM_FORM_YUV422;
		cameraInfo.framerate=30;
		cameraInfo.resolution=CAM_RESO_VGA;
		cameraInfo.info_ver=0x0200;
		cameraInfo.container=container;
		
		ret = cameraOpenEx(0, &cameraInfo);
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
				printf("Video dimensions: %dx%d\n", cameraInfo.width, cameraInfo.height);
				printf("Buffer at %08X\n", cameraInfo.buffer);
				printf("Setting CameraReadEx buffer to cameraInfo buffer\n");
				readex.buffer=cameraInfo.buffer; 
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
int calibrateGem(int numgem)
{
	int ret=0;
	int ret1;
	int flag=0;
	int padflag=1;
	int error=0;
	u32 hues[4];
	hues[0]=4<<24;//to leave Sony choose the best hue value when it is calibrating internally
	hues[1]=4<<24;
	hues[2]=4<<24;
	hues[3]=4<<24;
	while(!flag && !error && padflag)
	{
		padflag=readPad();
		ret1=gemCalibrate(numgem);
			if (readCamera()!=0)
			{
				proccessGem(0);
				proccessGem(1);
				proccessGem(2);
				proccessGem(3);
				u8 * buf = (u8*)(u64)cameraInfo.buffer;
				displayCameraFrame(buffers[currentBuffer],buf,0,0,cameraInfo.width,cameraInfo.height);
				ret=gemGetState(numgem,1,0,&gem_state);
		//		printf("internal gemGetState %d \n",ret);
			}
		ret=gemGetState(numgem,1,0,&gem_state);
		switch(ret)
		{
			case 1:
				printf("status %d No Move device connected\n",ret);
				break;
			case 2:
				printf("status %d first calibrate device\n",ret);
				ret1=gemCalibrate(numgem);
				printf("GemCalibrate return %X\n",ret1);
				printf("Calibrating is in proccess...\n");
				break;
			case 3:
				printf("status %d Calibrating is in proccess...\n",ret);
				break;
			case 4:
				printf("status %d Computing colors in proccess...\n",ret);
				break;
			case 5:
				printf("status %d Second set a hue value to track or let Sony choose for you :P\n",ret);
				ret1=gemTrackHues(hues, NULL);
				printf("GemTrackHues return %X\n",ret1);
				
				break;	
			case 6:
				printf("status %d No video\n",ret);
				break;
			case 7:
				printf("status %d No state available at that time\n",ret);
				break;
			case 0:
				printf("status %d Move calibrated and ready!!\n",ret);
				printf("Yeah Move it's running fine, light on!!!!!\n");
				
				 flag=1;
				break;
			default:
				printf("Error %X return by gemGetState \n",ret);
				error=1;
		
		}
	
	//	flip(currentBuffer); // Flip buffer onto screen
	//	currentBuffer = !currentBuffer;
	
		
		
	}
	if(padflag==0)
	{
		ret=1;
	}
	
	return ret;
	
}

int initGem()
{
	int ret;
	int i;
	
	ret=gemGetMemorySize(1);
	printf("return from GemGetMemorySize %X size in bytes needed for move device to init libgem\n",ret);
	ret = lv2MemContinerCreate(	&container1, ret);

	Spurs spurs;
	

	
	printf("lv2MemContinerCreate() for memory usage needed in libgem returned %d\n", ret);
	printf("preparing GemAttribute structure with sprus and memory stuff is very important align correctly spurs structure for this reason i chaged move.h to \n");
	
	gem_attr.version = 2;
	gem_attr.max = 1;
	gem_attr.spurs = (u32)(u64)&spurs;
	gem_attr.memory=(u32)(u64)container1;
	for( i=0; i<8; ++i) 	gem_attr.spu_priorities[i] = 16;
	printf("calling GemInit with GemAttribute structure version=%d max_connect=%d spurs=%X memory_ptr=%X  \n",gem_attr.version,gem_attr.max,gem_attr.spurs,gem_attr.memory);
	ret=gemInit(&gem_attr);
	printf("return from GemInit %X \n",ret);
	printf("Preparing GemVideoConvert structure \n");
	
	gem_video_convert.version=2;
	gem_video_convert.format=GEM_RGBA_640x480;
	gem_video_convert.conversion= GEM_AUTO_WHITE_BALANCE|GEM_COMBINE_PREVIOUS_INPUT_FRAME|GEM_FILTER_OUTLIER_PIXELS|GEM_GAMMA_BOOST;
	gem_video_convert.gain=1.0f;
	gem_video_convert.red_gain=1.0f;
	gem_video_convert.green_gain=1.0f;
	gem_video_convert.blue_gain=1.0f;
	buffer_mem=(void *)memalign(128,640*480);
	video_out=(void *)malloc(640*480*4);  //TODO always is empty i don't know why		
	gem_video_convert.buffer_memory=(u64)buffer_mem;
	gem_video_convert.video_data_out=(u64)video_out;
	gem_video_convert.alpha=255;
		
			
	ret=gemPrepareVideoConvert(&gem_video_convert); //return 0 but it does not work like i want TODO
	printf("GemPrepareVideoConvert return %X  \n",ret);
			
	ret=gemPrepareCamera(500, 0.5);
	printf("GemPrepareCamera return %d exposure set to 500 and quality to 0.5\n",ret); 
	ret=gemReset(0);
	printf("GemReset return %X \n",ret);
	return ret;
	
}
void readGemPad(int num_gem)
{
	int ret;
	ret=gemGetState(0,0,0,&gem_state);
	if(ret!=0)
	{
		printf("Error %X  in gemGetState \n ",ret);
	}
	
	newGemPad=gem_state.paddata.buttons & (~oldGemPad);
	newGemAnalogT=gem_state.paddata.ANA_T;
	oldGemPad= gem_state.paddata.buttons;
	
}
void readGemAccPosition(int num_gem)
{	
	vec_float4 position;
	gemGetAccelerometerPositionInDevice(num_gem,&position);

	printf(" accelerometer device coordinates [%f,%f,%fm,%f]\n",position.array[0],position.array[1],position.array[2],position.array[3]);
	
}
void readGemInertial(int num_gem)
{
	int ret;
	ret=gemGetInertialState(num_gem, 0, 0, &gem_inertial_state);
	printf("gemGetInertialState return %X\n",ret);
	printf("counter %d temperature %f\n",gem_inertial_state.counter,gem_inertial_state.temperature);

	
		printf(" accelerometer sensor   [%f,%f,%f,%f]\n",gem_inertial_state.accelerometer.array[0],gem_inertial_state.accelerometer.array[1],gem_inertial_state.accelerometer.array[2],gem_inertial_state.accelerometer.array[3]);

	
		printf(" accelerometer_bias sensor   [%f,%f,%f,%f]\n",gem_inertial_state.accelerometer_bias.array[0],gem_inertial_state.accelerometer_bias.array[1],gem_inertial_state.accelerometer_bias.array[2],gem_inertial_state.accelerometer_bias.array[3]);
	
		printf(" gyro sensor  [%f,%f,%f,%f]\n",gem_inertial_state.gyro.array[0],gem_inertial_state.gyro.array[1],gem_inertial_state.gyro.array[2],gem_inertial_state.gyro.array[3]);
	
		printf(" gyro_bias sensor  [%f,%f,%f,%f]\n",gem_inertial_state.gyro_bias.array[0],gem_inertial_state.gyro_bias.array[1],gem_inertial_state.gyro_bias.array[2],gem_inertial_state.gyro_bias.array[3]);
	
	
}
s32 main(s32 argc, const char* argv[])
{

	
	int p;
	netInitialize();
	
	debugInit();
	p=SysLoadModule(SYSMODULE_CAM);
	printf("cam return %X\n",p);
	p=SysLoadModule(SYSMODULE_MOVE);
	printf("move return %X\n",p);
	atexit(appCleanup);
	
	sysRegisterCallback(EVENT_SLOT0, eventHandle, NULL);
	
	int ret;
	int running = 1, cameraSetup = 0, calibrate=0;
	

	
	
	ret = lv2MemContinerCreate(&container, 0x200000);
	printf("lv2MemContinerCreate() returned %d\n", ret);
	
	printf("Initializing screen...\n");
	
	init_screen();
	
	printf("Initializing pad...\n");
	
	ioPadInit(6);
	
	printf("cameraInit() returned %d\n", cameraInit());
	
	initGem();
	
	
	
	// Ok, everything is setup. Now for the main loop.
	while(running){
		// Check the pads.
		running=readPad();

		waitFlip(); // Wait for the last flip to finish, so we can draw to the old buffer
		fillFrame(buffers[currentBuffer]->ptr, 0x00FFFFFF); // Draw into the unused buffer
		
		
		
		
		if(!cameraSetup){ //If camera is not setup already setup is done here
			
			cameraSetup=setupCamera();
			if(!cameraSetup)
			{
				running=0;   //If there is not a PlayStation Eye exit and we will go to XMB
			}
		}
		else
		{	
			if (readCamera()!=0){  //we read frame and begin to proccess with libgem if all was fine with Eye
				
				proccessGem(0);
				
				proccessGem(2);
				
				proccessGem(1); //about gemConvertVideoStart and gemConvertVideoFinish  i am not sure, output_buffer is always empty in 3.41 more tests are needed, however both return ok
				
				proccessGem(3);
				
			
				
				if(calibrate==0) //If calibrate is not done already here we make calibration. Point your move control to Eye and press Move Button
				{
					ret=gemGetState(0,0,0,&gem_state);
				//	printf("GemGetState return %d\n",ret);
				//	printf("GemGetState return %d\n",gem_state.paddata.buttons);
					if(gem_state.paddata.buttons==4) 
					{
						printf("Move button pressed, time to make calibration\n");
						ret=calibrateGem(0);
						if(ret==0) //all was fine calibrate done!!! now you will have your Move sphere iluminated
						{
							calibrate=1;
						
							printf("Calibrating proccess finalized\n");						
						}
						else
						{	//Control error is done in calibrateGem and showed in debug output so if something was wrong exit and we will go to xmb
							printf("return %d \n",ret);
							running=0;
							
						}
					}
				}
				else  //calibrating is done and now we will play with Move
				{
					readGemPad(0); //This will read buttons from Move 
					switch(newGemPad) 					//Buttons stuff i don't need to explain it. Don't you? :P. 
					{                                   //Press Triangle in Move and gyro on x and y axis will be tracked to move camera frame. Move your control to see it.
														//Press Circle in Move and tracking gyro is stopped
														//Press Select in Move to decrement step on image in display
						case 1:							//Press Trigger to show world coordinate postion of sphere. TODO
							printf("Select pressed \n");//Press Move button in Move to increment step on image display
							dx=dx-1;					//Press Cross to get accelerometer postion in device coordinates alwasys fixed 
							dy=dy-1;					//Press Square to get inertial sensor state
							if(dx<1)
							{
								dx=1;
								
							}
							if(dy<1)
							{
									dy=1;

							}
							printf("%d dx %d dy\n",dx,dy);
							break;
						case 2:
							printf("T pressed value %d\n",newGemAnalogT);
							printf("Frame %d center of the sphere in world coordinates %f %f %f %f \n",readex.frame,gem_state.pos.array[0],gem_state.pos.array[1],gem_state.pos.array[2],gem_state.pos.array[3]);
							// many times positions return nan values or cero TODO learn how to work with this to accurate tracking position. 
							break;
						case 4:
							printf("Move pressed \n");
							dx=dx+1;
							dy=dy+1;
							if(dx>200)
							{
								dx=200;
								
							}
							if(dy>200)
							{
									dy=200;

							}
							printf("%d dx %d dy\n",dx,dy);
							
							break;
						case 8:
							printf("Start pressed \n");
							pos_x=0;
							pos_y=0;
							break;
						case 16:
							printf("Triangle pressed \n");
							printf("Start tracking gyro sensor\n");
							tracking=1;
							break;
						case 32:
							printf("Circle pressed \n");
							printf("Stop tracking gyro sensor\n");
							tracking=0;
							break;
						case 64:
							printf("Cross pressed \n");
							readGemAccPosition(0);
							break;
						case 128:
							printf("Square pressed \n");
							readGemInertial(0);
							break;
						default:
							break;	
					}
					if(tracking)
					{
						ret=gemGetInertialState(0, 0, 0, &gem_inertial_state);
						if( (gem_inertial_state.gyro_bias.array[0]!=0.00000) &&  (gem_inertial_state.gyro_bias.array[1]!=0.00000) &&  (gem_inertial_state.gyro_bias.array[2]!=0.00000))
						{
							
							rx=gem_inertial_state.gyro.array[1]; //x rotation negative right, postive left
							ry=gem_inertial_state.gyro.array[0]; //y rotation negative down, positive up
							pos_x=pos_x-(int)(rx*dx);
							pos_y=pos_y-(int)(ry*dy);
							if(pos_x<0)
							{
								pos_x=0;
							}
							if(pos_x>res.width-cameraInfo.width)
							{
								pos_x=res.width-cameraInfo.width;
							}
							if(pos_y<0)
							{
									pos_y=0;
							}
							if(pos_y>res.height-cameraInfo.height)
							{
									pos_y=res.height-cameraInfo.height;
							}
					
						}
					}
				}
			
					u8 * buf = (u8*)(u64)cameraInfo.buffer;
					displayCameraFrame(buffers[currentBuffer],buf,pos_x,pos_y,cameraInfo.width,cameraInfo.height);  //Display camera frame default pos_x=0, pos_y=0 upper left in your TV Display
				
			}
		}
		
			flip(currentBuffer); // Flip buffer onto screen
			currentBuffer = !currentBuffer;
			sysCheckCallback();
		
	}
			
				
		
		
		
	
	
	return 0;
}
