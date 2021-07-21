#ifndef SYSY_SYMTAB_SSA_H
#define SYSY_SYMTAB_SSA_H
#include "../SysY.type/SysY.type.def.h"
#include "../ssa/ssa.h"
#include "./SysY.symtab.def.h"

struct IntConst *getIntConstStatic(int value);
struct ArrayTabElem *toSSAConstArrayDefs(struct ConstArrayDefs *arraydefs);
void __var_def_array_init(struct InitVal *initval, struct VarTabElem *elem, BASIC_BLOCK_TYPE *basic_block);
void __var_def_init(struct VarDef *vardef, struct VarTabElem *elem, BASIC_BLOCK_TYPE *basic_block);
int toSSAVarDecl(struct VarDecl *vardecl, BASIC_BLOCK_TYPE *basic_block);
struct ArrayTabElem *__offset_to_max_hight_dis(struct ArrayTabElem *array, int offset);
void __const_def_array_init(struct ConstInitVals *constinitvals, struct ArrayTabElem *array, int *buffer);
void __const_def_init(struct ConstDef *constdef, struct VarTabElem *elem, BASIC_BLOCK_TYPE *basic_block);
int toSSAConstDecl(struct ConstDecl *constdecl, BASIC_BLOCK_TYPE *basic_block);
int toSSADecl(struct Decl *decl, BASIC_BLOCK_TYPE *basic_block);
struct ArrayTabElem *toSSAExpArrayDefs(struct ExpArrayDefs *exparraydefs, BASIC_BLOCK_TYPE *basic_block);
void toSSALValWrite(struct LVal *lval, OPERAND_TYPE *result, BASIC_BLOCK_TYPE *basic_block);
void toSSAAssign(struct Assign *assign, BASIC_BLOCK_TYPE *basic_block);
void toSSALAndExp(struct LAndExp *landexp, BASIC_BLOCK_TYPE *true_block, BASIC_BLOCK_TYPE *false_block, BASIC_BLOCK_TYPE *basic_block);
void toSSALOrExp(struct LOrExp *lorexp, BASIC_BLOCK_TYPE *true_block, BASIC_BLOCK_TYPE *false_block, BASIC_BLOCK_TYPE *basic_block);
void toSSACond(struct Cond *cond, BASIC_BLOCK_TYPE *true_block, BASIC_BLOCK_TYPE *false_block, BASIC_BLOCK_TYPE *basic_block);
void toSSAIfStmt(struct IfStmt *ifstmt, BASIC_BLOCK_TYPE **basic_block_p);
void toSSAWhileStmt(struct WhileStmt *whilestmt, BASIC_BLOCK_TYPE **basic_block_p);
void toSSAReturnStmt(struct ReturnStmt *returnstmt, BASIC_BLOCK_TYPE *basic_block);
void toSSABreakStmt(BASIC_BLOCK_TYPE *basic_block);
void toSSAContinueStmt(BASIC_BLOCK_TYPE *basic_block);
int toSSAStmt(struct Stmt *stmt, BASIC_BLOCK_TYPE **basic_block_p);
int toSSABlock(struct Block *block, BASIC_BLOCK_TYPE **basic_block_p);
void toSSAFuncDef(struct FuncDef *funcdef);
static int basic_block_equal(void *k1, void *k2);
void toSSACompUnit(struct CompUnit *cp);

#endif