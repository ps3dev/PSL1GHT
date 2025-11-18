#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fpparser.h"


#define INPUT_1V     1
#define INPUT_2V     2
#define INPUT_3V     3
#define INPUT_1S     4
#define INPUT_2S     5
#define INPUT_CC     6
#define INPUT_1V_T   7  /* one source vector, plus textureId */
#define INPUT_3V_T   8  /* one source vector, plus textureId */
#define INPUT_NONE   9
#define INPUT_1V_S  10  /* a string and a vector register */
#define OUTPUT_V    20
#define OUTPUT_S    21
#define OUTPUT_NONE 22

/* IRIX defines some of these */
#undef _R
#undef _H
#undef _X
#undef _C
#undef _S

/* Optional suffixes */
#define _R  FLOAT32  /* float */
#define _H  FLOAT16  /* half-float */
#define _X  FIXED12  /* fixed */
#define _C  0x08     /* set cond codes */
#define _S  0x10     /* saturate, clamp result to [0,1] */

struct _opcode
{
	const char *name;
	u32 opcode;
	u32 inputs;
	u32 outputs;
	u32 suffixes;
} fp_opcodes[] = {
   { "ABS", OPCODE_ABS, INPUT_1V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "ADD", OPCODE_ADD, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "BRK", OPCODE_BRK, INPUT_CC, OUTPUT_NONE, 0                   },
   { "CAL", OPCODE_CAL, INPUT_CC, OUTPUT_NONE, 0				   },
   { "CMP", OPCODE_CMP, INPUT_3V, OUTPUT_V,                _C | _S },
   { "COS", OPCODE_COS, INPUT_1S, OUTPUT_S, _R | _H |      _C | _S },
   { "DDX", OPCODE_DDX, INPUT_1V, OUTPUT_V, _R | _H |      _C | _S },
   { "DDY", OPCODE_DDY, INPUT_1V, OUTPUT_V, _R | _H |      _C | _S },
   { "DIV", OPCODE_DIV, INPUT_1V_S, OUTPUT_V, _R | _H |    _C | _S },
   { "DP2", OPCODE_DP2, INPUT_2V, OUTPUT_S, _R | _H | _X | _C | _S },
   { "DP2A", OPCODE_DP2A, INPUT_3V, OUTPUT_S, _R | _H | _X | _C | _S },
   { "DP3", OPCODE_DP3, INPUT_2V, OUTPUT_S, _R | _H | _X | _C | _S },
   { "DP4", OPCODE_DP4, INPUT_2V, OUTPUT_S, _R | _H | _X | _C | _S },
   { "DPH", OPCODE_DPH, INPUT_2V, OUTPUT_S, _R | _H | _X | _C | _S },
   { "DST", OPCODE_DST, INPUT_2V, OUTPUT_V, _R | _H |      _C | _S },
   { "ELSE", OPCODE_ELSE, INPUT_NONE, OUTPUT_NONE, 0               },
   { "ENDIF", OPCODE_ENDIF, INPUT_NONE, OUTPUT_NONE, 0             },
   { "ENDLOOP", OPCODE_ENDLOOP, INPUT_NONE, OUTPUT_NONE, 0         },
   { "ENDREP", OPCODE_ENDREP, INPUT_NONE, OUTPUT_NONE, 0           },
   { "EX2", OPCODE_EX2, INPUT_1S, OUTPUT_S, _R | _H |      _C | _S },
   { "FLR", OPCODE_FLR, INPUT_1V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "FRC", OPCODE_FRC, INPUT_1V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "IF", OPCODE_IF, INPUT_CC, OUTPUT_NONE, 0                     },
   { "KIL", OPCODE_KIL, INPUT_CC, OUTPUT_NONE, 0                },
   { "LG2", OPCODE_LG2, INPUT_1S, OUTPUT_S, _R | _H |      _C | _S },
   { "LIT", OPCODE_LIT, INPUT_1V, OUTPUT_V, _R | _H |      _C | _S },
   { "LOOP", OPCODE_BGNLOOP, INPUT_1V, OUTPUT_NONE, 0			   },
   { "LRP", OPCODE_LRP, INPUT_3V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "MAD", OPCODE_MAD, INPUT_3V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "MAX", OPCODE_MAX, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "MIN", OPCODE_MIN, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "MOV", OPCODE_MOV, INPUT_1V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "MUL", OPCODE_MUL, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "NRM", OPCODE_NRM3, INPUT_1V, OUTPUT_V, _R | _H |     _C | _S },
   { "PK2H",  OPCODE_PK2H,  INPUT_1V, OUTPUT_S, 0                  },
   { "PK2US", OPCODE_PK2US, INPUT_1V, OUTPUT_S, 0                  },
   { "PK4B",  OPCODE_PK4B,  INPUT_1V, OUTPUT_S, 0                  },
   { "PK4UB", OPCODE_PK4UB, INPUT_1V, OUTPUT_S, 0                  },
   { "POW", OPCODE_POW, INPUT_2S, OUTPUT_S, _R | _H |      _C | _S },
   { "RCP", OPCODE_RCP, INPUT_1S, OUTPUT_S, _R | _H |      _C | _S },
   { "REP", OPCODE_BGNREP, INPUT_1V, OUTPUT_NONE, 0                },
   { "RET", OPCODE_RET, INPUT_CC, OUTPUT_NONE, 0                   },
   { "RFL", OPCODE_RFL, INPUT_2V, OUTPUT_V, _R | _H |      _C | _S },
   { "RSQ", OPCODE_RSQ, INPUT_1S, OUTPUT_S, _R | _H |      _C | _S },
   { "SCS", OPCODE_SCS, INPUT_1S, OUTPUT_S, _R | _H |      _C | _S },
   { "SEQ", OPCODE_SEQ, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "SFL", OPCODE_SFL, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "SGE", OPCODE_SGE, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "SGT", OPCODE_SGT, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "SIN", OPCODE_SIN, INPUT_1S, OUTPUT_S, _R | _H |      _C | _S },
   { "SLE", OPCODE_SLE, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "SLT", OPCODE_SLT, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "SNE", OPCODE_SNE, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "STR", OPCODE_STR, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "SUB", OPCODE_SUB, INPUT_2V, OUTPUT_V, _R | _H | _X | _C | _S },
   { "SWZ", OPCODE_SWZ, INPUT_1V, OUTPUT_V, _R | _H |      _C | _S },
   { "TEX", OPCODE_TEX, INPUT_1V_T, OUTPUT_V,              _C | _S },
   { "TXB", OPCODE_TXB, INPUT_1V_T, OUTPUT_V,              _C | _S },
   { "TXD", OPCODE_TXD, INPUT_3V_T, OUTPUT_V,              _C | _S },
   { "TXL", OPCODE_TXL, INPUT_1V_T, OUTPUT_V,              _C | _S },
   { "TXP", OPCODE_TXP, INPUT_1V_T, OUTPUT_V,           _C | _S },
   { "UP2H",  OPCODE_UP2H,  INPUT_1S, OUTPUT_V,            _C | _S },
   { "UP2US", OPCODE_UP2US, INPUT_1S, OUTPUT_V,            _C | _S },
   { "UP4B",  OPCODE_UP4B,  INPUT_1S, OUTPUT_V,            _C | _S },
   { "UP4UB", OPCODE_UP4UB, INPUT_1S, OUTPUT_V,            _C | _S },
   { "X2D", OPCODE_X2D, INPUT_3V, OUTPUT_V, _R | _H |      _C | _S },
   { "XPD", OPCODE_X2D, INPUT_2V, OUTPUT_V, _R | _H |      _C | _S },
   //
   { "END", OPCODE_END,0,0,0 },
};
static const size_t FP_OPCODES_CNT = sizeof(fp_opcodes)/sizeof(struct _opcode);

static ioset fp_inputs[] =
{
	{ "fragment.position", 0 },
	{ "fragment.color.secondary", 2},
	{ "fragment.color.primary", 1},
	{ "fragment.color", 1 },
	{ "fragment.fogcoord", 3},
	{ "fragment.texcoord", 4},
	{ "WPOS", 0 },
	{ "COL0", 1 },
	{ "COL1", 2 },
	{ "FOGC", 3 },
	{ "TEX0", 4 },
	{ "TEX1", 5 },
	{ "TEX2", 6 },
	{ "TEX3", 7 },
	{ "TEX4", 8 },
	{ "TEX5", 9 },
	{ "TEX6", 10 },
	{ "TEX7", 11 }
};
static const size_t FP_INPUTS_CNT = sizeof(fp_inputs)/sizeof(ioset);

static ioset fp_outputs[] =
{
	{ "result.color", 0},
	{ "result.depth", 1},
	{ "COLR", 0},
	{ "COLH", 0},
	{ "DEPR", 1}
};
static const size_t FP_OUTPUTS_CNT = sizeof(fp_outputs)/sizeof(ioset);

CFPParser::CFPParser() : CParser()
{
	m_pInstructions = new struct nvfx_insn[MAX_NV_FRAGMENT_PROGRAM_INSTRUCTIONS];
}

CFPParser::~CFPParser()
{
}

int CFPParser::Parse(const char *str)
{
	int i,iline = 0;
	bool inProgram = false;
	std::stringstream input(str);

	while(!input.eof()) {
		char line[256];
		struct nvfx_insn *insn = NULL;

		input.getline(line,255);
		iline++;
			
		for(i=0;i<256;i++) {
			char c = line[i];

			if(c=='\n' || c=='\r' || c==';')
				c = 0;
			if(c=='\t')
				c = ' ';

			line[i] = c;
			if(c==0) break;
		}

		if(line[0]=='#') {
			ParseComment(line);
			continue;
		}

		if(!inProgram) {
			if(strncmp(line,"!!FP2.0",7)==0)
				inProgram = true;
			else if(strncmp(line,"!!ARBfp1.0",10)==0)
				inProgram = true;

			continue;
		}

		const char *col_ptr = NULL;
		const char *ptr = line;
		const char *param_str = NULL;
		
		if((col_ptr = strstr((char*)ptr,":"))!=NULL) {
			int j = 0;
			bool valid = true;
			
			while((ptr+j)<col_ptr) {
				if(j==0 && !(isLetter(ptr[j]) || ptr[j]=='_')) valid = false;
				if(!(isLetter(ptr[j]) || isDigit(ptr[j]) || ptr[j]=='_')) valid = false;
				j++;
			}

			if(valid) {
				(void)strtok((char*)ptr,":\x20");
				ptr = col_ptr + 1;
			}
		}

		ptr = SkipSpaces(ptr);
		if ((param_str = strstr(ptr, "OPTION"))!=NULL) {
			param_str = SkipSpaces(param_str + 6);
			if(strncasecmp(param_str,"NV_fragment_program2",20)==0)
				m_nOption |= NV_OPTION_FP2;
			continue;
		} 
		else if ((param_str = strstr(ptr, "PARAM"))!=NULL)
			continue;
		else if ((param_str = strstr(ptr, "TEMP"))!=NULL)
			continue;
		else if ((param_str = strstr(ptr, "OUTPUT"))!=NULL) {
			ParseOutput(ptr);
			continue;
		} else {
			const char *opcode = strtok((char*)ptr," ");
			struct _opcode opc = FindOpcode(opcode);

			if(opc.opcode>=MAX_OPCODE)
				continue;
			
			insn = &m_pInstructions[m_nInstructions];
			param_str = SkipSpaces(strtok(NULL,"\0"));

			InitInstruction(insn,opc.opcode);
			if(opc.opcode==OPCODE_END) {
				m_nInstructions++;
				break;
			}

			ParseInstruction(insn,&opc,param_str);
			m_nInstructions++;
		}
	}
	return 0;
}

void CFPParser::ParseInstruction(struct nvfx_insn *insn,opcode *opc,const char *param_str)
{
	const char *token = SkipSpaces(strtok((char*)param_str,","));

	insn->precision = opc->suffixes&(_R|_H|_X);
	insn->sat = ((opc->suffixes&_S) ? TRUE : FALSE);
	insn->cc_update = ((opc->suffixes&_C) ? TRUE : FALSE);

	if(opc->outputs==OUTPUT_S || opc->outputs==OUTPUT_V) {
		ParseMaskedDstReg(token,insn);
	} else if(opc->outputs==OUTPUT_NONE) {
		SetNoneDestReg(insn);
	}

	if(opc->outputs!=OUTPUT_NONE && opc->inputs!=INPUT_NONE) {
		token = SkipSpaces(strtok(NULL,","));
	}

	if(opc->inputs==INPUT_1V) {
		ParseVectorSrc(token,&insn->src[0]);
	} else if(opc->inputs==INPUT_2V) {
		ParseVectorSrc(token,&insn->src[0]);

		token = SkipSpaces(strtok(NULL,","));
		ParseVectorSrc(token,&insn->src[1]);
	} else if(opc->inputs==INPUT_3V) {
		ParseVectorSrc(token,&insn->src[0]);

		token = SkipSpaces(strtok(NULL,","));
		ParseVectorSrc(token,&insn->src[1]);

		token = SkipSpaces(strtok(NULL,","));
		ParseVectorSrc(token,&insn->src[2]);
	} else if(opc->inputs==INPUT_1S) {
		ParseScalarSrc(token,&insn->src[0]);
	} else if(opc->inputs==INPUT_2S) {
		ParseScalarSrc(token,&insn->src[0]);

		token = SkipSpaces(strtok(NULL,","));
		ParseScalarSrc(token,&insn->src[1]);
	} else if(opc->inputs==INPUT_1V_T) {
		ParseVectorSrc(token,&insn->src[0]);

		token = SkipSpaces(strtok(NULL,","));
		ParseTextureUnit(token,&insn->tex_unit);

		token = SkipSpaces(strtok(NULL,","));
		ParseTextureTarget(token,&insn->tex_target);
	} else if(opc->inputs==INPUT_3V_T) {
		ParseVectorSrc(token,&insn->src[0]);

		token = SkipSpaces(strtok(NULL,","));
		ParseVectorSrc(token,&insn->src[1]);

		token = SkipSpaces(strtok(NULL,","));
		ParseVectorSrc(token,&insn->src[2]);

		token = SkipSpaces(strtok(NULL,","));
		ParseTextureUnit(token,&insn->tex_unit);

		token = SkipSpaces(strtok(NULL,","));
		ParseTextureTarget(token,&insn->tex_target);
	} else if(opc->inputs==INPUT_CC) {
		if (*token == '(') token++;
		ParseCond(token,insn);
	}

	// finally check for insns disabling perspective correction interpolation
	// only at this point we know everyhting about the insn to decide.
	insn->disable_pc = IsPCDisablingInstruction(insn);
}

opcode CFPParser::FindOpcode(const char *mnemonic)
{
	struct _opcode result;

	result.name = NULL;
	result.opcode = MAX_OPCODE;
	result.inputs = 0;
	result.outputs = 0;
	result.suffixes = 0;

	for(size_t i=0;i<FP_OPCODES_CNT;i++) {
		const struct _opcode *inst = &fp_opcodes[i];
		if(strncmp(mnemonic,inst->name,strlen(inst->name))==0) {
			int i = strlen(inst->name);

			result = *inst;
			result.suffixes = 0;

			if(mnemonic[i]=='R') {
				result.suffixes |= _R;
				i++;
			} else if(mnemonic[i]=='H') {
				result.suffixes |= _H;
				i++;
			} else if(mnemonic[i]=='X') {
				result.suffixes |= _X;
				i++;
			}

			if(mnemonic[i]=='C') {
				result.suffixes |= _C;
				i++;
			}
			if(mnemonic[i+0]=='_' && mnemonic[i+1]=='S' &&
			   mnemonic[i+2]=='A' && mnemonic[i+3]=='T')
			{
				result.suffixes |= _S;
			}
			return result;
		}
	}

	return result;
}

s32 CFPParser::ConvertInputReg(const char *token)
{
	if(strcasecmp(token,"WPOS")==0)
		return 0;
	if(strcasecmp(token,"COL0")==0)
		return 1;
	if(strcasecmp(token,"COL1")==0)
		return 2;
	if(strcasecmp(token,"FOGC")==0)
		return 3;
	if(strcasecmp(token,"TEX0")==0)
		return 4;
	if(strcasecmp(token,"TEX1")==0)
		return 5;
	if(strcasecmp(token,"TEX2")==0)
		return 6;
	if(strcasecmp(token,"TEX3")==0)
		return 7;
	if(strcasecmp(token,"TEX4")==0)
		return 8;
	if(strcasecmp(token,"TEX5")==0)
		return 9;
	if(strcasecmp(token,"TEX6")==0)
		return 10;
	if(strcasecmp(token,"TEX7")==0)
		return 11;

	return -1;
}

void CFPParser::ParseOutput(const char *param_str)
{
	oparam p;
	u8 is_fp16 = 0;
	s32 reg = -1;
	const char *param = SkipSpaces(strstr(param_str, "OUTPUT") + 6);
	const char *token = SkipSpaces(strtok((char*)param," ="));
	const char *name = SkipSpaces(strtok(NULL,"=\0"));

	ParseOutputReg(name,&reg,&is_fp16);

	p.alias = token;
	p.index = reg;
	p.is_fp16 = (strncmp(param_str, "SHORT", 5) == 0) || is_fp16;

	m_lOParameters.push_back(p);
}

const char* CFPParser::ParseOutputReg(const char *token, s32 *reg,u8 *is_fp16)
{
	if(isdigit(*token)) {
		char *p = (char*)token;
		while(isdigit(*p)) p++;

		*reg = atoi(token);

		return (token + (p - token));
	}

	for(size_t i=0;i<FP_OUTPUTS_CNT;i++) {
		size_t tlen = strlen(fp_outputs[i].name.c_str());
		if(strncmp(token,fp_outputs[i].name.c_str(),tlen)==0) {
			*reg = fp_outputs[i].index;
			if(strcmp(fp_outputs[i].name.c_str(), "result.color")==0 ||
			   strcmp(fp_outputs[i].name.c_str(), "COLR")==0 ||
			   strcmp(fp_outputs[i].name.c_str(), "COLH")==0)
			{
				if(strcmp(fp_outputs[i].name.c_str(), "COLH")==0)
					*is_fp16 = 1;

				if(token[tlen]=='[' && isdigit(token[tlen+1])) {
					char *p = (char*)(token + tlen + 1);
					while(isdigit(*p)) p++;

					*reg = *reg + atoi(token + tlen + 1) + 1;
					tlen = (p - token) + 1;
				}
			}
			return (token + tlen);
		}
	}
	return NULL;
}

const char* CFPParser::ParseInputReg(const char *token, s32 *reg)
{
	if(isdigit(*token)) {
		char *p = (char*)token;
		while(isdigit(*p)) p++;

		*reg = atoi(token);

		return (token + (p - token));
	}

	for(size_t i=0;i<FP_INPUTS_CNT;i++) {
		size_t tlen = strlen(fp_inputs[i].name.c_str());
		if(strncmp(token,fp_inputs[i].name.c_str(),tlen)==0) {
			*reg = fp_inputs[i].index;
			if(strcmp(fp_inputs[i].name.c_str(),"fragment.texcoord")==0) {
				if(token[tlen]!='[' || !isdigit(token[tlen+1])) return NULL;

				char *p = (char*)(token + tlen + 1);
				while(isdigit(*p)) p++;

				*reg = *reg + atoi(token + tlen + 1);

				if(*p!=']') return NULL;

				tlen = (p - token) + 1;
			}
			return (token + tlen);
		}
	}
	return NULL;
}

const char* CFPParser::ParseOutputRegAlias(const char *token,s32 *reg,u8 *is_fp16)
{
	std::list<oparam>::iterator it = m_lOParameters.begin();

	*is_fp16 = 0;

	for(;it!=m_lOParameters.end();it++) {
		size_t len = 0;
		size_t tlen = strlen(token);

		// cut off the dst mask for comparison
		while ((len < it->alias.size() && len < tlen) && // Bounds check
			(token[len] != '.' && token[len] != '(' && token[len] != '\0')) { // Delimiter check
			len++;
		}
		
		if(strncmp(token, it->alias.c_str(), len) == 0) {
			*reg = it->index;
			*is_fp16 = it->is_fp16;
			return token + len;
		}
	}
	return ParseOutputReg(token,reg,is_fp16);
}

void CFPParser::ParseMaskedDstReg(const char *token,struct nvfx_insn *insn)
{
	s32 idx;
	u8 is_fp16 = 0;

	if(!token) return;

	if(strncmp(token,"RC",2)==0 ||
	   strncmp(token,"HC",2)==0)
	{
		SetNoneDestReg(insn);
		token += 2;
	} else if(token[0]=='R' || token[0]=='H') {
		insn->dst.type = NVFXSR_TEMP;
		insn->dst.is_fp16 = (token[0]=='H');
		token = ParseTempReg(token,&insn->dst.index);
	} else if(token[0]=='o' && token[1]=='[') {
		token = ParseOutputReg(&token[2],&idx,&is_fp16);
		token++;

		insn->dst.type = NVFXSR_OUTPUT;
		insn->dst.index = idx;
		insn->dst.is_fp16 = is_fp16;
	} else {
		token = ParseOutputRegAlias(token,&idx,&is_fp16);

		insn->dst.type = NVFXSR_OUTPUT;
		insn->dst.index = idx;
	}
	ParseMaskedDstRegExt(token,insn);
}

void CFPParser::ParseVectorSrc(const char *token,struct nvfx_src *reg)
{
	s32 idx;
	//f32 sign = 1.0f;

	if(!token) return;

	if(token[0]=='-') {
		reg->negate = TRUE;
		token++;
	} else if(token[0]=='+') {
		reg->negate = FALSE;
		token++;
	}

	if(token[0]=='|') {
		reg->abs = TRUE;
		token++;
	}

	if(token[0]=='R' || token[0]=='H') {
		reg->reg.type = NVFXSR_TEMP;
		reg->reg.is_fp16 = (token[0]=='H');
		token = ParseTempReg(token,&reg->reg.index);
	} else if(token[0]=='f') {
		if(token[1]=='[') {
			token = ParseInputReg(&token[2],&idx);
			if(*token==']') token++;
		} else
			token = ParseInputReg(token,&idx);

		reg->reg.type = NVFXSR_INPUT;
		reg->reg.index = idx;
	} else if(token[0]=='c' && token[1]=='[' && isdigit(token[2])) {
		char *p = (char*)(token + 2);
		
		while(isdigit(*p)) p++;

		reg->reg.index = atoi(token+2);
		reg->reg.type = GetConstRegType(reg->reg.index);

		if(*p==']') p++;
		token = p;
	}

	token = ParseRegSwizzle(token,reg);
}

void CFPParser::ParseScalarSrc(const char *token,struct nvfx_src *reg)
{
	s32 idx = -1;

	if(!token) return;

	if(token[0]=='-') {
		reg->negate = TRUE;
		token++;
	} else if(token[0]=='+') {
		reg->negate = FALSE;
		token++;
	}

	if(token[0]=='|') {
		reg->abs = TRUE;
		token++;
	}

	if(token[0]=='R' || token[0]=='H') {
		token = ParseTempReg(token,&idx);

		reg->reg.type = NVFXSR_TEMP;
		reg->reg.index = idx;
	} else if(token[0]=='f') {
		if(token[1]=='[') {
			token = ParseInputReg(&token[2],&idx);
			if(*token==']') token++;
		} else
			token = ParseInputReg(token,&idx);

		reg->reg.type = NVFXSR_INPUT;
		reg->reg.index = idx;
	} else if(token[0]=='c' && token[1]=='[' && isdigit(token[2])) {
		char *p = (char*)(token + 2);
		
		while(isdigit(*p)) p++;

		reg->reg.index = atoi(token+2);
		reg->reg.type = GetConstRegType(reg->reg.index);

		if(*p==']') p++;
		token = p;
	}

	token = ParseRegSwizzle(token,reg);
}

int CFPParser::GetConstRegType(int index)
{
	s32 i;
	std::list<param>::iterator it = m_lParameters.begin();

	for(;it!=m_lParameters.end();it++) {
		for(i=0;i<it->count;i++) {
			if((int)(it->index + i)==index) {
				if(it->is_const && it->is_internal)
					return NVFXSR_IMM;
				else if(it->is_const)
					return NVFXSR_CONST;
			}
		}
	}
	return -1;
}

const char* CFPParser::ParseOutputMask(const char *token,u8 *mask)
{
	if(!token) return NULL;

	if(token[0]=='.') {
		s32 k = 0;

		token++;

		*mask = 0;
		if(token[k]=='x') {
			*mask |= NVFX_FP_MASK_X;
			k++;
		}
		if(token[k]=='y') {
			*mask |= NVFX_FP_MASK_Y;
			k++;
		}
		if(token[k]=='z') {
			*mask |= NVFX_FP_MASK_Z;
			k++;
		}
		if(token[k]=='w') {
			*mask |= NVFX_FP_MASK_W;
			k++;
		}
		token += k;
	}
	return token;
}

void CFPParser::SetNoneDestReg(struct nvfx_insn *insn)
{
	insn->dst.type = NVFXSR_NONE;
	insn->dst.index = 0x3f;
	insn->dst.is_fp16 = 0;		//always treat as fp32 (on RSX there's only RC)
}

u8 CFPParser::IsPCDisablingInstruction(struct nvfx_insn *insn)
{
	switch(insn->op) {
		case OPCODE_DP2:
		case OPCODE_DP2A:
		case OPCODE_DP3:
		case OPCODE_DP4:
		case OPCODE_MUL:
		case OPCODE_DIV:
		case OPCODE_NRM3:
			return 1;
		case OPCODE_TEX:
			if(insn->tex_target == PARAM_SAMPLERCUBE) return 1;
		default:
			return 0;
	}
}
