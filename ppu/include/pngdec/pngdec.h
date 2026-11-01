#ifndef __LV2_PNGDEC_H__
#define __LV2_PNGDEC_H__

#include <ppu-types.h>

#define PNGDEC_ERROR_OK						0
#define PNGDEC_ERROR_HEADER					0x80611201
#define PNGDEC_ERROR_STREAM_FORMAT			0x80611202
#define PNGDEC_ERROR_ARG					0x80611203
#define PNGDEC_ERROR_SEQ					0x80611204
#define PNGDEC_ERROR_BUSY					0x80611205
#define PNGDEC_ERROR_FATAL					0x80611206
#define PNGDEC_ERROR_OPEN_FILE				0x80611207
#define PNGDEC_ERROR_SPU_UNSUPPORT			0x80611208
#define PNGDEC_ERROR_SPU_ERROR				0x80611209
#define PNGDEC_ERROR_CB_PARAM				0x8061120a

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _pngdec_stream_info CellPngDecStreamInfo;
typedef struct _pngdec_stream_param CellPngDecStreamParam;
typedef struct _pngdec_disp_info CellPngDecDispInfo;
typedef struct _pngdec_disp_param CellPngDecDispParam;

typedef void* (*CellPngCbCtrlMalloc)(u32 size,void *cbCtrlArg);
typedef void (*CellPngCbCtrlFree)(void *ptr,void *cbCtrlArg);

typedef s32 (*CellPngCbCtrlStrm)(CellPngDecStreamInfo *strmInfo,CellPngDecStreamParam *strmParam,void *cbStrmArg);

typedef s32 (*CellPngCbCtrlDisp)(CellPngDecDispInfo *dispInfo,CellPngDecDispParam *dispParam,void *cbDispArg);

typedef enum
{
	CELL_PNGDEC_SPU_THREAD_DISABLE = 0,
	CELL_PNGDEC_SPU_THREAD_ENABLE = 1
} CellPngSpuThreadEna;

typedef enum
{
	CELL_PNGDEC_FILE		= 0,
	CELL_PNGDEC_BUFFER	= 1
} CellPngStreamSel;

typedef enum
{
	CELL_PNGDEC_GRAYSCALE		= 1,
	CELL_PNGDEC_RGB				= 2,
	CELL_PNGDEC_PALETTE			= 4,
	CELL_PNGDEC_GRAYSCALE_ALPHA	= 9,
	CELL_PNGDEC_RGBA				= 10,
	CELL_PNGDEC_ARGB				= 20
} CellPngColorSpace;

typedef enum
{
	CELL_PNGDEC_NO_INTERLACE = 0,
	CELL_PNGDEC_ADAM7_INTERLACE = 1
} CellPngInterlaceMode;

typedef enum
{
	CELL_PNGDEC_STATUS_FINISH = 0,
	CELL_PNGDEC_STATUS_STOP = 1
} CellPngDecodeStatus;

typedef enum
{
	CELL_PNGDEC_CONTINUE = 0,
	CELL_PNGDEC_STOP = 1
} CellPngCommand;

typedef enum
{
	CELL_PNGDEC_TOP_TO_BOTTOM = 0,
	CELL_PNGDEC_BOTTOM_TO_TOP = 1
} CellPngOutputMode;

typedef enum
{
	CELL_PNGDEC_1BYTE_PER_NPIXEL = 0,
	CELL_PNGDEC_1BYTE_PER_1PIXEL = 1
} CellPngPackFlag;

typedef enum
{
	CELL_PNGDEC_STREAM_ALPHA = 0,
	CELL_PNGDEC_FIX_ALPHA = 1
} CellPngAlphaSelect;

typedef enum
{
	CELL_PNGDEC_LINE_MODE = 1					/*! \brief LINE mode.<br>Decode the number of lines specified with <i>output_counts</i> at one time. */
} CellPngBufferMode;

typedef enum
{
	CELL_PNGDEC_RECEIVE_EVENT = 0,				/*! \brief Decoder uses \ref spursQueuePopBegin to wait for termination of Disp callback function.<br>Transistion to WAITING state and may be affected by the behavior of other SPU threads. */
	CELL_PNGDEC_TRYRECEIVE_EVENT = 1				/*! \brief Decoder uses \ref spursQueueTryPopBegin to wait for termination of Disp callback function.<br>Since there is no state transistion, it is not affected by the behavior of other SPU threads. */
} CellPngSpuMode;

typedef struct _pngdec_thread_in_param
{
	u32 spu_enable;
	u32 ppu_prio;
	u32 spu_prio;
	CellPngCbCtrlMalloc malloc_func ATTRIBUTE_PRXPTR;
	void *malloc_arg ATTRIBUTE_PRXPTR;
	CellPngCbCtrlFree free_func ATTRIBUTE_PRXPTR;
	void *free_arg ATTRIBUTE_PRXPTR;
} CellPngDecThreadInParam;

typedef struct _pngdec_thread_out_param
{
	u32 version;
} CellPngDecThreadOutParam;

typedef struct _pngdec_src
{
	/*! \brief Input stream selection. Possible values:
	  - \ref PNGDEC_FILE
	  - \ref PNGDEC_BUFFER
	*/
	u32 stream_sel;
	const char *file_name ATTRIBUTE_PRXPTR;
	s64 file_offset;
	u32 file_size;
	void *stream_ptr ATTRIBUTE_PRXPTR;
	u32 stream_size;
	u32 spu_enable;
} CellPngDecSource;

typedef struct _pngdec_info
{
	u32 width;
	u32 height;
	u32 num_comp;
	u32 color_space;
	u32 bit_depth;
	u32 interlace_mode;
	u32 chunk_info;
} CellPngDecInfo;

typedef struct _pngdec_data_info
{
	u32 chunk_info;
	u32 num_text;
	u32 num_unk_chunk;
	u32 decode_status;
} CellPngDecDataInfo;

typedef struct _pngdec_in_param
{
	vu32 *cmd_ptr ATTRIBUTE_PRXPTR;
	u32 output_mode;
	u32 color_space;
	u32 bit_depth;
	u32 pack_flag;
	u32 alpha_select;
	u32 alpha;
} CellPngDecInParam;

typedef struct _pngdec_out_param
{
	u64 width_byte;
	u32 width;
	u32 height;
	u32 num_comp;
	u32 bit_depth;
	u32 output_mode;
	u32 color_space;
	u32 use_memory_space;
} CellPngDecOutParam;

typedef struct _pngdec_datactrl_param
{
	u64 output_bytes_per_line;
} CellPngDecDataCtrlParam;

typedef struct _pngdec_opn_info
{
	u32 init_space_allocated;
} CellPngDecOpnInfo;

typedef struct _pngdec_opn_param
{
	u32 select_chunk;
} CellPngDecOpnParam;

typedef struct _pngdec_ctrl_strm
{
	CellPngCbCtrlStrm stream_func ATTRIBUTE_PRXPTR;
	void *stream_arg ATTRIBUTE_PRXPTR;
} CellPngDecCtrlStrm;

typedef struct _pngdec_ext_info
{
	u64 reserved;
} CellPngDecExtInfo;

typedef struct _pngdec_extin_param
{
	/*! \brief Mode of output of partial images. Possible values:
	  - \ref PNGDEC_LINE_MODE
	*/
	u32 buffer_mode;
	u32 output_counts;
	u32 spu_mode;
}  CellPngDecExtInParam;

typedef struct _pngdec_extout_param
{
	u64 output_width_bytes;
	u32 output_height;
} CellPngDecExtOutParam;

typedef struct _pngdec_ctrl_disp
{
	CellPngCbCtrlDisp disp_func ATTRIBUTE_PRXPTR;
	void *disp_arg ATTRIBUTE_PRXPTR;
} CellPngDecCtrlDisp;

struct _pngdec_stream_info
{
	u32 decoded_stream_size;
};

struct _pngdec_stream_param
{
	void *strm_ptr ATTRIBUTE_PRXPTR;
	u32 strm_size;
};

struct _pngdec_disp_param
{
	void *next_output_image ATTRIBUTE_PRXPTR;
};

struct _pngdec_disp_info
{
	u64 output_frame_width_bytes;
	u32 output_frame_height;
	u64 output_start_xbyte;
	u32 output_start_y;
	u64 output_width_byte;
	u32 output_height;
	u32 output_bit_depth;
	u32 output_components;
	u32 next_output_start_y;
	u32 scan_pass_count;
	void *output_image ATTRIBUTE_PRXPTR;
};

typedef struct _png_data
{
	void *bmp_out;

	u32 pitch;
	u32 width;
	u32 height;
} CellPngData;

s32 cellPngDecCreate(s32 *handle,CellPngDecThreadInParam *in,CellPngDecThreadOutParam *out);
s32 cellPngDecOpen(s32 handle,s32 *subhandle,const CellPngDecSource *src,CellPngDecOpnInfo *open_info);
s32 cellPngDecReadHeader(s32 handle,s32 subhandle,CellPngDecInfo *info);
s32 cellPngDecSetParameter(s32 handle,s32 subhandle,const CellPngDecInParam *in,CellPngDecOutParam *out);
s32 cellPngDecDecodeData(s32 handle,s32 subhandle,u8 *data,const CellPngDecDataCtrlParam *dataCtrlParam,CellPngDecDataInfo *info);
s32 cellPngDecClose(s32 handle,s32 subhandle);
s32 cellPngDecDestroy(s32 handle);

s32 cellPngDecExtOpen(s32 handle,s32 *subhandle,const CellPngDecSource *src,CellPngDecOpnInfo *open_info,const CellPngDecCtrlStrm *cbCtrlStrm,const CellPngDecOpnParam *opnParam);
s32 cellPngDecExtReadHeader(s32 handle,s32 subhandle,CellPngDecInfo *info,CellPngDecExtInfo *extInfo);
s32 cellPngDecExtSetParameter(s32 handle,s32 subhandle,const CellPngDecInParam *inParam,CellPngDecOutParam *outParam,const CellPngDecExtInParam *extInParam,CellPngDecExtOutParam *extOutParam);
s32 cellPngDecExtDecodeData(s32 handle,s32 subhandle,u8 *data,const CellPngDecDataCtrlParam *dataCtrlParam,CellPngDecDataInfo *dataOutInfo,const CellPngDecCtrlDisp *cbCtrlDisp,CellPngDecDispParam *dispParam);

s32 cellPngLoadFromFile(const char *filename,CellPngData *out);
s32 cellPngLoadFromBuffer(const void *buffer,u32 size,CellPngData *out);

#ifdef __cplusplus
	}
#endif

#endif
