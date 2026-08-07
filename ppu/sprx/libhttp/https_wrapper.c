#include <stdio.h>
#include <ppu-asm.h>

#include <http/https.h>


extern s32 cellHttpClientSetSslCallbackEx(CellHttpClientId cid, opd32 *opd, void *userArg);


s32 cellHttpClientSetSslCallback(CellHttpClientId cid, CellHttpsSslCallback cb, void *arg)
{
    printf ( "IN: httpClientSetSslCallback(%d, %p, %p)\n", cid, cb, arg) ;
    printf ( "OUT: httpClientSetSslCallbackEx(%d, %p, %p)\n", cid, (opd32*)__get_opd32(cb), arg) ;
    return cellHttpClientSetSslCallbackEx(cid, (opd32*)__get_opd32(cb), arg);
}

