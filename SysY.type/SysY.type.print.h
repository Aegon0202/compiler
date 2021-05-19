#ifndef SYSY_TYPE_PRINT_H
#define SYSY_TYPE_PRINT_H
#include <stdio.h>
#include <stdlib.h>
#include "../utils/NullPointMacro.h"
#include "./SysY.type.visitor.h"
VisitorDeclGenerator(printToken_t, printToken, void);
const char *EnumTypeToString(int type);
void printFresh();
void initPrintToken();
extern struct printToken_t *printToken_p;

void printKeyword(struct Keyword *);
void printIntConst(struct IntConst *);
void printCompUnit(struct CompUnit *);
void printDecl(struct Decl *);
void printConstDecl(struct ConstDecl *);
void printBType(struct BType *);
void printConstDefs(struct ConstDefs *);
void printConstDef(struct ConstDef *);
void printConstArrayDefs(struct ConstArrayDefs *);
void printConstArrayDef(struct ConstArrayDef *);
void printConstInitVal(struct ConstInitVal *);
void printConstInitVals(struct ConstInitVals *);
void printVarDecl(struct VarDecl *);
void printVarDefs(struct VarDefs *);
void printVarDef(struct VarDef *);
void printInitVal(struct InitVal *);
void printInitVals(struct InitVals *);
void printFuncDef(struct FuncDef *);
void printFuncType(struct FuncType *);
void printFuncFParams(struct FuncFParams *);
void printFuncFParam(struct FuncFParam *);
void printExpArrayDefs(struct ExpArrayDefs *);
void printExpArrayDef(struct ExpArrayDef *);
void printBlock(struct Block *);
void printBlockItems(struct BlockItems *);
void printBlockItem(struct BlockItem *);
void printStmt(struct Stmt *);
void printAssign(struct Assign *);
void printIfStmt(struct IfStmt *);
void printWhileStmt(struct WhileStmt *);
void printReturnStmt(struct ReturnStmt *);
void printExp(struct Exp *);
void printCond(struct Cond *);
void printLVal(struct LVal *);
void printArrayImpl(struct ArrayImpl *);
void printPrimaryExp(struct PrimaryExp *);
void printNumber(struct Number *);
void printUnaryExp(struct UnaryExp *);
void printFuncImpl(struct FuncImpl *);
void printUnaryExps(struct UnaryExps *);
void printUnaryOp(struct UnaryOp *);
void printFuncRParams(struct FuncRParams *);
void printFuncRParam(struct FuncRParam *);
void printMulExp(struct MulExp *);
void printMulOp(struct MulOp *);
void printAddExp(struct AddExp *);
void printAddOp(struct AddOp *);
void printRelExp(struct RelExp *);
void printRelOp(struct RelOp *);
void printEqExp(struct EqExp *);
void printEqOp(struct EqOp *);
void printLAndExp(struct LAndExp *);
void printLOrExp(struct LOrExp *);
void printConstExp(struct ConstExp *);
void printIdent(struct Ident *);
void printString(struct String *);
#endif