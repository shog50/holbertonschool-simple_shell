#include "shell.h"

#define READ_BUF_SIZE 1024

ssize_t _getline(char **lineptr, size_t *n)
{
    static char buffer[READ_BUF_SIZE];
    static size_t pos, len;
    char *new_lineptr, *newline;
    size_t new_size;
    ssize_t bytes_read;

    if (!lineptr || !n)
        return (-1);

    if (!*lineptr || *n == 0)
    {
        *n = READ_BUF_SIZE;
        *lineptr = malloc(*n);
        if (!*lineptr)
            return (-1);
    }

    pos = len = 0;
    while (1)
    {
        if (pos >= len)
        {
            bytes_read = read(STDIN_FILENO, buffer, READ_BUF_SIZE);
            if (bytes_read <= 0)
                return (-1);
            pos = 0;
            len = bytes_read;
        }

        (*lineptr)[0] = '\0';
        while (pos < len && (*lineptr)[*n - 1] != '\n')
        {
            (*lineptr)[pos] = buffer[pos];
            if (buffer[pos] == '\n')
            {
                (*lineptr)[pos + 1] = '\0';
                return (pos + 1);
            }
            pos++;
        }

        new_size = *n * 2;
        new_lineptr = realloc(*lineptr, new_size);
        if (!new_lineptr)
            return (-1);
        *lineptr = new_lineptr;
*n = new_size;
}
}
