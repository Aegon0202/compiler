//#include "../SysY.AST/SysY.AST.new.h"
#include "../SysY.type/SysY.type.def.h"
// #include "../ssa/ssa.h"
#include "../utils/LinearList.h"
#include "../utils/Malloc.h"
#include "./SysY.symtab.calcConst.h"
#include "./SysY.symtab.def.h"
#include "./tmp_declara.h"

#define INT_SIZE 4

static struct LinkedTable* id_block_table;
static struct LinkedTable* id_tail_block_table;
static int block_id_next = 0;

static struct LinkedTable* while_break_table;
static struct LinkedTable* while_continue_table;
static int while_level = 0;

static struct LinkedTable* cond_true_table;
static struct LinkedTable* cond_false_table;
static int cond_level = 0;

static int level = 0;
static int global_offset = 0;
static int func_offset = 0;  // maybe offset of $fp
static struct FuncTabElem* producing_func;

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
            array_address = toSSAOffset(GLOBALDATA, vte->offset, basic_block);
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
    int is_array;
    switch (lval->valuetype) {
        case IDENT:
            elem = getVarTabElemByName(lval->value.ident->name, display);
            if (elem->is_array) {
                switch (elem->level) {
                    case 0:
                        return toSSAOffset(GLOBALDATA, elem->offset, basic_block);
                    case 1:
                        newIR(LOAD, toSSAOffset(FRAMEPOINT, elem->offset, basic_block), toSSAIntConst(getIntConstStatic(0), basic_block), operand, basic_block);
                        return operand;
                    default:
                        return toSSAOffset(FRAMEPOINT, elem->offset, basic_block);
                }
            } else {
                return toSSAVarTabElem(elem, basic_block);
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
                t_op = toSSATempVariable(basic_block);
                newIR(LOAD, operand, toSSAIntConst(getIntConstStatic(0), basic_block), t_op, basic_block);
                return t_op;
            }
            PrintErrExit("unknown error happen");

        default:
            PrintErrExit("toSSALValRead not support valuetype %s", EnumTypeToString(lval->valuetype));
    }
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
}

OPERAND_TYPE* toSSAFuncImpl(struct FuncImpl* funcimpl, BASIC_BLOCK_TYPE* basic_block) {
    struct FuncTabElem* fte = getFuncTabElemByName(funcimpl->ident->name, func_table);
    struct LinearList i;
    return NULL;
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

#define to_ssa_bi_exp_generator(exp_type, sub_type, sub_name, op)                        \
    OPERAND_TYPE* toSSA##exp_type(struct exp_type* exp, BASIC_BLOCK_TYPE* basic_block) { \
        struct exp_type* head = exp;                                                     \
        OPERAND_TYPE* op1 = NULL;                                                        \
        do {                                                                             \
            OPERAND_TYPE* op2 = toSSA##sub_type(exp->sub_name, basic_block);             \
            if (op1 != NULL) {                                                           \
                OPERAND_TYPE* op3 = toSSATempVariable(basic_block);                      \
                newIR(op, op1, op2, op3, basic_block);                                   \
                op1 = op3;                                                               \
            } else {                                                                     \
                op1 = op2;                                                               \
            }                                                                            \
            exp = exp->next;                                                             \
        } while (head != exp);                                                           \
        return op1;                                                                      \
    }
to_ssa_bi_exp_generator(LAndExp, EqExp, eqexp, K_AND);
to_ssa_bi_exp_generator(LOrExp, LAndExp, landexp, K_OR);

#undef to_ssa_bi_exp_generator

OPERAND_TYPE* toSSAExp(struct Exp* exp, BASIC_BLOCK_TYPE* basic_block) {
    return toSSAAddExp(exp->addexp, basic_block);
}

void __var_decl_init_helper(struct InitVal* initval, struct VarTabElem* elem, BASIC_BLOCK_TYPE* basic_block) {
    if (initval->valuetype != INITVALS) {
        PrintErrExit("this function only support initvals");
    }
}

void VarDeclInit(struct VarDef* vardef, struct VarTabElem* elem, BASIC_BLOCK_TYPE* basic_block) {
    if (vardef->initval != NULL) {
        if (elem->level == 1) {
            PrintErrExit("function paramentor not support init value");
        } else if (elem->level == 0) {
            //??? global init
            if (elem->is_array) {
            } else {
            }
        } else {
            if (elem->is_array) {
            } else {
                if (vardef->initval->valuetype != EXP) {
                    PrintErrExit("not support use initials to init variable: %s", elem->name);
                }
                OPERAND_TYPE* init_op = toSSAExp(vardef->initval->value.exp, basic_block);
                newIR(K_ADD, init_op, toSSAIntConst((0), basic_block), toSSAVarTabElem(elem, basic_block), basic_block);
            }
        }
    }
}

int toSSAVarDecl(struct VarDecl* vardecl, BASIC_BLOCK_TYPE* basic_block) {
    IfNull(vardecl, return 0;);
    int type_value = vardecl->btype->typevalue;
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
            elem->offset = func_offset - elem->size;
            func_offset -= elem->size;
        } else {
            elem->offset = global_offset;
            global_offset += elem->size;
        }

        struct BlockTabElem* block = getLastDisplay(display);
        elem->link = block->last;
        block->last = elem->link;
        block->size += elem->size;

        VarDeclInit(vardef, elem, basic_block);

        vardefs = vardefs->next;
    } while (vardefs != head);
}

int toSSADecl(struct Decl* decl) {
    IfNull(decl, return 0;);
    switch (decl->valuetype) {
        case VARDECL:
            return toSSAVarDecl(decl->value.vardecl, NULL);
            break;
        case CONSTDECL:
            return toSSAConstDecl(decl->value.constdecl);
            break;
        default:
            PrintErrExit("NOT SUPPORT Decl ValueType %s", EnumTypeToString(decl->valuetype));
            break;
    }
    return 0;
}

void toSSACompUnit(struct CompUnit* cp) {
    IfNull(cp, return;);
    struct CompUnit* head = cp;
    level = 0;
    do {
        switch (cp->valuetype) {
            case FUNCDEF:
                toSSAFuncDef(cp->value.funcdef);
                break;
            case DECL:
                toSSADecl(cp->value.decl);
                break;
            default:
                PrintErrExit("NOT SUPPORT CompUnit ValueType %s", EnumTypeToString(cp->valuetype));
        }
        if (cp->valuetype == FUNCDEF) {
        } else if (cp)
            cp = cp->next;
    } while (cp != head);
    return;
}