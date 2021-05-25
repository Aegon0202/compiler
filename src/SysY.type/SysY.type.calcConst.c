#include "SysY.type.calcConst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../SysY.AST/SysY.AST.def.h"
#include "../parser/SysY.tab.h"
#include "../utils/NullPointMacro.h"
#include "../utils/PrintHelper.h"
#include "SysY.type.def.h"
#include "SysY.type.symtab.h"

int calcConstArrayImpl(struct ArrayImpl *array) {
    struct VarSymEntry *vse = findVarInTable(array->ident->name);
    if (vse == NULL) {
        PrintErrExit("NOT FOUND VARIABLE %s \n", array->ident->name);
    }
    if (vse->is_const == 0) {
        PrintErrExit("CALC CONST ARRAY IMPL MUST BE CONST\n");
    }
    struct ExpArrayDefs *head = array->exparraydefs;
    struct ExpArrayDefs *exparraydefs = array->exparraydefs;
    int offset_array = 1;
    int r = 0;
    int i;
    for (i = vse->array_dimensional_num; i && exparraydefs != head; i--, exparraydefs = exparraydefs->next) {
        r += calcConstExp(exparraydefs->exparraydef->exp) * offset_array;
        offset_array *= calcConstOperand(vse->array_shape[i]);
    }
    if (i) {
        PrintErrExit("CALC CONST ARRAY IMPL AST MUST BE COMPELELY IMPLETMENT\n");
    }
    return calcConstOperand(vse->initval[r]);
}

int calcConstArrayImplAST(struct ArrayImplAST *array) {
    if (array->array_impl_size != array->array_varsymentry->array_dimensional_num) {
        PrintErrExit("CALC CONST ARRAY IMPL AST MUST BE COMPELELY IMPLETMENT\n");
    }
    int offset_array = 1;
    int r = 0;
    for (int i = array->array_impl_size; i; i--) {
        r += calcConstOperand(array->array_impl[i - 1]) * offset_array;
        offset_array *= calcConstOperand(array->array_varsymentry->array_shape[i]);
    }
    return calcConstOperand(array->array_varsymentry->initval[r]);
}

int calcConstOperand(struct Operand *operand) {
    struct VarSymEntry *vse;
    switch (operand->valuetype) {
        case INTCONST:
            return operand->value.intconst->value;
        case VARSYMENTRY:
            vse = operand->value.variable;
            if (vse->is_const == 0) {
                PrintErrExit("CALC CONST IDNET VARIABLE MUST BE CONST\n");
            }
            return calcConstOperand((vse->initval)[0]);
        case ARRAYIMPLAST:
            return calcConstArrayImplAST(operand->value.array);
        case EXPAST:
            return calcConstExpAST(operand->value.exp);
        default:
            PrintErrExit("CALCED CONST OPERAND IS NOT BE A UNKNOWN TYPE\n");
    }
}

int calcConstExpAST(struct ExpAST *exp) {
    switch (exp->op) {
        case K_ADD:
            return calcConstOperand(exp->op1) + calcConstOperand(exp->op2);
        case K_SUB:
            return calcConstOperand(exp->op1) - calcConstOperand(exp->op2);
        case K_MUL:
            return calcConstOperand(exp->op1) * calcConstOperand(exp->op2);
        case K_DIV:
            return calcConstOperand(exp->op1) / calcConstOperand(exp->op2);
        case K_MOD:
            return calcConstOperand(exp->op1) % calcConstOperand(exp->op2);
        case K_AND:
            return calcConstOperand(exp->op1) && calcConstOperand(exp->op2);
        case K_OR:
            return calcConstOperand(exp->op1) || calcConstOperand(exp->op2);
        case K_NOT:
            return !calcConstOperand(exp->op1);
        case K_EQ:
            return calcConstOperand(exp->op1) == calcConstOperand(exp->op2);
        case K_NEQ:
            return calcConstOperand(exp->op1) != calcConstOperand(exp->op2);
        case K_GT:
            return calcConstOperand(exp->op1) > calcConstOperand(exp->op2);
        case K_GTE:
            return calcConstOperand(exp->op1) >= calcConstOperand(exp->op2);
        case K_LT:
            return calcConstOperand(exp->op1) < calcConstOperand(exp->op2);
        case K_LTE:
            return calcConstOperand(exp->op1) <= calcConstOperand(exp->op2);
        case INTCONST:
            return calcConstOperand(exp->op1);
        default:
            PrintErrExit("CALCED CONST EXP AST IS NOT BE A UNKNOWN TYPE\n");
    }
}

int calcConstIdent(struct Ident *ident) {
    struct VarSymEntry *vse = findVarInTable(ident->name);
    if (vse == NULL) {
        PrintErrExit("NOT FOUND VARIABLE %s \n", ident->name);
    }
    if (vse->is_const == 0) {
        PrintErrExit("CALC CONST IDNET VARIABLE MUST BE CONST\n");
    }

    return calcConstOperand((vse->initval)[0]);
}

int calcConstLVal(struct LVal *lval) {
    switch (lval->valuetype) {
        case IDENT:
            return calcConstIdent(lval->value.ident);
        case ARRAYIMPL:
            return calcConstArrayImpl(lval->value.arrayimpl);
        default:
            PrintErrExit("CALCED CONST LVAL IS NOT BE A UNKNOWN TYPE\n");
    }
}

int calcConstExp(struct Exp *exp) {
    return calcConstAddExp(exp->addexp);
}

int calcConstNumber(struct Number *number) {
    return number->intconst->value;
}

int calcConstPrimaryExp(struct PrimaryExp *primaryexp) {
    switch (primaryexp->valuetype) {
        case EXP:
            return calcConstExp(primaryexp->value.exp);
        case LVAL:
            return calcConstLVal(primaryexp->value.lval);
        case NUMBER:
            return calcConstNumber(primaryexp->value.number);
        default:
            PrintErrExit("CALCED CONST PRIMARY EXP IS NOT BE A UNKNOWN TYPE\n");
    }
}

int calcConstUnaryExps(struct UnaryExps *unaryexps) {
    int r = calcConstUnaryExp(unaryexps->unaryexp);
    if (unaryexps->unaryop != NULL) {
        switch (unaryexps->unaryop->typevalue) {
            case K_ADD:
                break;
            case K_SUB:
                r = -r;
                break;
            case K_NOT:
                r = !r;
                break;
            default:
                PrintErrExit("CALCED CONST ADD OP IS NOT BE A UNKNOWN TYPE\n");
        }
    }
    return r;
}

int calcConstUnaryExp(struct UnaryExp *unaryexp) {
    switch (unaryexp->valuetype) {
        case PRIMARYEXP:
            return calcConstPrimaryExp(unaryexp->value.primaryexp);
        case FUNCIMPL:
            PrintErrExit("CALCED CONST UNARY EXP IS NOT BE A FUNCTION IMPLEMENT\n");
        case UNARYEXPS:
            return calcConstUnaryExps(unaryexp->value.unaryexps);
        default:
            PrintErrExit("CALCED CONST UNARY EXP IS NOT BE A UNKNOWN TYPE\n");
    }
}

int calcConstMulExp(struct MulExp *mulexp) {
    struct MulExp *head = mulexp;
    int result = 1;
    do {
        int t = calcConstUnaryExp(mulexp->unaryexp);
        if (mulexp->mulop != NULL) {
            switch (mulexp->mulop->typevalue) {
                case K_MUL:
                    result *= t;
                    break;
                case K_DIV:
                    result /= t;
                    break;
                case K_MOD:
                    result %= t;
                    break;
                default:
                    PrintErrExit("CALCED CONST MUL OP IS NOT BE A UNKNOWN TYPE\n");
            }
        } else {
            result *= t;
        }
        mulexp = mulexp->next;
    } while (mulexp != head);
    return result;
}

int calcConstAddExp(struct AddExp *addexp) {
    struct AddExp *head = addexp;
    int result = 0;
    do {
        int t = calcConstMulExp(addexp->mulexp);
        if (addexp->addop != NULL) {
            switch (addexp->addop->typevalue) {
                case K_SUB:
                    result -= t;
                    break;
                case K_ADD:
                    result += t;
                    break;
                default:
                    PrintErrExit("CALCED CONST ADD OP IS NOT BE A UNKNOWN TYPE\n");
            }
        } else {
            result += t;
        }

        addexp = addexp->next;
    } while (addexp != head);
    return result;
}

int calcConstConstExp(struct ConstExp *const_exp) {
    return calcConstAddExp(const_exp->addexp);
}