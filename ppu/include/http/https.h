#ifndef __HTTPS_H__
#define __HTTPS_H__


#include <http/http.h>
#include <ssl/ssl.h>


#ifdef __cplusplus
extern "C" {
#endif


/*
 * structures
 */

typedef struct _https_data
{
  char *ptr ATTRIBUTE_PRXPTR;
  u32 size;
} CellHttpsData;


/*
 * callbacks
 */

typedef int (*CellHttpsSslCallback)(s32 verErr,CellSslCert const sslCerts[],int certNum,const char *host,CellHttpSslId id,void *arg);


/*
 * functions
 */

/* initialization */
s32 cellHttpsInit(u32 caCertNum,const CellHttpsData *caList);
s32 cellHttpsEnd(void);

/* SSL certificate */
s32 cellHttpClientSetSslClientCertificate(CellHttpClientId cid,const CellHttpsData *cert,const CellHttpsData *privKey);

/* SSL callback */
s32 cellHttpClientSetSslCallback(CellHttpClientId cid,CellHttpsSslCallback cb,void *arg);


#ifdef __cplusplus
	}
#endif

#endif

