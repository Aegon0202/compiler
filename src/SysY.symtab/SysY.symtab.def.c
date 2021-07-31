#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "SysY.symtab.def.h"

#include "../SysY.type/SysY.type.def.h"
#include "../utils/Malloc.h"
#include "../utils/link.h"
struct VarTable* var_table;
struct BlockTable* block_table;
struct Display* display;
struct ArrayTable* array_table;
struct FuncTable* func_table;

// need free
const char* getUniquieName() {
    static long long int index = 0;
    MALLOC(r, char, 64);
    EnsureNotNull(r);
    memset(r, 0, 64);
    snprintf(r, 63, "uniquie#%lld", index);
    index++;
    return r;
}

const char* getUniquieNameStaic() {
    static long long int index = 0;
    static char r[64];
    //EnsureNotNull(r);
    memset(r, 0, 64);
    snprintf(r, 63, "uniquie#s#%lld", index);
    index++;
    return r;
}

void init_all_table() {
#define table_init(table_name, table_type)             \
    MALLOC_WITHOUT_DECLARE(table_name, table_type, 1); \
    table_name->table = newLinearList();               \
    table_name->next = 0;

    table_init(var_table, struct VarTable);
    table_init(block_table, struct BlockTable);
    table_init(display, struct Display);
    table_init(array_table, struct ArrayTable);
    MALLOC_WITHOUT_DECLARE(func_table, struct FuncTable, 1);
    func_table->table = newLinkedTable(string_equal);
    func_table->all_funcs = newLinearList();
    func_table->next_func_index = 0;

#undef table_init
    appendDisplay(newBlockTabElem(NULL, block_table), display);

    add_library_funcs(func_table);
}

#define append_table_generaotr(elem_type, table_type)                          \
    int append##table_type(struct elem_type* elem, struct table_type* table) { \
        void* value = setLinearList(table->table, table->next, elem);          \
        IfNotNull(value, PrintErrExit("the value must be null"););             \
        int n = table->next;                                                   \
        table->next++;                                                         \
        return n;                                                              \
    }

append_table_generaotr(VarTabElem, VarTable);
append_table_generaotr(ArrayTabElem, ArrayTable);
append_table_generaotr(BlockTabElem, BlockTable);
append_table_generaotr(BlockTabElem, Display);

void addFuncTable(struct FuncTabElem* elem, struct FuncTable* table) {
    void* value = setLinkedTable(table->table, elem->name, elem);
    IfNotNull(value, PrintErrExit("redefine function %s", elem->name));
}

#undef append_table_generaotr

void removeLastDisplay(struct Display* display) {
    EnsureNotNull(display);
    if (display->next <= 0) {
        PrintErrExit("Only Support pop Display Not Empty");
    }
    removeLinearList(display->table, display->next - 1);
    display->next--;
}

struct BlockTabElem* getLastDisplay(struct Display* display) {
    EnsureNotNull(display);
    if (display->next <= 0) {
        PrintErrExit("SomeThing Error Happen");
    }
    return (struct BlockTabElem*)getLinearList(display->table, display->next - 1);
}

struct FuncTabElem* getFuncTabElemByName(const char* name, struct FuncTable* table) {
    void* value = getLinkedTable(table->table, (void*)name);
    IfNull(value, PrintErrExit("not defined function %s", name));
    return value;
}

struct VarTabElem* getVarTabElemByName(const char* name, struct Display* display) {
    struct BlockTabElem* block = getLastDisplay(display);
    while (block != NULL) {
        struct VarTabElem* elem = block->last;
        while (elem != NULL) {
            if (string_equal((void*)name, (void*)elem->name)) {
                return elem;
            }
            elem = elem->link;
        }
        block = block->link;
    }
    PrintErrExit("not defined variable %s", name);
    return NULL;
}

#define get_by_index_generator(elem_type, table_type)                                \
    struct elem_type* get##elem_type##ByIndex(int index, struct table_type* table) { \
        return (struct elem_type*)getLinearList(table->table, index);                \
    }

get_by_index_generator(VarTabElem, VarTable);
get_by_index_generator(ArrayTabElem, ArrayTable);
get_by_index_generator(BlockTabElem, BlockTable);
#undef get_by_index_generator

struct FuncTabElem* newFuncTabElem(const char* name, struct FuncTable* table) {
    MALLOC(elem, struct FuncTabElem, 1);
    elem->name = strdup(name);
    elem->blocks = NULL;
    elem->parameters_num = 0;
    elem->parameters_ref = 0;
    elem->parameters_size = 0;
    elem->return_type = K_VOID;
    elem->has_side_effect = 1;
    elem->rcfg_blocks = NULL;
    IfNotNull(table, {
        addFuncTable(elem, table);
        setLinearList(table->all_funcs, table->next_func_index, elem);
        table->next_func_index++;
    });
    return elem;
}

struct VarTabElem* newVarTabElem(const char* name, struct VarTable* table) {
    MALLOC(elem, struct VarTabElem, 1);
    elem->name = strdup(name);
    elem->level = -1;
    elem->type = K_VOID;
    elem->offset = 0;
    elem->size = 0;
    elem->is_array = 0;
    elem->is_const = 0;
    elem->array_ref = NULL;
    elem->link = NULL;
    elem->const_init_value = NULL;
    IfNotNull(table, appendVarTable(elem, table););
    return elem;
}

struct BlockTabElem* newBlockTabElem(struct BlockTabElem* link, struct BlockTable* table) {
    MALLOC(elem, struct BlockTabElem, 1);
    elem->last = NULL;
    elem->size = 0;
    elem->link = link;
    IfNotNull(table, appendBlockTable(elem, table););
    return elem;
}

struct ArrayTabElem* newArrayTabElem(struct ArrayTable* table) {
    MALLOC(elem, struct ArrayTabElem, 1);
    elem->elem_type = K_INT;
    elem->elem_num = 0;
    elem->elem_size_offset = 0;
    elem->elem_ref = NULL;
    elem->elem_size = 0;
    elem->size = 0;
    IfNotNull(table, appendArrayTable(elem, table););
    return elem;
}

void add_library_funcs(struct FuncTable* func_table) {
    struct FuncTabElem* func;
    func = newFuncTabElem("getint", func_table);
    func->return_type = K_INT;

    func = newFuncTabElem("getch", func_table);
    func->return_type = K_INT;

    func = newFuncTabElem("getarray", func_table);
    func->return_type = K_INT;

    func = newFuncTabElem("putint", func_table);

    func = newFuncTabElem("putch", func_table);

    func = newFuncTabElem("putarray", func_table);

    func = newFuncTabElem("putf", func_table);

    func = newFuncTabElem("_sysy_starttime", func_table);

    func = newFuncTabElem("_sysy_stoptime", func_table);
}