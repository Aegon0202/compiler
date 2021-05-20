#ifndef SYSY_AST_DEF_H
#define SYSY_AST_DEF_H

#include "../SysY.type/SysY.type.def.hpp"
#include "../SysY.type/SysY.type.symtab.hpp"

enum
{
    CONST = 700,
    VARIABLE,
    LABEL,
    FUNCTION
};

struct Operand
{
    int type;      // OPERAND
    int valuetype; // CONST VAR FUNCTION LABEL
    union
    {
        struct VarSymEntry *varsymentry;
        struct FuncSymEntry *funcsymentry;
        struct LabelSymEntry *labelsymentry;
    } vaule;
};

struct AST
{
    int type;
    int op;
};

#endif