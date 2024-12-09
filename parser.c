// parser.c
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Reads a line of input from the user
char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0; // getline will allocate a buffer
    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);  // Exit on Ctrl+D
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

// Splits the input line into commands separated by pipes
int split_commands(char *line, char **commands) {
    int position = 0;
    char *token;

    token = strtok(line, "|");
    while (token != NULL && position < MAX_COMMANDS) {
        commands[position++] = token;
        token = strtok(NULL, "|");
    }
    commands[position] = NULL;
    return position;
}

// Splits a command into arguments based on whitespace
char **split_line(char *line) {
    int bufsize = MAX_TOKENS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while (token != NULL && position < MAX_TOKENS - 1) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += MAX_TOKENS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "shell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}


// Substitutes environment variables in arguments
void substitute_variables(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] == '$') {
            char *env_var = getenv(args[i] + 1); // Skip '$'
            if (env_var != NULL) {
                args[i] = env_var;
            } else {
                args[i] = ""; // Replace undefined variables with empty string
            }
        }
    }
}

// Parses the input line and fills the commands array
int parse_line(char *line, Command *cmds) {
    char *commands[MAX_COMMANDS];
    int num_commands = split_commands(line, commands);

    for (int i = 0; i < num_commands; i++) {
        cmds[i].args[0] = NULL;
        char **args = split_line(commands[i]);
        if (args[0] == NULL) {
            free(args);
            continue;
        }
        substitute_variables(args);
        int j = 0;
        while (args[j] != NULL && j < MAX_TOKENS - 1) {
            cmds[i].args[j] = args[j];
            j++;
        }
        cmds[i].args[j] = NULL;

        if (j > 0 && strcmp(cmds[i].args[j - 1], "&") == 0) {
            cmds[i].args[j - 1] = NULL;
            fprintf(stderr, "shell: Ignoring '&'.\n");
        }

        free(args);
    }

    return num_commands;
}
