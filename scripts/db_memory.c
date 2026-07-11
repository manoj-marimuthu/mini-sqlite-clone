#include <stdlib.h>
#include <db_memory.h>
#include <db_error.h>

DbMemNode* DbAllocations = NULL;

DbMemNode* createMemNode(size_t n){
    DbMemNode* node = malloc(sizeof(DbMemNode));
    if(node == NULL){
        log_error("Memory allocation failed",COMPILE_TIME_ERROR);
    }
    node->next = NULL;
    node->ptr = malloc(n);
    if(node->ptr == NULL){
        log_error("Memory allocation failed",COMPILE_TIME_ERROR);
    }
    // add allocation to global allocations list
    if(DbAllocations == NULL){
        DbAllocations = node;
    }else{
        DbAllocations->next = node;
    }
    return node;
}

void reallocMemNode(DbMemNode* node,size_t new_size){
    if(node == NULL){
        log_error("Cannot reallocate a DbMemNode* of NULL",COMPILE_TIME_ERROR);
    }
    void* ptr = node->ptr;
    if(ptr == NULL){
        log_error("Cannot reallocate a ptr of NULL in DbMemNode",COMPILE_TIME_ERROR);
    }
    ptr = realloc(ptr,new_size);
    if(ptr == NULL){
        log_error("Memory allocation failed",COMPILE_TIME_ERROR);
    }
    free(node->ptr);
    node->ptr = ptr;
}

void clean_up(){
    DbMemNode* currentDbMemNode = DbAllocations;
    DbMemNode* temp = NULL;
    while(currentDbMemNode != NULL){
        temp = currentDbMemNode->next;
        free(currentDbMemNode->ptr);
        free(currentDbMemNode);
        currentDbMemNode = temp;
    }
}