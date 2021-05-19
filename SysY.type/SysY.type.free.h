#ifndef SYSY_TYPE_FREE_H
#define SYSY_TYPE_FREE_H
#include <stdio.h>
#include <stdlib.h>
#include "../utils/NullPointMacro.h"
#include "./SysY.type.visitor.h"
VisitorDeclGenerator(freeToken_t, freeToken, void, int);
void initFreeToken();

void freeKeyword(struct Keyword *, int recusive);
void freeIntConst(struct IntConst *, int recusive);
void freeCompUnit(struct CompUnit *, int recusive);
void freeDecl(struct Decl *, int recusive);
void freeConstDecl(struct ConstDecl *, int recusive);
void freeBType(struct BType *, int recusive);
void freeConstDefs(struct ConstDefs *, int recusive);
void freeConstDef(struct ConstDef *, int recusive);
void freeConstArrayDefs(struct ConstArrayDefs *, int recusive);
void freeConstArrayDef(struct ConstArrayDef *, int recusive);
void freeConstInitVal(struct ConstInitVal *, int recusive);
void freeConstInitVals(struct ConstInitVals *, int recusive);
void freeVarDecl(struct VarDecl *, int recusive);
void freeVarDefs(struct VarDefs *, int recusive);
void freeVarDef(struct VarDef *, int recusive);
void freeInitVal(struct InitVal *, int recusive);
void freeInitVals(struct InitVals *, int recusive);
void freeFuncDef(struct FuncDef *, int recusive);
void freeFuncType(struct FuncType *, int recusive);
void freeFuncFParams(struct FuncFParams *, int recusive);
void freeFuncFParam(struct FuncFParam *, int recusive);
void freeExpArrayDefs(struct ExpArrayDefs *, int recusive);
void freeExpArrayDef(struct ExpArrayDef *, int recusive);
void freeBlock(struct Block *, int recusive);
void freeBlockItems(struct BlockItems *, int recusive);
void freeBlockItem(struct BlockItem *, int recusive);
void freeStmt(struct Stmt *, int recusive);
void freeAssign(struct Assign *, int recusive);
void freeIfStmt(struct IfStmt *, int recusive);
void freeWhileStmt(struct WhileStmt *, int recusive);
void freeReturnStmt(struct ReturnStmt *, int recusive);
void freeExp(struct Exp *, int recusive);
void freeCond(struct Cond *, int recusive);
void freeLVal(struct LVal *, int recusive);
void freeArrayImpl(struct ArrayImpl *, int recusive);
void freePrimaryExp(struct PrimaryExp *, int recusive);
void freeNumber(struct Number *, int recusive);
void freeUnaryExp(struct UnaryExp *, int recusive);
void freeFuncImpl(struct FuncImpl *, int recusive);
void freeUnaryExps(struct UnaryExps *, int recusive);
void freeUnaryOp(struct UnaryOp *, int recusive);
void freeFuncRParams(struct FuncRParams *, int recusive);
void freeFuncRParam(struct FuncRParam *, int recusive);
void freeMulExp(struct MulExp *, int recusive);
void freeMulOp(struct MulOp *, int recusive);
void freeAddExp(struct AddExp *, int recusive);
void freeAddOp(struct AddOp *, int recusive);
void freeRelExp(struct RelExp *, int recusive);
void freeRelOp(struct RelOp *, int recusive);
void freeEqExp(struct EqExp *, int recusive);
void freeEqOp(struct EqOp *, int recusive);
void freeLAndExp(struct LAndExp *, int recusive);
void freeLOrExp(struct LOrExp *, int recusive);
void freeConstExp(struct ConstExp *, int recusive);
void freeIdent(struct Ident *, int recusive);
void freeString(struct String *, int recusive);

#endif