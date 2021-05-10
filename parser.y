%require "3.5"
%language "c++"
%output "parser.cpp" 

%define parse.error verbose

%define api.parser.class {Parser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace {saltyfish}

%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdint.h>

    using namespace std;

    namespace saltyfish {

    }
}

%code top
{
    #include <iostream>
    #include "parser.hpp"
    #include "lexer.hpp"
    using namespace std;
    using namespace saltyfish;
    saltyfish::Parser::symbol_type yylex(){
        static Lexer* lexer = new Lexer();
        return lexer->get_next_token();
    }
}


/* declare token */
%token T_COMMA "逗号"
%token T_SEMICOLON "分号"
%token T_LB "左大括号"
%token T_RB "右大括号"
%token T_LM "左中括号"
%token T_RM "右中括号"
%token T_LS "左小括号"
%token T_RS "右小括号"

%token T_EQUAL  "等号"

%token T_CONST "const关键字";
%token T_INT   "int关键字"
%token T_VOID "void关键字"

%token T_IF "if关键字"
%token T_ELSE "else关键字"
%token T_WHILE "while关键字"
%token T_BREAK "break关键字"
%token T_CONTINUE "continue关键字"
%token T_RETURN "return关键字"

%token T_ADD "加号"
%token T_SUB "减号"
%token T_MUL "乘号"
%token T_DIV "除号"
%token T_MOD "余号"
%token T_NOT "感叹号"
%token T_B "小于号"
%token T_A "大于号"
%token T_BE "小于等于号"
%token T_AE "大于等于号"
%token T_EE "等于等于号"
%token T_NE "不等于号"
%token T_AND "与号"
%token T_OR "或号"

%token <const char *> T_IDENT "标识符"
%token <int> T_DECIMAL "十进制常数"
%token <int> T_OCTAL "八进制常数"
%token <int> T_HEXADECIMAL "十六进制常数"

%token END 0 "文件结尾"

%nonassoc T_IF
%nonassoc K_LOW_THAN_ELSE
%nonassoc T_ELSE

%start CompUnit

%%
CompUnit
    :   Decl 
    |   FuncDef
    |   CompUnit FuncDef 
    |   CompUnit Decl 
    ;

Decl
    :   ConstDecl 
    |   VarDecl
    ;


ConstDecl
    :    T_CONST T_INT ConstDefs T_SEMICOLON 
    ;

ConstDefs
    :   ConstDef
    |   ConstDefs T_COMMA ConstDef
    ;

ConstDef
    :   T_IDENT ArrayDef T_EQUAL ConstInitVal
    |   T_IDENT T_EQUAL ConstInitVal 
    ;

ArrayDef
    :   ArrayDef T_LM ConstExp T_RM
    |   T_LM ConstExp T_RM
    ;

ConstInitVal
    :   Number
    ;

VarDecl
    :   T_INT VarDefs T_SEMICOLON
    ;

VarDefs
    :   VarDef
    |   VarDefs T_COMMA VarDef
    ;

VarDef
    :   T_IDENT ArrayDef T_EQUAL InitVal 
    |   T_IDENT ArrayDef
    |   T_IDENT
    |   T_IDENT T_EQUAL InitVal
    ;


InitVal
    :   Exp
    |   T_LB InitVals T_RB
    |   T_LB T_RB
    ;

InitVals
    :   InitVal
    |   InitVals T_COMMA InitVal
    ;


FuncDef
    :   T_INT T_IDENT T_LS FuncFParams T_RS Block
    |   T_VOID T_IDENT T_LS FuncFParams T_RS Block
    |   T_INT T_IDENT T_LS T_RS Block
    |   T_VOID T_IDENT T_LS T_RS Block
    ;

FuncFParams
    :   FuncFParam
    |   FuncFParams T_COMMA FuncFParam
    ;

FuncFParam
    :   T_INT T_IDENT
    |   T_INT T_IDENT T_LM T_RM ExpArrayDefs
    |   T_INT T_IDENT T_LM T_RM
    ;

ExpArrayDefs
    :   ExpArrayDef
    |   ExpArrayDefs ExpArrayDef
    ;

ExpArrayDef
    :   T_LM Exp T_RM
    ;

Block
    :   T_LB BlockItems T_RB
    |   T_LB T_RB
    ;

BlockItems
    :   BlockItem
    |   BlockItems BlockItem
    ;

BlockItem
    :   Decl
    |   Stmt
    ;

Stmt
    :   LVal T_EQUAL Exp T_SEMICOLON
    |   Exp T_SEMICOLON
    |   T_SEMICOLON
    |   Block
    |   T_IF T_LS Condition T_RS Stmt %prec K_LOW_THAN_ELSE
    |   T_IF T_LS Condition T_RS Stmt T_ELSE Stmt   {cout<< "if-else语句"<<endl;}
    |   T_WHILE T_LS Condition T_RS Stmt
    |   T_BREAK T_SEMICOLON
    |   T_CONTINUE T_SEMICOLON
    |   T_RETURN T_SEMICOLON
    |   T_RETURN Exp T_SEMICOLON
    ;

Exp
    :   AddExp
    ;

Condition
    : LOrExp
    ;

LVal
    :   T_IDENT ExpArrayDefs
    |   T_IDENT
    ;

PrimaryExp
    :   T_LS Exp T_RS
    |   LVal
    |   Number
    ;

Number 
    :   IntConst
    ;

UnaryExp
    :   PrimaryExp
    |   T_IDENT T_LS FuncRParams T_RS
    |   T_IDENT T_LS T_RS
    |   UnaryOp UnaryExp
    ;

UnaryOp
    :   T_ADD
    |   T_SUB
    |   T_NOT
    ;

FuncRParams
    :   FuncRParam
    |   FuncRParams T_COMMA FuncRParam
    ;

FuncRParam
    : Exp
    ;

MulExp
    :   UnaryExp
    |   MulExp MulOp UnaryExp
    ;

MulOp   
    :   T_MUL
    |   T_DIV
    |   T_MOD
    ;

AddExp
    :   MulExp
    |   AddExp AddOp MulExp
    ;

AddOp
    :   T_ADD
    |   T_SUB
    ;

RelExp
    :   AddExp
    |   RelExp RelOp AddExp
    ;

RelOp
    :   T_B
    |   T_BE
    |   T_A
    |   T_AE
    ;

EqExp
    :   RelExp
    |   EqExp EqOp RelExp
    ;

EqOp   
    :   T_EE
    |   T_NE
    ;

LAndExp
    :   EqExp 
    |   LAndExp T_AND EqExp
    ;

LOrExp
    :   LAndExp
    |   LOrExp T_OR LAndExp
    ;

ConstExp
    :   AddExp
    ;

IntConst
    :   T_DECIMAL
    |   T_OCTAL
    |   T_HEXADECIMAL
    ;

%%

namespace saltyfish
{
  auto Parser::error (const std::string& msg) -> void
  {
    std::cerr << msg << '\n';
  }
}

int main ()
{
  saltyfish::Parser parser;
  return parser.parse();
}


