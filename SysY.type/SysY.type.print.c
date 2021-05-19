#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../flex.bison/SysY.tab.h"
#include "SysY.type.def.h"
#include "SysY.type.new.h"
#include "SysY.type.visitor.h"
#include "SysY.type.print.h"

static int identation;
#define LEVEL_IDENT 1
#define PRINT(msg, ...)                               \
    do                                                \
    {                                                 \
        for (register int i = 0; i < identation; i++) \
            fprintf(stdout, "|---");                  \
        fprintf(stdout, msg, ##__VA_ARGS__);          \
    } while (0)

VisitorFuncImplGenerator(printToken_t, printToken, void)();
struct printToken_t *printToken_p;

#define ListLikePrintGenerator1(funcname, listype, dataname1)                       \
    void funcname(listype *list)                                                    \
    {                                                                               \
        IfNull(list, {                                                              \
            PRINT("NULL POINT\n");                                                  \
            return;                                                                 \
        });                                                                         \
        listype *head = list;                                                       \
        do                                                                          \
        {                                                                           \
            PRINT("%s\n", EnumTypeToString(list->type));                            \
            identation += LEVEL_IDENT;                                              \
            IfNotNull(list->dataname1, printToken(list->dataname1, printToken_p);); \
            identation -= LEVEL_IDENT;                                              \
            list = list->next;                                                      \
        } while (list != head);                                                     \
    }

#define ListLikePrintGenerator2(funcname, listype, dataname1, dataname2)            \
    void funcname(listype *list)                                                    \
    {                                                                               \
        IfNull(list, {                                                              \
            PRINT("NULL POINT\n");                                                  \
            return;                                                                 \
        });                                                                         \
        listype *head = list;                                                       \
        do                                                                          \
        {                                                                           \
            PRINT("%s\n", EnumTypeToString(list->type));                            \
            identation += LEVEL_IDENT;                                              \
            IfNotNull(list->dataname1, printToken(list->dataname1, printToken_p);); \
            IfNotNull(list->dataname2, printToken(list->dataname2, printToken_p);); \
            identation -= LEVEL_IDENT;                                              \
            list = list->next;                                                      \
        } while (list != head);                                                     \
    }

#define OnlyDataPrintGenerator1(funcname, printtype, dataname1)                   \
    void funcname(printtype *token)                                               \
    {                                                                             \
        IfNull(token, {                                                           \
            PRINT("NULL POINT\n");                                                \
            return;                                                               \
        });                                                                       \
        PRINT("%s\n", EnumTypeToString(token->type));                             \
        identation += LEVEL_IDENT;                                                \
        IfNotNull(token->dataname1, printToken(token->dataname1, printToken_p);); \
        identation -= LEVEL_IDENT;                                                \
    }

#define OnlyDataPrintGenerator2(funcname, printtype, dataname1, dataname2)        \
    void funcname(printtype *token)                                               \
    {                                                                             \
        IfNull(token, {                                                           \
            PRINT("NULL POINT\n");                                                \
            return;                                                               \
        });                                                                       \
        PRINT("%s\n", EnumTypeToString(token->type));                             \
        identation += LEVEL_IDENT;                                                \
        IfNotNull(token->dataname1, printToken(token->dataname1, printToken_p);); \
        IfNotNull(token->dataname2, printToken(token->dataname2, printToken_p);); \
        identation -= LEVEL_IDENT;                                                \
    }

#define OnlyDataPrintGenerator3(funcname, printtype, dataname1, dataname2, dataname3) \
    void funcname(printtype *token)                                                   \
    {                                                                                 \
        IfNull(token, {                                                               \
            PRINT("NULL POINT\n");                                                    \
            return;                                                                   \
        });                                                                           \
        PRINT("%s\n", EnumTypeToString(token->type));                                 \
        identation += LEVEL_IDENT;                                                    \
        IfNotNull(token->dataname1, printToken(token->dataname1, printToken_p););     \
        IfNotNull(token->dataname2, printToken(token->dataname2, printToken_p););     \
        IfNotNull(token->dataname3, printToken(token->dataname3, printToken_p););     \
        identation -= LEVEL_IDENT;                                                    \
    }

#define OnlyDataPrintGenerator4(funcname, printtype, dataname1, dataname2, dataname3, dataname4) \
    void funcname(printtype *token)                                                              \
    {                                                                                            \
        IfNull(token, {                                                                          \
            PRINT("NULL POINT\n");                                                               \
            return;                                                                              \
        });                                                                                      \
        PRINT("%s\n", EnumTypeToString(token->type));                                            \
        identation += LEVEL_IDENT;                                                               \
        IfNotNull(token->dataname1, printToken(token->dataname1, printToken_p););                \
        IfNotNull(token->dataname2, printToken(token->dataname2, printToken_p););                \
        IfNotNull(token->dataname3, printToken(token->dataname3, printToken_p););                \
        IfNotNull(token->dataname4, printToken(token->dataname4, printToken_p););                \
        identation -= LEVEL_IDENT;                                                               \
    }

#define OnlyUnionPrintGenerator(funcname, printtype, value_in_union_name)                                         \
    void funcname(printtype *token)                                                                               \
    {                                                                                                             \
        IfNull(token, {                                                                                           \
            PRINT("NULL POINT\n");                                                                                \
            return;                                                                                               \
        });                                                                                                       \
        PRINT("%s %s\n", EnumTypeToString(token->type), EnumTypeToString(token->valuetype));                      \
        identation += LEVEL_IDENT;                                                                                \
        IfNotNull(token->value.value_in_union_name, printToken(token->value.value_in_union_name, printToken_p);); \
        identation -= LEVEL_IDENT;                                                                                \
    }

void printKeyword(struct Keyword *keyword)
{
    IfNull(keyword, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("KEYWORD %s\n", keyword->value);
}

void printIntConst(struct IntConst *intconst)
{
    IfNull(intconst, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("INTCONST %d\n", intconst->value);
}

void printCompUnit(struct CompUnit *cp)
{
    IfNull(cp, {
        PRINT("NULL POINT\n");
        return;
    });
    struct CompUnit *head = cp;
    do
    {
        PRINT("COMPUNIT %s\n", EnumTypeToString(cp->type));
        identation += LEVEL_IDENT;
        IfNotNull(cp->value.decl, printToken(cp->value.decl, printToken_p););
        identation -= LEVEL_IDENT;
        cp = cp->next;
    } while (cp != head);
}

OnlyUnionPrintGenerator(printDecl, struct Decl, constdecl);

OnlyDataPrintGenerator2(printConstDecl, struct ConstDecl, btype, constdefs);

void printBType(struct BType *btype)
{
    IfNull(btype, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("BTYPE %s\n", EnumTypeToString(btype->typevalue));
}

ListLikePrintGenerator1(printConstDefs, struct ConstDefs, constdef);

OnlyDataPrintGenerator3(printConstDef, struct ConstDef, ident, constarraydefs, constinitval);

ListLikePrintGenerator1(printConstArrayDefs, struct ConstArrayDefs, constarraydef);

OnlyDataPrintGenerator1(printConstArrayDef, struct ConstArrayDef, constexp);

OnlyUnionPrintGenerator(printConstInitVal, struct ConstInitVal, constexp);

ListLikePrintGenerator1(printConstInitVals, struct ConstInitVals, constinitval);

OnlyDataPrintGenerator2(printVarDecl, struct VarDecl, btype, vardefs);

ListLikePrintGenerator1(printVarDefs, struct VarDefs, vardef);

OnlyDataPrintGenerator3(printVarDef, struct VarDef, ident, constarraydefs, initval);

OnlyUnionPrintGenerator(printInitVal, struct InitVal, exp);

ListLikePrintGenerator1(printInitVals, struct InitVals, initval);

OnlyDataPrintGenerator4(printFuncDef, struct FuncDef, functype, ident, funcfparams, block);

void printFuncType(struct FuncType *functype)
{
    IfNull(functype, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("FUNCTYPE %s\n", EnumTypeToString(functype->typevalue));
}

ListLikePrintGenerator1(printFuncFParams, struct FuncFParams, funcfparam);

OnlyDataPrintGenerator3(printFuncFParam, struct FuncFParam, btype, ident, exparraydefs);

ListLikePrintGenerator1(printExpArrayDefs, struct ExpArrayDefs, exparraydef);

OnlyDataPrintGenerator1(printExpArrayDef, struct ExpArrayDef, exp);

OnlyDataPrintGenerator1(printBlock, struct Block, blockitems);

ListLikePrintGenerator1(printBlockItems, struct BlockItems, blockitem);

OnlyUnionPrintGenerator(printBlockItem, struct BlockItem, decl);

OnlyUnionPrintGenerator(printStmt, struct Stmt, assign);

OnlyDataPrintGenerator2(printAssign, struct Assign, lval, exp);

OnlyDataPrintGenerator3(printIfStmt, struct IfStmt, cond, then, otherwise);

OnlyDataPrintGenerator2(printWhileStmt, struct WhileStmt, cond, stmt);

OnlyDataPrintGenerator1(printReturnStmt, struct ReturnStmt, exp);

OnlyDataPrintGenerator1(printExp, struct Exp, addexp);

OnlyDataPrintGenerator1(printCond, struct Cond, lorexp);

OnlyUnionPrintGenerator(printLVal, struct LVal, ident);

OnlyDataPrintGenerator2(printArrayImpl, struct ArrayImpl, ident, exparraydefs);

OnlyUnionPrintGenerator(printPrimaryExp, struct PrimaryExp, exp);

OnlyDataPrintGenerator1(printNumber, struct Number, intconst);

OnlyUnionPrintGenerator(printUnaryExp, struct UnaryExp, primaryexp);

OnlyDataPrintGenerator2(printFuncImpl, struct FuncImpl, ident, funcrparams);

OnlyDataPrintGenerator2(printUnaryExps, struct UnaryExps, unaryop, unaryexp);

void printUnaryOp(struct UnaryOp *op)
{
    IfNull(op, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("UNARYOP %s\n", EnumTypeToString(op->typevalue));
}

ListLikePrintGenerator1(printFuncRParams, struct FuncRParams, funcrparam);

OnlyUnionPrintGenerator(printFuncRParam, struct FuncRParam, exp);

ListLikePrintGenerator2(printMulExp, struct MulExp, mulop, unaryexp);

void printMulOp(struct MulOp *op)
{
    IfNull(op, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("MULOP %s\n", EnumTypeToString(op->typevalue));
}

ListLikePrintGenerator2(printAddExp, struct AddExp, addop, mulexp);

void printAddOp(struct AddOp *op)
{
    IfNull(op, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("ADDOP %s\n", EnumTypeToString(op->typevalue));
}

ListLikePrintGenerator2(printRelExp, struct RelExp, relop, addexp);

void printRelOp(struct RelOp *op)
{
    IfNull(op, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("RELOP %s\n", EnumTypeToString(op->typevalue));
}

ListLikePrintGenerator2(printEqExp, struct EqExp, eqop, relexp);

void printEqOp(struct EqOp *op)
{
    IfNull(op, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("EQOP %s\n", EnumTypeToString(op->typevalue));
}

ListLikePrintGenerator1(printLAndExp, struct LAndExp, eqexp);

ListLikePrintGenerator1(printLOrExp, struct LOrExp, landexp);

OnlyDataPrintGenerator1(printConstExp, struct ConstExp, addexp);

void printIdent(struct Ident *ident)
{
    IfNull(ident, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("IDENT %s\n", ident->name);
}

void printString(struct String *string)
{
    IfNull(string, {
        PRINT("NULL POINT\n");
        return;
    });
    PRINT("STRING %s\n", string->content);
}

// only use once for init
void initPrintToken()
{
    printToken_p = malloc(sizeof(struct printToken_t));
    printToken_p->visitKeyword = printKeyword;
    printToken_p->visitIntConst = printIntConst;
    printToken_p->visitCompUnit = printCompUnit;
    printToken_p->visitDecl = printDecl;
    printToken_p->visitConstDecl = printConstDecl;
    printToken_p->visitBType = printBType;
    printToken_p->visitConstDefs = printConstDefs;
    printToken_p->visitConstDef = printConstDef;
    printToken_p->visitConstArrayDefs = printConstArrayDefs;
    printToken_p->visitConstArrayDef = printConstArrayDef;
    printToken_p->visitConstInitVal = printConstInitVal;
    printToken_p->visitConstInitVals = printConstInitVals;
    printToken_p->visitVarDecl = printVarDecl;
    printToken_p->visitVarDefs = printVarDefs;
    printToken_p->visitVarDef = printVarDef;
    printToken_p->visitInitVal = printInitVal;
    printToken_p->visitInitVals = printInitVals;
    printToken_p->visitFuncDef = printFuncDef;
    printToken_p->visitFuncType = printFuncType;
    printToken_p->visitFuncFParams = printFuncFParams;
    printToken_p->visitFuncFParam = printFuncFParam;
    printToken_p->visitExpArrayDefs = printExpArrayDefs;
    printToken_p->visitExpArrayDef = printExpArrayDef;
    printToken_p->visitBlock = printBlock;
    printToken_p->visitBlockItems = printBlockItems;
    printToken_p->visitBlockItem = printBlockItem;
    printToken_p->visitStmt = printStmt;
    printToken_p->visitAssign = printAssign;
    printToken_p->visitIfStmt = printIfStmt;
    printToken_p->visitWhileStmt = printWhileStmt;
    printToken_p->visitReturnStmt = printReturnStmt;
    printToken_p->visitExp = printExp;
    printToken_p->visitCond = printCond;
    printToken_p->visitLVal = printLVal;
    printToken_p->visitArrayImpl = printArrayImpl;
    printToken_p->visitPrimaryExp = printPrimaryExp;
    printToken_p->visitNumber = printNumber;
    printToken_p->visitUnaryExp = printUnaryExp;
    printToken_p->visitFuncImpl = printFuncImpl;
    printToken_p->visitUnaryExps = printUnaryExps;
    printToken_p->visitUnaryOp = printUnaryOp;
    printToken_p->visitFuncRParams = printFuncRParams;
    printToken_p->visitFuncRParam = printFuncRParam;
    printToken_p->visitMulExp = printMulExp;
    printToken_p->visitMulOp = printMulOp;
    printToken_p->visitAddExp = printAddExp;
    printToken_p->visitAddOp = printAddOp;
    printToken_p->visitRelExp = printRelExp;
    printToken_p->visitRelOp = printRelOp;
    printToken_p->visitEqExp = printEqExp;
    printToken_p->visitEqOp = printEqOp;
    printToken_p->visitLAndExp = printLAndExp;
    printToken_p->visitLOrExp = printLOrExp;
    printToken_p->visitConstExp = printConstExp;
    printToken_p->visitIdent = printIdent;
    printToken_p->visitString = printString;

    identation = 0;
}

void printFresh()
{
    identation = 0;
}

const char *EnumTypeToString(int type)
{
    switch (type)
    {
    case KEYWORD:
        return "KEYWORD";
    case INTCONST:
        return "INTCONST";
    case COMPUNIT:
        return "COMPUNIT";
    case DECL:
        return "DECL";
    case CONSTDECL:
        return "CONSTDECL";
    case BTYPE:
        return "BTYPE";
    case CONSTDEFS:
        return "CONSTDEFS";
    case CONSTDEF:
        return "CONSTDEF";
    case CONSTARRAYDEFS:
        return "CONSTARRAYDEFS";
    case CONSTARRAYDEF:
        return "CONSTARRAYDEF";
    case CONSTINITVAL:
        return "CONSTINITVAL";
    case CONSTINITVALS:
        return "CONSTINITVALS";
    case VARDECL:
        return "VARDECL";
    case VARDEFS:
        return "VARDEFS";
    case VARDEF:
        return "VARDEF";
    case INITVAL:
        return "INITVAL";
    case INITVALS:
        return "INITVALS";
    case FUNCDEF:
        return "FUNCDEF";
    case FUNCTYPE:
        return "FUNCTYPE";
    case FUNCFPARAMS:
        return "FUNCFPARAMS";
    case FUNCFPARAM:
        return "FUNCFPARAM";
    case EXPARRAYDEFS:
        return "EXPARRAYDEFS";
    case EXPARRAYDEF:
        return "EXPARRAYDEF";
    case BLOCK:
        return "BLOCK";
    case BLOCKITEMS:
        return "BLOCKITEMS";
    case BLOCKITEM:
        return "BLOCKITEM";
    case STMT:
        return "STMT";
    case ASSIGN:
        return "ASSIGN";
    case IFSTMT:
        return "IFSTMT";
    case WHILESTMT:
        return "WHILESTMT";
    case RETURNSTMT:
        return "RETURNSTMT";
    case EXP:
        return "EXP";
    case COND:
        return "COND";
    case LVAL:
        return "LVAL";
    case ARRAYIMPL:
        return "ARRAYIMPL";
    case PRIMARYEXP:
        return "PRIMARYEXP";
    case NUMBER:
        return "NUMBER";
    case UNARYEXP:
        return "UNARYEXP";
    case FUNCIMPL:
        return "FUNCIMPL";
    case UNARYEXPS:
        return "UNARYEXPS";
    case UNARYOP:
        return "UNARYOP";
    case FUNCRPARAMS:
        return "FUNCRPARAMS";
    case FUNCRPARAM:
        return "FUNCRPARAM";
    case MULEXP:
        return "MULEXP";
    case MULOP:
        return "MULOP";
    case ADDEXP:
        return "ADDEXP";
    case ADDOP:
        return "ADDOP";
    case RELEXP:
        return "RELEXP";
    case RELOP:
        return "RELOP";
    case EQEXP:
        return "EQEXP";
    case EQOP:
        return "EQOP";
    case LANDEXP:
        return "LANDEXP";
    case LOREXP:
        return "LOREXP";
    case CONSTEXP:
        return "CONSTEXP";
    case IDENT:
        return "IDENT";
    case STRING:
        return "STRING";
    case K_ADD:
        return "K_ADD";
    case K_AND:
        return "K_AND";
    case K_ASSIGNOP:
        return "K_ASSIGNOP";
    case K_BREAK:
        return "K_BREAK";
    case K_COMMA:
        return "K_COMMA";
    case K_CONST:
        return "K_CONST";
    case K_CONTINUE:
        return "K_CONTINUE";
    case K_CURLY_L:
        return "K_CURLY_L";
    case K_CURLY_R:
        return "K_CURLY_R";
    case K_DIV:
        return "K_DIV";
    case K_ELSE:
        return "K_ELSE";
    case K_EQ:
        return "K_EQ";
    case K_GT:
        return "K_GT";
    case K_GTE:
        return "K_GTE";
    case K_IF:
        return "K_IF";
    case K_INT:
        return "K_INT";
    case K_LOW_THAN_ELSE:
        return "K_LOW_THAN_ELSE";
    case K_LT:
        return "K_LT";
    case K_LTE:
        return "K_LTE";
    case K_MOD:
        return "K_MOD";
    case K_MUL:
        return "K_MUL";
    case K_NEQ:
        return "K_NEQ";
    case K_NOT:
        return "K_NOT";
    case K_OR:
        return "K_OR";
    case K_PARENTHESES_L:
        return "K_PARENTHESES_L";
    case K_PARENTHESES_R:
        return "K_PARENTHESES_R";
    case K_RETURN:
        return "K_RETURN";
    case K_SEMICOLON:
        return "K_SEMICOLON";
    case K_SQUARE_L:
        return "K_SQUARE_L";
    case K_SQUARE_R:
        return "K_SQUARE_R";
    case K_SUB:
        return "K_SUB";
    case K_VOID:
        return "K_VOID";
    case K_WHILE:
        return "K_WHILE";
    case 0:
        return "NULL";
    default:
        return "UNKNOWN";
    }
}