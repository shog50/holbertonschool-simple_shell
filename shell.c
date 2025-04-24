#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int is_interactive(void)
{
return (isatty(STDIN_FILENO));
}

char *_strdup(const char *str)
{
int len = 0, i;
char *copy;

if (!str)
return (NULL);

while (str[len])
len++;

copy = malloc(len + 1);
if (!copy)
return (NULL);

for (i = 0; i < len; i++)
copy[i] = str[i];
copy[i] = '\0';

return (copy);
}

char *_strcat(char *dest, const char *src)
{
int i = 0, j = 0;

while (dest[i])
i++;

while (src[j])
{
dest[i] = src[j];
i++;
j++;
}
dest[i] = '\0';

return (dest);
}

int _strlen(const char *s)
{
int len = 0;
while (s[len])
len++;
return (len);
}

char *_strchr(const char *s, int c)
{
while (*s)
{
if (*s == (char)c)
return ((char *)s);
s++;
}
if (c == '\0')
return ((char *)s);
return (NULL);
}

char *find_command(char *command)
{
char *path = NULL, *copy = NULL, *dir = NULL, *full = NULL;
int i = 0, len = 0;

for (i = 0; environ[i]; i++)
{
if (_strcmp(environ[i], "PATH=") == 0)
{
path = environ[i] + 5;
break;
}
}

if (!path)
return (NULL);

copy = _strdup(path);
if (!copy)
return (NULL);

dir = strtok(copy, ":");
while (dir)
{
len = _strlen(dir) + _strlen(command) + 2;
full = malloc(len);
if (!full)
{
free(copy);
return (NULL);
}
full[0] = '\0';
_strcat(full, dir);
_strcat(full, "/");
_strcat(full, command);

if (access(full, X_OK) == 0)
{
free(copy);
return (full);
}
free(full);
dir = strtok(NULL, ":");
}

free(copy);
return (NULL);
}

void execute_command(char *command)
{
pid_t pid;
int status;
char *argv[MAX_ARGS];
char *token;
int i = 0;
char *full_path;

if (!command || command[0] == '\0')
return;

token = strtok(command, " ");
while (token && i < MAX_ARGS - 1)
{
argv[i++] = token;
token = strtok(NULL, " ");
}
argv[i] = NULL;

if (!argv[0])
return;

if (access(argv[0], X_OK) != 0)
{
full_path = find_command(argv[0]);
if (!full_path)
{
perror("./hsh");
return;
}
argv[0] = full_path;
}
else
{
full_path = argv[0];
}

pid = fork();
if (pid == -1)
{
perror("fork");
free(full_path);
return;
}

if (pid == 0)
{
execve(full_path, argv, environ);
perror("./hsh");
exit(EXIT_FAILURE);
}
else
{
waitpid(pid, &status, 0);
}

if (full_path != argv[0])
free(full_path);
}

void run_shell(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;

while (1)
{
if (is_interactive())
write(STDOUT_FILENO, "$ ", 2);

nread = _getline(&line, &len);
if (nread == -1)
{
free(line);
write(STDOUT_FILENO, "\n", 1);
break;
}

if (line[nread - 1] == '\n')
line[nread - 1] = '\0';

if (_strcmp(line, "exit") == 0)
{
free(line);
exit(0);
}

if (_strcmp(line, "env") == 0)
{
int i = 0;
while (environ[i])
{
write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
write(STDOUT_FILENO, "\n", 1);
i++;
}
continue;
}

execute_command(line);
}

free(line);
}
