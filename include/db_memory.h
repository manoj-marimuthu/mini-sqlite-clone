#ifndef DB_MEMORY_H
#define DB_MEMORY_H
#include <stdlib.h>

typedef struct DbMemNode{
    void* ptr;
    struct DbMemNode* next;
} DbMemNode;

// global allocations storage
DbMemNode* DbAllocations;

// Db memory APIs

DbMemNode* createMemNode();
void reallocMemNode(DbMemNode* node,size_t new_size);
void clean_up();

#endif