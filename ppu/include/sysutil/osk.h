/*! \file osk.h
\brief PS3 on-screen keyboard management.
*/

#ifndef __LV2_OSK_H__
#define __LV2_OSK_H__

#include <ppu-types.h>

#define OSK_10KEY_PANEL							1
#define OSK_FULLKEY_PANEL						2

#define OSK_DEVICE_MASK_PAD						0xff

#define OSK_LANG_KOREAN							0x00001000
#define OSK_LANG_DANISH							0x00020000
#define OSK_LANG_SWEDISH						0x00040000
#define OSK_LANG_NORWEGIAN						0x00080000
#define OSK_LANG_FINNISH						0x00100000
#define OSK_LANG_TRADITIONAL_CHINESE
#define OSK_LANG_SIMPLIFIED_CHINESE


#define OSK_PANEL_TYPE_DEFAULT 					0x00000000
#define OSK_PANEL_TYPE_DEFAULT_NO_JAPANESE 		0x00000200
#define OSK_PANEL_TYPE_ALPHABET_FULL_WIDTH 		0x00800000
#define OSK_PANEL_TYPE_ALPHABET 				0x01000000
#define OSK_PANEL_TYPE_NUMERAL_FULL_WIDTH 		0x04000000
#define OSK_PANEL_TYPE_NUMERAL 					0x08000000
#define OSK_PANEL_TYPE_JAPANESE 				0x00000100
#define OSK_PANEL_TYPE_JAPANESE_HIRAGANA 		0x00200000
#define OSK_PANEL_TYPE_JAPANESE_KATAKANA 		0x00400000
#define OSK_PANEL_TYPE_ENGLISH 					0x00000002
#define OSK_PANEL_TYPE_FRENCH 					0x00000008
#define OSK_PANEL_TYPE_SPANISH 					0x00000004
#define OSK_PANEL_TYPE_DUTCH 					0x00000020
#define OSK_PANEL_TYPE_GERMAN 					0x00000001
#define OSK_PANEL_TYPE_RUSSIAN 					0x00000080
#define OSK_PANEL_TYPE_ITALIAN 					0x00000010
#define OSK_PANEL_TYPE_PORTUGUESE 				0x00000040
#define OSK_PANEL_TYPE_KOREAN 					0x00001000
#define OSK_PANEL_TYPE_DANISH 					0x00020000
#define OSK_PANEL_TYPE_SWEDISH 					0x00040000
#define OSK_PANEL_TYPE_NORWEGIAN 				0x00080000
#define OSK_PANEL_TYPE_FINNISH 					0x00100000
#define OSK_PANEL_TYPE_TRADITIONAL_CHINESE 		0x00004000
#define OSK_PANEL_TYPE_SIMPLIFIED_CHINESE 		0x00008000
#define OSK_PANEL_TYPE_LATIN 					0x02000000
#define OSK_PANEL_TYPE_URL 						0x10000000
#define OSK_PANEL_TYPE_PASSWORD 				0x20000000


#define OSK_PROHIBIT_SPACE 						0x00000001
#define OSK_PROHIBIT_RETURN 					0x00000002
#define OSK_PROHIBIT_INPUT_ANALOG 				0x00000008 

#define OSK_LAYOUTMODE_HORIZONTAL_ALIGN_RIGHT 	0x00000200
#define OSK_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER 	0x00000400
#define OSK_LAYOUTMODE_HORIZONTAL_ALIGN_LEFT 	0x00000800
#define OSK_LAYOUTMODE_VERTICAL_ALIGN_BOTTOM	0x00001000
#define OSK_LAYOUTMODE_VERTICAL_ALIGN_CENTER 	0x00002000
#define OSK_LAYOUTMODE_VERTICAL_ALIGN_TOP 	0x00004000

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief On-screen keyboard initial layout values. */
typedef enum
{
	OSK_INITIAL_SYSTEM_PANEL,	/*!< \brief System panel. */
	OSK_INITIAL_10KEY_PANEL,	/*!< \brief 10-key keypad panel. */
	OSK_INITIAL_FULLKEY_PANEL	/*!< \brief full keyboard panel. */
} oskKeyLayout;

/*! \brief Result of on-screen keyboard input. */
typedef enum
{
	OSK_OK,						/*!< \brief Input success. */
	OSK_CANCELED,				/*!< \brief Input canceled. */
	OSK_ABORT,					/*!< \brief Input aborted. */
	OSK_NO_TEXT					/*!< \brief Input empty. */
} oskInputFieldResult;

/*! \brief On-screen keyboard input device values. */
typedef enum
{
	OSK_DEVICE_PAD,				/*!< \brief PS3 Pad/controller. */
	OSK_DEVICE_KB				/*!< \brief Keyboard. */
} oskInputDevice;

/*! \brief On-screen keyboard continuous mode setting values. */
typedef enum
{
	OSK_CONTINUOUS_MODE_NONE,			/*!< \brief No continuous mode. */
	OSK_CONTINUOUS_MODE_REMAIN_OPEN,	/*!< \brief Keep panel open. */
	OSK_CONTINUOUS_MODE_HIDE,			/*!< \brief Hide panel by default. */
	OSK_CONTINUOUS_MODE_SHOW			/*!< \brief Show panel by default. */
} oskContinuousMode;

/*! \brief On-screen keyboard type values. */
typedef enum
{
	OSK_SINGLELINE,						//!< Single line panel.
	OSK_MULTILINE,						//!< Multiline panel.
	OSK_FULLKB_SINGLELINE,				//!< Single line panel with full keyboard.
	OSK_FULLKB_MULTILINE,				//!< Multiline panel with full keyboard.
	OSK_SEPARATE_SINGLELINE_WINDOWED,	//!< Separated single line panel.
	OSK_SEPARATE_MULTILINE_WINDOWED,	//!< Separated multiline panel.
	OSK_SEPARATE_INPUT_PANEL_WINDOWED,	//!< Separated single line panel with 10-key keypad.
	OSK_SEPARATE_FULLKB_INPUT_PANEL_WINDOWED,	//!< Separated single line panel with full keyboard.
	OSK_SEPARATE_CANDIDATE_WINDOWED		//!< Separated panel with candidate selector (for keyboard input)
} oskType;

/*! \brief Point structure for on-screen keyboard management. */
typedef struct _osk_pnt
{
	f32 x;		//!< Horizontal coordinate.
	f32 y;		//!< Vertical coordinate.
} oskPoint;

/*! \brief On-screen keyboard manager parameters. */
typedef struct _osk_param
{
	/*! \brief Allowed panel types

	This is an OR'ed combination of the following values:
	- \ref OSK_PANEL_TYPE_DEFAULT
	- \ref OSK_PANEL_TYPE_DEFAULT_NO_JAPANESE
	- \ref OSK_PANEL_TYPE_ALPHABET_FULL_WIDTH
	- \ref OSK_PANEL_TYPE_ALPHABET
	- \ref OSK_PANEL_TYPE_NUMERAL_FULL_WIDTH
	- \ref OSK_PANEL_TYPE_NUMERAL
	- \ref OSK_PANEL_TYPE_JAPANESE
	- \ref OSK_PANEL_TYPE_JAPANESE_HIRAGANA
	- \ref OSK_PANEL_TYPE_JAPANESE_KATAKANA
	- \ref OSK_PANEL_TYPE_ENGLISH
	- \ref OSK_PANEL_TYPE_FRENCH
	- \ref OSK_PANEL_TYPE_SPANISH
	- \ref OSK_PANEL_TYPE_DUTCH
	- \ref OSK_PANEL_TYPE_GERMAN
	- \ref OSK_PANEL_TYPE_RUSSIAN
	- \ref OSK_PANEL_TYPE_ITALIAN
	- \ref OSK_PANEL_TYPE_PORTUGUESE
	- \ref OSK_PANEL_TYPE_KOREAN
	- \ref OSK_PANEL_TYPE_DANISH
	- \ref OSK_PANEL_TYPE_SWEDISH
	- \ref OSK_PANEL_TYPE_NORWEGIAN
	- \ref OSK_PANEL_TYPE_FINNISH
	- \ref OSK_PANEL_TYPE_TRADITIONAL_CHINESE
	- \ref OSK_PANEL_TYPE_SIMPLIFIED_CHINESE
	- \ref OSK_PANEL_TYPE_LATIN
	- \ref OSK_PANEL_TYPE_URL
	- \ref OSK_PANEL_TYPE_PASSWORD
	*/
	u32 allowedPanels;

	/*! \brief First panel type to be used.

	This is one of the following values:
	- \ref OSK_PANEL_TYPE_DEFAULT
	- \ref OSK_PANEL_TYPE_DEFAULT_NO_JAPANESE
	- \ref OSK_PANEL_TYPE_ALPHABET_FULL_WIDTH
	- \ref OSK_PANEL_TYPE_ALPHABET
	- \ref OSK_PANEL_TYPE_NUMERAL_FULL_WIDTH
	- \ref OSK_PANEL_TYPE_NUMERAL
	- \ref OSK_PANEL_TYPE_JAPANESE
	- \ref OSK_PANEL_TYPE_JAPANESE_HIRAGANA
	- \ref OSK_PANEL_TYPE_JAPANESE_KATAKANA
	- \ref OSK_PANEL_TYPE_ENGLISH
	- \ref OSK_PANEL_TYPE_FRENCH
	- \ref OSK_PANEL_TYPE_SPANISH
	- \ref OSK_PANEL_TYPE_DUTCH
	- \ref OSK_PANEL_TYPE_GERMAN
	- \ref OSK_PANEL_TYPE_RUSSIAN
	- \ref OSK_PANEL_TYPE_ITALIAN
	- \ref OSK_PANEL_TYPE_PORTUGUESE
	- \ref OSK_PANEL_TYPE_KOREAN
	- \ref OSK_PANEL_TYPE_DANISH
	- \ref OSK_PANEL_TYPE_SWEDISH
	- \ref OSK_PANEL_TYPE_NORWEGIAN
	- \ref OSK_PANEL_TYPE_FINNISH
	- \ref OSK_PANEL_TYPE_TRADITIONAL_CHINESE
	- \ref OSK_PANEL_TYPE_SIMPLIFIED_CHINESE
	- \ref OSK_PANEL_TYPE_LATIN
	- \ref OSK_PANEL_TYPE_URL
	- \ref OSK_PANEL_TYPE_PASSWORD
	*/
	u32 firstViewPanel;
	/*! \brief Panel original control point. */
	oskPoint controlPoint;
	/*! \brief Prohibit flags.

	This is an OR'ed combination of the following values:
	- \ref OSK_PROHIBIT_SPACE
	- \ref OSK_PROHIBIT_RETURN
	- \ref OSK_PROHIBIT_INPUT_ANALOG
	*/
	s32 prohibitFlags;
} oskParam;

/*! \brief On-screen keyboard panel layout info data structure. */
typedef struct _osk_layout_info
{
	/*! \brief Layout mode.

	This is an OR'ed combination of the following values:
	- \ref OSK_LAYOUTMODE_HORIZONTAL_ALIGN_LEFT
	- \ref OSK_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER
	- \ref OSK_LAYOUTMODE_HORIZONTAL_ALIGN_RIGHT
	- \ref OSK_LAYOUTMODE_VERTICAL_ALIGN_TOP
	- \ref OSK_LAYOUTMODE_VERTICAL_ALIGN_CENTER
	- \ref OSK_LAYOUTMODE_VERTICAL_ALIGN_BOTTOM
	*/
	s32 layoutMode;
	/*! \brief Panel position. */
	oskPoint position;
} oskLayoutInfo;

/*! \brief On-screen keyboard input field information data structure. */
typedef struct _osk_input_field_info
{
	u16 *message ATTRIBUTE_PRXPTR;			//!< Buffer for message
	u16 *startText ATTRIBUTE_PRXPTR;		//!< Initial message.
	s32 maxLength;							//!< Maximum number of characters.
} oskInputFieldInfo;

/*! \brief On-screen keyboard return parameters data structure. */
typedef struct _osk_cb_return_param
{
	oskInputFieldResult res;				//!< Result status.
	s32 len;								//!< Number of characters entered.
	u16 *str ATTRIBUTE_PRXPTR;				//!< Array of characters entered.
} oskCallbackReturnParam;

/*! \brief  On-screen keyboard separate window options data structure. */
typedef struct _osk_separate_window_option
{
	oskContinuousMode continuous;							//!< Continuous mode.
	s32 mask;												//!< Devices mask. Either \c 0 or \ref OSK_DEVICE_MASK_PAD.
	s32 inputWidth;											//!< Input field width.
	f32 inputFieldTransparency;								//!< Input field transparency.
	oskLayoutInfo *inputFieldLayoutInfo ATTRIBUTE_PRXPTR;	//!< Input field layout info.
	oskLayoutInfo *inputPanelLayoutInfo ATTRIBUTE_PRXPTR;	//!< Input panel layout info.
	u32 reserved;											//!< Reserved.
} oskSeparateWindowOption;

/*! \brief Set initial input device.
\param[in] input Initial input device value.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskSetInitialInputDevice(oskInputDevice input);

/*! \brief Get size of on-screen keyboard panel.
\param[out] width Width of OSK panel in pixels.
\param[out] height Height of OSK panel in pixels.
\param[in] type OSK type.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskGetSize(u16 *width, u16 *height, oskType type);

/*! \brief Close the on-screen keyboard panel.
\param[out] param Return parameters.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskUnloadAsync(oskCallbackReturnParam *param);

/*! \brief Disable the on-screen keyboard panel's background dimmer filter.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskDisableDimmer();

/*! \brief Set the on-screen keyboard key layout.
\param[in] flags Key layout. Possible values are:
- \ref OSK_10KEY_PANEL
- \ref OSK_FULLKEY_PANEL
\return zero if no error occured, nonzero otherwise.
*/
s32 oskSetKeyLayoutOption(u32 flags);

/*! \brief Abort the currently opened on-screen keyboard panel.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskAbort();

/*! \brief Set on-screen keyboard device mask.
\param[in] mask Devices mask. Either \c 0 or \ref OSK_DEVICE_MASK_PAD.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskSetDeviceMask(u32 mask);

/*! \brief Set on-screen keyboard separate window option.
\param[in] option Separate window option.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskSetSeparateWindowOption(oskSeparateWindowOption *option);

/*! \brief Get on-screen keyboard input text.
\param[out] info Separate window option.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskGetInputText(oskCallbackReturnParam *info);

/*! \brief Add a supported language to on-screen keyboard.
\param[in] lang Additional language. \n
	This is one of the following values:
	- \ref OSK_PANEL_TYPE_DEFAULT
	- \ref OSK_PANEL_TYPE_DEFAULT_NO_JAPANESE
	- \ref OSK_PANEL_TYPE_ALPHABET_FULL_WIDTH
	- \ref OSK_PANEL_TYPE_ALPHABET
	- \ref OSK_PANEL_TYPE_NUMERAL_FULL_WIDTH
	- \ref OSK_PANEL_TYPE_NUMERAL
	- \ref OSK_PANEL_TYPE_JAPANESE
	- \ref OSK_PANEL_TYPE_JAPANESE_HIRAGANA
	- \ref OSK_PANEL_TYPE_JAPANESE_KATAKANA
	- \ref OSK_PANEL_TYPE_ENGLISH
	- \ref OSK_PANEL_TYPE_FRENCH
	- \ref OSK_PANEL_TYPE_SPANISH
	- \ref OSK_PANEL_TYPE_DUTCH
	- \ref OSK_PANEL_TYPE_GERMAN
	- \ref OSK_PANEL_TYPE_RUSSIAN
	- \ref OSK_PANEL_TYPE_ITALIAN
	- \ref OSK_PANEL_TYPE_PORTUGUESE
	- \ref OSK_PANEL_TYPE_KOREAN
	- \ref OSK_PANEL_TYPE_DANISH
	- \ref OSK_PANEL_TYPE_SWEDISH
	- \ref OSK_PANEL_TYPE_NORWEGIAN
	- \ref OSK_PANEL_TYPE_FINNISH
	- \ref OSK_PANEL_TYPE_TRADITIONAL_CHINESE
	- \ref OSK_PANEL_TYPE_SIMPLIFIED_CHINESE
	- \ref OSK_PANEL_TYPE_LATIN
	- \ref OSK_PANEL_TYPE_URL
	- \ref OSK_PANEL_TYPE_PASSWORD
\return zero if no error occured, nonzero otherwise.
*/
s32 oskAddSupportLanguage(u32 lang);

/*! \brief Open the on-screen keyboard panel.
\param[in] container Memory container id. Typical used sizes are 4 MB.
\param[in] param Parameters.
\param[in] inputInfo Input information.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskLoadAsync(sys_mem_container_t container, const oskParam *param, const oskInputFieldInfo *inputInfo);

/*! \brief Set the on-screen keyboard initial key layout.
\param[in] layout Initial key layout.
\return zero if no error occured, nonzero otherwise.
*/
s32 oskSetInitialKeyLayout(oskKeyLayout layout);

/*! \brief Set the on-screen keyboard initial layout mode.
\param[in] mode Layout mode. \n
	This is an OR'ed combination of the following values:
	- \ref OSK_LAYOUTMODE_HORIZONTAL_ALIGN_LEFT
	- \ref OSK_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER
	- \ref OSK_LAYOUTMODE_HORIZONTAL_ALIGN_RIGHT
	- \ref OSK_LAYOUTMODE_VERTICAL_ALIGN_TOP
	- \ref OSK_LAYOUTMODE_VERTICAL_ALIGN_CENTER
	- \ref OSK_LAYOUTMODE_VERTICAL_ALIGN_BOTTOM
\return zero if no error occured, nonzero otherwise.
*/
s32 oskSetLayoutMode (s32 mode);

#ifdef __cplusplus
	}
#endif

#endif
