#ifndef ENUM_H
#define ENUM_H
#include "./parser/SysY.tab.h"
enum {
    COMPUNIT = 500,
    DECL,
    CONSTDECL,
    BTYPE,
    CONSTDEFS,
    CONSTDEF,
    CONSTARRAYDEFS,
    CONSTARRAYDEF,
    CONSTINITVAL,
    CONSTINITVALS,
    VARDECL,
    VARDEFS,
    VARDEF,
    INITVAL,
    INITVALS,
    FUNCDEF,
    FUNCTYPE,
    FUNCFPARAMS,
    FUNCFPARAM,
    EXPARRAYDEFS,
    EXPARRAYDEF,
    BLOCK,
    BLOCKITEMS,
    BLOCKITEM,
    STMT,
    ASSIGN,
    IFSTMT,
    WHILESTMT,
    RETURNSTMT,
    BREAKSTMT,
    CONTINUESTMT,
    EXP,
    COND,
    LVAL,
    ARRAYIMPL,
    PRIMARYEXP,
    NUMBER,
    UNARYEXP,
    FUNCIMPL,
    UNARYEXPS,
    UNARYOP,
    FUNCRPARAMS,
    FUNCRPARAM,
    MULEXP,
    ADDEXP,
    ADDOP,
    RELEXP,
    EQEXP,
    LANDEXP,
    LOREXP,
    CONSTEXP,

    VARSYMENTRY = 600,
    FUNCSYMENTRY,
    FUNCSYMTABLE,
    VARSYMTABLE,

    LABELENTRY = 700,
    //LABELSYMTABLE,
    FUNCIMPLAST,
    ARRAYIMPLAST,
    EXPAST,
    OPREAND,

    PARAM = 800,
    CALL,
    JUMP,
    BRANCH,
    LOAD,
    STORE,
    NOP,

    ARRAY,
    FRAMEPOINT,
    STACKPOINT,
    GLOBALDATA
};

#endif