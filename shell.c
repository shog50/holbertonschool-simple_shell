#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

void run_shell(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;
pid_t child_pid;
int status;

while (1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "#cisfun$ ", 9);

nread = getline(&line, &len, stdin);

if (nread == -1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
free(line);
exit(0);
}

if (line[nread - 1] == '\n')
line[nread - 1] = '\0';

if (strlen(line) == 0)
continue;

child_pid = fork();

if (child_pid == -1)
{
perror("fork");
continue;
}

if (child_pid == 0)
{
if (execve(line, &line, environ) == -1)
{
perror(line);
exit(1);
}
}
else
{
wait(&status);
}
}
}

