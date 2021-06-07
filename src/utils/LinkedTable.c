#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "./LinkedTable.h"

struct LinkedElem* newLinkedElem(void* key, void* value, struct LinkedElem* prev) {
    EnsureNotNull(key);
    EnsureNotNull(value);
    struct LinkedElem* linked_elem = (struct LinkedElem*)malloc(sizeof(struct LinkedElem));
    EnsureNotNull(linked_elem);
    linked_elem->item = (struct Item*)malloc(sizeof(struct Item));
    EnsureNotNull(linked_elem->item);
    linked_elem->item->key = key;
    linked_elem->item->value = value;
    if (prev == NULL) {
        linked_elem->prev = linked_elem;
        linked_elem->next = linked_elem;
    } else {
        linked_elem->prev = prev;
        linked_elem->next = prev->next;
        prev->next->prev = linked_elem;
        prev->next = linked_elem;
    }
    return linked_elem;
}

void removeLinkedELem(struct LinkedElem* target) {
    EnsureNotNull(target);
    target->prev->next = target->next;
    target->next->prev = target->prev;
    free(target);
}

struct LinkedElem* findLinkedElem(struct LinkedElem* head, void* key, int (*key_equal)(void*, void*)) {
    EnsureNotNull(head);
    EnsureNotNull(key);
    EnsureNotNull(key_equal);
    struct LinkedElem* le = head->next;
    while (le != head) {
        if (key_equal(le->item->key, key)) {
            return le;
        }
        le = le->next;
    }
    return NULL;
}

struct LinkedTable* newLinkedTable(int (*key_equal)(void*, void*)) {
    struct LinkedTable* linked_table = (struct LinkedTable*)malloc(sizeof(struct LinkedTable));
    EnsureNotNull(linked_table);
    linked_table->head = newLinkedElem(malloc(sizeof(int)), malloc(sizeof(int)), NULL);
    linked_table->size = 0;
    linked_table->key_equal = key_equal;
    return linked_table;
}

// return old valie point or null
void* setLinkedTable(struct LinkedTable* linked_table, void* key, void* value) {
    EnsureNotNull(linked_table);
    EnsureNotNull(key);
    EnsureNotNull(value);
    void* old = NULL;
    if (linked_table->head == NULL) {
        linked_table->head = newLinkedElem(key, value, NULL);
        linked_table->size++;
        return old;
    }
    struct LinkedElem* le = findLinkedElem(linked_table->head, key, linked_table->key_equal);
    if (le != NULL) {
        old = le->item->value;
        le->item->value = value;
        return old;
    }
    le = newLinkedElem(key, value, linked_table->head);
    linked_table->size++;
    return old;
}

// return value point or NULL(not exist)
void* getLinkedTable(struct LinkedTable* linked_table, void* key) {
    EnsureNotNull(linked_table);
    EnsureNotNull(key);

    IfNull(linked_table->head, return NULL;);
    struct LinkedElem* le = findLinkedElem(linked_table->head, key, linked_table->key_equal);
    IfNull(le, return NULL;);

    return le->item->value;
}

// return Item point or NULL(not exist)
// Item point need free if not used
struct Item* removeLinkedTable(struct LinkedTable* linked_table, void* key) {
    EnsureNotNull(linked_table);
    EnsureNotNull(key);

    IfNull(linked_table->head, return NULL;);
    struct LinkedElem* le = findLinkedElem(linked_table->head, key, linked_table->key_equal);
    IfNull(le, return NULL;);

    struct Item* item = le->item;
    removeLinkedELem(le);
    linked_table->size--;
    return item;
}

// remove the last? Item in table
// return NULL if contain none item
struct Item* popLinkedTable(struct LinkedTable* linked_table) {
    EnsureNotNull(linked_table);
    if (linked_table->head->prev == linked_table->head) {
        return NULL;
    }
    struct Item* item = linked_table->head->prev->item;
    removeLinkedELem(linked_table->head->prev);
    linked_table->size--;
    return item;
}

// free the linked table
// 0: success, -1: this still elem in linked table
int freeLinkedTable(struct LinkedTable** linked_table_p) {
    EnsureNotNull(linked_table_p);
    struct LinkedTable* linked_table = *linked_table_p;
    IfNull(linked_table, return 0;);
    if (linked_table->head->prev != linked_table->head) {
        return -1;
    }
    free(linked_table->head->item->key);
    free(linked_table->head->item->value);
    free(linked_table->head->item);
    free(linked_table->head);
    free(linked_table);
    *linked_table_p = NULL;
    return 0;
}