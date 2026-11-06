/*! \file resc.h
 * \brief libresc
 */

#ifndef __RESC_H__
#define __RESC_H__

#include <ppu-types.h>

#include <rsx/gcm_sys.h>
#include <sysutil/sysutil.h>


/*
 * constants
 */

/* Errors */
#define CELL_RESC_ERROR_NOT_INITIALIZED               (0x80210301)
#define CELL_RESC_ERROR_REINITIALIZED                 (0x80210302)
#define CELL_RESC_ERROR_BAD_ALIGNMENT                 (0x80210303)
#define CELL_RESC_ERROR_BAD_ARGUMENT                  (0x80210304)
#define CELL_RESC_ERROR_LESS_MEMORY                   (0x80210305)
#define CELL_RESC_ERROR_GCM_FLIP_QUE_FULL             (0x80210306)
#define CELL_RESC_ERROR_BAD_COMBINATION               (0x80210307)

/* resource policies */
#define CELL_RESC_CONSTANT_VRAM                       0
#define CELL_RESC_MINIMUM_VRAM                        1
#define CELL_RESC_CONSTANT_GPU_LOAD                   0 /* do not use */
#define CELL_RESC_MINIMUM_GPU_LOAD                    2

/* dst formats */
#define CELL_RESC_SURFACE_A8R8G8B8                    CELL_GCM_TF_COLOR_A8R8G8B8       /* 8 */
#define CELL_RESC_SURFACE_F_W16Z16Y16X16              CELL_GCM_TF_COLOR_F_W16Z16Y16X16 /* 11 */

/* buffer modes */
#define CELL_RESC_UNDEFINED                           0
#define CELL_RESC_720x480                             1
#define CELL_RESC_720x576                             2
#define CELL_RESC_1280x720                            4
#define CELL_RESC_1920x1080                           8

/* ratio convert modes */
#define CELL_RESC_FULLSCREEN                          0
#define CELL_RESC_LETTERBOX                           1
#define CELL_RESC_PANSCAN                             2

/* PAL temporal mode */
#define CELL_RESC_PAL_50                              0
#define CELL_RESC_PAL_60_DROP                         1
#define CELL_RESC_PAL_60_INTERPOLATE                  2
#define CELL_RESC_PAL_60_INTERPOLATE_30_DROP          3
#define CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE    4
#define CELL_RESC_PAL_60_FOR_HSYNC                    5

/* convolution filter mode */
#define CELL_RESC_NORMAL_BILINEAR                     0
#define CELL_RESC_INTERLACE_FILTER                    1
#define CELL_RESC_3X3_GAUSSIAN                        2
#define CELL_RESC_2X3_QUINCUNX                        3
#define CELL_RESC_2X3_QUINCUNX_ALT                    4

/* table element */
#define CELL_RESC_ELEMENT_HALF                        0
#define CELL_RESC_ELEMENT_FLOAT                       1

/* flip mode */
#define CELL_RESC_DISPLAY_VSYNC                       0
#define CELL_RESC_DISPLAY_HSYNC                       1


#ifdef __cplusplus
extern "C" {
#endif


/*
 * enumerations
 */

/*
typedef enum resc_resource_policy
{
  RESC_CONSTANT_VRAM =                           0,
  RESC_MINIMUM_VRAM =                            1,
  RESC_CONSTANT_GPU_LOAD =                       0,
  RESC_MINIMUM_GPU_LOAD =                        2,
} rescResourcePolicy;

typedef enum resc_display_buffer_mode
{
  RESC_UNDEFINED =                               0,
  RESC_720x480 =                                 1,
  RESC_720x576 =                                 2,
  RESC_1280x720 =                                4,
  RESC_1920x1080 =                               8
} rescDisplayBufferMode;

typedef enum resc_ratio_convert_mode
{
  RESC_FULLSCREEN =                              0,
  RESC_LETTERBOX =                               1,
  RESC_PANSCAN =                                 2
} rescRatioConvertMode;

typedef enum resc_pal_temporal_mode
{
  RESC_PAL_50 =                                  0,
  RESC_PAL_60_DROP =                             1,
  RESC_PAL_60_INTERPOLATE =                      2,
  RESC_PAL_60_INTERPOLATE_30_DROP =              3,
  RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE =        4,
  RESC_PAL_60_FOR_HSYNC =                        5
} rescPalTemporalMode;

typedef enum resc_convolution_filter_mode
{
  RESC_NORMAL_BILINEAR =                         0,
  RESC_INTERLACE_FILTER =                        1,
  RESC_3X3_GAUSSIAN =                            2,
  RESC_2X3_QUINCUNX =                            3,
  RESC_2X3_QUINCUNX_ALT =                        4

} rescConvolutionFilterMode;

typedef enum resc_flip_mode
{
  RESC_DISPLAY_VSYNC =                           0,
  RESC_DISPLAY_HSYNC =                           1
} rescFlipMode;
*/


/*
 * structures
 */

typedef struct _resc_init_config
{
    u32 size;
    u32 resourcePolicy;
    u32 supportModes;
    u32 ratioMode;
    u32 palTemporalMode;
    u32 interlaceMode;
    u32 flipMode;
} CellRescInitConfig;

typedef struct _resc_src
{
    u32 format;
    u32 pitch;
    u16 width;
    u16 height;
    u32 offset;
} CellRescSrc;

typedef struct _resc_dsts
{
    u32 format;
    u32 pitch;
    u32 heightAlign;
} CellRescDsts;


/*
 * functions
 */

/* equivalent functions
 *
 * cellRescSetSrc()               cellGcmSetDisplayBuffer()
 * cellRescSetConvertAndFlip()    cellGcmSetFlip()
 * cellRescSetWaitFlip()          cellGcmSetWaitFlip()
 * cellRescSetFlipHandler()       cellGcmSetFlipHandler()
 * cellRescSetVBlankHandler()     cellGcmSetVBlankHandler()
 * cellRescGetFlipStatus()        cellGcmGetFlipStatus()
 * cellRescGetLasFlipTime()       cellGcmGetLastFlipTime()
 * cellRescResetFlipStatus()      cellGcmResetFlipStatus()
 */

s32 cellRescInit(const CellRescInitConfig* const initConfig);
void cellRescExit(void);
s32 cellRescSetDsts(u32 dstsMode, CellRescDsts *dsts);
s32 cellRescSetDisplayMode(u32 bufferMode);
s32 cellRescGetNumColorBuffers(u32 dstsMode, u32 palTemporalMode, u32 reserved);
s32 cellRescGetBufferSize(int *colorBuffers, int *vertexArray, int *fragmentShader);
s32 cellRescSetBufferAddress(void *colorBuffers, void *vertexArray, void *fragmentShader);
s32 cellRescSetSrc(s32 idx, CellRescSrc *src);
s32 cellRescSetConvertAndFlip(CellGcmContextData *context, s32 idx);
void cellRescSetWaitFlip(CellGcmContextData *context);
s64 cellRescGetLastFlipTime(void);
void cellRescResetFlipStatus(void);
u32 cellRescGetFlipStatus(void);
s32 cellRescGetRegisterCount(void);
void cellRescSetRegisterCount(s32 count);
s32 cellRescSetPalInterpolateDropFlexRatio(float ratio);
s32 cellRescCreateInterlaceTable(void *ea, float srcH, s32 depth, int length);
s32 cellRescAdjustAspectRatio(float horizontal, float vertical);

/* Register event handler */
void cellRescSetVBlankHandler(void (*handler)(u32 head));
void cellRescSetFlipHandler(void (*handler)(u32 head));

/* Utility functions */
s32  cellRescGcmSurface2RescSrc(CellGcmSurface *surface, CellRescSrc *src);
s32  cellRescVideoResolution2RescBufferMode(u32 resolutionId, u32 *bufferMode);

#ifdef __cplusplus
    }
#endif

#endif

