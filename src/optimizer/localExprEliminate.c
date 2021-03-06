#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "../utils/DequeList.h"
#include "../utils/LinkedTable.h"
#include "./local_op.h"

struct AEB {
    IR_TYPE* ir;
    OPERAND_TYPE* tmp;
};

int __is_op_equal(OPERAND_TYPE* op1, OPERAND_TYPE* op2) {
    if (op1->type != op2->type) {
        return 0;
    }
    switch (op1->type) {
        case INT:
        case FRAMEPOINT:
        case GLOBALDATA:
            return op1->operand.v.intValue == op2->operand.v.intValue;
        case REGISTER:
            return op1->operand.reg_idx == op2->operand.reg_idx;
        default:
            return 0;
    }
}

int __is_ir_equal(IR_TYPE* ir_1, IR_TYPE* ir_2) {
    if (ir_1->type != ir_2->type) {
        return 0;
    }
    switch (ir_1->type) {
        case K_NOT:
            return __is_op_equal(ir_1->op1, ir_2->op1);
        case K_ADD:
        case K_MUL:
        case K_EQ:
        case K_NEQ:
            return (__is_op_equal(ir_1->op1, ir_2->op1) && __is_op_equal(ir_1->op2, ir_2->op2)) || (__is_op_equal(ir_1->op1, ir_2->op2) && __is_op_equal(ir_1->op2, ir_2->op1));
        case K_SUB:
        case K_DIV:
        case K_MOD:
        case K_LT:
        case K_LTE:
        case K_GT:
        case K_GTE:
            return (__is_op_equal(ir_1->op1, ir_2->op1) && __is_op_equal(ir_1->op2, ir_2->op2));
        default:
            return 0;
    }
}

int __is_ir_equal_void(void* ir1, void* ir2) {
    return __is_ir_equal((IR_TYPE*)ir1, (IR_TYPE*)ir2);
}

int __is_can_expr_eliminate(IR_TYPE* ir) {
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
            return 1;
        default:
            return 0;
    }
}

struct AEB* newAEB(IR_TYPE* ir, OPERAND_TYPE* tmp) {
    MALLOC(aeb, struct AEB, 1);
    aeb->ir = ir;
    aeb->tmp = tmp;
    return aeb;
}

void __local_expr_eliminate(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct LinkedTable* table = newLinkedTable(__is_ir_equal_void);
    list_entry_t* head = &basic_block->ir_list->ir_link;
    list_entry_t* next = list_next(head);
    while (next != head) {
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
        if (__is_can_expr_eliminate(ir)) {
            struct AEB* aeb = getLinkedTable(table, ir);
            if (aeb == NULL) {
                aeb = newAEB(ir, NULL);
                setLinkedTable(table, ir, aeb);
            } else {
                printf("local expr eliminate\n");
                if (aeb->tmp == NULL) {
                    OPERAND_TYPE* tmp = toSSATempVariable(basic_block);
                    OPERAND_TYPE* n_op1 = create_new_operand(ir->op1->type, ir->op1->operand.reg_idx, ir->op1->operand.v.intValue);
                    OPERAND_TYPE* n_op2 = create_new_operand(ir->op2->type, ir->op2->operand.reg_idx, ir->op2->operand.v.intValue);
                    aeb->tmp = tmp;
                    IR_TYPE* n_ir = create_new_ir(ir->type, n_op1, n_op2, tmp, basic_block);
                    list_add_before(&(aeb->ir->ir_link), &(n_ir->ir_link));
                    create_new_definition(tmp->operand.reg_idx, n_ir, basic_block);
                    add_user(n_op1, n_ir);
                    add_user(n_op2, n_ir);
                    delete_user(aeb->ir->op1, aeb->ir);
                    delete_user(aeb->ir->op2, aeb->ir);
                    IfNotNull(aeb->ir->op1, { delete_operand(aeb->ir->op1); });
                    IfNotNull(aeb->ir->op2, { delete_operand(aeb->ir->op2); });

                    tmp = create_new_operand(tmp->type, tmp->operand.reg_idx, tmp->operand.v.intValue);
                    aeb->ir->type = ASSIGN;
                    aeb->ir->op1 = tmp;
                    aeb->ir->op2 = NULL;
                    add_user(tmp, aeb->ir);
                }

                delete_user(ir->op2, ir);
                delete_user(ir->op1, ir);
                IfNotNull(ir->op2, { delete_operand(ir->op2); });
                IfNotNull(ir->op1, { delete_operand(ir->op1); });

                ir->type = ASSIGN;
                ir->op1 = create_new_operand(aeb->tmp->type, aeb->tmp->operand.reg_idx, aeb->tmp->operand.v.intValue);
                ir->op2 = NULL;
                add_user(aeb->tmp, ir);
            }
        }
        next = list_next(next);
    }
    struct Item* item = NULL;
    while ((item = popLinkedTable(table)) != NULL) {
        free(item->value);
        free(item);
    }
    freeLinkedTable(&table);
}

void localExprEliminate(struct FuncTabElem* elem) {
    deepTraverseSuccessorsBasicBlock(elem->blocks, __local_expr_eliminate, NULL);
}
