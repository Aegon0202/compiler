#ifndef ENUM_H
#define ENUM_H
#include "./parser/SysY.tab.h"

#define INT_SIZE 4

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

    //operand type
    PHI,
    INT,
    ConstSTRING,
    FUNCID,
    REGISTER,
    FRAMEPOINT,
    STACKPOINT,
    GLOBALDATA,
    ADDRESS,
    BASIC_BLOCK,

    NOT_ON_STACK,
    ON_STACK,
    MUST_ON_STACK,
    READ_REG,
    WRITE_REG,

    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15
};

#endif
