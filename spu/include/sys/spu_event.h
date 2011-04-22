/*! \file spu_event.h
  \brief SPU runtime library.

  These are the SPU library calls for SPU events.

  \see \ref spu_man
*/

#ifndef __SPU_EVENT_H__
#define __SPU_EVENT_H__

#include <stdint.h>
#include <sys/cdefs.h>

#define EVENT_DATA0_MASK		0x00FFFFFF
#define EVENT_PORT_SHIFT		24
#define EVENT_PORT_MAX_NUM		63

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Send an SPU thread user event.

This function sends an SPU thread user event to an event queue (secure mode).
The sent data will be received as \ref sys_event_t structure type. The data will be stored as the following:
- \ref sys_event_t::source "source" = \ref SPU_THREAD_EVENT_USER_KEY
- \ref sys_event_t::data_1 "data_1" = SPU thread ID
- \ref sys_event_t::data_2 "data_2" (upper 32 bits) = SPU port number
- \ref sys_event_t::data_2 "data_2" (lower 32 bits) = lower 24 bits of \p data0
- \ref sys_event_t::data_3 "data_3" = \p data1
\param spup SPU port number.
\param data0 Data 0 (only lower 24 bits are sent)
\param data1 Data 1
\return zero if no error occured, nonzero otherwise.
*/
int spu_thread_send_event(uint8_t spup,uint32_t data0,uint32_t data1);

/*! \brief Send an SPU thread user event.

This function sends an SPU thread user event to an event queue (fire and forget
mode).
The sent data will be received as \ref sys_event_t structure type. The data will be stored as the following:
- \ref sys_event_t::source "source" = \ref SPU_THREAD_EVENT_USER_KEY
- \ref sys_event_t::data_1 "data_1" = SPU thread ID
- \ref sys_event_t::data_2 "data_2" (upper 32 bits) = SPU port number
- \ref sys_event_t::data_2 "data_2" (lower 32 bits) = lower 24 bits of \p data0
- \ref sys_event_t::data_3 "data_3" = \p data1
\param spup SPU port number.
\param data0 Data 0 (only lower 24 bits are sent)
\param data1 Data 1
\return zero if no error occured, nonzero otherwise.
*/
int spu_thread_throw_event(uint8_t spup,uint32_t data0,uint32_t data1);

/*! \brief Receive an event from a bound SPU event queue.

This is a blocking call. It waits for an event to be received.
\param spuq Event queue binding id (as provided to \ref sysSpuThreadBindQueue).
\param data0 Pointer to storage for the first event data field.
\param data1 Pointer to storage for the second event data field.
\param data2 Pointer to storage for the third event data field.
\return zero if no error occured, nonzero otherwise.
*/
int spu_thread_receive_event(uint32_t spuq,uint32_t *data0,uint32_t *data1,uint32_t *data2);

/*! \brief Receive an event from a bound SPU event queue if an event is present.

This call is non blocking. It returns even if no event has been received.
\param spuq Event queue binding id (as provided to \ref sysSpuThreadBindQueue).
\param data0 Pointer to storage for the first event data field.
\param data1 Pointer to storage for the second event data field.
\param data2 Pointer to storage for the third event data field.
\return zero if an event was received, nonzero if no event was received or
in case of error.
*/
int spu_thread_tryreceive_event(uint32_t spuq,uint32_t *data0,uint32_t *data1,uint32_t *data2);

#ifdef __cplusplus
	}
#endif

#endif
