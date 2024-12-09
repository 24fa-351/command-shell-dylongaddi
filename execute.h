// execute.h
#ifndef EXECUTE_H
#define EXECUTE_H

#include "parser.h"

// Executes a single command without piping
void execute_command(char **args);

// Executes two commands connected by a pipe
void execute_pipe(char **args1, char **args2);

#endif // EXECUTE_H
