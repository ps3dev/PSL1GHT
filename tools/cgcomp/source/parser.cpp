#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"

static paramtype paramtypes[] =
{
	{ "float", PARAM_FLOAT },
	{ "float2", PARAM_FLOAT2 },
	{ "float3", PARAM_FLOAT3 },
	{ "float4", PARAM_FLOAT4 },
	{ "float4x4", PARAM_FLOAT4x4 },
	{ "sampler1D", PARAM_SAMPLER1D },
	{ "sampler2D", PARAM_SAMPLER2D },
	{ "sampler3D", PARAM_SAMPLER3D },
	{ "samplerCUBE", PARAM_SAMPLERCUBE },
	{ "samplerRECT", PARAM_SAMPLERRECT },
};
static const u32 PARAM_TYPE_CNT = sizeof(paramtypes)/sizeof(struct _paramtype);

CParser::CParser()
{
	m_nOption = 0;
	m_nInstructions = 0;
}

CParser::~CParser()
{
	if(m_pInstructions) delete [] m_pInstructions;
}

void CParser::ParseComment(const char *line)
{
	param p;

	if(!line) return;

	line++;

	if(strncasecmp(line,"var",3)==0) {
		char *token = SkipSpaces(strtok((char*)(line+3)," :"));
		p.type = GetParamType(token);
		p.is_const = 0;
		p.is_internal = 0;
		p.count = 1;
		p.name = SkipSpaces(strtok(NULL," :"));

		token = SkipSpaces(strtok(NULL," :"));
		if(strstr(token,"$vin")) {
			token = SkipSpaces(strtok(NULL," :"));
			if(strncasecmp(token,"ATTR",4)==0)
				p.index = atoi(token+4);
			else
				p.index = ConvertInputReg(token);
		} else if(strstr(token,"texunit")) {
			token = SkipSpaces(strtok(NULL," :"));
			p.index = atoi(token);
		} else if(token[0]=='c') {
			p.is_const = 1;
			p.index = atoi(token+2);

			token = strtok(NULL," ,");
			if(isdigit(*token)) p.count = atoi(token);
		} else
			return;

		InitParameter(&p);

		m_lParameters.push_back(p);
	} else if(strncasecmp(line,"const",5)==0) {
		char  *token = SkipSpaces(strtok((char*)(line+5)," "));

		p.is_const = 1;
		p.is_internal = 1;
		p.type = PARAM_FLOAT4;
		p.count = 1;

		InitParameter(&p);

		if(token[0]=='c' && token[1]=='[') {
			u32 i;
			f32 *pVal = p.values[0];

			p.index = atoi(token+2);
			for(i=0;i<4;i++) {
				token = strtok(NULL," =");
				if(token)
					pVal[i] = (f32)atof(token);
				else
					pVal[i] = 0.0f;
			}
		} else
			return;

		m_lParameters.push_back(p);
	}
}

void CParser::InitParameter(param *p)
{
	if(!p) return;

	p->values = (f32(*)[4])calloc(p->count*4,sizeof(f32));
	
	switch(p->type) {
		case PARAM_FLOAT4x4:
			p->values[0][0] = 1.0f;
			p->values[1][1] = 1.0f;
			p->values[2][2] = 1.0f;
			p->values[3][3] = 1.0f;
			break;
		default:
			break;
	}
}

const char* CParser::ConvertCond(const char *token,struct nvfx_insn *insn)
{
	if(strncasecmp(token,"FL",2)==0)
		insn->cc_cond = NVFX_COND_FL;
	else if(strncasecmp(token,"LT",2)==0)
		insn->cc_cond = NVFX_COND_LT;
	else if(strncasecmp(token,"EQ",2)==0)
		insn->cc_cond = NVFX_COND_EQ;
	else if(strncasecmp(token,"LE",2)==0)
		insn->cc_cond = NVFX_COND_LE;
	else if(strncasecmp(token,"GT",2)==0)
		insn->cc_cond = NVFX_COND_GT;
	else if(strncasecmp(token,"NE",2)==0)
		insn->cc_cond = NVFX_COND_NE;
	else if(strncasecmp(token,"GE",2)==0)
		insn->cc_cond = NVFX_COND_GE;
	else if(strncasecmp(token,"TR",2)==0)
		insn->cc_cond = NVFX_COND_TR;
	
	token += 2;
	if(isdigit(*token)) {
		insn->cc_test_reg = atoi(token);
		token++;
	}
	insn->cc_test = 1;

	return token;
}


void CParser::InitInstruction(struct nvfx_insn *insn,u8 op)
{
	insn->op = op;
	insn->scale = 0;
	insn->unit = -1;
	insn->precision = FLOAT32;
	insn->mask = NVFX_VP_MASK_ALL;
	insn->cc_swz[0] = 0; insn->cc_swz[1] = 1; insn->cc_swz[2] = 2; insn->cc_swz[3] = 3;
	insn->sat = FALSE;
	insn->cc_update = FALSE;
	insn->cc_update_reg = 0;
	insn->cc_cond = NVFX_COND_TR;
	insn->cc_test = 0;
	insn->cc_test_reg = 0;
	insn->dst = nvfx_reg(NVFXSR_NONE,0);
	insn->src[0] = nvfx_src(nvfx_reg(NVFXSR_NONE,0)); insn->src[1] = nvfx_src(nvfx_reg(NVFXSR_NONE,0)); insn->src[2] = nvfx_src(nvfx_reg(NVFXSR_NONE,0));
}

bool CParser::isLetter(int c)
{
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

bool CParser::isDigit(int c)
{
	return (c>='0' && c<='9');
}

bool CParser::isWhitespace(int c)
{
	return (c==' ' || c=='\t' || c=='\n' || c=='\r');
}

s32 CParser::GetParamType(const char *param_str)
{
	u32 i;

	for(i=0;i<PARAM_TYPE_CNT;i++) {
		if(strcasecmp(param_str,paramtypes[i].ident)==0)
			return (s32)paramtypes[i].type;
	}
	return -1;
}

const char* CParser::ParseTempReg(const char *token,s32 *reg)
{
	const char *p;

	if(token[0]!='R' && token[0]!='H')
		return NULL;

	if(!isdigit(token[1]))
		return token;

	p = token + 1;
	while(isdigit(*p)) p++;

	*reg = atoi(token+1);

	return p;
}

const char* CParser::ParseMaskedDstRegExt(const char *token,struct nvfx_insn *insn)
{
	token = ParseOutputMask(token,&insn->mask);
	if(token && *token!='\0') {
		if(token[0]=='(') {
			token = ParseCond(&token[1],insn);
			token++;
		}
	}
	return token;
}

const char* CParser::ParseCond(const char *token,struct nvfx_insn *insn)
{
	token = ConvertCond(token,insn);
	if(token[0]=='.') {
		s32 k = 0;
		
		token++;

		insn->cc_swz[0] = insn->cc_swz[1] = insn->cc_swz[2] = insn->cc_swz[3] = 0;
		for(k=0;token[k] && token[k]!=')' && k<4;k++) {
			if(token[k]=='x')
				insn->cc_swz[k] = NVFX_SWZ_X;
			else if(token[k]=='y')
				insn->cc_swz[k] = NVFX_SWZ_Y;
			else if(token[k]=='z')
				insn->cc_swz[k] = NVFX_SWZ_Z;
			else if(token[k]=='w')
				insn->cc_swz[k] = NVFX_SWZ_W;
		}
		if(k && k<4) {
			u8 lastswz = insn->cc_swz[k - 1];
			while(k<4) {
				insn->cc_swz[k] = lastswz;
				k++;
			}
		}
		token += k;
	}
	return token;
}

const char* CParser::ParseRegSwizzle(const char *token,struct nvfx_src *reg)
{
	if(token && *token!='\0') {
		if(token[0]=='.') {
			u32 k;

			token++;

			reg->swz[0] = reg->swz[1] = reg->swz[2] = reg->swz[3] = 0;
			for(k=0;*token!='\0' && k<4;k++,token++) {
				if(*token=='x')
					reg->swz[k] = NVFX_SWZ_X;
				else if(*token=='y')
					reg->swz[k] = NVFX_SWZ_Y;
				else if(*token=='z')
					reg->swz[k] = NVFX_SWZ_Z;
				else if(*token=='w')
					reg->swz[k] = NVFX_SWZ_W;
			}
			if(k && k<4) {
				u8 lastswz = reg->swz[k - 1];
				while(k<4) {
					reg->swz[k] = lastswz;
					k++;
				}
			}
		}
	}
	return token;
}
