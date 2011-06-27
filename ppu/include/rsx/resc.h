/*! \file resc.h
 * \brief libresc
 * */

#ifndef __RESC_H__
#define __RESC_H__

#include <ppu-types.h>

#include <rsx/gcm_sys.h>
#include <sysutil/sysutil.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Error macros */
#define RESC_ERROR_BASE                          (0x80210300)

#define RESC_ERROR_NOT_INITIALIZED               (RESC_ERROR_BASE | 0x1)
#define RESC_ERROR_REINITIALIZED                 (RESC_ERROR_BASE | 0x2)
#define RESC_ERROR_BAD_ALIGNMENT                 (RESC_ERROR_BASE | 0x3)
#define RESC_ERROR_BAD_ARGUMENT                  (RESC_ERROR_BASE | 0x4)
#define RESC_ERROR_LESS_MEMORY                   (RESC_ERROR_BASE | 0x5)
#define RESC_ERROR_GCM_FLIP_QUE_FULL             (RESC_ERROR_BASE | 0x6)
#define RESC_ERROR_BAD_COMBINATION               (RESC_ERROR_BASE | 0x7)

/* Enum */
typedef enum _rescResourcePolicy
{
    RESC_CONSTANT_VRAM =                         (0 << 0), 
    RESC_MINIMUM_VRAM =                          (1 << 0), 
    RESC_CONSTANT_GPU_LOAD =                     (0 << 1),  /* do not use */
    RESC_MINIMUM_GPU_LOAD =                      (1 << 1), 
} rescResourcePolicy;

typedef enum _rescDstFormat
{
    RESC_SURFACE_A8R8G8B8 =                      GCM_SURFACE_A8R8G8B8,       /* 8 */
    RESC_SURFACE_F_W16Z16Y16X16 =                GCM_SURFACE_F_W16Z16Y16X16, /* 11 */
}rescDstFormat;

typedef enum _rescDisplayBufferMode
{
    RESC_720x480 =                               (1 << 0), 
    RESC_720x576 =                               (1 << 1), 
    RESC_1280x720 =                              (1 << 2), 
    RESC_1920x1080 =                             (1 << 3), 
} rescBufferMode;

typedef enum _rescRatioConvertMode
{
    RESC_FULLSCREEN =                            0, 
    RESC_LETTERBOX =                             1, 
    RESC_PANSCAN =                               2, 
} rescRatioConvertMode;

typedef enum _rescPalTemporalMode
{
    RESC_PAL_50 =                                0, 
    RESC_PAL_60_DROP =                           1, 
    RESC_PAL_60_INTERPOLATE =                    2, 
    RESC_PAL_60_INTERPOLATE_30_DROP =            3, 
    RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE =      4, 
    RESC_PAL_60_FOR_HSYNC =                      5, 
} rescPalTemporalMode;

typedef enum _rescConvolutionFilterMode
{
    RESC_NORMAL_BILINEAR =                       0, 
    RESC_INTERLACE_FILTER =                      1, 
    RESC_3X3_GAUSSIAN =                          2, 
    RESC_2X3_QUINCUNX =                          3, 
    RESC_2X3_QUINCUNX_ALT =                      4, 
} rescConvolutionFilterMode;

/* typedef rescConvolutionFilterMode rescInterlaceFilterMode; */  /* for backword compatibility */

typedef enum _rescTableElement
{
    RESC_ELEMENT_HALF =                          0, 
    RESC_ELEMENT_FLOAT =                         1, 
} rescTableElement;

typedef enum _rescFlipMode
{
    RESC_DISPLAY_VSYNC =                         0, 
    RESC_DISPLAY_HSYNC =                         1, 
} rescFlipMode;

/* Structures */
typedef struct _rescInitConfig
{
    size_t size;
    u32 resourcePolicy;
    u32 supportModes;
    u32 ratioMode;
    u32 palTemporalMode;
    u32 interlaceMode;
    u32 flipMode;
}rescInitConfig;

typedef struct _rescSrc
{
    u32 format;
    u32 pitch;
    u16 width;
    u16 height;
    u32 offset;
} rescSrc;

typedef struct _rescDsts
{
    u32 format;
    u32 pitch;
    u32 heightAlign;
} rescDsts;

s32 rescInit(const rescInitConfig* const initConfig);
void rescExit();
s32 rescSetDsts(const rescBufferMode dstsMode, const rescDsts* const dsts);
s32 rescSetDisplayMode(const rescBufferMode bufferMode);
s32 rescGetNumColorBuffers(const rescBufferMode dstsMode, const rescPalTemporalMode palTemporalMode, const u32 reserved);
s32 rescGetBufferSize(s32* const colorBuffers, s32* const vertexArray, s32* const fragmentShader);
s32 rescSetBufferAddress(const void* const colorBuffers, const void* const vertexArray, const void* const fragmentShader);
s32 rescSetSrc(const s32 idx, const rescSrc* const src);
s32 rescSetConvertAndFlip(gcmContextData* con, const s32 idx);
void rescSetWaitFlip(gcmContextData* con);
s64 rescGetLastFlipTime();
void rescResetFlipStatus();
u32 rescGetFlipStatus();
s32 rescGetRegisterCount();
void rescSetRegisterCount(const s32 regCount);
s32 rescSetPalInterpolateDropFlexRatio(const float ratio);
s32 rescCreateInterlaceTable(void* ea, const float srcH, const rescTableElement depth, const int length);
s32 rescAdjustAspectRatio(const float horizontal, const float vertical);

/* Register event handler */
void rescSetVBlankHandler(void (*handler)(const u32 head));
void rescSetFlipHandler(void (*handler)(const u32 head));


/* Utility functions */
s32  rescGcmSurface2RescSrc(const gcmSurface* const gcmSurface, rescSrc* const rescSrc);
s32  rescVideoOutResolutionId2RescBufferMode(const videoResolutionId resolutionId, rescBufferMode* const bufferMode);

#ifdef __cplusplus
    }
#endif

#endif

