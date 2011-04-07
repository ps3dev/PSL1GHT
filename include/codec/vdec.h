#pragma once

#include <stdint.h>
#include <stdbool.h>


#define VDEC_ERROR_ARG   0x80610101
#define VDEC_ERROR_SEQ   0x80610102
#define VDEC_ERROR_BUSY  0x80610103
#define VDEC_ERROR_EMPTY 0x80610104
#define VDEC_ERROR_AU    0x80610105
#define VDEC_ERROR_PIC   0x80610106

#define VDEC_TS_INVALID 0xffffffff



#define	VDEC_CODEC_TYPE_MPEG2 0
#define	VDEC_CODEC_TYPE_H264  1

#define VDEC_CALLBACK_AUDONE  0
#define VDEC_CALLBACK_PICOUT  1
#define VDEC_CALLBACK_SEQDONE 2
#define VDEC_CALLBACK_ERROR   3

#define VDEC_DECODER_MODE_NORMAL       0
#define VDEC_DECODER_MODE_SKIP_NON_REF 1

#define VDEC_PICFMT_ARGB32  0
#define VDEC_PICFMT_RGBA32  1
#define VDEC_PICFMT_UYVY422 2
#define VDEC_PICFMT_YUV420P 3

#define VDEC_COLOR_MATRIX_BT601 0
#define VDEC_COLOR_MATRIX_BT709 1

#define VDEC_PICTURE_NORMAL  0
#define VDEC_PICTURE_SKIPPED 1


typedef struct vdec_type {
	uint32_t codec_type;
	uint32_t profile_level;
} vdec_type;


typedef struct vdec_attr {
	uint32_t mem_size;
	uint8_t cmd_depth;
	uint32_t ver_major;
	uint32_t ver_minor;
} vdec_attr;

typedef struct vdec_config {
	uint32_t mem_addr;
	uint32_t mem_size;
	uint32_t ppu_thread_prio;
	uint32_t ppu_thread_stack_size;
	uint32_t spu_thread_prio;
	uint32_t num_spus;
} vdec_config;

typedef struct vdec_ts {
	uint32_t low,hi;
} vdec_ts;

typedef struct vdec_au {
	uint32_t packet_addr;
	uint32_t packet_size;
	vdec_ts pts;
	vdec_ts dts;
	uint64_t userdata;
	uint64_t reserved;
} vdec_au;

typedef struct vdec_picture {
	uint32_t codec_type;
	uint32_t picture_addr;
	uint32_t picture_size;
	uint8_t access_units;
	vdec_ts pts[2];
	vdec_ts dts[2];
	uint64_t userdata[2];
	uint32_t status;
	uint32_t attr;
	uint32_t codec_specific_addr;
} vdec_picture;


typedef struct vdec_picture_format {
	uint32_t format_type;
	uint32_t color_matrix;
	uint8_t alpha;
} vdec_picture_format;


typedef uint32_t (*vdec_callback)(uint32_t handle, uint32_t msgtype,
				  uint32_t msgdata, uint32_t arg);


typedef struct vdec_closure {
  uint32_t fn;
  uint32_t arg;
} vdec_closure;


int32_t vdec_query_attr(const vdec_type *type, vdec_attr *attr);
int32_t vdec_open(const vdec_type *type, const vdec_config *config,
		  const vdec_closure *c, uint32_t *handleptr);
int32_t vdec_close(uint32_t handle);
int32_t vdec_start_sequence(uint32_t handle);
int32_t vdec_end_sequence(uint32_t handle);
int32_t vdec_decode_au(uint32_t handle, int mode, const vdec_au *auInfo);
int32_t vdec_get_picture(uint32_t handle, const vdec_picture_format *format,
			 void *buffer);
int32_t vdec_get_pic_item(uint32_t handle, uint32_t *pic_item_addr_p);





#define VDEC_MPEG2_MP_LL       0
#define VDEC_MPEG2_MP_ML       1
#define VDEC_MPEG2_MP_H14      2
#define VDEC_MPEG2_MP_HL       3


typedef struct vdec_mpeg2_info {
	uint16_t width;
	uint16_t height;
	uint8_t  aspect_ratio;
	uint8_t  frame_rate;
	bool     progressive_sequence;
	bool     low_delay;
	uint8_t  video_format;
	bool     color_description;
	uint8_t  color_primaries;
	uint8_t  transfer_characteristics;
	uint8_t  matrix_coefficients;
	uint16_t temporal_reference[2];
	uint8_t  picture_coding_type[2];
	uint8_t  picture_structure[2];
	bool     top_field_first;
	bool     repeat_first_field;
	bool     progressive_frame;
	uint32_t time_code;
	bool     closed_gop;
	bool     broken_link;
	uint16_t vbv_delay[2];
	uint16_t display_horizontal_size;
	uint16_t display_vertical_size;
	uint8_t  number_of_frame_centre_offsets[2];
	uint16_t frame_centre_horizontal_offset[2][3];
	uint16_t frame_centre_vertical_offset[2][3];
	uint32_t headerPresentFlags;
	uint32_t headerRetentionFlags;
	bool     mpeg1Flag;
} vdec_mpeg2_info;

#define VDEC_MPEG2_ARI_SAR_1_1     1
#define VDEC_MPEG2_ARI_DAR_4_3     2
#define VDEC_MPEG2_ARI_DAR_16_9    3
#define VDEC_MPEG2_ARI_DAR_2P21_1  4






typedef struct vdec_h264_info {
	uint16_t width;
	uint16_t height;
	uint8_t  picture_type[2];
	bool     idr_picture_flag;
	uint8_t  aspect_ratio_idc;
	uint16_t sar_height;
	uint16_t sar_width;
	uint8_t  pic_struct;
	int16_t  pic_order_count[2];
	bool     vui_parameters_present_flag;
	bool     frame_mbs_only_flag;
	bool     video_signal_type_present_flag;
	uint8_t  video_format;
	bool     video_full_range_flag;
	bool     color_description_present_flag;
	uint8_t  color_primaries;
	uint8_t  transfer_characteristics;
	uint8_t  matrix_coefficients;
	bool     timing_info_present_flag;
	uint8_t  frame_rate;
	bool     fixed_frame_rate_flag;
	bool     low_delay_hrd_flag;
	bool     entropy_coding_mode_flag;
	uint16_t nalUnitPresentFlags;
} vdec_h264_info;
