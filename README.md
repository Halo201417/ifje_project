# Expression Transformation Project - IFJe README

**Author:** Jose María Morán Gutiérrez  
**Deadline:** December 9th, 2025

---

## 1. What is the project about?

A C program that **transforms a nested function call of the form**

```less
add(5, mul(3, sub(10, pow(6, 4))))
```

into an equivalent mathematical expression in **infix notation with minimal parentheses**:

```scss
5 + 3 * (10 - 6^4)
```

The project supports the following functions: `add`,`sub`,`mul`,`div`,`mod`,`pow`,`tern` (the last one corresponds to the ternary operator `?:`). The program reads from `stdin` and writes the resulting expression to `stdout`.

---
## Content of the deliverable
Structure of the file/zip to be uploaded:
```r
expr-project/
├─ README.md                
├─ Makefile                  
├─ src/
│  ├─ main.c
│  ├─ lexer.c
│  ├─ lexer.h
│  ├─ parser.c
│  ├─ parser.h
│  ├─ ast.c
│  ├─ ast.h
│  ├─ printer.c
│  └─ printer.h
├─ tests/
│  ├─ test1.in               
│  ├─ test1.out              
│  └─ other_tests/*.in
└─ extras/
   └─ run_tests.sh
```

---
## 3. Compile and run (Merlin — `merlin.fit.vutbr.cz`)
On the Merlin server (or locally) it is recommended to compile with `gcc`

> [!WARNING]
> Check the gcc version with `gcc --version`, it should be on 
> 12.5.0

With **Makefile**:
```sh
make
#Execute it with the input file
./expr < tests/test1.in
```

Run automated tests:
```sh
make test
```

If you don't use the Makefile (or it is corrupted):
```sh
# Compile manually all source files
gcc -std=c11 -Wall -Wextra -g src/*.c -o expr

# Run the program with the input file
./expr < tests/test1.in
```

Or check the program's output against the expected output:
```sh
./expr < tests/test1.in > out1.txt
diff -u tests/test1.out out1.txt
```

---
## 4.Design and implementation
 - **Language**: C
 - **Main components**:
   - `lexer` — lexical analysis. Produces tokens: `NUMBER`, `IDENT`, `(`,`)`,`,`,`EOF`. Ignores whitespace and block comments `/*.....*/`. Preserves the numeric literal in the token.
   - `parser` — recursive descent parser that builds an AST. Simplified grammar example: `<expr> ::= <number> | <ident> '(' <arglist> ')'`
   - `ast` — internal structure with nodes like `NUMBER` and `OP`. Stores the original numeric literal for exact printing.
   - `printer` — converts the AST into an infix string applying precedence and associativity rules to omit unnecessary parentheses.
   - `main` — reads from `stdin`, parses, and writes to `stdout`
 - **Operator precedence (from lowest to highest)**:
    - `?:` (terniary)
    - `+` `-`
    - `*` `/` `%`
    - `^`
  - **Parentheses rule**: wrap a child in parentheses when its precedence is lower than the parent's. If they have equal precedence, apply associativity rules
---

## 5.Error handling
- Lexical errors: invalid character, unclosed comment — report with offset (position).

- Syntactic errors: missing parentheses/commas, incorrect number of arguments for a function

## 6.Tests included
- `tests/test1.in` - mandatory input
- `tests/test1.out` - expected output