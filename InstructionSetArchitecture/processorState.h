#ifndef PROCESSOR_STATE
#define PROCESSOR_STATE

#include "common.h"
#include <systemc.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

#define H 10
#define W 100

class ProcessorState { 

  sc_uint<DATA_WIDTH> bigWindow[H][W];

  vector<sc_uint<DATA_WIDTH> >  commonRegs[N_REGS];
  vector<sc_uint<DATA_WIDTH> >  localRegs[N_THREADS][N_REGS];

  int curThreadNo;
  int windowXPos;                                                        `
  int windowYPos;

public:

  ProcessorState() : curThreadNo(0), windowXPos(0), windowYPos(0) { 

  vector<sc_uint<DATA_WIDTH> > beef;
  for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) beef.push_back(0xbeef);

  for (int i = 0; i < N_REGS; ++i) commonRegs[i] = beef;
  for (int t = 0; t < N_THREADS; ++t) for (int i = 0; i < N_REGS; ++i) localRegs[t][i] = beef;

  // setting test data
    for (int y = 0; y < H; ++y) for (int x = 0; x < W; ++x) bigWindow[y][x] = x + y;

  } 

  vector<sc_uint<DATA_WIDTH> > getWindow(sc_uint<LOG_N_REGS> x) { 
    vector<sc_uint<DATA_WIDTH> > outp;
    assert(windowXPos + x < W);
    assert(windowYPos + VECTOR_ALU_WIDTH < H);
    for (int y = 0; y < VECTOR_ALU_WIDTH; ++y) outp.push_back(bigWindow[y + windowYPos][x + windowXPos]);
    return outp;
  }
  
  vector<sc_uint<DATA_WIDTH> > getLocal(sc_uint<LOG_N_REGS> x) { 
    assert(x < N_REGS);
    return localRegs[curThreadNo][x];
  }

  void setLocal(sc_uint<LOG_N_REGS> x, vector<sc_uint<DATA_WIDTH> > value) { 
    assert(x < N_REGS);
    localRegs[curThreadNo][x] = value;
  }

  vector<sc_uint<DATA_WIDTH> > getCommon(sc_uint<LOG_N_REGS> x) { 
    assert(x < N_REGS);
    return commonRegs[x];
  }

  void setCommon(sc_uint<LOG_N_REGS> x, vector<sc_uint<DATA_WIDTH> > value) { 
    assert(x < N_REGS);
    commonRegs[x] = value;
  }

  
  void moveWindow() { 
    fprintf(stderr, "move\n");
    ++windowXPos;
    if (windowXPos + VECTOR_ALU_WIDTH == W) { 
      windowXPos = 0;
      ++windowYPos;
      if (windowYPos + VECTOR_ALU_WIDTH == H) { fprintf(stderr, "Enough for now.\n"); exit(0); } 
    }
  }

};

#endif