#ifndef OPTIMIZER_LOCAL_OP_H
#define OPTIMIZER_LOCAL_OP_H

struct Definition* get_op_definition(OPERAND_TYPE* op);
IR_TYPE* newIR_WITH_SSA(int type, OPERAND_TYPE* op1, OPERAND_TYPE* op2, OPERAND_TYPE* op3);
void delete_operand(OPERAND_TYPE* op);
void change_def_address(IR_TYPE* old_ir, BASIC_BLOCK_TYPE* old_block, BASIC_BLOCK_TYPE* new_block, IR_TYPE* new_before_ir);
struct Definition* create_new_definition(int reg, Ir* ir, BasicBlock* block);
int alloc_register();

void localExprEliminate(struct FuncTabElem* elem);
void loopInvariantExtraction(struct FuncTabElem* elem);

#endif