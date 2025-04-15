#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
char *argv[2];

while (1)
{
write(STDOUT_FILENO, "$ ", 2);
nread = getline(&line, &len, stdin);
if (nread == -1)
{
write(STDOUT_FILENO, "\n", 1);
break;
}
if (line[nread - 1] == '\n')
line[nread - 1] = '\0';
if (line[0] == '\0')
continue;
if (strcmp(line, "exit") == 0)
break;

child_pid = fork();
if (child_pid == -1)
{
perror("fork");
continue;
}
if (child_pid == 0)
{
argv[0] = line;
argv[1] = NULL;
if (execve(argv[0], argv, environ) == -1)
{
perror(argv[0]);
exit(EXIT_FAILURE);
}
}
else
wait(NULL);
}
free(line);
}
