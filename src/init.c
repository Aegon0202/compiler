#include "./SysY.symtab/SysY.symtab.def.h"
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
    init_all_table();
}