#ifndef SYSY_SYMTAB_DEF_H
#define SYSY_SYMTAB_DEF_H
#include "../ENUM.h"
// #include "../ssa/ssa.h"
#include "../utils/DequeList.h"
#include "../utils/LinkedTable.h"

struct VarTabElem {
    char* name;
    int level;
    int type;    // K_INT STRING ARRAY
    int offset;  // 变量在内存中相对于fp的偏移
    int size;    // 变量的大小
    int is_const;
    int is_array;
    struct ArrayTabElem* array_ref;  // 当type为ARRAY时有效
    int* const_init_value;
    struct VarTabElem* link;  // 上一个引用
};

struct ArrayTabElem {
    int elem_type;                  // K_INT ARRAY
    struct ArrayTabElem* elem_ref;  // 当 elem_type 为 ARRAY 时 有效
    int elem_num;                   // 元素个数
    int elem_size_offset;           // 元素个数在内存中相对于fp的偏移
    int elem_size;                  // 元素大小
    int size;                       // 数组总大小
};

struct FuncTabElem {
    char* name;
    int return_type;                    // K_INT K_VOID
    int parameters_num;                 // 参数个数
    struct VarTabElem* parameters_ref;  // 参数的最后一个引用
    int parameters_size;                // 参数的大小
    int var_offset_end;                 // 变量在栈中用到的最后的偏移
    int has_side_effect;                // 是否有副作用，每次同样的参数会不会有不同的影响
#ifdef BASIC_BLOCK_TYPE
    BASIC_BLOCK_TYPE* blocks;
    BASIC_BLOCK_TYPE* rcfg_blocks;
#else
    void* blocks;
    void* rcfg_blocks;
#endif
};

struct BlockTabElem {
    struct VarTabElem* last;    // 当前块变量上一个的引用
    int size;                   // 当前块变量的总大小
    struct BlockTabElem* link;  // 上层块
};

struct VarTable {
    struct LinearList* table;
    int next;
};

struct BlockTable {
    struct LinearList* table;
    int next;
};

struct Display {
    struct LinearList* table;
    int next;
};

struct ArrayTable {
    struct LinearList* table;
    int next;
};

struct FuncTable {
    struct LinkedTable* table;
    struct LinearList* all_funcs;
    int next_func_index;
};

extern struct VarTable* var_table;
extern struct BlockTable* block_table;
extern struct Display* display;
extern struct ArrayTable* array_table;
extern struct FuncTable* func_table;
const char* getUniquieName();
const char* getUniquieNameStaic();
void init_all_table();
int appendVarTable(struct VarTabElem* elem, struct VarTable* table);
int appendArrayTable(struct ArrayTabElem* elem, struct ArrayTable* table);
int appendBlockTable(struct BlockTabElem* elem, struct BlockTable* table);
int appendDisplay(struct BlockTabElem* elem, struct Display* table);
void addFuncTable(struct FuncTabElem* elem, struct FuncTable* table);
void removeLastDisplay(struct Display* display);
struct BlockTabElem* getLastDisplay(struct Display* display);
struct FuncTabElem* getFuncTabElemByName(const char* name, struct FuncTable* table);
struct VarTabElem* getVarTabElemByName(const char* name, struct Display* display);
struct VarTabElem* getVarTabElemByIndex(int index, struct VarTable* table);
struct ArrayTabElem* getArrayTabElemByIndex(int index, struct ArrayTable* table);
struct BlockTabElem* getBlockTabElemByIndex(int index, struct BlockTable* table);
struct FuncTabElem* newFuncTabElem(const char* name, struct FuncTable* table);
struct VarTabElem* newVarTabElem(const char* name, struct VarTable* table);
struct BlockTabElem* newBlockTabElem(struct BlockTabElem* link, struct BlockTable* table);
struct ArrayTabElem* newArrayTabElem(struct ArrayTable* table);
void add_library_funcs(struct FuncTable* func_table);

#endif