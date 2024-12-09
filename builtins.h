// builtins.h
#ifndef BUILTINS_H
#define BUILTINS_H

#include "parser.h"

// Function pointer type for built-in commands
typedef int (*builtin_func)(char **args);

// Built-in command declarations
int shell_cd(char **args);
int shell_pwd(char **args);
void shell_exit(char **args);
int shell_help(char **args);

// Function to execute built-in commands
int execute_builtin(char **args);

// Number of built-in commands
int num_builtins();

#endif // BUILTINS_H
