/* libjpg.c

Copyright (c) 2010 Hermes <www.elotrolado.net>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this list
of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.
- The names of the contributors may not be used to endorse or promote products derived
from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <malloc.h>
#include <string.h>
#include <ppu-asm.h>

#include <jpgdec/jpgdec.h>

static void* jpg_malloc(u32 size,void *usrdata)
{
	return malloc(size);
}

static void jpg_free(void *ptr,void *usrdata)
{
	free(ptr);
}

static s32 decodeJPEG(jpgDecSource *src,jpgData *out)
{
	s32 mHandle,sHandle,ret;
	u32 space_allocated;
	u64 bytes_per_line;
	jpgDecInfo DecInfo;
	jpgDecInParam inParam;
	jpgDecOutParam outParam;
	jpgDecDataInfo DecDataInfo;
	jpgDecThreadInParam InThdParam;
	jpgDecThreadOutParam OutThdParam;

	InThdParam.enable = 0;
	InThdParam.ppu_prio = 512;
	InThdParam.spu_prio = 200;
	InThdParam.malloc_func = __get_addr32(__get_opd32(jpg_malloc));
	InThdParam.malloc_arg = 0; /* no args */
	InThdParam.free_func = __get_addr32(__get_opd32(jpg_free));
	InThdParam.free_arg = 0; /* no args */

	ret = jpgDecCreate(&mHandle,&InThdParam,&OutThdParam);

	out->bmp_out = NULL;
	if(ret==0) {
		ret = jpgDecOpen(mHandle,&sHandle,src,&space_allocated);
		if(ret==0) {
			ret = jpgDecReadHeader(mHandle,sHandle,&DecInfo);
			if(ret==0 && DecInfo.space==0) ret = -1;

			if(ret==0) {
				inParam.cmd_ptr = 0;
				inParam.down_scale = 1;
				inParam.quality = JPGDEC_LOW_QUALITY;
				inParam.mode = JPGDEC_TOP_TO_BOTTOM;
				inParam.space = JPGDEC_ARGB;
				inParam.alpha = 0xff;

				ret = jpgDecSetParameter(mHandle,sHandle,&inParam,&outParam);
			}

			if(ret==0) {
				out->pitch = bytes_per_line = outParam.width*4;
				out->bmp_out = malloc(bytes_per_line*outParam.height);
				if(!out->bmp_out)
					ret = -1;
				else {
					memset(out->bmp_out,0,(bytes_per_line*outParam.height));

					ret = jpgDecDecodeData(mHandle,sHandle,out->bmp_out,&bytes_per_line,&DecDataInfo);
					if(ret==0 && DecDataInfo.status==0) {
						out->width = outParam.width;
						out->height = outParam.height;

						ret = 0;
					}
				}
			}
			jpgDecClose(mHandle,sHandle);
		}
		if(ret && out->bmp_out) {
			free(out->bmp_out);
			out->bmp_out = NULL;
		}

		jpgDecDestroy(mHandle);
	}
	return ret;
}

s32 jpgLoadFromFile(const char *filename,jpgData *out)
{
	jpgDecSource source;

	memset(&source,0,sizeof(jpgDecSource));

	source.stream = JPGDEC_FILE;
	source.file_name = __get_addr32(filename);
	source.enable = JPGDEC_DISABLE;

	return decodeJPEG(&source,out);
}

s32 jpgLoadFromBuffer(const void *buffer,u32 size,jpgData *out)
{
	jpgDecSource source;

	memset(&source,0,sizeof(jpgDecSource));

	source.stream = JPGDEC_BUFFER;
	source.stream_ptr = __get_addr32(buffer);
	source.stream_size = size;
	source.enable = JPGDEC_DISABLE;

	return decodeJPEG(&source,out);
}
