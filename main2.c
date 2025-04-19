#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main(void)
{
char *command = NULL;
size_t bufsize = 0;
ssize_t n_read;

while (1)
{
if (is_interactive())
{
printf(":) ");
}

n_read = getline(&command, &bufsize, stdin);
if (n_read == -1)
{
free(command);
break;
}

command[strcspn(command, "\n")] = '\0';

if (strcmp(command, "exit") == 0)
{
free(command);
break;
}

execute_command(command);
}

return 0;
}

