#include "peephole.h"

int constFolding(BasicBlock* block) {
    Ir* head = block->ir_list;
    le2struct(list_next(&(head->ir_link)), Ir, ir_link);
    switch
}
