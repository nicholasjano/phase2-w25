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
static int error_count = 0;

// Forward declarations for utility functions
void parse_error(ParseError error, Token token);
static void advance(void);
static ASTNode *create_node(ASTNodeType type);
static int match(TokenType type);
static void synchronize(void);

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

// Reset all parser state variables
static void reset_parser_state(void) {
    position = 0;
    pos2 = 0;
    source = NULL;
    last_reported_line = 0;
    last_reported_column = 0;
    error_reporting_enabled = 1;
    error_count = 0;
    
    // Reset the lexer state too
    reset_lexer();
}

void parse_error(ParseError error, Token token) {
    // Only report errors if reporting is enabled
    if (!error_reporting_enabled) {
        return;
    }
    
    // Only report errors for actual tokens (not EOF)
    if (token.type == TOKEN_EOF) {
        return;
    }
    
    // Skip duplicate errors at the same location (but not entirely the same line)
    if (token.line == last_reported_line && token.column == last_reported_column) {
        return;
    }
    
    // Update the last reported error location
    last_reported_line = token.line;
    last_reported_column = token.column;
    error_count++;
    
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
    // Get the next token
    current_token = get_next_token(source, &position);
    
    // Skip comments and error tokens during error recovery
    while (current_token.type == TOKEN_ERROR || current_token.type == TOKEN_SKIP || current_token.type == TOKEN_COMMENT) {
        // The error reporting is now handled by the lexer not here
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

// Try to synchronize after an error
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
                    free(node); // Free the original identifier node
                    return factorial_node;
                }
                
                // Parse argument
                factorial_node->left = parse_expression();
                
                // Expect closing parenthesis
                if (!match(TOKEN_RPAREN)) {
                    parse_error(PARSE_ERROR_MISSING_PARENTHESES, current_token);
                    synchronize();
                    free(node); // Free the original identifier node
                    return factorial_node;
                }
                advance(); // Consume ')'
                
                free(node); // Free the original identifier node
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
                    free(node); // Free the original identifier node
                    return call_node;
                }
                advance(); // Consume ')'
                
                free(node); // Free the original identifier node
                return call_node;
            }
        }
    } else if (match(TOKEN_FACTORIAL)) {
        // Direct factorial token
        Token factorial_token = current_token;
        advance(); // Consume 'lairotcaf'
        
        // Missing opening parenthesis
        if (!match(TOKEN_LPAREN)) {
            parse_error(PARSE_ERROR_MISSING_PARENTHESES, factorial_token);
            
            // Check for special error case: factorial immediately followed by closing parenthesis
            if (match(TOKEN_RPAREN)) {
                ASTNode *node = create_node(AST_FACTORIAL);
                parse_error(PARSE_ERROR_INVALID_FUNCTION_CALL, factorial_token);
                advance(); // Consume ')'
                return node;
            }
            
            synchronize();
            return create_node(AST_FACTORIAL);
        }
        
        ASTNode *node = create_node(AST_FACTORIAL);
        advance(); // Consume '('
        
        // Handle incomplete factorial call
        if (match(TOKEN_EOF) || match(TOKEN_SEMICOLON) || match(TOKEN_RBRACE)) {
            parse_error(PARSE_ERROR_INVALID_FUNCTION_CALL, factorial_token);
            return node;
        }
        
        // Empty parentheses, create a dummy argument
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
        
        // Empty parentheses, create a dummy expression
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
        // Invalid expression (errors caught)
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
    // Check for empty or invalid expressions
    if (match(TOKEN_SEMICOLON) || match(TOKEN_RPAREN)) {
        parse_error(PARSE_ERROR_INVALID_EXPRESSION, current_token);
        // Create a dummy node for recovery
        ASTNode *dummy = create_node(AST_NUMBER);
        dummy->token.lexeme[0] = '0';  
        dummy->token.lexeme[1] = '\0';
        return dummy;
    }
    
    return parse_logical_or_expression();
}

// Parse variable declaration: tni x;
static ASTNode *parse_declaration(void) {
    ASTNode *node = create_node(AST_VARDECL);
    Token type_token = current_token; // Save the type token
    advance(); // consume type keyword (like 'tni')

    if (!match(TOKEN_IDENTIFIER)) {
        parse_error(PARSE_ERROR_MISSING_IDENTIFIER, type_token);
        synchronize();
        return node;
    }

    node->token = current_token;
    advance();

    // Handle initialization if present
    if (match(TOKEN_EQUALS)) {
        advance(); // consume '='
        
        // Check for invalid expression after equals
        if (match(TOKEN_SEMICOLON)) {
            parse_error(PARSE_ERROR_INVALID_EXPRESSION, current_token);
            advance(); // consume semicolon
            return node;
        }
        
        node->right = parse_expression();
    }

    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
        // Continue parsing without consuming the token
    } else {
        advance(); // Consume semicolon if present
    }
    
    return node;
}

// Parse function declaration with parameter handling
static ASTNode *parse_function_declaration(void) {
    ASTNode *node = create_node(AST_FUNCTION_DECL);
    Token type_token = current_token; // Save the return type token
    advance(); // consume type (like 'tni')

    if (!match(TOKEN_IDENTIFIER)) {
        parse_error(PARSE_ERROR_MISSING_IDENTIFIER, type_token);
        synchronize();
        return node;
    }

    node->token = current_token; // Save function name
    Token function_name = current_token; // Keep function name for error reporting
    advance(); // consume function name

    // Parse parameters
    if (!match(TOKEN_LPAREN)) {
        parse_error(PARSE_ERROR_MISSING_PARENTHESES, current_token);
    } else {
        advance(); // Consume '('
    }
    
    // Create a parameter list that will become the left child of the function declaration
    ASTNode *param_list = NULL;
    ASTNode *current_param = NULL;
    
    // Handle parameters
    if (match(TOKEN_VOID)) {
        // No parameters (void)
        advance();
    }
    else {
        // Parse parameter list
        while (!match(TOKEN_RPAREN) && !match(TOKEN_EOF)) {
            // Parameter type
            if (!match(TOKEN_INT) && !match(TOKEN_FLOAT_KEY) && !match(TOKEN_CHAR) &&
                !match(TOKEN_VOID) && !match(TOKEN_LONG) && !match(TOKEN_SHORT) &&
                !match(TOKEN_DOUBLE) && !match(TOKEN_SIGNED) && !match(TOKEN_UNSIGNED)) {
                parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
                break;
            }
            
            // Create parameter node
            ASTNode *param = create_node(AST_VARDECL);
            Token param_type = current_token; // Save parameter type
            advance();
            
            // Parameter name
            if (!match(TOKEN_IDENTIFIER)) {
                parse_error(PARSE_ERROR_MISSING_IDENTIFIER, param_type);
                free(param); // Free unused node
                break;
            }
            
            // Save parameter name to node
            param->token = current_token;
            advance();
            
            // Add parameter to list
            if (param_list == NULL) {
                param_list = param;
                current_param = param;
            } else {
                current_param->right = param;
                current_param = param;
            }
            
            // Handle comma for multiple parameters
            if (match(TOKEN_COMMA)) {
                advance();
            } else {
                break; // End of parameter list
            }
        }
    }

    if (!match(TOKEN_RPAREN)) {
        parse_error(PARSE_ERROR_MISSING_PARENTHESES, current_token);
    } else {
        advance(); // Consume ')'
    }
    
    // Set parameter list as the left child
    node->left = param_list;
    
    // Check for function definition without body (just a semicolon)
    if (match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_BLOCK_BRACES, function_name);
        advance(); // Consume ';'
        return node;
    }
    
    // Parse function body as the right child
    node->right = parse_block();
    return node;
}

// Parse assignment: x = 5;
static ASTNode *parse_assignment(void) {
    ASTNode *node = create_node(AST_ASSIGN);
    node->left = create_node(AST_IDENTIFIER);
    node->left->token = current_token;
    Token id_token = current_token; // Save for error reporting
    advance();

    if (!match(TOKEN_EQUALS)) {
        parse_error(PARSE_ERROR_MISSING_EQUALS, id_token);
        
        // Clean up if we can't continue
        free_ast(node->left);
        free(node);
        
        synchronize();
        return create_node(AST_PROGRAM); // Return a dummy node
    }
    
    advance(); // Consume '='
    
    // Check for invalid expression after equals
    if (match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_INVALID_EXPRESSION, current_token);
        advance(); // consume semicolon
        return node;
    }
    
    node->right = parse_expression();
    
    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
    } else {
        advance(); // Consume semicolon if present
    }
    
    return node;
}

// Parse block statement
static ASTNode *parse_block(void) {
    if (!match(TOKEN_LBRACE)) {
        parse_error(PARSE_ERROR_BLOCK_BRACES, current_token);
        // Create an empty block node
        return create_node(AST_BLOCK);
    }
    
    Token opening_brace = current_token; // Save for error reporting
    advance(); // Consume '{'
    
    // Handle empty block
    if (match(TOKEN_RBRACE)) {
        advance(); // consume '}'
        return create_node(AST_BLOCK);
    }

    ASTNode *block = create_node(AST_BLOCK);
    ASTNode *current = block;
    int stmt_count = 0;

    // Parse statements until closing brace
    while (!match(TOKEN_RBRACE) && !match(TOKEN_EOF)) {
        current->left = parse_statement();
        stmt_count++;
        
        // Continue building the block if we have more statements
        if (!match(TOKEN_RBRACE) && !match(TOKEN_EOF)) {
            current->right = create_node(AST_BLOCK);
            current = current->right;
        }
    }

    if (!match(TOKEN_RBRACE)) {
        parse_error(PARSE_ERROR_BLOCK_BRACES, opening_brace);
        // We've reached EOF without a closing brace
        return block;
    }
    
    advance(); // Consume '}'
    return block;
}

// Parse if statement
static ASTNode *parse_if_statement(void) {
    ASTNode *node = create_node(AST_IF);
    Token if_token = current_token; // Save for error reporting
    advance(); // consume 'fi'

    if (!match(TOKEN_LPAREN)) {
        parse_error(PARSE_ERROR_MISSING_PARENTHESES, if_token);
    } else {
        advance(); // Consume '('
    }
    
    // Handle empty condition
    if (match(TOKEN_RPAREN)) {
        parse_error(PARSE_ERROR_MISSING_CONDITION, if_token);
        // Create a dummy condition
        node->left = create_node(AST_NUMBER);
        node->left->token.lexeme[0] = '0';
        node->left->token.lexeme[1] = '\0';
        advance(); // Consume ')'
    } else {
        node->left = parse_expression(); // Parse condition
        
        if (!match(TOKEN_RPAREN)) {
            parse_error(PARSE_ERROR_MISSING_PARENTHESES, if_token);
        } else {
            advance(); // Consume ')'
        }
    }

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

// Parse while loop
static ASTNode *parse_while_statement(void) {
    ASTNode *node = create_node(AST_WHILE);
    Token while_token = current_token; // Save for error reporting
    advance(); // consume 'elihw'

    if (!match(TOKEN_LPAREN)) {
        parse_error(PARSE_ERROR_MISSING_PARENTHESES, while_token);
    } else {
        advance(); // Consume '('
    }
    
    // Handle empty condition
    if (match(TOKEN_RPAREN)) {
        parse_error(PARSE_ERROR_MISSING_CONDITION, while_token);
        // Create a dummy condition
        node->left = create_node(AST_NUMBER);
        node->left->token.lexeme[0] = '0';
        node->left->token.lexeme[1] = '\0';
        advance(); // Consume ')'
    } else {
        node->left = parse_expression(); // Parse condition
        
        if (!match(TOKEN_RPAREN)) {
            parse_error(PARSE_ERROR_MISSING_PARENTHESES, while_token);
        } else {
            advance(); // Consume ')'
        }
    }
    
    node->right = parse_block(); // Parse loop body
    
    return node;
}

static ASTNode *parse_repeat_until_statement(void) {
    ASTNode *node = create_node(AST_FOR); // Reusing FOR node type for repeat-until
    // Remove the unused variable
    advance(); // consume 'taeper'

    node->left = parse_block(); // Parse loop body

    if (!match(TOKEN_UNTIL)) {
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
        // Expected 'until' token but not found
        Token error_token = current_token;
        error_token.lexeme[0] = '?'; // Placeholder for missing token
        error_token.lexeme[1] = '\0';
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, error_token);
        synchronize();
        return node;
    }
    
    Token until_token = current_token; // Save for error reporting
    advance(); // consume 'litnu'

    if (!match(TOKEN_LPAREN)) {
        parse_error(PARSE_ERROR_MISSING_PARENTHESES, until_token);
    } else {
        advance(); // Consume '('
    }
    
    // Handle empty condition
    if (match(TOKEN_RPAREN)) {
        parse_error(PARSE_ERROR_MISSING_CONDITION, until_token);
        // Create a dummy condition
        node->right = create_node(AST_NUMBER);
        node->right->token.lexeme[0] = '0';
        node->right->token.lexeme[1] = '\0';
        advance(); // Consume ')'
    } else {
        node->right = parse_expression(); // Parse condition
        
        if (!match(TOKEN_RPAREN)) {
            parse_error(PARSE_ERROR_MISSING_PARENTHESES, until_token);
        } else {
            advance(); // Consume ')'
        }
    }
    
    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
    } else {
        advance(); // Consume ';'
    }
    
    return node;
}

// Parse print statement
static ASTNode *parse_print_statement(void) {
    ASTNode *node = create_node(AST_PRINT);
    // Remove the unused variable
    advance(); // consume 'tnirp'

    node->left = parse_expression(); // Parse expression to print
    
    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
    } else {
        advance(); // Consume semicolon if present
    }
    
    return node;
}

// Parse return statement: nruter <expression>;
static ASTNode *parse_return_statement(void) {
    ASTNode *node = create_node(AST_RETURN);
    Token return_token = current_token; // Save for error reporting
    advance(); // consume 'nruter'

    // Check for missing return value (just a semicolon)
    if (match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_INVALID_EXPRESSION, return_token);
        // Create a dummy return value
        node->left = create_node(AST_NUMBER);
        node->left->token.lexeme[0] = '0';
        node->left->token.lexeme[1] = '\0';
        advance(); // Consume ';'
        return node;
    }
    
    // Parse the return value expression
    node->left = parse_expression();
    
    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
    } else {
        advance(); // Consume semicolon if present
    }
    
    return node;
}

// Parse statement
static ASTNode *parse_statement(void) {

    if (match(TOKEN_INT) || match(TOKEN_FLOAT_KEY) || match(TOKEN_CHAR) ||
        match(TOKEN_VOID) || match(TOKEN_LONG) || match(TOKEN_SHORT) ||
        match(TOKEN_DOUBLE) || match(TOKEN_SIGNED) || match(TOKEN_UNSIGNED)) {
        
        // Look ahead to see if this is a function declaration
        int save_position = position;
        Token save_token = current_token;
        
        advance(); // consume type
        
        if (match(TOKEN_IDENTIFIER)) {
            advance(); // consume identifier
            
            if (match(TOKEN_LPAREN)) {
                // This is a function declaration
                position = save_position; // Backtrack to the type token
                current_token = save_token; // Restore the saved token
                return parse_function_declaration();
            }
        }
        
        // Not a function declaration, backtrack
        position = save_position; // Return to the type token
        current_token = save_token; // Restore the saved token
        
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
    } else if (match(TOKEN_FACTORIAL)) {
        // Handle standalone factorial calls
        ASTNode *expr = parse_primary_expression();
        
        // Check for missing semicolon
        if (!match(TOKEN_SEMICOLON)) {
            parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
        } else {
            advance(); // Consume semicolon
        }
        
        return expr;
    } else {
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
        synchronize();
        return create_node(AST_PROGRAM); // Return a dummy node
    }
}

// Parse program (multiple statements)
static ASTNode *parse_program(void) {
    ASTNode *program = create_node(AST_PROGRAM);
    
    // Handle edge case of empty input
    if (match(TOKEN_EOF)) {
        return program;
    }
    
    // Function declaration check
    if (match(TOKEN_INT) || match(TOKEN_VOID) || match(TOKEN_CHAR) ||
        match(TOKEN_FLOAT_KEY) || match(TOKEN_LONG) || match(TOKEN_SHORT) ||
        match(TOKEN_DOUBLE)) {
        // Look ahead to see if this is a function declaration
        int save_position = position;
        Token save_token = current_token;
        
        advance(); // consume type
        
        if (match(TOKEN_IDENTIFIER)) {
            advance(); // consume identifier
            
            if (match(TOKEN_LPAREN)) {
                // This is a function declaration
                position = save_position; // Backtrack to the type token
                current_token = save_token; // Restore the saved token
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
        current_token = save_token; // Restore the saved token
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
    error_count = 0;
    advance(); // Get first token
}

// Main parse function
ASTNode *parse(void) {
    // Enable error reporting for all parsing
    error_reporting_enabled = 1;
    ASTNode *result = parse_program();
    return result;
}

// Print AST
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
            printf("Function Call: %s\n", node->token.lexeme);
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

// Print the token input stream
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

/* Process test files */
void proc_test_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }
    
    // Reset everything for each file
    reset_parser_state();
    
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
    error_count = 0;
    
    // First show token stream
    printf("TOKEN STREAM:\n");
    print_token_stream(buffer);
    
    // IMPORTANT FIX: Reset lexer state again after token stream printing
    reset_lexer();
    
    // Then parse and display AST
    parser_init(buffer);
    ASTNode *ast = parse();

    printf("\nABSTRACT SYNTAX TREE:\n");
    print_ast(ast, 0);
    
    if (error_count > 0) {
        printf("\nParsing completed with %d errors.\n", error_count);
    } else {
        printf("\nParsing completed successfully with no errors.\n");
    }
    
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