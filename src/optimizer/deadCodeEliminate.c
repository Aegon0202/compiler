#include "../SysY.symtab/SysY.symtab.def.h"
#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "../utils/IrType.h"
#include "./local_op.h"
struct LinearList* block_2_rcfg_block;
struct LinearList* rcfg_block_2_block;
void __get_all_blocks(BASIC_BLOCK_TYPE* basic_block, void* args);

void __gen_rcfg_block(BASIC_BLOCK_TYPE* block, void* args) {
    if (getLinearList(block_2_rcfg_block, (size_t)block) == NULL) {
        BASIC_BLOCK_TYPE* r_block = create_new_block();
        setLinearList(block_2_rcfg_block, (size_t)block, r_block);
        setLinearList(rcfg_block_2_block, (size_t)r_block, block);
    }
}

void __connect_rcfg_block(BASIC_BLOCK_TYPE* block, void* args) {
    BASIC_BLOCK_TYPE* rcfg_entry = (BASIC_BLOCK_TYPE*)args;
    BASIC_BLOCK_TYPE* r_block = getLinearList(block_2_rcfg_block, (size_t)block);
    list_entry_t* head = &block->successors->block_link;
    list_entry_t* next = list_next(head);
    while (head != next) {
        BASIC_BLOCK_TYPE* succ = le2BasicBlock(next)->value;
        BASIC_BLOCK_TYPE* r_pre = getLinearList(block_2_rcfg_block, (size_t)succ);
        connect_block(r_pre, r_block);
        next = list_next(next);
    }

    IR_TYPE* ir = le2struct(block->ir_list->ir_link.prev, IR_TYPE, ir_link);
    if (ir->type == RETURNSTMT) {
        connect_block(rcfg_entry, r_block);
    }
}

void free_CFG(BASIC_BLOCK_TYPE* block) {
    struct DequeList* deque = newDequeList();
    deepTraverseSuccessorsBasicBlock(block, __get_all_blocks, deque);
    while (!isEmptyDequeList(deque)) {
        /* code */
    }
}

void generateRCFG(struct FuncTabElem* elem) {
    if (elem->rcfg_blocks == NULL) {
        elem->rcfg_blocks = create_new_block();
        deepTraverseSuccessorsBasicBlock(elem->blocks, __gen_rcfg_block, NULL);
        deepTraverseSuccessorsBasicBlock(elem->blocks, __connect_rcfg_block, elem->rcfg_blocks);
        __dominance_frontier(elem->rcfg_blocks);
    }
}

struct AddressSet {
    Address address;
    list_entry_t link;
};
void __prepare_ir_list(BASIC_BLOCK_TYPE* block, void* args) {
    list_entry_t* all_head = (list_entry_t*)args;
    list_entry_t* head = &block->ir_list->ir_link;
    list_entry_t* next = list_next(head);
    while (head != next) {
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
        MALLOC(a_set, struct AddressSet, 1);
        a_set->address.block = block;
        a_set->address.ir = ir;
        list_add_before(all_head, &a_set->link);
        next = list_next(next);
    }
}

int __is_prelive(IR_TYPE* ir) {
    switch (ir->type) {
        case RETURNSTMT:
        case JUMP:
        case STORE:
        case BRANCH:
            return 1;
        case PARAM:
            return ((struct FuncTabElem*)(ir->op1->operand.v.funcID))->has_side_effect;
        case CALL:
            return ((struct FuncTabElem*)(ir->op1->operand.v.funcID))->has_side_effect;
        default:
            return 0;
    }
}

static void __read_op(OPERAND_TYPE* op, struct AddressSet* address, struct DequeList* work_list, struct LinearList* live, struct LinearList* ir_2_address) {
    IfNull(op, return;);
    if (op->type != REGISTER || op->bottom_index == -1) {
        return;
    }
    struct Definition* def = get_op_definition(op);
    int* status = getLinearList(live, (size_t)def->def_address->ir);
    EnsureNotNull(status);
    if (!(*status)) {
        *status = 1;
        pushFrontDequeList(work_list, getLinearList(ir_2_address, (size_t)def->def_address->ir));
        IR_TYPE* def_ir = def->def_address->ir;
        if (def_ir->type == CALL) {
            list_entry_t* ir_list = def_ir->ir_link.prev;
            int param_num = def_ir->op2->operand.v.intValue;
            for (int i = 0; i < param_num; i++) {
                IR_TYPE* param = le2struct(ir_list, IR_TYPE, ir_link);
                int* status = getLinearList(live, (size_t)param);
                EnsureNotNull(status);
                if (!(*status)) {
                    *status = 1;
                    pushFrontDequeList(work_list, getLinearList(ir_2_address, (size_t)param));
                }
                ir_list = list_prev(ir_list);
            }
        }
    }
}

void deadCodeEliminate(struct FuncTabElem* func) {
    if (func->blocks == NULL) {
        return;
    }
    list_entry_t all_ir;
    list_init(&all_ir);
    struct LinearList* live = newLinearList();  // index: ir_address, value: true or false
    struct DequeList* work_list = newDequeList();
    struct LinearList* ir_2_address = newLinearList();

    generateRCFG(func);
    deepTraverseSuccessorsBasicBlock(func->blocks, __prepare_ir_list, &all_ir);

    list_entry_t* head = &all_ir;
    list_entry_t* next = list_next(head);
    while (head != next) {
        struct AddressSet* address = le2struct(next, struct AddressSet, link);
        MALLOC(status, int, 1);
        *status = __is_prelive(address->address.ir);
        setLinearList(live, (size_t)address->address.ir, status);
        setLinearList(ir_2_address, (size_t)address->address.ir, address);
        if (*status) {
            pushFrontDequeList(work_list, address);
        }
        next = list_next(next);
    }

    while (!isEmptyDequeList(work_list)) {
        struct AddressSet* address = popFrontDequeList(work_list);

#define READ_OP(op) __read_op(op, address, work_list, live, ir_2_address)
#define WRITE_OP(op)
        IR_OP_READ_WRITE(address->address.ir, READ_OP, WRITE_OP, break;);
#undef READ_OP
#undef WRITE_OP

        list_entry_t* rdf_head = &(((BasicBlock*)getLinearList(block_2_rcfg_block, (size_t)address->address.block))->dominant_frontier->block_link);
        list_entry_t* rdf_next = list_next(rdf_head);
        while (rdf_head != rdf_next) {
            BASIC_BLOCK_TYPE* r_b = le2BasicBlock(rdf_next)->value;
            BASIC_BLOCK_TYPE* b = getLinearList(rcfg_block_2_block, (size_t)r_b);
            if (b != NULL) {
                IR_TYPE* last_ir = le2struct(b->ir_list->ir_link.prev, IR_TYPE, ir_link);
                int* status = getLinearList(live, (size_t)last_ir);
                EnsureNotNull(status);
                if (!(*status)) {
                    *status = 1;
                    pushFrontDequeList(work_list, getLinearList(ir_2_address, (size_t)last_ir));
                }
            }
            rdf_next = list_next(rdf_next);
        }
    }
    freeDequeList(&work_list);

    next = list_next(head);
    while (head != next) {
        struct AddressSet* address = le2struct(next, struct AddressSet, link);
        int* status = getLinearList(live, (size_t)address->address.ir);
        if (!(*status)) {
            delete_ir(address->address.ir, address->address.block);
        }
        next = list_next(next);
    }
}