/*! \file osk.h
\brief PS3 on-screen keyboard management.
*/

#ifndef __LV2_CELL_OSKDIALOG_H__
#define __LV2_CELL_OSKDIALOG_H__

#include <ppu-types.h>

#define CELL_OSKDIALOG_10KEY_PANEL							1
#define CELL_OSKDIALOG_FULLKEY_PANEL						2

#define CELL_OSKDIALOG_DEVICE_MASK_PAD						0xff

#define CELL_OSKDIALOG_LANG_KOREAN							0x00001000
#define CELL_OSKDIALOG_LANG_DANISH							0x00020000
#define CELL_OSKDIALOG_LANG_SWEDISH						0x00040000
#define CELL_OSKDIALOG_LANG_NORWEGIAN						0x00080000
#define CELL_OSKDIALOG_LANG_FINNISH						0x00100000
#define CELL_OSKDIALOG_LANG_TRADITIONAL_CHINESE
#define CELL_OSKDIALOG_LANG_SIMPLIFIED_CHINESE


#define CELL_OSKDIALOG_PANEL_TYPE_DEFAULT 					0x00000000
#define CELL_OSKDIALOG_PANEL_TYPE_DEFAULT_NO_JAPANESE 		0x00000200
#define CELL_OSKDIALOG_PANEL_TYPE_ALPHABET_FULL_WIDTH 		0x00800000
#define CELL_OSKDIALOG_PANEL_TYPE_ALPHABET 				0x01000000
#define CELL_OSKDIALOG_PANEL_TYPE_NUMERAL_FULL_WIDTH 		0x04000000
#define CELL_OSKDIALOG_PANEL_TYPE_NUMERAL 					0x08000000
#define CELL_OSKDIALOG_PANEL_TYPE_JAPANESE 				0x00000100
#define CELL_OSKDIALOG_PANEL_TYPE_JAPANESE_HIRAGANA 		0x00200000
#define CELL_OSKDIALOG_PANEL_TYPE_JAPANESE_KATAKANA 		0x00400000
#define CELL_OSKDIALOG_PANEL_TYPE_ENGLISH 					0x00000002
#define CELL_OSKDIALOG_PANEL_TYPE_FRENCH 					0x00000008
#define CELL_OSKDIALOG_PANEL_TYPE_SPANISH 					0x00000004
#define CELL_OSKDIALOG_PANEL_TYPE_DUTCH 					0x00000020
#define CELL_OSKDIALOG_PANEL_TYPE_GERMAN 					0x00000001
#define CELL_OSKDIALOG_PANEL_TYPE_RUSSIAN 					0x00000080
#define CELL_OSKDIALOG_PANEL_TYPE_ITALIAN 					0x00000010
#define CELL_OSKDIALOG_PANEL_TYPE_PORTUGUESE 				0x00000040
#define CELL_OSKDIALOG_PANEL_TYPE_KOREAN 					0x00001000
#define CELL_OSKDIALOG_PANEL_TYPE_DANISH 					0x00020000
#define CELL_OSKDIALOG_PANEL_TYPE_SWEDISH 					0x00040000
#define CELL_OSKDIALOG_PANEL_TYPE_NORWEGIAN 				0x00080000
#define CELL_OSKDIALOG_PANEL_TYPE_FINNISH 					0x00100000
#define CELL_OSKDIALOG_PANEL_TYPE_TRADITIONAL_CHINESE 		0x00004000
#define CELL_OSKDIALOG_PANEL_TYPE_SIMPLIFIED_CHINESE 		0x00008000
#define CELL_OSKDIALOG_PANEL_TYPE_LATIN 					0x02000000
#define CELL_OSKDIALOG_PANEL_TYPE_URL 						0x10000000
#define CELL_OSKDIALOG_PANEL_TYPE_PASSWORD 				0x20000000


#define CELL_OSKDIALOG_PROHIBIT_SPACE 						0x00000001
#define CELL_OSKDIALOG_PROHIBIT_RETURN 					0x00000002
#define CELL_OSKDIALOG_PROHIBIT_INPUT_ANALOG 				0x00000008 

#define CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_RIGHT 	0x00000200
#define CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER 	0x00000400
#define CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_LEFT 	0x00000800
#define CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_BOTTOM	0x00001000
#define CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_CENTER 	0x00002000
#define CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_TOP 	0x00004000

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief On-screen keyboard initial layout values. */
typedef enum
{
	CELL_OSKDIALOG_INITIAL_SYSTEM_PANEL,	/*!< \brief System panel. */
	CELL_OSKDIALOG_INITIAL_10KEY_PANEL,	/*!< \brief 10-key keypad panel. */
	CELL_OSKDIALOG_INITIAL_FULLKEY_PANEL	/*!< \brief full keyboard panel. */
} CellOskDialogKeyLayout;

/*! \brief Result of on-screen keyboard input. */
typedef enum
{
	CELL_OSKDIALOG_OK,						/*!< \brief Input success. */
	CELL_OSKDIALOG_CANCELED,				/*!< \brief Input canceled. */
	CELL_OSKDIALOG_ABORT,					/*!< \brief Input aborted. */
	CELL_OSKDIALOG_NO_TEXT					/*!< \brief Input empty. */
} CellOskDialogInputFieldResult;

/*! \brief On-screen keyboard input device values. */
typedef enum
{
	CELL_OSKDIALOG_DEVICE_PAD,				/*!< \brief PS3 Pad/controller. */
	CELL_OSKDIALOG_DEVICE_KB				/*!< \brief Keyboard. */
} CellOskDialogInputDevice;

/*! \brief On-screen keyboard continuous mode setting values. */
typedef enum
{
	CELL_OSKDIALOG_CONTINUOUS_MODE_NONE,			/*!< \brief No continuous mode. */
	CELL_OSKDIALOG_CONTINUOUS_MODE_REMAIN_OPEN,	/*!< \brief Keep panel open. */
	CELL_OSKDIALOG_CONTINUOUS_MODE_HIDE,			/*!< \brief Hide panel by default. */
	CELL_OSKDIALOG_CONTINUOUS_MODE_SHOW			/*!< \brief Show panel by default. */
} CellOskDialogContinuousMode;

/*! \brief On-screen keyboard type values. */
typedef enum
{
	CELL_OSKDIALOG_SINGLELINE,						//!< Single line panel.
	CELL_OSKDIALOG_MULTILINE,						//!< Multiline panel.
	CELL_OSKDIALOG_FULLKB_SINGLELINE,				//!< Single line panel with full keyboard.
	CELL_OSKDIALOG_FULLKB_MULTILINE,				//!< Multiline panel with full keyboard.
	CELL_OSKDIALOG_SEPARATE_SINGLELINE_WINDOWED,	//!< Separated single line panel.
	CELL_OSKDIALOG_SEPARATE_MULTILINE_WINDOWED,	//!< Separated multiline panel.
	CELL_OSKDIALOG_SEPARATE_INPUT_PANEL_WINDOWED,	//!< Separated single line panel with 10-key keypad.
	CELL_OSKDIALOG_SEPARATE_FULLKB_INPUT_PANEL_WINDOWED,	//!< Separated single line panel with full keyboard.
	CELL_OSKDIALOG_SEPARATE_CANDIDATE_WINDOWED		//!< Separated panel with candidate selector (for keyboard input)
} CellOskDialogType;

/*! \brief Point structure for on-screen keyboard management. */
typedef struct _osk_pnt
{
	f32 x;		//!< Horizontal coordinate.
	f32 y;		//!< Vertical coordinate.
} CellOskDialogPoint;

/*! \brief On-screen keyboard manager parameters. */
typedef struct _osk_param
{
	/*! \brief Allowed panel types

	This is an OR'ed combination of the following values:
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DEFAULT
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DEFAULT_NO_JAPANESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ALPHABET_FULL_WIDTH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ALPHABET
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NUMERAL_FULL_WIDTH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NUMERAL
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE_HIRAGANA
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE_KATAKANA
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ENGLISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_FRENCH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SPANISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DUTCH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_GERMAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_RUSSIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ITALIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_PORTUGUESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_KOREAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DANISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SWEDISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NORWEGIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_FINNISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_TRADITIONAL_CHINESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SIMPLIFIED_CHINESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_LATIN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_URL
	- \ref CELL_OSKDIALOG_PANEL_TYPE_PASSWORD
	*/
	u32 allowedPanels;

	/*! \brief First panel type to be used.

	This is one of the following values:
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DEFAULT
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DEFAULT_NO_JAPANESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ALPHABET_FULL_WIDTH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ALPHABET
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NUMERAL_FULL_WIDTH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NUMERAL
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE_HIRAGANA
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE_KATAKANA
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ENGLISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_FRENCH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SPANISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DUTCH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_GERMAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_RUSSIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ITALIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_PORTUGUESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_KOREAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DANISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SWEDISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NORWEGIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_FINNISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_TRADITIONAL_CHINESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SIMPLIFIED_CHINESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_LATIN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_URL
	- \ref CELL_OSKDIALOG_PANEL_TYPE_PASSWORD
	*/
	u32 firstViewPanel;
	/*! \brief Panel original control point. */
	CellOskDialogPoint controlPoint;
	/*! \brief Prohibit flags.

	This is an OR'ed combination of the following values:
	- \ref CELL_OSKDIALOG_PROHIBIT_SPACE
	- \ref CELL_OSKDIALOG_PROHIBIT_RETURN
	- \ref CELL_OSKDIALOG_PROHIBIT_INPUT_ANALOG
	*/
	s32 prohibitFlags;
} CellOskDialogParam;

/*! \brief On-screen keyboard panel layout info data structure. */
typedef struct _osk_layout_info
{
	/*! \brief Layout mode.

	This is an OR'ed combination of the following values:
	- \ref CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_LEFT
	- \ref CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER
	- \ref CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_RIGHT
	- \ref CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_TOP
	- \ref CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_CENTER
	- \ref CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_BOTTOM
	*/
	s32 layoutMode;
	/*! \brief Panel position. */
	CellOskDialogPoint position;
} CellOskDialogLayoutInfo;

/*! \brief On-screen keyboard input field information data structure. */
typedef struct _osk_input_field_info
{
	u16 *message ATTRIBUTE_PRXPTR;			//!< Buffer for message
	u16 *startText ATTRIBUTE_PRXPTR;		//!< Initial message.
	s32 maxLength;							//!< Maximum number of characters.
} CellOskDialogInputFieldInfo;

/*! \brief On-screen keyboard return parameters data structure. */
typedef struct _osk_cb_return_param
{
	CellOskDialogInputFieldResult res;				//!< Result status.
	s32 len;								//!< Number of characters entered.
	u16 *str ATTRIBUTE_PRXPTR;				//!< Array of characters entered.
} CellOskDialogCallbackReturnParam;

/*! \brief  On-screen keyboard separate window options data structure. */
typedef struct _osk_separate_window_option
{
	CellOskDialogContinuousMode continuous;							//!< Continuous mode.
	s32 mask;												//!< Devices mask. Either \c 0 or \ref CELL_OSKDIALOG_DEVICE_MASK_PAD.
	s32 inputWidth;											//!< Input field width.
	f32 inputFieldTransparency;								//!< Input field transparency.
	CellOskDialogLayoutInfo *inputFieldLayoutInfo ATTRIBUTE_PRXPTR;	//!< Input field layout info.
	CellOskDialogLayoutInfo *inputPanelLayoutInfo ATTRIBUTE_PRXPTR;	//!< Input panel layout info.
	u32 reserved;											//!< Reserved.
} CellOskDialogSeparateWindowOption;

/*! \brief Set initial input device.
\param[in] input Initial input device value.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogSetInitialInputDevice(CellOskDialogInputDevice input);

/*! \brief Get size of on-screen keyboard panel.
\param[out] width Width of OSK panel in pixels.
\param[out] height Height of OSK panel in pixels.
\param[in] type OSK type.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogGetSize(u16 *width, u16 *height, CellOskDialogType type);

/*! \brief Close the on-screen keyboard panel.
\param[out] param Return parameters.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogUnloadAsync(CellOskDialogCallbackReturnParam *param);

/*! \brief Disable the on-screen keyboard panel's background dimmer filter.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogDisableDimmer();

/*! \brief Set the on-screen keyboard key layout.
\param[in] flags Key layout. Possible values are:
- \ref CELL_OSKDIALOG_10KEY_PANEL
- \ref CELL_OSKDIALOG_FULLKEY_PANEL
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogSetKeyLayoutOption(u32 flags);

/*! \brief Abort the currently opened on-screen keyboard panel.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogAbort();

/*! \brief Set on-screen keyboard device mask.
\param[in] mask Devices mask. Either \c 0 or \ref CELL_OSKDIALOG_DEVICE_MASK_PAD.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogSetDeviceMask(u32 mask);

/*! \brief Set on-screen keyboard separate window option.
\param[in] option Separate window option.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogSetSeparateWindowOption(CellOskDialogSeparateWindowOption *option);

/*! \brief Get on-screen keyboard input text.
\param[out] info Separate window option.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogGetInputText(CellOskDialogCallbackReturnParam *info);

/*! \brief Add a supported language to on-screen keyboard.
\param[in] lang Additional language. \n
	This is one of the following values:
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DEFAULT
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DEFAULT_NO_JAPANESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ALPHABET_FULL_WIDTH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ALPHABET
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NUMERAL_FULL_WIDTH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NUMERAL
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE_HIRAGANA
	- \ref CELL_OSKDIALOG_PANEL_TYPE_JAPANESE_KATAKANA
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ENGLISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_FRENCH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SPANISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DUTCH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_GERMAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_RUSSIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_ITALIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_PORTUGUESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_KOREAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_DANISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SWEDISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_NORWEGIAN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_FINNISH
	- \ref CELL_OSKDIALOG_PANEL_TYPE_TRADITIONAL_CHINESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_SIMPLIFIED_CHINESE
	- \ref CELL_OSKDIALOG_PANEL_TYPE_LATIN
	- \ref CELL_OSKDIALOG_PANEL_TYPE_URL
	- \ref CELL_OSKDIALOG_PANEL_TYPE_PASSWORD
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogAddSupportLanguage(u32 lang);

/*! \brief Open the on-screen keyboard panel.
\param[in] container Memory container id. Typical used sizes are 4 MB.
\param[in] param Parameters.
\param[in] inputInfo Input information.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogLoadAsync(sys_mem_container_t container, const CellOskDialogParam *param, const CellOskDialogInputFieldInfo *inputInfo);

/*! \brief Set the on-screen keyboard initial key layout.
\param[in] layout Initial key layout.
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogSetInitialKeyLayout(CellOskDialogKeyLayout layout);

/*! \brief Set the on-screen keyboard initial layout mode.
\param[in] mode Layout mode. \n
	This is an OR'ed combination of the following values:
	- \ref CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_LEFT
	- \ref CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER
	- \ref CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_RIGHT
	- \ref CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_TOP
	- \ref CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_CENTER
	- \ref CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_BOTTOM
\return zero if no error occured, nonzero otherwise.
*/
s32 cellOskDialogSetLayoutMode (s32 mode);

#ifdef __cplusplus
	}
#endif

#endif
