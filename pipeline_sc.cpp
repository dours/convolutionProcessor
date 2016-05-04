#include "LordOfTheHeaders.h"
#include <string>
using namespace std;
 
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

string getName(int n){
	char buf[128];
	snprintf(buf, 128, "UNIT_%i", n);
	return string(buf);
}

pipeline_sc::pipeline_sc(::sc_core::sc_module_name) : clock("clock") {
    SC_METHOD(genWindow);
    sensitive << clock.pos();
    
    for (int i = 0; i < UNITS_COUNT; i++) {
        units[i] = static_cast<Unit *>(::sc_core::sc_module_dynalloc(new Unit(getName(i).c_str())));
        units[i]->clock(clock);
    }

    for (int i = 0; i < UNITS_COUNT - 1; i++) {
        for (int j = 0; j < N_REGS; j++) {
            for (int k = 0; k < VECTOR_ALU_WIDTH; k++) {
            	units[i]->next_data[j][k](res_data[i][j][k]);	
            	units[i]->next_local_data[j][k](res_local_data[i][j][k]);	
            
                if(i != 0){
                	units[i]->data[j][k](res_data[i-1][j][k]);	
            		units[i]->local_data[j][k](res_local_data[i-1][j][k]);	
				}

            }
        }
    }

    for (int j = 0; j < N_REGS; j++) {
        for (int k = 0; k < VECTOR_ALU_WIDTH; k++) {
        	units[0]->data[j][k](res_img[j][k]);
        	
        	units[UNITS_COUNT - 1]->next_data[j][k](end_data[j][k]);
        	units[UNITS_COUNT - 1]->next_local_data[j][k](end_local_data[j][k]);

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
			res_img[y][x] = st->big_window[y + WindowY][x + WindowX];    
}

void pipeline_sc::setProc(ProcessorState *proc) {
    for (int i = 0; i < UNITS_COUNT; i++)
        units[i]->proc = proc;

	this->st = proc;
}
