#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

/**
* main - Simple Shell implementation
* @argc: Number of arguments
* @argv: Array of argument strings
*
* Return: Always 0 (Success)
*/
int main(int argc, char *argv[])
{
char *input = NULL;
size_t len = 0;
ssize_t nread;
char *token;
char *delim = " \t\r\n\a";
pid_t pid;
int status;

(void)argc;
(void)argv;


while (1)
{

printf("$ ");
nread = getline(&input, &len, stdin);
if (nread == -1)
{
if (feof(stdin))
break;
perror("getline");
exit(EXIT_FAILURE);
}


token = strtok(input, delim);
if (token == NULL)
continue;
pid = fork();
if (pid == 0)
{
if (execve(token, &token, NULL) == -1)
{
perror("Error executing command");
exit(EXIT_FAILURE);
}
}
else if (pid < 0)
{
perror("Error creating child process");
exit(EXIT_FAILURE);
}
else 
{
waitpid(pid, &status, 0);
}
}

free(input);
return (0);
}
