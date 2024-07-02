CBULang

1 phase
An interpreter for mathematical and logical expressions, capable of evaluating expressions with arithmetic, logical, and comparison operators.
Features

    Arithmetic Operations: +, -, *, /, ^
    Logical Operators: and, or, not, xor
    Comparison Operators: ==, !=, <, >, <=, >=
    Supported Types: Integers, floats, booleans, strings

program         → "program" IDENTIFIER ";" function* mainBlock

function        → "function" IDENTIFIER "(" parameters? ")" ":" type ";" block

parameters      → parameter ("," parameter)*
parameter       → IDENTIFIER ":" type

mainBlock       → "begin" declaration* statement* "end"

block           → "begin" declaration* statement* "end"

declaration     → varDeclaration
                | functionDeclaration

varDeclaration  → type IDENTIFIER ("," IDENTIFIER)* ";"

type            → "int" | "float" | "string" | IDENTIFIER

statement       → expressionStmt
                | ifStmt
                | whileStmt
                | returnStmt
                | block

expressionStmt  → expression ";"
ifStmt          → "if" "(" expression ")" statement ("else" statement)?
whileStmt       → "while" "(" expression ")" statement
returnStmt      → "return" expression? ";"

expression      → assignment

assignment      → IDENTIFIER "=" assignment
                | logicOr

logicOr         → logicAnd ("or" logicAnd)*

logicAnd        → equality ("and" equality)*

equality        → comparison (("==" | "!=") comparison)*

comparison      → term ((">" | ">=" | "<" | "<=") term)*

term            → factor (("+" | "-") factor)*

factor          → power (("*" | "/" | "%") power)*

power           → unary ("^" unary)*

unary           → ("!" | "-") unary
                | primary

primary         → NUMBER | STRING | "true" | "false" | "(" expression ")"
                | IDENTIFIER
                | IDENTIFIER "(" arguments? ")"

arguments       → expression ("," expression)*


2 phase Variables 



3 phase functions
