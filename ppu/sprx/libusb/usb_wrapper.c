#include <stdio.h>
#include <ppu-asm.h>
#include <usb/usb.h>

typedef struct _usb_ldd_ops_ex
{
  const char *name ATTRIBUTE_PRXPTR;
  opd32 *probe ATTRIBUTE_PRXPTR;
  opd32 *attach ATTRIBUTE_PRXPTR;
  opd32 *detach ATTRIBUTE_PRXPTR;
} CellUsbLddOpsEx;

extern s32 cellUsbdControlTransferEx(s32 pipe_id, CellUsbdDeviceRequest *req, void *buf, opd32 *opd, void *arg);
extern s32 cellUsbdBulkTransferEx(s32 pipe_id, void *buf, s32 len, opd32 *opd, void *arg);
extern s32 cellUsbdInterruptTransferEx(s32 pipe_id, void *buf, s32 len, opd32 *opd, void *arg);
extern s32 cellUsbdIsochronousTransferEx(s32 pipe_id, CellUsbdIsochRequest *req, opd32 *opd, void *arg);
extern s32 cellUsbdHSIsochronousTransferEx(s32 pipe_id, CellUsbdHSIsochRequest *req, opd32 *opd, void *arg);

extern s32 cellUsbdUnregisterLddEx(CellUsbLddOpsEx *lddops);
extern s32 cellUsbRegisterLddEx(CellUsbLddOpsEx *lddops);
extern s32 cellUsbdRegisterExtraLddEx(CellUsbLddOpsEx *lddops, u16 vendor_id, u16 product_id);
extern s32 cellUsbdRegisterExtraLdd2Ex(CellUsbLddOpsEx *lddops, u16 vendor_id, u16 product_id_min, u16 product_id_max);
extern s32 cellUsbdUnregisterExtraLddEx(CellUsbLddOpsEx *lddops);


s32 cellUsbdControlTransfer(s32 pipe_id, CellUsbdDeviceRequest *req, void *buf, CellUsbdDoneCallback cb, void *arg)
{
/*
    printf ( "IN: cellUsbdControlTransfer(%d, %p, %p, %p, %p)\n", pipe_id, req, buf, cb, arg ) ;
    printf ( "OUT: cellUsbdControlTransferEx(%d, %p, %p, %p, %p)\n", pipe_id, req, buf, (opd32*)__get_opd32(cb), arg ) ;
*/
    return cellUsbdControlTransferEx(pipe_id, req, buf, (opd32*)__get_opd32(cb), arg);
}

s32 cellUsbdBulkTransfer(s32 pipe_id, void *buf, s32 len, CellUsbdDoneCallback cb, void *arg)
{
    return cellUsbdBulkTransferEx(pipe_id, buf, len, (opd32*)__get_opd32(cb), arg);
}

s32 cellUsbdInterruptTransfer(s32 pipe_id, void *buf, s32 len, CellUsbdDoneCallback cb, void *arg)
{
    return cellUsbdInterruptTransferEx(pipe_id, buf, len, (opd32*)__get_opd32(cb), arg);
}

s32 cellUsbdIsochronousTransfer(s32 pipe_id, CellUsbdIsochRequest *req, CellUsbdIsochDoneCallback cb, void *arg)
{
    return cellUsbdIsochronousTransferEx(pipe_id, req, (opd32*)__get_opd32(cb), arg);
}

s32 cellUsbdHSIsochronousTransfer(s32 pipe_id, CellUsbdHSIsochRequest *req, CellUsbdHSIsochDoneCallback cb, void *arg)
{
    return cellUsbdHSIsochronousTransferEx(pipe_id, req, (opd32*)__get_opd32(cb), arg);
}

s32 cellUsbdRegisterLdd(CellUsbdLddOps *ldd_ops)
{
    CellUsbLddOpsEx ldd_ops_ex =
    {
        "desc",
        (opd32*)__get_opd32(ldd_ops->probe),
        (opd32*)__get_opd32(ldd_ops->attach),
        (opd32*)__get_opd32(ldd_ops->detach)
    };
/*
    printf ( "IN:  cellUsbdRegisterLdd(%p) = probe=%p, attach=%p, detach=%p\n", ldd_ops, ldd_ops->probe, ldd_ops->attach, ldd_ops->detach ) ;
    printf ( "OUT: cellUsbdRegisterLddEx(%p) = probe=%p, attach=%p, detach=%p\n", &ldd_ops_ex, ldd_ops_ex.probe, ldd_ops_ex.attach, ldd_ops_ex.detach ) ;
*/
    return cellUsbRegisterLddEx(&ldd_ops_ex);
}

s32 cellUsbUnregisterLdd(CellUsbdLddOps *ldd_ops)
{
    CellUsbLddOpsEx ldd_ops_ex =
    {
        "desc",
        (opd32*)__get_opd32(ldd_ops->probe),
        (opd32*)__get_opd32(ldd_ops->attach),
        (opd32*)__get_opd32(ldd_ops->detach)
    };
/*
    printf ( "IN:  cellUsbdUnregisterLdd(%p) = probe=%p, attach=%p, detach=%p\n", ldd_ops, ldd_ops->probe, ldd_ops->attach, ldd_ops->detach ) ;
    printf ( "OUT: cellUsbdUnregisterLddEx(%p) = probe=%p, attach=%p, detach=%p\n", &ldd_ops_ex, ldd_ops_ex.probe, ldd_ops_ex.attach, ldd_ops_ex.detach ) ;
*/
    return cellUsbdUnregisterLddEx(&ldd_ops_ex);
}

s32 cellUsbdRegisterExtraLdd(CellUsbdLddOps *ldd_ops, u16 vendor_id, u16 product_id)
{
    CellUsbLddOpsEx ldd_ops_ex =
    {
        "desc",
        (opd32*)__get_opd32(ldd_ops->probe),
        (opd32*)__get_opd32(ldd_ops->attach),
        (opd32*)__get_opd32(ldd_ops->detach)
    };
/*
    printf ( "IN:  cellUsbdRegisterExtraLdd(%p) = probe=%p, attach=%p, detach=%p\n", ldd_ops, ldd_ops->probe, ldd_ops->attach, ldd_ops->detach ) ;
    printf ( "OUT: cellUsbdRegisterExtraLddEx(%p) = probe=%p, attach=%p, detach=%p\n", &ldd_ops_ex, ldd_ops_ex.probe, ldd_ops_ex.attach, ldd_ops_ex.detach ) ;
*/
    return cellUsbdRegisterExtraLddEx(&ldd_ops_ex, vendor_id, product_id);
}

s32 cellUsbdRegisterExtraLdd2(CellUsbdLddOps *ldd_ops, u16 vendor_id, u16 product_id_min, u16 product_id_max)
{
    CellUsbLddOpsEx ldd_ops_ex =
    {
        "desc",
        (opd32*)__get_opd32(ldd_ops->probe),
        (opd32*)__get_opd32(ldd_ops->attach),
        (opd32*)__get_opd32(ldd_ops->detach)
    };
/*
    printf ( "IN:  cellUsbdRegisterExtraLdd2(%p) = probe=%p, attach=%p, detach=%p\n", ldd_ops, ldd_ops->probe, ldd_ops->attach, ldd_ops->detach ) ;
    printf ( "OUT: cellUsbdRegisterExtraLdd2Ex(%p) = probe=%p, attach=%p, detach=%p\n", &ldd_ops_ex, ldd_ops_ex.probe, ldd_ops_ex.attach, ldd_ops_ex.detach ) ;
*/
    return cellUsbdRegisterExtraLdd2Ex(&ldd_ops_ex, vendor_id, product_id_min, product_id_max);
}

s32 cellUsbdUnregisterExtraLdd(CellUsbdLddOps *ldd_ops)
{
    CellUsbLddOpsEx ldd_ops_ex =
    {
        "desc",
        (opd32*)__get_opd32(ldd_ops->probe),
        (opd32*)__get_opd32(ldd_ops->attach),
        (opd32*)__get_opd32(ldd_ops->detach)
    };
/*
    printf ( "IN:  cellUsbdUnregisterExtraLdd(%p) = probe=%p, attach=%p, detach=%p\n", ldd_ops, ldd_ops->probe, ldd_ops->attach, ldd_ops->detach ) ;
    printf ( "OUT: cellUsbdUnregisterExtraLddEx(%p) = probe=%p, attach=%p, detach=%p\n", &ldd_ops_ex, ldd_ops_ex.probe, ldd_ops_ex.attach, ldd_ops_ex.detach ) ;
*/
    return cellUsbdUnregisterExtraLddEx(&ldd_ops_ex);
}

