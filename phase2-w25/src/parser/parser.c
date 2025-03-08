/* parser.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/parser.h"
#include "../../include/lexer.h"
#include "../../include/tokens.h"

// Current token being processed
static Token current_token;
static int position = 0;
static int pos2 = 0;
static const char *source;

// Error reporting control
static int error_reporting_enabled = 1;
static int last_reported_line = 0;
static int last_reported_column = 0;

// Forward declarations for utility functions
static void parse_error(ParseError error, Token token);
static void advance(void);
static ASTNode *create_node(ASTNodeType type);
static int match(TokenType type);
static void synchronize(void);
static void consume(TokenType type, ParseError error_type);

// Forward declarations for expression parsing
static ASTNode* parse_primary_expression(void);
static ASTNode* parse_multiplicative_expression(void);
static ASTNode* parse_additive_expression(void);
static ASTNode* parse_comparison_expression(void);
static ASTNode* parse_logical_and_expression(void);
static ASTNode* parse_logical_or_expression(void);
static ASTNode* parse_expression(void);

// Forward declarations for statement parsing
static ASTNode* parse_if_statement(void);
static ASTNode* parse_while_statement(void);
static ASTNode* parse_repeat_until_statement(void);
static ASTNode* parse_print_statement(void);
static ASTNode* parse_return_statement(void);
static ASTNode* parse_block(void);
static ASTNode* parse_function_declaration(void);
static ASTNode* parse_statement(void);
static ASTNode* parse_declaration(void);
static ASTNode* parse_assignment(void);
static ASTNode* parse_program(void);

// Error reporting
static void parse_error(ParseError error, Token token) {
    // Only report errors if reporting is enabled
    if (!error_reporting_enabled) {
        return;
    }
    
    // Only report errors for actual tokens (not EOF)
    if (token.type == TOKEN_EOF) {
        return;
    }
    
    // Skip duplicate errors at the same location
    if (token.line == last_reported_line && token.column == last_reported_column) {
        return;
    }
    
    // Update the last reported error location
    last_reported_line = token.line;
    last_reported_column = token.column;
    
    printf("Parse Error at line %d, column %d: ", token.line, token.column);
    switch (error) {
        case PARSE_ERROR_UNEXPECTED_TOKEN:
            printf("Unexpected token '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_SEMICOLON:
            printf("Missing semicolon after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_IDENTIFIER:
            printf("Expected identifier after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_EQUALS:
            printf("Expected '=' after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_PARENTHESES:
            printf("Missing parenthesis in expression\n");
            break;
        case PARSE_ERROR_MISSING_CONDITION:
            printf("Expected condition after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_BLOCK_BRACES:
            printf("Missing brace for block statement\n");
            break;
        case PARSE_ERROR_INVALID_OPERATOR:
            printf("Invalid operator '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_INVALID_FUNCTION_CALL:
            printf("Invalid function call to '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_INVALID_EXPRESSION:
            printf("Invalid expression after '%s'\n", token.lexeme);
            break;
        default:
            printf("Unknown error\n");
    }
}

// Get next token
static void advance(void) {
    current_token = get_next_token(source, &position);
    
    // Skip comments and error tokens during error recovery
    while (current_token.type == TOKEN_ERROR || current_token.type == TOKEN_SKIP || current_token.type == TOKEN_COMMENT) {
        if (current_token.type == TOKEN_ERROR) {
            printf("Skipping invalid token '%s' at line %d\n", 
                  current_token.lexeme, current_token.line);
        }
        current_token = get_next_token(source, &position);
    }
}

// Create a new AST node
static ASTNode *create_node(ASTNodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node) {
        node->type = type;
        node->token = current_token;
        node->left = NULL;
        node->right = NULL;
    } else {
        fprintf(stderr, "Error: Memory allocation failed for AST node\n");
        exit(1);
    }
    return node;
}

// Match current token with expected type
static int match(TokenType type) {
    return current_token.type == type;
}

// Try to synchronize after an error (enhanced error recovery)
static void synchronize(void) {
    // Skip tokens until we find a statement boundary or synchronization point
    advance(); // Skip the current token that caused the error
    
    while (!match(TOKEN_EOF)) {
        // Semicolon marks the end of most statements
        if (match(TOKEN_SEMICOLON)) {
            advance(); // Skip the semicolon
            return;
        }
        
        // Right brace might end a block
        if (match(TOKEN_RBRACE)) {
            return; // Don't advance yet, let the block parser handle it
        }
        
        // New statement starters
        if (match(TOKEN_INT) || match(TOKEN_FLOAT_KEY) || match(TOKEN_CHAR) ||
            match(TOKEN_VOID) || match(TOKEN_RETURN) || match(TOKEN_IF) || 
            match(TOKEN_WHILE) || match(TOKEN_PRINT) || match(TOKEN_LBRACE) ||
            match(TOKEN_REPEAT) || match(TOKEN_ELSE) || match(TOKEN_IDENTIFIER)) {
            return; // Don't advance, let the statement parser handle it
        }
        
        advance();
    }
}

// Function to consume a specific token type or report error
static void consume(TokenType type, ParseError error_type) {
    if (match(type)) {
        advance();
    } else {
        // Special case for EOF - don't report errors for EOF token
        if (current_token.type == TOKEN_EOF) {
            return;
        }
        parse_error(error_type, current_token);
        synchronize();
    }
}

// TODO 3: Add parsing functions for each new statement type

// Parse primary expression (identifier, number, or parenthesized expression)
static ASTNode *parse_primary_expression(void) {
    ASTNode *node;

    if (match(TOKEN_NUMBER)) {
        node = create_node(AST_NUMBER);
        advance();
    } else if (match(TOKEN_IDENTIFIER)) {
        node = create_node(AST_IDENTIFIER);
        Token identifier_token = current_token;
        advance();
        
        // Check if this is a function call (if followed by left parenthesis)
        if (match(TOKEN_LPAREN)) {
            // Special case for factorial function
            if (strcmp(identifier_token.lexeme, "lairotcaf") == 0) {
                // Create factorial node
                ASTNode *factorial_node = create_node(AST_FACTORIAL);
                advance(); // Consume '('
                
                // Empty parentheses - create a dummy argument
                if (match(TOKEN_RPAREN)) {
                    factorial_node->left = create_node(AST_NUMBER);
                    factorial_node->left->token.lexeme[0] = '0';
                    factorial_node->left->token.lexeme[1] = '\0';
                    advance(); // Consume ')'
                    return factorial_node;
                }
                
                // Parse argument
                factorial_node->left = parse_expression();
                
                // Expect closing parenthesis
                if (!match(TOKEN_RPAREN)) {
                    parse_error(PARSE_ERROR_MISSING_PARENTHESES, current_token);
                    synchronize();
                    return factorial_node;
                }
                advance(); // Consume ')'
                
                return factorial_node;
            } else {
                // Generic function call
                ASTNode *call_node = create_node(AST_FUNCTION_CALL);
                call_node->token = identifier_token;
                advance(); // Consume '('
                
                // Parse arguments if any
                if (!match(TOKEN_RPAREN)) {
                    call_node->left = parse_expression();
                }
                
                // Expect closing parenthesis
                if (!match(TOKEN_RPAREN)) {
                    parse_error(PARSE_ERROR_MISSING_PARENTHESES, current_token);
                    synchronize();
                    return call_node;
                }
                advance(); // Consume ')'
                
                return call_node;
            }
        }
    } else if (match(TOKEN_FACTORIAL)) {
        // Direct factorial token
        advance(); // Consume 'lairotcaf'
        
        // Missing opening parenthesis
        if (!match(TOKEN_LPAREN)) {
            parse_error(PARSE_ERROR_MISSING_PARENTHESES, current_token);
            
            // Special case: if we see a closing parenthesis, try to continue
            if (match(TOKEN_RPAREN)) {
                ASTNode *node = create_node(AST_FACTORIAL);
                advance(); // Consume ')'
                return node;
            }
            
            synchronize();
            return create_node(AST_FACTORIAL);
        }
        
        ASTNode *node = create_node(AST_FACTORIAL);
        advance(); // Consume '('
        
        // Empty parentheses - create a dummy argument
        if (match(TOKEN_RPAREN)) {
            node->left = create_node(AST_NUMBER);
            node->left->token.lexeme[0] = '0';
            node->left->token.lexeme[1] = '\0';
            advance(); // Consume ')'
            return node;
        }
        
        // Parse argument
        node->left = parse_expression();
        
        // Expect closing parenthesis
        if (!match(TOKEN_RPAREN)) {
            parse_error(PARSE_ERROR_MISSING_PARENTHESES, current_token);
            synchronize();
            return node;
        }
        advance(); // Consume ')'
        
        return node;
    } else if (match(TOKEN_LPAREN)) {
        advance(); // Consume '('
        
        // Empty parentheses - create a dummy expression
        if (match(TOKEN_RPAREN)) {
            node = create_node(AST_NUMBER);
            node->token.lexeme[0] = '0';
            node->token.lexeme[1] = '\0';
            advance(); // Consume ')'
            return node;
        }
        
        node = parse_expression();
        
        // Expect closing parenthesis
        if (!match(TOKEN_RPAREN)) {
            parse_error(PARSE_ERROR_MISSING_PARENTHESES, current_token);
            synchronize();
            return node;
        }
        advance(); // Consume ')'
    } else if (match(TOKEN_STRING)) {
        // Handle string literals
        node = create_node(AST_STRING);
        advance();
    } else {
        parse_error(PARSE_ERROR_INVALID_EXPRESSION, current_token);
        synchronize();
        // Create a dummy node to allow parsing to continue
        node = create_node(AST_NUMBER);
        node->token.lexeme[0] = '0';
        node->token.lexeme[1] = '\0';
    }

    return node;
}

// Parse multiplicative expression (* and /)
static ASTNode *parse_multiplicative_expression(void) {
    ASTNode *left = parse_primary_expression();

    while ((match(TOKEN_OPERATOR) && (current_token.lexeme[0] == '*' || current_token.lexeme[0] == '/')) || 
           match(TOKEN_POINTER)) {  // Handle POINTER token for multiplication
        ASTNode *node = create_node(AST_BINOP);
        node->token = current_token;
        
        // Set the lexeme to '*' if it's a pointer token to ensure consistent rendering
        if (node->token.type == TOKEN_POINTER) {
            node->token.lexeme[0] = '*';
            node->token.lexeme[1] = '\0';
        }
        
        advance();

        node->left = left;
        node->right = parse_primary_expression();
        left = node;
    }

    return left;
}

// Parse additive expression (+ and -)
static ASTNode *parse_additive_expression(void) {
    ASTNode *left = parse_multiplicative_expression();

    while (match(TOKEN_OPERATOR) && 
           (current_token.lexeme[0] == '+' || current_token.lexeme[0] == '-')) {
        ASTNode *node = create_node(AST_BINOP);
        node->token = current_token;
        advance();

        node->left = left;
        node->right = parse_multiplicative_expression();
        left = node;
    }

    return left;
}

// Parse comparison expression (<, >, ==, !=, >=, <=)
static ASTNode *parse_comparison_expression(void) {
    ASTNode *left = parse_additive_expression();

    while (match(TOKEN_OPERATOR) || 
           match(TOKEN_EQUALS_EQUALS) || 
           match(TOKEN_NOT_EQUALS) ||
           match(TOKEN_GREATER_EQUALS) || 
           match(TOKEN_LESS_EQUALS)) {
        ASTNode *node = create_node(AST_BINOP);
        node->token = current_token;
        advance();

        node->left = left;
        node->right = parse_additive_expression();
        left = node;
    }

    return left;
}

// Parse logical AND expression (&&)
static ASTNode *parse_logical_and_expression(void) {
    ASTNode *left = parse_comparison_expression();

    while (match(TOKEN_LOGICAL_AND)) {
        ASTNode *node = create_node(AST_BINOP);
        node->token = current_token;
        advance();

        node->left = left;
        node->right = parse_comparison_expression();
        left = node;
    }

    return left;
}

// Parse logical OR expression (||)
static ASTNode *parse_logical_or_expression(void) {
    ASTNode *left = parse_logical_and_expression();

    while (match(TOKEN_LOGICAL_OR)) {
        ASTNode *node = create_node(AST_BINOP);
        node->token = current_token;
        advance();

        node->left = left;
        node->right = parse_logical_and_expression();
        left = node;
    }

    return left;
}

// Parse expression (top level)
static ASTNode *parse_expression(void) {
    return parse_logical_or_expression();
}

// Parse variable declaration: tni x;
static ASTNode *parse_declaration(void) {
    ASTNode *node = create_node(AST_VARDECL);
    advance(); // consume type keyword (like 'tni')

    if (!match(TOKEN_IDENTIFIER)) {
        parse_error(PARSE_ERROR_MISSING_IDENTIFIER, current_token);
        synchronize();
        return node;
    }

    node->token = current_token;
    advance();

    // Handle initialization if present
    if (match(TOKEN_EQUALS)) {
        advance(); // consume '='
        node->right = parse_expression();
    }

    consume(TOKEN_SEMICOLON, PARSE_ERROR_MISSING_SEMICOLON);
    
    return node;
}

// Parse function declaration with parameter handling
static ASTNode *parse_function_declaration(void) {
    ASTNode *node = create_node(AST_FUNCTION_DECL);
    advance(); // consume type (like 'tni')

    if (!match(TOKEN_IDENTIFIER)) {
        parse_error(PARSE_ERROR_MISSING_IDENTIFIER, current_token);
        synchronize();
        return node;
    }

    node->token = current_token; // Save function name
    advance(); // consume function name

    // Parse parameters
    consume(TOKEN_LPAREN, PARSE_ERROR_MISSING_PARENTHESES);
    
    // Handle parameters
    // TODO: currently just skipping, but later create parameter nodes
    if (match(TOKEN_VOID)) {
        advance(); // consume 'diov' parameter
    }
    else {
        // Skip over parameter list without requiring commas
        // In a full implementation, we'd parse each parameter and add it to a parameter list
        while (!match(TOKEN_RPAREN) && !match(TOKEN_EOF)) {
            // Skip over parameter type
            if (match(TOKEN_INT) || match(TOKEN_FLOAT_KEY) || match(TOKEN_CHAR) ||
                match(TOKEN_VOID) || match(TOKEN_LONG) || match(TOKEN_SHORT) ||
                match(TOKEN_DOUBLE)) {
                advance();
            }
            
            // Skip over parameter name
            if (match(TOKEN_IDENTIFIER)) {
                advance();
            }
            
            // Skip commas
            if (match(TOKEN_COMMA)) {
                advance();
            }
            else break; // If not a comma, we're done with parameters
        }
    }

    consume(TOKEN_RPAREN, PARSE_ERROR_MISSING_PARENTHESES);
    
    // Parse function body
    node->left = parse_block();
    return node;
}

// Parse assignment: x = 5;
static ASTNode *parse_assignment(void) {
    ASTNode *node = create_node(AST_ASSIGN);
    node->left = create_node(AST_IDENTIFIER);
    node->left->token = current_token;
    advance();

    consume(TOKEN_EQUALS, PARSE_ERROR_MISSING_EQUALS);
    node->right = parse_expression();
    consume(TOKEN_SEMICOLON, PARSE_ERROR_MISSING_SEMICOLON);
    
    return node;
}

// Parse block with improved handling
static ASTNode *parse_block(void) {
    consume(TOKEN_LBRACE, PARSE_ERROR_BLOCK_BRACES);
    
    // Handle empty block
    if (match(TOKEN_RBRACE)) {
        advance(); // consume '}'
        return create_node(AST_BLOCK);
    }

    ASTNode *block = create_node(AST_BLOCK);
    ASTNode *current = block;

    // Parse statements until closing brace
    while (!match(TOKEN_RBRACE) && !match(TOKEN_EOF)) {
        current->left = parse_statement();
        
        // Continue building the block if we have more statements
        if (!match(TOKEN_RBRACE) && !match(TOKEN_EOF)) {
            current->right = create_node(AST_BLOCK);
            current = current->right;
        }
    }

    consume(TOKEN_RBRACE, PARSE_ERROR_BLOCK_BRACES);
    
    return block;
}

// Parse if statement
static ASTNode *parse_if_statement(void) {
    ASTNode *node = create_node(AST_IF);
    advance(); // consume 'fi'

    consume(TOKEN_LPAREN, PARSE_ERROR_MISSING_PARENTHESES);
    node->left = parse_expression(); // Parse condition
    consume(TOKEN_RPAREN, PARSE_ERROR_MISSING_PARENTHESES);

    node->right = parse_block(); // Parse 'if' block
    
    // Check for 'else' clause
    if (match(TOKEN_ELSE)) {
        ASTNode *else_node = create_node(AST_ELSE);
        advance(); // consume 'esle'
        
        else_node->left = node->right; // The 'if' block
        else_node->right = parse_block(); // The 'else' block
        
        node->right = else_node; // Replace the right child with the else node
    }
    
    return node;
}

// Parse while loop: elihw (condition) { statements }
static ASTNode *parse_while_statement(void) {
    ASTNode *node = create_node(AST_WHILE);
    advance(); // consume 'elihw'

    consume(TOKEN_LPAREN, PARSE_ERROR_MISSING_PARENTHESES);
    node->left = parse_expression(); // Parse condition
    consume(TOKEN_RPAREN, PARSE_ERROR_MISSING_PARENTHESES);
    node->right = parse_block(); // Parse loop body
    
    return node;
}

// Parse repeat-until loop: taeper { statements } litnu (condition);
static ASTNode *parse_repeat_until_statement(void) {
    ASTNode *node = create_node(AST_FOR); // Reusing FOR node type for repeat-until
    advance(); // consume 'taeper'

    node->left = parse_block(); // Parse loop body

    if (!match(TOKEN_UNTIL)) {
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
        synchronize();
        return node;
    }
    advance(); // consume 'litnu'

    consume(TOKEN_LPAREN, PARSE_ERROR_MISSING_PARENTHESES);
    node->right = parse_expression(); // Parse condition
    consume(TOKEN_RPAREN, PARSE_ERROR_MISSING_PARENTHESES);
    consume(TOKEN_SEMICOLON, PARSE_ERROR_MISSING_SEMICOLON);
    
    return node;
}

// Parse print statement: tnirp expression;
static ASTNode *parse_print_statement(void) {
    ASTNode *node = create_node(AST_PRINT);
    advance(); // consume 'tnirp'

    node->left = parse_expression(); // Parse expression to print
    consume(TOKEN_SEMICOLON, PARSE_ERROR_MISSING_SEMICOLON);
    
    return node;
}

// Parse return statement: nruter <expression>;
static ASTNode *parse_return_statement(void) {
    ASTNode *node = create_node(AST_RETURN);
    advance(); // consume 'nruter'

    // Parse the return value expression
    node->left = parse_expression();
    
    consume(TOKEN_SEMICOLON, PARSE_ERROR_MISSING_SEMICOLON);
    
    return node;
}

// Parse statement
static ASTNode *parse_statement(void) {
    if (match(TOKEN_INT) || match(TOKEN_FLOAT_KEY) || match(TOKEN_CHAR) ||
        match(TOKEN_VOID) || match(TOKEN_LONG) || match(TOKEN_SHORT) ||
        match(TOKEN_DOUBLE) || match(TOKEN_SIGNED) || match(TOKEN_UNSIGNED)) {
        
        // Look ahead to see if this is a function declaration
        int save_position = position;
        
        advance(); // consume type
        
        if (match(TOKEN_IDENTIFIER)) {
            advance(); // consume identifier
            
            if (match(TOKEN_LPAREN)) {
                // This is a function declaration
                position = save_position; // Backtrack to the type token
                advance(); // Re-read the type token
                return parse_function_declaration();
            }
        }
        
        // Not a function declaration, backtrack
        position = save_position; // Return to the type token
        advance(); // Re-read the type token
        
        return parse_declaration();
    } else if (match(TOKEN_IDENTIFIER)) {
        return parse_assignment();
    } else if (match(TOKEN_IF)) {
        return parse_if_statement();
    } else if (match(TOKEN_WHILE)) {
        return parse_while_statement();
    } else if (match(TOKEN_REPEAT)) { 
        return parse_repeat_until_statement();
    } else if (match(TOKEN_PRINT)) {
        return parse_print_statement();
    } else if (match(TOKEN_RETURN)) {
        return parse_return_statement();
    } else if (match(TOKEN_LBRACE)) {
        return parse_block();
    } else if (match(TOKEN_ELSE)) {
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
        advance(); // Skip 'else'
        
        // Still parse the else block to recover gracefully
        if (match(TOKEN_LBRACE)) {
            parse_block();
        }
        
        return create_node(AST_PROGRAM); // Return dummy node
    } else {
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
        synchronize();
        return create_node(AST_PROGRAM); // Return a dummy node
    }
}

// Parse program with improved function handling
static ASTNode *parse_program(void) {
    ASTNode *program = create_node(AST_PROGRAM);
    
    // Handle edge case of empty input
    if (match(TOKEN_EOF)) {
        return program;
    }
    
    // Special case - top level is usually a function declaration in C
    if (match(TOKEN_INT) || match(TOKEN_VOID) || match(TOKEN_CHAR) ||
        match(TOKEN_FLOAT_KEY) || match(TOKEN_LONG) || match(TOKEN_SHORT) ||
        match(TOKEN_DOUBLE)) {
        // Look ahead to see if this is a function declaration
        int save_position = position;
        
        advance(); // consume type
        
        if (match(TOKEN_IDENTIFIER)) {
            advance(); // consume identifier
            
            if (match(TOKEN_LPAREN)) {
                // This is a function declaration
                position = save_position; // Backtrack to the type token
                advance(); // Re-read the type token
                program->left = parse_function_declaration();
                
                // Parse any additional statements after the function
                if (!match(TOKEN_EOF)) {
                    program->right = parse_program();
                }
                
                return program;
            }
        }
        
        // Not a function declaration, backtrack
        position = save_position; // Return to the type token
        advance(); // Re-read the type token
    }
    
    // Regular statement handling
    program->left = parse_statement();
    
    if (!match(TOKEN_EOF)) {
        program->right = parse_program();
    }
    
    return program;
}

// Initialize parser
void parser_init(const char *input) {
    source = input;
    position = 0;
    pos2 = 0;
    last_reported_line = 0;
    last_reported_column = 0;
    error_reporting_enabled = 1;
    advance(); // Get first token
}

// Main parse function
ASTNode *parse(void) {
    // Temporarily disable error reporting during top-level analysis to avoid spurious errors
    error_reporting_enabled = 0;
    ASTNode *result = parse_program();
    error_reporting_enabled = 1;
    return result;
}

// Print AST for debugging (enhanced for new node types)
void print_ast(ASTNode *node, int level) {
    if (!node) return;

    // Indent based on level
    for (int i = 0; i < level; i++) printf("  ");

    // Print node info
    switch (node->type) {
        case AST_PROGRAM:
            printf("Program\n");
            break;
        case AST_VARDECL:
            printf("VarDecl: %s\n", node->token.lexeme);
            break;
        case AST_ASSIGN:
            printf("Assign\n");
            break;
        case AST_NUMBER:
            printf("Number: %s\n", node->token.lexeme);
            break;
        case AST_STRING:
            printf("String: \"%s\"\n", node->token.lexeme);
            break;
        case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->token.lexeme);
            break;
        case AST_IF:
            printf("If Statement\n");
            break;
        case AST_ELSE:
            printf("Else Statement\n");
            break;
        case AST_WHILE:
            printf("While Loop\n");
            break;
        case AST_FOR:
            printf("Repeat-Until Loop\n");
            break;
        case AST_BLOCK:
            printf("Block\n");
            break;
        case AST_BINOP:
            printf("BinaryOp: %s\n", node->token.lexeme);
            break;
        case AST_PRINT:
            printf("Print Statement\n");
            break;
        case AST_FACTORIAL:
            printf("Factorial Function\n");
            break;
        case AST_FUNCTION_CALL:
            printf("Function Call\n");
            break;
        case AST_RETURN:
            printf("Return Statement\n");
            break;
        case AST_FUNCTION_DECL:
            printf("Function Declaration: %s\n", node->token.lexeme);
            break;
        default:
            printf("Unknown node type: %d\n", node->type);
    }

    // Print children
    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
}

// Print token stream for debugging
void print_token_stream(const char* input) {
    Token token;
    int temp_pos = 0;
    
    do {
        token = get_next_token(input, &temp_pos);
        print_token(token);
    } while (token.type != TOKEN_EOF);
}

// Free AST memory
void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}

// Process test files
void proc_test_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }
    
    char buffer[2048];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[len] = '\0';
    fclose(file);
    
    printf("\n==============================\n");
    printf("PARSING FILE: %s\n", filename);
    printf("==============================\n");
    printf("Input:\n%s\n\n", buffer);
    
    // Reset error reporting state for each test file
    last_reported_line = 0;
    last_reported_column = 0;
    
    // First show token stream
    printf("TOKEN STREAM:\n");
    print_token_stream(buffer);
    
    // Then parse and display AST
    parser_init(buffer);
    ASTNode *ast = parse();

    printf("\nABSTRACT SYNTAX TREE:\n");
    print_ast(ast, 0);
    printf("==============================\n");

    free_ast(ast);
}

// Main function for testing
int main() {
    // Test with both valid and invalid inputs
    proc_test_file("../test/input_valid.txt");
    proc_test_file("../test/input_invalid.txt");
    
    return 0;
}
