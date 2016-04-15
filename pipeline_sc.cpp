#include "LordOfTheHeaders.h"

char *getNumName(char *name, int num) {
    name[4] = (char) ('A' + num);
    return name;
}

void pipeline_sc::genProgram() {
    (*((Unit *) (units[0]))).instruction = new Instruction(LOCAL, 0, LOCAL, 0, LOCAL, 0, VECTOR, XOR, 0);

    (*((Unit *) (units[1]))).instruction = new Instruction(WINDOW, 0, LOCAL, 0, COMMON, 0, VECTOR, SHIFT_IN, 1);
    (*((Unit *) (units[2]))).instruction = new Instruction(WINDOW, 0, LOCAL, 0, COMMON, 0, VECTOR, SHIFT_IN, 2);
    (*((Unit *) (units[3]))).instruction = new Instruction(WINDOW, 0, LOCAL, 0, COMMON, 0, VECTOR, SHIFT_IN, 1);

    (*((Unit *) (units[4]))).instruction = new Instruction(WINDOW, 0, LOCAL, 0, COMMON, 1, VECTOR, SHIFT_IN, 2);
    (*((Unit *) (units[5]))).instruction = new Instruction(WINDOW, 0, LOCAL, 1, COMMON, 1, VECTOR, SHIFT_IN, 4);
    (*((Unit *) (units[6]))).instruction = new Instruction(WINDOW, 0, LOCAL, 1, COMMON, 1, VECTOR, SHIFT_IN, 2);

    (*((Unit *) (units[7]))).instruction = new Instruction(WINDOW, 0, LOCAL, 0, COMMON, 2, VECTOR, SHIFT_IN, 1);
    (*((Unit *) (units[8]))).instruction = new Instruction(WINDOW, 0, LOCAL, 2, COMMON, 2, VECTOR, SHIFT_IN, 2);
    (*((Unit *) (units[9]))).instruction = new Instruction(WINDOW, 0, LOCAL, 2, COMMON, 2, VECTOR, SHIFT_IN, 1);

    (*((Unit *) (units[10]))).instruction = new Instruction(WINDOW, 0, COMMON, 0, LOCAL, 1, VECTOR, SHORT_MUL, 0);
    (*((Unit *) (units[11]))).instruction = new Instruction(LOCAL, 0, LOCAL, 1, LOCAL, 0, VECTOR, ADD, 0);

    (*((Unit *) (units[12]))).instruction = new Instruction(WINDOW, 1, COMMON, 1, LOCAL, 1, VECTOR, SHORT_MUL, 0);
    (*((Unit *) (units[13]))).instruction = new Instruction(LOCAL, 0, LOCAL, 1, LOCAL, 0, VECTOR, ADD, 0);

    (*((Unit *) (units[14]))).instruction = new Instruction(WINDOW, 2, COMMON, 2, LOCAL, 1, VECTOR, SHORT_MUL, 0);
    (*((Unit *) (units[15]))).instruction = new Instruction(LOCAL, 0, LOCAL, 1, LOCAL, 0, VECTOR, ADD, 0);

    (*((Unit *) (units[16]))).instruction = new Instruction(LOCAL, 2, LOCAL, 0, LOCAL, 0, REDUCE, ADD, 0);
    (*((Unit *) (units[17]))).instruction = new Instruction(LOCAL, 3, LOCAL, 0, LOCAL, 0, REDUCE, ADD, 0);

    (*((Unit *) (units[18]))).instruction = new Instruction(LOCAL, 0, LOCAL, 0, LOCAL, 0, VECTOR, PRINT, 0);

    (*((Unit *) (units[19]))).instruction = new Instruction(COMMON, 0, COMMON, 0, COMMON, 0, VECTOR, PRINT, 0);
    (*((Unit *) (units[20]))).instruction = new Instruction(COMMON, 1, COMMON, 1, COMMON, 1, VECTOR, PRINT, 0);
    (*((Unit *) (units[21]))).instruction = new Instruction(COMMON, 2, COMMON, 2, COMMON, 2, VECTOR, PRINT, 0);
}

pipeline_sc::pipeline_sc(::sc_core::sc_module_name) {
    for (int i = 0; i < UNITS_COUNT; i++) {
        units[i] = ::sc_core::sc_module_dynalloc(new Unit("Unit"));
        (*((Unit *) (units[i]))).clock(clock);
    }

    for (int i = 0; i < UNITS_COUNT - 1; i++) {
        for (int j = 0; j < N_REGS; j++) {
            for (int k = 0; k < VECTOR_ALU_WIDTH; k++) {
                (*((Unit *) (units[i]))).next_data[j][k] = &(*((Unit *) (units[i + 1]))).data[j][k];
                (*((Unit *) (units[i]))).next_local_data[j][k] = &(*((Unit *) (units[i + 1]))).local_data[j][k];
            }
        }
    }

    for (int j = 0; j < N_REGS; j++) {
        for (int k = 0; k < VECTOR_ALU_WIDTH; k++) {
            (*((Unit *) (units[UNITS_COUNT - 1]))).next_data[j][k] = &(*((Unit *) (units[UNITS_COUNT - 1]))).data[j][k];
            (*((Unit *) (units[UNITS_COUNT - 1]))).next_local_data[j][k] = &(*((Unit *) (units[UNITS_COUNT - 1]))).local_data[j][k];
        }
    }

    genProgram();

}

void pipeline_sc::setProc(ProcessorState *proc) {
    for (int i = 0; i < UNITS_COUNT; i++)
        (*((Unit *) (units[i]))).proc = proc;
}
