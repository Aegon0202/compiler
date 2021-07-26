#include "../ssa/ssa.h"

int peep_optimize(BasicBlock* start_block);
void constFolding(BasicBlock* block, void*);
int constProp(BasicBlock* block);
