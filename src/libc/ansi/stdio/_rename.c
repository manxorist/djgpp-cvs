/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <dos.h>
#include <go32.h>
#include <dpmi.h>
#include <libc/dosio.h>
 
int _rename(const char *old, const char *new)
{
  __dpmi_regs r;
  int olen    = strlen(old) + 1;
  int i;
  int use_lfn = _USE_LFN;
  char tempfile[FILENAME_MAX], tempfile1[FILENAME_MAX];
  const char *orig = old;
  int lfn_fd = -1;

  /* If OLD and NEW are the same file, do nothing.  */
  if (__file_exists(new)
      && strcmp(_truename(old, tempfile), _truename(new, tempfile1)) == 0)
    return 0;

  r.x.dx = __tb_offset;
  r.x.di = __tb_offset + olen;
  r.x.ds = r.x.es = __tb_segment;

  if (use_lfn)
  {
    /* Windows 95 bug: for some filenames, when you rename
       file -> file~ (as in Emacs, to leave a backup), the
       short 8+3 alias doesn't change, which effectively
       makes OLD and NEW the same file.  We must rename
       through a temporary file to work around this.  */

    char *pbase = 0, *p;
    static char try_char[] = "abcdefghijklmnopqrstuvwxyz012345789";
    int idx = sizeof(try_char)-1;

    /* Generate a temporary name.  Can't use `tmpnam', since $TMPDIR
       might point to another drive, which will fail the DOS call.  */
    strcpy(tempfile, old);
    for (p = tempfile; *p; p++)	/* ensure temporary is on the same drive */
      if (*p == '/' || *p == '\\' || *p == ':')
	pbase = p;
    if (pbase)
      pbase++;
    else
      pbase = tempfile;
    strcpy(pbase, "X$$djren$$.$$temp$$");

    do
    {
      if (idx <= 0)
	return -1;
      *pbase = try_char[--idx];
    } while (_chmod(tempfile, 0) != -1);

    r.x.ax = 0x7156;
    _put_path2(tempfile, olen);
    _put_path(old);
    __dpmi_int(0x21, &r);
    if (r.x.flags & 1)
    {
      errno = __doserr_to_errno(r.x.ax);
      return -1;
    }

    /* Now create a file with the original name.  This will
       ensure that NEW will always have a 8+3 alias
       different from that of OLD.  (Seems to be required
       when NameNumericTail in the Registry is set to 0.)  */
    lfn_fd = _creat(old, 0);

    olen = strlen(tempfile) + 1;
    old  = tempfile;
    r.x.di = __tb_offset + olen;
  }

  for (i=0; i<2; i++)
  {
    if(use_lfn)
      r.x.ax = 0x7156;
    else if ((_osmajor > 7 && _osmajor < 10) /* OS/2 returns v10 and above */
	     || (_osmajor == 7 && _osminor >= 20))
    {
      /* DOS 7.20 (Windows 98) and later supports a new function with
	 a maximum path length of 128 characters instead of 67.  This
	 is important for deeply-nested directories.  */
      r.x.ax = 0x43ff;
      r.x.bp = 0x5053;
      r.h.cl = 0x56;
    }
    else
      r.h.ah = 0x56;
    _put_path2(new, olen);
    _put_path(old);
    __dpmi_int(0x21, &r);
    if(r.x.flags & 1)
    {
      if (i == 0
	  && (r.x.ax == 5 || (r.x.ax == 2 && __file_exists(old))))
	remove(new);		 /* and try again */
      else
      {
	errno = __doserr_to_errno(r.x.ax);

	/* Restore to original name if we renamed it to temporary.  */
	if (use_lfn)
	{
	  if (lfn_fd != -1)
	  {
	    _close (lfn_fd);
	    remove (orig);
	  }
	  _put_path2(orig, olen);
	  _put_path(tempfile);
	  r.x.ax = 0x7156;
	  __dpmi_int(0x21, &r);
	}
	return -1;
      }
    }
    else
      break;
  }

  /* Success.  Delete the file possibly created to work
     around the Windows 95 bug.  */
  if (lfn_fd != -1)
    return (_close (lfn_fd) == 0) ? remove (orig) : -1;
  return 0;
}

#ifdef TEST

#include <string.h>

int main(int argc, char *argv[])
{
  if (argc > 2)
    {
      printf ("%s -> %s: ", argv[1], argv[2]);
      if (_rename (argv[1], argv[2]))
	printf ("%s\n", strerror (errno));
      else
	printf ("Done\n");
    }
  return 0;
}

#endif
