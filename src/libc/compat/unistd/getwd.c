/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <unistd.h>
#include <limits.h>

char *
getwd(char *buffer)
{
  if (buffer == 0)
    return 0;
  return getcwd(buffer, PATH_MAX);
}
