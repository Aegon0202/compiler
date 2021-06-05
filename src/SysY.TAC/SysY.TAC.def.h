#ifndef SYSY_AST_TAC_H
#define SYSY_AST_TAC_H

enum{

}
/*
 * op:
 * ASSIGN: op1: var, op2: var
 *
 * K_NOT: only op1
 * 
 * K_ADD, K_SUB, K_MUL, K_DIV, K_MOD, 
 * K_AND, K_OR, K_EQ, K_NEQ, 
 * K_LT, K_LTE, K_GT, K_GTE,
 * 
 * BRANCH: op1
 * JUMP: op1
 * RETURN: op1: return value
 * STORE: op1: point(var ), op2: offset, op3: var
 * LOAD: op1: point(var ), op2: offset, op3: target(var)
 * PARAM: op1: param name, op2: var name, op3: func name
 * CALL: op1: func name, op2: param num, op3: return value
*/
struct TAC{
    int type;
    int op;

}

#endif