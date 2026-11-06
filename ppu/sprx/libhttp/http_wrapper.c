#include <stdio.h>
#include <ppu-asm.h>

#include <http/http.h>


extern s32 cellHttpClientSetAuthenticationCallbackEx(CellHttpClientId cid, opd32 *opd, void *arg);
extern s32 cellHttpClientSetTransactionStateCallbackEx(CellHttpClientId cid, opd32 *opd, void *arg);
extern s32 cellHttpClientSetRedirectCallbackEx(CellHttpClientId cid, opd32 *opd, void *arg);
extern s32 cellHttpClientSetCookieSendCallbackEx(CellHttpClientId cid, opd32 *opd, void *arg);
extern s32 cellHttpClientSetCookieRecvCallbackEx(CellHttpClientId cid, opd32 *opd, void *arg);


s32 cellHttpClientSetAuthenticationCallback(CellHttpClientId cid,CellHttpAuthenticationCallback cb,void *arg)
{
    printf ( "IN: cellHttpClientSetAuthenticationStateCallback(%d, %p, %p)\n", cid, cb, arg) ;
    printf ( "OUT: cellHttpClientSetAuthenticationStateCallbackEx(%d, %p, %p)\n", cid, (opd32*)__get_opd32(cb), arg) ;
    return cellHttpClientSetAuthenticationCallbackEx(cid, (opd32*)__get_opd32(cb), arg);
}

s32 cellHttpClientSetTransactionStateCallback(CellHttpClientId cid,CellHttpTransactionStateCallback cb,void *arg)
{
    printf ( "IN: cellHttpClientSetTransactionStateCallback(%d, %p, %p)\n", cid, cb, arg) ;
    printf ( "OUT: cellHttpClientSetTransactionStateCallbackEx(%d, %p, %p)\n", cid, (opd32*)__get_opd32(cb), arg) ;
    return cellHttpClientSetTransactionStateCallbackEx(cid, (opd32*)__get_opd32(cb), arg);
}

s32 cellHttpClientSetRedirectCallback(CellHttpClientId cid,CellHttpRedirectCallback cb,void *arg)
{
    printf ( "IN: cellHttpClientSetRedirectCallback(%d, %p, %p)\n", cid, cb, arg) ;
    printf ( "OUT: cellHttpClientSetRedirectCallbackEx(%d, %p, %p)\n", cid, (opd32*)__get_opd32(cb), arg) ;
    return cellHttpClientSetRedirectCallbackEx(cid, (opd32*)__get_opd32(cb), arg);
}

s32 cellHttpClientSetCookieSendCallback(CellHttpClientId cid, CellHttpCookieSendCallback cb, void *arg)
{
    printf ( "IN: cellHttpClientSetCookieSendCallback(%d, %p, %p)\n", cid, cb, arg ) ;
    printf ( "OUT: cellHttpClientSetCookieSendCallbackEx(%d, %p, %p)\n", cid, (opd32*)__get_opd32(cb), arg) ;
    return cellHttpClientSetCookieSendCallbackEx(cid, (opd32*)__get_opd32(cb), arg);
}

s32 cellHttpClientSetCookieRecvCallback(CellHttpClientId cid, CellHttpCookieRecvCallback cb, void *arg)
{
    printf ( "IN: cellHttpClientSetCookieRecvCallback(%d, %p, %p)\n", cid, cb, arg ) ;
    printf ( "OUT: cellHttpClientSetCookieRecvCallbackEx(%d, %p, %p)\n", cid, (opd32*)__get_opd32(cb), arg) ;
    return cellHttpClientSetCookieRecvCallbackEx(cid, (opd32*)__get_opd32(cb), arg);
}

