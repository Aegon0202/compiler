#ifndef SYSY_AST_PRINT_H
#define SYSY_AST_PRINT_H
#include "../SysY.type/SysY.type.def.h"
#include "../SysY.type/SysY.type.symtab.h"
#include "../parser/SysY.tab.h"
#include "../utils/NullPointMacro.h"
#include "../utils/PrintHelper.h"
#include "SysY.AST.def.h"

void printASTOperand(struct Operand* operand);
void printASTExpAST(struct ExpAST* expast);
void printASTVarSymEntry(struct VarSymEntry* varsymentry, int full);
void printASTFuncSymEntry(struct FuncSymEntry* funcsymentry);
void printASTAll(struct FuncSymEntry* head);

#endif