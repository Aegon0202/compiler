#include "../SysY.AST/SysY.AST.new.h"
#include "../SysY.type/SysY.type.def.h"
#include "../ssa/ssa.h"
#include "../utils/LinearList.h"
#include "../utils/Malloc.h"
#include "./SysY.symtab.calcConst.h"
#include "./SysY.symtab.def.h"

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

#define BASIC_BLOCK_TYPE BasicBlock
#define IR_LIST_TYPE Ir
#define OPERAND_TYPE Operand
#define IR_TYPE Ir

//需要在SSA中完成的函数
/**
 * 生成一个新的基本块
 * 
 * :param (BASIC_BLOCK_TYPE*) predecessor 一个前驱，可能为NULL
 * 
 * :return (BASIC_BLOCK_TYPE*) 一个新生成的基本块
 */
BASIC_BLOCK_TYPE* newBasicBlock(BASIC_BLOCK_TYPE* predecessor);

/**
 * 设置一个基本块已封闭
 * 会在这个基本块的所有前驱都设置完的情况下调用
 * 
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本快
 * 
 * :return (void) 没有返回值
 */
void setBasicBlockSealed(BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct IntConst*）转化成一个合法的操作符
 * 
 * :param (struct IntConst*) int_const 一个输入，代表一个常数
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAIntConst(struct IntConst* int_const, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct String*）转化成一个合法的操作符
 * 
 * :param (struct String*) str 一个输入，代表一个字符串
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAString(struct String* str, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct VarTabElem*）转化成一个合法的操作符
 * 可能是作为输入，也可能是作为输出
 * 
 * :param (struct VarTabElem*) vte 一个输入，代表一个int变量
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAVarTabElem(struct VarTabElem* vte, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个偏移量（int）转成一个合法的代指其绝对地址的操作数。
 * 
 * :param (int) base 基址 取值为 FRAMEPOINT STACKPOINT GLOBALDATA
 * :param (int) offset 偏移量
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
 */
OPERAND_TYPE* toSSAOffset(int base, int offset, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（BASIC_BLOCK_TYPE*）转化成一个合法的操作符
 * 
 * :param (BASIC_BLOCK_TYPE*) target_block 一个输入，代表一个基本块，即跳转地址
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSABasicBlock(BASIC_BLOCK_TYPE* target_block, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct FuncTabElem*）转化成一个合法的操作符，作为函数名称。
 * 可用于 PARAM和CALL
 * 
 * :param (struct FuncTabElem*) fte 一个输入，函数名。
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAFuncName(struct FuncTabElem* fte, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct VarSymEntry*）转化成一个合法的操作符，作为函数中形参的名称。
 * 可用于 PARAM
 * vse 可能为NULL，作为可变参数用。按照从左向右传参。
 * 
 * :param (struct VarSymEntry*) vse 一个输入，函数形参名。
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAParamName(struct VarSymEntry* vse, BASIC_BLOCK_TYPE* basic_block);

/**
 * 返回一个可用于存放并读取数据的操作数。
 * 
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSATempVariable(BASIC_BLOCK_TYPE* basic_block);

/**
 * 创建一条ir
 * 
 * :param (int) op ir的操作指令
 * :param (OPERAND_TYPE*) op1,op2,op3 操作符
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将新创建的ir放到此基本块中
 * 
 * :return (IR_LIST_TYPE*) 一条ir指令
*/
IR_TYPE* newIR(int op, OPERAND_TYPE* op1, OPERAND_TYPE* op2, OPERAND_TYPE* op3, BASIC_BLOCK_TYPE* basic_block);

/**
 * 添加一条由predecessor指向successor的边
 * 
 * :param (BASIC_BLOCK_TYPE*) predecessor   前驱
 * :param (BASIC_BLOCK_TYPE*) successor     后继
*/
void addBasicBlockEdge(BASIC_BLOCK_TYPE* predecessor, BASIC_BLOCK_TYPE* successor);

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
    return head_array;
}

OPERAND_TYPE* toSSALValRead(struct LVal* lval, BASIC_BLOCK_TYPE* basic_block) {
    struct VarTabElem* elem;
    OPERAND_TYPE* operand = NULL;
    switch (lval->valuetype) {
        case IDENT:
            elem = getVarTabElemByName(lval->value.ident->name, display);
            if (elem->is_array) {
                switch (elem->level) {
                    case 0:
                        return toSSAOffset(GLOBALDATA, elem->offset, basic_block);
                    case 1:
                        newIR(LOAD, toSSAOffset(FRAMEPOINT, elem->offset, basic_block), toSSAIntConst(newIntConstAST(0), basic_block), operand, basic_block);
                        return operand;
                    default:
                        return toSSAOffset(FRAMEPOINT, elem->offset, basic_block);
                }
            } else {
                return toSSAVarTabElem(elem, basic_block);
            }
        case ARRAYIMPL:
            elem = getVarTabElemByName(lval->value.arrayimpl->ident->name, display);
            if (!elem->is_array) {
                PrintErrExit("only array variable use in array impl %s", lval->value.arrayimpl->ident->name);
            }

            break;
        default:
            break;
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
            PrintErrExit("primaryexp not support valurtype %s", EnumTypeToString(primaryexp->valuetype));
    }
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
            PrintErrExit("toSSAUnaryExp not support valurtype %s", EnumTypeToString(unaryexp->valuetype));
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

void VarDeclInit(struct VarDef* vardef, struct VarTabElem* elem, BASIC_BLOCK_TYPE* basic_block) {
    if (vardef->initval) {
        if (level == 1) {
            PrintErrExit("function paramentor not support init value");
        } else if (level == 0) {
            if (elem->is_array) {
            } else {
            }
        } else {
            if (elem->is_array) {
            } else {
                //toSSAExp()
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