#include"link.h"
#include"defs.h"
#define MAX_CAPACITY 1000000
#define DEF_TABLE_SIZE 1000

typedef void* ID;


extern int current_size = 0;
extern int max_capacity = MAX_CAPACITY;

extern Value reg_list[MAX_CAPACITY];     //寄存器堆，在这个阶段，寄存器的数量为无限大
extern ID id_list[MAX_CAPACITY];         //这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量的
extern Definition tag[MAX_CAPACITY];     //这个序列也是对应与寄存器中的每个值，在优化阶段需要用到这些信息

enum IrType {HEAD,Add,Sub,Mul,Div,Minus,And,Or,Not,Branch,Jump,Call,Return,Function,Param};


typedef union{
    Phi* phi;
    int intValue;
} Value;

typedef struct {
    BasicBlock* block;
    Ir* ir;
}Address;


typedef struct {
    int is_sealed;
    int is_full;
    int begin_reg_idx;
    int end_reg_idx;
    Ir ir_list;
    int predecessor_num;
    BasicBlockNode predecessors;
    int successor_num;
    BasicBlockNode successors;
    
    int phi_num;
    int phi_list[1000];     //存储
} BasicBlock;

typedef struct{
    BasicBlock* value;
    list_entry_t block_link;
} BasicBlockNode;

typedef struct{
    enum Operand_type{VALUE,REGISTER} type;
    union{
        Value v;
        int reg_idx;
    } operand;
} Operand;

typedef struct{
    enum IrType type;
    Operand op1,op2,op3;
    list_entry_t ir_link;
} Ir;

typedef struct{
    int value_idx;
    Address def_address;
    def_use_node users;
} Definition;

typedef struct{
    Address address;
    list_entry_t def_use_link;
} def_use_node;

typedef struct{
    int idx;
    list_entry_t phi_para_link;
} Phi_parameter;

typedef struct{
    int is_complete;
    BasicBlock* def_block;
    Phi_parameter parameters;
} Phi;

#define le2struct(le,type,member)    \
        to_struct((le),type,member)


Ir* create_new_ir(enum IrType op_type, Operand op1,Operand,Operand);
Phi* create_new_phi(BasicBlock*);
Definition* create_new_definition();

def_use_node* create_new_def_use_node();
void add_user(int i, BasicBlock* block, Ir* ir)；
void delete_user(int , Ir* );
BasicBlock* create_new_block();
Value* new_Value();

int read_variable(ID id, BasicBlock* block);
int read_variable_recursively(ID id, BasicBlock* block);

void write_variable(ID operand, BasicBlock* block,Ir* ir, Value v);
void add_phi_operand(ID id,Phi* phi);
void remove_trivial_phi();
void seal_block();

void try_remove_trivial_phi(Phi*);
