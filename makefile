CC = gcc
CFLAGS = -g -Lfl -Wall -O2

FLEX_DIR = flex.bison
FLEX_SRC = $(FLEX_DIR)/SysY.l
FLEX_GEN = $(FLEX_DIR)/lex.yy.c

BISON_DIR = flex.bison
BISON_SRC = $(BISON_DIR)/SysY.y
BISON_GEN_H = $(BISON_DIR)/SysY.tab.h 
BISON_GEN_C = $(BISON_DIR)/SysY.tab.c

SYSY_DEF_DIR = SysY.type
SYSY_DEF_SRC = $(SYSY_DEF_DIR)/SysY.type.def.h \
				$(SYSY_DEF_DIR)/SysY.type.new.h \
				$(SYSY_DEF_DIR)/SysY.type.visitor.h \
				$(SYSY_DEF_DIR)/SysY.type.free.h \
				$(SYSY_DEF_DIR)/SysY.type.print.h \
				$(SYSY_DEF_DIR)/SysY.type.new.c \
				$(SYSY_DEF_DIR)/SysY.type.free.c \
				$(SYSY_DEF_DIR)/SysY.type.print.c


parser: $(BISON_GEN_H) $(BISON_GEN_C) $(FLEX_GEN) $(SYSY_DEF_SRC) main.c
	$(CC) $(CFLAGS) $^ -o $@

$(BISON_GEN_H) $(BISON_GEN_C): $(BISON_SRC)
	bison -d $< -o $(BISON_DIR)/SysY.tab.h -o $(BISON_GEN_H) -o $(BISON_GEN_C)

$(FLEX_GEN): $(FLEX_SRC)
	flex -o $@ $<

clean:
	rm -f $(BISON_GEN_H) $(BISON_GEN_C) $(FLEX_GEN) parser
