#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

/**
* main - Simple shell
* Return: Always 0
*/
int main(void)
{
char input[MAX_INPUT];
char *args[MAX_ARGS];
char *token;
pid_t pid;
int status;
char *cmd_path;

while (1)
{
printf(":) ");
if (fgets(input, MAX_INPUT, stdin) == NULL)
{
printf("\n");
break;
}

if (input[0] == '\n')
continue;

input[strcspn(input, "\n")] = '\0'; /* remove newline */

/* split input into args */
int i = 0;
token = strtok(input, " ");
while (token && i < MAX_ARGS - 1)
{
args[i++] = token;
token = strtok(NULL, " ");
}
args[i] = NULL;

/* handle "exit" */
if (strcmp(args[0], "exit") == 0)
break;

/* absolute or relative path */
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
waitpid(pid, &status, 0);
if (cmd_path != args[0])
free(cmd_path);
}
}
return (0);
}

