#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void run_shell(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;
pid_t child_pid;
char *argv[2];

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

if (line[0] == '\0')
continue;

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
