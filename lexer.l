
/* scanner for a toy Pascal-like language */
%option yylineno noyywrap 
/*%option case-insensitive*/

%x LINE_COMMENT
%x BLOCK_COMMENT

%{
#include "sysy.h"
#include <stdio.h>
%}

%%

"int"                                   {printf("T_INT,%s\n",yytext);return T_INT;}
"const"                                 {printf("T_CONST,%s\n",yytext);return T_CONST;}
"void"                                  {printf("T_VOID,%s\n",yytext);return T_VOID;}
"if"                                    {printf("T_IF,%s\n",yytext);return T_IF;}
"else"                                  {printf("T_ELSE,%s\n",yytext);return T_ELSE;}
"while"                                 {printf("T_WHILE,%s\n",yytext);return T_WHILE;}
"break"                                 {printf("T_BREAK,%s\n",yytext);return T_BREAK;}
"continue"                              {printf("T_CONTINUE,%s\n",yytext);return T_CONTINUE;}
"return"                                {printf("T_RETURN,%s\n",yytext);return T_RETURN;}

[_[:alpha:]]([_[:alpha:]]|[[:digit:]])* {printf("T_IDENT,%s\n",yytext);return T_IDENT;}

[123456789][[:digit:]]*                 {printf("T_DECIMAL,%s\n",yytext);return T_DECIMAL;}
0[01234567]*                            {printf("T_OCTAL,%s\n",yytext);return T_OCTAL;}
0[xX][[:xdigit:]]+                      {printf("T_HEXADECIMAL,%s\n",yytext);return T_HEXADECIMAL;}

"="                                     {printf("T_EQUAL,%s\n",yytext);return T_EQUAL;}
"+"                                     {printf("T_ADD,%s\n",yytext);return T_ADD;}
"-"                                     {printf("T_SUB,%s\n",yytext);return T_SUB;}
"*"                                     {printf("T_MUL,%s\n",yytext);return T_MUL;}
"/"                                     {printf("T_DIV,%s\n",yytext);return T_DIV;}
"%"                                     {printf("T_MOD,%s\n",yytext);return T_MOD;}

"<"                                     {printf("T_B,%s\n",yytext);return T_B;}
">"                                     {printf("T_A,%s\n",yytext);return T_A;}
"<="                                    {printf("T_BE,%s\n",yytext);return T_BE;}
">="                                    {printf("T_AE,%s\n",yytext);return T_AE;}
"!"                                     {printf("T_NOT,%s\n",yytext);return T_NOT;}
"=="                                    {printf("T_E,%s\n",yytext);return T_EE;}
"!="                                    {printf("T_NE,%s\n",yytext);return T_NE;}
"&&"                                    {printf("T_AND,%s\n",yytext);return T_AND;}
"||"                                    {printf("T_OR,%s\n",yytext);return T_OR;}

";"                                     {printf("T_SEMICOLON,%s\n",yytext);return T_SEMICOLON;}
","                                     {printf("T_COMMA,%s\n",yytext);return T_COMMA;}
"("                                     {printf("T_LS,%s\n",yytext);return T_LS;}
")"                                     {printf("T_RS,%s\n",yytext);return T_RS;}
"["                                     {printf("T_LM,%s\n",yytext);return T_LM;}
"]"                                     {printf("T_RM,%s\n",yytext);return T_RM;}
"{"                                     {printf("T_LB,%s\n",yytext);return T_LB;}
"}"                                     {printf("T_RB,%s\n",yytext);return T_RB;}
<<EOF>>                                 {return -1;}

[[:blank:]]+                            {return 1;}
\n                                      {return 1;}
.                                       printf( "Unrecognized character: %s\n\n",yytext );

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

int main(){
    while(yylex()!=-1);
    return 1;
}