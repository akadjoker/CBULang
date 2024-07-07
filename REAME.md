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


## Supported Features

### Program Structure

- **Program Definition**: Every program starts with the `program` keyword followed by the program name.
- **Main Block**: The main block is enclosed by `begin` and `end.`.

### Data Types

- `int`: Integer numbers.
- `float`: Floating-point numbers.
- `bool`: Boolean values (`true` or `false`).
- `string`: Sequences of characters.

### Variables

- Declaration and initialization of variables:
  ```plaintext
  int a = 50;
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
  function fibonacci(int n): int
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
    int i = 0;
    while (i < 18)
    begin
        print("fibonacci:" + fibonacci(i));
        i = i + 1;
    end
    ```
  - **Repeat Until Loop**:
    ```plaintext
    int c = 0;
    repeat
    begin
        print(c);
        c = c + 1;
    end
    until (c > 10);
    ```
  - **For Loop**:
    ```plaintext
    for (int i = 0; i < 10; i++)
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
  int state = 10;
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
    process
    pointers
    structs
    vm bytecode

### Example Program

```plaintext
program main;

procedure test(string msg)
begin
    print(msg);
end

function fibonacci(int n): int
begin
    if (n <= 1)
        return n;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
end

function say(): int
begin
    print("function ola ;) ");
    return 100;
end

begin
    int a = 50;

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

    if (a == 4)
        print(a);

    float start_time = now;

    print("Start: " + start_time);

    int i = 0;
    while (i < 18)
    begin
        print("fibonacci:" + fibonacci(i));
        i = i + 1;
    end

    float end_time = now;

    print("End: " + end_time);

    float time = (end_time - start_time);

    print("Time passed: " + time);

    print("end loop with result: " + i);
    print("end main script ;)");

    int state = 10;
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

    int c = 0;
    repeat
    begin
        print(c);
        c = c + 1;
    end
    until (c > 10);

    for (int i = 0; i < 10; i++)
    begin
        if (i == 4)
            break;
        print("For " + i);
        if (i == 6)
            continue;
    end

    int var = 0;

    print(var);

    var++;
    var++;
    var++;
    int s = ++var;

    print(var);
    print(s);

end.
```
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
                            if (x>=800.0)
                            begin 
                                x=0.0;
                            end
                        life = life + 1.0;
                        if (life>=2000.0)
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