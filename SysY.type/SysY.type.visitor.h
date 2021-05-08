#ifndef SYSY_TYPE_VISITOR_H
#define SYSY_TYPE_VISITOR_H
#include <stdio.h>
#include <stdlib.h>
#include "./SysY.type.def.h"
#include "../flex.bison/SysY.tab.h"
#include "../utils/NullPointMacro.h"

// not use direct. it is used by VisitorDeclGenerator macro
#define VisitTokenFuncBody(...)                                                                 \
    {                                                                                           \
        EnsureNotNull(token);                                                                   \
        EnsureNotNull(visitor);                                                                 \
        switch (*(int *)token)                                                                  \
        {                                                                                       \
        case KEYWORD:                                                                           \
            EnsureNotNull(visitor->visitKeyword);                                               \
            return visitor->visitKeyword((struct Keyword *)token, ##__VA_ARGS__);               \
        case INTCONST:                                                                          \
            EnsureNotNull(visitor->visitIntConst);                                              \
            return visitor->visitIntConst((struct IntConst *)token, ##__VA_ARGS__);             \
        case COMPUNIT:                                                                          \
            EnsureNotNull(visitor->visitCompUnit);                                              \
            return visitor->visitCompUnit((struct CompUnit *)token, ##__VA_ARGS__);             \
        case DECL:                                                                              \
            EnsureNotNull(visitor->visitDecl);                                                  \
            return visitor->visitDecl((struct Decl *)token, ##__VA_ARGS__);                     \
        case CONSTDECL:                                                                         \
            EnsureNotNull(visitor->visitConstDecl);                                             \
            return visitor->visitConstDecl((struct ConstDecl *)token, ##__VA_ARGS__);           \
        case BTYPE:                                                                             \
            EnsureNotNull(visitor->visitBType);                                                 \
            return visitor->visitBType((struct BType *)token, ##__VA_ARGS__);                   \
        case CONSTDEFS:                                                                         \
            EnsureNotNull(visitor->visitConstDefs);                                             \
            return visitor->visitConstDefs((struct ConstDefs *)token, ##__VA_ARGS__);           \
        case CONSTDEF:                                                                          \
            EnsureNotNull(visitor->visitConstDef);                                              \
            return visitor->visitConstDef((struct ConstDef *)token, ##__VA_ARGS__);             \
        case CONSTARRAYDEFS:                                                                    \
            EnsureNotNull(visitor->visitConstArrayDefs);                                        \
            return visitor->visitConstArrayDefs((struct ConstArrayDefs *)token, ##__VA_ARGS__); \
        case CONSTARRAYDEF:                                                                     \
            EnsureNotNull(visitor->visitConstArrayDef);                                         \
            return visitor->visitConstArrayDef((struct ConstArrayDef *)token, ##__VA_ARGS__);   \
        case CONSTINITVAL:                                                                      \
            EnsureNotNull(visitor->visitConstInitVal);                                          \
            return visitor->visitConstInitVal((struct ConstInitVal *)token, ##__VA_ARGS__);     \
        case CONSTINITVALS:                                                                     \
            EnsureNotNull(visitor->visitConstInitVals);                                         \
            return visitor->visitConstInitVals((struct ConstInitVals *)token, ##__VA_ARGS__);   \
        case VARDECL:                                                                           \
            EnsureNotNull(visitor->visitVarDecl);                                               \
            return visitor->visitVarDecl((struct VarDecl *)token, ##__VA_ARGS__);               \
        case VARDEFS:                                                                           \
            EnsureNotNull(visitor->visitVarDefs);                                               \
            return visitor->visitVarDefs((struct VarDefs *)token, ##__VA_ARGS__);               \
        case VARDEF:                                                                            \
            EnsureNotNull(visitor->visitVarDef);                                                \
            return visitor->visitVarDef((struct VarDef *)token, ##__VA_ARGS__);                 \
        case INITVAL:                                                                           \
            EnsureNotNull(visitor->visitInitVal);                                               \
            return visitor->visitInitVal((struct InitVal *)token, ##__VA_ARGS__);               \
        case INITVALS:                                                                          \
            EnsureNotNull(visitor->visitInitVals);                                              \
            return visitor->visitInitVals((struct InitVals *)token, ##__VA_ARGS__);             \
        case FUNCDEF:                                                                           \
            EnsureNotNull(visitor->visitFuncDef);                                               \
            return visitor->visitFuncDef((struct FuncDef *)token, ##__VA_ARGS__);               \
        case FUNCTYPE:                                                                          \
            EnsureNotNull(visitor->visitFuncType);                                              \
            return visitor->visitFuncType((struct FuncType *)token, ##__VA_ARGS__);             \
        case FUNCFPARAMS:                                                                       \
            EnsureNotNull(visitor->visitFuncFParams);                                           \
            return visitor->visitFuncFParams((struct FuncFParams *)token, ##__VA_ARGS__);       \
        case FUNCFPARAM:                                                                        \
            EnsureNotNull(visitor->visitFuncFParam);                                            \
            return visitor->visitFuncFParam((struct FuncFParam *)token, ##__VA_ARGS__);         \
        case EXPARRAYDEFS:                                                                      \
            EnsureNotNull(visitor->visitExpArrayDefs);                                          \
            return visitor->visitExpArrayDefs((struct ExpArrayDefs *)token, ##__VA_ARGS__);     \
        case EXPARRAYDEF:                                                                       \
            EnsureNotNull(visitor->visitExpArrayDef);                                           \
            return visitor->visitExpArrayDef((struct ExpArrayDef *)token, ##__VA_ARGS__);       \
        case BLOCK:                                                                             \
            EnsureNotNull(visitor->visitBlock);                                                 \
            return visitor->visitBlock((struct Block *)token, ##__VA_ARGS__);                   \
        case BLOCKITEMS:                                                                        \
            EnsureNotNull(visitor->visitBlockItems);                                            \
            return visitor->visitBlockItems((struct BlockItems *)token, ##__VA_ARGS__);         \
        case BLOCKITEM:                                                                         \
            EnsureNotNull(visitor->visitBlockItem);                                             \
            return visitor->visitBlockItem((struct BlockItem *)token, ##__VA_ARGS__);           \
        case STMT:                                                                              \
            EnsureNotNull(visitor->visitStmt);                                                  \
            return visitor->visitStmt((struct Stmt *)token, ##__VA_ARGS__);                     \
        case ASSIGN:                                                                            \
            EnsureNotNull(visitor->visitAssign);                                                \
            return visitor->visitAssign((struct Assign *)token, ##__VA_ARGS__);                 \
        case IFSTMT:                                                                            \
            EnsureNotNull(visitor->visitIfStmt);                                                \
            return visitor->visitIfStmt((struct IfStmt *)token, ##__VA_ARGS__);                 \
        case WHILESTMT:                                                                         \
            EnsureNotNull(visitor->visitWhileStmt);                                             \
            return visitor->visitWhileStmt((struct WhileStmt *)token, ##__VA_ARGS__);           \
        case RETURNSTMT:                                                                        \
            EnsureNotNull(visitor->visitReturnStmt);                                            \
            return visitor->visitReturnStmt((struct ReturnStmt *)token, ##__VA_ARGS__);         \
        case EXP:                                                                               \
            EnsureNotNull(visitor->visitExp);                                                   \
            return visitor->visitExp((struct Exp *)token, ##__VA_ARGS__);                       \
        case COND:                                                                              \
            EnsureNotNull(visitor->visitCond);                                                  \
            return visitor->visitCond((struct Cond *)token, ##__VA_ARGS__);                     \
        case LVAL:                                                                              \
            EnsureNotNull(visitor->visitLVal);                                                  \
            return visitor->visitLVal((struct LVal *)token, ##__VA_ARGS__);                     \
        case ARRAYIMPL:                                                                         \
            EnsureNotNull(visitor->visitArrayImpl);                                             \
            return visitor->visitArrayImpl((struct ArrayImpl *)token, ##__VA_ARGS__);           \
        case PRIMARYEXP:                                                                        \
            EnsureNotNull(visitor->visitPrimaryExp);                                            \
            return visitor->visitPrimaryExp((struct PrimaryExp *)token, ##__VA_ARGS__);         \
        case NUMBER:                                                                            \
            EnsureNotNull(visitor->visitNumber);                                                \
            return visitor->visitNumber((struct Number *)token, ##__VA_ARGS__);                 \
        case UNARYEXP:                                                                          \
            EnsureNotNull(visitor->visitUnaryExp);                                              \
            return visitor->visitUnaryExp((struct UnaryExp *)token, ##__VA_ARGS__);             \
        case FUNCIMPL:                                                                          \
            EnsureNotNull(visitor->visitFuncImpl);                                              \
            return visitor->visitFuncImpl((struct FuncImpl *)token, ##__VA_ARGS__);             \
        case UNARYEXPS:                                                                         \
            EnsureNotNull(visitor->visitUnaryExps);                                             \
            return visitor->visitUnaryExps((struct UnaryExps *)token, ##__VA_ARGS__);           \
        case UNARYOP:                                                                           \
            EnsureNotNull(visitor->visitUnaryOp);                                               \
            return visitor->visitUnaryOp((struct UnaryOp *)token, ##__VA_ARGS__);               \
        case FUNCRPARAMS:                                                                       \
            EnsureNotNull(visitor->visitFuncRParams);                                           \
            return visitor->visitFuncRParams((struct FuncRParams *)token, ##__VA_ARGS__);       \
        case FUNCRPARAM:                                                                        \
            EnsureNotNull(visitor->visitFuncRParam);                                            \
            return visitor->visitFuncRParam((struct FuncRParam *)token, ##__VA_ARGS__);         \
        case MULEXP:                                                                            \
            EnsureNotNull(visitor->visitMulExp);                                                \
            return visitor->visitMulExp((struct MulExp *)token, ##__VA_ARGS__);                 \
        case MULOP:                                                                             \
            EnsureNotNull(visitor->visitMulOp);                                                 \
            return visitor->visitMulOp((struct MulOp *)token, ##__VA_ARGS__);                   \
        case ADDEXP:                                                                            \
            EnsureNotNull(visitor->visitAddExp);                                                \
            return visitor->visitAddExp((struct AddExp *)token, ##__VA_ARGS__);                 \
        case ADDOP:                                                                             \
            EnsureNotNull(visitor->visitAddOp);                                                 \
            return visitor->visitAddOp((struct AddOp *)token, ##__VA_ARGS__);                   \
        case RELEXP:                                                                            \
            EnsureNotNull(visitor->visitRelExp);                                                \
            return visitor->visitRelExp((struct RelExp *)token, ##__VA_ARGS__);                 \
        case RELOP:                                                                             \
            EnsureNotNull(visitor->visitRelOp);                                                 \
            return visitor->visitRelOp((struct RelOp *)token, ##__VA_ARGS__);                   \
        case EQEXP:                                                                             \
            EnsureNotNull(visitor->visitEqExp);                                                 \
            return visitor->visitEqExp((struct EqExp *)token, ##__VA_ARGS__);                   \
        case EQOP:                                                                              \
            EnsureNotNull(visitor->visitEqOp);                                                  \
            return visitor->visitEqOp((struct EqOp *)token, ##__VA_ARGS__);                     \
        case LANDEXP:                                                                           \
            EnsureNotNull(visitor->visitLAndExp);                                               \
            return visitor->visitLAndExp((struct LAndExp *)token, ##__VA_ARGS__);               \
        case LOREXP:                                                                            \
            EnsureNotNull(visitor->visitLOrExp);                                                \
            return visitor->visitLOrExp((struct LOrExp *)token, ##__VA_ARGS__);                 \
        case CONSTEXP:                                                                          \
            EnsureNotNull(visitor->visitConstExp);                                              \
            return visitor->visitConstExp((struct ConstExp *)token, ##__VA_ARGS__);             \
        case IDENT:                                                                             \
            EnsureNotNull(visitor->visitIdent);                                                 \
            return visitor->visitIdent((struct Ident *)token, ##__VA_ARGS__);                   \
        case STRING:                                                                            \
            EnsureNotNull(visitor->visitString);                                                \
            return visitor->visitString((struct String *)token, ##__VA_ARGS__);                 \
        default:                                                                                \
            fprintf(stderr, "NOT VALID TOKEN TYPE %d\n", *(int *)token);                        \
            exit(-1);                                                                           \
        }                                                                                       \
    }

// use like VisitorDeclGenerator(interfacename, funcname, returntype ,visitfuncfparams)(visitfuncrparams);
#define VisitorDeclGenerator(interfacename, funcname, returntype, ...)             \
    struct interfacename                                                           \
    {                                                                              \
        returntype (*visitKeyword)(struct Keyword *, ##__VA_ARGS__);               \
        returntype (*visitIntConst)(struct IntConst *, ##__VA_ARGS__);             \
        returntype (*visitCompUnit)(struct CompUnit *, ##__VA_ARGS__);             \
        returntype (*visitDecl)(struct Decl *, ##__VA_ARGS__);                     \
        returntype (*visitConstDecl)(struct ConstDecl *, ##__VA_ARGS__);           \
        returntype (*visitBType)(struct BType *, ##__VA_ARGS__);                   \
        returntype (*visitConstDefs)(struct ConstDefs *, ##__VA_ARGS__);           \
        returntype (*visitConstDef)(struct ConstDef *, ##__VA_ARGS__);             \
        returntype (*visitConstArrayDefs)(struct ConstArrayDefs *, ##__VA_ARGS__); \
        returntype (*visitConstArrayDef)(struct ConstArrayDef *, ##__VA_ARGS__);   \
        returntype (*visitConstInitVal)(struct ConstInitVal *, ##__VA_ARGS__);     \
        returntype (*visitConstInitVals)(struct ConstInitVals *, ##__VA_ARGS__);   \
        returntype (*visitVarDecl)(struct VarDecl *, ##__VA_ARGS__);               \
        returntype (*visitVarDefs)(struct VarDefs *, ##__VA_ARGS__);               \
        returntype (*visitVarDef)(struct VarDef *, ##__VA_ARGS__);                 \
        returntype (*visitInitVal)(struct InitVal *, ##__VA_ARGS__);               \
        returntype (*visitInitVals)(struct InitVals *, ##__VA_ARGS__);             \
        returntype (*visitFuncDef)(struct FuncDef *, ##__VA_ARGS__);               \
        returntype (*visitFuncType)(struct FuncType *, ##__VA_ARGS__);             \
        returntype (*visitFuncFParams)(struct FuncFParams *, ##__VA_ARGS__);       \
        returntype (*visitFuncFParam)(struct FuncFParam *, ##__VA_ARGS__);         \
        returntype (*visitExpArrayDefs)(struct ExpArrayDefs *, ##__VA_ARGS__);     \
        returntype (*visitExpArrayDef)(struct ExpArrayDef *, ##__VA_ARGS__);       \
        returntype (*visitBlock)(struct Block *, ##__VA_ARGS__);                   \
        returntype (*visitBlockItems)(struct BlockItems *, ##__VA_ARGS__);         \
        returntype (*visitBlockItem)(struct BlockItem *, ##__VA_ARGS__);           \
        returntype (*visitStmt)(struct Stmt *, ##__VA_ARGS__);                     \
        returntype (*visitAssign)(struct Assign *, ##__VA_ARGS__);                 \
        returntype (*visitIfStmt)(struct IfStmt *, ##__VA_ARGS__);                 \
        returntype (*visitWhileStmt)(struct WhileStmt *, ##__VA_ARGS__);           \
        returntype (*visitReturnStmt)(struct ReturnStmt *, ##__VA_ARGS__);         \
        returntype (*visitExp)(struct Exp *, ##__VA_ARGS__);                       \
        returntype (*visitCond)(struct Cond *, ##__VA_ARGS__);                     \
        returntype (*visitLVal)(struct LVal *, ##__VA_ARGS__);                     \
        returntype (*visitArrayImpl)(struct ArrayImpl *, ##__VA_ARGS__);           \
        returntype (*visitPrimaryExp)(struct PrimaryExp *, ##__VA_ARGS__);         \
        returntype (*visitNumber)(struct Number *, ##__VA_ARGS__);                 \
        returntype (*visitUnaryExp)(struct UnaryExp *, ##__VA_ARGS__);             \
        returntype (*visitFuncImpl)(struct FuncImpl *, ##__VA_ARGS__);             \
        returntype (*visitUnaryExps)(struct UnaryExps *, ##__VA_ARGS__);           \
        returntype (*visitUnaryOp)(struct UnaryOp *, ##__VA_ARGS__);               \
        returntype (*visitFuncRParams)(struct FuncRParams *, ##__VA_ARGS__);       \
        returntype (*visitFuncRParam)(struct FuncRParam *, ##__VA_ARGS__);         \
        returntype (*visitMulExp)(struct MulExp *, ##__VA_ARGS__);                 \
        returntype (*visitMulOp)(struct MulOp *, ##__VA_ARGS__);                   \
        returntype (*visitAddExp)(struct AddExp *, ##__VA_ARGS__);                 \
        returntype (*visitAddOp)(struct AddOp *, ##__VA_ARGS__);                   \
        returntype (*visitRelExp)(struct RelExp *, ##__VA_ARGS__);                 \
        returntype (*visitRelOp)(struct RelOp *, ##__VA_ARGS__);                   \
        returntype (*visitEqExp)(struct EqExp *, ##__VA_ARGS__);                   \
        returntype (*visitEqOp)(struct EqOp *, ##__VA_ARGS__);                     \
        returntype (*visitLAndExp)(struct LAndExp *, ##__VA_ARGS__);               \
        returntype (*visitLOrExp)(struct LOrExp *, ##__VA_ARGS__);                 \
        returntype (*visitConstExp)(struct ConstExp *, ##__VA_ARGS__);             \
        returntype (*visitIdent)(struct Ident *, ##__VA_ARGS__);                   \
        returntype (*visitString)(struct String *, ##__VA_ARGS__);                 \
    };                                                                             \
    returntype funcname(void *token, struct interfacename *visitor, ##__VA_ARGS__);

#define VisitorFuncImplGenerator(interfacename, funcname, returntype, ...)         \
    returntype funcname(void *token, struct interfacename *visitor, ##__VA_ARGS__) \
        VisitTokenFuncBody

VisitorDeclGenerator(printToken, printTokenFunc, void);

#endif