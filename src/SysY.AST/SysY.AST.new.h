#ifndef SYSY_AST_NEW_H
#define SYSY_AST_NEW_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../SysY.type/SysY.type.def.h"
#include "../parser/SysY.tab.h"
#include "../utils/NullPointMacro.h"
#include "../utils/PrintHelper.h"
#include "SysY.AST.def.h"
struct ExpAST* newExpAST(int op, struct Operand* op1, struct Operand* op2, struct Operand* op3, struct ExpAST* next);
struct Operand* newOperand(int valuetype, void* value);
struct IntConst* newIntConstAST(int value);
#endif