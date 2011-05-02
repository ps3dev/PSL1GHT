/*! \file event_queue.h
 \brief Event queue management functions.
*/

#ifndef __SYS_EVENT_QUEUE_H__
#define __SYS_EVENT_QUEUE_H__

#include <ppu-lv2.h>

/*! \brief Event queue type PPU */
#define SYS_EVENT_QUEUE_PPU						0x01
/*! \brief Event queue type SPU */
#define SYS_EVENT_QUEUE_SPU						0x02

/*! \brief Synchronize event queue FIFO */
#define SYS_EVENT_QUEUE_FIFO					0x01
/*! \brief Synchronize event queue PRIO */
#define SYS_EVENT_QUEUE_PRIO					0x02
/*! \brief Synchronize event queue PRIO_INHERIT */
#define SYS_EVENT_QUEUE_PRIO_INHERIT			0x03

/*! \brief Event port type LOCAL */
#define SYS_EVENT_PORT_LOCAL					0x01

/*! \brief Used to auto create a port name */
#define SYS_EVENT_PORT_NO_NAME					0x00

/*! \brief Used to auto create a event queue key. */
#define SYS_EVENT_QUEUE_KEY_LOCAL				0x00

#ifdef __cplusplus
	extern "C" {
#endif

/*! \brief Data structure for create attributes for an event queue */
typedef struct sys_event_queue_attr
{
	/*! \brief Synchronization attribute.

	Possible values:
	- \ref SYS_EVENT_QUEUE_FIFO
	- \ref SYS_EVENT_QUEUE_PRIO
	- \ref SYS_EVENT_QUEUE_PRIO_INHERIT
	*/
	u32 attr_protocol;

	/*! \brief Event queue type.

	Possible values:
	- \ref SYS_EVENT_QUEUE_PPU
	- \ref SYS_EVENT_QUEUE_SPU
	*/
	s32 type;

	/*! \brief Name identifier. */
	char name[8];
} sys_event_queue_attr_t;

/*! \brief Data structure for received event data. */
typedef struct sys_event
{
	u64 source;			/*!< \brief id of emitting source */
	u64 data_1;			/*!< \brief data field 1 */
	u64 data_2;			/*!< \brief data field 2 */
	u64 data_3;			/*!< \brief data field 3 */
} sys_event_t;

/*! \brief Create an event queue.
\param eventQ Pointer to receive the event queue id.
\param attrib Pointer to attribute structure.
\param key Key to be used with this event queue.
\param size Initial size of the event queue.
\return zero if no error, nonzero otherwise.
*/
LV2_SYSCALL sysEventQueueCreate(sys_event_queue_t *eventQ,sys_event_queue_attr_t *attrib,sys_ipc_key_t key,s32 size)
{
	lv2syscall4(128,(u64)eventQ,(u64)attrib,key,size);
	return_to_user_prog(s32);
}

/*! \brief Destroy an event queue.
\param eventQ The event queue id.
\param mode Destroy mode (0: normal).
\return zero if no error, nonzero otherwise.
*/
LV2_SYSCALL sysEventQueueDestroy(sys_event_queue_t eventQ,s32 mode)
{
	lv2syscall2(129,eventQ,mode);
	return_to_user_prog(s32);
}

/*! \brief Receive an event from an event queue.

The current thread blocks until an event is received or the timeout period
(in microseconds) has been reached.
\param eventQ The event queue id.
\param event Pointer to a structure for received event data.
\param timeout_usec Timeout period, or 0 for no timeout.
\return zero if an event was received, nonzero if the timeout delay was reached
        or if an error occured.
*/
LV2_SYSCALL sysEventQueueReceive(sys_event_queue_t eventQ,sys_event_t *event,u64 timeout_usec)
{
	lv2syscall3(130,eventQ,(u64)event,timeout_usec);
#ifdef REG_PASS_SYS_EVENT_QUEUE_RECEIVE
	REG_PASS_SYS_EVENT_QUEUE_RECEIVE;
#endif
	return_to_user_prog(s32);
}

/*! \brief Drain an event queue.

This function removes all pending events in the queue, making it empty.
\param eventQ The event queue id.
\return zero if no error, nonzero otherwise.
*/
LV2_SYSCALL sysEventQueueDrain(sys_event_queue_t eventQ)
{
	lv2syscall1(133,eventQ);
	return_to_user_prog(s32);
}

/*! \brief Create an event port.

This function creates an event port for sending events thru a connected event queue.
\param portId Pointer to receive the event port id.
\param portType Type of the port to create. \n
Possible values:
- \ref SYS_EVENT_PORT_LOCAL
\param name User defined name or \ref SYS_EVENT_PORT_NO_NAME.
\return zero if no error, nonzero otherwise.
*/
LV2_SYSCALL sysEventPortCreate(sys_event_port_t *portId,int portType,u64 name)
{
	lv2syscall3(134,(u64)portId,portType,name);
	return_to_user_prog(s32);
}

/*! \brief Destroys an event port.
\param portId Event port id of the event port to be destroyed.
\return zero if no error, nonzero otherwise.
*/
LV2_SYSCALL sysEventPortDestroy(sys_event_port_t portId)
{
	lv2syscall1(135,portId);
	return_to_user_prog(s32);
}

/*! \brief Send an event.

This function sends an event to the event queue that is connected with the specified event port. Three additional 64-bit data can be sent with the event.
\param portId Event port id.
\param data0 Additional data.
\param data1 Additional data.
\param data2 Additional data.
\return zero if no error, nonzero otherwise.
*/
LV2_SYSCALL sysEventPortSend(sys_event_port_t portId,u64 data0,u64 data1,u64 data2)
{
	lv2syscall4(138,portId,data0,data1,data2);
	return_to_user_prog(s32);
}

/*! \brief Connect an event port to an event queue in the same process.
\param portId The event port id.
\param eventQ The event queue id.
\return zero if no error, nonzero otherwise.
*/
LV2_SYSCALL sysEventPortConnectLocal(sys_event_port_t portId,sys_event_queue_t eventQ)
{
	lv2syscall2(136,portId,eventQ);
	return_to_user_prog(s32);
}

/*! \brief Disconnect an event port from an event queue.
\param portId The event port id.
\return zero if no error, nonzero otherwise.
*/
LV2_SYSCALL sysEventPortDisconnect(sys_event_port_t portId)
{
	lv2syscall1(137,portId);
	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
