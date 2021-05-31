#ifndef SYSY_TYPE_TOSSA_H
#define SYSY_TYPE_TOSSA_H

#include "../SysY.AST/SysY.AST.def.h"
#include "../SysY.AST/SysY.AST.new.h"
#include "SysY.type.ast.h"
#include "SysY.type.calcConst.h"
#include "SysY.type.def.h"
#include "SysY.type.symtab.h"

void *toASTCompUnit(struct CompUnit *cp);
struct Operand *toASTString(struct String *string);
struct Operand *toASTFuncRParam(struct FuncRParam *funcrparam);
struct Operand *toASTFuncImpl(struct FuncImpl *funcimpl);
struct Operand *toASTNumber(struct Number *number);
struct Operand *toASTIdent(struct Ident *ident);
struct Operand *toASTArrayImpl(struct ArrayImpl *arrayimpl);
struct Operand *toASTLVal(struct LVal *lval);
struct Operand *toASTPrimaryExp(struct PrimaryExp *primaryexp);
struct Operand *toASTUnaryExps(struct UnaryExps *unaryexps);
struct Operand *toASTUnaryExp(struct UnaryExp *unaryexp);
struct Operand *toASTMulExp(struct MulExp *mulexp);
struct Operand *toASTAddExp(struct AddExp *addexp);
struct Operand *toASTExp(struct Exp *exp);
struct Operand *toASTRelExp(struct RelExp *relexp);
struct Operand *toASTEqExp(struct EqExp *eqexp);
struct Operand *toASTLAndExp(struct LAndExp *landexp);
struct Operand *toASTLOrExp(struct LOrExp *lorexp);
struct Operand *toASTCond(struct Cond *cond);
void *toASTInitVal(struct InitVal *initval, struct Operand **init_target, struct Operand **array_shape, int array_size, int array_dimensional_num);
struct VarSymEntry *toASTVarDef(struct VarDef *vardef);
void *toASTVarDecl(struct VarDecl *vardecl);
void *toASTConstInitVal(struct ConstInitVal *initval, struct Operand **init_target, struct Operand **array_shape, int array_size, int array_dimensional_num);
struct VarSymEntry *toASTConstDef(struct ConstDef *constdef);
void *toASTConstDecl(struct ConstDecl *constdecl);
void *toASTDecl(struct Decl *decl);
void *toASTFuncFParams(struct FuncFParams *funcfparams, struct FuncSymEntry *fse);
struct ExpAST *toASTIfStmt(struct IfStmt *ifstmt);
struct ExpAST *toASTWhileStmt(struct WhileStmt *whilestmt);
struct ExpAST *toASTStmt(struct Stmt *stmt);
struct ExpAST *toASTBlockItem(struct BlockItem *blockitem);
struct ExpAST *toASTBlockItems(struct BlockItems *blockitems);
struct ExpAST *toASTBlock(struct Block *block);
struct FuncSymEntry *toASTFuncDef(struct FuncDef *funcdef);

#endif