#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

/**
* main - Simple shell program
* Return: Always 0
*/
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
pid_t pid;
int status;
char *args[2];

while (1)
{
printf("#cisfun$ ");
read = getline(&line, &len, stdin);

if (read == -1)
{
if (feof(stdin))
{
printf("\n");
break; /* Ctrl+D pressed */
}
perror("getline");
exit(EXIT_FAILURE);
}

line[strcspn(line, "\n")] = '\0'; /* Remove newline */

if (strlen(line) == 0)
continue;

args[0] = line;
args[1] = NULL;

pid = fork();
if (pid == 0)
{
if (execve(args[0], args, NULL) == -1)
{
perror(args[0]);
exit(EXIT_FAILURE);
}
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

free(line);
return (0);
}

