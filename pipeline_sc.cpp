#include "LordOfTheHeaders.h"
 
void pipeline_sc::genProgram() {
    (*((Unit *) (units[0]))).instruction = new Instruction(LOCAL, 0, LOCAL, 0, LOCAL, 0, VECTOR, XOR, 0);
   
    (*((Unit *) (units[1]))).instruction = new Instruction(WINDOW, 0, COMMON, 0, LOCAL, 1, VECTOR, SHORT_MUL, 0);
    (*((Unit *) (units[2]))).instruction = new Instruction(LOCAL, 0, LOCAL, 1, LOCAL, 0, VECTOR, ADD, 0);

    (*((Unit *) (units[3]))).instruction = new Instruction(WINDOW, 1, COMMON, 1, LOCAL, 1, VECTOR, SHORT_MUL, 0);
    (*((Unit *) (units[4]))).instruction = new Instruction(LOCAL, 0, LOCAL, 1, LOCAL, 0, VECTOR, ADD, 0);

    (*((Unit *) (units[5]))).instruction = new Instruction(WINDOW, 2, COMMON, 2, LOCAL, 1, VECTOR, SHORT_MUL, 0);
    (*((Unit *) (units[6]))).instruction = new Instruction(LOCAL, 0, LOCAL, 1, LOCAL, 0, VECTOR, ADD, 0);

    (*((Unit *) (units[7]))).instruction = new Instruction(LOCAL, 2, LOCAL, 0, LOCAL, 0, REDUCE, ADD, 0);
    (*((Unit *) (units[8]))).instruction = new Instruction(LOCAL, 3, LOCAL, 0, LOCAL, 0, REDUCE, ADD, 0);

    (*((Unit *) (units[9]))).instruction = new Instruction(LOCAL, 0, LOCAL, 0, LOCAL, 0, VECTOR, PRINT, 0);
}

pipeline_sc::pipeline_sc(::sc_core::sc_module_name) {
    SC_METHOD(genWindow);
    sensitive << clock.pos();
    
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

    WindowX = -1;
    WindowY = 0;

    genProgram();

}

void pipeline_sc::genWindow(){

	WindowX++;
	if(WindowX + WINDOW_SIZE == W){
		WindowX = 0;
		WindowY++;
	}

	if(WindowY + WINDOW_SIZE == H)
		WindowY = 0;

	fprintf(stderr, "Generated window x = %i y = %i\n", WindowX, WindowY);	

    for (int y = 0; y < WINDOW_SIZE; y++)
    	for (int x = 0; x < WINDOW_SIZE; x++)
			(*((Unit *) (units[0]))).data[y][x] = st->big_window[y + WindowY][x + WindowX]; 
}

void pipeline_sc::setProc(ProcessorState *proc) {
    for (int i = 0; i < UNITS_COUNT; i++)
        (*((Unit *) (units[i]))).proc = proc;

	this->st = proc;
}
