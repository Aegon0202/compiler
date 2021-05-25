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
struct Operand *toASTPrimaryExp(struct PrimaryExp *primaryexp);
struct Operand *toASTUnaryExp(struct UnaryExp *unaryexp);
struct Operand *toASTMulExp(struct MulExp *mulexp);
struct Operand *toASTAddExp(struct AddExp *addexp);
struct Operand *toASTExp(struct Exp *exp);
struct VarSymEntry *toASTVarDef(struct VarDef *vardef);
void *toASTVarDecl(struct VarDecl *vardecl);
void *toASTDecl(struct Decl *decl);
struct FuncSymEntry *toASTFuncDef(struct FuncDef *funcdef);
struct Operand *toASTNumber(struct Number *number);
struct Operand *toASTLVal(struct LVal *lval);
#endif