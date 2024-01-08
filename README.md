# BangerKing Language

A custom language made in C++

See the [grammar file](./grammar.txt) to learn more about the syntax.
Look at [the example](./examples/hello_world.bk) to see what the language is supposed to look like.

> **NOTE**: This project uses C++ 20. It might not be compatible with previous versions.

**WORK IN PROGRESS**

## How to use it?

- Clone the repo
- Make sure you've installed "g++"
- Make sure you can run a Makefile via the command `make`

To compile the project and start the Command Line Interface (CLI):

```bash
make cli
```

To compile the project and execute a file:

```bash
make file entrypoint=examples/main.bk
```

`examples/main.bk` being the entrypoint of the user's program. In this case, it's the main file containing the beginning of the BangerKing program.

To run the current executable:

```bash
make execute
# or
make execute entrypoint=examples/main.bk
```

`make execute` will run the current executable located in the build folder.
This command will fail if there is no executable.
By default, this action should not be necessary.

Specify an entrypoint if you compiled the project without the CLI (so if you used `make file` beforehand).

### Measure performance

```bash
make perf
```

### Tests

```bash
make tests
```

Test a specific program:

```bash
make single_test test=3-parser
# this test is just for the Parser.
# Look inside the "tests" folder to know the name of the tests.
# They have a specific order because an error with Tokens, for example,
# will crash all the other tests.
```

## How does it work?

Each symbol in the source code is a `Token`. The program first creates a list of tokens. Therefore, a code like this:

```
5+5
```

will produce this result:

```
[
  Token(TokenType::NUMBER, "5"),
  Token(TokenType::PLUS, "+"),
  Token(TokenType::NUMBER, "5")
]
```

This step is done by the [Lexer](./include/lexer.h). These tokens simplify the task of the [Parser](./include/parser.h). The parser takes as argument a list of tokens and creates a list of nodes. It's the parser's role to make sure that the order of a mathematical expression is preserved:

```
1+2-10/2
```

You know maths, so you know that in this case, you'll do the addition first, then the division must is done, and finally the expression returns the result of the substraction. The Parser creates an `Abstract Syntax Tree` that looks like this:

```
SubstractNode(
  AddNode(
    IntegerNode(1),
    IntegerNode(2)
  ),
  DivideNode(
    IntegerNode(10),
    IntegerNode(2)
  )
)
```

This way, the addition is done first, the division is done afterwards, and finally the code returns the result of the substraction.

The last step is the `Interpreter` that's responsible of actually executing these expressions. It will first take as input the ListNode instance that was produced by the Parser and recursively "visit" each node. When it visits an instance of "AddNode" for example, it will visit member "a" and make sure there was no error during this visit, and then it will visit member "b", and once again make sure there was no error, and finally it'll apply a certain behavior depending on the types of the addition members: between two integers, it will add them together, between an integer and a string, it will produce a new string, etc. Each visit produces a new value. Old values are deallocated as soon as possible.

## Variables

The language allows the use of custom variables:

```bangerking
store a as int = 5
store b as int = 10
store c as int = a + b
```

The variables `a`, `b` and `c` are stored in a symbol table (`std::map<std::string, Value*>`), itself stored within a context. A new context is created when inside a function or a loop (not yet implemented). Since each context has its own symbol table, two variables of the same name can be declared and accessed in contexts of different depth. To keep track of this depth, the contexts are stored as a "reversed linked-list" where each context holds its symbol table as well as a reference to its parent context. The global context has a `nullptr` as parent. The symbol tables are also reversed linked-list to make the research of a variable easier.

When accessed, a variable returns a copy of its value stored in the symbol table and the garbage collector automatically deallocates the memory of this returned value so as to avoid memory leaks of inaccessible values returned by previous statements.

This is also why, in this code...

```bangerking
store a as int = 5
store b as int = 10
a = b
b = 5
```

a != b, indeed `a = 10`, `b = 5`.

## Disclaimer

This repo is meant to be a joke. I'm bored during classes and I want to learn C++ so I'm making this language whose syntax is inspired by "Burger King" (the fast food). The source code of this repository will **not** be perfect. _Do not use this in production for any serious project._

## License

MIT License.