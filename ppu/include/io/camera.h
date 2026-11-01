#ifndef __LV2_CAMERA_H__
#define __LV2_CAMERA_H__

#include <ppu-types.h>
#include <sys/memory.h>

#define CELL_CAMERA_ERROR_DOUBLE_INIT			0x80140801
#define CELL_CAMERA_ERROR_NEED_INIT				0x80140803
#define CELL_CAMERA_ERROR_BAD_PARAM				0x80140804
#define CELL_CAMERA_ERROR_DOUBLE_OPEN			0x80140805
#define CELL_CAMERA_ERROR_NEED_OPEN				0x80140806
#define CELL_CAMERA_ERROR_NO_DEVICE_FOUND		0x80140807
#define CELL_CAMERA_ERROR_DEVICE_DEACTIVATED	0x80140808
#define CELL_CAMERA_ERROR_NEED_START			0x80140809
#define CELL_CAMERA_ERROR_UNKNOWN_FORMAT		0x8014080a
#define CELL_CAMERA_ERROR_UNKNOWN_RESOLUTION	0x8014080b
#define CELL_CAMERA_ERROR_BAD_FRAMERATE			0x8014080c
#define CELL_CAMERA_ERROR_TIMEOUT				0x8014080d
#define CELL_CAMERA_ERROR_BUSY					0x8014080e
#define CELL_CAMERA_ERROR_FATAL					0x8014080f
#define CELL_CAMERA_ERROR_MUTEX					0x80140810

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _camera_attrib
{
	CELL_CAMERA_ATTRIBUTE_GAIN,
	CELL_CAMERA_ATTRIBUTE_RED_BLUE_GAIN,
	CELL_CAMERA_ATTRIBUTE_SATURATION,
	CELL_CAMERA_ATTRIBUTE_EXPOSURE,
	CELL_CAMERA_ATTRIBUTE_BRIGHTNESS,
	CELL_CAMERA_ATTRIBUTE_AUTO_EXPOSURE,
	CELL_CAMERA_ATTRIBUTE_AUTO_GAIN_CONTROL,
	CELL_CAMERA_ATTRIBUTE_AUTO_WHITE_BALANCE,
	CELL_CAMERA_ATTRIBUTE_AUTO_BRIGHTNESS,
	CELL_CAMERA_ATTRIBUTE_LED,
	CELL_CAMERA_ATTRIBUTE_AUDIO_GAIN,
	CELL_CAMERA_ATTRIBUTE_QS,
	CELL_CAMERA_ATTRIBUTE_NONZERO_COEFFS,
	CELL_CAMERA_ATTRIBUTE_YUV_FLAG,
	CELL_CAMERA_ATTRIBUTE_JPEG_FLAG,
	CELL_CAMERA_ATTRIBUTE_BACKLIGHT_COMPENSATION,
	CELL_CAMERA_ATTRIBUTE_MIRROR_FLAG,
	CELL_CAMERA_ATTRIBUTE_MEASURED_QS,
	CELL_CAMERA_ATTRIBUTE_422_FLAG,
	CELL_CAMERA_ATTRIBUTE_USBLOAD,
	CELL_CAMERA_ATTRIBUTE_GAMMA,
	CELL_CAMERA_ATTRIBUTE_GREEN_GAIN,
	CELL_CAMERA_ATTRIBUTE_AUTO_GAIN_CONTROL_LIMIT,
	CELL_CAMERA_ATTRIBUTE_DENOISE,
	CELL_CAMERA_ATTRIBUTE_FRAMERATE_ADJUST,
	CELL_CAMERA_ATTRIBUTE_PIXEL_OUTLIER_FILTER,
	CELL_CAMERA_ATTRIBUTE_AUTO_GAIN_CONTROL_LOW,
	CELL_CAMERA_ATTRIBUTE_AUTO_GAIN_CONTROL_HIGH,

	CELL_CAMERA_ATTRIBUTE_FORMAT_CAP=100,
	CELL_CAMERA_ATTRIBUTE_FORMAT_INDEX,
	CELL_CAMERA_ATTRIBUTE_FRAME_NUM,
	CELL_CAMERA_ATTRIBUTE_FRAME_INDEX,
	CELL_CAMERA_ATTRIBUTE_FRAME_SIZE,
	CELL_CAMERA_ATTRIBUTE_INTERVAL_TYPE,
	CELL_CAMERA_ATTRIBUTE_INTERVAL_INDEX,
	CELL_CAMERA_ATTRIBUTE_INTERVAL_VALUE,
	CELL_CAMERA_ATTRIBUTE_COLOR_MATCHING,
	CELL_CAMERA_ATTRIBUTE_PL_FREQ, 
	CELL_CAMERA_ATTRIBUTE_DEV_ID,
	CELL_CAMERA_ATTRIBUTE_DEV_CAP,
	CELL_CAMERA_ATTRIBUTE_DEV_SPEED,
	CELL_CAMERA_ATTRIBUTE_USBVIDCLASS_REQUEST_CODE, 
	CELL_CAMERA_ATTRIBUTE_USBVIDCLASS_REQUEST_DATA, 
	CELL_CAMERA_ATTRIBUTE_DEVICE_ID,	

	CELL_CAMERA_ATTRIBUTE_READ_MODE=300,
	CELL_CAMERA_ATTRIBUTE_GAME_PID,		
	CELL_CAMERA_ATTRIBUTE_PBUFFER,		
	CELL_CAMERA_ATTRIBUTE_READ_FINISH,
	
	CELL_CAMERA_ATTRIBUTE_UNKNOWN=500,
} CellCameraAttribute;

typedef enum _camera_type
{
	CELL_CAMERA_TYPE_UNKNOWN,
	CELL_CAMERA_TYPE_EYETOY,
	CELL_CAMERA_TYPE_PLAYSTATION_EYE,
	CELL_CAMERA_TYPE_USBVIDEO_CLASS,
} CellCameraType;

typedef enum _camera_format
{
	CELL_CAMERA_FORMAT_UNKNOWN,
	CELL_CAMERA_FORMAT_JPG,
	CELL_CAMERA_FORMAT_RAW8,
	CELL_CAMERA_FORMAT_YUV422,
	CELL_CAMERA_FORMAT_RAW10,
	CELL_CAMERA_FORMAT_RGBA,
	CELL_CAMERA_FORMAT_YUV420
} CellCameraFormat;

typedef enum _camera_resolution
{
	CELL_CAMERA_RESOLUTION_UNKNOWN,
	CELL_CAMERA_RESOLUTION_VGA,
	CELL_CAMERA_RESOLUTION_QVGA,
	CELL_CAMERA_RESOLUTION_WGA,
	CELL_CAMERA_RESOLUTION_SPECIFIED
} CellCameraResolution;

typedef struct _camera_info
{
	CellCameraFormat format;
	CellCameraResolution resolution;
	s32 framerate;
	u32 buffer;
	s32 bytesize;
	s32 width;
	s32 height;
	s32 dev_num;
	s32 guid;
	s32 info_ver;
	sys_mem_container_t container;
} CellCameraInfo;

typedef struct _camera_info_ex
{
	CellCameraFormat format;
	CellCameraResolution resolution;
	s32 framerate;
	u32 buffer;
	s32 bytesize;
	s32 width;
	s32 height;
	s32 dev_num;
	s32 guid;

	s32 info_ver;
	sys_mem_container_t container;

	s32 readmode;
	u32 pbuf[2];
} CellCameraInfoEx;

typedef struct _camera_read_info
{
	s32 version;
	u32 frame;
	u32 readcount;
	s64 timestamp;
	u32 buffer;
} CellCameraReadInfo;

s32 cellCameraInit();
s32 cellCameraEnd();
s32 cellCameraStart(s32 num);
s32 cellCameraStop(s32 num);
s32 cellCameraOpenEx(s32 num,CellCameraInfoEx *info);
s32 cellCameraClose(s32 num);

s32 cellCameraRead(s32 num,u32 *frame,u32 *readcount);
s32 cellCameraReadEx(s32 num,CellCameraReadInfo *info);
s32 cellCameraGetBufferInfoEx(s32 num,CellCameraInfoEx *info);
s32 cellCameraReset(s32 num);
s32 cellCameraGetAttribute(s32 num,CellCameraAttribute attribute,u32* arg0,u32* arg1);
s32 cellCameraSetAttribute(s32 num,CellCameraAttribute attribute,u32 arg0,u32 arg1);
s32 cellCameraGetType(s32 num,CellCameraType* type);
s32 cellCameraGetDeviceGUID(s32 num,u32* guid);
s32 cellCameraGetBufferSize(s32 num,CellCameraInfoEx* info);

s32 cellCameraIsStarted(s32 num);
s32 cellCameraIsOpen(s32 num);
s32 cellCameraIsAvailable(s32 num);
s32 cellCameraIsAttached(s32 num);

s32 cellCameraSetNotifyEventQueue(u64 key);
s32 cellCameraSetNotifyEventQueue2(u64 key,u64 source,u64 flag);
s32 cellCameraRemoveNotifyEventQueue(u64 key);
s32 cellCameraRemoveNotifyEventQueue2(u64 key);
s32 cellCameraReadComplete(s32 num,u32 buf,u32 arg);
s32 cellCameraPrepExtensionUnit(s32 num,u8* guidExtCode);
s32 cellCameraCtrlExtensionUnit(s32 num,u8 req,u16 wval,u16 wlen,u8* data);
s32 cellCameraGetExtensionUnit(s32 num,u16 wval,u16 wlen,u8* data);
s32 cellCameraSetExtensionUnit(s32 num,u16 wval,u16 wlen,u8* data);

#ifdef __cplusplus
	}
#endif

#endif