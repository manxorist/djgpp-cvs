/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <process.h>
#include <go32.h>
#include <dpmi.h>
#include <ctype.h>
#include <sys/system.h>
#include <sys/movedata.h>
#include <libc/dosexec.h>
#include <libc/unconst.h>
#include <libc/dosio.h>
#include <libc/farptrgs.h>

/* FIXME: this is not LFN-clean.  Win95 has a way to
   pass long command lines, but we don't support it here.  */
#define CMDLEN_LIMIT 125

extern char **environ;

int __dosexec_in_system = 0;

typedef struct {
  unsigned short eseg;
  unsigned short argoff;
  unsigned short argseg;
  unsigned short fcb1_off;
  unsigned short fcb1_seg;
  unsigned short fcb2_off;
  unsigned short fcb2_seg;
} Execp;

static Execp parm;

static unsigned long tbuf_ptr;
static unsigned long tbuf_beg;
static unsigned long tbuf_end;
static unsigned long tbuf_len;
#if 0
static int	     tbuf_selector;
#endif

static int script_exec(const char *, char **, char **);

/* Allocate AMT bytes off the transfer buffer.  */
static unsigned long talloc(size_t amt)
{
  unsigned long rv = tbuf_ptr;
  tbuf_ptr += amt;
  return rv;
}

/* Make sure we can allocate AMT bytes off the transfer buffer
   without overflowing it.  Return non-zero if we can, zero otherwise.

   WARNING: This can relocate the data already in the transfer buffer,
	    so all linear addresses which use it should be relative to
	    TBUF_BEG!  */
static int check_talloc(size_t amt)
{
  int retval = 1;

  if (tbuf_ptr + amt > tbuf_end)
  {
#if 0
    /* Code that reallocs the transfer buffer; currently disabled.  */
    unsigned long new_tb;
    unsigned long min_len = tbuf_len + amt;
    unsigned long max_len = 0x10000; /* 64KB */
    int old_selector = tbuf_selector;
    int max_avail;
    int e = errno;

    errno = E2BIG;

    /* Try to allocate new, larger DOS buffer, upto 64KB.  */
    if (min_len > max_len)
    {
      retval = 0;
      goto done;
    }
    while (tbuf_len <= max_len && tbuf_len < min_len)
      tbuf_len *= 2;
    if (tbuf_len < min_len)
    {
      retval = 0;
      goto done;
    }

    tbuf_len = (tbuf_len + 15) & 0xffff0; /* round to nearest paragraph */

    if ((new_tb =
	 __dpmi_allocate_dos_memory(tbuf_len/16, &max_avail)) == -1)
    {
      if (max_avail*16 < min_len
	  || (new_tb =
	      __dpmi_allocate_dos_memory(max_avail, &tbuf_selector)) == -1)
      {
	retval = 0;
	goto done;
      }
      tbuf_len = max_avail*16;
    }
    else
      tbuf_selector = max_avail;

    new_tb *= 16;  /* convert to linear address */
    movedata (_dos_ds, tbuf_beg, _dos_ds, new_tb, tbuf_ptr - tbuf_beg);
    tbuf_ptr = new_tb + tbuf_ptr - tbuf_beg;
    tbuf_beg = new_tb;
    tbuf_end = tbuf_beg + tbuf_len - 1;

    errno = e;

  done:
    /* Assume caller will return immediately in case of
       failure to reallocate, so they won't need the old data.  */
    if (!retval)
      tbuf_selector = 0;
    if (old_selector)
      __dpmi_free_dos_memory(old_selector);
#else
    errno = E2BIG;
    retval = 0;
#endif
  }

  return retval;
}

extern char   __PROXY[];	/* defined on crt0/crt1.c */
extern size_t __PROXY_LEN;

/* Functions that call `direct_exec_tail' after they've put
   some data into the transfer buffer, should set LFN parameter
   to either 0 (no LFN support) or 1 (LFN supported), but NOT 2!
   if LFN is 2, there is a possiblity that the contents of the
   transfer buffer will be overrun!  */
static int
direct_exec_tail_1 (const char *program, const char *args,
		 char * const envp[], const char *proxy, int lfn)
{
  __dpmi_regs r;
  unsigned long program_la;
  unsigned long arg_la;
  unsigned long parm_la;
  unsigned long env_la, env_e_la;
  size_t proxy_len = proxy ? strlen(proxy)+1 : 0;
  int seen_proxy = 0;
  char arg_header[3];
  char short_name[FILENAME_MAX];
  const char *progname;
  unsigned proglen;
  int i;
  unsigned long fcb1_la, fcb2_la, fname_la;
  
  /* This used to just call sync().  But `sync' flushes the disk
     cache nowadays, and that can slow down the child tremendously,
     since some caches (e.g. SmartDrv) invalidate all of their
     buffers when `_flush_disk_cache' is called.  */
  for (i = 0; i < 255; i++)
    fsync(i);

  if (lfn == 2)		/* don't know yet */
    lfn = _USE_LFN;

  /* The pathname of the executable to run.  */
  proglen = strlen(program)+1;
  if (!check_talloc(proglen))
    return -1;
  /* Make sure any magic names, like /dev/c/foo, are converted to the
     usual DOS form, and, under LFN, to the short 8+3 alias.  */
  _put_path2(program, tbuf_beg == __tb ? tbuf_ptr - tbuf_beg : 0);
  if(lfn) {
    r.x.ax = 0x7160;			/* Truename */
    r.x.cx = 1;				/* Get short name */
    r.x.ds = r.x.es = tbuf_ptr / 16;
    r.x.si = r.x.di = tbuf_ptr & 15;
    __dpmi_int(0x21, &r);
    if (r.x.flags & 1)
    {
      errno = __doserr_to_errno(r.x.ax);
      return -1;
    }
  }
  dosmemget(tbuf_beg == __tb ? tbuf_ptr : __tb, FILENAME_MAX, short_name);
  progname = short_name;
  proglen = strlen(short_name)+1;

  if (!check_talloc(proglen + strlen(args) + 3 + sizeof(Execp) + 48))
    return -1;
  program_la = talloc(proglen);
  arg_la     = talloc(strlen(args)+3);
  parm_la    = talloc(sizeof(Execp));

  dosmemput(progname, proglen, program_la);

  /* The command-line tail.  */
  arg_header[0] = strlen(args);
  arg_header[1] = '\r';
  dosmemput(arg_header, 1, arg_la);
  dosmemput(args, strlen(args), arg_la+1);
  dosmemput(arg_header+1, 1, arg_la+1+strlen(args));

  /* The 2 FCBs.  Some programs (like XCOPY from DOS 6.x) need them.  */
  fcb1_la = talloc(16);	       /* allocate space for 1st FCB */
  fname_la = arg_la + 1;       /* first character of command tail */
  r.x.ax = 0x2901;	       /* AL = 1 means skip leading separators */
  r.x.ds = fname_la / 16;      /* pointer to 1st cmd argument */
  r.x.si = fname_la & 15;
  r.x.es = fcb1_la / 16;       /* pointer to FCB buffer */
  r.x.di = fcb1_la & 15;
  __dpmi_int (0x21, &r);

  /* We cannot be sure that Int 21h/AX=2901h parsed the entire
     first command-line argument (it might not be a filename
     at all!).  We need to get to the next command-line arg
     before calling 2901 again.  2901 returns the pointer to
     first unparsed character in DS:SI.

     Note that, in case there is no second command-line argument,
     the following loop is terminated by the trailing CR which
     ends the command-line tail.  */
  for (_farsetsel(_dos_ds), fname_la = ((unsigned)r.x.ds) * 16 + r.x.si;
       !isspace(_farnspeekb(fname_la));
       fname_la++)
    ;

  fcb2_la = talloc(16);
  r.x.ax = 0x2901;
  r.x.ds = fname_la / 16;      /* begin parsing 2nd arg from here */
  r.x.si = fname_la & 15;
  r.x.es = fcb2_la / 16;
  r.x.di = fcb2_la & 15;
  __dpmi_int (0x21, &r);

  /* The environment must be on a segment boundary, so get
     to the first location in the transfer buffer whose
     linear address is divisable by 16.  */
  do {
    env_la = talloc(1);
  } while (env_la & 15);
  talloc(-1);

#if 0
  /* Convert to relative, since `check_talloc' may relocate.  */
  arg_la  -= tbuf_beg;
  env_la  -= tbuf_beg;
  fcb1_la -= tbuf_beg;
  fcb2_la -= tbuf_beg;
  parm_la -= tbuf_beg;
  program_la -= tbuf_beg;
#endif

  /* The environment.  Replace the !proxy variable, if there is
     one (for nested programs) if we are called from `system',
     or skip it, if we are called from `spawnXX'.  */
  for (i=0; envp[i]; i++)
  {
    const char *ep = envp[i];
    size_t env_len = strlen(ep)+1;

    if (strncmp(ep, __PROXY, __PROXY_LEN) == 0 && ep[__PROXY_LEN] == '=')
    {
      seen_proxy = 1;
      if (proxy)
      {
	ep = proxy;
	env_len = proxy_len;
      }
      else
	continue;
    }
    if (!check_talloc(env_len))
      return -1;
    env_e_la = talloc(env_len);
    dosmemput(ep, env_len, env_e_la);
  }

  /* If no !proxy variable was found, create one.  */
  if (proxy && !seen_proxy)
  {
    if (!check_talloc(proxy_len))
      return -1;
    env_e_la = talloc(proxy_len);
    dosmemput(proxy, proxy_len, env_e_la);
  }

  /* Terminate by an extra NULL char.  */
  arg_header[0] = 0;

  /* The name of the program that owns the environment.  */
  arg_header[1] = 1;	/* the number of strings (1, little-endian) */
  arg_header[2] = 0;
  if (!check_talloc(3 + proglen))
    return -1;
  dosmemput(arg_header, 3, talloc(3));
  env_e_la = talloc(proglen);
  dosmemput(progname, proglen, env_e_la);

  /* Prepare the parameter block and call Int 21h/AX=4B00h.  */
#if 0
  arg_la  += tbuf_beg;
  env_la  += tbuf_beg;
  fcb1_la += tbuf_beg;
  fcb2_la += tbuf_beg;
  parm_la += tbuf_beg;
  program_la += tbuf_beg;
#endif
  parm.eseg     = env_la / 16;
  parm.argseg	= arg_la / 16;
  parm.argoff	= arg_la & 15;
  parm.fcb1_seg = fcb1_la / 16;
  parm.fcb1_off = fcb1_la & 15;
  parm.fcb2_seg = fcb2_la / 16;
  parm.fcb2_off = fcb2_la & 15;
  dosmemput(&parm, sizeof(parm), parm_la);

  r.x.ax = 0x4b00;
  r.x.ds = program_la / 16;
  r.x.dx = program_la & 15;
  r.x.es = parm_la / 16;
  r.x.bx = parm_la & 15;
  __dpmi_int(0x21, &r);
#if 0
  if (tbuf_selector)
    __dpmi_free_dos_memory (tbuf_selector);
  tbuf_selector = 0;
#endif
  if (r.x.flags & 1)
  {
    errno = __doserr_to_errno(r.x.ax);
    return -1;
  }
  
  r.h.ah = 0x4d;
  __dpmi_int(0x21, &r);
  
  if (r.x.flags & 1)
  {
    errno = __doserr_to_errno(r.x.ax);
    return -1;
  }

  /* AH holds the ``system exit code'' which is non-zero if the
     child was aborted by Ctrl-C, or Critical Device error (also
     if the child installs itself as a TSR).  */
  if (r.h.ah && r.h.ah != 3) /* 3 means it exited as TSR (is it ``normal''?) */
    {
      errno = EINTR;	/* what else can we put in `errno'? */
      return ( ((r.h.ah == 1 ? SIGINT : SIGABRT) << 8) | r.h.al );
    }
  return r.h.al;	/* AL holds the child exit code */
}

static int direct_exec_tail (const char *program, const char *args,
		 char * const envp[], const char *proxy, int lfn)
{
  int i, ret;
  int sel1, sel2;
  static char first_call = 1;
  static char workaround_descriptor_leaks = 0;
  static unsigned char larbyte = 0;
  static int how_deep = 100;		/* Tunable parameter */
  char desc_map[how_deep];

  sel1 = sel2 = 0;			/* Clear warnings */

  if (first_call)			/* One time algorithm detection */
  {
    int flags;
    char dpmi_vendor[128];
    unsigned char desc_buf1[8], desc_buf2[8];

    /* Disable descriptors leak workaround when CWSDPMI is being used */
    /* since not needed.  Does not work with CWSDPMI versions before  */
    /* r5 as corresponding DPMI call is supported beginning with v5.  */

    ret = __dpmi_get_capabilities(&flags,dpmi_vendor);
    if (ret == 0 && strcmp(dpmi_vendor+2,"CWSDPMI") == 0)
      workaround_descriptor_leaks = 0;
    else {

      /* Test the DPMI provider to see how it behaves.  We allocate
         a descriptor, get it's access rights and full 8 byte descriptor.
         We then free it, and see what changed.  The present bit, run
         ring and selector type (user) might change.  Algorithm 1: These
         can be detected with a single hardware instruction (LAR).
         __dpmi_get_descriptor_access_rights is actually not an interrupt
         but just a wrapper around this instruction.  However, Win NT/2K
         do something strange on the LAR, so once a selector is allocated
         it stays visable.  Algorithm 2: Get the full descriptor and
         compare extracted LAR nibble.  If the get descriptor call fails
         we also know this descriptor is currently not user allocated. */

      sel1 = __dpmi_allocate_ldt_descriptors(1);
      larbyte = 0xf0 & __dpmi_get_descriptor_access_rights(sel1);
      __dpmi_get_descriptor(sel1, &desc_buf1);
      __dpmi_free_ldt_descriptor(sel1);
      flags = __dpmi_get_descriptor_access_rights(sel1);	/* freed */

      if (larbyte != (flags & 0xf0)) {	/* present+ring+sys changed */
        workaround_descriptor_leaks = 1;

      } else {				/* Win NT/2K/XP lie about lar */
        larbyte = desc_buf1[5] & 0xf0;
        ret = __dpmi_get_descriptor(sel1, &desc_buf2);
        if (ret == -1 || (larbyte != (desc_buf2[5] & 0xf0))) {
          workaround_descriptor_leaks = 2;
        } else
          workaround_descriptor_leaks = 0;	/* Don't do anything */
      }
    }
    first_call = 0;
  }

  if (workaround_descriptor_leaks)		/* Create the unused map */
  {
    unsigned char desc_buf[8];
    char * map = desc_map;

    /* Create a map of the free descriptors in the probable range the
       children will use.  We start by allocating a descriptor, assuming
       it will be close to the start of what the children will use.  We
       then scan an area "how_deep" beyond that.  Since a DJGPP app will
       leak 4 descriptors if it's all cleaned up, we allocate 25 times that
       much - maybe one of them is before this fix.  We use the algorithm
       determined in the one pass identification. */

    sel1 = __dpmi_allocate_ldt_descriptors(1);
    if(sel1 < _dos_ds) {			/* Failure -1 also matches */
      sel1 = _dos_ds;				/* Our last descriptor */
      *map++ = 0;				/* don't free it */
    } else
      *map++ = 1;				/* sel1 always released */
    sel2 = sel1 + 8 * how_deep - 8;		/* end of scan range inclusive */

    if (workaround_descriptor_leaks == 1) {
      for (i=sel1+8; i<=sel2; i+=8)
        *map++ = (__dpmi_get_descriptor_access_rights(i) & 0xf0) != larbyte;

    } else if (workaround_descriptor_leaks == 2) {
      for (i=sel1+8; i<=sel2; i+=8)
        if ((__dpmi_get_descriptor_access_rights(i) & 0xf0) != larbyte)
          *map++ = 1;				/* Never touched, free it */
        else if (__dpmi_get_descriptor(i, &desc_buf) == -1)
          *map++ = 1;				/* Means free on NT */
        else
          *map++ = (desc_buf[5] & 0xf0) != larbyte;
    }
  }

  ret = direct_exec_tail_1 ( program, args, envp, proxy, lfn);

  if (workaround_descriptor_leaks)		/* Free the unused map */
  {
    int endsel;
    char * map;

    /* First, allocate a selector to estimate the end of the leaked range.
       This is only used to determine if our "how_deep" range was adequate.
       Use the map of the free descriptors created before the children
       were spawned.  For algorithm 1 since the LAR instruction is fast
       we check first to see if the selector is allocated before deallocating.
       For algorithm 2, the check is a DPMI call and slow, so we just free
       the descriptors which were free before we called the children and
       touched according to the LAR byte.  Finally, if the "how_deep"
       range was too small, we permanently lose the extra selectors, but
       we double the range beyond what we saw this time.  If we lose a few
       it's the price we pay for better performance.  */

    map = desc_map + how_deep;

    endsel = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_free_ldt_descriptor (endsel);

    if (workaround_descriptor_leaks) {		/* Algorithms 1 & 2 */
      for (i=sel2; i>=sel1; i-=8)
        if (*--map)
          if ((__dpmi_get_descriptor_access_rights(i) & 0xf0) == larbyte)
            __dpmi_free_ldt_descriptor(i);
    }

    if (endsel > sel2) {
      how_deep = (endsel - sel1) / 4;		/* Twice what's needed */
#if 0
      for (i=endsel-8; i>=sel2; i-=8)		/* Unsafe free, no map */
        __dpmi_free_ldt_descriptor(i);
#endif
    }
  }
  return ret;
}

int
_dos_exec(const char *program, const char *args, char * const envp[])
{
  tbuf_beg = tbuf_ptr = __tb;
  tbuf_len = _go32_info_block.size_of_transfer_buffer;
  tbuf_end = tbuf_beg + tbuf_len - 1;
  return direct_exec_tail(program, args, envp, 0, 2);
}

static char GO32_V2_STRING[] = "go32-v2.exe";
static char GO32_STRING[]    = "go32.exe";

/* A list of known shells which require we DON'T quote command
   lines that are passed to them with the /c or -c switch.  */
static const char *shell_brokets[] = {
  "COMMAND.COM",
  "4DOS.COM",
  "NDOS.COM",
  0
};

/* A list of Unix-like shells and other non-DJGPP programs
   which treat single quote specially.  */
static const char *unix_shells[] = {
  "SH.EXE",
  "-SH.EXE", /* people create `-sh.exe' and `-bash.exe' to have login shells */
  "SH16.EXE",
  "SH32.EXE",
  "TCSH.EXE",
  "-TCSH.EXE",
  "BASH.EXE",
  "-BASH.EXE",
  0
};

static int
list_member (const char *program, const char *program_list[])
{
  const char *p = program, *ptail = program;
  int i;

  while (*p)
  {
    if (*p == '/' || *p == ':' || *p == '\\')
      ptail = p + 1;
    p++;
  }

  for (i = 0; program_list[i]; i++)
    if (!stricmp (ptail, program_list[i]))
      return 1;

  return 0;
}

int
_is_unixy_shell (const char *shellpath)
{
  return list_member (shellpath, unix_shells);
}

int
_is_dos_shell (const char *shellpath)
{
  return list_member (shellpath, shell_brokets);
}

static int direct_exec(const char *program, char **argv, char **envp)
{
  int i, arglen;
  char *args, *argp;
  int need_quote = !__dosexec_in_system;
  int unescape_quote = __dosexec_in_system;

  /* PROGRAM can be a shell which expects a single argument
     (beyond the /c or -c switch) that is the entire command
     line.  With some shells, we must NOT quote that command
     line, because that will confuse the shell.

     The hard problem is to know when PROGRAM names a shell
     that doesn't like its command line quoted...  */

  if (need_quote
      && argv[1] && !strcmp (argv[1], "/c")
      && argv[2] && !argv[3]
      && _is_dos_shell (program))
    need_quote = 0;

  if (unescape_quote && _is_unixy_shell (program))
    unescape_quote = 0;

  arglen = 0;
  for (i=1; argv[i]; i++)
    arglen += 2*strlen(argv[i]) + 1 + 2;

  args = (char *)alloca(arglen+1);
  argp = args;
  for (i=1; argv[i]; i++)
  {
    int quoted = 0;
    const char *p = argv[i];

    if (argp - args > CMDLEN_LIMIT)
      break;
    *argp++ = ' ';
    /* If invoked by `spawnXX' or `execXX' functions, we need to
       quote arguments which include whitespace, so they end up
       as a single argument on the child side.
       We will invoke PROGRAM directly by DOS Exec function (not
       through COMMAND.COM), therefore no need to quote characters
       special only to COMMAND.COM.
       We also assume that DJGPP programs aren't invoked through
       here, so a single quote `\'' is also not special.  The only
       programs other than DJGPP that treat a single quote specially
       are Unix-like shells, but whoever uses them should know to
       escape the quotes himself.  */
    if (need_quote && strpbrk(p, " \t") != 0)
    {
      *argp++ = '"';
      quoted = 1;
    }
    while (*p)
    {
      if (argp - args > CMDLEN_LIMIT)
        break;
      if (*p == '"' && (quoted || need_quote))
	*argp++ = '\\';
      /* Most non-DJGPP programs don't treat `\'' specially,
	 but our `system' requires we always escape it, so
	 we should undo the quoting here.  */
      else if (*p == '\\' && p[1] == '\'' && unescape_quote)
	p++;
      *argp++ = *p++;
    }
    if (quoted && argp - args <= CMDLEN_LIMIT)
      *argp++ = '"';
  }
  *argp = 0;

  if (argp - args > CMDLEN_LIMIT)
    errno = E2BIG;
  
  tbuf_beg = tbuf_ptr = __tb;
  tbuf_len = _go32_info_block.size_of_transfer_buffer;
  tbuf_end = tbuf_beg + tbuf_len - 1;
  return direct_exec_tail(program, args, envp, 0, 2);
}

static int go32_exec(const char *program, char **argv, char **envp)
{
  const _v2_prog_type * type;
  char *save_argv0;
  int i;
  char *go32, *sip=0;
  char rpath[FILENAME_MAX];
  int argc=0;

  int si_la=0, si_off=0, rm_off, argv_off;
  char cmdline[CMDLEN_LIMIT+2], *cmdp = cmdline;
  char *pcmd = cmdline, *pproxy = 0, *proxy_cmdline = 0;
  int retval;
  int lfn = 2;	/* means don't know yet */

  type = _check_v2_prog (program, -1);

  /* Because this function is called only, when program
     exists, I can skip the check for type->valid */

#define v2_0 (type->version.v.major > 1)
#define is_stubbed (type->exec_format == _V2_EXEC_FORMAT_STUBCOFF)
#define is_coff (type->object_format == _V2_OBJECT_FORMAT_COFF)
#define found_si (type->has_stubinfo)

  if (type->exec_format == _V2_EXEC_FORMAT_UNIXSCRIPT)
  {
    return script_exec(program, argv, envp);
  }

  /* Non-DJGPP programs cannot be run by !proxy.  */
  if (!is_coff)
  {
    if (type->exec_format == _V2_EXEC_FORMAT_EXE)
      return direct_exec(program, argv, envp);
    else
      return __dosexec_command_exec (program, argv, envp);
  }

  if (found_si)
    go32 = type->stubinfo->go32;
  else if (v2_0 && !is_stubbed)
    go32 = GO32_V2_STRING;
  else
    go32 = GO32_STRING;

  if (v2_0 && is_stubbed)
  {
    strcpy(rpath, program);
  }
  else
  {
    int e = errno;
    if (!__dosexec_find_on_path(go32, envp, rpath))
    {
      errno = e;
      return direct_exec(program, argv, envp); /* give up and just run it */
    }

    if (found_si)
    {  
      sip = (char *)type->stubinfo;
    }
  }

  /* V2.0 programs invoked by `system' must be run via
     `direct_exec', because otherwise the command-line arguments
     won't be globbed correctly by the child.  Only v2.01 and
     later knows how to get long command lines from `system' AND
     glob them correctly.  But we don't want to check with which
     version was the child compiled, so we need to create both the
     usual DOS command line and the !proxy one (which will be put
     into the environment).  Sigh...  */
  save_argv0 = argv[0];
  argv[0] = unconst(program, char *); /* since that's where we really found it */
  /* Construct the DOS command tail */
  for (argc=0; argv[argc]; argc++);

  if (__dosexec_in_system && v2_0)
  {
    /* If PROGRAM is an un-stubbed COFF, its name must be passed
       in the command tail as well, since we call GO32 to run it.  */
    for (i = (is_stubbed ? 1 : 0); i < argc; i++)
    {
      const char *p = argv[i];
      if (cmdp - cmdline > CMDLEN_LIMIT)
	break;
      *cmdp++ = ' ';
      while (*p)
      {
	if (cmdp - cmdline > CMDLEN_LIMIT)
	  break;
	*cmdp++ = *p++;
      }
    }
    *cmdp = '\0';
  }

  lfn = _USE_LFN;

  /* Can't call any functions that use the transfer buffer beyond
     this point: they will overwrite the data already in __tb.  */
  tbuf_beg = tbuf_ptr = __tb;
  tbuf_len = _go32_info_block.size_of_transfer_buffer;
  tbuf_end = tbuf_ptr + tbuf_len - 1;

  /* If called from `system' and we have a command line shorter
     than the DOS limit, we don't need to use !proxy at all.
     Note that v1.x programs are always run through !proxy,
     to prevent go32.exe from printing its copyright line. */
  if (!__dosexec_in_system || !v2_0 || cmdp - cmdline > CMDLEN_LIMIT)
  {
    if (!check_talloc(found_si ?
		      type->stubinfo->struct_length : 0
		      + (argc+1)*sizeof(short)))
    {
      argv[0] = save_argv0;
      return -1;
    }
    if (found_si)
    {
      si_la = talloc(type->stubinfo->struct_length);
      si_off = si_la - tbuf_beg;
      dosmemput(sip, type->stubinfo->struct_length, si_la);
    }

    rm_off = argv_off = talloc((argc+1) * sizeof(short)) - tbuf_beg;
#if 0
    /* `alloca' could be dangerous with long command lines.  We
       will instead move the offsets one by one with `_farpokew'.  */
    rm_argv = (short *)alloca((argc+1) * sizeof(short));
#endif

    for (i=0; i<argc; i++)
    {
      char *pargv = argv[i];
      int sl = strlen(pargv) + 1;
      unsigned long q;

      if (check_talloc(sl))
      {
	q = talloc(sl);
	dosmemput(pargv, sl, q);
	_farpokew(_dos_ds, tbuf_beg + argv_off, (q - tbuf_beg) & 0xffff);
	argv_off += sizeof(short);
      }
      else	/* not enough space to pass args */
      {
	argv[0] = save_argv0;
	return -1;
      }
    }

    _farpokew (_dos_ds, tbuf_beg + argv_off, 0);
    argv_off += sizeof(short);

    argv[0] = save_argv0;
    /* Environment variables are all malloced.  */
    proxy_cmdline = (char *)malloc (34);
    if (!proxy_cmdline)
      return -1;
    
    sprintf(proxy_cmdline, "%s=%04x %04x %04x %04x %04x",
	     __PROXY, argc,
	    (unsigned)(tbuf_beg >> 4), rm_off & 0xffff,
	    (unsigned)(tbuf_beg >> 4), si_off & 0xffff);
    if (!found_si)
      proxy_cmdline[22] = 0; /* remove stubinfo information */

    if (__dosexec_in_system && v2_0)
      pproxy = proxy_cmdline;
    else
    {
      /* `proxy_cmdline looks like an environment variable " !proxy=value".
         This is used as the REAL command line specification by 2.01
	 and later executables when called by `system'.  But if that's
	 not the case, we need a blank instead of the `='.  */
      proxy_cmdline[__PROXY_LEN] = ' ';
      pcmd = proxy_cmdline;
    }
  }
  else
    argv[0] = save_argv0;

  retval = direct_exec_tail(rpath, pcmd, envp, pproxy, lfn);
  if (proxy_cmdline)
    free(proxy_cmdline);
  return retval;
}

int
__dosexec_command_exec(const char *program, char **argv, char **envp)
{
  const char *comspec=0;
  char *cmdline;
  int cmdlen;
  int i;
  int was_quoted = 0;	/* was the program name quoted? */

  /* Add spare space for possible quote characters.  */
  cmdlen = strlen(program) + 4 + 2;
  for (i=0; argv[i]; i++)
    cmdlen += 2*strlen(argv[i]) + 1;
  cmdline = (char *)alloca(cmdlen);

  /* FIXME: is this LFN-clean?  What special characters can
     the program name have and how should they be quoted?  */
  strcpy(cmdline, "/c ");
  if (strchr(program, ' ') || strchr(program, '\t'))
  {
    was_quoted = 1;
    cmdline[3] = '"';
  }
  for (i = 0; program[i] > ' '; i++)
  {
    /* COMMAND.COM cannot grok program names with forward slashes.  */
    if (program[i] == '/')
      cmdline[i+3+was_quoted] = '\\';
    else
      cmdline[i+3+was_quoted] = program[i];
  }
  for (; program[i]; i++)
    cmdline[i+3+was_quoted] = program[i];
  if (was_quoted)
  {
    cmdline[i+3+was_quoted] = '"';
    i++;
  }
  cmdline[i+3+was_quoted] = 0;
  for (i=1; argv[i]; i++)
  {
    strcat(cmdline, " ");
    /* If called by `spawnXX' or `execXX' functions, must quote
       arguments that have embedded whitespace or characters which
       are special to COMMAND.COM and its ilk.  We don't quote all
       the arguments so the command line won't grow larger than
       the 126-char limit, if it doesn't have to.  */
    if (!__dosexec_in_system && strpbrk(argv[i], " \t<>|'\"%") != 0)
    {
      char *d = cmdline + strlen(cmdline);
      char *s = argv[i];
      /* COMMAND.COM doesn't understand escaped quotes, so we must
	 insert additional quotes around redirection characters if
	 it would seem to COMMAND.COM we're outside of quoted part.
	 This variable keeps track of whether we are in- or outside
	 quotes as far as COMMAND.COM is concerned.  */
      int  outside_quote = 0;

      *d++ = '"';
      while (*s)
      {
	if (*s == '"')
	{
	  outside_quote = !outside_quote;
	  *d++ = '\\';
	}
	else if (outside_quote && (*s == '|' || *s == '<' || *s == '>'))
	{
	  *d++ = '"';
	  *d++ = *s++;
	  *d++ = '"';
	  continue;
	}
	else if (*s == '%')
	  *d++ = '%';
	*d++ = *s++;
      }
      *d++ = '"';
      *d++ = '\0';
    }
    else
      strcat(cmdline, argv[i]);
  }
  for (i=0; envp[i]; i++)
    if (strncmp(envp[i], "COMSPEC=", 8) == 0)
      comspec = envp[i]+8;
  if (!comspec)
    for (i=0; environ[i]; i++)
      if (strncmp(environ[i], "COMSPEC=", 8) == 0)
        comspec = environ[i]+8;
  if (!comspec)
    comspec = "c:\\command.com";

  /* FIXME: 126-char limit below isn't LFN-clean.  */
  if (strlen(cmdline) > CMDLEN_LIMIT + 1)
  {
    cmdline[CMDLEN_LIMIT+1] = '\0';
    errno = E2BIG;
  }

  tbuf_beg = tbuf_ptr = __tb;
  tbuf_len = _go32_info_block.size_of_transfer_buffer;
  tbuf_end = tbuf_ptr + tbuf_len - 1;
  i = direct_exec_tail(comspec, cmdline, envp, 0, 2);
  return i;
}

static int script_exec(const char *program, char **argv, char **envp)
{
  char line[130], interp[FILENAME_MAX], iargs[130];
  FILE *f;
  char **newargs;
  int i, hasargs=0;
  char *base, *p;
  int has_extension = 0, has_drive = 0;
  char pinterp[FILENAME_MAX];
  int (*spawnfunc)(int, const char *, char *const [], char *const []);

  f = fopen(program, "rt");
  if (!f)
  {
    errno = ENOENT;
    return -1;
  }
  fgets(line, sizeof(line), f);
  fclose(f);

  if (strncmp(line, "#!", 2))		/* prevent infinite loop */
    return go32_exec(program, argv, envp);

  /* Paranoia: is this at all a text file?  */
  for (i=0; i < sizeof(line)-1 && line[i] != '\0'; i++)
    if (line[i] < 7 && line[i] >= 0)
      return direct_exec(program, argv, envp);

  iargs[0] = 0;
  interp[0] = 0;
  sscanf(line, "#! %s %[^\n]", interp, iargs);

  /* If no interpreter, invoke the default shell in $COMSPEC.  */
  if (interp[0] == 0)
    return __dosexec_command_exec(program, argv, envp); /* it couldn't be .exe or .com if here */
  if (iargs[0])
    hasargs=1;

  for (i=0; argv[i]; i++);
  newargs = (char **)alloca((i+2+hasargs)*sizeof(char *));
  for (i=0; argv[i]; i++)
    newargs[i+1+hasargs] = unconst(argv[i], char *);
  newargs[i+1+hasargs] = 0;
  /* Some interpreters might have their own ideas about $PATH.
     Therefore, pass them the full pathname of the script.  */
  newargs[0] = newargs[1+hasargs] = unconst(program, char *);
  if (hasargs)
    newargs[1] = iargs;

  /* If INTERP is a Unix-style pathname, like "/bin/sh", we will try
     it with the usual extensions and, if that fails, will further
     search for the basename of the shell along the PATH; this
     allows to run Unix shell scripts without editing their first line.  */
  for (base=p=interp; *p; p++)
  {
    if (*p == '.')
      has_extension = 1;
    if (*p == '/' || *p == '\\' || *p == ':')
    {
      if (*p == ':')
	has_drive = 1;
      has_extension = 0;
      base = p + 1;
    }
  }

  if (has_drive || has_extension)
  {
    strcpy (pinterp, interp);
    spawnfunc = spawnvpe;
  }
  else if (__dosexec_find_on_path(interp, (char **)0, pinterp)
	   || __dosexec_find_on_path(base, envp, pinterp))
    spawnfunc = spawnve;	/* no need to search on PATH: we've found it */
  else
    return -1;

  i = (*spawnfunc)(P_WAIT, pinterp, newargs, envp);
  return i;
}

/* Note: the following list is not supposed to mention *every*
   possible extension of an executable file.  It only mentions
   those extensions that can be *omitted* when you invoke the
   executable from one of the shells used on MSDOS.  */
static struct {
  const char *extension;
  int (*interp)(const char *, char **, char **);
} interpreters[] = {
  { ".com", direct_exec },
  { ".exe", go32_exec },
  { ".bat", __dosexec_command_exec },
  { ".btm", __dosexec_command_exec },
  { ".sh",  script_exec },  /* for compatibility with ms_sh */
  { ".ksh", script_exec },
  { ".pl", script_exec },   /* Perl */
  { ".sed", script_exec },
  { "",     go32_exec },
  { 0,      script_exec },  /* every extension not mentioned above calls it */
  { 0,      0 },
};

/* This is the index into the above array of the interpreter
   which is called when the program filename has no extension.  */
#define INTERP_NO_EXT (sizeof(interpreters)/sizeof(interpreters[0]) - 3)

/*-------------------------------------------------*/

char *
__dosexec_find_on_path(const char *program, char *envp[], char *buf)
{
  char *pp, *rp, *pe;
  const char *ptr;
  int i, hasdot=0, haspath=0;
  int tried_dot = 0;
  int e = errno, blen = strlen(program);

  if (blen > FILENAME_MAX - 1)
  {
    errno = ENAMETOOLONG;
    return 0;
  }
  strncpy(buf, program, blen + 1);
  rp = buf + blen;

  for (ptr=program; *ptr; ptr++)
  {
    if (*ptr == '.')
      hasdot = 1;
    if (*ptr == '/' || *ptr == '\\' || *ptr == ':')
    {
      haspath = 1;
      hasdot = 0;
    }
  }

  /* Under LFN, we must try the extensions even if PROGRAM already has one.  */
  if (!hasdot || _use_lfn(program))
    for (i=0; interpreters[i].extension; i++)
    {
      strcpy(rp, interpreters[i].extension);
      if (access(buf, 0) == 0 && access(buf, D_OK))
      {
	/* If some of the `access' calls failed, `errno' will hold
	   the reason for the failure which is irrelevant to the
	   caller (we *did* find the execuatble).  Restore the value
	   `errno' had when we were called.  */
	errno = e;
	return buf;
      }
    }

  if (access(buf, 0) == 0 && access(buf, D_OK))
  {
    errno = e;
    return buf;
  }

  if (haspath || !envp)
    return 0;
  *rp = 0;

  pp = 0;
  for (i=0; envp[i]; i++)
    if (strncmp(envp[i], "PATH=", 5) == 0)
      pp = envp[i] + 5;
  if (pp == 0)
    return 0;

  while (1)
  {
    if (!tried_dot)
    {
      rp = buf;
      pe = pp;
      tried_dot = 1;
    }
    else
    {
      rp = buf;
      for (pe = pp; *pe && *pe != ';'; pe++)
        *rp++ = *pe;
      pp = pe+1;
      if (rp > buf && rp[-1] != '/' && rp[-1] != '\\' && rp[-1] != ':')
        *rp++ = '/';
    }
    for (ptr = program; *ptr; ptr++)
      *rp++ = *ptr;
    *rp = 0;
    
    if (!hasdot || _use_lfn(buf))
      for (i=0; interpreters[i].extension; i++)
      {
        strcpy(rp, interpreters[i].extension);
        if (access(buf, 0) == 0 && access(buf, D_OK))
	{
	  errno = e;
          return buf;
	}
      }
    if (access(buf, 0) == 0 && access(buf, D_OK))
    {
      errno = e;
      return buf;
    }
    if (*pe == 0)
      return 0;
  }
}

int __spawnve(int mode, const char *path, char *const argv[], char *const envp[])
{
  /* This is the one that does the work! */
  union { char *const *x; char **p; } u;
  int i = -1;
  char **argvp;
  char **envpp;
  char rpath[FILENAME_MAX], *rp, *rd=0;
  int e = errno;
  int is_dir = 0;
  int found = 0;

  if (path == 0 || argv[0] == 0)
  {
    errno = EINVAL;
    return -1;
  }
  if (strlen(path) > FILENAME_MAX - 1)
  {
    errno = ENAMETOOLONG;
    return -1;
  }

  u.x = argv; argvp = u.p;
  u.x = envp; envpp = u.p;

  fflush(stdout); /* just in case */
  for (rp=rpath; *path; *rp++ = *path++)
  {
    if (*path == '.')
      rd = rp;
    if (*path == '\\' || *path == '/')
      rd = 0;
  }
  *rp = 0;

  /* If LFN is supported on the volume where rpath resides, we
     might have something like foo.bar.exe or even foo.exe.com.
     If so, look for RPATH.ext before even trying RPATH itself. */
  if (_use_lfn(rpath) || !rd)
  {
    for (i=0; interpreters[i].extension; i++)
    {
      strcpy(rp, interpreters[i].extension);
      if (access(rpath, F_OK) == 0 && !(is_dir = (access(rpath, D_OK) == 0)))
      {
	found = 1;
	break;
      }
    }
  }

  if (!found)
  {
    const char *rpath_ext;

    if (rd)
    {
      i = 0;
      rpath_ext = rd;
    }
    else
    {
      i = INTERP_NO_EXT;
      rpath_ext = "";
    }
    for ( ; interpreters[i].extension; i++)
      if (stricmp(rpath_ext, interpreters[i].extension) == 0
	  && access(rpath, F_OK) == 0
	  && !(is_dir = (access(rpath, D_OK) == 0)))
      {
	found = 1;
        break;
      }
  }
  if (!found)
  {
    errno = is_dir ? EISDIR : ENOENT;
    return -1;
  }
  errno = e;
  i = interpreters[i].interp(rpath, argvp, envpp);
  if (mode == P_OVERLAY)
    exit(i);
  return i;
}
