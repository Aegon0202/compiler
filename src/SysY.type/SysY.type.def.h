#ifndef SYSY_TYPE_DEF_H
#define SYSY_TYPE_DEF_H
#include "../utils/NullPointMacro.h"

enum TYPE
{
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
};

struct Keyword
{
    int type; // KEYWORD
    int keytype;
    char *value;
};

struct CompUnit
{
    int type;      // COMPUNIT
    int valuetype; // DECL, FUNCDEF
    union
    {
        struct Decl *decl;
        struct FuncDef *funcdef;
    } value;
    struct CompUnit *next;
    struct CompUnit *prev;
};

struct Decl
{
    int type;      // DECL
    int valuetype; // CONSTDECL, VARDECL
    union
    {
        struct ConstDecl *constdecl;
        struct VarDecl *vardecl;
    } value;
};

struct ConstDecl
{
    int type; // CONSTDECL
    struct BType *btype;
    struct ConstDefs *constdefs;
};

struct BType
{
    int type;      // BTYPE
    int typevalue; // K_INT, K_VOID
};

struct ConstDefs
{
    int type; // CONSTDEFS
    struct ConstDef *constdef;
    struct ConstDefs *prev;
    struct ConstDefs *next;
};

struct ConstDef
{
    int type; // CONSTDEF
    struct Ident *ident;
    struct ConstArrayDefs *constarraydefs; //当定义为常量时为NULL，为常量数组时不为NULL
    struct ConstInitVal *constinitval;
};

struct ConstArrayDefs
{
    int type; // CONSTARRAYDEF
    struct ConstArrayDef *constarraydef;
    struct ConstArrayDefs *prev;
    struct ConstArrayDefs *next;
};

struct ConstArrayDef
{
    int type; // CONSTARRRAYDEF
    struct ConstExp *constexp;
};

struct ConstInitVal
{
    int type;      // CONSTINITVAL
    int valuetype; // CONSTEXP CONSTINITVALS
    union
    {
        struct ConstExp *constexp;
        struct ConstInitVals *constinitvals;
    } value;
};
// { 1,2,4,5,{6,7,9}}

struct ConstInitVals
{
    int type;                          // CONSTINITVALS
    struct ConstInitVal *constinitval; // optional : {}
    struct ConstInitVals *prev;
    struct ConstInitVals *next;
};

struct VarDecl
{
    int type; // VARDECL
    struct BType *btype;
    struct VarDefs *vardefs;
};

struct VarDefs
{
    int type; // VARDEFS
    struct VarDef *vardef;
    struct VarDefs *prev;
    struct VarDefs *next;
};

struct VarDef
{
    int type; // VARDEF
    struct Ident *ident;
    struct ConstArrayDefs *constarraydefs; // optional
    struct InitVal *initval;               // optional
};

struct InitVal
{
    int type; // INITVAL
    int valuetype;
    union
    {
        struct Exp *exp;
        struct InitVals *initvals;
    } value;
};

struct InitVals
{
    int type;                // INITVALS
    struct InitVal *initval; // optional : {}
    struct InitVals *prev;
    struct InitVals *next;
};

struct FuncDef
{
    int type; // FUNCDEF
    struct FuncType *functype;
    struct Ident *ident;
    struct FuncFParams *funcfparams;
    struct Block *block;
};

struct FuncType
{
    int type;      // FUNCTYPE
    int typevalue; // K_INT K_VOID
};

struct FuncFParams
{
    int type;                      // FUNCFPARAMS
    struct FuncFParam *funcfparam; // optional : ()
    struct FuncFParams *prev;
    struct FuncFParams *next;
};

struct FuncFParam
{
    int type; // FUNCFPARAM
    struct BType *btype;
    struct Ident *ident;
    struct ExpArrayDefs *exparraydefs; // optional null: 非数组
};

struct ExpArrayDefs
{
    int type; // EXPARRAYDEFS
    struct ExpArrayDef *exparraydef;
    struct ExpArrayDefs *prev;
    struct ExpArrayDefs *next;
};

struct ExpArrayDef
{
    int type;        // EXPARRAYDEF
    struct Exp *exp; // optional null: []
};

struct Block
{
    int type;                      // BLOCK
    struct BlockItems *blockitems; // optional null: {}
};

struct BlockItems
{
    int type; // BLOCKITEMS
    struct BlockItem *blockitem;
    struct BlockItems *prev;
    struct BlockItems *next;
};

struct BlockItem
{
    int type;      // BLOCKITEM
    int valuetype; // DECL STMT
    union
    {
        struct Decl *decl;
        struct Stmt *stmt;
    } value;
};

struct Stmt
{
    int type;      // STMT
    int valuetype; // ASSIGN BLOCK IFSTMT WHILESTMT RETURNSTMT
                   // EXP optional. null: ;
                   // BREAKSTMT(value: null)
                   // CONTINUESTMT(value: null)
    union
    {
        struct Keyword *keyword;
        struct Assign *assign;
        struct Exp *exp;
        struct Block *block;
        struct IfStmt *ifstmt;
        struct WhileStmt *whilestmt;
        struct ReturnStmt *returnstmt;
    } value;
};

struct Assign
{
    int type; // ASSIGN
    struct LVal *lval;
    struct Exp *exp;
};

struct IfStmt
{
    int type; // IFSTMT
    struct Cond *cond;
    struct Stmt *then;
    struct Stmt *otherwise; // optional else branch
};

struct WhileStmt
{
    int type; // WHILESTMT
    struct Cond *cond;
    struct Stmt *stmt;
};

struct ReturnStmt
{
    int type;        // RETURNSTMT
    struct Exp *exp; // optional null: reutrn;
};

struct Exp
{
    int type; // EXP
    struct AddExp *addexp;
};

struct Cond
{
    int type; // CIND
    struct LOrExp *lorexp;
};

struct LVal
{
    int type;      // LVAL
    int valuetype; // IDNET ARRAYIMPL
    union
    {
        struct Ident *ident;
        struct ArrayImpl *arrayimpl;
    } value;
};

struct ArrayImpl
{
    int type; // ARRAYIMPL
    struct Ident *ident;
    struct ExpArrayDefs *exparraydefs;
};

struct PrimaryExp
{
    int type;      // PRIMARYEXP
    int valuetype; // EXP LVAL NUMBER
    union
    {
        struct Exp *exp;
        struct LVal *lval;
        struct Number *number;
    } value;
};

struct Number
{
    int type; // NUMBER
    struct IntConst *intconst;
};

struct IntConst
{
    int type;  // INTCONST
    int value; // int value
};

struct UnaryExp
{
    int type;      // UNARYEXP
    int valuetype; // PRIMARYEXP FUNCIMMPL UNARYEXPS
    union
    {
        struct PrimaryExp *primaryexp;
        struct FuncImpl *funcimpl;
        struct UnaryExps *unaryexps;
    } value;
};

struct FuncImpl
{
    int type; // FUNCIMPL
    struct Ident *ident;
    struct FuncRParams *funcrparams;
};

struct UnaryExps
{
    int type;                // UNARYEXP
    struct UnaryOp *unaryop; // optional
    struct UnaryExp *unaryexp;
};

struct UnaryOp
{
    int type;      // UNARYOP
    int typevalue; // K_ADD K_SUB K_NOT
};

struct FuncRParams
{
    int type;                      // FUNCRPARAMS
    struct FuncRParam *funcrparam; // optional null: func()
    struct FuncRParams *prev;
    struct FuncRParams *next;
};

struct FuncRParam
{
    int type;      // FUNCRPARAM
    int valuetype; // EXP STRING
    union
    {
        struct Exp *exp;
        struct String *string;
    } value;
};

struct MulExp
{
    int type;            // MULEXP
    struct MulOp *mulop; // optional. first elem is null
    struct UnaryExp *unaryexp;
    struct MulExp *prev;
    struct MulExp *next;
};

struct MulOp
{
    int type;      // MULOP
    int typevalue; // K_MUL K_DIV K_MOD
};

struct AddExp
{
    int type;            // ADDEXP
    struct AddOp *addop; // optional. first elem is null
    struct MulExp *mulexp;
    struct AddExp *prev;
    struct AddExp *next;
};

struct AddOp
{
    int type;      // ADDOP
    int typevalue; // K_ADD K_SUB
};

struct RelExp
{
    int type;            // REPEXP
    struct RelOp *relop; // optional. first elem is null
    struct AddExp *addexp;
    struct RelExp *prev;
    struct RelExp *next;
};

struct RelOp
{
    int type;      // RELOP
    int typevalue; // K_GT : > , K_LT : < , K_GTE : >= , K_LTE : <=
};

struct EqExp
{
    int type;          // EQEXP
    struct EqOp *eqop; // optional. first elem is null
    struct RelExp *relexp;
    struct EqExp *prev;
    struct EqExp *next;
};

struct EqOp
{
    int type;      // EQOP
    int typevalue; // K_EQ K_NEQ
};

struct LAndExp
{
    int type; // LANDEXP
    struct EqExp *eqexp;
    struct LAndExp *prev;
    struct LAndExp *next;
};

struct LAndOp
{
    int type; // LANDOP
};

struct LOrExp
{
    int type; // LOREXP
    struct LAndExp *landexp;
    struct LOrExp *prev;
    struct LOrExp *next;
};

struct LOrOp
{
    int type; // LOROP
};

struct ConstExp
{
    int type; // CONSTEXP
    struct AddExp *addexp;
};

struct Ident
{
    int type; // IDENT
    char *name;
    // struct RefList *reflist;
};

struct String
{
    int type; // STRING
    char *content;
};
#endif