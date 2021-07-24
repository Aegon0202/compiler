#include "peephole.h"

#include "../ENUM.h"
/*
int __is_const_op(Operand* op, const int n) {
    if (op->type == INT)
        if (op->operand.v.intValue == n)
            return 2;
        else
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
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue + ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_SUB:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue - ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_MUL:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue * ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_DIV:
                    if (ir->op2->operand.v.intValue == 0)
                        break;
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue / ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_MOD:
                    if (ir->op2->operand.v.intValue == 0)
                        break;
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue % ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_AND:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue && ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_OR:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue || ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_NOT:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = !ir->op1->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_EQ:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue == ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_NEQ:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue != ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_LT:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue < ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_LTE:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue <= ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_GT:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue > ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_GTE:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue >= ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
            }
    }
}
/*
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
                        ir->type = K_ADD;
                        ir->op1->operand.v.intValue = 0;
                        ir->op2->operand.v.intValue = 0;
                    }
                    break;
                case K_DIV:
                    if (__is_const_op(ir->op1, 0) != 0)
                        ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue / ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_MOD:
                    if (ir->op2->operand.v.intValue == 0)
                        break;
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue % ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_AND:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue && ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_OR:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = ir->op1->operand.v.intValue || ir->op2->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_NOT:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = !ir->op1->operand.v.intValue;
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_EQ:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue == ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_NEQ:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue != ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_LT:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue < ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_LTE:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue <= ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_GT:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue > ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
                case K_GTE:
                    ir->type = K_ADD;
                    ir->op1->operand.v.intValue = (ir->op1->operand.v.intValue >= ir->op2->operand.v.intValue);
                    ir->op2->operand.v.intValue = 0;
                    break;
            }
    }
}*/