#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SysY.AST/SysY.AST.print.h"
#include "SysY.type/SysY.type.ast.h"
#include "SysY.type/SysY.type.def.h"
#include "SysY.type/SysY.type.free.h"
#include "SysY.type/SysY.type.new.h"
#include "SysY.type/SysY.type.print.h"
#include "SysY.type/SysY.type.visitor.h"
#include "parser/SysY.tab.h"

YYSTYPE result;

int main() {
    //initPrintToken();
    initFreeToken();
    initSymTable();
    yyparse();
    printToken(result.compunit, &printToken_ins);
    //toASTCompUnit(result.compunit);
    //printASTAll(funcsymtable_p->head);
    //printToken(result.compunit, printToken_ins);
    return 0;
}
