/* Copyright (C) 2001 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <libc/file.h>
#include <libc/dosio.h>
#include <io.h>

FILE *
freopen(const char *file, const char *mode, FILE *f)
{
  int fd, fdo, rw, oflags=0;
  char tbchar;

  if (file == 0 || mode == 0 || f == 0)
    return 0;

  rw = (mode[1] == '+') || (mode[1] && (mode[2] == '+'));

  fdo = fileno(f);
  fclose(f);

  switch (*mode) {
  case 'a':
    oflags = O_CREAT | (rw ? O_RDWR : O_WRONLY) | O_APPEND;
    break;
  case 'r':
    oflags = rw ? O_RDWR : O_RDONLY;
    break;
  case 'w':
    oflags = O_TRUNC | O_CREAT | (rw ? O_RDWR : O_WRONLY);
    break;
  default:
    return NULL;
  }
  if (mode[1] == '+')
    tbchar = mode[2];
  else
    tbchar = mode[1];
  if (tbchar == 't')
    oflags |= O_TEXT;
  else if (tbchar == 'b')
    oflags |= O_BINARY;
  else
    oflags |= (_fmode & (O_TEXT|O_BINARY));

  fd = open(file, oflags, 0666);
  if (fd < 0)
    return NULL;

  if(fd != fdo && fdo >= 0) {   /* Might rarely happen, but if it does for */
    dup2(fd, fdo);	/* stdin/stdout/stderr handles, we must fix it or */
    _close(fd);		/* child processes won't popen properly. */
    fd = fdo;
  }

  f->_cnt = 0;
  f->_file = fd;
  f->_bufsiz = 0;
  if (rw)
    f->_flag = _IORW;
  else if (*mode == 'r')
    f->_flag = _IOREAD;
  else
    f->_flag = _IOWRT;

  if (*mode == 'a')
  {
    llseek(fd, 0LL, SEEK_END);
  }

  f->_base = f->_ptr = NULL;
  return f;
}
