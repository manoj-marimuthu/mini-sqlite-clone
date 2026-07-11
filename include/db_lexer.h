#ifndef DB_LEXER_H
#define DB_LEXER_H

typedef enum {
    TABLE,
    SELECT,
    FROM,
    ALL,
    INSERT,
    VALUES,
    STRING,
    CREATE,
    INTO,
    INT,
    TEXT,
    COMMA,
    L_BRACK,
    HELP,
    R_BRACK,
    NUMBER,
    IDENTIFIER,
    CLOSE,
    DESCRIBE
} DbLexerNodeType;

typedef struct DbLexerNode{
    DbLexerNodeType type;
    union{
        int numData;
        char* strData;
    } data;
    struct DbLexerNode* next;
} DbLexerNode;

// db_lexer APIs

DbLexerNode* createDbLexerNode(DbLexerNodeType type);
DbLexerNode* lexer_output;
DbLexerNode* lexer_tail;
void line_lexer(char* line);

#endif