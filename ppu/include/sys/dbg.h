/*! \file dbg.h
 \brief Debugging management library.
*/

#ifndef __SYS_DBG_H__
#define __SYS_DBG_H__

#include <ppu-types.h>
#include <sys/thread.h>
#include <sys/event_queue.h>

/*! \brief number of PPU GPR registers */
#define SYS_DBG_PPU_NUM_GPR			32
/*! \brief number of PPU FPR registers */
#define SYS_DBG_PPU_NUM_FPR			32
/*! \brief number of PPU VR registers */
#define SYS_DBG_PPU_NUM_VR			32
/*! \brief size (in bytes) of a PPU VR register */
#define SYS_DBG_PPU_VR_WIDTH		16

/*! \brief number of SPU GPR registers */
#define SYS_DBG_SPU_NUM_GPR			128
/*! \brief number of SPU MFC/CQ/SR registers */
#define SYS_DBG_SPU_NUM_MFC_CQ_SR	96
/*! \brief size (in bytes) of a SPU GPR register */
#define SYS_DBG_SPU_GPR_WIDTH		16
/*! \brief number of SPU inbound MB entries */
#define SYS_DBG_SPU_NUM_MB			4

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Possible statuses for PPU threads. */
typedef enum
{
	PPU_THREAD_STATUS_IDLE,
	PPU_THREAD_STATUS_RUNNABLE,
	PPU_THREAD_STATUS_ONPROC,
	PPU_THREAD_STATUS_SLEEP,
	PPU_THREAD_STATUS_STOP,
	PPU_THREAD_STATUS_ZOMBIE,
	PPU_THREAD_STATUS_DELETED,
	PPU_THREAD_STATUS_UNKNOWN
} sys_dbg_ppu_thread_status_t;

/*! \brief Possible statuses for PPU thread groups. */
typedef enum
{
	SPU_THREAD_GROUP_STATUS_NOTINITIALIZED,
	SPU_THREAD_GROUP_STATUS_INITIALIZED,
	SPU_THREAD_GROUP_STATUS_READY,
	SPU_THREAD_GROUP_STATUS_WAITING,
	SPU_THREAD_GROUP_STATUS_SUSPENDED,
	SPU_THREAD_GROUP_STATUS_WAITING_AND_SUSPENDED,
	SPU_THREAD_GROUP_STATUS_RUNNING,
	SPU_THREAD_GROUP_STATUS_STOPPED,
	SPU_THREAD_GROUP_STATUS_UNKNOWN,
} sys_dbg_spu_thread_group_status_t;

/*! \brief contents of a PPU VR register */
typedef union
{
	/*! \brief byte vector format. */
	u8 byte[SYS_DBG_PPU_VR_WIDTH];
	/*! \brief halfword vector format. */
	u16 halfword[SYS_DBG_PPU_VR_WIDTH/sizeof(u16)];
	/*! \brief word vector format. */
	u32 word[SYS_DBG_PPU_VR_WIDTH/sizeof(u32)];
	/*! \brief double word vector format. */
	u64 dword[SYS_DBG_PPU_VR_WIDTH/sizeof(u64)];
} sys_dbg_vr_t;

/*! \brief contents of a SPU GPR register */
typedef union
{
	/*! \brief byte vector format. */
	u8 byte[SYS_DBG_SPU_GPR_WIDTH];
	/*! \brief halfword vector format. */
	u16 halfword[SYS_DBG_SPU_GPR_WIDTH/sizeof(u16)];
	/*! \brief word vector format. */
	u32 word[SYS_DBG_SPU_GPR_WIDTH/sizeof(u32)];
	/*! \brief double word vector format. */
	u64 dword[SYS_DBG_SPU_GPR_WIDTH/sizeof(u64)];
} sys_dbg_spu_gpr_t;

/*! \brief PPU thread context. */
typedef struct _sys_dbg_ppu_thread_context
{
	u64 gpr[SYS_DBG_PPU_NUM_GPR];           /*!< \brief GPR registers */
	u32 cr;                                 /*!< \brief CR register */
	u32 xer;                                /*!< \brief XER register */
	u32 lr;                                 /*!< \brief LR register */
	u32 ctr;                                /*!< \brief CTR register */
	u32 pc;                                 /*!< \brief PC register */
	u64 fpr[SYS_DBG_PPU_NUM_FPR];           /*!< \brief FPR registers */
	u32 fpscr;                              /*!< \brief FPSCR register */
	sys_dbg_vr_t vr[SYS_DBG_PPU_NUM_VR];    /*!< \brief VR registers */
	sys_dbg_vr_t vscr;                      /*!< \brief VSCR register */
} sys_dbg_ppu_thread_context_t;

/*! \brief SPU thread context. */
typedef struct _sys_dbg_spu_thread_context
{
	sys_dbg_spu_gpr_t gpr[SYS_DBG_SPU_NUM_GPR];  /*!< \brief GPR registers */
	u32 npc;                                /*!< \brief NPC register */
	u32 fpscr;                              /*!< \brief FPSCR register */
	u32 srr0;                               /*!< \brief SRR0 register */
	u32 spu_status;                         /*!< \brief SPU status */
	u64 spu_cfg;                            /*!< \brief SPU config */
	u32 mb_stat;                            /*!< \brief MB status register */
	u32 ppu_mb;                             /*!< \brief outbound MB register */
	u32 spu_mb[SYS_DBG_SPU_NUM_MB];         /*!< \brief inbound MB queue */
	u32 decrementer;                        /*!< \brief Decrementer register */
	u64 mfc_cq_sr[SYS_DBG_SPU_NUM_MFC_CQ_SR]; /*!< \brief MFC CQ SR registers */
} sys_dbg_spu_thread_context_t;

/*! \brief Function pointer to a PPU exception handler. */
typedef void (*dbg_exception_handler_t)(u64 cause,sys_ppu_thread_t tid,u64 dar);

/*! \brief Set stack size of PPU thread exception handler.
\param stacksize New stack size.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgSetStacksizePPUExceptionHandler(size_t stacksize);

/*! \brief Get the list of SPU thread group ids.
\param group_ids Pointer to storage for the SPU thread group ids.
\param[in] num_ids Pointer to the number of available elements in \p group_ids array.
\param[out] num_ids Number of SPU thread group ids stored into the array.
\param num_all_ids Pointer to storage for the total number of SPU thread groups.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgGetSPUThreadGroupIds(u32 *group_ids,u64 *num_ids,u64 *num_all_ids);

/*! \brief Get the list of PPU thread ids.
\param ids Pointer to storage for the PPU thread ids.
\param[in] num_ids Pointer to the number of available elements in \p ids array.
\param[out] num_ids Number of PPU thread ids stored into the array.
\param num_all_ids Pointer to storage for the total number of PPU threads.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgGetPPUThreadIds(sys_ppu_thread_t *ids,u64 *num_ids,u64 *num_all_ids);

/*! \brief Get the list of SPU thread ids in a SPU thread group.
\param group_id SPU thread group id.
\param thread_ids Pointer to storage for the SPU thread ids.
\param[in] num_ids Pointer to the number of available elements in \p thread_ids array.
\param[out] num_ids Number of SPU thread ids stored into the array.
\param num_all_ids Pointer to storage for the total number of SPU threads in the
group.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgGetSPUThreadIds(u32 group_id,u32 *thread_ids,u64 *num_ids,u64 *num_all_ids);

/*! \brief Register a PPU exception handler.
\param handler Pointer to the exception handler function.
\param flags Flags.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgRegisterPPUExceptionHandler(dbg_exception_handler_t handler,u64 flags);
//s32 sysDbgGetEventQueueInformation();

/*! \brief Get PPU thread name.
\param id PPU thread id.
\param name Pointer to storage for the PPU thread name.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgGetPPUThreadName(sys_ppu_thread_t id,char *name);

/*! \brief Get SPU thread name.
\param thread_id SPU thread id.
\param name Pointer to storage for the SPU thread name.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgGetSPUThreadName(u32 thread_id,char *name);
//s32 sysDbgGetMutexInformation();
//s32 sysDbgGetCondInformation();

/*! \brief Get PPU thread status.
\param id PPU thread id.
\param status Pointer to storage for the PPU thread status.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgGetPPUThreadStatus(sys_ppu_thread_t id,sys_dbg_ppu_thread_status_t *status);
//s32 sysDbgGetRWLockInformation();

/*! \brief Get SPU thread group status.
\param group_id SPU thread group id.
\param status Pointer to storage for the SPU thread group status.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgGetGetSPUThreadGroupStatus(u32 group_id,sys_dbg_spu_thread_group_status_t *status);
//s32 sysDbgGetSemaphoreInformation();

/*! \brief Get SPU thread group name.
\param group_id SPU thread group id.
\param name Pointer to storage for the SPU thread group name.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgGetSPUThreadGroupName(u32 group_id,char *name);

/*! Finalize the PPU exception handler.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgFinalizePPUExceptionHandler();

/*! \brief Read a SPU thread context.
\param thread_id SPU thread id.
\param context Pointer to storage for SPU thread context.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgReadSPUThreadContext(u32 thread_id,sys_dbg_spu_thread_context_t *context);

/*! \brief Initialization of the PPU thread exception handler.
\param prio Priority of the PPU thread exception handler.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgInitializePPUExceptionHandler(s32 prio);

/*! \brief Read a PPU thread context.
\param id PPU thread id.
\param context Pointer to storage for PPU thread context.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgReadPPUThreadContext(sys_ppu_thread_t id,sys_dbg_ppu_thread_context_t *context);

/*! \brief Unregister the PPU thread exception handler.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysDbgUnregisterPPUExceptionHandler();
//s32 sysDbgGetLwMutexInformation();

#ifdef __cplusplus
	}
#endif

#endif
