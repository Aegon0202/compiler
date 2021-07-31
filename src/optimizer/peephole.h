#include "../ssa/ssa.h"
#include "../ssa/traverse.h"

int peep_optimize(BasicBlock* start_block);
void constFolding(BasicBlock* block, void*);
int constProp(BasicBlock* block);
extern struct LinearList* constMark;
extern struct LinearList* constValue;
extern struct LinearList* constType;
extern struct DequeList* prop_worklist;
extern struct DequeList* simp_worklist;