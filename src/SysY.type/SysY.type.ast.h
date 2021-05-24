#ifndef SYSY_TYPE_TOSSA_H
#define SYSY_TYPE_TOSSA_H

#include "SysY.type.visitor.h"

struct toAST
{
    void *(*visitKeyword)(struct Keyword *);
    void *(*visitIntConst)(struct IntConst *);
    void *(*visitCompUnit)(struct CompUnit *);
    void *(*visitDecl)(struct Decl *);
    void *(*visitConstDecl)(struct ConstDecl *);
    void *(*visitBType)(struct BType *);
    void *(*visitConstDefs)(struct ConstDefs *);
    void *(*visitConstDef)(struct ConstDef *);
    void *(*visitConstArrayDefs)(struct ConstArrayDefs *);
    void *(*visitConstArrayDef)(struct ConstArrayDef *);
    void *(*visitConstInitVal)(struct ConstInitVal *);
    void *(*visitConstInitVals)(struct ConstInitVals *);
    void *(*visitVarDecl)(struct VarDecl *);
    void *(*visitVarDefs)(struct VarDefs *);
    void *(*visitVarDef)(struct VarDef *);
    void *(*visitInitVal)(struct InitVal *);
    void *(*visitInitVals)(struct InitVals *);
    void *(*visitFuncDef)(struct FuncDef *);
    void *(*visitFuncType)(struct FuncType *);
    void *(*visitFuncFParams)(struct FuncFParams *);
    void *(*visitFuncFParam)(struct FuncFParam *);
    void *(*visitExpArrayDefs)(struct ExpArrayDefs *);
    void *(*visitExpArrayDef)(struct ExpArrayDef *);
    void *(*visitBlock)(struct Block *);
    void *(*visitBlockItems)(struct BlockItems *);
    void *(*visitBlockItem)(struct BlockItem *);
    void *(*visitStmt)(struct Stmt *);
    void *(*visitAssign)(struct Assign *);
    void *(*visitIfStmt)(struct IfStmt *);
    void *(*visitWhileStmt)(struct WhileStmt *);
    void *(*visitReturnStmt)(struct ReturnStmt *);
    void *(*visitExp)(struct Exp *);
    void *(*visitCond)(struct Cond *);
    void *(*visitLVal)(struct LVal *);
    void *(*visitArrayImpl)(struct ArrayImpl *);
    void *(*visitPrimaryExp)(struct PrimaryExp *);
    void *(*visitNumber)(struct Number *);
    void *(*visitUnaryExp)(struct UnaryExp *);
    void *(*visitFuncImpl)(struct FuncImpl *);
    void *(*visitUnaryExps)(struct UnaryExps *);
    void *(*visitUnaryOp)(struct UnaryOp *);
    void *(*visitFuncRParams)(struct FuncRParams *);
    void *(*visitFuncRParam)(struct FuncRParam *);
    void *(*visitMulExp)(struct MulExp *);
    void *(*visitMulOp)(struct MulOp *);
    void *(*visitAddExp)(struct AddExp *);
    void *(*visitAddOp)(struct AddOp *);
    void *(*visitRelExp)(struct RelExp *);
    void *(*visitRelOp)(struct RelOp *);
    void *(*visitEqExp)(struct EqExp *);
    void *(*visitEqOp)(struct EqOp *);
    void *(*visitLAndExp)(struct LAndExp *);
    void *(*visitLOrExp)(struct LOrExp *);
    void *(*visitConstExp)(struct ConstExp *);
    void *(*visitIdent)(struct Ident *);
    void *(*visitString)(struct String *);
};
void *TokenToAST(void *token, struct toAST *visitor);
void *toASTFuncDef(struct FuncDef *funcdef);

#endif