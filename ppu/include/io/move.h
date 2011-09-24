#ifndef __LV2_MOVE_H__
#define __LV2_MOVE_H__

#include <ppu-types.h>
#include <vec_types.h>

#define MOVE_VERSION						2
#define MAX_MOVES							4
#define EXTERNAL_PORT_DATA_SIZE				32

#define GEM_TRACKING_POSITION_TRACKED		1
#define GEM_TRACKING_VISIBLE				2

#define STATE_CURRENT_TIME					0
#define STATE_LATEST_IMAGE_TIME				1
#define STATE_SPECIFY_TIME					2

#define GEM_AUTO_WHITE_BALANCE				1
#define GEM_GAMMA_BOOST						2
#define GEM_COMBINE_PREVIOUS_INPUT_FRAME	4
#define GEM_FILTER_OUTLIER_PIXELS			8

#define GEM_INERTIAL_LATEST					0
#define GEM_INERTIAL_PREVIOUS				1
#define GEM_INERTIAL_NEXT					2


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	GEM_NO_VIDEO_OUTPUT=1,
	GEM_RGBA_640x480,
	GEM_YUV_640x480,
	GEM_YUV422_640x480,
	GEM_YUV411_640x480,
	GEM_RGBA_320x240,
	GEM_BAYER_RESTORED,
	GEM_BAYER_RESTORED_RGGB,
	GEM_BAYER_RESTORED_RASTERIZED
} gemVideoConvertFormatEnum;

typedef struct _gem_ext_port_data
{
	u16 isconnected;
	
	unsigned int				:  8;		/* reserved */
	/* Button information */
	/* 0: UP, 1: DOWN     */
	unsigned int BTN_LEFT 		:  1;
	unsigned int BTN_DOWN 		:  1;
	unsigned int BTN_RIGHT 		:  1;
	unsigned int BTN_UP 		:  1;
	unsigned int BTN_START 		:  1;
	unsigned int BTN_R3 		:  1;
	unsigned int BTN_L3 		:  1;
	unsigned int BTN_SELECT		:  1;
	
	unsigned int				:  8;		/* reserved */
	unsigned int BTN_SQUARE 	:  1;
	unsigned int BTN_CROSS 		:  1;
	unsigned int BTN_CIRCLE 	:  1;
	unsigned int BTN_TRIANGLE	:  1;
	unsigned int BTN_R1 		:  1;
	unsigned int BTN_L1 		:  1;
	unsigned int BTN_R2 		:  1;
	unsigned int BTN_L2			:  1;
	
	/* Analog nub information */
	/* 0x0000 - 0x00FF        */
	unsigned int ANA_R_H		: 16;
	unsigned int ANA_R_V		: 16;
	unsigned int ANA_L_H		: 16;
	unsigned int ANA_L_V		: 16;
	
	u8 data[5];
} gemExtPortData;

typedef struct _gem_pad_data
{
	u16 buttons;
	u16 ANA_T;
} gemPadData;

typedef struct _gem_state
{
    vec_float4 pos;
    vec_float4 vel;
    vec_float4 accel; 
    vec_float4 quat;
    vec_float4 angvel;
    vec_float4 angaccel;
    vec_float4 handle_pos;
    vec_float4 handle_vel;
    vec_float4 handle_accel;
    gemPadData paddata;
    gemExtPortData extportdata;
    system_time_t time;
    f32 temperature;
    f32 camera_pitch_angle;
    u32 tracking;
} gemState;

typedef struct _gem_attribute
{
	u32 version;
	u32 max;
	void * memory ATTRIBUTE_PRXPTR; //pointer to memory to use for gem lib, if you specify null it will automatically allocate for you
	Spurs *spurs ATTRIBUTE_PRXPTR; //pointer to Spurs object
	u8 spu_priorities[8]; 
} gemAttribute;

typedef struct _gem_cam_state 
{
  s32 exposure;
  f32 exposure_time;
  f32 gain;
  f32 pitch_angle;
  f32 pitch_angle_estimate;
} gemCameraState;

typedef struct _gem_img_state {
	system_time_t frame_time;
	system_time_t time;
	f32 u;
	f32 v;
	f32 r;
	f32 projectionx;
	f32 projectiony;
	f32 distance;
	u8 visible;
	u8 r_valid;
} gemImageState;

typedef struct _gem_info
{
    u32 max;
    u32 connected;
    u32 status[MAX_MOVES];
    u32 port[MAX_MOVES];
} gemInfo;


typedef struct _gem_video_convert_attribute {
	s32 version; // set to MOVE_VERSION
	gemVideoConvertFormatEnum format;
	s32 conversion;
	f32 gain;
	f32 red_gain;
	f32 green_gain;
	f32 blue_gain;
	void *buffer_memory ATTRIBUTE_PRXPTR;
	void *video_data_out ATTRIBUTE_PRXPTR;
	u8 alpha;
} gemVideoConvertAttribute;

typedef struct _gem_inertial_state
{
	vec_float4 accelerometer;
	vec_float4 gyro;
	vec_float4 accelerometer_bias;
	vec_float4 gyro_bias;
	gemPadData pad;
	gemExtPortData ext;
	system_time_t  time;
	s32 counter;
	f32 temperature;
} gemInertialState;

s32 gemGetEnvironmentLightingColor(f32* r, f32* g, f32* b);
s32 gemUpdateStart(const void* camera_frame, system_time_t timestamp);
s32 gemInit(const gemAttribute* attr);
s32 gemGetTrackerHue(u32 num, u32* hue);
s32 gemConvertVideoFinish();
s32 gemEnableMagnetometer(u32 num, s32 enable);
s32 gemGetRGB(u32 num, f32* r, f32* g, f32* b);
s32 gemWriteExternalPort(u32 num, u8 data[EXTERNAL_PORT_DATA_SIZE]);

//hues points to an array that can hold 360 boolean values, my assumption is 360 bytes or 360 chars.
s32 gemGetAllTrackableHues(u8* hues);
s32 gemGetMemorySize(s32 max);
s32 gemUpdateFinish();
s32 gemEnableCameraPitchAngleCorrection(s32 enable);

s32 gemGetRumble(u32 num, u8* intensity);
s32 gemSetRumble(u32 num, u8 intensity);

s32	gemGetState(u32 num, u32 timeflag, system_time_t  time, gemState* state);

s32 gemGetAccelerometerPositionInDevice(u32 num, vec_float4* pos);
s32 gemConvertVideoStart(const void* frame);

s32 gemFilterState(u32 num, u32 enable);
s32 gemSetYaw(u32 num, vec_float4 zdir);
s32 gemGetCameraState(gemCameraState* state);

s32 gemTrackHues(const u32* req_hues, u32* res_hues);

s32 gemGetImageState(u32 num, gemImageState* state);

s32 gemClearStatusFlags(u32 num, u64 mask);

s32 gemGetInfo(gemInfo* info);

s32 gemPrepareCamera(s32 maxexposure, f32 quality);

s32 gemCalibrate(u32 num);

s32 gemGetHuePixels(const void* frame, u32 hue, u8* pixels);
s32 gemPrepareVideoConvert(const gemVideoConvertAttribute* attr);
s32 gemHSVtoRGB(f32 h, f32 s, f32 v, f32* r, f32* g, f32* b);
s32 gemForceRGB(u32 num, f32 r, f32 g, f32 b);
s32 gemGetInertialState(u32 num, u32 flag, system_time_t time, gemInertialState* inertial);
s32 gemReset(u32 num);
s32 gemEnd();
s32 gemInvalidateCalibration(u32 num);
s32 gemIsTrackableHue(u32 hue);
s32 gemGetStatusFlags(u32 num, u64* flags);


#ifdef __cplusplus
	}
#endif

#endif
