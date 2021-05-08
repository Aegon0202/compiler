all:
	flex -o lexer.cpp lexer.l
	bison -d parser.y
	g++ -g lexer.cpp parser.cpp -o compiler

clean:
	rm -rf compiler lexer.cpp parser.cpp