#include "shell.h"

/**
* print_prompt - Prints the prompt
*/
void print_prompt(void)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "($) ", 4);
}

/**
* read_line - Reads a line from stdin
* @line: Buffer to store the line
* @len: Size of the buffer
* Return: The number of characters read or -1 on error
*/
ssize_t read_line(char **line, size_t *len)
{
return (getline(line, len, stdin));
}

/**
* handle_exit - Handles the exit command
* @args: Command arguments
* @line: The input line
* @last_status: The last exit status
* Return: Exit status
*/
int handle_exit(char **args, char *line, int last_status)
{
int status = last_status;

if (args[1])
status = atoi(args[1]);

free(line);
free(args);
exit(status);
}

/**
* main - Entry point of the shell
* Return: Always 0
*/
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
char **args;
int status = 0;

while (1)
{
print_prompt();

read = read_line(&line, &len);
if (read == -1)  /* EOF case (Ctrl+D) */
{
free(line);
exit(status);  /* Exit with the last recorded case */
}

args = parse_line(line);
if (args[0] != NULL)
{
if (strcmp(args[0], "exit") == 0)
{
handle_exit(args, line, status);
}
else
{
status = execute_cmd(args);/* Save the exit status of the command execution */
}
}

free(args);
free(line); /* Freeing memory */
line = NULL; /* Reset variable */
len = 0; /* Reset buffer lemgth */
}


return (status);
}
