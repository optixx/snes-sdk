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

.macro GetCarry
	mov FLAGS,r1
	shlr r1
.endm

.macro GetNotCarry
	mov #Carry,r1
	tst r1,FLAGS
.endm

.macro SetZNjmp R
	jmp	@r13
	exts.b	\R,r7
.endm

.macro Set16ZNjmp R
	jmp	@r13
	exts.w	\R,r7
.endm

.macro SetZNlink R
	lds	r13,pr
	jmp	@r3
	exts.b	\R,r7
.endm

.macro Set16ZNlink R
	lds	r13,pr
	jmp	@r3
	exts.w	\R,r7
.endm

.macro Xlink
	jmp	@r3
	lds	r13,pr
.endm
	
#if 0
.macro SetZNC
	setc _Carry
	movb %al, _Negative
	movb %al, _Zero
.endm

.macro Set16ZNC
	setc _Carry
	setnz _Zero
	movb %ah, _Negative
.endm

.macro SetZNCV
	setc _Carry
	seto _Overflow
	movb %al, _Negative
	movb %al, _Zero
.endm

.macro Set16ZNCV
	setc _Carry
	seto _Overflow
	setnz _Zero
	movb %ah, _Negative
.endm

.macro SetZNV
	seto _Overflow
	movb %al, _Negative
	movb %al, _Zero
.endm

.macro Set16ZNV
	seto _Overflow
	setnz _Zero
	movb %ah, _Negative
.endm
#endif

/************* IMMEDIATE8 ****************/
.Macro Immediate8 K
	mov.b @PC+,r0
	extu.b r0,r0
.endm

/************* IMMEDIATE16 ****************/
.macro Immediate16 K
	mov.b @PC+,r0
	shll8 r0
	mov.b @PC+,r2
	extu.b r2,r2
	or r2,r0
	swap.b r0,r0
	extu.w r0,r0
.endm

/************* Relative ****************/
.macro Relative K
	mov.b @PC+,r4
!	mov.l SA1,r1
!	exts.b r4,r4
	mov.l SA1PCBase,r2
	add PC,r4
	sub r2,r4
.endm

/************* RelativeLong ****************/
.macro RelativeLong K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.b @PC+,r0
!	mov.l SA1,r1
	shll8 r0
	mov.l SA1PCBase,r2
	or r0,r4
	add PC,r4
	sub r2,r4
.endm

	
/************* AbsoluteIndexedIndirect8 ****************/
.macro AbsoluteIndexedIndirect8 K
	mov.b @PC+,r2
	extu.b r2,r2
	mov.b @PC+,r4
	shll8 r4
	mov.w XX,r0
	add r2,r4
	add r0,r4
	mov.l S9xSA1GetWord,r3
	extu.w r4,r4
	mov.l SA1ShiftedPB,r2
	jsr @r3
	or r2,r4
	extu.w r0,r4
.endm

/************* AbsoluteIndirectLong8 ****************/
.macro AbsoluteIndirectLong8 K
	mov.b @PC+,r2
	extu.b r2,r2
	mov.b @PC+,r4
	shll8 r4
	mov.l S9xSA1GetWord,r3
	or r2,r4
	jsr @r3
	extu.w r4,r4
	mov.l r0,@-r15
	mov.l S9xSA1GetByte,r3
	add #2,r4
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r4
	shll16 r0
	or r0,r4
.endm

.macro AbsoluteIndirect8 K
	mov.b @PC+,r2
	extu.b r2,r2
	mov.b @PC+,r4
	shll8 r4
	mov.l S9xSA1GetWord,r3
	or r2,r4
	jsr @r3
	extu.w r4,r4
	extu.w r0,r4
.endm

.macro Absolute8 K
	mov.b @PC+,r2
	extu.b r2,r2
	mov.b @PC+,r3
	shll8 r3
	mov.l SA1ShiftedDB,r4
	or r2,r3
	extu.w r3,r3
	add r3,r4
.endm

.macro AbsoluteLong8 K
	mov.b @PC+,r2
	extu.b r2,r2
	mov.b @PC+,r4
	extu.b r4,r4
	mov.b @PC+,r0
	extu.b r0,r0
	shll16 r0
	shll8 r4
	or r0,r2
	or r2,r4
.endm

.macro Direct8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w DD,r0
	add r0,r4
	extu.w r4,r4
.endm

.macro DirectIndirectIndexed8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w DD,r0
	add r0,r4
	mov.l S9xSA1GetWord,r3
	jsr @r3
	extu.w r4,r4
	mov.l SA1,r3
	mov.l @(16,r3),r4
	add r0,r4
	mov.w YY,r0
	extu.w r0,r0
	add r0,r4
.endm

.macro DirectIndirectIndexedLong8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w DD,r0
	add r0,r4
	mov.l S9xSA1GetWord,r3
	jsr @r3
	extu.w r4,r4
	mov.l r0,@-r15
	mov.l S9xSA1GetByte,r3
	jsr @r3
	add #2,r4
	mov.l @r15+,r4
	shll16 r0
	or r0,r4
	mov.w YY,r0
	extu.w r0,r0
	add r0,r4
	.endm
	
.macro DirectIndexedIndirect8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w DD,r0
	add r0,r4
	mov.w XX,r0
	add r0,r4
	mov.l S9xSA1GetWord,r3
	jsr @r3
	extu.w r4,r4
	mov.l SA1,r3
	mov.l @(16,r3),r4
	add r0,r4
.endm

.macro DirectIndexedX8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w DD,r0
	add r0,r4
	mov.w XX,r0
	add r0,r4
	extu.w r4,r4
.endm

.macro DirectIndexedY8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w DD,r0
	add r0,r4
	mov.w YY,r0
	add r0,r4
	extu.w r4,r4
.endm

.macro AbsoluteIndexedX8 K
	mov.b @PC+,r2
	extu.b r2,r2
	mov.b @PC+,r3
	shll8 r3
	mov.l SA1ShiftedDB,r4
	or r2,r3
	extu.w r3,r3
	mov.w XX,r0
	add r3,r4
	extu.w r0,r0
	add r0,r4
.endm

.macro AbsoluteIndexedY8 K
	mov.b @PC+,r2
	extu.b r2,r2
	mov.b @PC+,r3
	shll8 r3
	mov.l SA1ShiftedDB,r4
	or r2,r3
	extu.w r3,r3
	mov.w YY,r0
	add r3,r4
	extu.w r0,r0
	add r0,r4
.endm

.macro AbsoluteLongIndexedX8 K
	mov.b @PC+,r2
	extu.b r2,r2
	mov.b @PC+,r3
	extu.b r3,r3
	mov.b @PC+,r4
	extu.b r4,r4
	shll8 r3
	or r2,r3
	mov.w XX,r0
	shll16 r4
	extu.w r0,r0
	or r3,r4
	add r0,r4
.endm

.macro DirectIndirect8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w DD,r0
	add r0,r4
	mov.l S9xSA1GetWord,r3
	jsr @r3
	extu.w r4,r4
	mov.l SA1,r3
	mov.l @(16,r3),r4
	add r0,r4
.endm

.macro DirectIndirectLong8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w DD,r0
	add r0,r4
	mov.l S9xSA1GetWord,r3
	jsr @r3
	extu.w r4,r4
	mov.l r0,@-r15
	mov.l S9xSA1GetByte,r3
	jsr @r3
	add #2,r4
	mov.l @r15+,r4
	shll16 r0
	or r0,r4
.endm

.macro StackRelative8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w SS,r0
	add r0,r4
	extu.w r4,r4
.endm

.macro StackRelativeIndirectIndexed8 K
	mov.b @PC+,r4
	extu.b r4,r4
	mov.w SS,r0
	mov.l S9xSA1GetWord,r3
	add r0,r4
	jsr @r3
	extu.w r4,r4
	mov.l SA1,r3
	mov.l @(16,r3),r4
	add r0,r4
	mov.w YY,r0
	extu.w r0,r0
	add r0,r4
.endm

.macro PushBytejmp K
	mov r0,r1
	mov.w SS,r0
	extu.w r0,r4
	mov.l S9xSA1SetByte,r3
	add #-1,r0
	mov.w r0,SS
	mov r1,r0
	Xlink
.endm

.macro PushWordjmp K
	mov r0,r1
	mov.w SS,r0
	add #-1,r0
	extu.w r0,r4
	mov.l S9xSA1SetWord,r3
	add #-1,r0
	mov.w r0,SS
	mov r1,r0
	Xlink
.endm

.macro PushByte K
	mov r0,r1
	mov.w SS,r0
	extu.w r0,r4
	mov.l S9xSA1SetByte,r3
	add #-1,r0
	mov.w r0,SS
	jsr @r3
	mov r1,r0
.endm

.macro PushWord K
	mov r0,r1
	mov.w SS,r0
	add #-1,r0
	extu.w r0,r4
	mov.l S9xSA1SetWord,r3
	add #-1,r0
	mov.w r0,SS
	jsr @r3
	mov r1,r0
.endm

.macro PullByte K
	mov.w SS,r0
	add #1,r0
	mov.l S9xSA1GetByte,r3
	extu.w r0,r4
	jsr @r3
	mov.w r0,SS
.endm

.macro PullWord K
	mov.w SS,r0
	add #1,r0
	mov.l S9xSA1GetWord,r3
	extu.w r0,r4
	add #1,r0
	jsr @r3
	mov.w r0,SS
.endm

