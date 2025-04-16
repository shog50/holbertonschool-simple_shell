#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void run_shell(void)
{
char *line = NULL, *token = NULL, *path = NULL, *dir = NULL;
char *argv[64], full_path[1024];
size_t len = 0;
ssize_t nread;
pid_t child_pid;
int i, found;

while (1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "$ ", 2);

nread = getline(&line, &len, stdin);
if (nread == -1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
break;
}

if (line[nread - 1] == '\n')
line[nread - 1] = '\0';

if (strcmp(line, "exit") == 0)
break;

if (strcmp(line, "env") == 0)
{
for (i = 0; environ[i]; i++)
{
write(STDOUT_FILENO, environ[i], strlen(environ[i]));
write(STDOUT_FILENO, "\n", 1);
}
continue;
}

i = 0;
token = strtok(line, " ");
while (token && i < 63)
{
argv[i++] = token;
token = strtok(NULL, " ");
}
argv[i] = NULL;

if (argv[0] == NULL)
continue;

if (strchr(argv[0], '/') == NULL)
{
path = getenv("PATH");
if (path == NULL)
{
perror("getenv");
continue;
}
found = 0;
path = strdup(path);
dir = strtok(path, ":");

while (dir)
{
snprintf(full_path, sizeof(full_path), "%s/%s", dir, argv[0]);
if (access(full_path, X_OK) == 0)
{
argv[0] = full_path;
found = 1;
break;
}
dir = strtok(NULL, ":");
}
free(path);

if (!found)
{
perror(argv[0]);
continue;
}
}
else
{
if (access(argv[0], X_OK) != 0)
{
perror(argv[0]);
continue;
}
}

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

free(line);
}
