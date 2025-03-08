/* lexer.c */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../../include/tokens.h"
#include "../../include/lexer.h"

// All global variables must be reset between files
static int current_line = 1;
static int current_column = 1; 
static char last_token_type = 'x'; // For checking consecutive operators
static int in_error_recovery = 0; // Flag for error recovery mode 

// Add variables to track stored errors
#define MAX_STORED_ERRORS 50000
static struct {
    char lexeme[100];
    int line;
    int column;
    ErrorType error_type;
} stored_errors[MAX_STORED_ERRORS];
static int num_stored_errors = 0;

// Reset all global variables after each file
void reset_all_globals(void) {
    current_line = 1; 
    current_column = 1; 
    last_token_type = 'x';
    in_error_recovery = 0;
    num_stored_errors = 0;
    
    // Clear stored errors array
    for (int i = 0; i < MAX_STORED_ERRORS; i++) {
        stored_errors[i].lexeme[0] = '\0';
        stored_errors[i].line = 0;
        stored_errors[i].column = 0;
        stored_errors[i].error_type = ERROR_NONE;
    }
}

// Clear stored errors
void clear_error_state(void) {
    num_stored_errors = 0;
}

// Reset the lexer state
void reset_lexer(void) {
    reset_all_globals();
}

// advance position and update column count 
static void advance_position(int *pos) {
    (*pos)++; 
    current_column++; 
}

/* Skip until the next character that matches any in the given string */
static void skip_until(const char *input, int *pos, const char *delimiters) {
    while (input[*pos] != '\0' && !strchr(delimiters, input[*pos])) {
        if (input[*pos] == '\n') {
            current_line++;
            current_column = 1;
        } else {
            current_column++;
        }
        (*pos)++;
    }
}

// Store an error for immediate reporting
static void store_error(ErrorType error, int line, int column, const char *lexeme) {
    // Don't store errors if we already have too many
    if (num_stored_errors >= MAX_STORED_ERRORS) {
        return;
    }
    
    // Store the error
    stored_errors[num_stored_errors].error_type = error;
    stored_errors[num_stored_errors].line = line;
    stored_errors[num_stored_errors].column = column;
    strncpy(stored_errors[num_stored_errors].lexeme, lexeme, sizeof(stored_errors[0].lexeme) - 1);
    stored_errors[num_stored_errors].lexeme[sizeof(stored_errors[0].lexeme) - 1] = '\0';
    num_stored_errors++;
    
    // Report the error immediately
    printf("Lexical Error at line %d, column %d: ", line, column);
    switch(error) {
        case ERROR_CONSECUTIVE_OPERATORS:
            printf("Consecutive operators not allowed\n");
            break;
        case ERROR_INVALID_CHAR:
            printf("Invalid token '%s'\n", lexeme);
            break;
        default:
            printf("Unknown error\n");
    }
}

// Keywords table
static struct {
    const char* word;
    TokenType type;
} keywords[] = {
    {"fi", TOKEN_IF},
    {"tni", TOKEN_INT},
    {"rahc", TOKEN_CHAR}, 
    {"diov", TOKEN_VOID}, 
    {"nruter", TOKEN_RETURN}, 
    {"rof", TOKEN_FOR}, 
    {"elihw", TOKEN_WHILE}, 
    {"od", TOKEN_DO},
    {"kaerb", TOKEN_BREAK}, 
    {"eunitnoc", TOKEN_CONTINUE},
    {"hctiws", TOKEN_CONTINUE},
    {"esac", TOKEN_CASE},
    {"tluafed", TOKEN_DEFAULT},
    {"otog", TOKEN_GOTO},
    {"foezis", TOKEN_SIZEOF},
    {"citats", TOKEN_STATIC},
    {"nretxe", TOKEN_EXTERN},
    {"tsnoc", TOKEN_CONST},
    {"elitalov", TOKEN_VOLATILE},
    {"tcurts", TOKEN_STRUCT},
    {"noinu", TOKEN_UNION},
    {"mune", TOKEN_ENUM},
    {"fedepyt", TOKEN_TYPEDEF},
    {"dengisnu", TOKEN_UNSIGNED},
    {"dengis", TOKEN_SIGNED},
    {"trohs", TOKEN_SHORT},
    {"gnol", TOKEN_LONG}, 
    {"taolf", TOKEN_FLOAT_KEY},
    {"elbuod", TOKEN_DOUBLE},
    {"esle", TOKEN_ELSE}, 
    {"diov*", TOKEN_VOID_STAR},
    {"tni*", TOKEN_INT_STAR},
    {"tnirp", TOKEN_PRINT},
    {"taeper", TOKEN_REPEAT},
    {"litnu", TOKEN_UNTIL},
    {"lairotcaf", TOKEN_FACTORIAL}
};

// Check if a string is a keyword 
static int is_keyword(const char* word) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(word, keywords[i].word) == 0) {
            return keywords[i].type;
        }
    }
    return 0;
}


// Print error messages for lexical errors 
void print_error(ErrorType error, int line, const char* lexeme) {
    printf("Lexical Error at line %d: ", line);
    switch(error) {
        case ERROR_INVALID_CHAR:
            printf("Invalid character '%s'\n", lexeme);
            break;
        case ERROR_INVALID_NUMBER:
            printf("Invalid number format\n");
            break;
        case ERROR_CONSECUTIVE_OPERATORS:
            printf("Consecutive operators not allowed\n");
            break;
        case ERROR_UNTERMINATED_STRING:
            printf("Unterminated string literal\n");
            break;
        case ERROR_UNTERMINATED_CHAR:
            printf("Unterminated character literal\n");
            break;
        case ERROR_INVALID_IDENTIFIER:
            printf("Invalid identifier\n");
            break;
        case ERROR_STRING_TOO_LONG:
            printf("String literal too long\n");
            break;
        case ERROR_INVALID_ESCAPE_SEQUENCE:
            printf("Invalid escape sequence\n");
            break;
        case ERROR_EMPTY_CHAR_LITERAL:
            printf("Empty character literal\n");
            break;
        case ERROR_MULTI_CHAR_LITERAL:
            printf("Multi-character literal not allowed\n");
            break;
        case ERROR_INVALID_FLOAT:
            printf("Invalid float format\n");
            break;
        case ERROR_RECOVERY_MODE: 
            printf("Skipping invalid input \n");
            break; 
        case ERROR_UNEXPECTED_TOKEN:
            printf("Unexpected token '%s'\n", lexeme);
            break;
        default:
            printf("Unknown error\n");
    }
}

void print_token(Token token) {
    if(token.type == TOKEN_SKIP){
        return; 
    }

    if (token.error != ERROR_NONE) {
        print_error(token.error, token.line, token.lexeme);
        return;
    }

    printf("Token: ");
    switch(token.type) {
        case TOKEN_NUMBER:    
            printf("NUMBER"); 
            break;
        case TOKEN_FLOAT:
            printf("FLOATING POINT NUMBER");
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR");
            break;
        case TOKEN_EQUALS_EQUALS:
            printf("EQUALS_EQUALS");
            break;
        case TOKEN_NOT_EQUALS:
            printf("NOT_EQUALS");
            break;
        case TOKEN_LOGICAL_AND:
            printf("LOGICAL_AND");
            break;
        case TOKEN_LOGICAL_OR:
            printf("LOGICAL_OR");
            break;
        case TOKEN_GREATER_EQUALS:
            printf("GREATER_EQUALS");
            break;
        case TOKEN_LESS_EQUALS:
            printf("LESS_EQUALS");
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER");
            break;
        case TOKEN_STRING:
            printf("STRING");
            break;
        case TOKEN_CHAR_LITERAL:
            printf("CHARACTER");
            break;
        case TOKEN_POINTER: 
            printf("POINTER");
            break;
        case TOKEN_COMMENT:
            printf("COMMENT");
            break; 
        case TOKEN_EQUALS:     
            printf("EQUALS"); 
            break;
        case TOKEN_SEMICOLON:  
            printf("SEMICOLON"); 
            break;
        case TOKEN_LPAREN:     
            printf("LPAREN"); 
            break;
        case TOKEN_RPAREN:     
            printf("RPAREN"); 
            break;
        case TOKEN_LBRACE:     
            printf("LBRACE"); 
            break;
        case TOKEN_RBRACE:     
            printf("RBRACE"); 
            break;
        case TOKEN_COMMA:
            printf("COMMA");
            break;
        case TOKEN_IF:         
            printf("IF"); 
            break;
        case TOKEN_INT:        
            printf("INT"); 
            break;
        case TOKEN_CHAR:
            printf("CHAR");
            break; 
        case TOKEN_VOID:
            printf("VOID");
            break; 
        case TOKEN_RETURN: 
            printf("RETURN");
            break;
        case TOKEN_FOR:
            printf("FOR");
            break; 
        case TOKEN_WHILE:
            printf("WHILE");
            break; 
        case TOKEN_DO:
            printf("DO");
            break; 
        case TOKEN_BREAK:
            printf("BREAK");
            break; 
        case TOKEN_CONTINUE:
            printf("CONTINUE");
            break; 
        case TOKEN_SWITCH:
            printf("SWITCH");
            break; 
        case TOKEN_CASE:
            printf("CASE");
            break; 
        case TOKEN_DEFAULT:
            printf("DEFAULT");
            break; 
        case TOKEN_GOTO:
            printf("GOTO");
            break; 
        case TOKEN_SIZEOF:
            printf("SIZEOF");
            break; 
        case TOKEN_STATIC:
            printf("STATIC");
            break; 
        case TOKEN_EXTERN:
            printf("EXTERN");
            break; 
        case TOKEN_CONST:
            printf("CONST");
            break; 
        case TOKEN_VOLATILE:
            printf("VOLATILE");
            break; 
        case TOKEN_STRUCT: 
            printf("STRUCT");
            break; 
        case TOKEN_UNION:
            printf("UNION");
            break; 
        case TOKEN_ENUM: 
            printf("ENUM");
            break; 
        case TOKEN_TYPEDEF:
            printf("TYPEDEF");
            break; 
        case TOKEN_UNSIGNED:
            printf("UNSIGNED");
            break; 
        case TOKEN_SHORT:
            printf("SHORT");
            break; 
        case TOKEN_LONG:
            printf("LONG");
            break; 
        case TOKEN_FLOAT_KEY:
            printf("FLOAT"); 
            break; 
        case TOKEN_DOUBLE: 
            printf("DOUBLE");
            break; 
        case TOKEN_ELSE:
            printf("ELSE");
            break; 
        case TOKEN_VOID_STAR:
            printf("VOID*");
            break; 
        case TOKEN_INT_STAR:
            printf("INT*");
            break;
        case TOKEN_PRINT:      
            printf("PRINT"); 
            break;
        case TOKEN_REPEAT:
            printf("REPEAT");
            break;
        case TOKEN_UNTIL:
            printf("UNTIL");
            break;
        case TOKEN_FACTORIAL:
            printf("FACTORIAL");
            break;
        case TOKEN_EOF:        
            printf("EOF"); 
            break;
        default:              
            printf("UNKNOWN");
    }
    printf(" | Lexeme: '%s' | Line: %d | Column: %d\n", token.lexeme, token.line, token.column);
}

/* Handle the escape sequences in strings and chars */
static char handle_escape_sequence(char c) {
    switch (c) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '0': return '\0';
        case '\\': return '\\';
        case '\'': return '\'';
        case '\"': return '\"';
        default: return 0;
    }
}

/* Handle string literals */
static Token handle_string(const char *input, int *pos) {
    Token token = {TOKEN_STRING, "", current_line, current_column, ERROR_NONE, RECOVERY_NONE};
    int i = 0;
    advance_position(pos); // Skip opening quote
    
    while (input[*pos] != '\0' && input[*pos] != '"' && input[*pos] != '\n') {
        if (i >= sizeof(token.lexeme) - 1) {
            token.error = ERROR_STRING_TOO_LONG;
            token.recovery = RECOVERY_TO_NEWLINE;
            skip_until(input, pos, "\n\"");
            return token;
        }
        
        if (input[*pos] == '\\') {
            advance_position(pos);
            char escaped = handle_escape_sequence(input[*pos]);
            if (escaped == 0) {
                token.error = ERROR_INVALID_ESCAPE_SEQUENCE;
                token.recovery = RECOVERY_TO_NEWLINE;
                skip_until(input, pos, "\n\"");
                return token;
            }
            token.lexeme[i++] = escaped;
        } else {
            token.lexeme[i++] = input[*pos];
        }
        advance_position(pos);
    }
    
    if (input[*pos] != '"') {
        token.error = ERROR_UNTERMINATED_STRING;
        token.recovery = RECOVERY_TO_NEWLINE;
        return token;
    }
    
    advance_position(pos); // Skip closing quote
    token.lexeme[i] = '\0';
    return token;
}

/* Handle character literals */
static Token handle_char(const char *input, int *pos) {
    Token token = {TOKEN_CHAR, "", current_line, current_column, ERROR_NONE, RECOVERY_NONE};
    advance_position(pos); // Skip opening quote
    
    if (input[*pos] == '\'') {
        token.error = ERROR_EMPTY_CHAR_LITERAL;
        advance_position(pos);
        return token;
    }
    
    int i = 0;
    if (input[*pos] == '\\') {
        advance_position(pos);
        char escaped = handle_escape_sequence(input[*pos]);
        if (escaped == 0) {
            token.error = ERROR_INVALID_ESCAPE_SEQUENCE;
            token.recovery = RECOVERY_TO_NEWLINE;
            skip_until(input, pos, "\n\'");
            return token;
        }
        token.lexeme[i++] = escaped;
        advance_position(pos);
    } else {
        token.lexeme[i++] = input[*pos];
        advance_position(pos);
    }
    
    if (input[*pos] != '\'') {
        if (input[*pos] != '\0' && input[*pos] != '\n') {
            token.error = ERROR_MULTI_CHAR_LITERAL;
        } else {
            token.error = ERROR_UNTERMINATED_CHAR;
        }
        token.recovery = RECOVERY_TO_NEWLINE;
        skip_until(input, pos, "\n\'");
        return token;
    }
    
    advance_position(pos); // Skip closing quote
    token.lexeme[i] = '\0';
    return token;
}

/* Handle comments */
static Token handle_comment(const char *input, int *pos) {
    Token token = {TOKEN_COMMENT, "", current_line, current_column, ERROR_NONE, RECOVERY_NONE};
    int i = 0;
    
    // Skip '//'
    *pos += 2;
    current_column += 2;
    
    while (input[*pos] != '\0' && input[*pos] != '\n') {
        if (i < sizeof(token.lexeme) - 1) {
            token.lexeme[i++] = input[*pos];
        }
        advance_position(pos);
    }
    
    token.lexeme[i] = '\0';
    return token;
}

/* Handle numbers */
static Token handle_number(const char *input, int *pos) {
    Token token = {TOKEN_NUMBER, "", current_line, current_column, ERROR_NONE, RECOVERY_NONE};
    int i = 0;
    int decimal_count = 0;
    
    // Get digits before decimal
    while (isdigit(input[*pos])) {
        token.lexeme[i++] = input[*pos];
        advance_position(pos);
    }
    
    // Check for decimal points
    if (input[*pos] == '.') {
        token.lexeme[i++] = input[*pos];
        advance_position(pos);
        
        if (!isdigit(input[*pos])) {
            token.error = ERROR_INVALID_NUMBER;
            token.recovery = RECOVERY_TO_DELIMITER;
            skip_until(input, pos, ";,) \t\n");
            return token;
        }
        
        decimal_count++;
        while (isdigit(input[*pos]) || input[*pos] == '.') {
            if (input[*pos] == '.') {
                decimal_count++;
                if (decimal_count > 1) {
                    token.error = ERROR_INVALID_FLOAT;
                    token.recovery = RECOVERY_TO_DELIMITER;
                    skip_until(input, pos, ";,) \t\n");
                    return token;
                }
            }
            token.lexeme[i++] = input[*pos];
            advance_position(pos);
        }
    }
    
    token.lexeme[i] = '\0';
    if (decimal_count == 1) {
        token.type = TOKEN_FLOAT;
    }
    return token;
}

// Get next token from input 
Token get_next_token(const char* input, int* pos) {
    Token token = {TOKEN_ERROR, "", current_line, current_column, ERROR_NONE, RECOVERY_NONE};
    char c;

    // Skip whitespace and track line numbers
    while ((c = input[*pos]) != '\0' && (c == ' ' || c == '\n' || c == '\t')) {
        if (c == '\n') {
            current_line++;
            current_column = 1; 
            in_error_recovery = 0; // Reset error recovery at new line 
        }
        else {
            current_column++; 
        }
        (*pos)++;
    }

    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        token.line = current_line;
        token.column = current_column;
        return token;
    }

    // If in error recovery mode, skip until appropriate delimiter
    if (in_error_recovery) {
        token.type = TOKEN_SKIP;
        token.error = ERROR_RECOVERY_MODE;
        token.line = current_line;
        token.column = current_column;
        skip_until(input, pos, ";\n");
        in_error_recovery = 0;
        return token;
    }

    c = input[*pos];
    token.column = current_column; 
    token.line = current_line;

    // Handle Comments 
    if(c == '/' && input[*pos + 1] == '/'){
        return handle_comment(input, pos);
    }

    // Handle character literals 
    if(c == '\''){
        return handle_char(input, pos);
    }

    // Handle numbers
    if (isdigit(c)) {
        return handle_number(input, pos);
    }

    // Handle identifiers and keywords
    if (isalpha(c) || c == '_') {
        int i = 0;
        do {
            token.lexeme[i++] = c;
            (*pos)++;
            c = input[*pos];
        } while ((isalnum(c) || c == '_') && i < sizeof(token.lexeme) - 1);

        token.lexeme[i] = '\0';

        // Check if it's a keyword
        TokenType keyword_type = is_keyword(token.lexeme);
        if (keyword_type) {
            token.type = keyword_type;
            last_token_type = 'k';

        } else {
            token.type = TOKEN_IDENTIFIER;
            last_token_type = 'i';
        }
        return token;
    }

    // Handles String Literals 
    if(c == '\"'){
        return handle_string(input, pos);
    }


    // Handle pointer operator
    if (c == '*' && (last_token_type == 'k' || last_token_type == 'i')) {
        token.type = TOKEN_POINTER;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        advance_position(pos);
        last_token_type = 'p';
        return token;
    }

    // Handle Operators 
    if (strchr("+-*/=<>!&|", c)) {
        // Single character operators and equality operators
        if (c == '=') {
            token.lexeme[0] = c;
            if (input[*pos + 1] == '=') {
                token.type = TOKEN_EQUALS_EQUALS;
                token.lexeme[1] = '=';
                token.lexeme[2] = '\0';
                *pos += 2;
                current_column += 2;
            } else {
                token.type = TOKEN_EQUALS;
                token.lexeme[1] = '\0';
                advance_position(pos);
            }
        } 
        // Logical operators
        else if (c == '&' && input[*pos + 1] == '&') {
            token.type = TOKEN_LOGICAL_AND;
            token.lexeme[0] = '&';
            token.lexeme[1] = '&';
            token.lexeme[2] = '\0';
            *pos += 2;
            current_column += 2;
        }
        else if (c == '|' && input[*pos + 1] == '|') {
            token.type = TOKEN_LOGICAL_OR;
            token.lexeme[0] = '|';
            token.lexeme[1] = '|';
            token.lexeme[2] = '\0';
            *pos += 2;
            current_column += 2;
        }
        // Comparison operators
        else if (c == '!' && input[*pos + 1] == '=') {
            token.type = TOKEN_NOT_EQUALS;
            token.lexeme[0] = '!';
            token.lexeme[1] = '=';
            token.lexeme[2] = '\0';
            *pos += 2;
            current_column += 2;
        }
        else if (c == '<' && input[*pos + 1] == '=') {
            token.type = TOKEN_LESS_EQUALS;
            token.lexeme[0] = '<';
            token.lexeme[1] = '=';
            token.lexeme[2] = '\0';
            *pos += 2;
            current_column += 2;
        }
        else if (c == '>' && input[*pos + 1] == '=') {
            token.type = TOKEN_GREATER_EQUALS;
            token.lexeme[0] = '>';
            token.lexeme[1] = '=';
            token.lexeme[2] = '\0';
            *pos += 2;
            current_column += 2;
        }
        // Basic operators
        else {
            if (last_token_type == 'o') {
                token.error = ERROR_CONSECUTIVE_OPERATORS;
                token.lexeme[0] = c;
                token.lexeme[1] = '\0';
                token.recovery = RECOVERY_TO_DELIMITER;
                
                store_error(ERROR_CONSECUTIVE_OPERATORS, current_line, current_column, token.lexeme);
                
                advance_position(pos);
                in_error_recovery = 1;
                return token;
            }
            
            token.type = TOKEN_OPERATOR;
            token.lexeme[0] = c;
            token.lexeme[1] = '\0';
            advance_position(pos);
        }
        
        last_token_type = 'o';
        return token;
    }

    // Handle Delimiter 
    if (strchr("(){}[];,", c)) {
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        switch(c){
            case ';':
                token.type = TOKEN_SEMICOLON;
                break;
            case '(':
                token.type = TOKEN_LPAREN;
                break;
            case ')':
                token.type = TOKEN_RPAREN;
                break;
            case '{':
                token.type = TOKEN_LBRACE;
                break;
            case '}':
                token.type = TOKEN_RBRACE;
                break;
            case ',':
                token.type = TOKEN_COMMA;
                break;
            default:
                token.type = TOKEN_DELIMITER;
                break;
        }
        advance_position(pos);
        last_token_type = 'd';
        return token;
    }

    // Handle invalid characters 
    token.error = ERROR_INVALID_CHAR;
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    token.recovery = RECOVERY_TO_DELIMITER;
    
    store_error(ERROR_INVALID_CHAR, current_line, current_column, token.lexeme);
    
    advance_position(pos);
    in_error_recovery = 1;
    return token;
}

/* Process test files */
void process_test_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }
    
    // Reset lexer state for new file
    reset_all_globals();
    
    char buffer[2048];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[len] = '\0';
    fclose(file);
    
    int position = 0;
    Token token;
    printf("\n==============================\n");
    printf("TESTING FILE: %s\n", filename);
    printf("==============================\n");
    printf("Input:\n%s\n\n", buffer);
    
    do {
        token = get_next_token(buffer, &position);
        print_token(token);
        
        if (token.recovery != RECOVERY_NONE) {
            in_error_recovery = 1;
        }
    } while (token.type != TOKEN_EOF);
    
    printf("\nEnd of %s\n", filename);
    printf("==============================\n");
}
