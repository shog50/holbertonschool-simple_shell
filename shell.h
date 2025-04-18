#ifndef SHELL_H
#define SHELL_H
#define MAX_ARGS 64
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

void run_shell(void);
ssize_t _getline(char **lineptr, size_t *n);
extern char **environ;
/* shell.h */
int _strcmp(const char *s1, const char *s2);
/* shell.h */
char *_strchr(const char *s, int c);
char *find_command(char *command);
int has_path_env(void);
void execute_command(char *command);
int is_interactive(void);


#endif /* SHELL_H */
