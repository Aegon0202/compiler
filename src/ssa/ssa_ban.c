#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include"./ssa.h"
#include<stdlib.h>
#include<stdio.h>

int current_phi_num = 0;
int current_size = 0;
int max_capacity = MAX_CAPACITY;
Ir* currentIr;
Value* reg_list[MAX_CAPACITY];     //寄存器堆，在这个阶段，寄存器的数量为无限大
ID id_list[MAX_CAPACITY];         //这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量的
Definition* tag[MAX_CAPACITY];     //这个序列也是对应与寄存器中的每个值，在优化阶段需要用到这些信息
Phi* phi_memory[MAX_CAPACITY];



Ir* create_new_ir(enum IrType op_type, Operand op1,Operand op2,Operand op3){
    Ir* ir = (Ir*)malloc(sizeof(Ir));
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    ir->type = op_type;
    return ir;
}

Phi* create_new_phi(BasicBlock* block){
<<<<<<< HEAD
    Phi* p = (Phi*)malloc(sizeof Phi);
    
=======
    Phi* p = (Phi*)malloc(sizeof(Phi));
>>>>>>> 58da50caa6ee794fb20a45a2fc3c3cc893acb1b4
    p->is_complete = 0;
    p->parameters.idx = -1;             //头节点;
    p->def_block = block;
    block->phi_list = current_phi_num;
    block->phi_num += 1;
    list_init(&(p->parameters.phi_para_link));
    phi_memory[current_phi_num++] = p;
    return p;
}

Definition* create_new_definition(){
    Definition* d = (Definition*)malloc(sizeof(Definition));
    d->users.address.block = NULL;
    list_init(&(d->users->def_use_link));
    return d;
}

void add_user(int i, BasicBlock* block, Ir* ir){
    def_use_node* tmp = (def_use_node*)malloc(sizeof def_use_node);    
    tmp->address.block = block;
    tmp->address.ir = currentIr;
    list_add(&(tag[i].users.def_use_link),&(tmp->def_use_link));
}

void delete_user(int i, Ir* ir){

}

//创建一个新的block
BasicBlock* create_new_block(){
    BasicBlock* block = (BasicBlock*)malloc(sizeof(BasicBlock));
    block->is_sealed = 0;
    block->is_full = 0;
    block->predecessor_num = 0;
    block->successor_num = 0;
    block->predecessors.value=NULL;
    block->successors.value=NULL;
    block->ir_list.type = HEAD;
    block->phi_num = 0;
    list_init(&(block->predecessors.block_link));
    list_init(&(block->successors.block_link));
    list_init(&(block->ir_list.ir_link));
    return block;
}


//为两个block建立祖先和后继的关系
void connect_block(BasicBlock* pre, BasicBlock* suc){
    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof(BasicBlockNode));
    tmp->value = suc;
    list_add(&(tmp->block_link), &(pre->successors->block_link));

    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof(BasicBlockNode));
    tmp->value = pre;
    list_add(&(tmp->block_link), &(suc->predecessors->block_link));

}



int read_variable(ID id, BasicBlock* block){
    int end = block->end_reg_idx, begin = block->begin_reg_idx;
    int i;
    for(i=end;i>=begin;i--)
        //维护def_use_chain，返回读取到的值
        if(id_list[i]== id){
            def_use_node* tmp = (def_use_node*)malloc(sizeof(def_use_node));    
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
        //insert an ir at the begin of the block, like assign phi, id
        write_variable(id,block,currentIr,val);     //表示插入了phi函数
        return -2;
    }
    else if(block->predecessor_num==0)      //该块为初始块
        return -1;                          //表示为初始化的变量
    else if(block->predecessor_num==1){     //该块只有唯一祖先
        list_entry_t* le = list_next(&(block->predecessors.block_link));
        BasicBlock* b = le2struct(le,BasicBlock,block_link);
        return read_variable(id,b);
    }   
    else{                                   //该块有多个祖先且已经sealed,这时需要创建phi，并且回溯确定phi的参数。
        Phi* phi = create_new_phi(block);   //同样，在这种情况也需要在基本块开始加上一条ir表示将phi赋值
        write_variable(id,block,currentIr,phi);
        add_phi_operand(id,phi);
        return -2;                                          
    }
    
}


void write_variable(ID id, BasicBlock* block, Ir* ir, Value* v){
    reg_list[++current_size] = v;
    id_list[current_size] = id;
                    
    Definition* d = create_new_definition();
    d->value_idx = current_size;
    d->def_address.block = block;
    d->def_address.ir = ir;

}

<<<<<<< HEAD
void add_phi_operand(ID id, Phi* phi){
    BasicBlock* tmp=phi->def_block;
    BasicBlockNode pre = tmp->predecessors;
    int pre_num = tmp->predecessor_num;
    int i;
    for(i=0;i<pre_num;i++){
        list_entry_t* le = list_next(&(pre.block_link));
        BasicBlock* pre_block = le2struct(le,BasicBlock,block_link);
        append_phi_operand(phi,read_variable(id,pre_block));
    }
    try_remove_trivial_phi(phi);
=======
Phi* add_phi_operand(ID id,Phi* phi){
    return NULL;
}

///---------------------------------------------------

BASIC_BLOCK_TYPE* newBasicBlock(BASIC_BLOCK_TYPE* predecessor){
    BasicBlock* block = create_new_block();
    if (predecessor){
        block->predecessor_num = 1;
        connect_block(predecessor,block);
    }
    return block;
}

void setBasicBlockSealed(BASIC_BLOCK_TYPE* basic_block){
    basic_block->is_sealed = 1;
}

OPERAND_TYPE* toSSAIntConst(struct IntConst* int_const, BASIC_BLOCK_TYPE* basic_block){
    OPERAND_TYPE* oprand = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    oprand->type = INT;
    oprand->operand.v.intValue = int_const->value;
    return oprand;
>>>>>>> 58da50caa6ee794fb20a45a2fc3c3cc893acb1b4
}

OPERAND_TYPE* toSSAString(struct String* str, BASIC_BLOCK_TYPE* basic_block){
    OPERAND_TYPE* oprand = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    oprand->type = STRING;
    oprand->operand.v.str = strdup(str->content);
    return oprand;
}

OPERAND_TYPE* toSSAVarSymEntry(struct VarSymEntry* vse, BASIC_BLOCK_TYPE* basic_block){
    OPERAND_TYPE* opr = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    opr->type = REGISTER;
    opr->operand.reg_idx = -1;
    return opr;
}

OPERAND_TYPE* toSSABasicBlock(BASIC_BLOCK_TYPE* target_block, BASIC_BLOCK_TYPE* basic_block){
    OPERAND_TYPE* opr = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));

    opr->type = INT;
    opr->operand.v.intValue = (int)(target_block);
    return opr;
}

OPERAND_TYPE* toSSAFuncName(struct FuncSymEntry* fse, BASIC_BLOCK_TYPE* basic_block){
    OPERAND_TYPE* opr = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    opr->type = FUNCID;
    opr->operand.v.funcID = (void*)fse;
    return opr;
}

OPERAND_TYPE* toSSAParamName(struct VarSymEntry* vse, BASIC_BLOCK_TYPE* basic_block){
    //wait to be realize
}

