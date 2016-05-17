#ifndef PROCESSOR
#define PROCESSOR

#include "common.h"
#include <systemc.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

SC_MODULE(Processor){

	sc_uint<DATA_WIDTH> bigWindow[H][W];

	vector<sc_uint<DATA_WIDTH> >  commonRegs[N_REGS];
	vector<sc_uint<DATA_WIDTH> >  localRegs[N_THREADS][N_REGS];

	vector<sc_uint<DATA_WIDTH> > operands[N_THEREADS][2];

	sc_clock clock;

	void read(){}
	void eval(){}
	void write(){}

	SC_CTOR(Processor){
		SC_METHOD(read);
		sensitive << clock.pos();
	
		SC_METHOD(eval);
		sensitive << clock.pos();
		
		SC_METHOD(write);
		sensitive << clock.pos();
	
	
	}

}

#endif