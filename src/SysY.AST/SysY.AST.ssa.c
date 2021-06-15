
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "./SysY.AST.ssa.h"

#include "./SysY.AST.new.h"
struct LinkedTable* func_block_table;

static struct LinkedTable* id_block_table;
static struct LinkedTable* id_tail_block_table;
static int block_id_next = 0;

static struct LinkedTable* while_break_table;
static struct LinkedTable* while_continue_table;
static int while_level = 0;

static struct LinkedTable* cond_true_table;
static struct LinkedTable* cond_false_table;
static int cond_level = 0;

BASIC_BLOCK_TYPE* newBasicBlock(BASIC_BLOCK_TYPE* predecessor);
void setBasicBlockSealed(BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAIntConst(struct IntConst* int_const, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAString(struct String* str, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAVarSymEntry(struct VarSymEntry* vse, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSABasicBlock(BASIC_BLOCK_TYPE* target_block, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAFuncName(struct FuncSymEntry* fse, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSAParamName(struct VarSymEntry* vse, BASIC_BLOCK_TYPE* basic_block);
OPERAND_TYPE* toSSATempVariable(BASIC_BLOCK_TYPE* basic_block);
IR_TYPE* newIR(int op, OPERAND_TYPE* op1, OPERAND_TYPE* op2, OPERAND_TYPE* op3, BASIC_BLOCK_TYPE* basic_block);
void addBasicBlockEdge(BASIC_BLOCK_TYPE* predecessor, BASIC_BLOCK_TYPE* successor);

OPERAND_TYPE* toSSAFuncImplAST(struct FuncImplAST* func_impl, int block_id) {
    BASIC_BLOCK_TYPE* block = getLinkedTable(id_block_table, &block_id);
    OPERAND_TYPE* func = toSSAFuncName(func_impl->function, block);
    OPERAND_TYPE* param = NULL;
    for (int i = 0; i < func_impl->param_num; i++) {
        if (i < func_impl->function->funcparamnum) {
            param = toSSAParamName(func_impl->function->funcfparam[i], block);
        } else {
            param = toSSAParamName(NULL, block);
        }
        newIR(PARAM, func, param, toSSAOperand(func_impl->param[i], block_id), block);
    }
    struct IntConst* int_const = newIntConstAST(func_impl->param_num);
    OPERAND_TYPE* result = toSSATempVariable(block);
    newIR(CALL, func, toSSAIntConst(int_const, block), result, block);
    return result;
}

OPERAND_TYPE* toSSAArrayImplAST(struct ArrayImplAST* array_impl, int block_id) {
    // not complete
}

OPERAND_TYPE* toSSAOperand(struct Operand* operand, int block_id) {
    EnsureNotNull(operand);
    BASIC_BLOCK_TYPE* basic_block = getLinkedTable(id_block_table, &block_id);
    int o_block_id = block_id;
    int t_block_id = block_id;
    OPERAND_TYPE* return_operand;
    switch (operand->valuetype) {
        case INTCONST:
            return toSSAIntConst(operand->value.intconst, basic_block);
        case VARSYMENTRY:
            return toSSAVarSymEntry(operand->value.variable, basic_block);
        case FUNCIMPLAST:
            return toSSAFuncImplAST(operand->value.function, block_id);
        case ARRAYIMPLAST:
            return toSSAArrayImplAST(operand->value.array, block_id);
        case EXPAST:
            return_operand = toSSAExpAST(operand->value.exp, &t_block_id);
            if (t_block_id != o_block_id) {
                PrintErrExit("USE IF OR WHILE IN OPERAND?");
            }
            return return_operand;
        case STRING:
            return toSSAString(operand->value.string, basic_block);
        default:
            PrintErrExit("NOT SUPPORT OPERAND VALUE TYPE");
    }
    return NULL;
}

void toSSAIfStmt(struct ExpAST* exp, int* block_id) {
    EnsureNotNull(exp);
    EnsureNotNull(block_id);

    OPERAND_TYPE* cond_op = toSSAOperand(exp->op1, *block_id);
    int cond_block_id = *block_id;
    BASIC_BLOCK_TYPE* cond_block = getLinkedTable(id_block_table, &cond_block_id);

    int merge_block_id = toSSABlock(NULL);
    int then_block_id = toSSABlock(exp->op2->value.exp);

    BASIC_BLOCK_TYPE* then_block[2] = {getLinkedTable(id_block_table, &then_block_id),
                                       getLinkedTable(id_tail_block_table, &then_block_id)};
    int else_block_id = toSSABlock(exp->op2->value.exp);
    BASIC_BLOCK_TYPE* else_block[2] = {getLinkedTable(id_block_table, &else_block_id),
                                       getLinkedTable(id_tail_block_table, &else_block_id)};

    BASIC_BLOCK_TYPE* merge_block = getLinkedTable(id_block_table, &merge_block_id);

    newIR(BRANCH, cond_op, toSSABasicBlock(then_block[0], cond_block), toSSABasicBlock(else_block[0], cond_block), cond_block);

    addBasicBlockEdge(cond_block, then_block[0]);
    addBasicBlockEdge(cond_block, else_block[0]);
    addBasicBlockEdge(then_block[1], merge_block);
    addBasicBlockEdge(else_block[1], merge_block);

    setBasicBlockSealed(then_block[1]);
    setBasicBlockSealed(else_block[1]);
    setBasicBlockSealed(cond_block);

    *block_id = merge_block_id;
}

OPERAND_TYPE* toSSAWhileStmt(struct ExpAST* exp, int* block_id) {
    EnsureNotNull(exp);
    EnsureNotNull(block_id);

    BASIC_BLOCK_TYPE* before_block = getLinkedTable(id_block_table, block_id);

    int cond_block_id = toSSABlock(NULL);
    BASIC_BLOCK_TYPE* cond_block = getLinkedTable(id_block_table, &cond_block_id);
    OPERAND_TYPE* cond_op = toSSAOperand(exp->op1, cond_block_id);

    newIR(JUMP, NULL, NULL, toSSABasicBlock(cond_block, before_block), before_block);
    addBasicBlockEdge(before_block, cond_block);
    setBasicBlockSealed(before_block);

    int loop_end_block_id = toSSABlock(NULL);
    BASIC_BLOCK_TYPE* loop_end_block = getLinkedTable(id_block_table, &loop_end_block_id);
    int merge_block_id = toSSABlock(NULL);
    BASIC_BLOCK_TYPE* merge_block = getLinkedTable(id_block_table, &merge_block_id);

    while_level++;
    int n_while_level = while_level;
    setLinkedTable(while_break_table, &n_while_level, &merge_block_id);
    setLinkedTable(while_continue_table, &n_while_level, &loop_end_block_id);

    int loop_block_id = toSSABlock(exp->op1->value.exp);

    BASIC_BLOCK_TYPE* loop_block[2] = {getLinkedTable(id_block_table, &loop_block_id),
                                       getLinkedTable(id_tail_block_table, &loop_block_id)};

    newIR(JUMP, NULL, NULL, toSSABasicBlock(loop_end_block, loop_block[1]), loop_block[1]);
    addBasicBlockEdge(loop_block[1], loop_end_block);
    setBasicBlockSealed(loop_block[1]);

    newIR(JUMP, NULL, NULL, toSSABasicBlock(cond_block, loop_end_block), loop_end_block);
    addBasicBlockEdge(loop_end_block, cond_block);
    setBasicBlockSealed(loop_end_block);

    newIR(BRANCH, cond_op, toSSABasicBlock(loop_block[0], cond_block), toSSABasicBlock(merge_block, cond_block), cond_block);
    addBasicBlockEdge(cond_block, loop_block[0]);
    addBasicBlockEdge(cond_block, merge_block);
    setBasicBlockSealed(cond_block);

    *block_id = merge_block_id;

    struct Item* item;
    item = removeLinkedTable(while_break_table, &n_while_level);
    free(item);
    item = removeLinkedTable(while_continue_table, &n_while_level);
    free(item);
    while_level--;
}

void toSSABreakStmt(int block_id) {
    BASIC_BLOCK_TYPE* block = getLinkedTable(id_block_table, &block_id);
    BASIC_BLOCK_TYPE* target = getLinkedTable(while_break_table, &while_level);
    newIR(JUMP, NULL, NULL, toSSABasicBlock(target, block), block);
    addBasicBlockEdge(block, target);
}

void toSSAContinuekStmt(int block_id) {
    BASIC_BLOCK_TYPE* block = getLinkedTable(id_block_table, &block_id);
    BASIC_BLOCK_TYPE* target = getLinkedTable(while_continue_table, &while_level);
    newIR(JUMP, NULL, NULL, toSSABasicBlock(target, block), block);
    addBasicBlockEdge(block, target);
}

/**
 * 将对应的语句插入block_id对应的块中
 * 返回表达式最后对应的值。
 * 如果创建了新的块，修改block_id
 * 返回最后被赋值的操作数
 */
OPERAND_TYPE* toSSAExpAST(struct ExpAST* exp, int* block_id) {
    EnsureNotNull(exp);
    EnsureNotNull(block_id);

    OPERAND_TYPE *op1, *op2, *op3;
    IR_TYPE* new_ir;
    switch (exp->op) {
        case OPREAND:
        case INTCONST:
            return toSSAOperand(exp->op1, *block_id);
        case ASSIGN:
            op1 = toSSAOperand(exp->op2, *block_id);
            op3 = toSSAOperand(exp->op1, *block_id);
            new_ir = newIR(exp->op, op1, NULL, op3, getLinkedTable(id_block_table, block_id));
            return op3;
        case K_NOT:
        case K_ADD:
        case K_SUB:
        case K_MUL:
        case K_DIV:
        case K_MOD:
            op1 = toSSAOperand(exp->op1, *block_id);
            op2 = toSSAOperand(exp->op2, *block_id);
            op3 = toSSAOperand(exp->op3, *block_id);
            new_ir = newIR(exp->op, op1, op2, op3, getLinkedTable(id_block_table, block_id));
            return op3;
        case K_AND:
        case K_OR:
        case K_EQ:
        case K_NEQ:
        case K_LT:
        case K_LTE:
        case K_GT:
        case K_GTE:
            PrintErrExit("NOT IMPLEMENT");
            op1 = toSSAOperand(exp->op1, *block_id);
            op2 = toSSAOperand(exp->op2, *block_id);
            op3 = toSSAOperand(exp->op3, *block_id);
            new_ir = newIR(exp->op, op1, op2, op3, getLinkedTable(id_block_table, block_id));
            return op3;
        case IFSTMT:
            toSSAIfStmt(exp, block_id);
            return NULL;
        case WHILESTMT:
            toSSAWhileStmt(exp, block_id);
            return NULL;
        case BREAKSTMT:
            toSSABreakStmt(*block_id);
            return NULL;
        case CONTINUESTMT:
            toSSAContinuekStmt(*block_id);
            return NULL;
        case RETURNSTMT:
            newIR(RETURNSTMT, toSSAOperand(exp->op1, *block_id), NULL, NULL, getLinkedTable(id_block_table, block_id));
            return NULL;
    }
    return NULL;
}

// return block_id
int toSSABlock(struct ExpAST* exp) {
    int block_id = block_id_next;
    int* first_block = (int*)malloc(sizeof(int));
    *first_block = block_id;
    block_id_next++;

    BASIC_BLOCK_TYPE* block_first = newBasicBlock(NULL);
    BASIC_BLOCK_TYPE* block_tail = NULL;
    setLinkedTable(id_block_table, first_block, block_first);
    if (exp == NULL) {
        setLinkedTable(id_tail_block_table, first_block, block_first);
        return *first_block;
    }

    while (exp != NULL) {
        toSSAExpAST(exp, &block_id);
        if (exp->op == RETURNSTMT || exp->op == CONTINUESTMT || exp->op == BREAKSTMT) {
            break;  // 跳过无法访问的语句.
        }
        exp = exp->next;
    }
    block_tail = getLinkedTable(id_block_table, &block_id);
    //setBasicBlockSealed(block_tail);
    setLinkedTable(id_tail_block_table, first_block, block_tail);
    return *first_block;
}

BASIC_BLOCK_TYPE* toSSAFunction(struct FuncSymEntry* fse) {
    int block_id = toSSABlock(fse->funcbody);
    BASIC_BLOCK_TYPE* block = getLinkedTable(id_block_table, &block_id);
    setBasicBlockSealed(block);
    return block;
}

void toSSAFunctions() {
    func_block_table = newLinkedTable(string_equal);

    id_block_table = newLinkedTable(int_euqal);
    id_tail_block_table = newLinkedTable(int_euqal);
    while_break_table = newLinkedTable(int_euqal);
    while_continue_table = newLinkedTable(int_euqal);
    cond_false_table = newLinkedTable(int_euqal);
    cond_true_table = newLinkedTable(int_euqal);

    struct FuncSymEntry* head = funcsymtable_p->head;
    struct FuncSymEntry* fse = head->next;
    while (fse != head) {
        BASIC_BLOCK_TYPE* body = toSSAFunction(fse);
        setLinkedTable(func_block_table, strdup(fse->name), body);
        fse = fse->next;
    }
}
