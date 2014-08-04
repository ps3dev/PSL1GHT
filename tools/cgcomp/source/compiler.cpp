#include "types.h"

#include "parser.h"
#include "compiler.h"

#define gen_op(o,t) \
	((NVFX_VP_INST_SLOT_##t<<7)|NVFX_VP_INST_##t##_OP_##o)

#define arith(s,d,m,s0,s1,s2) \
	nvfx_insn((s), 0, -1, (d), (m), (s0), (s1), (s2))

#define arith_ctor(ins,d,s0,s1,s2) \
	nvfx_insn_ctor((ins), (d), (s0), (s1), (s2))

static INLINE s32 ffs(u32 u)
{
	u32 i = 0;
	if(!(u&0xffffffff)) return 0;
	while(!(u&0x1)) {
		u >>= 1;
		i++;
	}
	return i + 1;
}

CCompiler::CCompiler()
{
	m_nInputMask = 0;
	m_nOutputMask = 0;
	m_nInstructions = 0;
	m_nConsts = 0;
	m_rTemps = 0;
	m_nNumRegs = 1;
	m_rTempsDiscard = 0;
	m_nCurInstruction = 0;
	m_pInstructions = NULL;
	m_pConstData = NULL;
	m_rTemp = NULL;
	m_rConst = NULL;
}

CCompiler::~CCompiler()
{
}

void CCompiler::Prepare(CParser *pParser)
{
	s32 high_const = -1,high_temp = -1;
	u32 i,j,nICount = pParser->GetInstructionCount();
	struct nvfx_insn *insns = pParser->GetInstructions();
	
	for(i=0;i<nICount;i++) {
		struct nvfx_insn *insn = &insns[i];

		for(j=0;j<3;j++) {
			struct nvfx_src *src = &insn->src[j];

			switch(src->reg.type) {
				case NVFXSR_TEMP:
					if((s32)src->reg.index>high_temp) high_temp = src->reg.index;
					break;
				case NVFXSR_CONST:
					if((s32)src->reg.index>high_const) high_const = src->reg.index;
					break;
			}
		}

		switch(insn->dst.type) {
			case NVFXSR_TEMP:
				if((s32)insn->dst.index>high_temp) high_temp = insn->dst.index;
				break;
			case NVFXSR_CONST:
				if((s32)insn->dst.index>high_const) high_const = insn->dst.index;
				break;
		}
	}

	if(++high_temp) {
		m_nNumRegs = high_temp;
		m_rTemp = (struct nvfx_reg*)calloc(high_temp,sizeof(struct nvfx_reg));
		for(i=0;i<(u32)high_temp;i++) m_rTemp[i] = temp();
		m_rTempsDiscard = 0;
	}

	if(++high_const) {
		m_rConst = (struct nvfx_reg*)calloc(high_const,sizeof(struct nvfx_reg));
		for(i=0;i<(u32)high_const;i++) m_rConst[i] = constant(i,0.0f,0.0f,0.0f,0.0f);
	}
}

void CCompiler::Compile(CParser *pParser)
{
	struct nvfx_src tmp;
	struct nvfx_relocation reloc;
	std::vector<u32> insns_pos;
	std::list<struct nvfx_relocation> label_reloc;
	int i,nICount = pParser->GetInstructionCount();
	struct nvfx_src none = nvfx_src(nvfx_reg(NVFXSR_NONE,0));
	struct nvfx_insn tmp_insn,*insns = pParser->GetInstructions();

	Prepare(pParser);

	for(i=0;i<nICount;i++) {
                /* u32 idx = (u32)insns_pos.size(); */
		struct nvfx_insn *insn = &insns[i];

		insns_pos.push_back(m_nInstructions);
		switch(insn->op) {
			case OPCODE_NOP:
				tmp_insn = arith(0,none.reg,0,none,none,none);
				emit_insn(gen_op(NOP,VEC),&tmp_insn);
				break;
			case OPCODE_ABS:
				tmp_insn = arith_ctor(insn,insn->dst,abs(insn->src[0]),none,none);
				emit_insn(gen_op(MOV,VEC),&tmp_insn);
				break;
			case OPCODE_ADD:
				emit_insn(gen_op(ADD,VEC),insn);
				break;
			case OPCODE_ARA:
				break;
			case OPCODE_ARL:
				break;
			case OPCODE_ARR:
				break;
			case OPCODE_BRA:
				reloc.location = m_nInstructions;
				reloc.target = insn->dst.index;
				label_reloc.push_back(reloc);

				tmp_insn = arith(0,none.reg,0,none,none,none);
				emit_insn(gen_op(BRA,SCA),&tmp_insn);
				break;
			case OPCODE_CAL:
				reloc.location = m_nInstructions;
				reloc.target = insn->dst.index;
				label_reloc.push_back(reloc);

				tmp_insn = arith(0,none.reg,0,none,none,none);
				emit_insn(gen_op(CAL,SCA),&tmp_insn);
				break;
			case OPCODE_COS:
				emit_insn(gen_op(COS,SCA),insn);
				break;
			case OPCODE_DP3:
				emit_insn(gen_op(DP3,VEC),insn);
				break;
			case OPCODE_DP4:
				emit_insn(gen_op(DP4,VEC),insn);
				break;
			case OPCODE_DPH:
				emit_insn(gen_op(DPH,VEC),insn);
				break;
			case OPCODE_DST:
				emit_insn(gen_op(DST,VEC),insn);
				break;
			case OPCODE_EX2:
				emit_insn(gen_op(EX2,SCA),insn);
				break;
			case OPCODE_EXP:
				emit_insn(gen_op(EXP,SCA),insn);
				break;
			case OPCODE_FLR:
				emit_insn(gen_op(FLR,VEC),insn);
				break;
			case OPCODE_FRC:
				emit_insn(gen_op(FRC,VEC),insn);
				break;
			case OPCODE_LG2:
				emit_insn(gen_op(LG2,SCA),insn);
				break;
			case OPCODE_LIT:
				emit_insn(gen_op(LIT,SCA),insn);
				break;
			case OPCODE_LOG:
				emit_insn(gen_op(LOG,SCA),insn);
				break;
			case OPCODE_MAD:
				emit_insn(gen_op(MAD,VEC),insn);
				break;
			case OPCODE_MAX:
				emit_insn(gen_op(MAX,VEC),insn);
				break;
			case OPCODE_MIN:
				emit_insn(gen_op(MIN,VEC),insn);
				break;
			case OPCODE_MOV:
				emit_insn(gen_op(MOV,VEC),insn);
				break;
			case OPCODE_MUL:
				emit_insn(gen_op(MUL,VEC),insn);
				break;
			case OPCODE_POW:
				tmp = nvfx_src(temp());
				
				tmp_insn = arith(0, tmp.reg, NVFX_VP_MASK_X, none, none, insn->src[0]);
				emit_insn(gen_op(LG2,SCA),&tmp_insn);

				tmp_insn = arith(0, tmp.reg, NVFX_VP_MASK_X, swz(tmp, X, X, X, X), insn->src[1], none);
				emit_insn(gen_op(MUL,VEC),&tmp_insn);
				
				tmp_insn = arith_ctor(insn, insn->dst, none, none, swz(tmp, X, X, X, X));
				emit_insn(gen_op(EX2,SCA),&tmp_insn);
				break;
			case OPCODE_RCC:
				emit_insn(gen_op(RCC,SCA),insn);
				break;
			case OPCODE_RCP:
				emit_insn(gen_op(RCP,SCA),insn);
				break;
			case OPCODE_RSQ:
				emit_insn(gen_op(RSQ,SCA),insn);
				break;
			case OPCODE_SEQ:
				emit_insn(gen_op(SEQ,VEC),insn);
				break;
			case OPCODE_SFL:
				emit_insn(gen_op(SFL,VEC),insn);
				break;
			case OPCODE_SGE:
				emit_insn(gen_op(SGE,VEC),insn);
				break;
			case OPCODE_SGT:
				emit_insn(gen_op(SGT,VEC),insn);
				break;
			case OPCODE_SIN:
				emit_insn(gen_op(SIN,SCA),insn);
				break;
			case OPCODE_SLE:
				emit_insn(gen_op(SLE,VEC),insn);
				break;
			case OPCODE_SLT:
				emit_insn(gen_op(SLT,VEC),insn);
				break;
			case OPCODE_SNE:
				emit_insn(gen_op(SNE,VEC),insn);
				break;
			case OPCODE_SSG:
				emit_insn(gen_op(SSG,VEC),insn);
				break;
			case OPCODE_STR:
				emit_insn(gen_op(STR,VEC),insn);
				break;
			case OPCODE_SUB:
				tmp_insn = arith_ctor(insn,insn->dst,insn->src[0],none,neg(insn->src[2]));
				emit_insn(gen_op(ADD,VEC),&tmp_insn);
				break;
			case OPCODE_END:
				if(m_nInstructions) m_pInstructions[m_nCurInstruction].data[3] |= NVFX_VP_INST_LAST;
				else {
					tmp_insn = arith(0,none.reg,0,none,none,none);
					emit_insn(gen_op(NOP,VEC),&tmp_insn);
					m_pInstructions[m_nCurInstruction].data[3] |= NVFX_VP_INST_LAST;
				}
				break;
		}
		release_temps();
	}

	for(std::list<struct nvfx_relocation>::iterator it = label_reloc.begin();it!=label_reloc.end();it++) {
		struct nvfx_relocation hw_reloc;

		hw_reloc.location = it->location;
		hw_reloc.target = insns_pos[it->target];

		m_lBranchRelocation.push_back(hw_reloc);
	}
}

void CCompiler::emit_insn(u8 opcode,struct nvfx_insn *insn)
{
	u32 *hw;
	u32 slot = opcode>>7;
	u32 op = opcode&0x7f;

	m_nCurInstruction = m_nInstructions++;
	m_pInstructions = (struct vertex_program_exec*)realloc(m_pInstructions,m_nInstructions*sizeof(struct vertex_program_exec));

	memset(&m_pInstructions[m_nCurInstruction],0,sizeof(struct vertex_program_exec));

	hw = m_pInstructions[m_nCurInstruction].data;

	emit_dst(hw,slot,insn);
	emit_src(hw,0,&insn->src[0]);
	emit_src(hw,1,&insn->src[1]);
	emit_src(hw,2,&insn->src[2]);

	hw[0] |= (insn->cc_cond << NVFX_VP(INST_COND_SHIFT));
	hw[0] |= (insn->cc_test << NVFX_VP(INST_COND_TEST_SHIFT));
	hw[0] |= (insn->cc_test_reg << NVFX_VP(INST_COND_REG_SELECT_SHIFT));
	hw[0] |= ((insn->cc_swz[0] << NVFX_VP(INST_COND_SWZ_X_SHIFT)) |
		  (insn->cc_swz[1] << NVFX_VP(INST_COND_SWZ_Y_SHIFT)) |
		  (insn->cc_swz[2] << NVFX_VP(INST_COND_SWZ_Z_SHIFT)) |
		  (insn->cc_swz[3] << NVFX_VP(INST_COND_SWZ_W_SHIFT)));
	if(insn->cc_update)
		hw[0] |= NVFX_VP(INST_COND_UPDATE_ENABLE);

	if(insn->sat)
	{
		hw[0] |= NV40_VP_INST_SATURATE;
	}

	if (slot == 0) {
		hw[1] |= (op << NV40_VP_INST_VEC_OPCODE_SHIFT);
		hw[3] |= NV40_VP_INST_SCA_DEST_TEMP_MASK;
		hw[3] |= (insn->mask << NV40_VP_INST_VEC_WRITEMASK_SHIFT);
    } else {
		hw[1] |= (op << NV40_VP_INST_SCA_OPCODE_SHIFT);
		hw[0] |= NV40_VP_INST_VEC_DEST_TEMP_MASK ;
		hw[3] |= (insn->mask << NV40_VP_INST_SCA_WRITEMASK_SHIFT);
	}
}

void CCompiler::emit_dst(u32 *hw,u8 slot,struct nvfx_insn *insn)
{
	struct nvfx_reg *dst = &insn->dst;

	switch(dst->type) {
		case NVFXSR_NONE:
			hw[3] |= NV40_VP_INST_DEST_MASK;
			if(slot==0)
				hw[0] |= NV40_VP_INST_VEC_DEST_TEMP_MASK;
			else
				hw[3] |= NV40_VP_INST_SCA_DEST_TEMP_MASK;
			break;
		case NVFXSR_TEMP:
			hw[3] |= NV40_VP_INST_DEST_MASK;
			if (slot == 0)
				hw[0] |= (dst->index << NV40_VP_INST_VEC_DEST_TEMP_SHIFT);
			else
				hw[3] |= (dst->index << NV40_VP_INST_SCA_DEST_TEMP_SHIFT);
			break;
		case NVFXSR_OUTPUT:
			switch (dst->index) {
				case NV30_VP_INST_DEST_CLP(0):
					dst->index = NVFX_VP(INST_DEST_FOGC);
					insn->mask = NVFX_VP_MASK_Y;
					m_nOutputMask |= (1 << 6);
					break;
				case NV30_VP_INST_DEST_CLP(1):
					dst->index = NVFX_VP(INST_DEST_FOGC);
					insn->mask = NVFX_VP_MASK_Z;
					m_nOutputMask |= (1 << 7);
					break;
				case NV30_VP_INST_DEST_CLP(2):
					dst->index = NVFX_VP(INST_DEST_FOGC);
					insn->mask = NVFX_VP_MASK_W;
					m_nOutputMask |= (1 << 8);
					break;
				case NV30_VP_INST_DEST_CLP(3):
					dst->index = NVFX_VP(INST_DEST_PSZ);
					insn->mask = NVFX_VP_MASK_Y;
					m_nOutputMask |= (1 << 9);
					break;
				case NV30_VP_INST_DEST_CLP(4):
					dst->index = NVFX_VP(INST_DEST_PSZ);
					insn->mask = NVFX_VP_MASK_Z;
					m_nOutputMask |= (1 << 10);
					break;
				case NV30_VP_INST_DEST_CLP(5):
					dst->index = NVFX_VP(INST_DEST_PSZ);
					insn->mask = NVFX_VP_MASK_W;
					m_nOutputMask |= (1 << 11);
					break;
				case NV40_VP_INST_DEST_COL0 : m_nOutputMask |= (1 << 0); break;
				case NV40_VP_INST_DEST_COL1 : m_nOutputMask |= (1 << 1); break;
				case NV40_VP_INST_DEST_BFC0 : m_nOutputMask |= (1 << 2); break;
				case NV40_VP_INST_DEST_BFC1 : m_nOutputMask |= (1 << 3); break;
				case NV40_VP_INST_DEST_FOGC : m_nOutputMask |= (1 << 4); break;
				case NV40_VP_INST_DEST_PSZ  : m_nOutputMask |= (1 << 5); break;
				default:
					if(dst->index>=NV40_VP_INST_DEST_TC(0) && dst->index<=NV40_VP_INST_DEST_TC(7)) m_nOutputMask |= (1<<(dst->index - NV40_VP_INST_DEST_TC0 + 14));
					break;
			}
			hw[3] |= (dst->index << NV40_VP_INST_DEST_SHIFT);
			if (slot == 0) {
				hw[0] |= NV40_VP_INST_VEC_RESULT;
				hw[0] |= NV40_VP_INST_VEC_DEST_TEMP_MASK;
			} else {
				hw[3] |= NV40_VP_INST_SCA_RESULT;
				hw[3] |= NV40_VP_INST_SCA_DEST_TEMP_MASK;
			}
			break;
	}
}

void CCompiler::emit_src(u32 *hw, u8 pos, struct nvfx_src *src)
{
	u32 sr = 0;
	struct nvfx_relocation reloc;

	switch(src->reg.type) {
		case NVFXSR_TEMP:
			sr |= (NVFX_VP(SRC_REG_TYPE_TEMP) << NVFX_VP(SRC_REG_TYPE_SHIFT));
			sr |= (src->reg.index << NVFX_VP(SRC_TEMP_SRC_SHIFT));
			break;
		case NVFXSR_INPUT:
			sr |= (NVFX_VP(SRC_REG_TYPE_INPUT) <<
				   NVFX_VP(SRC_REG_TYPE_SHIFT));
			m_nInputMask |= (1 << src->reg.index);
			hw[1] |= (src->reg.index << NVFX_VP(INST_INPUT_SRC_SHIFT));
			break;
		case NVFXSR_CONST:
			sr |= (NVFX_VP(SRC_REG_TYPE_CONST) <<
				   NVFX_VP(SRC_REG_TYPE_SHIFT));
			reloc.location = m_nCurInstruction;
			reloc.target = src->reg.index;
			m_lConstRelocation.push_back(reloc);
			break;
		case NVFXSR_NONE:
			sr |= (NVFX_VP(SRC_REG_TYPE_INPUT) <<
				   NVFX_VP(SRC_REG_TYPE_SHIFT));
			break;
	}

	if (src->negate)
		sr |= NVFX_VP(SRC_NEGATE);

	if (src->abs)
		hw[0] |= (1 << (21 + pos));

	sr |= ((src->swz[0] << NVFX_VP(SRC_SWZ_X_SHIFT)) |
	       (src->swz[1] << NVFX_VP(SRC_SWZ_Y_SHIFT)) |
	       (src->swz[2] << NVFX_VP(SRC_SWZ_Z_SHIFT)) |
	       (src->swz[3] << NVFX_VP(SRC_SWZ_W_SHIFT)));

	if(src->indirect) {
		if(src->reg.type == NVFXSR_CONST)
			hw[3] |= NVFX_VP(INST_INDEX_CONST);
		else if(src->reg.type == NVFXSR_INPUT)
			hw[0] |= NVFX_VP(INST_INDEX_INPUT);
		if(src->indirect_reg)
			hw[0] |= NVFX_VP(INST_ADDR_REG_SELECT_1);
		hw[0] |= src->indirect_swz << NVFX_VP(INST_ADDR_SWZ_SHIFT);
	}

	switch (pos) {
		case 0:
			hw[1] |= (((sr & NVFX_VP(SRC0_HIGH_MASK)) >> NVFX_VP(SRC0_HIGH_SHIFT)) << NVFX_VP(INST_SRC0H_SHIFT));
			hw[2] |= ((sr & NVFX_VP(SRC0_LOW_MASK)) << NVFX_VP(INST_SRC0L_SHIFT));
			break;
		case 1:
			hw[2] |= (sr << NVFX_VP(INST_SRC1_SHIFT));
			break;
		case 2:
			hw[2] |= (((sr & NVFX_VP(SRC2_HIGH_MASK)) >> NVFX_VP(SRC2_HIGH_SHIFT)) << NVFX_VP(INST_SRC2H_SHIFT));
			hw[3] |= ((sr & NVFX_VP(SRC2_LOW_MASK)) << NVFX_VP(INST_SRC2L_SHIFT));
			break;
	}
}

struct nvfx_reg CCompiler::temp()
{
	s32 idx = ffs(~m_rTemps) - 1;

	if(idx<0) return nvfx_reg(NVFXSR_NONE,0);

	m_rTemps |= (1<<idx);
	m_rTempsDiscard |= (1<<idx);

	if((s32)m_nNumRegs<idx) m_nNumRegs = idx;

	return nvfx_reg(NVFXSR_TEMP,idx);
}

void CCompiler::release_temps()
{
	m_rTemps &= ~m_rTempsDiscard;
	m_rTempsDiscard = 0;
}

struct nvfx_reg CCompiler::constant(s32 pipe, f32 x, f32 y, f32 z, f32 w)
{
	int idx;
	struct vertex_program_data *vpd;

	if(pipe>=0) {
		for(idx=0;idx<m_nConsts;idx++) {
			if(m_pConstData[idx].index==pipe) return nvfx_reg(NVFXSR_CONST,idx);
		}
	}

	idx = m_nConsts++;
	m_pConstData = (struct vertex_program_data*)realloc(m_pConstData,sizeof(struct vertex_program_data)*m_nConsts);

	vpd = &m_pConstData[idx];
	vpd->index = pipe;
	vpd->value[0] = x;
	vpd->value[1] = y;
	vpd->value[2] = z;
	vpd->value[3] = w;
	return nvfx_reg(NVFXSR_CONST,idx);
}
