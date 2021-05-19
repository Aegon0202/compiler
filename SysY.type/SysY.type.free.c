#include "./SysY.type.free.h"
VisitorFuncImplGenerator(freeToken_t, freeToken, void, int recusive)(recusive);
struct freeToken_t *freeToken_p;

#define ListLikeFreeGenerator1(funcname, listype, dataname1) \
    void funcname(listype *list, int recusive)               \
    {                                                        \
        IfNull(list, return;);                               \
        listype *head = list;                                \
        if (recusive)                                        \
        {                                                    \
            while (head->next != head)                       \
            {                                                \
                listype *tmp = head->next;                   \
                head->next = tmp->next;                      \
                freeToken(tmp->dataname1, freeToken_p, 1);   \
                free(tmp);                                   \
            }                                                \
            freeToken(head->dataname1, freeToken_p, 1);      \
        }                                                    \
        free(head);                                          \
    }

#define ListLikeFreeGenerator2(funcname, listype, dataname1, dataname2) \
    void funcname(listype *list, int recusive)                          \
    {                                                                   \
        IfNull(list, return;);                                          \
        listype *head = list;                                           \
        if (recusive)                                                   \
        {                                                               \
            while (head->next != head)                                  \
            {                                                           \
                listype *tmp = head->next;                              \
                head->next = tmp->next;                                 \
                freeToken(tmp->dataname1, freeToken_p, 1);              \
                freeToken(tmp->dataname2, freeToken_p, 1);              \
                free(tmp);                                              \
            }                                                           \
            freeToken(head->dataname1, freeToken_p, 1);                 \
            freeToken(head->dataname2, freeToken_p, 1);                 \
        }                                                               \
        free(head);                                                     \
    }

#define OnlyDataFreeGenerator1(funcname, freetype, dataname1) \
    void funcname(freetype *token, int recusive)              \
    {                                                         \
        IfNull(token, return;);                               \
        if (recusive)                                         \
        {                                                     \
            freeToken(token->dataname1, freeToken_p, 1);      \
        }                                                     \
        free(token);                                          \
    }

#define OnlyDataFreeGenerator2(funcname, freetype, dataname1, dataname2) \
    void funcname(freetype *token, int recusive)                         \
    {                                                                    \
        IfNull(token, return;);                                          \
        if (recusive)                                                    \
        {                                                                \
            freeToken(token->dataname1, freeToken_p, 1);                 \
            freeToken(token->dataname2, freeToken_p, 1);                 \
        }                                                                \
        free(token);                                                     \
    }

#define OnlyDataFreeGenerator3(funcname, freetype, dataname1, dataname2, dataname3) \
    void funcname(freetype *token, int recusive)                                    \
    {                                                                               \
        IfNull(token, return;);                                                     \
        if (recusive)                                                               \
        {                                                                           \
            freeToken(token->dataname1, freeToken_p, 1);                            \
            freeToken(token->dataname2, freeToken_p, 1);                            \
            freeToken(token->dataname3, freeToken_p, 1);                            \
        }                                                                           \
        free(token);                                                                \
    }

#define OnlyDataFreeGenerator4(funcname, freetype, dataname1, dataname2, dataname3, dataname4) \
    void funcname(freetype *token, int recusive)                                               \
    {                                                                                          \
        IfNull(token, return;);                                                                \
        if (recusive)                                                                          \
        {                                                                                      \
            freeToken(token->dataname1, freeToken_p, 1);                                       \
            freeToken(token->dataname2, freeToken_p, 1);                                       \
            freeToken(token->dataname3, freeToken_p, 1);                                       \
            freeToken(token->dataname4, freeToken_p, 1);                                       \
        }                                                                                      \
        free(token);                                                                           \
    }

#define OnlyUnionFreeGenerator(funcname, freetype, value_in_union_name)  \
    void funcname(freetype *token, int recusive)                         \
    {                                                                    \
        IfNull(token, return;);                                          \
        if (recusive)                                                    \
        {                                                                \
            freeToken(token->value.value_in_union_name, freeToken_p, 1); \
        }                                                                \
        free(token);                                                     \
    }

void freeKeyword(struct Keyword *keyword, int recusive)
{
    IfNull(keyword, return;);
    if (recusive)
    {
        free(keyword->value);
    }
    free(keyword);
}

void freeIntConst(struct IntConst *intconst, int recusive)
{
    IfNull(intconst, return;);
    free(intconst);
}

void freeCompUnit(struct CompUnit *cp, int recusive)
{
    IfNull(cp, return;);
    struct CompUnit *head = cp;
    if (recusive)
    {
        while (head->next != head)
        {
            struct CompUnit *tmp = head;
            head->next = tmp->next;
            freeToken(tmp->value.decl, freeToken_p, 1);
            free(tmp);
        }
        free(cp->value.decl);
    }
    free(head);
}

OnlyUnionFreeGenerator(freeDecl, struct Decl, constdecl);

OnlyDataFreeGenerator2(freeConstDecl, struct ConstDecl, btype, constdefs);

void freeBType(struct BType *btype, int recusive)
{
    IfNull(btype, return;);
    free(btype);
}

ListLikeFreeGenerator1(freeConstDefs, struct ConstDefs, constdef);

OnlyDataFreeGenerator3(freeConstDef, struct ConstDef, ident, constarraydefs, constinitval);

ListLikeFreeGenerator1(freeConstArrayDefs, struct ConstArrayDefs, constarraydef);

OnlyDataFreeGenerator1(freeConstArrayDef, struct ConstArrayDef, constexp);

OnlyUnionFreeGenerator(freeConstInitVal, struct ConstInitVal, constexp);

ListLikeFreeGenerator1(freeConstInitVals, struct ConstInitVals, constinitval);

OnlyDataFreeGenerator2(freeVarDecl, struct VarDecl, btype, vardefs);

ListLikeFreeGenerator1(freeVarDefs, struct VarDefs, vardef);

OnlyDataFreeGenerator3(freeVarDef, struct VarDef, ident, constarraydefs, initval);

OnlyUnionFreeGenerator(freeInitVal, struct InitVal, exp);

ListLikeFreeGenerator1(freeInitVals, struct InitVals, initval);

OnlyDataFreeGenerator4(freeFuncDef, struct FuncDef, functype, ident, funcfparams, block);

void freeFuncType(struct FuncType *functype, int recusive)
{
    IfNull(functype, return;);
    free(functype);
}

ListLikeFreeGenerator1(freeFuncFParams, struct FuncFParams, funcfparam);

OnlyDataFreeGenerator3(freeFuncFParam, struct FuncFParam, btype, ident, exparraydefs);

ListLikeFreeGenerator1(freeExpArrayDefs, struct ExpArrayDefs, exparraydef);

OnlyDataFreeGenerator1(freeExpArrayDef, struct ExpArrayDef, exp);

OnlyDataFreeGenerator1(freeBlock, struct Block, blockitems);

ListLikeFreeGenerator1(freeBlockItems, struct BlockItems, blockitem);

OnlyUnionFreeGenerator(freeBlockItem, struct BlockItem, decl);

OnlyUnionFreeGenerator(freeStmt, struct Stmt, assign);

OnlyDataFreeGenerator2(freeAssign, struct Assign, lval, exp);

OnlyDataFreeGenerator3(freeIfStmt, struct IfStmt, cond, then, otherwise);

OnlyDataFreeGenerator2(freeWhileStmt, struct WhileStmt, cond, stmt);

OnlyDataFreeGenerator1(freeReturnStmt, struct ReturnStmt, exp);

OnlyDataFreeGenerator1(freeExp, struct Exp, addexp);

OnlyDataFreeGenerator1(freeCond, struct Cond, lorexp);

OnlyUnionFreeGenerator(freeLVal, struct LVal, ident);

OnlyDataFreeGenerator2(freeArrayImpl, struct ArrayImpl, ident, exparraydefs);

OnlyUnionFreeGenerator(freePrimaryExp, struct PrimaryExp, exp);

OnlyDataFreeGenerator1(freeNumber, struct Number, intconst);

OnlyUnionFreeGenerator(freeUnaryExp, struct UnaryExp, primaryexp);

OnlyDataFreeGenerator2(freeFuncImpl, struct FuncImpl, ident, funcrparams);

OnlyDataFreeGenerator2(freeUnaryExps, struct UnaryExps, unaryop, unaryexp);

void freeUnaryOp(struct UnaryOp *op, int recusive)
{
    IfNull(op, return;);
    free(op);
}

ListLikeFreeGenerator1(freeFuncRParams, struct FuncRParams, funcrparam);

OnlyUnionFreeGenerator(freeFuncRParam, struct FuncRParam, exp);

ListLikeFreeGenerator2(freeMulExp, struct MulExp, mulop, unaryexp);

void freeMulOp(struct MulOp *op, int recusive)
{
    IfNull(op, return;);
    free(op);
}

ListLikeFreeGenerator2(freeAddExp, struct AddExp, addop, mulexp);

void freeAddOp(struct AddOp *op, int recusive)
{
    IfNull(op, return;);
    free(op);
}

ListLikeFreeGenerator2(freeRelExp, struct RelExp, relop, addexp);

void freeRelOp(struct RelOp *op, int recusive)
{
    IfNull(op, return;);
    free(op);
}

ListLikeFreeGenerator2(freeEqExp, struct EqExp, eqop, relexp);

void freeEqOp(struct EqOp *op, int recusive)
{
    IfNull(op, return;);
    free(op);
}

ListLikeFreeGenerator1(freeLAndExp, struct LAndExp, eqexp);

ListLikeFreeGenerator1(freeLOrExp, struct LOrExp, landexp);

OnlyDataFreeGenerator1(freeConstExp, struct ConstExp, addexp);

void freeIdent(struct Ident *ident, int recusive)
{
    IfNull(ident, return;);
    if (recusive)
    {
        free(ident->name);
    }
    free(ident);
}

void freeString(struct String *string, int recusive)
{
    IfNull(string, return;);
    if (recusive)
    {
        free(string->content);
    }
    free(string);
}

void initFreeToken()
{
    freeToken_p = malloc(sizeof(struct freeToken_t));
    EnsureNotNull(freeToken_p);
    freeToken_p->visitKeyword = freeKeyword;
    freeToken_p->visitIntConst = freeIntConst;
    freeToken_p->visitCompUnit = freeCompUnit;
    freeToken_p->visitDecl = freeDecl;
    freeToken_p->visitConstDecl = freeConstDecl;
    freeToken_p->visitBType = freeBType;
    freeToken_p->visitConstDefs = freeConstDefs;
    freeToken_p->visitConstDef = freeConstDef;
    freeToken_p->visitConstArrayDefs = freeConstArrayDefs;
    freeToken_p->visitConstArrayDef = freeConstArrayDef;
    freeToken_p->visitConstInitVal = freeConstInitVal;
    freeToken_p->visitConstInitVals = freeConstInitVals;
    freeToken_p->visitVarDecl = freeVarDecl;
    freeToken_p->visitVarDefs = freeVarDefs;
    freeToken_p->visitVarDef = freeVarDef;
    freeToken_p->visitInitVal = freeInitVal;
    freeToken_p->visitInitVals = freeInitVals;
    freeToken_p->visitFuncDef = freeFuncDef;
    freeToken_p->visitFuncType = freeFuncType;
    freeToken_p->visitFuncFParams = freeFuncFParams;
    freeToken_p->visitFuncFParam = freeFuncFParam;
    freeToken_p->visitExpArrayDefs = freeExpArrayDefs;
    freeToken_p->visitExpArrayDef = freeExpArrayDef;
    freeToken_p->visitBlock = freeBlock;
    freeToken_p->visitBlockItems = freeBlockItems;
    freeToken_p->visitBlockItem = freeBlockItem;
    freeToken_p->visitStmt = freeStmt;
    freeToken_p->visitAssign = freeAssign;
    freeToken_p->visitIfStmt = freeIfStmt;
    freeToken_p->visitWhileStmt = freeWhileStmt;
    freeToken_p->visitReturnStmt = freeReturnStmt;
    freeToken_p->visitExp = freeExp;
    freeToken_p->visitCond = freeCond;
    freeToken_p->visitLVal = freeLVal;
    freeToken_p->visitArrayImpl = freeArrayImpl;
    freeToken_p->visitPrimaryExp = freePrimaryExp;
    freeToken_p->visitNumber = freeNumber;
    freeToken_p->visitUnaryExp = freeUnaryExp;
    freeToken_p->visitFuncImpl = freeFuncImpl;
    freeToken_p->visitUnaryExps = freeUnaryExps;
    freeToken_p->visitUnaryOp = freeUnaryOp;
    freeToken_p->visitFuncRParams = freeFuncRParams;
    freeToken_p->visitFuncRParam = freeFuncRParam;
    freeToken_p->visitMulExp = freeMulExp;
    freeToken_p->visitMulOp = freeMulOp;
    freeToken_p->visitAddExp = freeAddExp;
    freeToken_p->visitAddOp = freeAddOp;
    freeToken_p->visitRelExp = freeRelExp;
    freeToken_p->visitRelOp = freeRelOp;
    freeToken_p->visitEqExp = freeEqExp;
    freeToken_p->visitEqOp = freeEqOp;
    freeToken_p->visitLAndExp = freeLAndExp;
    freeToken_p->visitLOrExp = freeLOrExp;
    freeToken_p->visitConstExp = freeConstExp;
    freeToken_p->visitIdent = freeIdent;
    freeToken_p->visitString = freeString;
}