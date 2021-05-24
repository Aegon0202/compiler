#ifndef SYSY_TYPE_SYMTAB_H
#define SYSY_TYPE_SYMTAB_H

enum
{
    VARSYMENTRY = 600,
    FUNCSYMENTRY,
    FUNCSYMTABLE,
    VARSYMTABLE
};
struct ExpAST;

struct VarSymEntry
{
    int type; // VARSYMENTRY
    char *name;
    int offset;
    int size;                    // Variable size
    int level;                   // global is zero; prefer to use on high level
    int typevalue;               // K_INT STRING(const char *)
    int is_array;                // 1 or 0
    int is_const;                // 1 or 0
    int array_dimensional_num;   // if not is array, this is 0
    struct ExpAST **array_shape; // if not is array, this is NULL
    int *initval;                // if not have init value, this is NULL. Otherwise, the occupied space is the same as the variable
    struct VarSymEntry *prev;
    struct VarSymEntry *next;
};

struct FuncSymEntry
{
    int type; // FUNCSYMENTRY
    char *name;
    int offset;
    int returntype; // K_VOID K_INT
    int has_effect; // 1 or 0
    int funcparamnum;
    struct VarSymEntry *funcparam_head;
    struct FuncSymEntry *prev;
    struct FuncSymEntry *next;
};

struct FuncSymTable
{
    int type; // FUNCSYMTABLE
    int num;
    struct FuncSymEntry *head;
};

struct VarSymTable
{
    int type; // VARSYMTABLE
    int num;
    struct VarSymEntry *head;
};

void initSymTable();
struct VarSymEntry *newVarSymEntry(int type, const char *name, int level, struct VarSymEntry *head);
struct FuncSymEntry *newFuncSymEntry(int type, const char *name, struct FuncSymEntry *head);
struct FuncSymEntry *findFuncInTable(const char *name);
struct VarSymEntry *findVarInTable(const char *name);
void removeVarFromSymTable(int level);
char *getUniquieName();

extern struct FuncSymTable *funcsymtable_p;
extern struct VarSymTable *varsymtable_active_p, *varsymtable_deactive_p;

#endif