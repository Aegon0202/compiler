
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
#include "traverse.h"

int current_size;
int max_capacity;
Ir* currentIr;

struct LinearList* id_list;  // index: VarTabElem* value: int* 这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量

struct LinearList* reg_id_vartabelem;  // index: int value: VarTabElem*

//--------以下3个变量用于构造ssa形式
struct LinearList* def_block;  // index: register index, value: list_entry_t*
struct LinearList* construct_Stack;
struct LinearList* construct_Counter;
struct LinearList* variable_bottom_index;  //每个变量不同下标到definition位置的映射

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

Ir* create_new_phi(Phi* op1, Operand* op3) {
    return create_new_ir(PHI, op1, NULL, op3);
}

//创建一个新的block
BasicBlock* create_new_block() {
    MALLOC(block, BasicBlock, 1);
    MALLOC_WITHOUT_DECLARE(block->predecessors, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->successors, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->ir_list, Ir, 1);
    MALLOC_WITHOUT_DECLARE(block->phi_list, Ir, 1);
    MALLOC_WITHOUT_DECLARE(block->dominator, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->i_dominator, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->I_dominant, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->dominant_frontier, BasicBlockNode, 1);
    block->is_sealed = 0;
    block->is_full = 0;
    block->predecessor_num = 0;
    block->successor_num = 0;
    block->predecessors->value = NULL;
    block->successors->value = NULL;
    block->ir_list->type = NOP;
    block->phi_list->type = NOP;
    block->dominator->value = NULL;
    block->i_dominator->value = NULL;
    block->I_dominant->value = NULL;
    block->dominant_frontier->value = NULL;
    block->has_already = 0;
    block->work = 0;
    list_init(&(block->predecessors->block_link));
    list_init(&(block->successors->block_link));
    list_init(&(block->ir_list->ir_link));
    list_init(&(block->phi_list->ir_link));
    list_init(&(block->dominator->block_link));
    list_init(&(block->i_dominator->block_link));
    list_init(&(block->I_dominant->block_link));
    list_init(&(block->dominant_frontier->block_link));
    return block;
}

BasicBlock* get_idominator(BasicBlock* block) {
    return le2BasicBlock(list_next(&(block->i_dominator->block_link)))->value;
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
            snprintf(buffer, 20, "%5s: $%d", "int", (int)op->operand.v.intValue);
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
            snprintf(buffer, 20, "%5s: %-10s", "$gd", ((struct VarTabElem*)op->operand.v.intValue)->name);
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

void __get_all_nodes(BasicBlock* block, void* node) {
    MALLOC(new_node, BasicBlockNode, 1);
    new_node->value = block;
    list_add_before(&(((BasicBlockNode*)(node))->block_link), &(new_node->block_link));
}

void __init_dominator(BasicBlock* block, BasicBlockNode* node_set, BasicBlock* start) {
    list_entry_t* head = &(block->dominator->block_link);
    list_entry_t* head_node_set = &(node_set->block_link);
    list_entry_t* elem = list_next(head_node_set);
    MALLOC(node, BasicBlockNode, 1);
    node->value = node_set->value;
    list_add(head, &(node->block_link));

    while (head_node_set != elem) {
        if (le2struct(elem, BasicBlockNode, block_link)->value != start) {
            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
            node->value = le2struct(elem, BasicBlockNode, block_link)->value;
            list_add(head, &(node->block_link));
        }
        elem = list_next(elem);
    }
    return;
}

void __init_strict_dominator(BasicBlock* block, void* args) {
    list_entry_t* list = &(block->i_dominator->block_link);
    list_entry_t* head = &(block->dominator->block_link);
    list_entry_t* elem = list_next(head);
    BasicBlockNode* node;
    while (head != elem) {
        if (le2BasicBlock(elem)->value != block) {
            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
            node->value = le2BasicBlock(elem)->value;
            list_add(list, &(node->block_link));
        }
        elem = list_next(elem);
    }
}

list_entry_t* __search_BlockNode_elem(list_entry_t* list, BasicBlock* value) {
    list_entry_t* elem = list_next(list);
    while (elem != list) {
        if (le2struct(elem, BasicBlockNode, block_link)->value == value)
            return elem;
        elem = list_next(elem);
    }
    return NULL;
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
    //list_entry_t* elem2 = list_next(list2);
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

list_entry_t* __union_list(list_entry_t* list1, list_entry_t* list2) {
    MALLOC(node, BasicBlockNode, 1);
    node->value = NULL;
    list_entry_t* list3 = &(node->block_link);
    list_entry_t* elem1 = list_next(list1);

    return list3;
}

//list1是不是list2的子集
int __is_sublist(list_entry_t* list1, list_entry_t* list2) {
    list_entry_t* elem = list_next(list1);
    while (elem != list1) {
        BasicBlock* value = le2BasicBlock(elem)->value;
        if (!__search_BlockNode_elem(list2, value))
            return 0;
        elem = list_next(elem);
    }
    return 1;
}

int __list_equal(list_entry_t* list1, list_entry_t* list2) {
    return __is_sublist(list1, list2) && __is_sublist(list2, list1);
}

void __caclulate_i_dominant(BasicBlock* block, void* args) {
    list_entry_t* list = &(block->i_dominator->value->I_dominant->block_link);
    if (!__search_BlockNode_elem(list, block)) {
        MALLOC(node, BasicBlockNode, 1);
        node->value = block;
        list_add(list, &(node->block_link));
    }
}

void __caculate_dominance(BasicBlock* start) {
    list_entry_t* head;
    list_entry_t* list = &(start->dominator->block_link);
    MALLOC(node, BasicBlockNode, 1);
    node->value = start;
    list_entry_t* elem = &(node->block_link);
    list_add(list, elem);

    MALLOC(node_set, BasicBlockNode, 1);
    list_init(&(node_set->block_link));
    deepTraverseSuccessorsBasicBlock(start, __get_all_nodes, node_set);
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
            list_entry_t* new_list = &(node_set->block_link);
            BasicBlock* value = le2struct(elem, BasicBlockNode, block_link)->value;
            //遍历所有前驱结点
            list_entry_t* pre_node_list = &(le2struct(elem, BasicBlockNode, block_link)->value->predecessors->block_link);
            list_entry_t* pre_node_elem = list_next(pre_node_list);
            while (pre_node_list != pre_node_elem) {
                list_entry_t l = le2struct(pre_node_elem, BasicBlockNode, block_link)->value->dominator->block_link;
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
            if (!__list_equal(new_list, &(value->dominator->block_link))) {
                change = 1;
                __delet_list(&(value->dominator->block_link));
                value->dominator = le2struct(new_list, BasicBlockNode, block_link);
            }
        }
    } while (change == 1);
}

void __immediate_dominance(BasicBlock* start) {
    __caculate_dominance(start);
    deepTraverseSuccessorsBasicBlock(start, __init_strict_dominator, NULL);
    list_entry_t *head, *elem;

    //获取全部basic block
    MALLOC(node_set, BasicBlockNode, 1);
    list_init(&(node_set->block_link));
    deepTraverseSuccessorsBasicBlock(start, __get_all_nodes, node_set);
    head = &(node_set->block_link);
    elem = list_next(head);

    //对所有非起始结点
    while (head != elem) {
        BasicBlock* n_value = le2BasicBlock(elem);
        list_entry_t* s_head = &(le2BasicBlock(elem)->value->i_dominator->block_link);
        list_entry_t* s_elem = list_next(s_head);
        while (s_head != s_elem) {
            BasicBlock* s_value = le2BasicBlock(s_elem);
            list_entry_t* t_head = &(le2BasicBlock(elem)->value->i_dominator->block_link);
            list_entry_t* t_elem = list_next(t_head);
            while (t_head != t_elem) {
                BasicBlock* t_value = le2BasicBlock(t_elem);
                if (t_value == s_value)
                    continue;
                list_entry_t* del = __search_BlockNode_elem(&(s_value->i_dominator->block_link), t_value);
                if (del) {
                    BasicBlockNode* del_node = le2BasicBlock(del);
                    list_del(del);
                    free(del_node);
                }
                t_elem = list_next(t_elem);
            }
            s_elem = list_next(s_elem);
        }
        elem = list_next(elem);
    }
    deepTraverseSuccessorsBasicBlock(start, __caclulate_i_dominant, NULL);
    __delet_list(node_set);
}

void __dominance_frontier(BasicBlock* start) {
    __immediate_dominance(start);
    list_entry_t *head, *elem;
    BasicBlockNode* node;
    //获取全部basic block
    MALLOC(node_set, BasicBlockNode, 1);
    list_init(&(node_set->block_link));
    deepTraverseSuccessorsBasicBlock(start, __get_all_nodes, node_set);
    head = &(node_set->block_link);
    elem = list_next(head);

    while (elem != head) {
        BasicBlock* value = le2BasicBlock(elem)->value;
        list_entry_t* y_head = &(le2BasicBlock(elem)->value->successors->block_link);
        list_entry_t* y_elem = list_next(y_head);
        list_entry_t* z_head = &(le2BasicBlock(elem)->value->I_dominant->block_link);
        list_entry_t* z_elem = list_next(z_head);

        while (y_head != y_elem) {
            BasicBlock* y_value = le2BasicBlock(y_elem)->value;
            if (get_idominator(y_value) != value) {
                MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                node->value = y_value;
                list_add(&(value->dominant_frontier->block_link), &(node->block_link));
            }
            y_elem = list_next(y_elem);
        }

        while (z_elem != z_head) {
            BasicBlock* z_value = le2BasicBlock(z_elem)->value;
            y_head = &(z_value->dominant_frontier->block_link);
            y_elem = list_next(y_head);
            while (y_head != y_elem) {
                BasicBlock* y_value = le2BasicBlock(y_elem)->value;
                if (get_idominator(y_value) != value) {
                    MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                    node->value = y_value;
                    list_add(&(value->dominant_frontier->block_link), &(node->block_link));
                }
                y_elem = list_next(y_elem);
            }

            z_elem = list_next(z_elem);
        }
    }
    elem = list_next(elem);
}

list_entry_t* caculate_DF_set(list_entry_t* list) {
    BasicBlockNode* node;
    MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
    node->value = NULL;
    list_entry_t* result = &(node->block_link);
    list_entry_t* elem = list_next(list);
    while (elem != list) {
        list_entry_t* list_df = &(le2BasicBlock(elem)->value->dominant_frontier->block_link);
        list_entry_t* tmp = __union_list(result, list_df);
        __delet_list(result);
        result = tmp;
        elem = list_next(elem);
    }
    return result;
}

list_entry_t* DF_plus(list_entry_t* list) {
    list_entry_t* result;
    list_entry_t* tmp;
    int change = 1;
    result = caculate_DF_set(list);
    while (change != 0) {
        change = 0;
        tmp = __union_list(result, list);
        if (!__list_equal(tmp, result)) {
            __delet_list(result);
            result = tmp;
            change = 1;
        } else
            __delet_list(tmp);
    }
    return result;
}

void __placement_phi(BasicBlock* start) {
    __dominance_frontier(start);
    int iter_count = 0;
    int loop_val = 0;
    MALLOC(node, BasicBlockNode, 1);
    list_entry_t* worklist = &(node->block_link);
    list_init(worklist);

    //遍历每个变量
    for (loop_val = 0; loop_val < current_size; loop_val++) {
        iter_count += 1;
        list_entry_t* head = getLinearList(def_block, loop_val);
        list_entry_t* elem = list_next(head);
        while (head != elem) {
            BasicBlock* X_value = le2BasicBlock(elem)->value;
            X_value->work = iter_count;

            if (!__search_BlockNode_elem(worklist, X_value)) {
                MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                node->value = X_value;
                list_add_before(worklist, &(node->block_link));
            }
        }

        while (worklist != list_next(worklist)) {
            list_entry_t* worklist_elem = list_next(worklist);
            BasicBlock* worklist_value = le2BasicBlock(worklist_elem)->value;

            list_entry_t* Y_head = &(worklist_value->dominant_frontier->block_link);
            list_entry_t* Y_elem = list_next(Y_head);
            while (Y_elem != Y_head) {
                BasicBlock* Y_value = le2BasicBlock(Y_elem)->value;
                if (Y_value->has_already < iter_count) {
                    MALLOC(op_3, Operand, 1);
                    op_3->type = REGISTER;
                    op_3->operand.reg_idx = loop_val;
                    Ir* ir = create_new_phi(NULL, &op_3);
                    list_add(&(Y_value->phi_list->ir_link), &(ir->ir_link));
                    Y_value->has_already = iter_count;
                    if (Y_value->work < iter_count) {
                        Y_value->work = iter_count;
                        if (!__search_BlockNode_elem(worklist, Y_value)) {
                            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                            node->value = Y_value;
                            list_add_before(worklist, &(node->block_link));
                        }
                    }
                }
            }
        }
    }
}

int __is_ordinary_assignment(Ir* ir) {
    int type = ir->type;
    int ans;
    switch (type) {
        case LOAD:
        case K_ADD:
        case K_SUB:
        case ASSIGN:
        case K_MUL:
        case K_DIV:
        case K_MOD:
        case K_AND:
        case K_OR:
        case K_EQ:
        case K_NEQ:
        case K_LT:
        case K_LTE:
        case K_GT:
        case K_GTE:
        case CALL:
            ans = 1;
            break;

        default:
            ans = 0;
            break;
    }
    return ans;
}

void renaming_variable(BasicBlock* start) {
    int cur_var = 0;

    for (cur_var = 0; cur_var < current_size; cur_var++) {
        struct DequeList* stack = newDequeList();
        setLinearList(construct_Counter, cur_var, 0);
        setLinearList(construct_Stack, cur_var, stack);
    }

    //-------------
    list_entry_t* ir_head = &(start->ir_list->ir_link);
    list_entry_t* ir_elem = list_next(ir_head);
    while (ir_elem != ir_head) {
        Ir* value = le2struct(ir_elem, Ir, ir_link);
        if (__is_ordinary_assignment(value)) {
            Operand* op1 = value->op1;
            Operand* op2 = value->op2;
            Operand* op3 = value->op3;
            if (op1 && op1->type == REGISTER) {
                int index = op1->operand.reg_idx;
                void* var_id = getLinearList(reg_id_vartabelem, index);
                struct DequeList* stack = getLinearList(construct_Stack, cur_var);
                int* i = getFrontDequeList(stack);
                EnsureNotNull(i);
            }
            if (op2 && op2->type == REGISTER) {
                int index = op2->operand.reg_idx;
                void* var_id = getLinearList(reg_id_vartabelem, index);
                struct DequeList* stack = getLinearList(construct_Stack, cur_var);
                int* i = getFrontDequeList(stack);
                EnsureNotNull(i);
            }

            int def_index = op3->operand.reg_idx;
            int* i = getLinearList(construct_Counter, def_index);
            //replace with v_i
            pushFrontDequeList(getLinearList(construct_Stack, def_index), );
        }

        ir_elem = list_next(ir_head);
    }
}