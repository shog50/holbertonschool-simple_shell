#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "simple_shell.h"
#include <sys/wait.h> /* For 'wait' function */

#define PROMPT "#cisfun$ "

extern char **environ; /* Declare the global 'environ' variable */

void simple_shell(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
char *args[2];

while (1)
{
printf(PROMPT);
read = getline(&line, &len, stdin);
if (read == -1) /* Handle EOF (Ctrl+D) */
{
free(line);
exit(0);
}

line[strcspn(line, "\n")] = '\0'; /* Remove newline character */
args[0] = line;
args[1] = NULL;

if (access(args[0], X_OK) == 0) /* Check if executable exists */
{
if (fork() == 0) /* Create child process */
{
execve(args[0], args, environ);
perror("./shell"); /* Print error if execve fails */
exit(1);
}
else
{
wait(NULL); /* Wait for child process to finish */
}
}
else
{
fprintf(stderr, "./shell: No such file or directory\n");
}
}
}

int main(void)
{
simple_shell();
return 0;
}

