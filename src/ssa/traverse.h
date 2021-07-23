#ifndef SSA_TRAVERSE_H
#define SSA_TRAVERSE_H

#include "./ssa.h"
void deepTraverseSuccessorsBasicBlock(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args);
void widthTraverseSuccessorsBasicBlock(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args);
#endif