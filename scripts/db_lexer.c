#include <db_lexer.h>
#include <db_memory.h>
#include <db_error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

DbLexerNode* lexer_output = NULL;
DbLexerNode* lexer_tail = NULL;

DbLexerNode* createDbLexerNode(DbLexerNodeType type){
    DbMemNode* lexerMemObj = createMemNode(sizeof(DbLexerNode));
    DbLexerNode* node = lexerMemObj->ptr;
    node->type = type;
    node->next = NULL;
    return node;
}

void line_lexer(char* line){
    int i = 0;
    int n = strlen(line);
    while(i < n){
        DbLexerNode* node;
        if(isspace(line[i])){
            i++;
            continue;
        }else if(isalpha(line[i])){
            DbMemNode* wordObj = createMemNode(1024);
            char* word = wordObj->ptr;
            int j = 0;
            int max_size = 1024;
            while( i < n && isalpha(line[i])){
                if(j >= max_size){
                    max_size *= 2;
                    reallocMemNode(wordObj,max_size);
                }
                word[j] = line[i];
                j++;
                i++;
            }
            word[j] = '\0';
            if(strcmp(word,"select") == 0 || strcmp(word,"SELECT") == 0){
                node = createDbLexerNode(SELECT);
            }
            else if(strcmp(word,"table") == 0 || strcmp(word,"TABLE") == 0){
                node = createDbLexerNode(TABLE);
            }
            else if(strcmp(word,"describe") == 0 || strcmp(word,"DESCRIBE") == 0){
                node = createDbLexerNode(DESCRIBE);
            }
            else if(strcmp(word,"from") == 0 || strcmp(word,"FROM") == 0){
                node = createDbLexerNode(FROM);
            }else if(strcmp(word,"insert") == 0 || strcmp(word,"INSERT") == 0){
                node = createDbLexerNode(INSERT);
            }
            else if(strcmp(word,"values") == 0 || strcmp(word,"VALUES") == 0){
                node = createDbLexerNode(VALUES);
            }
            else if(strcmp(word,"create") == 0 || strcmp(word,"CREATE") == 0){
                node = createDbLexerNode(CREATE);
            }
            else if(strcmp(word,"into") == 0 || strcmp(word,"INTO") == 0){
                node = createDbLexerNode(INTO);
            }
            else if(strcmp(word,"int") == 0 || strcmp(word,"INT") == 0){
                node = createDbLexerNode(INT);
            }
            else if(strcmp(word,"help") == 0 || strcmp(word,"HELP") == 0){
                node = createDbLexerNode(HELP);
            }
            else if(strcmp(word,"text") == 0 || strcmp(word,"TEXT") == 0){
                node = createDbLexerNode(TEXT);
            }
            else{
                node = createDbLexerNode(IDENTIFIER);
                node->data.strData = word;
            }
        }
        else if(line[i] == ','){
            node = createDbLexerNode(COMMA);
            i++;
        }
        else if(line[i] == '*'){
            node = createDbLexerNode(ALL);
            i++;
        }
        else if(line[i] == '('){
            node = createDbLexerNode(L_BRACK);
            i++;
        }
        else if(line[i] == ')'){
            node = createDbLexerNode(R_BRACK);
            i++;
        }
        else if(line[i] == ';'){
            node = createDbLexerNode(CLOSE);
            i++;
        }
        else if(line[i] == '\'' || line[i] == '"'){
            char quote = line[i];
            i++; // consume quote
            DbMemNode* strObj = createMemNode(1024);
            char* str = strObj->ptr;
            int j = 0;
            int max_size = 1024;
            while(i < n && line[i] != quote){ 
                if(j >= max_size){
                    max_size *= 2;
                    reallocMemNode(strObj,max_size);
                }
                str[j] = line[i];
                j++;
                i++;
            }
            str[j] = '\0';
            if(line[i] != quote){
                log_error("Closing quote is required",SYNTAX_ERROR);
            }
            i++; // consume closing quote
            node = createDbLexerNode(STRING);
            node->data.strData = str;
        }else if(isdigit(line[i])){
            int num = 0;
            while(i < n && isdigit(line[i])){
                num = num * 10 + (line[i] - '0');
                i++;
            }
            node = createDbLexerNode(NUMBER);
            node->data.numData = num;
        }
        else{
            log_error("undefined token found",SYNTAX_ERROR);
        }
        node->next = NULL;
        if(lexer_output == NULL){
            lexer_output = node;
            lexer_tail = node;
        }else{
            lexer_tail->next = node;
            lexer_tail = node;
        }
    }
}