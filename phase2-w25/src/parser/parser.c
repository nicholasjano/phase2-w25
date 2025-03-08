/* parser.c */
#include <stdio.h>
#include <stdlib.h>
#include "../../include/parser.h"
#include "../../include/lexer.h"
#include "../../include/tokens.h"


// TODO 1: Add more parsing function declarations for: (DONE)
// - if statements: if (condition) { ... }
// - while loops: while (condition) { ... }
// - repeat-until: repeat { ... } until (condition)
// - print statements: print x;
// - blocks: { statement1; statement2; }
// - factorial function: factorial(x)

static ASTNode* parse_if_statement(void);
static ASTNode* parse_while_statement(void);
static ASTNode* parse_for_statement(void);
static ASTNode* parse_print_statement(void);
static ASTNode* parse_block(void);
static ASTNode* parse_factorial(void);



// Current token being processed
static Token current_token;
static int position = 0;
static int pos2 = 0;
static const char *source;


static void parse_error(ParseError error, Token token) {
    // TODO 2: Add more error types for: (DONE)
    // - Missing parentheses
    // - Missing condition
    // - Missing block braces
    // - Invalid operator
    // - Function call errors

    printf("Parse Error at line %d: ", token.line);
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
            printf("Expected parenthesis in '%s' \n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_CONDITION:
            printf("Expected condition in '%s' \n", token.lexeme);
            break;
        case PARSE_ERROR_BLOCK_BRACES:
            printf("Missing braces in '%s' \n", token.lexeme);
            break; 
        case PARSE_ERROR_INVALID_OPERATOR:
            printf("Invalid Operator in '%s' \n", token.lexeme);
            break; 
        case PARSE_ERROR_INVALID_FUNCTION_CALL:
            printf("Invalid Function Call in '%s' \n", token.lexeme);
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
}

// Create a new AST node
static ASTNode *create_node(ASTNodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node) {
        node->type = type;
        node->token = current_token;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

// Match current token with expected type
static int match(TokenType type) {
    return current_token.type == type;
}

// Expect a token type or error
static void expect(TokenType type) {
    if (match(type)) {
        advance();
    } else {
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
        exit(1); // Or implement error recovery
    }
}

// TODO 3: Add parsing functions for each new statement type
// static ASTNode* parse_if_statement(void) {...}
// static ASTNode* parse_while_statement(void) { ... }
// static ASTNode* parse_repeat_statement(void) { ... }
// static ASTNode* parse_print_statement(void) { ... }
// static ASTNode* parse_print_statement(void){...}
// static ASTNode* parse_block(void) {...}
// static ASTNode* parse_factorial(void) { ... }



// Forward declarations
static ASTNode *parse_statement(void);

static ASTNode *parse_expression(void);

// Parse variable declaration: tni x;
static ASTNode *parse_declaration(void) {
    ASTNode *node = create_node(AST_VARDECL);
    advance(); // consume 'tni'

    if (!match(TOKEN_IDENTIFIER)) {
        parse_error(PARSE_ERROR_MISSING_IDENTIFIER, current_token);
        exit(1);
    }

    node->token = current_token;
    advance();

    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
        exit(1);
    }
    advance();
    return node;
}

// Parse assignment: x = 5;
static ASTNode *parse_assignment(void) {
    ASTNode *node = create_node(AST_ASSIGN);
    node->left = create_node(AST_IDENTIFIER);
    node->left->token = current_token;
    advance();

    if (!match(TOKEN_EQUALS)) {
        parse_error(PARSE_ERROR_MISSING_EQUALS, current_token);
        exit(1);
    }
    advance();

    node->right = parse_expression();

    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
        exit(1);
    }
    advance();
    return node;
}

// Parse block statement
static ASTNode *parse_block(void) {
    if (!match(TOKEN_LBRACE)){
        return NULL; //error message here for block
        exit(1);
    }
    ASTNode *block = create_node(AST_BLOCK);
    ASTNode *current = block;
    while (!match(TOKEN_RBRACE) && !match(TOKEN_EOF)) {
        current->left = parse_statement();
        if (!match(TOKEN_RBRACE)) {
            current->right = create_node(AST_BLOCK);
            current = current->right;
        }
    }
    if (!match(TOKEN_RBRACE)){
        return NULL; //error message here for block
        exit(1);
    } 
    return block;
}

// Parse if statement
static ASTNode *parse_if_statement(void) {
    ASTNode *node = create_node(AST_IF);
    if(!match(TOKEN_IF)){
        return NULL; //error message here for if
        exit(1);
    }
    advance(); // Consume 'if'
    if(!match(TOKEN_LPAREN)){
        return NULL; //error message here for missing left param
        exit(1);
    }
    node->left = parse_expression(); // Condition
    if(!match(TOKEN_RPAREN)){
        return NULL; //error message here for missing right param
        exit(1);
    }
    node->right = parse_block(); // If body
    return node;
}

// Parse while loop
static ASTNode *parse_while_statement(void) {
    if(!match(TOKEN_WHILE)){
        return NULL; //error message here for missing while param
        exit(1);
    }
    ASTNode *node = create_node(AST_WHILE);
    advance(); // Consume 'while'
    if(!match(TOKEN_LPAREN)){
        return NULL; //error message here for missing left param
        exit(1);
    }
    node->left = parse_expression(); // Condition
    if(!match(TOKEN_RPAREN)){
        return NULL; //error message here for missing right param
        exit(1);
    }
    node->right = parse_block(); // Loop body
    return node;
}

// Parse repeat-until loop
static ASTNode *parse_repeat_statement(void) {
    ASTNode *node = create_node(AST_FOR);
    advance(); // Consume 'repeat'
    node->left = parse_block(); // Loop body
    if(!match(TOKEN_FOR)){
        return NULL; //error message here for missing repeat param
        exit(1);
    }
    if(!match(TOKEN_LPAREN)){
        return NULL; //error message here for missing left param
        exit(1);
    }
    node->right = parse_expression(); // Condition
    if(!match(TOKEN_RPAREN)){
        return NULL; //error message here for missing right param
        exit(1);
    }
    return node;
}

// Parse print statement
static ASTNode *parse_print_statement(void) {
    ASTNode *node = create_node(AST_PRINT);
    advance(); // Consume 'print'
    node->left = parse_expression(); // Expression to print
    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
        exit(1);
    }
    return node;
}

// Parse factorial function call
static ASTNode *parse_factorial(void) {
    ASTNode *node = create_node(AST_FUNCTION_CALL);
    advance(); // Consume 'factorial'
    if(!match(TOKEN_LPAREN)){
        return NULL; //error message here for missing left param
        exit(1);
    };
    node->left = parse_expression(); // Argument inside parentheses
    if(!match(TOKEN_RPAREN)){
        return NULL; //error message here for missing right param
        exit(1);
    }
    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
        exit(1);
    }
    return node;
}
// Parse statement
static ASTNode *parse_statement(void) {
    if (match(TOKEN_INT)) {
        return parse_declaration();
    } else if (match(TOKEN_IDENTIFIER)) {
        return parse_assignment();
    } else if (match(TOKEN_IF)) {
        return parse_if_statement();
    } else if (match(TOKEN_WHILE)) {
        return parse_while_statement();
    } else if (match(TOKEN_FOR)) {
        return parse_repeat_statement();
    } else if (match(TOKEN_PRINT)) {
        return parse_print_statement();
    } else if (match(TOKEN_LBRACE)) {
        return parse_block();
    }

    // TODO 4: Add cases for new statement types
    // else if (match(TOKEN_IF)) return parse_if_statement();
    // else if (match(TOKEN_WHILE)) return parse_while_statement();
    // else if (match(TOKEN_REPEAT)) return parse_repeat_statement();
    // else if (match(TOKEN_PRINT)) return parse_print_statement();
    // ...

    printf("Syntax Error: Unexpected token\n");
    exit(1);
}

// Parse expression (currently only handles numbers and identifiers)

// TODO 5: Implement expression parsing
// Current expression parsing is basic. Need to implement:
// - Binary operations (+-*/)
// - Comparison operators (<, >, ==, etc.)
// - Operator precedence
// - Parentheses grouping
// - Function calls

static ASTNode *parse_expression(void) {
    ASTNode *node;

    if (match(TOKEN_NUMBER)) {
        node = create_node(AST_NUMBER);
        advance();
    } else if (match(TOKEN_IDENTIFIER)) {
        node = create_node(AST_IDENTIFIER);
        advance();
    }
    else if(match(TOKEN_OPERATOR)){
        node = create_node(AST_OPERATOR);
        advance(); 
    }
    else {
        printf("Syntax Error: Expected expression\n");
        exit(1);
    }

    return node;
}

// Parse program (multiple statements)
static ASTNode *parse_program(void) {
    ASTNode *program = create_node(AST_PROGRAM);
    ASTNode *current = program;

    while (!match(TOKEN_EOF)) {
        current->left = parse_statement();
        if (!match(TOKEN_EOF)) {
            current->right = create_node(AST_PROGRAM);
            current = current->right;
        }
    }

    return program;
}

// Initialize parser
void parser_init(const char *input) {
    source = input;
    position = 0;
    advance(); // Get first token
}

// Main parse function
ASTNode *parse(void) {
    return parse_program();
}

// Print AST (for debugging)
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
        case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->token.lexeme);
            break;
        // TODO 6: Add cases for new node types (DONE for Now)
        case AST_IF: 
            printf("If\n"); 
            break;
        case AST_WHILE: 
            printf("While\n"); 
            break;
        case AST_FOR: 
            printf("For\n"); 
            break;
        case AST_BLOCK: 
            printf("Block\n"); 
            break;
        case AST_BINOP: 
            printf("BinaryOp: %s\n", node->token.lexeme); 
            break;


        default:
            printf("Unknown node type\n");
    }

    // Print children
    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
}

// Print the token input stream
void print_token_stream(const char* input){ 
    Token token; 
    do{
        token = get_next_token(input, &pos2);
        print_token(token);
    } while(token.type != TOKEN_EOF);
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
    
    char buffer[2048];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[len] = '\0';
    fclose(file);
    
    printf("Parsing input:\n%s\n", buffer);
    parser_init(buffer);
    ASTNode *ast = parse();

    printf("\nAbstract Syntax Tree:\n");
    print_ast(ast, 0);

    free_ast(ast);
}


// Main function for testing
int main() {
    // // Test with both valid and invalid inputs
    // const char *valid_input = "tni x;\n" // Valid declaration
    //         "x = 42;\n"; // Valid assignment;

    // TODO 8: Add more test cases and read from a file: (DONE)
    proc_test_file("test/input_valid.txt");
    proc_test_file("test/input_invalid.txt");

    // const char *invalid_input = "tni x;\n"
    //                             "x = 42;\n"
    //                             "tni ;";

    // printf("TOKEN Stream: \n");
    // print_token_stream(valid_input);

    // printf("Parsing input:\n%s\n", valid_input);
    // parser_init(valid_input);
    // ASTNode *ast = parse();

    // printf("\nAbstract Syntax Tree:\n");
    // print_ast(ast, 0);

    // free_ast(ast);
    return 0;
}
