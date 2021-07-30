#include "peephole.h"

#include "../ENUM.h"
#include "local_op.h"
struct LinearList* constMark;
struct LinearList* constValue;

struct DequeList* prop_worklist;
struct DequeList* simp_worklist;

extern int reg_begin;
extern int current_size;
int __is_ir_value_const(IR_TYPE* ir, int* value);

int __is_op3_writable(Ir* ir) {
    switch (ir->type) {
        case K_NOT:
        case ASSIGN:
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
        case PHI:
        case CALL:
        case LOAD:
            return 1;
            break;
    }
    return 0;
}

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

int caculate_const_value(Ir* ir) {
    int value;
    int num1, num2;
    num1 = *(int*)getLinearList(constValue, ir->op1->operand.reg_idx);
    num2 = *(int*)getLinearList(constValue, ir->op2->operand.reg_idx);
    switch (ir->type) {
        case K_NOT:
            value = !num1;
            break;
        case ASSIGN:
            value = num1;
            break;
        case K_ADD:
            value = num1 + num2;
            break;
        case K_MUL:
            value = num1 * num2;
            break;
        case K_EQ:
            value = (num1 == num2);
            break;
        case K_NEQ:
            value = (num1 != num2);
            break;
        case K_SUB:
            value = (num1 - num2);
            break;
        case K_DIV:
            value = (num1 / num2);
            break;
        case K_MOD:
            value = (num1 % num2);
            break;
        case K_LT:
            value = (num1 < num2);
            break;
        case K_LTE:
            value = (num1 <= num2);
            break;
        case K_GT:
            value = (num1 > num2);
            break;
        case K_GTE:
            value = (num1 >= num2);
            break;
    }
    return value;
}

int __is_op_value_const(OPERAND_TYPE* op) {
    int is_op_const = 0;
    if (op->type != REGISTER) {
        is_op_const = 1;
    }

    int* mark = getLinearList(constMark, op->operand.reg_idx);
    if (!mark) {
        struct Definition* def = get_op_definition(op);
        int value;
        is_op_const = __is_ir_value_const(def->def_address->ir, &value);
        MALLOC(i, int, 1);
        *i = is_op_const;
        setLinearList(constMark, op->operand.reg_idx, i);
        if (is_op_const)
            pushFrontDequeList(prop_worklist, op);
        return is_op_const;
    } else if (*mark == 1) {
        return 1;
    } else if (*mark == 0) {
        return 0;
    }
    PrintErrExit("error");
    return -1;
}

int __is_ir_value_const(IR_TYPE* ir, int* value) {
    int is_const = 0;
    switch (ir->type) {
        case K_NOT:
        case ASSIGN:
            is_const = __is_op_value_const(ir->op1);
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
            is_const = __is_op_value_const(ir->op1) && __is_op_value_const(ir->op2);
            break;
        default:
            is_const = 0;
            break;
    }
    if (is_const) {
        MALLOC(value, int, 1);
        *value = caculate_const_value(ir);
        setLinearList(constValue, ir->op3->operand.reg_idx, value);
        return 1;
    }
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
                        delete_user(ir->op1, ir);
                        delete_user(ir->op2, ir);
                        delete_operand(ir->op1);
                        delete_operand(ir->op2);
                        ir->op1 = create_new_operand(INT, -1, 0);
                        ir->op2 = create_new_operand(INT, -1, 0);
                    }
                    if (__is_const_op(ir->op1, 1) == 2) {
                        ir->type = ASSIGN;
                        delete_user(ir->op1, ir);
                        delete_operand(ir->op1);
                        ir->op1 = ir->op2;
                        ir->op2 = create_new_operand(INT, -1, 0);
                    } else if (__is_const_op(ir->op2, 1) == 2) {
                        ir->type = ASSIGN;
                        delete_user(ir->op1, ir);
                        delete_operand(ir->op2);
                        ir->op2 = create_new_operand(INT, -1, 0);
                    }
                    break;
                case K_DIV:
                    if (__is_const_op(ir->op2, 0) == 2) break;
                    if (__is_const_op(ir->op1, 0) == 2) {
                        ir->type = ASSIGN;
                        delete_user(ir->op2, ir);
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
                        delete_user(ir->op2, ir);
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

void getAssignStm_local(BasicBlock* block, void* args) {
    struct DequeList* workList = (struct DequeList*)args;
    list_entry_t* ir_head = &(block->ir_list->ir_link);
    list_entry_t* ir_elem = list_next(ir_head);
    while (ir_head != ir_elem) {
        Ir* ir_value = le2struct(ir_elem, Ir, ir_link);
        if (ir_value->type == ASSIGN)
            pushBackDequeList(workList, ir_value);

        ir_elem = list_next(ir_elem);
    }
}

void getAssignStm_global(BasicBlock* start, void* args) {
    deepTraverseSuccessorsBasicBlock(start, getAssignStm_local, args);
}

void copy_propgation(BasicBlock* start) {
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
            ir_elem = list_next(ir_elem);

            if (ir_value->type != PHI) {
                if (__is_operand_equal(ir_value->op1, reg)) {
                    delete_user(ir_value->op1, ir_value);
                    delete_operand(ir_value->op1);
                    ir_value->op1 = create_new_operand(subor->type, subor->operand.reg_idx, subor->operand.v.intValue);
                    add_user(ir_value->op1, ir_value);
                }
                if (__is_operand_equal(ir_value->op2, reg)) {
                    delete_user(ir_value->op2, ir_value);
                    delete_operand(ir_value->op2);
                    ir_value->op2 = create_new_operand(subor->type, subor->operand.reg_idx, subor->operand.v.intValue);
                    add_user(ir_value->op2, ir_value);
                }
            } else {
                Operand* op = search_op_in_phi_list(ir_value, reg);
                int op_reg = op->operand.reg_idx;
                delete_user(op, ir_value);
                delete_operand(op);
                if (subor->type == INT) {
                    op = create_new_operand(subor->type, op_reg, subor->operand.v.intValue);
                } else {
                    op = create_new_operand(subor->type, subor->operand.reg_idx, subor->operand.v.intValue);
                    add_user(op, ir_value);
                }
            }
        }
    }
}

void const_propgation(Operand* op) {
    Ir* ir = get_op_definition(op)->def_address->ir;
    list_entry_t* du_head = &(get_op_definition(op)->chain->DU_chain);
    list_entry_t* du_elem = list_next(du_head);
    while (du_head != du_elem) {
        Ir* ir_value = le2struct(du_elem, def_use_chain, DU_chain)->user;
        int const_value = *(int*)(getLinearList(constValue, op->operand.reg_idx));
        du_elem = list_next(du_elem);
        if (ir_value->type != PHI) {
            if (__is_operand_equal(ir_value->op1, op->operand.reg_idx)) {
                delete_operand(ir_value->op1);
                ir_value->op1 = create_new_operand(INT, -1, const_value);
            }
            if (__is_operand_equal(ir_value->op2, op->operand.reg_idx)) {
                delete_operand(ir_value->op2);
                ir_value->op2 = create_new_operand(INT, -1, const_value);
            }
        } else {
            Operand* phi_op = search_op_in_phi_list(ir_value, op->operand.reg_idx);
            int op_reg = phi_op->operand.reg_idx;
            delete_operand(phi_op);
            phi_op = create_new_operand(INT, op_reg, const_value);
        }
    }
    delete_ir(ir, ir->block);
}

void __mark_const(BasicBlock* block, void* args) {
    list_entry_t* ir_head = &(block->ir_list->ir_link);
    list_entry_t* ir_elem = list_next(ir_head);

    while (ir_head != ir_elem) {
        Ir* ir_value = le2struct(ir_elem, Ir, ir_link);
        if (__is_op3_writable(ir_value))
            __is_op_value_const(ir_value->op3);
    }
}

void alSimplifyAndConstProp(BasicBlock* start) {
    simp_worklist = newDequeList();
    prop_worklist = newDequeList();
    constMark = newLinearList();
    constValue = newLinearList();
    deepTraverseSuccessorsBasicBlock(start, algebraic_simplification, NULL);
    deepTraverseSuccessorsBasicBlock(start, __mark_const, NULL);

    while (!isEmptyDequeList(prop_worklist)) {
        Operand* op = popBackDequeList(prop_worklist);
        const_propgation(op);
    }
    deepTraverseSuccessorsBasicBlock(start, algebraic_simplification, NULL);
    copy_propgation(start);
}
