
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "./ssa.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils/DequeList.h"
#include "../utils/LinkedTable.h"

int current_size;
int max_capacity;
Ir* currentIr;
Value value_list[MAX_CAPACITY];
ID id_list[MAX_CAPACITY];  //这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量的

int alloc_register() {
    return current_size++;
}

Ir* create_new_ir(int op_type, Operand* op1, Operand* op2, Operand* op3) {
    Ir* ir = (Ir*)malloc(sizeof(Ir));
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    ir->type = op_type;
    return ir;
}

//创建一个新的block
BasicBlock* create_new_block() {
    MALLOC(block, BasicBlock, 1);
    MALLOC_WITHOUT_DECLARE(block->predecessors, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->successors, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->ir_list, Ir, 1);
    block->is_sealed = 0;
    block->is_full = 0;
    block->predecessor_num = 0;
    block->successor_num = 0;
    block->predecessors->value = NULL;
    block->successors->value = NULL;
    block->ir_list->type = NOP;
    list_init(&(block->predecessors->block_link));
    list_init(&(block->successors->block_link));
    list_init(&(block->ir_list->ir_link));
    return block;
}

//为两个block建立祖先和后继的关系
void connect_block(BasicBlock* pre, BasicBlock* suc) {
    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof(BasicBlockNode));
    tmp->value = suc;
    list_add(&(tmp->block_link), &(pre->successors->block_link));

    tmp = (BasicBlockNode*)malloc(sizeof(BasicBlockNode));
    tmp->value = pre;
    list_add(&(tmp->block_link), &(suc->predecessors->block_link));
}

int read_variable(ID id, BasicBlock* block) {
    //根据id寻找是否在次之前定义过这个变量
    int i = 0;
    for (; i < current_size; i++)
        if (id_list[i] == id)
            return i;
    return -1;
}

void write_variable(ID id, BasicBlock* block, Ir* ir) {
    //定义这个变量
    int reg = alloc_register();
    id_list[reg] = id;
    value_list[reg].complex_value = ir;
}

BASIC_BLOCK_TYPE* newBasicBlock(BASIC_BLOCK_TYPE* predecessor) {
    BasicBlock* b = create_new_block();
    if (predecessor) {
        b->predecessor_num++;
        connect_block(predecessor, b);
    }

    return b;
}

void setBasicBlockSealed(BASIC_BLOCK_TYPE* basic_block) {
    basic_block->is_sealed = 1;
}

OPERAND_TYPE* toSSAIntConst(struct IntConst* int_const, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(operand, OPERAND_TYPE, 1);
    operand->type = INT;
    operand->operand.v.intValue = int_const->value;
    return operand;
}

OPERAND_TYPE* toSSAString(struct String* str, BASIC_BLOCK_TYPE* basic_block) {
    OPERAND_TYPE* oprand = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    oprand->type = ConstSTRING;
    oprand->operand.v.str = strdup(str->content);
    return oprand;
}

OPERAND_TYPE* toSSAVarTabElemRead(struct VarTabElem* vte, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(op, OPERAND_TYPE, 1);
    op->type = REGISTER;
    ID id = (ID)vte;
    int res = read_variable(id, basic_block);
    if (res != -1)
        op->operand.reg_idx = res;
    else
        op->operand.reg_idx = 0;
    return op;
}

OPERAND_TYPE* toSSAVarTabElemWrite(struct VarTabElem* vte, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(op, OPERAND_TYPE, 1);
    int res = read_variable((ID)vte, basic_block);
    if (res != -1) {
        op->type = REGISTER;
        op->operand.reg_idx = res;
    } else {
        int new_reg = alloc_register();
        op->type = REGISTER;
        op->operand.reg_idx = new_reg;
        id_list[new_reg] = (ID)vte;
    }
    return op;
}

OPERAND_TYPE* toSSAOffset(int base, int offset, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(op, OPERAND_TYPE, 1);
    op->type = base;
    op->operand.v.intValue = offset;
    return op;
}

OPERAND_TYPE* toSSABasicBlock(BASIC_BLOCK_TYPE* target_block, BASIC_BLOCK_TYPE* basic_block) {
    OPERAND_TYPE* opr = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    opr->type = BASIC_BLOCK;
    opr->operand.v.b = target_block;
    return opr;
}

OPERAND_TYPE* toSSAFuncName(struct FuncTabElem* fte, BASIC_BLOCK_TYPE* basic_block) {
    OPERAND_TYPE* opr = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    opr->type = FUNCID;
    opr->operand.v.funcID = (void*)fte;
    return opr;
}

OPERAND_TYPE* toSSAParamName(struct VarTabElem* vse, BASIC_BLOCK_TYPE* basic_block) {
    return NULL;
}

OPERAND_TYPE* toSSATempVariable(BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(op, OPERAND_TYPE, 1);
    op->type = REGISTER;
    op->operand.reg_idx = alloc_register();
    id_list[op->operand.reg_idx] = (ID)op;
    return op;
}

IR_TYPE* newIR(int op, OPERAND_TYPE* op1, OPERAND_TYPE* op2, OPERAND_TYPE* op3, BASIC_BLOCK_TYPE* basic_block) {
    EnsureNotNull(basic_block);
    Ir* ir = create_new_ir(op, op1, op2, op3);
    Ir* head = basic_block->ir_list;
    list_add_before(&(head->ir_link), &(ir->ir_link));
    return ir;
}

void addBasicBlockEdge(BASIC_BLOCK_TYPE* predecessor, BASIC_BLOCK_TYPE* successor) {
    connect_block(predecessor, successor);
}

static int __block_equal(void* k1, void* k2) {
    return k1 == k2;
}

void goThroughFunction(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args) {
    struct LinkedTable* visited = newLinkedTable(__block_equal);
    struct DequeList* queue = newDequeList();
    setLinkedTable(visited, basic_block_head, basic_block_head);
    pushFrontDequeList(queue, basic_block_head);
    while (!isEmptyDequeList(queue)) {
        BasicBlock* block = popBackDequeList(queue);
        func(block, args);
        list_entry_t* head = &(block->successors->block_link);
        list_entry_t* next = list_next(head);
        while (next != head) {
            BasicBlockNode* t_node = le2struct(next, BasicBlockNode, block_link);
            BasicBlock* t_block = t_node->value;
            void* res = getLinkedTable(visited, t_block);
            if (res == NULL) {
                setLinkedTable(visited, t_block, t_block);
            }
            next = list_next(next);
        }
    }
}

void __print_op(Operand* op) {
    if (op == NULL) {
        printf("- ");
        return;
    }
    switch (op->type) {
        case INT:
            printf("$%d ", op->operand.v.intValue);
            break;
        case REGISTER:
            printf("%%%d ", op->operand.reg_idx);
            break;
        case FRAMEPOINT:
        case STACKPOINT:
        case GLOBALDATA:
            printf("@0x%x ", op->operand.v.intValue);
            break;
        case ConstSTRING:
            printf("%s ", op->operand.v.str);
            break;
        case FUNCID:
            printf("%s ", ((struct FuncTabElem*)(op->operand.v.funcID))->name);
            break;
        case BASIC_BLOCK:
            printf("0x%p ", op->operand.v.b);
            break;
        default:
            PrintErrExit("error %s", EnumTypeToString(op->type));
    }
}

void __print_basic_block(BASIC_BLOCK_TYPE* basic_block, void* args) {
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = head->next;
    while (next != head) {
        Ir* ir = le2struct(next, Ir, ir_link);
        printf("op: %12s ", EnumTypeToString(ir->type));
        printf("op1: ");
        __print_op(ir->op1);
        printf("op2: ");
        __print_op(ir->op2);
        printf("op3: ");
        __print_op(ir->op3);
        printf("\n");
        next = list_next(next);
    }
}