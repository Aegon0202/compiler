#ifndef SYSY_AST_DEF_H
#define SYSY_AST_DEF_H

#include "../SysY.type/SysY.type.def.h"
#include "../SysY.type/SysY.type.symtab.h"

enum {
    LABELENTRY = 700,
    //LABELSYMTABLE,
    FUNCIMPLAST,
    ARRAYIMPLAST,
    EXPAST,
    OPREAND
};

struct LabelEntry {
    int type;
    char *label;
    struct ExpAST *position;
};

struct FuncImplAST {
    int type;
    struct FuncSymEntry *function;
    int param_num;
    struct Operand **param;  // optional null when funtion doesn't need param
};

struct ArrayImplAST {
    int type;
    struct VarSymEntry *array_varsymentry;
    int array_impl_size;
    struct Operand **array_impl;  // optional null;
};

struct Operand {
    int type;       // OPERAND
    int valuetype;  // INTCONST, LABELENTRY, VARSYMENTRY, FUNCIMPLAST, ARRAYIMPLAST,EXPAST
    union {
        struct LabelEntry *label;
        struct VarSymEntry *variable;
        struct FuncImpl *function;
        struct ArrayImplAST *array;
        struct IntConst *intconst;
        struct ExpAST *exp;
        struct String *string;
    } value;
};

/*
 * op: 
 * IFSTMT: op1: conditionAST, op2: trueAST, op3: falseAST
 * WHILESTMT: op1: conditionAST, op2: loopAST
 * BREAKATMT: no op
 * CONTINUESTMT: no op
 * RETURNSTMT: op1: returnAST
 * 
 * ASSIGN: op1: LVAL, op2: EXPAST
 *
 * LABELENTRY: op1: label operand
 * K_NOT: only op1
 * 
 * INTCONST: intconst operand
 * 
 * K_ADD, K_SUB, K_MUL, K_DIV, K_MOD, 
 * K_AND, K_OR, K_EQ, K_NEQ, 
 * K_LT, K_LTE, K_GT, K_GTE,
 * 
 * OPERAND: only op1
 */
struct ExpAST {
    int type;
    int op;  //
    struct Operand *op1;
    struct Operand *op2;  // optional:  null
    struct Operand *op3;  // optional:  null
    struct ExpAST *next;
};

#endif