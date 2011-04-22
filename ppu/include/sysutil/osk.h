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

#define OSK_LAYOUTMODE_HORIZONTAL_ALIGN_LEFT 	0x00000200
#define OSK_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER 	0x00000400
#define OSK_LAYOUTMODE_HORIZONTAL_ALIGN_RIGHT 	0x00000800
#define OSK_LAYOUTMODE_VERTICAL_ALIGN_TOP 		0x00001000
#define OSK_LAYOUTMODE_VERTICAL_ALIGN_CENTER 	0x00002000
#define OSK_LAYOUTMODE_VERTICAL_ALIGN_BOTTOM 	0x00004000

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	OSK_INITIAL_SYSTEM_PANEL,
	OSK_INITIAL_10KEY_PANEL,
	OSK_INITIAL_FULLKEY_PANEL
} oskKeyLayout;

typedef enum
{
	OSK_OK,
	OSK_CANCELED,
	OSK_ABORT,
	OSK_NO_TEXT
} oskInputFieldResult;

typedef enum
{
	OSK_DEVICE_PAD,
	OSK_DEVICE_KB
} oskInputDevice;

typedef enum
{
	OSK_CONTINUOUS_MODE_NONE,
	OSK_CONTINUOUS_MODE_REMAIN_OPEN,
	OSK_CONTINUOUS_MODE_HIDE,
	OSK_CONTINUOUS_MODE_SHOW
} oskContinuousMode;

typedef enum
{
	OSK_SINGLELINE,
	OSK_MULTILINE,
	OSK_FULLKB_SINGLELINE,
	OSK_FULLKB_MULTILINE,
	OSK_SEPARATE_SINGLELINE_WINDOWED,
	OSK_SEPARATE_MULTILINE_WINDOWED,
	OSK_SEPARATE_INPUT_PANEL_WINDOWED,
	OSK_SEPARATE_FULLKB_INPUT_PANEL_WINDOWED,
	OSK_SEPARATE_CANDIDATE_WINDOWED
} oskType;

typedef struct _osk_pnt
{
	f32 x;
	f32 y;
} oskPoint;

typedef struct _osk_param
{
	u32 allowedPanels;
	u32 firstViewPanel;
	oskPoint controlPoint;
	s32 prohibitFlags;
} oskParam;

typedef struct _osk_layout_info
{
	s32 layoutMode;
	oskPoint position;
} oskLayoutInfo;

typedef struct _osk_input_field_info
{
	u16 *message;
	u16 *startText;
	s32 maxLength;
} oskInputFieldInfo;

typedef struct _osk_cb_return_param
{
	oskInputFieldResult res;
	s32 len;
	u16 *str;
} oskCallbackReturnParam;

typedef struct _osk_separate_window_option
{
	oskContinuousMode continuous;
	s32 mask;
	s32 inputWidth;
	f32 inputFieldTransparency;
	oskLayoutInfo *inputFieldLayoutInfo;
	oskLayoutInfo *inputPanelLayoutInfo;
	u32 reserved;
} oskSeparateWindowOption;

s32 oskSetInitialInputDevice(oskInputDevice input);
s32 oskGetSize(u16 *width, u16 *height, oskType type);
s32 oskUnloadAsync(oskCallbackReturnParam *param);
s32 oskDisableDimmer();
s32 oskSetKeyLayoutOption(u32 flags);
s32 oskAbort();
s32 oskSetDeviceMask(u32 mask);
s32 oskSetSeparateWindowOption(oskSeparateWindowOption *option);
s32 oskGetInputText(oskCallbackReturnParam *info);
s32 oskAddSupportLanguage(u32 lang);
s32 oskLoadAsync(mem_container_t container, const oskParam *param, const oskInputFieldInfo *inputInfo);
s32 oskSetInitialKeyLayout(oskInitialKeyLayout layout);
s32 oskSetLayoutMode (s32 mode);

#ifdef __cplusplus
	}
#endif

#endif
