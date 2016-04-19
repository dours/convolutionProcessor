#include "LordOfTheHeaders.h"

#define SHORT_ENOUGH 8 // for example

sc_uint<DATA_WIDTH> Instruction::scalarOp(tOperationType op, sc_uint<DATA_WIDTH> a, sc_uint<DATA_WIDTH> b) {
  switch (op) { 
    case ADD: return a + b;
    case SUB: return a - b;
    case XOR:  return a ^ b;
    case SHORT_MUL: 
	      if (b >= SHORT_ENOUGH) fprintf(stderr, "b = %i\n", b.to_uint());
              assert(b < SHORT_ENOUGH); 
              return a * b;
    case CMP: 
              if (a < b) return (-1);
              if (a > b) return 1;
              return 0;
    case PRINT:
              fprintf(stderr, "%i ", b.to_uint());
              return b;
    default:
              return 0xbeef;
  }
}

void Instruction::execute(ProcessorState *st, Unit &u) {

  sc_uint<DATA_WIDTH> *l, *r;
  if (WINDOW == lopndk) l = u.getWindow(lopnd); else l = u.getLocal(lopnd);
  if (COMMON == ropndk) r = st->common_reg[ropnd]; else r = u.getLocal(ropnd);

  if (REDUCE == opKind) { 
    int reductionStage = (1 << lopnd); // lopnd is not a number of any register in this case 
    for (int i = 0; i < VECTOR_ALU_WIDTH / reductionStage; ++i) l[i] = r[i + VECTOR_ALU_WIDTH / reductionStage];
    for (int i = VECTOR_ALU_WIDTH / reductionStage; i < VECTOR_ALU_WIDTH; ++i) l[i] = r[i] = 0;
  }

	sc_uint<DATA_WIDTH> result[VECTOR_ALU_WIDTH];
	for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) result[i] = 0xbeef;

	if (SHIFT_IN == opType[VECTOR_ALU_WIDTH - 1]) { // this rather special instruction is to load constant vectors in fact
		for (int i = VECTOR_ALU_WIDTH - 1; i > 0; --i)
			result[i] = r[i - 1];
		result[0] = theConstant;
	} else { 
		for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) result[i] = scalarOp(opType[i], l[i], r[i]);
	}

	if (PRINT == opType[0]){ fprintf(stderr,"\n");}
	else
	{ 
	    fprintf(stderr, "from ");
	    for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) fprintf(stderr, "%i ", l[i].to_uint());
	    fprintf(stderr, "and ");
	    for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) fprintf(stderr, "%i ", r[i].to_uint());
	    fprintf(stderr, "result ");
	    for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) fprintf(stderr, "%i ", result[i].to_uint());
	    fprintf(stderr, "\n");

		u.setLocal(dst, result);
	}

	delete [] l;
	if (COMMON != ropndk) delete [] r;
}

  Instruction::Instruction(){}
  Instruction::Instruction(
    tOperandType lk, sc_uint<LOG_N_REGS> l,
    tOperandType rk, sc_uint<LOG_N_REGS> r,
    tOperandType dstk, sc_uint<LOG_N_REGS> dst,
    tOperationKind   opk, tOperationType op,
    sc_uint<DATA_WIDTH> c) {

    lopndk = lk;
    lopnd = l;
    ropndk = rk;
    ropnd = r;
    this -> dstk = dstk;
    this -> dst = dst;
    opKind = opk;
    for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) { opType[i] = op; }
    if (SHIFT_IN == op) theConstant = c; else theConstant = 0xbeef;
  }
