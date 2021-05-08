
/* scanner for a toy Pascal-like language */
%option yylineno noyywrap c++ yyclass="Scanner" prefix="sf"
/*%option case-insensitive*/

%x LINE_COMMENT
%x BLOCK_COMMENT

%{
    #include <math.h>
    #include "scanner.hpp"
    #include "location.hh"
    #include "driver.hpp"

    #define yyterminate() saltyfish::Parser::make_END(driver.getLoc());
    #define YY_USER_ACTION \
        driver.getLoc().step();    \
        driver.getLoc().columns(yyleng);
%}

%%

"int"                                   {return saltyfish::Parser::make_T_INT(yytext,driver.getLoc());}
"const"                                 {return saltyfish::Parser::make_T_CONST(yytext,driver.getLoc());}
"void"                                  {return saltyfish::Parser::make_T_VOID(yytext,driver.getLoc());}
"if"                                    {return saltyfish::Parser::make_T_IF(yytext,driver.getLoc());}
"else"                                  {return saltyfish::Parser::make_T_ELSE(yytext,driver.getLoc());}
"while"                                 {return saltyfish::Parser::make_T_WHILE(yytext,driver.getLoc());}
"break"                                 {return saltyfish::Parser::make_T_BREAK(yytext,driver.getLoc());}
"continue"                              {return saltyfish::Parser::make_T_CONTINUE(yytext,driver.getLoc());}
"return"                                {return saltyfish::Parser::make_T_RETURN(yytext,driver.getLoc());}

[_[:alpha:]]([_[:alpha:]]|[[:digit:]])* {return saltyfish::Parser::make_IdentStr(yytext,driver.getLoc());}

([[:digit:]]{-}[0])[[:digit:]]*         {return saltyfish::Parser::make_DECIMAL(atoi(yytext),driver.getLoc());}
0[01234567]*                            {return saltyfish::Parser::make_OCTAL(atoi(yytext),driver.getLoc());}
0[xX][[:xdigit:]]+                      {return saltyfish::Parser::make_HEXADECIMAL(atoi(yytext),driver.getLoc());}

"="                                     {return saltyfish::Parser::make_T_EQUAL(driver.getLoc());}
"+"                                     {return saltyfish::Parser::make_T_ADD(driver.getLoc());}
"-"                                     {return saltyfish::Parser::make_T_SUB(driver.getLoc());}
"*"                                     {return saltyfish::Parser::make_T_MUL(driver.getLoc());}
"/"                                     {return saltyfish::Parser::make_T_DIV(driver.getLoc());}
"%"                                     {return saltyfish::Parser::make_T_MOD(driver.getLoc());}

"<"                                     {return saltyfish::Parser::make_T_B(driver.getLoc());}
">"                                     {return saltyfish::Parser::make_T_A(driver.getLoc());}
"<="                                    {return saltyfish::Parser::make_T_BE(driver.getLoc());}
">="                                    {return saltyfish::Parser::make_T_AE(driver.getLoc());}
"!"                                     {return saltyfish::Parser::make_T_NOT(driver.getLoc());}
"=="                                    {return saltyfish::Parser::make_T_E(driver.getLoc());}
"!="                                    {return saltyfish::Parser::make_T_NE(driver.getLoc());}
"&&"                                    {return saltyfish::Parser::make_T_AND(driver.getLoc());}
"||"                                    {return saltyfish::Parser::make_T_OR(driver.getLoc());}

";"                                     {return saltyfish::Parser::make_T_SEMICOLON(driver.getLoc());}
","                                     {return saltyfish::Parser::make_T_COMMA(driver.getLoc());}
"("                                     {return saltyfish::Parser::make_T_LS(driver.getLoc());}
")"                                     {return saltyfish::Parser::make_T_RS(driver.getLoc());}
"["                                     {return saltyfish::Parser::make_T_LM(driver.getLoc());}
"]"                                     {return saltyfish::Parser::make_T_RM(driver.getLoc());}
"{"                                     {return saltyfish::Parser::make_T_LB(driver.getLoc());}
"}"                                     {return saltyfish::Parser::make_T_RB(driver.getLoc());}

[[:blank:]]+                            {}
\n                                      {driver.getLoc().lines();}
<<EOF>>                                 {return saltyfish::Parser::make_END(driver.getLoc());}
.                                       

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