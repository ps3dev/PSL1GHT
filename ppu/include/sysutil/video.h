/*! \file video.h
    \brief Video mode management.
*/

#ifndef __LV2_VIDEO_H__
#define __LV2_VIDEO_H__

#include <ppu-types.h>

#define VIDEO_STATE_DISABLED                          0
#define VIDEO_STATE_ENABLED                           1
#define VIDEO_STATE_BUSY                              3

#define VIDEO_PRIMARY                                 0
#define VIDEO_SECONDARY                               1

#define VIDEO_SCANMODE_INTERLACE                      0
#define VIDEO_SCANMODE_PROGRESSIVE                    1

#define VIDEO_SCANMODE2_AUTO                          0
#define VIDEO_SCANMODE2_INTERLACE                     1
#define VIDEO_SCANMODE2_PROGRESSIVE                   2

#define VIDEO_BUFFER_FORMAT_XRGB                      0
#define VIDEO_BUFFER_FORMAT_XBGR                      1
#define VIDEO_BUFFER_FORMAT_FLOAT                     2

#define VIDEO_ASPECT_AUTO                             0
#define VIDEO_ASPECT_4_3                              1
#define VIDEO_ASPECT_16_9                             2

#define VIDEO_RESOLUTION_UNDEFINED                    0
#define VIDEO_RESOLUTION_1080                         1
#define VIDEO_RESOLUTION_720                          2
#define VIDEO_RESOLUTION_480                          4
#define VIDEO_RESOLUTION_576                          5
#define VIDEO_RESOLUTION_1600x1080                    10
#define VIDEO_RESOLUTION_1440x1080                    11
#define VIDEO_RESOLUTION_1280x1080                    12
#define VIDEO_RESOLUTION_960x1080                     13

#define VIDEO_COLOR_RGB                               0x01
#define VIDEO_COLOR_YUV                               0x02
#define VIDEO_COLOR_XVYCC                             0x04

#define VIDEO_RESOLUTION_720_3D_FRAME_PACKING         0x81
#define VIDEO_RESOLUTION_1024x720_3D_FRAME_PACKING    0x88
#define VIDEO_RESOLUTION_960x720_3D_FRAME_PACKING     0x89
#define VIDEO_RESOLUTION_800x720_3D_FRAME_PACKING     0x8a
#define VIDEO_RESOLUTION_640x720_3D_FRAME_PACKING     0x8b

#define VIDEO_REFRESH_AUTO                            0x00
#define VIDEO_REFRESH_59_94HZ                         0x01
#define VIDEO_REFRESH_50HZ                            0x02
#define VIDEO_REFRESH_60HZ                            0x04
#define VIDEO_REFRESH_30HZ                            0x08

#define VIDEO_PORT_NONE                               0x00
#define VIDEO_PORT_HDMI                               0x01
#define VIDEO_PORT_NETWORK                            0x41
#define VIDEO_PORT_COMPOSITE                          0x81
#define VIDEO_PORT_D                                  0x82
#define VIDEO_PORT_COMPONENT                          0x83
#define VIDEO_PORT_RGB                                0x84
#define VIDEO_PORT_SCART                              0x85
#define VIDEO_PORT_DSUB                               0x86

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Video resolution.
*/
typedef struct _videoresolution
{
    u16 width;               /*!< \brief Screen width in pixels. */
    u16 height;              /*!< \brief Screen height in pixels. */
} videoResolution;

/*! \brief Video display mode.
*/
typedef struct _videodisplaymode
{
/*! \brief resolution id.

    The possible values are:
    - \c VIDEO_RESOLUTION_1080
    - \c VIDEO_RESOLUTION_720
    - \c VIDEO_RESOLUTION_480
    - \c VIDEO_RESOLUTION_576
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
    - \c VIDEO_ASPECT_AUTO
    - \c VIDEO_ASPECT_4_3
    - \c VIDEO_ASPECT_16_9
*/
    u8 aspect;
/*! \brief unused. */
    u8 padding[2];
/*! \brief Refresh rates.
    \todo more explanations needed. */
    u16 refreshRates;
} videoDisplayMode;

/*! \brief Video state stucture.
*/
typedef struct _videostate
{
/*! \brief state value.

    The possible values are:
    - \c VIDEO_STATE_DISABLED
    - \c VIDEO_STATE_ENABLED
    - \c VIDEO_STATE_BUSY
*/
    u8 state;
/*! \brief color space.

    The possible values are:
    - \c VIDEO_BUFFER_FORMAT_XRGB
    - \c VIDEO_BUFFER_FORMAT_XBGR
    - \c VIDEO_BUFFER_FORMAT_FLOAT
    \todo verify
*/
    u8 colorSpace;
/*! \brief unused. */
    u8 padding[6];
/*! \brief display mode. */
    videoDisplayMode displayMode;
} videoState;

/*! \brief Video configuration structure.
*/
typedef struct _videoconfig
{
/*! \brief resolution id.

    The possible values are:
    - \c VIDEO_RESOLUTION_1080
    - \c VIDEO_RESOLUTION_720
    - \c VIDEO_RESOLUTION_480
    - \c VIDEO_RESOLUTION_576
*/
    u8 resolution;

/*! \brief video buffer format.

    The possible values are:
    - \c VIDEO_BUFFER_FORMAT_XRGB
    - \c VIDEO_BUFFER_FORMAT_XBGR
    - \c VIDEO_BUFFER_FORMAT_FLOAT
*/
    u8 format;

/*! \brief aspect ratio.

    The possible values are:
    - \c VIDEO_ASPECT_AUTO
    - \c VIDEO_ASPECT_4_3
    - \c VIDEO_ASPECT_16_9
*/
    u8 aspect;
/*! \brief unused. */
    u8 padding[9];

/*! \brief offset in bytes between the beginnings of consecutive lines.
*/
    u32 pitch;
} videoConfiguration;

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
} videoColorInfo;

typedef struct _videoKSVList
{
    u8 ksv[32*5];
    u8 padding[4];
    u32 count;
} videoKSVList;

typedef struct _videoDeviceInfo
{
    u8 portType;
    u8 colorSpace;
    u16 latency;
    u8 availableModeCount;
    u8 state;
    u8 rgbOutputRange;
    u8 padding[5];
    videoColorInfo colorInfo;
    videoDisplayMode availableModes[32];
    videoKSVList ksvList;
} videoDeviceInfo;


/*! \brief Get video state

    For the default display, just use 0 for \p videoOut and \p deviceIndex.
    \param videoOut Video output id.
    \param deviceIndex Devide index.
    \param state Pointer to a video state structure to be updated.
    \return zero if no error, nonzero otherwise.
    \todo verify the parameters signification.
*/
s32 videoGetState(s32 videoOut,s32 deviceIndex,videoState *state);

/*! \brief Get video resolution from resolution id.
    \param resolutionId The input resolution id. The possible values are:
    - \c VIDEO_RESOLUTION_1080
    - \c VIDEO_RESOLUTION_720
    - \c VIDEO_RESOLUTION_480
    - \c VIDEO_RESOLUTION_576
    \param resolution Pointer to the video resolution structure to be updated.
    \return zero if no error, nonzero otherwise.
*/
s32 videoGetResolution(s32 resolutionId,videoResolution *resolution);

/*! \brief Configure the video output.
    \param videoOut Video output id.
    \param config Pointer to a video configuration structure.
    \param option Pointer to additional video configuration data.
    \param blocking Nonzero indicates if the call is blocking or not.
    \return zero if no error, nonzero otherwise.
    \todo verify the parameters signification.
*/
s32 videoConfigure(s32 videoOut,videoConfiguration *config,void *option,s32 blocking);

s32 videoGetNumberOfDevice(u32 videoOut);
s32 videoGetDeviceInfo(u32 videoOut, u32 deviceIndex, videoDeviceInfo *info);
s32 videoGetConfiguration(u32 videoOut, videoConfiguration *config, void *option);
s32 videoGetResolutionAvailability(u32 videoOut, u32 resolutionId, u32 aspect, u32 option);
s32 videoDebugSetMonitorType(u32 videoOut, u32 monitorType);
s32 videoGetConvertCursorColorInfo(u8 *rgbOutputRange);

/* TODO: typedef int (*videoCallback)(u32 slot, u32 videoOut, u32 deviceIndex, u32 event, videoDeviceInfo *info, void *userData ); */
/* TODO: s32 videoRegisterCallback(u32 slot, videoCallback function, void *userData); */
/* TODO: s32 videoUnregisterCallback(u32 slot); */

#ifdef __cplusplus
    }
#endif

#endif
