/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>

#define NO_LFN(n) (!_use_lfn(n))

#include "oread.h"
#include "zread.h"

static void
Fatal(const char *msg)
{
  fprintf(stderr, "Fatal! %s!\n", msg);
  exit(1);
}

/*------------------------------------------------------------------------*/

typedef struct CE {
  struct CE *next;
  char *from;
  char *to;
} CE;

#define HASHSIZE 2048
#define HASHMASK 2047
#define HASHBITS 11
CE *htab[HASHSIZE];

static unsigned long
hash(unsigned char *cp)
{
  unsigned long rv = 0;
  while (*cp)
    rv += *cp++;
  while (rv > HASHMASK)
    rv = (rv & HASHMASK) + (rv >> HASHBITS);
  return rv;
}

static void
store_entry(char *from, char *to)
{
  unsigned long h = hash(from);
  CE *ce = (CE *)malloc(sizeof(CE));
  if (ce == 0)
    Fatal("Out of memory");
  ce->from = strdup(from);
  ce->to = strdup(to);
  ce->next = htab[h];
  htab[h] = ce;
}

static char *
get_entry(char *from)
{
  CE *ce;
  for (ce = htab[hash(from)]; ce; ce=ce->next)
  {
    if (strcmp(ce->from, from) == 0)
      return ce->to;
  }
  return from;
}

static void
DoNameChanges(char *fname)
{
  FILE *f = fopen(fname, "r");
  char from[PATH_MAX], to[PATH_MAX];
  char line[PATH_MAX*2 + 10];
  if (f == 0)
  {
    perror(fname);
    exit(1);
  }
  while (1)
  {
    fgets(line, sizeof(line), f);
    if (feof(f))
      break;
    to[0] = 0;
    sscanf(line, "%s %s", from, to);
    if (to[0])
      store_entry(from, to);
  }
  fclose(f);
}

/*------------------------------------------------------------------------*/

FILE *change_file;

int v_switch = 0;
int dot_switch = 1;
int text_unix = 0;
int text_dos = 0;
int z_switch = 0;
int to_stdout = 0;
int to_tty = 0;
int ignore_csum = 0;
int list_only = 1;
char skipped_str[] = "[skipped]";

char *only_dir;

/*------------------------------------------------------------------------*/

typedef struct CHANGE {
  struct CHANGE *next;
  char *old;
  char *new;
  int isdir; /* 0=file, 1=dir, 2=skip */
} CHANGE;

CHANGE *change_root = 0;

static void
dump_changes(void)
{
  CHANGE *c;
  for (c=change_root; c; c=c->next)
    fprintf(change_file, "%s -> %s\n", c->old, c->new);
}

int
change(char *fname, const char *problem, int isadir)
{
  CHANGE *ch;
  char new[PATH_MAX];
  char *pos;

  for (ch=change_root; ch; ch = ch->next)
    if ((strncmp(fname, ch->old, strlen(ch->old)) == 0) && ch->isdir
	/* Don't use change rules which failed to work before.  */
	&& access(ch->new, D_OK) == 0)
    {
      if (ch->isdir == 2)
      {
        fprintf(log_out, "  [ skipping %s ]\n", fname);
        return 0;
      }
/*      fprintf(log_out, "  [ changing %s to ", fname); */
      sprintf(new, "%s%s", ch->new, fname+strlen(ch->old));
      strcpy(fname, new);
/*      fprintf(log_out, "%s ]\n", fname); */
      return 1;
    }
  
  fprintf(log_out, "  %s %s\n  new name : ", problem, fname);
  fflush(log_out);
  new[0] = '\0';
  if (!gets(new))
    Fatal("EOF while reading stdin");

  if ((strcmp(new, "") == 0) && (isadir == 2))
    return 0;
  if (isadir) isadir=1;
  ch = (CHANGE *)malloc(sizeof(CHANGE));
  if (ch == 0)
    Fatal("Out of memory");
  ch->next = change_root;
  change_root = ch;
  ch->old = strdup(fname);
  pos = strrchr(fname, '/');
  if (pos && (strchr(new, '/') == 0))
  {
    if (new[0] == 0)
      ch->new = skipped_str;
    else
    {
      ch->new = (char *)malloc(strlen(new) + (pos-fname) + 2);
      if (ch->new == 0)
        Fatal("Out of memory");
      *pos = 0;
      sprintf(ch->new, "%s/%s", fname, new);
    }
  }
  else if (new[0] == 0)
    ch->new = skipped_str;
  else
    ch->new = strdup(new);
  ch->isdir = isadir;
  strcpy(fname, ch->new);
  if (new[0] == 0)
  {
    ch->isdir = 2;
    return 0;
  }
  return 1;
}

/*------------------------------------------------------------------------*/

int
isadir(char *n)
{
#ifdef __DJGPP__
  return (access(n, D_OK) == 0);
#else
  union REGS r;
  struct SREGS s;
  r.x.ax = 0x4300;
  r.x.dx = FP_OFF(n);
  s.ds = FP_SEG(n);
  int86x(0x21, &r, &r, &s);
  if (r.x.flags & 1)
    return 0;
  return r.x.cx & 0x10;
#endif
}

void
do_directories(char *n)
{
  char tmp[PATH_MAX];
  char *sl;
  int r;
  sprintf(tmp, "%s", n);
  n = tmp;
  for (sl=n; *sl; sl++)
  {
    if (sl > tmp && (*sl == '/' || *sl == '\\') && sl[-1] != ':')
    {
      char save = *sl;
      *sl = 0;
      do {
	if (isadir(n))
	  break;
	r = mkdir (n, 0777);
	if (r)
	  r = change(n, "Unable to create directory", 1);
      } while (r);
      *sl = save;
    }
  }
}

/*------------------------------------------------------------------------*/

/* Guess DOS file type by looking at its contents.  */
File_type
guess_file_type(char *buf, register size_t buflen)
{
  int crlf_seen = 0;
  /* Use unsigned char, so this will work with foreign characters.  */
  register unsigned char *bp = buf;

  while (buflen--)
    {
      /* Binary files have characters with ASCII code less then 32 decimal,
         unless they are one of: BS (for man pages), TAB, LF, FF, CR, ^Z. */
      if (*bp  < ' '  && !(*bp > 7 && *bp <= '\n') &&
          *bp != '\f' &&   *bp != '\r' && *bp != '\32')
        return DOS_BINARY;

      /* CR before LF means DOS text file (unless we later see
         binary characters).  */
      else if (*bp == '\r' && buflen && bp[1] == '\n')
        crlf_seen++;

      bp++;
    }

  return crlf_seen ? DOS_TEXT : UNIX_TEXT;
}

/*------------------------------------------------------------------------*/
char new[2048];  /* got to think about LFN's! */

char *
get_new_name(char *name_to_change, int *should_be_written)
{
  char *changed_name, *info;

  /* ONLY_DIR says to extract only files which are siblings
     of that directory.  */
  *should_be_written = list_only == 0;
  if (*should_be_written &&
      only_dir && strncmp(only_dir, name_to_change, strlen(only_dir)))
    *should_be_written = 0;

  changed_name = get_entry(name_to_change);
  if (*should_be_written && !to_stdout && NO_LFN(changed_name))
  {
    info = strstr(changed_name, ".info-");
    if (info)
    {
      strcpy(new, changed_name);
      info = strstr(new, ".info-");
      strcpy(info+2, info+6);
      fprintf(log_out, "[ changing %s to %s ]\n", changed_name, new);
    }
    else
    {
      char *tgz = strstr(changed_name, ".tar.gz");
      if (tgz)
      {
	strcpy(new, changed_name);
	tgz = strstr(new, ".tar.gz");
	strcpy(tgz, ".tgz");
	strcat(tgz, tgz+7);
	fprintf(log_out, "[ changing %s to %s ]\n", changed_name, new);
      }
      else
      {
	char *plus = strstr(changed_name, "++"), *plus2;
	if (plus)
	{
	  strcpy(new, changed_name);
	  plus2 = strstr(new, "++");
	  strcpy(plus2, "plus");
	  strcpy(plus2+4, plus+2);
	  fprintf(log_out, "[ changing %s to %s ]\n", changed_name, new);
	}
	else
	{
	  strcpy(new, changed_name);
	}
      }
    }
    changed_name = new;

    if (dot_switch)
    {
      char *p = changed_name, *name_start = changed_name;
      int state = 0;
      /* 0 = start of item
	 1 = before dot (not counting initial dot), but not first char
	 2 = after first dot */

      /* ".gdbinit" -> "_gdbinit"
	 "emacs-19.28.90/ChangeLog" -> "emacs-19.28-90/ChangeLog"
	 "./dir/file" -> "./dir/file"
	 "sh.lex.c" -> "sh_lex.c"
      */
      while (*p)
      {
	switch (*p++)
	{
	  case '/':
	  case '\\':
	    state = 0;
	    name_start = p;
	    break;
	  case '.':
	    /* Don't waste our limited 8-char real estate in the
	       name part too early, unless the name is really short. */
	    if (state == 1 && p - name_start < 5)
	    {
	      size_t namelen    = strlen(p);
	      char *next_slash  = memchr(p, '/', namelen);
	      char *next_bslash = memchr(p, '\\', namelen);
	      char *next_dot    = memchr(p, '.', namelen);

	      /* Find where this name ends.  */
	      if (next_slash == (char *)0)
	      {
		if (next_bslash)
		  next_slash = next_bslash;
		else
		  next_slash = p + namelen;
	      }

	      else if (next_bslash && next_bslash < next_slash)
		next_slash = next_bslash;

	      /* If name has more dots, convert this one to `_'. */
	      if (next_dot && next_dot < next_slash)
	      {
		p[-1] = '_';
		break;      /* don't bump `state' */
	      }
	    }

	    /* Leave "./", "../", "/." etc. alone.  */
	    if (state != 0 ||
		(*p && *p != '/' && *p != '\\' && *p != '.'))
	      p[-1] = "_.-"[state];
	    if (state < 2) state++;
	    break;
	  default:
	    if (state == 0) state = 1;
	}
      }
    }
  }
  return changed_name;
}

/*------------------------------------------------------------------------*/

/* We could have a file in a Unix archive whose name is reserved on
   MS-DOS by a device driver.  Trying to extract such a file would
   fail at best and wedge us at worst.  We need to rename such files.  */
void
rename_if_dos_device(char *fn)
{
  char *base = strrchr(fn, '/');
  struct stat st_buf;

  /* We don't care about backslashified file names because archives
     created on DOS cannot possibly include DOS device names.  */
  if (base)
    base++;
  else
    base = fn;

  /* The list of character devices is not constant: it depends on
     what device drivers did they install in their CONFIG.SYS.
     `stat' will tell us if the basename of the file name is a
     character device.  */
  if (stat(base, &st_buf) == 0 && S_ISCHR(st_buf.st_mode))
  {
    char orig[PATH_MAX];

    strcpy(orig, fn);
    /* Prepend a '_'.  */
    memmove(base + 1, base, strlen(base) + 1);
    base[0] = '_';
    fprintf(log_out, "[ changing %s to %s ]\n", orig, fn);
  }
}

/*------------------------------------------------------------------------*/

void
make_directory(char *dirname)
{
  int status;

  do_directories(dirname);	/* make sure parent exists */
  do {
    if (strcmp(dirname, ".") == 0)
      status = 0;		/* current dir always exists */
    else if (strcmp(dirname, "..") == 0)
      status = !isadir(dirname); /* root might have no parent */
    else
      status = mkdir(dirname, 0755);
    if (status && (errno==EEXIST))
    {
      status = change(dirname, "Duplicate directory name", 2);
      continue;
    }
    if (status)
      status = change(dirname, "Unable to create directory", 1);
    else
      fprintf(log_out, "Making directory %s\n", dirname);
  } while (status);
}

/*------------------------------------------------------------------------*/

FILE *log_out = stdout;
static char djtarx[] = "djtarx.exe";
static char djtart[] = "djtart.exe";

int
main(int argc, char **argv)
{
  int i = 1;
  char *tp;
  char *xp;

  progname = strlwr(strdup(argv[0]));

  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s [-n changeFile] [-p] [-i] [-t|x] [-o dir] [-v] [-u|d|b] [-[!].] tarfile...\n", progname);
    exit(1);
  }

  /* DJTARX -> ``djtar -x'', DJTART -> ``djtar -t''.  */
  tp = strstr(progname, djtart);
  xp = strstr(progname, djtarx);
  /* Check both with and without .exe, just in case.  */
  if (tp && (tp[sizeof(djtart)-1] == '\0' || tp[sizeof(djtart)-5] == '\0'))
    list_only = 1;
  else if (xp && (xp[sizeof(djtarx)-1] == '\0' || xp[sizeof(djtarx)-5] == '\0'))
    list_only = 0;
  while ((argc > i) && (argv[i][0] == '-') && argv[i][1])
  {
    switch (argv[i][1])
    {
      case 'n':
        DoNameChanges(argv[++i]);
        break;
      case 'v':
        v_switch = 1;
        break;
      case 'u':
	text_unix = 1;
	text_dos = 0;
	break;
      case 'd':
	text_dos = 1;
	text_unix = 0;
	break;
      case 'b':
	text_dos = 0;
	text_unix = 0;
	break;
      case '.':
	dot_switch = 1;
	break;
      case '!':
	if (argv[i][2] == '.')
	  dot_switch = 0;
	break;
      case 'o':
        only_dir = strdup(argv[++i]);
        break;
      case 'p':
        to_stdout = 1;
        to_tty = isatty(fileno(stdout));
        log_out = stderr;
        text_dos = 1;
        break;
      case 't':
        list_only = 1;
        break;
      case 'i':
        ignore_csum = 1;
        break;
      case 'x':
        list_only = 0;
    }
    i++;
  }

  for (; i < argc; i++)
    if(stricmp(argv[i] + strlen(argv[i]) - 4, ".zip") == 0)
      epunzip_read(argv[i]);
    else
      tar_gz_read(argv[i]);

  if (to_stdout)
  {
    setmode(fileno(stdout), O_TEXT);
    return 0;
  }
  else
  {
    change_file = fopen("tarchange.lst", "w");
    if (change_file != (FILE *)0)
    {
      dump_changes();
      fclose(change_file);
      return 0;
    }
    else
      return 1;
  }
}
