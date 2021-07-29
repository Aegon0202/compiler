#include "../SysY.symtab/SysY.symtab.def.h"
#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "../utils/IrType.h"
#include "./local_op.h"
struct LinearList* block_2_rcfg_block;
struct LinearList* rcfg_block_2_block;

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
            return 1;
        case PARAM:
        case CALL:
            return ((struct FuncTabElem*)(ir->op3->operand.v.funcID))->has_side_effect;
        default:
            return 0;
    }
}

static void __read_op(OPERAND_TYPE* op, struct AddressSet* address, struct DequeList* work_list, struct LinearList* live, struct LinearList* ir_2_address) {
    IfNull(op, return;);
    if (op->type != REGISTER) {
        return;
    }
    struct Definition* def = get_op_definition(op);
    int* status = getLinearList(live, (size_t)def->def_address->ir);
    EnsureNotNull(status);
    if (!(*status)) {
        *status = 1;
        pushFrontDequeList(work_list, getLinearList(ir_2_address, (size_t)def->def_address->ir));
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
        setLinearList(live, (size_t)address->address.ir, address);
        if (*status) {
            pushFrontDequeList(work_list, &address->address);
        }
        next = list_next(next);
    }

    while (!isEmptyDequeList(work_list)) {
        struct AddressSet* address = popFrontDequeList(work_list);

#define READ_OP(num) __read_op(address->address.ir->op##num, address, work_list, live, ir_2_address)
#define WRITE_OP(num)
        IR_OP_READ_WRITE(address->address.ir->type, READ_OP, WRITE_OP, break;);
#undef READ_OP
#undef WRITE_OP

        if (address->address.ir->type == PHI) {
            list_entry_t* phi_head = address->address.ir->op1->operand.v.phi_op_list;
            list_entry_t* phi_next = list_next(phi_head);
            while (phi_next != phi_head) {
                Phi* phi = le2struct(phi_next, Phi, op_link);
                __read_op(phi->value, address, work_list, live, ir_2_address);
                phi_next = list_next(phi_next);
            }
        }

        list_entry_t* rdf_head = &(((BasicBlock*)getLinearList(block_2_rcfg_block, (size_t)address->address.block))->dominant_frontier->block_link);
        list_entry_t* rdf_next = list_next(rdf_head);
        while (rdf_head != rdf_next) {
            BASIC_BLOCK_TYPE* b = le2BasicBlock(rdf_next);
            IR_TYPE* last_ir = le2struct(b->ir_list->ir_link.prev, IR_TYPE, ir_link);
            if (last_ir->type == BRANCH) {
                __read_op(last_ir->op1, address, work_list, live, ir_2_address);
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
            delete_ir(address->address.ir);
        }
        next = list_next(next);
    }
}