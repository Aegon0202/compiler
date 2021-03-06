#ifndef __LIBS_LIST_H__
#define __LIBS_LIST_H__

#define offset(type, member) ((size_t) & (((type *)(void *)0)->member))

/* to_struct - get the struct from a ptr
 * @ptr:    a struct pointer of member
 * @type:   the type of the struct this is embedded in
 * @member: the name of the member within the struct
 * */
#define to_struct(ptr, type, member) \
    ((type *)((char *)(ptr)-offset(type, member)))
/* *
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when manipulating
 * whole lists rather than single entries, as sometimes we already know
 * the next/prev entries and we can generate better code by using them
 * directly rather than using the generic single-entry routines.
 * */
#define le2struct(le, type, member) \
    to_struct((le), type, member)

#define le2BasicBlock(elem) le2struct(elem, BasicBlockNode, block_link)

struct list_entry {
    struct list_entry *prev, *next;
};

typedef struct list_entry list_entry_t;

void list_init(list_entry_t *elm);
void list_add(list_entry_t *listelm, list_entry_t *elm);
void list_add_before(list_entry_t *listelm, list_entry_t *elm);
void list_add_after(list_entry_t *listelm, list_entry_t *elm);
void list_del(list_entry_t *listelm);
void list_del_init(list_entry_t *listelm);
int list_empty(list_entry_t *list);
list_entry_t *list_next(list_entry_t *listelm);
list_entry_t *list_prev(list_entry_t *listelm);

void __list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next);
void __list_del(list_entry_t *prev, list_entry_t *next);

#endif /* !__LIBS_LIST_H__ */
