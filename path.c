#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

/**
* find_command - Search for a command in PATH
* @command: the command to find
* Return: full path if found, or NULL
*/
char *find_command(char *command)
{
char *path_env = NULL;
char *path_copy, *dir;
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
dir = strtok(path_copy, ":");

while (dir)
{
snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
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

