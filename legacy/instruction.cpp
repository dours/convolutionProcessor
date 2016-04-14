
#include "instruction.h"

#define SHORT_ENOUGH 8 // for example

sc_uint<DATA_WIDTH> scalarOp(tOperationType op, sc_uint<DATA_WIDTH> a, sc_uint<DATA_WIDTH> b) { 
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
              printf("%i, ", b.to_uint());
              return b;
    default:
              return 0xbeef;
  }
}

void Instruction::execute(ProcessorState& st) { 

  vector<sc_uint<DATA_WIDTH> > l, r;
  if (WINDOW == lopndk) l = st.getWindow(lopnd); else l = st.getLocal(lopnd);
  if (COMMON == ropndk) r = st.getCommon(ropnd); else r = st.getLocal(ropnd);  

  if (REDUCE == opKind) { 
    int reductionStage = (1 << lopnd); // lopnd is not a number of any register in this case 
    for (int i = 0; i < VECTOR_ALU_WIDTH / reductionStage; ++i) l.at(i) = r.at(i + VECTOR_ALU_WIDTH / reductionStage);
    for (int i = VECTOR_ALU_WIDTH / reductionStage; i < VECTOR_ALU_WIDTH; ++i) l.at(i) = r.at(i) = 0;
  }

  vector<sc_uint<DATA_WIDTH> > result;
  for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) result.push_back(0xbeef);
 
  if (SHIFT_IN == opType.at(VECTOR_ALU_WIDTH - 1)) { // this rather special instruction is to load constant vectors in fact
    for (int i = VECTOR_ALU_WIDTH - 1; i > 0; --i) result.at(i) = r.at(i - 1); 
    result.at(0) = theConstant;
  } else { 
    for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) result.at(i) = scalarOp(opType.at(i), l.at(i), r.at(i));
  }

  if (PRINT == opType.at(0)) printf("\n"); 

  if (COMMON == dstk) st.setCommon(dst, result); else st.setLocal(dst, result);

  if (finishedThisWindow) st.moveWindow();

}

