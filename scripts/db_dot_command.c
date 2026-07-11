#include <db_dot_command.h>
#include <db_error.h>
#include <db_memory.h>
#include <stdio.h>
#include <string.h>
#define VERSION "Mini RDBMS v0.0.0"

void execute_dot_command(char* line){
    if(strcmp(line,".help") == 0){
        printf("SQL Statements \n");
        printf("---------------");
        printf("CREATE TABLE <table_name>(SCHEMA); \n");
        printf("INSERT INTO <table_name>(<values>); \n");
        printf("SELECT * FROM <table_name>; \n");
        printf("DESCRIBE <table_name>; \n");
        printf("\n");
        printf("SUPPORTED TYPES \n");
        printf("---------------- \n");
        printf("INT \n");
        printf("TEXT \n");
        printf("Meta Commands \n");
        printf("-------------- \n");
        printf(".help     Shows list of available features \n");
        printf(".version  Display version number \n");
        printf(".exit     Exit database shell \n");
    }else if(strcmp(line,".exit") == 0){
        clean_up();
    }else if(strcmp(line,".version") == 0){
        printf("%s \n",VERSION);
    }else{
        log_error("Unknow meta command , try again or see .help",RUNTIME_ERROR);
    }
}