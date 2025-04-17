#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

/**
* run_shell - Runs the custom shell
*/
void run_shell(void)
{
char *line = NULL, *token = NULL, *path = NULL, *dir = NULL;
char *argv[64], full_path[1024];
size_t len = 0;
ssize_t nread;
pid_t child_pid;
int i, found, status, last_status = 0;

while (1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "$ ", 2);

nread = _getline(&line, &len);
if (nread == -1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
free(line);
exit(last_status);
}

if (line[nread - 1] == '\n')
line[nread - 1] = '\0';

if (strcmp(line, "exit") == 0)
{
free(line);
exit(last_status);
}

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
if (!path)
continue;
path = strdup(path);
if (!path)
continue;

dir = strtok(path, ":");
found = 0;
while (dir)
{
snprintf(full_path, sizeof(full_path), "%s/%s", dir, argv[0]);
if (access(full_path, X_OK) == 0)
{
argv[0] = strdup(full_path);
found = 1;
break;
}
dir = strtok(NULL, ":");
}
free(path);

if (!found)
{
perror(argv[0]);
last_status = 127;
continue;
}
}
else if (access(argv[0], X_OK) != 0)
{
perror(argv[0]);
last_status = 127;
continue;
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
exit(2);
}
}
else
{
wait(&status);
last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 2;
}
}
}

int main(void)
{
    run_shell();
    return 0;
}

