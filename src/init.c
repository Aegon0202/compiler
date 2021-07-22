#include "./SysY.symtab/SysY.symtab.def.h"
#include "./ssa/ssa.h"
// 进行所有必要的初始化。
void init() {
    id_list = newLinearList();
    init_all_table();
}