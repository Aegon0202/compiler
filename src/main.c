#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SysY.symtab/SysY.symtab.ssa.h"
#include "SysY.target/SysY.target.arm.h"
#include "SysY.type/SysY.type.def.h"
#include "SysY.type/SysY.type.print.h"
#include "optimizer/local_op.h"
#include "parser/SysY.tab.h"
#include "ssa/ssa.h"
#include "ssa/traverse.h"

YYSTYPE result;
int S_flag;
int O_flag;
int yyparse(void);
void init();
void __debug_pause_there();
int df_plus_test(BasicBlock* start);
void __placement_phi(BasicBlock* start);
void convertAlltoSSAform();
void convertAllOutSSAform();

int main(int argc, char** argv) {
    init();
    int index;
    const char* input_file_name = argv[4];
    const char* output_file_name = argv[3];
    FILE* output_file;
    S_flag = 1;
    O_flag = 1;
    freopen(input_file_name, "r", stdin);
    output_file = fopen(output_file_name, "w");
    yyparse();
    toSSACompUnit(result.compunit);

    deepTraverseSuccessorsBasicBlock(getFuncTabElemByName("main", func_table)->blocks, __print_basic_block, 1);
    printf("\n\n\n");
    convertAlltoSSAform();
    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks) {
            loopInvariantExtraction(elem);
            //localExprEliminate(elem);
        }
    }
    convertAllOutSSAform();
    //deepTraverseSuccessorsBasicBlock(getFuncTabElemByName("main", func_table)->blocks, __print_basic_block, 1);
    generateAllToOutFile(output_file);
    //printToken(result.compunit, &printToken_ins);
    //toASTCompUnit(result.compunit);
    //printASTAll(funcsymtable_p->head);
    //printToken(result.compunit, printToken_ins);
    return 0;
}

void __debug_pause_there() {
    printf("debug pause there");
}