/* libpng.c

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

#include <pngdec/pngdec.h>

static void* png_malloc(u32 size,void *usrdata)
{
	return malloc(size);
}

static void png_free(void *ptr,void *usrdata)
{
	return free(ptr);
}

static s32 decodePNG(pngDecSource *src,pngData *out)
{
	s32 mHandle,sHandle,ret;
	u32 space_allocated;
	u64 bytes_per_line;
	pngDecInfo DecInfo;
	pngDecInParam inParam;
	pngDecOutParam outParam;
	pngDecDataInfo DecDataInfo;
	pngDecThreadInParam InThdParam;
	pngDecThreadOutParam OutThdParam;

	InThdParam.enable = 0;
	InThdParam.ppu_prio = 512;
	InThdParam.spu_prio = 200;
	InThdParam.malloc_func = __get_addr32(__get_opd32(png_malloc));
	InThdParam.malloc_arg = 0; /* no args */
	InThdParam.free_func = __get_addr32(__get_opd32(png_free));
	InThdParam.free_arg = 0; /* no args */

	ret= pngDecCreate(&mHandle, &InThdParam, &OutThdParam);

	out->bmp_out = NULL;
	if(ret==0) {
		ret = pngDecOpen(mHandle,&sHandle,src,&space_allocated);
		if(ret==0) {
			ret = pngDecReadHeader(mHandle,sHandle,&DecInfo);
			if(ret==0) {
				inParam.cmd_ptr = 0;
				inParam.mode = PNGDEC_TOP_TO_BOTTOM;
				inParam.space = PNGDEC_ARGB;
				inParam.bit_depth = 8;
				inParam.pack_flag = 1;
				if(DecInfo.space==PNGDEC_GRAYSCALE_ALPHA || DecInfo.space==PNGDEC_RGBA || DecInfo.chunk_info&0x10)
					inParam.alpha_select = 0;
				else
					inParam.alpha_select = 1;

				inParam.alpha = 0xff;

				ret = pngDecSetParameter(mHandle,sHandle,&inParam,&outParam);
			}

			if(ret==0) {
				out->pitch = bytes_per_line = outParam.width*4;
				out->bmp_out = malloc(out->pitch*outParam.height);
				if(!out->bmp_out)
					ret = -1;
				else {
					memset(out->bmp_out,0,(bytes_per_line*outParam.height));
					
					ret = pngDecDecodeData(mHandle,sHandle,out->bmp_out,&bytes_per_line,&DecDataInfo);
					if(ret==0 && DecDataInfo.status==0) {
						out->width = outParam.width;
						out->height = outParam.height;

						ret = 0;
					}
				}
			}
			pngDecClose(mHandle,sHandle);
		}
		if(ret && out->bmp_out) {
			free(out->bmp_out);
			out->bmp_out = NULL;
		}

		pngDecDestroy(mHandle);
	}
	return ret;
}

s32 pngLoadFromFile(const char *filename,pngData *out)
{
	pngDecSource source;

	memset(&source,0,sizeof(pngDecSource));

	source.stream = PNGDEC_FILE;
	source.file_name = __get_addr32(filename);
	source.enable = PNGDEC_DISABLE;

	return decodePNG(&source,out);
}

s32 pngLoadFromBuffer(const void *buffer,u32 size,pngData *out)
{
	pngDecSource source;

	memset(&source,0,sizeof(pngDecSource));

	source.stream = PNGDEC_BUFFER;
	source.stream_ptr = __get_addr32(buffer);
	source.stream_size = size;
	source.enable = PNGDEC_DISABLE;

	return decodePNG(&source,out);
}
