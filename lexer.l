
/* scanner for a toy Pascal-like language */
%option yylineno noyywrap c++ yyclass="Scanner" prefix="sf"
/*%option case-insensitive*/

%x LINE_COMMENT
%x BLOCK_COMMENT

%{
    #include <math.h>
    #include "lexer.hpp"
%}

%%

"int"                                   {return saltyfish::Parser::make_T_INT();}
"const"                                 {return saltyfish::Parser::make_T_CONST();}
"void"                                  {return saltyfish::Parser::make_T_VOID();}
"if"                                    {return saltyfish::Parser::make_T_IF();}
"else"                                  {return saltyfish::Parser::make_T_ELSE();}
"while"                                 {return saltyfish::Parser::make_T_WHILE();}
"break"                                 {return saltyfish::Parser::make_T_BREAK();}
"continue"                              {return saltyfish::Parser::make_T_CONTINUE();}
"return"                                {return saltyfish::Parser::make_T_RETURN();}

[_[:alpha:]]([_[:alpha:]]|[[:digit:]])* {return saltyfish::Parser::make_Ident(yytext);}

([[:digit:]]{-}[0])[[:digit:]]*         {return saltyfish::Parser::make_DECIMAL(atoi(yytext));}
0[01234567]*                            {return saltyfish::Parser::make_OCTAL(atoi(yytext));}
0[xX][[:xdigit:]]+                      {return saltyfish::Parser::make_HEXADECIMAL(atoi(yytext));}

"="                                     {return saltyfish::Parser::make_T_EQUAL();}
"+"                                     {return saltyfish::Parser::make_T_ADD();}
"-"                                     {return saltyfish::Parser::make_T_SUB();}
"*"                                     {return saltyfish::Parser::make_T_MUL();}
"/"                                     {return saltyfish::Parser::make_T_DIV();}
"%"                                     {return saltyfish::Parser::make_T_MOD();}

"<"                                     {return saltyfish::Parser::make_T_B();}
">"                                     {return saltyfish::Parser::make_T_A();}
"<="                                    {return saltyfish::Parser::make_T_BE();}
">="                                    {return saltyfish::Parser::make_T_AE();}
"!"                                     {return saltyfish::Parser::make_T_NOT();}
"=="                                    {return saltyfish::Parser::make_T_E();}
"!="                                    {return saltyfish::Parser::make_T_NE();}
"&&"                                    {return saltyfish::Parser::make_T_AND();}
"||"                                    {return saltyfish::Parser::make_T_OR();}

";"                                     {return saltyfish::Parser::make_T_SEMICOLON();}
","                                     {return saltyfish::Parser::make_T_COMMA();}
"("                                     {return saltyfish::Parser::make_T_LS();}
")"                                     {return saltyfish::Parser::make_T_RS();}
"["                                     {return saltyfish::Parser::make_T_LM();}
"]"                                     {return saltyfish::Parser::make_T_RM();}
"{"                                     {return saltyfish::Parser::make_T_LB();}
"}"                                     {return saltyfish::Parser::make_T_RB();}

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