#include"../ssa/ssa.h"

int peep_optimize(BasicBlock* start_block);
int constFolding(BasicBlock* block);
int constProp(BasicBlock* block);
