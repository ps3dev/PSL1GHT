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
} httpsData;


/*
 * callbacks
 */

typedef int (*httpsSslCallback)(s32 verErr,sslCert const sslCerts[],int certNum,const char *host,httpSslId id,void *arg);


/*
 * functions
 */

/* initialization */
s32 httpsInit(u32 caCertNum,const httpsData *caList);
s32 httpsEnd(void);

/* SSL certificate */
s32 httpClientSetSslClientCertificate(httpClientId cid,const httpsData *cert,const httpsData *privKey);

/* SSL callback */
s32 httpClientSetSslCallback(httpClientId cid,httpsSslCallback cb,void *arg);


#ifdef __cplusplus
	}
#endif

#endif

