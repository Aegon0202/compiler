#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "./SysY.target.arm.h"

struct block_list {
    struct LinearList* list;
    int block_num;
};

void __add_block_2_list(BASIC_BLOCK_TYPE* block, void* args) {
    struct LinearList* block_2_list = (struct LinearList*)args;
    MALLOC(l, struct block_list, 1);
    setLinearList(l, l->block_num, block);
    l->block_num++;
    setLinearList(block_2_list, (size_t)block, l);
}

void __get_all_block(BASIC_BLOCK_TYPE* block, void* args) {
    BasicBlockNode* block_list = (BasicBlockNode*)args;
    MALLOC(node, BasicBlockNode, 1);
    node->value = block;
    list_add_before(&block_list->block_link, &node->block_link);
}

BasicBlockNode* convertToLinear(struct FuncTabElem* func) {
    BASIC_BLOCK_TYPE* entry = func->blocks;
    MALLOC(node, BasicBlockNode, 1);
    list_init(&node->block_link);
    node->value = NULL;
    deepTraverseSuccessorsBasicBlock(entry, __get_all_block, node);
    return node;
}