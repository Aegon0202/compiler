#include"link.h"
#include"defs.h"
//
enum IrType {Add,Sub,Mul,Div,Minus,And,Or,Not,Branch,Jump,Call,Return,Function,Param};

typedef struct {
    BasicBlock* block;
    Ir* ir;
} Address;

typedef struct{
    enum IrType op_type;
    Operand_def* op1, op2;
} MidVariable;

typedef union{
    Phi* phi;
    int intValue;
    MidVariable Mid;
} Value;

typedef struct {
    int is_sealed;
    int is_full;
    IrList  ir_list;
    BasicBlockNode predecessors;
    BasicBlockNode successors;
} BasicBlock;

typedef struct{
    BasicBlock* value;
    list_entry_t block_link;
} BasicBlockNode;

typedef struct{
    enum IrType type;
    Operand_def* op1,op2,op3;
    list_entry_t ir_link;
} Ir,*IrList;

typedef struct{
    enum value_type{Phi, undef, constInt, MidVar} type;
    int complete; // 如果是phi类型的数，该标志标识此phi函数是否complete
    char* label;  //标识该值的原始标号
    Value value;
    Address address;
    def_use_chain users;
} Operand_def;

typedef struct{
    Ir* value;
    list_entry_t def_use_link;
} *def_use_chain, def_use_node;

typedef struct{
    int is_complete;
    union Value{
        int const_ini_value;
        Phi phi_value;
    } value;
    list_entry_t phi_para_link;
} Phi;
#define le2struct(le,type,member)    \
        to_struct((le),type,member)


Ir* create_new_ir();
Phi* create_new_phi();
Operand_def* create_new_definition();
def_use_node* create_new_def_use_chain();
void add_user(Operand_def* operand, Ir* value);
void delete_user(Operand_def* operand, Ir* value);
BasicBlock* create_new_block();

void writeVariable(void* variable,BasicBlock* block,Value value);
void readVariable();
