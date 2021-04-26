all:
	flex -o scanner.cpp scanner.l
	bison -o parser.cpp parser.y
	g++ -g scanner.cpp parser.cpp -o a.out

clean:
	rm -rf a.out scanner.cpp parser.cpp