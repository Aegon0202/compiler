/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_FLEX_BISON_SYSY_TAB_H_INCLUDED
# define YY_YY_FLEX_BISON_SYSY_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTCONST = 258,
    MULOP = 259,
    EQOP = 260,
    RELOP = 261,
    STRING = 262,
    K_CURLY_L = 263,
    K_CURLY_R = 264,
    K_SQUARE_L = 265,
    K_SQUARE_R = 266,
    K_PARENTHESES_L = 267,
    K_PARENTHESES_R = 268,
    K_COMMA = 269,
    K_SEMICOLON = 270,
    K_ASSIGNOP = 271,
    K_ADD = 272,
    K_SUB = 273,
    K_MUL = 274,
    K_DIV = 275,
    K_MOD = 276,
    K_GT = 277,
    K_GTE = 278,
    K_LT = 279,
    K_LTE = 280,
    K_EQ = 281,
    K_NEQ = 282,
    K_AND = 283,
    K_OR = 284,
    K_NOT = 285,
    KEYWORD = 286,
    K_CONST = 287,
    K_VOID = 288,
    K_INT = 289,
    K_IF = 290,
    K_ELSE = 291,
    K_WHILE = 292,
    K_BREAK = 293,
    K_CONTINUE = 294,
    K_RETURN = 295,
    IDENT = 296,
    COMMENT = 297,
    FILEEND = 298,
    K_LOW_THAN_ELSE = 299
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "flex.bison/SysY.y"

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

#line 164 "flex.bison/SysY.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_FLEX_BISON_SYSY_TAB_H_INCLUDED  */
