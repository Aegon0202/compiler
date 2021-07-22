#ifndef SYSY_TARGET_DEF_H
#define SYSY_TARGET_DEF_H

#include "../SysY.symtab/SysY.symtab.def.h"
#include "../ssa/ssa.h"
#include "../utils/LinearList.h"
#include "../utils/LinkedTable.h"

#define REG_ID_TYPE int

struct IrLevelActive {
    REG_ID_TYPE* reg;
    struct LinearList* block_line_active;  // index: block address value: int[2] begin_line, end_line
};

struct BlockLevelActive {
    BASIC_BLOCK_TYPE* basic_block;
    struct LinearList* in_block_reg_active;   // index: reg_id, value: IrLevelActive
    struct LinearList* out_block_reg_active;  // index: reg_id, value: IrLevelActive
    struct LinearList* block_reg_active;      // index: reg_id, value: IrLevelActive
    struct LinearList* total_reg_active;      // index: reg_id, value: IrLevelActive
};

struct FuncActive {
    struct FuncTabElem* funcelem;
    struct LinearList* linear_block;
    long long unsigned int block_next_index;
    struct LinearList* reg_active_table;  // key: reg_id value: IrLevelActive
};

#endif