# BangerKing Language

A custom language made in C++

See the [grammar file](./grammar.txt) to learn more about the syntax.
Look at [the example](./examples/hello_world.bk) to see what the language is supposed to look like.

> **NOTE**: This project uses C++ 20. It might not be compatible with previous versions.

**WORK IN PROGRESS**

## How to test it?

- Clone the repo
- Make sure you've installed "g++"
- Make sure you can execute Bash scripts

Execute one of these scripts to...

#### Compile and run

```bash
./run.sh
```

#### Run

```bash
./exec.sh
```

#### Measure performance

```bash
./perf.sh
```

#### Test

```bash
./test.sh
```

Test a specific program:

```bash
./test.sh 3-parser
# this test is just for the Parser
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

This step is done by the [Lexer](./include/lexer.h). These tokens simplify the task of the [Parser](./include/parser.h). The parser takes a argument a list of tokens and creates a list of nodes. It's the parser's role to make sure that the order of a mathematical expression is preserved:

```
1+2-10/2
```

You know maths, so you know that in this case, you'll do the addition first, then the division must is done, and finally the expression returns the result of the substraction. The Parser creates an `Abstract Syntax Tree` that looks like this:

```
SubstractNode(
  AddNode(
    NumberNode(1),
    NumberNode(2)
  ),
  DivideNode(
    NumberNode(10),
    NumberNode(2)
  )
)
```

This way, the addition is done first, the division is done afterwards, and finally the code returns the result of the substraction.

The last step will be to add the `Interpreter` to actually execute these expressions.

## Disclaimer

This repo is meant to be a joke. I'm bored during classes and I want to learn C++ so I'm making this language whose syntax is inspired by "Burger King" (the fast food). The source code of this repository will **not** be perfect. _Do not use this in production for any serious project._

## License

MIT License.