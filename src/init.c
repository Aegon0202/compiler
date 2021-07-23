#include "./SysY.symtab/SysY.symtab.def.h"
#include "./ssa/ssa.h"
// 进行所有必要的初始化。
void init() {
    id_list = newLinearList();
    reg_id_vartabelem = newLinearList();
    init_all_table();
}