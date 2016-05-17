
#include "instruction.h"

// this code loads a following 3x3 kernel matrix into common regs of the processor
// 1 2 1
// 2 4 2
// 1 2 1
vector<Instruction> init, programm;

void initInitCommon() { 
  vector<Instruction> a;

  a.push_back(Instruction(LOCAL,  0, LOCAL,  0, LOCAL,  0, VECTOR, XOR,      0, false)); // local_reg[0] := 0

  a.push_back(Instruction(WINDOW, 0, LOCAL , 0, COMMON, 0, VECTOR, SHIFT_IN, 1, false));
  a.push_back(Instruction(WINDOW, 0, COMMON, 0, COMMON, 0, VECTOR, SHIFT_IN, 2, false));
  a.push_back(Instruction(WINDOW, 0, COMMON, 0, COMMON, 0, VECTOR, SHIFT_IN, 1, false));

  a.push_back(Instruction(WINDOW, 0, LOCAL , 0, COMMON, 1, VECTOR, SHIFT_IN, 2, false));
  a.push_back(Instruction(WINDOW, 0, COMMON, 1, COMMON, 1, VECTOR, SHIFT_IN, 4, false));
  a.push_back(Instruction(WINDOW, 0, COMMON, 1, COMMON, 1, VECTOR, SHIFT_IN, 2, false));

  a.push_back(Instruction(WINDOW, 0, LOCAL , 0, COMMON, 2, VECTOR, SHIFT_IN, 1, false));
  a.push_back(Instruction(WINDOW, 0, COMMON, 2, COMMON, 2, VECTOR, SHIFT_IN, 2, false));
  a.push_back(Instruction(WINDOW, 0, COMMON, 2, COMMON, 2, VECTOR, SHIFT_IN, 1, false));

  a.push_back(Instruction(LOCAL, 0, LOCAL, 0, LOCAL, 0, VECTOR, PRINT,    0, false));
  a.push_back(Instruction(COMMON, 0, COMMON, 0, COMMON, 0, VECTOR, PRINT,    0, false));
  a.push_back(Instruction(COMMON, 1, COMMON, 1, COMMON, 1, VECTOR, PRINT,    0, false));
  a.push_back(Instruction(COMMON, 2, COMMON, 2, COMMON, 2, VECTOR, PRINT,    0, false));

  init = a;
}



void initComputePseudoBlur() { 
  vector<Instruction> a;

  a.push_back(Instruction(LOCAL,  0, LOCAL, 0, LOCAL, 0, VECTOR, XOR, 0, false)); // local_reg[0] := 0  

  a.push_back(Instruction(WINDOW, 0, COMMON, 0, LOCAL, 1, VECTOR, SHORT_MUL, 0, false)); // local_reg[1] := window[0] * common_reg[0]
  a.push_back(Instruction(LOCAL,  0, LOCAL,  1, LOCAL, 0, VECTOR, ADD,       0, false)); // local_reg[0] += local_reg[1]
//  a.push_back(Instruction(LOCAL,  0, LOCAL,  0, LOCAL, 0, VECTOR, PRINT,     0, false)); // debug print result

  a.push_back(Instruction(WINDOW, 1, COMMON, 1, LOCAL, 1, VECTOR, SHORT_MUL, 0, false)); // local_reg[1] := window[0] * common_reg[0]
  a.push_back(Instruction(LOCAL,  0, LOCAL,  1, LOCAL, 0, VECTOR, ADD,       0, false)); // local_reg[0] += local_reg[1]
//  a.push_back(Instruction(LOCAL,  0, LOCAL,  0, LOCAL, 0, VECTOR, PRINT,     0, false)); // debug print result

  a.push_back(Instruction(WINDOW, 2, COMMON, 2, LOCAL, 1, VECTOR, SHORT_MUL, 0, false)); // local_reg[1] := window[0] * common_reg[0]
  a.push_back(Instruction(LOCAL,  0, LOCAL,  1, LOCAL, 0, VECTOR, ADD,       0, false)); // local_reg[0] += local_reg[1]

//  a.push_back(Instruction(LOCAL,  0, LOCAL,  0, LOCAL, 0, VECTOR, PRINT,     0, false)); // debug print result
  a.push_back(Instruction(LOCAL,  2/*reductionStage*/, LOCAL,  0, LOCAL, 0, REDUCE, ADD,       0, false)); // add all components of the vector with two "reduction" additions
  a.push_back(Instruction(LOCAL,  3/*reductionStage*/, LOCAL,  0, LOCAL, 0, REDUCE, ADD,       0, false)); // 

  a.push_back(Instruction(LOCAL,  0, LOCAL,  0, LOCAL, 0, VECTOR, PRINT,     0, true)); // debug print result

  programm = a;
}

vector<Instruction> runProgram(ProcessorState& ps) { 
  init();
  initProgram();
  
  for (int i = 0; i < init.size(); ++i) 
  	init.at(i).execute(ps);
  
  return programm;
}

