// builtins.c
#include "builtins.h"
#include "env.h" // For environment variable functions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Built-in command names
char *builtin_str[] = {
    "cd",
    "pwd",
    "exit",
    "help",
    "set",
    "unset"
};

// Corresponding functions
builtin_func builtin_func_list[] = {
    &shell_cd,
    &shell_pwd,
    &shell_exit,
    &shell_help,
    &shell_setenv,   // From env.c
    &shell_unsetenv  // From env.c
};

// Number of built-in commands
int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// Built-in: Change Directory
int shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "shell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shell");
        }
    }
    return 1;
}

// Built-in: Print Working Directory
int shell_pwd(char **args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("shell");
    }
    return 1;
}

// Built-in: Exit Shell
void shell_exit(char **args) {
  exit(0);
}

// Built-in: Help
int shell_help(char **args) {
    int i;
    printf("shell: Supported built-in commands:\n");
    for (i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }
    printf("Use the man command for information on other programs.\n");
    return 1;
}

// Execute built-in commands
int execute_builtin(char **args) {
    if (args[0] == NULL) {
        return 1; // Empty command
    }

    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return builtin_func_list[i](args);
        }
    }

    return 1; // Not a built-in command
}
