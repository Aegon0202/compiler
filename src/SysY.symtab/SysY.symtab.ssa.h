#ifndef SYSY_SYMTAB_SSA_H
#define SYSY_SYMTAB_SSA_H
#include "../SysY.type/SysY.type.def.h"
#include "../ssa/ssa.h"
#include "./SysY.symtab.def.h"

struct ArrayTabElem* toSSAConstArrayDefs(struct ConstArrayDefs* arraydefs);
OPERAND_TYPE* toSSAArrayImplAddress(struct ArrayImpl* arrayimpl, struct VarTabElem* vte, int* result_is_array, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSALValRead(struct LVal* lval, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSANumber(struct Number* number, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAPrimaryExp(struct PrimaryExp* primaryexp, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAFuncImpl(struct FuncImpl* funcimpl, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAUnaryExps(struct UnaryExps* unaryexps, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAUnaryExp(struct UnaryExp* unaryexp, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAMulExp(struct MulExp* exp, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAAddExp(struct AddExp* exp, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSARelExp(struct RelExp* exp, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAEqExp(struct EqExp* exp, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAExp(struct Exp* exp, BASIC_BLOCK_TYPE* basic_block);
int toSSAVarDecl(struct VarDecl* vardecl, BASIC_BLOCK_TYPE* basic_block);
int toSSAConstDecl(struct ConstDecl* constdecl, BASIC_BLOCK_TYPE* basic_block);
int toSSADecl(struct Decl* decl, BASIC_BLOCK_TYPE* basic_block);
struct ArrayTabElem* toSSAExpArrayDefs(struct ExpArrayDefs* exparraydefs, BASIC_BLOCK_TYPE* basic_block);
void toSSALValWrite(struct LVal* lval, OPERAND_TYPE* result, BASIC_BLOCK_TYPE* basic_block);
void toSSAAssign(struct Assign* assign, BASIC_BLOCK_TYPE* basic_block);
void toSSALAndExp(struct LAndExp* landexp, BASIC_BLOCK_TYPE* true_block, BASIC_BLOCK_TYPE* false_block, BASIC_BLOCK_TYPE* basic_block);
void toSSALOrExp(struct LOrExp* lorexp, BASIC_BLOCK_TYPE* true_block, BASIC_BLOCK_TYPE* false_block, BASIC_BLOCK_TYPE* basic_block);
void toSSACond(struct Cond* cond, BASIC_BLOCK_TYPE* true_block, BASIC_BLOCK_TYPE* false_block, BASIC_BLOCK_TYPE* basic_block);
int toSSAIfStmt(struct IfStmt* ifstmt, BASIC_BLOCK_TYPE** basic_block_p);
int toSSAWhileStmt(struct WhileStmt* whilestmt, BASIC_BLOCK_TYPE** basic_block_p);
void toSSAReturnStmt(struct ReturnStmt* returnstmt, BASIC_BLOCK_TYPE* basic_block);
void toSSABreakStmt(BASIC_BLOCK_TYPE* basic_block);
void toSSAContinueStmt(BASIC_BLOCK_TYPE* basic_block);
int toSSAStmt(struct Stmt* stmt, BASIC_BLOCK_TYPE** basic_block_p);
int toSSABlock(struct Block* block, BASIC_BLOCK_TYPE** basic_block_p);
void toSSAFuncDef(struct FuncDef* funcdef);
void toSSACompUnit(struct CompUnit* cp);

#endif