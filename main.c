#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

extern char **environ;

/**
* main - Simple shell main loop
*
* Return: Always 0
*/
int main(void)
{
char *line = NULL, *token, *cmd_path;
size_t len = 0;
ssize_t read;
char *args[64];
int i;
pid_t pid;

while (1)
{
printf(":) ");
read = getline(&line, &len, stdin);
if (read == -1)
break;

line[strcspn(line, "\n")] = '\0';  /* remove newline */

token = strtok(line, " ");
i = 0;
while (token)
{
args[i++] = token;
token = strtok(NULL, " ");
}
args[i] = NULL;

if (args[0] == NULL)
continue;

if (access(args[0], X_OK) == 0)
{
cmd_path = args[0];
}
else
{
cmd_path = find_command(args[0]);
if (!cmd_path)
{
fprintf(stderr, "%s: command not found\n", args[0]);
continue;
}
}

pid = fork();
if (pid == 0)
{
execve(cmd_path, args, environ);
perror("execve");
exit(EXIT_FAILURE);
}
else
{
wait(NULL);
if (cmd_path != args[0])
free(cmd_path);
}
}

free(line);
return (0);
}

