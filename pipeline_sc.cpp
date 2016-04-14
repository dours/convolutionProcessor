#include "LordOfTheHeaders.h"

pipeline_sc::pipeline_sc(::sc_core::sc_module_name ){

        for(int i= 0; i < UNITS_COUNT; i++) {
            units[i] = SC_NEW(Unit("Unit"));
            (*((Unit*)((units[i])))).clock(clock);
            (*((Unit*)((units[i])))).proc = proc;
        }

        for(int i= 0; i < UNITS_COUNT - 1; i++){
            for(int j = 0; j < N_REGS; j++) {
                for (int k = 0; k < VECTOR_ALU_WIDTH; k++) {
                    (*((Unit*)((units[i + 1])))).data[j][k]((*((Unit*)((units[i])))).next_data[j][k]);
                    (*((Unit*)((units[i + 1])))).local_data[j][k]((*((Unit*)((units[i])))).next_local_data[j][k]);
                }
            }
        }

        for(int j = 0; j < N_REGS; j++) {
            for (int k = 0; k < VECTOR_ALU_WIDTH; k++) {
                (*((Unit *) ((units[0])))).data[j][k](res_data[j][k]);
                (*((Unit *) ((units[0])))).local_data[j][k](res_local_data[j][k]);
            }
        }
    }


