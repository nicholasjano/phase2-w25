# Backwards C Parser Error Codes and Error Handling

## 1. Parse Error Type Enumeration

### PARSE_ERROR_NONE (0)

- Normal parsing state
- No error condition present

```c
// Successful parsing
tni x = 10;  // No error
```

### PARSE_ERROR_UNEXPECTED_TOKEN (1)

- Token doesn't match expected grammar
- Example: Invalid tokens in expressions or statements

```c
tni 10;       // Expected identifier after 'tni'
fi $ (x > 0)  // Unexpected token '$'
```

### PARSE_ERROR_MISSING_SEMICOLON (2)

- Statement missing terminating semicolon
- Examples:

```c
tni x = 10    // Missing semicolon
tnirp "Hello" // Missing semicolon
```

### PARSE_ERROR_MISSING_IDENTIFIER (3)

- Expected identifier not found
- Examples:

```c
tni = 30;     // Missing variable name after type
tni niam( {   // Missing parameter name
```

### PARSE_ERROR_MISSING_EQUALS (4)

- Assignment without equals sign
- Examples:

```c
x 10;         // Missing '=' in assignment
tni y 20;     // Missing '=' in initialization
```

### PARSE_ERROR_MISSING_PARENTHESES (5)

- Unbalanced or missing parentheses
- Examples:

```c
fi (x > 10 {  // Missing closing parenthesis
lairotcaf);   // Missing opening parenthesis
```

### PARSE_ERROR_MISSING_CONDITION (6)

- Empty condition in control structures
- Examples:

```c
fi () {       // Empty condition in if
elihw () {    // Empty condition in while
```

### PARSE_ERROR_BLOCK_BRACES (7)

- Unbalanced or missing braces in blocks
- Examples:

```c
fi (x > 10) { // Missing closing brace
  tnirp x;
tni func() ;  // Function without body
```

### PARSE_ERROR_INVALID_OPERATOR (8)

- Operator in invalid context
- Examples:

```c
x + + y;      // Double operator
fi (a === b)  // Invalid operator '==='
```

### PARSE_ERROR_INVALID_FUNCTION_CALL (9)

- Malformed function call
- Examples:

```c
lairotcaf(;   // Missing argument
func(a, );    // Trailing comma
```

### PARSE_ERROR_INVALID_EXPRESSION (10)

- Malformed or empty expression
- Examples:

```c
tni x = ;     // Missing expression after '='
x = a + ;     // Incomplete expression
```

## 2. Parser Synchronization Strategy

The parser uses synchronization to recover from errors and continue parsing:

### 2.1 Synchronization Points

- Semicolons (statement boundaries)
- Opening/closing braces (block boundaries)
- Keywords that start new statements

```c
static void synchronize(void) {
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
```

### 2.2 Dummy Node Creation

When an error is encountered, the parser creates dummy nodes to maintain AST structure:

```c
if (match(TOKEN_RPAREN)) {
    parse_error(PARSE_ERROR_MISSING_CONDITION, if_token);
    // Create a dummy condition
    node->left = create_node(AST_NUMBER);
    node->left->token.lexeme[0] = '0';
    node->left->token.lexeme[1] = '\0';
    advance(); // Consume ')'
}
```

## 3. Implementation Details

### 3.1 Error Reporting Structure

```c
void parse_error(ParseError error, Token token) {
    // Only report errors if reporting is enabled
    if (!error_reporting_enabled) {
        return;
    }
    
    // Skip duplicate errors at the same location
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
        // ... other cases
    }
}
```

### 3.2 Error Tracking Variables

```c
static int error_reporting_enabled = 1;
static int last_reported_line = 0;
static int last_reported_column = 0;
static int error_count = 0;
```

### 3.3 Error Recovery Process

1. Error Detection

```c
if (!match(TOKEN_RPAREN)) {
    parse_error(PARSE_ERROR_MISSING_PARENTHESES, if_token);
}
```

2. Error Reporting

```c
parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
```

3. Recovery Action

```c
synchronize(); // Skip to next valid parsing point
```

## 4. Error Prevention Strategies

### 4.1 Prevention of Cascading Errors

```c
// Skip duplicate errors at the same location
if (token.line == last_reported_line && token.column == last_reported_column) {
    return;
}
```

### 4.2 Robust Expression Parsing

```c
// Check for empty or invalid expressions
if (match(TOKEN_SEMICOLON) || match(TOKEN_RPAREN)) {
    parse_error(PARSE_ERROR_INVALID_EXPRESSION, current_token);
    // Create a dummy node for recovery
    ASTNode *dummy = create_node(AST_NUMBER);
    dummy->token.lexeme[0] = '0';  
    dummy->token.lexeme[1] = '\0';
    return dummy;
}
```

### 4.3 Nullable Parameter Handling

```c
// Handle empty parentheses, create a dummy expression
if (match(TOKEN_RPAREN)) {
    node = create_node(AST_NUMBER);
    node->token.lexeme[0] = '0';
    node->token.lexeme[1] = '\0';
    advance(); // Consume ')'
    return node;
}
```