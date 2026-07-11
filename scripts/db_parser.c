#include <db_parser.h>
#include <db_memory.h>
#include <db_error.h>
#include <db_lexer.h>
#include <stdio.h>

DbLexerNode* current = NULL;

astNode* createAstNode(astNodeType type){
    DbMemNode* astNodeObj = createMemNode(sizeof(astNode));
    astNode* node = astNodeObj->ptr;
    node->next = NULL;
    node->child = NULL;
    node->next_statement = NULL;
    node->type = type;
    node->col_count = 0;
    return node;
}
void consume(){
    if(current){
        current = current->next;
    }
}

void consumeColon(){
    if(current && current->type == CLOSE) consume();
}

astNode* parseAtom(){
    if(current && current->type == STRING){
        astNode* node = createAstNode(AST_STRING);
        node->data.strData = current->data.strData;
        consume();
        return node;
    }else if(current && current->type == NUMBER){
        astNode* node = createAstNode(AST_NUMBER);
        node->data.numData = current->data.numData;
        consume();
        return node;
    }else{
        log_error("Undefined parsing error",COMPILE_TIME_ERROR);
        return NULL;
    }
    return NULL;
}

astNode* parseSelect(){
    if(current && current->type == SELECT){
        consume();
        astNode* node = createAstNode(AST_SELECT);
        if(current && current->type == ALL){
            consume();
            if(current && current->type == FROM){
                consume();
                if(current && current->type == IDENTIFIER){
                    node->data.strData = current->data.strData;
                    consume();
                }else{
                    log_error("Unknown token found in SELECT statement",SYNTAX_ERROR);
                }
            }else{
                log_error("Incorrect syntax, missing 'FROM' in SELECT statement",SYNTAX_ERROR);
            }
        }
        else{
            log_error("Incorrect syntax at select statement",SYNTAX_ERROR);
        }
        consumeColon();
        return node;
    }else{
        return NULL;
    }
}

astNode* parseInsert(){
    if(current && current->type == INSERT){
        consume();
        if(current && current->type == INTO){
            consume();
            if(current && current->type == IDENTIFIER){
                astNode* node = createAstNode(AST_INSERT);
                node->data.strData = current->data.strData;
                consume();
                if(current && current->type == VALUES){
                    consume();
                    if(current && current->type == L_BRACK){
                        consume();
                        astNode* head = NULL;
                        astNode* tail = NULL; 
                        while(current && current->type != R_BRACK){
                            astNode* val = NULL;
                            if(current &&  (current->type == NUMBER || current->type == STRING)){
                                val = parseAtom();
                            }else{
                                log_error("Unknown type found in INSERT statement",RUNTIME_ERROR);
                            }
                            if(head == NULL){
                                head = val;
                                tail = val;
                            }else{
                                tail->next = val;
                                tail = val;
                            }
                            if(current && current->type == COMMA){
                                consume();
                            }
                        }
                        if(current && current->type == R_BRACK){
                            consume();
                        }else{
                            log_error("Missing ')' in INSERT statement",SYNTAX_ERROR);
                        }
                        node->child = head;
                        consumeColon();
                        return node;
                    }else{
                        log_error("Missing '(' in INSERT statement",SYNTAX_ERROR);
                    }
                }else{
                    log_error("Missing VALUES keyword in INSERT statement",SYNTAX_ERROR);
                }
            }
        }else{
            log_error("Missing INTO keyword in INSERT statement",SYNTAX_ERROR);
            return NULL;
        }
    }else{
        return NULL;
    }
    return NULL;
}

astNode* parseCreate(){
    if(current && current->type == CREATE){
        consume();
        if(current && current->type == TABLE){
            consume();
            if(current && current->type == IDENTIFIER){
                char* table_name = current->data.strData;
                consume();
                if(current && current->type == L_BRACK){
                    consume();
                    astNode* node = createAstNode(AST_CREATE);
                    astNode* head = NULL;
                    astNode* tail = NULL;
                    node->data.strData = table_name;
                    while(current && current->type != R_BRACK){
                        astNode* colNode = NULL;
                        char* colName;
                        if(current && current->type == IDENTIFIER){
                            colName = current->data.strData;
                            consume();
                            if(current && current->type == TEXT){
                                consume();
                                colNode = createAstNode(AST_TYPE_TEXT);
                            }else if(current && current->type == INT){
                                consume();
                                colNode = createAstNode(AST_TYPE_INT);
                            }else{
                                log_error("Invalid type given for field in CREATE statement",RUNTIME_ERROR);
                                return NULL;
                            }
                            colNode->data.strData = colName;
                            if(head == NULL){
                                head = colNode;
                                tail = colNode;
                            }else{
                                tail->next = colNode;
                                tail = colNode;
                            }
                            if(current && current->type == COMMA) consume();
                            node->col_count++;
                        }else{
                            log_error("Missing column name in CREATE statement",SYNTAX_ERROR);
                        } 
                    }
                    if(current && current->type == R_BRACK){
                        consume();
                        node->child = head;
                        consumeColon();
                        return node;
                    }else{
                        log_error("Missing ')' for CREATE statement",SYNTAX_ERROR);
                    }
                }else{
                    log_error("Missing '(' for CREATE statement, after table name",SYNTAX_ERROR);
                }
            }else{
                log_error("Table name must be valid, not strings or integers",RUNTIME_ERROR);
            }
        }else{
            log_error("Missing TABLE keyword in CREATE statement",SYNTAX_ERROR);
        }
    }else{
        return NULL;
    }
    return NULL;
}

astNode* parseDescribe(){
    if(current && current->type == DESCRIBE){
        consume();
        if(current && current->type == IDENTIFIER){
            astNode* node = createAstNode(AST_DESCRIBE);
            node->data.strData = current->data.strData;
            consumeColon();
            return node;
        }else{
            log_error("Missing table name, received something else in DESCRIBE",RUNTIME_ERROR);
        }
    }
    else{
        return NULL;
    }
    return NULL;
}

astNode* parseStatement(){
    if(current){
        switch(current->type){
            case SELECT: 
                return parseSelect();
            case INSERT:
                return parseInsert();
            case CREATE:
                return parseCreate();
            case DESCRIBE:
                return parseDescribe();
            default:
                log_error("Unknown statement type found",RUNTIME_ERROR);
                break;
        }
    }
    return NULL;
}