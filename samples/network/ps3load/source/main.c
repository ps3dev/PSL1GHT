#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <malloc.h>
#include <fcntl.h>
#include <ppu-types.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/socket.h>
#include <net/net.h>

#include <sys/thread.h>
#include <sys/process.h>
#include <sysutil/sysutil.h>
#include <sysmodule/sysmodule.h>

#include <zlib.h>
#include <zip.h>

#include "pad.h"
#include "rsxutil.h"

#define PS3LOADVER		"v0.3"

#define PORT 4299
#define MAX_ARG_CNT		256

#define STACKSIZE		(256*1024)

#define MIN(a,b)		((a)<(b) ? (a) : (b))

#define ZIP_PATH "/dev_hdd0/tmp/ps3load"
#define SELF_PATH ZIP_PATH ".self"

#define ERROR(err,msg) do { \
	if((err)<0) { \
		fprintf(stderr,"PS3Load: " msg "\n"); \
		return 1; \
	} \
} while(0)

static vu32 running = 0;
static sys_ppu_thread_t thr0;
//static sys_ppu_thread_t thr1;

static vu32 exit_flag = 0;
static volatile int listen_socket = -1;

static void empty_directory(const char *path)
{
	DIR *dirp;
	struct dirent *entry;

	dirp = opendir(path);
	if(!dirp) return;

	while((entry = readdir(dirp))!=NULL) {
		char newpath[0x440];

		if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
			continue;

		strcpy(newpath,path);
		strcat(newpath,"/");
		strcat(newpath,entry->d_name);

		if(entry->d_type==DT_DIR) {
			empty_directory(newpath);
			rmdir(newpath);
		} else
			remove(newpath);
		}
	closedir(dirp);
}

static void control_thread(void *arg)
{
	u32 btns;

	printf("PS3Load Control thread\n");

	while(running) {
		btns = pad_read();

		flip();
	}
	printf("PS3Load Control thread exit\n");
	sysThreadExit(0);
}

void release_all()
{
	printf("release_all(%02x,%08x)\n",running,listen_socket);

	running = 0;
	exit_flag = 1;

	sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);

	if(listen_socket>=0) shutdown(listen_socket,SHUT_RDWR);
}

void finalize()
{
	u64 retval;

	sysThreadJoin(thr0,&retval);

	netDeinitialize();

	gcmSetWaitFlip(context);
	rsxFinish(context,1);

	sysModuleUnload(SYSMODULE_PNGDEC);
	sysModuleUnload(SYSMODULE_JPGDEC);
	sysModuleUnload(SYSMODULE_FS);
}

void program_exit_callback()
{
	finalize();
}

void sysutil_exit_callback(u64 status,u64 param,void *usrdata)
{
	switch(status) {
		case SYSUTIL_EXIT_GAME:
			release_all();
			break;
		case SYSUTIL_DRAW_BEGIN:
		case SYSUTIL_DRAW_END:
			break;
		default:
			break;
	}
}

int main(int argc,char *argv[])
{
	s32 ret,i;
	struct sockaddr_in server;
	void *host_addr = memalign(1024*1024,HOST_SIZE);

	printf("PS3Load " PS3LOADVER "\n");

	sysModuleLoad(SYSMODULE_FS);
	sysModuleLoad(SYSMODULE_JPGDEC);
	sysModuleLoad(SYSMODULE_PNGDEC);

	init_screen(host_addr,HOST_SIZE);
	ioPadInit(7);

	ret = atexit(program_exit_callback);
	ret = sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0,sysutil_exit_callback,NULL);

	ret = netInitialize();
	ERROR(ret,"Error initializing network");

	mkdir(ZIP_PATH,0777);
	empty_directory(ZIP_PATH);

	running = 1;
	sysThreadCreate(&thr0,control_thread,NULL,999,STACKSIZE,THREAD_JOINABLE,"PS3Load Control Thread");

	memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	listen_socket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	ERROR(listen_socket,"Error creating socket()");

	ret = bind(listen_socket,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
	ERROR(ret,"Error bind()ing socket");

	ret = listen(listen_socket,10);
	ERROR(ret,"Error calling listen()");

#define continueloop() { close(f); goto reloop; }

reloop:
	while(!exit_flag) {
		printf("Waiting for connection...\n");
		int f = accept(listen_socket,NULL,NULL);
		printf("exit_flag: %02x\n",exit_flag);
		if(exit_flag) break;

		ERROR(f,"Error calling accept()");

		u32 magic = 0;
		if(read(f,&magic,sizeof(u32))<0)
			continueloop();

		if(magic!=0x48415858) {
			fprintf(stderr,"Wrong HAXX magic\n");
			continueloop();
		}

		if(read(f,&magic,sizeof(u32))<0)
			continueloop();

		u32 filesize = 0;
		u32 uncompressed = 0;
		u16 argslen = magic&0xffff;
		if(read(f,&filesize,sizeof(u32))<0)
			continueloop();

		if(read(f,&uncompressed,sizeof(u32))<0)
			continueloop();

		printf("Receiving data....(%d/%d)\n",filesize,uncompressed);

		u32 cnt;
		u32 pos = 0;
		u8 *data = (u8*)malloc(filesize);
		while(pos<filesize) {
			cnt = MIN(4096,filesize - pos);
			ret = read(f,data + pos,cnt);
			if(ret<0)
				continueloop();

			pos += ret;
		}

		printf("Receiving arguments....(%d)\n",argslen);

		u8 *args = NULL;
		if(argslen) {
			args = (u8*)malloc(argslen);
			if(read(f,args,argslen)<0)
				continueloop();
		}

		close(f);

		printf("Decompressing...\n");

		if(uncompressed) {
			u8 *compressed = data;
			uLongf final = uncompressed;

			data = (u8*)malloc(final);
			ret = uncompress(data,&final,compressed,filesize);
			if(ret!=Z_OK)
				continue;

			free(compressed);

			if(uncompressed!=final)
				continue;

			uncompressed = final;
		} else
			uncompressed = filesize;

		printf("Launching...\n");

		f = open(SELF_PATH,(O_CREAT | O_TRUNC | O_WRONLY), (S_IRWXU | S_IRWXG | S_IRWXO));
		ERROR(f,"Error opening temporary file");

		pos = 0;
		while(pos<uncompressed) {
			cnt = MIN(4096,uncompressed - pos);
			if(write(f,data + pos,cnt)!=cnt)
				continueloop();

			pos += cnt;
		}
		close(f);
		free(data);

		char bootpath[MAXPATHLEN + 1];
		struct zip *archive = zip_open(SELF_PATH,ZIP_CHECKCONS,NULL);
		int files = zip_get_num_files(archive);

		strcpy(bootpath,SELF_PATH);
		if(files>0) {
			strcpy(bootpath,"");

			for(i=0;i<files;i++) {
				char path[MAXPATHLEN + 1];
				const char *filename = zip_get_name(archive,i,0);

				strcpy(path,ZIP_PATH);

				if(!filename) continue;
				if(filename[0]!='/') strcat(path,"/");

				strcat(path,filename);

#define ENDS_WITH(needle) \
	(strlen(filename)>=strlen(needle) && !strcasecmp(filename + strlen(filename) - strlen(needle), needle))

				if(ENDS_WITH("EBOOT.BIN") || ENDS_WITH(".self"))
					strcpy(bootpath,path);

				if(filename[strlen(filename) - 1]!='/') {
					struct zip_stat st;
					struct zip_file *zfd;

					if(zip_stat_index(archive,i,0,&st)) {
						fprintf(stderr,"Unable to access file \'%s\' in zip.\n",filename);
						continue;
					}

					zfd = zip_fopen_index(archive,i,0);
					if(!zfd) {
						fprintf(stderr,"Unable to open file \'%s\' in zip.\n",filename);
						continue;
					}

					f = open(path, (O_CREAT | O_TRUNC | O_WRONLY), (S_IRWXU | S_IRWXG | S_IRWXO));
					ERROR(f,"Error opening temporary file");

					pos = 0;
					u8 *buffer = (u8*)malloc(4096);
					while(pos<st.size) {
						cnt = MIN(4096,st.size - pos);
						if(zip_fread(zfd,buffer,cnt)!=cnt)
							ERROR(1, "Error reading from zip");
						if(write(f,buffer,cnt)!=cnt)
							continueloop();

						pos += cnt;
					}
					free(buffer);

					zip_fclose(zfd);
					close(f);
				} else
					mkdir(path,0777);
			}
		}

		if(archive) zip_close(archive);
		if(!strlen(bootpath)) continue;

		char *launchenvp[2];
		char *launchargv[MAX_ARG_CNT];

		memset(launchenvp,0,sizeof(launchenvp));
		memset(launchargv,0,sizeof(launchargv));

		launchenvp[0] = (char*)malloc(0x440);
		snprintf(launchenvp[0],0x440,"ps3load=%s",argv[0]);

		i = 0;
		pos = 0;
		while(pos<argslen) {
			int len = strlen((char*)(args + pos));
			if(!len) break;

			launchargv[i] = (char*)malloc(len + 1);
			strcpy(launchargv[i],(char*)(args + pos));

			pos += (len + 1);
			i++;
		}
		release_all();
		finalize();

		sysProcessExitSpawn2(bootpath,(const char**)launchargv,(const char**)launchenvp,NULL,0,1001,SYS_PROCESS_SPAWN_STACK_SIZE_1M);

	}

	printf("exiting main....\n");
	return 0;
}
