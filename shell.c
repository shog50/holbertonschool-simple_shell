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
int is_interactive(void);
void process_multiple_commands(char *command);

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

command[n_read - 1] = '\0';

process_multiple_commands(command);
}

free(command);
return (0);
}

void process_multiple_commands(char *commands)
{
char *command = strtok(commands, " ");
while (command != NULL)
{
execute_command(command);
command = strtok(NULL, " ");
}
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
