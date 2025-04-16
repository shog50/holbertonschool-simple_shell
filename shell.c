#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "shell.h"

void run_shell(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;
pid_t child_pid;
char *argv[MAX_ARGS];
char *token;
int i;
char *path, *full_path, *dir;
struct stat st;

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

if (strcmp(line, "env") == 0)
{
for (i = 0; environ[i] != NULL; i++)
printf("%s\n", environ[i]);
continue;
}

if (line[0] == '\0')
continue;

i = 0;
token = strtok(line, " ");
while (token != NULL && i < MAX_ARGS - 1)
{
argv[i++] = token;
token = strtok(NULL, " ");
}
argv[i] = NULL;

if (stat(argv[0], &st) == 0)
{
child_pid = fork();
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
else
{
path = getenv("PATH");
if (path)
{
path = strdup(path);
dir = strtok(path, ":");
while (dir != NULL)
{
full_path = malloc(strlen(dir) + strlen(argv[0]) + 2);
if (full_path == NULL)
break;
sprintf(full_path, "%s/%s", dir, argv[0]);
if (stat(full_path, &st) == 0)
{
child_pid = fork();
if (child_pid == 0)
{
execve(full_path, argv, environ);
perror(argv[0]);
exit(EXIT_FAILURE);
}
else
{
wait(NULL);
free(full_path);
break;
}
}
free(full_path);
dir = strtok(NULL, ":");
}
free(path);
}
else
perror(argv[0]);
}
}
free(line);
}
