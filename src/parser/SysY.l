%option nodefault noyywrap yylineno
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../SysY.type/SysY.type.def.h"
    #include "../SysY.type/SysY.type.new.h"
    #include "./SysY.tab.h"
    #define PRINT(type,text) 
    int fileno(FILE* stream);
    //fprintf(yyout, "( " #type " , %s )\n", text)
%}

%x COMMENT

%%

0|[1-9][0-9]*  |
0[1-7][0-7]+ |
(0x|0X)[0-9a-fA-F]  {
    PRINT(INTCONST,yytext);
    yylval.intconst = newIntConst(INTCONST,yytext);
    return INTCONST;
}

"+" {
    PRINT(K_ADD,yytext);
    return K_ADD;
}

"-" {
    PRINT(K_SUB,yytext);
    return K_SUB;
}

"!" {
    PRINT(K_NOT,yytext);
    return K_NOT;
}

"*" {
    PRINT(K_MUL,yytext);
    yylval.mulop=newMulOp(MULOP,K_MUL);
    return MULOP;
}

"/" {
    PRINT(K_DIV,yytext);
    yylval.mulop=newMulOp(MULOP,K_DIV);
    return MULOP;
}

"%" {
    PRINT(K_MOD,yytext);
    yylval.mulop=newMulOp(MULOP,K_MOD);
    return MULOP;
}

"=="    {
    PRINT(EQOP,yytext);
    yylval.eqop = newEqOp(EQOP,K_EQ);
    return EQOP;
}

"!="   {
    PRINT(EQOP,yytext);
    yylval.eqop = newEqOp(EQOP,K_NEQ);
    return EQOP;
}

"=" {
    PRINT(ASSIGNOP,yytext);
    return K_ASSIGNOP;
}

">"   {
    PRINT(RELOP,yytext);
    yylval.relop = newRelOp(RELOP,K_GT);
    return RELOP;
}

"<"   {
    PRINT(RELOP,yytext);
    yylval.relop = newRelOp(RELOP,K_LT);
    return RELOP;
}

">="   {
    PRINT(RELOP,yytext);
    yylval.relop = newRelOp(RELOP,K_GTE);
    return RELOP;
}

"<="   {
    PRINT(RELOP,yytext);
    yylval.relop = newRelOp(RELOP,K_LTE);
    return RELOP;
}

"&&" {
    PRINT(LANDOP,yytext);
    return K_AND;
}

"||" {
    PRINT(LOROP,yytext);
    return K_OR;
}

\"[^"]*\" {
    if(yytext[yyleng-2] == '\\'){
        yyless(yyleng-1);
        yymore();
    }else{
        PRINT(STRING,yytext);
        yylval.string = newString(STRING,yytext);
        return STRING;
    }
}

"{" {
    PRINT(K_CURLY_L,yytext);
    return K_CURLY_L;
}

"}" {
    PRINT(K_CURLY_R,yytext);
    return K_CURLY_R;
}

"[" {
    PRINT(K_SQUARE_L,yytext);
    return K_SQUARE_L;
}

"]" {
    PRINT(K_SQUARE_R,yytext);
    return K_SQUARE_R;
}

"(" {
    PRINT(K_PARENTHESES_L,yytext);
    return K_PARENTHESES_L;
}

")" {
    PRINT(K_PARENTHESES_R,yytext);
    return K_PARENTHESES_R;
}

"," {
    PRINT(K_COMMA,yytext);
    return K_COMMA;
}

";" {
    PRINT(K_SEMICOLON,yytext);
    return K_SEMICOLON;
}

"const" {
    PRINT(K_CONST,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_CONST,yytext);
    return K_CONST;
}

"int" {
    PRINT(K_INT,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_INT,yytext);
    return K_INT;
}

"void" {
    PRINT(K_VOID,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_VOID,yytext);
    return K_VOID;
}

"return" {
    PRINT(K_RETURN,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_RETURN,yytext);
    return K_RETURN;
}

"if" {
    PRINT(K_IF,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_IF,yytext);
    return K_IF;
}

"else" {
    PRINT(K_ELSE,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_ELSE,yytext);
    return K_ELSE;
}

"while" {
    PRINT(K_RETURN,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_WHILE,yytext);
    return K_WHILE;
}

"break" {
    PRINT(K_BREAK,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_BREAK,yytext);
    return K_BREAK;
}

"continue" {
    PRINT(K_CONTINUE,yytext);
    // yylval.keyword = newKeyword(KEYWORD,K_CONTINUE,yytext);
    return K_CONTINUE;
}

[_a-zA-Z][_a-zA-Z0-9]* {
    PRINT(IDENT,yytext);
    yylval.ident = newIdent(IDENT,yytext);
    return IDENT;
}

\/\/.*$ {}

\/\* {BEGIN COMMENT;}

<COMMENT>. |
<COMMENT>[\n\t\r ] {}
<COMMENT>\*\/ {BEGIN INITIAL;}

[\n\r\t ]* {}

.   {fprintf(yyout,"ERROR HAPPEN %d %s\n",yylineno,yytext);exit(-1);}

%%
