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