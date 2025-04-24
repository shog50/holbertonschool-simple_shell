# Simple Shell

## Description

A simple Unix shell that replicates basic functionalities of the /bin/sh shell. It handles command execution, built-in commands like exit and env, and handles errors. It supports commands with and without arguments, along with the PATH.

## Features

- Display a prompt and wait for user input.
- Handle command execution with or without arguments.
- Built-in commands:
  - exit: Exits the shell.
  - env: Prints the current environment variables.
- Error handling for invalid commands.

## Installation

Clone the repository:
- git clone https://github.com/shog50/holbertonschool-simple_shell.git
- cd holbertonschool-simple_shell

## Compilation

To compile the shell:
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

## Usage

Run the shell interactively:
./hsh

Run the shell with a command:
echo "/bin/ls" | ./hsh

Exit the shell:
exit

## Authors
- Rawan Albaraiki
- Shouq Alosaimi
