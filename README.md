# complier

SF(salty fish) complier is a C-developed optimizing complier which is being produced by a group of undergraduate students 
from Huazhong University of Science and Technology.  
It supports the programming language: [Sysy2021](https://gitlab.eduxiji.net/nscscc/compiler2021/-/blob/master/SysY语言定义.pdf),
operating system: Ubuntu 18.04, hardware architecture: ARMv7-32bit.



| SSA_IR_OP                                        | OP1      | OP2      | OP3        |
| ------------------------------------------------ | -------- | -------- | ---------- |
| NOP(占位用)                                      | -        | -        | -          |
| PARAM（传参）                                    | 函数名称 | 函数形参 | 函数实参   |
| CALL（函数调用）                                 | 函数名称 | 参数个数 | 返回值     |
| JUMP（无条件跳转）                               | -        | -        | 目标块     |
| BRANCH（条件为真跳转）                           | 条件     | 条件为真的目标块 | 条件为假的目标块     |
| RETURNSTMT（返回）`return`  `op1` （`op1`可能为空） | 返回值   | -        | -          |
| LOAD（加载数据）`op3`  `<-` mem[ `op1`+`op2` ]   | 基址     | 偏移量   | 目标操作数 |
| STORE（存储数据）mem[ `op1`+`op2` ] `<-` `op3`   | 基址     | 偏移量   | 源操作数   |
| ASSIGN（赋值）`op3`  `<-`  `op1`                 | 源操作数 | -        | 目标变量   |
| K_NOT（求反）`op3`  `<-`  `!`  `op1`             | 源操作数 | -        | 目标操作数 |
| K_ADD（求和）`op3`  `<-`  `op1`  `+`  `op2`      | 源操作数 | 源操作数 | 目标操作数 |
| K_SUB（求差）`op3`  `<-`  `op1`  `-`  `op2`      | 源操作数 | 源操作数 | 目标操作数 |
| K_MUL（求积）`op3`  `<-`  `op1`  `*`  `op2`      | 源操作数 | 源操作数 | 目标操作数 |
| K_DIV（求商）`op3`  `<-`  `op1`  `/`  `op2`      | 源操作数 | 源操作数 | 目标操作数 |
| K_MOD（求余）`op3`  `<-`  `op1`  `%`  `op2`      | 源操作数 | 源操作数 | 目标操作数 |
| K_AND（逻辑与）`op3`  `<-`  `op1`  `&&`  `op2`   | 源操作数 | 源操作数 | 目标操作数 |
| K_OR（逻辑或）`op3`  `<-`  `op1`  `||`  `op2`    | 源操作数 | 源操作数 | 目标操作数 |
| K_EQ（相等）`op3`  `<-`  `op1`  `==`  `op2`      | 源操作数 | 源操作数 | 目标操作数 |
| K_EQ（不相等）`op3`  `<-`  `op1`  `！=`  `op2`   | 源操作数 | 源操作数 | 目标操作数 |
| K_LT（小于）`op3`  `<-`  `op1`  `<`  `op2`       | 源操作数 | 源操作数 | 目标操作数 |
| K_LTE（小于等于）`op3`  `<-`  `op1`  `<=`  `op2` | 源操作数 | 源操作数 | 目标操作数 |
| K_GT（大于）`op3`  `<-`  `op1`  `>`  `op2`       | 源操作数 | 源操作数 | 目标操作数 |
| K_GTE（大于等于）`op3`  `<-`  `op1`  `>=`  `op2` | 源操作数 | 源操作数 | 目标操作数 |

```c
#define BASIC_BLOCK_TYPE BasicBlock
#define IR_LIST_TYPE Ir
#define OPERAND_TYPE Operand

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
 * 将ir添加到ir_list的末尾
 * 
 * :param (IR_LIST_TYPE*) ir_list 一个ir的链表
 * :param (IR_LIST_TYPE*) ir 一条ir指令
 * 
 * :return (void) 无返回值
*/
//void appendIrList(IR_LIST_TYPE* ir_list, IR_TYPE* ir);

/**
 * 将ir添加到BasicBlock中ir_list的末尾
 * 
 * :param (BASIC_BLOCK_TYPE*) basic_block 一个ir的链表
 * :param (IR_LIST_TYPE*) ir 一条ir指令
 * 
 * :return (void) 无返回值
*/
//void appendToBasicBlock(BASIC_BLOCK_TYPE* basic_block, IR_TYPE* ir);

/**
 * 将next添加到head的末尾
 * 
 * :param (IR_LIST_TYPE*) head 一个ir的链表
 * :param (IR_LIST_TYPE*) next 另一个ir的链表
 * 
 * :return (void) 无返回值
*/
//void mergeIrList(IR_LIST_TYPE* head, IR_LIST_TYPE* next);

/**
 * 获取ir_list最后产生的操作符
 * 
 * :param (IR_LIST_TYPE*) ir_list
 * 
 * :return (OPERAND_TYPE*) 最后的操作符
 * 
 * :example 
 *      ir_list: add op1,op2,op3
 *               sub op2,op4,op5
 *      return op5
*/
//OPERAND_TYPE* getLastOperandIrList(IR_LIST_TYPE* ir_list);

/**
 * 添加一条由predecessor指向successor的边
 * 
 * :param (BASIC_BLOCK_TYPE*) predecessor   前驱
 * :param (BASIC_BLOCK_TYPE*) successor     后继
*/
void addBasicBlockEdge(BASIC_BLOCK_TYPE* predecessor, BASIC_BLOCK_TYPE* successor);

```