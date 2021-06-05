#include "SysY.AST.new.h"

struct ExpAST* newExpAST(int op, struct Operand* op1, struct Operand* op2, struct Operand* op3, struct ExpAST* next) {
    struct ExpAST* exp = (struct ExpAST*)malloc(sizeof(struct ExpAST));
    EnsureNotNull(exp);
    exp->type = EXPAST;
    exp->op1 = op1;
    exp->op2 = op2;
    exp->op3 = op3;
    exp->op = op;
    exp->next = next;
    return exp;
}

struct Operand* newOperand(int valuetype, void* value) {
    struct Operand* operand = (struct Operand*)malloc(sizeof(struct Operand));
    EnsureNotNull(operand);
    operand->type = OPREAND;
    operand->valuetype = valuetype;
    operand->value.label = value;
    return operand;
}

struct IntConst* newIntConstAST(int value) {
    struct IntConst* intconst = (struct IntConst*)malloc(sizeof(struct IntConst));
    EnsureNotNull(intconst);
    intconst->type = INTCONST;
    intconst->value = value;
    return intconst;
}

struct FuncImplAST* newFuncImplAST(struct FuncSymEntry* function, int param_num, struct Operand** param) {
    struct FuncImplAST* funcimpl = (struct FuncImplAST*)malloc(sizeof(struct FuncImplAST));
    EnsureNotNull(funcimpl);
    funcimpl->type = FUNCIMPLAST;
    funcimpl->function = function;
    funcimpl->param = param;
    funcimpl->param_num = param_num;
    return funcimpl;
}

struct ArrayImplAST* newArrayImplAST(struct VarSymEntry* array_varsymentry, int array_impl_size, struct Operand** array_impl) {
    struct ArrayImplAST* arrayimpl = (struct ArrayImplAST*)malloc(sizeof(struct ArrayImplAST));
    EnsureNotNull(arrayimpl);
    arrayimpl->type = ARRAYIMPLAST;
    arrayimpl->array_impl = array_impl;
    arrayimpl->array_impl_size = array_impl_size;
    arrayimpl->array_varsymentry = array_varsymentry;
    return arrayimpl;
}