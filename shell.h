#ifndef SHELL_H
#define SHELL_H
#define MAX_ARGS 64
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void run_shell(void);
ssize_t _getline(char **lineptr, size_t *n);
extern char **environ;
int _strcmp(const char *s1, const char *s2);
char *_strchr(const char *s, int c);
char *find_command(char *command);
int is_interactive(void);
char *_strdup(const char *str);
char *_strcat(char *dest, const char *src);
int _strlen(const char *s);
int _strncmp(const char *s1, const char *s2, size_t n);

#endif
