#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define PROMPT "#cisfun$ "
#define BUFSIZE 1024

extern char **environ;

void execute_command(char *command);
int is_interactive(void);
void trim_spaces(char *str);

int main(void)
{
char *command = NULL;
size_t bufsize = 0;
ssize_t n_read;

while (1)
{
if (is_interactive())
write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

n_read = getline(&command, &bufsize, stdin);
if (n_read == -1)
{
free(command);
if (is_interactive())
write(STDOUT_FILENO, "\n", 1);
exit(EXIT_SUCCESS);
}

/* Remove newline character */
command[n_read - 1] = '\0';

/* Trim spaces */
trim_spaces(command);

/* Skip empty commands */
if (command[0] == '\0')
continue;

execute_command(command);
}

free(command);
return (0);
}

void execute_command(char *command)
{
pid_t pid;
int status;
char *argv[2];

argv[0] = command;
argv[1] = NULL;

pid = fork();
if (pid == -1)
return;

if (pid == 0)
{
if (execve(command, argv, environ) == -1)
{
perror("./hsh");
exit(EXIT_FAILURE);
}
}
else
{
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
}

int is_interactive(void)
{
return isatty(STDIN_FILENO);
}

/* Function to trim leading, trailing, and multiple internal spaces */
void trim_spaces(char *str)
{
char *start, *end, *dst;

/* Trim leading spaces */
start = str;
while (isspace(*start))
start++;

/* Trim trailing spaces */
end = start + strlen(start) - 1;
while (end > start && isspace(*end))
end--;
*(end + 1) = '\0';

/* Remove extra spaces in between */
dst = str;
while (*start)
{
if (!isspace(*start) || (dst > str && !isspace(*(dst - 1))))
*dst++ = *start;
start++;
}
*dst = '\0';
}

