/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

extern int errno;

#include "oread.h"
#include "zread.h"

/*
extern int text_unix;
extern int text_dos;
extern int ignore_csum;
*/

extern int to_stdout;
extern int to_tty;
extern int list_only;
extern FILE *log_out;

#define CRYFLG 1
#define EXTFLG 8

static int epoutfile = 0;
static char *changed_name = NULL;

int epcopy(char *, long);

void
epunzip_read(char *zipfilename)
{
  clear_bufs();
  errno = 0;
  ifd = oread_open(zipfilename);
  if(errno)
    {
      fprintf(log_out, "%s: %s\n", zipfilename, strerror(errno));
      return;
    }

  while(1)
    {
      int buffer, ext_header, timedate, crc, size, length, name_length,
	extra_length, should_be_written, count, real_file = 0;
      char filename[2048];

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();

      if(*(short *)&buffer != *(short *)"PK")
	{
	  fprintf(log_out, "%s: invalid zip file structure\n", zipfilename);
	  break;
	}

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();

      if(*(short *)&buffer != *(short *)"\3\4")
	{
	  /* not a local header - all done */
	  break;
	}

      /* version info - ignore it */
      get_byte();
      get_byte();

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();

      if(*(short *)&buffer & CRYFLG)
	{
	  fprintf(log_out, "%s has encrypted file(s) - use unzip\n", zipfilename);
	  break;
	}
      ext_header = *(short *)&buffer & EXTFLG ? 1 : 0;

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();

      method = *(short *)&buffer;
      if(method != 8 && method != 0)
	{
	  fprintf(log_out, "%s has file(s) compressed with unsupported method - use unzip\n", zipfilename);
	  break;
	}

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();
      ((char *)&buffer)[2] = (char)get_byte();
      ((char *)&buffer)[3] = (char)get_byte();
      timedate = buffer;

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();
      ((char *)&buffer)[2] = (char)get_byte();
      ((char *)&buffer)[3] = (char)get_byte();
      crc = buffer;

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();
      ((char *)&buffer)[2] = (char)get_byte();
      ((char *)&buffer)[3] = (char)get_byte();
      size = buffer;

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();
      ((char *)&buffer)[2] = (char)get_byte();
      ((char *)&buffer)[3] = (char)get_byte();
      length = buffer;

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();
      name_length = *(short *)&buffer;

      ((char *)&buffer)[0] = (char)get_byte();
      ((char *)&buffer)[1] = (char)get_byte();
      extra_length = *(short *)&buffer;

      for(count = 0; count < name_length; count++)
	{
	  filename[count] = (char)get_byte();
	}
      filename[name_length] = 0;

      for(count = 0; count < extra_length; count++)
	{
	  get_byte();
	}

      changed_name = get_new_name(filename, &should_be_written);

      fprintf(log_out, "%s%s\n", changed_name,
              !should_be_written && !list_only ? "\t[ skipped ]" : "");

      if(!should_be_written)
	epoutfile = open("/dev/null",
			 O_WRONLY | O_BINARY,
			 S_IWRITE | S_IREAD);
      else if(changed_name[strlen(changed_name) - 1] == '/' && !to_stdout)
	{
	  changed_name[strlen(changed_name) - 1] = 0;
	  make_directory(changed_name);
	  continue;
	}
      else
	{
	open_file:
	  if(!to_stdout)
	    {
	      do_directories(changed_name);
	      rename_if_dos_device(changed_name);
	      epoutfile = open(changed_name,
			       O_WRONLY | O_BINARY | O_CREAT | O_EXCL,
			       S_IWRITE | S_IREAD);
	      if(epoutfile < 0)
	      {
		if(change(changed_name, "Cannot exclusively open file", 0))
		  goto open_file;
		else
		  epoutfile = open("/dev/null",
				   O_WRONLY | O_BINARY,
				   S_IWRITE | S_IREAD);
	      }
	      real_file = 1;
	    }
	  else
	    {
	      epoutfile = fileno(stdout);
	      if(!to_tty)
		setmode(epoutfile, O_BINARY);
	    }
	}

      updcrc(NULL, 0);

      if(method == 0) /* stored */
	{

	  if(ext_header)
	    {
	      fprintf(log_out, "%s has stored file with extended local header\
\n\aKill the program that produced it!!!\n", zipfilename);
	      break;
	    }

	  if(size != length)
	    {
	      fprintf(log_out, "%s has stored file with different lengths\n",
		      zipfilename);
	      break;
	    }

	  for(count = length; count; count--)
	    {
	      char c = (char)get_byte();
	      put_ubyte(c, epcopy);
	    }
	  flush_window(epcopy);

	}
      else /* deflated */
	{
	  if(inflate(epcopy))
	    {
	      fprintf(log_out, "inflation failed on %s\n", zipfilename);
	      break;
	    }

	  if(ext_header)
	    {
	      ((char *)&buffer)[0] = (char)get_byte();
	      ((char *)&buffer)[1] = (char)get_byte();
	      ((char *)&buffer)[2] = (char)get_byte();
	      ((char *)&buffer)[3] = (char)get_byte();
	      if(buffer != *(int *)"PK\7\x8")
		{
		  fprintf(log_out, "%s: invalid zip file structure\n",
			  zipfilename);
		  break;
		}

	      ((char *)&buffer)[0] = (char)get_byte();
	      ((char *)&buffer)[1] = (char)get_byte();
	      ((char *)&buffer)[2] = (char)get_byte();
	      ((char *)&buffer)[3] = (char)get_byte();
	      crc = buffer;

	      ((char *)&buffer)[0] = (char)get_byte();
	      ((char *)&buffer)[1] = (char)get_byte();
	      ((char *)&buffer)[2] = (char)get_byte();
	      ((char *)&buffer)[3] = (char)get_byte();
	      size = buffer;

	      ((char *)&buffer)[0] = (char)get_byte();
	      ((char *)&buffer)[1] = (char)get_byte();
	      ((char *)&buffer)[2] = (char)get_byte();
	      ((char *)&buffer)[3] = (char)get_byte();
	      length = buffer;
	    }

	}

      if((unsigned)crc != updcrc(outbuf, 0))
	{
	  fprintf(log_out, "invalid crc on %s\n", zipfilename);
	  break;
	}

      if(epoutfile != fileno(stdout))
	close(epoutfile);

      if(real_file)
	{
	  epoutfile = open(changed_name, O_RDONLY);
	  setftime(epoutfile, (struct ftime *)&timedate);
	  close(epoutfile);
	  real_file = 0;
	}
    }

  oread_close(ifd);
}

int
epcopy(char *buffer, long size)
{
  errno = 0;
  if(write(epoutfile, buffer, size) < size)
    {
      if (errno == 0)
	errno = ENOSPC;
      fprintf(log_out, "%s: %s\n", changed_name, strerror(errno));
    }
  return 0;
}
