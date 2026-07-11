#include <stdio.h>
#include <stdlib.h>
#include <db_error.h>
#include <db_memory.h>
#include <db_lexer.h>
#include <db_parser.h>
#include <db_executor.h>
#include <string.h>
#include <db_dot_command.h>

void print_on_startup(){
    printf("Mini RDBMS v0.0.0 \n");
    printf("Type .help for help \n");
}

int main(){
    char line[1024];
    print_on_startup();
    while(1){
        printf(">> ");
        if(fgets(line,sizeof(line),stdin) == NULL){
            break;
        }
        if(line[0] == '\0' || line[0] == '\n') continue;
        // check for Dot commands
        line[strcspn(line,"\n")] = '\0';
        if(strcmp(line,".exit") == 0 || strcmp(line,".quit") == 0){
            break;
        }
        if(line[0] == '.'){
            execute_dot_command(line);
            continue;
        }
        // tokenize line
        line_lexer(line);
        current = lexer_output;
        astNode* statement_node = parseStatement();
        execute(statement_node);
        // reset for REPL
        current = NULL;
        lexer_output = NULL;
        lexer_tail = NULL;
    }
    clean_up();
    return 0;
}