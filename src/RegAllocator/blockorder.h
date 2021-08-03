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
    int last_op_id;  //data flow
    int cur_val_num;

    struct DequeList* block_live_in;
    struct DequeList* block_live_out;
    struct DequeList* block_live_gen;
    struct DequeList* block_live_kill;

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
struct DequeList* getPredecessors(BlockBegin* block);
void numberLirOp(struct DequeList* blocks);

//wo
struct DequeList* computeBlockOrder(BlockBegin* func_entry_block);
void insertBlock(BlockBegin* block, list_entry_t* work_list_head);
#endif