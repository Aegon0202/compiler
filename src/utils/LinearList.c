#include "./LinearList.h"

#include "./Malloc.h"
#include "./NullPointMacro.h"
#define TOTAL_LEVEL 32
typedef unsigned long long int ull;
static ull container_size[TOTAL_LEVEL];

struct LinearListElem* newLinearListElem(int rest_level) {
    MALLOC(elem, struct LinearListElem, 1);
    elem->rest_level = rest_level;
    elem->content = NULL;
    return elem;
}

void* getLinearListElem(struct LinearListElem* elems, ull index) {
    IfNull(elems, return NULL;);
    IfNull(elems->content, return NULL;);
    if (index >= container_size[elems->rest_level]) {
        return NULL;
    }
    if (elems->rest_level == 0) {
        return elems->item->value;
    }
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        if (index < container_size[elems->rest_level - 1]) {
            return getLinearListElem(elems->content[i], index);
        }
        index -= container_size[elems->rest_level - 1];
    }
    return NULL;
}

void* setLinearListElem(struct LinearListElem* elems, ull index, void* value) {
    EnsureNotNull(elems);
    EnsureNotNull(value);
    if (index >= container_size[elems->rest_level]) {
        PrintErrExit("Index Out Of Range.");
    }
    if (elems->rest_level == 0) {
        void* r_value = NULL;

        IfNullElse(
            elems->item,
            { MALLOC_WITHOUT_DECLARE(elems->item, struct Item, 1);MALLOC_WITHOUT_DECLARE(elems->item->key,ull,1); },
            { r_value = elems->item->value; });

        *(ull*)(elems->item->key) = index;
        elems->item->value = value;
        return r_value;
    }
    IfNull(elems->content, { MALLOC_WITHOUT_DECLARE(elems->content, struct LinearListElem*, TOTAL_LEVEL); });
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        if (index < container_size[elems->rest_level - 1]) {
            IfNull(elems->content[i], elems->content[i] = newLinearListElem(elems->rest_level - 1););
            return setLinearListElem(elems->content[i], index, value);
        }
        index -= container_size[elems->rest_level - 1];
    }
    PrintErrExit("Unknown Error Happen");
    return NULL;
}

struct Item* popLinearListElem(struct LinearListElem* elems) {
    IfNull(elems, return NULL;);
    IfNull(elems->content, return NULL);
    if (elems->rest_level == 0) {
        void* value = elems->item;
        elems->item = NULL;
        return value;
    }
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        void* r_value = popLinearListElem(elems->content[i]);
        if (r_value != NULL) {
            return r_value;
        }
        free(elems->content[i]);
        elems->content[i] = NULL;
    }
    free(elems->content);
    elems->content = NULL;
    return NULL;
}

void* removeLinearListElem(struct LinearListElem* elems, unsigned long long int index) {
    IfNull(elems, return NULL;);
    IfNull(elems->content, return NULL;);
    if (index >= container_size[elems->rest_level]) {
        return NULL;
    }
    if (elems->rest_level == 0) {
        void* value = elems->item->value;
        free(elems->item->key);
        free(elems->item);
        elems->item = NULL;
        return value;
    }
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        if (index < container_size[elems->rest_level - 1]) {
            return removeLinearListElem(elems->content[i], index);
        }
        index -= container_size[elems->rest_level - 1];
    }
    return NULL;
}

struct LinearList* newLinearList() {
    static int need_init_flag = 1;
    if (need_init_flag) {
        need_init_flag = 0;
        ull total = 1;
        for (int i = 0; i < TOTAL_LEVEL; i++) {
            container_size[i] = total;
            total *= TOTAL_LEVEL;
        }
    }
    MALLOC(linear, struct LinearList, 1);
    MALLOC_WITHOUT_DECLARE(linear->content, struct LinearListElem*, TOTAL_LEVEL);
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        linear->content[i] = newLinearListElem(i);
    }
    return linear;
}

void* getLinearList(struct LinearList* linear, ull index) {
    IfNull(linear, return NULL;);
    IfNull(linear->content, return NULL;);
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        if (index < container_size[i]) {
            return getLinearListElem(linear->content[i], index);
        }
        index -= container_size[i];
    }
    return NULL;
}

void* setLinearList(struct LinearList* linear, ull index, void* value) {
    EnsureNotNull(linear);
    EnsureNotNull(linear->content);
    EnsureNotNull(value);
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        if (index < container_size[i]) {
            return setLinearListElem(linear->content[i], index, value);
        }
        index -= container_size[i];
    }
    PrintErrExit("Unknown Error In Here.");
    return NULL;
}

void* popLinearList(struct LinearList* linear) {
    EnsureNotNull(linear);
    EnsureNotNull(linear->content);
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        struct Item* item = popLinearListElem(linear->content[i]);
        if (item != NULL) {
            void* value = item->value;
            free(item->key);
            free(item);
            return value;
        }
    }
    return NULL;
}

int freeLinearList(struct LinearList** linear_p) {
    IfNull(linear_p, return 0;);
    IfNull(*linear_p, { *linear_p = NULL; return 0; });
    struct LinearList* linear = *linear_p;
    IfNull(linear->content, { free(linear->content); free(linear); *linear_p=NULL; return 0; });
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        struct Item* item = popLinearListElem(linear->content[i]);
        if (item != NULL) {
            setLinearList(linear, *(ull*)(item->key), item->value);
            free(item->key);
            free(item);
            return -1;
        }
    };
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        free(linear->content[i]);
    }
    free(linear->content);
    free(linear);
    *linear_p = NULL;
    return 0;
}

void* removeLinearList(struct LinearList* linear, unsigned long long int index) {
    EnsureNotNull(linear);
    EnsureNotNull(linear->content);
    for (int i = 0; i < TOTAL_LEVEL; i++) {
        if (index < container_size[i]) {
            return removeLinearListElem(linear->content[i], index);
        }
        index -= container_size[i];
    }
    return NULL;
}

// from begin_index +1 to end_index
struct LinearList* mapLinearList(struct LinearList* linear, ull begin_index, ull end_index, void* (*map_func)(void* value), int is_local) {
    struct LinearList* n_list = NULL;
    if (is_local) {
        n_list = linear;
    } else {
        n_list = newLinearList();
    }
    for (ull i = begin_index; i != end_index; i++) {
        setLinearList(n_list, i, map_func(getLinearList(linear, i)));
    }
    return n_list;
}

void* reduceLinearList(struct LinearList* linear, ull begin_index, ull end_index, void* (*reduce_func)(void* total, void* value), void* init_value) {
    void* value = init_value;
    for (ull i = begin_index; i != end_index; i++) {
        value = reduce_func(value, getLinearList(linear, i));
    }
    return value;
}