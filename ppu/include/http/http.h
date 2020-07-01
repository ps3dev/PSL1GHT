#ifndef __HTTP_H__
#define __HTTP_H__


#include <http/util.h>


/* transaction states */
#define HTTP_TRANSACTION_STATE_GETTING_CONNECTION        (1)
#define HTTP_TRANSACTION_STATE_PREPARING_REQUEST         (2)
#define HTTP_TRANSACTION_STATE_SENDING_REQUEST           (3)
#define HTTP_TRANSACTION_STATE_SENDING_BODY              (4)
#define HTTP_TRANSACTION_STATE_WAITING_FOR_REPLY         (5)
#define HTTP_TRANSACTION_STATE_READING_REPLY             (6)
#define HTTP_TRANSACTION_STATE_SETTING_REDIRECTION       (7)
#define HTTP_TRANSACTION_STATE_SETTING_AUTHENTICATION    (8)

/* informational 1xx */
#define HTTP_STATUS_CODE_Continue                        (100)
#define HTTP_STATUS_CODE_Switching_Protocols             (101)
#define HTTP_STATUS_CODE_Processing                      (102) /* RFC2518 WebDAV */

/* successful 2xx */
#define HTTP_STATUS_CODE_OK                              (200)
#define HTTP_STATUS_CODE_Created                         (201)
#define HTTP_STATUS_CODE_Accepted                        (202)
#define HTTP_STATUS_CODE_NonAuthoritive_Information      (203)
#define HTTP_STATUS_CODE_No_Content                      (204)
#define HTTP_STATUS_CODE_Reset_Content                   (205)
#define HTTP_STATUS_CODE_Partial_Content                 (206)
#define HTTP_STATUS_CODE_MultiStatus                     (207) /* RFC2518 WebDAV */

/* redirection 3xx */
#define HTTP_STATUS_CODE_Multiple_Choices                (300)
#define HTTP_STATUS_CODE_Moved_Permanently               (301)
#define HTTP_STATUS_CODE_Moved_Temporarily               (302) /* RFC1945 */
#define HTTP_STATUS_CODE_Found HTTP_STATUS_CODE_Moved_Temporarily /* RFC2616 */
#define HTTP_STATUS_CODE_See_Other                       (303)
#define HTTP_STATUS_CODE_Not_Modified                    (304)
#define HTTP_STATUS_CODE_Use_Proxy                       (305)
#define HTTP_STATUS_CODE_UNUSED                          (306)
#define HTTP_STATUS_CODE_Temporary_Redirect              (307)

/* client error 4xx */
#define HTTP_STATUS_CODE_Bad_Request                     (400)
#define HTTP_STATUS_CODE_Unauthorized                    (401)
#define HTTP_STATUS_CODE_Payment_Required                (402)
#define HTTP_STATUS_CODE_Forbidden                       (403)
#define HTTP_STATUS_CODE_Not_Found                       (404)
#define HTTP_STATUS_CODE_Method_Not_Allowed              (405)
#define HTTP_STATUS_CODE_Not_Acceptable                  (406)
#define HTTP_STATUS_CODE_Proxy_Authentication_Required   (407)
#define HTTP_STATUS_CODE_Request_Timeout                 (408)
#define HTTP_STATUS_CODE_Conflict                        (409)
#define HTTP_STATUS_CODE_Gone                            (410)
#define HTTP_STATUS_CODE_Length_Required                 (411)
#define HTTP_STATUS_CODE_Precondition_Failed             (412)
#define HTTP_STATUS_CODE_Request_Entity_Too_Large        (413)
#define HTTP_STATUS_CODE_RequestURI_Too_Long             (414)
#define HTTP_STATUS_CODE_Unsupported_Media_Type          (415)
#define HTTP_STATUS_CODE_Requested_Range_Not_Satisfiable (416)
#define HTTP_STATUS_CODE_Expectation_Failed              (417)
#define HTTP_STATUS_CODE_Unprocessable_Entity            (422)  /* RFC2518 WebDAV */
#define HTTP_STATUS_CODE_Locked                          (423)  /* RFC2518 WebDAV */
#define HTTP_STATUS_CODE_Failed_Dependency               (424)  /* RFC2518 WebDAV */
#define HTTP_STATUS_CODE_Upgrade_Required                (426)  /* RFC2817 TLS/HTTP */

/* server error 5xx */
#define HTTP_STATUS_CODE_Internal_Server_Error           (500)
#define HTTP_STATUS_CODE_Not_Implemented                 (501)
#define HTTP_STATUS_CODE_Bad_Gateway                     (502)
#define HTTP_STATUS_CODE_Service_Unavailable             (503)
#define HTTP_STATUS_CODE_Gateway_Timeout                 (504)
#define HTTP_STATUS_CODE_HTTP_Version_Not_Supported      (505)
#define HTTP_STATUS_CODE_Insufficient_Storage            (507)  /* RFC2518 WebDAV */

#define CELL_HTTP_MAX_USERNAME                           (256)
#define CELL_HTTP_MAX_PASSWORD                           (256)


#ifdef __cplusplus
extern "C" {
#endif


/*
 * http methods
 */

static const char HTTP_METHOD_OPTIONS[] = "OPTIONS";
static const char HTTP_METHOD_GET[]     = "GET";
static const char HTTP_METHOD_HEAD[]    = "HEAD";
static const char HTTP_METHOD_POST[]    = "POST";
static const char HTTP_METHOD_PUT[]     = "PUT";
static const char HTTP_METHOD_DELETE[]  = "DELETE";
static const char HTTP_METHOD_TRACE[]   = "TRACE";


/*
 * structures
 */

typedef s32 httpClientId;
typedef s32 httpTransId;

typedef s32 httpSslId;


/*
 * callbacks
 */

typedef s32 (*httpAuthenticationCallback)(httpTransId tid,const char *realm,const httpUri *uri,char *user,char *pword,bool *save,void *arg);
typedef s32 (*httpTransactionStateCallback)(httpTransId tid,s32 state,void *arg);
typedef s32 (*httpRedirectCallback)(httpTransId tid,const httpStatusLine *response,const httpUri *from,const httpUri *to,void *arg);
typedef int (*httpCookieSendCallback)(httpTransId tid,const httpUri *uri,const char *cookie,void *arg);
typedef int (*httpCookieRecvCallback)(httpTransId tid,const httpUri *uri,const char *cookie,void *arg);


/*
 * functions
 */

/* initialization */
s32 httpInit(void *pool,u32 poolSize);
s32 httpEnd(void);

/* transaction request */
s32 httpSendRequest(httpTransId tid,const char *buf,u32 size,u32 *sent);
s32 httpRecvResponse(httpTransId tid,char *buf,u32 size,u32 *recvd);

/* proxy */
s32 httpSetProxy(const httpUri *proxy);
s32 httpGetProxy(httpUri *proxy,void *pool,u32 poolSize,u32 *required);

/* request content length */
s32 httpRequestSetContentLength(httpTransId tid,u64 totalSize);
s32 httpRequestGetContentLength(httpTransId tid,u64 *totalSize);

/* request headers */
s32 httpRequestGetAllHeaders(httpTransId tid,httpHeader **headers,u32 *items,void *pool,u32 poolSize,u32 *required);
s32 httpRequestSetHeader(httpTransId tid,const httpHeader *header);
s32 httpRequestGetHeader(httpTransId tid,httpHeader *header,const char *name,void *pool,u32 poolSize,u32 *required);
s32 httpRequestAddHeader(httpTransId tid,const httpHeader *header);
s32 httpRequestDeleteHeader(httpTransId tid,const char *name);

/* response status code */
s32 httpResponseGetStatusCode(httpTransId tid,int32_t *code);

/* response content length */
s32 httpResponseGetContentLength(httpTransId tid,u64 *totalSize);

/* response status line */
s32 httpResponseGetStatusLine(httpTransId tid,httpStatusLine *status,void *pool,u32 poolSize,u32 *required);

/* cookies */
s32 httpInitCookie(void *pool,u32 poolSize);
s32 httpEndCookie(void);
s32 httpAddCookieWithClientId(const httpUri *uri,const char *cookie,httpClientId cid);
s32 httpSessionCookieFlush(httpClientId cid);
s32 httpCookieExportWithClientId(void *buf,u32 size,u32 *exportSize,httpClientId cid);
s32 httpCookieImportWithClientId(const void *buf,u32 size,httpClientId cid);

/* cookie callbacks */
s32 httpClientSetCookieSendCallback(httpClientId cid,httpCookieSendCallback cb,void *arg);
s32 httpClientSetCookieRecvCallback(httpClientId cid,httpCookieRecvCallback cb,void *arg);


/*
 * client functions
 */

/* client create / destroy */
s32 httpCreateClient(httpClientId *cid);
s32 httpDestroyClient(httpClientId cid);

/* proxy */
s32 httpClientSetProxy(httpClientId cid,const httpUri *proxy);
s32 httpClientGetProxy(httpClientId cid,httpUri *proxy,void *pool,u32 poolSize,u32 *required);

/* version */
s32 httpClientSetVersion(httpClientId cid,u32 major,u32 minor);
s32 httpClientGetVersion(httpClientId cid,u32 *major,u32 *minor);

/* pipeline */
s32 httpClientSetPipeline(httpClientId cid,u32 enable);
s32 httpClientGetPipeline(httpClientId cid,u32 *enable);

/* keep alive */
s32 httpClientSetKeepAlive(httpClientId cid,u32 enable);
s32 httpClientGetKeepAlive(httpClientId cid,u32 *enable);

/* redirect */
s32 httpClientSetAutoRedirect(httpClientId cid,u32 enable);
s32 httpClientGetAutoRedirect(httpClientId cid,u32 *enable);

/* authentication */
s32 httpClientSetAutoAuthentication(httpClientId cid,u32 enable);
s32 httpClientGetAutoAuthentication(httpClientId cid,u32 *enable);

/* cache status */
s32 httpClientSetAuthenticationCacheStatus(httpClientId cid,u32 enable);
s32 httpClientGetAuthenticationCacheStatus(httpClientId cid,u32 *enable);

/* cookie status */
s32 httpClientSetCookieStatus(httpClientId cid,u32 enable);
s32 httpClientGetCookieStatus(httpClientId cid,u32 *enable); 

/* user agent */
s32 httpClientSetUserAgent(httpClientId cid,const char *userAgent);
s32 httpClientGetUserAgent(httpClientId cid,char *userAgent,u32 size,u32 *required);

/* buffer max */
s32 httpClientSetResponseBufferMax(httpClientId cid,u32 max);
s32 httpClientGetResponseBufferMax(httpClientId cid,u32 *max);

/* close connections */
s32 httpClientCloseAllConnections(httpClientId cid);
s32 httpClientCloseConnections(httpClientId cid,const httpUri *uri);

/* poll connections */
s32 httpClientPollConnections(httpClientId cid,httpTransId *tid,s64 usec);

/* receive timeout */
s32 httpClientSetRecvTimeout(httpClientId cid,s64 usec);
s32 httpClientGetRecvTimeout(httpClientId cid,s64 *usec);

/* send timeout */
s32 httpClientSetSendTimeout(httpClientId cid,s64 usec);
s32 httpClientGetSendTimeout(httpClientId cid,s64 *usec);

/* connection timeout */
s32 httpClientSetConnTimeout(httpClientId cid,s64 usec);
s32 httpClientGetConnTimeout(httpClientId cid,s64 *usec);

/* pool size */
s32 httpClientSetTotalPoolSize(httpClientId cid,u32 poolSize);
s32 httpClientGetTotalPoolSize(httpClientId cid,u32 *poolSize);
s32 httpClientSetPerHostPoolSize(httpClientId cid,u32 poolSize);
s32 httpClientGetPerHostPoolSize(httpClientId cid,u32 *poolSize);

/* keep alive */
s32 httpClientSetPerHostKeepAliveMax(httpClientId cid,u32 maxSize);
s32 httpClientGetPerHostKeepAliveMax(httpClientId cid,u32 *maxSize);

/* pipeline */
s32 httpClientSetPerPipelineMax(httpClientId cid,u32 pipeMax);
s32 httpClientGetPerPipelineMax(httpClientId cid,u32 *pipeMax);

/* client headers */
s32 httpClientGetAllHeaders(httpClientId cid,httpHeader **headers,u32 *items,void *pool,u32 poolSize,u32 *required);
s32 httpClientSetHeader(httpClientId cid,const httpHeader *header);

s32 httpClientGetHeader(httpClientId cid,httpHeader *header,const char *name,void *pool,u32 poolSize,u32 *required);
s32 httpClientAddHeader(httpClientId cid,const httpHeader *header);
s32 httpClientDeleteHeader(httpClientId cid,const char *name);

/* client callbacks */
s32 httpClientSetAuthenticationCallback(httpClientId cid,httpAuthenticationCallback cb,void *arg);
s32 httpClientSetRedirectCallback(httpClientId cid,httpRedirectCallback cb,void *arg);


/*
 * transactions
 */

/* general transactions */
s32 httpCreateTransaction(httpTransId *tid,httpClientId cid,const char *method,const httpUri *uri);
s32 httpDestroyTransaction(httpTransId tid);
s32 httpTransactionGetUri(httpTransId tid,httpUri *uri,void *pool,u32 poolSize,u32 *required);
s32 httpTransactionCloseConnection(httpTransId tid);
s32 httpTransactionReleaseConnection(httpTransId tid,int *sid);
s32 httpTransactionAbortConnection(httpTransId tid);

/* SSL transactions */
s32 httpTransactionGetSslCipherName(httpTransId tid,char *name,u32 size,u32 *required);
s32 httpTransactionGetSslCipherId(httpTransId tid,int32_t *id);
s32 httpTransactionGetSslCipherVersion(httpTransId tid,char *version,u32 size,u32 *required);
s32 httpTransactionGetSslCipherBits(httpTransId tid,int32_t *effectiveBits,int32_t *algorithmBits);
s32 httpTransactionGetSslCipherString(httpTransId tid,char *buffer,u32 size);
s32 httpTransactionGetSslVersion(httpTransId tid,int32_t *version);
s32 httpTransactionGetSslId(httpTransId tid,httpSslId *id);

/* transaction callback */
s32 httpClientSetTransactionStateCallback(httpClientId cid,httpTransactionStateCallback cb,void *arg);

#ifdef __cplusplus
	}
#endif

#endif

