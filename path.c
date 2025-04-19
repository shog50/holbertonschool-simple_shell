#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

extern char **environ;

/**
* find_command - Finds the full path of a command using PATH
* @command: the command name
* Return: Full path if found, else NULL
*/
char *find_command(char *command)
{
char *path_env = NULL, *path_copy, *dir;
char full_path[1024];
int i = 0;

while (environ[i])
{
if (strncmp(environ[i], "PATH=", 5) == 0)
{
path_env = environ[i] + 5;
break;
}
i++;
}

if (!path_env)
return (NULL);

path_copy = strdup(path_env);
if (!path_copy)
return (NULL);

dir = strtok(path_copy, ":");
while (dir)
{
sprintf(full_path, "%s/%s", dir, command);
if (access(full_path, X_OK) == 0)
{
free(path_copy);
return (strdup(full_path));
}
dir = strtok(NULL, ":");
}

free(path_copy);
return (NULL);
}

