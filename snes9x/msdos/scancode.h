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

#ifndef _SCANCODES_H_
#define _SCANCODE_H_

#define SCANCODE_ESCAPE			1

#define SCANCODE_1			2
#define SCANCODE_2			3
#define SCANCODE_3			4
#define SCANCODE_4			5
#define SCANCODE_5			6
#define SCANCODE_6			7
#define SCANCODE_7			8
#define SCANCODE_8			9
#define SCANCODE_9			10
#define SCANCODE_0			11

#define SCANCODE_MINUS			12
#define SCANCODE_EQUAL			13

#define SCANCODE_BACKSPACE		14
#define SCANCODE_TAB			15

#define SCANCODE_Q			16
#define SCANCODE_W			17
#define SCANCODE_E			18
#define SCANCODE_R			19
#define SCANCODE_T			20
#define SCANCODE_Y			21
#define SCANCODE_U			22
#define SCANCODE_I			23
#define SCANCODE_O			24
#define SCANCODE_P			25
#define SCANCODE_BRACKET_LEFT		26
#define SCANCODE_BRACKET_RIGHT		27

#define SCANCODE_ENTER			28

#define SCANCODE_LEFTCONTROL		29

#define SCANCODE_A			30
#define SCANCODE_S			31
#define SCANCODE_D			32
#define SCANCODE_F			33
#define SCANCODE_G			34
#define SCANCODE_H			35
#define SCANCODE_J			36
#define SCANCODE_K			37
#define SCANCODE_L			38
#define SCANCODE_SEMICOLON		39
#define SCANCODE_APOSTROPHE		40
#define SCANCODE_GRAVE			41

#define SCANCODE_LEFTSHIFT		42
#define SCANCODE_BACKSLASH		43

#define SCANCODE_Z			44
#define SCANCODE_X			45
#define SCANCODE_C			46
#define SCANCODE_V			47
#define SCANCODE_B			48
#define SCANCODE_N			49
#define SCANCODE_M			50
#define SCANCODE_COMMA			51
#define SCANCODE_PERIOD			52
#define SCANCODE_SLASH			53

#define SCANCODE_RIGHTSHIFT		54
#define SCANCODE_KEYPADMULTIPLY		55

#define SCANCODE_LEFTALT		56
#define SCANCODE_SPACE			57
#define SCANCODE_CAPSLOCK		58

#define SCANCODE_F1			59
#define SCANCODE_F2			60
#define SCANCODE_F3			61
#define SCANCODE_F4			62
#define SCANCODE_F5			63
#define SCANCODE_F6			64
#define SCANCODE_F7			65
#define SCANCODE_F8			66
#define SCANCODE_F9			67
#define SCANCODE_F10			68

#define SCANCODE_NUMLOCK		69
#define SCANCODE_SCROLLLOCK		70

#define SCANCODE_KEYPAD7		71
#define SCANCODE_CURSORUPLEFT		71
#define SCANCODE_KEYPAD8		72
#define SCANCODE_CURSORUP		72
#define SCANCODE_KEYPAD9		73
#define SCANCODE_CURSORUPRIGHT		73
#define SCANCODE_KEYPADMINUS		74
#define SCANCODE_KEYPAD4		75
#define SCANCODE_CURSORLEFT		75
#define SCANCODE_KEYPAD5		76
#define SCANCODE_KEYPAD6		77
#define SCANCODE_CURSORRIGHT		77
#define SCANCODE_KEYPADPLUS		78
#define SCANCODE_KEYPAD1		79
#define SCANCODE_CURSORDOWNLEFT		79
#define SCANCODE_KEYPAD2		80
#define SCANCODE_CURSORDOWN		80
#define SCANCODE_KEYPAD3		81
#define SCANCODE_CURSORDOWNRIGHT	81
#define SCANCODE_KEYPAD0		82
#define SCANCODE_KEYPADPERIOD		83

#define SCANCODE_LESS			86

#define SCANCODE_F11			87
#define SCANCODE_F12			88

#define SCANCODE_KEYPADENTER		96
#define SCANCODE_RIGHTCONTROL		97
#define SCANCODE_CONTROL		97
#define SCANCODE_KEYPADDIVIDE		98
#define SCANCODE_PRINTSCREEN		99
#define SCANCODE_RIGHTALT		100
#define SCANCODE_BREAK			101	/* Beware: is 119     */
#define SCANCODE_BREAK_ALTERNATIVE	119	/* on some keyboards! */

#define SCANCODE_HOME			102
#define SCANCODE_CURSORBLOCKUP		103	/* Cursor key block */
#define SCANCODE_PAGEUP			104
#define SCANCODE_CURSORBLOCKLEFT	105	/* Cursor key block */
#define SCANCODE_CURSORBLOCKRIGHT	106	/* Cursor key block */
#define SCANCODE_END			107
#define SCANCODE_CURSORBLOCKDOWN	108	/* Cursor key block */
#define SCANCODE_PAGEDOWN		109
#define SCANCODE_INSERT			110
#define SCANCODE_REMOVE			111

#endif

