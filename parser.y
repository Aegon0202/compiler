%require "3.5"
%language "c++"
%output "parser.cpp"
%defines
%locations
%lex-param	 { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%define parse.error verbose
%define api.parser.class {Parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { saltyfish }

%code requires
{
		#include <iostream>
		#include <string>
		#include <vector>
		#include <stdint.h>
		#include <utility>
		#include "AST/AST/ASTUnit.h"
		#include "AST/AST/CompUnit.h"
		#include "AST/AST/ConstantInt.h"
		#include "AST/AST/Ident.h"
		#include "AST/AST/Type.h"
		#include "AST/AST/ValueDef.h"
		#include "AST/AST/FuncDef.h"
		#include "AST/AST/Decl.h"
		#include "AST/AST/ValueDecl.h"
		#include "AST/AST/FuncDecl.h"
		#include "AST/AST/FuncParamDecl.h"
		#include "AST/AST/Exp.h"
		#include "AST/AST/BinaryExp.h"
		#include "AST/AST/UnaryExp.h"
		#include "AST/AST/PrimaryExp.h"
		#include "AST/AST/FuncallExp.h"
		#include "AST/AST/Stmt.h"
		#include "AST/AST/IfStmt.h"
		#include "AST/AST/WhileStmt.h"
		#include "AST/AST/BreakStmt.h"
		#include "AST/AST/ReturnStmt.h"
		#include "AST/AST/ContinueStmt.h"
		#include "AST/AST/AssignStmt.h"
		#include "AST/AST/EmptyStmt.h"
		#include "AST/AST/BlockStmt.h"
		#include "AST/AST/ExpStmt.h"
		#include "AST/ASTVisitor/MessageVisitor.h"
		using namespace std;

		namespace saltyfish {
			class Driver;
			class Scanner;
		}

		#define BINARY_EXP(a,b,c,binaryExpType,d) {	\
			unique_ptr<Exp> Lexp=std::move((b));unique_ptr<Exp> Rexp=std::move((c));	\
			auto e=make_unique<BinaryExp>(BinaryExp::BinaryExpType::binaryExpType,std::move(Lexp),std::move(Rexp),d);	\
			(a)=std::move(e);	\
		}
}

%code top
{
		#include <iostream>
		#include "scanner.hpp"
		#include "parser.hpp"
		#include "location.hh"
		#include "driver.hpp"
		using namespace saltyfish;
}

%code
{
	#define yylex scanner.get_next_token
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

%token <std::string> T_CONST "const关键字";
%token <std::string> T_INT   "int关键字"
%token <std::string> T_VOID "void关键字"

%token <std::string> T_IF "if关键字"
%token <std::string> T_ELSE "else关键字"
%token <std::string> T_WHILE "while关键字"
%token <std::string> T_BREAK "break关键字"
%token <std::string> T_CONTINUE "continue关键字"
%token <std::string> T_RETURN "return关键字"

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

%left T_EQUAL
%left T_OR
%left  T_AND
%left T_B T_A T_BE T_AE
%left T_E T_NE
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
%right T_NOT

%token <std::string> IdentStr "标识符"
%token <int> DECIMAL "十进制常数"
%token <int> OCTAL "八进制常数"
%token <int> HEXADECIMAL "十六进制常数"

%token END 0 "文件结尾"

%nonassoc T_LOWER_THEN_ELSE
%nonassoc T_ELSE

%type < unique_ptr<ConstantInt> > Number
%type < unique_ptr<Ident> >       Ident
%type < unique_ptr<Type> >        BType
%type < unique_ptr<ValueDef> >      ValueDef
%type < std::vector<unique_ptr<ValueDef>> > ValueDefList
%type < unique_ptr<Exp> >         Exp
%type < unique_ptr<ValueDecl> >    ValueDecl
%type < unique_ptr<FuncParamDecl> > FuncParamDecl
%type < std::vector<unique_ptr<FuncParamDecl>> > FuncParamDeclList
%type < std::vector<unique_ptr<Exp>> >  FuncallParamList
%type < std::vector<unique_ptr<Exp>> >			ArrayDimSubList ArrayDimList 
%type < std::unique_ptr<Stmt> >		Stmt
%type < std::unique_ptr<Decl>>		Decl
%type < std::vector<std::unique_ptr<ASTUnit>> > BlockItemList
%type < std::unique_ptr<BlockStmt> >	Block
%type < std::unique_ptr<FuncDef> >	FuncDef
%type < std::vector<std::unique_ptr<ASTUnit>> > CompUnitList
%type < std::unique_ptr<CompUnit> >	CompUnit


%start CompUnit

%%

CompUnit:
	CompUnitList
		{
			std::vector<std::unique_ptr<ASTUnit>> &compUnitList=$1;
			auto compUnit=std::make_unique<CompUnit>(std::move(compUnitList),@$);
			MessageVisitor visitor;
			compUnit->accept(visitor);
			$$=std::move(compUnit);
		}

CompUnitList:
	%empty
		{
			$$=std::vector<std::unique_ptr<ASTUnit>>();
		}
|	CompUnitList Decl
		{
			std::unique_ptr<ASTUnit> decl=std::move($2);
			std::vector<std::unique_ptr<ASTUnit>> &compUnitList=$1;
			compUnitList.push_back(std::move(decl));
			$$=std::move(compUnitList);
		}
|	CompUnitList FuncDef
		{
			std::unique_ptr<ASTUnit> funcDef=std::move($2);
			std::vector<std::unique_ptr<ASTUnit>> &compUnitList=$1;
			compUnitList.push_back(std::move(funcDef));
			$$=std::move(compUnitList);
		}

ArrayDimList:
	%empty
		{
			std::vector<unique_ptr<Exp>> arrayDimList;
			$$=std::move(arrayDimList);
		}
|	ArrayDimSubList
		{
			std::vector<unique_ptr<Exp>> &arrayDimList=$1;
			$$=std::move(arrayDimList);
		}
|	T_LM T_RM ArrayDimSubList
		{
			std::vector<unique_ptr<Exp>> &arrayDimList=$3;
			arrayDimList.insert(arrayDimList.begin(),nullptr);
			$$=std::move(arrayDimList);
		}

ArrayDimSubList:
	T_LM Exp T_RM
		{
			std::vector<unique_ptr<Exp>> arrayDimSubList;
			unique_ptr<Exp> exp=std::move($2);
			arrayDimSubList.push_back(std::move(exp));
			$$=std::move(arrayDimSubList);
		}
|	ArrayDimSubList T_LM Exp T_RM
		{
			std::vector<unique_ptr<Exp>> &arrayDimSubList=$1;
			unique_ptr<Exp> exp=std::move($3);
			arrayDimSubList.push_back(std::move(exp));
			$$=std::move(arrayDimSubList);
		}

BType:
	T_INT {$$=std::make_unique<Type>($1);}
|	T_VOID {$$=std::make_unique<Type>($1);}

Decl:
	ValueDecl	{$$=std::move($1);}

ValueDecl:
	T_CONST BType ValueDefList T_SEMICOLON
		{

		}
|	BType ValueDefList T_SEMICOLON
		{
			unique_ptr<Type> type=std::move($1);
			std::vector<unique_ptr<ValueDef>> &valueDefList=$2;
			auto v=make_unique<ValueDecl>(std::move(type),std::move(valueDefList),@$);
			$$=std::move(v);
			cout<<@$<<endl;
		}

ValueDefList:
	ValueDef
		{
			unique_ptr<ValueDef> valueDef=std::move($1);
			$$=std::vector<unique_ptr<ValueDef>>();
			$$.push_back(std::move(valueDef));
		}
|	ValueDefList T_COMMA ValueDef
		{
			unique_ptr<ValueDef> &valueDef=$3;
			std::vector<unique_ptr<ValueDef>> &varDefList=$1;
			varDefList.push_back(std::move(valueDef));
			$$=std::move(varDefList);
		}

ValueDef:
	Ident ArrayDimList
		{
			unique_ptr<Ident> &ident=$1;
			std::vector<unique_ptr<Exp>> &arrayDimList=$2;
			auto valueDef=make_unique<ValueDef>(std::move(ident),std::move(arrayDimList),@$);
			$$=std::move(valueDef);
		}
|	Ident ArrayDimList T_EQUAL Exp
		{
			unique_ptr<Ident> ident=std::move($1);
			std::vector<unique_ptr<Exp>> &arrayDimList=$2;
			auto valueDef=make_unique<ValueDef>(std::move(ident),std::move(arrayDimList),@$);
			$$=std::move(valueDef);
		}
|	Ident ArrayDimList T_EQUAL ArrayAssignList
		{

		}

ArrayAssignList:
	T_LB T_RB {}
|	T_LB ArrayAssignSubList T_RB {}

ArrayAssignSubList:
	Exp
		{
		}
|	ArrayAssignSubList T_COMMA Exp
		{
		}
|	ArrayAssignSubList T_COMMA ArrayAssignList
		{
		}

FuncDef:
	BType Ident T_LS T_RS Block
	{
		std::unique_ptr<Type> &type=$1;
		std::unique_ptr<Ident> &ident=$2;
		std::unique_ptr<BlockStmt> &block=$5;
		auto funcDef=std::make_unique<FuncDef>(std::move(type),std::move(ident),std::move(block),@$);
		$$=std::move(funcDef);
	}
|	BType Ident T_LS FuncParamDeclList T_RS Block
		{
			std::unique_ptr<Type> &type=$1;
			std::unique_ptr<Ident> &ident=$2;
			std::vector<std::unique_ptr<FuncParamDecl>> &funcParamDeclList=$4;
			std::unique_ptr<BlockStmt> &block=$6;
			auto funcDef=std::make_unique<FuncDef>(std::move(type),std::move(ident),std::move(funcParamDeclList),std::move(block),@$);
			$$=std::move(funcDef);
		}

FuncParamDeclList:
	FuncParamDecl
		{
			$$=std::vector<unique_ptr<FuncParamDecl>>();
			unique_ptr<FuncParamDecl> funcParamDecl=std::move($1);
			$$.push_back(std::move(funcParamDecl));
		}
|	FuncParamDeclList T_COMMA FuncParamDecl
		{
			std::vector<unique_ptr<FuncParamDecl>> &funcParamDeclList=$1;
			unique_ptr<FuncParamDecl> funcParamDecl=std::move($3);
			funcParamDeclList.push_back(std::move(funcParamDecl));
			$$=std::move(funcParamDeclList);
		}

FuncParamDecl:
	BType Ident
		{
			unique_ptr<Type> type=std::move($1);
			unique_ptr<Ident> ident=std::move($2);
			unique_ptr<FuncParamDecl> funcParamDecl=make_unique<FuncParamDecl>(std::move(type),std::move(ident),@$);
			$$=std::move(funcParamDecl);
		}

Block:
	T_LB T_RB {$$=std::move(std::make_unique<BlockStmt>());}
|	T_LB BlockItemList T_RB
		{
			std::vector<std::unique_ptr<ASTUnit>> &blockItemList=$2;
			auto block=std::make_unique<BlockStmt>(std::move(blockItemList),@$);
			$$=std::move(block);
		}

BlockItemList:
	Decl
		{
			$$=std::vector<std::unique_ptr<ASTUnit>>();
			std::unique_ptr<ASTUnit> decl=std::move($1);
			$$.push_back(std::move(decl));
		}
|	Stmt
		{
			$$=std::vector<std::unique_ptr<ASTUnit>>();
			std::unique_ptr<ASTUnit> stmt=std::move($1);
			$$.push_back(std::move(stmt));
		}
|	BlockItemList Decl
		{
			std::vector<std::unique_ptr<ASTUnit>> &blockItemList=$1;
			std::unique_ptr<ASTUnit> unit=std::move($2);
			blockItemList.push_back(std::move(unit));
			$$=std::move(blockItemList);
		}
|	BlockItemList Stmt
		{
			std::vector<std::unique_ptr<ASTUnit>> &blockItemList=$1;
			std::unique_ptr<ASTUnit> unit=std::move($2);
			blockItemList.push_back(std::move(unit));
			$$=std::move(blockItemList);
		}

Stmt:
	Ident T_EQUAL Exp T_SEMICOLON
		{
			std::unique_ptr<Ident> &ident=$1;
			std::unique_ptr<Exp> &exp=$3;
			auto assignStmt=std::make_unique<AssignStmt>(std::move(ident),std::move(exp),@$);
			$$=std::move(assignStmt);
		}
|	T_SEMICOLON
		{
			auto emptyStmt=std::make_unique<EmptyStmt>(@$);
			$$=std::move(emptyStmt);
		}
|	Exp T_SEMICOLON
		{
			std::unique_ptr<Exp> &exp=$1;
			auto expStmt=std::make_unique<ExpStmt>(std::move(exp),@$);
			$$=std::move(expStmt);
		}
|	Block {$$=std::move($1);}
|	T_IF T_LS Exp T_RS Stmt %prec T_LOWER_THEN_ELSE
		{
			std::unique_ptr<Exp> &cond=$3;
			std::unique_ptr<Stmt> &ifBody=$5;
			std::unique_ptr<IfStmt> ifStmt=std::make_unique<IfStmt>(std::move(cond),std::move(ifBody),@$);
			$$=std::move(ifStmt);
		}
|	T_IF T_LS Exp T_RS Stmt T_ELSE Stmt
		{
			std::unique_ptr<Exp> &cond=$3;
			std::unique_ptr<Stmt> &ifBody=$5;
			std::unique_ptr<Stmt> &elseBody=$7;
			auto ifStmt=std::make_unique<IfStmt>(std::move(cond),std::move(ifBody),std::move(elseBody),@$);
			$$=std::move(ifStmt);
		}
|	T_WHILE T_LS Exp T_RS Stmt
		{
			std::unique_ptr<Exp> &cond=$3;
			std::unique_ptr<Stmt> &body=$5;
			auto whileStmt=std::make_unique<WhileStmt>(std::move(cond),std::move(body),@$);
			$$=std::move(whileStmt);
		}
|	T_BREAK T_SEMICOLON
		{
			auto breakStmt=std::make_unique<BreakStmt>(@$);
			$$=std::move(breakStmt);
		}
|	T_CONTINUE T_SEMICOLON
		{
			auto continueStmt=std::make_unique<ContinueStmt>(@$);
			$$=std::move(continueStmt);
		}
|	T_RETURN Exp T_SEMICOLON
		{
			std::unique_ptr<Exp> &exp=$2;
			auto returnStmt=std::make_unique<ReturnStmt>(std::move(exp),@$);
			$$=std::move(returnStmt);
		}
|	T_RETURN T_SEMICOLON
		{
			auto returnStmt=std::make_unique<ReturnStmt>(@$);
			$$=std::move(returnStmt);
		}

Ident:
	IdentStr
		{
			std::string identStr=$1;
			auto ident=std::make_unique<Ident>(identStr,@$);
			$$=std::move(ident);
		}

Number:
	DECIMAL
		{
			int n=$1;
			auto constant=std::make_unique<ConstantInt>(ConstantInt::ConstantIntType::Decimal,n,@$);
			$$=std::move(constant);
		}
|	OCTAL
		{
			int n=$1;
			auto constant=make_unique<ConstantInt>(ConstantInt::ConstantIntType::Octal,n,@$);
			$$=std::move(constant);
		}
|	HEXADECIMAL
		{
			int n=$1;
			auto constant=make_unique<ConstantInt>(ConstantInt::ConstantIntType::Hexadecimal,n,@$);
			$$=std::move(constant);
		}

FuncallParamList:
	Exp
		{
			$$=std::vector<unique_ptr<Exp>>();
			std::unique_ptr<Exp> &exp=$1;
			$$.push_back(std::move(exp));
		}
|	FuncallParamList T_COMMA Exp
		{
			std::unique_ptr<Exp> &exp=$3;
			std::vector<std::unique_ptr<Exp>> &funcParamList=$1;
			funcParamList.push_back(std::move(exp));
			$$=std::move(funcParamList);
		}

Exp:
	Exp T_MUL Exp	{BINARY_EXP($$,$1,$3,Mul,@$);}
|	Exp T_DIV Exp	{BINARY_EXP($$,$1,$3,Div,@$);}
|	Exp T_MOD Exp	{BINARY_EXP($$,$1,$3,Mod,@$);}
|	Exp T_ADD Exp	{BINARY_EXP($$,$1,$3,Add,@$);}
|	Exp T_SUB Exp	{BINARY_EXP($$,$1,$3,Sub,@$);}
|	Exp T_B Exp		{BINARY_EXP($$,$1,$3,B,@$);}
|	Exp T_A Exp		{BINARY_EXP($$,$1,$3,A,@$);}
|	Exp T_BE Exp	{BINARY_EXP($$,$1,$3,Be,@$);}
|	Exp T_AE Exp	{BINARY_EXP($$,$1,$3,Ae,@$);}
|	Exp T_E Exp		{BINARY_EXP($$,$1,$3,E,@$);}
|	Exp T_NE Exp	{BINARY_EXP($$,$1,$3,Ne,@$);}
|	Exp T_AND Exp	{BINARY_EXP($$,$1,$3,And,@$);}
|	Exp T_OR Exp	{BINARY_EXP($$,$1,$3,Or,@$);}
|	T_LS Exp T_RS	{$$=std::move($2);}
|	Ident			{$$=std::move(make_unique<PrimaryExp>(PrimaryExp::PrimaryExpType::Ident,std::move($1),@$));}
|	Number			{$$=std::move(make_unique<PrimaryExp>(PrimaryExp::PrimaryExpType::ConstantInt,std::move($1),@$));}
|	Ident T_LS T_RS	{$$=std::move(make_unique<FuncallExp>(std::move($1),@$));}
|	Ident T_LS FuncallParamList T_RS
	{
		std::vector<std::unique_ptr<Exp>> &funcallParamList=$3;
		std::unique_ptr<Ident> &ident=$1;
		auto funcallExp=make_unique<FuncallExp>(std::move(ident),std::move(funcallParamList));
		$$=std::move(funcallExp);
	}
|	T_ADD Exp		{$$=std::move(make_unique<UnaryExp>(UnaryExp::UnaryExpType::Add,std::move($2),@$));}
|	T_SUB Exp		{$$=std::move(make_unique<UnaryExp>(UnaryExp::UnaryExpType::Sub,std::move($2),@$));}
|	T_NOT Exp		{$$=std::move(make_unique<UnaryExp>(UnaryExp::UnaryExpType::Not,std::move($2),@$));}
%%
namespace saltyfish
{
	// Report an error to the user.
	auto Parser::error (const saltyfish::location &loc,const std::string& msg) -> void
	{
		std::cerr << msg <<"     location:"<<loc<< '\n';
	}
}