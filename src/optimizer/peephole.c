#include "peephole.h"

#include "../ENUM.h"
#include "local_op.h"

int __is_const_op(Operand* op, const int n) {
    if (op->type == INT)
        if (op->operand.v.intValue == n)
            return 2;
        else
            return 1;
    else if (op->type == FRAMEPOINT || op->type == STACKPOINT)
        return 1;
    return 0;
}

void constFolding(BasicBlock* block, void* not_use) {
    list_entry_t* head = &(block->ir_list->ir_link);
    list_entry_t* t = head;
    while ((t = list_next(t)) != head) {
        Ir* ir = le2struct(t, Ir, ir_link);
        if (__is_const_op(ir->op1, 0) && __is_const_op(ir->op2, 0))
            switch (ir->type) {
                case K_ADD:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue + ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_SUB:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue - ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_MUL:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue * ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_DIV:
                    if (ir->op2->operand.v.intValue == 0)
                        break;
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue / ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_MOD:
                    if (ir->op2->operand.v.intValue == 0)
                        break;
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue % ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_NOT:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = !ir->op1->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_EQ:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue == ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_NEQ:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue != ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_LT:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue < ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_LTE:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue <= ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_GT:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue > ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_GTE:
                    ir->type = ASSIGN;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue >= ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
            }
    }
}

//代数化简
void algebraic_simplification(BasicBlock* block, void* notuse) {
    list_entry_t* head = &(block->ir_list->ir_link);
    list_entry_t* t = head;
    while ((t = list_next(t)) != head) {
        Ir* ir = le2struct(t, Ir, ir_link);
        if (__is_const_op(ir->op1, 0) != 0 || __is_const_op(ir->op2, 0) != 0)
            switch (ir->type) {
                case K_MUL:
                    if (__is_const_op(ir->op1, 0) == 2 || __is_const_op(ir->op2, 0) == 2) {
                        ir->type = ASSIGN;
                        delete_operand(ir->op1);
                        delete_operand(ir->op2);
                        ir->op1 = create_new_operand(INT, -1, 0);
                        ir->op2 = create_new_operand(INT, -1, 0);
                    }
                    if (__is_const_op(ir->op1, 1) == 2) {
                        ir->type = ASSIGN;
                        delete_operand(ir->op1);
                        ir->op1 = ir->op2;
                        ir->op2 = create_new_operand(INT, -1, 0);
                    } else if (__is_const_op(ir->op2, 1) == 2) {
                        ir->type = ASSIGN;
                        delete_operand(ir->op2);
                        ir->op2 = create_new_operand(INT, -1, 0);
                    }
                    break;
                case K_DIV:
                    if (__is_const_op(ir->op2, 0) == 2) break;
                    if (__is_const_op(ir->op1, 0) == 2) {
                        ir->type = ASSIGN;
                        delete_operand(ir->op2);
                    }
                    if (__is_const_op(ir->op2, 1) == 2) {
                        ir->type = ASSIGN;
                    }
                    break;
                case K_MOD:
                    if (__is_const_op(ir->op2, 0) == 2) break;
                    if (__is_const_op(ir->op1, 0) == 2 || __is_const_op(ir->op1, 1) == 2) {
                        ir->type = ASSIGN;
                        delete_operand(ir->op2);
                    }
                    if (__is_const_op(ir->op2, 1) == 2) {
                        ir->type = ASSIGN;
                    }
                    break;
            }
    }
}

int __is_operand_equal(Operand* op, int reg) {
    return op && op->type == REGISTER && op->operand.reg_idx == reg;
}

void getAssignStm_local(BasicBlock* block, struct DequeList* workList) {
    list_entry_t* ir_head = &(block->ir_list->ir_link);
    list_entry_t* ir_elem = list_next(ir_head);
    while (ir_head != ir_elem) {
        Ir* ir_value = le2struct(ir_elem, Ir, ir_link);
        if (ir_value->type == Assign)
            pushBackDequeList(workList, ir_value);

        ir_elem = list_next(ir_elem);
    }
}

void getAssignStm_global(BasicBlock* start, void* args) {
    deepTraverseSuccessorsBasicBlock(start, getAssignStm_local, args);
}
void copy_propgation(BasicBlock* block, BasicBlock* start) {
    struct DequeList* workList = newDequeList();
    getAssignStm_global(start, workList);

    while (!isEmptyDequeList(workList)) {
        Ir* process_elem = popFrontDequeList(workList);
        list_entry_t* ir_head = &(get_op_definition(process_elem->op3)->chain->DU_chain);
        list_entry_t* ir_elem = list_next(ir_head);

        while (ir_elem != ir_head) {
            Ir* ir_value = le2struct(ir_elem, Ir, ir_link);
            Operand* subor = process_elem->op1;
            int reg = process_elem->op3->operand.reg_idx;
            list_entry_t* tmp = list_next(ir_elem);

            if (__is_operand_equal(ir_value->op1, reg)) {
                delete_operand(ir_value->op1);
                ir_value->op1 = create_new_operand(subor->type, subor->operand.reg_idx, subor->operand.v.intValue);
            }
            if (__is_operand_equal(ir_value->op2, reg)) {
                delete_operand(ir_value->op2);
                ir_value->op2 = create_new_operand(subor->type, subor->operand.reg_idx, subor->operand.v.intValue);
            }
            ir_elem = tmp;
        }
    }
}