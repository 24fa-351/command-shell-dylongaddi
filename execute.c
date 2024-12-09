// execute.c
#include "execute.h"
#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

// Executes a single command, handling redirection
void execute_command(char **args) {
    // Check for built-in commands
    if (execute_builtin(args) == 0) {
        return; // Built-in command executed and may have altered shell state
    }

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process

        // Handle redirection
        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], ">") == 0) {
                args[i] = NULL; // Terminate arguments
                if (args[i + 1] == NULL) {
                    fprintf(stderr, "shell: expected filename after '>'\n");
                    exit(EXIT_FAILURE);
                }
                int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                break;
            } else if (strcmp(args[i], ">>") == 0) {
                args[i] = NULL;
                if (args[i + 1] == NULL) {
                    fprintf(stderr, "shell: expected filename after '>>'\n");
                    exit(EXIT_FAILURE);
                }
                int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                break;
            } else if (strcmp(args[i], "<") == 0) {
                args[i] = NULL;
                if (args[i + 1] == NULL) {
                    fprintf(stderr, "shell: expected filename after '<'\n");
                    exit(EXIT_FAILURE);
                }
                int fd = open(args[i + 1], O_RDONLY);
                if (fd < 0) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                break;
            }
        }

        // Execute the command
        if (execvp(args[0], args) == -1) {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("shell");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

// Executes two commands connected by a pipe, handling  execution
void execute_pipe(char **args1, char **args2) {
    int pipefd[2];
    pid_t p1, p2;
    int status1, status2;

    if (pipe(pipefd) == -1) {
        perror("shell: pipe");
        return;
    }

    p1 = fork();
    if (p1 < 0) {
        perror("shell: fork");
        return;
    }

    if (p1 == 0) {
        // Child 1: Executes first command and writes to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Handle redirection in first command if any
        for (int i = 0; args1[i] != NULL; i++) {
            if (strcmp(args1[i], ">") == 0 || strcmp(args1[i], ">>") == 0 || strcmp(args1[i], "<") == 0) {
                fprintf(stderr, "shell: Invalid.\n");
                exit(EXIT_FAILURE);
            }
        }

        // Execute the first command
        if (execvp(args1[0], args1) == -1) {
            perror("shell");
            exit(EXIT_FAILURE);
        }
    }

    p2 = fork();
    if (p2 < 0) {
        perror("shell: fork");
        return;
    }

    if (p2 == 0) {
        // Child 2: Reads from pipe and executes second command
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);

        // Handle redirection in second command if any
        for (int i = 0; args2[i] != NULL; i++) {
            if (strcmp(args2[i], ">") == 0) {
                args2[i] = NULL;
                if (args2[i + 1] == NULL) {
                    fprintf(stderr, "shell: expected filename after '>'\n");
                    exit(EXIT_FAILURE);
                }
                int fd = open(args2[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                break;
            } else if (strcmp(args2[i], ">>") == 0) {
                args2[i] = NULL;
                if (args2[i + 1] == NULL) {
                    fprintf(stderr, "shell: expected filename after '>>'\n");
                    exit(EXIT_FAILURE);
                }
                int fd = open(args2[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                break;
            } else if (strcmp(args2[i], "<") == 0) {
                args2[i] = NULL;
                if (args2[i + 1] == NULL) {
                    fprintf(stderr, "shell: expected filename after '<'\n");
                    exit(EXIT_FAILURE);
                }
                int fd = open(args2[i + 1], O_RDONLY);
                if (fd < 0) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                break;
            }
        }

        // Execute the second command
        if (execvp(args2[0], args2) == -1) {
            perror("shell");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process closes both ends of the pipe
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both children to finish
    waitpid(p1, &status1, 0);
    waitpid(p2, &status2, 0);

}
