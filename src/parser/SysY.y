%{
    #include "./SysY.tab.h"
    #include "../SysY.type/SysY.type.def.h"
    #include "../SysY.type/SysY.type.new.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #define YYERROR_VERBOSE
    extern YYSTYPE result;
    int yylex();
    void yyerror(const char *s);
%}

%union {
    struct Keyword *keyword;
    struct CompUnit *compunit;
    struct Decl *decl;
    struct ConstDecl *constdecl;
    struct BType *btype;
    struct ConstDefs *constdefs;
    struct ConstDef *constdef;
    struct ConstArrayDefs *constarraydefs;
    struct ConstArrayDef *constarraydef;
    struct ConstInitVal *constinitval;
    struct ConstInitVals *constinitvals;
    struct VarDecl *vardecl;
    struct VarDefs *vardefs;
    struct VarDef *vardef;
    struct InitVal *initval;
    struct InitVals *initvals;
    struct FuncDef *funcdef;
    struct FuncType *functype;
    struct FuncFParams *funcfparams;
    struct FuncFParam *funcfparam;
    struct ExpArrayDefs *exparraydefs;
    struct ExpArrayDef *exparraydef;
    struct Block *block;
    struct BlockItems *blockitems;
    struct BlockItem *blockitem;
    struct Stmt *stmt;
    struct Assign *assign;
    struct IfStmt *ifstmt;
    struct WhileStmt *whilestmt;
    struct ReturnStmt *returnstmt;
    struct Exp *exp;
    struct Cond *cond;
    struct LVal *lval;
    struct ArrayImpl *arrayimpl;
    struct PrimaryExp *primaryexp;
    struct Number *number;
    struct IntConst *intconst;
    struct UnaryExp *unaryexp;
    struct FuncImpl *funcimpl;
    struct UnaryExps *unaryexps;
    struct UnaryOp *unaryop;
    struct FuncRParams *funcrparams;
    struct FuncRParam *funcrparam;
    struct MulExp *mulexp;
    struct MulOp *mulop;
    struct AddExp *addexp;
    struct AddOp *addop;
    struct RelExp *relexp;
    struct RelOp *relop;
    struct EqExp *eqexp;
    struct EqOp *eqop;
    struct LAndExp *landexp;
    struct LAndOp *landop;
    struct LOrExp *lorexp;
    struct LOrOp *lorop;
    struct ConstExp *constexp;
    struct Ident *ident;
    struct RefList *reflist;
    struct String* string;
}

%token <intconst>INTCONST

%token <mulop> MULOP
%token <eqop> EQOP
%token <relop> RELOP
%token <string> STRING

%token K_CURLY_L 
%token K_CURLY_R 
%token K_SQUARE_L 
%token K_SQUARE_R
%token K_PARENTHESES_L
%token K_PARENTHESES_R
%token K_COMMA
%token K_SEMICOLON
%token K_ASSIGNOP
%token K_ADD
%token K_SUB

%token K_MUL
%token K_DIV
%token K_MOD
%token K_GT
%token K_GTE
%token K_LT
%token K_LTE
%token K_EQ
%token K_NEQ
%token K_AND
%token K_OR
%token K_NOT

%token KEYWORD
%token K_CONST K_VOID K_INT
%token K_IF
%token K_ELSE
%token K_WHILE
%token K_BREAK
%token K_CONTINUE
%token K_RETURN

%token <ident>IDENT

%token COMMENT
%token FILEEND

%type <compunit> COMPUNIT
%type <decl> DECL
%type <constdecl> CONSTDECL
%type <constdefs> CONSTDEFS
%type <constdef> CONSTDEF
%type <constarraydefs> CONSTARRAYDEFS
%type <constarraydef> CONSTARRAYDEF
%type <constinitval> CONSTINITVAL
%type <constinitvals> CONSTINITVALS
%type <vardecl> VARDECL
%type <vardefs> VARDEFS
%type <vardef> VARDEF
%type <initval> INITVAL
%type <initvals> INITVALS
%type <funcdef> FUNCDEF
%type <funcfparams> FUNCFPARAMS
%type <funcfparam> FUNCFPARAM
%type <exparraydefs> EXPARRAYDEFS
%type <exparraydef> EXPARRAYDEF
%type <block> BLOCK
%type <blockitems> BLOCKITEMS
%type <blockitem> BLOCKITEM
%type <stmt> STMT
%type <exp> EXP
%type <cond> COND
%type <lval> LVAL
%type <primaryexp> PRIMARYEXP
%type <number> NUMBER
%type <unaryexp> UNARYEXP
%type <unaryop> UNARYOP
%type <funcrparams> FUNCRPARAMS
%type <funcrparam> FUNCRPARAM
%type <mulexp> MULEXP
%type <addexp> ADDEXP
%type <relexp> RELEXP
%type <eqexp> EQEXP
%type <landexp> LANDEXP
%type <lorexp> LOREXP
%type <constexp> CONSTEXP

%nonassoc K_IF
%nonassoc K_LOW_THAN_ELSE
%nonassoc K_ELSE

%%
COMPUNIT
    :   DECL    { $$ = newCompUnit(COMPUNIT,DECL,$1,NULL); result.compunit = $$->next; }
    |   FUNCDEF { $$ = newCompUnit(COMPUNIT,FUNCDEF,$1,NULL); result.compunit = $$->next; }
    |   COMPUNIT DECL   { $$ = newCompUnit(COMPUNIT,DECL,$2,$1); result.compunit = $$->next; }
    |   COMPUNIT FUNCDEF    { $$ = newCompUnit(COMPUNIT,FUNCDEF,$2,$1); result.compunit = $$->next; }
    ;

DECL
    :   CONSTDECL   { $$ = newDecl(DECL,CONSTDECL,$1); }
    |   VARDECL { $$ = newDecl(DECL,VARDECL,$1); }
    ;

CONSTDECL
    :   K_CONST K_INT CONSTDEFS K_SEMICOLON { $$ = newConstDecl(CONSTDECL,newBType(BTYPE,K_INT),$3->next); }
    ;

CONSTDEFS
    :   CONSTDEF    { $$ = newConstDefs(CONSTDEFS,$1,NULL); }
    |   CONSTDEFS K_COMMA CONSTDEF  { $$ = newConstDefs(CONSTDEFS,$3,$1); }
    ;

CONSTDEF
    :   IDENT K_ASSIGNOP CONSTINITVAL   { $$ = newConstDef(CONSTDEF,$1,NULL,$3); }
    |   IDENT CONSTARRAYDEFS K_ASSIGNOP CONSTINITVAL    { $$ = newConstDef(CONSTDEF,$1,$2->next,$4); }
    ;

CONSTARRAYDEFS
    :   CONSTARRAYDEF   { $$ = newConstArrayDefs(CONSTARRAYDEFS,$1,NULL); }
    |   CONSTARRAYDEFS CONSTARRAYDEF    { $$ = newConstArrayDefs(CONSTARRAYDEFS,$2,$1); }
    ;

CONSTARRAYDEF
    :   K_SQUARE_L CONSTEXP K_SQUARE_R  { $$ = newConstArrayDef(CONSTARRAYDEF,$2); }
    ;

CONSTINITVAL
    :   CONSTEXP    { $$ = newConstInitVal(CONSTINITVAL,CONSTEXP,$1); }
    |   K_CURLY_L K_CURLY_R { $$ = newConstInitVal(CONSTINITVAL,CONSTINITVALS,newConstInitVals(CONSTINITVALS,NULL,NULL)); }
    |   K_CURLY_L CONSTINITVALS K_CURLY_R   { $$ = newConstInitVal(CONSTINITVAL,CONSTINITVALS,$2->next); }
    ;

CONSTINITVALS
    :   CONSTINITVAL    { $$ = newConstInitVals(CONSTINITVALS,$1,NULL); }
    |   CONSTINITVALS K_COMMA CONSTINITVAL  { $$ = newConstInitVals(CONSTINITVALS,$3,$1); }
    ;

VARDECL
    :   K_INT VARDEFS K_SEMICOLON   { $$ = newVarDecl(VARDECL,newBType(BTYPE,K_INT),$2->next); }
    ;

VARDEFS
    :   VARDEF  { $$ = newVarDefs(VARDEFS,$1,NULL); }
    |   VARDEFS K_COMMA VARDEF  { $$ = newVarDefs(VARDEFS,$3,$1); }
    ;

VARDEF
    :   IDENT   { $$ = newVarDef(VARDEF,$1,NULL,NULL); }
    |   IDENT CONSTARRAYDEFS    { $$ = newVarDef(VARDEF,$1,$2->next,NULL); }
    |   IDENT K_ASSIGNOP INITVAL    { $$ = newVarDef(VARDEF,$1,NULL,$3); }
    |   IDENT CONSTARRAYDEFS K_ASSIGNOP INITVAL { $$ = newVarDef(VARDEF,$1,$2->next,$4); }
    ;

INITVAL
    :   EXP { $$ = newInitVal(INITVAL,EXP,$1); }
    |   K_CURLY_L K_CURLY_R { $$ = newInitVal(INITVAL,INITVALS,newInitVals(INITVALS,NULL,NULL)); }
    |   K_CURLY_L INITVALS K_CURLY_R    { $$ = newInitVal(INITVAL,INITVALS,$2->next); }
    ;

INITVALS
    :   INITVAL { $$ = newInitVals(INITVALS,$1,NULL); }
    |   INITVALS K_COMMA INITVAL    { $$ = newInitVals(INITVALS,$3,$1); }
    ;

FUNCDEF
    :   K_INT IDENT K_PARENTHESES_L K_PARENTHESES_R BLOCK   { $$ = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_INT),$2,newFuncFParams(FUNCFPARAMS,NULL,NULL),$5); }
    |   K_VOID IDENT K_PARENTHESES_L K_PARENTHESES_R BLOCK  { $$ = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_VOID),$2,newFuncFParams(FUNCFPARAMS,NULL,NULL),$5); }
    |   K_INT IDENT K_PARENTHESES_L FUNCFPARAMS K_PARENTHESES_R BLOCK   { $$ = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_INT),$2,$4->next,$6); }
    |   K_VOID IDENT K_PARENTHESES_L FUNCFPARAMS K_PARENTHESES_R BLOCK  { $$ = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_VOID),$2,$4->next,$6); }
    ;

FUNCFPARAMS
    :   FUNCFPARAM  { $$ = newFuncFParams(FUNCFPARAMS,$1,NULL); }
    |   FUNCFPARAMS K_COMMA FUNCFPARAM  { $$ = newFuncFParams(FUNCFPARAMS,$3,$1); }
    ;

FUNCFPARAM
    :   K_INT IDENT { $$ = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),$2,NULL); }
    |   K_INT IDENT K_SQUARE_L K_SQUARE_R   { $$ = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),$2,newExpArrayDefs(EXPARRAYDEFS,newExpArrayDef(EXPARRAYDEF,NULL),NULL)); }
    |   K_INT IDENT K_SQUARE_L K_SQUARE_R EXPARRAYDEFS  { $$ = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),$2,newExpArrayDefs(EXPARRAYDEFS,newExpArrayDef(EXPARRAYDEF,NULL),$5)); }
    ;

EXPARRAYDEFS
    :   EXPARRAYDEF { $$ = newExpArrayDefs(EXPARRAYDEFS,$1,NULL); }
    |   EXPARRAYDEFS EXPARRAYDEF    { $$ = newExpArrayDefs(EXPARRAYDEFS,$2,$1); }
    ;

EXPARRAYDEF
    :   K_SQUARE_L EXP K_SQUARE_R   { $$ = newExpArrayDef(EXPARRAYDEF,$2); }
    ;

BLOCK
    :   K_CURLY_L K_CURLY_R { $$ = newBlock(BLOCK,NULL); }
    |   K_CURLY_L BLOCKITEMS K_CURLY_R  { $$ = newBlock(BLOCK,$2->next); }
    ;

BLOCKITEMS
    :   BLOCKITEM   { $$ = newBlockItems(BLOCKITEMS,$1,NULL); }
    |   BLOCKITEMS BLOCKITEM    { $$ = newBlockItems(BLOCKITEMS,$2,$1); }
    ;

BLOCKITEM
    :   DECL    { $$ = newBlockItem(BLOCKITEM,DECL,$1); }
    |   STMT    { $$ = newBlockItem(BLOCKITEM,STMT,$1); }
    ;

STMT
    :   LVAL K_ASSIGNOP EXP K_SEMICOLON { $$ = newStmt(STMT,ASSIGN,newAssign(ASSIGN,$1,$3)); }
    |   K_SEMICOLON { $$ = newStmt(STMT,EXP,newExp(EXP,NULL)); }
    |   EXP K_SEMICOLON { $$ = newStmt(STMT,EXP,$1); }
    |   BLOCK   { $$ = newStmt(STMT,BLOCK,$1); }
    |   K_IF K_PARENTHESES_L COND K_PARENTHESES_R STMT %prec K_LOW_THAN_ELSE { $$ = newStmt(STMT,IFSTMT,newIfStmt(IFSTMT,$3,$5,NULL)); }
    |   K_IF K_PARENTHESES_L COND K_PARENTHESES_R STMT K_ELSE STMT  { $$ = newStmt(STMT,IFSTMT,newIfStmt(IFSTMT,$3,$5,$7)); }
    |   K_WHILE K_PARENTHESES_L COND K_PARENTHESES_R STMT   { $$ = newStmt(STMT,WHILESTMT,newWhileStmt(WHILESTMT,$3,$5)); }
    |   K_BREAK K_SEMICOLON { $$ = newStmt(STMT,BREAKSTMT,NULL); }
    |   K_CONTINUE K_SEMICOLON  { $$ = newStmt(STMT,CONTINUESTMT,NULL); }
    |   K_RETURN K_SEMICOLON    { $$ = newStmt(STMT,RETURNSTMT,newReturnStmt(RETURNSTMT,NULL)); }
    |   K_RETURN EXP K_SEMICOLON    { $$ = newStmt(STMT,RETURNSTMT,newReturnStmt(RETURNSTMT,$2)); }
    ;

EXP
    :   ADDEXP  { $$ = newExp(EXP,$1->next); }
    ;

COND
    :   LOREXP  { $$ = newCond(COND,$1->next); }
    ;

LVAL
    :   IDENT   { $$ = newLVal(LVAL,IDENT,$1); }
    |   IDENT EXPARRAYDEFS  { $$ = newLVal(LVAL,ARRAYIMPL,newArrayImpl(ARRAYIMPL,$1,$2->next)); }
    ;

PRIMARYEXP
    :   K_PARENTHESES_L EXP K_PARENTHESES_R { $$ = newPrimaryExp(PRIMARYEXP,EXP,$2); }
    |   LVAL    { $$ = newPrimaryExp(PRIMARYEXP,LVAL,$1); }
    |   NUMBER  { $$ = newPrimaryExp(PRIMARYEXP,NUMBER,$1); }
    ;

NUMBER
    :   INTCONST    { $$ = newNumber(NUMBER,$1); }
    ;

UNARYEXP
    :   PRIMARYEXP  { $$ = newUnaryExp(UNARYEXP,PRIMARYEXP,$1); }
    |   IDENT K_PARENTHESES_L K_PARENTHESES_R   { $$ = newUnaryExp(UNARYEXP,FUNCIMPL,newFuncImpl(FUNCIMPL,$1,newFuncRParams(FUNCRPARAMS,NULL,NULL))); }
    |   IDENT K_PARENTHESES_L FUNCRPARAMS K_PARENTHESES_R   { $$ = newUnaryExp(UNARYEXP,FUNCIMPL,newFuncImpl(FUNCIMPL,$1,$3->next)); }
    |   UNARYOP UNARYEXP    { $$ = newUnaryExp(UNARYEXP,UNARYEXPS,newUnaryExps(UNARYEXPS,$1,$2)); }
    ;

UNARYOP
    :   K_ADD   { $$ = newUnaryOp(UNARYOP,K_ADD); }
    |   K_SUB   { $$ = newUnaryOp(UNARYOP,K_SUB); }
    |   K_NOT   { $$ = newUnaryOp(UNARYOP,K_NOT); }
    ;

FUNCRPARAMS
    :   FUNCRPARAM  { $$ = newFuncRParams(FUNCRPARAMS,$1,NULL); }
    |   FUNCRPARAMS K_COMMA FUNCRPARAM  { $$ = newFuncRParams(FUNCRPARAMS,$3,$1); }
    ;

FUNCRPARAM
    :   EXP { $$ = newFuncRParam(FUNCRPARAM,EXP,$1); }
    |   STRING  { $$ = newFuncRParam(FUNCRPARAM,STRING,$1); }

MULEXP
    :   UNARYEXP    { $$ = newMulExp(MULEXP,NULL,$1,NULL); }
    |   MULEXP MULOP UNARYEXP   { $$ = newMulExp(MULEXP,$2,$3,$1); }
    ;

ADDEXP
    :   MULEXP  { $$ = newAddExp(ADDEXP,NULL,$1->next,NULL); }
    |   ADDEXP K_ADD MULEXP { $$ = newAddExp(ADDEXP,newAddOp(ADDOP,K_ADD),$3->next,$1); }
    |   ADDEXP K_SUB MULEXP { $$ = newAddExp(ADDEXP,newAddOp(ADDOP,K_SUB),$3->next,$1); }
    ;

RELEXP
    :   ADDEXP  { $$ = newRelExp(RELEXP,NULL,$1->next,NULL); }
    |   RELEXP RELOP ADDEXP { $$ = newRelExp(RELEXP,$2,$3->next,$1); }
    ;

EQEXP
    :   RELEXP  { $$ = newEqExp(EQEXP,NULL,$1->next,NULL); }
    |   EQEXP EQOP RELEXP   { $$ = newEqExp(EQEXP,$2,$3->next,$1); }
    ;

LANDEXP
    :   EQEXP   { $$ = newLAndExp(LANDEXP,$1->next,NULL); }
    |   LANDEXP K_AND EQEXP    { $$ = newLAndExp(LANDEXP,$3->next,$1); }
    ;

LOREXP
    :   LANDEXP { $$ = newLOrExp(LOREXP,$1->next,NULL); }
    |   LOREXP K_OR LANDEXP    { $$ = newLOrExp(LOREXP,$3->next,$1); }
    ;

CONSTEXP
    :   ADDEXP  { $$ = newConstExp(CONSTEXP,$1->next); }
    ;
%%