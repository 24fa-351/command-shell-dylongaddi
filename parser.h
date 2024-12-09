// parser.h
#ifndef PARSER_H
#define PARSER_H

#define MAX_TOKENS 100
#define MAX_COMMANDS 10

typedef struct {
    char *args[MAX_TOKENS];
} Command;

// Reads a line of input from the user
char *read_line(void);

// Splits the input line into commands separated by pipes
int split_commands(char *line, char **commands);

// Splits a command into arguments
char **split_line(char *line);

// Parses the input line and fills the commands array
int parse_line(char *line, Command *cmds);

// Function to substitute environment variables in arguments
void substitute_variables(char **args);

#endif // PARSER_H
