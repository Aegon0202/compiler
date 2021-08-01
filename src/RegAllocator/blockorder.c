#include "./blockorder.h"

#include "../ssa/traverse.h"
#include "../utils/Malloc.h"
#include "../utils/link.h"
//to be finished
struct DequeList* computeBlockOrder(BlockBegin* func_entry_block) {
    int count = 0;
    MALLOC(work_list, BlockBeginNode, 1);
    work_list->value = 0;
    list_entry_t* work_list_head = &work_list->link;
    list_init(work_list_head);
    MALLOC(node, BlockBeginNode, 1);
    node->value = func_entry_block;
    list_add(work_list_head, &node->link);

    struct DequeList* blocks = newDequeList();
    struct DequeList* successors = NULL;

    struct LinearList* isUse = newLinearList();
    while (!list_empty(work_list_head)) {
        list_entry_t* tail = list_prev(work_list_head);
        list_del(tail);
        BlockBeginNode* bNode = le2struct(tail, BlockBeginNode, link);
        BlockBegin* b = bNode->value;
        successors = getSuccessors(b);
        pushBackDequeList(blocks, b);
        count++;
        for (; !isEmptyDequeList(successors);) {
            BlockBegin* bb = (BlockBegin*)popBackDequeList(successors);
            if (setLinearList(isUse, (size_t)bb, bb) == NULL) {
                insertBlock(bb, work_list_head);
            };
        }
        freeDequeList(&successors);
    }
    while (popLinearList(isUse) != NULL) {
    }
    freeLinearList(&isUse);
    return blocks;
}

//bug:worklisthead为空
void insertBlock(BlockBegin* block, list_entry_t* work_list_head) {
    MALLOC(node, BlockBeginNode, 1);
    node->value = block;
    list_entry_t* next = list_next(work_list_head);
    while (next != work_list_head) {
        BlockBegin* b = le2struct(next, BlockBeginNode, link)->value;
        if (b->loop_depth > block->loop_depth) {
            break;
        }
        next = list_next(next);
    }
    list_add_before(next, &node->link);
    return;
}

void numberLirOp(struct LinearList* blocks) {
}

void __compute_loop_info_func(struct FuncTabElem* func) {
    int num = sizeDequeList(func->loop_blocks);
    for (int i = 0; i < num; i++) {
        struct LoopBlocks* lb = getDequeList(func, i);
        int b_num = sizeDequeList(lb->blocks);
        for (int j = 0; j < b_num; j++) {
            BASIC_BLOCK_TYPE* block = getDequeList(lb->blocks, j);
            block->block_LRA->loop_depth++;
        }
    }

    for (int i = 0; i < num; i++) {
        struct LoopBlocks* lb = getDequeList(func, i);
        int b_num = sizeDequeList(lb->blocks);
        BASIC_BLOCK_TYPE* entry = lb->loop_entry;
        for (int j = 0; j < b_num; j++) {
            BASIC_BLOCK_TYPE* block = getDequeList(lb->blocks, j);
            if (block->block_LRA->loop_depth == entry->block_LRA->loop_depth) {
                block->block_LRA->loop_index = entry;
            }
        }
    }
}

void computeLoopInfo() {
    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* func = getLinearList(func_table, i);
        if (func->blocks != NULL) {
            __compute_loop_info_func(func);
        }
    }
}

struct DequeList* getSuccessors(BlockBegin* block) {
    list_entry_t* head = &(block->block->successors->block_link);
    list_entry_t* next = list_next(head);
    struct DequeList* succ = newDequeList();
    while (head != next) {
        pushFrontDequeList(succ, le2BasicBlock(next)->value);
        next = list_next(next);
    }
    return succ;
}