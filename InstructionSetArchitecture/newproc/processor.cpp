#ifndef PROCESSOR
#define PROCESSOR

#include "common.h"
#include <systemc.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "programm.cpp"
using namespace std;

SC_MODULE(Processor){

	sc_uint<DATA_WIDTH> bigWindow[H][W];
	vector<Instructio> programm;

	vector<sc_uint<DATA_WIDTH> >  commonRegs[N_REGS];
	vector<sc_uint<DATA_WIDTH> >  localRegs[N_THREADS][N_REGS];

	vector<sc_uint<DATA_WIDTH> >  local[N_THREADS][3]; //[0] - a, [1] - b, [2] - result

	sc_clock clock;

	void read(){
	
	}
	
	void eval(){
	
	}
	
	void write(){
	
	}

	SC_CTOR(Processor){
		SC_METHOD(read);
		sensitive << clock.pos();
	
		SC_METHOD(eval);
		sensitive << clock.pos();
		
		SC_METHOD(write);
		sensitive << clock.pos();

		initLocalStorage();
		programm = initProgramm(*this);
	}

	void initLocalStorage(){
		vector<sc_uint<DATA_WIDTH> > beef;
  		for (int i = 0; i < VECTOR_ALU_WIDTH; ++i) beef.push_back(0xbeef);

 	 	for (int i = 0; i < N_REGS; ++i) commonRegs[i] = beef;
 	 	for (int t = 0; t < N_THREADS; ++t) for (int i = 0; i < N_REGS; ++i) localRegs[t][i] = beef;
 	 	for (int t = 0; t < N_THREADS; ++t) for (int i = 0; i < 3; ++i) localRegs[t][i] = beef;

  	 for (int y = 0; y < H; ++y) for (int x = 0; x < W; ++x) bigWindow[y][x] = x + y;



	}

}

#endif