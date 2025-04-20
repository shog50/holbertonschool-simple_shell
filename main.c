#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define PROMPT "#cisfun$ "
#define BUFSIZE 1024

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
execute_command(command);
}

free(command);
return (0);
}

