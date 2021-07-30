#include "./SysY.symtab.calcConst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ENUM.h"
#include "../SysY.type/SysY.type.def.h"
#include "../utils/NullPointMacro.h"
#include "../utils/PrintHelper.h"

int calcConstArrayImpl(struct ArrayImpl *arrayimpl) {
    struct VarTabElem *elem = getVarTabElemByName(arrayimpl->ident->name, display);
    if (elem == NULL) {
        PrintErrExit("NOT FOUND VARIABLE %s \n", arrayimpl->ident->name);
    }
    if (elem->is_const == 0) {
        PrintErrExit("CALC CONST Arrray Impl VARIABLE MUST BE CONST\n");
    }
    if (elem->is_array == 0) {
        PrintErrExit("calcConstArrayImpl Only use to calc array impl calue");
    }
    struct ExpArrayDefs *head = arrayimpl->exparraydefs;
    struct ExpArrayDefs *exparraydefs = head;
    struct ArrayTabElem *array = elem->array_ref;
    EnsureNotNull(array);
    int offset = 0;
    do {
        struct ExpArrayDef *exparraydef = exparraydefs->exparraydef;
        int n = calcConstExp(exparraydef->exp);
        offset *= array->elem_num;
        offset += n;
        exparraydefs = exparraydefs->next;
        array = array->elem_ref;
    } while (head != exparraydefs && array != NULL);

    if (head == exparraydefs || array == NULL) {
        PrintErrExit("array impl is not complete");
    }

    return elem->const_init_value[offset];
}
int calcConstIdent(struct Ident *ident) {
    struct VarTabElem *elem = getVarTabElemByName(ident->name, display);

    if (elem == NULL) {
        PrintErrExit("NOT FOUND VARIABLE %s \n", ident->name);
    }
    if (elem->is_const == 0) {
        PrintErrExit("CALC CONST IDNET VARIABLE MUST BE CONST\n");
    }
    if (elem->is_array) {
        PrintErrExit("calcConstIdent Only use to calc single variable");
    }
    return *(elem->const_init_value);
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
    return 0;
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
    return 0;
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
    return 0;
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