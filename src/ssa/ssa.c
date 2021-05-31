#include"./ssa.h"
#include<stdlib.h>
#include<stdio.h>

Ir* create_new_ir(){

}

Phi* create_new_phi(){

}

Operand_def* create_new_definition(){

}

def_use_node* create_new_def_use_chain(){

}

void add_user(Operand_def* operand, Ir* value){

}

void delete_user(Operand_def* operand, Ir* value){

}

//创建一个新的block
BasicBlock* create_new_block(){
    BasicBlock* block = (BasicBlock*)malloc(sizeof BasicBlock);
    block->is_sealed = 0;
    block->is_full = 0;
    block->predecessors.value=NULL;
    block->successors.value=NULL;
    list_init(&(block->predecessors.block_link));
    list_init(&(block->successors.block_link));
    return block;
}


//为两个block建立祖先和后继的关系
void connect_block(BasicBlock* pre, BasicBlock* suc){
    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof BasicBlockNode);
    tmp->value = suc;
    list_add(&(tmp->block_link), &(pre->successors->block_link));

    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof BasicBlockNode);
    tmp->value = pre;
    list_add(&(tmp->block_link), &(suc->predecessors->block_link));

}



BasicBlock* generate_ssa(Program p){

}
