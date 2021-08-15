#include <stdio.h>
#include <stdlib.h>

#include "../utils/DequeList.h"
#include "../utils/IrType.h"
#include "../utils/LinkedTable.h"
#include "./ssa.h"
#include "traverse.h"

static int n_for_fast_dom;
static BasicBlock* n0;

void depth_first_search_dom(BasicBlock* block) {
    block->sdno = ++n_for_fast_dom;
    block->ancestor = block->child = n0;
    block->size = 1;
    block->label = block;
    block->index = block->sdno;

    list_entry_t* block_head = &block->successors->block_link;
    list_entry_t* block_elem = list_next(block_head);
    while (block_head != block_elem) {
        BasicBlock* suc = le2BasicBlock(block_elem)->value;
        if (!suc->sdno) {
            suc->parent = block;
            depth_first_search_dom(suc);
        }
        block_elem = list_next(block_elem);
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
    if (block->ancestor == n0)
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

void __init_index_to_block(BasicBlock* block, void* args) {
    BasicBlock** index_to_block = (BasicBlock**)args;
    index_to_block[block->index] = block;
}

void __delete_old_data(BasicBlock* block, void* args) {
    block->index = 0;
    block->sdno = 0;
    block->depth_num = 0;
    block->size = 0;
    block->parent = NULL;
    block->idom = NULL;
    block->label = NULL;
    block->child = NULL;
    block->ancestor = NULL;

    if (block->bucket != NULL) {
        freeBitMap(block->bucket);
        block->bucket = NULL;
    }
}

void __immediate_dominance(struct FuncTabElem* func) {
    BasicBlock* start = func->blocks;
    int max_index;
    n_for_fast_dom = 0;

    deepTraverseSuccessorsBasicBlock(start, __delete_old_data, NULL);

    n0->size = 0;
    n0->sdno = 0;
    n0->label = n0;
    n0->ancestor = n0;

    depth_first_search_dom(start);
    max_index = n_for_fast_dom + 1;

    if (n0->bucket != NULL) {
        free(n0->bucket);
    }
    n0->bucket = newBitMap(max_index);

    if (func->index_to_block != NULL) {
        free(func->index_to_block);
    }
    MALLOC_WITHOUT_DECLARE(func->index_to_block, void*, max_index);
    deepTraverseSuccessorsBasicBlock(start, __init_index_to_block, func->index_to_block);
    BasicBlock** index_to_block = (BasicBlock**)func->index_to_block;

    index_to_block[0] = n0;
    for (int i = 1; i < max_index; i++) {
        BasicBlock* block = index_to_block[i];
        block->bucket = newBitMap(max_index);
    }

    for (int i = n_for_fast_dom; i >= 2; i--) {
        BasicBlock* w = index_to_block[i];

        list_entry_t* head = &w->predecessors->block_link;
        list_entry_t* elem = list_next(head);
        while (head != elem) {
            BasicBlock* v = le2BasicBlock(elem)->value;
            BasicBlock* u = Eval(v);
            if (u->sdno < w->sdno) {
                w->sdno = u->sdno;
            }
            elem = list_next(elem);
        }
        setBitMap(index_to_block[w->sdno]->bucket, w->index);
        Link(w->parent, w);

        struct BitMap* bucket = w->parent->bucket;
        int index;
        if (getBitMap(bucket, 0)) {
            index = 0;
        } else {
            index = getNextSetBitMap(bucket, 0);
        }
        while (index != -1) {
            BasicBlock* v = index_to_block[index];
            clearBitMap(bucket, index);
            BasicBlock* u = Eval(v);
            if (u->sdno < v->sdno) {
                v->idom = u;
            } else {
                v->idom = w->parent;
            }
            index = getNextSetBitMap(bucket, index);
        }
    }

    for (int i = 2; i <= n_for_fast_dom; i++) {
        BasicBlock* w = index_to_block[i];
        if (w->idom != index_to_block[w->sdno]) {
            w->idom = w->idom->idom;
        }
    }

    for (int i = 2; i <= n_for_fast_dom; i++) {
        BasicBlock* block = index_to_block[i];

        list_entry_t* head = &block->i_dominator->block_link;
        while (!list_empty(head)) {
            list_entry_t* elem = list_next(head);
            list_del(elem);
            free(le2BasicBlock(elem));
        }
        MALLOC(i_node, BasicBlockNode, 1);
        i_node->value = block->idom;
        list_add(head, &i_node->block_link);

        head = &block->Children->block_link;
        while (!list_empty(head)) {
            list_entry_t* elem = list_next(head);
            list_del(elem);
            free(le2BasicBlock(elem));
        }
    }

    for (int i = 2; i <= n_for_fast_dom; i++) {
        BasicBlock* block = index_to_block[i];

        list_entry_t* head = &block->idom->Children->block_link;

        MALLOC(i_node, BasicBlockNode, 1);
        i_node->value = block;
        list_add(head, &i_node->block_link);
    }
}

void __caculate_dominance(struct FuncTabElem* func) {
    static BasicBlock b;
    n0 = &b;

    __immediate_dominance(func);
    struct DequeList* queue = newDequeList();
    pushFrontDequeList(queue, func->blocks);
    while (!isEmptyDequeList(queue)) {
        BasicBlock* block = popBackDequeList(queue);
        BasicBlock* idom = block->idom;

        list_entry_t* b_dom_head = &block->dominator->block_link;
        while (!list_empty(b_dom_head)) {
            list_entry_t* elem = list_next(b_dom_head);
            list_del(elem);
            free(le2BasicBlock(elem));
        }
        if (idom != NULL) {
            list_entry_t* i_dom_head = &idom->dominator->block_link;
            list_entry_t* i_dom_elem = list_next(i_dom_head);
            while (i_dom_elem != i_dom_head) {
                MALLOC(node, BasicBlockNode, 1);
                node->value = le2BasicBlock(i_dom_elem)->value;
                list_add(b_dom_head, &node->block_link);
                i_dom_elem = list_next(i_dom_elem);
            }
        }
        MALLOC(node, BasicBlockNode, 1);
        node->value = block;
        list_add(b_dom_head, &node->block_link);

        list_entry_t* b_child_head = &block->Children->block_link;
        list_entry_t* b_child_elem = list_next(b_child_head);
        while (b_child_elem != b_child_head) {
            pushFrontDequeList(queue, le2BasicBlock(b_child_elem)->value);
            b_child_elem = list_next(b_child_elem);
        }
    }
}