#ifndef __LV2_MOVE_H__
#define __LV2_MOVE_H__

#include <ppu-types.h>
#include <vec_types.h>
#include <spurs/types.h>

#define CELL_MOVE_VERSION						2
#define CELL_MAX_MOVES							4
#define CELL_EXTERNAL_PORT_DATA_SIZE			32

#define CELL_GEM_TRACKING_POSITION_TRACKED		1
#define CELL_GEM_TRACKING_VISIBLE				2

#define CELL_STATE_CURRENT_TIME					0
#define CELL_STATE_LATEST_IMAGE_TIME			1
#define CELL_STATE_SPECIFY_TIME					2

#define CELL_GEM_AUTO_WHITE_BALANCE				1
#define CELL_GEM_GAMMA_BOOST					2
#define CELL_GEM_COMBINE_PREVIOUS_INPUT_FRAME	4
#define CELL_GEM_FILTER_OUTLIER_PIXELS			8

#define CELL_GEM_INERTIAL_LATEST				0
#define CELL_GEM_INERTIAL_PREVIOUS				1
#define CELL_GEM_INERTIAL_NEXT					2


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	CELL_GEM_NO_VIDEO_OUTPUT=1,
	CELL_GEM_RGBA_640x480,
	CELL_GEM_YUV_640x480,
	CELL_GEM_YUV422_640x480,
	CELL_GEM_YUV411_640x480,
	CELL_GEM_RGBA_320x240,
	CELL_GEM_BAYER_RESTORED,
	CELL_GEM_BAYER_RESTORED_RGGB,
	CELL_GEM_BAYER_RESTORED_RASTERIZED
} CellGemVideoConvertFormatEnum;

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
} CellGemExtPortData;

typedef struct _gem_pad_data
{
	u16 buttons;
	u16 ANA_T;
} CellGemPadData;

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
    CellGemPadData paddata;
    CellGemExtPortData extportdata;
    system_time_t time;
    f32 temperature;
    f32 camera_pitch_angle;
    u32 tracking;
} CellGemState;

typedef struct _gem_attribute
{
	u32 version;
	u32 max;
	void * memory ATTRIBUTE_PRXPTR; //pointer to memory to use for gem lib, if you specify null it will automatically allocate for you
	CellSpurs *spurs ATTRIBUTE_PRXPTR; //pointer to CellSpurs object
	u8 spu_priorities[8]; 
} CellGemAttribute;

typedef struct _gem_cam_state 
{
  s32 exposure;
  f32 exposure_time;
  f32 gain;
  f32 pitch_angle;
  f32 pitch_angle_estimate;
} CellGemCameraState;

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
} CellGemImageState;

typedef struct _gem_info
{
    u32 max;
    u32 connected;
    u32 status[CELL_MAX_MOVES];
    u32 port[CELL_MAX_MOVES];
} CellGemInfo;


typedef struct _gem_video_convert_attribute {
	s32 version; // set to MOVE_VERSION
	CellGemVideoConvertFormatEnum format;
	s32 conversion;
	f32 gain;
	f32 red_gain;
	f32 green_gain;
	f32 blue_gain;
	void *buffer_memory ATTRIBUTE_PRXPTR;
	void *video_data_out ATTRIBUTE_PRXPTR;
	u8 alpha;
} CellGemVideoConvertAttribute;

typedef struct _gem_inertial_state
{
	vec_float4 accelerometer;
	vec_float4 gyro;
	vec_float4 accelerometer_bias;
	vec_float4 gyro_bias;
	CellGemPadData pad;
	CellGemExtPortData ext;
	system_time_t  time;
	s32 counter;
	f32 temperature;
} CellGemInertialState;

s32 cellGemGetEnvironmentLightingColor(f32* r, f32* g, f32* b);
s32 cellGemUpdateStart(const void* camera_frame, system_time_t timestamp);
s32 cellGemInit(const CellGemAttribute* attr);
s32 cellGemGetTrackerHue(u32 num, u32* hue);
s32 cellGemConvertVideoFinish();
s32 cellGemEnableMagnetometer(u32 num, s32 enable);
s32 cellGemGetRGB(u32 num, f32* r, f32* g, f32* b);
s32 cellGemWriteExternalPort(u32 num, u8 data[CELL_EXTERNAL_PORT_DATA_SIZE]);

//hues points to an array that can hold 360 boolean values, my assumption is 360 bytes or 360 chars.
s32 cellGemGetAllTrackableHues(u8* hues);
s32 cellGemGetMemorySize(s32 max);
s32 cellGemUpdateFinish();
s32 cellGemEnableCameraPitchAngleCorrection(s32 enable);

s32 cellGemGetRumble(u32 num, u8* intensity);
s32 cellGemSetRumble(u32 num, u8 intensity);

s32	cellGemGetState(u32 num, u32 timeflag, system_time_t  time, CellGemState* state);

s32 cellGemGetAccelerometerPositionInDevice(u32 num, vec_float4* pos);
s32 cellGemConvertVideoStart(const void* frame);

s32 cellGemFilterState(u32 num, u32 enable);
s32 cellGemSetYaw(u32 num, vec_float4 zdir);
s32 cellGemGetCameraState(CellGemCameraState* state);

s32 cellGemTrackHues(const u32* req_hues, u32* res_hues);

s32 cellGemGetImageState(u32 num, CellGemImageState* state);

s32 cellGemClearStatusFlags(u32 num, u64 mask);

s32 cellGemGetInfo(CellGemInfo* info);

s32 cellGemPrepareCamera(s32 maxexposure, f32 quality);

s32 cellGemCalibrate(u32 num);

s32 cellGemGetHuePixels(const void* frame, u32 hue, u8* pixels);
s32 cellGemPrepareVideoConvert(const CellGemVideoConvertAttribute* attr);
s32 cellGemHSVtoRGB(f32 h, f32 s, f32 v, f32* r, f32* g, f32* b);
s32 cellGemForceRGB(u32 num, f32 r, f32 g, f32 b);
s32 cellGemGetInertialState(u32 num, u32 flag, system_time_t time, CellGemInertialState* inertial);
s32 cellGemReset(u32 num);
s32 cellGemEnd();
s32 cellGemInvalidateCalibration(u32 num);
s32 cellGemIsTrackableHue(u32 hue);
s32 cellGemGetStatusFlags(u32 num, u64* flags);


#ifdef __cplusplus
	}
#endif

#endif
