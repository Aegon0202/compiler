#ifndef SYSY_AST_DEF_H
#define SYSY_AST_DEF_H

#include "../SysY.type/SysY.type.def.h"
#include "../SysY.type/SysY.type.symtab.h"

enum
{
    CONST = 700,
    VARIABLE,
    LABEL,
    FUNCTION,
    LABELSYMENTRY,
    LABELSYMTABLE,
    FUNCIMPLAST,
    ARRAYIMPLAST,
};

struct LabelSymEntry
{
    int type;
    char *label;
};

struct FuncImplAST
{
    int type;
    struct FuncSymEntry *function;
    int param_num;
    struct ExpAST **param; // optional null when funtion doesn't need param
};

struct ArrayImplAST
{
    int type;
    struct VarSymTable *array;
    struct ExpAST **array_impl; // optional null;
};

struct Operand
{
    int type;      // OPERAND
    int valuetype; //
    union
    {
        struct LabelSymEntry *label;
        struct VarSymEntry *variable;
        struct FuncImpl *function;
        struct ArrayImplAST *array;
        struct IntConst *intconst;
        struct ExpAST *exp;
    } vaule;
};

/*
 * op: 
 * if: op1: conditionAST, op2: trueAST, op3: falseAST
 * while: op1: conditionAST, op2: loopAST
 * break: op1: goto label
 * continue: op1: goto label
 * return: op1: returnAST
 *
 * label: op1: label entry
 */
struct ExpAST
{
    int type;
    int op;
    struct Operand *op1;
    struct Operand *op2; // optional:  null
    struct Operand *op3; // optional:  null
    struct ExpAST *next;
};

#endif