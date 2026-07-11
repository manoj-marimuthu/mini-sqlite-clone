#ifndef DB_ERROR_H
#define DB_ERROR_H

typedef enum{
    SYNTAX_ERROR,
    RUNTIME_ERROR,
    COMPILE_TIME_ERROR
} DbError;

void log_error(char* msg,DbError errorType);
#endif