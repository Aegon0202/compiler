#ifndef SYSY_AST_SSA_H
#define SYSY_AST_SSA_H

#include "../SysY.type/SysY.type.def.h"
#include "../SysY.type/SysY.type.symtab.h"
#include "../parser/SysY.tab.h"
#include "../ssa/ssa.h"
#include "../utils/LinkedTable.h"
#include "../utils/NullPointMacro.h"
#include "../utils/PrintHelper.h"
#include "./SysY.AST.def.h"

#define BASIC_BLOCK_TYPE BasicBlock
#define IR_LIST_TYPE Ir
#define IR_TYPE Ir
#define OPERAND_TYPE Operand

/**
 * SSA_IR_OP
 * 详见 README.md
*/
enum SSA_IR_OP {
    PARAM = 800,
    CALL,
    JUMP,
    BRANCH,
    LOAD,
    STORE,
    NOP,
};

// 一个函数名到对应的基本块首地址的对应表
extern struct LinkedTable* func_block_table;

OPERAND_TYPE* toSSAFuncImplAST(struct FuncImplAST* func_impl, int block_id);
OPERAND_TYPE* toSSAArrayImplAST(struct ArrayImplAST* array_impl, int block_id);
OPERAND_TYPE* toSSAOperand(struct Operand* operand, int block_id);
void toSSAIfStmt(struct ExpAST* exp, int* block_id);
OPERAND_TYPE* toSSAWhileStmt(struct ExpAST* exp, int* block_id);
void toSSABreakStmt(int block_id);
void toSSAContinuekStmt(int block_id);
OPERAND_TYPE* toSSAExpAST(struct ExpAST* exp, int* block_id);
int toSSABlock(struct ExpAST* exp);
BASIC_BLOCK_TYPE* toSSAFunction(struct FuncSymEntry* fse);
void toSSAFunctions();

#endif