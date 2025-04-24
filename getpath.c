#include <string.h>
#include "shell.h"

extern char **environ;

char *_getpath(void)
{
 int i = 0;
 char *path = NULL;

 while (environ[i])
 {
  if (strncmp(environ[i], "PATH=", 5) == 0)
  {
   path = environ[i] + 5; /* skip "PATH=" */
   break;
  }
  i++;
 }

 return path;
}
