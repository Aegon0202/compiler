#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flex.bison/SysY.tab.h"
#include "SysY.type/SysY.type.def.h"
#include "SysY.type/SysY.type.new.h"
#include "SysY.type/SysY.type.visitor.h"
#include "SysY.type/SysY.type.print.h"

YYSTYPE result;

int main()
{
    initPrintToken();
    yyparse();
    printToken(result.compunit, printToken_p);
    return 0;
}