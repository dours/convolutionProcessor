#include "LordOfTheHeaders.h"
 
ProcessorState::ProcessorState() {
	for(int i = 0; i < H; i++)
		for(int j = 0; j < W; j++)
			big_window[i][j] = i + j;

	common_reg[0][0] = 1;
	common_reg[0][1] = 2;
	common_reg[0][2] = 1;

	common_reg[1][0] = 2;
	common_reg[1][1] = 4;
	common_reg[1][2] = 2;
	
	common_reg[2][0] = 1;
	common_reg[2][1] = 2;
	common_reg[2][2] = 1;
}