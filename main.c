// main.c
#include "parser.h"
#include "execute.h"
#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 10

int main(int argc, char **argv) {
    char *line;
    Command cmds[MAX_COMMANDS];
    int num_commands;
    int status = 1;

    // Initialize shell environment variables if needed

    while (status) {
        printf("shell# ");
        fflush(stdout);
        line = read_line();

        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // Parse the input line
        num_commands = parse_line(line, cmds);

        if (num_commands == 0) {
            free(line);
            continue;
        }

        // Handle built-in commands like 'exit' and 'cd' within execute_command
        // Execute commands
        if (num_commands == 1) {
            // Single command
            execute_command(cmds[0].args);
        } else {
            // Handle piping for multiple commands
            execute_pipe(cmds[0].args, cmds[1].args);
        }

        free(line);
    }

    return EXIT_SUCCESS;
}
