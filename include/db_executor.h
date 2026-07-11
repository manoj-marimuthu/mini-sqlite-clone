#ifndef DB_EXECUTOR_H
#define DB_EXECUTOR_H

#include <db_parser.h>
#include <stdlib.h>

typedef enum {
    TEXT_COL,
    INT_COL
} ColumnType;

typedef struct Column{
    ColumnType type;
    char col_name[32];
    size_t col_size;
    struct Column* next;
} Column;

typedef struct Row{
    ColumnType type;
    union{
        char strData[32];
        int numData;
    } data;
    struct Row* next;
} Row;
Column* createColumn(ColumnType type);
Row* createRow(ColumnType type);
void execute(astNode* node);

#endif