/*! \file sysutil.h
\brief Miscellaneous system utility functions.
*/

#ifndef __LV2_SYSUTIL_H__
#define __LV2_SYSUTIL_H__

#include <ppu-types.h>

/*! \brief Return value */
#define SYSUTIL_OK                              0

/*! \brief Event slot 0 */
#define SYSUTIL_EVENT_SLOT0			0
/*! \brief Event slot 1 */
#define SYSUTIL_EVENT_SLOT1			1
/*! \brief Event slot 2 */
#define SYSUTIL_EVENT_SLOT2			2
/*! \brief Event slot 3 */
#define SYSUTIL_EVENT_SLOT3			3

/*! \brief Exit game requested. */
#define SYSUTIL_EXIT_GAME			0x0101
/*! \brief Beginning of XMB menu drawing. */
#define SYSUTIL_DRAW_BEGIN			0x0121
/*! \brief End of XMB menu drawing. */
#define SYSUTIL_DRAW_END			0x0122
/*! \brief XMB menu has been opened. */
#define SYSUTIL_MENU_OPEN			0x0131
/*! \brief XMB menu has been closed. */
#define SYSUTIL_MENU_CLOSE			0x0132
/*! \brief On-screen keyboard has been loaded. */
#define SYSUTIL_OSK_LOADED                      0x0502
/*! \brief On-screen keyboard has finished a user entry. */
#define SYSUTIL_OSK_DONE                        0x0503
/*! \brief On-screen keyboard has been unloaded. */
#define SYSUTIL_OSK_UNLOADED                    0x0504
/*! \brief On-screen keyboard has canceled input. */
#define SYSUTIL_OSK_INPUT_CANCELED              0x0506

#define SYSUTIL_SYSTEMPARAM_ID_LANG                             0x0111
#define SYSUTIL_SYSTEMPARAM_ID_ENTER_BUTTON_ASSIGN              0x0112
#define SYSUTIL_SYSTEMPARAM_ID_NICKNAME                         0x0113
#define SYSUTIL_SYSTEMPARAM_ID_DATE_FORMAT                      0x0114
#define SYSUTIL_SYSTEMPARAM_ID_TIME_FORMAT                      0x0115
#define SYSUTIL_SYSTEMPARAM_ID_TIMEZONE                         0x0116
#define SYSUTIL_SYSTEMPARAM_ID_SUMMERTIME                       0x0117
#define SYSUTIL_SYSTEMPARAM_ID_GAME_PARENTAL_LEVEL              0x0121
#define SYSUTIL_SYSTEMPARAM_ID_GAME_PARENTAL_LEVEL0_RESTRICT    0x0123
#define SYSUTIL_SYSTEMPARAM_ID_INTERNET_BROWSER_START_RESTRICT  0x0125
#define SYSUTIL_SYSTEMPARAM_ID_CURRENT_USERNAME                 0x0131
#define SYSUTIL_SYSTEMPARAM_ID_CURRENT_USER_HAS_NP_ACCOUNT      0x0141
#define SYSUTIL_SYSTEMPARAM_ID_CAMERA_PLFREQ                    0x0151
#define SYSUTIL_SYSTEMPARAM_ID_PAD_RUMBLE                       0x0152
#define SYSUTIL_SYSTEMPARAM_ID_KEYBOARD_TYPE                    0x0153
#define SYSUTIL_SYSTEMPARAM_ID_JAPANESE_KEYBOARD_ENTRY_METHOD   0x0154
#define SYSUTIL_SYSTEMPARAM_ID_CHINESE_KEYBOARD_ENTRY_METHOD    0x0155
#define SYSUTIL_SYSTEMPARAM_ID_PAD_AUTOOFF                      0x0156
#define SYSUTIL_SYSTEMPARAM_ID_MAGNETOMETER                     0x0157

#define SYSUTIL_SYSTEMPARAM_NICKNAME_SIZE                       128
#define SYSUTIL_SYSTEMPARAM_CURRENT_USERNAME_SIZE               64

#define SYSUTIL_LANG_JAPANESE           0   // Japanese 
#define SYSUTIL_LANG_ENGLISH_US         1   // English (United States) 
#define SYSUTIL_LANG_FRENCH             2   // French 
#define SYSUTIL_LANG_SPANISH            3   // Spanish 
#define SYSUTIL_LANG_GERMAN             4   // German 
#define SYSUTIL_LANG_ITALIAN            5   // Italian 
#define SYSUTIL_LANG_DUTCH              6   // Dutch 
#define SYSUTIL_LANG_PORTUGUESE_PT      7   // Portuguese (Portugal) 
#define SYSUTIL_LANG_RUSSIAN            8   // Russian 
#define SYSUTIL_LANG_KOREAN             9   // Korean 
#define SYSUTIL_LANG_CHINESE_T          10  // Chinese (traditional) 
#define SYSUTIL_LANG_CHINESE_S          11  // Chinese (simplified) 
#define SYSUTIL_LANG_FINNISH            12  // Finnish 
#define SYSUTIL_LANG_SWEDISH            13  // Swedish 
#define SYSUTIL_LANG_DANISH             14  // Danish 
#define SYSUTIL_LANG_NORWEGIAN          15  // Norwegian 
#define SYSUTIL_LANG_POLISH             16  // Polish 
#define SYSUTIL_LANG_PORTUGUESE_BR      17  // Portuguese (Brazil) 
#define SYSUTIL_LANG_ENGLISH_GB         18  // English (United Kingdom) 
#define SYSUTIL_LANG_TURKISH            19  // Turkish 


#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Pointer to callback function.

  \param status Event value. Possible values are:
    - \ref SYSUTIL_EXIT_GAME
    - \ref SYSUTIL_DRAW_BEGIN
    - \ref SYSUTIL_DRAW_END
    - \ref SYSUTIL_MENU_OPEN
    - \ref SYSUTIL_MENU_CLOSE
    - \ref SYSUTIL_OSK_LOADED
    - \ref SYSUTIL_OSK_DONE
    - \ref SYSUTIL_OSK_UNLOADED
    - \ref SYSUTIL_OSK_INPUT_CANCELED
  \param param Event parameter, depending on event value.
  \param usrdata Pointer to user data as provided to the
      \ref sysUtilRegisterCallback function.
*/
typedef void (*sysutilCallback)(u64 status,u64 param,void *usrdata);

/*! \brief Check for events.

This function usually is called in the main loop of a program, to regularly
check for new events. In case of an event, the callback function (previously
registered using \ref sysUtilRegisterCallback) is called.
*/
s32 sysUtilCheckCallback();


/*! \brief Unregister a callback function.

This function usually is called at the end of a program to unregister the
event manager.
*/
s32 sysUtilUnregisterCallback(s32 slot);

/*! \brief Register a callback function for system events.

This function usually is called at the beginning of a program to set up an
event manager. There can be up to 4 event managers installed at the same
time, by placing them in different slots. All system events are delivered
to all 4 slots in order, so each manager will see all of them.

  \param slot Event slot identifier. Possible values are:
    - \ref SYSUTIL_EVENT_SLOT0
    - \ref SYSUTIL_EVENT_SLOT1
    - \ref SYSUTIL_EVENT_SLOT2
    - \ref SYSUTIL_EVENT_SLOT3
  \param cb Pointer to callback function.
  \param usrdata Pointer to user data, to be sent to callback function.
      May be \c NULL.
*/
s32 sysUtilRegisterCallback(s32 slot,sysutilCallback cb,void *usrdata);

s32 sysUtilGetSystemParamInt(s32 id,s32 *value);
s32 sysUtilGetSystemParamString(s32 id,char *buf,u32 bufsize);

#ifdef __cplusplus
	}
#endif

#endif
