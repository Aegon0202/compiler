
/* scanner for a toy Pascal-like language */
%option yylineno noyywrap c++ yyclass="Scanner" prefix="sf"
/*%option case-insensitive*/

%{
#include "sys.h"
#include <stdio.h>
%}

%%

"int"                                   {printf("T_INT,%s",yytext);return Parser.T_INT;}
"const"                                 {printf("T_CONST,%s",yytext);return Parser.T_CONST;}
"void"                                  {printf("T_VOID,%s",yytext);return Parser.T_VOID;}
"if"                                    {printf("T_IF,%s",yytext);return Parser.T_IF}
"else"                                  {printf("T_ELSE,%s",yytext);return Parser.T_ELSE;}
"while"                                 {printf("T_WHILE,%s",yytext);return Parser.T_WHILE;}
"break"                                 {printf("T_BREAK,%s",yytext);return Parser.T_BREAK;}
"continue"                              {printf("T_CONTINUE,%s",yytext);return Parser.T_CONTINUE;}
"return"                                {printf("T_RETURN,%s",yytext);return Parser.T_RETURN;}

[_[:alpha:]]([_[:alpha:]]|[[:digit:]])* {printf("T_IDENT,%s",yytext);return Parser.T_IDENT;}

(123456789)[[:digit:]]*                 {printf("T_DECIMAL,%s",yytext);return Parser.T_DECIMAL;}
0[01234567]*                            {printf("T_OCTAL,%s",yytext);return Parser.T_OCTAL;}
0[xX][[:xdigit:]]+                      {printf("T_HEXADECIMAL,%s",yytext);return Parser.T_HEXADECIMAL;}

"="                                     {printf("T_EQUAL,%s",yytext);return Parser.T_EQUAL;}
"+"                                     {printf("T_ADD,%s",yytext);return Parser.T_ADD;}
"-"                                     {printf("T_SUB,%s",yytext);return Parser.T_SUB;}
"*"                                     {printf("T_MUL,%s",yytext);return Parser.T_MUL;}
"/"                                     {printf("T_DIV,%s",yytext);return Parser.T_DIV;}
"%"                                     {printf("T_MOD,%s",yytext);return Parser.T_MOD;}

"<"                                     {printf("T_B,%s",yytext);return Parser.T_B;}
">"                                     {printf("T_A,%s",yytext);return Parser.T_A;}
"<="                                    {printf("T_BE,%s",yytext);return Parser.T_BE;}
">="                                    {printf("T_AE,%s",yytext);return Parser.T_AE;}
"!"                                     {printf("T_NOT,%s",yytext);return Parser.T_NOT;}
"=="                                    {printf("T_E,%s",yytext);return Parser.T_E;}
"!="                                    {printf("T_NE,%s",yytext);return Parser.T_NE;}
"&&"                                    {printf("T_AND,%s",yytext);return Parser.T_AND;}
"||"                                    {printf("T_OR,%s",yytext);return Parser.T_OR;}

";"                                     {printf("T_SEMICOLON,%s",yytext);return Parser.T_SEMICOLON;}
","                                     {printf("T_COMMA,%s",yytext);return Parser.T_COMMA;}
"("                                     {printf("T_LS,%s",yytext);return Parser.T_LS;}
")"                                     {printf("T_RS,%s",yytext);return Parser.T_RS;}
"["                                     {printf("T_LM,%s",yytext);return Parser.T_LM;}
"]"                                     {printf("T_RM,%s",yytext);return Parser.T_RM;}
"{"                                     {printf("T_LB,%s",yytext);return Parser.T_LB;}
"}"                                     {printf("T_RB,%s",yytext);return Parser.T_RB;}

[[:blank:]]+                            {}
\n                                      {}
.                                       printf( "Unrecognized character: %s\n",yytext );

<INITIAL>{
    "//"      BEGIN(LINE_COMMENT);
}
<LINE_COMMENT>{
    "\n"      BEGIN(0);
    [^\n]+    ;
}

<INITIAL>{
    "/*"      BEGIN(BLOCK_COMMENT);
}
<BLOCK_COMMENT>{
    "*/"      BEGIN(0);
    [^*\n]+   ;
    "*"[^/]   ;
    \n        ;
}

%%
