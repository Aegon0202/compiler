#include "../SysY.type/SysY.type.def.h"
#include "../ssa/traverse.h"
#include "../utils/DequeList.h"
#include "../utils/Malloc.h"
#include "./SysY.symtab.calcConst.h"
#include "./SysY.symtab.def.h"
#include "./SysY.symtab.ssa.h"

static struct DequeList* break_target_queue;
static struct DequeList* continue_target_queue;

static int level = 0;
int global_offset = 0;
static int func_offset = 0;  // variable maybe offset of $fp
static int func_fparam_offset = 0;
static int max_func_offset = 0;
static int has_side_effect = 1;
struct ArrayTabElem* __offset_to_max_hight_dis(struct ArrayTabElem* array, int offset);

struct IntConst* getIntConstStatic(int value) {
    static struct IntConst int_const;
    int_const.type = INTCONST;
    int_const.value = value;
    return &int_const;
}

struct ArrayTabElem* toSSAConstArrayDefs(struct ConstArrayDefs* arraydefs) {
    IfNull(arraydefs, return 0;);
    struct ConstArrayDefs* head = arraydefs;
    struct ArrayTabElem* head_array = NULL;
    struct ArrayTabElem* prev_array = NULL;
    struct ArrayTabElem* array = NULL;
    struct LinearList* array_list = newLinearList();
    int list_next = 0;
    do {
        struct ConstArrayDef* arraydef = arraydefs->constarraydef;
        int num = calcConstConstExp(arraydef->constexp);
        array = newArrayTabElem(array_table);
        array->elem_num = num;
        if (level != 0) {
            array->elem_size_offset = func_offset - INT_SIZE;
            func_offset -= INT_SIZE;
        } else {
            array->elem_size_offset = global_offset;
            global_offset += INT_SIZE;
        }

        array->elem_ref = NULL;
        IfNull(head_array, head_array = array;);
        IfNotNull(prev_array, prev_array->elem_ref = array;);
        prev_array = array;

        IfNotNull(setLinearList(array_list, list_next, array), PrintErrExit("Must Be NULL"););
        list_next++;

        arraydefs = arraydefs->next;
    } while (head != arraydefs);

    array = getLinearList(array_list, list_next - 1);
    EnsureNotNull(array);
    array->elem_type = K_INT;
    array->elem_size = INT_SIZE;
    array->size = array->elem_num * array->elem_size;

    for (int n = list_next - 2; n >= 0; n--) {
        array = getLinearList(array_list, n);
        EnsureNotNull(array);
        array->elem_type = ARRAY;
        array->elem_size = array->elem_ref->size;
        array->size = array->elem_num * array->elem_size;
    }

    while (popLinearList(array_list) != NULL)
        ;
    freeLinearList(&array_list);

    return head_array;
}

OPERAND_TYPE* toSSAArrayImplAddress(struct ArrayImpl* arrayimpl, struct VarTabElem* vte, int* result_is_array, BASIC_BLOCK_TYPE* basic_block) {
    struct ExpArrayDefs* head = arrayimpl->exparraydefs;
    struct ExpArrayDefs* exparraydefs = arrayimpl->exparraydefs;
    struct ArrayTabElem* array = vte->array_ref;
    OPERAND_TYPE* array_address;
    switch (vte->level) {
        case 0:
            array_address = toSSAOffset(GLOBALDATA, (unsigned long long)vte, basic_block);
            break;
        case 1:
            array_address = toSSATempVariable(basic_block);
            newIR(LOAD, toSSAOffset(FRAMEPOINT, vte->offset, basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), array_address, basic_block);
            break;
        default:
            array_address = toSSAOffset(FRAMEPOINT, vte->offset, basic_block);
            break;
    }

    do {
        struct ExpArrayDef* exparraydef = exparraydefs->exparraydef;
        OPERAND_TYPE* num = toSSAExp(exparraydef->exp, basic_block);
        OPERAND_TYPE* t_op = toSSATempVariable(basic_block);
        OPERAND_TYPE* o_op = toSSATempVariable(basic_block);
        OPERAND_TYPE* size;
        if (vte->level == 1) {
            size = toSSATempVariable(basic_block);
            newIR(LOAD, toSSAOffset(FRAMEPOINT, array->elem_size_offset, basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), size, basic_block);
        } else {
            size = toSSAIntConst(getIntConstStatic(array->elem_size), basic_block);
        }

        newIR(K_MUL, size, num, t_op, basic_block);            // t_op = size*num
        newIR(K_ADD, array_address, t_op, o_op, basic_block);  // o_op = offset+t_op
        array_address = o_op;                                  // array_address = o_op

        exparraydefs = exparraydefs->next;
        array = array->elem_ref;
    } while (exparraydefs != head && array != NULL);

    *result_is_array = 0;
    if (exparraydefs == head && array != NULL) {
        *result_is_array = 1;
    }
    if (exparraydefs != head && array == NULL) {
        PrintErrExit("array impl dis more than declare");
    }

    return array_address;
}

OPERAND_TYPE* toSSALValRead(struct LVal* lval, BASIC_BLOCK_TYPE* basic_block) {
    struct VarTabElem* elem;
    OPERAND_TYPE* operand = NULL;
    OPERAND_TYPE* t_op;
    int is_array = 0;
    switch (lval->valuetype) {
        case IDENT:
            elem = getVarTabElemByName(lval->value.ident->name, display);
            if (elem->is_array) {
                switch (elem->level) {
                    case 0:
                        return toSSAOffset(GLOBALDATA, (unsigned long long)elem, basic_block);
                    case 1:
                        operand = toSSATempVariable(basic_block);
                        newIR(LOAD, toSSAOffset(FRAMEPOINT, elem->offset, basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), operand, basic_block);
                        return operand;
                    default:
                        return toSSAOffset(FRAMEPOINT, elem->offset, basic_block);
                }
            } else {
                if (elem->level == 0) {
                    has_side_effect = 1;
                    operand = toSSATempVariable(basic_block);
                    newIR(LOAD, toSSAOffset(GLOBALDATA, (long long)elem, basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), operand, basic_block);
                    return operand;
                } else {
                    return toSSAVarTabElemRead(elem, basic_block);
                }
            }
            PrintErrExit("unknown error happen");

        case ARRAYIMPL:
            elem = getVarTabElemByName(lval->value.arrayimpl->ident->name, display);
            if (!elem->is_array) {
                PrintErrExit("only array variable use in array impl %s", lval->value.arrayimpl->ident->name);
            }
            operand = toSSAArrayImplAddress(lval->value.arrayimpl, elem, &is_array, basic_block);
            if (is_array) {
                return operand;
            } else {
                if (elem->level == 0 || elem->level == 1) {
                    has_side_effect = 1;
                }
                t_op = toSSATempVariable(basic_block);
                newIR(LOAD, operand, toSSAIntConst(getIntConstStatic(0), basic_block), t_op, basic_block);
                return t_op;
            }
            PrintErrExit("unknown error happen");

        default:
            PrintErrExit("toSSALValRead not support valuetype %s", EnumTypeToString(lval->valuetype));
    }
    PrintErrExit("unknown error happen");
    return NULL;
}

OPERAND_TYPE* toSSANumber(struct Number* number, BASIC_BLOCK_TYPE* basic_block) {
    return toSSAIntConst(number->intconst, basic_block);
}

OPERAND_TYPE* toSSAPrimaryExp(struct PrimaryExp* primaryexp, BASIC_BLOCK_TYPE* basic_block) {
    switch (primaryexp->valuetype) {
        case EXP:
            return toSSAExp(primaryexp->value.exp, basic_block);
        case LVAL:
            return toSSALValRead(primaryexp->value.lval, basic_block);
        case NUMBER:
            return toSSANumber(primaryexp->value.number, basic_block);
        default:
            PrintErrExit("primaryexp not support valuetype %s", EnumTypeToString(primaryexp->valuetype));
    }
    PrintErrExit("unknown error happen");
    return NULL;
}

OPERAND_TYPE* toSSAFuncImpl(struct FuncImpl* funcimpl, BASIC_BLOCK_TYPE* basic_block) {
    struct FuncTabElem* fte = getFuncTabElemByName(funcimpl->ident->name, func_table);
    OPERAND_TYPE* func_op = toSSAFuncName(fte, basic_block);
    OPERAND_TYPE* result_op = toSSATempVariable(basic_block);

    if (fte->has_side_effect) {
        has_side_effect = 1;
    }

    struct LinearList* param_list = newLinearList();
    int param_next = 0;
    struct FuncRParams* frps = funcimpl->funcrparams;
    struct FuncRParams* head = funcimpl->funcrparams;

    do {
        struct FuncRParam* frp = frps->funcrparam;
        if (frp == NULL) {
            break;
        }
        OPERAND_TYPE* param_op = NULL;
        switch (frp->valuetype) {
            case STRING:
                param_op = toSSAString(frp->value.string, basic_block);
                break;
            case EXP:
                param_op = toSSAExp(frp->value.exp, basic_block);
                break;
            default:
                PrintErrExit("FuncRParam not support valuetype %s", EnumTypeToString(frp->valuetype));
        }
        IfNotNull(setLinearList(param_list, param_next, param_op), PrintErrExit("this should be NULL"););
        param_next++;
        frps = frps->next;
    } while (frps != head);
    for (int i = 0; i < param_next; i++) {
        OPERAND_TYPE* param_op = getLinearList(param_list, i);
        EnsureNotNull(param_op);
        newIR(PARAM, func_op, toSSAIntConst(getIntConstStatic(i), basic_block), param_op, basic_block);
    }
    newIR(CALL, func_op, toSSAIntConst(getIntConstStatic(param_next), basic_block), result_op, basic_block);
    return result_op;
}

OPERAND_TYPE* toSSAUnaryExps(struct UnaryExps* unaryexps, BASIC_BLOCK_TYPE* basic_block) {
    OPERAND_TYPE* op = toSSAUnaryExp(unaryexps->unaryexp, basic_block);
    OPERAND_TYPE* r_op = NULL;
    switch (unaryexps->unaryop->typevalue) {
        case K_ADD:
            r_op = op;
            break;
        case K_SUB:
            r_op = toSSATempVariable(basic_block);
            newIR(K_SUB, toSSAIntConst(getIntConstStatic(0), basic_block), op, r_op, basic_block);
            break;
        case K_NOT:
            r_op = toSSATempVariable(basic_block);
            newIR(K_NOT, op, NULL, r_op, basic_block);
            break;
        default:
            PrintErrExit("UnaryExps not support valuetype %s", EnumTypeToString(unaryexps->unaryop->typevalue));
    }
    return r_op;
}

OPERAND_TYPE* toSSAUnaryExp(struct UnaryExp* unaryexp, BASIC_BLOCK_TYPE* basic_block) {
    switch (unaryexp->valuetype) {
        case PRIMARYEXP:
            return toSSAPrimaryExp(unaryexp->value.primaryexp, basic_block);
        case FUNCIMPL:
            return toSSAFuncImpl(unaryexp->value.funcimpl, basic_block);
        case UNARYEXPS:
            return toSSAUnaryExps(unaryexp->value.unaryexps, basic_block);
        default:
            PrintErrExit("toSSAUnaryExp not support valuetype %s", EnumTypeToString(unaryexp->valuetype));
    }
    return NULL;
}

#define to_ssa_bi_exp_generator(exp_type, sub_type, sub_name, op_name)                   \
    OPERAND_TYPE* toSSA##exp_type(struct exp_type* exp, BASIC_BLOCK_TYPE* basic_block) { \
        struct exp_type* head = exp;                                                     \
        OPERAND_TYPE* op1 = NULL;                                                        \
        IfNull(exp, return NULL;);                                                       \
        do {                                                                             \
            OPERAND_TYPE* op2 = toSSA##sub_type(exp->sub_name, basic_block);             \
            if (exp->op_name) {                                                          \
                OPERAND_TYPE* op3 = toSSATempVariable(basic_block);                      \
                newIR(exp->op_name->typevalue, op1, op2, op3, basic_block);              \
                op1 = op3;                                                               \
            } else {                                                                     \
                op1 = op2;                                                               \
            }                                                                            \
            exp = exp->next;                                                             \
        } while (head != exp);                                                           \
        return op1;                                                                      \
    }

to_ssa_bi_exp_generator(MulExp, UnaryExp, unaryexp, mulop);
to_ssa_bi_exp_generator(AddExp, MulExp, mulexp, addop);
to_ssa_bi_exp_generator(RelExp, AddExp, addexp, relop);
to_ssa_bi_exp_generator(EqExp, RelExp, relexp, eqop);
#undef to_ssa_bi_exp_generator

OPERAND_TYPE* toSSAExp(struct Exp* exp, BASIC_BLOCK_TYPE* basic_block) {
    return toSSAAddExp(exp->addexp, basic_block);
}

void __var_def_array_init(struct InitVals* initvals, struct ArrayTabElem* array, BASIC_BLOCK_TYPE* basic_block, OPERAND_TYPE** buffer) {
    struct InitVals* head = initvals;
    int offset = 0;
    struct ArrayTabElem* t_array;
    do {
        struct InitVal* initval = initvals->initval;
        if (initval == NULL) {
            return;
        }
        switch (initval->valuetype) {
            case EXP:
                buffer[offset] = toSSAExp(initval->value.exp, basic_block);
                offset += 1;
                break;
            case INITVALS:
                t_array = __offset_to_max_hight_dis(array, offset);
                __var_def_array_init(initval->value.initvals, t_array, basic_block, buffer + offset);
                offset += t_array->elem_size / INT_SIZE;
                break;
            default:
                PrintErrExit("NOT SUPPORT ConstInitVal ValueType %s", EnumTypeToString(initval->valuetype));
        }
        initvals = initvals->next;
    } while (initvals != head);
}

void __global_def_array_init(struct InitVals* initvals, struct ArrayTabElem* array, int* buffer) {
    struct InitVals* head = initvals;
    int offset = 0;
    struct ArrayTabElem* t_array;
    do {
        struct InitVal* initval = initvals->initval;
        if (initval == NULL) {
            return;
        }
        switch (initval->valuetype) {
            case EXP:
                buffer[offset] = calcConstExp(initval->value.exp);
                offset += 1;
                break;
            case INITVALS:
                t_array = __offset_to_max_hight_dis(array, offset);
                __global_def_array_init(initval->value.initvals, t_array, buffer + offset);
                offset += t_array->elem_size / INT_SIZE;
                break;
            default:
                PrintErrExit("NOT SUPPORT ConstInitVal ValueType %s", EnumTypeToString(initval->valuetype));
        }
        initvals = initvals->next;
    } while (initvals != head);
}

void __var_def_init(struct VarDef* vardef, struct VarTabElem* elem, BASIC_BLOCK_TYPE* basic_block) {
    if (elem->level == 1) {
        PrintErrExit("function paramentor not support init value");
    } else if (elem->level == 0) {
        MALLOC_WITHOUT_DECLARE(elem->const_init_value, int, elem->size / INT_SIZE);
        if (elem->is_array) {
            if (vardef->initval->valuetype != INITVALS) {
                PrintErrExit("not support use const exp to init array: %s", elem->name);
            }
            __global_def_array_init(vardef->initval->value.initvals, elem->array_ref, elem->const_init_value);
        } else {
            if (vardef->initval->valuetype != EXP) {
                PrintErrExit("not support use const initvals to init variable: %s", elem->name);
            }
            *(elem->const_init_value) = calcConstExp(vardef->initval->value.exp);
        }
    } else {
        if (elem->is_array) {
            if (vardef->initval->valuetype != INITVALS) {
                PrintErrExit("not support use exp to init array: %s", elem->name);
            }
            int total_num = elem->size / INT_SIZE;
            MALLOC(operand_buffer, OPERAND_TYPE*, total_num);
            __var_def_array_init(vardef->initval->value.initvals, elem->array_ref, basic_block, operand_buffer);
            OPERAND_TYPE* base = toSSAOffset(FRAMEPOINT, 0, basic_block);
            int offset = elem->offset;
            for (int i = 0; i < total_num; i++) {
                if (operand_buffer[i] == NULL) {
                    newIR(STORE, base, toSSAIntConst(getIntConstStatic(offset), basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), basic_block);
                } else {
                    newIR(STORE, base, toSSAIntConst(getIntConstStatic(offset), basic_block), operand_buffer[i], basic_block);
                }
                offset += INT_SIZE;
            }
            free(operand_buffer);
        } else {
            if (vardef->initval->valuetype != EXP) {
                PrintErrExit("not support use initials to init variable: %s", elem->name);
            }
            OPERAND_TYPE* init_op = toSSAExp(vardef->initval->value.exp, basic_block);
            newIR(K_ADD, init_op, toSSAIntConst(getIntConstStatic(0), basic_block), toSSAVarTabElemWrite(elem, basic_block), basic_block);
        }
    }
}

int toSSAVarDecl(struct VarDecl* vardecl, BASIC_BLOCK_TYPE* basic_block) {
    IfNull(vardecl, return 0;);
    struct VarDefs* head = vardecl->vardefs;
    struct VarDefs* vardefs = head;
    do {
        struct VarDef* vardef = vardefs->vardef;
        struct VarTabElem* elem = newVarTabElem(vardef->ident->name, var_table);
        elem->level = level;
        elem->is_const = 0;
        elem->const_init_value = NULL;

        if (vardef->constarraydefs != NULL) {
            elem->is_array = 1;
            elem->type = ARRAY;
            elem->array_ref = toSSAConstArrayDefs(vardef->constarraydefs);
            elem->size = elem->array_ref->size;
        } else {
            elem->is_array = 0;
            elem->type = K_INT;
            elem->array_ref = NULL;
            elem->size = INT_SIZE;
        }

        if (level != 0) {
            if (elem->is_array) {
                elem->offset = func_offset - elem->size;
                func_offset -= elem->size;
            } else {
                elem->offset = INT32_MAX;
            }
        } else {
            elem->offset = global_offset;
            global_offset += elem->size;
        }

        struct BlockTabElem* block = getLastDisplay(display);
        elem->link = block->last;
        block->last = elem;
        block->size += elem->size;

        if (vardef->initval != NULL) {
            __var_def_init(vardef, elem, basic_block);
        }

        vardefs = vardefs->next;
    } while (vardefs != head);

    return 0;
}

struct ArrayTabElem* __offset_to_max_hight_dis(struct ArrayTabElem* array, int offset) {
    while (array != NULL) {
        if (offset % (array->elem_size / INT_SIZE) == 0) {
            return array;
        }
        array = array->elem_ref;
    }
    PrintErrExit("init const array failure");
    return NULL;
}

void __const_def_array_init(struct ConstInitVals* constinitvals, struct ArrayTabElem* array, int* buffer) {
    struct ConstInitVals* head = constinitvals;
    int offset = 0;
    struct ArrayTabElem* t_array;
    do {
        struct ConstInitVal* constinitval = constinitvals->constinitval;
        if (constinitval == NULL) {
            return;
        }
        switch (constinitval->valuetype) {
            case CONSTEXP:
                buffer[offset] = calcConstConstExp(constinitval->value.constexp);
                offset += 1;
                break;
            case CONSTINITVALS:
                t_array = __offset_to_max_hight_dis(array, offset);
                __const_def_array_init(constinitval->value.constinitvals, t_array, buffer + offset);
                offset += t_array->elem_size / INT_SIZE;
                break;
            default:
                PrintErrExit("NOT SUPPORT ConstInitVal ValueType %s", EnumTypeToString(constinitval->valuetype));
        }
        constinitvals = constinitvals->next;
    } while (constinitvals != head);
}

void __const_def_init(struct ConstDef* constdef, struct VarTabElem* elem, BASIC_BLOCK_TYPE* basic_block) {
    if (elem->level == 1) {
        PrintErrExit("function paramentor not support const value");
    }
    EnsureNotNull(constdef->constinitval);
    MALLOC_WITHOUT_DECLARE(elem->const_init_value, int, elem->size / INT_SIZE);
    if (elem->is_array) {
        if (constdef->constinitval->valuetype != CONSTINITVALS) {
            PrintErrExit("not support use const exp to init array: %s", elem->name);
        }
        __const_def_array_init(constdef->constinitval->value.constinitvals, elem->array_ref, elem->const_init_value);

    } else {
        if (constdef->constinitval->valuetype != CONSTEXP) {
            PrintErrExit("not support use const initvals to init variable: %s", elem->name);
        }
        *(elem->const_init_value) = calcConstConstExp(constdef->constinitval->value.constexp);
    }

    if (elem->level == 0) {
        return;
    }

    if (elem->is_array) {
        int total_num = elem->size / INT_SIZE;
        OPERAND_TYPE* base = toSSAOffset(FRAMEPOINT, 0, basic_block);
        int offset = elem->offset;
        for (int i = 0; i < total_num; i++) {
            newIR(STORE, base, toSSAIntConst(getIntConstStatic(offset), basic_block), toSSAIntConst(getIntConstStatic(elem->const_init_value[i]), basic_block), basic_block);
            offset += INT_SIZE;
        }
    } else {
        OPERAND_TYPE* init_op = toSSAVarTabElemWrite(elem, basic_block);
        MALLOC(int_c, struct IntConst, 1);
        int_c->type = INTCONST;
        int_c->value = 0;
        newIR(K_ADD, toSSAIntConst(getIntConstStatic(elem->const_init_value[0]), basic_block), toSSAIntConst(int_c, basic_block), init_op, basic_block);
        free(int_c);
    }
}

int toSSAConstDecl(struct ConstDecl* constdecl, BASIC_BLOCK_TYPE* basic_block) {
    IfNull(constdecl, return 0;);
    struct ConstDefs* head = constdecl->constdefs;
    struct ConstDefs* constdefs = head;
    do {
        struct ConstDef* constdef = constdefs->constdef;
        struct VarTabElem* elem = newVarTabElem(constdef->ident->name, var_table);
        elem->level = level;
        elem->is_const = 0;
        elem->const_init_value = NULL;

        if (constdef->constarraydefs != NULL) {
            elem->is_array = 1;
            elem->type = ARRAY;
            elem->array_ref = toSSAConstArrayDefs(constdef->constarraydefs);
            elem->size = elem->array_ref->size;
        } else {
            elem->is_array = 0;
            elem->type = K_INT;
            elem->array_ref = NULL;
            elem->size = INT_SIZE;
        }

        if (elem->level != 0) {
            if (elem->is_array) {
                elem->offset = func_offset - elem->size;
                func_offset -= elem->size;
            } else {
                elem->offset = INT32_MAX;
            }
        } else {
            elem->offset = global_offset;
            global_offset += elem->size;
        }

        struct BlockTabElem* block = getLastDisplay(display);
        elem->link = block->last;
        block->last = elem;
        block->size += elem->size;

        EnsureNotNull(constdef->constinitval);
        __const_def_init(constdef, elem, basic_block);
        elem->is_const = 1;

        constdefs = constdefs->next;
    } while (constdefs != head);

    return 0;
}

int toSSADecl(struct Decl* decl, BASIC_BLOCK_TYPE* basic_block) {
    IfNull(decl, return 0;);
    switch (decl->valuetype) {
        case VARDECL:
            return toSSAVarDecl(decl->value.vardecl, basic_block);
            break;
        case CONSTDECL:
            return toSSAConstDecl(decl->value.constdecl, basic_block);
            break;
        default:
            PrintErrExit("NOT SUPPORT Decl ValueType %s", EnumTypeToString(decl->valuetype));
            break;
    }
    return 0;
}

struct ArrayTabElem* toSSAExpArrayDefs(struct ExpArrayDefs* exparraydefs, BASIC_BLOCK_TYPE* basic_block) {
    struct ExpArrayDefs* head = exparraydefs;
    struct ArrayTabElem* head_array = NULL;
    struct ArrayTabElem* prev_array = NULL;
    struct ArrayTabElem* array = NULL;
    struct DequeList* array_size_op = newDequeList();
    struct DequeList* array_queue = newDequeList();

    do {
        struct ExpArrayDef* exparraydef = exparraydefs->exparraydef;
        array = newArrayTabElem(array_table);

        array->elem_num = 0;
        array->elem_size = 0;
        array->elem_type = ARRAY;
        array->elem_size_offset = func_offset - INT_SIZE;
        func_offset -= INT_SIZE;
        array->size = 0;

        array->elem_ref = NULL;
        IfNull(head_array, head_array = array;);
        IfNotNull(prev_array, prev_array->elem_ref = array;);
        prev_array = array;

        if (exparraydef->exp != NULL) {
            OPERAND_TYPE* num_op = toSSAExp(exparraydef->exp, basic_block);
            pushFrontDequeList(array_size_op, num_op);
        } else {
            pushFrontDequeList(array_size_op, toSSAIntConst(getIntConstStatic(INT_SIZE), basic_block));
        }
        pushFrontDequeList(array_queue, array);

        exparraydefs = exparraydefs->next;
    } while (exparraydefs != head);

    OPERAND_TYPE* elem_size_op = NULL;
    while (!isEmptyDequeList(array_size_op)) {
        array = popFrontDequeList(array_queue);
        if (elem_size_op == NULL) {
            OPERAND_TYPE* num_op = popBackDequeList(array_size_op);
            newIR(STORE, toSSAOffset(FRAMEPOINT, array->elem_size_offset, basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), num_op, basic_block);
            elem_size_op = num_op;
        } else {
            OPERAND_TYPE* num_op = popFrontDequeList(array_size_op);
            OPERAND_TYPE* new_size = toSSATempVariable(basic_block);
            newIR(K_MUL, elem_size_op, num_op, new_size, basic_block);
            newIR(STORE, toSSAOffset(FRAMEPOINT, array->elem_size_offset, basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), new_size, basic_block);
            elem_size_op = new_size;
        }
    }
    freeDequeList(&array_size_op);
    freeDequeList(&array_queue);

    return head_array;
}

void toSSALValWrite(struct LVal* lval, OPERAND_TYPE* result, BASIC_BLOCK_TYPE* basic_block) {
    struct VarTabElem* elem;
    OPERAND_TYPE* operand = NULL;
    OPERAND_TYPE* t_op;
    int is_array;
    switch (lval->valuetype) {
        case IDENT:
            elem = getVarTabElemByName(lval->value.ident->name, display);
            if (elem->is_array) {
                PrintErrExit("not support assign to array");
            }
            if (elem->level == 0) {
                has_side_effect = 1;

                t_op = toSSAOffset(GLOBALDATA, (unsigned long long)elem, basic_block);
                newIR(STORE, t_op, toSSAIntConst(getIntConstStatic(0), basic_block), result, basic_block);
            } else {
                t_op = toSSAVarTabElemWrite(elem, basic_block);
                newIR(K_ADD, result, toSSAIntConst(getIntConstStatic(0), basic_block), t_op, basic_block);
            }
            break;
        case ARRAYIMPL:
            elem = getVarTabElemByName(lval->value.arrayimpl->ident->name, display);
            if (!elem->is_array) {
                PrintErrExit("only array variable use in array impl %s", lval->value.arrayimpl->ident->name);
            }
            operand = toSSAArrayImplAddress(lval->value.arrayimpl, elem, &is_array, basic_block);
            if (is_array) {
                PrintErrExit("not support assign to array");
            }

            if (elem->level == 0 || elem->level == 1) {
                has_side_effect = 1;
            }

            newIR(STORE, operand, toSSAIntConst(getIntConstStatic(0), basic_block), result, basic_block);
            break;
        default:
            PrintErrExit("toSSALValWrite not support valuetype %s", EnumTypeToString(lval->valuetype));
    }
    return;
}

void toSSAAssign(struct Assign* assign, BASIC_BLOCK_TYPE* basic_block) {
    OPERAND_TYPE* result = toSSAExp(assign->exp, basic_block);
    toSSALValWrite(assign->lval, result, basic_block);
}

void toSSALAndExp(struct LAndExp* landexp, BASIC_BLOCK_TYPE* true_block, BASIC_BLOCK_TYPE* false_block, BASIC_BLOCK_TYPE* basic_block) {
    struct LAndExp* head = landexp;
    do {
        OPERAND_TYPE* cond = toSSAEqExp(landexp->eqexp, basic_block);
        if (landexp->next != head) {
            BASIC_BLOCK_TYPE* new_block = newBasicBlock(NULL);
            newIR(BRANCH, cond, toSSABasicBlock(new_block, basic_block), toSSABasicBlock(false_block, basic_block), basic_block);
            addBasicBlockEdge(basic_block, new_block);
            addBasicBlockEdge(basic_block, false_block);

            basic_block = new_block;
        } else {
            newIR(BRANCH, cond, toSSABasicBlock(true_block, basic_block), toSSABasicBlock(false_block, basic_block), basic_block);
            addBasicBlockEdge(basic_block, true_block);
            addBasicBlockEdge(basic_block, false_block);
        }
        landexp = landexp->next;
    } while (landexp != head);
}

void toSSALOrExp(struct LOrExp* lorexp, BASIC_BLOCK_TYPE* true_block, BASIC_BLOCK_TYPE* false_block, BASIC_BLOCK_TYPE* basic_block) {
    struct LOrExp* head = lorexp;
    do {
        if (lorexp->next != head) {
            BASIC_BLOCK_TYPE* new_block = newBasicBlock(NULL);
            toSSALAndExp(lorexp->landexp, true_block, new_block, basic_block);

            basic_block = new_block;
        } else {
            toSSALAndExp(lorexp->landexp, true_block, false_block, basic_block);
        }
        lorexp = lorexp->next;
    } while (lorexp != head);
    return;
}
void toSSACond(struct Cond* cond, BASIC_BLOCK_TYPE* true_block, BASIC_BLOCK_TYPE* false_block, BASIC_BLOCK_TYPE* basic_block) {
    toSSALOrExp(cond->lorexp, true_block, false_block, basic_block);
}

int toSSAIfStmt(struct IfStmt* ifstmt, BASIC_BLOCK_TYPE** basic_block_p) {
    BASIC_BLOCK_TYPE* true_block = newBasicBlock(NULL);
    BASIC_BLOCK_TYPE* false_block = newBasicBlock(NULL);
    BASIC_BLOCK_TYPE* merge_block = newBasicBlock(NULL);
    BASIC_BLOCK_TYPE* cond_block = *basic_block_p;
    toSSACond(ifstmt->cond, true_block, false_block, cond_block);
    int has_next = 0;

    if (toSSAStmt(ifstmt->then, &true_block) == 0) {
        has_next = 1;
        newIR(JUMP, NULL, NULL, toSSABasicBlock(merge_block, true_block), true_block);
        addBasicBlockEdge(true_block, merge_block);
    }
    if (toSSAStmt(ifstmt->otherwise, &false_block) == 0) {
        has_next = 1;
        newIR(JUMP, NULL, NULL, toSSABasicBlock(merge_block, false_block), false_block);
        addBasicBlockEdge(false_block, merge_block);
    }

    if (has_next) {
        *basic_block_p = merge_block;
        return 0;
    } else {
        *basic_block_p = NULL;
        // need free merge block
        return 1;
    }
}

int toSSAWhileStmt(struct WhileStmt* whilestmt, BASIC_BLOCK_TYPE** basic_block_p) {
    BASIC_BLOCK_TYPE* cond_block = *basic_block_p;
    BASIC_BLOCK_TYPE* loop_block = newBasicBlock(NULL);
    BASIC_BLOCK_TYPE* merge_block = newBasicBlock(NULL);
    BASIC_BLOCK_TYPE* loop_head = loop_block;

    toSSACond(whilestmt->cond, loop_block, merge_block, cond_block);

    cond_block = newBasicBlock(NULL);
    pushFrontDequeList(break_target_queue, merge_block);
    pushFrontDequeList(continue_target_queue, cond_block);

    if (toSSAStmt(whilestmt->stmt, &loop_block) == 0) {
        newIR(JUMP, NULL, NULL, toSSABasicBlock(cond_block, loop_block), loop_block);
        addBasicBlockEdge(loop_block, cond_block);
        toSSACond(whilestmt->cond, loop_head, merge_block, cond_block);
    } else if (getPredecessorNum(cond_block) > 0) {
        toSSACond(whilestmt->cond, loop_head, merge_block, cond_block);
    } else {
        // need free cond block
    }

    popFrontDequeList(break_target_queue);
    popFrontDequeList(continue_target_queue);

    *basic_block_p = merge_block;
    return 0;
}

void toSSAReturnStmt(struct ReturnStmt* returnstmt, BASIC_BLOCK_TYPE* basic_block) {
    newIR(RETURNSTMT, returnstmt->exp == NULL ? NULL : toSSAExp(returnstmt->exp, basic_block), NULL, NULL, basic_block);
}

void toSSABreakStmt(BASIC_BLOCK_TYPE* basic_block) {
    BASIC_BLOCK_TYPE* target_block = getFrontDequeList(break_target_queue);
    EnsureNotNull(target_block);
    newIR(JUMP, NULL, NULL, toSSABasicBlock(target_block, basic_block), basic_block);
    addBasicBlockEdge(basic_block, target_block);
}

void toSSAContinueStmt(BASIC_BLOCK_TYPE* basic_block) {
    BASIC_BLOCK_TYPE* target_block = getFrontDequeList(continue_target_queue);
    EnsureNotNull(target_block);
    newIR(JUMP, NULL, NULL, toSSABasicBlock(target_block, basic_block), basic_block);
    addBasicBlockEdge(basic_block, target_block);
}

// 0: continue, 1: stop this block
int toSSAStmt(struct Stmt* stmt, BASIC_BLOCK_TYPE** basic_block_p) {
    IfNull(stmt, return 0;);
    switch (stmt->valuetype) {
        case ASSIGN:
            toSSAAssign(stmt->value.assign, *basic_block_p);
            return 0;
        case BLOCK:
            return toSSABlock(stmt->value.block, basic_block_p);
        case IFSTMT:
            return toSSAIfStmt(stmt->value.ifstmt, basic_block_p);
        case WHILESTMT:
            return toSSAWhileStmt(stmt->value.whilestmt, basic_block_p);
        case EXP:
            toSSAExp(stmt->value.exp, *basic_block_p);
            return 0;
        case RETURNSTMT:
            toSSAReturnStmt(stmt->value.returnstmt, *basic_block_p);
            return 1;
        case BREAKSTMT:
            toSSABreakStmt(*basic_block_p);
            return 1;
        case CONTINUESTMT:
            toSSAContinueStmt(*basic_block_p);
            return 1;
        default:
            PrintErrExit("NOT SUPPORT BlockItem ValueType %s", EnumTypeToString(stmt->valuetype));
    }
    PrintErrExit("unknown error happen");
    return 0;
}

// 0: continue, 1: stop this block( break continue return )
int toSSABlock(struct Block* block, BASIC_BLOCK_TYPE** basic_block_p) {
    level++;
    appendDisplay(newBlockTabElem(getLastDisplay(display), block_table), display);
    int old_func_offset = func_offset;
    struct BlockItems* head = block->blockitems;
    struct BlockItems* blockitems = block->blockitems;
    IfNull(head, {
        level--;
        removeLastDisplay(display);
        return 0;
    });
    do {
        struct BlockItem* blockitem = blockitems->blockitem;
        switch (blockitem->valuetype) {
            case DECL:
                toSSADecl(blockitem->value.decl, *basic_block_p);
                break;
            case STMT:
                if (toSSAStmt(blockitem->value.stmt, basic_block_p)) {
                    max_func_offset = func_offset < max_func_offset ? func_offset : max_func_offset;
                    func_offset = old_func_offset;
                    level--;
                    removeLastDisplay(display);
                    return 1;
                }
                break;
            default:
                PrintErrExit("NOT SUPPORT BlockItem ValueType %s", EnumTypeToString(blockitem->valuetype));
        }
        blockitems = blockitems->next;
    } while (blockitems != head);

    max_func_offset = func_offset < max_func_offset ? func_offset : max_func_offset;
    func_offset = old_func_offset;
    level--;
    removeLastDisplay(display);
    return 0;
}

void toSSAFuncDef(struct FuncDef* funcdef) {
    struct FuncTabElem* fte = newFuncTabElem(funcdef->ident->name, func_table);
    func_offset = -32;       // 32:
    func_fparam_offset = 4;  // $fp, $lr
    max_func_offset = 0;
    has_side_effect = 0;
    printf("start parse func:%s\n", fte->name);
    struct FuncFParams* f_head = funcdef->funcfparams;
    struct FuncFParams* funcfparams = f_head;

    BASIC_BLOCK_TYPE* basic_block = newBasicBlock(NULL);
    fte->blocks = basic_block;
    fte->return_type = funcdef->functype->typevalue;

    level++;
    appendDisplay(newBlockTabElem(getLastDisplay(display), block_table), display);
    do {
        struct FuncFParam* funcfparam = funcfparams->funcfparam;
        if (funcfparam == NULL) {
            break;
        }
        struct VarTabElem* elem = newVarTabElem(funcfparam->ident->name, var_table);
        elem->size = INT_SIZE;
        elem->level = level;
        elem->const_init_value = NULL;
        elem->is_const = 0;

        if (funcfparam->exparraydefs != NULL) {
            elem->is_array = 1;
            elem->type = ARRAY;
            elem->array_ref = toSSAExpArrayDefs(funcfparam->exparraydefs, basic_block);
        } else {
            elem->is_array = 0;
            elem->type = K_INT;
            elem->array_ref = NULL;
        }

        elem->offset = func_fparam_offset;
        func_fparam_offset += INT_SIZE;

        struct BlockTabElem* block = getLastDisplay(display);
        elem->link = block->last;
        block->last = elem;
        block->size += elem->size;

        fte->parameters_num++;
        elem->link = fte->parameters_ref;
        fte->parameters_size += elem->size;
        fte->parameters_ref = elem;

        OPERAND_TYPE* init_param = toSSAVarTabElemWrite(elem, basic_block);
        newIR(LOAD, toSSAOffset(FRAMEPOINT, elem->offset, basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), init_param, basic_block);

        funcfparams = funcfparams->next;
    } while (funcfparams != f_head);

    toSSABlock(funcdef->block, &basic_block);
    if (basic_block != NULL) {
        newIR(RETURNSTMT, NULL, NULL, NULL, basic_block);
    }

    fte->var_offset_end = max_func_offset;
    fte->has_side_effect = has_side_effect;
    level--;
    removeLastDisplay(display);
}

void toSSACompUnit(struct CompUnit* cp) {
    IfNull(cp, return;);
    struct CompUnit* head = cp;
    level = 0;
    static int need_init_flag = 1;
    if (need_init_flag) {
        need_init_flag = 0;
        break_target_queue = newDequeList();
        continue_target_queue = newDequeList();
    }
    do {
        switch (cp->valuetype) {
            case FUNCDEF:
                toSSAFuncDef(cp->value.funcdef);
                break;
            case DECL:
                toSSADecl(cp->value.decl, NULL);
                break;
            default:
                PrintErrExit("NOT SUPPORT CompUnit ValueType %s", EnumTypeToString(cp->valuetype));
        }
        cp = cp->next;
    } while (cp != head);
    return;
}

void __avoid_operand_double_free(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct LinearList* operand_used = (struct LinearList*)args;
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = list_next(head);
    while (head != next) {
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
#define __avoid(num)                                                              \
    if (ir->op##num != NULL) {                                                    \
        OPERAND_TYPE* used_op = getLinearList(operand_used, (size_t)ir->op##num); \
        if (used_op == NULL) {                                                    \
            setLinearList(operand_used, (size_t)ir->op##num, ir->op##num);        \
        } else {                                                                  \
            OPERAND_TYPE* new_op = operand_dup(ir->op##num);                      \
            ir->op##num = new_op;                                                 \
            setLinearList(operand_used, (size_t)new_op, new_op);                  \
        }                                                                         \
    }
        __avoid(1);
        __avoid(2);
        __avoid(3);
#undef __avoid
        next = list_next(next);
    }
}

void avoidOperandDoubleFree() {
    struct LinearList* operand_used = newLinearList();
    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks) {
            deepTraverseSuccessorsBasicBlock(elem->blocks, __avoid_operand_double_free, operand_used);
        }
    }
    while (popLinearList(operand_used) != NULL)
        ;
    freeLinearList(&operand_used);
}