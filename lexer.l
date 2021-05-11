%option yylineno noyywrap c++ yyclass="Lexer" prefix="sf"

%x LINE_COMMENT
%x BLOCK_COMMENT

%{
#include <stdio.h>
#include "lexer.hpp"
#include "location.hh"
#include "index.hpp"
#define yyterminate() saltyfish::Parser::make_END(index.get_location());
#define YY_USER_ACTION \
    index.get_location().step();    \
    index.get_location().columns(yyleng);
%}

%%

"int"                                   {printf("T_INT,%s\n",yytext);return saltyfish::Parser::make_T_INT(index.get_location());}
"const"                                 {printf("T_CONST,%s\n",yytext);return saltyfish::Parser::make_T_CONST(index.get_location());}
"void"                                  {printf("T_VOID,%s\n",yytext);return saltyfish::Parser::make_T_VOID(index.get_location());}
"if"                                    {printf("T_IF,%s\n",yytext);return saltyfish::Parser::make_T_IF(index.get_location());}
"else"                                  {printf("T_ELSE,%s\n",yytext);return saltyfish::Parser::make_T_ELSE(index.get_location());}
"while"                                 {printf("T_WHILE,%s\n",yytext);return saltyfish::Parser::make_T_WHILE(index.get_location());}
"break"                                 {printf("T_BREAK,%s\n",yytext);return saltyfish::Parser::make_T_BREAK(index.get_location());}
"continue"                              {printf("T_CONTINUE,%s\n",yytext);return saltyfish::Parser::make_T_CONTINUE(index.get_location());}
"return"                                {printf("T_RETURN,%s\n",yytext);return saltyfish::Parser::make_T_RETURN(index.get_location());}

[_[:alpha:]]([_[:alpha:]]|[[:digit:]])* {printf("T_IDENT,%s\n",yytext);return saltyfish::Parser::make_T_IDENT(yytext,index.get_location());}

[123456789][[:digit:]]*                 {printf("T_DECIMAL,%s\n",yytext);return saltyfish::Parser::make_T_DECIMAL(atoi(yytext),index.get_location());}
0[01234567]*                            {printf("T_OCTAL,%s\n",yytext);return saltyfish::Parser::make_T_OCTAL(atoi(yytext),index.get_location());}
0[xX][[:xdigit:]]+                      {printf("T_HEXADECIMAL,%s\n",yytext);return saltyfish::Parser::make_T_HEXADECIMAL(atoi(yytext),index.get_location());}

"="                                     {printf("T_EQUAL,%s\n",yytext);return saltyfish::Parser::make_T_EQUAL(index.get_location());}
"+"                                     {printf("T_ADD,%s\n",yytext);return saltyfish::Parser::make_T_ADD(index.get_location());}
"-"                                     {printf("T_SUB,%s\n",yytext);return saltyfish::Parser::make_T_SUB(index.get_location());}
"*"                                     {printf("T_MUL,%s\n",yytext);return saltyfish::Parser::make_T_MUL(index.get_location());}
"/"                                     {printf("T_DIV,%s\n",yytext);return saltyfish::Parser::make_T_DIV(index.get_location());}
"%"                                     {printf("T_MOD,%s\n",yytext);return saltyfish::Parser::make_T_MOD(index.get_location());}

"<"                                     {printf("T_B,%s\n",yytext);return saltyfish::Parser::make_T_B(index.get_location());}
">"                                     {printf("T_A,%s\n",yytext);return saltyfish::Parser::make_T_A(index.get_location());}
"<="                                    {printf("T_BE,%s\n",yytext);return saltyfish::Parser::make_T_BE(index.get_location());}
">="                                    {printf("T_AE,%s\n",yytext);return saltyfish::Parser::make_T_AE(index.get_location());}
"!"                                     {printf("T_NOT,%s\n",yytext);return saltyfish::Parser::make_T_NOT(index.get_location());}
"=="                                    {printf("T_EE,%s\n",yytext);return saltyfish::Parser::make_T_EE(index.get_location());}
"!="                                    {printf("T_NE,%s\n",yytext);return saltyfish::Parser::make_T_NE(index.get_location());}
"&&"                                    {printf("T_AND,%s\n",yytext);return saltyfish::Parser::make_T_AND(index.get_location());}
"||"                                    {printf("T_OR,%s\n",yytext);return saltyfish::Parser::make_T_OR(index.get_location());}

";"                                     {printf("T_SEMICOLON,%s\n",yytext);return saltyfish::Parser::make_T_SEMICOLON(index.get_location());}
","                                     {printf("T_COMMA,%s\n",yytext);return saltyfish::Parser::make_T_COMMA(index.get_location());}
"("                                     {printf("T_LS,%s\n",yytext);return saltyfish::Parser::make_T_LS(index.get_location());}
")"                                     {printf("T_RS,%s\n",yytext);return saltyfish::Parser::make_T_RS(index.get_location());}
"["                                     {printf("T_LM,%s\n",yytext);return saltyfish::Parser::make_T_LM(index.get_location());}
"]"                                     {printf("T_RM,%s\n",yytext);return saltyfish::Parser::make_T_RM(index.get_location());}
"{"                                     {printf("T_LB,%s\n",yytext);return saltyfish::Parser::make_T_LB(index.get_location());}
"}"                                     {printf("T_RB,%s\n",yytext);return saltyfish::Parser::make_T_RB(index.get_location());}

[[:blank:]]+                            {}
\n                                      {index.get_location().lines();}
.                                       {return saltyfish::Parser::make_END(index.get_location());}

<INITIAL>{
    "//"      {printf("this is single line comment\n");BEGIN(LINE_COMMENT);}
}
<LINE_COMMENT>{
    "\n"      BEGIN(0);
    [^\n]+    ;
}

<INITIAL>{
    "/*"       {printf("this is multiple line comment\n");BEGIN(BLOCK_COMMENT);}
}
<BLOCK_COMMENT>{
    "*/"      BEGIN(0);
    [^*\n]+   ;
    "*"[^/]   ;
    \n        ;
}

%%

