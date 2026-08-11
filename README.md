# Lexical Analyzer in C

A minimalistic C-based Lexical Analyzer designed to process C source code and break it down into categorized tokens as part of a compiler front-end.

## Overview

In compiler design, a lexical analyzer (or lexer) reads a stream of characters from a source file, strips out unneeded elements like comments and whitespace, and groups the remaining characters into meaningful symbols called tokens. 

This project takes a C file as input and scans it character-by-character to identify keywords, identifiers, literals, operators, and special characters.

## Features

- Skips single-line (`//`) and multi-line (`/* */`) comments alongside unnecessary whitespace.
- Recognizes C language keywords like `int`, `float`, `return`, `if`, and `while`.
- Detects custom variable names and function names as Identifiers.
- Supports integer, decimal, string, and character literals.
- Handles standard single-character operators and multi-character operators like `+=`, `-=`, `==`, `!=`, `++`, and `--`.
- Identifies array brackets (`[`, `]`) and statement delimiters.

## Project Structure

- `lexer.h`: Header file containing the token definitions, structure declarations, and function prototypes.
- `lexer.c`: Core implementation for character scanning, lookahead logic, and token classification.
- `main.c`: Entry point handling command-line arguments and managing the output loop.
- `Makefile`: Script for automated building and cleanup.
- `test.c`: Sample source file used for testing the lexical analyzer.

## How to Build and Run

### Prerequisites
- GCC Compiler
- GNU Make

### Compilation

Build the project using the Makefile:

```bash
make
