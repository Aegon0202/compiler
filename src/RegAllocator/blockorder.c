#include "./blockorder.h"

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
