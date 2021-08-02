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

void b(BASIC_BLOCK_TYPE* block, void* args) {
    printf("block address %p\n", block);
    printf("predecessors: ");
    list_entry_t* head = &block->predecessors->block_link;
    list_entry_t* elem = list_next(head);
    while (head != elem) {
        BasicBlock* bb = le2BasicBlock(elem)->value;
        elem = list_next(elem);
        printf("%p ", bb);
        IR_TYPE* last_ir = le2struct(bb->ir_list->ir_link.prev, IR_TYPE, ir_link);
        if (last_ir->type == BRANCH && last_ir->op2->operand.v.b == block) {
        } else if (last_ir->type == BRANCH && last_ir->op3->operand.v.b == block) {
        } else if (last_ir->type == JUMP && last_ir->op3->operand.v.b == block) {
        } else {
            PrintErrExit("error");
        }
    }

    printf("\nsuccessors: ");
    head = &block->successors->block_link;
    elem = list_next(head);
    IR_TYPE* last_ir = le2struct(block->ir_list->ir_link.prev, IR_TYPE, ir_link);
    if (head->next == head) {
        if (last_ir->type != RETURNSTMT) {
            PrintErrExit("error");
        }
    }
    while (head != elem) {
        BasicBlock* bb = le2BasicBlock(elem)->value;
        elem = list_next(elem);
        printf("%p ", bb);
        if (head->next->next == head && last_ir->type == JUMP) {
        } else if (head->next->next->next == head && last_ir->type == BRANCH) {
        } else {
            PrintErrExit("error");
        }
        if (last_ir->type == BRANCH && last_ir->op2->operand.v.b == bb) {
        } else if (last_ir->type == BRANCH && last_ir->op3->operand.v.b == bb) {
        } else if (last_ir->type == JUMP && last_ir->op3->operand.v.b == bb) {
        } else {
            PrintErrExit("error");
        }
    }
    printf("\n\n");
}

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
    avoidOperandDoubleFree();

    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks != NULL) {
            __dominance_frontier(elem->blocks);
            update_CFG(elem->blocks);
        }
    }
    // for (int i = 0; i < func_table->next_func_index; i++) {
    //     struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
    //     if (elem->blocks) {
    //         printf("func name:%s\n", elem->name);
    //         deepTraverseSuccessorsBasicBlock(elem->blocks, __print_basic_block, NULL);
    //     }
    // }
    calcAllLoopBlocks();
    printf("\n\n\n");
    convertAlltoSSAform();

    // for (int i = 0; i < func_table->next_func_index; i++) {
    //     struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
    //     if (elem->blocks) {
    //         printf("func name:%s\n", elem->name);
    //         deepTraverseSuccessorsBasicBlock(elem->blocks, b, NULL);
    //     }
    // }
    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks) {
            printf("func name:%s\n", elem->name);
            deepTraverseSuccessorsBasicBlock(elem->blocks, __print_basic_block, NULL);
        }
    }
    convertAllOutSSAform();
    generateAllToOutFile(output_file);
    return 0;
    //printf("\n\n\n\n");

    deepTraverseSuccessorsBasicBlock(getFuncTabElemByName("main", func_table)->blocks, __print_basic_block, NULL);
    deepTraverseSuccessorsBasicBlock(getFuncTabElemByName("avgPooling", func_table)->blocks, __print_basic_block, NULL);
    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks) {
            //printf("func name:%s\n", elem->name);
            alSimplifyAndConstProp(elem->blocks);
            deadCodeEliminate(elem);
            alSimplifyAndConstProp(elem->blocks);
            localExprEliminate(elem);
            loopInvariantExtraction(elem);
            deadCodeEliminate(elem);
            alSimplifyAndConstProp(elem->blocks);
            //printf("\n\n\n");
            //deepTraverseSuccessorsBasicBlock(getFuncTabElemByName("main", func_table)->blocks, __print_basic_block, NULL);
        }
    }
    //printf("\n\n\n");
    //deepTraverseSuccessorsBasicBlock(getFuncTabElemByName("uniquePaths", func_table)->blocks, __print_basic_block, NULL);
    convertAllOutSSAform();
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