#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RegAllocator/LRA.h"
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
struct DequeList* allBlock;
int yyparse(void);
void init();
void __debug_pause_there();
void convertAlltoSSAform();
void convertAllOutSSAform();
void convertSSAToArmFunc(struct FuncTabElem* func);
void __get_all_blocks(BASIC_BLOCK_TYPE* basic_block, void* args);
void LinearScanRegAllocation(struct FuncTabElem* elem, FILE* out_file);
void generateGlobalToOutFile(FILE* out_file);
void __init_bit_map_global();

void __print_pre_and_suc(BASIC_BLOCK_TYPE* block, void* args) {
    list_entry_t* head;
    list_entry_t* elem;
    printf("block address: %p:\n", block);
    head = &block->predecessors->block_link;
    elem = list_next(head);
    printf("predecessors:");
    while (head != elem) {
        printf(" %p", le2BasicBlock(elem)->value);
        elem = list_next(elem);
    }
    printf("\n");

    head = &block->successors->block_link;
    elem = list_next(head);
    printf("successors:");
    while (head != elem) {
        printf(" %p", le2BasicBlock(elem)->value);
        elem = list_next(elem);
    }
    printf("\n");
    printf("\n");
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

    // deepTraverseSuccessorsBasicBlock(getFuncTabElemByName("main", func_table)->blocks, __print_basic_block, NULL);
    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks != NULL) {
            __dominance_frontier(elem);
        }
    }
    calcAllLoopBlocks();
    convertAlltoSSAform();

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
        }
    }
    //printf("\n\n\n");
    convertAllOutSSAform();

    // deepTraverseSuccessorsBasicBlock(getFuncTabElemByName("main", func_table)->blocks, __print_basic_block, NULL);
    allBlock = newDequeList();
    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks) {
            convertSSAToArmFunc(elem);
            deepTraverseSuccessorsBasicBlock(elem->blocks, __get_all_blocks, allBlock);
        }
    }

    __init_bit_map_global();

    for (int i = 0; i < func_table->next_func_index; i++) {
        struct FuncTabElem* elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks) {
            LinearScanRegAllocation(elem, output_file);
        }
    }
    generateGlobalToOutFile(output_file);

    return 0;
}

void __get_all_blocks(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct DequeList* deque = (struct DequeList*)args;
    pushFrontDequeList(deque, basic_block->block_LRA);
}

void __debug_pause_there() {
    printf("debug pause there");
}