#include "shell.h"

/**
 * main - basic shell loop
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *args[64];
	int i;
	pid_t pid;

	while (1)
	{
		printf("$ ");
		read = getline(&line, &len, stdin);
		if (read == -1)
			break;

		line[read - 1] = '\0'; /* Remove newline */

		i = 0;
		args[i] = strtok(line, " ");
		while (args[i])
			args[++i] = strtok(NULL, " ");

		if (args[0] == NULL)
			continue;

		pid = fork();
		if (pid == 0)
		{
			if (execve(args[0], args, NULL) == -1)
				perror(args[0]);
			exit(EXIT_FAILURE);
		}
		else
			wait(NULL);
	}
	free(line);
	return (0);
}
