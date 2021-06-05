#ifndef SYSY_TYPE_PRINT_H
#define SYSY_TYPE_PRINT_H
#include <stdio.h>
#include <stdlib.h>

#include "../utils/NullPointMacro.h"
#include "./SysY.type.visitor.h"

struct printToken_t {
    void (*visitKeyword)(struct Keyword *);
    void (*visitIntConst)(struct IntConst *);
    void (*visitCompUnit)(struct CompUnit *);
    void (*visitDecl)(struct Decl *);
    void (*visitConstDecl)(struct ConstDecl *);
    void (*visitBType)(struct BType *);
    void (*visitConstDefs)(struct ConstDefs *);
    void (*visitConstDef)(struct ConstDef *);
    void (*visitConstArrayDefs)(struct ConstArrayDefs *);
    void (*visitConstArrayDef)(struct ConstArrayDef *);
    void (*visitConstInitVal)(struct ConstInitVal *);
    void (*visitConstInitVals)(struct ConstInitVals *);
    void (*visitVarDecl)(struct VarDecl *);
    void (*visitVarDefs)(struct VarDefs *);
    void (*visitVarDef)(struct VarDef *);
    void (*visitInitVal)(struct InitVal *);
    void (*visitInitVals)(struct InitVals *);
    void (*visitFuncDef)(struct FuncDef *);
    void (*visitFuncType)(struct FuncType *);
    void (*visitFuncFParams)(struct FuncFParams *);
    void (*visitFuncFParam)(struct FuncFParam *);
    void (*visitExpArrayDefs)(struct ExpArrayDefs *);
    void (*visitExpArrayDef)(struct ExpArrayDef *);
    void (*visitBlock)(struct Block *);
    void (*visitBlockItems)(struct BlockItems *);
    void (*visitBlockItem)(struct BlockItem *);
    void (*visitStmt)(struct Stmt *);
    void (*visitAssign)(struct Assign *);
    void (*visitIfStmt)(struct IfStmt *);
    void (*visitWhileStmt)(struct WhileStmt *);
    void (*visitReturnStmt)(struct ReturnStmt *);
    void (*visitExp)(struct Exp *);
    void (*visitCond)(struct Cond *);
    void (*visitLVal)(struct LVal *);
    void (*visitArrayImpl)(struct ArrayImpl *);
    void (*visitPrimaryExp)(struct PrimaryExp *);
    void (*visitNumber)(struct Number *);
    void (*visitUnaryExp)(struct UnaryExp *);
    void (*visitFuncImpl)(struct FuncImpl *);
    void (*visitUnaryExps)(struct UnaryExps *);
    void (*visitUnaryOp)(struct UnaryOp *);
    void (*visitFuncRParams)(struct FuncRParams *);
    void (*visitFuncRParam)(struct FuncRParam *);
    void (*visitMulExp)(struct MulExp *);
    void (*visitMulOp)(struct MulOp *);
    void (*visitAddExp)(struct AddExp *);
    void (*visitAddOp)(struct AddOp *);
    void (*visitRelExp)(struct RelExp *);
    void (*visitRelOp)(struct RelOp *);
    void (*visitEqExp)(struct EqExp *);
    void (*visitEqOp)(struct EqOp *);
    void (*visitLAndExp)(struct LAndExp *);
    void (*visitLOrExp)(struct LOrExp *);
    void (*visitConstExp)(struct ConstExp *);
    void (*visitIdent)(struct Ident *);
    void (*visitString)(struct String *);
};
void printToken(void *token, struct printToken_t *visitor);

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