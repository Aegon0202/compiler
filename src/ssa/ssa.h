#ifndef SSA_SSA_H
#define SSA_SSA_H
#include "../ENUM.h"
#include "../SysY.symtab/SysY.symtab.def.h"
#include "../SysY.type/SysY.type.def.h"
#include "../utils/DequeList.h"
#include "../utils/Malloc.h"
#include "../utils/link.h"
#define MAX_CAPACITY 1000000
#define DEF_TABLE_SIZE 1000

typedef void* ID;

typedef struct BasicBlockNode BasicBlockNode;
typedef struct Ir Ir;
typedef struct Phi Phi;
typedef struct Operand Operand;

struct Phi {
    Operand* value;
    list_entry_t op_link;
};

typedef struct BasicBlock {
    int is_sealed;                      //前驱节点是否已经确定
    int is_full;                        //
    Ir* ir_list;                        // 基本块内包含的ir
    Ir* phi_list;                       //基本块内包含的phi函数
    int predecessor_num;                //前驱节点个数
    BasicBlockNode* predecessors;       //前驱节点
    int successor_num;                  //后代个数
    BasicBlockNode* successors;         //后代节点
    BasicBlockNode* dominator;          //必经节点集
    BasicBlockNode* i_dominator;        //直接必经结点，长度为1的链表
    BasicBlockNode* Children;           //该结点直接统治的结点集
    BasicBlockNode* dominant_frontier;  //该结点的必经边界

    //-----------------以下为生成ssa的辅助标号
    int has_already;
    int work;
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
    list_entry_t* phi_op_list;
} Value;

//放在ir中的操作数，可以是Value也可以是寄存器，type表示的是操作数的类型,如果是value则只能是address或者int
struct Operand {
    int type;
    union {
        Value v;
        int reg_idx;
    } operand;
    int bottom_index;
};

//IR
struct Ir {
    int type;
    Operand *op1, *op2, *op3;
    list_entry_t ir_link;
};

typedef struct Address {
    BasicBlock* block;
    Ir* ir;
} Address;

typedef struct def_use_chain {
    Ir* user;  //
    list_entry_t DU_chain;
} def_use_chain;

struct Definition {
    int variable;          //存放这个变量的寄存器编号
    Address* def_address;  //这个变量被定义的位置
    def_use_chain* chain;
};

#define le2struct(le, type, member) \
    to_struct((le), type, member)

#define le2BasicBlock(elem) le2struct(elem, BasicBlockNode, block_link)

extern int current_size;
extern int max_capacity;
extern Value* reg_list[MAX_CAPACITY];  //寄存器堆，在这个阶段，寄存器的数量为无限大

extern struct LinearList* id_list;            //这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量的
extern struct LinearList* reg_id_vartabelem;  // index: int value: VarTabElem*

extern struct LinearList* def_block;  // index: register index, value: list_entry_t*
extern struct LinearList* construct_Stack;
extern struct LinearList* construct_Counter;
extern struct LinearList* variable_bottom_index;
extern struct LinearList* bottom_index2New_reg;
extern struct LinearList* reg2def;

Ir* create_new_ir(int op_type, Operand* op1, Operand*, Operand*);
BasicBlock* create_new_block();
Value* new_Value();
void disconnect_block(BasicBlock* pre, BasicBlock* suc);
Ir* create_new_phi(Phi* op1, Operand* op3, Operand*);
void update_CFG(BasicBlock* start);
//建立祖先后代关系
void connect_block(BasicBlock* pre, BasicBlock* suc);
int read_variable(ID id, BasicBlock* block);
void write_variable(ID operand, BasicBlock* block, Ir* ir);
void seal_block();
int alloc_register();
int get_init_register();
const char* _op_to_str(Operand* op);

void delete_ir(Ir* ir);
void delete_operand(Operand*);
void delete_user(Operand* def, Ir* user);
void add_user(Operand* def, Ir* user);
void change_def_address(Ir* old_ir, BasicBlock* old_block, BasicBlock* new_block, Ir* new_before_ir);

void __dominance_frontier(BasicBlock* start);
void __caculate_dominance(BasicBlock* start);
void __immediate_dominance(BasicBlock* start);
list_entry_t* DF_plus(list_entry_t* list);
void __get_all_nodes(BasicBlock* block, void* node);

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
