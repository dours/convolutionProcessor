#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "common.h"
#include "processorState.h"
#include <systemc.h>
#include <assert.h>

#define INSTRUCTION_WIDTH (3 * (1 + LOG_N_REGS) + 1 + 3 * VECTOR_ALU_WIDTH + 1)
  // a left instruction operand may be a vector from a current window given by its X-coordinate in the window
  // or it may be a local register of the window given by its number
  // a right instruction operand may be a local register or a register, which is common for all the windows currently being processed
  enum tOperandType  { WINDOW, LOCAL, COMMON };

  enum tOperationType { SHORT_MUL, ADD, SUB, SHIFT_IN, CMP, EXTRACT, PRINT, XOR };
  // Vector kind means that operation is applied to both operands. 
  // Reduce means that op is applied to pairs of scalars taken from the right operand, while the left one is ignored. 
  enum tOperationKind { VECTOR, REDUCE };

 
/// the class describes an instruction in a programmer-readable form. It can also generate sc_uint bit vector (which is the actual instruction code)
class Instruction {

  tOperandType lopndk, ropndk, dstk;
  sc_uint<LOG_N_REGS> lopnd, ropnd, dst;

  tOperationKind opKind;
  vector<tOperationType> opType;

  sc_uint<DATA_WIDTH> theConstant; 
  bool finishedThisWindow; 

public:
  // this one creates an instruction in which all the operations are the same
  Instruction(
    tOperandType lk, sc_uint<LOG_N_REGS> l, 
    tOperandType rk, sc_uint<LOG_N_REGS> r,
    tOperandType dstk, sc_uint<LOG_N_REGS> dst,
    tOperationKind   opk, tOperationType op,
    sc_uint<DATA_WIDTH> c, bool f
  ) {
    assert(VECTOR_ALU_WIDTH <= N_REGS); // just a simplification, maybe the code may be more generic, maybe not
    lopndk = lk;
    lopnd = l;
    ropndk = rk;
    ropnd = r;
    this -> dstk = dstk;
    this -> dst = dst;
    opKind = opk;
    for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) { opType.push_back(op); } 
    if (SHIFT_IN == op) theConstant = c; else theConstant = 0xbeef;
    finishedThisWindow = f;
  }

  void execute(ProcessorState& state);
  void decode(sc_uint<INSTRUCTION_WIDTH>); // assign the instruction from a coded bit vector
  sc_uint<INSTRUCTION_WIDTH> encode();

};



#endif

