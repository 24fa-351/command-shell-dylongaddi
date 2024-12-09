// env.c
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Built-in: Set Environment Variable
int shell_setenv(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "shell: expected arguments to \"set\"\n");
        return 1;
    }
    if (setenv(args[1], args[2], 1) != 0) {
        perror("shell");
    }
    return 1;
}

// Built-in: Unset Environment Variable
int shell_unsetenv(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "shell: expected argument to \"unset\"\n");
        return 1;
    }
    if (unsetenv(args[1]) != 0) {
        perror("shell");
    }
    return 1;
}
