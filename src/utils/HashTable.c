#include "./HashTable.h"

struct HashTable* newHashTable(int capacity, int threshold, int (*hash_key)(void* key), int (*key_equal)(void* key_1, void* key_2)) {
    EnsureNotNull(key_equal);
    EnsureNotNull(hash_key);
    struct HashTable* hash_table = (struct HashTable*)malloc(sizeof(struct HashTable));
    EnsureNotNull(hash_table);
    if (capacity <= 0) {
        capacity = 20;
    }
    hash_table->capacity = capacity;
    if (threshold <= 1) {
        threshold = 20;
    }
    hash_table->threshold = threshold;
    hash_table->size = 0;
    hash_table->key_equal = key_equal;
    hash_table->hash_key = hash_key;
    hash_table->table = (struct LinkedTable**)malloc(sizeof(struct LinkedTable*) * hash_table->capacity);
    EnsureNotNull(hash_table->table);
    for (int i = 0; i < capacity; i++) {
        hash_table->table[i] = newLinkedTable(key_equal);
    }
    return hash_table;
}

void resizeHashTable(struct HashTable* hash_table, int capacity) {
    EnsureNotNull(hash_table);
    if (capacity <= 0) {
        capacity = __INT_MAX__;
    }
    struct LinkedTable** table = (struct LinkedTable**)malloc(sizeof(struct LinkedTable*) * capacity);
    for (int i = 0; i < capacity; i++) {
        table[i] = newLinkedTable(hash_table->key_equal);
    }
    struct Item* item;
    for (int i = 0; i < hash_table->capacity; i++) {
        while ((item = popLinkedTable(hash_table->table[i])) != NULL) {
            setLinkedTable(table[hash_table->hash_key(item->key) % capacity], item->key, item->value);
            free(item);
        }
        freeLinkedTable(&hash_table->table[i]);
    }
    free(hash_table->table);
    hash_table->table = table;
    hash_table->capacity = capacity;
}

// return old valie point or null
void* setHashTable(struct HashTable* hash_table, void* key, void* value) {
    EnsureNotNull(hash_table);
    EnsureNotNull(key);
    EnsureNotNull(value);
    void* old = setLinkedTable(hash_table->table[hash_table->hash_key(key) % hash_table->capacity], key, value);
    if (old == NULL) {
        hash_table->size++;
    }
    if (hash_table->size > hash_table->capacity * hash_table->threshold) {
        resizeHashTable(hash_table, hash_table->capacity * 2);
    }
    return old;
}

// return value point or NULL(not exist)
void* getHashTable(struct HashTable* hash_table, void* key) {
    EnsureNotNull(hash_table);
    EnsureNotNull(key);
    return getLinkedTable(hash_table->table[hash_table->hash_key(key) % hash_table->capacity], key);
}

// return Item point or NULL(not exist)
// Item point need free if not used
struct Item* removeHashTable(struct HashTable* hash_table, void* key) {
    EnsureNotNull(hash_table);
    EnsureNotNull(key);
    struct Item* item = removeLinkedTable(hash_table->table[hash_table->hash_key(key) % hash_table->capacity], key);
    if (item != NULL) {
        hash_table->size--;
    }
    return item;
}

// free the linked table
// 0: success, -1: this still elem in linked table
int freeHashTable(struct HashTable** hash_table_p) {
    EnsureNotNull(hash_table_p);
    struct HashTable* hash_table = *hash_table_p;
    IfNull(hash_table, return 0;);
    if (hash_table->size > 0) {
        return -1;
    }
    for (int i = 0; i < hash_table->capacity; i++) {
        int r = freeLinkedTable(&hash_table->table[i]);
        if (r == -1) {
            PrintErrExit("HASH TABLE NOT EMPTY. UNKNOWN ERROR HAPPEN");
        }
    }
    free(hash_table->table);
    free(hash_table);
    *hash_table_p = NULL;
    return 0;
}