#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "../utils/DequeList.h"
#include "../utils/LinkedTable.h"
#include "./local_op.h"

int __is_const_op_in_loop(OPERAND_TYPE* op, struct LoopBlocks* loop);

void __find_back_edge(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct DequeList* queue = (struct DequeList*)args;
    list_entry_t* succ_head = &basic_block->successors->block_link;
    list_entry_t* succ_next = list_next(succ_head);
    while (succ_head != succ_next) {
        BASIC_BLOCK_TYPE* succ_block = (le2BasicBlock(succ_next)->value);
        list_entry_t* dom_head = &basic_block->dominator->block_link;
        list_entry_t* dom_next = &basic_block->dominator->block_link;
        while (dom_next != dom_head) {
            BASIC_BLOCK_TYPE* dom_block = (le2BasicBlock(dom_next)->value);
            if (succ_block == dom_block) {
                MALLOC(item, struct Item, 1);  // key为basic_block value为必经节点
                item->key = basic_block;
                item->value = dom_block;
                pushFrontDequeList(queue, item);
            }
            dom_next = list_next(dom_next);
        }
        succ_next = list_next(succ_next);
    }
}

struct DequeList* __get_loop_basic_block(BASIC_BLOCK_TYPE* basic_block, BASIC_BLOCK_TYPE* dom_block) {
    struct LinearList* loop_all = newLinearList();
    struct DequeList* stack = newDequeList();
    setLinearList(loop_all, (size_t)basic_block, basic_block);
    setLinearList(loop_all, (size_t)dom_block, basic_block);

    if (basic_block != dom_block) {
        pushFrontDequeList(stack, basic_block);
        while (!isEmptyDequeList(stack)) {
            BASIC_BLOCK_TYPE* b_block = popFrontDequeList(stack);
            list_entry_t* head = &b_block->predecessors->block_link;
            list_entry_t* next = list_next(head);
            while (head != next) {
                BASIC_BLOCK_TYPE* p_block = (le2BasicBlock(next)->value);
                void* value = setLinearList(loop_all, (size_t)p_block, p_block);
                if (value == NULL) {
                    pushFrontDequeList(stack, p_block);
                }
                next = list_next(next);
            }
        }
    }

    void* value = NULL;
    while ((value = popLinearList(loop_all)) != NULL) {
        pushFrontDequeList(stack, value);
    }
    freeLinearList(&loop_all);
    return stack;
}

int __is_type_inviriant_extraction(IR_TYPE* ir) {
    switch (ir->type) {
        case K_NOT:
        case K_ADD:
        case K_MUL:
        case K_EQ:
        case K_NEQ:
        case K_SUB:
        case K_DIV:
        case K_MOD:
        case K_LT:
        case K_LTE:
        case K_GT:
        case K_GTE:
        case ASSIGN:
        case CALL:
            return 1;
        default:
            return 0;
    }
}

struct LoopBlocks {
    struct LinearList* list;
    BASIC_BLOCK_TYPE* loop_entry;
    int loop_block_num;
    struct LinearList* out_blocks;
    int out_blocks_num;

    struct DequeList* func_param;
    struct DequeList* ir_rely;
};

Address* __new_address(BASIC_BLOCK_TYPE* block, IR_TYPE* ir) {
    MALLOC(adr, Address, 1);
    adr->block = block;
    adr->ir = ir;
    return adr;
}

struct LoopBlock* newLoopBlock(struct LinearList* linear, BASIC_BLOCK_TYPE* loop_entry, int block_num) {
    MALLOC(lb, struct LoopBlocks, 1);
    lb->list = linear;
    lb->loop_entry = loop_entry;
    lb->loop_block_num = block_num;
    lb->func_param = newDequeList();
    lb->ir_rely = newDequeList();
    return lb;
}

void freeLoopBlock(struct LoopBlocks* lb) {
    void* value;
    while ((value = popLinearList(lb->list)) != NULL)
        ;
    freeLinearList(&lb->list);
    while ((value = popLinearList(lb->out_blocks)) != NULL)
        ;
    freeLinearList(&lb->out_blocks);

    freeDequeList(&lb->func_param);
    freeDequeList(&lb->ir_rely);
}

int __is_block_in_BasicBlockNode(BASIC_BLOCK_TYPE* basic_block, BasicBlockNode* node) {
    list_entry_t* head = &node->block_link;
    list_entry_t* next = list_next(head);
    while (head != next) {
        BASIC_BLOCK_TYPE* b = (le2BasicBlock(next)->value);
        if (b == basic_block) {
            return 1;
        }
        next = list_next(next);
    }
    return 0;
}

int __is_block_in_liner_list(BASIC_BLOCK_TYPE* basi_block, struct LinearList* list, int num) {
    for (int i = 0; i < num; i++) {
        BASIC_BLOCK_TYPE* b = getLinearList(list, i);
        if (b == basi_block) {
            return 1;
        }
    }
    return 0;
}

int __is_const_ir_in_loop(IR_TYPE* ir, struct LoopBlocks* loop) {
    int is_const = 0;

    switch (ir->type) {
        case K_NOT:
        case ASSIGN:
            is_const = __is_const_op_in_loop(ir->op1, loop);
            break;
        case K_ADD:
        case K_MUL:
        case K_EQ:
        case K_NEQ:
        case K_SUB:
        case K_DIV:
        case K_MOD:
        case K_LT:
        case K_LTE:
        case K_GT:
        case K_GTE:
            is_const = __is_const_op_in_loop(ir->op1, loop) && __is_const_op_in_loop(ir->op2, loop);
            break;
        default:
            is_const = 0;
            break;
    }
    if (ir->type == CALL) {
        if (((struct FuncTabElem*)(ir->op1->operand.v.funcID))->has_side_effect) {
            is_const = 0;
        } else {
            is_const = 1;
            while (!isEmptyDequeList(loop->func_param)) {
                IR_TYPE* p = popFrontDequeList(loop->func_param);
                if (!__is_const_op_in_loop(p->op3, loop)) {
                    is_const = 0;
                }
            }
        }
    }
    if (is_const) {
        pushFrontDequeList(loop->ir_rely, ir);
        return 1;
    }
    void* value = NULL;
    return 0;
}

int __is_const_op_in_loop(OPERAND_TYPE* op, struct LoopBlocks* loop) {
    if (op->type != REGISTER) {
        return 1;
    }
    struct Definition* def = getOperandDefinition(op);
    for (int i = 0; i < loop->loop_block_num; i++) {
        BASIC_BLOCK_TYPE* b = getLinearList(loop->list, i);
        if (b == def->def_address->block) {
            return __is_const_ir_in_loop(def->def_address->ir, loop);
        }
    }
    return 1;
}

int __is_can_extraction(IR_TYPE* ir, BASIC_BLOCK_TYPE* basic_block, struct LoopBlocks* loop) {
    for (int i = 0; i < loop->out_blocks_num; i++) {
        BASIC_BLOCK_TYPE* out = getLinearList(loop->out_blocks, i);
        if (!__is_block_in_BasicBlockNode(basic_block, out->dominator)) {
            return 0;
        }
    }
    OPERAND_TYPE tmp_op;
    tmp_op.type = REGISTER;

    struct Definition* def = getOperandDefinition(ir->op3);
    list_entry_t* head = &def->chain->DU_chain;
    list_entry_t* next = list_next(head);
    while (head != next) {
        def_use_chain* use = le2struct(next, def_use_chain, DU_chain);
        tmp_op.operand.reg_idx = use->user;
        struct Definition* u_def = getOperandDefinition(&tmp_op);
        if (!__is_block_in_BasicBlockNode(basic_block, u_def->def_address->block->dominator)) {
            return 0;
        }

        next = list_next(next);
    }
    return 1;
}

void __loop_invariant_extraction(struct LoopBlocks* loop) {
    BASIC_BLOCK_TYPE* before_entry = NULL;
    // index: basic block address value: LinearList< index: ir address value: is_invariant >

    struct LinearList* invariant_block_ir = newLinearList();
    for (int i = 0; i < loop->loop_block_num; i++) {
        BASIC_BLOCK_TYPE* block = getLinearList(loop->list, i);
        struct LinearList* ir_list = newLinearList();
        list_entry_t* head = &block->ir_list->ir_link;
        list_entry_t* next = list_next(head);
        while (head != next) {
            IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
            MALLOC(is_const, int, 1);
            if (__is_type_inviriant_extraction(ir->type) &&
                __is_const_ir_in_loop(ir, loop) &&
                __is_can_extraction(ir, block, loop)) {
                while (!isEmptyDequeList(loop->ir_rely)) {
                    IR_TYPE* move_ir = popBackDequeList(loop->ir_rely);
                    struct Definition* def = getOperandDefinition(move_ir->op3);
                    change_def_address(move_ir, def->def_address->block, before_entry, NULL);
                }
            }

            while (!isEmptyDequeList(loop->ir_rely)) {
                popBackDequeList(loop->ir_rely);
            }
            setLinearList(ir_list, ir, is_const);
            next = list_next(next);
        }
        setLinearList(invariant_block_ir, block, ir_list);
    }
}

void __add_loop_outblock(struct LoopBlocks* lb) {
    lb->out_blocks = newLinearList();
    lb->out_blocks_num = 0;
    for (int i = 0; i < lb->loop_block_num; i++) {
        BASIC_BLOCK_TYPE* b = getLinearList(lb->list, i);
        list_entry_t* head = &b->successors->block_link;
        list_entry_t* next = list_next(head);
        while (head != next) {
            BASIC_BLOCK_TYPE* succ = (le2BasicBlock(next)->value);
            if (!__is_block_in_liner_list(succ, lb->list, lb->loop_block_num) &&
                !__is_block_in_liner_list(succ, lb->out_blocks, lb->out_blocks_num)) {
                setLinearList(lb->out_blocks, lb->out_blocks_num, succ);
                lb->out_blocks_num++;
            }
            next = list_next(next);
        }
    }
}

void __add_loop_entry_before(struct LoopBlocks* lb) {
    BASIC_BLOCK_TYPE* new_b = create_new_block();
    list_entry_t* head = &lb->loop_entry->predecessors->block_link;
    list_entry_t* next = list_next(next);
    while (head != next) {
        BASIC_BLOCK_TYPE* block = (le2BasicBlock(next)->value);
        if (!__is_block_in_liner_list(block, lb->list, lb->loop_block_num)) {
            disconnect_block(block, lb->loop_entry);
            connect_block(block, new_b);
        }
        next = list_next(next);
    }
    connect_block(new_b, lb->loop_entry);
    lb->loop_entry = new_b;
}

void loopInvariantExtraction(struct FuncTabElem* elem) {
    struct DequeList* back_edge = newDequeList();
    struct DequeList* loop_queue = newDequeList();
    struct LinearList* table = newLinearList();
    deepTraverseSuccessorsBasicBlock(elem->blocks, __find_back_edge, back_edge);
    while (!isEmptyDequeList(back_edge)) {
        unsigned long long int num;
        struct Item* item = popFrontDequeList(back_edge);
        struct LinearList* list = convertToLinearList(__get_loop_basic_block(item->key, item->value), &num);
        struct LoopBlocks* o_b = getLinearList(table, item->value);
        if (o_b == NULL) {
            struct LoopBlocks* lb = newLoopBlock(list, item->value, num);
            setLinearList(table, item->value, lb);
            pushFrontDequeList(loop_queue, lb);
        } else {
            BASIC_BLOCK_TYPE* basic_block;
            while ((basic_block = popLinearList(list)) != NULL) {
                if (!__is_block_in_liner_list(basic_block, o_b->list, o_b->loop_block_num)) {
                    setLinearList(o_b->list, o_b->loop_block_num, basic_block);
                    o_b->loop_block_num++;
                }
            }
            freeLinearList(&list);
        }
        free(item);
    }

    freeDequeList(&back_edge);
    void* value = NULL;
    while ((value = popLinearList(table)) != NULL)
        ;
    freeLinearList(&table);

    unsigned long long int loop_num;
    struct LinearList* loops_list = convertToLinearList(loop_queue, &loop_num);
    for (unsigned long long int i = 0; i < loop_num; i++) {
        struct LoopBlocks* lb = getLinearList(loops_list, i);
        __add_loop_outblock(lb);
        __add_loop_entry_before(lb);
    }

    update_CFG(elem->blocks);

    for (unsigned long long int i = 0; i < loop_num; i++) {
        struct LoopBlocks* lb = getLinearList(loops_list, i);
        __loop_invariant_extraction(lb);
    }
}