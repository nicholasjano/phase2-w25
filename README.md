# Phase 2: Syntax Analysis - Parser Implementation
## Overview
In this phase, you'll build upon your lexical analyzer to create a parser that constructs an Abstract Syntax Tree (AST) from the token stream. The parser validates the syntactic structure of the input according to the language grammar and creates a tree representation suitable for further processing.
## Getting Started
### Project Structure
```
phase2/
├── include/
│   ├── tokens.h        # Token definitions from Phase 1
│   ├── lexer.h        # Lexer interface
│   └── parser.h       # Parser definitions
├── src/
│   ├── lexer/
│   │   └── lexer.c    # Lexer implementation from Phase 1
│   └── parser/
│       └── parser.c   # Parser implementation
└── test/
    ├── input_valid.txt
    └── input_invalid.txt
```
### Current Implementation
The starter code provides:
- Basic AST node structure
- Simple statement parsing (declarations and assignments)
- Error reporting framework
- AST printing utilities
Example of currently supported syntax:
```c
int x;      // Variable declaration
x = 42;     // Assignment statement
```
## Tasks to Complete
### 1. Expression Parsing
- [ ] Implement operator precedence
- [ ] Add support for:
  - Binary operations (+, -, *, /)
  - Comparison operators (<, >, ==, !=)
  - Parenthesized expressions
### 2. Statement Types
- [ ] If statements: `if (condition) { statements }`
- [ ] While loops: `while (condition) { statements }`
- [ ] Repeat-Until: `repeat { statements } until (condition)`
- [ ] Print statements: `print expression;`
- [ ] Block statements: `{ statement1; statement2; }`
### 3. Special Features
- [ ] Factorial function support
- [ ] Runtime error detection
- [ ] Block scoping
### 4. Error Handling
- [ ] Extend error types
- [ ] Improve error messages
- [ ] Add error recovery
- [ ] Track line and column information
## Implementation Guide
### Step 1: Study the Starter Code
1. Understand the current AST structure
2. Review existing parsing functions
3. Examine error handling mechanism
### Step 2: Add New Features
1. Locate TODOs in the code
2. Implement features incrementally
3. Test each addition thoroughly
4. Update error handling accordingly
### Step 3: Testing
Test your implementation with various inputs:
```c
// Valid syntax
int x;
x = 42;
if (x > 0) {
    print x;
}
// Error cases
if (x > ) {     // Invalid expression
while x > 0 {   // Missing parentheses
int ;           // Missing identifier
```
## Error Handling Examples
Your parser should provide clear error messages:
```
Parse Error at line 2: Expected identifier after 'int'
Parse Error at line 3: Missing parentheses in if statement
Parse Error at line 4: Invalid expression after '='
```
## Tips for Success
-  Start with expression parsing
-  Test incrementally
-  Focus on error handling
-  Document your changes
-  Use provided test cases
-  Update AST printing for debugging
## Submission Requirements
1. Complete implementation of your custom parser.c
2. Test files demonstrating your features
3. Documentation of your:
   - Grammar rules
   - Error handling strategy
   - AST structure
   - Test cases
  
Good luck!
