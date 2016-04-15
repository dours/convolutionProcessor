#include "LordOfTheHeaders.h"

void ProcessorState::setCommon(int addr, sc_uint<DATA_WIDTH> *from) {
    for (int i = 0; i < VECTOR_ALU_WIDTH; i++)
        common_reg[addr][i] = from[i];
}

ProcessorState::ProcessorState() { }