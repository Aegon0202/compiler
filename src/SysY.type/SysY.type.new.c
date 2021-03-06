#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parser/SysY.tab.h"
#include "../utils/NullPointMacro.h"
#include "../utils/PrintHelper.h"
#include "./SysY.type.def.h"
extern int yylineno;

//YYSTYPE yylval;

#define ListLikeNewGenerator1(funcname, listype, datatype1, dataname1) \
    listype *funcname(int type, datatype1 dataname1, listype *prev) {  \
        listype *nrtp = (listype *)malloc(sizeof(listype));            \
        EnsureNotNull(nrtp);                                           \
        nrtp->type = type;                                             \
        nrtp->dataname1 = dataname1;                                   \
        if (prev == NULL) {                                            \
            nrtp->prev = nrtp;                                         \
            nrtp->next = nrtp;                                         \
        } else {                                                       \
            nrtp->prev = prev;                                         \
            nrtp->next = prev->next;                                   \
            prev->next->prev = nrtp;                                   \
            prev->next = nrtp;                                         \
        }                                                              \
        return nrtp;                                                   \
    }

#define ListLikeNewGenerator2(funcname, listype, datatype1, dataname1, datatype2, dataname2) \
    listype *funcname(int type, datatype1 dataname1, datatype2 dataname2, listype *prev) {   \
        listype *nrtp = (listype *)malloc(sizeof(listype));                                  \
        EnsureNotNull(nrtp);                                                                 \
        nrtp->type = type;                                                                   \
        nrtp->dataname1 = dataname1;                                                         \
        nrtp->dataname2 = dataname2;                                                         \
        if (prev == NULL) {                                                                  \
            nrtp->prev = nrtp;                                                               \
            nrtp->next = nrtp;                                                               \
        } else {                                                                             \
            nrtp->prev = prev;                                                               \
            nrtp->next = prev->next;                                                         \
            prev->next->prev = nrtp;                                                         \
            prev->next = nrtp;                                                               \
        }                                                                                    \
        return nrtp;                                                                         \
    }

#define OnlyDataNewGenerator1(funcname, returntype, datatype1, dataname1) \
    returntype *funcname(int type, datatype1 dataname1) {                 \
        returntype *rt = (returntype *)malloc(sizeof(returntype));        \
        EnsureNotNull(rt);                                                \
        rt->type = type;                                                  \
        rt->dataname1 = dataname1;                                        \
        return rt;                                                        \
    }

#define OnlyDataNewGenerator2(funcname, returntype, datatype1, dataname1, datatype2, dataname2) \
    returntype *funcname(int type, datatype1 dataname1, datatype2 dataname2) {                  \
        returntype *rt = (returntype *)malloc(sizeof(returntype));                              \
        EnsureNotNull(rt);                                                                      \
        rt->type = type;                                                                        \
        rt->dataname1 = dataname1;                                                              \
        rt->dataname2 = dataname2;                                                              \
        return rt;                                                                              \
    }

#define OnlyDataNewGenerator3(funcname, returntype, datatype1, dataname1, datatype2, dataname2, datatype3, dataname3) \
    returntype *funcname(int type, datatype1 dataname1, datatype2 dataname2, datatype3 dataname3) {                   \
        returntype *rt = (returntype *)malloc(sizeof(returntype));                                                    \
        EnsureNotNull(rt);                                                                                            \
        rt->type = type;                                                                                              \
        rt->dataname1 = dataname1;                                                                                    \
        rt->dataname2 = dataname2;                                                                                    \
        rt->dataname3 = dataname3;                                                                                    \
        return rt;                                                                                                    \
    }

#define OnlyDataNewGenerator4(funcname, returntype, datatype1, dataname1, datatype2, dataname2, datatype3, dataname3, datatype4, dataname4) \
    returntype *funcname(int type, datatype1 dataname1, datatype2 dataname2, datatype3 dataname3, datatype4 dataname4) {                    \
        returntype *rt = (returntype *)malloc(sizeof(returntype));                                                                          \
        EnsureNotNull(rt);                                                                                                                  \
        rt->type = type;                                                                                                                    \
        rt->dataname1 = dataname1;                                                                                                          \
        rt->dataname2 = dataname2;                                                                                                          \
        rt->dataname3 = dataname3;                                                                                                          \
        rt->dataname4 = dataname4;                                                                                                          \
        return rt;                                                                                                                          \
    }

#define OnlyUnionNewGenerator(funcname, returntype, value_in_union_type, value_in_union_name) \
    returntype *funcname(int type, int valuetype, void *value_in_union_name) {                \
        returntype *rt = (returntype *)malloc(sizeof(returntype));                            \
        EnsureNotNull(rt);                                                                    \
        rt->type = type;                                                                      \
        rt->valuetype = valuetype;                                                            \
        rt->value.value_in_union_name = (value_in_union_type)value_in_union_name;             \
        return rt;                                                                            \
    }

struct Keyword *newKeyword(int type, int keytype, const char *keyword) {
    struct Keyword *kp = (struct Keyword *)malloc(sizeof(struct Keyword));
    EnsureNotNull(kp);
    kp->type = type;
    kp->keytype = keytype;
    kp->value = strdup(keyword);
    return kp;
}

struct IntConst *newIntConst(int type, char *number) {
    struct IntConst *intconst = (struct IntConst *)malloc(sizeof(struct IntConst));
    EnsureNotNull(intconst);
    int sum = 0, base = 10, n, k = 0;
    if (number[0] == '0') {
        k = 1;
        base = 8;
        if (number[1] == 'X' || number[1] == 'x') {
            k = 2;
            base = 16;
        }
    }
    while ((n = number[k])) {
        if (n >= '0' && n <= '9') {
            n = n - '0';
        } else if (n >= 'A' && n <= 'F') {
            n = n - 'A' + 10;
        } else {
            n = n - 'a' + 10;
        }
        sum = sum * base + n;
        k++;
    }
    intconst->type = type;
    intconst->value = sum;
    return intconst;
}

struct CompUnit *newCompUnit(int type, int valuetype, void *value, struct CompUnit *reducecp) {
    struct CompUnit *cp = (struct CompUnit *)malloc(sizeof(struct CompUnit));
    EnsureNotNull(cp);
    cp->type = type;
    cp->valuetype = valuetype;
    cp->value.decl = (struct Decl *)value;

    if (reducecp == NULL) {
        cp->next = cp;
        cp->prev = cp;
    } else {
        cp->prev = reducecp;
        cp->next = reducecp->next;
        reducecp->next->prev = cp;
        reducecp->next = cp;
    }
    return cp;
}

OnlyUnionNewGenerator(newDecl, struct Decl, struct ConstDecl *, constdecl);

OnlyDataNewGenerator2(newConstDecl, struct ConstDecl, struct BType *, btype, struct ConstDefs *, constdefs);

OnlyDataNewGenerator1(newBType, struct BType, int, typevalue);

ListLikeNewGenerator1(newConstDefs, struct ConstDefs, struct ConstDef *, constdef);

OnlyDataNewGenerator3(newConstDef, struct ConstDef, struct Ident *, ident, struct ConstArrayDefs *, constarraydefs, struct ConstInitVal *, constinitval);

ListLikeNewGenerator1(newConstArrayDefs, struct ConstArrayDefs, struct ConstArrayDef *, constarraydef);

OnlyDataNewGenerator1(newConstArrayDef, struct ConstArrayDef, struct ConstExp *, constexp);

OnlyUnionNewGenerator(newConstInitVal, struct ConstInitVal, struct ConstExp *, constexp);

ListLikeNewGenerator1(newConstInitVals, struct ConstInitVals, struct ConstInitVal *, constinitval);

OnlyDataNewGenerator2(newVarDecl, struct VarDecl, struct BType *, btype, struct VarDefs *, vardefs);

ListLikeNewGenerator1(newVarDefs, struct VarDefs, struct VarDef *, vardef);

OnlyDataNewGenerator3(newVarDef, struct VarDef, struct Ident *, ident, struct ConstArrayDefs *, constarraydefs, struct InitVal *, initval);

OnlyUnionNewGenerator(newInitVal, struct InitVal, struct Exp *, exp);

ListLikeNewGenerator1(newInitVals, struct InitVals, struct InitVal *, initval);

OnlyDataNewGenerator4(newFuncDef, struct FuncDef, struct FuncType *, functype, struct Ident *, ident, struct FuncFParams *, funcfparams, struct Block *, block);

OnlyDataNewGenerator1(newFuncType, struct FuncType, int, typevalue);

ListLikeNewGenerator1(newFuncFParams, struct FuncFParams, struct FuncFParam *, funcfparam);

OnlyDataNewGenerator3(newFuncFParam, struct FuncFParam, struct BType *, btype, struct Ident *, ident, struct ExpArrayDefs *, exparraydefs);

ListLikeNewGenerator1(newExpArrayDefs, struct ExpArrayDefs, struct ExpArrayDef *, exparraydef);

OnlyDataNewGenerator1(newExpArrayDef, struct ExpArrayDef, struct Exp *, exp);

OnlyDataNewGenerator1(newBlock, struct Block, struct BlockItems *, blockitems);

ListLikeNewGenerator1(newBlockItems, struct BlockItems, struct BlockItem *, blockitem);

OnlyUnionNewGenerator(newBlockItem, struct BlockItem, struct Decl *, decl);

OnlyUnionNewGenerator(newStmt, struct Stmt, struct Assign *, assign);

OnlyDataNewGenerator2(newAssign, struct Assign, struct LVal *, lval, struct Exp *, exp);

OnlyDataNewGenerator3(newIfStmt, struct IfStmt, struct Cond *, cond, struct Stmt *, then, struct Stmt *, otherwise);

OnlyDataNewGenerator2(newWhileStmt, struct WhileStmt, struct Cond *, cond, struct Stmt *, stmt);

OnlyDataNewGenerator1(newReturnStmt, struct ReturnStmt, struct Exp *, exp);

OnlyDataNewGenerator1(newExp, struct Exp, struct AddExp *, addexp);

OnlyDataNewGenerator1(newCond, struct Cond, struct LOrExp *, lorexp);

OnlyUnionNewGenerator(newLVal, struct LVal, struct Ident *, ident);

OnlyDataNewGenerator2(newArrayImpl, struct ArrayImpl, struct Ident *, ident, struct ExpArrayDefs *, exparraydefs);

OnlyUnionNewGenerator(newPrimaryExp, struct PrimaryExp, struct Exp *, exp);

OnlyDataNewGenerator1(newNumber, struct Number, struct IntConst *, intconst);

OnlyUnionNewGenerator(newUnaryExp, struct UnaryExp, struct PrimaryExp *, primaryexp);

OnlyDataNewGenerator2(newUnaryExps, struct UnaryExps, struct UnaryOp *, unaryop, struct UnaryExp *, unaryexp);

OnlyDataNewGenerator1(newUnaryOp, struct UnaryOp, int, typevalue);

ListLikeNewGenerator1(newFuncRParams, struct FuncRParams, struct FuncRParam *, funcrparam);

OnlyUnionNewGenerator(newFuncRParam, struct FuncRParam, struct Exp *, exp);

ListLikeNewGenerator2(newMulExp, struct MulExp, struct MulOp *, mulop, struct UnaryExp *, unaryexp);

OnlyDataNewGenerator1(newMulOp, struct MulOp, int, typevalue);

ListLikeNewGenerator2(newAddExp, struct AddExp, struct AddOp *, addop, struct MulExp *, mulexp);

OnlyDataNewGenerator1(newAddOp, struct AddOp, int, typevalue);

ListLikeNewGenerator2(newRelExp, struct RelExp, struct RelOp *, relop, struct AddExp *, addexp);

OnlyDataNewGenerator1(newRelOp, struct RelOp, int, typevalue);

ListLikeNewGenerator2(newEqExp, struct EqExp, struct EqOp *, eqop, struct RelExp *, relexp);

OnlyDataNewGenerator1(newEqOp, struct EqOp, int, typevalue);

ListLikeNewGenerator1(newLAndExp, struct LAndExp, struct EqExp *, eqexp);

ListLikeNewGenerator1(newLOrExp, struct LOrExp, struct LAndExp *, landexp);

OnlyDataNewGenerator1(newConstExp, struct ConstExp, struct AddExp *, addexp);

struct Ident *newIdent(int type, const char *name) {
    struct Ident *ident_p = (struct Ident *)malloc(sizeof(struct Ident));
    EnsureNotNull(ident_p);
    ident_p->type = type;
    ident_p->name = strdup(name);
    return ident_p;
}

struct String *newString(int type, const char *content) {
    struct String *sp = (struct String *)malloc(sizeof(struct String));
    EnsureNotNull(sp);
    sp->type = type;
    sp->content = strdup(content);
    return sp;
}

struct FuncImpl *newFuncImpl(int type, struct Ident *ident, struct FuncRParams *funcrparams) {
    struct FuncImpl *rt = (struct FuncImpl *)malloc(sizeof(struct FuncImpl));
    static char lineno_buf[64];
    snprintf(lineno_buf, 64, "%d", yylineno);
    EnsureNotNull(rt);
    rt->type = type;
    rt->ident = ident;
    if (strcmp(ident->name, "starttime") == 0) {
        ident->name = strdup("_sysy_starttime");
        funcrparams->funcrparam = newFuncRParam(
            FUNCRPARAM, EXP, newExp(EXP, newAddExp(ADDEXP, NULL, newMulExp(MULEXP, NULL, newUnaryExp(UNARYEXP, PRIMARYEXP, newPrimaryExp(PRIMARYEXP, NUMBER, newNumber(NUMBER, newIntConst(INTCONST, lineno_buf)))), NULL), NULL)));
    } else if (strcmp(ident->name, "stoptime") == 0) {
        ident->name = strdup("_sysy_stoptime");
        funcrparams->funcrparam = newFuncRParam(
            FUNCRPARAM, EXP, newExp(EXP, newAddExp(ADDEXP, NULL, newMulExp(MULEXP, NULL, newUnaryExp(UNARYEXP, PRIMARYEXP, newPrimaryExp(PRIMARYEXP, NUMBER, newNumber(NUMBER, newIntConst(INTCONST, lineno_buf)))), NULL), NULL)));
    }
    rt->funcrparams = funcrparams;
    return rt;
}

void yyerror(char const *s) {
    PrintErrExit("%s\n", s);
}
