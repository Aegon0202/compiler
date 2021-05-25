#include"./ssa.h"
#include<stdlib.h>
#include<stdio.h>

def_use_node* init_user_chain(){
    def_use_node* head = (def_use_node*)malloc(sizeof def_use_value);
    if(!head){
        printf("memory allocate failed at init!");
        exit(0);
    }
    head->next = NULL;
    head->value = NULL;
    return head;
}

void addUser(Operand_def* operand, Ir* value){
    if(check_if_exist(operand, Ir)) return ;
    //将新的user加在开头
    def_use_node* tmp = (def_use_node*)malloc(sizeof def_use_node);
    if(!tmp){
        printf("memory allocate failed at addUser!\n");
        exit(0);
    }
    tmp->value = value;
    tmp->next = operand->users->next;
    operand->users->next =  tmp;
}

void deleteUser(Operand_def* operand, Ir* value){
    if(!check_if_exist(operand, Ir)) return ;
    def_use_node* tmp,delete;
    tmp = operand->users;
    while(tmp->next){
        def_use_node* next_node = tmp->next;
        if(next->value==value){
            tmp->next = next->next;
            next->next = NULL;
            free(next);
            return ;
        }
        tmp = tmp->next;
    }
}


BasicBlock* create_new_block(){
    BasicBlock* block = (BasicBlock*)malloc(sizeof BasicBlock);
}

BasicBlockNode* init_block_list(){
    BasicBlockNode* head = (BasicBlockNode*)malloc(sizeof BasicBlockNode);
    if(!head){
        printf("memory allocate failed at init!");
        exit(0);
    }
    head->next = NULL;
    head->value = NULL;
    return head;
}

void add_predecessor(BasicBlock* block, BasicBlock* value){
    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof BasicBlockNode);
    tmp->value = value;

    tmp->next = block->predecessors->next;
    block->predecessors->next = tmp;
}

void add_successor(BasicBlock* block, BasicBlock* value){
    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof BasicBlockNode);
    tmp->value = value;

    tmp->next = block->successors->next;
    block->successors->next = tmp;
}

BasicBlock* generate_ssa(Program p){

}
