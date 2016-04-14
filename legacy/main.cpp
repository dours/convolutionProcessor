#include "processorState.h"

void runProgram(ProcessorState&);

int sc_main(int, char**) { 
  ProcessorState ps;
  runProgram(ps);
  return 22;
}

