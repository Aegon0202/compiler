#include "./SysY.target.def.h"

static int __reg_id_equal(void* k1, void* k2) {
    return k1 == k2;
}

static int __block_equal(void* k1, void* k2) {
    return k1 == k2;
}

struct FuncActive* newFuncActive(struct FuncTabElem* elem) {
    MALLOC(f_active, struct FuncActive, 1);
    f_active->funcelem = elem;
    f_active->linear_block = newLinearList();
    f_active->block_next_index = 0;
    f_active->reg_active_table = newLinkedTable(__reg_id_equal);
    return f_active;
}

struct BlockLevelActive* newBlockLevelActive(BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(b_active, struct BlockLevelActive, 1);
    b_active->basic_block = basic_block;
    b_active->in_block_reg_active = newLinkedTable(__reg_id_equal);
    b_active->out_block_reg_active = newLinkedTable(__reg_id_equal);
    b_active->block_reg_active = newLinkedTable(__reg_id_equal);
    b_active->total_reg_active = newLinkedTable(__reg_id_equal);
    return b_active;
}

struct IrLevelActive* newIrLevelActive(REG_ID_TYPE* reg) {
    MALLOC(r_active, struct IrLevelActive, 1);
    r_active->block_line_active = newLinkedTable(__reg_id_equal);
}

void deepTraverseSuccessorsBasicBlock(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args) {
    struct LinkedTable* visited = newLinkedTable(__block_equal);
    struct DequeList* queue = newDequeList();
    setLinkedTable(visited, basic_block_head, basic_block_head);
    pushFrontDequeList(queue, basic_block_head);
    while (!isEmptyDequeList(queue)) {
        BasicBlock* block = popFrontDequeList(queue);
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

void widthTraverseSuccessorsBasicBlock(BASIC_BLOCK_TYPE* basic_block_head, void (*func)(BASIC_BLOCK_TYPE*, void*), void* args) {
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

void __generator_block_level(BASIC_BLOCK_TYPE* basic_block, void* func_active) {
    struct FuncActive* f_active = (struct FuncActive*)func_active;
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = head->next;
    struct BlockLevelActive* b_active = newBlockLevelActive(basic_block);
    f_active->linear_block;
    while (head != next) {
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
        switch (ir->type) {
            case NOP:
                break;
            case PARAM:
                break;
            case CALL:
                break;
            case JUMP:
                break;
            case BRANCH:
                break;
            case RETURNSTMT:
                break;
            case LOAD:
                break;
            case STORE:
                break;
            case ASSIGN:
                break;
            case K_NOT:
                break;
            case K_ADD:
                break;
            case K_SUB:
                break;
            case K_MUL:
                break;
            case K_DIV:
                break;
            case K_MOD:
                break;
            case K_AND:
                break;
            case K_OR:
                break;
            case K_EQ:
                break;
            case K_NEQ:
                break;
            case K_LT:
                break;
            case K_LTE:
                break;
            case K_GT:
                break;
            case K_GTE:
                break;
            default:
                PrintErrExit("Not support ir type %s", EnumTypeToString(ir->type));
        }
        next = list_next(next);
    }
}

struct FuncActive* analyzeFuncLevelActive(struct FuncTabElem* vte) {
    struct FuncActive* f_active = newFuncActive(vte);
}