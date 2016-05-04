#include "LordOfTheHeaders.h"

    sc_uint<DATA_WIDTH> *Unit::getWindow(int addr) {
        sc_uint<DATA_WIDTH> *res = new sc_uint<DATA_WIDTH>[VECTOR_ALU_WIDTH];
        for (int i = 0; i < VECTOR_ALU_WIDTH; i++)
            res[i] = data[addr][i].read();
        return res;
    }

    sc_uint<DATA_WIDTH> *Unit::getLocal(int addr) {
        sc_uint<DATA_WIDTH> *res = new sc_uint<DATA_WIDTH>[VECTOR_ALU_WIDTH];
        for (int i = 0; i < VECTOR_ALU_WIDTH; i++)
            res[i] = local_data[addr][i].read();
        return res;
    }

    void Unit::setLocal(int addr, sc_uint<DATA_WIDTH> *from) {
        for (int i = 0; i < VECTOR_ALU_WIDTH; i++)
            next_local_data[addr][i].write(from[i]);
    }
                           
    void Unit::execute() {
        if(instruction != NULL){

        	for (int i = 0; i < N_REGS; i++)
        		for (int j = 0; j < VECTOR_ALU_WIDTH; j++){
        			next_data[i][j].write(data[i][j]);
        			next_local_data[i][j].write(local_data[i][j]);
        		}
        			

        	fprintf(stderr, this->basename());
        	fprintf(stderr, " Exec ");
            instruction->execute(proc, *this);
    	}
    }

    Unit::Unit(::sc_core::sc_module_name) : clock("clock") {
        SC_METHOD(execute);               	
        sensitive << clock.pos();
    }

    Unit::Unit() { };



