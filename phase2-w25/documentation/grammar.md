# Backwards C Grammar Specification

## 1. Lexical Grammar

### 1.1 Basic Elements

```bnf
letter     ::= [a-zA-Z]
digit      ::= [0-9]
whitespace ::= ' ' | '\t' | '\n'
```

### 1.2 Identifiers

```bnf
identifier ::= (letter | '_') (letter | digit | '_')*
```

Examples:

- Valid: `counter`, `myVar_1`, `_temp`
- Invalid: `1var`, `my@var`, `if`

### 1.3 Keywords

All C keywords reversed:

```
Original -> Reversed
if       -> fi
int      -> tni
char     -> rahc
void     -> diov
return   -> nruter
for      -> rof
while    -> elihw
do       -> od
break    -> kaerb
continue -> eunitnoc
switch   -> hctiws
case     -> esac
...etc
```

### 1.4 Numbers

```bnf
number    ::= integer | float
integer   ::= digit+
float     ::= digit+ '.' digit+
```

Examples:

- Valid: `123`, `3.14`, `0.123`
- Invalid: `123.`, `.123`, `1.2.3`

### 1.5 Characters

```bnf
char_literal ::= '\'' char_char '\''
char_char    ::= escape_seq | any_char_except_quote_or_newline
escape_seq   ::= '\\' ('n' | 't' | 'r' | '0' | '\\' | '\'' | '\"')
```

Examples:

- Valid: `'a'`, `'\n'`, `'\t'`
- Invalid: `'ab'`, `''`, `'`

### 1.6 Strings

```bnf
string      ::= '"' string_char* '"'
string_char ::= escape_seq | any_char_except_quote_or_newline
```

Examples:

- Valid: `"Hello"`, `"Line\n"`, `"Tab\there"`
- Invalid: `"Unterminated`, `"Invalid\k"`

### 1.7 Operators

```bnf
operator    ::= arithmetic | relational | logical | assignment
arithmetic  ::= '+' | '-' | '*' | '/'
relational  ::= '==' | '!=' | '<' | '>' | '<=' | '>='
logical     ::= '&&' | '||' | '!'
assignment  ::= '='
```

### 1.8 Delimiters

```bnf
delimiter ::= '(' | ')' | '{' | '}' | '[' | ']' | ';' | ','
```

### 1.9 Comments

```bnf
comment ::= '//' any_char* newline
```

## 2. Token Patterns

### 2.1 Regular Expressions

```
IDENTIFIER:      [a-zA-Z_][a-zA-Z0-9_]*
INTEGER:         [0-9]+
FLOAT:           [0-9]+\.[0-9]+
OPERATOR:        [+\-*/=<>!]=?|&&|\|\|
EQUALS_EQUALS:   ==
NOT_EQUALS:      !=
LOGICAL_AND:     &&
LOGICAL_OR:      ||
GREATER_EQUALS:  >=
LESS_EQUALS:     <=
EQUALS:          =
DELIMITER:       [()\[\]{},;]
SEMICOLON:       ;
LPAREN:          (
RPAREN:          )
LBRACE:          {
RBRACE:          }
COMMENT:         //.*\n
```

### 2.2 Precedence Rules

1. Keywords take precedence over identifiers
2. Longest match for operators
3. Comments consume to end of line
4. Whitespace is ignored except as separator

## 3. Syntactic Grammar (Parser Rules)

### 3.1 Program Structure

```bnf
Program         → Statement* EOF
Statement       → Declaration | Assignment | IfStatement | WhileStatement | 
                  RepeatUntilStatement | PrintStatement | ReturnStatement | 
                  BlockStatement | FunctionDeclaration
```

### 3.2 Declarations and Assignments

```bnf
Declaration     → Type Identifier ("=" Expression)? ";"
Assignment      → Identifier "=" Expression ";"
Type            → "tni" | "rahc" | "taolf" | "diov"  
```

### 3.3 Control Flow Statements

```bnf
IfStatement     → "fi" "(" Expression ")" BlockStatement ("esle" BlockStatement)?
WhileStatement  → "elihw" "(" Expression ")" BlockStatement
RepeatUntilStatement → "taeper" BlockStatement "litnu" "(" Expression ")" ";"
```

### 3.4 Other Statements

```bnf
PrintStatement  → "tnirp" Expression ";"
ReturnStatement → "nruter" Expression ";"
BlockStatement  → "{" Statement* "}"
```

### 3.5 Function-Related

```bnf
FunctionDeclaration → Type Identifier "(" ParameterList? ")" BlockStatement
ParameterList   → Parameter ("," Parameter)*
Parameter       → Type Identifier
FunctionCall    → Identifier "(" (Expression ("," Expression)*)? ")"
FactorialCall   → "lairotcaf" "(" Expression ")"
```

### 3.6 Expression Grammar (With Precedence)

```bnf
Expression      → LogicalOrExpression
LogicalOrExpression → LogicalAndExpression ("||" LogicalAndExpression)*
LogicalAndExpression → ComparisonExpression ("&&" ComparisonExpression)*
ComparisonExpression → AdditiveExpression (("=="|"!="|"<"|">"|"<="|">=") AdditiveExpression)*
AdditiveExpression → MultiplicativeExpression (("+"|"-") MultiplicativeExpression)*
MultiplicativeExpression → PrimaryExpression (("*"|"/") PrimaryExpression)*
PrimaryExpression → Identifier | Number | String | "(" Expression ")" | FunctionCall | FactorialCall
```

## 4. AST Structure

### 4.1 Node Types

```
AST_PROGRAM        - Root node of the program
AST_VARDECL        - Variable declaration (tni x;)
AST_ASSIGN         - Assignment statement (x = 5;)
AST_PRINT          - Print statement (tnirp x;)
AST_NUMBER         - Numeric literal
AST_STRING         - String literal
AST_OPERATOR       - Operators
AST_IDENTIFIER     - Variable names
AST_IF             - If statement (fi)
AST_ELSE           - Else clause (esle)
AST_WHILE          - While loop (elihw)
AST_FOR            - Repeat-until loop (taeper/litnu)
AST_BLOCK          - Block of statements
AST_BINOP          - Binary operation
AST_FACTORIAL      - Factorial function
AST_FUNCTION_CALL  - Generic function call
AST_RETURN         - Return statement
AST_FUNCTION_DECL  - Function declaration
```

### 4.2 Node Structure Examples

**If-Else Statement:**
```
AST_IF
├── left: Condition expression
└── right: AST_ELSE
            ├── left: Then-block
            └── right: Else-block
```

**Repeat-Until Loop:**
```
AST_FOR (reused for repeat-until)
├── left: Loop body
└── right: Until condition
```

**Binary Operation:**
```
AST_BINOP (token = operator)
├── left: Left operand
└── right: Right operand
```

## 5. Complete Program Example

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

## 6. Operator Precedence Table

| Precedence | Operators            | Associativity | Grammar Rule              |
|------------|----------------------|---------------|---------------------------|
| 1 (highest)| (), function calls   | Left to right | PrimaryExpression         |
| 2          | *, /                 | Left to right | MultiplicativeExpression  |
| 3          | +, -                 | Left to right | AdditiveExpression        |
| 4          | <, >, <=, >=, ==, != | Left to right | ComparisonExpression      |
| 5          | &&                   | Left to right | LogicalAndExpression      |
| 6 (lowest) | \|\|                 | Left to right | LogicalOrExpression       |

## 7. Special Features

### 7.1 Factorial Function

The factorial function is a built-in special case that is handled differently from regular function calls:

```c
tni x = lairotcaf(5);  // Computes 5!
```

### 7.2 Block Scoping

Variables declared within a block are local to that block:

```c
{
    tni local_var = 42;  // Only accessible within this block
    tnirp local_var;
}
// local_var not accessible here
```

### 7.3 Repeat-Until Loop

The repeat-until loop executes the body at least once, then checks the condition:

```c
taeper {
    // Loop body (executed at least once)
} litnu (condition);  // Loop until condition is true
```