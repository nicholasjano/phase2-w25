# Compiler Parser Phase

## Parser Overview

This parser is the second phase for the "Backwards C" compiler. It takes the token stream from the lexical analyzer (Phase 1, but also updated here) and transforms it into an Abstract Syntax Tree (AST) representation of the program. Our parser uses a recursive descent approach with precedence climbing for expressions, allowing it to correctly handle operator precedence and nested constructs.

Key components of the parser implementation include:
- Token consumption and matching functions
- AST node creation and management
- Expression parsing with proper precedence handling
- Statement and declaration parsing
- Error detection, reporting, and recovery mechanisms

## Features

- **Expression Parsing**
  - Full arithmetic operator support (+, -, *, /)
  - Comparison operators (<, >, ==, !=, <=, >=)
  - Logical operators (&&, ||)
  - Proper operator precedence handling
  - Parenthesized expressions for grouping

- **Statement Types**
  - Variable declarations with optional initialization
  - Assignment statements
  - Conditional statements (if-else)
  - Iteration constructs (while loops, repeat-until loops)
  - Block statements with proper scoping
  - Function declarations with parameter handling
  - Print statements
  - Return statements

- **Special Language Features**
  - Factorial function support as a built-in operation
  - Block-level variable scoping
  - Support for various data types (int, float, char, void)

- **Error Handling**
  - Detailed error messages with line and column information
  - Synchronization-based error recovery to continue parsing after errors
  - Prevention of cascading errors
  - Support for common error patterns in user code
  - Error count tracking and summary reporting

- **AST Generation and Visualization**
  - Creation of a comprehensive Abstract Syntax Tree
  - Tree visualization for debugging
  - Memory management for AST nodes
  - Support for complex nested structures

## Language Syntax Supported

### Basic Program Structure
```c
// Main function declaration
tni niam(diov) {
    // Statements go here
    nruter 0;
}
```

### Variable Declarations and Assignments
```c
// Variable declarations
tni a;                  // Simple declaration
tni b = 10;             // Declaration with initialization
rahc c = 'x';           // Character variable
taolf d = 3.14;         // Floating point variable

// Assignments
a = 5;                  // Simple assignment
b = a + 10;             // Assignment with expression
```

### Expressions
```c
// Arithmetic operations
a = b + c;              // Addition
x = y - z;              // Subtraction
p = q * r;              // Multiplication
m = n / o;              // Division

// Comparison operations
fi (a > b) { }          // Greater than
fi (x <= y) { }         // Less than or equal
fi (p == q) { }         // Equality
fi (m != n) { }         // Inequality

// Logical operations
fi (a > b && x < y) { } // Logical AND
fi (p == q || m != n) { } // Logical OR

// Parenthesized expressions
result = (a + b) * (c - d);
```

### Control Structures
```c
// If-Else statement
fi (condition) {
    // Then branch
} esle {
    // Else branch
}

// While loop
elihw (condition) {
    // Loop body
}

// Repeat-Until loop
taeper {
    // Loop body - executes at least once
} litnu (condition);
```

### Functions
```c
// Function declaration
tni add(tni a, tni b) {
    nruter a + b;
}

// Function call
tni result = add(5, 10);
```

### Special Features
```c
// Factorial function
tni fact = lairotcaf(5);  // Computes 5!

// Block scoping
{
    tni local_var = 42;  // Only accessible within this block
    tnirp local_var;
}
// local_var not accessible here

// Print statement
tnirp "Hello, world!";
tnirp x + y;
```