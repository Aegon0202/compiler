#include "./blockorder.h"

#include "../utils/Malloc.h"
//to be finished
struct LinearList* computeBlockOrder(BlockBegin* func_entry_block) {
    int count = 0;
    MALLOC(work_list, BlockBeginNode, 1);
    work_list->value = 0;
    list_entry_t* work_list_head = &work_list->link;
    list_init(work_list_head);
    MALLOC(node, BlockBeginNode, 1);
    node->value = func_entry_block;
    list_add(work_list_head, &node->link);

    struct LinearList* blocks = newLinearList();
    struct DequeList* successors = NULL;

    while (!list_empty(work_list_head)) {
        list_entry_t* tail = list_prev(work_list_head);
        list_del(tail);
        BlockBeginNode* bNode = le2struct(tail, BlockBeginNode, link);
        BlockBegin* b = bNode->value;
        successors = getSuccessors(b);
        setLinearList(blocks, count, b);
        count++;
        for (; !isEmptyDequeList(successors);) {
            BlockBegin* bb = (BlockBegin*)popBackDequeList(successors);
            insertBlock(bb, work_list_head);
        }
        freeDequeList(&successors);
    }
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
            list_add_before(next, &node->link);
        }
        next = list_next(next);
    }
    return;
}

void numberLirOp(struct LinearList* blocks) {
    int next_id = 0;
}