# Backwards C Test Cases and Expected Output

## 1. Test Cases

### 1.1 Valid Input

```c
// Valid Backwards C program example
tni niam(diov) {
    // Variable declarations
    tni a = 10;
    tni b = 20;
    tni c;
    
    // Assignment with binary operations
    c = a + b * 5;
    
    // Print statement
    tnirp c;
    
    // If statement
    fi (c > 100) {
        tnirp "c is greater than 100";
    } esle {
        tnirp "c is less than or equal to 100";
    }
    
    // While loop
    elihw (a > 0) {
        tnirp a;
        a = a - 1;
    }
    
    // Repeat-until loop
    taeper {
        b = b + 1;
        tnirp b;
    } litnu (b > 25);
    
    // Factorial function
    tni factorial_result = lairotcaf(5);
    tnirp factorial_result;
    
    // Block statement with scoping
    {
        tni x = 50;
        tnirp x;
    }
    
    // Complex expressions with parentheses
    c = (a + b) * (10 - 5) / 2;
    
    // Comparison operators
    fi (a == 0 && b != 30 || c >= 50) {
        tnirp "Complex condition is true";
    }
    
    nruter 0;
}
```

### 1.2 Invalid Input

```c
// Invalid Backwards C program example with multiple error types
tni niam(diov) {
    // Unterminated string
    tnirp "This string is not terminated;
    
    // Invalid escape sequence
    tnirp "Invalid escape sequence: \z";
    
    // Consecutive operators (+++, &&&)
    c = a +++ b;
    fi (a && &&& b) {
        tnirp "Multiple consecutive operators";
    }
    
    // Missing semicolon
    tni a = 10
    
    // Invalid expression
    tni b = 20 + ;
    
    // Unbalanced parentheses
    fi (c > 100 {
        tnirp "Missing closing parenthesis";
    }
    
    // Missing identifiers
    tni = 30;
    
    // Unbalanced braces in while loop
    elihw (a > 0) {
        tnirp a;
        a = a - 1;
    
    // Malformed repeat-until
    taeper {
        b = b + 1;
        tnirp b;
    litnu (b > 25);
    
    // Mismatched function call parentheses
    tni factorial_result = lairotcaf);
    
    // Missing conditional expression
    fi () {
        tnirp "Missing condition";
    }
    
    // Unterminated character literal
    rahc invalid_char = 'x;
    
    // Unmatched else
    esle {
        tnirp "No matching if statement";
    }
    
    // Incorrectly nested blocks
    {
        {
            tnirp "Unbalanced block";
    }
    
    // Invalid operators and syntax
    fi (a === b || c &&& d) {
        tnirp "Invalid logical operators";
    }
    
    // Incomplete factorial call
    lairotcaf(
    
    // Missing return value
    nruter;
}

// Function definition without body
tni rehtona_noitcnuf(diov);
```

## 2. Expected outputs

### 2.1 Valid Input

```
==============================
PARSING FILE: ../test/input_valid.txt
==============================
Input:
// Valid Backwards C program example
tni niam(diov) {
    // Variable declarations
    tni a = 10;
    tni b = 20;
    tni c;

    // Assignment with binary operations
    c = a + b * 5;

    // Print statement
    tnirp c;

    // If statement
    fi (c > 100) {
        tnirp "c is greater than 100";
    } esle {
        tnirp "c is less than or equal to 100";
    }

    // While loop
    elihw (a > 0) {
        tnirp a;
        a = a - 1;
    }

    // Repeat-until loop
    taeper {
        b = b + 1;
        tnirp b;
    } litnu (b > 25);

    // Factorial function
    tni factorial_result = lairotcaf(5);
    tnirp factorial_result;

    // Block statement with scoping
    {
        tni x = 50;
        tnirp x;
    }

    // Complex expressions with parentheses
    c = (a + b) * (10 - 5) / 2;

    // Comparison operators
    fi (a == 0 && b != 30 || c >= 50) {
        tnirp "Complex condition is true";
    }

    nruter 0;
}


TOKEN STREAM:
Token: COMMENT | Lexeme: ' Valid Backwards C program example' | Line: 1 | Column: 1
Token: INT | Lexeme: 'tni' | Line: 2 | Column: 1
Token: IDENTIFIER | Lexeme: 'niam' | Line: 2 | Column: 2
Token: LPAREN | Lexeme: '(' | Line: 2 | Column: 2
Token: VOID | Lexeme: 'diov' | Line: 2 | Column: 3
Token: RPAREN | Lexeme: ')' | Line: 2 | Column: 3
Token: LBRACE | Lexeme: '{' | Line: 2 | Column: 5
Token: COMMENT | Lexeme: ' Variable declarations' | Line: 3 | Column: 5
Token: INT | Lexeme: 'tni' | Line: 4 | Column: 5
Token: IDENTIFIER | Lexeme: 'a' | Line: 4 | Column: 6
Token: EQUALS | Lexeme: '=' | Line: 4 | Column: 7
Token: NUMBER | Lexeme: '10' | Line: 4 | Column: 9
Token: SEMICOLON | Lexeme: ';' | Line: 4 | Column: 11
Token: INT | Lexeme: 'tni' | Line: 5 | Column: 5
Token: IDENTIFIER | Lexeme: 'b' | Line: 5 | Column: 6
Token: EQUALS | Lexeme: '=' | Line: 5 | Column: 7
Token: NUMBER | Lexeme: '20' | Line: 5 | Column: 9
Token: SEMICOLON | Lexeme: ';' | Line: 5 | Column: 11
Token: INT | Lexeme: 'tni' | Line: 6 | Column: 5
Token: IDENTIFIER | Lexeme: 'c' | Line: 6 | Column: 6
Token: SEMICOLON | Lexeme: ';' | Line: 6 | Column: 6
Token: COMMENT | Lexeme: ' Assignment with binary operations' | Line: 8 | Column: 5
Token: IDENTIFIER | Lexeme: 'c' | Line: 9 | Column: 5
Token: EQUALS | Lexeme: '=' | Line: 9 | Column: 6
Token: IDENTIFIER | Lexeme: 'a' | Line: 9 | Column: 8
Token: OPERATOR | Lexeme: '+' | Line: 9 | Column: 9
Token: IDENTIFIER | Lexeme: 'b' | Line: 9 | Column: 11
Token: POINTER | Lexeme: '*' | Line: 9 | Column: 12
Token: NUMBER | Lexeme: '5' | Line: 9 | Column: 14
Token: SEMICOLON | Lexeme: ';' | Line: 9 | Column: 15
Token: COMMENT | Lexeme: ' Print statement' | Line: 11 | Column: 5
Token: PRINT | Lexeme: 'tnirp' | Line: 12 | Column: 5
Token: IDENTIFIER | Lexeme: 'c' | Line: 12 | Column: 6
Token: SEMICOLON | Lexeme: ';' | Line: 12 | Column: 6
Token: COMMENT | Lexeme: ' If statement' | Line: 14 | Column: 5
Token: IF | Lexeme: 'fi' | Line: 15 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 15 | Column: 6
Token: IDENTIFIER | Lexeme: 'c' | Line: 15 | Column: 7
Token: OPERATOR | Lexeme: '>' | Line: 15 | Column: 8
Token: NUMBER | Lexeme: '100' | Line: 15 | Column: 10
Token: RPAREN | Lexeme: ')' | Line: 15 | Column: 13
Token: LBRACE | Lexeme: '{' | Line: 15 | Column: 15
Token: PRINT | Lexeme: 'tnirp' | Line: 16 | Column: 9
Token: STRING | Lexeme: 'c is greater than 100' | Line: 16 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 16 | Column: 33
Token: RBRACE | Lexeme: '}' | Line: 17 | Column: 5
Token: ELSE | Lexeme: 'esle' | Line: 17 | Column: 7
Token: LBRACE | Lexeme: '{' | Line: 17 | Column: 8
Token: PRINT | Lexeme: 'tnirp' | Line: 18 | Column: 9
Token: STRING | Lexeme: 'c is less than or equal to 100' | Line: 18 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 18 | Column: 42
Token: RBRACE | Lexeme: '}' | Line: 19 | Column: 5
Token: COMMENT | Lexeme: ' While loop' | Line: 21 | Column: 5
Token: WHILE | Lexeme: 'elihw' | Line: 22 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 22 | Column: 6
Token: IDENTIFIER | Lexeme: 'a' | Line: 22 | Column: 7
Token: OPERATOR | Lexeme: '>' | Line: 22 | Column: 8
Token: NUMBER | Lexeme: '0' | Line: 22 | Column: 10
Token: RPAREN | Lexeme: ')' | Line: 22 | Column: 11
Token: LBRACE | Lexeme: '{' | Line: 22 | Column: 13
Token: PRINT | Lexeme: 'tnirp' | Line: 23 | Column: 9
Token: IDENTIFIER | Lexeme: 'a' | Line: 23 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 23 | Column: 10
Token: IDENTIFIER | Lexeme: 'a' | Line: 24 | Column: 9
Token: EQUALS | Lexeme: '=' | Line: 24 | Column: 10
Token: IDENTIFIER | Lexeme: 'a' | Line: 24 | Column: 12
Token: OPERATOR | Lexeme: '-' | Line: 24 | Column: 13
Token: NUMBER | Lexeme: '1' | Line: 24 | Column: 15
Token: SEMICOLON | Lexeme: ';' | Line: 24 | Column: 16
Token: RBRACE | Lexeme: '}' | Line: 25 | Column: 5
Token: COMMENT | Lexeme: ' Repeat-until loop' | Line: 27 | Column: 5
Token: REPEAT | Lexeme: 'taeper' | Line: 28 | Column: 5
Token: LBRACE | Lexeme: '{' | Line: 28 | Column: 6
Token: IDENTIFIER | Lexeme: 'b' | Line: 29 | Column: 9
Token: EQUALS | Lexeme: '=' | Line: 29 | Column: 10
Token: IDENTIFIER | Lexeme: 'b' | Line: 29 | Column: 12
Token: OPERATOR | Lexeme: '+' | Line: 29 | Column: 13
Token: NUMBER | Lexeme: '1' | Line: 29 | Column: 15
Token: SEMICOLON | Lexeme: ';' | Line: 29 | Column: 16
Token: PRINT | Lexeme: 'tnirp' | Line: 30 | Column: 9
Token: IDENTIFIER | Lexeme: 'b' | Line: 30 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 30 | Column: 10
Token: RBRACE | Lexeme: '}' | Line: 31 | Column: 5
Token: UNTIL | Lexeme: 'litnu' | Line: 31 | Column: 7
Token: LPAREN | Lexeme: '(' | Line: 31 | Column: 8
Token: IDENTIFIER | Lexeme: 'b' | Line: 31 | Column: 9
Token: OPERATOR | Lexeme: '>' | Line: 31 | Column: 10
Token: NUMBER | Lexeme: '25' | Line: 31 | Column: 12
Token: RPAREN | Lexeme: ')' | Line: 31 | Column: 14
Token: SEMICOLON | Lexeme: ';' | Line: 31 | Column: 15
Token: COMMENT | Lexeme: ' Factorial function' | Line: 33 | Column: 5
Token: INT | Lexeme: 'tni' | Line: 34 | Column: 5
Token: IDENTIFIER | Lexeme: 'factorial_result' | Line: 34 | Column: 6
Token: EQUALS | Lexeme: '=' | Line: 34 | Column: 7
Token: FACTORIAL | Lexeme: 'lairotcaf' | Line: 34 | Column: 9
Token: LPAREN | Lexeme: '(' | Line: 34 | Column: 9
Token: NUMBER | Lexeme: '5' | Line: 34 | Column: 10
Token: RPAREN | Lexeme: ')' | Line: 34 | Column: 11
Token: SEMICOLON | Lexeme: ';' | Line: 34 | Column: 12
Token: PRINT | Lexeme: 'tnirp' | Line: 35 | Column: 5
Token: IDENTIFIER | Lexeme: 'factorial_result' | Line: 35 | Column: 6
Token: SEMICOLON | Lexeme: ';' | Line: 35 | Column: 6
Token: COMMENT | Lexeme: ' Block statement with scoping' | Line: 37 | Column: 5
Token: LBRACE | Lexeme: '{' | Line: 38 | Column: 5
Token: INT | Lexeme: 'tni' | Line: 39 | Column: 9
Token: IDENTIFIER | Lexeme: 'x' | Line: 39 | Column: 10
Token: EQUALS | Lexeme: '=' | Line: 39 | Column: 11
Token: NUMBER | Lexeme: '50' | Line: 39 | Column: 13
Token: SEMICOLON | Lexeme: ';' | Line: 39 | Column: 15
Token: PRINT | Lexeme: 'tnirp' | Line: 40 | Column: 9
Token: IDENTIFIER | Lexeme: 'x' | Line: 40 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 40 | Column: 10
Token: RBRACE | Lexeme: '}' | Line: 41 | Column: 5
Token: COMMENT | Lexeme: ' Complex expressions with parentheses' | Line: 43 | Column: 5
Token: IDENTIFIER | Lexeme: 'c' | Line: 44 | Column: 5
Token: EQUALS | Lexeme: '=' | Line: 44 | Column: 6
Token: LPAREN | Lexeme: '(' | Line: 44 | Column: 8
Token: IDENTIFIER | Lexeme: 'a' | Line: 44 | Column: 9
Token: OPERATOR | Lexeme: '+' | Line: 44 | Column: 10
Token: IDENTIFIER | Lexeme: 'b' | Line: 44 | Column: 12
Token: RPAREN | Lexeme: ')' | Line: 44 | Column: 12
Token: OPERATOR | Lexeme: '*' | Line: 44 | Column: 14
Token: LPAREN | Lexeme: '(' | Line: 44 | Column: 16
Token: NUMBER | Lexeme: '10' | Line: 44 | Column: 17
Token: OPERATOR | Lexeme: '-' | Line: 44 | Column: 20
Token: NUMBER | Lexeme: '5' | Line: 44 | Column: 22
Token: RPAREN | Lexeme: ')' | Line: 44 | Column: 23
Token: OPERATOR | Lexeme: '/' | Line: 44 | Column: 25
Token: NUMBER | Lexeme: '2' | Line: 44 | Column: 27
Token: SEMICOLON | Lexeme: ';' | Line: 44 | Column: 28
Token: COMMENT | Lexeme: ' Comparison operators' | Line: 46 | Column: 5
Token: IF | Lexeme: 'fi' | Line: 47 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 47 | Column: 6
Token: IDENTIFIER | Lexeme: 'a' | Line: 47 | Column: 7
Token: EQUALS_EQUALS | Lexeme: '==' | Line: 47 | Column: 8
Token: NUMBER | Lexeme: '0' | Line: 47 | Column: 11
Token: LOGICAL_AND | Lexeme: '&&' | Line: 47 | Column: 13
Token: IDENTIFIER | Lexeme: 'b' | Line: 47 | Column: 16
Token: NOT_EQUALS | Lexeme: '!=' | Line: 47 | Column: 17
Token: NUMBER | Lexeme: '30' | Line: 47 | Column: 20
Token: LOGICAL_OR | Lexeme: '||' | Line: 47 | Column: 23
Token: IDENTIFIER | Lexeme: 'c' | Line: 47 | Column: 26
Token: GREATER_EQUALS | Lexeme: '>=' | Line: 47 | Column: 27
Token: NUMBER | Lexeme: '50' | Line: 47 | Column: 30
Token: RPAREN | Lexeme: ')' | Line: 47 | Column: 32
Token: LBRACE | Lexeme: '{' | Line: 47 | Column: 34
Token: PRINT | Lexeme: 'tnirp' | Line: 48 | Column: 9
Token: STRING | Lexeme: 'Complex condition is true' | Line: 48 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 48 | Column: 37
Token: RBRACE | Lexeme: '}' | Line: 49 | Column: 5
Token: RETURN | Lexeme: 'nruter' | Line: 51 | Column: 5
Token: NUMBER | Lexeme: '0' | Line: 51 | Column: 6
Token: SEMICOLON | Lexeme: ';' | Line: 51 | Column: 7
Token: RBRACE | Lexeme: '}' | Line: 52 | Column: 1
Token: EOF | Lexeme: 'EOF' | Line: 53 | Column: 1

ABSTRACT SYNTAX TREE:
Program
  Function Declaration: niam
    Block
      VarDecl: a
        Number: 10
      Block
        VarDecl: b
          Number: 20
        Block
          VarDecl: c
          Block
            Assign
              Identifier: c
              BinaryOp: +
                Identifier: a
                BinaryOp: *
                  Identifier: b
                  Number: 5
            Block
              Print Statement
                Identifier: c
              Block
                If Statement
                  BinaryOp: >
                    Identifier: c
                    Number: 100
                  Else Statement
                    Block
                      Print Statement
                        String: "c is greater than 100"
                    Block
                      Print Statement
                        String: "c is less than or equal to 100"
                Block
                  While Loop
                    BinaryOp: >
                      Identifier: a
                      Number: 0
                    Block
                      Print Statement
                        Identifier: a
                      Block
                        Assign
                          Identifier: a
                          BinaryOp: -
                            Identifier: a
                            Number: 1
                  Block
                    Repeat-Until Loop
                      Block
                        Assign
                          Identifier: b
                          BinaryOp: +
                            Identifier: b
                            Number: 1
                        Block
                          Print Statement
                            Identifier: b
                      BinaryOp: >
                        Identifier: b
                        Number: 25
                    Block
                      VarDecl: factorial_result
                        Factorial Function
                          Number: 5
                      Block
                        Print Statement
                          Identifier: factorial_result
                        Block
                          Block
                            VarDecl: x
                              Number: 50
                            Block
                              Print Statement
                                Identifier: x
                          Block
                            Assign
                              Identifier: c
                              BinaryOp: /
                                BinaryOp: *
                                  BinaryOp: +
                                    Identifier: a
                                    Identifier: b
                                  BinaryOp: -
                                    Number: 10
                                    Number: 5
                                Number: 2
                            Block
                              If Statement
                                BinaryOp: ||
                                  BinaryOp: &&
                                    BinaryOp: ==
                                      Identifier: a
                                      Number: 0
                                    BinaryOp: !=
                                      Identifier: b
                                      Number: 30
                                  BinaryOp: >=
                                    Identifier: c
                                    Number: 50
                                Block
                                  Print Statement
                                    String: "Complex condition is true"
                              Block
                                Return Statement
                                  Number: 0

Parsing completed successfully with no errors.
==============================
```

### 2.2 Invalid Input

```
==============================
PARSING FILE: ../test/input_invalid.txt
==============================
Input:
// Invalid Backwards C program example with multiple error types
tni niam(diov) {
    // Unterminated string
    tnirp "This string is not terminated;

    // Invalid escape sequence
    tnirp "Invalid escape sequence: \z";

    // Consecutive operators (+++, &&&)
    c = a +++ b;
    fi (a && &&& b) {
        tnirp "Multiple consecutive operators";
    }

    // Missing semicolon
    tni a = 10

    // Invalid expression
    tni b = 20 + ;

    // Unbalanced parentheses
    fi (c > 100 {
        tnirp "Missing closing parenthesis";
    }

    // Missing identifiers
    tni = 30;

    // Unbalanced braces in while loop
    elihw (a > 0) {
        tnirp a;
        a = a - 1;

    // Malformed repeat-until
    taeper {
        b = b + 1;
        tnirp b;
    litnu (b > 25);

    // Mismatched function call parentheses
    tni factorial_result = lairotcaf);

    // Missing conditional expression
    fi () {
        tnirp "Missing condition";
    }

    // Unterminated character literal
    rahc invalid_char = 'x;

    // Unmatched else
    esle {
        tnirp "No matching if statement";
    }

    // Incorrectly nested blocks
    {
        {
            tnirp "Unbalanced block";
    }

    // Invalid operators and syntax
    fi (a === b || c &&& d) {
        tnirp "Invalid logical operators";
    }

    // Incomplete factorial call
    lairotcaf(

    // Missing return value
    nruter;
}

// Function definition without body
tni rehtona_noitcnuf(diov);

TOKEN STREAM:
Token: COMMENT | Lexeme: ' Invalid Backwards C program example with multiple error types' | Line: 1 | Column: 1
Token: INT | Lexeme: 'tni' | Line: 2 | Column: 1
Token: IDENTIFIER | Lexeme: 'niam' | Line: 2 | Column: 2
Token: LPAREN | Lexeme: '(' | Line: 2 | Column: 2
Token: VOID | Lexeme: 'diov' | Line: 2 | Column: 3
Token: RPAREN | Lexeme: ')' | Line: 2 | Column: 3
Token: LBRACE | Lexeme: '{' | Line: 2 | Column: 5
Token: COMMENT | Lexeme: ' Unterminated string' | Line: 3 | Column: 5
Token: PRINT | Lexeme: 'tnirp' | Line: 4 | Column: 5
Lexical Error at line 4: Unterminated string literal
Token: COMMENT | Lexeme: ' Invalid escape sequence' | Line: 6 | Column: 5
Token: PRINT | Lexeme: 'tnirp' | Line: 7 | Column: 5
Lexical Error at line 7: Invalid escape sequence
Lexical Error at line 7: Unterminated string literal
Token: COMMENT | Lexeme: ' Consecutive operators (+++, &&&)' | Line: 9 | Column: 5
Token: IDENTIFIER | Lexeme: 'c' | Line: 10 | Column: 5
Token: EQUALS | Lexeme: '=' | Line: 10 | Column: 6
Token: IDENTIFIER | Lexeme: 'a' | Line: 10 | Column: 8
Token: OPERATOR | Lexeme: '+' | Line: 10 | Column: 9
Lexical Error at line 10, column 10: Consecutive operators not allowed
Lexical Error at line 10: Consecutive operators not allowed
Token: SEMICOLON | Lexeme: ';' | Line: 10 | Column: 14
Token: IF | Lexeme: 'fi' | Line: 11 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 11 | Column: 6
Token: IDENTIFIER | Lexeme: 'a' | Line: 11 | Column: 7
Token: LOGICAL_AND | Lexeme: '&&' | Line: 11 | Column: 8
Token: LOGICAL_AND | Lexeme: '&&' | Line: 11 | Column: 11
Lexical Error at line 11, column 13: Consecutive operators not allowed
Lexical Error at line 11: Consecutive operators not allowed
Token: PRINT | Lexeme: 'tnirp' | Line: 12 | Column: 9
Token: STRING | Lexeme: 'Multiple consecutive operators' | Line: 12 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 12 | Column: 42
Token: RBRACE | Lexeme: '}' | Line: 13 | Column: 5
Token: COMMENT | Lexeme: ' Missing semicolon' | Line: 15 | Column: 5
Token: INT | Lexeme: 'tni' | Line: 16 | Column: 5
Token: IDENTIFIER | Lexeme: 'a' | Line: 16 | Column: 6
Token: EQUALS | Lexeme: '=' | Line: 16 | Column: 7
Token: NUMBER | Lexeme: '10' | Line: 16 | Column: 9
Token: COMMENT | Lexeme: ' Invalid expression' | Line: 18 | Column: 5
Token: INT | Lexeme: 'tni' | Line: 19 | Column: 5
Token: IDENTIFIER | Lexeme: 'b' | Line: 19 | Column: 6
Token: EQUALS | Lexeme: '=' | Line: 19 | Column: 7
Token: NUMBER | Lexeme: '20' | Line: 19 | Column: 9
Lexical Error at line 19, column 12: Consecutive operators not allowed
Lexical Error at line 19: Consecutive operators not allowed
Token: SEMICOLON | Lexeme: ';' | Line: 19 | Column: 14
Token: COMMENT | Lexeme: ' Unbalanced parentheses' | Line: 21 | Column: 5
Token: IF | Lexeme: 'fi' | Line: 22 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 22 | Column: 6
Token: IDENTIFIER | Lexeme: 'c' | Line: 22 | Column: 7
Token: OPERATOR | Lexeme: '>' | Line: 22 | Column: 8
Token: NUMBER | Lexeme: '100' | Line: 22 | Column: 10
Token: LBRACE | Lexeme: '{' | Line: 22 | Column: 14
Token: PRINT | Lexeme: 'tnirp' | Line: 23 | Column: 9
Token: STRING | Lexeme: 'Missing closing parenthesis' | Line: 23 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 23 | Column: 39
Token: RBRACE | Lexeme: '}' | Line: 24 | Column: 5
Token: COMMENT | Lexeme: ' Missing identifiers' | Line: 26 | Column: 5
Token: INT | Lexeme: 'tni' | Line: 27 | Column: 5
Token: EQUALS | Lexeme: '=' | Line: 27 | Column: 6
Token: NUMBER | Lexeme: '30' | Line: 27 | Column: 8
Token: SEMICOLON | Lexeme: ';' | Line: 27 | Column: 10
Token: COMMENT | Lexeme: ' Unbalanced braces in while loop' | Line: 29 | Column: 5
Token: WHILE | Lexeme: 'elihw' | Line: 30 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 30 | Column: 6
Token: IDENTIFIER | Lexeme: 'a' | Line: 30 | Column: 7
Token: OPERATOR | Lexeme: '>' | Line: 30 | Column: 8
Token: NUMBER | Lexeme: '0' | Line: 30 | Column: 10
Token: RPAREN | Lexeme: ')' | Line: 30 | Column: 11
Token: LBRACE | Lexeme: '{' | Line: 30 | Column: 13
Token: PRINT | Lexeme: 'tnirp' | Line: 31 | Column: 9
Token: IDENTIFIER | Lexeme: 'a' | Line: 31 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 31 | Column: 10
Token: IDENTIFIER | Lexeme: 'a' | Line: 32 | Column: 9
Token: EQUALS | Lexeme: '=' | Line: 32 | Column: 10
Token: IDENTIFIER | Lexeme: 'a' | Line: 32 | Column: 12
Token: OPERATOR | Lexeme: '-' | Line: 32 | Column: 13
Token: NUMBER | Lexeme: '1' | Line: 32 | Column: 15
Token: SEMICOLON | Lexeme: ';' | Line: 32 | Column: 16
Token: COMMENT | Lexeme: ' Malformed repeat-until' | Line: 34 | Column: 5
Token: REPEAT | Lexeme: 'taeper' | Line: 35 | Column: 5
Token: LBRACE | Lexeme: '{' | Line: 35 | Column: 6
Token: IDENTIFIER | Lexeme: 'b' | Line: 36 | Column: 9
Token: EQUALS | Lexeme: '=' | Line: 36 | Column: 10
Token: IDENTIFIER | Lexeme: 'b' | Line: 36 | Column: 12
Token: OPERATOR | Lexeme: '+' | Line: 36 | Column: 13
Token: NUMBER | Lexeme: '1' | Line: 36 | Column: 15
Token: SEMICOLON | Lexeme: ';' | Line: 36 | Column: 16
Token: PRINT | Lexeme: 'tnirp' | Line: 37 | Column: 9
Token: IDENTIFIER | Lexeme: 'b' | Line: 37 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 37 | Column: 10
Token: UNTIL | Lexeme: 'litnu' | Line: 38 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 38 | Column: 6
Token: IDENTIFIER | Lexeme: 'b' | Line: 38 | Column: 7
Token: OPERATOR | Lexeme: '>' | Line: 38 | Column: 8
Token: NUMBER | Lexeme: '25' | Line: 38 | Column: 10
Token: RPAREN | Lexeme: ')' | Line: 38 | Column: 12
Token: SEMICOLON | Lexeme: ';' | Line: 38 | Column: 13
Token: COMMENT | Lexeme: ' Mismatched function call parentheses' | Line: 40 | Column: 5
Token: INT | Lexeme: 'tni' | Line: 41 | Column: 5
Token: IDENTIFIER | Lexeme: 'factorial_result' | Line: 41 | Column: 6
Token: EQUALS | Lexeme: '=' | Line: 41 | Column: 7
Token: FACTORIAL | Lexeme: 'lairotcaf' | Line: 41 | Column: 9
Token: RPAREN | Lexeme: ')' | Line: 41 | Column: 9
Token: SEMICOLON | Lexeme: ';' | Line: 41 | Column: 10
Token: COMMENT | Lexeme: ' Missing conditional expression' | Line: 43 | Column: 5
Token: IF | Lexeme: 'fi' | Line: 44 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 44 | Column: 6
Token: RPAREN | Lexeme: ')' | Line: 44 | Column: 7
Token: LBRACE | Lexeme: '{' | Line: 44 | Column: 9
Token: PRINT | Lexeme: 'tnirp' | Line: 45 | Column: 9
Token: STRING | Lexeme: 'Missing condition' | Line: 45 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 45 | Column: 29
Token: RBRACE | Lexeme: '}' | Line: 46 | Column: 5
Token: COMMENT | Lexeme: ' Unterminated character literal' | Line: 48 | Column: 5
Token: CHAR | Lexeme: 'rahc' | Line: 49 | Column: 5
Token: IDENTIFIER | Lexeme: 'invalid_char' | Line: 49 | Column: 6
Token: EQUALS | Lexeme: '=' | Line: 49 | Column: 7
Lexical Error at line 49: Multi-character literal not allowed
Token: COMMENT | Lexeme: ' Unmatched else' | Line: 51 | Column: 5
Token: ELSE | Lexeme: 'esle' | Line: 52 | Column: 5
Token: LBRACE | Lexeme: '{' | Line: 52 | Column: 6
Token: PRINT | Lexeme: 'tnirp' | Line: 53 | Column: 9
Token: STRING | Lexeme: 'No matching if statement' | Line: 53 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 53 | Column: 36
Token: RBRACE | Lexeme: '}' | Line: 54 | Column: 5
Token: COMMENT | Lexeme: ' Incorrectly nested blocks' | Line: 56 | Column: 5
Token: LBRACE | Lexeme: '{' | Line: 57 | Column: 5
Token: LBRACE | Lexeme: '{' | Line: 58 | Column: 9
Token: PRINT | Lexeme: 'tnirp' | Line: 59 | Column: 13
Token: STRING | Lexeme: 'Unbalanced block' | Line: 59 | Column: 14
Token: SEMICOLON | Lexeme: ';' | Line: 59 | Column: 32
Token: RBRACE | Lexeme: '}' | Line: 60 | Column: 5
Token: COMMENT | Lexeme: ' Invalid operators and syntax' | Line: 62 | Column: 5
Token: IF | Lexeme: 'fi' | Line: 63 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 63 | Column: 6
Token: IDENTIFIER | Lexeme: 'a' | Line: 63 | Column: 7
Token: EQUALS_EQUALS | Lexeme: '==' | Line: 63 | Column: 8
Token: EQUALS | Lexeme: '=' | Line: 63 | Column: 10
Token: IDENTIFIER | Lexeme: 'b' | Line: 63 | Column: 12
Token: LOGICAL_OR | Lexeme: '||' | Line: 63 | Column: 13
Token: IDENTIFIER | Lexeme: 'c' | Line: 63 | Column: 16
Token: LOGICAL_AND | Lexeme: '&&' | Line: 63 | Column: 17
Lexical Error at line 63, column 19: Consecutive operators not allowed
Lexical Error at line 63: Consecutive operators not allowed
Token: PRINT | Lexeme: 'tnirp' | Line: 64 | Column: 9
Token: STRING | Lexeme: 'Invalid logical operators' | Line: 64 | Column: 10
Token: SEMICOLON | Lexeme: ';' | Line: 64 | Column: 37
Token: RBRACE | Lexeme: '}' | Line: 65 | Column: 5
Token: COMMENT | Lexeme: ' Incomplete factorial call' | Line: 67 | Column: 5
Token: FACTORIAL | Lexeme: 'lairotcaf' | Line: 68 | Column: 5
Token: LPAREN | Lexeme: '(' | Line: 68 | Column: 5
Token: COMMENT | Lexeme: ' Missing return value' | Line: 70 | Column: 5
Token: RETURN | Lexeme: 'nruter' | Line: 71 | Column: 5
Token: SEMICOLON | Lexeme: ';' | Line: 71 | Column: 5
Token: RBRACE | Lexeme: '}' | Line: 72 | Column: 1
Token: COMMENT | Lexeme: ' Function definition without body' | Line: 74 | Column: 1
Token: INT | Lexeme: 'tni' | Line: 75 | Column: 1
Token: IDENTIFIER | Lexeme: 'rehtona_noitcnuf' | Line: 75 | Column: 2
Token: LPAREN | Lexeme: '(' | Line: 75 | Column: 2
Token: VOID | Lexeme: 'diov' | Line: 75 | Column: 3
Token: RPAREN | Lexeme: ')' | Line: 75 | Column: 3
Token: SEMICOLON | Lexeme: ';' | Line: 75 | Column: 4
Token: EOF | Lexeme: 'EOF' | Line: 75 | Column: 5
Parse Error at line 7, column 5: Missing semicolon after 'tnirp'
Parse Error at line 7, column 34: Missing semicolon after ';'
Lexical Error at line 10, column 10: Consecutive operators not allowed
Parse Error at line 10, column 14: Invalid expression after ';'
Parse Error at line 11, column 5: Missing semicolon after 'fi'
Parse Error at line 11, column 11: Invalid expression after '&&'
Lexical Error at line 11, column 13: Consecutive operators not allowed
Parse Error at line 11, column 5: Missing parenthesis in expression
Parse Error at line 12, column 9: Missing brace for block statement
Parse Error at line 19, column 5: Missing semicolon after 'tni'
Lexical Error at line 19, column 18: Consecutive operators not allowed
Parse Error at line 22, column 5: Missing parenthesis in expression
Parse Error at line 27, column 5: Expected identifier after 'tni'
Parse Error at line 38, column 5: Unexpected token 'litnu'
Parse Error at line 38, column 7: Expected '=' after 'b'
Parse Error at line 41, column 12: Missing parenthesis in expression
Parse Error at line 44, column 5: Expected condition after 'fi'
Parse Error at line 49, column 12: Invalid expression after 'x'
Parse Error at line 52, column 5: Missing semicolon after 'esle'
Parse Error at line 63, column 10: Invalid expression after '='
Parse Error at line 63, column 5: Missing parenthesis in expression
Parse Error at line 63, column 12: Missing brace for block statement
Parse Error at line 63, column 16: Expected '=' after 'c'
Lexical Error at line 63, column 19: Consecutive operators not allowed
Parse Error at line 71, column 5: Invalid expression after 'nruter'
Parse Error at line 72, column 1: Missing parenthesis in expression
Parse Error at line 75, column 1: Missing semicolon after 'tni'
Parse Error at line 75, column 4: Missing brace for block statement
Parse Error at line 35, column 6: Missing brace for block statement
Parse Error at line 30, column 13: Missing brace for block statement

ABSTRACT SYNTAX TREE:
Program
  Function Declaration: niam
    Block
      Print Statement
        String: "This string is not terminated;"
      Block
        Print Statement
          String: "Invalid escape sequence: "
        Block
          Program
          Block
            Assign
              Identifier: c
              BinaryOp: +
                Identifier: a
                Number: 0
            Block
              If Statement
                BinaryOp: &&
                  Identifier: a
                  Number: 0
                Block
              Block
                Print Statement
                  String: "Multiple consecutive operators"
  Program
    VarDecl: a
      Number: 10
    Program
      VarDecl: b
        Number: 20
      Program
        If Statement
          BinaryOp: >
            Identifier: c
            Number: 100
          Block
            Print Statement
              String: "Missing closing parenthesis"
        Program
          VarDecl: tni
          Program
            While Loop
              BinaryOp: >
                Identifier: a
                Number: 0
              Block
                Print Statement
                  Identifier: a
                Block
                  Assign
                    Identifier: a
                    BinaryOp: -
                      Identifier: a
                      Number: 1
                  Block
                    Repeat-Until Loop
                      Block
                        Assign
                          Identifier: b
                          BinaryOp: +
                            Identifier: b
                            Number: 1
                        Block
                          Print Statement
                            Identifier: b
                          Block
                            Program
                            Block
                              Program
                              Block
                                VarDecl: factorial_result
                                  Factorial Function
                                Block
                                  If Statement
                                    Number: 0
                                    Block
                                      Print Statement
                                        String: "Missing condition"
                                  Block
                                    VarDecl: invalid_char
                                      Number: 0
                                    Block
                                      Program
                                      Block
                                        Block
                                          Block
                                            Print Statement
                                              String: "Unbalanced block"
                                          Block
                                            If Statement
                                              BinaryOp: ==
                                                Identifier: a
                                                Number: 0
                                              Block
                                            Block
                                              Program
                                              Block
                                                Program
                                                Block
                                                  Print Statement
                                                    String: "Invalid logical operators"
                                        Block
                                          Factorial Function
                                            Number: 0
                                          Block
                                            Function Declaration: rehtona_noitcnuf

Parsing completed with 26 errors.
==============================
```