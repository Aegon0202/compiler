#include "./SysY.type.free.h"
#include "../utils/PrintHelper.h"
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
    freeToken_p = (struct freeToken_t *)malloc(sizeof(struct freeToken_t));
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

void freeToken(void *token, struct freeToken_t *visitor, int recusive)
{
    EnsureNotNull(token);
    EnsureNotNull(visitor);
    switch (*(int *)token)
    {
    case KEYWORD:
        EnsureNotNull(visitor->visitKeyword);
        return visitor->visitKeyword((struct Keyword *)token, recusive);
    case INTCONST:
        EnsureNotNull(visitor->visitIntConst);
        return visitor->visitIntConst((struct IntConst *)token, recusive);
    case COMPUNIT:
        EnsureNotNull(visitor->visitCompUnit);
        return visitor->visitCompUnit((struct CompUnit *)token, recusive);
    case DECL:
        EnsureNotNull(visitor->visitDecl);
        return visitor->visitDecl((struct Decl *)token, recusive);
    case CONSTDECL:
        EnsureNotNull(visitor->visitConstDecl);
        return visitor->visitConstDecl((struct ConstDecl *)token, recusive);
    case BTYPE:
        EnsureNotNull(visitor->visitBType);
        return visitor->visitBType((struct BType *)token, recusive);
    case CONSTDEFS:
        EnsureNotNull(visitor->visitConstDefs);
        return visitor->visitConstDefs((struct ConstDefs *)token, recusive);
    case CONSTDEF:
        EnsureNotNull(visitor->visitConstDef);
        return visitor->visitConstDef((struct ConstDef *)token, recusive);
    case CONSTARRAYDEFS:
        EnsureNotNull(visitor->visitConstArrayDefs);
        return visitor->visitConstArrayDefs((struct ConstArrayDefs *)token, recusive);
    case CONSTARRAYDEF:
        EnsureNotNull(visitor->visitConstArrayDef);
        return visitor->visitConstArrayDef((struct ConstArrayDef *)token, recusive);
    case CONSTINITVAL:
        EnsureNotNull(visitor->visitConstInitVal);
        return visitor->visitConstInitVal((struct ConstInitVal *)token, recusive);
    case CONSTINITVALS:
        EnsureNotNull(visitor->visitConstInitVals);
        return visitor->visitConstInitVals((struct ConstInitVals *)token, recusive);
    case VARDECL:
        EnsureNotNull(visitor->visitVarDecl);
        return visitor->visitVarDecl((struct VarDecl *)token, recusive);
    case VARDEFS:
        EnsureNotNull(visitor->visitVarDefs);
        return visitor->visitVarDefs((struct VarDefs *)token, recusive);
    case VARDEF:
        EnsureNotNull(visitor->visitVarDef);
        return visitor->visitVarDef((struct VarDef *)token, recusive);
    case INITVAL:
        EnsureNotNull(visitor->visitInitVal);
        return visitor->visitInitVal((struct InitVal *)token, recusive);
    case INITVALS:
        EnsureNotNull(visitor->visitInitVals);
        return visitor->visitInitVals((struct InitVals *)token, recusive);
    case FUNCDEF:
        EnsureNotNull(visitor->visitFuncDef);
        return visitor->visitFuncDef((struct FuncDef *)token, recusive);
    case FUNCTYPE:
        EnsureNotNull(visitor->visitFuncType);
        return visitor->visitFuncType((struct FuncType *)token, recusive);
    case FUNCFPARAMS:
        EnsureNotNull(visitor->visitFuncFParams);
        return visitor->visitFuncFParams((struct FuncFParams *)token, recusive);
    case FUNCFPARAM:
        EnsureNotNull(visitor->visitFuncFParam);
        return visitor->visitFuncFParam((struct FuncFParam *)token, recusive);
    case EXPARRAYDEFS:
        EnsureNotNull(visitor->visitExpArrayDefs);
        return visitor->visitExpArrayDefs((struct ExpArrayDefs *)token, recusive);
    case EXPARRAYDEF:
        EnsureNotNull(visitor->visitExpArrayDef);
        return visitor->visitExpArrayDef((struct ExpArrayDef *)token, recusive);
    case BLOCK:
        EnsureNotNull(visitor->visitBlock);
        return visitor->visitBlock((struct Block *)token, recusive);
    case BLOCKITEMS:
        EnsureNotNull(visitor->visitBlockItems);
        return visitor->visitBlockItems((struct BlockItems *)token, recusive);
    case BLOCKITEM:
        EnsureNotNull(visitor->visitBlockItem);
        return visitor->visitBlockItem((struct BlockItem *)token, recusive);
    case STMT:
        EnsureNotNull(visitor->visitStmt);
        return visitor->visitStmt((struct Stmt *)token, recusive);
    case ASSIGN:
        EnsureNotNull(visitor->visitAssign);
        return visitor->visitAssign((struct Assign *)token, recusive);
    case IFSTMT:
        EnsureNotNull(visitor->visitIfStmt);
        return visitor->visitIfStmt((struct IfStmt *)token, recusive);
    case WHILESTMT:
        EnsureNotNull(visitor->visitWhileStmt);
        return visitor->visitWhileStmt((struct WhileStmt *)token, recusive);
    case RETURNSTMT:
        EnsureNotNull(visitor->visitReturnStmt);
        return visitor->visitReturnStmt((struct ReturnStmt *)token, recusive);
    case EXP:
        EnsureNotNull(visitor->visitExp);
        return visitor->visitExp((struct Exp *)token, recusive);
    case COND:
        EnsureNotNull(visitor->visitCond);
        return visitor->visitCond((struct Cond *)token, recusive);
    case LVAL:
        EnsureNotNull(visitor->visitLVal);
        return visitor->visitLVal((struct LVal *)token, recusive);
    case ARRAYIMPL:
        EnsureNotNull(visitor->visitArrayImpl);
        return visitor->visitArrayImpl((struct ArrayImpl *)token, recusive);
    case PRIMARYEXP:
        EnsureNotNull(visitor->visitPrimaryExp);
        return visitor->visitPrimaryExp((struct PrimaryExp *)token, recusive);
    case NUMBER:
        EnsureNotNull(visitor->visitNumber);
        return visitor->visitNumber((struct Number *)token, recusive);
    case UNARYEXP:
        EnsureNotNull(visitor->visitUnaryExp);
        return visitor->visitUnaryExp((struct UnaryExp *)token, recusive);
    case FUNCIMPL:
        EnsureNotNull(visitor->visitFuncImpl);
        return visitor->visitFuncImpl((struct FuncImpl *)token, recusive);
    case UNARYEXPS:
        EnsureNotNull(visitor->visitUnaryExps);
        return visitor->visitUnaryExps((struct UnaryExps *)token, recusive);
    case UNARYOP:
        EnsureNotNull(visitor->visitUnaryOp);
        return visitor->visitUnaryOp((struct UnaryOp *)token, recusive);
    case FUNCRPARAMS:
        EnsureNotNull(visitor->visitFuncRParams);
        return visitor->visitFuncRParams((struct FuncRParams *)token, recusive);
    case FUNCRPARAM:
        EnsureNotNull(visitor->visitFuncRParam);
        return visitor->visitFuncRParam((struct FuncRParam *)token, recusive);
    case MULEXP:
        EnsureNotNull(visitor->visitMulExp);
        return visitor->visitMulExp((struct MulExp *)token, recusive);
    case MULOP:
        EnsureNotNull(visitor->visitMulOp);
        return visitor->visitMulOp((struct MulOp *)token, recusive);
    case ADDEXP:
        EnsureNotNull(visitor->visitAddExp);
        return visitor->visitAddExp((struct AddExp *)token, recusive);
    case ADDOP:
        EnsureNotNull(visitor->visitAddOp);
        return visitor->visitAddOp((struct AddOp *)token, recusive);
    case RELEXP:
        EnsureNotNull(visitor->visitRelExp);
        return visitor->visitRelExp((struct RelExp *)token, recusive);
    case RELOP:
        EnsureNotNull(visitor->visitRelOp);
        return visitor->visitRelOp((struct RelOp *)token, recusive);
    case EQEXP:
        EnsureNotNull(visitor->visitEqExp);
        return visitor->visitEqExp((struct EqExp *)token, recusive);
    case EQOP:
        EnsureNotNull(visitor->visitEqOp);
        return visitor->visitEqOp((struct EqOp *)token, recusive);
    case LANDEXP:
        EnsureNotNull(visitor->visitLAndExp);
        return visitor->visitLAndExp((struct LAndExp *)token, recusive);
    case LOREXP:
        EnsureNotNull(visitor->visitLOrExp);
        return visitor->visitLOrExp((struct LOrExp *)token, recusive);
    case CONSTEXP:
        EnsureNotNull(visitor->visitConstExp);
        return visitor->visitConstExp((struct ConstExp *)token, recusive);
    case IDENT:
        EnsureNotNull(visitor->visitIdent);
        return visitor->visitIdent((struct Ident *)token, recusive);
    case STRING:
        EnsureNotNull(visitor->visitString);
        return visitor->visitString((struct String *)token, recusive);
    default:
        PrintErrExit("NOT VALID TOKEN TYPE %dn", *(int *)token);
    }
}