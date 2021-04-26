#flex scanner.l
#gcc -g lex.yy.c -o scanner -lfl
#./scanner

bison parser.y
g++ -o parser parser.cpp
./parser