#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser/SysY.tab.hpp"
#include "SysY.type/SysY.type.def.hpp"
#include "SysY.type/SysY.type.new.hpp"
#include "SysY.type/SysY.type.visitor.hpp"
#include "SysY.type/SysY.type.print.hpp"

YYSTYPE result;

int main()
{
    initPrintToken();
    yyparse();
    printToken(result.compunit, printToken_p);
    return 0;
}
