#ifndef SYSY_TARGET_OFFSET_H
#define SYSY_TARGET_OFFSET_H
#include "../SysY.symtab/SysY.symtab.def.h"
#include "../ssa/ssa.h"
#include "../utils/LinearList.h"
struct BlockRegOffset {
    BASIC_BLOCK_TYPE* basic_block;
    struct LinearList* reg_offset;  // index: reg_id value int* offset
    int now_offset;
};

struct FuncRegOffset {
    struct FuncTabElem* funcelem;
    int now_offset;
    int max_offset;
    struct LinearList* block_offsets;  // index: basic_block address value: BlockRegOffset
    struct LinearList* reg_offset;     // index: reg_id value int* offset
    struct LinearList* reg_2_users;    // index: reg_id value: struct Item <key: int* , value: DequeList/LinearList<BasicBlock*> user所在的基本快 >
    struct LinearList* reg_2_defs;     // index: reg_id value: struct Item <key: int* , value: DequeList/LinearList<BasicBlock*> def所在的基本快 >
};

struct FuncRegOffset* generatorRegOffset(struct FuncTabElem* elem);

#endif