#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * run_shell - runs a simple shell loop
 */
void run_shell(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;
pid_t child_pid;
char *argv[64];
int i;

while (1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "$ ", 2);

nread = getline(&line, &len, stdin);
if (nread == -1)
{
free(line);
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
exit(0);
}

if (line[nread - 1] == '\n')
line[nread - 1] = '\0';

if (strcmp(line, "exit") == 0)
{
free(line);
exit(0);
}

i = 0;
argv[i] = strtok(line, " ");
while (argv[i] != NULL)
{
i++;
argv[i] = strtok(NULL, " ");
}

if (argv[0] == NULL)
continue;

child_pid = fork();
if (child_pid == -1)
{
perror("fork");
continue;
}

if (child_pid == 0)
{
if (execve(argv[0], argv, environ) == -1)
{
perror(argv[0]);
exit(EXIT_FAILURE);
}
}
else
wait(NULL);
}
}
