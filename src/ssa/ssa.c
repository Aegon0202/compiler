
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
struct LinearList* id_list;            // index: VarTabElem* value: int*
struct LinearList* reg_id_vartabelem;  // index: int value: VarTabElem*
//ID id_list[MAX_CAPACITY];  //这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量的

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
    MALLOC_WITHOUT_DECLARE(block->dominantor, BasicBlockNode, 1);
    block->is_sealed = 0;
    block->is_full = 0;
    block->predecessor_num = 0;
    block->successor_num = 0;
    block->predecessors->value = NULL;
    block->successors->value = NULL;
    block->ir_list->type = NOP;
    block->dominantor->value = NULL;
    list_init(&(block->predecessors->block_link));
    list_init(&(block->successors->block_link));
    list_init(&(block->ir_list->ir_link));
    list_init(&(block->dominantor->block_link));
    return block;
}

//为两个block建立祖先和后继的关系
void connect_block(BasicBlock* pre, BasicBlock* suc) {
    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof(BasicBlockNode));
    tmp->value = suc;
    list_add(&(pre->successors->block_link), &(tmp->block_link));

    tmp = (BasicBlockNode*)malloc(sizeof(BasicBlockNode));
    tmp->value = pre;
    list_add(&(suc->predecessors->block_link), &(tmp->block_link));
}

int read_variable(ID id, BasicBlock* block) {
    //根据id寻找是否在次之前定义过这个变量
    int* reg_p = getLinearList(id_list, (size_t)id);
    if (reg_p == NULL) {
        return -1;
    } else {
        return *reg_p;
    }
}

void write_variable(ID id, BasicBlock* block, Ir* ir) {
    //定义这个变量
    int reg = alloc_register();

    MALLOC(reg_p, int, 1);
    *reg_p = reg;
    setLinearList(id_list, (size_t)id, reg_p);
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

        MALLOC(reg_p, int, 1);
        *reg_p = new_reg;
        setLinearList(id_list, (size_t)vte, reg_p);
        setLinearList(reg_id_vartabelem, *reg_p, vte);
    }
    return op;
}

OPERAND_TYPE* toSSAOffset(int base, long long offset, BASIC_BLOCK_TYPE* basic_block) {
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

    MALLOC(reg_id, int, 1);
    *reg_id = op->operand.reg_idx;
    setLinearList(id_list, (size_t)op, reg_id);
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
                pushFrontDequeList(queue, t_block);
                setLinkedTable(visited, t_block, t_block);
            }
            next = list_next(next);
        }
    }
    freeDequeList(&queue);
    struct Item* t;
    while ((t = popLinkedTable(visited)) != NULL)
        free(t);
    freeLinkedTable(&visited);
}

const char* _op_to_str(Operand* op) {
    static char buffer[20];
    if (op == NULL) {
        snprintf(buffer, 20, "%5s: -", "-");
        return buffer;
    }
    switch (op->type) {
        case INT:
            snprintf(buffer, 20, "%5s: $%d", "int", op->operand.v.intValue);
            break;
        case REGISTER:
            snprintf(buffer, 20, "%5s: %%%d", "reg", op->operand.reg_idx);
            break;
        case FRAMEPOINT:
            snprintf(buffer, 20, "%5s: @%lld", "$fp", op->operand.v.intValue);
            break;
        case STACKPOINT:
            snprintf(buffer, 20, "%5s: @%lld", "$sp", op->operand.v.intValue);
            break;
        case GLOBALDATA:
            snprintf(buffer, 20, "%5s: @%p", "$gd", op->operand.v.intValue);
            break;
        case ConstSTRING:
            snprintf(buffer, 20, "%5s: %-10s", "str", op->operand.v.str);
            break;
        case FUNCID:
            snprintf(buffer, 20, "%5s: %-10s", "func", ((struct FuncTabElem*)(op->operand.v.funcID))->name);
            break;
        case BASIC_BLOCK:
            snprintf(buffer, 20, "%5s: %p", "block", op->operand.v.b);
            break;
        default:
            PrintErrExit("error %s", EnumTypeToString(op->type));
    }
    return buffer;
}

void __print_basic_block(BASIC_BLOCK_TYPE* basic_block, void* args) {
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = head->next;
    printf("block address %p:\n", basic_block);
    while (next != head) {
        Ir* ir = le2struct(next, Ir, ir_link);
        printf("op: %12s\t", EnumTypeToString(ir->type));
        printf("op1: %-20s\t", _op_to_str(ir->op1));
        printf("op2: %-20s\t", _op_to_str(ir->op2));
        printf("op3: %-20s\t", _op_to_str(ir->op3));
        printf("\n");
        next = list_next(next);
    }
}

//ssa form construct-------------------------------------------------------------------

void __get_all_nodes(BasicBlock* block, BasicBlockNode* node) {
    return;
}

void __init_dominator(BasicBlock* block, BasicBlockNode* node_set, BasicBlock* start) {
    list_entry_t* head = &(block->dominantor->block_link);
    list_entry_t* head_node_set = &(node_set->block_link);
    list_entry_t* elem = list_next(head_node_set);
    MALLOC(node, BasicBlockNode, 1);
    node->value = node_set->value;
    list_add(head, &(node->block_link));

    while (head_node_set != elem) {
        if (le2struct(elem, BasicBlockNode, block_link)->value != start) {
            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
            node->value = le2struct(elem, BasicBlockNode, block_link)->value;
            list_add(head, node);
        }
        elem = list_next(elem);
    }
    return;
}

int __search_BlockNode_elem(list_entry_t* list, BasicBlock* value) {
    list_entry_t* elem = list_next(list);
    while (elem != list) {
        if (le2struct(elem, BasicBlockNode, block_link)->value == value)
            return 1;
        elem = list_next(elem);
    }
    return 0;
}

void __delet_list(list_entry_t* list1) {
    list_entry_t* elem1 = list_next(list1);
    while (elem1 != list1) {
        list_entry_t* elem1_copy = elem1;
        elem1 = list_next(elem1);
        free(le2struct(elem1_copy, BasicBlockNode, block_link));
    }
    free(le2struct(list1, BasicBlockNode, block_link));
}

//list1 = list1交list2
list_entry_t* __intersection_list(list_entry_t* list1, list_entry_t* list2) {
    list_entry_t* elem1 = list_next(list1);
    list_entry_t* elem2 = list_next(list2);
    BasicBlockNode* node;
    list_entry_t* result;
    MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
    result = &(node->block_link);
    list_init(result);
    node->value = NULL;
    while (elem1 != list1) {
        if (__search_BlockNode_elem(list2, le2struct(elem1, BasicBlockNode, block_link)->value)) {
            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
            node->value = le2struct(elem1, BasicBlockNode, block_link)->value;
            list_add(result, &(node->block_link));
        }
        elem1 = list_next(elem1);
    }
    elem1 = list_next(list1);
    return result;
}

//list1是不是list2的子集
int __is_sublist(list_entry_t* list1, list_entry_t* list2) {
    return 1;
}

int __list_equal(list_entry_t* list1, list_entry_t* list2) {
    return __is_sublist(list1, list2) && __is_sublist(list2, list1);
}

void caculate_dominance(BasicBlock* start) {
    list_entry_t* head;
    list_entry_t* list = &(start->dominantor->block_link);
    MALLOC(node, BasicBlockNode, 1);
    node->value = start;
    list_entry_t* elem = &(node->block_link);
    list_add(list, elem);

    MALLOC(node_set, BasicBlockNode, 1);
    list_init(&(node_set->block_link));
    __get_all_nodes(start, &node_set);
    head = &(node_set->block_link);
    elem = list_next(head);

    //对每一个basicblock结点初始化
    while (head != elem) {
        __init_dominator(le2struct(elem, BasicBlockNode, block_link)->value, node_set, start);
        elem = list_next(elem);
    }

    int change = 1;
    do {
        change = 0;
        elem = list_next(head);
        //遍历所有结点
        while (elem != head) {
            //对前驱结点的dominator集合求并集
            //收敛之后就推出循环
            list_entry_t* new_list = &(node_set->block_link);
            BasicBlock* value = le2struct(elem, BasicBlockNode, block_link)->value;
            //遍历所有前驱结点
            list_entry_t* pre_node_list = &(le2struct(elem, BasicBlockNode, block_link)->value->predecessors->block_link);
            list_entry_t* pre_node_elem = list_next(pre_node_list);
            while (pre_node_list != pre_node_elem) {
                list_entry_t l = le2struct(pre_node_elem, BasicBlockNode, block_link)->value->dominantor->block_link;
                list_entry_t* tmp = __intersection_list(new_list, &l);
                __delet_list(new_list);
                new_list = tmp;
                pre_node_elem = list_next(pre_node_elem);
            }
            if (!__search_BlockNode_elem(new_list, value)) {
                MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                node->value = value;
                list_add(new_list, &(node->block_link));
            }
            if (!__list_equal(new_list, &(value->dominantor->block_link))) {
                change = 1;
                __delet_list(&(value->dominantor->block_link));
                value->dominantor = le2struct(new_list, BasicBlockNode, block_link);
            }
        }
    } while (change == 1);
}
