/* tokens.h */
#ifndef TOKENS_H
#define TOKENS_H

// Token Types that need to be recognized by the lexer
typedef enum {
    TOKEN_EOF,
    TOKEN_NUMBER,         // e.g., "123", "456"
    TOKEN_FLOAT,          // e.g., "1.23", "0.123"
    TOKEN_OPERATOR,       // +, -, *, /
    TOKEN_EQUALS_EQUALS,  // ==
    TOKEN_NOT_EQUALS,     // !=
    TOKEN_LOGICAL_AND,    // &&
    TOKEN_LOGICAL_OR,     // ||
    TOKEN_GREATER_EQUALS, // >=
    TOKEN_LESS_EQUALS,    // <=
    TOKEN_ERROR,          // Error Token
    TOKEN_IDENTIFIER,     // Variable names
    TOKEN_STRING,         // String literals
    TOKEN_CHAR_LITERAL,   // Character literals 
    TOKEN_DELIMITER,      // Separators and delimiters
    TOKEN_COMMENT,        // Comments
    TOKEN_POINTER,        // Pointer operator *
    TOKEN_SKIP,           // Token to indicate skipping during error recovery
    TOKEN_EQUALS,         // =
    TOKEN_SEMICOLON,      // ;
    TOKEN_LPAREN,         // (
    TOKEN_RPAREN,         // )
    TOKEN_LBRACE,         // {
    TOKEN_RBRACE,         // }
    TOKEN_COMMA,          // ,
    TOKEN_IF,             // if keyword
    TOKEN_INT,            // int keyword
    TOKEN_CHAR,           // char keyword 
    TOKEN_VOID,           // void keyword 
    TOKEN_RETURN,         // return keyword
    TOKEN_FOR,            // for keyword 
    TOKEN_WHILE,          // while keyword 
    TOKEN_DO,             // do keyword 
    TOKEN_BREAK,          // break keyword 
    TOKEN_CONTINUE,       // continue keyword 
    TOKEN_SWITCH,         // switch keyword 
    TOKEN_CASE,           // case keyword   
    TOKEN_DEFAULT,        // default keyword 
    TOKEN_GOTO,           // goto keyword 
    TOKEN_SIZEOF,         // sizeof keyword 
    TOKEN_STATIC,         // static keyword 
    TOKEN_EXTERN,         // extern keyword 
    TOKEN_CONST,          // const keyword 
    TOKEN_VOLATILE,       // volatile keyword 
    TOKEN_STRUCT,         // struct keyword 
    TOKEN_UNION,          // union keyword 
    TOKEN_ENUM,           // enum keyword 
    TOKEN_TYPEDEF,        // typedef keyword
    TOKEN_UNSIGNED,       // unsigned keyword 
    TOKEN_SIGNED,         // signed keyword
    TOKEN_SHORT,          // short keyword 
    TOKEN_LONG,           // long keyword 
    TOKEN_FLOAT_KEY,      // float keyword 
    TOKEN_DOUBLE,         // double keyword 
    TOKEN_ELSE,           // else keyword
    TOKEN_VOID_STAR,      // void* keyword 
    TOKEN_INT_STAR,       // int* keyword 
    TOKEN_PRINT,          // print keyword
    TOKEN_REPEAT,         // repeat keyword 
    TOKEN_UNTIL,          // until keyword
    TOKEN_FACTORIAL       // factorial function
} TokenType;


// Error types for lexical analysis
typedef enum {
    ERROR_NONE,
    ERROR_INVALID_CHAR,
    ERROR_INVALID_NUMBER,
    ERROR_CONSECUTIVE_OPERATORS,
    ERROR_UNTERMINATED_STRING,
    ERROR_UNTERMINATED_CHAR,
    ERROR_INVALID_IDENTIFIER,
    ERROR_STRING_TOO_LONG,
    ERROR_INVALID_ESCAPE_SEQUENCE,
    ERROR_EMPTY_CHAR_LITERAL,
    ERROR_MULTI_CHAR_LITERAL,
    ERROR_INVALID_FLOAT,
    ERROR_RECOVERY_MODE,
    ERROR_UNEXPECTED_TOKEN
} ErrorType;

/* Error recovery modes */
typedef enum {
    RECOVERY_NONE,
    RECOVERY_TO_SEMICOLON,    // Recover until next semicolon
    RECOVERY_TO_NEWLINE,      // Recover until next newline
    RECOVERY_TO_DELIMITER     // Recover until next delimiter
} RecoveryMode;

// Token structure to store token information
typedef struct {
    TokenType type;
    char lexeme[100];       // Actual text of the token
    int line;               // Line number in source file
    int column;             // Column number in source file 
    ErrorType error;        // Error type if any
    RecoveryMode recovery;  // Recovery mode if error 
} Token;

#endif /* TOKENS_H */