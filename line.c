#include "shell.h"

/**
 * parse_line - splits a line into tokens (arguments)
 * @line: the input line
 *
 * Return: array of strings (tokens)
 */
char **parse_line(char *line)
{
	int bufsize = 64, i = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " \n\t");
	while (token)
	{
		tokens[i++] = token;
		if (i >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, " \n\t");
	}
	tokens[i] = NULL;


	return (tokens);
}
