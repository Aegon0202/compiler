#ifndef SYSY_TARGET_DEF_H
#define SYSY_TARGET_DEF_H

#include "../SysY.symtab/SysY.symtab.def.h"
#include "../ssa/ssa.h"
#include "../utils/LinearList.h"
#include "../utils/LinkedTable.h"

#define REG_ID_TYPE int

struct ActiveStatus {
    int status;  // NOT_ON_STACK ON_STACK MUST_ON_STACK
    int action;  // READ_REG WRITE_REG NOP
    int line_num;
    BASIC_BLOCK_TYPE* basic_block;
};

struct IrLevelActive {
    REG_ID_TYPE reg;
    struct LinearList* block_line_active;  // index: basic_block address value: LinearList<index: line_num value: status>
};

struct BlockLevelActive {
    BASIC_BLOCK_TYPE* basic_block;
    int ir_total_num;
    struct LinearList* in_block_reg_active;   // index: reg_id, value: IrLevelActive
    struct LinearList* out_block_reg_active;  // index: reg_id, value: IrLevelActive
    struct LinearList* def_in_block;          // index: reg_id, value: IrLevelActive
    struct LinearList* total_reg;
    int total_reg_num;  // index: reg_id, value: IrLevelActive
};

struct FuncActive {
    struct FuncTabElem* funcelem;
    struct LinearList* all_block_active;
    long long unsigned int block_next_index;
    struct LinearList* basic_block_active_table;  // index: basic_block address value: BlockLevelActive
    struct LinearList* reg_active;                // index: reg_id value: IrLevelActive
};

#endif