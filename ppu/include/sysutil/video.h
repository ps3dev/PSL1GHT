/*! \file video.h
    \brief Video mode management.
*/

#ifndef __LV2_CELL_VIDEO_OUT_H__
#define __LV2_CELL_VIDEO_OUT_H__

#include <ppu-types.h>

#define CELL_VIDEO_OUT_STATE_DISABLED                          0
#define CELL_VIDEO_OUT_STATE_ENABLED                           1
#define CELL_VIDEO_OUT_STATE_BUSY                              3

#define CELL_VIDEO_OUT_PRIMARY                                 0
#define CELL_VIDEO_OUT_SECONDARY                               1

#define CELL_VIDEO_OUT_SCANMODE_INTERLACE                      0
#define CELL_VIDEO_OUT_SCANMODE_PROGRESSIVE                    1

#define CELL_VIDEO_OUT_SCANMODE2_AUTO                          0
#define CELL_VIDEO_OUT_SCANMODE2_INTERLACE                     1
#define CELL_VIDEO_OUT_SCANMODE2_PROGRESSIVE                   2

#define CELL_VIDEO_OUT_BUFFER_FORMAT_XRGB                      0
#define CELL_VIDEO_OUT_BUFFER_FORMAT_XBGR                      1
#define CELL_VIDEO_OUT_BUFFER_FORMAT_FLOAT                     2

#define CELL_VIDEO_OUT_ASPECT_AUTO                             0
#define CELL_VIDEO_OUT_ASPECT_4_3                              1
#define CELL_VIDEO_OUT_ASPECT_16_9                             2

#define CELL_VIDEO_OUT_RESOLUTION_UNDEFINED                    0
#define CELL_VIDEO_OUT_RESOLUTION_1080                         1
#define CELL_VIDEO_OUT_RESOLUTION_720                          2
#define CELL_VIDEO_OUT_RESOLUTION_480                          4
#define CELL_VIDEO_OUT_RESOLUTION_576                          5
#define CELL_VIDEO_OUT_RESOLUTION_1600x1080                    10
#define CELL_VIDEO_OUT_RESOLUTION_1440x1080                    11
#define CELL_VIDEO_OUT_RESOLUTION_1280x1080                    12
#define CELL_VIDEO_OUT_RESOLUTION_960x1080                     13

#define CELL_VIDEO_OUT_COLOR_RGB                               0x01
#define CELL_VIDEO_OUT_COLOR_YUV                               0x02
#define CELL_VIDEO_OUT_COLOR_XVYCC                             0x04

#define CELL_VIDEO_OUT_RESOLUTION_720_3D_FRAME_PACKING         0x81
#define CELL_VIDEO_OUT_RESOLUTION_1024x720_3D_FRAME_PACKING    0x88
#define CELL_VIDEO_OUT_RESOLUTION_960x720_3D_FRAME_PACKING     0x89
#define CELL_VIDEO_OUT_RESOLUTION_800x720_3D_FRAME_PACKING     0x8a
#define CELL_VIDEO_OUT_RESOLUTION_640x720_3D_FRAME_PACKING     0x8b

#define CELL_VIDEO_OUT_REFRESH_AUTO                            0x00
#define CELL_VIDEO_OUT_REFRESH_59_94HZ                         0x01
#define CELL_VIDEO_OUT_REFRESH_50HZ                            0x02
#define CELL_VIDEO_OUT_REFRESH_60HZ                            0x04
#define CELL_VIDEO_OUT_REFRESH_30HZ                            0x08

#define CELL_VIDEO_OUT_PORT_NONE                               0x00
#define CELL_VIDEO_OUT_PORT_HDMI                               0x01
#define CELL_VIDEO_OUT_PORT_NETWORK                            0x41
#define CELL_VIDEO_OUT_PORT_COMPOSITE                          0x81
#define CELL_VIDEO_OUT_PORT_D                                  0x82
#define CELL_VIDEO_OUT_PORT_COMPONENT                          0x83
#define CELL_VIDEO_OUT_PORT_RGB                                0x84
#define CELL_VIDEO_OUT_PORT_SCART                              0x85
#define CELL_VIDEO_OUT_PORT_DSUB                               0x86

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Video resolution.
*/
typedef struct _videoresolution
{
    u16 width;               /*!< \brief Screen width in pixels. */
    u16 height;              /*!< \brief Screen height in pixels. */
} CellVideoOutResolution;

/*! \brief Video display mode.
*/
typedef struct _videodisplaymode
{
/*! \brief resolution id.

    The possible values are:
    - \c CELL_VIDEO_OUT_RESOLUTION_1080
    - \c CELL_VIDEO_OUT_RESOLUTION_720
    - \c CELL_VIDEO_OUT_RESOLUTION_480
    - \c CELL_VIDEO_OUT_RESOLUTION_576
*/
    u8 resolution;
/*! \brief Scan mode.
    \todo more explanations needed. */
    u8 scanMode;
/*! \brief Conversion mode.
    \todo more explanations needed. */
    u8 conversion;
/*! \brief aspect ratio.

    The possible values are:
    - \c CELL_VIDEO_OUT_ASPECT_AUTO
    - \c CELL_VIDEO_OUT_ASPECT_4_3
    - \c CELL_VIDEO_OUT_ASPECT_16_9
*/
    u8 aspect;
/*! \brief unused. */
    u8 padding[2];
/*! \brief Refresh rates.
    \todo more explanations needed. */
    u16 refreshRates;
} CellVideoOutDisplayMode;

/*! \brief Video state stucture.
*/
typedef struct _videostate
{
/*! \brief state value.

    The possible values are:
    - \c CELL_VIDEO_OUT_STATE_DISABLED
    - \c CELL_VIDEO_OUT_STATE_ENABLED
    - \c CELL_VIDEO_OUT_STATE_BUSY
*/
    u8 state;
/*! \brief color space.

    The possible values are:
    - \c CELL_VIDEO_OUT_BUFFER_FORMAT_XRGB
    - \c CELL_VIDEO_OUT_BUFFER_FORMAT_XBGR
    - \c CELL_VIDEO_OUT_BUFFER_FORMAT_FLOAT
    \todo verify
*/
    u8 colorSpace;
/*! \brief unused. */
    u8 padding[6];
/*! \brief display mode. */
    CellVideoOutDisplayMode displayMode;
} CellVideoOutState;

/*! \brief Video configuration structure.
*/
typedef struct _videoconfig
{
/*! \brief resolution id.

    The possible values are:
    - \c CELL_VIDEO_OUT_RESOLUTION_1080
    - \c CELL_VIDEO_OUT_RESOLUTION_720
    - \c CELL_VIDEO_OUT_RESOLUTION_480
    - \c CELL_VIDEO_OUT_RESOLUTION_576
*/
    u8 resolution;

/*! \brief video buffer format.

    The possible values are:
    - \c CELL_VIDEO_OUT_BUFFER_FORMAT_XRGB
    - \c CELL_VIDEO_OUT_BUFFER_FORMAT_XBGR
    - \c CELL_VIDEO_OUT_BUFFER_FORMAT_FLOAT
*/
    u8 format;

/*! \brief aspect ratio.

    The possible values are:
    - \c CELL_VIDEO_OUT_ASPECT_AUTO
    - \c CELL_VIDEO_OUT_ASPECT_4_3
    - \c CELL_VIDEO_OUT_ASPECT_16_9
*/
    u8 aspect;
/*! \brief unused. */
    u8 padding[9];

/*! \brief offset in bytes between the beginnings of consecutive lines.
*/
    u32 pitch;
} CellVideoOutConfiguration;

typedef struct _videoColorInfo
{
    u16 redX;
    u16 redY;
    u16 greenX;
    u16 greenY;
    u16 blueX;
    u16 blueY;
    u16 whiteX;
    u16 whiteY;
    u32 gamma;
} CellVideoOutColorInfo;

typedef struct _videoKSVList
{
    u8 ksv[32*5];
    u8 padding[4];
    u32 count;
} CellVideoOutKSVList;

typedef struct _videoDeviceInfo
{
    u8 portType;
    u8 colorSpace;
    u16 latency;
    u8 availableModeCount;
    u8 state;
    u8 rgbOutputRange;
    u8 padding[5];
    CellVideoOutColorInfo colorInfo;
    CellVideoOutDisplayMode availableModes[32];
    CellVideoOutKSVList ksvList;
} CellVideoOutDeviceInfo;

typedef s32 (*CellVideoOutCallback)(u32 slot, u32 videoOut, u32 deviceIndex, u32 event, CellVideoOutDeviceInfo *info, void *userData);

/*! \brief Get video state

    For the default display, just use 0 for \p videoOut and \p deviceIndex.
    \param videoOut Video output id.
    \param deviceIndex Devide index.
    \param state Pointer to a video state structure to be updated.
    \return zero if no error, nonzero otherwise.
    \todo verify the parameters signification.
*/
s32 cellVideoOutGetState(s32 videoOut,s32 deviceIndex,CellVideoOutState *state);

/*! \brief Get video resolution from resolution id.
    \param resolutionId The input resolution id. The possible values are:
    - \c CELL_VIDEO_OUT_RESOLUTION_1080
    - \c CELL_VIDEO_OUT_RESOLUTION_720
    - \c CELL_VIDEO_OUT_RESOLUTION_480
    - \c CELL_VIDEO_OUT_RESOLUTION_576
    \param resolution Pointer to the video resolution structure to be updated.
    \return zero if no error, nonzero otherwise.
*/
s32 cellVideoOutGetResolution(s32 resolutionId,CellVideoOutResolution *resolution);

/*! \brief Configure the video output.
    \param videoOut Video output id.
    \param config Pointer to a video configuration structure.
    \param option Pointer to additional video configuration data.
    \param blocking Nonzero indicates if the call is blocking or not.
    \return zero if no error, nonzero otherwise.
    \todo verify the parameters signification.
*/
s32 cellVideoOutConfigure(s32 videoOut,CellVideoOutConfiguration *config,void *option,s32 blocking);

s32 cellVideoOutGetNumberOfDevice(u32 videoOut);
s32 cellVideoOutGetDeviceInfo(u32 videoOut, u32 deviceIndex, CellVideoOutDeviceInfo *info);
s32 cellVideoOutGetConfiguration(u32 videoOut, CellVideoOutConfiguration *config, void *option);
s32 cellVideoOutGetResolutionAvailability(u32 videoOut, u32 resolutionId, u32 aspect, u32 option);
s32 cellVideoOutDebugSetMonitorType(u32 videoOut, u32 monitorType);
s32 cellVideoOutGetConvertCursorColorInfo(u8 *rgbOutputRange);

s32 cellVideoOutRegisterCallback(u32 slot, CellVideoOutCallback cbVideo, void *userData);
s32 cellVideoOutUnregisterCallback(u32 slot);

#ifdef __cplusplus
    }
#endif

#endif
