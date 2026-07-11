#ifndef DB_PARSER_H
#define DB_PARSER_H
#include <db_lexer.h>

typedef enum{
    AST_CREATE,
    AST_SELECT,
    AST_NUMBER,
    AST_STRING,
    AST_IDENTIFIER,
    AST_INSERT,
    AST_TYPE_TEXT,
    AST_TYPE_INT,
    AST_DESCRIBE,
    AST_HELP,
} astNodeType;

typedef struct astNode{
    astNodeType type;
    union{
        int numData;
        char* strData;
    } data;
    int col_count;
    struct astNode* next;
    struct astNode* child;
    struct astNode* next_statement;
} astNode;

// parsing functions

astNode* parseAtom();
astNode* parseSelect();
astNode* parseInsert();
astNode* parseCreate();
astNode* parseDescribe();
astNode* parseStatement();
DbLexerNode* current;
// parser APIs

astNode* createAstNode(astNodeType type);
#endif