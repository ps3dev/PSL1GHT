#ifndef __HTTP_H__
#define __HTTP_H__


#include <http/util.h>


/* transaction states */
#define CELL_HTTP_TRANSACTION_STATE_GETTING_CONNECTION        (1)
#define CELL_HTTP_TRANSACTION_STATE_PREPARING_REQUEST         (2)
#define CELL_HTTP_TRANSACTION_STATE_SENDING_REQUEST           (3)
#define CELL_HTTP_TRANSACTION_STATE_SENDING_BODY              (4)
#define CELL_HTTP_TRANSACTION_STATE_WAITING_FOR_REPLY         (5)
#define CELL_HTTP_TRANSACTION_STATE_READING_REPLY             (6)
#define CELL_HTTP_TRANSACTION_STATE_SETTING_REDIRECTION       (7)
#define CELL_HTTP_TRANSACTION_STATE_SETTING_AUTHENTICATION    (8)

/* informational 1xx */
#define CELL_HTTP_STATUS_CODE_Continue                        (100)
#define CELL_HTTP_STATUS_CODE_Switching_Protocols             (101)
#define CELL_HTTP_STATUS_CODE_Processing                      (102) /* RFC2518 WebDAV */

/* successful 2xx */
#define CELL_HTTP_STATUS_CODE_OK                              (200)
#define CELL_HTTP_STATUS_CODE_Created                         (201)
#define CELL_HTTP_STATUS_CODE_Accepted                        (202)
#define CELL_HTTP_STATUS_CODE_NonAuthoritive_Information      (203)
#define CELL_HTTP_STATUS_CODE_No_Content                      (204)
#define CELL_HTTP_STATUS_CODE_Reset_Content                   (205)
#define CELL_HTTP_STATUS_CODE_Partial_Content                 (206)
#define CELL_HTTP_STATUS_CODE_MultiStatus                     (207) /* RFC2518 WebDAV */

/* redirection 3xx */
#define CELL_HTTP_STATUS_CODE_Multiple_Choices                (300)
#define CELL_HTTP_STATUS_CODE_Moved_Permanently               (301)
#define CELL_HTTP_STATUS_CODE_Moved_Temporarily               (302) /* RFC1945 */
#define CELL_HTTP_STATUS_CODE_Found HTTP_STATUS_CODE_Moved_Temporarily /* RFC2616 */
#define CELL_HTTP_STATUS_CODE_See_Other                       (303)
#define CELL_HTTP_STATUS_CODE_Not_Modified                    (304)
#define CELL_HTTP_STATUS_CODE_Use_Proxy                       (305)
#define CELL_HTTP_STATUS_CODE_UNUSED                          (306)
#define CELL_HTTP_STATUS_CODE_Temporary_Redirect              (307)

/* client error 4xx */
#define CELL_HTTP_STATUS_CODE_Bad_Request                     (400)
#define CELL_HTTP_STATUS_CODE_Unauthorized                    (401)
#define CELL_HTTP_STATUS_CODE_Payment_Required                (402)
#define CELL_HTTP_STATUS_CODE_Forbidden                       (403)
#define CELL_HTTP_STATUS_CODE_Not_Found                       (404)
#define CELL_HTTP_STATUS_CODE_Method_Not_Allowed              (405)
#define CELL_HTTP_STATUS_CODE_Not_Acceptable                  (406)
#define CELL_HTTP_STATUS_CODE_Proxy_Authentication_Required   (407)
#define CELL_HTTP_STATUS_CODE_Request_Timeout                 (408)
#define CELL_HTTP_STATUS_CODE_Conflict                        (409)
#define CELL_HTTP_STATUS_CODE_Gone                            (410)
#define CELL_HTTP_STATUS_CODE_Length_Required                 (411)
#define CELL_HTTP_STATUS_CODE_Precondition_Failed             (412)
#define CELL_HTTP_STATUS_CODE_Request_Entity_Too_Large        (413)
#define CELL_HTTP_STATUS_CODE_RequestURI_Too_Long             (414)
#define CELL_HTTP_STATUS_CODE_Unsupported_Media_Type          (415)
#define CELL_HTTP_STATUS_CODE_Requested_Range_Not_Satisfiable (416)
#define CELL_HTTP_STATUS_CODE_Expectation_Failed              (417)
#define CELL_HTTP_STATUS_CODE_Unprocessable_Entity            (422)  /* RFC2518 WebDAV */
#define CELL_HTTP_STATUS_CODE_Locked                          (423)  /* RFC2518 WebDAV */
#define CELL_HTTP_STATUS_CODE_Failed_Dependency               (424)  /* RFC2518 WebDAV */
#define CELL_HTTP_STATUS_CODE_Upgrade_Required                (426)  /* RFC2817 TLS/HTTP */

/* server error 5xx */
#define CELL_HTTP_STATUS_CODE_Internal_Server_Error           (500)
#define CELL_HTTP_STATUS_CODE_Not_Implemented                 (501)
#define CELL_HTTP_STATUS_CODE_Bad_Gateway                     (502)
#define CELL_HTTP_STATUS_CODE_Service_Unavailable             (503)
#define CELL_HTTP_STATUS_CODE_Gateway_Timeout                 (504)
#define CELL_HTTP_STATUS_CODE_HTTP_Version_Not_Supported      (505)
#define CELL_HTTP_STATUS_CODE_Insufficient_Storage            (507)  /* RFC2518 WebDAV */

#define CELL_HTTP_MAX_USERNAME                           (256)
#define CELL_HTTP_MAX_PASSWORD                           (256)


#ifdef __cplusplus
extern "C" {
#endif


/*
 * http methods
 */

static const char CELL_HTTP_METHOD_OPTIONS[] = "OPTIONS";
static const char CELL_HTTP_METHOD_GET[]     = "GET";
static const char CELL_HTTP_METHOD_HEAD[]    = "HEAD";
static const char CELL_HTTP_METHOD_POST[]    = "POST";
static const char CELL_HTTP_METHOD_PUT[]     = "PUT";
static const char CELL_HTTP_METHOD_DELETE[]  = "DELETE";
static const char CELL_HTTP_METHOD_TRACE[]   = "TRACE";


/*
 * structures
 */

typedef s32 CellHttpClientId;
typedef s32 CellHttpTransId;

typedef s32 CellHttpSslId;


/*
 * callbacks
 */

typedef s32 (*CellHttpAuthenticationCallback)(CellHttpTransId tid,const char *realm,const CellHttpUri *uri,char *user,char *pword,bool *save,void *arg);
typedef s32 (*CellHttpTransactionStateCallback)(CellHttpTransId tid,s32 state,void *arg);
typedef s32 (*CellHttpRedirectCallback)(CellHttpTransId tid,const CellHttpStatusLine *response,const CellHttpUri *from,const CellHttpUri *to,void *arg);
typedef int (*CellHttpCookieSendCallback)(CellHttpTransId tid,const CellHttpUri *uri,const char *cookie,void *arg);
typedef int (*CellHttpCookieRecvCallback)(CellHttpTransId tid,const CellHttpUri *uri,const char *cookie,void *arg);


/*
 * functions
 */

/* initialization */
s32 cellHttpInit(void *pool,u32 poolSize);
s32 cellHttpEnd(void);

/* transaction request */
s32 cellHttpSendRequest(CellHttpTransId tid,const char *buf,u32 size,u32 *sent);
s32 cellHttpRecvResponse(CellHttpTransId tid,char *buf,u32 size,u32 *recvd);

/* proxy */
s32 cellHttpSetProxy(const CellHttpUri *proxy);
s32 cellHttpGetProxy(CellHttpUri *proxy,void *pool,u32 poolSize,u32 *required);

/* request content length */
s32 cellHttpRequestSetContentLength(CellHttpTransId tid,u64 totalSize);
s32 cellHttpRequestGetContentLength(CellHttpTransId tid,u64 *totalSize);

/* request headers */
s32 cellHttpRequestGetAllHeaders(CellHttpTransId tid,CellHttpHeader **headers,u32 *items,void *pool,u32 poolSize,u32 *required);
s32 cellHttpRequestSetHeader(CellHttpTransId tid,const CellHttpHeader *header);
s32 cellHttpRequestGetHeader(CellHttpTransId tid,CellHttpHeader *header,const char *name,void *pool,u32 poolSize,u32 *required);
s32 cellHttpRequestAddHeader(CellHttpTransId tid,const CellHttpHeader *header);
s32 cellHttpRequestDeleteHeader(CellHttpTransId tid,const char *name);

/* response status code */
s32 cellHttpResponseGetStatusCode(CellHttpTransId tid,int32_t *code);

/* response content length */
s32 cellHttpResponseGetContentLength(CellHttpTransId tid,u64 *totalSize);

/* response status line */
s32 cellHttpResponseGetStatusLine(CellHttpTransId tid,CellHttpStatusLine *status,void *pool,u32 poolSize,u32 *required);

/* cookies */
s32 cellHttpInitCookie(void *pool,u32 poolSize);
s32 cellHttpEndCookie(void);
s32 cellHttpAddCookieWithClientId(const CellHttpUri *uri,const char *cookie,CellHttpClientId cid);
s32 cellHttpSessionCookieFlush(CellHttpClientId cid);
s32 cellHttpCookieExportWithClientId(void *buf,u32 size,u32 *exportSize,CellHttpClientId cid);
s32 cellHttpCookieImportWithClientId(const void *buf,u32 size,CellHttpClientId cid);

/* cookie callbacks */
s32 cellHttpClientSetCookieSendCallback(CellHttpClientId cid,CellHttpCookieSendCallback cb,void *arg);
s32 cellHttpClientSetCookieRecvCallback(CellHttpClientId cid,CellHttpCookieRecvCallback cb,void *arg);


/*
 * client functions
 */

/* client create / destroy */
s32 cellHttpCreateClient(CellHttpClientId *cid);
s32 cellHttpDestroyClient(CellHttpClientId cid);

/* proxy */
s32 cellHttpClientSetProxy(CellHttpClientId cid,const CellHttpUri *proxy);
s32 cellHttpClientGetProxy(CellHttpClientId cid,CellHttpUri *proxy,void *pool,u32 poolSize,u32 *required);

/* version */
s32 cellHttpClientSetVersion(CellHttpClientId cid,u32 major,u32 minor);
s32 cellHttpClientGetVersion(CellHttpClientId cid,u32 *major,u32 *minor);

/* pipeline */
s32 cellHttpClientSetPipeline(CellHttpClientId cid,u32 enable);
s32 cellHttpClientGetPipeline(CellHttpClientId cid,u32 *enable);

/* keep alive */
s32 cellHttpClientSetKeepAlive(CellHttpClientId cid,u32 enable);
s32 cellHttpClientGetKeepAlive(CellHttpClientId cid,u32 *enable);

/* redirect */
s32 cellHttpClientSetAutoRedirect(CellHttpClientId cid,u32 enable);
s32 cellHttpClientGetAutoRedirect(CellHttpClientId cid,u32 *enable);

/* authentication */
s32 cellHttpClientSetAutoAuthentication(CellHttpClientId cid,u32 enable);
s32 cellHttpClientGetAutoAuthentication(CellHttpClientId cid,u32 *enable);

/* cache status */
s32 cellHttpClientSetAuthenticationCacheStatus(CellHttpClientId cid,u32 enable);
s32 cellHttpClientGetAuthenticationCacheStatus(CellHttpClientId cid,u32 *enable);

/* cookie status */
s32 cellHttpClientSetCookieStatus(CellHttpClientId cid,u32 enable);
s32 cellHttpClientGetCookieStatus(CellHttpClientId cid,u32 *enable); 

/* user agent */
s32 cellHttpClientSetUserAgent(CellHttpClientId cid,const char *userAgent);
s32 cellHttpClientGetUserAgent(CellHttpClientId cid,char *userAgent,u32 size,u32 *required);

/* buffer max */
s32 cellHttpClientSetResponseBufferMax(CellHttpClientId cid,u32 max);
s32 cellHttpClientGetResponseBufferMax(CellHttpClientId cid,u32 *max);

/* close connections */
s32 cellHttpClientCloseAllConnections(CellHttpClientId cid);
s32 cellHttpClientCloseConnections(CellHttpClientId cid,const CellHttpUri *uri);

/* poll connections */
s32 cellHttpClientPollConnections(CellHttpClientId cid,CellHttpTransId *tid,s64 usec);

/* receive timeout */
s32 cellHttpClientSetRecvTimeout(CellHttpClientId cid,s64 usec);
s32 cellHttpClientGetRecvTimeout(CellHttpClientId cid,s64 *usec);

/* send timeout */
s32 cellHttpClientSetSendTimeout(CellHttpClientId cid,s64 usec);
s32 cellHttpClientGetSendTimeout(CellHttpClientId cid,s64 *usec);

/* connection timeout */
s32 cellHttpClientSetConnTimeout(CellHttpClientId cid,s64 usec);
s32 cellHttpClientGetConnTimeout(CellHttpClientId cid,s64 *usec);

/* pool size */
s32 cellHttpClientSetTotalPoolSize(CellHttpClientId cid,u32 poolSize);
s32 cellHttpClientGetTotalPoolSize(CellHttpClientId cid,u32 *poolSize);
s32 cellHttpClientSetPerHostPoolSize(CellHttpClientId cid,u32 poolSize);
s32 cellHttpClientGetPerHostPoolSize(CellHttpClientId cid,u32 *poolSize);

/* keep alive */
s32 cellHttpClientSetPerHostKeepAliveMax(CellHttpClientId cid,u32 maxSize);
s32 cellHttpClientGetPerHostKeepAliveMax(CellHttpClientId cid,u32 *maxSize);

/* pipeline */
s32 cellHttpClientSetPerPipelineMax(CellHttpClientId cid,u32 pipeMax);
s32 cellHttpClientGetPerPipelineMax(CellHttpClientId cid,u32 *pipeMax);

/* client headers */
s32 cellHttpClientGetAllHeaders(CellHttpClientId cid,CellHttpHeader **headers,u32 *items,void *pool,u32 poolSize,u32 *required);
s32 cellHttpClientSetHeader(CellHttpClientId cid,const CellHttpHeader *header);

s32 cellHttpClientGetHeader(CellHttpClientId cid,CellHttpHeader *header,const char *name,void *pool,u32 poolSize,u32 *required);
s32 cellHttpClientAddHeader(CellHttpClientId cid,const CellHttpHeader *header);
s32 cellHttpClientDeleteHeader(CellHttpClientId cid,const char *name);

/* client callbacks */
s32 cellHttpClientSetAuthenticationCallback(CellHttpClientId cid,CellHttpAuthenticationCallback cb,void *arg);
s32 cellHttpClientSetRedirectCallback(CellHttpClientId cid,CellHttpRedirectCallback cb,void *arg);


/*
 * transactions
 */

/* general transactions */
s32 cellHttpCreateTransaction(CellHttpTransId *tid,CellHttpClientId cid,const char *method,const CellHttpUri *uri);
s32 cellHttpDestroyTransaction(CellHttpTransId tid);
s32 cellHttpTransactionGetUri(CellHttpTransId tid,CellHttpUri *uri,void *pool,u32 poolSize,u32 *required);
s32 cellHttpTransactionCloseConnection(CellHttpTransId tid);
s32 cellHttpTransactionReleaseConnection(CellHttpTransId tid,int *sid);
s32 cellHttpTransactionAbortConnection(CellHttpTransId tid);

/* SSL transactions */
s32 cellHttpTransactionGetSslCipherName(CellHttpTransId tid,char *name,u32 size,u32 *required);
s32 cellHttpTransactionGetSslCipherId(CellHttpTransId tid,int32_t *id);
s32 cellHttpTransactionGetSslCipherVersion(CellHttpTransId tid,char *version,u32 size,u32 *required);
s32 cellHttpTransactionGetSslCipherBits(CellHttpTransId tid,int32_t *effectiveBits,int32_t *algorithmBits);
s32 cellHttpTransactionGetSslCipherString(CellHttpTransId tid,char *buffer,u32 size);
s32 cellHttpTransactionGetSslVersion(CellHttpTransId tid,int32_t *version);
s32 cellHttpTransactionGetSslId(CellHttpTransId tid,CellHttpSslId *id);

/* transaction callback */
s32 cellHttpClientSetTransactionStateCallback(CellHttpClientId cid,CellHttpTransactionStateCallback cb,void *arg);

#ifdef __cplusplus
	}
#endif

#endif

