#include "shell.h"
#include <unistd.h>

#define READ_SIZE 1024

ssize_t _getline(char **lineptr, size_t *n)
{
static char buffer[READ_SIZE];
static size_t start, end;
char *new_line = NULL;
size_t len = 0;
ssize_t read_count;

if (!lineptr || !n)
return (-1);

*lineptr = NULL;
*n = 0;

while (1)
{
if (start >= end)
{
read_count = read(STDIN_FILENO, buffer, READ_SIZE);
if (read_count <= 0)
return (len ? (ssize_t)len : -1);

start = 0;
end = read_count;
}

while (start < end)
{
if (len + 1 >= *n)
{
*n += READ_SIZE;
new_line = realloc(*lineptr, *n);
if (!new_line)
{
free(*lineptr);
return (-1);
}
*lineptr = new_line;
}

(*lineptr)[len++] = buffer[start++];
if ((*lineptr)[len - 1] == '\n')
{
(*lineptr)[len] = '\0';
return (len);
}
}
}
}
