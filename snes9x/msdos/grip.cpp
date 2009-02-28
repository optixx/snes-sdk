/**********************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2002  Gary Henderson (gary.henderson@ntlworld.com) and
                             Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2004  Matthew Kendora

  (c) Copyright 2002 - 2005  Peter Bortas (peter@bortas.org)

  (c) Copyright 2004 - 2005  Joel Yliluoma (http://iki.fi/bisqwit/)

  (c) Copyright 2001 - 2006  John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2006  Brad Jorsch (anomie@users.sourceforge.net),
                             funkyass (funkyass@spam.shaw.ca),
                             Kris Bleakley (codeviolation@hotmail.com),
                             Nach (n-a-c-h@users.sourceforge.net), and
                             zones (kasumitokoduck@yahoo.com)

  BS-X C emulator code
  (c) Copyright 2005 - 2006  Dreamer Nom,
                             zones

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003  _Demo_ (_demo_@zsnes.com),
                             Nach,
                             zsKnight (zsknight@zsnes.com)

  C4 C++ code
  (c) Copyright 2003 - 2006  Brad Jorsch,
                             Nach

  DSP-1 emulator code
  (c) Copyright 1998 - 2006  _Demo_,
                             Andreas Naive (andreasnaive@gmail.com)
                             Gary Henderson,
                             Ivar (ivar@snes9x.com),
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora,
                             Nach,
                             neviksti (neviksti@hotmail.com)

  DSP-2 emulator code
  (c) Copyright 2003         John Weidman,
                             Kris Bleakley,
                             Lord Nightmare (lord_nightmare@users.sourceforge.net),
                             Matthew Kendora,
                             neviksti


  DSP-3 emulator code
  (c) Copyright 2003 - 2006  John Weidman,
                             Kris Bleakley,
                             Lancer,
                             z80 gaiden

  DSP-4 emulator code
  (c) Copyright 2004 - 2006  Dreamer Nom,
                             John Weidman,
                             Kris Bleakley,
                             Nach,
                             z80 gaiden

  OBC1 emulator code
  (c) Copyright 2001 - 2004  zsKnight,
                             pagefault (pagefault@zsnes.com),
                             Kris Bleakley,
                             Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002         Matthew Kendora with research by
                             zsKnight,
                             John Weidman,
                             Dark Force

  S-DD1 C emulator code
  (c) Copyright 2003         Brad Jorsch with research by
                             Andreas Naive,
                             John Weidman

  S-RTC C emulator code
  (c) Copyright 2001-2006    byuu,
                             John Weidman

  ST010 C++ emulator code
  (c) Copyright 2003         Feather,
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003  _Demo_,
                             pagefault,
                             zsKnight,

  Super FX C emulator code
  (c) Copyright 1997 - 1999  Ivar,
                             Gary Henderson,
                             John Weidman

  Sound DSP emulator code is derived from SNEeSe and OpenSPC:
  (c) Copyright 1998 - 2003  Brad Martin
  (c) Copyright 1998 - 2006  Charles Bilyue'

  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004  Marcus Comstedt (marcus@mc.pp.se)

  2xSaI filter
  (c) Copyright 1999 - 2001  Derek Liauw Kie Fa

  HQ2x filter
  (c) Copyright 2003         Maxim Stepin (maxim@hiend3d.com)

  Specific ports contains the works of other authors. See headers in
  individual files.

  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and/or distribute Snes9x in both binary
  and source form, for non-commercial purposes, is hereby granted without 
  fee, providing that this license information and copyright notice appear 
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for Snes9x or software derived from
  Snes9x, including Snes9x or derivatives in commercial game bundles, and/or
  using Snes9x as a promotion for your commercial product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
**********************************************************************************/

/************************************************************************\
**                                                                      **
**                GrIP Prototype API Interface Library                  **
**                                for                                   **
**                               DJGPP                                  **
**                                                                      **
**                           Revision 1.00                              **
**                                                                      **
**  COPYRIGHT:                                                          **
**                                                                      **
**    (C) Copyright Advanced Gravis Computer Technology Ltd 1995.       **
**        All Rights Reserved.                                          **
**                                                                      **
**  DISCLAIMER OF WARRANTIES:                                           **
**                                                                      **
**    The following [enclosed] code is provided to you "AS IS",         **
**    without warranty of any kind.  You have a royalty-free right to   **
**    use, modify, reproduce and distribute the following code (and/or  **
**    any modified version) provided that you agree that Advanced       **
**    Gravis has no warranty obligations and shall not be liable for    **
**    any damages arising out of your use of this code, even if they    **
**    have been advised of the possibility of such damages.  This       **
**    Copyright statement and Disclaimer of Warranties may not be       **
**    removed.                                                          **
**                                                                      **
**  HISTORY:                                                            **
**                                                                      **
**    0.102   Jul 12 95   David Bollo     Initial public release on     **
**                                          GrIP hardware               **
**    0.200   Aug 10 95   David Bollo     Added Gravis Loadable Library **
**                                          support                     **
**    0.201   Aug 11 95   David Bollo     Removed Borland C++ support   **
**                                          for maintenance reasons     **
**    1.00    Nov  1 95   David Bollo     First official release as     **
**                                          part of GrIP SDK            **
**                                                                      **
\************************************************************************/
#include <stdlib.h>
#include <go32.h>
#include <dpmi.h>
#include <sys/movedata.h>
#include <sys/farptr.h>
#include "grip.h"

/* Standard String Constants for Loadable Library */
char GrLibName[] = "GRIP.GLL";
char GrLibEnv[] = "GRIP";
char GrLibDir[] = "C:\\GRIP\\";

/* Global Thunking Data */
GRIP_THUNK GRIP_Thunk;
static int GRIP_Thunked = 0;
static unsigned int GRIP_CS;
static unsigned int GRIP_DS;
#if defined(GRIP_DEBUG)
static unsigned short GRIP_ES;
#endif

#ifdef __cplusplus
extern "C"
  {
#endif

#define DPMI_AllocDOS(a, b) (__dpmi_allocate_dos_memory(a, (int *) b) << 4)
#define DPMI_FreeDOS(a) __dpmi_free_dos_memory(a)
#define DPMI_AllocSel() __dpmi_allocate_ldt_descriptors(1)
#define DPMI_FreeSel(a) __dpmi_free_ldt_descriptor(a)

unsigned char DPMI_SetCodeAR(unsigned short sel);
unsigned char DPMI_SetDataAR(unsigned short sel);

unsigned char DPMI_SetBounds(unsigned short sel, unsigned long base, unsigned short lm) {
  if (__dpmi_set_segment_base_address(sel, base) == -1)  return -1;
  if (__dpmi_set_segment_limit(sel, lm - 1) == -1) return -1;
  return 0;
}

GRIP_bool8 GrLink(GRIP_BUF image, GRIP_VALUE size)
  {
  unsigned short img_off;     /* Offset of binary image     */
  unsigned short img_size;    /* Size of binary image       */
  unsigned char img_check;    /* Checksum of binary image   */
  unsigned long memory;       /* Image memory               */
  GRIP_BUF_C header;          /* Image header               */
  GRIP_BUF_C core;            /* Image core                 */
  int i;

  /* Check for duplicate GrLink calls */
  if (GRIP_Thunked) return 0;

  /* Sanity check the inputs */
  if (image == 0 || size < 16) return 0;

  /* Check the image header for the proper signature bytes */
  header = (GRIP_BUF_C) image;
  if (header[0] != 'D' || header[1] != 'B') return 0;

  /* Extract the image offsets from the header */
  img_off = ( (header[2] & 0x0F) << 12 ) |
            ( (header[3] & 0x0F) <<  8 ) |
            ( (header[4] & 0x0F) <<  4 ) |
            ( (header[5] & 0x0F) );
  img_size = ( (header[6] & 0x0F) << 12 ) |
             ( (header[7] & 0x0F) <<  8 ) |
             ( (header[8] & 0x0F) <<  4 ) |
             ( (header[9] & 0x0F) );
  img_check = ( (header[10] & 0x0F) <<  4 ) |
              ( (header[11] & 0x0F) );

  /* Compute the start of image core */
  if ( size < ((img_off + img_size) << 4) ) return 0;
  core = header + (img_off << 4);

  /* Verify the checksum */
  for (i = 0; i < (img_size << 4); i++) img_check ^= core[i];
  if (img_check != 0) return 0;

  /* Allocate memory for the core */
  memory = (unsigned long) DPMI_AllocDOS(img_size, &GRIP_DS);
  if (!memory) return 0;

  /* Copy the image */
  _dosmemputl(core, img_size << 2, memory);

  /* Allocate a code selector for the core */
  GRIP_CS = DPMI_AllocSel();
  if (GRIP_CS == 0) {
     DPMI_FreeDOS(GRIP_DS);
     return 0;
  }
  if (DPMI_SetBounds(GRIP_CS, memory, img_size << 4) != 0) {
     DPMI_FreeSel(GRIP_CS);
     DPMI_FreeDOS(GRIP_DS);
     return 0;
  }
  if (DPMI_SetCodeAR(GRIP_CS) != 0) {
     DPMI_FreeSel(GRIP_CS);
     DPMI_FreeDOS(GRIP_DS);
     return 0;
  }

  /* Prepare the thunking layer */
  for (i = 0; i < sizeof(GRIP_THUNK); i++)
    GRIP_Thunk[i] = ((GRIP_BUF_C)core)[i];

    *(GRIP_BUF_S)(GRIP_Thunk+0x02) = 0;
    *(GRIP_BUF_S)(GRIP_Thunk+0x06) = GRIP_CS;
    *(GRIP_BUF_S)(GRIP_Thunk+0x0C) = GRIP_CS;

  /* Allocate a debugging selector if GRIP_DEBUG is defined */
  #if defined(GRIP_DEBUG)
      /* I don't know what this does... at least it compiles okay */
      GRIP_ES = DPMI_AllocSel();
      if (GRIP_ES == 0) {
         DPMI_FreeSel(GRIP_CS);
         DPMI_FreeDOS(GRIP_DS);
         return 0;
      }
      if (DPMI_SetBounds(GRIP_ES, 0xB0000, 80*25*2) != 0)
        {
        DPMI_FreeSel(GRIP_ES);
        DPMI_FreeSel(GRIP_CS);
        DPMI_FreeDOS(GRIP_DS);
        return 0;
        }
      if (DPMI_SetDataAR(GRIP_ES) != 0)
        {
        DPMI_FreeSel(GRIP_ES);
        DPMI_FreeSel(GRIP_CS);
        DPMI_FreeDOS(GRIP_DS);
        return 0;
        }
      _farpokew(_dos_ds, memory + 0x12, 0xB000);
      _farpokew(_dos_ds, memory + 0x16, GRIP_ES);
  #else
    _farpokew(_dos_ds, memory + 0x12, 0);
    _farpokew(_dos_ds, memory + 0x16, 0);
  #endif

  // Save the data selector
  _farpokew(_dos_ds, memory + 0x10, 0);
  _farpokew(_dos_ds, memory + 0x14, GRIP_DS);

  /* Complete the link */
  if (Gr__Link() == 0)
    {
      #if defined(GRIP_DEBUG)
        DPMI_FreeSel(GRIP_ES);
      #endif
      DPMI_FreeSel(GRIP_CS);
      DPMI_FreeDOS(GRIP_DS);
    return 0;
    }

  GRIP_Thunked = 1;

  return 1;
  }

GRIP_bool8 GrUnlink(void)
  {
  // Check that the library has been loaded
  if (!GRIP_Thunked)
    return 0;

  // Unlink the library
  Gr__Unlink();

  // Free the library memory
    #if defined(GRIP_DEBUG)
      if (DPMI_FreeSel(GRIP_ES) != 0)
        return 0;
    #endif
    if (DPMI_FreeSel(GRIP_CS) != 0)
      return 0;
    if (DPMI_FreeDOS(GRIP_DS) != 0)
      return 0;

  GRIP_Thunked = 0;

  return 1;
  }

#ifdef __cplusplus
  }
#endif

