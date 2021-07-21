#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SysY.symtab/SysY.symtab.ssa.h"
#include "SysY.type/SysY.type.def.h"
#include "SysY.type/SysY.type.free.h"
#include "SysY.type/SysY.type.new.h"
#include "SysY.type/SysY.type.print.h"
#include "SysY.type/SysY.type.visitor.h"
#include "parser/SysY.tab.h"

YYSTYPE result;

int main() {
    const int N[2][2] = {{1}, 2, 3};
    int* b = (int*)&(N[0][0]);
    //initPrintToken();
    // initFreeToken();
    init_all_table();
    yyparse();
    toSSACompUnit(result.compunit);
    struct VarTabElem* vte = (struct VarTabElem*)getLinearList(var_table->table, 0);
    int* t = vte->const_init_value;
    for (int i = 0; i < 4; i++) {
        assert(t[i] == b[i]);
    }
    printf("ok\n");
    //printToken(result.compunit, &printToken_ins);
    //toASTCompUnit(result.compunit);
    //printASTAll(funcsymtable_p->head);
    //printToken(result.compunit, printToken_ins);
    return 0;
}
