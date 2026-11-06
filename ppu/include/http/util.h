#ifndef __SYS_HTTP_UTIL_H__
#define __SYS_HTTP_UTIL_H__


#include <ppu-types.h>


#ifdef __cplusplus
extern "C" {
#endif


#define CELL_HTTP_UTIL_URI_FLAG_FULL_URI         (0x00000000)
#define CELL_HTTP_UTIL_URI_FLAG_NO_SCHEME        (0x00000001)
#define CELL_HTTP_UTIL_URI_FLAG_NO_CREDENTIALS   (0x00000002)
#define CELL_HTTP_UTIL_URI_FLAG_NO_PASSWORD      (0x00000004)
#define CELL_HTTP_UTIL_URI_FLAG_NO_PATH          (0x00000008)

#define CELL_HTTP_UTIL_BASE64_ENC_BUF_SIZE(_size) ((_size + 2) / 3 * 4)
#define CELL_HTTP_UTIL_BASE64_DEC_BUF_SIZE(_size) (_size / 4 * 3)


/*
 * structures
 */

typedef struct _http_uri
{
  const char *scheme ATTRIBUTE_PRXPTR;           /* http / https */
  const char *hostname ATTRIBUTE_PRXPTR;         /* hostname */
  const char *username ATTRIBUTE_PRXPTR;
  const char *password ATTRIBUTE_PRXPTR;
  const char *path ATTRIBUTE_PRXPTR;             /* foo.html */
  u32 port;
  u8 _pad[4];
} CellHttpUri;

typedef struct _http_uri_path
{
  const char *path ATTRIBUTE_PRXPTR;
  const char *query ATTRIBUTE_PRXPTR;
  const char *fragment ATTRIBUTE_PRXPTR;
} CellHttpUriPath;

typedef struct _http_request_line
{
  const char *method ATTRIBUTE_PRXPTR;
  const char *path ATTRIBUTE_PRXPTR;
  const char *protocol ATTRIBUTE_PRXPTR;
  u32 majorVersion;
  u32 minorVersion;
} CellHttpRequestLine;

typedef struct _http_status_line
{
  u32 protocol ATTRIBUTE_PRXPTR;
  u32 majorVersion;
  u32 minorVersion;
  u32 reasonPhrase ATTRIBUTE_PRXPTR;
  s32 statusCode;
  u8 _pad[4];
} CellHttpStatusLine;

typedef struct _http_header
{
  const char *name ATTRIBUTE_PRXPTR;
  const char *value ATTRIBUTE_PRXPTR;
} CellHttpHeader;


/*
 * util functions
 */

/* build */
s32 cellHttpUtilBuildRequestLine(const CellHttpRequestLine *req,char *buf,u32 len,u32 *required);
s32 cellHttpUtilBuildHeader(const CellHttpHeader *header,char *buf,u32 len,u32 *required);
s32 cellHttpUtilBuildUri(const CellHttpUri *uri,char *buf,u32 len,u32 *required,int32_t flags);
s32 cellHttpUtilSweepPath(char *dst,const char *src,u32 srcSize);

/* encode */
s32 cellHttpUtilEscapeUri(char *out,u32 outSize,const unsigned char *in,u32 inSize,u32 *required);
s32 cellHttpUtilUnescapeUri(unsigned char *out,u32 size,const char *in,u32 *required);
s32 cellHttpUtilFormUrlEncode(char *out,u32 outSize,const unsigned char *in,u32 inSize,u32 *required);
s32 cellHttpUtilFormUrlDecode(unsigned char *out,u32 size,const char *in,u32 *required);
s32 cellHttpUtilBase64Encoder(char *out,const void *in,u32 len);
s32 cellHttpUtilBase64Decoder(char *out,const void *in,u32 len);

/* copy */
s32 cellHttpUtilCopyUri(CellHttpUri *dest,const CellHttpUri *src,void *pool,u32 poolSize,u32 *required);
s32 cellHttpUtilCopyHeader(CellHttpHeader *dest,const CellHttpHeader *src,void *pool,u32 poolSize,u32 *required);
s32 cellHttpUtilCopyStatusLine(CellHttpStatusLine *dest,const CellHttpStatusLine *src,void *pool,u32 poolSize,u32 *required);
s32 cellHttpUtilMergeUriPath(CellHttpUri *uri,const CellHttpUri *src,const char *path,void *pool,u32 poolSize,u32 *required);
s32 cellHttpUtilAppendHeaderValue(CellHttpHeader *dest,const CellHttpHeader *src,const char *value,void *pool,u32 poolSize,u32 *required);

/* parse */
s32 cellHttpUtilParseUri(CellHttpUri *uri,const char *str,void *pool,u32 size,u32 *required);
s32 cellHttpUtilParseUriPath(CellHttpUriPath *path,const char *str,void *pool,u32 size,u32 *required);
s32 cellHttpUtilParseProxy(CellHttpUri *uri,const char *str,void *pool,u32 size,u32 *required);
s32 cellHttpUtilParseStatusLine(CellHttpStatusLine *resp,const char *str,u32 len,void *pool,u32 size,u32 *required,u32 *parsedLength);
s32 cellHttpUtilParseHeader(CellHttpHeader *header,const char *str,u32 len,void *pool,u32 size,u32 *required,u32 *parsedLength);


#ifdef __cplusplus
	}
#endif

#endif
