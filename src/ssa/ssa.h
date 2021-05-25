#include"link.h"
#include"defs.h"
//
typedef struct {
    IrList  ir_list;
//incomplete phi
    BasicBlockNode predecessors_link;
    BasicBlockNode successors_link;
} BasicBlock;

typedef struct{
    BasicBlock* value;
    list_entry_t block_link;
} BasicBlockNode;

typedef struct{
    enum IrType {Add,Sub,Mul,Div,Minus,And,Or,Not,Branch,Jump,Call,Return,Function,Param} type;
    int operator;
    void* op1,op2,op3;
    list_entry_t ir_link;
} Ir,*IrList;

typedef struct{
    enum value_type{phi, undef, const, int} type;
    int complete; // 如果是phi类型的数，该标志标识此phi函数是否complete
    char* label;  //标识该值的原始标号
    int int_value;    //当类型为const int， 其值被记录在此。
    def_use_chain users;
} Operand_def;

typedef struct{
    Ir* value;
    list_entry_t def_use_link;
} *def_use_chain, def_use_node;


#define le2struct(le,type,member)    \
        to_struct((le),type,member)



def_use_node* init_user_chain();
void addUser(Operand_def* operand, Ir* value);
void deleteUser(Operand_def* operand, Ir* value);
BasicBlock* create_new_block();

