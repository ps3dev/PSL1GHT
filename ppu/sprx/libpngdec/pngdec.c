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

static s32 decodePNG(CellPngDecSource *src,CellPngData *out)
{
	s32 mHandle,sHandle,ret;
	CellPngDecInfo DecInfo;
	CellPngDecOpnInfo openInfo;
	CellPngDecInParam inParam;
	CellPngDecOutParam outParam;
	CellPngDecDataInfo DecDataInfo;
	CellPngDecThreadInParam InThdParam;
	CellPngDecThreadOutParam OutThdParam;
	CellPngDecDataCtrlParam dataCtrlParam;
	CellPngCbCtrlMalloc fnMalloc = png_malloc;
	CellPngCbCtrlFree fnFree = png_free;

	InThdParam.spu_enable = CELL_PNGDEC_SPU_THREAD_DISABLE;
	InThdParam.ppu_prio = 512;
	InThdParam.spu_prio = 200;
	InThdParam.malloc_func = (CellPngCbCtrlMalloc)__get_opd32(fnMalloc);
	InThdParam.malloc_arg = NULL;
	InThdParam.free_func = (CellPngCbCtrlFree)__get_opd32(fnFree);
	InThdParam.free_arg = NULL;

	ret= cellPngDecCreate(&mHandle, &InThdParam, &OutThdParam);

	out->bmp_out = NULL;
	if(ret==0) {
		ret = cellPngDecOpen(mHandle,&sHandle,src,&openInfo);
		if(ret==0) {
			ret = cellPngDecReadHeader(mHandle,sHandle,&DecInfo);
			if(ret==0) {
				inParam.cmd_ptr = 0;
				inParam.output_mode = CELL_PNGDEC_TOP_TO_BOTTOM;
				inParam.color_space = CELL_PNGDEC_ARGB;
				inParam.bit_depth = 8;
				inParam.pack_flag = CELL_PNGDEC_1BYTE_PER_1PIXEL;
				if(DecInfo.color_space==CELL_PNGDEC_GRAYSCALE_ALPHA || DecInfo.color_space==CELL_PNGDEC_RGBA || DecInfo.chunk_info&0x10)
					inParam.alpha_select = 0;
				else
					inParam.alpha_select = 1;

				inParam.alpha = 0xff;

				ret = cellPngDecSetParameter(mHandle,sHandle,&inParam,&outParam);
			}

			if(ret==0) {
				out->pitch = outParam.width*4;
				out->bmp_out = malloc(out->pitch*outParam.height);
				if(!out->bmp_out)
					ret = -1;
				else {
					memset(out->bmp_out,0,(out->pitch*outParam.height));
					
					dataCtrlParam.output_bytes_per_line = out->pitch;
					ret = cellPngDecDecodeData(mHandle,sHandle,out->bmp_out,&dataCtrlParam,&DecDataInfo);
					if(ret==0 && DecDataInfo.decode_status==0) {
						out->width = outParam.width;
						out->height = outParam.height;

						ret = 0;
					}
				}
			}
			cellPngDecClose(mHandle,sHandle);
		}
		if(ret && out->bmp_out) {
			free(out->bmp_out);
			out->bmp_out = NULL;
		}

		cellPngDecDestroy(mHandle);
	}
	return ret;
}

s32 cellPngLoadFromFile(const char *filename,CellPngData *out)
{
	CellPngDecSource source;

	memset(&source,0,sizeof(CellPngDecSource));

	source.stream_sel = CELL_PNGDEC_FILE;
	source.file_name = filename;
	source.spu_enable = CELL_PNGDEC_SPU_THREAD_DISABLE;

	return decodePNG(&source,out);
}

s32 cellPngLoadFromBuffer(const void *buffer,u32 size,CellPngData *out)
{
	CellPngDecSource source;

	memset(&source,0,sizeof(CellPngDecSource));

	source.stream_sel = CELL_PNGDEC_BUFFER;
	source.stream_ptr = (void*)buffer;
	source.stream_size = size;
	source.spu_enable = CELL_PNGDEC_SPU_THREAD_DISABLE;

	return decodePNG(&source,out);
}
