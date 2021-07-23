#ifndef SSA_SSA_H
#define SSA_SSA_H
#include "../ENUM.h"
#include "../SysY.symtab/SysY.symtab.def.h"
#include "../SysY.type/SysY.type.def.h"
#include "../utils/Malloc.h"
#include "defs.h"
#include "link.h"
#define MAX_CAPACITY 1000000
#define DEF_TABLE_SIZE 1000

typedef void* ID;

typedef struct BasicBlockNode BasicBlockNode;
typedef struct Ir Ir;

typedef struct BasicBlock {
    int is_sealed;                 //前驱节点是否已经确定
    int is_full;                   //
    Ir* ir_list;                   // 基本块内包含的ir
    int predecessor_num;           //前驱节点个数
    BasicBlockNode* predecessors;  //前驱节点
    int successor_num;             //后代个数
    BasicBlockNode* successors;    //后代节点
    BasicBlockNode* dominantor;    //必经节点集
} BasicBlock;

//基本块链表
struct BasicBlockNode {
    BasicBlock* value;
    list_entry_t block_link;
};

typedef union {
    long long int intValue;
    char* str;
    void* funcID;
    BasicBlock* b;
    Ir* complex_value;
} Value;

//放在ir中的操作数，可以是Value也可以是寄存器，type表示的是操作数的类型,如果是value则只能是address或者int
typedef struct {
    int type;
    union {
        Value v;
        int reg_idx;
    } operand;
} Operand;

//IR
struct Ir {
    int type;
    Operand *op1, *op2, *op3;
    list_entry_t ir_link;
};

#define le2struct(le, type, member) \
    to_struct((le), type, member)

extern int current_size;

extern int max_capacity;

extern Value* reg_list[MAX_CAPACITY];  //寄存器堆，在这个阶段，寄存器的数量为无限大

extern struct LinearList* id_list;            //这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量的
extern struct LinearList* reg_id_vartabelem;  // index: int value: VarTabElem*

Ir* create_new_ir(int op_type, Operand* op1, Operand*, Operand*);
BasicBlock* create_new_block();
Value* new_Value();

//建立祖先后代关系
void connect_block(BasicBlock* pre, BasicBlock* suc);
void add_user(int i, BasicBlock* block, Ir* ir);
void delete_user(int, Ir*);
int read_variable(ID id, BasicBlock* block);
void write_variable(ID operand, BasicBlock* block, Ir* ir);
void seal_block();
int alloc_register();

#define BASIC_BLOCK_TYPE BasicBlock
#define IR_LIST_TYPE Ir
#define OPERAND_TYPE Operand
#define IR_TYPE Ir
//需要在SSA中完成的函数
/**
 * 生成一个新的基本块
 * 
 * :param (BASIC_BLOCK_TYPE*) predecessor 一个前驱，可能为NULL
 * 
 * :return (BASIC_BLOCK_TYPE*) 一个新生成的基本块
 */
BASIC_BLOCK_TYPE* newBasicBlock(BASIC_BLOCK_TYPE* predecessor);

/**
 * 设置一个基本块已封闭
 * 会在这个基本块的所有前驱都设置完的情况下调用
 * 
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本快
 * 
 * :return (void) 没有返回值
 */
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
 * 将一个（struct VarTabElem*）转化成一个合法的操作符
 * 作为输入
 * 
 * :param (struct VarTabElem*) vte 一个输入，代表一个int变量
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAVarTabElemRead(struct VarTabElem* vte, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个（struct VarTabElem*）转化成一个合法的操作符
 * 作为输出
 * 
 * :param (struct VarTabElem*) vte 一个输入，代表一个int变量
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAVarTabElemWrite(struct VarTabElem* vte, BASIC_BLOCK_TYPE* basic_block);

/**
 * 将一个偏移量（int）转成一个合法的代指其绝对地址的操作数。
 * 
 * :param (int) base 基址 取值为 FRAMEPOINT STACKPOINT GLOBALDATA
 * :param (int) offset 偏移量
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
 */
OPERAND_TYPE* toSSAOffset(int base, long long offset, BASIC_BLOCK_TYPE* basic_block);

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
 * 将一个（struct FuncTabElem*）转化成一个合法的操作符，作为函数名称。
 * 可用于 PARAM和CALL
 * 
 * :param (struct FuncTabElem*) fte 一个输入，函数名。
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSAFuncName(struct FuncTabElem* fte, BASIC_BLOCK_TYPE* basic_block);

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
OPERAND_TYPE* toSSAParamName(struct VarTabElem* vse, BASIC_BLOCK_TYPE* basic_block);

/**
 * 返回一个可用于存放并读取数据的操作数。
 * 
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将需要的ir或其他东西放到此基本块中
 * 
 * :return (OPERAND_TYPE*) ssa中的操作符
*/
OPERAND_TYPE* toSSATempVariable(BASIC_BLOCK_TYPE* basic_block);

/**
 * 创建一条ir
 * 
 * :param (int) op ir的操作指令
 * :param (OPERAND_TYPE*) op1,op2,op3 操作符
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个基本块，将新创建的ir放到此基本块中
 * 
 * :return (IR_LIST_TYPE*) 一条ir指令
*/
IR_TYPE* newIR(int op, OPERAND_TYPE* op1, OPERAND_TYPE* op2, OPERAND_TYPE* op3, BASIC_BLOCK_TYPE* basic_block);

/**
 * 添加一条由predecessor指向successor的边
 * 
 * :param (BASIC_BLOCK_TYPE*) predecessor   前驱
 * :param (BASIC_BLOCK_TYPE*) successor     后继
*/
void addBasicBlockEdge(BASIC_BLOCK_TYPE* predecessor, BASIC_BLOCK_TYPE* successor);

void goThroughFunction(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args);
void __print_basic_block(BASIC_BLOCK_TYPE* basic_block, void* args);
#endif