/*! \file kb.h
\brief PS3 keyboard management.
*/

#pragma once

#include <ppu-types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CELL_MAX_KEYBOARDS	127
#define CELL_MAX_KB_PORT_NUM	7
#define CELL_MAX_KEYCODES	62

/* Raw Data Code and Keypad Code Data definition */
#define CELL_KB_RAWDAT	0x8000U
#define CELL_KB_KEYPAD	0x4000U
 
/* Keyboard RAWDAT Key code (can't be converted to ASCII codes) */
#define CELL_KB_RAWKEY_NO_EVENT				0x00
#define CELL_KB_RAWKEY_E_ROLLOVER			0x01
#define CELL_KB_RAWKEY_E_POSTFAIL			0x02
#define CELL_KB_RAWKEY_E_UNDEF				0x03
#define CELL_KB_RAWKEY_ESCAPE				0x29
#define CELL_KB_RAWKEY_106_KANJI			0x35	/* The half-width/full width Kanji key code */
#define CELL_KB_RAWKEY_CAPS_LOCK			0x39
#define CELL_KB_RAWKEY_F1					0x3a
#define CELL_KB_RAWKEY_F2					0x3b
#define CELL_KB_RAWKEY_F3					0x3c
#define CELL_KB_RAWKEY_F4					0x3d
#define CELL_KB_RAWKEY_F5					0x3e
#define CELL_KB_RAWKEY_F6					0x3f
#define CELL_KB_RAWKEY_F7					0x40
#define CELL_KB_RAWKEY_F8					0x41
#define CELL_KB_RAWKEY_F9					0x42
#define CELL_KB_RAWKEY_F10					0x43
#define CELL_KB_RAWKEY_F11					0x44
#define CELL_KB_RAWKEY_F12					0x45
#define CELL_KB_RAWKEY_PRINTSCREEN			0x46
#define CELL_KB_RAWKEY_SCROLL_LOCK			0x47
#define CELL_KB_RAWKEY_PAUSE				0x48
#define CELL_KB_RAWKEY_INSERT				0x49
#define CELL_KB_RAWKEY_HOME					0x4a
#define CELL_KB_RAWKEY_PAGE_UP				0x4b
#define CELL_KB_RAWKEY_DELETE				0x4c
#define CELL_KB_RAWKEY_END					0x4d
#define CELL_KB_RAWKEY_PAGE_DOWN			0x4e
#define CELL_KB_RAWKEY_RIGHT_ARROW			0x4f
#define CELL_KB_RAWKEY_LEFT_ARROW			0x50
#define CELL_KB_RAWKEY_DOWN_ARROW			0x51
#define CELL_KB_RAWKEY_UP_ARROW				0x52
#define CELL_KB_RAWKEY_NUM_LOCK				0x53
#define CELL_KB_RAWKEY_APPLICATION			0x65	/* Application key code */
#define CELL_KB_RAWKEY_KANA					0x88	/* Katakana/Hiragana/Romaji key code */
#define CELL_KB_RAWKEY_HENKAN				0x8a	/* Conversion key code */
#define CELL_KB_RAWKEY_MUHENKAN				0x8b	/* No Conversion key code */

/* Keyboard RAW Key Code definition */
#define CELL_KB_RAWKEY_A						0x04
#define CELL_KB_RAWKEY_B						0x05
#define CELL_KB_RAWKEY_C						0x06
#define CELL_KB_RAWKEY_D						0x07
#define CELL_KB_RAWKEY_E						0x08
#define CELL_KB_RAWKEY_F						0x09
#define CELL_KB_RAWKEY_G						0x0A
#define CELL_KB_RAWKEY_H						0x0B
#define CELL_KB_RAWKEY_I						0x0C
#define CELL_KB_RAWKEY_J						0x0D
#define CELL_KB_RAWKEY_K						0x0E
#define CELL_KB_RAWKEY_L						0x0F
#define CELL_KB_RAWKEY_M						0x10
#define CELL_KB_RAWKEY_N						0x11
#define CELL_KB_RAWKEY_O						0x12
#define CELL_KB_RAWKEY_P						0x13
#define CELL_KB_RAWKEY_Q						0x14
#define CELL_KB_RAWKEY_R						0x15
#define CELL_KB_RAWKEY_S						0x16
#define CELL_KB_RAWKEY_T						0x17
#define CELL_KB_RAWKEY_U						0x18
#define CELL_KB_RAWKEY_V						0x19
#define CELL_KB_RAWKEY_W						0x1A
#define CELL_KB_RAWKEY_X						0x1B
#define CELL_KB_RAWKEY_Y						0x1C
#define CELL_KB_RAWKEY_Z						0x1D
#define CELL_KB_RAWKEY_1						0x1E
#define CELL_KB_RAWKEY_2						0x1F
#define CELL_KB_RAWKEY_3						0x20
#define CELL_KB_RAWKEY_4						0x21
#define CELL_KB_RAWKEY_5						0x22
#define CELL_KB_RAWKEY_6						0x23
#define CELL_KB_RAWKEY_7						0x24
#define CELL_KB_RAWKEY_8						0x25
#define CELL_KB_RAWKEY_9						0x26
#define CELL_KB_RAWKEY_0						0x27
#define CELL_KB_RAWKEY_ENTER					0x28
#define CELL_KB_RAWKEY_ESC					0x29
#define CELL_KB_RAWKEY_BS					0x2A
#define CELL_KB_RAWKEY_TAB					0x2B
#define CELL_KB_RAWKEY_SPACE					0x2C
#define CELL_KB_RAWKEY_MINUS					0x2D
#define CELL_KB_RAWKEY_EQUAL_101				0x2E	/* = and + */
#define CELL_KB_RAWKEY_ACCENT_CIRCONFLEX_106 0x2E	/* ^ and ~ */
#define CELL_KB_RAWKEY_LEFT_BRACKET_101		0x2F	/* [ */
#define CELL_KB_RAWKEY_ATMARK_106			0x2F	/* @ */
#define CELL_KB_RAWKEY_RIGHT_BRACKET_101		0x30	/* ] */
#define CELL_KB_RAWKEY_LEFT_BRACKET_106		0x30	/* [ */
#define CELL_KB_RAWKEY_BACKSLASH_101			0x31	/* \ and | */
#define CELL_KB_RAWKEY_RIGHT_BRACKET_106		0x32	/* ] */
#define CELL_KB_RAWKEY_SEMICOLON				0x33	/* ; */
#define CELL_KB_RAWKEY_QUOTATION_101			0x34	/* ' and " */
#define CELL_KB_RAWKEY_COLON_106				0x34	/* : and * */
#define CELL_KB_RAWKEY_COMMA		    		0x36
#define CELL_KB_RAWKEY_PERIOD				0x37
#define CELL_KB_RAWKEY_SLASH		    		0x38
#define CELL_KB_RAWKEY_CAPS_LOCK				0x39
#define CELL_KB_RAWKEY_KPAD_NUMLOCK			0x53
#define CELL_KB_RAWKEY_KPAD_SLASH			0x54
#define CELL_KB_RAWKEY_KPAD_ASTERISK			0x55
#define CELL_KB_RAWKEY_KPAD_MINUS			0x56
#define CELL_KB_RAWKEY_KPAD_PLUS				0x57
#define CELL_KB_RAWKEY_KPAD_ENTER			0x58
#define CELL_KB_RAWKEY_KPAD_1				0x59
#define CELL_KB_RAWKEY_KPAD_2				0x5A
#define CELL_KB_RAWKEY_KPAD_3				0x5B
#define CELL_KB_RAWKEY_KPAD_4				0x5C
#define CELL_KB_RAWKEY_KPAD_5				0x5D
#define CELL_KB_RAWKEY_KPAD_6				0x5E
#define CELL_KB_RAWKEY_KPAD_7				0x5F
#define CELL_KB_RAWKEY_KPAD_8				0x60
#define CELL_KB_RAWKEY_KPAD_9				0x61
#define CELL_KB_RAWKEY_KPAD_0				0x62
#define CELL_KB_RAWKEY_KPAD_PERIOD			0x63
#define CELL_KB_RAWKEY_BACKSLASH_106			0x87
#define CELL_KB_RAWKEY_YEN_106				0x89

/*! \brief Keyboard Led State. */
typedef struct CellKbLed {
	union {
		u32 leds;
		struct {
		u32 reserved	: 27;	/*!< \brief Reserved MSB */
		u32 kana		: 1;	/*!< \brief LED Kana 0:OFF 1:ON Bit4 */
		u32 compose		: 1;	/*!< \brief LED Compose 0:OFF 1:ON Bit3 */
		u32 scroll_lock	: 1;	/*!< \brief LED Scroll Lock 0:OFF 1:ON Bit2 */
		u32 caps_lock	: 1;	/*!< \brief LED Caps Lock 0:OFF 1:ON Bit1 */
		u32 num_lock	: 1;	/*!< \brief LED Num Lock 0:OFF 1:ON Bit0 LSB */
		};
	};
} CellKbLed;

/*! \brief Keyboard Modifier Key State. */
typedef struct CellKbMkey {
	union {
		u32 mkeys;
		struct {
		u32 reserved	: 24;	/*!< \brief Reserved MSB */
		u32 r_win		: 1;	/*!< \brief Modifier Key Right WIN 0:OFF 1:ON Bit7 */
		u32 r_alt		: 1;	/*!< \brief Modifier Key Right ALT 0:OFF 1:ON Bit6 */
		u32 r_shift		: 1;	/*!< \brief Modifier Key Right SHIFT 0:OFF 1:ON Bit5 */		
		u32 r_ctrl		: 1;	/*!< \brief Modifier Key Right CTRL 0:OFF 1:ON Bit4 */
		u32 l_win		: 1;	/*!< \brief Modifier Key Left WIN 0:OFF 1:ON Bit3 */
		u32 l_alt		: 1;	/*!< \brief Modifier Key Left ALT 0:OFF 1:ON Bit2 */
		u32 l_shift		: 1;	/*!< \brief Modifier Key Left SHIFT 0:OFF 1:ON Bit1 */
		u32 l_ctrl		: 1;	/*!< \brief Modifier Key Left CTRL 0:OFF 1:ON Bit0 LSB */
		/* For Macintosh Keyboard ALT & WIN correspond respectively to OPTION & APPLE keys */
		};
	};
} CellKbMkey;

/*! \brief Keyboard input data data structure. */
typedef struct CellKbData {
	CellKbLed led;					/*!< \brief Keyboard Led State */
	CellKbMkey mkey;				/*!< \brief Keyboard Modifier Key State */
	s32 nb_keycode;				/*!< \brief Number of key codes (0 equal no data) */
	u16 keycode[CELL_MAX_KEYCODES];	/*!< \brief Keycode values */
} CellKbData;

/*! \brief Keyboard input mode values. */
typedef enum CellKbRmode {
	CELL_KB_RMODE_INPUTCHAR	= 0,	/*!< \brief Character Input Mode */
	CELL_KB_RMODE_PACKET		= 1		/*!< \brief Packet Mode */
} CellKbRmode;

/*! \brief Keyboard raw or ASCII mode values. */
typedef enum CellKbCodeType {
	CELL_KB_CODETYPE_RAW		= 0,	/*!< \brief Obtain device code as RAW code type */
	CELL_KB_CODETYPE_ASCII	= 1		/*!< \brief Obtain device code as ASCII code type */
} CellKbCodeType;

/*! \brief Keyboard mapping values. */
typedef enum CellKbMapping {
	CELL_KB_MAPPING_101							= 0,
	CELL_KB_MAPPING_106							= 1,
	CELL_KB_MAPPING_106_KANA					= 2,
	CELL_KB_MAPPING_GERMAN_GERMANY				= 3,
	CELL_KB_MAPPING_SPANISH_SPAIN				= 4,
	CELL_KB_MAPPING_FRENCH_FRANCE				= 5,
	CELL_KB_MAPPING_ITALIAN_ITALY				= 6,
	CELL_KB_MAPPING_DUTCH_NETHERLANDS			= 7,
	CELL_KB_MAPPING_PORTUGUESE_PORTUGAL			= 8,
	CELL_KB_MAPPING_RUSSIAN_RUSSIA				= 9,
	CELL_KB_MAPPING_ENGLISH_UK					= 10,
	CELL_KB_MAPPING_KOREAN_KOREA				= 11,
	CELL_KB_MAPPING_NORWEGIAN_NORWAY			= 12,
	CELL_KB_MAPPING_FINNISH_FINLAND				= 13,
	CELL_KB_MAPPING_DANISH_DENMARK				= 14,
	CELL_KB_MAPPING_SWEDISH_SWEDEN				= 15,
	CELL_KB_MAPPING_CHINESE_TRADITIONAL			= 16,
	CELL_KB_MAPPING_CHINESE_SIMPLIFIED			= 17,
	CELL_KB_MAPPING_SWISS_FRENCH_SWITZERLAND	= 18,
	CELL_KB_MAPPING_SWISS_GERMAN_SWITZERLAND	= 19,
	CELL_KB_MAPPING_CANADIAN_FRENCH_CANADA		= 20
} CellKbMapping;

/*! \brief Keyboard info data structure. */
typedef struct CellKbInfo {
	u32 max;					/*!< \brief max kb allowed to connect */
	u32 connected;				/*!< \brief how many kb connected */
	u32 info;					/*!< \brief Bit 0 lets the system intercept keyboard? other bits are reserved */
	u8 status[CELL_MAX_KEYBOARDS];	/*!< \brief Connections status, status Index equal to Keyboard Number
								   value 0: Not connected and 1: Connected */
} CellKbInfo;

/*! \brief Keyboard config data structure. */
typedef struct CellKbConfig {
	u32 mapping;	/*!< \brief See KbMapping enum definition */
	u32 rmode;		/*!< \brief See KbRmode enum definition */
	u32 codetype;	/*!< \brief See KbCodeType enum definition */
} CellKbConfig;

/*!
 * \brief Initialize Keyboard library
 *
 * \param[in] max maximum keyboard allowed to connect (max limit is 7)
 * \return 0 if OK or else error code
 */
s32 cellKbInit(const u32 max);

/*!
 * \brief Close/End Keyboard library
 *
 * \return 0 if OK or else error code
 */
s32 cellKbEnd(void);

/*!
 * \brief Read data from keyboard data buffer
 *
 * \param[in] kb_no keyboard port number
 * \param[out] data keyboard data
 * \return 0 if OK or else error code
 */
s32 cellKbRead(const u32 kb_no, CellKbData* data);

/*!
 * \brief Set Keyboard data read mode
 *
 * \param[in] kb_no keyboard port number
 * \param[in] kb_rmode keyboard read mode
 * \return 0 if OK or else error code
 */
s32 cellKbSetReadMode(const u32 kb_no, const CellKbRmode kb_rmode);

/*!
 * \brief Set Keyboard code type
 *
 * \param[in] kb_no keyboard port number
 * \param[in] kb_codetype keyboard code type setting
 * \return 0 if OK or else error code
 */
s32 cellKbSetCodeType(const u32 kb_no, const CellKbCodeType kb_codetype);

/*!
 * \brief Convert Keyboard device key code
 *
 * \param[in] mapping keyboard mapping
 * \param[in] mkey keyboard modifier key state
 * \param[in] led keyboard led state
 * \param[in] rawcode keyboard key code to convert
 * \return Converted keyboard code
 */
u16 cellKbCnvRawCode(const CellKbMapping mapping, const CellKbMkey mkey, const CellKbLed led, const u16 rawcode);

/*!
 * \brief Control Keyboard LED
 *
 * \param[in] kb_no keyboard port number
 * \param[in] led_state keyboard led state to set
 * \return 0 if OK or else error code
 */ 
s32 cellKbSetLEDStatus(const u32 kb_no, const CellKbLed led_state);

/*!
 * \brief Get Keyboard Information
 *
 * \param[in] kb_info keyboard connection info
 * \return 0 if OK or else error code
 */ 
s32 cellKbGetInfo(CellKbInfo* kb_info);

/*!
 * \brief Get Keyboard Configuration.
 *
 * \param[in] kb_no keyboard port number
 * \param[in] kb_config keyboard configuration
 * \return 0 if OK or else error code
 */ 
s32 cellKbGetConfiguration(const u32 kb_no, CellKbConfig* kb_config);

/*!
 * \brief Clear Keyboard Buffer Data.
 *
 * \param[in] kb_no keyboard port number
 * \return 0 if OK or else error code
 */ 
s32 cellKbClearBuf(const u32 kb_no);

#ifdef __cplusplus
	}
#endif

