/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
/*
 * This is file GETDOS_V.C
 *
 * Copyright (c) 1994,95 Eli Zaretskii <eliz@is.elta.co.il>
 *
 * This software may be used freely so long as this copyright notice is
 * left intact.  There is no warranty on this software.
 *
 */

#include <libc/bss.h>
#include <dpmi.h>
#include <dos.h>

/* Return OS version number, either true or ``simulated'' one.
   Global variables _OSMAJOR and _OSMINOR are ALWAYS set to whatever
   INT 21h/AX=30h returns.  Global _OS_FLAVOR is set to a string
   which identifies the OEM of DOS.
 */

/* From Ralph Brown's interrupt list, v.45.
   Note: some OEMs, like Novell NDOS 7, Compaq DOS 3.31 and possibly
         others return 0 OEM code, like IBM PC-DOS.  There is nothing
         I can do to overcome this.
*/
static struct _oem_info {
  unsigned char   oem_num;
  const    char * oem_name;
} dos_flavors[] = {
    {0,    "IBM PC-DOS"},
    {1,    "Compaq DOS"},
    {2,    "Microsoft DOS"},
    {4,    "AT&T DOS"},
    {5,    "Zenith DOS"},
    {6,    "Hewlett-Packard DOS"},
    {7,    "Groupe Bull DOS"},
    {0xd,  "Packard-Bell DOS"},
    {0x16, "DEC DOS"},
    {0x23, "Olivetti DOS"},
    {0x28, "Texas Instruments DOS"},
    {0x29, "Toshiba DOS"},
    {0x33, "Novell Win3 device ID"},
    {0x34, "MS Multimedia Win3 device ID"},
    {0x35, "MS Multimedia Win3 device ID"},
    {0x4d, "Hewlett-Packard DOS"},
    {0x5e, "RxDOS"},
    {0x66, "PTS-DOS"},
    {0x99, "GenSoft Embedded DOS"},
    {0xee, "DR-DOS"},
    {0xef, "Novell DOS"},
    {0xff, "Microsoft MS-DOS"},
    {0,    (char *)0}
};

static int getdosv_count = -1;

unsigned short
_get_dos_version(int true_version)
{
  unsigned char     oem_number;
  struct _oem_info *p = dos_flavors;
  __dpmi_regs       regs;

  /* Invalidate previous info, if we were restarted (emacs).  */
  if (getdosv_count != __bss_count)
    {
      getdosv_count = __bss_count;
      _osmajor = 0;
    }

  /* If already have the info, return what we know. */
  if (_osmajor && !true_version)
    return (_osmajor << 8) + _osminor;

  regs.x.ax = 0x3000;
  __dpmi_int(0x21, &regs);
  _osminor = regs.h.ah;
  _osmajor = regs.h.al ? regs.h.al : 1;     /* DOS 1.x returns 0 */

  oem_number = regs.h.bh;
  for ( ; p->oem_name && p->oem_num != oem_number; ++p);

  if (p->oem_name)
    _os_flavor = p->oem_name;
  else
    _os_flavor = "??Unknown??";

  if (true_version)
    {
      regs.x.ax = 0x3306;
      regs.x.bx = 0;    /* in case 213306 isn't supported, BX stays 0 */
      __dpmi_int(0x21, &regs);

      /* Sanity checks suggested by Ralph Brown's Interrupt list. */
      if ( !(regs.x.flags & 1) &&
           regs.h.al != 0xff &&
           regs.h.bh < 100 &&
           regs.h.bl >= 5)
        return (regs.h.bl << 8) + regs.h.bh;
    }

  return (_osmajor << 8) + _osminor;
}
