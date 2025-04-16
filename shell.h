#ifndef SHELL_H
#define SHELL_H
#define MAX_ARGS 64
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void run_shell(void);
ssize_t _getline(char **lineptr, size_t *n);
extern char **environ;

#endif /* SHELL_H */
