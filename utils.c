#include "shell.h"

/**
 * _strcmp - Compare two strings
 * @s1: first string
 * @s2: second string
 * Return: difference between characters or 0 if equal
 */
int _strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

/**
 * _strchr - Locate character in string
 * @s: string to search
 * @c: character to find
 * Return: pointer to first occurrence or NULL
 */
char *_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    if (c == '\0')
        return (char *)s;
    return NULL;
}
