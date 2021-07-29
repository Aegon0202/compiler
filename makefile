CC = clang
CFLAGS = -g -Wall -std=c11 -lm 

FLEX_DIR = src/parser
FLEX_SRC = $(FLEX_DIR)/SysY.l
FLEX_GEN = $(FLEX_DIR)/lex.yy.c

BISON_DIR = src/parser
BISON_SRC = $(BISON_DIR)/SysY.y
BISON_GEN_H = $(BISON_DIR)/SysY.tab.h
BISON_GEN_C = $(BISON_DIR)/SysY.tab.c

SYSY_DEF_DIR = src/SysY.type
SYSY_DEF_SRC = 	$(SYSY_DEF_DIR)/*.c

SYSY_AST_DIR = src/SysY.AST
SYSY_AST_SRC = $(SYSY_AST_DIR)/SysY.AST.new.c

SYSY_SYMTAB_DIR = src/SysY.symtab
SYSY_SYMTAB_SRC = $(SYSY_SYMTAB_DIR)/*.c

SYSY_TARGET_DIR = src/SysY.target
SYSY_TARGET_SRC = $(SYSY_TARGET_DIR)/SysY.target.offset.c \
					$(SYSY_TARGET_DIR)/SysY.target.generator.c

OPTIMIZER_DIR = src/optimizer
OPTIMIZER_SRC = $(OPTIMIZER_DIR)/localExprEliminate.c \
				$(OPTIMIZER_DIR)/loopInvariantExtraction.c

UTILS_DIR = src/utils
UTILS_SRC = $(UTILS_DIR)/*.c

IR = src/ssa/*.c

MAIN = src/main.c src/init.c

compiler: $(BISON_GEN_H) $(BISON_GEN_C) $(FLEX_GEN) $(SYSY_DEF_SRC) $(UTILS_SRC) $(SYSY_SYMTAB_SRC) $(IR) $(SYSY_TARGET_SRC) $(OPTIMIZER_SRC) $(MAIN)
	$(CC) $(CFLAGS) $^ 
	mv ./a.out $@
	objdump -d -S $@ > tmp_parser.asm

test: compiler
	./compiler -S -o  tmp.s tmp.c
	scp tmp.s pi@192.168.4.1:/home/pi/Documents/git_repo/tmp/

$(BISON_GEN_H) $(BISON_GEN_C): $(BISON_SRC)
	bison -d $< -o $(BISON_GEN_H) -o $(BISON_GEN_C)

$(FLEX_GEN): $(FLEX_SRC)
	flex -o $@ $<

clean:
	rm -f $(BISON_GEN_H) $(BISON_GEN_C) $(FLEX_GEN) compiler

debug: compiler
	gdb --args ./compiler -S -o tmp.s tmp.c
