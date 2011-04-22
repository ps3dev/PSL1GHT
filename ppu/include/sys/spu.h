/*! \file sys/spu.h
  \brief SPU management syscalls.

  These are the LV2 syscalls for SPU management (raw SPUs and SPU threads).

  \see \ref spu_man
*/

/*! \page spu_man Quick guide to SPU management

\section Introduction

The Cell Broadband Engine (CBE) is the processor of the PS3. It features one
main core, the PPE (Power Processing Element), which is a dual threaded core from
the IBM POWER family, and six Synergistic Processing Elements (SPEs) are available
to the user programs.

The SPEs are the computing elements of the Cell Broadband Engine processor of the
PS3. They feature a computing core (SPU = Synergistic Processing Unit), 256 kB
of local memory (local store) and a Memory Flow Controller (MFC), responsible
for DMA transfers between local store and main memory, as well as synchronization
with the other SPEs and the PPE.

The system allows two ways of handling SPEs by either using the <em>raw SPU</em>
primitives or the <em>SPU thread</em> primitives.
Both methods allow to
efficiently exploit the SPEs, but SPU threads allow more flexibility.
This tutorial only deals with SPU threads.


\section spu_threads SPU threads

SPU threads are dedicated SPU programs running on the SPEs. There can
be more running SPU threads as there are available SPEs, in that cas the system
performs thread scheduling in order all threads to appear to be running
simultaneously.
However, scheduling has a cost, so performance-aware applications should try
to avoid context switches by running exactly the needed number of threads.
Ideally, unless in some very specific circumstances, only maximum 6 threads
should be running simultaneously, as 6 SPEs are available on the PS3.

\subsection sputhr_run Running and stopping threads

Creating and running a SPU thread basically requires the following steps:
 - import a SPU program into a SPU image (see \ref sysSpuImageOpen, \ref sysSpuImageImport)
 - create a SPU thread group (see \ref sysSpuThreadGroupCreate).
 - create the SPU thread (\ref sysSpuThreadInitialize) for that group. Eventually
   create several threads for the group.
 - run the SPU thread group (\ref sysSpuThreadGroupStart).

Then all threads of the group will run simultaneously.
The whole thread group
may eventually be preempted to allow the execution of concurrent thread groups.
Obvisoulsy, if there is only one thread group running, or if all running thread
groups do not exceed a total of 6 threads, no thread group is preempted.

The PPU can suspend and resume a thread group (\ref sysSpuThreadGroupSuspend,
\ref sysSpuThreadGroupResume), preempt it (\ref sysSpuThreadGroupYield),
or even terminate it (\ref sysSpuThreadGroupTerminate).

A SPU thread can eventually terminate itself with a call to \ref spu_thread_exit.
It also can terminate the whole SPU thread group (\ref spu_thread_group_exit),
or call the SPU thread group scheduler (\ref spu_thread_group_yield).

A SPU thread group is terminated if all its threads are terminated.

The PPE must join a SPU thread group (\ref sysSpuThreadGroupJoin) to ensure its
threads are terminated.


\subsection sputhr_com Communication between threads

SPU threads can communicate with the running PPE threads, and also with other
SPU threads within the same SPU thread group.

A PPE thread can :
 - read or write to a SPU thread's local store
   (\ref sysSpuThreadWriteLocalStorage, \ref sysSpuThreadReadLocalStorage)
 - write to a SPU's signal notification register (\ref sysSpuThreadWriteSignal)

A SPU thread can :
 - issue DMA transfers with its MFC, between its local store and main memory, or
   between its local store and another SPU thread's local store. In the latter
   case the beginning of a SPU thread's local store effective address is
   <code>SPU_THREAD_BASE + spu * SPU_THREAD_OFFSET</code> (having \c spu the
   same thread index value as provided to \ref sysSpuThreadInitialize).
 - write to another SPU's signal notification register. This must be done by
   writing to the memory-mapped IO registers through DMA transfers.
   In that case, the effective address to write to is
   <code>SPU_THREAD_BASE + spu * SPU_THREAD_OFFSET</code> (having \c spu the same
   thread index value as provided to \ref sysSpuThreadInitialize) to which we
   add either \c SPU_THREAD_Sig_Notify_1 or \c SPU_THREAD_Sig_Notify_2, depending
   on the signal notification register to be written to.

For more information about DMA transfers and SPU programming in general, refer
to the Cell Broadband Engine documentation.
*/
#ifndef __SYS_SPU_H__
#define __SYS_SPU_H__

#include <ppu-asm.h>
#include <ppu-lv2.h>
#include <lv2/spu.h>

/* MFC channel registers */
#define MFC_LSA									0x3004	//!< local store address
#define MFC_EAH									0x3008	//!< effective address high
#define MFC_EAL									0x300C	//!< effective address low
#define MFC_Size								0x3010	//!< size of MFC DMA transfer
#define MFC_Class_CMD							0x3014	//!< class of MFC command
#define MFC_CMD_Status							0x3014	//!< MFC command status
#define MFC_QStatus								0x3104	//!< MFC query status
#define Prxy_QueryType							0x3204	//!< Proxy query type
#define Prxy_QueryMask							0x321C	//!< Proxy Query mask
#define Prxy_TagStatus							0x322C	//!< Proxy tag status
#define SPU_Out_MBox							0x4004	//!< Outbound mailbox
#define SPU_In_MBox								0x400C	//!< Inbound mailbox
#define SPU_MBox_Status							0x4014	//!< Outbound mailbox status
#define SPU_RunCtrl								0x401C	//!< SPU Run control
#define SPU_Status								0x4024	//!< SPU status
#define SPU_NextPC								0x4034	//!< SPU next PC
#define SPU_Sig_Notify_1						0x1400C	//!< Signal notification 1
#define SPU_Sig_Notify_2						0x1C00C	//!< Signal notification 2

//! No thread attributes
#define SPU_THREAD_ATTR_NONE					0x00
//! Enables interrupts.
#define SPU_THREAD_ATTR_ASYNC_INT_ENABLE		0x01
//! Synchronize SPU decrementer with PPU time base
#define SPU_THREAD_ATTR_DEC_SYNC_TB_ENABLE		0x02

//! User SPU thread event
#define SPU_THREAD_EVENT_USER					0x01
//! DMA SPU thread event
#define SPU_THREAD_EVENT_DMA					0x02

//! SPU thread event user key
#define SPU_THREAD_EVENT_USER_KEY				0xFFFFFFFF53505501ULL
//! SPU thread event DMA key
#define SPU_THREAD_EVENT_DMA_KEY				0xFFFFFFFF53505502ULL

//! Configure signal notification register 1 to overwrite mode
#define SPU_SIGNAL1_OVERWRITE					0x00
//! Configure signal notification register 1 to OR mode
#define SPU_SIGNAL1_OR							0x01
//! Configure signal notification register 2 to overwrite mode
#define SPU_SIGNAL2_OVERWRITE					0x00
//! Configure signal notification register 2 to OR mode
#define SPU_SIGNAL2_OR							0x02

//! Base of memory-mapped SPU thread resources
#define SPU_THREAD_BASE							0xF0000000ULL
//! Offset between resources for consecutive SPU threads
#define SPU_THREAD_OFFSET						0x00100000ULL
//! Base of memory-mapped raw SPU program resources
#define SPU_RAW_BASE							0xE0000000ULL
//! Offset between resources for consecutive raw SPU programs
#define SPU_RAW_OFFSET							0x00100000ULL
//! Offset from the beginning of a SPU thread/raw program's resources for local store memory
#define SPU_LOCAL_OFFSET						0x00000000ULL
//! Offset from the beginning of a SPU thread/raw program's resources for IO registers
#define SPU_PROBLEM_OFFSET						0x00040000ULL

/*! \brief Get base offset for a raw SPU program.
\param spu SPU number (0-5)
*/
#define SPU_RAW_GET_BASE_OFFSET(spu) \
	(SPU_RAW_BASE + (SPU_RAW_OFFSET*(spu)))

//! Returns the memory-mapped address of local storage of a raw SPU.
#define SPU_RAW_GET_LOCAL_STORAGE(spu,reg) \
	(SPU_RAW_GET_BASE_OFFSET(spu) + SPU_LOCAL_OFFSET + (reg))

//! Returns the memory-mapped address of problem storage of a raw SPU.
#define SPU_RAW_GET_PROBLEM_STORAGE(spu,reg) \
	(SPU_RAW_GET_BASE_OFFSET(spu) + SPU_PROBLEM_OFFSET + (reg))

/*! \brief Get base offset for a SPU thread.
\param spu SPU thread id
*/
#define SPU_THREAD_GET_BASE_OFFSET(spu) \
	(SPU_THREAD_BASE + (SPU_THREAD_OFFSET*(spu)))

//! Returns the memory-mapped address of local storage of a SPU thread.
#define SPU_THREAD_GET_LOCAL_STORAGE(spu,reg) \
	(SPU_THREAD_GET_BASE_OFFSET(spu) + SPU_LOCAL_OFFSET + (reg))

//! Returns the memory-mapped address of problem storage of a SPU thread.
#define SPU_THREAD_GET_PROBLEM_STORAGE(spu,reg) \
	(SPU_THREAD_GET_BASE_OFFSET(spu) + SPU_PROBLEM_OFFSET + (reg))

#ifdef __cplusplus
extern "C" {
#endif

//! A structure containing the arguments passed to the SPU main function.
typedef struct _sys_spu_thread_arg
{
	u64 arg0;	//!< First argument.
	u64 arg1;	//!< Second argument.
	u64 arg2;	//!< Third argument.
	u64 arg3;	//!< Fourth argument.
} sysSpuThreadArgument;

/*! \brief A structure containing SPU thread attributes.

 Possible values for \p attribute flags are \ref SPU_THREAD_ATTR_NONE or an
 OR'ed list of the following flags:
   - \ref SPU_THREAD_ATTR_ASYNC_INT_ENABLE
   - \ref SPU_THREAD_ATTR_DEC_SYNC_TB_ENABLE
*/
typedef struct _sys_spu_thread_attr
{
	u32 nameAddress;	//!< Effective address of the thread's name string.
	u32 nameSize;		//!< Size of the name string in bytes (including terminating null byte).
	u32 attribute;		//!< OR'ed list of SPU thread attribute flags (or \ref SPU_THREAD_ATTR_NONE)
} sysSpuThreadAttribute;

//! A structure containing SPU thread group attributes.
typedef struct _sys_spu_thread_group_attr
{
	u32 nameSize;		//!< Size of the name string in bytes (including terminating null byte).
	u32 nameAddress;	//!< Effective address of the thread group's name string.
	u32 groupType;		//!< Thread group type (\c 0 for normal thread groups).
	u32 memContainer;	//!< Memory container id (\c 0 for normal thread groups).
} sysSpuThreadGroupAttribute;

/*! \brief Initialize the SPU management.
 \param spus
 Total number of needed SPUs (from 1 to 6).
 \param rawspus
 Number of required raw SPUs (from 1 to 5).
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuInitialize(u32 spus,u32 rawspus) 
{ 
	lv2syscall2(169,spus,rawspus); 
	return_to_user_prog(s32);
}

/*! \brief Allocate a raw SPU.
 \param spu
 Pointer to a returned raw SPU identifier.
 \param attributes
 Pointer to an attributes structure (or \c NULL if default attributes are used).
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuRawCreate(u32 *spu,u32 *attributes)
{
	lv2syscall2(160,(u64)spu,(u64)attributes);
	return_to_user_prog(s32);
}

/*! \brief De-allocate a raw SPU.
 \param spu
 The raw SPU identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuRawDestroy(u32 spu)
{
	lv2syscall1(161,spu);
	return_to_user_prog(s32);
}

/*! \brief Create an interrupt tag for a raw SPU.
 \param spu
 The raw SPU identifier.
 \param classid
 The interrupt class identifier.
 \param hardwarethread
 The hardware thread identifier.
 \param tag
 Pointer to the returned interrupt tag identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuCreateInterrupTag(u32 spu,u32 classid,u32 hardwarethread,u32 *tag)
{
	lv2syscall4(150,spu,classid,hardwarethread,(u64)tag);
	return_to_user_prog(s32);
}

/*! \brief Set the value of the interrupt mask register for a raw SPU.
 \param spu
 The raw SPU identifier.
 \param classid
 The interrupt class identifier.
 \param mask
 The new interrupt mask register value.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuSetIntMask(u32 spu,u32 classid,u64 mask)
{
	lv2syscall3(151,spu,classid,mask);
	return_to_user_prog(s32);
}

/*! \brief Get the value of the interrupt mask register for a raw SPU.
 \param spu
 The raw SPU identifier.
 \param classid
 The interrupt class identifier.
 \param mask
 Pointer to the returned interrupt mask register value.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuGetIntMask(u32 spu,u32 classid,u64 *mask)
{
	lv2syscall3(152,spu,classid,(u64)mask);
	return_to_user_prog(s32);
}

/*! \brief Set the value of the interrupt status register for a raw SPU.
 \param spu
 The raw SPU identifier.
 \param classid
 The interrupt class identifier.
 \param mask
 The new interrupt status register value.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuRawSetIntStat(u32 spu, u32 classid, u64 stat)
{ 
	lv2syscall3(153, spu, classid, stat);
	return_to_user_prog(s32);
}

/*! \brief Get the value of the interrupt status register for a raw SPU.
 \param spu
 The raw SPU identifier.
 \param classid
 The interrupt class identifier.
 \param mask
 Pointer to the returned interrupt status register value.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuRawGetIntStat(u32 spu, u32 classid, u64* stat)
{
	lv2syscall3(154, spu, classid, (u64)stat);
	return_to_user_prog(s32);
}

/*! \brief Read from the raw SPU's outbound interrupt mailbox register.
 \param spu
 The raw SPU identifier.
 \param value
 Pointer to the read value.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuRawReadPuintMb(u32 spu, u32* value)
{
	lv2syscall2(163, spu, (u64)value);
	return_to_user_prog(s32);
}

/*! \brief Configure the raw SPU's signal notification.
 \param spu
 The raw SPU identifier.
 \param value
 A OR combination of the following values:
  - \ref SPU_SIGNAL1_OVERWRITE
  - \ref SPU_SIGNAL1_OR
  - \ref SPU_SIGNAL2_OVERWRITE
  - \ref SPU_SIGNAL2_OR
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuRawSetConfiguration(u32 spu, u32 value)
{
	lv2syscall2(196, spu, value);
	return_to_user_prog(s32);
}

/*! \brief Get the configuration of the raw SPU's signal notification.
 \param spu
 The raw SPU identifier.
 \param value
 Pointer to the read configuration value, which is a OR combination of the
 following values:
  - \ref SPU_SIGNAL1_OVERWRITE
  - \ref SPU_SIGNAL1_OR
  - \ref SPU_SIGNAL2_OVERWRITE
  - \ref SPU_SIGNAL2_OR
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuRawGetConfirugation(u32 spu, u32* value)
{
	lv2syscall2(197, spu, (u64)value);
	return_to_user_prog(s32);
}

/*! \brief Recover a raw SPU from a page fault.
 \param spu
 The raw SPU identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuRawRecoverPageFault(u32 spu)
{
	lv2syscall1(199, spu);
	return_to_user_prog(s32);
}

/*! \brief Create a SPU image from an ELF file.
 \param image
 Pointer to the SPU image structure.
 \param path
 The pathname of the ELF file to be read from.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuImageOpen(sysSpuImage* image, const char* path)
{ 
	lv2syscall2(156, (u64)image, (u64)path);
	return_to_user_prog(s32);
}

/*! \brief Create a SPU image from an open binary file.
 \param image
 Pointer to the SPU image structure.
 \param fd
 File descriptor of the opened file.
 \param offset
 Offset to the beginning of the SPU image in the file.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuImageOpenFd(sysSpuImage* image, s32 fd, u64 offset)
{ 
	lv2syscall3(260, (u64)image, fd, offset);
	return_to_user_prog(s32);
}

/*! \brief Initialize a SPU thread.
 \param thread
 Pointer to the returned thread identifier.
 \param group
 Group identifier.
 \param spu
 Index of thread in group (from 0 to the thread group size minus 1).
 \param image
 Pointer to the SPU image structure.
 \param attributes
 Pointer to a SPU thread attribute structure.
 \param arguments
 Pointer to the arguments for the thread's main function.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadInitialize(u32* thread, u32 group, u32 spu, sysSpuImage* image, sysSpuThreadAttribute* attributes, sysSpuThreadArgument* arguments)
{
	lv2syscall6(172, (u64)thread, group, spu, (u64)image, (u64)attributes, (u64)arguments);
	return_to_user_prog(s32);
}

/*! \brief Set the SPU thread arguments.
 \param thread
 Thread identifier.
 \param arguments
 Pointer to the arguments list.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadSetArguments(u32 thread, sysSpuThreadArgument* arguments)
{ 
	lv2syscall2(166, thread, (u64)arguments);
	return_to_user_prog(s32);
}

/*! \brief Get the exit status of a thread.

 The exit status is the value the SPU program used as an argument to spu_thread_exit.
 \param thread
 Thread identifier.
 \param arguments
 Pointer to the returned exit status.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGetExitStatus(u32 thread, s32* status)
{
	lv2syscall2(165, thread, (u64)status);
	return_to_user_prog(s32);
}

/*! \brief Connect an event queue to a SPU thread.
 \param thread
 Thread identifier.
 \param queue
 Event queue identifier.
 \param type
 Event queue type. \n
 Possible values are:
 - \ref SPU_THREAD_EVENT_USER
 - \ref SPU_THREAD_EVENT_DMA
 \param spup
 SPU event port number (value in <code>0..63</code> interval).
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadConnectEvent(u32 thread, u32 queue, u32 type, u8 spup)
{
	lv2syscall4(191, thread, queue, type, spup);
	return_to_user_prog(s32);
}

/*! \brief Disconnect an event queue from a SPU thread.
 \param thread
 Thread identifier.
 \param type
 Event queue type. \n
 Possible values are:
 - \ref SPU_THREAD_EVENT_USER
 - \ref SPU_THREAD_EVENT_DMA
 \param spup
 SPU event port number.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadDisconnectEvent(u32 thread, u32 type, u8 spup) 
{ 
	lv2syscall3(192, thread, type, spup); 
	return_to_user_prog(s32);
}

/*! \brief Bind an event queue to a SPU thread.
 \param thread
 Thread identifier.
 \param queue
 Event queue identifier.
 \param spuq_num
 Event queue binding id.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadBindQueue(u32 thread, u32 queue, u32 spuq_num) 
{ 
	lv2syscall3(193, thread, queue, spuq_num); 
	return_to_user_prog(s32);
}

/*! \brief Unbind an event queue from a SPU thread.
 \param thread
 Thread identifier.
 \param spuq_num
 Event queue binding id.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadUnbindQueue(u32 thread, u32 spuq_num) 
{ 
	lv2syscall2(194, thread, spuq_num); 
	return_to_user_prog(s32);
}

/*! \brief Write a value to a SPU thread's local store memory.

 \param thread
 Thread identifier.
 \param address
 Address in local store to write to.
 \param value
 Value to be written.
 \param type
 Type of value to be written.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadWriteLocalStorage(u32 thread, u32 address, u64 value, u32 type) 
{ 
	lv2syscall4(181, thread, address, value, type); 
	return_to_user_prog(s32);
}

/*! \brief Read a value From the SPU thread's local store memory.

 \param thread
 Thread identifier.
 \param address
 Address in local store to read from.
 \param value
 Pointer to read value.
 \param type
 Type of value to be read.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadReadLocalStorage(u32 thread, u32 address, u64* value, u32 type) 
{ 
	lv2syscall4(182, thread, address, (u64)value, type); 
	return_to_user_prog(s32);
}

/*! \brief Write to the SPU thread's signal notification register.

 \param thread
 Thread identifier.
 \param signal
  - 0 : write to signal notification register 1
  - 1 : write to signal notification register 2
 \param value
 Value to be written.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadWriteSignal(u32 thread,u32 signal,u32 value)
{
	lv2syscall3(184,thread,signal,value);
	return_to_user_prog(s32);
}

/*! \brief Configure the SPU thread's signal notification.

 \param thread
 Thread identifier.
 \param value
 A OR combination of the following values:
  - \ref SPU_SIGNAL1_OVERWRITE
  - \ref SPU_SIGNAL1_OR
  - \ref SPU_SIGNAL2_OVERWRITE
  - \ref SPU_SIGNAL2_OR
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadSetConfiguration(u32 thread, u64 value) 
{ 
	lv2syscall2(187, thread, value); 
	return_to_user_prog(s32);
}

/*! \brief Get the configuration of the SPU thread's signal notification.

 \param thread
 Thread identifier.
 \param value
 Pointer to the read configuration value, which is a OR combination of the
 following values:
  - \ref SPU_SIGNAL1_OVERWRITE
  - \ref SPU_SIGNAL1_OR
  - \ref SPU_SIGNAL2_OVERWRITE
  - \ref SPU_SIGNAL2_OR
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGetConfiguration(u32 thread, u64* value) 
{ 
	lv2syscall2(188, thread, (u64)value); 
	return_to_user_prog(s32);
}

/*! \brief Write to the SPU thread's inbound mailbox register.

 \param thread
 Thread identifier.
 \param value
 Value to be written.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadWriteMb(u32 thread, u32 value) 
{ 
	lv2syscall2(190, thread, value); 
	return_to_user_prog(s32);
}

/*! \brief Recover from a SPU thread page fault.

 \param thread
 Thread identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadRecoverPageFault(u32 thread) 
{ 
	lv2syscall1(198, thread); 
	return_to_user_prog(s32);
}

/*! \brief Create a SPU thread group.

 \param group
 Pointer to the returned SPU thread group identifier.
 \param num
 Number of SPU threads in the thread group.
 \param prio
 Priority of the thread group.
 \param attr
 Pointer to a SPU thread group attribute structure.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupCreate(u32 *group,u32 num,u32 prio,sysSpuThreadGroupAttribute *attr)
{
	lv2syscall4(170,(u64)group,num,prio,(u64)attr);
	return_to_user_prog(s32);
}

/*! \brief Destroy a SPU thread group.

 \param group
 SPU thread group identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupDestroy(u32 group)
{
	lv2syscall1(171,group);
	return_to_user_prog(s32);
}

/*! \brief Start a SPU thread group.

 \param group
 SPU thread group identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupStart(u32 group)
{
	lv2syscall1(173,group);
	return_to_user_prog(s32);
}

/*! \brief Suspend a SPU thread group.

 \param group
 SPU thread group identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupSuspend(u32 group)
{
	lv2syscall1(174,group);
	return_to_user_prog(s32);
}

/*! \brief Resume a SPU thread group.

 \param group
 SPU thread group identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupResume(u32 group)
{
	lv2syscall1(175,group);
	return_to_user_prog(s32);
}

/*! \brief Yield a SPU thread group.

 \param group
 SPU thread group identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupYield(u32 group)
{
	lv2syscall1(176,group);
	return_to_user_prog(s32);
}

/*! \brief Terminate a SPU thread group.

 \param group
 SPU thread group identifier.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupTerminate(u32 group,u32 value)
{
	lv2syscall2(177,group,value);
	return_to_user_prog(s32);
}

/*! \brief Wait for a SPU thread group to finish its execution.

 \param group
 SPU thread group identifier.
 \param cause
 Pointer to returned cause of thread group termination.
 \param status
 Pointer to returned status of thread group termination.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupJoin(u32 group,u32 *cause,u32 *status)
{
	lv2syscall3(178,group,(u64)cause,(u64)status);
	return_to_user_prog(s32);
}

/*! \brief Set a SPU thread group's priority.

 \param group
 SPU thread group identifier.
 \param prio
 New priority value.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupSetPriority(u32 group,u32 prio)
{
	lv2syscall2(179,group,prio);
	return_to_user_prog(s32);
}

/*! \brief Get a SPU thread group's priority.

 \param group
 SPU thread group identifier.
 \param prio
 Pointer to returned priority value.
 \return
 zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSpuThreadGroupGetPriority(u32 group,u32 *prio)
{
	lv2syscall2(180,group,(u64)prio);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysSpuThreadGroupConnectEvent(u32 group,u32 eventQ,u32 eventType)
{
	lv2syscall3(185,group,eventQ,eventType);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysSpuThreadGroupDisconnectEvent(u32 group,u32 eventType)
{
	lv2syscall2(186,group,eventType);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysSpuThreadGroupConnectEventAllThreads(u32 group,u32 eventQ,u64 req,u8 *spup)
{
	lv2syscall4(251,group,eventQ,req,(u64)spup);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysSpuThreadGroupDisonnectEventAllThreads(u32 group,u8 spup)
{
	lv2syscall2(252,group,spup);
	return_to_user_prog(s32);
}

/*! \brief Get a SPU thread group's priority.

 \param group
 SPU thread group identifier.
 \param prio
 Pointer to returned priority value.
 \return
 zero if no error occured, nonzero otherwise.
*/

static inline void sysSpuRawWriteProblemStorage(u32 spu,u32 reg,u32 value)
{
	__write32(SPU_RAW_GET_PROBLEM_STORAGE(spu,reg),value);
}

static inline u32 sysSpuRawReadProblemStorage(u32 spu,u32 reg)
{
	return __read32(SPU_RAW_GET_PROBLEM_STORAGE(spu,reg));
}

static inline void sysSpuRawWriteLocalStorage(u32 spu,u32 reg,u32 value)
{
	__write32(SPU_RAW_GET_LOCAL_STORAGE(spu,reg),value);
}

static inline u32 sysSpuRawReadLocalStorage(u32 spu,u32 reg)
{
	return __read32(SPU_RAW_GET_LOCAL_STORAGE(spu,reg));
}

static inline void sysSpuThreadWriteProblemStorage(u32 spu,u32 reg,u32 value)
{
	__write32(SPU_THREAD_GET_PROBLEM_STORAGE(spu,reg),value);
}

static inline u32 sysSpuThreadReadProblemStorage(u32 spu,u32 reg)
{
	return __read32(SPU_THREAD_GET_PROBLEM_STORAGE(spu,reg));
}

#ifdef __cplusplus
	}
#endif

#endif
