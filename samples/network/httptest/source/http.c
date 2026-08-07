#include <http/https.h>
#include <ssl/ssl.h>
#include <net/net.h>
#include <sysmodule/sysmodule.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HTTP_YES		1
#define HTTP_NO 		0
#define HTTP_SUCCESS 	1
#define HTTP_FAILED	 	0

#define HTTP_USER_AGENT "Mozilla/5.0 (PLAYSTATION 3; 1.00)"

typedef struct
{
    void* http_pool;
    void* ssl_pool;
    CellHttpsData* caList;
    void* cert_buffer;
} t_http_pools;

static t_http_pools http_pools;
u8 cancel = HTTP_NO;

static char getBuffer[64*1024];


int http_init(void)
{
    int ret;
	u32 cert_size=0;
	u8 module_https_loaded=HTTP_NO;
	u8 module_http_loaded=HTTP_NO;
	u8 module_net_loaded=HTTP_NO;
	u8 module_ssl_loaded=HTTP_NO;
	
	u8 https_init=HTTP_NO;
	u8 http_init=HTTP_NO;
	u8 net_init=HTTP_NO;
	u8 ssl_init=HTTP_NO;

	//init
	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_NET);
	if (ret < 0) {
		printf("Error : cellSysmoduleLoadModule(CELL_SYSMODULE_NET) HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	} else module_net_loaded=HTTP_YES;

	ret = netInitialize();
	if (ret < 0) {
		printf("Error : netInitialize HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	} else net_init=HTTP_YES;

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_HTTP);
	if (ret < 0) {
		printf("Error : cellSysmoduleLoadModule(CELL_SYSMODULE_HTTP) HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	} else module_http_loaded=HTTP_YES;

	http_pools.http_pool = malloc(0x10000);
	if (http_pools.http_pool == NULL) {
		printf("Error : out of memory (http_pool)");
		ret=HTTP_FAILED;
		goto end;
	}

	ret = cellHttpInit(http_pools.http_pool, 0x10000);
	if (ret < 0) {
		printf("Error : cellHttpInit HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	} else http_init=HTTP_YES;

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_HTTPS);
	if (ret < 0) {
		printf("Error : cellSysmoduleLoadModule(CELL_SYSMODULE_HTTP) HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	} else module_https_loaded=HTTP_YES;

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_SSL);
	if (ret < 0) {
		printf("Error : cellSysmoduleLoadModule(CELL_SYSMODULE_HTTP) HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	} else module_ssl_loaded=HTTP_YES;

	http_pools.ssl_pool = malloc(0x40000);
	if (http_pools.ssl_pool == NULL) {
		printf("Error : out of memory (ssl_pool)");
		ret=HTTP_FAILED;
		goto end;
	}

	ret = cellSslInit(http_pools.ssl_pool, 0x40000);
	if (ret < 0) {
		printf("Error : cellSslInit HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	} else ssl_init=HTTP_YES;

	http_pools.caList = (CellHttpsData *)malloc(sizeof(CellHttpsData));
	ret = cellSslCertificateLoader(CELL_SLL_LOAD_CERT_ALL, NULL, 0, &cert_size);
	if (ret < 0) {
		printf("Error : cellSslCertificateLoader HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	}

	http_pools.cert_buffer = malloc(cert_size);
	if (http_pools.cert_buffer==NULL) {
		printf("Error : out of memory (cert_buffer)");
		ret=HTTP_FAILED;
		goto end;
	}

	ret = cellSslCertificateLoader(CELL_SLL_LOAD_CERT_ALL, http_pools.cert_buffer, cert_size, NULL);
	if (ret < 0) {
		printf("Error : cellSslCertificateLoader HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	}

	(&http_pools.caList[0])->ptr = http_pools.cert_buffer;
	(&http_pools.caList[0])->size = cert_size;

	ret = cellHttpsInit(1, (CellHttpsData *) http_pools.caList);
	if (ret < 0) {
		printf("Error : cellHttpsInit HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	} else https_init=HTTP_YES;

	return HTTP_SUCCESS;

end:
	if(http_pools.caList) free(http_pools.caList);
	if(https_init) cellHttpsEnd();
	if(ssl_init) cellSslEnd();
	if(http_init) cellHttpEnd();
	if(net_init) netDeinitialize();
	
	if(module_http_loaded) cellSysmoduleUnloadModule(CELL_SYSMODULE_HTTP);
	if(module_https_loaded) cellSysmoduleUnloadModule(CELL_SYSMODULE_HTTPS);
	if(module_ssl_loaded) cellSysmoduleUnloadModule(CELL_SYSMODULE_SSL);
	if(module_net_loaded) cellSysmoduleUnloadModule(CELL_SYSMODULE_NET);
	
	if(http_pools.http_pool) free(http_pools.http_pool);
	if(http_pools.ssl_pool) free(http_pools.ssl_pool);
	if(http_pools.cert_buffer) free(http_pools.cert_buffer);
	
	return ret;
}

void http_end(void)
{
	if(http_pools.caList) free(http_pools.caList);
	cellHttpsEnd();
	cellSslEnd();
	cellHttpEnd();
	netDeinitialize();
	
	cellSysmoduleUnloadModule(CELL_SYSMODULE_HTTP);
	cellSysmoduleUnloadModule(CELL_SYSMODULE_HTTPS);
	cellSysmoduleUnloadModule(CELL_SYSMODULE_SSL);
	cellSysmoduleUnloadModule(CELL_SYSMODULE_NET);

	if(http_pools.http_pool) free(http_pools.http_pool);
	if(http_pools.ssl_pool) free(http_pools.ssl_pool);
	if(http_pools.cert_buffer) free(http_pools.cert_buffer);
	
	return;
}

char* escape_filename(const char* filename)
{
	int len = strlen(filename);
    char* ret = (char *)calloc(1, len*3);

	cellHttpUtilEscapeUri(ret, len*3, (uint8_t*) filename, len, 0);

	return ret;
}

int http_download(const char* url, const char* filename, const char* local_dst)
{
	int ret = 0, httpCode = 0;
	CellHttpUri uri;
	CellHttpClientId httpClient = 0;
	CellHttpTransId httpTrans = 0;
	FILE* fp=NULL;
	u32 nRecv = 1;
	u32 size = 0;
	uint64_t length = 0;
	void *uri_pool = NULL;
	char* escaped_name = NULL;
	char* escaped_url = NULL;

	ret = cellHttpCreateClient(&httpClient);
	if (ret < 0) {
		printf("Error : cellHttpCreateClient HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	}
    cellHttpClientSetConnTimeout(httpClient, 10 * 1000 * 1000);
    cellHttpClientSetUserAgent(httpClient, HTTP_USER_AGENT);
    cellHttpClientSetAutoRedirect(httpClient, 1);

	// Escape URL file name characters
	escaped_name = escape_filename(filename);
	asprintf(&escaped_url, "%s%s", url, escaped_name);
	
	printf("Downloading (%s) -> (%s)", escaped_url, local_dst);

	//URI
	ret = cellHttpUtilParseUri(&uri, escaped_url, NULL, 0, &size);
	if (ret < 0) {
		printf("Error : cellHttpUtilParseUri() HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	}

	uri_pool = malloc(size);
	if (uri_pool == NULL) {
		printf("Error : out of memory (uri_pool)");
		ret=HTTP_FAILED;
		goto end;
	}

	ret = cellHttpUtilParseUri(&uri, escaped_url, uri_pool, size, 0);
	if (ret < 0) {
		printf("Error : cellHttpUtilParseUri() HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	}
	//END of URI	

	//SEND REQUEST
	ret = cellHttpCreateTransaction(&httpTrans, httpClient, CELL_HTTP_METHOD_GET, &uri);
	if (ret < 0) {
		printf("Error : cellHttpCreateTransaction() HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	}

	ret = cellHttpSendRequest(httpTrans, NULL, 0, NULL);
	if (ret < 0) {
		printf("Error : cellHttpSendRequest() HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	}
	
	//GET SIZE
	cellHttpResponseGetContentLength(httpTrans, &length);

	ret = cellHttpResponseGetStatusCode(httpTrans, &httpCode);
	if (ret < 0) {
		printf("Error : cellHttpResponseGetStatusCode() HTTP_FAILED (%x)", ret);
		ret=HTTP_FAILED;
		goto end;
	}

	if(httpCode != CELL_HTTP_STATUS_CODE_OK && httpCode >= 400 ) {
		printf("Error : Status code (%d)", httpCode);
		ret=HTTP_FAILED;
		goto end;
	}
	
	//TRANSFER
	fp = fopen(local_dst, "wb");
	if(fp == NULL) {
		printf("Error : fopen() HTTP_FAILED : %s", local_dst);
		ret=HTTP_FAILED;
		goto end;
	}
	
	while(nRecv != 0) {
		if(cellHttpRecvResponse(httpTrans, (void*) getBuffer, sizeof(getBuffer)-1, &nRecv) > 0) break;
		if(nRecv == 0)	break;
		fwrite((char*) getBuffer, nRecv, 1, fp);
		if(cancel==HTTP_YES) break;
	}
	fclose(fp);
	
	if(cancel==HTTP_YES) {
		unlink((char*)local_dst);
		ret=HTTP_FAILED;
		cancel=HTTP_NO;
	}

	//END of TRANSFER
	ret=HTTP_SUCCESS;

end:
	if(httpTrans) cellHttpDestroyTransaction(httpTrans);
	if(httpClient) cellHttpDestroyClient(httpClient);
	if(uri_pool) free(uri_pool);
	if(escaped_url) free(escaped_url);
	if(escaped_name) free(escaped_name);

	return ret;
}

s32 main(s32 argc, const char* argv[])
{
	if (http_init() == HTTP_SUCCESS)
	{
		http_download("https://google.com/", "robots.txt", "/dev_hdd0/tmp/file.txt");
		http_end();
	}
	return 0;
}
