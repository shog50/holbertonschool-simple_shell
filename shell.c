#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "#cisfun$ "
#define BUFSIZE 1024

extern char **environ;

void execute_command(char *command);

int main(void)
{
char *command = NULL;
size_t bufsize = 0;
ssize_t n_read;

while (1)
{
write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
n_read = getline(&command, &bufsize, stdin);
if (n_read == -1)
{
free(command);
write(STDOUT_FILENO, "\n", 1);
exit(EXIT_SUCCESS);
}
command[n_read - 1] = '\0';
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
exit(EXIT_FAILURE);
}
else
{
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
}

