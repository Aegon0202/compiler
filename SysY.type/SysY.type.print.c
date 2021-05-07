#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SysY.tab.h"
#include "SysY.type.def.h"
#include "SysY.type.new.h"
#include "SysY.type.visitor.h"
typedef struct printToken printToken_t;
struct printToken *printToken;

void printTokenInit()
{
    printToken = malloc(sizeof(printToken_t));
}