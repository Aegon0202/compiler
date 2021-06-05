#include"./ssa.h"
#include<stdlib.h>
#include<stdio.h>

extern int current_size = 0;
extern int max_capacity = MAX_CAPACITY;
extern Ir* currentIr;
extern Value reg_list[MAX_CAPACITY];     //寄存器堆，在这个阶段，寄存器的数量为无限大
extern ID id_list[MAX_CAPACITY];         //这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量的
extern Definition tag[MAX_CAPACITY];     //这个序列也是对应与寄存器中的每个值，在优化阶段需要用到这些信息


Ir* create_new_ir(enum IrType op_type, Operand op1,Operand op2,Operand op3){
    Ir* ir = (Ir*)malloc(sizeof Ir);
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    ir->type = op_type;
    return ir;
}

Phi* create_new_phi(BasicBlock* block){
    Phi* p = (Phi*)malloc(sizeof Phi);
    p->is_complete = 0;
    p->parameters.idx = -1;             //头节点;
    p->def_block = block;
    list_init(&(p->parameters.phi_para_link));
    return p;
}

Definition* create_new_definition(){
    Definition* d = (Definition*)malloc(sizeof Definition);
    d->users.address.block = NULL;
    list_init(&(d->users->def_use_link));
    return d;
}

void add_user(ID id, Ir* value){

}

void delete_user(ID id, Ir* value){

}

//创建一个新的block
BasicBlock* create_new_block(){
    BasicBlock* block = (BasicBlock*)malloc(sizeof BasicBlock);
    block->is_sealed = 0;
    block->is_full = 0;
    block->predecessor_num = 0;
    block->successor_num = 0;
    block->predecessors.value=NULL;
    block->successors.value=NULL;
    block->ir_list.type = HEAD;
    list_init(&(block->predecessors.block_link));
    list_init(&(block->successors.block_link));
    list_init(&(block->ir_list.ir_link));
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



int read_variable(ID id, BasicBlock* block){
    int end = block->end_reg_idx, begin = block->begin_reg_idx;
    int i;
    for(i=end;i>=begin;i--)
        //维护def_use_chain，返回读取到的值
        if(id_list[i]== id){
            def_use_node* tmp = (def_use_node*)malloc(sizeof def_use_node);    
            tmp->address.block = block;
            tmp->address.ir = currentIr;
            list_add(&(tag[i].users.def_use_link),&(tmp->def_use_link));
            return i;
        }
    
    return read_variable_recursively(id,block);
}

int read_variable_recursively(ID id, BasicBlock* block){
    if(block->is_sealed==0)                 //该块未sealed
    {
        Phi* val = create_new_phi(block);
        reg_list[++current_size] = val;
        return current_size;
    }
    else if(block->predecessor_num==0)      //该块为初始块
        return -1;                          //表示为初始化的变量
    else if(block->predecessor_num==1){     //该块只有唯一祖先
        list_entry_t* le = list_next(&(block->predecessors->block_link));
        BasicBlock* b = le2struct(le,BasicBlock,block_link);
        return read_variable(id,b);
    }   
    else{                                   //该块有多个祖先且已经sealed,这时需要创建phi，并且回溯确定phi的参数。
        
        return 0;
    }
    
}


void write_variable(ID id, BasicBlock* block, Ir* ir, Value v){
    reg_list[++current_size] = v;
    id_list[current_size] = id;
                    
    Definition* d = create_new_definition();
    d->value_idx = current_size;
    d->def_address.block = block;
    d->def_address.ir = ir;

}

Phi* add_phi_operand(ID id,Phi* phi){
    
}
