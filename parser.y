%require "3.5"
%language "c++"
%output "parser.cpp"
%defines
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

    using namespace saltyfish;
}

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
%token T_E "等于等于号"
%token T_NE "不等于号"
%token T_AND "与号"
%token T_OR "或号"

%token <std::string> Ident "标识符"
%token <int> DECIMAL "十进制常数"
%token <int> OCTAL "八进制常数"
%token <int> HEXADECIMAL "十六进制常数"

%token END 0 "文件结尾"

%code
{
  namespace saltyfish
  {
    saltyfish::Lexer lexer;
    // Return the next token.
    auto yylex () -> Parser::symbol_type
    {      
      return lexer.get_next_token();
    }
  }
}

%start CompUnit

%%
CompUnit:
  %empty
| CompUnit Decl
| CompUnit FuncDef

Decl:
  ConstDecl | VarDecl

ConstDecl:
  T_CONST BType constdef_comma_list T_SEMICOLON {cout<<"常量声明"<<endl;}

constdef_comma_list:
  ConstDef
| constdef_comma_list T_COMMA ConstDef

BType:
  T_INT

ConstDef:
  Ident T_EQUAL ConstInitVal

ConstInitVal:
  Number        {cout<<"数字"<<endl;}

VarDecl:
  BType vardecl_comma_list T_SEMICOLON {cout<<"变量声明"<<endl;}

vardecl_comma_list:
  VarDef
| vardecl_comma_list T_COMMA VarDef

VarDef:
  Ident
| Ident T_EQUAL InitVal

InitVal:
  Exp
| T_LB exp_comma_list T_RB

exp_comma_list:
  Exp T_COMMA Exp
| exp_comma_list T_COMMA Exp

FuncDef:
  FuncType Ident T_LS T_RS Block {cout<<"函数无参声明"<<endl;}
| FuncType Ident T_LS FuncFParams T_RS Block {cout<<"函数有参声明"<<endl;}

FuncType:
  T_VOID | T_INT

FuncFParams:
  FuncFParam
| FuncFParams T_COMMA FuncFParam

FuncFParam:
  BType Ident

Block:
  T_LB T_RB {cout<<"空块"<<endl;}
| T_LB BlockItem T_RB

BlockItem:
  Decl | Stmt

Stmt:
  LVal T_EQUAL Exp T_SEMICOLON
| Block
| T_IF T_LS Cond T_RS Stmt T_ELSE Stmt  {cout<<"if else语句"<<endl;}
| T_IF T_LS Cond T_RS Stmt              {cout<<"if语句"<<endl;}
| T_WHILE T_LS Cond T_RS Stmt           {cout<<"while语句"<<endl;}
| T_BREAK T_SEMICOLON                   {cout<<"break语句"<<endl;}
| T_CONTINUE T_SEMICOLON                {cout<<"continue语句"<<endl;}
| T_RETURN Stmt T_SEMICOLON             {cout<<"return sth.语句"<<endl;}
| T_RETURN T_SEMICOLON                  {cout<<"return语句"<<endl;}

Exp:
  AddExp

Cond:
  LOrExp

LVal:
  Ident

PrimaryExp:
  T_LB Exp T_RB | LVal | Number

Number:
  DECIMAL | OCTAL | HEXADECIMAL

UnaryExp:
  PrimaryExp
| Ident T_LS FuncRParams T_RS 
| UnaryOp UnaryExp

UnaryOp:
  T_ADD | T_SUB |T_NOT

FuncRParams:
  Exp
| FuncRParams T_COMMA Exp

MulExp:
  UnaryExp
| MulExp T_MUL UnaryExp {cout<<"乘法"<<endl;}
| MulExp T_DIV UnaryExp
| MulExp T_MOD UnaryExp

AddExp:
  MulExp
| AddExp T_ADD MulExp {cout<<"加法"<<endl;}
| AddExp T_SUB MulExp

RelExp:
  AddExp
| RelExp T_B AddExp
| RelExp T_A AddExp
| RelExp T_BE AddExp
| RelExp T_AE AddExp

EqExp:
  RelExp
| EqExp T_E RelExp  {cout<<"逻辑等于表达式"<<endl;}
| EqExp T_NE RelExp {cout<<"逻辑不等于表达与"<<endl;}

LAndExp:
  EqExp
| LAndExp T_AND EqExp {cout<<"逻辑与表达式"<<endl;}

LOrExp:
  LAndExp
| LOrExp T_OR LAndExp {cout<<"逻辑或表达式"<<endl;}

ConstExp:
  AddExp

%%
namespace saltyfish
{
  // Report an error to the user.
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