#ifndef SSA_TRAVERSE_H
#define SSA_TRAVERSE_H

#include "../RegAllocator/LRA.h"
#include "./ssa.h"

void deepTraverseSuccessorsBasicBlock(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args);
void widthTraverseSuccessorsBasicBlock(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args);

void gothrough_BlockBeginNode_list(BlockBeginNode* block_list_head, void (*func)(BlockBegin*, void*), void* args);
void gothrough_BlockBeginNode_list_reverse(BlockBeginNode* block_list_head, void (*func)(BlockBegin*, void*), void* args);
#endif