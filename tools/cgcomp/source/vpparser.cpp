#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vpparser.h"

struct _opcode
{
	const char *mnemonic;
	u32 opcode;
	s32 src_slots[3];
	u32 nr_src;
	bool is_imm;
} vp_opcodes[] = {
	{ "NOP", OPCODE_NOP,{-1,-1,-1},0,false },
	{ "ABS", OPCODE_ABS,{ 0,-1,-1},1,false },
	{ "ADD", OPCODE_ADD,{ 0, 2,-1},2,false },
	{ "ARA", OPCODE_ARA,{ 0,-1,-1},1,false },
	{ "ARL", OPCODE_ARL,{ 0,-1,-1},1,false },
	{ "ARR", OPCODE_ARR,{ 0,-1,-1},1,false },
	{ "BRA", OPCODE_BRA,{-1,-1,-1},0,true },
	{ "CAL", OPCODE_CAL,{-1,-1,-1},0,true },
	{ "COS", OPCODE_COS,{ 2,-1,-1},1,false },
	{ "DP3", OPCODE_DP3,{ 0, 1,-1},2,false },
	{ "DP4", OPCODE_DP4,{ 0, 1,-1},2,false },
	{ "DPH", OPCODE_DPH,{ 0, 1,-1},2,false },
	{ "DST", OPCODE_DST,{ 0, 1,-1},2,false },
	{ "EX2", OPCODE_EX2,{ 2,-1,-1},1,false },
	{ "EXP", OPCODE_EXP,{ 2,-1,-1},1,false },
	{ "FLR", OPCODE_FLR,{ 0,-1,-1},1,false },
	{ "FRC", OPCODE_FRC,{ 0,-1,-1},1,false },
	{ "LG2", OPCODE_LG2,{ 2,-1,-1},1,false },
	{ "LIT", OPCODE_LIT,{ 2,-1,-1},1,false },
	{ "LOG", OPCODE_LOG,{ 2,-1,-1},1,false },
	{ "MAD", OPCODE_MAD,{ 0, 1, 2},3,false },
	{ "MAX", OPCODE_MAX,{ 0, 1,-1},2,false },
	{ "MIN", OPCODE_MIN,{ 0, 1,-1},2,false },
	{ "MOV", OPCODE_MOV,{ 0,-1,-1},1,false },
	{ "MUL", OPCODE_MUL,{ 0, 1,-1},2,false },
	{ "POPA",OPCODE_POPA,{-1,-1,-1},0,false },
	{ "POW", OPCODE_POW,{ 0, 1,-1},2,false },
	{ "PUSHA",OPCODE_PUSHA,{-1,-1,-1},0,false },
	{ "RCC", OPCODE_RCC,{ 2,-1,-1},1,false },
	{ "RCP", OPCODE_RCP,{ 2,-1,-1},1,false },
	{ "RET", OPCODE_RET,{ 2,-1,-1},1,false },
	{ "RSQ", OPCODE_RSQ,{ 2,-1,-1},1,false },
	{ "SEQ", OPCODE_SEQ,{ 0, 1,-1},2,false },
	{ "SFL", OPCODE_SFL,{ 0, 1,-1},2,false },
	{ "SGE", OPCODE_SGE,{ 0, 1,-1},2,false },
	{ "SGT", OPCODE_SGT,{ 0, 1,-1},2,false },
	{ "SIN", OPCODE_SIN,{ 2,-1,-1},1,false },
	{ "SLE", OPCODE_SLE,{ 0, 1,-1},2,false },
	{ "SLT", OPCODE_SLT,{ 0, 1,-1},2,false },
	{ "SNE", OPCODE_SNE,{ 0, 1,-1},2,false },
	{ "SSG", OPCODE_SSG,{ 0, 1,-1},2,false },
	{ "STR", OPCODE_STR,{ 0, 1,-1},2,false },
	{ "SUB", OPCODE_SUB,{ 0, 2,-1},2,false },
	{ "SWZ", OPCODE_SWZ,{ 0, 2,-1},2,false },
	{ "TEX", OPCODE_TEX,{ 0, 1,-1},2,false },
	{ "TXB", OPCODE_TXB,{ 0, 1,-1},2,false },
	{ "TXL", OPCODE_TXL,{ 0, 1,-1},2,false },
	{ "TXP", OPCODE_TXP,{ 0, 1,-1},2,false },
	{ "XPD", OPCODE_XPD,{ 0, 1,-1},2,false },
	/* end */
	{ "END", OPCODE_END,{},0,false}
};
static const u32 VP_OPCODES_CNT = sizeof(vp_opcodes)/sizeof(struct _opcode);

static ioset vp_inputs[] =
{
	{ "vertex.position", 0 },
	{ "vertex.weight", 1 },
	{ "vertex.normal", 2 },
	{ "vertex.color.secondary", 4 },
	{ "vertex.color.primary", 3 },
	{ "vertex.color", 3 },
	{ "vertex.fogcoord", 5 },
	{ "vertex.texcoord", 8 },
	{ "vertex.attrib", 16 },
	{ "OPOS", 0 },
	{ "POSITION", 0 },
	{ "WHGT", 1 },
	{ "NRML", 2 },
	{ "COL0", 3 },
	{ "COL1", 4 },
	{ "FOGC", 5 },
	{ "TEX0", 8 },
	{ "TEX1", 9 },
	{ "TEX2", 10 },
	{ "TEX3", 11 },
	{ "TEX4", 12 },
	{ "TEX5", 13 },
	{ "TEX6", 14 },
	{ "TEX7", 15 }
};
static const u32 VP_INPUTS_CNT = sizeof(vp_inputs)/sizeof(ioset);

static ioset vp_outputs[] =
{
	{ "result.position", 0 },
	{ "result.color.front.secondary", 2 },
	{ "result.color.back.secondary", 4 },
	{ "result.color.front.primary", 1 },
	{ "result.color.back.primary", 3 },
	{ "result.color.secondary", 2 },
	{ "result.color.primary", 1 },
	{ "result.color.front", 1 },
	{ "result.color.back", 3 },
	{ "result.color", 1 },
	{ "result.fogcoord", 5 },
	{ "result.pointsize", 6 },
	{ "result.texcoord", 7 },
	{ "result.clip", 17 },
	{ "HPOS", 0 },
	{ "POSITION", 0 },
	{ "COL0", 1 },
	{ "COL1", 2 },
	{ "BFC0", 3 },
	{ "BFC1", 4 },
	{ "FOGC", 5 },
	{ "PSIZ", 6 },
	{ "PSZ", 6 },
	{ "TEX0", 7 },
	{ "TEX1", 8 },
	{ "TEX2", 9 },
	{ "TEX3", 10 },
	{ "TEX4", 11 },
	{ "TEX5", 12 },
	{ "TEX6", 13 },
	{ "TEX7", 14 },
	{ "TEMP", 15 }
};
static const u32 VP_OUTPUTS_CNT = sizeof(vp_outputs)/sizeof(ioset);

CVPParser::CVPParser() : CParser()
{
	m_pInstructions = new struct nvfx_insn[MAX_NV_VERTEX_PROGRAM_INSTRUCTIONS];
}

CVPParser::~CVPParser()
{
}

int CVPParser::Parse(const char *str)
{
	int i,iline = 0;
	bool inProgram = false;
	std::stringstream input(str);
	struct nvfx_src none = nvfx_src(nvfx_reg(NVFXSR_NONE,0));

	while(!input.eof()) {
		char line[256];
		opcode *opc = NULL;
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
			if(strncmp(line,"!!VP2.0",7)==0)
				inProgram = true;
			else if(strncmp(line,"!!ARBvp1.0",10)==0)
				inProgram = true;

			continue;
		}

		char *label = NULL;
		char *col_ptr = NULL;
		char *opcode = NULL;
		char *ptr = line;
		
		if((col_ptr = strstr((char*)ptr,":"))!=NULL) {
			int j = 0;
			bool valid = true;
			
			while((ptr+j)<col_ptr) {
				if(j==0 && !(isLetter(ptr[j]) || ptr[j]=='_')) valid = false;
				if(!(isLetter(ptr[j]) || isDigit(ptr[j]) || ptr[j]=='_')) valid = false;
				j++;
			}

			if(valid) {
				label = strtok(ptr,":\x20");
				ptr = col_ptr + 1;
			}
		}

		opcode = strtok(ptr," ");

		if(label) {
			jmpdst d;

			strcpy(d.ident,label);
			d.location = m_nInstructions;
			m_lIdent.push_back(d);
		}

		if(opcode) {
			char *param_str = SkipSpaces(strtok(NULL,"\0"));
			if(strcasecmp(opcode,"OPTION")==0) {
				if(strncasecmp(param_str,"NV_vertex_program3",18)==0)
					m_nOption |= NV_OPTION_VP3;
				continue;
			} else if(strcasecmp(opcode,"PARAM")==0)
				continue;
			else if(strcasecmp(opcode,"TEMP")==0)
				continue;
			else {
				opc = FindOpcode(opcode);
				insn = &m_pInstructions[m_nInstructions];

				if(!opc) continue;

				InitInstruction(insn,opc->opcode);
				if(opc->opcode==OPCODE_END) {
					m_nInstructions++;
					break;
				}

				char *opc_ext = opcode + strlen(opc->mnemonic);
				if(m_nOption&(NV_OPTION_VP2|NV_OPTION_VP3)) {
					if(opc_ext[0]=='C') {
						insn->cc_update = TRUE;

						if(m_nOption&NV_OPTION_VP3 && (opc_ext[1]=='0' || opc_ext[1]=='1')) {
							switch(opc_ext[1]) {
								case '0':
									insn->cc_update_reg = 0;
									break;
								case '1':
									insn->cc_update_reg = 1;
									break;
							}
							opc_ext++;
						}
						opc_ext++;
					}
				}
				if(opc_ext[0]=='_') {
					if(strncasecmp(opc_ext,"_sat",4)==0) insn->sat = TRUE;
				}
				ParseInstruction(insn,opc,param_str);
				m_nInstructions++;
			}
		}
	}

	for(std::list<jmpdst>::iterator r=m_lJmpDst.begin();r!=m_lJmpDst.end();r++) {
		bool found = false;

		for(std::list<jmpdst>::iterator i=m_lIdent.begin();i!=m_lIdent.end();i++) {
			if(strcmp(r->ident,i->ident)==0) {
				found = true;
				m_pInstructions[r->location].dst = nvfx_reg(NVFXSR_RELOCATED,i->location);
				break;
			}
		}

		if(found==false) {
			fprintf(stderr,"Identifier \'%s\' not found.\n",r->ident);
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

void CVPParser::ParseInstruction(struct nvfx_insn *insn,opcode *opc,const char *param_str)
{
	u32 i;
	char *token = SkipSpaces(strtok((char*)param_str,","));

	if(opc->is_imm)
		ParseMaskedDstAddr(token,insn);
	else
		ParseMaskedDstReg(token,insn);

	for(i=0;i<opc->nr_src;i++) {
		token = SkipSpaces(strtok(NULL,","));
		ParseSwizzledSrcReg(token,&insn->src[opc->src_slots[i]]);
	}
}

void CVPParser::ParseMaskedDstReg(const char *token,struct nvfx_insn *insn)
{
	s32 idx;

	if(!token) return;

	if(token[0]=='R') {
		if(token[1]=='C') return;

		token = ParseTempReg(token,&idx);
		insn->dst.type = NVFXSR_TEMP;
		insn->dst.index = idx;
	} else if(token[0]=='r' && token[1]=='e') {
		token = ParseOutputReg(token,&idx);
		insn->dst.type = NVFXSR_OUTPUT;
		insn->dst.index = idx;
	} else if(token[0]=='o' && token[1]=='[') {
		token = ParseOutputReg(&token[2],&idx);
		insn->dst.type = NVFXSR_OUTPUT;
		insn->dst.index = idx;
	} else if(token[0]=='C' && token[1]=='C')
		return;

	ParseMaskedDstRegExt(token,insn);
}

opcode* CVPParser::FindOpcode(const char *mnemonic)
{
	u32 i;

	for(i=0;i<VP_OPCODES_CNT;i++) {
		if(strncmp(mnemonic,vp_opcodes[i].mnemonic,strlen(vp_opcodes[i].mnemonic))==0) return &vp_opcodes[i];
	}
	return NULL;
}

void CVPParser::ParseMaskedDstAddr(const char *token,struct nvfx_insn *insn)
{
	jmpdst d;
	u32 len;

	if(!token) return;

	char *cond = (char*)strchr(token,'(');

	len = (u32)strlen(token);
	if(cond) len = (cond - token);

	strncpy(d.ident,token,len);
	d.location = m_nInstructions;
	m_lJmpDst.push_back(d);

	ParseMaskedDstRegExt(cond,insn);
}

void CVPParser::ParseSwizzledSrcReg(const char *token,struct nvfx_src *reg)
{
	s32 idx;

	if(!token) return;

	if(token[0]=='-') {
		reg->negate = TRUE;
		token++;
	}
	if(token[0]=='|') {
		reg->abs = TRUE;
		token++;
	}

	if(token[0]=='v') {
		if(token[1]=='e')
			token = ParseInputReg(token,&idx);
		else if(token[1]=='[') {
			token = ParseInputReg(&token[2],&idx);
			if(*token==']') token++;
		}

		reg->reg.type = NVFXSR_INPUT;
		reg->reg.index = idx;
	} else if(token[0]=='R') {
		token = ParseTempReg(token,&idx);
		reg->reg.type = NVFXSR_TEMP;
		reg->reg.index = idx;
	} else if(token[0]=='c' && token[1]=='[') {
		token = ParseParamReg(&token[2],reg);
	}

	token = ParseRegSwizzle(token,reg);
}


const char* CVPParser::ParseParamReg(const char *token,struct nvfx_src *reg)
{
	if(!token) return NULL;

	char *p = (char*)token;

	if(isdigit(*p)) {
		reg->reg.type = NVFXSR_CONST;
		reg->reg.index = atoi(p);

		while(isdigit(*p)) p++;
	} else if(strncmp(p,"A0",2)==0 || strncmp(p,"A1",2)==0) {
		reg->reg.type = NVFXSR_IMM;
		reg->reg.index = 0;

		reg->indirect = TRUE;
		reg->indirect_reg = atoi(p+1);

		p += 2;
		if(*p=='.') {
			u32 k;

			p++;

			reg->indirect_swz = 0;
			for(k=0;p[k] && k<4;k++) {
				if(p[k]=='x')
					reg->indirect_swz |= NVFX_SWZ_X;
				else if(p[k]=='y')
					reg->indirect_swz |= NVFX_SWZ_Y;
				else if(p[k]=='z')
					reg->indirect_swz |= NVFX_SWZ_Z;
				else if(p[k]=='w')
					reg->indirect_swz |= NVFX_SWZ_W;
			}
			p += k;
		}
		if(*p=='-' || *p=='+') {
			const char sign = *p++;

			if(isdigit(*p)) {
				const s32 k = atoi(p);
				if(sign=='-') {
					if(k<64) reg->reg.index = -k;
				} else {
					if(k<63) reg->reg.index = k;
				}
				while(isdigit(*p)) p++;
			}
		}
	}
	if(*p!=']') return NULL;
	
	return ++p;
}

s32 CVPParser::ConvertInputReg(const char *token)
{
	if(strcasecmp(token,"POSITION")==0)
		return 0;
	if(strcasecmp(token,"WHGT")==0)
		return 1;
	if(strcasecmp(token,"NORMAL")==0)
		return 2;
	if(strcasecmp(token,"COLOR")==0 || strcasecmp(token,"COLOR0")==0 || strcasecmp(token,"DIFFUSE")==0)
		return 3;
	if(strcasecmp(token,"COLOR1")==0 || strcasecmp(token,"SPECULAR")==0)
		return 4;
	if(strcasecmp(token,"FOGCOORD")==0)
		return 5;
	if(strcasecmp(token,"TEXCOORD0")==0)
		return 8;
	if(strcasecmp(token,"TEXCOORD1")==0)
		return 9;
	if(strcasecmp(token,"TEXCOORD2")==0)
		return 10;
	if(strcasecmp(token,"TEXCOORD3")==0)
		return 11;
	if(strcasecmp(token,"TEXCOORD4")==0)
		return 12;
	if(strcasecmp(token,"TEXCOORD5")==0)
		return 13;
	if(strcasecmp(token,"TEXCOORD6")==0)
		return 14;
	if(strcasecmp(token,"TEXCOORD7")==0)
		return 15;

	return -1;
}

const char* CVPParser::ParseOutputReg(const char *token,s32 *reg)
{
	u32 i;

	if(isdigit(*token)) {
		char *p = (char*)token;
		while(isdigit(*p)) p++;

		*reg = atoi(token);

		return (token + (p - token));
	}

	for(i=0;i<VP_OUTPUTS_CNT;i++) {
		u32 tlen = (u32)strlen(vp_outputs[i].name);
		if(strncmp(token,vp_outputs[i].name,tlen)==0) {
			*reg = vp_outputs[i].index;
			if(strcmp(vp_outputs[i].name,"result.texcoord")==0 ||
				strcmp(vp_outputs[i].name,"result.clip")==0) 
			{
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

const char* CVPParser::ParseInputReg(const char *token,s32 *reg)
{
	u32 i;

	if(isdigit(*token)) {
		char *p = (char*)token;
		while(isdigit(*p)) p++;

		*reg = atoi(token);

		return (token + (p - token));
	}

	for(i=0;i<VP_INPUTS_CNT;i++) {
		u32 tlen = (u32)strlen(vp_inputs[i].name);
		if(strncmp(token,vp_inputs[i].name,tlen)==0) {
			*reg = vp_inputs[i].index;
			if(strcmp(vp_inputs[i].name,"vertex.texcoord")==0 ||
				strcmp(vp_inputs[i].name,"vertex.attrib")==0)
			{
				u32 off = 0;

				if(token[tlen]!='[' || !isdigit(token[tlen+1])) return NULL;

				char *p = (char*)(token + tlen + 1);
				while(isdigit(*p)) p++;

				if(vp_inputs[i].index==16) off = 16;
				*reg = *reg + atoi(token + tlen + 1) - off;

				if(*p!=']') return NULL;

				tlen = (p - token) + 1;
			}
			return (token + tlen);
		}
	}
	return NULL;
}

const char* CVPParser::ParseOutputMask(const char *token,u8 *mask)
{
	if(!token) return NULL;

	if(token[0]=='.') {
		s32 k = 0;

		token++;

		*mask = 0;
		if(token[k]=='x') {
			*mask |= NVFX_VP_MASK_X;
			k++;
		}
		if(token[k]=='y') {
			*mask |= NVFX_VP_MASK_Y;
			k++;
		}
		if(token[k]=='z') {
			*mask |= NVFX_VP_MASK_Z;
			k++;
		}
		if(token[k]=='w') {
			*mask |= NVFX_VP_MASK_W;
			k++;
		}
		token += k;
	}
	return token;
}
