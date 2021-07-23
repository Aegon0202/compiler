#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SysY.symtab/SysY.symtab.ssa.h"
#include "SysY.target/SysY.target.arm.h"
#include "SysY.type/SysY.type.def.h"
#include "parser/SysY.tab.h"
YYSTYPE result;
void init();

int main() {
    init();
    yyparse();
    toSSACompUnit(result.compunit);
    generateAllToOutFile(stdout);
    //printToken(result.compunit, &printToken_ins);
    //toASTCompUnit(result.compunit);
    //printASTAll(funcsymtable_p->head);
    //printToken(result.compunit, printToken_ins);
    return 0;
}
