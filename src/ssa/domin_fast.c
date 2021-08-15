#include <stdio.h>
#include <stdlib.h>

#include "../utils/DequeList.h"
#include "../utils/IrType.h"
#include "../utils/LinkedTable.h"
#include "./ssa.h"
#include "traverse.h"

extern int n_for_fast_dom;
extern BasicBlock* n0;

void depth_first_search_dom(BasicBlock* block) {
    block->sdno = ++n_for_fast_dom;
    block->ancestor = block->child = n0;
    block->size = 1;

    list_entry_t* block_head = &block->successors->block_link;
    list_entry_t* block_elem = list_next(block_head);
    while (block_head != block_elem) {
        BasicBlock* suc = le2BasicBlock(block_elem)->value;
        if (!suc->sdno) {
            suc->parent = block;
            depth_first_search_dom(suc);
        }
    }
}

void compress(BasicBlock* block) {
    if (block->ancestor->ancestor != n0) {
        compress(block->ancestor);
        if (block->ancestor->label->sdno < block->label->sdno)
            block->label = block->ancestor->label;
        block->ancestor = block->ancestor->ancestor;
    }
}

BasicBlock* Eval(BasicBlock* block) {
    if (block->ancestor = n0)
        return block->label;
    else {
        compress(block);
        if (block->ancestor->label->sdno >= block->label->sdno)
            return block->label;
        else
            return block->ancestor->label;
    }
}

void Link(BasicBlock* v, BasicBlock* w) {
    BasicBlock* s = w;
    BasicBlock* tmp;

    while (w->label->sdno < s->child->label->sdno) {
        if (s->size + s->child->child->size >= 2 * s->child->size) {
            s->child->ancestor = s;
            s->child = s->child->child;
        } else {
            s->child->size = s->size;
            s->ancestor = s->child;
            s = s->ancestor;
        }
    }
    s->label = w->label;
    v->size += w->size;
    if (v->size < 2 * w->size) {
        tmp = s;
        s = v->child;
        v->child = tmp;
    }
    while (s != n0) {
        s->ancestor = v;
        s = s->child;
    }
}