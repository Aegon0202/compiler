#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SysY.type.symtab.h"
#include "SysY.type.def.h"
#include "SysY.type.visitor.h"

struct FuncSymTable *funcsymtable_p;
struct VarSymTable *varsymtable_active_p, *varsymtable_deactive_p;

// need free
char *getUniquieName()
{
    static int index = 0;
    char *r = (char *)malloc(64);
    EnsureNotNull(r);
    snprintf(r, 63, "uniquie#%d", index);
    return r;
}

// new a FuncSymEntry and add to list end
struct FuncSymEntry *newFuncSymEntry(int type, const char *name, struct FuncSymEntry *head)
{
    struct FuncSymEntry *fse = (struct FuncSymEntry *)malloc(sizeof(struct FuncSymEntry));
    EnsureNotNull(fse);
    fse->type = type;
    IfNullElse(name, fse->name = getUniquieName();, fse->name = strdup(name););
    fse->funcparam_head = newVarSymEntry(VARSYMENTRY, "%INVALIDNAME", -1, NULL);
    if (head != NULL)
    {
        fse->prev = head->prev;
        head->prev->next = fse;
        fse->next = head;
        head->prev = fse;
    }
    else
    {
        fse->prev = fse;
        fse->next = fse;
    }
    return fse;
}

// new a VarSymEntry and add to list end
struct VarSymEntry *newVarSymEntry(int type, const char *name, int level, struct VarSymEntry *head)
{
    struct VarSymEntry *vse = (struct VarSymEntry *)malloc(sizeof(struct VarSymEntry));
    EnsureNotNull(vse);
    vse->type = type;
    IfNullElse(name, vse->name = getUniquieName();, vse->name = strdup(name););
    vse->level = level;
    if (head != NULL)
    {
        vse->prev = head->prev;
        head->prev->next = vse;
        vse->next = head;
        head->prev = vse;
    }
    else
    {
        vse->prev = vse;
        vse->next = vse;
    }
    return vse;
}

void initSymTable()
{
    funcsymtable_p = (struct FuncSymTable *)malloc(sizeof(struct FuncSymTable));
    funcsymtable_p->type = FUNCSYMTABLE;
    funcsymtable_p->num = 0;
    funcsymtable_p->head = newFuncSymEntry(FUNCSYMENTRY, "%%INVALIDENTRY", NULL);

    varsymtable_active_p = (struct VarSymTable *)malloc(sizeof(struct VarSymTable));
    varsymtable_active_p->type = VARSYMTABLE;
    varsymtable_active_p->num = 0;
    varsymtable_active_p->head = newVarSymEntry(VARSYMENTRY, "%%INVALIDNAME", -1, NULL);

    varsymtable_deactive_p = (struct VarSymTable *)malloc(sizeof(struct VarSymTable));
    varsymtable_deactive_p->type = VARSYMTABLE;
    varsymtable_deactive_p->num = 0;
    varsymtable_deactive_p->head = newVarSymEntry(VARSYMENTRY, "%%INVALIDNAME", -1, NULL);
}

struct FuncSymEntry *findFuncInTable(const char *name)
{
    struct FuncSymEntry *fse = funcsymtable_p->head->prev;
    while (fse != funcsymtable_p->head)
    {
        if (strcmp(fse->name, name) == 0)
        {
            return fse;
        }
        fse = fse->prev;
    }
    return NULL;
}

struct VarSymEntry *findVarInTable(const char *name)
{
    struct VarSymEntry *vse = varsymtable_active_p->head->prev;
    while (vse != varsymtable_active_p->head)
    {
        if (strcmp(vse->name, name) == 0)
        {
            return vse;
        }
        vse = vse->prev;
    }
    return NULL;
}

void removeVarFromSymTable(int level)
{
    struct VarSymEntry *head = varsymtable_active_p->head;
    struct VarSymEntry *dhead = varsymtable_deactive_p->head;
    struct VarSymEntry *vse = head->prev;

    while (vse != head && vse->level == level)
    {
        head->prev = vse->prev;
        head->prev->next = head;

        vse->prev = dhead->prev;
        vse->next = head;
        head->prev->next = vse;
        head->prev = vse;
    }
}
