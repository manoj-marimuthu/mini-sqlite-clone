#include <db_error.h>
#include <db_memory.h>
#include <stdlib.h>
#include <stdio.h>

void log_error(char* msg,DbError errorType){
    char* errorTypeString;
    char* hint;
    switch(errorType){
        case COMPILE_TIME_ERROR:
            errorTypeString = "COMPILER TIME ERROR";
            hint = "Try re-running the application / command";
            break;
        case RUNTIME_ERROR:
            errorTypeString = "RUNTIME ERROR";
            hint = "Check if something is wrong with the associated .db file";
            break;
        case SYNTAX_ERROR:
            errorTypeString = "SYNTAX ERROR";
            hint = "Check syntax at github to learn";
            break;
        default:
            break;  
    }
    printf("[DB Error] %s \n",msg);
    printf("Error type = %s \n",errorTypeString);
    printf("Hint: %s \n",hint);
    clean_up();
    exit(1);
}