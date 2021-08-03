#include "./RegAllocator/LRA.h"
#include "./SysY.symtab/SysY.symtab.def.h"
#include "./optimizer/local_op.h"
#include "./optimizer/peephole.h"
#include "./ssa/ssa.h"
// 进行所有必要的初始化。
void init() {
    id_list = newLinearList();
    reg_id_vartabelem = newLinearList();
    def_block = newLinearList();
    construct_Counter = newLinearList();
    construct_Stack = newLinearList();
    variable_bottom_index = newLinearList();
    bottom_index2New_reg = newLinearList();
    reg2def = newLinearList();

    block_2_rcfg_block = newLinearList();
    rcfg_block_2_block = newLinearList();

    simp_worklist = newDequeList();
    prop_worklist = newDequeList();
    constMark = newLinearList();
    constValue = newLinearList();
    constType = newLinearList();
    reg2Intival = newLinearList();
    init_all_table();
}