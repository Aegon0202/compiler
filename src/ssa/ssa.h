#include"link.h"
#include"defs.h"
#include"../SysY.type/SysY.type.def.h"
#include"../SysY.type/SysY.type.symtab.h"
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
    char* str;
    void* funcID;
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
} BasicBlock;

typedef struct{
    BasicBlock* value;
    list_entry_t block_link;
} BasicBlockNode;

typedef struct{
    enum Operand_type{PHI,INT,STRING,FUNCID,REGISTER} type;
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
void add_user(ID id, Ir* value);
void delete_user(ID id, Ir* value);
BasicBlock* create_new_block();
Value* new_Value();

int read_variable(ID id, BasicBlock* block);
int read_variable_recursively(ID id, BasicBlock* block);

void write_variable(ID operand, BasicBlock* block,Ir* ir, Value v);
Phi* add_phi_operand(ID id,Phi* phi);
void remove_trivial_phi();
void seal_block();

#define BASIC_BLOCK_TYPE BasicBlock
#define IR_LIST_TYPE Ir
#define OPERAND_TYPE Operand

BASIC_BLOCK_TYPE* newBasicBlock(BASIC_BLOCK_TYPE* predecessor);
void setBasicBlockSealed(BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct IntConst*）转化成一个合法的操作符
 * 
 * :param (struct IntConst*) int_const 一个输入，代表一个常数
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAIntConst(struct IntConst* int_const, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct String*）转化成一个合法的操作符
 * 
 * :param (struct String*) str 一个输入，代表一个字符串
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAString(struct String* str, BASIC_BLOCK_TYPE* basic_block);


/**
 * 将一个（struct VarSymEntry*）转化成一个合法的操作符
 * 可能是作为输入，也可能是作为输出
 * 
 * :param (struct VarSymEntry*) vse 一个输入，代表一个int变量
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAVarSymEntry(struct VarSymEntry* vse, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（BASIC_BLOCK_TYPE*）转化成一个合法的操作符
 * 
 * :param (BASIC_BLOCK_TYPE*) target_block 一个输入，代表一个基本块，即跳转地址
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSABasicBlock(BASIC_BLOCK_TYPE* target_block, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct FuncSymEntry*）转化成一个合法的操作符，作为函数名称。
 * 可用于 PARAM和CALL
 * 
 * :param (struct FuncSymEntry*) fse 一个输入，函数名。
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAFuncName(struct FuncSymEntry* fse, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct VarSymEntry*）转化成一个合法的操作符，作为函数中形参的名称。
 * 可用于 PARAM
 * vse 可能为NULL，作为可变参数用。按照从左向右传参。
 * 
 * :param (struct VarSymEntry*) vse 一个输入，函数形参名。
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAParamName(struct VarSymEntry* vse, BASIC_BLOCK_TYPE* basic_block);

/**
 * 返回一个可用于存放并读取数据的操作数。
 * 
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSATempVariable(BASIC_BLOCK_TYPE* basic_block);