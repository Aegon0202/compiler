CC = clang
CFLAGS = -g -Wall -O2 -std=c++17 -lm

FLEX_DIR = src/parser
FLEX_SRC = $(FLEX_DIR)/SysY.l
FLEX_GEN = $(FLEX_DIR)/lex.yy.cpp

BISON_DIR = src/parser
BISON_SRC = $(BISON_DIR)/SysY.y
BISON_GEN_H = $(BISON_DIR)/SysY.tab.hpp 
BISON_GEN_C = $(BISON_DIR)/SysY.tab.cpp

SYSY_DEF_DIR = src/SysY.type
SYSY_DEF_SRC = 	$(SYSY_DEF_DIR)/*.cpp


parser: $(BISON_GEN_H) $(BISON_GEN_C) $(FLEX_GEN) $(SYSY_DEF_SRC) src/main.cpp
	$(CC) $(CFLAGS) $^ 

$(BISON_GEN_H) $(BISON_GEN_C): $(BISON_SRC)
	bison -d $< -o $(BISON_DIR)/SysY.tab.h -o $(BISON_GEN_H) -o $(BISON_GEN_C)

$(FLEX_GEN): $(FLEX_SRC)
	flex -o $@ $<

clean:
	rm -f $(BISON_GEN_H) $(BISON_GEN_C) $(FLEX_GEN) parser
