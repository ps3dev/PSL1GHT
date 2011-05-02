/*! \file video.h
\brief Video mode management.
*/

#ifndef __LV2_VIDEO_H__
#define __LV2_VIDEO_H__

#include <ppu-types.h>

#define VIDEO_STATE_DISABLED			0
#define VIDEO_STATE_ENABLED				1
#define VIDEO_STATE_BUSY				3

#define VIDEO_BUFFER_FORMAT_XRGB		0
#define VIDEO_BUFFER_FORMAT_XBGR		1
#define VIDEO_BUFFER_FORMAT_FLOAT		2

#define VIDEO_ASPECT_AUTO				0
#define VIDEO_ASPECT_4_3				1
#define VIDEO_ASPECT_16_9				2

#define VIDEO_RESOLUTION_1080			1
#define VIDEO_RESOLUTION_720			2
#define VIDEO_RESOLUTION_480			4
#define VIDEO_RESOLUTION_576			5

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Video resolution.
*/
typedef struct _videoresolution
{
	u16 width;		/*!< \brief Screen width in pixels. */
	u16 height;		/*!< \brief Screen height in pixels. */
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

#ifdef __cplusplus
	}
#endif

#endif
