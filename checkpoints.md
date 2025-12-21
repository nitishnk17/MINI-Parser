Day 1 : Tokenization
            Project structure, Flex lexer implementation, All token defined, comment handling

// lexer.l   --(Flex) -->   lex.yy.c   --(C++ compiler) -->  lexer executable


Day 2 : Grammar and Parser
            Bison parser, Grammar rules, Expression hierarachy with precedence, Integration with above

Day 3 : AST Construction
            AST class hierarchy design, Expression and statement nodes, Smart pointer memory management, AST visualization

Day 4 : Evaluation
            Symbol table, Expression evaluator, Statement evaluator, Error Handling
            
Day 5 : Testing and Documentation
            Test and report



/*
Flex:

Reads lexer.l
Converts your rules into a real C/C++ program 
Outputs that program as lex.yy.c

*/

What Is Inside lex.yy.c?

lex.yy.c contains:

    A function called yylex()
    A big state machine

Code that:

    Reads input character-by-character

    Matches patterns efficiently

    Executes our { ... } actions

------------------------------------------- 
lexer.l = Blueprint

lex.yy.c = Machine built from blueprint

lexer = Running machine
-------------------------------------------

Day 1- source code -> flex -> Tokens 

Day 2 - source code -> flex -> tokens -> Bison -> parsed structure

Bison'e job -> Do these tokens form a valid program according to my grammar?

