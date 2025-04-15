#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * run_shell - executes the shell loop
 */
void run_shell(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;
pid_t pid;

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
break;
}

if (line[nread - 1] == '\n')
line[nread - 1] = '\0';

if (strcmp(line, "exit") == 0)
{
free(line);
exit(0);
}

pid = fork();
if (pid == -1)
{
perror("fork");
continue;
}
else if (pid == 0)
{
char *args[2];
args[0] = line;
args[1] = NULL;
if (execve(args[0], args, environ) == -1)
{
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
exit(127);
}
}
else
wait(NULL);
}
free(line);
}
