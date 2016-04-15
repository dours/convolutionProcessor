#include "LordOfTheHeaders.h"

    sc_uint<DATA_WIDTH> *Unit::getWindow(int addr) {
        sc_uint<DATA_WIDTH> *res = new sc_uint<DATA_WIDTH>[VECTOR_ALU_WIDTH];
        for (int i = 0; i < VECTOR_ALU_WIDTH; i++)
            res[i] = data[addr][i];
        return res;
    }

    sc_uint<DATA_WIDTH> *Unit::getLocal(int addr) {
        sc_uint<DATA_WIDTH> *res = new sc_uint<DATA_WIDTH>[VECTOR_ALU_WIDTH];
        for (int i = 0; i < VECTOR_ALU_WIDTH; i++)
            res[i] = local_data[addr][i];
        return res;
    }

    void Unit::setLocal(int addr, sc_uint<DATA_WIDTH> *from) {
        for (int i = 0; i < VECTOR_ALU_WIDTH; i++)
            local_data[addr][i] = from[i];
    }

    void Unit::regWrite() {

        fprintf(stderr, this->basename());
        fprintf(stderr, " Write\n");

        for (int i = 0; i < N_REGS; i++) {
            for (int j = 0; j < VECTOR_ALU_WIDTH; j++) {
                *next_data[i][j] = data[i][j];
                *next_local_data[i][j] = local_data[i][j];

            }
        }

    }

    void Unit::execute() {
        fprintf(stderr, this->basename());
        fprintf(stderr, " Exec\n");
        if(instruction != NULL)
            instruction->execute(proc, *this);
    }

    Unit::Unit(::sc_core::sc_module_name) {
        SC_METHOD(regWrite);
        sensitive << clock.pos();
        SC_METHOD(execute);
        sensitive << clock.pos();
    }

    Unit::Unit() { };



