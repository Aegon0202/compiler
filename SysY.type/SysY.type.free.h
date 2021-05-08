#ifndef SYSY_TYPE_FREE_H
#define SYSY_TYPE_FREE_H
#include <stdio.h>
#include <stdlib.h>
#include "../utils/NullPointMacro.h"
#include "./SysY.type.visitor.h"
VisitorDeclGenerator(freeToken_t, freeToken, void, int);
#endif