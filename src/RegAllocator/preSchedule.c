
#include "LRA.h"
static int maxpriority = 0;

struct DependencyNode {
    int type;
    void* ir_index;
};

struct propertyNode {
    int priority;
    int pressure;
    int maxDepth;
    int near;
    int org_id;
    int is_schedule;
};

struct LinearList* dependency_suc;  //用linearList实现邻接表,linearList每个元素是一个dequeList
struct LinearList* dependency_pred;
struct LinearList* propertyList;

struct propertyNode* create_new_prop_node() {
    MALLOC(node, struct propertyNode, 1);
    return node;
}

int __is_store(struct ArmIr* ir) {
    switch (ir->id) {
        case ARM_STR_I:
        case ARM_STR_I_PRE:
        case ARM_STR_I_POST:
        case ARM_STR_R:
        case ARM_STR_R_PRE:
        case ARM_STR_R_POST:
            return 1;
        default:
            return 0;
    }
}
int __is_load(struct ArmIr* ir) {
    switch (ir->id) {
        case ARM_LDR_I:
        case ARM_LDR_I_PRE:
        case ARM_LDR_I_POST:
        case ARM_LDR_R:
        case ARM_LDR_R_PRE:
        case ARM_LDR_R_POST:
            return 1;
        default:
            return 0;
    }
}

int __is_cmp(struct ArmIr* ir) {
    return ir->type == ARM_CMP || ir->type == ARM_CMN;
}

int __is_conditional_ir(struct ArmIr* ir) {
    return ir->cond->type != AL;
}

void __get_sux(struct ArmIr* ir, struct DequeList* list) {
    struct DequeList* sux = getLinearList(dependency_suc, ir);
    for (int i = 0; i < sizeDequeList(sux); i++) {
        struct DependencyNode* node = getDequeList(sux, i);
        pushBackDequeList(list, node->ir_index);
    }
}

void __get_pred(struct ArmIr* ir, struct DequeList* list) {
    struct DequeList* pred = getLinearList(dependency_pred, ir);
    for (int i = 0; i < sizeDequeList(pred); i++) {
        struct DependencyNode* node = getDequeList(pred, i);
        pushBackDequeList(list, node->ir_index);
    }
}

int __get_dependent_type(struct ArmIr* pre, struct ArmIr* suc) {
    struct DequeList* sux = getLinearList(dependency_suc, pre);
    for (int i = 0; i < sizeDequeList(sux); i++) {
        struct DependencyNode* node = getDequeList(sux, i);
        if (node->ir_index == suc) return node->type;
    }
    PrintErrExit(" ");
}

void __init_dependenc_graph(BlockBegin* block) {
    list_entry_t* head = getIrListFromBlock(block);
    list_entry_t* elem = list_next(head);
    while (head != elem) {
        struct ArmIr* ir_value = le2struct(elem, struct ArmIr, ir_link);
        struct DequeList* dl1 = newDequeList();
        struct DequeList* dl2 = newDequeList();
        setLinearList(dependency_suc, ir_value, dl1);
        setLinearList(dependency_pred, ir_value, dl2);
        elem = list_next(elem);
    }
}

void __is_used_read_op(struct Register* op, struct Register* target, int* res) {
    if (op->type != REGISTER) return;
    if (op->reg == target->reg) *res = (*res) & 0x0;
}

void __is_used_read_op2(struct Operand2* op, struct Register* target, int* res) {
    if (op->type == REGISTER) __is_used_read_op(op->Rm.reg, target, res);
    if (op->shift_type == REGISTER) __is_used_read_op(op->shift.reg, target, res);
}

void __is_last_used(BlockBegin* block, struct ArmIr* ir, struct Register* target, int* res) {
    list_entry_t* ir_head = getIrListFromBlock(block);
    list_entry_t* ir_elem = list_next(&ir->ir_link);
    int index1 = target->reg / 64;
    int index2 = target->reg - index1 * 64;
    struct DequeList* block_out = getBlock_out(block);
    long long int* j = getDequeList(block_out, index1);
    long long int i = *j & (0x1 << index2);
    if (i != 0) return 0;
    while (ir_elem != ir_head) {
        struct ArmIr* ir_value = le2struct(ir_elem, struct ArmIr, ir_link);

//对这条ir，遍历它的读操作数
#define READ_OP(op) __is_used_read_op(op, target, res)
#define WRITE_OP(op)
#define READ_OP2(op) __is_used_read_op2(op, target, res)
        ARM_IR_OP_READ_WRITE(ir_value, READ_OP, READ_OP2, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
#undef READ_OP2
        ir_elem = list_next(ir_elem);
    }
}

void __compute_pressure_read_op(BlockBegin* block, struct ArmIr* ir, struct Register* target, int* pressure) {
    if (target->type != REGISTER) return;
    int res = 1;
    __is_last_used(block, ir, target, res);
    if (res) *pressure = *pressure - 1;
}

void __compute_pressure_read_op2(BlockBegin* block, struct ArmIr* ir, struct Operand2* op, int* pressure) {
    if (op->type == REGISTER) __compute_pressure_read_op(block, ir, op->Rm.reg, pressure);
    if (op->shift_type == REGISTER) __compute_pressure_read_op(block, ir, op->shift.reg, pressure);
}

void __compute_pressure_write_op(struct Register* op, int* pressure) {
    if (op->type != REGISTER) PrintErrExit(" ");
    *pressure = *pressure + 1;
}

int __compute_pressure(BlockBegin* block, struct ArmIr* ir) {
    int pressure = 0;
#define READ_OP(op) __compute_pressure_read_op(block, ir, op, &pressure)
#define WRITE_OP(op) __compute_pressure_write_op(op, &pressure);
#define READ_OP2(op) __compute_pressure_read_op2(block, ir, op, &pressure)
    ARM_IR_OP_READ_WRITE(ir, READ_OP, READ_OP2, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
#undef READ_OP2
    return pressure;
}

void connectNode(struct ArmIr* pre, struct ArmIr* suc, int type) {
    MALLOC(node1, struct DependencyNode, 1);
    node1->type = type;
    node1->ir_index = suc;
    MALLOC(node2, struct DependencyNode, 1);
    node2->type = type;
    node2->ir_index = pre;
    struct DequeList* dl = getLinearList(dependency_suc, pre);
    pushBackDequeList(dl, node1);
    struct DequeList* dl2 = getLinearList(dependency_pred, suc);
    pushBackDequeList(dl2, node2);
}

void __dependencer_write_op(struct Register* op_back, struct Register* op_front, struct ArmIr* dependencer, struct ArmIr* dependency) {
    if (op_back->type != REGISTER) PrintErrExit(" ");

    if (op_back->reg == op_front->reg) {
        connectNode(dependency, dependencer, RAW);
    }
}

void __dependencer_read_op(struct Register* op_back, struct Register* op_front, struct ArmIr* dependencer, struct ArmIr* dependency) {
    if (op_back->type != REGISTER) return;
    if (op_back->reg == op_front->reg) {
        connectNode(dependency, dependencer, WAR);
    }
}

void __dependencer_read_op2(struct Operand2* op_back, struct Register* op_front, struct ArmIr* dependencer, struct ArmIr* dependency) {
    if (op_back->shift_op != REGISTER) return;
    __dependencer_read_op(op_back->shift.reg, op_front, dependencer, dependency);
    if (op_back->type != REGISTER) return;
    __dependencer_read_op(op_back->Rm.reg, op_front, dependencer, dependency);
}

//
void __DG_read_op(struct ArmIr* ir_value, struct Register* op_front, list_entry_t* head, list_entry_t* begin) {
    if (op_front->type != REGISTER) return;

    list_entry_t* elem = begin;
    while (elem != head) {
        struct ArmIr* value = le2struct(elem, struct ArmIr, ir_link);
#define READ_OP(op)
#define WRITE_OP(op) __dependencer_write_op(op, op_front, value, ir_value);
#define READ_OP2(op)
        ARM_IR_OP_READ_WRITE(value, READ_OP, READ_OP2, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
#undef READ_OP2
        elem = list_next(elem);
    }
}

void __DG_read_op2(struct ArmIr* ir_value, struct Operand2* op_front, list_entry_t* head, list_entry_t* begin) {
    if (op_front->type != REGISTER) return;
    __DG_read_op(ir_value, op_front->Rm.reg, head, begin);
    if (op_front->shift_op != REGISTER) return;
    __DG_read_op(ir_value, op_front->shift.reg, head, begin);
}

void __DG_write_op(struct ArmIr* ir_value, struct Register* op_front, list_entry_t* head, list_entry_t* begin) {
    if (op_front->type != REGISTER) PrintErrExit(" ");
    list_entry_t* elem = begin;
    while (elem != head) {
        struct ArmIr* value = le2struct(elem, struct ArmIr, ir_link);
#define READ_OP(op) __dependencer_read_op(op, op_front, value, ir_value);
#define WRITE_OP(op)
#define READ_OP2(op) __dependencer_read_op2(op, op_front, value, ir_value);
        ARM_IR_OP_READ_WRITE(value, READ_OP, READ_OP2, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
#undef READ_OP2
        elem = list_next(elem);
    }
}

//------O(n^2)的复杂度，对每条指令，找出依赖它的所有指令，需要遍历这条指令之后的所有指令
//------对这里出现的代码，在每个基本块内，不存在被定义超过一次的变量，
void dependency_analysis_block(BlockBegin* block, void* args) {
    list_entry_t* ir_head = getIrListFromBlock(block);
    list_entry_t* ir_elem = list_next(ir_head);
    list_entry_t* ir_elem_tmp;
    while (ir_head != ir_elem) {
        struct ArmIr* ir_value = le2struct(ir_elem, struct ArmIr, ir_link);
        if (ir_value->type == ARM_LABEL) continue;

            //计算ir_value的普通依赖者
#define READ_OP(op) __DG_read_op(ir_value, op, ir_head, ir_elem)    //对读操作数，有读后写冲突
#define WRITE_OP(op) __DG_write_op(ir_value, op, ir_head, ir_elem)  //对写操作数，有写后读冲突
#define READ_OP2(op) __DG_read_op2(ir_value, op, ir_head, ir_elem)
        ARM_IR_OP_READ_WRITE(ir_value, READ_OP, READ_OP2, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
#undef READ_OP2

        //------对访存指令
        switch (ir_value->type) {
            case ARM_LDR_I:
            case ARM_LDR_I_PRE:
            case ARM_LDR_I_POST:
            case ARM_LDR_R:
            case ARM_LDR_R_PRE:
            case ARM_LDR_R_POST:
                ir_elem_tmp = list_next(ir_elem);
                while (ir_elem_tmp != ir_head) {
                    struct ArmIr* ir_value_tmp = le2struct(ir_elem_tmp, struct ArmIr, ir_link);
                    if (__is_store(ir_value_tmp)) {
                        connectNode(ir_value, ir_value_tmp, LAS);
                    }
                    ir_elem_tmp = list_next(ir_elem_tmp);
                }
                break;
            case ARM_STR_I:
            case ARM_STR_I_PRE:
            case ARM_STR_I_POST:
            case ARM_STR_R:
            case ARM_STR_R_PRE:
            case ARM_STR_R_POST:
                ir_elem_tmp = list_next(ir_elem);
                while (ir_elem_tmp != ir_head) {
                    struct ArmIr* ir_value_tmp = le2struct(ir_elem_tmp, struct ArmIr, ir_link);

                    if (__is_store(ir_value_tmp)) {
                        connectNode(ir_value, ir_value_tmp, SAS);
                    }
                    if (__is_load(ir_value_tmp)) {
                        connectNode(ir_value, ir_value_tmp, SAL);
                    }
                    ir_elem_tmp = list_next(ir_elem_tmp);
                }
                break;
            case ARM_CMP:
                ir_elem_tmp = list_next(ir_elem);
                while (ir_elem_tmp != ir_head) {
                    struct ArmIr* ir_value_tmp = le2struct(ir_elem_tmp, struct ArmIr, ir_link);

                    if (__is_cmp(ir_value_tmp)) {
                        connectNode(ir_value, ir_value_tmp, WAW);
                    } else if (__is_conditional_ir(ir_value_tmp)) {
                        connectNode(ir_value, ir_value_tmp, WAR);
                    }
                    ir_elem_tmp = list_next(ir_elem_tmp);
                }
            default:
                break;
        }
        if (__is_conditional_ir(ir_value)) {
            ir_elem_tmp = list_next(ir_elem);
            while (ir_elem_tmp != ir_head) {
                struct ArmIr* ir_value_tmp = le2struct(ir_elem_tmp, struct ArmIr, ir_link);
                if (__is_cmp(ir_value_tmp)) {
                    connectNode(ir_value, ir_value_tmp, RAW);
                }
                ir_elem_tmp = list_next(ir_elem_tmp);
            }
        }
        ir_elem = list_next(ir_elem);
    }
}

void __update_priority(struct ArmIr* ir_node) {
    struct DequeList* worklist = newDequeList();
    struct DequeList* tmp = newDequeList();
    __set_priority(ir_node, maxpriority);
    maxpriority++;
    pushBackDequeList(worklist, ir_node);
    while (!isEmptyDequeList(worklist)) {
        struct ArmIr* node = popFrontDequeList(worklist);
        __get_pred(node, tmp);
        while (!isEmptyDequeList(tmp)) {
            struct ArmIr* pred_node = popFrontDequeList(tmp);
            if (__get_priority(pred_node) != maxpriority && !__is_free2schedule(pred_node)) {
                __set_priority(pred_node, maxpriority);
                pushBackDequeList(worklist, pred_node);
            }
        }
    }
    freeDequeList(worklist);
    freeDequeList(tmp);
}

void __compute_near(struct ArmIr* ir) {
    struct DequeList* sux = newDequeList();
    __get_sux(ir, sux);
    while (!isEmptyDequeList(sux)) {
        struct ArmIr* suc = popBackDequeList(sux);
        if (__get_dependent_type(ir, suc) == RAW) {
            struct propertyNode* suc_prop = getLinearList(propertyList, suc);
            struct propertyNode* pre_prop = getLinearList(propertyList, ir);
            suc_prop->near = suc_prop->near > (pre_prop->near + 1) ? suc_prop->near : pre_prop->near + 1;
        }
    }
    freeDequeList(sux);
}

int __compute_maxdepth(struct ArmIr* ir) {
    struct propertyNode* property = getLinearList(propertyList, ir);
    if (property->maxDepth != -1) return property->maxDepth;

    struct DequeList* sux = newDequeList();
    __get_sux(ir, sux);
    int maxdepth = -1;
    while (!isEmptyDequeList(sux)) {
        struct ArmIr* suc = popBackDequeList(sux);
        int tmp = __compute_maxdepth(suc);
        maxdepth = maxdepth > tmp ? maxdepth : tmp;
    }
    freeDequeList(sux);
    return maxdepth;
}

void __init_property(BlockBegin* block) {
    propertyList = newLinearList();
    list_entry_t* head = getIrListFromBlock(block);
    list_entry_t* elem = list_next(head);
    while (head != elem) {
        struct ArmIr* value = le2struct(elem, struct ArmIr, ir_link);
        struct propertyNode* property = create_new_prop_node();
        property->org_id = value->id;
        property->near = 0;
        property->maxDepth = -1;
        property->pressure = 0;
        property->pressure = __compute_pressure(block, value);
        property->is_schedule = 0;
        property->priority = 0;

        setLinearList(propertyList, value, property);
        elem = list_next(elem);
    }
    elem = list_next(head);
    while (head != elem) {
        struct ArmIr* value = le2struct(elem, struct ArmIr, ir_link);
        __compute_near(value);
        __compute_maxdepth(value);
        elem = list_next(elem);
    }
}

void __init_readyList(struct DequeList* readyList, list_entry_t* head) {
    list_entry_t* elem = list_next(head);
    while (elem != head) {
        struct ArmIr* ir_value = le2struct(elem, struct ArmIr, ir_link);
        struct DequeList* preds = getLinearList(dependency_pred, ir_value);
        if (isEmptyDequeList(preds)) pushBackDequeList(readyList, ir_value);
        elem = list_next(elem);
    }
}

struct ArmIr* selectNode(struct DequeList* readyList) {
    struct ArmIr* ir_max = getDequeList(readyList, 0);
    for (int i = 1; i < sizeDequeList; i++) {
        struct ArmIr* ir = getDequeList(readyList, i);
        struct propertyNode* max_property = getLinearList(propertyList, ir_max);
        struct propertyNode* property = getLinearList(propertyList, ir);
        if (max_property->priority > property->priority) continue;
        if (max_property->priority < property->priority) {
            ir_max = ir;
            continue;
        }

        if (max_property->pressure < property->priority) continue;
        if (max_property->pressure < property->priority) {
            ir_max = ir;
            continue;
        }

        if (max_property->near + max_property->maxDepth > property->near + property->maxDepth) continue;
        if (max_property->near + max_property->maxDepth > property->near + property->maxDepth) {
            ir_max = ir;
            continue;
        }

        if (max_property->near > property->near) continue;
        if (max_property->near > property->near) {
            ir_max = ir;
            continue;
        }

        if (max_property->org_id < property->org_id) ir_max = ir;
    }
    return ir_max;
}

int __is_free2schedule(struct ArmIr* ir) {
    struct DequeList* pred = newDequeList();
    __get_pred(ir, pred);
    while (!isEmptyDequeList(pred)) {
        struct ArmIr* ir_value = popBackDequeList(pred);
        struct propertyNode* prop = getLinearList(propertyList, ir_value);
        if (!prop->is_schedule) return 0;
    }
    freeDequeList(pred);
    return 1;
}

struct DequeList* preSchedule_local(BlockBegin* block) {
    __init_denpendency_graph(block);
    dependency_analysis_block(block, NULL);
    __init_property(block);
    struct DequeList* tmp_deque = newDequeList();
    struct DequeList* readyList = newDequeList();
    struct DequeList* schedule_list = newDequeList();
    list_entry_t* head = getIrListFromBlock(block);
    __init_readyList(readyList, head);
    while (isEmptyDequeList(readyList)) {
        struct ArmIr* ir_node = selectNode(readyList);
        if (!__is_free2schedule(ir_node)) {
            __update_priority(ir_node);
            continue;
        }
        //调度
        pushBackDequeList(schedule_list, ir_node);
        struct propertyNode* prop = getLinearList(propertyList, ir_node);
        prop->is_schedule = 1;
        __get_sux(ir_node, tmp_deque);
        while (!isEmptyDequeList(tmp_deque)) {
            struct ArmIr* suc = popBackDequeList(tmp_deque);
            if (__is_free2schedule(suc) || __get_dependent_type(ir_node, suc) == RAW) {
                pushBackDequeList(readyList, suc);
            }
        }
    }
    freeDequeList(readyList);
    freeDequeList(tmp_deque);
    return schedule_list;
}

void __construct_new_ir_list(struct DequeList* schedule, list_entry_t* ir_head) {
    while (!isEmptyDequeList(schedule)) {
        struct ArmIr* ir = popFrontDequeList(schedule);
        list_entry_t* ir_elem = &ir->ir_link;
        list_del(ir_elem);
        list_add_before(ir_head, ir_elem);
    }
    freeDequeList(schedule);
}

void __reorder_opid(BlockBegin* block) {
    int cur = block->first_op_id;
    list_entry_t* ir_head = getIrListFromBlock(block);
    list_entry_t* ir_elem = list_next(ir_head);
    while (ir_head != ir_elem) {
        struct ArmIr* ir_value = le2struct(ir_elem, struct ArmIr, ir_link);
        ir_value->id = cur;
        cur += 2;
        ir_elem = list_next(ir_elem);
    }
}

void schedule_block(BlockBegin* block, void* args) {
    struct DequeList* schedule_order = preSchedule_local(block);
    __construct_new_ir_list(schedule_order, getIrListFromBlock(block));
    __reorder_opid(block);
}

void schedule_global(struct DequeList* blocks) {
    gothrough_BlockBeginNode_list(blocks, schedule_block, NULL);
}