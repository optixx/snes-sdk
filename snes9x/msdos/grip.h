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

#ifndef GRIP_H
#define GRIP_H

#ifdef __cplusplus
extern "C"
  {
#endif

/* 2. Type Definitions */
typedef unsigned char         GRIP_SLOT;
typedef unsigned char         GRIP_CLASS;
typedef unsigned char         GRIP_INDEX;
typedef unsigned short        GRIP_VALUE;
typedef unsigned long         GRIP_BITFIELD;
typedef unsigned short        GRIP_bool8;
typedef char *                GRIP_STRING;
typedef void *                GRIP_BUF;
typedef unsigned char *       GRIP_BUF_C;
typedef unsigned short *      GRIP_BUF_S;
typedef unsigned long *       GRIP_BUF_L;

/* Standard Classes */
#define GRIP_CLASS_BUTTON                 1
#define GRIP_CLASS_AXIS                   2
#define GRIP_CLASS_POV_HAT                3
#define GRIP_CLASS_VELOCITY               4

/* Refresh Flags */
#define GRIP_REFRESH_COMPLETE             0           /* Default */
#define GRIP_REFRESH_PARTIAL              2
#define GRIP_REFRESH_TRANSMIT             0           /* Default */
#define GRIP_REFRESH_NOTRANSMIT           4

/* 3.1 System API Calls */
GRIP_bool8 GrInitialize(void);
void GrShutdown(void);
GRIP_BITFIELD GrRefresh(GRIP_BITFIELD flags);

/* 3.2 Configuration API Calls */
GRIP_BITFIELD GrGetSlotMap(void);
GRIP_BITFIELD GrGetClassMap(GRIP_SLOT s);
GRIP_BITFIELD GrGetOEMClassMap(GRIP_SLOT s);
GRIP_INDEX GrGetMaxIndex(GRIP_SLOT s, GRIP_CLASS c);
GRIP_VALUE GrGetMaxValue(GRIP_SLOT s, GRIP_CLASS c);

/* 3.3 Data API Calls */
GRIP_VALUE GrGetValue(GRIP_SLOT s, GRIP_CLASS c, GRIP_INDEX i);
GRIP_BITFIELD GrGetPackedValues(GRIP_SLOT s, GRIP_CLASS c, GRIP_INDEX start, GRIP_INDEX end);
void GrSetValue(GRIP_SLOT s, GRIP_CLASS c, GRIP_INDEX i, GRIP_VALUE v);

/* 3.4 OEM Information API Calls */
void GrGetVendorName(GRIP_SLOT s, GRIP_STRING name);
GRIP_VALUE GrGetProductName(GRIP_SLOT s, GRIP_STRING name);
void GrGetControlName(GRIP_SLOT s, GRIP_CLASS c, GRIP_INDEX i, GRIP_STRING name);
GRIP_BITFIELD GrGetCaps(GRIP_SLOT s, GRIP_CLASS c, GRIP_INDEX i);

/* 3.5 Library Management Calls */
GRIP_bool8 GrLink(GRIP_BUF image, GRIP_VALUE size);
GRIP_bool8 GrUnlink(void);

GRIP_bool8 Gr__Link(void);
void Gr__Unlink(void);

/* Diagnostic Information Calls */
GRIP_VALUE GrGetSWVer(void);
GRIP_VALUE GrGetHWVer(void);
GRIP_VALUE GrGetDiagCnt(void);
unsigned long GrGetDiagReg(GRIP_INDEX reg);

/* Standard String Constants for Loadable Library */
extern char GrLibName[];
extern char GrLibEnv[];
extern char GrLibDir[];

/* API Call Thunk */
typedef unsigned char GRIP_THUNK[14];
extern GRIP_THUNK GRIP_Thunk;

#ifdef __cplusplus
  }
#endif

#endif /* GRIP_H */

