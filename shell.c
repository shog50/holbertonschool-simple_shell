#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

extern char **environ;

void execute_command(char *command)
{
pid_t pid;
int status;
char *argv[3];
char *exec = strtok(command, " ");
char *arg = strtok(NULL, " ");

if (exec == NULL)
return;

argv[0] = exec;
argv[1] = arg;
argv[2] = NULL;

pid = fork();
if (pid == -1)
return;

if (pid == 0)
{
if (execve(argv[0], argv, environ) == -1)
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

