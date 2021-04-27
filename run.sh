flex lexer.l
gcc lex.yy.c -o lexer
rm lex.yy.c
./lexer<test.cpp
