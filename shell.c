#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>

char *_strchr(const char *s, int c)
{
while (*s)
{
if (*s == c)
return (char *)s;
s++;
}
return NULL;
}

int _strcmp(const char *s1, const char *s2)
{
while (*s1 && *s2 && *s1 == *s2)
{
s1++;
s2++;
}
return (unsigned char)*s1 - (unsigned char)*s2;
}

int is_interactive(void)
{
return isatty(STDIN_FILENO);
}

int has_path_env(void)
{
int i = 0;
while (environ[i])
{
if (_strcmp(environ[i], "PATH=") == 0 ||
(_strchr(environ[i], '=') &&
_strcmp(environ[i], "PATH") == 0))
return 1;
i++;
}
return 0;
}

char *find_command(char *command)
{
char *path = getenv("PATH"), *token, *full_path;
size_t len;

if (!path || !command || command[0] == '/')
return strdup(command);

token = strtok(path, ":");
while (token)
{
len = strlen(token) + strlen(command) + 2;
full_path = malloc(len);
if (!full_path)
return NULL;

snprintf(full_path, len, "%s/%s", token, command);
if (access(full_path, X_OK) == 0)
return full_path;

free(full_path);
token = strtok(NULL, ":");
}

return strdup(command); /* fallback */
}

void execute_command(char *command)
{
pid_t pid;
int status;
char *argv[MAX_ARGS];
char *token, *exec_path;
int i = 0;

while (*command == ' ') command++; /* remove leading spaces */
if (*command == '\0') return;       /* skip empty */

token = strtok(command, " ");
while (token && i < MAX_ARGS - 1)
{
argv[i++] = token;
token = strtok(NULL, " ");
}
argv[i] = NULL;

if (argv[0] == NULL)
return;

/* If contains slash or PATH is unset, use as is */
if (_strchr(argv[0], '/') || !getenv("PATH"))
{
exec_path = strdup(argv[0]);
}
else
{
char *path_copy = strdup(getenv("PATH")); /* strtok modifies */
exec_path = find_command(argv[0]);
free(path_copy);
}

if (access(exec_path, X_OK) != 0)
{
perror("./hsh");
free(exec_path);
return;
}

pid = fork();
if (pid == -1)
{
perror("fork");
free(exec_path);
return;
}

if (pid == 0)
{
if (execve(exec_path, argv, environ) == -1)
{
perror("./hsh");
exit(EXIT_FAILURE);
}
}
else
{
waitpid(pid, &status, 0);
}

free(exec_path);
}

