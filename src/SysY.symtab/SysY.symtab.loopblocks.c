#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "./SysY.symtab.def.h"

struct LoopBlocks* newLoopBlock(struct DequeList* blocks, BASIC_BLOCK_TYPE* loop_entry) {
    MALLOC(lb, struct LoopBlocks, 1);
    lb->blocks = blocks;
    lb->loop_entry = loop_entry;
    return lb;
}

void freeLoopBlock(struct LoopBlocks* lb) {
    void* value;
    while ((value = popBackDequeList(lb->blocks)) != NULL)
        ;
    freeDequeList(&lb->blocks);
    while ((value = popBackDequeList(lb->out_blocks)) != NULL)
        ;
    freeDequeList(&lb->out_blocks);
    free(lb);
}

void __find_back_edge(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct DequeList* queue = (struct DequeList*)args;
    list_entry_t* succ_head = &basic_block->successors->block_link;
    list_entry_t* succ_next = list_next(succ_head);
    while (succ_head != succ_next) {
        BASIC_BLOCK_TYPE* succ_block = (le2BasicBlock(succ_next)->value);
        list_entry_t* dom_head = &basic_block->dominator->block_link;
        list_entry_t* dom_next = list_next(dom_head);
        while (dom_next != dom_head) {
            BASIC_BLOCK_TYPE* dom_block = (le2BasicBlock(dom_next)->value);
            if (succ_block == dom_block) {
                MALLOC(item, struct Item, 1);  // key为basic_block value为必经节点
                item->key = basic_block;
                item->value = dom_block;
                pushFrontDequeList(queue, item);
            }
            dom_next = list_next(dom_next);
        }
        succ_next = list_next(succ_next);
    }
}

int __is_block_in_deque(BASIC_BLOCK_TYPE* basic_block, struct DequeList* deque) {
    int deque_length = sizeDequeList(deque);
    for (int i = 0; i < deque_length; i++) {
        BASIC_BLOCK_TYPE* b = getDequeList(deque, i);
        if (b == basic_block) {
            return 1;
        }
    }
    return 0;
}

void __get_loop_basic_block(struct LoopBlocks* lb) {
    struct LinearList* visited = newLinearList();
    struct DequeList* stack = newDequeList();
    void* value = NULL;
    while ((value = popFrontDequeList(lb->blocks)) != NULL) {
        if (value != lb->loop_entry) {
            pushFrontDequeList(stack, value);
        }
        setLinearList(visited, (size_t)value, value);
    }
    setLinearList(visited, (size_t)lb->loop_entry, lb->loop_entry);
    pushFrontDequeList(lb->blocks, lb->loop_entry);

    while (!isEmptyDequeList(stack)) {
        BASIC_BLOCK_TYPE* b_block = popFrontDequeList(stack);
        pushFrontDequeList(lb->blocks, b_block);
        list_entry_t* head = &b_block->predecessors->block_link;
        list_entry_t* next = list_next(head);
        while (head != next) {
            BASIC_BLOCK_TYPE* p_block = (le2BasicBlock(next)->value);
            void* value = setLinearList(visited, (size_t)p_block, p_block);
            if (value == NULL) {
                pushFrontDequeList(stack, p_block);
            }
            next = list_next(next);
        }
    }
    freeDequeList(&stack);
    while (popLinearList(visited) != NULL)
        ;
    freeLinearList(&visited);
    return;
}

void __add_loop_outblock(struct LoopBlocks* lb) {
    lb->out_blocks = newDequeList();
    int size = sizeDequeList(lb->blocks);
    for (int i = 0; i < size; i++) {
        BASIC_BLOCK_TYPE* b = getDequeList(lb->blocks, i);
        list_entry_t* head = &b->successors->block_link;
        list_entry_t* next = list_next(head);
        while (head != next) {
            BASIC_BLOCK_TYPE* succ = (le2BasicBlock(next)->value);
            if (!__is_block_in_deque(succ, lb->blocks) &&
                !__is_block_in_deque(b, lb->out_blocks)) {
                pushFrontDequeList(lb->out_blocks, b);
            }
            next = list_next(next);
        }
    }
}

void __add_loop_entry_before(struct LoopBlocks* lb) {
    BASIC_BLOCK_TYPE* new_b = create_new_block();
    BASIC_BLOCK_TYPE* entry = lb->loop_entry;
    list_entry_t* head = &entry->predecessors->block_link;
    list_entry_t* next = list_next(head);
    while (head != next) {
        BASIC_BLOCK_TYPE* block = (le2BasicBlock(next)->value);
        next = list_next(next);
        if (!__is_block_in_deque(block, lb->blocks)) {
            disconnect_block(block, lb->loop_entry);
            //connect_block(block, lb->loop_entry);
            connect_block(block, new_b);
            IR_TYPE* ir = le2struct(block->ir_list->ir_link.prev, IR_TYPE, ir_link);
            if (ir->type == BRANCH && ir->op2->operand.v.b == lb->loop_entry) {
                ir->op2->operand.v.b = new_b;
            } else {
                ir->op3->operand.v.b = new_b;
            }
        }
    }
    connect_block(new_b, lb->loop_entry);
    lb->loop_before = new_b;

    MALLOC(j_op, OPERAND_TYPE, 1);
    j_op->type = BASIC_BLOCK;
    j_op->operand.v.b = lb->loop_entry;
    IR_TYPE* ir = create_new_ir(JUMP, NULL, NULL, j_op, new_b);
    list_add_before(&(new_b->ir_list->ir_link), &(ir->ir_link));
}

void __calc_func_loop_blocks(struct FuncTabElem* func) {
    if (func->loop_blocks == NULL) {
        func->loop_blocks = newDequeList();
    }
    struct DequeList* back_edge = newDequeList();
    struct LinearList* table = newLinearList();

    deepTraverseSuccessorsBasicBlock(func->blocks, __find_back_edge, back_edge);

    while (!isEmptyDequeList(back_edge)) {
        struct Item* item = popFrontDequeList(back_edge);
        struct LoopBlocks* o_b = getLinearList(table, (size_t)item->value);
        if (o_b == NULL) {
            struct DequeList* blocks = newDequeList();
            pushFrontDequeList(blocks, item->key);
            struct LoopBlocks* lb = newLoopBlock(blocks, item->value);
            setLinearList(table, (size_t)item->value, lb);
            pushFrontDequeList(func->loop_blocks, lb);
        } else {
            pushFrontDequeList(o_b->blocks, item->value);
        }
        free(item);
    }

    freeDequeList(&back_edge);
    void* value = NULL;
    while ((value = popLinearList(table)) != NULL)
        ;
    freeLinearList(&table);

    int loop_blocks_num = sizeDequeList(func->loop_blocks);
    for (int i = 0; i < loop_blocks_num; i++) {
        struct LoopBlocks* lb = getDequeList(func->loop_blocks, i);
        __get_loop_basic_block(lb);
        __add_loop_outblock(lb);
    }
    for (int i = 0; i < loop_blocks_num; i++) {
        struct LoopBlocks* lb = getDequeList(func->loop_blocks, i);
        __add_loop_entry_before(lb);
    }
    for (int i = 0; i < loop_blocks_num; i++) {
        struct LoopBlocks* lb = getDequeList(func->loop_blocks, i);
        __get_loop_basic_block(lb);
    }
}

void a(BasicBlock* block, void* args) {
    list_entry_t* head = &block->successors->block_link;
    list_entry_t* elem = list_next(head);
    while (head != elem) {
        BasicBlock* bb = le2BasicBlock(elem)->value;
        elem = list_next(elem);
        disconnect_block(block, bb);
        connect_block(block, bb);
    }
}

void calcAllLoopBlocks() {
    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* func = getLinearList(func_table->all_funcs, i);
        if (func->blocks != NULL) {
            update_CFG(func->blocks);
            __calc_func_loop_blocks(func);
            //deepTraverseSuccessorsBasicBlock(func->blocks, a, NULL);
            //deepTraverseSuccessorsBasicBlock(func->blocks, a, NULL);
            update_CFG(func->blocks);
        }
    }
}