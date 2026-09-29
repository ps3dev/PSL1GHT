#include "types.h"

#include "parser.h"
#include "compilerfp.h"

#define arith(s,d,m,s0,s1,s2) \
	nvfx_insn((s),0,-1,-1,(d),(m),(s0),(s1),(s2))

#define arith_ctor(ins,d,s0,s1,s2) \
	nvfx_insn_ctor((ins),(d),(s0),(s1),(s2))

#define src_abs_flag(pos) \
	((pos) == 2 ? NVFX_FP_OP_SRC2_ABS : ((pos) == 1 ? NVFX_FP_OP_SRC1_ABS : NVFX_FP_OP_SRC0_ABS))

CCompilerFP::CCompilerFP()
{
	m_rTemps = 0;
	m_hTemps = 0;
	m_nNumRegs = 2;
	m_nSamplers = 0;
	m_nFPControl = 0;
	m_nTexcoords = 0;
	m_nTexcoord2D = 0;
	m_nTexcoord3D = 0;
	m_nInstructions = 0;
	m_rTempsDiscard = 0;
	m_hTempsDiscard = 0;
	m_nCurInstruction = 0;
	m_pInstructions = NULL;
}

CCompilerFP::~CCompilerFP()
{
}

void CCompilerFP::Prepare(CParser *pParser)
{
	int i,j,nCount = pParser->GetInstructionCount();
	struct nvfx_insn *insns = pParser->GetInstructions();

	memset(m_RRegs, 0, NUM_HW_REGS);
	memset(m_HRegs, 0, NUM_HW_REGS);
	m_lParameters = pParser->GetParameters();

	for(i=0;i<nCount;i++) {
		struct nvfx_insn *insn = &insns[i];

		for(j=0;j<3;j++) {
			struct nvfx_src *src = &insn->src[j];

			switch(src->reg.type) {
				case NVFXSR_INPUT:
					break;
				case NVFXSR_TEMP:
					reserveReg(src->reg);
					break;
			}
		}

		switch(insn->dst.type) {
			case NVFXSR_OUTPUT:
				reserveReg(insn->dst);
				break;
			case NVFXSR_TEMP:
				reserveReg(insn->dst);
				break;
		}
	}

	RemapHRegs(pParser);
}

void CCompilerFP::RemapHRegs(CParser *pParser)
{
	u32 nCount = pParser->GetInstructionCount();
	struct nvfx_insn *insns = pParser->GetInstructions();
	u32 usedHRegs[NUM_HW_REGS];
	s32 mappedHRegs[NUM_HW_REGS];

	memset(usedHRegs, 0, sizeof(u32)*NUM_HW_REGS);
	memset(mappedHRegs, -1, sizeof(s32)*NUM_HW_REGS);

	// set used for all R-regs
	for (u32 i=0;i < NUM_HW_REGS/2;i++) {
		if (m_RRegs[i]) {
			usedHRegs[(i*2) + 0] = 1;
			usedHRegs[(i*2) + 1] = 1;
		}
	}

	// set used for all H-regs
	for (u32 i=0;i < NUM_HW_REGS;i++) {
		if (m_HRegs[i] && !m_RRegs[i>>1]) {
			mappedHRegs[i] = i;
			usedHRegs[i] = 1;
		}
	}

	// create remapping map
	for (u32 i=0, j=0, k=0;i < NUM_HW_REGS;i++) {
		if (!usedHRegs[i] && mappedHRegs[i] == -1 && mappedHRegs[j] == -1)
			mappedHRegs[j++] = k++;
		else if (mappedHRegs[j] == -1)
			k++;
		if (mappedHRegs[j] != -1)
			j++;
	}

	// now remap H-regs
	for(u32 i=0;i<nCount;i++) {
		struct nvfx_insn *insn = &insns[i];

		for(u32 j=0;j<3;j++) {
			struct nvfx_src *src = &insn->src[j];

			switch(src->reg.type) {
				case NVFXSR_INPUT:
					break;
				case NVFXSR_TEMP:
					if (src->reg.is_fp16) {
						src->reg.index = mappedHRegs[src->reg.index];
						reserveReg(src->reg);
					}
					break;
			}
		}

		switch(insn->dst.type) {
			case NVFXSR_TEMP:
				if (insn->dst.is_fp16) {
					insn->dst.index = mappedHRegs[insn->dst.index];
					reserveReg(insn->dst);
				}
				break;
		}
	}
}

void CCompilerFP::Compile(CParser *pParser)
{
	int i,nCount = pParser->GetInstructionCount();
	struct nvfx_insn *insns = pParser->GetInstructions();

	Prepare(pParser);

	for(i=0;i<nCount;i++) {
		struct nvfx_insn *insn = &insns[i];

		switch(insn->op) {
			case OPCODE_ADD:
				emit_insn(insn,NVFX_FP_OP_OPCODE_ADD);
				break;
			case OPCODE_BRK:
				emit_brk(insn);
				break;
			case OPCODE_COS:
				emit_insn(insn,NVFX_FP_OP_OPCODE_COS);
				break;
			case OPCODE_DDX:
				emit_ddx(insn);
				break;
			case OPCODE_DDY:
				emit_ddy(insn);
				break;
			case OPCODE_DP2:
				emit_insn(insn,NVFX_FP_OP_OPCODE_DP2);
				break;
			case OPCODE_DP3:
				emit_insn(insn,NVFX_FP_OP_OPCODE_DP3);
				break;
			case OPCODE_DP4:
				emit_insn(insn,NVFX_FP_OP_OPCODE_DP4);
				break;
			case OPCODE_DST:
				emit_insn(insn,NVFX_FP_OP_OPCODE_DST);
				break;
			case OPCODE_EX2:
				emit_insn(insn,NVFX_FP_OP_OPCODE_EX2);
				break;
			case OPCODE_FLR:
				emit_insn(insn,NVFX_FP_OP_OPCODE_FLR);
				break;
			case OPCODE_FRC:
				emit_insn(insn,NVFX_FP_OP_OPCODE_FRC);
				break;
			case OPCODE_KIL:
				emit_insn(insn,NVFX_FP_OP_OPCODE_KIL);
				break;
			case OPCODE_LG2:
				emit_insn(insn,NVFX_FP_OP_OPCODE_LG2);
				break;
			case OPCODE_LIT:
				emit_lit(insn);
				break;
			case OPCODE_LRP:
				emit_lrp(insn);
				break;
			case OPCODE_MAX:
				emit_insn(insn,NVFX_FP_OP_OPCODE_MAX);
				break;
			case OPCODE_MIN:
				emit_insn(insn,NVFX_FP_OP_OPCODE_MIN);
				break;
			case OPCODE_MAD:
				emit_insn(insn,NVFX_FP_OP_OPCODE_MAD);
				break;
			case OPCODE_MOV:
				emit_insn(insn,NVFX_FP_OP_OPCODE_MOV);
				break;
			case OPCODE_MUL:
				emit_insn(insn,NVFX_FP_OP_OPCODE_MUL);
				break;
			case OPCODE_NRM3:
				emit_insn(insn,NVFX_FP_OP_OPCODE_NRM);
				break;
			case OPCODE_PK4B:
				emit_insn(insn,NVFX_FP_OP_OPCODE_PK4B);
				break;
			case OPCODE_PK4UB:
				emit_insn(insn,NVFX_FP_OP_OPCODE_PK4UB);
				break;
			case OPCODE_PK2H:
				emit_insn(insn,NVFX_FP_OP_OPCODE_PK2H);
				break;
			case OPCODE_PK2US:
				emit_insn(insn,NVFX_FP_OP_OPCODE_PK2US);
				break;
			case OPCODE_POW:
				emit_pow(insn);
				break;
			case OPCODE_RCP:
				emit_insn(insn,NVFX_FP_OP_OPCODE_RCP);
				break;
			case OPCODE_RSQ:
				emit_insn(insn,NVFX_FP_OP_OPCODE_RSQ);
				break;
			case OPCODE_SEQ:
				emit_insn(insn,NVFX_FP_OP_OPCODE_SEQ);
				break;
			case OPCODE_SFL:
				emit_insn(insn,NVFX_FP_OP_OPCODE_SFL);
				break;
			case OPCODE_SGE:
				emit_insn(insn,NVFX_FP_OP_OPCODE_SGE);
				break;
			case OPCODE_SGT:
				emit_insn(insn,NVFX_FP_OP_OPCODE_SGT);
				break;
			case OPCODE_SIN:
				emit_insn(insn,NVFX_FP_OP_OPCODE_SIN);
				break;
			case OPCODE_SLE:
				emit_insn(insn,NVFX_FP_OP_OPCODE_SLE);
				break;
			case OPCODE_SLT:
				emit_insn(insn,NVFX_FP_OP_OPCODE_SLT);
				break;
			case OPCODE_SNE:
				emit_insn(insn,NVFX_FP_OP_OPCODE_SNE);
				break;
			case OPCODE_TEX:
				emit_tex(insn,NVFX_FP_OP_OPCODE_TEX);
				break;
			case OPCODE_TXB:
				emit_tex(insn,NVFX_FP_OP_OPCODE_TXB);
				break;
			case OPCODE_TXL:
				emit_txl(insn);
				break;
			case OPCODE_TXP:
				emit_tex(insn,NVFX_FP_OP_OPCODE_TXP);
				break;
			case OPCODE_UP4B:
				emit_insn(insn,NVFX_FP_OP_OPCODE_UP4B);
				break;
			case OPCODE_UP4UB:
				emit_insn(insn,NVFX_FP_OP_OPCODE_UP4UB);
				break;
			case OPCODE_UP2H:
				emit_insn(insn,NVFX_FP_OP_OPCODE_UP2H);
				break;
			case OPCODE_UP2US:
				emit_insn(insn,NVFX_FP_OP_OPCODE_UP2US);
				break;
			case OPCODE_BGNLOOP:
				emit_loop(insn);
				break;
			case OPCODE_ENDLOOP:
				fixup_loop();
				break;
			case OPCODE_BGNREP:
				emit_rep(insn);
				break;
			case OPCODE_ENDREP:
				fixup_rep();
				break;
			case OPCODE_IF:
				emit_if(insn);
				break;
			case OPCODE_ENDIF:
				fixup_if();
				break;
			case OPCODE_ELSE:
				fixup_else();
				break;
			case OPCODE_END:
				if(m_nInstructions) m_pInstructions[m_nCurInstruction].data[0] |= NVFX_FP_OP_PROGRAM_END;
				else {
					m_nCurInstruction = grow_insns(1);
					m_pInstructions[m_nCurInstruction].data[0] = 0x00000001;
					m_pInstructions[m_nCurInstruction].data[1] = 0x00000000;
					m_pInstructions[m_nCurInstruction].data[2] = 0x00000000;
					m_pInstructions[m_nCurInstruction].data[3] = 0x00000000;
				}
		}
		release_temps();
	}
}

void CCompilerFP::emit_insn(struct nvfx_insn *insn,u8 op)
{
	u32 *hw;
	bool have_const = false;

	m_nCurInstruction = grow_insns(1);
	memset(&m_pInstructions[m_nCurInstruction],0,sizeof(struct fragment_program_exec));

	hw = m_pInstructions[m_nCurInstruction].data;

	if(op==NVFX_FP_OP_OPCODE_KIL)
		m_nFPControl |= NV30_3D_FP_CONTROL_USES_KIL;

	hw[0] |= (op << NVFX_FP_OP_OPCODE_SHIFT);
	hw[0] |= (insn->mask << NVFX_FP_OP_OUTMASK_SHIFT);
	hw[0] |= (insn->precision << NVFX_FP_OP_PRECISION_SHIFT);
	hw[2] |= (insn->scale << NVFX_FP_OP_DST_SCALE_SHIFT);

	if(insn->sat)
		hw[0] |= NVFX_FP_OP_OUT_SAT;

	if(insn->cc_update)
		hw[0] |= NVFX_FP_OP_COND_WRITE_ENABLE;

	hw[1] |= (insn->cc_cond << NVFX_FP_OP_COND_SHIFT);
	hw[1] |= ((insn->cc_swz[0] << NVFX_FP_OP_COND_SWZ_X_SHIFT) |
			  (insn->cc_swz[1] << NVFX_FP_OP_COND_SWZ_Y_SHIFT) |
		      (insn->cc_swz[2] << NVFX_FP_OP_COND_SWZ_Z_SHIFT) |
		      (insn->cc_swz[3] << NVFX_FP_OP_COND_SWZ_W_SHIFT));

	emit_dst(insn,&have_const);
	emit_src(insn,0,&have_const);
	emit_src(insn,1,&have_const);
	emit_src(insn,2,&have_const);
}

void CCompilerFP::emit_dst(struct nvfx_insn *insn,bool *have_const)
{
	struct nvfx_reg *dst = &insn->dst;
	u32 *hw = m_pInstructions[m_nCurInstruction].data;

	s32 index = dst->index;
	switch(dst->type) {
		case NVFXSR_TEMP:
		{
			u32 hwReg = dst->is_fp16 ? (index >> 1) : index;
			if(m_nNumRegs<(s32)(hwReg + 1))
				m_nNumRegs = (hwReg + 1);
		}
		break;

		case NVFXSR_OUTPUT:
			if(dst->index==0 && !dst->is_fp16)
				m_nFPControl |= 0x40;
			else if(dst->index==1)
				m_nFPControl |= 0xe;
			break;
		case NVFXSR_NONE:
			hw[0] |= NV40_FP_OP_OUT_NONE;
			break;
	}
	if(dst->is_fp16)
		hw[0] |= NVFX_FP_OP_OUT_REG_HALF;

	hw[0] |= (index << NVFX_FP_OP_OUT_REG_SHIFT);
}

void CCompilerFP::emit_src(struct nvfx_insn *insn,s32 pos,bool *have_const)
{
	u32 sr = 0;
	struct nvfx_src *src = &insn->src[pos];
	u32 *hw = m_pInstructions[m_nCurInstruction].data;

	switch(src->reg.type) {
		case NVFXSR_INPUT:
			sr |= (NVFX_FP_REG_TYPE_INPUT << NVFX_FP_REG_TYPE_SHIFT);
			hw[0] |= (src->reg.index << NVFX_FP_OP_INPUT_SRC_SHIFT);

			if(src->reg.index>=NVFX_FP_OP_INPUT_SRC_TC(0) && src->reg.index<=NVFX_FP_OP_INPUT_SRC_TC(7)) {
				param fpi = GetInputAttrib(src->reg.index);

				hw[3] |= ((insn->disable_pc << NV40_FP_OP_DISABLE_PC_SHIFT) | (0x7fc << NV40_FP_OP_ADDR_INDEX_SHIFT));

				if((int)fpi.index!=-1) {
					if(fpi.type>PARAM_FLOAT2)
						m_nTexcoord3D |= (1 << (src->reg.index - NVFX_FP_OP_INPUT_SRC_TC0));
					else
						m_nTexcoord2D |= (1 << (src->reg.index - NVFX_FP_OP_INPUT_SRC_TC0));
				}
				m_nTexcoords |= (1 << (src->reg.index - NVFX_FP_OP_INPUT_SRC_TC0));
			}
			break;
		case NVFXSR_TEMP:
			sr |= (NVFX_FP_REG_TYPE_TEMP << NVFX_FP_REG_TYPE_SHIFT);
			sr |= (src->reg.index << NVFX_FP_REG_SRC_SHIFT);
			break;
		case NVFXSR_IMM:
			if(!*have_const) {
				grow_insns(1);
				memset(&m_pInstructions[m_nCurInstruction + 1], 0,4*sizeof(f32));
				hw = m_pInstructions[m_nCurInstruction].data;
				*have_const = true;
			}
			{
				param fpd = GetImmData(src->reg.index);
				if(fpd.values!=NULL) memcpy(&m_pInstructions[m_nCurInstruction + 1],fpd.values,4*sizeof(f32));
				sr |= (NVFX_FP_REG_TYPE_CONST << NVFX_FP_REG_TYPE_SHIFT);
			}
			break;
		case NVFXSR_CONST:
			if(!*have_const) {
				grow_insns(1);
				hw = m_pInstructions[m_nCurInstruction].data;
				*have_const = true;
			}
			{
				struct fragment_program_data fpd;

				fpd.offset = m_nCurInstruction + 1;
				fpd.index = src->reg.index;
				fpd.user = -1;
				memset(&m_pInstructions[fpd.offset],0,sizeof(struct fragment_program_exec));
				m_lConstData.push_back(fpd);
			}
			sr |= (NVFX_FP_REG_TYPE_CONST << NVFX_FP_REG_TYPE_SHIFT);
			break;
		case NVFXSR_NONE:
			sr |= (NVFX_FP_REG_TYPE_TEMP << NVFX_FP_REG_TYPE_SHIFT);
			break;
		case NVFXSR_OUTPUT:
			fprintf(stderr,"Output register used as input.\n");
			exit(EXIT_FAILURE);
			return;
	}

	if(src->reg.is_fp16)
		sr |= NVFX_FP_REG_SRC_HALF;

	if(src->negate)
		sr |= NVFX_FP_REG_NEGATE;

	if(src->abs)
		sr |= src_abs_flag(pos);

	sr |= ((src->swz[0] << NVFX_FP_REG_SWZ_X_SHIFT) |
	       (src->swz[1] << NVFX_FP_REG_SWZ_Y_SHIFT) |
	       (src->swz[2] << NVFX_FP_REG_SWZ_Z_SHIFT) |
	       (src->swz[3] << NVFX_FP_REG_SWZ_W_SHIFT));

	hw[pos + 1] |= sr;
}

void CCompilerFP::emit_rep(struct nvfx_insn *insn)
{
	u32 *hw;
	int count;

	m_nCurInstruction = grow_insns(1);
	memset(&m_pInstructions[m_nCurInstruction],0,sizeof(struct fragment_program_exec));

	hw = m_pInstructions[m_nCurInstruction].data;

	param fpd = GetImmData(insn->src[0].reg.index);

	if (insn->src[0].reg.type != NVFXSR_IMM ||
		(*fpd.values)[0] < 0.0 || (*fpd.values)[0] > 255.0) {
		fprintf(stderr,"Input to REP must be immediate number 0-255\n");
		exit(EXIT_FAILURE);
	}

	count = (int)(*fpd.values)[0];
	hw[0] |= (NV40_FP_OP_BRA_OPCODE_REP << NVFX_FP_OP_OPCODE_SHIFT);
	hw[0] |= NV40_FP_OP_OUT_NONE;
	hw[0] |= NVFX_FP_PRECISION_FP16 <<  NVFX_FP_OP_PRECISION_SHIFT;
	hw[2] |= NV40_FP_OP_OPCODE_IS_BRANCH;
	hw[2] |= (count<<NV40_FP_OP_REP_COUNT1_SHIFT) |
		     (count<<NV40_FP_OP_REP_COUNT2_SHIFT) |
			 (count<<NV40_FP_OP_REP_COUNT3_SHIFT);
	hw[1] |= (insn->cc_cond << NVFX_FP_OP_COND_SHIFT);
	hw[1] |= ((insn->cc_swz[0] << NVFX_FP_OP_COND_SWZ_X_SHIFT) |
		      (insn->cc_swz[1] << NVFX_FP_OP_COND_SWZ_Y_SHIFT) |
			  (insn->cc_swz[2] << NVFX_FP_OP_COND_SWZ_Z_SHIFT) |
			  (insn->cc_swz[3] << NVFX_FP_OP_COND_SWZ_W_SHIFT));

	m_repStack.push(m_nCurInstruction);
}

void CCompilerFP::fixup_rep()
{
	u32 *hw;

	hw = m_pInstructions[m_repStack.top()].data;

	hw[3] |= m_nInstructions * 4;

	m_repStack.pop();
}

void CCompilerFP::fixup_loop()
{
	u32 *hw;

	hw = m_pInstructions[m_loopStack.top()].data;

	hw[3] |= m_nInstructions * 4;

	m_loopStack.pop();
}

void CCompilerFP::fixup_if()
{
	u32 *hw;

	hw = m_pInstructions[m_ifStack.top()].data;

	if(!hw[2]) hw[2] = (NV40_FP_OP_OPCODE_IS_BRANCH | (m_nInstructions*4));
	hw[3] = (m_nInstructions*4);

	m_ifStack.pop();
}

void CCompilerFP::fixup_else()
{
	u32 *hw;

	hw = m_pInstructions[m_ifStack.top()].data;
	hw[2] = (NV40_FP_OP_OPCODE_IS_BRANCH | (m_nInstructions*4));
}

void CCompilerFP::emit_brk(struct nvfx_insn *insn)
{
	u32 *hw;

	m_nCurInstruction = grow_insns(1);
	memset(&m_pInstructions[m_nCurInstruction],0,sizeof(struct fragment_program_exec));

	hw = m_pInstructions[m_nCurInstruction].data;

	hw[0] |= (NV40_FP_OP_BRA_OPCODE_BRK << NVFX_FP_OP_OPCODE_SHIFT);
	hw[0] |= NV40_FP_OP_OUT_NONE;
	hw[2] |= NV40_FP_OP_OPCODE_IS_BRANCH;

	hw[1] |= (insn->cc_cond << NVFX_FP_OP_COND_SHIFT);
	hw[1] |= ((insn->cc_swz[0] << NVFX_FP_OP_COND_SWZ_X_SHIFT) |
		      (insn->cc_swz[1] << NVFX_FP_OP_COND_SWZ_Y_SHIFT) |
			  (insn->cc_swz[2] << NVFX_FP_OP_COND_SWZ_Z_SHIFT) |
			  (insn->cc_swz[3] << NVFX_FP_OP_COND_SWZ_W_SHIFT));
}

void CCompilerFP::emit_if(struct nvfx_insn *insn)
{
	u32 *hw;

	m_nCurInstruction = grow_insns(1);
	memset(&m_pInstructions[m_nCurInstruction],0,sizeof(struct fragment_program_exec));

	hw = m_pInstructions[m_nCurInstruction].data;

	hw[0] |= (NV40_FP_OP_BRA_OPCODE_IF << NVFX_FP_OP_OPCODE_SHIFT);
	hw[0] |= NV40_FP_OP_OUT_NONE;
	hw[0] |= (NVFX_FP_PRECISION_FP16 <<  NVFX_FP_OP_PRECISION_SHIFT);

	hw[1] |= (insn->cc_cond << NVFX_FP_OP_COND_SHIFT);
	hw[1] |= ((insn->cc_swz[0] << NVFX_FP_OP_COND_SWZ_X_SHIFT) |
		      (insn->cc_swz[1] << NVFX_FP_OP_COND_SWZ_Y_SHIFT) |
			  (insn->cc_swz[2] << NVFX_FP_OP_COND_SWZ_Z_SHIFT) |
			  (insn->cc_swz[3] << NVFX_FP_OP_COND_SWZ_W_SHIFT));

	m_ifStack.push(m_nCurInstruction);
}

void CCompilerFP::emit_loop(struct nvfx_insn *insn)
{
	u32 *hw;
	int count1,count2,count3;

	m_nCurInstruction = grow_insns(1);
	memset(&m_pInstructions[m_nCurInstruction],0,sizeof(struct fragment_program_exec));

	hw = m_pInstructions[m_nCurInstruction].data;

	param fpd = GetImmData(insn->src[0].reg.index);

	if (insn->src[0].reg.type != NVFXSR_IMM ||
	    (*fpd.values)[0] < 0.0 || (*fpd.values)[0] > 255.0) {
		fprintf(stderr,"Input to LOOP must be immediate number 0-255\n");
		exit(EXIT_FAILURE);
	}

	count1 = (int)(*fpd.values)[0];
	count2 = (int)(*fpd.values)[1];
	count3 = (int)(*fpd.values)[2];
	hw[0] |= (NV40_FP_OP_BRA_OPCODE_LOOP << NVFX_FP_OP_OPCODE_SHIFT);
	hw[0] |= NV40_FP_OP_OUT_NONE;
	hw[0] |= NVFX_FP_PRECISION_FP16 <<  NVFX_FP_OP_PRECISION_SHIFT;
	hw[2] |= NV40_FP_OP_OPCODE_IS_BRANCH;
	hw[2] |= (count1<<NV40_FP_OP_REP_COUNT1_SHIFT) |
		     (count2<<NV40_FP_OP_REP_COUNT2_SHIFT) |
			 (count3<<NV40_FP_OP_REP_COUNT3_SHIFT);
	hw[1] |= (insn->cc_cond << NVFX_FP_OP_COND_SHIFT);
	hw[1] |= ((insn->cc_swz[0] << NVFX_FP_OP_COND_SWZ_X_SHIFT) |
		      (insn->cc_swz[1] << NVFX_FP_OP_COND_SWZ_Y_SHIFT) |
			  (insn->cc_swz[2] << NVFX_FP_OP_COND_SWZ_Z_SHIFT) |
			  (insn->cc_swz[3] << NVFX_FP_OP_COND_SWZ_W_SHIFT));

	m_loopStack.push(m_nCurInstruction);
}

void CCompilerFP::emit_lrp(struct nvfx_insn *insn)
{
	struct nvfx_insn tmp_insn;
	struct nvfx_src tmp = nvfx_src(temp(insn));

	tmp_insn = arith(0,tmp.reg,insn->mask,neg(insn->src[0]),insn->src[2],insn->src[2]);
	emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_MAD);

	tmp_insn = arith(insn->sat,insn->dst,insn->mask,insn->src[0],insn->src[1],tmp);
	emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_MAD);
}

void CCompilerFP::emit_pow(struct nvfx_insn *insn)
{
	struct nvfx_insn tmp_insn;
	struct nvfx_src tmp = nvfx_src(temp(insn));
	struct nvfx_src none = nvfx_src(nvfx_reg(NVFXSR_NONE,0));

	tmp_insn = arith(0, tmp.reg, NVFX_FP_MASK_X, insn->src[0], none, none);
	emit_insn(&tmp_insn, NVFX_FP_OP_OPCODE_LG2);

	tmp_insn = arith(0, tmp.reg, NVFX_FP_MASK_X, swz(tmp, X, X, X, X), insn->src[1], none);
	emit_insn(&tmp_insn, NVFX_FP_OP_OPCODE_MUL);

	tmp_insn = arith_ctor(insn, insn->dst, swz(tmp, X, X, X, X), none, none);
	emit_insn(&tmp_insn, NVFX_FP_OP_OPCODE_EX2);
}

void CCompilerFP::emit_lit(struct nvfx_insn *insn)
{
	struct nvfx_insn tmp_insn;
	struct nvfx_src tmp = nvfx_src(temp(insn));
	struct nvfx_src none = nvfx_src(nvfx_reg(NVFXSR_NONE,0));
	struct nvfx_src maxs = nvfx_src(imm(0.0f, FLT_MIN, 0.0f, 0.0f));

	tmp_insn = arith(0, tmp.reg, (NVFX_FP_MASK_Y | NVFX_FP_MASK_W), swz(insn->src[0], X, X, X, Y), swz(maxs, X, X, Y, Y), none);
	emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_MAX);

	tmp_insn = arith(0, tmp.reg, NVFX_FP_MASK_W, swz(tmp, W, W, W, W), none, none);
	emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_LG2);

	tmp_insn = arith(0, tmp.reg, NVFX_FP_MASK_W, swz(tmp, W, W, W, W), swz(insn->src[0], W, W, W, W), none);
	emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_MUL);

	tmp_insn = arith_ctor(insn, insn->dst, swz(tmp, Y, Y, W, W), none, none);
	emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_LITEX2_NV40);
}

void CCompilerFP::emit_ddx(struct nvfx_insn *insn)
{
	if(insn->mask&(NVFX_FP_MASK_Z | NVFX_FP_MASK_W)) {
		struct nvfx_insn tmp_insn;
		struct nvfx_src tmp = nvfx_src(temp(insn));
		struct nvfx_src none = nvfx_src(nvfx_reg(NVFXSR_NONE,0));

		tmp_insn = arith(insn->sat, tmp.reg, (NVFX_FP_MASK_X | NVFX_FP_MASK_Y), swz(insn->src[0], Z, W, Z, W), none, none);
		emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_DDX);

		tmp_insn = arith(0, tmp.reg, (NVFX_FP_MASK_Z | NVFX_FP_MASK_W), swz(tmp, X, Y, X, Y), none, none);
		emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_MOV);

		tmp_insn = arith(insn->sat, tmp.reg, (NVFX_FP_MASK_X | NVFX_FP_MASK_Y), insn->src[0], none, none);
		emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_DDX);

		tmp_insn = arith(0, insn->dst, insn->mask, tmp, none, none);
		emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_MOV);
	} else
		emit_insn(insn,NVFX_FP_OP_OPCODE_DDX);
}

void CCompilerFP::emit_ddy(struct nvfx_insn *insn)
{
	if(insn->mask&(NVFX_FP_MASK_Z | NVFX_FP_MASK_W)) {
		struct nvfx_insn tmp_insn;
		struct nvfx_src tmp = nvfx_src(temp(insn));
		struct nvfx_src none = nvfx_src(nvfx_reg(NVFXSR_NONE,0));

		tmp_insn = arith(insn->sat, tmp.reg, (NVFX_FP_MASK_X | NVFX_FP_MASK_Y), swz(insn->src[0], Z, W, Z, W), none, none);
		emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_DDY);

		tmp_insn = arith(0, tmp.reg, (NVFX_FP_MASK_Z | NVFX_FP_MASK_W), swz(tmp, X, Y, X, Y), none, none);
		emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_MOV);

		tmp_insn = arith(insn->sat, tmp.reg, (NVFX_FP_MASK_X | NVFX_FP_MASK_Y), insn->src[0], none, none);
		emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_DDY);

		tmp_insn = arith(0, insn->dst, insn->mask, tmp, none, none);
		emit_insn(&tmp_insn,NVFX_FP_OP_OPCODE_MOV);
	} else
		emit_insn(insn,NVFX_FP_OP_OPCODE_DDY);
}

void CCompilerFP::emit_txl(struct nvfx_insn *insn)
{
	u32 *hw;
	struct nvfx_insn tmp_insn;
	struct nvfx_src none = nvfx_src(nvfx_reg(NVFXSR_NONE,0));

	if(insn->tex_unit<0 || insn->tex_unit>15) return;

	tmp_insn = arith_ctor(insn, insn->dst, insn->src[0], swz(insn->src[0], W, W, W, W), none);
	emit_insn(&tmp_insn, NVFX_FP_OP_OPCODE_TXL_NV40);

	hw = m_pInstructions[m_nCurInstruction].data;
	hw[0] |= (insn->tex_unit << NVFX_FP_OP_TEX_UNIT_SHIFT);

	m_nSamplers |= (1<<insn->tex_unit);
	m_nFPControl |= 0x8000;
}

void CCompilerFP::emit_tex(struct nvfx_insn *insn,u8 op)
{
	u32 *hw;

	if(insn->tex_unit<0 || insn->tex_unit>15) return;

	emit_insn(insn,op);

	hw = m_pInstructions[m_nCurInstruction].data;
	hw[0] |= (insn->tex_unit << NVFX_FP_OP_TEX_UNIT_SHIFT);

	m_nSamplers |= (1<<insn->tex_unit);
	m_nFPControl |= 0x8000;
}

struct nvfx_reg CCompilerFP::imm(f32 x, f32 y, f32 z, f32 w)
{
	param p;
	f32 v[4] = { x, y, z, w };
	s32 idx = m_lParameters.size();

	p.count = 1;
	p.is_const = 1;
	p.is_internal = 1;
	p.type = PARAM_FLOAT4;

	CParser::InitParameter(&p);
	memcpy(p.values, v, sizeof(f32)*4);

	m_lParameters.push_back(p);

	return nvfx_reg(NVFXSR_IMM, idx);
}

void CCompilerFP::reserveReg(const struct nvfx_reg& reg)
{
	u32 index = reg.is_fp16 ? (reg.index >> 1) : reg.index;
	if (reg.is_fp16) {
		m_HRegs[reg.index] = 1;
		m_hTemps |= (1 << reg.index);
	} else {
		m_RRegs[reg.index] = 1;
		m_hTemps |= (3 << (reg.index<<1));
	}
	m_rTemps |= (1 << index);
}

struct nvfx_reg CCompilerFP::temp(struct nvfx_insn *insn)
{
	s32 reg = 0, idx = -1;
	bool useFp16 = canUseTempFp16(insn);

	if (useFp16) {
		reg = idx = __builtin_ctzll(~m_hTemps);
		if (idx < 0)
			throw std::runtime_error("Error: No temprary register left to allocate.");

		reg = idx;
		m_hTemps |= (1 << idx);
		m_hTempsDiscard |= (1 << idx);
		idx >>= 1;
	} else {
		idx = __builtin_ctzll(~m_rTemps);
		if (idx < 0)
			throw std::runtime_error("Error: No temprary register left to allocate.");

		reg = idx;
		m_hTemps |= (3 << (idx << 1));
		m_hTempsDiscard |= (3 << (idx << 1));
	}

	m_rTemps |= (1<<idx);
	m_rTempsDiscard |= (1<<idx);

	return nvfx_reg(NVFXSR_TEMP, reg);
}

void CCompilerFP::release_temps()
{
	m_rTemps &= ~m_rTempsDiscard;
	m_rTempsDiscard = 0;

	m_hTemps &= ~m_hTempsDiscard;
	m_hTempsDiscard = 0;
}

bool CCompilerFP::canUseTempFp16(struct nvfx_insn *insn)
{	
	u8 useFp16 = 1;

	useFp16 &= (insn->dst.is_fp16 && insn->dst.type == NVFXSR_TEMP);
	for (u32 i=0; i < 3;i++) {
		const struct nvfx_reg& reg = insn->src[i].reg;
		if (reg.type == NVFXSR_TEMP)
			useFp16 &= reg.is_fp16;
	}

	return (bool)useFp16;
}

int CCompilerFP::grow_insns(int count)
{
	int pos = m_nInstructions;

	m_nInstructions += count;
	m_pInstructions = (struct fragment_program_exec*)realloc(m_pInstructions,m_nInstructions*sizeof(struct fragment_program_exec));

	return pos;
}
