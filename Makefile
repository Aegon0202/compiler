AST_SRC := $(shell find AST/AST -name "*.cpp")
ASTVISITOR_SRC := $(shell find AST/ASTVisitor -name "*.cpp")
ASTVISITOR_SRC := $(filter-out AST/ASTVisitor/main.cpp, $(ASTVISITOR_SRC))
MAIN_SRC = driver.cpp scanner.cpp parser.cpp main.cpp

INC = -IAST/AST -IAST/ASTVisitor

all:
	flex -o scanner.cpp scanner.l
	bison -v -o parser.cpp parser.y
	g++ -g $(MAIN_SRC) $(AST_SRC) $(ASTVISITOR_SRC) $(INC) -o a.out

clean:
	rm -rf a.out scanner.cpp parser.cpp parser.hpp location.hh 