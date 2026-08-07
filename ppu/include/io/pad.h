/*! \file pad.h
    \brief PS3 pad/controller management.
*/

#ifndef __LV2_PAD_H__
#define __LV2_PAD_H__

#include <ppu-types.h>

#define CELL_MAX_PADS                                (127)     /*!< maximum controllers */
#define CELL_MAX_PAD_CODES                           (64)      /*!< maximum controller buttons */
#define CELL_MAX_PORT_NUM                            (7)       /*!< controller port number */
#define CELL_MAX_PAD_CAPABILITY_INFO                 (32)      /*!< controller capability information */
#define CELL_MAX_PAD_ACTUATOR                        (2)       /*!< controller vibrator actuator */

#define CELL_PAD_TYPE_STANDARD                       (0)       /*!< standard controller */
#define CELL_PAD_TYPE_REMOTE                         (4)       /*!< BD Remote Controller */
#define CELL_PAD_TYPE_LDD                            (5)       /*!< Custom Controller */

#define CELL_PAD_PRESS_MODE_ON						(1)
#define CELL_PAD_PRESS_MODE_OFF						(0)

#define CELL_PAD_INFO_SUPPORTED_PRESS_MODE			(1)

#define CELL_PAD_SENSOR_MODE_ON						(1)
#define CELL_PAD_SENSOR_MODE_OFF						(0)

#define CELL_PAD_INFO_SUPPORTED_SENSOR_MODE			(1)

#define CELL_PAD_SETTINGS_PRESS_ON                   (1<<1)    
#define CELL_PAD_SETTINGS_PRESS_OFF                  (0)    
#define CELL_PAD_SETTINGS_SENSOR_ON                  (1<<2)    
#define CELL_PAD_SETTINGS_SENSOR_OFF                 (0)    

// CELL_PAD_BUTTON_OFFSET_DIGITAL1 assignment
#define CELL_PAD_CTRL_LEFT                           (1<<7)
#define CELL_PAD_CTRL_DOWN                           (1<<6)
#define CELL_PAD_CTRL_RIGHT                          (1<<5)
#define CELL_PAD_CTRL_UP                             (1<<4)
#define CELL_PAD_CTRL_START                          (1<<3)
#define CELL_PAD_CTRL_R3                             (1<<2)
#define CELL_PAD_CTRL_L3                             (1<<1)
#define CELL_PAD_CTRL_SELECT                         (1<<0)

// CELL_PAD_BUTTON_OFFSET_DIGITAL2 assignment
#define CELL_PAD_CTRL_SQUARE                         (1<<7)
#define CELL_PAD_CTRL_CROSS                          (1<<6)
#define CELL_PAD_CTRL_CIRCLE                         (1<<5)
#define CELL_PAD_CTRL_TRIANGLE                       (1<<4)
#define CELL_PAD_CTRL_R1                             (1<<3)
#define CELL_PAD_CTRL_L1                             (1<<2)
#define CELL_PAD_CTRL_R2                             (1<<1)
#define CELL_PAD_CTRL_L2                             (1<<0)

#define CELL_PAD_OK                                  0
#define CELL_PAD_ERROR_FATAL                         0x80121101
#define CELL_PAD_ERROR_INVALID_PARAMETER             0x80121102
#define CELL_PAD_ERROR_ALREADY_INITIALIZED           0x80121103
#define CELL_PAD_ERROR_NOT_INITIALIZED               0x80121104

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Pad info data structure (old format). */
typedef struct _pad_info
{
    u32 max;                                /*!< \brief max pads allowed to connect */
    u32 connected;                          /*!< \brief how many pads connected */
    u32 info;                               /*!< \brief bit 0 lets the system intercept pad? other bits are reserved */
    u16 vendor_id[CELL_MAX_PADS];                /*!< \brief array of vendor ids */
    u16 product_id[CELL_MAX_PADS];               /*!< \brief array of product ids */
    u8 status[CELL_MAX_PADS];                    /*!< \brief array of pad statuses */
} CellPadInfo;

/*! \brief Pad info data structure (new format). */
typedef struct _pad_info2
{
    u32 max;                                /*!< \brief max pads allowed to connect */
    u32 connected;                          /*!< \brief how many pads connected */
    u32 info;                               /*!< \brief Bit 0 lets the system intercept pad? other bits are reserved */
    u32 port_status[CELL_MAX_PORT_NUM];          /*!< \brief Bit 0: Connected (0: Disconnected, 1: Connected), Bit 1: assign changes? */
    u32 port_setting[CELL_MAX_PORT_NUM];         /*!< \brief Bit 1: Pressure sensitivity turned on, Bit 2: Sensors turned on */
    u32 device_capability[CELL_MAX_PORT_NUM];    /*!< \brief See: PadCapabilityInfo. Bit 0: PS3SPEC, Bit 1: has_pressure, Bit 2: has_sensor, Bit 3: has_hps, Bit 4: has_vibrate */
    u32 device_type[CELL_MAX_PORT_NUM];          /*!< \brief 0: Standard, 4: Bluray Remote, 5: LDD */
} CellPadInfo2;

/*! \brief Pad data data structure.
    All 1-bit button fields are zero when inctive, and 1 when active.
*/
typedef struct _pad_data
{
    s32 len;                                /*!< \brief structure length */
    union{
        u16 button[CELL_MAX_PAD_CODES];          /*!< \brief array of button values */
        struct {
            u16 zeroes;                     /*!< \brief zero bits */
            unsigned int : 8;               /*!< \brief reserved */
            unsigned int seven : 4;         /*!< \brief always 0x7 */
            unsigned int halflen : 4;       /*!< \brief len/2 */
            unsigned int : 8;               /*!< \brief reserved */
            /* Button information */
            /* 0: UP, 1: DOWN */
            unsigned int BTN_LEFT : 1;      /*!< \brief left button */
            unsigned int BTN_DOWN : 1;      /*!< \brief down button */
            unsigned int BTN_RIGHT : 1;     /*!< \brief right button */
            unsigned int BTN_UP : 1;        /*!< \brief up button */
            unsigned int BTN_START : 1;     /*!< \brief start button */
            unsigned int BTN_R3 : 1;        /*!< \brief R3 button */
            unsigned int BTN_L3 : 1;        /*!< \brief L3 button */
            unsigned int BTN_SELECT : 1;    /*!< \brief select button */

            unsigned int : 8;               /*!< \brief reserved */
            unsigned int BTN_SQUARE : 1;    /*!< \brief square button */
            unsigned int BTN_CROSS : 1;     /*!< \brief cross button */
            unsigned int BTN_CIRCLE : 1;    /*!< \brief circle button */
            unsigned int BTN_TRIANGLE : 1;  /*!< \brief triangle button */
            unsigned int BTN_R1 : 1;        /*!< \brief R1 button */
            unsigned int BTN_L1 : 1;        /*!< \brief L1 button */
            unsigned int BTN_R2 : 1;        /*!< \brief R2 button */
            unsigned int BTN_L2 : 1;        /*!< \brief L2 button */

            /* Analog nub information */
            /* 0x0000 - 0x00FF */
            /*! \brief right analog nub horizontal position (\c 0x0000 - \c 0x00FF) */
            unsigned int ANA_R_H : 16;
            /*! \brief right analog nub vertical position (\c 0x0000 - \c 0x00FF) */
            unsigned int ANA_R_V : 16;
            /*! \brief left analog nub horizontal position (\c 0x0000 - \c 0x00FF) */
            unsigned int ANA_L_H : 16;
            /*! \brief left analog nub vertical position (\c 0x0000 - \c 0x00FF) */
            unsigned int ANA_L_V : 16;

            /* Pressure-sensitive information */
            /* 0x0000 - 0x00FF */
            /*! \brief right button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_RIGHT : 16;
            /*! \brief left button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_LEFT : 16;
            /*! \brief up button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_UP : 16;
            /*! \brief down button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_DOWN : 16;
            /*! \brief triangle button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_TRIANGLE : 16;
            /*! \brief circle button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_CIRCLE : 16;
            /*! \brief cross button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_CROSS : 16;
            /*! \brief square button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_SQUARE : 16;
            /*! \brief L1 button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_L1 : 16;
            /*! \brief R1 button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_R1 : 16;
            /*! \brief L2 button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_L2 : 16;
            /*! \brief R2 button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_R2 : 16;

            /* Sensor information */
            /* 0x0000 - 0x03FF */
            /*! \brief X sensor information (\c 0x0000 - \c 0x03FF) */
            unsigned int SENSOR_X : 16;
            /*! \brief Y sensor information (\c 0x0000 - \c 0x03FF) */
            unsigned int SENSOR_Y : 16;
            /*! \brief Z sensor information (\c 0x0000 - \c 0x03FF) */
            unsigned int SENSOR_Z : 16;
            /*! \brief G sensor information (\c 0x0000 - \c 0x03FF) */
            unsigned int SENSOR_G : 16;

            /* BD remote information */
            unsigned int BTN_BDLEN : 16;    /*! \brief BD remote len information */
            /*! 0x0000 - 0xffff */
            unsigned int BTN_BDCODE : 16;   /*! \brief BD remote code information see ioPadBdCode for values */

            /*! \brief reserved */
            u8 reserved[76];
        };
    };
} CellPadData;

/*! \brief Pad capability information data structure.
    All 1-bit values are 1 when the capability is present, 0 otherwise.
*/
typedef struct _pad_capability_info
{
    union {
        /*! \brief The whole capabilities in an array of 32-bit integers. */
        u32 info[CELL_MAX_PAD_CAPABILITY_INFO];
        struct {
            unsigned int ps3spec : 1;       /*!< \brief controller meets ps3 specifications */
            unsigned int has_pressure : 1;  /*!< \brief controller has pressure-sensitive buttons */
            unsigned int has_sensors : 1;   /*!< \brief controller has sensors, e.g. sixaxis, X,Y,Z,G */
            unsigned int has_hps : 1;       /*!< \brief controller has high precision stick */
            unsigned int has_vibrate : 1;   /*!< \brief controller has inbuilt vibration motor */
            unsigned int : 27;              /*!< \brief reserved */
            u32 reserved[CELL_MAX_PAD_CAPABILITY_INFO-1];  /*!< \brief reserved */
        };
    };
} CellPadCapabilityInfo;

/*! \brief Pad action parameters data structure. */
typedef struct _pad_act_param
{
    union {
        /*! \brief All motors statuses. */
        u8 motor[CELL_MAX_PAD_ACTUATOR];
        struct {
            /*! \brief Small Motor parameter 0: OFF, 1: On */
            u8 small_motor;
            /*! \brief Large Motor 0: OFF, 1-255: Motor Speed, higher == faster. */
            u8 large_motor;
        };
    };
    /*! \brief reserved */
    u8 reserved[6];
} CellPadActParam;

/*! \brief Pad peripheral information data structure. */
typedef struct _pad_periph_info
{
    u32 max;                                /*!< \brief max pads allowed to connect */
    u32 connected;                          /*!< \brief how many pads connected */
    u32 info;                               /*!< \brief bit 0 lets the system intercept pad? other bits are reserved */
    u32 port_status[CELL_MAX_PORT_NUM];          /*!< \brief Bit 0: Connected (0: Disconnected, 1: Connected), Bit 1: assign changes? */
    u32 port_setting[CELL_MAX_PORT_NUM];         /*!< \brief Bit 1: Pressure sensitivity turned on, Bit 2: Sensors turned on */
    /*! \brief Pad capability.
        - Bit 0: PS3SPEC
        - Bit 1: has_pressure
        - Bit 2: has_sensor
        - Bit 3: has_hps
        - Bit 4: has_vibrate
        \see \ref padCapabilityInfo
    */
    u32 device_capability[CELL_MAX_PORT_NUM];
    u32 device_type[CELL_MAX_PORT_NUM];          /*!< \brief 0: Standard, 4: Bluray Remote, 5: LDD */
    u32 pclass_type[CELL_MAX_PORT_NUM];          /*!< \brief Pad class type */
    u32 pclass_profile[CELL_MAX_PORT_NUM];       /*!< \brief Pad class profile */
} CellPadPeriphInfo;

/*! \brief Pad peripheral data data structure. */
typedef struct _pad_periph_data
{
    u32 pclass_type;                        /*!< \brief Pad class type */
    u32 pclass_profile;                     /*!< \brief Pas class profile */
    s32 len;                                /*!< \brief Length of button array */
    u16 button[CELL_MAX_PAD_CODES];              /*!< \brief Button array */
} CellPadPeriphData;

/*! \brief BD remote button codes */
typedef enum _io_pad_bd_code
{
    CELL_BTN_BD_KEY_1           = 0x00,
    CELL_BTN_BD_KEY_2           = 0x01,
    CELL_BTN_BD_KEY_3           = 0x02,
    CELL_BTN_BD_KEY_4           = 0x03,
    CELL_BTN_BD_KEY_5           = 0x04,
    CELL_BTN_BD_KEY_6           = 0x05,
    CELL_BTN_BD_KEY_7           = 0x06,
    CELL_BTN_BD_KEY_8           = 0x07,
    CELL_BTN_BD_KEY_9           = 0x08,
    CELL_BTN_BD_KEY_0           = 0x09,
    CELL_BTN_BD_ENTER           = 0x0b,
    CELL_BTN_BD_RETURN          = 0x0e,
    CELL_BTN_BD_CLEAR           = 0x0f,
    CELL_BTN_BD_EJECT           = 0x16,
    CELL_BTN_BD_TOPMENU         = 0x1a,
    CELL_BTN_BD_TIME            = 0x28,
    CELL_BTN_BD_PREV            = 0x30,
    CELL_BTN_BD_NEXT            = 0x31,
    CELL_BTN_BD_PLAY            = 0x32,
    CELL_BTN_BD_SCAN_REV        = 0x33,
    CELL_BTN_BD_SCAN_FWD        = 0x34,
    CELL_BTN_BD_STOP            = 0x38,
    CELL_BTN_BD_PAUSE           = 0x39,
    CELL_BTN_BD_POPUP_MENU      = 0x40,
    CELL_BTN_BD_SELECT          = 0x50,
    CELL_BTN_BD_L3              = 0x51,
    CELL_BTN_BD_R3              = 0x52,
    CELL_BTN_BD_START           = 0x53,
    CELL_BTN_BD_UP              = 0x54,
    CELL_BTN_BD_RIGHT           = 0x55,
    CELL_BTN_BD_DOWN            = 0x56,
    CELL_BTN_BD_LEFT            = 0x57,
    CELL_BTN_BD_L2              = 0x58,
    CELL_BTN_BD_R2              = 0x59,
    CELL_BTN_BD_L1              = 0x5a,
    CELL_BTN_BD_R1              = 0x5b,
    CELL_BTN_BD_TRIANGLE        = 0x5c,
    CELL_BTN_BD_CIRCLE          = 0x5d,
    CELL_BTN_BD_CROSS           = 0x5e,
    CELL_BTN_BD_SQUARE          = 0x5f,
    CELL_BTN_BD_SLOW_REV        = 0x60,
    CELL_BTN_BD_SLOW_FWD        = 0x61,
    CELL_BTN_BD_SUBTITLE        = 0x63,
    CELL_BTN_BD_AUDIO           = 0x64,
    CELL_BTN_BD_ANGLE           = 0x65,
    CELL_BTN_BD_DISPLAY         = 0x70,
    CELL_BTN_BD_BLUE            = 0x80,
    CELL_BTN_BD_RED             = 0x81,
    CELL_BTN_BD_GREEN           = 0x82,
    CELL_BTN_BD_YELLOW          = 0x83,
    CELL_BTN_BD_RELEASE         = 0xff,

    /* TV controller */
    CELL_BTN_BD_NUMBER_11       = 0x101e,
    CELL_BTN_BD_NUMBER_12       = 0x101f,
    CELL_BTN_BD_NUMBER_PERIOD   = 0x102a,
    CELL_BTN_BD_PROGRAM_UP      = 0x1030,
    CELL_BTN_BD_PROGRAM_DOWN    = 0x1031,
    CELL_BTN_BD_PREV_CHANNEL    = 0x1032,
    CELL_BTN_BD_PROGRAM_GUIDE   = 0x1053
} CellPadBdCode;

typedef enum _io_pad_button_data_offset
{
    CELL_PAD_BUTTON_OFFSET_DIGITAL1          = 2,
    CELL_PAD_BUTTON_OFFSET_DIGITAL2          = 3,
    CELL_PAD_BUTTON_OFFSET_ANALOG_RIGHT_X    = 4,
    CELL_PAD_BUTTON_OFFSET_ANALOG_RIGHT_Y    = 5,
    CELL_PAD_BUTTON_OFFSET_ANALOG_LEFT_X     = 6,
    CELL_PAD_BUTTON_OFFSET_ANALOG_LEFT_Y     = 7,
    CELL_PAD_BUTTON_OFFSET_PRESS_RIGHT       = 8,
    CELL_PAD_BUTTON_OFFSET_PRESS_LEFT        = 9,
    CELL_PAD_BUTTON_OFFSET_PRESS_UP          = 10,
    CELL_PAD_BUTTON_OFFSET_PRESS_DOWN        = 11,
    CELL_PAD_BUTTON_OFFSET_PRESS_TRIANGLE    = 12,
    CELL_PAD_BUTTON_OFFSET_PRESS_CIRCLE      = 13,
    CELL_PAD_BUTTON_OFFSET_PRESS_CROSS       = 14,
    CELL_PAD_BUTTON_OFFSET_PRESS_SQUARE      = 15,
    CELL_PAD_BUTTON_OFFSET_PRESS_L1          = 16,
    CELL_PAD_BUTTON_OFFSET_PRESS_R1          = 17,
    CELL_PAD_BUTTON_OFFSET_PRESS_L2          = 18,
    CELL_PAD_BUTTON_OFFSET_PRESS_R2          = 19,
    CELL_PAD_BUTTON_OFFSET_SENSOR_X          = 20,
    CELL_PAD_BUTTON_OFFSET_SENSOR_Y          = 21,
    CELL_PAD_BUTTON_OFFSET_SENSOR_Z          = 22,
    CELL_PAD_BUTTON_OFFSET_SENSOR_G          = 23
} CellPadButtonDataOffset;

/*! \brief Initialize Pad IO management.
    \param max maximum number of supported pads.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadInit(u32 max);

/*! \brief End of Pad IO management.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadEnd();

/*! \brief Get pad information.
    \param info Pointer to the pad info structure.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadGetInfo(CellPadInfo* info);

/*! \brief Clear pad buffer.
    \param port Pad port.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadClearBuf(u32 port);

/*! \brief Get pad capability information.
    \param port Pad port.
    \param capabilities Pointer to the pad capability information structure.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadGetCapabilityInfo(u32 port, CellPadCapabilityInfo* capabilities);

/*! \brief Get pad data.
    \param port Pad port.
    \param data Pointer to the pad data structure.
    \return zero if no error occured, nonzero otherwise.

    \note The CellPadData structure is only filled if there is a change in the input since the last call.
    If there is no change, the structure is zero filled. If the member 'len' is zero, it indicates 
    that there was no new input.
*/
s32 cellPadGetData(u32 port, CellPadData* data);

/*! \brief Get pad data.
    \param port Pad port.
    \param type Pointer to the device type.
    \param data Pointer to the pad data structure.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadGetDataExtra(u32 port, u32* type, CellPadData* data);

/*! \brief Get pad press mode.
    \param port Pad port.
    \return The current press mode value, or a negative value in case of error.
*/
s32 cellPadInfoPressMode(u32 port);

/*! \brief Set pad press mode.
    \param port Pad port.
    \param mode The selected press mode.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadSetPressMode(u32 port, u32 mode);

/*! \brief Get pad sensor mode.
    \param port Pad port.
    \return The current sensor mode value, or a negative value in case of error.
*/
s32 cellPadInfoSensorMode(u32 port);

/*! \brief Set pad sensor mode.
    \param port Pad port.
    \param mode The selected sensor mode.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadSetSensorMode(u32 port, u32 mode);

/*! \brief Set pad action (rumble) data.
    \param port Pad port.
    \param actParam Pointer to the action data structure.
    \return zero if no error occured, nonzero otherwise.
*/
u32 cellPadSetActDirect(u32 port, CellPadActParam* actParam);

/*! \brief Insert pad data into virtual pad.
    \param handle The registered handle for the virtual pad.
    \param CellPadData Pointer to the pad data value.
    \return zero if no error occured, nonzero otherwise.
    \todo to be confirmed.
*/
u32 cellPadLddDataInsert(s32 handle, CellPadData* data);

/*! \brief Get port number from registered virtual pad handle.
    \param handle The registered handle for the virtual pad.
    \return The port number for the virtual pad, or a negative value in case of error.
*/
s32 cellPadLddGetPortNo(s32 handle);

/*! \brief Register a new virtual pad.
    \return The handle for the virtual pad, or a negative value in case of error.
*/
s32 cellPadLddRegisterController();

/*! \brief Unregister a virtual pad.
    \param handle The registered handle for the virtual pad.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadLddUnregisterController(s32 handle);

/* new in 3.41 */
/*! \brief Get pad information (new format).
    \param info Pointer to the pad info structure.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadGetInfo2(CellPadInfo2* info);

/*! \brief Get pad peripheral information.
    \param periphInfo Pointer to the pad peripheral info structure.
    \return zero if no error occured, nonzero otherwise.
*/
s32 cellPadPeriphGetInfo(CellPadPeriphInfo* periphInfo);

/*! \brief Get pad peripheral data.
    \param port Pad port.
    \param periphData Pointer to the pad peripheral data structure.
    \return zero if no error occured, nonzero otherwise.

    \note The CellPadPeriphData structure is only filled if there is a change in the input since the last call.
    If there is no change, the structure is partially updated. If the member 'len' is zero, it indicates 
    that there was no new input.
*/
s32 cellPadPeriphGetData(u32 port, CellPadPeriphData* periphData);

/*! \brief Set pad port setting.
    \param port Pad port.
    \param setting Setting value.
    \return zero if no error occured, nonzero otherwise.
    \todo confirm this..
*/
s32 cellPadSetPortSetting(u32 port, u32 setting);

#ifdef __cplusplus
    }
#endif

#endif
