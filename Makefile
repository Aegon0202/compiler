all:
	flex -o lexer.cpp lexer.l
	bison -o grammar.cpp grammar.y
	g++ -g lexer.cpp grammar.cpp -o compiler

clean:
	rm -rf compiler lexer.cpp parser.cpp