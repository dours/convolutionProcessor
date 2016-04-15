#include"LordOfTheHeaders.h"

int sc_main(int argc, char** argv){
    sc_clock clock("clock", 1, 0.5);

    ProcessorState ps;
    pipeline_sc pipe("pipeline");
    pipe.setProc(&ps);

    pipe.clock(clock);

    sc_start(50, SC_NS);
    return 0;
}