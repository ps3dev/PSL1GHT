#ifndef __LV2_VDEC_H__
#define __LV2_VDEC_H__

#include <ppu-types.h>

#define VDEC_ERROR_ARG					0x80610101
#define VDEC_ERROR_SEQ					0x80610102
#define VDEC_ERROR_BUSY					0x80610103
#define VDEC_ERROR_EMPTY				0x80610104
#define VDEC_ERROR_AU					0x80610105
#define VDEC_ERROR_PIC					0x80610106

#define VDEC_TS_INVALID					0xffffffff

#define	VDEC_CODEC_TYPE_MPEG2			0
#define	VDEC_CODEC_TYPE_H264			1

#define VDEC_CALLBACK_AUDONE			0
#define VDEC_CALLBACK_PICOUT			1
#define VDEC_CALLBACK_SEQDONE			2
#define VDEC_CALLBACK_ERROR				3

#define VDEC_DECODER_MODE_NORMAL		0
#define VDEC_DECODER_MODE_SKIP_NON_REF	1

#define VDEC_PICFMT_ARGB32				0
#define VDEC_PICFMT_RGBA32				1
#define VDEC_PICFMT_UYVY422				2
#define VDEC_PICFMT_YUV420P				3

#define VDEC_COLOR_MATRIX_BT601			0
#define VDEC_COLOR_MATRIX_BT709			1

#define VDEC_PICTURE_NORMAL				0
#define VDEC_PICTURE_SKIPPED			1

#define VDEC_MPEG2_MP_LL				0
#define VDEC_MPEG2_MP_ML				1
#define VDEC_MPEG2_MP_H14				2
#define VDEC_MPEG2_MP_HL				3

#define VDEC_MPEG2_ARI_SAR_1_1			1
#define VDEC_MPEG2_ARI_DAR_4_3			2
#define VDEC_MPEG2_ARI_DAR_16_9			3
#define VDEC_MPEG2_ARI_DAR_2P21_1		4

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _vdec_type
{
	u32 codec_type;
	u32 profile_level;
} vdecType;


typedef struct _vdec_attr
{
	u32 mem_size;
	u8 cmd_depth;
	u32 ver_major;
	u32 ver_minor;
} vdecAttr;

typedef struct _vdec_config
{
	u32 mem_addr;
	u32 mem_size;
	u32 ppu_thread_prio;
	u32 ppu_thread_stack_size;
	u32 spu_thread_prio;
	u32 num_spus;
} vdecConfig;

typedef struct _vdec_closure
{
  u32 fn;
  u32 arg;
} vdecClosure;

typedef struct _vdec_ts 
{
	u32 low,hi;
} vdecTS;

typedef struct _vdec_au
{
	u32 packet_addr;
	u32 packet_size;
	vdecTS pts;
	vdecTS dts;
	u64 userdata;
	u64 reserved;
} vdecAU;

typedef struct _vdec_picture
{
	u32 codec_type;
	u32 picture_addr;
	u32 picture_size;
	u8 access_units;
	vdecTS pts[2];
	vdecTS dts[2];
	u64 userdata[2];
	u32 status;
	u32 attr;
	u32 codec_specific_addr;
} vdecPicture;


typedef struct _vdec_picture_format 
{
	u32 format_type;
	u32 color_matrix;
	u8 alpha;
} vdecPictureFormat;

typedef struct _vdec_mpeg2_info
{
	u16 width;
	u16 height;
	u8  aspect_ratio;
	u8  frame_rate;
	bool progressive_sequence;
	bool low_delay;
	u8  video_format;
	bool color_description;
	u8  color_primaries;
	u8  transfer_characteristics;
	u8  matrix_coefficients;
	u16 temporal_reference[2];
	u8  picture_coding_type[2];
	u8  picture_structure[2];
	bool top_field_first;
	bool repeat_first_field;
	bool progressive_frame;
	u32 time_code;
	bool closed_gop;
	bool broken_link;
	u16 vbv_delay[2];
	u16 display_horizontal_size;
	u16 display_vertical_size;
	u8  number_of_frame_centre_offsets[2];
	u16 frame_centre_horizontal_offset[2][3];
	u16 frame_centre_vertical_offset[2][3];
	u32 headerPresentFlags;
	u32 headerRetentionFlags;
	bool mpeg1Flag;
} vdecMPEG2Info;

typedef struct _vdec_h264_info 
{
	u16 width;
	u16 height;
	u8  picture_type[2];
	bool idr_picture_flag;
	u8  aspect_ratio_idc;
	u16 sar_height;
	u16 sar_width;
	u8  pic_struct;
	u8  _pad0;
	s16 pic_order_count[2];
	bool vui_parameters_present_flag;
	bool frame_mbs_only_flag;
	bool video_signal_type_present_flag;
	u8  video_format;
	bool video_full_range_flag;
	bool color_description_present_flag;
	u8  color_primaries;
	u8  transfer_characteristics;
	u8  matrix_coefficients;
	bool timing_info_present_flag;
	u8  frame_rate;
	bool fixed_frame_rate_flag;
	bool low_delay_hrd_flag;
	bool entropy_coding_mode_flag;
	u16 nalUnitPresentFlags;
	u8  ccDataLength[2];
	u8  ccData[2][128];
	u64 reserved[2];
} vdecH264Info;

typedef u32 (*vdecCallback)(u32 handle,u32 msgtype,u32 msgdata,u32 arg);

s32 vdecQueryAttr(const vdecType *type,vdecAttr *attr);
s32 vdecOpen(const vdecType *type,const vdecConfig *config,const vdecClosure *c,u32 *handleptr);
s32 vdecClose(u32 handle);
s32 vdecStartSequence(u32 handle);
s32 vdecEndSequence(u32 handle);
s32 vdecDecodeAu(u32 handle,s32 mode,const vdecAU *auInfo);
s32 vdecGetPicture(u32 handle,const vdecPictureFormat *format,void *buffer);
s32 vdecGetPicItem(u32 handle,u32 *pic_item_addr_p);


#ifdef __cplusplus
	}
#endif

#endif
