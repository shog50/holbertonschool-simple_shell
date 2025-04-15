#include "simple_shell.h"

/**
* simple_shell - The main loop of the simple shell program.
*/
void simple_shell(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
char *args[2];

while (1)
{
printf(PROMPT); /* Display the prompt */
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
perror("./hsh"); /* Print error if execve fails */
exit(1);
}
else
{
wait(NULL); /* Wait for child process to finish */
}
}
else
{
fprintf(stderr, "./hsh: No such file or directory\n");
}
}
}

