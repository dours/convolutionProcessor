#include <systemc.h>
#include "common.h"

#ifndef CONVOLUTIONPROC_LORDOFTHEHEADERS_H
#define CONVOLUTIONPROC_LORDOFTHEHEADERS_H

class Instruction;

enum tOperandType {
    WINDOW, LOCAL, COMMON
};
enum tOperationType {
    SHORT_MUL, ADD, SUB, SHIFT_IN, CMP, EXTRACT, PRINT, XOR
};
enum tOperationKind {
    VECTOR, REDUCE
};

struct ProcessorState {
    sc_uint<DATA_WIDTH> big_window[H][W];
    sc_uint<DATA_WIDTH> common_reg[N_REGS][VECTOR_ALU_WIDTH];

    ProcessorState();
};

SC_MODULE(pipeline_sc) {

    sc_in<bool> clock;

    sc_module *units[UNITS_COUNT];

    ProcessorState *st;

    int WindowX;
    int WindowY;

    sc_signal<sc_uint<DATA_WIDTH> > res_data[N_REGS][VECTOR_ALU_WIDTH];
    sc_signal<sc_uint<DATA_WIDTH> > res_local_data[N_REGS][VECTOR_ALU_WIDTH];

    SC_CTOR(pipeline_sc);

    void genWindow();
    void genProgram();
    void setProc(ProcessorState *proc);


};

SC_MODULE(Unit) {
    sc_uint<DATA_WIDTH> data[N_REGS][VECTOR_ALU_WIDTH];
    sc_uint<DATA_WIDTH> local_data[N_REGS][VECTOR_ALU_WIDTH];

    sc_uint<DATA_WIDTH> *next_data[N_REGS][VECTOR_ALU_WIDTH];
    sc_uint<DATA_WIDTH> *next_local_data[N_REGS][VECTOR_ALU_WIDTH];

    sc_in<bool> clock;

    sc_uint<DATA_WIDTH> *getWindow(int addr);

    sc_uint<DATA_WIDTH> *getLocal(int addr);

    Instruction *instruction;
    ProcessorState *proc;

    void setLocal(int addr, sc_uint<DATA_WIDTH> *from);

    void regWrite();

    void execute();

    SC_CTOR(Unit);

    Unit();

};

class Instruction {

    tOperandType lopndk, ropndk, dstk;
    sc_uint<LOG_N_REGS> lopnd, ropnd, dst;

    tOperationKind opKind;
    tOperationType opType[VECTOR_ALU_WIDTH];

    sc_uint<DATA_WIDTH> theConstant;

public:
    // this one creates an instruction in which all the operations are the same
    Instruction();

    Instruction(
            tOperandType lk, sc_uint<LOG_N_REGS> l,
            tOperandType rk, sc_uint<LOG_N_REGS> r,
            tOperandType dstk, sc_uint<LOG_N_REGS> dst,
            tOperationKind opk, tOperationType op,
            sc_uint<DATA_WIDTH> c);

    void execute(ProcessorState *state, Unit &u);

    sc_uint<DATA_WIDTH> scalarOp(tOperationType op, sc_uint<DATA_WIDTH> a, sc_uint<DATA_WIDTH> b);


};

#endif //CONVOLUTIONPROC_LORDOFTHEHEADERS_H
