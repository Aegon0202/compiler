#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "../utils/DequeList.h"
#include "../utils/LinkedTable.h"
#include "./local_op.h"

struct __loop_invariant {
    struct DequeList* ir_rely;
};

int __is_ir_can_extraction(IR_TYPE* ir, BASIC_BLOCK_TYPE* basic_block, struct LoopBlocks* loop, struct __loop_invariant* li);
Address* __new_address(BASIC_BLOCK_TYPE* block, IR_TYPE* ir) {
    MALLOC(adr, Address, 1);
    adr->block = block;
    adr->ir = ir;
    return adr;
}
int __is_block_in_deque(BASIC_BLOCK_TYPE* basic_block, struct DequeList* deque);
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

int __is_op_can_extraction(OPERAND_TYPE* op, BASIC_BLOCK_TYPE* basic_block, struct LoopBlocks* loop, struct __loop_invariant* li) {
    if (op->type != REGISTER) {
        return 1;
    }
    struct Definition* def = get_op_definition(op);
    int num = sizeDequeList(loop->blocks);
    for (int i = 0; i < num; i++) {
        BASIC_BLOCK_TYPE* b = getDequeList(loop->blocks, i);
        if (b == def->def_address->block) {
            return __is_ir_can_extraction(def->def_address->ir, basic_block, loop, li);
        }
    }
    return 1;
}

int __is_ir_can_extraction(IR_TYPE* ir, BASIC_BLOCK_TYPE* basic_block, struct LoopBlocks* loop, struct __loop_invariant* li) {
    int is_can_extract = 0;

    switch (ir->type) {
        case K_NOT:
        case ASSIGN:
            is_can_extract = __is_op_can_extraction(ir->op1, basic_block, loop, li);
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
            is_can_extract = __is_op_can_extraction(ir->op1, basic_block, loop, li) && __is_op_can_extraction(ir->op2, basic_block, loop, li);
            break;
        default:
            is_can_extract = 0;
            break;
    }

    if (ir->type == CALL) {
        if (!(((struct FuncTabElem*)(ir->op1->operand.v.funcID))->has_side_effect)) {
            is_can_extract = 1;
            list_entry_t* ir_list = list_prev(&ir->ir_link);
            int param_num = ir->op2->operand.v.intValue;
            for (int i = 0; i < param_num; i++) {
                IR_TYPE* p_ir = le2struct(ir_list, IR_TYPE, ir_link);
                ir_list = list_prev(ir_list);
                if (!__is_op_can_extraction(p_ir->op3, basic_block, loop, li)) {
                    is_can_extract = 0;
                    break;
                }
            }
        }
    }

    if (is_can_extract) {
        int num = sizeDequeList(loop->out_blocks);
        for (int i = 0; i < num; i++) {
            BASIC_BLOCK_TYPE* out = getDequeList(loop->out_blocks, i);
            if (!__is_block_in_BasicBlockNode(basic_block, out->dominator)) {
                is_can_extract = 0;
                break;
            }
        }
    }

    if (is_can_extract) {
        OPERAND_TYPE tmp_op;
        tmp_op.type = REGISTER;

        struct Definition* def = get_op_definition(ir->op3);
        list_entry_t* head = &def->chain->DU_chain;
        list_entry_t* next = list_next(head);
        while (head != next) {
            def_use_chain* use = le2struct(next, def_use_chain, DU_chain);
            tmp_op.operand.reg_idx = use->user->op3->operand.reg_idx;
            struct Definition* u_def = get_op_definition(&tmp_op);
            if (__is_block_in_deque(u_def->def_address->block, loop->blocks) && !__is_block_in_BasicBlockNode(basic_block, u_def->def_address->block->dominator)) {
                is_can_extract = 0;
                break;
            }

            next = list_next(next);
        }
    }

    if (is_can_extract) {
        pushFrontDequeList(li->ir_rely, ir);
        return 1;
    }

    return 0;
}

void __loop_invariant_extraction(struct LoopBlocks* loop) {
    BASIC_BLOCK_TYPE* before_entry = loop->loop_before;
    IR_TYPE* before_entry_last_ir = le2struct(list_prev(&before_entry->ir_list->ir_link), IR_TYPE, ir_link);
    // index: basic block address value: LinearList< index: ir address value: is_invariant >
    struct __loop_invariant li;
    li.ir_rely = newDequeList();

    int num = sizeDequeList(loop->blocks);
    for (int i = 0; i < num; i++) {
        BASIC_BLOCK_TYPE* block = getDequeList(loop->blocks, i);

        list_entry_t* head = &block->ir_list->ir_link;
        list_entry_t* next = list_next(head);
        while (head != next) {
            IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
            next = list_next(next);
            MALLOC(is_const, int, 1);
            if (__is_ir_can_extraction(ir, block, loop, &li)) {
                printf("loop invirant extraction\n");
                while (!isEmptyDequeList(li.ir_rely)) {
                    IR_TYPE* move_ir = popBackDequeList(li.ir_rely);
                    struct Definition* def = get_op_definition(move_ir->op3);
                    IR_TYPE* before_ir = le2struct(list_prev(&before_entry_last_ir->ir_link), IR_TYPE, ir_link);
                    change_def_address(move_ir, def->def_address->block, before_entry, before_ir);
                }
            }
            while (!isEmptyDequeList(li.ir_rely)) {
                popBackDequeList(li.ir_rely);
            }
        }
    }

    while (popBackDequeList(li.ir_rely) != NULL)
        ;
    freeDequeList(&li.ir_rely);
}

void loopInvariantExtraction(struct FuncTabElem* elem) {
    int num = sizeDequeList(elem->loop_blocks);
    for (unsigned long long int i = 0; i < num; i++) {
        struct LoopBlocks* lb = getDequeList(elem->loop_blocks, i);
        __loop_invariant_extraction(lb);
    }
}