#ifndef REGALLOCATOR_BLOCK_ORDER_H
#define REGALLOCATOR_BLOCK_ORDER_H

#include "../ssa/ssa.h"
#include "../utils/DequeList.h"
#include "../utils/link.h"
typedef struct BasicBlock BasicBlock;

typedef struct BlockBegin {
    BasicBlock* block;
    int loop_index;
    int loop_depth;
    int first_op_id;
    int last_op_id;
} BlockBegin;

typedef struct BlockBeginNode {
    BlockBegin* value;
    list_entry_t link;
} BlockBeginNode;

//nimen
// 在所有函数分配之前只调用一次，遍历符号表，将所有的函数涉及到的循环信息计算出来
void computeLoopInfo();
//successor 是一个blockbegin*
struct DequeList* getSuccessors(BlockBegin* block);
void numberLirOp(struct LinearList* blocks);

//wo
struct DequeList* computeBlockOrder(BlockBegin* func_entry_block);
void insertBlock(BlockBegin* block, list_entry_t* work_list_head);
#endif