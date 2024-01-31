# BangerKing Language

A custom hybrid language made in C++

For now, it's an "interpreted" language (meaning nothing gets compiled, pretty much like JavaScript or PHP). However, I consider my language as "hybrid" because it will also be a compiled language for some architectures. Indeed, BangerKing code will be compiled into Assembly code. This feature is not yet implemented, and the first architecture that I will support is ARMv7 32 bits (due to its simplicity).

See the [grammar file](./grammar.txt) to learn more about the syntax.
Look at [the examples](./examples) to see what the language is supposed to look like.

> **NOTE**: This project uses C++ 20. It might not be compatible with previous versions.

**WORK IN PROGRESS** (still very early in the development process)

## How to use it?

- Clone the repo
- Make sure you have "g++"
- Make sure you can run a Makefile via the command `make`

This project uses [cmake](https://cmake.org/), so the [Makefile](./Makefile) that I provide should not be necessary, however it's still quite useful for anyone who doesn't want to use it. Note that instructions on how to use `cmake` are available in [CMakeLists.txt](./CMakeLists.txt).

To compile the project and start the Command Line Interface (CLI), just type:

```bash
make
# or simple "make file" without entrypoint
```

To compile the project and execute a `.bk` file:

```bash
make file entrypoint=examples/main.bk
# "hello_world.bk" will not work for now
```

`examples/main.bk` being the entrypoint of the user's program. In this case, it's the main file containing the beginning of the BangerKing program.

The executable is placed in this path: `./build/bangerking`.

To run the current executable:

```bash
make execute
# or
make execute entrypoint=examples/main.bk
```

`make execute` will run the current executable located in the build folder. Specify an entrypoint if you want to run a file, don't if you want the CLI. This command will fail if there is no executable. By default, this action should not be necessary.

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

I use [doctest](https://github.com/doctest/doctest) for the [tests](./tests).

### Measure performance

```bash
make perf
```

This command generates a Markdown log file in [/logs](/logs), as well as an output in the terminal. It measures the time that BangerKing takes to analyse, parse and interpret a large sample code.

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

This step is done by the [Lexer](./include/lexer.hpp), we call this step the "lexical analysis" or "lexical tokenization". These tokens simplify the task of the [Parser](./include/parser.hpp). The parser takes as argument a list of tokens and creates a list of nodes. It's the parser's role to make sure that the order of a mathematical expression is preserved:

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

This way, the addition is done first, the division is done afterwards, and finally the code returns the result of the substraction. We read this tree in a "pre-order traversal" (préfixe in French ^^).

The last step is the `Interpreter` that's responsible for actually executing these expressions. It will first take as input the ListNode instance that was produced by the Parser and recursively "visit" each node. When it visits an instance of "AddNode" for example, it will visit member "a" and make sure there was no error during this visit, and then it will visit member "b", and once again make sure there was no error, and finally it'll apply a certain behavior depending on the types of the addition members: between two integers, it will add them together, between an integer and a string, it will produce a new string, etc. Each visit produces a new value. Old values are deallocated as soon as possible.

## Variables

The language allows the use of custom variables:

```bangerking
store a as int = 5
store b as int = 10
store c as int = a + b
```

The variables `a`, `b` and `c` are stored in a symbol table (`std::map<std::string, Value*>`), itself stored within a context. A new context is created when inside a function or a loop (not yet implemented). Since each context has its own symbol table, two variables of the same name can be declared and accessed in contexts of different depth. To keep track of this depth, the contexts are stored as a "reversed linked-list" where each context holds its symbol table as well as a reference to its parent context. The global context has a `nullptr` as parent. The symbol tables are also reversed linked-list to make the research of a variable easier.

When accessed, a variable returns a copy of its value stored in the symbol table (the values within the symbol table become immutable) and the garbage collector automatically deallocates the memory of this returned value to avoid memory leaks of inaccessible values returned by previous statements.

This is also why, in this code...

```bangerking
store a as int = 5
store b as int = 10
a = b
b = 5
```

a != b, indeed `a = 10`, `b = 5`.

### Performance issues

I spotted one problem in the way I'm doing all this: everything is done one step at a time. Indeed, as of now, this is how the program behaves:

1. Read the entire file and store it into a string
2. Create a list of tokens
3. Wait for this list, and then parse the tokens to create an AST.
4. Once the AST is created, interpret it.

As you can see, the first and second step can be merged quite easily, and I might also be able to do the parser at the same time that the list of tokens gets created.

### Memory leaks?

C and old versions of C++ use "raw pointers". Look at this example in C++:

```cpp
int main() {
  MyObject* obj = new MyObject();
  if (5 == 5) {
    // ...
    return 1;
  }
  delete obj; // you'd use "free()" in C
  return 0;
}
```

This code is very simple, but having to manually deallocate the pointers make it very easy to forget one. In the provided snippet, you may have already spotted the issue. Indeed, if `5 == 5` (which might be `true`, who knows), then the pointer doesn't get deallocated and you end up with a memory leak. It doesn't matter for such a small program, especially since we're exiting the program and that the [destructor](https://en.cppreference.com/w/cpp/language/destructor) might be handling this already, but bear with me for a second.

Here `obj` is a "raw pointer". C++ introduces **smart pointers**. Pretty much like [Rust](https://www.rust-lang.org/) does, the pointers are controlled, stored once, by a unique owner, and then moved/shared to other variables (but not copied).

In the C++ example below, memory deallocations are handled automatically as soon as a pointer gets out of scope:

```cpp
int main() {
  // nested scope
  {
    std::unique_ptr<Creeper> mob = make_unique<Creeper>();
    std::cout << "hello" << std::endl;
  } // here all pointers defined in this scope get deallocated

  return 0;
}
```

This way, no need to worry about "delete" (or "free"). This subject is quite complex, as there are "move" actions (to not copy the object), and "shared_ptr" if multiple pointers need to own the same value.

BangerKing uses smart pointers to easily handle memory, and to make sure that there is not too many unnecessary copies of values. Therefore, there should not be memory leak, at least I hope so.

Here a video explaining them well: https://www.youtube.com/watch?v=tSIBKys2eBQ

## Disclaimer

This repo is meant to be a joke. I'm bored during classes and I want to learn C++ so I'm making this language whose syntax is inspired by "Burger King" (the fast food). The source code of this repository will **not** be perfect. _Do not use this in production for any serious project._

## License

MIT License.