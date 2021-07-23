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
#include "ssa/ssa.h"

YYSTYPE result;
void init();

int main() {
    init();
    yyparse();
    toSSACompUnit(result.compunit);
    struct FuncTabElem* fte = getFuncTabElemByName("test", func_table);
    goThroughFunction(fte->blocks, __print_basic_block, NULL);
    //printToken(result.compunit, &printToken_ins);
    //toASTCompUnit(result.compunit);
    //printASTAll(funcsymtable_p->head);
    //printToken(result.compunit, printToken_ins);
    return 0;
}
