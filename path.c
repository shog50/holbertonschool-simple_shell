#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

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

void execute_command(char *command)
{
pid_t pid;
int status;

pid = fork();
if (pid == 0)
{
char **argv = malloc(2 * sizeof(char *));
if (argv == NULL)
{
perror("malloc");
exit(EXIT_FAILURE);
}

argv[0] = command;
argv[1] = NULL;

execve(command, argv, environ);
perror("execve");
free(argv);
exit(EXIT_FAILURE);
}
else if (pid < 0)
{
perror("fork");
}
else
{
waitpid(pid, &status, 0);
}
}

int is_interactive(void)
{
return isatty(STDIN_FILENO);
}

