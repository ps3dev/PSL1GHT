#ifndef __LV2_JPGDEC_H__
#define __LV2_JPGDEC_H__

#include <ppu-types.h>

#define CELL_JPGDEC_ERROR_OK					0
#define CELL_JPGDEC_ERROR_HEADER				0x80611101
#define CELL_JPGDEC_ERROR_STREAM_FORMAT			0x80611102
#define CELL_JPGDEC_ERROR_ARG					0x80611103
#define CELL_JPGDEC_ERROR_SEQ					0x80611104
#define CELL_JPGDEC_ERROR_BUSY					0x80611105
#define CELL_JPGDEC_ERROR_FATAL					0x80611106
#define CELL_JPGDEC_ERROR_OPEN_FILE				0x80611107
#define CELL_JPGDEC_ERROR_SPU_UNSUPPORT			0x80611108
#define CELL_JPGDEC_ERROR_CB_PARAM				0x80611109

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _jpgdec_strm_info CellJpgDecStrmInfo;
typedef struct _jpgdec_strm_param CellJpgDecStrmParam;
typedef struct _jpgdec_disp_info CellJpgDecDispInfo;
typedef struct _jpgdec_disp_param CellJpgDecDispParam;

typedef void* (*CellJpgCbCtrlMalloc)(u32 size,void *cbCtrlArg);
typedef void (*CellJpgCbCtrlFree)(void *ptr,void *cbCtrlArg);

typedef s32 (*CellJpgCbCtrlStrm)(CellJpgDecStrmInfo *strmInfo,CellJpgDecStrmParam *strmParam,void *cbCtrlArg);
typedef s32 (*CellJpgCbCtrlDisp)(CellJpgDecDispInfo *dispInfo,CellJpgDecDispParam *dispParam,void *cbDispArg);

typedef enum
{
	CELL_JPGDEC_FILE		= 0,
	CELL_JPGDEC_BUFFER	    = 1
} CellJpgStreamSel;

typedef enum
{
	CELL_JPGDEC_SPU_THREAD_DISABLE = 0,
	CELL_JPGDEC_SPU_THREAD_ENABLE  = 1
} CellJpgSpuThreadEna;

typedef enum
{
	CELL_JPGDEC_GRAYSCALE		    = 1,
	CELL_JPGDEC_RGB				    = 2,
	CELL_JPGDEC_YCBCR			    = 3,
	CELL_JPGDEC_RGBA				= 10,
	CELL_JPGDEC_UPSTREAM			= 11,
	CELL_JPGDEC_ARGB				= 20,
} CellJpgColorSpace;

typedef enum
{
	CELL_JPGDEC_STATUS_FINISH = 0,
	CELL_JPGDEC_STATUS_STOP   = 1
} CellJpgDecodeStatus;

typedef enum
{
	CELL_JPGDEC_CONTINUE = 0,
	CELL_JPGDEC_STOP 	 = 1
} CellJpgCommand;

typedef enum
{
	CELL_JPGDEC_QUALITY = 0,
	CELL_JPGDEC_FAST	= 5
} CellJpgMethod;

typedef enum
{
	CELL_JPGDEC_TOP_TO_BOTTOM = 0,
	CELL_JPGDEC_BOTTOM_TO_TOP = 1
} CellJpgOutputMode;

typedef enum
{
	CELL_JPGDEC_MCU_MODE  = 0,
	CELL_JPGDEC_LINE_MODE = 1
} CellJpgBufferMode;

typedef enum
{
	CELL_JPGDEC_RECEIVE_EVENT    = 0,
	CELL_JPGDEC_TRYRECEIVE_EVENT = 1
} CellJpgSpuMode;

typedef struct _jpgdec_thread_in_param
{
	u32 spu_enable;
	u32 ppu_prio;
	u32 spu_prio;
	CellJpgCbCtrlMalloc malloc_func ATTRIBUTE_PRXPTR;
	void *malloc_arg ATTRIBUTE_PRXPTR;
	CellJpgCbCtrlFree free_func ATTRIBUTE_PRXPTR;
	void *free_arg ATTRIBUTE_PRXPTR;
} CellJpgDecThreadInParam;

typedef struct _jpgdec_thread_out_param
{
	u32 version;
} CellJpgDecThreadOutParam;

typedef struct _jpgdec_src
{
	u32 stream_sel;
	const char *file_name ATTRIBUTE_PRXPTR;
	s64 file_offset;
	u32 file_size;
	void *stream_ptr ATTRIBUTE_PRXPTR;
	u32 stream_size;
	u32 spu_enable;
} CellJpgDecSource;

typedef struct _jpgdec_info
{
	u32 width;
	u32 height;
	u32 num_comp;
	u32 color_space;
} CellJpgDecInfo;

typedef struct _jpgdec_data_info
{
	f32 value;
	u32 output_lines;
	u32 decode_status;
} CellJpgDecDataInfo;

typedef struct _jpgdec_opn_info
{
	u32 init_space_allocated;
} CellJpgDecOpnInfo;

typedef struct _jpgdec_in_param
{
	vu32 *cmd_ptr ATTRIBUTE_PRXPTR;
	u32 down_scale;
	u32 quality_mode;
	u32 output_mode;
	u32 color_space;
	u8 alpha;
	u8 pad[3];
} CellJpgDecInParam;

typedef struct _jpgdec_out_param
{
	u64 width_bytes;
	u32 width;
	u32 height;
	u32 num_comp;
	u32 output_mode;
	u32 color_space;
	u32 down_scale;
	u32 use_memory_space;
} CellJpgDecOutParam;

typedef struct _jpgdec_datactrl_param
{
	u64 output_bytes_per_line;
} CellJpgDecDataCtrlParam;

typedef struct _jpgdec_ctrl_strm
{
	CellJpgCbCtrlStrm strm_func ATTRIBUTE_PRXPTR;
	void *strm_arg ATTRIBUTE_PRXPTR;
} CellJpgDecCtrlStrm;

typedef struct _jpgdec_ext_info
{
	u64 coeff_buffer_size;
	u32 mcu_width;
} CellJpgDecExtInfo;

typedef struct _jpgdec_extin_param
{
	void *coeff_buffer ATTRIBUTE_PRXPTR;
	u32 buffer_mode;
	u32 output_counts;
	u32 spu_mode;
} CellJpgDecExtInParam;

typedef struct _jpgdec_extout_param
{
	u64 output_width_byte;
	u32 output_height;
	u32 one_mcu_width;
	u32 one_mcu_height;
} CellJpgDecExtOutParam;

typedef struct _jpgdec_ctrl_disp
{
	CellJpgCbCtrlDisp disp_func ATTRIBUTE_PRXPTR;
	void *disp_arg ATTRIBUTE_PRXPTR;
} CellJpgDecCtrlDisp;

struct _jpgdec_strm_info
{
	u32 decoded_strm_size;
};

struct _jpgdec_strm_param
{
	void *strm_ptr ATTRIBUTE_PRXPTR;
	u32 strm_size;
};

struct _jpgdec_disp_info
{
	u64 output_frame_width_byte;
	u32 output_frame_height;
	u64 output_start_xbyte;
	u32 output_start_y;
	u64 output_width_byte;
	u32 output_height;
	u32 output_components;
	void *output_image ATTRIBUTE_PRXPTR;
};

struct _jpgdec_disp_param
{
	void *next_output_image ATTRIBUTE_PRXPTR;
};

typedef struct _jpg_data
{
	void *bmp_out;

	u32 pitch;
	u32 width;
	u32 height;
} CellJpgData;

s32 cellJpgDecCreate(s32 *handle,CellJpgDecThreadInParam *in,CellJpgDecThreadOutParam *out);
s32 cellJpgDecOpen(s32 handle,s32 *subhandle,const CellJpgDecSource *src,CellJpgDecOpnInfo *openInfo);
s32 cellJpgDecReadHeader(s32 handle,s32 subhandle,CellJpgDecInfo *info);
s32 cellJpgDecSetParameter(s32 handle,s32 subhandle,const CellJpgDecInParam *in,CellJpgDecOutParam *out);
s32 cellJpgDecDecodeData(s32 handle,s32 subhandle,u8 *data,const CellJpgDecDataCtrlParam *dataCtrlParam,CellJpgDecDataInfo *info);
s32 cellJpgDecClose(s32 handle,s32 subhandle);
s32 cellJpgDecDestroy(s32 handle);

s32 cellJpgDecExtOpen(s32 handle,s32 *subhandle,const CellJpgDecSource *src,CellJpgDecOpnInfo *openInfo,const CellJpgDecCtrlStrm *cbCtrlStrm);
s32 cellJpgDecExtReadHeader(s32 handle,s32 subhandle,CellJpgDecInfo *info,CellJpgDecExtInfo *extInfo);
s32 cellJpgDecExtSetParameter(s32 handle,s32 subhandle,const CellJpgDecInParam *inParam,CellJpgDecOutParam *outParam,const CellJpgDecExtInParam *extInParam,CellJpgDecExtOutParam *extOutParam);
s32 cellJpgDecExtDecodeData(s32 handle,s32 subhandle,u8 *data,const CellJpgDecDataCtrlParam *dataCtrlParam,CellJpgDecDataInfo *info,const CellJpgDecCtrlDisp *cbCtrlDisp,CellJpgDecDispParam *dispParam);

s32 cellJpgLoadFromFile(const char *filename,CellJpgData *out);
s32 cellJpgLoadFromBuffer(const void *buffer,u32 size,CellJpgData *out);

#ifdef __cplusplus
	}
#endif

#endif
