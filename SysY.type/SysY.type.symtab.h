#ifndef SYSY_TYPE_SYMTAB_H
#define SYSY_TYPE_SYMTAB_H

struct SymTabEntry
{
};

struct HashEntry
{
    struct SymTabEntry *symtabentry;
    struct HashEntry *next;
};

struct HashTable
{
    int hashnum;
    struct HashEntry *head;
};

#endif