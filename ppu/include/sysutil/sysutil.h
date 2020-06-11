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

#ifdef __cplusplus
	}
#endif

#endif
