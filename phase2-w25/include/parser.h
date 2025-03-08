/* parser.h */
#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"

// Basic node types for AST
typedef enum {
    AST_PROGRAM,        // Program node
    AST_VARDECL,        // Variable declaration (int x)
    AST_ASSIGN,         // Assignment (x = 5)
    AST_PRINT,          // Print statement
    AST_NUMBER,         // Number literal
    AST_STRING,         // String literal
    AST_OPERATOR,       // Operators such as +,-,*,/
    AST_IDENTIFIER,     // Variable name
    AST_IF,             // IF keyword
    AST_ELSE,           // ELSE keyword (for else statements)
    AST_WHILE,          // WHILE keyword   
    AST_FOR,            // FOR keyword (also used for repeat-until)
    AST_BLOCK,          // BLOCK keyword
    AST_BINOP,          // Binary operations
    AST_FACTORIAL,      // Factorial function
    AST_FUNCTION_CALL,  // Generic function call
    AST_RETURN,         // Return statement
    AST_FUNCTION_DECL   // Function declaration
} ASTNodeType;

typedef enum {
    PARSE_ERROR_NONE,
    PARSE_ERROR_UNEXPECTED_TOKEN,
    PARSE_ERROR_MISSING_SEMICOLON,
    PARSE_ERROR_MISSING_IDENTIFIER,
    PARSE_ERROR_MISSING_EQUALS,
    PARSE_ERROR_MISSING_PARENTHESES, 
    PARSE_ERROR_MISSING_CONDITION, 
    PARSE_ERROR_BLOCK_BRACES, 
    PARSE_ERROR_INVALID_OPERATOR,
    PARSE_ERROR_INVALID_FUNCTION_CALL, 
    PARSE_ERROR_INVALID_EXPRESSION
} ParseError;

// AST Node structure
typedef struct ASTNode {
    ASTNodeType type;           // Type of node
    Token token;               // Token associated with this node
    struct ASTNode* left;      // Left child
    struct ASTNode* right;     // Right child
    // TODO: Add more fields if needed
} ASTNode;

// Parser functions
void parser_init(const char* input);
ASTNode* parse(void);
void print_ast(ASTNode* node, int level);
void free_ast(ASTNode* node);
void print_token_stream(const char* input);
void proc_test_file(const char* filename);

#endif /* PARSER_H */