# Mini Language Parser

A simple imperative programming language parser with AST construction and execution capabilities.

## Overview

This project implements a complete lexer and parser for a mini programming language using Flex and Bison. The parser constructs an Abstract Syntax Tree (AST) during parsing and includes an interpreter to execute the parsed programs. The implementation supports variable declarations, arithmetic/comparison expressions, and control flow structures (if-else, while loops).

## Build Instructions

Make sure you have the following installed on your system:
- g++ (C++ compiler)
- flex (lexical analyzer)
- bison (parser generator)
- make

To build the project, simply run:

```bash
make
```

This will generate the parser executable at `build/parser`.

To clean up all generated files:

```bash
make clean
```

## Running the Parser

The parser reads input from standard input (stdin). You can use it in two ways:

### Option 1: Interactive Input
```bash
./build/parser
```
Then type your program and press Ctrl+D (EOF) when done.

### Option 2: File Input (Recommended)
```bash
./build/parser < test/test1.txt
```

## Running Tests

We've included 15 test cases in the `test/` directory covering various language features.

To run a specific test:
```bash
./build/parser < test/test1.txt
```

To run all tests, you can use a simple loop:
```bash
for i in {1..15}; do
    echo "=== Running test$i.txt ==="
    ./build/parser < test/test$i.txt
    echo ""
done
```

### Test Coverage

Our test cases cover:
- Variable declarations (test1, test2)
- Assignments (test3)
- Arithmetic expressions (test4)
- Comparison operators (test5)
- If statements without else (test6)
- If-else statements (test7)
- While loops (test8)
- Nested control structures and blocks (test9, test15)
- Invalid cases like missing semicolons, undefined variables, etc. (test10-14)

## Language Features

The parser supports:

### 1. Variable Declaration
```
var x;
var y = 10;
```

### 2. Assignment
```
x = 5;
y = x + 3;
```

### 3. Arithmetic Operators
- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- Unary minus: `-x`

### 4. Comparison Operators
- Equal to: `==`
- Not equal to: `!=`
- Less than: `<`
- Greater than: `>`
- Less than or equal: `<=`
- Greater than or equal: `>=`

### 5. Control Flow
**If Statement:**
```
if (x > 5)
    y = 10;

if (x > 5)
    y = 10;
else
    y = 20;
```

**While Loop:**
```
while (i < 10)
    i = i + 1;
```

### 6. Block Statements
```
{
    var x = 5;
    var y = 10;
    x = x + y;
}
```

## Output Format

The parser produces three main outputs:

1. **Parse Status** - Indicates whether parsing started and finished successfully
2. **Abstract Syntax Tree** - A tree representation of the parsed program
3. **Symbol Table** - Final values of all variables after execution

Example output:
```
Parsing started.......
Parsing finished.

==== Abstract Syntax Tree ====
VarDeclInitStmt("x")
  IntExpr(10)

---- Symbol Table ----
x = 10
```

## Assumptions and Design Decisions

1. **Variables must be declared before use** - We enforce strict variable declaration. Using an undeclared variable results in a runtime error.

2. **Integer-only arithmetic** - The language only supports integer values. No floating-point numbers.

3. **Division by zero** - Handled as a runtime error with proper error message.

4. **No variable scoping** - All variables share a global scope. Variables declared inside blocks are accessible outside.

5. **Operator Precedence** - Follows standard mathematical precedence:
   - Highest: Unary minus
   - Multiplication and Division
   - Addition and Subtraction
   - Comparison operators
   - Lowest: Equality operators

6. **Dangling Else Problem** - Resolved using Bison's precedence rules. The `else` associates with the nearest `if`.

7. **Comment Support** - Single-line comments using `//` are supported in the lexer.

## Known Limitations

- No functions or procedures
- No arrays or complex data types
- No string support
- Variables cannot be redeclared (but can be reassigned)
- No break/continue statements in loops
- Block statements don't create new scopes

## Error Handling

### Syntax Errors
Detected during parsing phase. Example:
```
var x = 10    // Missing semicolon
```
Output: `Syntax error: ...`

### Runtime Errors
Detected during execution. Examples:
- Using undefined variables
- Division by zero

These produce error messages but don't crash the program.

## Project Structure

```
mini_lang/
├── src/
│   ├── lexer.l          # Flex lexer specification
│   ├── parser.y         # Bison parser grammar with main()
│   ├── ast.cpp          # AST execution and printing logic
│   └── ast.hpp          # AST node definitions
├── test/
│   └── test*.txt        # Test programs (15 tests)
├── Makefile             # Build configuration
└── README.md            # This file
```

## Implementation

### Lexer (src/lexer.l)
Tokenizes the input using regular expressions. Handles keywords (var, if, else, while), operators, identifiers, integers, and comments.

### Parser (src/parser.y)
Implements the grammar rules and builds AST nodes during parsing. Uses Bison's precedence directives to handle operator precedence and the dangling-else problem. The main() function is included here, which orchestrates parsing, AST printing, and execution.

### AST (src/ast.cpp & src/ast.hpp)
- **ast.hpp**: Defines all AST node structures (expressions and statements)
- **ast.cpp**: Implements execution logic (evalExpr, execStmt), symbol table management, and AST pretty-printing functions

### Execution Flow
1. Lexer tokenizes input -> Parser builds AST
2. AST is printed for debugging
3. AST is executed using a tree-walking interpreter
4. Symbol table (final variable values) is displayed

## Files Generated During Build

When we run `make`, the following files are generated in the `build/` directory:
- `parser.tab.c` / `parser.tab.h` - Generated parser from Bison
- `lex.yy.c` - Generated lexer from Flex
- `*.o` - Object files
- `parser` - Final executable


## Additional Notes

- The parser includes an AST pretty-printer that shows the tree structure before execution, which is helpful for debugging and understanding how our program is parsed
- We've tested the parser with all 15 test cases and it handles both valid and invalid programs correctly
- The implementation follows standard compiler design principles with clear separation between lexing, parsing, and execution phases

