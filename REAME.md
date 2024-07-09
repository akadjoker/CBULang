BUcLang is a hybrid programming language that combines elements of Pascal, Python, and C. Below are the features and constructs supported by the language so far.


### Operators

- **Arithmetic**: Operations: +, -, *, /, ^, %
- **Logical**: Operators: and, or, not, xor
- **Comparison**: Operators: ==, !=, <, >, <=, >=

- **Increment and Decrement**:
  - Pre-increment and pre-decrement: `++var`, `--var`.
  - Post-increment and post-decrement: `var++`, `var--`.



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

type            → "int" | "float" | "string" | bool | IDENTIFIER

statement       → expressionStmt
                | ifStmt
                | whileStmt
                | returnStmt
                | block
                | repeat


expressionStmt  → expression ";"
ifStmt          → "if" "(" expression ")" statement ("else" statement)?
whileStmt       → "while" "(" expression ")" statement
returnStmt      → "return" expression? ";"

expression      → assignment

assignment      → IDENTIFIER ("="| "+=" | "*=" | "/=" | "-=") assignment
                | logicOr

logicOr         → logicAnd ("or" logicAnd)*

logicAnd        → equality ("and" equality)*

equality        → comparison (("==" | "!=") comparison)*

comparison      → term ((">" | ">=" | "<" | "<=") term)*

term            → factor (("+" | "-") factor)*

factor          → power (("*" | "/" | "%") power)*

power           → unary ("^" unary)*

unary           → ("!" | "-" | not | "++" | "--" ) unary
                | primary

primary         → float | STRING | "true" | "false" | "(" expression ")"
                | IDENTIFIER
                | IDENTIFIER "(" arguments? ")"

arguments       → expression ("," expression)*


## Supported Features

### Program Structure

- **Program Definition**: Every program starts with the `program` keyword followed by the program name.
- **Main Block**: The main block is enclosed by `begin` and `end.`.

### Data Types

- `byte`:  unsigned char as internal
- `float`:  double as internal
- `int`:  long as internal
- `bool`: Boolean values (`true` or `false`).
- `string`: Sequences of characters.

### Variables

- Declaration and initialization of variables:
  ```plaintext
  float a = 50;
  float start_time = now;
  bool flag = true;
  string msg = "Hello, World!";
  ```

### Procedures and Functions

- **Procedures**: Defined using the `procedure` keyword.
  ```plaintext
  procedure test(string msg)
  begin
      print(msg);
  end
  ```
- **Functions**: Defined using the `function` keyword with a specified return type.
  ```plaintext
  function fibonacci(float n): float
  begin
      if (n <= 1)
          return n;
      else
          return fibonacci(n - 1) + fibonacci(n - 2);
  end
  ```

### Control Structures

- **Conditionals**: `if`, `elif`, and `else`.
  ```plaintext
  if (a == 4)
  begin
      print("4");
  end elif (a == 5)
  begin
      print("5");
  end else
  begin
      print("What?");
  end
  ```

- **Loops**:
  - **While Loop**:
    ```plaintext
    float i = 0;
    while (i < 18)
    begin
        print("fibonacci:" + fibonacci(i));
        i = i + 1;
    end
    ```
  - **Repeat Until Loop**:
    ```plaintext
    float c = 0;
    repeat
    begin
        print(c);
        c = c + 1;
    end
    until (c > 10);
    ```
  - **For Loop**:
    ```plaintext
    for (float i = 0; i < 10; i++)
    begin
        if (i == 4)
            break;
        print("For " + i);
        if (i == 6)
            continue;
    end
    ```

- **Switch Case**:
  ```plaintext
  float state = 10;
  switch (state)
  begin
      case 0:
      begin
          print("0");
      end
      case 1:
      begin
          print("1");
      end
      default:
          print("default");
  end
  ```

### Built-in Functions

- `print()`: Function for outputting data.
- `now`: Function to get the current time.


### ToDo:
    classes
    structs
    vm bytecode

### Example Process

```plaintext

    program main;
        process nave()
        begin
                x=mouse_x();
                y=mouse_y();
                float life =0.0;
                loop
                    begin 
                        x = x + 5;
                            if (x>=800)
                            begin 
                                x=0;
                            end
                        life = life + 1.0;
                        if (life>=2000)
                        begin
                            break;
                        end
                    end 
        end
        process main()
        begin 
            loop 
            begin 
               if (mouse_down(0))
               begin
                    nave();
               end
            end
        end
    begin
       main();//main process 
    end.
```    