#include "types.h"
#include "fpparser.h"
#include "vpparser.h"
#include "compiler.h"
#include "compilerfp.h"

#if !defined(WIN32)
#include <dlfcn.h>
#endif

#define PROG_TYPE_NONE			0
#define PROG_TYPE_VP			1
#define PROG_TYPE_FP			2

#ifdef __BIG_ENDIAN__
#define SWAP16(v) (v)
#define SWAP32(v) (v)
#else
#define SWAP16(v) ((v)>>8)|((v)<<8)
#define SWAP32(v) ((v)>>24)|((v)<<24)|(((v)&0xFF00)<<8)|(((v)&0xFF0000)>>8)
#endif

struct _options
{
	const char *src_file;
	const char *dst_file;
	const char *entry;
	int prog_type;
	bool gen_asm;
	bool compile;
	bool strip;
} Options = {
	NULL,
	NULL,
	"main",
	PROG_TYPE_NONE,
	false,
	true,
	false
};
 
#define CG_SOURCE				4112
#define CG_COMPILED_PROGRAM		4106
#define CG_PROFILE_VP30			6148
#define CG_PROFILE_FP30			6149
#define CG_PROFILE_FP40			6151
#define CG_PROFILE_VP40			7001

typedef void*(*_cgCreateContext)();
typedef void(*_cgDestroyContext)(void *context);
typedef void*(*_cgCreateProgramFromFile)(void* context, int program_type, const char *program, int profile, const char *entry, const char **args);
typedef const char*(*_cgGetProgramString)(void *program,int pname);
typedef const char *(*_cgGetLastListing)(void *context);

_cgCreateContext cgCreateContext=NULL;
_cgDestroyContext cgDestroyContext=NULL;
_cgCreateProgramFromFile cgCreateProgramFromFile=NULL;
_cgGetProgramString cgGetProgramString=NULL;
_cgGetLastListing cgGetLastListing=NULL;

static bool InitCompiler()
{
#if defined(WIN32)
	HMODULE libcgc=LoadLibrary("cg.dll");
#elif defined(__APPLE__)
    void *libcgc=dlopen("cg.dylib", RTLD_LAZY);
#else
    void *libcgc=dlopen("libCg.so", RTLD_LAZY);
#endif

    if (libcgc==NULL) return false;

#ifdef WIN32
	cgCreateContext=(_cgCreateContext)GetProcAddress(libcgc,"cgCreateContext");
	cgDestroyContext=(_cgDestroyContext)GetProcAddress(libcgc,"cgDestroyContext");
	cgCreateProgramFromFile=(_cgCreateProgramFromFile)GetProcAddress(libcgc,"cgCreateProgramFromFile");
	cgGetProgramString=(_cgGetProgramString)GetProcAddress(libcgc,"cgGetProgramString");
	cgGetLastListing=(_cgGetLastListing)GetProcAddress(libcgc,"cgGetLastListing");
#else
    cgCreateContext=(_cgCreateContext)dlsym(libcgc, "cgCreateContext");
    cgDestroyContext=(_cgDestroyContext)dlsym(libcgc, "cgDestroyContext");
    cgCreateProgramFromFile=(_cgCreateProgramFromFile)dlsym(libcgc, "cgCreateProgramFromFile");
    cgGetProgramString=(_cgGetProgramString)dlsym(libcgc, "cgGetProgramString");
    cgGetLastListing=(_cgGetLastListing)dlsym(libcgc, "cgGetLastListing");
#endif

    return (cgCreateContext!=NULL);
}

static u32 endian_fp(u32 v)
{
  return ( ( ( v >> 16 ) & 0xffff ) << 0 ) |
         ( ( ( v >> 0 ) & 0xffff ) << 16 );
}

void usage()
{
	printf("cgcomp [options] input output\n");
	printf("\t-f Input is fragment program\n");
	printf("\t-v Input is vertex program\n");
}

void readoptions(struct _options *options,int argc,char *argv[])
{
	int i;

	for(i=1;i<argc;i++) {
		if(argv[i][0]=='-') {
			switch(argv[i][1]) {
				case 'f': options->prog_type = PROG_TYPE_FP; break;
				case 'v': options->prog_type = PROG_TYPE_VP; break;
				case 'e': options->entry = argv[++i]; break;
				case 'a': options->gen_asm = true; break;
			}
		} else
			break;
	}
	if(i+2!=argc) usage();

	options->src_file = argv[i];
	options->dst_file = argv[i+1];
}

char* readfile(const char *filename)
{
	char *prg = NULL;
	unsigned int len = 0;
	FILE *f = fopen(filename,"rb");

	if(f==NULL) {
		fprintf(stderr,"Unable to open input file \'%s\'\n",filename);
		return NULL;
	}

	fseek(f,0,SEEK_END);
	len = ftell(f);
	fseek(f,0,SEEK_SET);

	prg = (char*)malloc(len + 1);
	if(fread(prg,1,len,f)!=len) {
		fprintf(stderr,"Input file read error\n");
		fclose(f);
		free(prg);
		return NULL;
	}

	fclose(f);
	return prg;
}

int compileVP()
{
	char *prg;
	void *context;
	void *program;

	if(Options.gen_asm==true) {
		prg = readfile(Options.src_file);
	} else {
		context = cgCreateContext();
		program = cgCreateProgramFromFile(context,CG_SOURCE,Options.src_file,CG_PROFILE_VP40,Options.entry,NULL);
		if(program==NULL) {
			const char *error = cgGetLastListing(context);
			fprintf(stderr,"%s\n",error);
			return EXIT_FAILURE;
		}
		prg = (char*)cgGetProgramString(program,CG_COMPILED_PROGRAM);
	}

	if(prg) {
		CVPParser parser;
		CCompiler compiler;

		parser.Parse(prg);
		compiler.Compile(&parser);

		struct vertex_program_exec *vpi = compiler.GetInstructions();
		std::list<struct nvfx_relocation> branch_reloc = compiler.GetBranchRelocations();
		for(std::list<struct nvfx_relocation>::iterator it = branch_reloc.begin();it!=branch_reloc.end();it++) {
			struct vertex_program_exec *vpe = &vpi[it->location];

			vpe->data[3] &= ~NV40_VP_INST_IADDRL_MASK;
			vpe->data[3] |= (it->target&7) << NV40_VP_INST_IADDRL_SHIFT;

			vpe->data[2] &= ~NV40_VP_INST_IADDRH_MASK;
			vpe->data[2] |= ((it->target >> 3)&0x3f) << NV40_VP_INST_IADDRH_SHIFT;
		}

		std::list<struct nvfx_relocation> const_reloc = compiler.GetConstRelocations();
		for(std::list<struct nvfx_relocation>::iterator it = const_reloc.begin();it!=const_reloc.end();it++) {
			struct vertex_program_exec *vpe = &vpi[it->location];
			vpe->data[1] &= ~NVFX_VP(INST_CONST_SRC_MASK);
			vpe->data[1] |= (it->target) << NVFX_VP(INST_CONST_SRC_SHIFT);
		}

		int n,i;
		u16 magic = ('V'<<8)|'P';
		u32 lastoff = sizeof(rsxVertexProgram);
		unsigned char *vertexprogram = (unsigned char*)calloc(2*1024*1024,1);

		rsxVertexProgram *vp = (rsxVertexProgram*)vertexprogram;

		vp->magic = SWAP16(magic);
		vp->start_insn = SWAP16(0);
		vp->const_start = SWAP16(0);
		vp->input_mask = SWAP32(compiler.GetInputMask());
		vp->output_mask = SWAP32(compiler.GetOutputMask());

		while(lastoff&3) 
			vertexprogram[lastoff++] = 0;

		rsxProgramAttrib *attribs = (rsxProgramAttrib*)(vertexprogram + lastoff);

		vp->attrib_off = SWAP32(lastoff);

		n = 0;
		std::list<param> params = parser.GetParameters();
		for(std::list<param>::iterator it = params.begin();it!=params.end();it++) {
			if(!it->is_const) {
				it->user = lastoff + (n*sizeof(rsxProgramAttrib));
				attribs[n].index = SWAP32(it->index);
				attribs[n].name_off = SWAP32(0);
				attribs[n].type = it->type;
				n++;
			}
		}
		vp->num_attrib = SWAP16(n);
		lastoff += (n*sizeof(rsxProgramAttrib));

		while(lastoff&3)
			vertexprogram[lastoff++] = 0;

		rsxProgramConst *consts = (rsxProgramConst*)(vertexprogram + lastoff);

		vp->const_off = SWAP32(lastoff);

		n = 0;
		for(std::list<param>::iterator it = params.begin();it!=params.end();it++) {
			if(it->is_const) {
				it->user = lastoff + (n*sizeof(rsxProgramConst));
				consts[n].count = it->count;
				consts[n].type = it->type;
				consts[n].is_internal = it->is_internal;
				consts[n].name_off = SWAP32(0);
				for(i=0;i<(s32)it->count;i++) {
					f32 *pVal = it->values[i];
					consts[n].index = SWAP32(it->index + i);
					consts[n].values[0].f = pVal[0];
					consts[n].values[0].u = SWAP32(consts[n].values[0].u);
					consts[n].values[1].f = pVal[1];
					consts[n].values[1].u = SWAP32(consts[n].values[1].u);
					consts[n].values[2].f = pVal[2];
					consts[n].values[2].u = SWAP32(consts[n].values[2].u);
					consts[n].values[3].f = pVal[3];
					consts[n].values[3].u = SWAP32(consts[n].values[3].u);
					n++;
				}
			}
		}
		vp->num_const = SWAP16(n);
		lastoff += (n*sizeof(rsxProgramConst));

		while(lastoff&3)
			vertexprogram[lastoff++] = 0;

		for(std::list<param>::iterator it = params.begin();it!=params.end();it++) {
			if(!it->name.empty() && !it->is_internal) {
				const char *name = it->name.c_str();
				int off = lastoff;
				while(*name) 
					vertexprogram[lastoff++] = *name++;
				vertexprogram[lastoff++] = 0;
				*((u32*)(vertexprogram + it->user)) = SWAP32(off);
			}
		}

		while(lastoff&15)
			vertexprogram[lastoff++] = 0;

		vp->ucode_off = SWAP32(lastoff);
		vp->num_insn = SWAP16(compiler.GetInstructionCount());

		u32 *dstcodeptr = (u32*)(vertexprogram + lastoff);
		for(i=0,n=0;i<compiler.GetInstructionCount();i++,n+=4,lastoff+=16) {
			dstcodeptr[n+0] = SWAP32(vpi[i].data[0]);
			dstcodeptr[n+1] = SWAP32(vpi[i].data[1]);
			dstcodeptr[n+2] = SWAP32(vpi[i].data[2]);
			dstcodeptr[n+3] = SWAP32(vpi[i].data[3]);
		}

		FILE *fOut = fopen(Options.dst_file,"wb");
		fwrite(vertexprogram,lastoff,1,fOut);
		fclose(fOut);

		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

int compileFP()
{
	char *prg;
	void *context;
	void *program;

	if(Options.gen_asm==true) {
		prg = readfile(Options.src_file);
	} else {
		context = cgCreateContext();
		program = cgCreateProgramFromFile(context,CG_SOURCE,Options.src_file,CG_PROFILE_FP40,Options.entry,NULL);
		if(program==NULL) {
			const char *error = cgGetLastListing(context);
			fprintf(stderr,"%s\n",error);
			return EXIT_FAILURE;
		}
		prg = (char*)cgGetProgramString(program,CG_COMPILED_PROGRAM);
	}

	if(prg) {
		CFPParser parser;
		CCompilerFP compiler;
		
		parser.Parse(prg);
		compiler.Compile(&parser);

		int n,i;
		u16 magic = ('F'<<8)|'P';
		u32 lastoff = sizeof(rsxFragmentProgram);
		unsigned char *fragmentprogram = (unsigned char*)calloc(2*1024*1024,1);

		rsxFragmentProgram *fp = (rsxFragmentProgram*)fragmentprogram;
		
		fp->magic = SWAP16(magic);
		fp->num_regs = SWAP32(compiler.GetNumRegs());
		fp->fp_control = SWAP32(compiler.GetFPControl());
		fp->texcoords = SWAP16(compiler.GetTexcoords());
		fp->texcoord2D = SWAP16(compiler.GetTexcoord2D());
		fp->texcoord3D = SWAP16(compiler.GetTexcoord3D());

		while(lastoff&3)
			fragmentprogram[lastoff++] = 0;

		fp->attrib_off = SWAP32(lastoff);
		rsxProgramAttrib *attribs = (rsxProgramAttrib*)(fragmentprogram + lastoff);

		n = 0;
		std::list<param> params = parser.GetParameters();
		for(std::list<param>::iterator it = params.begin();it!=params.end();it++) {
			if(!it->is_const) {
				it->user = lastoff + (n*sizeof(rsxProgramAttrib));
				attribs[n].index = SWAP32(it->index);
				attribs[n].name_off = SWAP32(0);
				attribs[n].type = it->type;
				n++;
			}
		}
		fp->num_attrib = SWAP16(n);
		lastoff += (n*sizeof(rsxProgramAttrib));

		while(lastoff&3)
			fragmentprogram[lastoff++] = 0;

		std::list<struct fragment_program_data> const_relocs = compiler.GetConstRelocations();
		for(std::list<param>::iterator it = params.begin();it!=params.end();it++) {
			if(it->is_const && !it->is_internal) {
				for(i=0;i<it->count;i++) {
					s32 k = 0;
					rsxConstOffsetTable *const_table = (rsxConstOffsetTable*)(fragmentprogram + lastoff);
					
					const_table->num = SWAP32(0);
					for(std::list<struct fragment_program_data>::iterator d=const_relocs.begin();d!=const_relocs.end();d++) {
						if(d->index==(it->index + i)) {
							const_table->offset[k++] = SWAP32((d->offset*16));
							d->user = lastoff;
						}
					}
					const_table->num = SWAP32(k);
					lastoff += (4*k + 4);
				}
			}
		}

		while(lastoff&3)
			fragmentprogram[lastoff++] = 0;

		fp->const_off = SWAP32(lastoff);
		rsxProgramConst *consts = (rsxProgramConst*)(fragmentprogram + lastoff);

		n = 0;
		for(std::list<param>::iterator it = params.begin();it!=params.end();it++) {
			if(it->is_const && !it->is_internal) {
				it->user = lastoff + (n*sizeof(rsxProgramConst));

				consts[n].count = it->count;
				consts[n].type = it->type;
				consts[n].is_internal = it->is_internal;
				consts[n].name_off = SWAP32(0);

				for(i=0;i<it->count;i++) {
					s32 table_off = -1;
					for(std::list<struct fragment_program_data>::iterator d=const_relocs.begin();d!=const_relocs.end();d++) {
						if(d->index==(it->index + i)) {
							table_off = d->user;
							break;
						}
					}

					f32 *pVal = it->values[i];
					consts[n].index = SWAP32(table_off);
					consts[n].values[0].f = pVal[0];
					consts[n].values[0].u = SWAP32(consts[n].values[0].u);
					consts[n].values[1].f = pVal[1];
					consts[n].values[1].u = SWAP32(consts[n].values[1].u);
					consts[n].values[2].f = pVal[2];
					consts[n].values[2].u = SWAP32(consts[n].values[2].u);
					consts[n].values[3].f = pVal[3];
					consts[n].values[3].u = SWAP32(consts[n].values[3].u);
					n++;
				}
			}
		}
		fp->num_const = SWAP16(n);
		lastoff += (n*sizeof(rsxProgramConst));

		while(lastoff&3)
			fragmentprogram[lastoff++] = 0;

		for(std::list<param>::iterator it = params.begin();it!=params.end();it++) {
			if(!it->name.empty() && !it->is_internal) {
				const char *name = it->name.c_str();
				int off = lastoff;
				while(*name) 
					fragmentprogram[lastoff++] = *name++;
				fragmentprogram[lastoff++] = 0;
				*((u32*)(fragmentprogram + it->user)) = SWAP32(off);
			}
		}

		while(lastoff&15)
			fragmentprogram[lastoff++] = 0;

		fp->ucode_off = SWAP32(lastoff);
		fp->num_insn = SWAP16(compiler.GetInstructionCount());

		struct fragment_program_exec *fpi = compiler.GetInstructions();
		u32 *dstcodeptr = (u32*)(fragmentprogram + lastoff);
		for(i=0,n=0;i<compiler.GetInstructionCount();i++,n+=4,lastoff+=16) {
			dstcodeptr[n+0] = endian_fp((SWAP32(fpi[i].data[0])));
			dstcodeptr[n+1] = endian_fp((SWAP32(fpi[i].data[1])));
			dstcodeptr[n+2] = endian_fp((SWAP32(fpi[i].data[2])));
			dstcodeptr[n+3] = endian_fp((SWAP32(fpi[i].data[3])));
		}

		FILE *fOut = fopen(Options.dst_file,"wb");
		fwrite(fragmentprogram,lastoff,1,fOut);
		fclose(fOut);

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int main(int argc,char *argv[])
{
	int ret = 0;

	readoptions(&Options,argc,argv);

	if(Options.gen_asm!=true && !InitCompiler()) {
        fprintf(stderr, "Unable to load Cg, aborting.\n");
        fprintf(stderr, "Please install Cg toolkit and/or set the path (i.e. LD_LIBRARY_PATH) to the shared library accordingly.\n");
        return EXIT_FAILURE;
    }

	switch(Options.prog_type) {
		case PROG_TYPE_VP:
			ret = compileVP();
			break;
		case PROG_TYPE_FP:
			ret = compileFP();
			break;
		default:
			break;
	}
	return ret;
}
