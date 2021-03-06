
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "./ssa.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils/DequeList.h"
#include "../utils/IrType.h"
#include "../utils/LinkedTable.h"
#include "traverse.h"
int current_size;
int begin_index;
int max_capacity;
Ir* currentIr;

struct LinearList* id_list;  // index: VarTabElem* value: int* 这个数组为ast和IR之间的桥梁，表示在每个寄存器中存的value在ast中是属于哪个变量

struct LinearList* reg_id_vartabelem;  // index: int value: VarTabElem*
struct LinearList* reg2def;            // index :int, value: Definition
//--------以下3个变量用于构造ssa形式
struct LinearList* def_block;  // index: register index, value: list_entry_t*
struct LinearList* construct_Stack;
struct LinearList* construct_Counter;

struct LinearList* variable_bottom_index;  //每个变量下标到不同definition位置的映射
struct LinearList* bottom_index2New_reg;   //每个变量的下标映射到不同的新寄存器

void __debug_pause_there();
void __search_block(BasicBlock*);
list_entry_t* __search_BlockNode_elem(list_entry_t*, BasicBlock*);

int alloc_register() {
    return current_size++;
}

int get_init_register() {
    return begin_index;
}

void set_init_register(int index) {
    begin_index = index;
}

Ir* create_new_ir(int op_type, Operand* op1, Operand* op2, Operand* op3, BasicBlock* block) {
    Ir* ir = (Ir*)malloc(sizeof(Ir));
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    ir->type = op_type;
    ir->block = block;
    return ir;
}

void delete_operand(Operand* op) {
    if (!op) return;
    if (op->type != PHI_OP)
        free(op);
    else {
        list_entry_t* phi_head = op->operand.v.phi_op_list;
        list_entry_t* phi_elem = list_next(phi_head);
        while (phi_head != phi_elem) {
            Operand* d_op = le2struct(phi_elem, Phi, op_link)->value;
            list_entry_t* tmp_elem = phi_elem;
            phi_elem = list_next(phi_elem);

            list_del(tmp_elem);
            free(d_op);
        }
    }
}

void __delete_DU_process_ir(Ir* ir, BasicBlock* block) {
#define READ_OP(op) delete_user(op, ir)
#define WRITE_OP(op)
    IR_OP_READ_WRITE(ir, READ_OP, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
}

//同时在这里维护du链
//删除一条单独的ir，没有在任何一条ir链中，除了自身
void delete_ir(Ir* ir, BasicBlock* block) {
    //change du link
    __delete_DU_process_ir(ir, block);
    list_del(&(ir->ir_link));
    delete_operand(ir->op1);
    delete_operand(ir->op2);
    delete_operand(ir->op3);
    free(ir);
}

Ir* create_new_phi(Phi* op1, Operand* op3, Operand* op2, BasicBlock* block) {
    MALLOC(op, Operand, 1);
    op->type = PHI_OP;
    op->operand.v.phi_op_list = &(op1->op_link);
    return create_new_ir(PHI, op, op2, op3, block);
}

struct Definition* create_new_definition(int reg, Ir* ir, BasicBlock* block) {
    MALLOC(def, struct Definition, 1);
    MALLOC_WITHOUT_DECLARE(def->chain, def_use_chain, 1);
    MALLOC_WITHOUT_DECLARE(def->def_address, Address, 1);
    list_init(&(def->chain->DU_chain));
    def->chain->user = NULL;
    def->variable = reg;
    def->def_address->block = block;
    def->def_address->ir = ir;
    setLinearList(reg2def, ir->op3->operand.reg_idx, def);
    return def;
}

struct Definition* get_op_definition(Operand* op) {
    return getLinearList(reg2def, op->operand.reg_idx);
}
//创建一个新的block
BasicBlock* create_new_block() {
    MALLOC(block, BasicBlock, 1);
    MALLOC_WITHOUT_DECLARE(block->predecessors, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->successors, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->ir_list, Ir, 1);
    MALLOC_WITHOUT_DECLARE(block->phi_list, Ir, 1);
    MALLOC_WITHOUT_DECLARE(block->dominator, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->i_dominator, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->Children, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->dominant_frontier, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->block_LRA, BlockBegin, 1);
    MALLOC_WITHOUT_DECLARE(block->arm_ir_list, struct ArmIr, 1);
    block->is_sealed = 0;
    block->is_full = 0;
    block->predecessor_num = 0;
    block->successor_num = 0;
    block->predecessors->value = NULL;
    block->successors->value = NULL;
    block->ir_list->type = NOP;
    block->phi_list->type = NOP;
    block->dominator->value = NULL;
    block->i_dominator->value = NULL;
    block->Children->value = NULL;
    block->dominant_frontier->value = NULL;
    block->has_already = 0;
    block->work = 0;
    block->block_LRA->block = block;
    block->arm_ir_list->type = NOP;
    block->block_LRA->block_live_gen = newDequeList();
    block->block_LRA->block_live_kill = newDequeList();
    block->block_LRA->block_live_in = newDequeList();
    block->block_LRA->block_live_out = newDequeList();
    list_init(&(block->predecessors->block_link));
    list_init(&(block->successors->block_link));
    list_init(&(block->ir_list->ir_link));
    list_init(&(block->phi_list->ir_link));
    list_init(&(block->dominator->block_link));
    list_init(&(block->i_dominator->block_link));
    list_init(&(block->Children->block_link));
    list_init(&(block->dominant_frontier->block_link));
    list_init(&(block->arm_ir_list->ir_link));

    BlockBegin* begin = block->block_LRA;
    begin->block = block;
    begin->cur_val_num = 0;
    begin->first_op_id = -1;
    begin->last_op_id = -1;
    begin->loop_depth = 0;
    begin->loop_index = 0;

    return block;
}
int getPredecessorNum(BasicBlock* basic_block) {
    return basic_block->predecessor_num;
}

void delete_BasicBlock(BasicBlock* block) {
    return;
}

Operand* create_new_operand(int type, int reg, long long value) {
    MALLOC(op, Operand, 1);
    op->type = type;
    if (type == REGISTER)
        op->operand.reg_idx = reg, op->bottom_index = 0;
    else {
        op->operand.v.intValue = value;
    }
    return op;
}

Operand* operand_dup(Operand* op) {
    return create_new_operand(op->type, op->operand.reg_idx, op->operand.v.intValue);
}

BasicBlock* get_idominator(BasicBlock* block) {
    return le2BasicBlock(list_next(&(block->i_dominator->block_link)))->value;
}

//为两个block建立祖先和后继的关系
void connect_block(BasicBlock* pre, BasicBlock* suc) {
    pre->successor_num++;
    suc->predecessor_num++;
    BasicBlockNode* tmp = (BasicBlockNode*)malloc(sizeof(BasicBlockNode));
    tmp->value = suc;
    list_add(&(pre->successors->block_link), &(tmp->block_link));

    tmp = (BasicBlockNode*)malloc(sizeof(BasicBlockNode));
    tmp->value = pre;
    list_add(&(suc->predecessors->block_link), &(tmp->block_link));
}

void disconnect_block(BasicBlock* pre, BasicBlock* suc) {
    pre->successor_num--;
    suc->predecessor_num--;

    list_entry_t* pre_list_head = &pre->successors->block_link;
    list_entry_t* suc_list_head = &suc->predecessors->block_link;

    list_entry_t* del1 = __search_BlockNode_elem(pre_list_head, suc);
    list_entry_t* del2 = __search_BlockNode_elem(suc_list_head, pre);

    EnsureNotNull(del1);
    EnsureNotNull(del2);

    list_del(del1);
    list_del(del2);

    free(le2BasicBlock(del1));
    free(le2BasicBlock(del2));
}

int read_variable(ID id, BasicBlock* block) {
    //根据id寻找是否在次之前定义过这个变量
    int* reg_p = getLinearList(id_list, (size_t)id);
    if (reg_p == NULL) {
        return -1;
    } else {
        return *reg_p;
    }
}

//user = def
void add_user(Operand* def, Ir* user) {
    if (user && def && def->type == REGISTER) {
        list_entry_t* du_head = &(get_op_definition(def)->chain->DU_chain);
        MALLOC(du_node, def_use_chain, 1);
        du_node->user = user;

        list_add(du_head, &(du_node->DU_chain));
    }
}

void delete_user(Operand* def, Ir* user) {
    if (user && def && def->type == REGISTER) {
        list_entry_t* du_head = &(get_op_definition(def)->chain->DU_chain);
        list_entry_t* du_elem = list_next(du_head);

        while (du_head != du_elem) {
            def_use_chain* tmp = le2struct(du_elem, def_use_chain, DU_chain);
            if (tmp->user == user) {
                list_del(du_elem);
                free(tmp);
                return;
            }
            du_elem = list_next(du_elem);
        }
        PrintErrExit("uesr do not find");
    }
}

Operand* search_op_in_phi_list(Ir* Phi_ir, int reg) {
    if (Phi_ir && Phi_ir->type == PHI) {
        list_entry_t* op_head = Phi_ir->op1->operand.v.phi_op_list;
        list_entry_t* op_elem = list_next(op_head);
        while (op_head != op_elem) {
            Operand* op_value = le2struct(op_elem, Phi, op_link)->value;
            if (op_value->type == REGISTER && op_value->operand.reg_idx == reg) {
                return op_value;
            }
            op_elem = list_next(op_elem);
        }
    }
    return NULL;
}

void __DU_process_ir(Ir* ir, BasicBlock* block) {
#define READ_OP(op) add_user(op, ir)
#define WRITE_OP(op)
    IR_OP_READ_WRITE(ir, READ_OP, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
}

void construct_DU_chain_local(BasicBlock* block, void* args) {
    list_entry_t* ir_head = &(block->ir_list->ir_link);
    list_entry_t* ir_elem = list_next(ir_head);

    while (ir_head != ir_elem) {
        Ir* ir_value = le2struct(ir_elem, Ir, ir_link);
        __DU_process_ir(ir_value, block);
        ir_elem = list_next(ir_elem);
    }
}

void construct_DU_chain_global(BasicBlock* start) {
    deepTraverseSuccessorsBasicBlock(start, construct_DU_chain_local, NULL);
}

void change_def(Operand* op, BasicBlock* block, Ir* ir) {
    EnsureNotNull(op);
    if (op->type != REGISTER)
        PrintErrExit("const do not has a definition");
    struct Definition* def = get_op_definition(op);
    if (block)
        def->def_address->block = block;
    if (ir)
        def->def_address->ir = ir;
}

void change_def_address(Ir* old_ir, BasicBlock* old_block, BasicBlock* new_block, Ir* new_before_ir) {
    list_del(&(old_ir->ir_link));
    old_ir->block = new_block;
    list_entry_t* ir_head = &(new_block->ir_list->ir_link);
    if (new_before_ir)
        list_add_after(&new_before_ir->ir_link, &old_ir->ir_link);
    else
        list_add_before(ir_head, &(old_ir->ir_link));

#define READ_OP(op)
#define WRITE_OP(op) change_def(op, new_block, NULL)
    IR_OP_READ_WRITE(old_ir, READ_OP, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
}

BASIC_BLOCK_TYPE* newBasicBlock(BASIC_BLOCK_TYPE* predecessor) {
    BasicBlock* b = create_new_block();
    if (predecessor)
        connect_block(predecessor, b);
    return b;
}

void setBasicBlockSealed(BASIC_BLOCK_TYPE* basic_block) {
    basic_block->is_sealed = 1;
}

OPERAND_TYPE* toSSAIntConst(struct IntConst* int_const, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(operand, OPERAND_TYPE, 1);
    operand->type = INT;
    operand->operand.v.intValue = int_const->value;
    return operand;
}

OPERAND_TYPE* toSSAString(struct String* str, BASIC_BLOCK_TYPE* basic_block) {
    OPERAND_TYPE* oprand = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    oprand->type = ConstSTRING;
    oprand->operand.v.str = strdup(str->content);
    return oprand;
}

OPERAND_TYPE* toSSAVarTabElemRead(struct VarTabElem* vte, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(op, OPERAND_TYPE, 1);
    op->type = REGISTER;
    ID id = (ID)vte;
    int res = read_variable(id, basic_block);
    if (res != -1)
        op->operand.reg_idx = res;
    else
        op->operand.reg_idx = 0;
    return op;
}

OPERAND_TYPE* toSSAVarTabElemWrite(struct VarTabElem* vte, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(op, OPERAND_TYPE, 1);
    int res = read_variable((ID)vte, basic_block);
    if (res != -1) {
        op->type = REGISTER;
        op->operand.reg_idx = res;
    } else {
        res = alloc_register();
        op->type = REGISTER;
        op->operand.reg_idx = res;

        MALLOC(reg_p, int, 1);
        *reg_p = res;
        setLinearList(id_list, (size_t)vte, reg_p);
        setLinearList(reg_id_vartabelem, *reg_p, vte);

        MALLOC(node, BasicBlockNode, 1);
        node->value = NULL;
        list_entry_t* l = &(node->block_link);
        list_init(l);
        setLinearList(def_block, res, l);

        struct LinearList* ll = newLinearList();
        struct LinearList* bottom2New_reg = newLinearList();
        setLinearList(variable_bottom_index, res, ll);
        setLinearList(bottom_index2New_reg, res, bottom2New_reg);
    }
    list_entry_t* list = getLinearList(def_block, res);
    if (!__search_BlockNode_elem(list, basic_block)) {
        MALLOC(node, BasicBlockNode, 1);
        node->value = basic_block;
        list_add(list, &(node->block_link));
    }

    return op;
}

OPERAND_TYPE* toSSAOffset(int base, long long offset, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(op, OPERAND_TYPE, 1);
    op->type = base;
    op->operand.v.intValue = offset;
    return op;
}

OPERAND_TYPE* toSSABasicBlock(BASIC_BLOCK_TYPE* target_block, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(opr, Operand, 1);
    opr->type = BASIC_BLOCK;
    opr->operand.v.b = target_block;
    return opr;
}

OPERAND_TYPE* toSSAFuncName(struct FuncTabElem* fte, BASIC_BLOCK_TYPE* basic_block) {
    OPERAND_TYPE* opr = (OPERAND_TYPE*)malloc(sizeof(OPERAND_TYPE));
    opr->type = FUNCID;
    opr->operand.v.funcID = (void*)fte;
    return opr;
}

OPERAND_TYPE* toSSAParamName(struct VarTabElem* vse, BASIC_BLOCK_TYPE* basic_block) {
    return NULL;
}

OPERAND_TYPE* toSSATempVariable(BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(op, OPERAND_TYPE, 1);
    op->type = REGISTER;
    op->operand.reg_idx = alloc_register();

    MALLOC(reg_id, int, 1);
    *reg_id = op->operand.reg_idx;
    setLinearList(id_list, (size_t)op, reg_id);

    MALLOC(node, BasicBlockNode, 1);
    node->value = NULL;
    list_entry_t* list = &(node->block_link);
    list_init(list);
    MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
    node->value = basic_block;
    list_add(list, &(node->block_link));
    setLinearList(def_block, *reg_id, list);

    struct LinearList* bottom_list = newLinearList();
    setLinearList(variable_bottom_index, *reg_id, bottom_list);
    return op;
}

IR_TYPE* newIR(int op, OPERAND_TYPE* op1, OPERAND_TYPE* op2, OPERAND_TYPE* op3, BASIC_BLOCK_TYPE* basic_block) {
    EnsureNotNull(basic_block);
    Ir* ir = create_new_ir(op, op1, op2, op3, basic_block);
    Ir* head = basic_block->ir_list;
    list_add_before(&(head->ir_link), &(ir->ir_link));
    return ir;
}

void addBasicBlockEdge(BASIC_BLOCK_TYPE* predecessor, BASIC_BLOCK_TYPE* successor) {
    connect_block(predecessor, successor);
}

static int __block_equal(void* k1, void* k2) {
    return k1 == k2;
}

void goThroughFunction(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args) {
    struct LinkedTable* visited = newLinkedTable(__block_equal);
    struct DequeList* queue = newDequeList();
    setLinkedTable(visited, basic_block_head, basic_block_head);
    pushFrontDequeList(queue, basic_block_head);
    while (!isEmptyDequeList(queue)) {
        BasicBlock* block = popBackDequeList(queue);
        func(block, args);
        list_entry_t* head = &(block->successors->block_link);
        list_entry_t* next = list_next(head);
        while (next != head) {
            BasicBlockNode* t_node = le2struct(next, BasicBlockNode, block_link);
            BasicBlock* t_block = t_node->value;
            void* res = getLinkedTable(visited, t_block);
            if (res == NULL) {
                pushFrontDequeList(queue, t_block);
                setLinkedTable(visited, t_block, t_block);
            }
            next = list_next(next);
        }
    }
    freeDequeList(&queue);
    struct Item* t;
    while ((t = popLinkedTable(visited)) != NULL)
        free(t);
    freeLinkedTable(&visited);
}

const char* _op_to_str(Operand* op) {
    static char buffer[20];
    if (op == NULL) {
        snprintf(buffer, 20, "%5s: -", "-");
        return buffer;
    }
    switch (op->type) {
        case INT:
            snprintf(buffer, 20, "%5s: $%d", "int", (int)op->operand.v.intValue);
            break;
        case REGISTER:
            snprintf(buffer, 20, "%5s: %%%d_%d", "reg", op->operand.reg_idx, op->bottom_index);
            break;
        case FRAMEPOINT:
            snprintf(buffer, 20, "%5s: @%lld", "$fp", op->operand.v.intValue);
            break;
        case STACKPOINT:
            snprintf(buffer, 20, "%5s: @%lld", "$sp", op->operand.v.intValue);
            break;
        case GLOBALDATA:
            snprintf(buffer, 20, "%5s: %-10s", "$gd", ((struct VarTabElem*)op->operand.v.intValue)->name);
            break;
        case ConstSTRING:
            snprintf(buffer, 20, "%5s: %-10s", "str", op->operand.v.str);
            break;
        case FUNCID:
            snprintf(buffer, 20, "%5s: %-10s", "func", ((struct FuncTabElem*)(op->operand.v.funcID))->name);
            break;
        case BASIC_BLOCK:
            snprintf(buffer, 20, "%5s: %p", "block", op->operand.v.b);
            break;
        case PHI_OP:
            snprintf(buffer, 20, "%5s: %p", "phi", op->operand.v.phi_op_list);
            break;
        default:
            PrintErrExit("error %s", EnumTypeToString(op->type));
    }
    return buffer;
}

void __print_ssa_ir(IR_TYPE* ir) {
    printf("op: %12s\t", EnumTypeToString(ir->type));
    printf("op1: %-20s\t", _op_to_str(ir->op1));
    printf("op2: %-20s\t", _op_to_str(ir->op2));
    printf("op3: %-20s\t", _op_to_str(ir->op3));
    printf("\n");
}

void __print_basic_block(BASIC_BLOCK_TYPE* basic_block, void* args) {
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next;
    printf("block address %p:\n", basic_block);

    next = list_next(head);
    while (next != head) {
        Ir* ir = le2struct(next, Ir, ir_link);
        __print_ssa_ir(ir);
        if (ir->type == PHI && args == NULL) {
            list_entry_t* phi_op_list = ir->op1->operand.v.phi_op_list;
            list_entry_t* phi_op_elem = list_next(phi_op_list);
            printf("phi_op:");
            while (phi_op_list != phi_op_elem) {
                Operand* op = le2struct(phi_op_elem, Phi, op_link)->value;
                printf("\t%15s", _op_to_str(op));
                phi_op_elem = list_next(phi_op_elem);
            }
            printf("\n");
        }
        next = list_next(next);
    }
}

//ssa form construct-------------------------------------------------------------------

void __get_all_nodes(BasicBlock* block, void* node) {
    MALLOC(new_node, BasicBlockNode, 1);
    new_node->value = block;
    list_add_before(&(((BasicBlockNode*)(node))->block_link), &(new_node->block_link));
}

void __init_dominator(BasicBlock* block, BasicBlockNode* node_set) {
    list_entry_t* head = &(block->dominator->block_link);
    list_entry_t* head_node_set = &(node_set->block_link);
    list_entry_t* elem = list_next(head_node_set);

    while (head_node_set != elem) {
        MALLOC(node, BasicBlockNode, 1);
        node->value = le2struct(elem, BasicBlockNode, block_link)->value;
        list_add(head, &(node->block_link));

        elem = list_next(elem);
    }
    return;
}

void __init_strict_dominator(BasicBlock* block, void* args) {
    list_entry_t* list = &(block->i_dominator->block_link);
    list_entry_t* head = &(block->dominator->block_link);
    list_entry_t* elem = list_next(head);
    BasicBlockNode* node;
    while (head != elem) {
        if (le2BasicBlock(elem)->value != block) {
            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
            node->value = le2BasicBlock(elem)->value;
            list_add(list, &(node->block_link));
        }
        elem = list_next(elem);
    }
}

list_entry_t* __search_BlockNode_elem(list_entry_t* list, BasicBlock* value) {
    list_entry_t* elem = list_next(list);
    list_entry_t* res = NULL;
    while (elem != list) {
        BasicBlock* b = le2struct(elem, BasicBlockNode, block_link)->value;
        if (b == value) {
            res = elem;
            break;
        }
        elem = list_next(elem);
    }
    return res;
}

void __delet_list(list_entry_t* list1) {
    list_entry_t* elem1 = list_next(list1);
    while (elem1 != list1) {
        list_entry_t* elem1_copy = elem1;
        elem1 = list_next(elem1);
        free(le2struct(elem1_copy, BasicBlockNode, block_link));
    }
    free(le2struct(list1, BasicBlockNode, block_link));
}

//list1 = list1交list2
list_entry_t* __intersection_list(list_entry_t* list1, list_entry_t* list2) {
    list_entry_t* elem1 = list_next(list1);
    //list_entry_t* elem2 = list_next(list2);
    BasicBlockNode* node;
    list_entry_t* result;
    MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
    result = &(node->block_link);
    list_init(result);
    node->value = NULL;
    while (elem1 != list1) {
        if (__search_BlockNode_elem(list2, le2struct(elem1, BasicBlockNode, block_link)->value)) {
            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
            node->value = le2struct(elem1, BasicBlockNode, block_link)->value;
            list_add(result, &(node->block_link));
        }
        elem1 = list_next(elem1);
    }
    elem1 = list_next(list1);
    return result;
}

list_entry_t* __union_list(list_entry_t* list1, list_entry_t* list2) {
    MALLOC(node, BasicBlockNode, 1);
    node->value = NULL;
    list_entry_t* list3 = &(node->block_link);
    list_init(list3);

    list_entry_t* elem = list_next(list1);
    while (elem != list1) {
        BasicBlock* value = le2BasicBlock(elem)->value;
        if (!__search_BlockNode_elem(list3, value)) {
            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
            node->value = value;
            list_add(list3, &(node->block_link));
        }
        elem = list_next(elem);
    }

    elem = list_next(list2);
    while (elem != list2) {
        BasicBlock* value = le2BasicBlock(elem)->value;
        if (!__search_BlockNode_elem(list3, value)) {
            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
            node->value = value;
            list_add(list3, &(node->block_link));
        }
        elem = list_next(elem);
    }
    return list3;
}

//list1是不是list2的子集
int __is_sublist(list_entry_t* list1, list_entry_t* list2) {
    list_entry_t* elem = list_next(list1);
    while (elem != list1) {
        BasicBlock* value = le2BasicBlock(elem)->value;
        if (!__search_BlockNode_elem(list2, value)) {
            return 0;
        }
        elem = list_next(elem);
    }
    return 1;
}

int __list_equal(list_entry_t* list1, list_entry_t* list2) {
    return __is_sublist(list1, list2) && __is_sublist(list2, list1);
}

void Post_order_traverse(BasicBlock* start, BasicBlockNode* node_set) {
    list_entry_t* list = &(node_set->block_link);
    list_entry_t* child_head = &(start->Children->block_link);
    list_entry_t* child_elem = list_next(child_head);
    while (child_head != child_elem) {
        BasicBlock* value = le2BasicBlock(child_elem)->value;
        Post_order_traverse(value, node_set);
        child_elem = list_next(child_elem);
    }

    MALLOC(node, BasicBlockNode, 1);
    node->value = start;
    list_add_before(list, &(node->block_link));
}

void __dominance_frontier(struct FuncTabElem* func) {
    __caculate_dominance(func);
    BasicBlock* start = func->blocks;

    list_entry_t *head, *elem;
    BasicBlockNode* node;
    //获取全部basic block
    MALLOC(node_set, BasicBlockNode, 1);
    list_init(&(node_set->block_link));
    Post_order_traverse(start, node_set);
    head = &(node_set->block_link);
    elem = list_next(head);

    while (elem != head) {
        BasicBlock* value = le2BasicBlock(elem)->value;

        list_entry_t* y_head = &(le2BasicBlock(elem)->value->successors->block_link);
        list_entry_t* y_elem = list_next(y_head);
        list_entry_t* z_head = &(le2BasicBlock(elem)->value->Children->block_link);
        list_entry_t* z_elem = list_next(z_head);

        while (y_head != y_elem) {
            BasicBlock* y_value = le2BasicBlock(y_elem)->value;
            if (get_idominator(y_value) != value) {
                MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                node->value = y_value;
                list_add(&(value->dominant_frontier->block_link), &(node->block_link));
            }
            y_elem = list_next(y_elem);
        }

        while (z_elem != z_head) {
            BasicBlock* z_value = le2BasicBlock(z_elem)->value;
            y_head = &(z_value->dominant_frontier->block_link);
            y_elem = list_next(y_head);
            while (y_head != y_elem) {
                BasicBlock* y_value = le2BasicBlock(y_elem)->value;
                if (get_idominator(y_value) != value) {
                    MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                    node->value = y_value;
                    list_add(&(value->dominant_frontier->block_link), &(node->block_link));
                }
                y_elem = list_next(y_elem);
            }

            z_elem = list_next(z_elem);
        }
        elem = list_next(elem);
    }

    __delet_list(&(node_set->block_link));
}

void delete_control_flow_ans(BasicBlock* block, void* args) {
    __delet_list(&block->dominator->block_link);
    __delet_list(&block->i_dominator->block_link);
    __delet_list(&block->Children->block_link);
    __delet_list(&block->dominant_frontier->block_link);
    MALLOC_WITHOUT_DECLARE(block->dominator, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->i_dominator, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->Children, BasicBlockNode, 1);
    MALLOC_WITHOUT_DECLARE(block->dominant_frontier, BasicBlockNode, 1);
    list_init(&block->dominator->block_link);
    list_init(&block->i_dominator->block_link);
    list_init(&block->Children->block_link);
    list_init(&block->dominant_frontier->block_link);
}

void update_CFG(struct FuncTabElem* func) {
    BasicBlock* start = func->blocks;
    deepTraverseSuccessorsBasicBlock(start, delete_control_flow_ans, NULL);

    __dominance_frontier(func);
}

list_entry_t* caculate_DF_set(list_entry_t* list) {
    BasicBlockNode* node;
    MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
    node->value = NULL;
    list_entry_t* result = &(node->block_link);
    list_init(result);

    list_entry_t* elem = list_next(list);
    while (elem != list) {
        list_entry_t* list_df = &(le2BasicBlock(elem)->value->dominant_frontier->block_link);
        list_entry_t* tmp = __union_list(result, list_df);
        __delet_list(result);
        result = tmp;
        elem = list_next(elem);
    }
    return result;
}

list_entry_t* DF_plus(list_entry_t* list) {
    list_entry_t* result;
    list_entry_t* tmp;

    int change = 1;

    result = caculate_DF_set(list);
    list = __union_list(result, list);
    do {
        change = 0;
        result = caculate_DF_set(list);
        tmp = __union_list(result, list);
        if (!__list_equal(tmp, list)) {
            __delet_list(list);
            list = tmp;
            change = 1;
        }
    } while (change != 0);
    __delet_list(list);
    __delet_list(tmp);
    return result;
}

void __placement_phi(BasicBlock* start) {
    int iter_count = 0;
    int loop_val = 0;

    //遍历每个变量

    for (loop_val = 0; loop_val < current_size; loop_val++) {
        if (!getLinearList(reg_id_vartabelem, loop_val)) continue;
        iter_count += 1;

        MALLOC(node, BasicBlockNode, 1);
        list_entry_t* worklist = &(node->block_link);
        list_init(worklist);

        list_entry_t* head = getLinearList(def_block, loop_val);
        list_entry_t* elem = list_next(head);
        while (head != elem) {
            BasicBlock* X_value = le2BasicBlock(elem)->value;
            X_value->work = iter_count;

            if (!__search_BlockNode_elem(worklist, X_value)) {
                MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                node->value = X_value;
                list_add_before(worklist, &(node->block_link));
            }
            elem = list_next(elem);
        }

        while (worklist != list_next(worklist)) {
            list_entry_t* worklist_elem = list_next(worklist);
            BasicBlock* worklist_value = le2BasicBlock(worklist_elem)->value;

            list_del(worklist_elem);
            free(le2BasicBlock(worklist_elem));

            list_entry_t* Y_head = &(worklist_value->dominant_frontier->block_link);
            list_entry_t* Y_elem = list_next(Y_head);
            while (Y_elem != Y_head) {
                BasicBlock* Y_value = le2BasicBlock(Y_elem)->value;
                if (Y_value->has_already < iter_count) {
                    MALLOC(op_3, Operand, 1);
                    op_3->type = REGISTER;
                    op_3->operand.reg_idx = loop_val;

                    MALLOC(phi_func, Phi, 1);
                    int i = 0;
                    phi_func->value = NULL;
                    list_init(&(phi_func->op_link));

                    for (i = 0; i < Y_value->predecessor_num; i++) {
                        MALLOC(op_node, Phi, 1);
                        MALLOC_WITHOUT_DECLARE(op_node->value, Operand, 1);
                        op_node->value->type = REGISTER;
                        op_node->value->operand.reg_idx = loop_val;

                        list_add(&(phi_func->op_link), &(op_node->op_link));
                    }

                    MALLOC(tmp_op, Operand, 1);
                    tmp_op->type = INT;
                    tmp_op->operand.v.intValue = Y_value->predecessor_num;

                    Ir* ir = create_new_phi(phi_func, op_3, tmp_op, Y_value);
                    list_add(&(Y_value->ir_list->ir_link), &(ir->ir_link));
                    Y_value->has_already = iter_count;
                    if (Y_value->work < iter_count) {
                        Y_value->work = iter_count;
                        if (!__search_BlockNode_elem(worklist, Y_value)) {
                            MALLOC_WITHOUT_DECLARE(node, BasicBlockNode, 1);
                            node->value = Y_value;
                            list_add_before(worklist, &(node->block_link));
                        }
                    }
                }
                Y_elem = list_next(Y_elem);
            }
        }
    }
}

void renaming_init(BasicBlock* start) {
    int cur_var = 0;

    for (cur_var = 0; cur_var < current_size; cur_var++) {
        struct DequeList* stack = newDequeList();
        MALLOC(i, int, 1);
        *i = 0;
        setLinearList(construct_Counter, cur_var, i);
        setLinearList(construct_Stack, cur_var, stack);
    }
}

void __process_read_op(Operand* op, Ir* ir) {
    if (ir->type == PHI)
        return;
    if (op && op->type == REGISTER) {
        int index = op->operand.reg_idx;
        struct DequeList* stack = getLinearList(construct_Stack, index);
        int* i = getFrontDequeList(stack);
        EnsureNotNull(i);
        op->bottom_index = *i;
    }
}

void __process_write_op(Operand* op3, Ir* ir_value, BasicBlock* block) {
    EnsureNotNull(op3);
    int def_index = op3->operand.reg_idx;
    int* i = getLinearList(construct_Counter, def_index);
    op3->bottom_index = *i;
    MALLOC(j, int, 1);
    *j = *i;
    struct Definition* def = create_new_definition(def_index, ir_value, block);
    struct LinearList* bottom_index_def = getLinearList(variable_bottom_index, def_index);
    setLinearList(bottom_index_def, *i, def);
    pushFrontDequeList(getLinearList(construct_Stack, def_index), j);
    *i = *i + 1;
}

//输入ir, 将该ir的目的操作数和源操作数放到相应位置
void __operand_decode(Ir* ir, BasicBlock* block) {
#define READ_OP(op) __process_read_op(op, ir)
#define WRITE_OP(op) __process_write_op(op, ir, block)
    IR_OP_READ_WRITE(ir, READ_OP, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
}

void __pop_op(Operand* op) {
    EnsureNotNull(op);
    int index = op->operand.reg_idx;
    struct DequeList* stack = getLinearList(construct_Stack, index);
    popFrontDequeList(stack);
}

void __pop_search(Ir* ir, BasicBlock* block) {
#define READ_OP(op)
#define WRITE_OP(op) __pop_op(op)
    IR_OP_READ_WRITE(ir, READ_OP, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
}

//suc 的第几个前驱是pre
int which_pre(BasicBlock* pre, BasicBlock* suc) {
    list_entry_t* head = &(suc->predecessors->block_link);
    list_entry_t* elem = list_next(head);
    int which = 1;
    while (head != elem) {
        BasicBlock* value = le2BasicBlock(elem)->value;
        if (value == pre) {
            return which;
        }
        elem = list_next(elem);
        which++;
    }
    PrintErrExit("do not find pre");
    return -1;
}

void __search_block(BasicBlock* block) {
    //-------------

    list_entry_t* ir_head = &(block->ir_list->ir_link);
    list_entry_t* ir_elem = list_next(ir_head);

    while (ir_elem != ir_head) {
        Ir* ir_value = le2struct(ir_elem, Ir, ir_link);
        __operand_decode(ir_value, block);
        ir_elem = list_next(ir_elem);
    }
    list_entry_t* suc_head = &(block->successors->block_link);
    list_entry_t* suc_elem = list_next(suc_head);
    int which;
    while (suc_elem != suc_head) {
        list_entry_t* phi_ir_head = &(le2BasicBlock(suc_elem)->value->ir_list->ir_link);
        list_entry_t* phi_ir_elem = list_next(phi_ir_head);
        which = which_pre(block, le2BasicBlock(suc_elem)->value);
        while (phi_ir_elem != phi_ir_head) {
            Ir* phi_ir = le2struct(phi_ir_elem, Ir, ir_link);
            if (phi_ir->type != PHI)
                break;
            list_entry_t* phi_op_list = phi_ir->op1->operand.v.phi_op_list;
            list_entry_t* phi_op_elem = list_next(phi_op_list);
            int j = 1;
            while (phi_op_list != phi_op_elem) {
                if (j == which) {
                    int* i = getFrontDequeList(getLinearList(construct_Stack, phi_ir->op3->operand.reg_idx));

                    le2struct(phi_op_elem, struct Phi, op_link)->which_pre = block;
                    if (i) {
                        le2struct(phi_op_elem, struct Phi, op_link)->value->bottom_index = *i;
                        break;
                    } else {
                        le2struct(phi_op_elem, struct Phi, op_link)->value->type = INT;
                        le2struct(phi_op_elem, struct Phi, op_link)->value->bottom_index = -1;
                        le2struct(phi_op_elem, struct Phi, op_link)->value->operand.reg_idx = -1;
                        le2struct(phi_op_elem, struct Phi, op_link)->value->operand.v.intValue = -1;
                        break;
                    }
                }
                j++;
                phi_op_elem = list_next(phi_op_elem);
            }

            phi_ir_elem = list_next(phi_ir_elem);
        }
        suc_elem = list_next(suc_elem);
    }

    list_entry_t* children_head = &(block->Children->block_link);
    list_entry_t* children_elem = list_next(children_head);
    while (children_head != children_elem) {
        BasicBlock* c_block = le2BasicBlock(children_elem)->value;
        __search_block(c_block);
        children_elem = list_next(children_elem);
    }

    ir_elem = list_next(ir_head);
    while (ir_elem != ir_head) {
        Ir* ir = le2struct(ir_elem, Ir, ir_link);
        __pop_search(ir, block);
        ir_elem = list_next(ir_elem);
    }
}

void __modify_op(Operand* op) {
    if (op && op->type == REGISTER && getLinearList(reg_id_vartabelem, op->operand.reg_idx)) {
        int new_reg;
        int old_reg = op->operand.reg_idx;
        int old_bottom_index = op->bottom_index;
        struct LinearList* l = getLinearList(bottom_index2New_reg, old_reg);
        int* i = getLinearList(l, old_bottom_index);
        new_reg = *i;
        op->operand.reg_idx = new_reg;
    }
    if (op && op->type == PHI_OP) {
        list_entry_t* phi_head = op->operand.v.phi_op_list;
        list_entry_t* phi_elem = list_next(phi_head);

        while (phi_head != phi_elem) {
            Operand* op_sub = le2struct(phi_elem, Phi, op_link)->value;
            if (op_sub->type != REGISTER) {
                phi_elem = list_next(phi_elem);
                continue;
            }
            __modify_op(op_sub);
            phi_elem = list_next(phi_elem);
        }
    }
}

void __modify_op_local(BasicBlock* block, void* args) {
    list_entry_t* ir_head = &(block->ir_list->ir_link);
    list_entry_t* ir_elem = list_next(ir_head);

    while (ir_head != ir_elem) {
        Ir* ir_value = le2struct(ir_elem, Ir, ir_link);

        __modify_op(ir_value->op1);
        __modify_op(ir_value->op2);
        __modify_op(ir_value->op3);

        ir_elem = list_next(ir_elem);
    }
}

//遍历所有基本块，将操作数对应的寄存器名以 reg_index, 改成newReg
void modify_op_global(BasicBlock* start) {
    deepTraverseSuccessorsBasicBlock(start, __modify_op_local, NULL);
}

void reallocate_register(BasicBlock* start) {
    int begin_reg = alloc_register();
    set_init_register(begin_reg + 1);

    int cur_val, reg;
    for (cur_val = 0; cur_val < begin_reg; cur_val++) {
        void* id = getLinearList(reg_id_vartabelem, cur_val);
        if (!id)
            continue;
        struct LinearList* bottom_index2def = getLinearList(variable_bottom_index, cur_val);
        struct LinearList* bottom2New_reg_list = getLinearList(bottom_index2New_reg, cur_val);

        int b_index = 0;
        struct Definition* def = getLinearList(bottom_index2def, 0);
        do {
            EnsureNotNull(def);
            reg = alloc_register();
            MALLOC(r, int, 1);
            *r = reg;
            setLinearList(reg_id_vartabelem, reg, id);
            setLinearList(reg2def, reg, def);
            setLinearList(bottom2New_reg_list, b_index, r);
            b_index++;
            def = getLinearList(bottom_index2def, b_index);
        } while (def);
    }
}

//取suc的第j个predecessor
BasicBlock* getPredecessor(BasicBlock* suc, int j) {
    if (suc->predecessor_num < j)
        PrintErrExit("this block do not have so many predecessors");

    list_entry_t* pre_head = &suc->predecessors->block_link;
    list_entry_t* pre_elem = list_next(pre_head);
    int count = 1;
    while (pre_head != pre_elem) {
        if (count == j)
            return le2BasicBlock(pre_elem)->value;
        count++;
        pre_elem = list_next(pre_elem);
    }
    return NULL;
}

void convertOutssa_local(BasicBlock* block, void* args) {
    list_entry_t* ir_head = &(block->ir_list->ir_link);
    list_entry_t* ir_elem = list_next(ir_head);

    while (ir_head != ir_elem) {
        Ir* ir_value = le2struct(ir_elem, Ir, ir_link);
        if (ir_value->type == PHI) {
            //遍历phi中的操作数
            list_entry_t* op_head = ir_value->op1->operand.v.phi_op_list;
            list_entry_t* op_elem = list_next(op_head);
            Operand* write_op = ir_value->op3;
            int write_reg = write_op->operand.reg_idx;
            int j = 1;
            while (op_head != op_elem) {
                Operand* op_value = le2struct(op_elem, Phi, op_link)->value;
                BasicBlock* target_pre = le2struct(op_elem, Phi, op_link)->which_pre;
                if (op_value->bottom_index == -1) {
                    op_elem = list_next(op_elem);
                    continue;
                }
                int read_reg = op_value->operand.reg_idx;

                //list_entry_t* target_ir_list = &(getPredecessor(block, j)->ir_list->ir_link);
                list_entry_t* target_ir_list = &target_pre->ir_list->ir_link;

                Operand* op2 = create_new_operand(INT, -1, 0);
                Operand* op1 = create_new_operand(op_value->type, read_reg, op_value->operand.v.intValue);
                Operand* op3 = create_new_operand(REGISTER, write_reg, 0);
                Ir* new_ir = create_new_ir(K_ADD, op1, op2, op3, block);
                Ir* last_ir = le2struct(list_prev(target_ir_list), Ir, ir_link);
                if (last_ir->type != JUMP && last_ir->type != BRANCH && last_ir->type != RETURNSTMT) {
                    list_add_before(target_ir_list, &(new_ir->ir_link));
                } else {
                    list_add_before(list_prev(target_ir_list), &(new_ir->ir_link));
                }
                j++;
                op_elem = list_next(op_elem);
            }
        }
        ir_elem = list_next(ir_elem);
    }
    //从原来的基本块中删除所有phi函数
    ir_elem = list_next(ir_head);
    while (ir_elem != ir_head) {
        Ir* ir_value = le2struct(ir_elem, Ir, ir_link);
        if (ir_value->type != PHI)
            break;
        ir_elem = list_next(ir_elem);
        delete_ir(ir_value, block);
    }
}

void convertOutssa(BasicBlock* start) {
    deepTraverseSuccessorsBasicBlock(start, convertOutssa_local, NULL);
    return;
}

void convertAlltoSSAform() {
    struct FuncTabElem* elem;

    __placement_phi(NULL);

    renaming_init(NULL);

    for (int i = 0; i < func_table->next_func_index; i++) {
        elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks != NULL) {
            __search_block(elem->blocks);
        }
    }

    reallocate_register(NULL);

    for (int i = 0; i < func_table->next_func_index; i++) {
        elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks != NULL) {
            modify_op_global(elem->blocks);
            construct_DU_chain_global(elem->blocks);
        }
    }
}

void convertAllOutSSAform() {
    struct FuncTabElem* elem;
    for (int i = 0; i < func_table->next_func_index; i++) {
        elem = getLinearList(func_table->all_funcs, i);
        if (elem->blocks != NULL) {
            convertOutssa(elem->blocks);
        }
    }
}