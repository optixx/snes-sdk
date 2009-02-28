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
	movb SA1_Carry, %dl
	shrb %dl
.endm

.macro GetNotCarry
	cmpb $1, SA1_Carry
.endm

.macro SetZN
	movb %al, SA1_Zero
	movb %al, SA1_Negative
.endm

.macro Set16ZN
	movb %ah, SA1_Negative
	setnz SA1_Zero
.endm

.macro SetZNC
	setc SA1_Carry
	movb %al, SA1_Negative
	movb %al, SA1_Zero
.endm

.macro Set16ZNC
	setc SA1_Carry
	setnz SA1_Zero
	movb %ah, SA1_Negative
.endm

.macro SetZNCV
	setc SA1_Carry
	seto SA1_Overflow
	movb %al, SA1_Negative
	movb %al, SA1_Zero
.endm

.macro Set16ZNCV
	setc SA1_Carry
	seto SA1_Overflow
	setnz SA1_Zero
	movb %ah, SA1_Negative
.endm

.macro SetZNV
	seto SA1_Overflow
	movb %al, SA1_Negative
	movb %al, SA1_Zero
.endm

.macro Set16ZNV
	seto SA1_Overflow
	setnz SA1_Zero
	movb %ah, SA1_Negative
.endm


/************* IMMEDIATE8 ****************/
.Macro Immediate8 K
	movb (PC), %al
	incl PC
.endm

/************* IMMEDIATE16 ****************/
.macro Immediate16 K
	movw (PC), %ax
	addl $2, PC
.endm

/************* Relative ****************/
.macro Relative K
	movl PC, %edx
	movsbl (%edx), %eax
	incl %edx
	movl %edx, PC
	subl SA1PCBase, %edx
	addl %eax, %edx
	andl $0xffff, %edx
.endm

/************* RelativeLong ****************/
.macro RelativeLong K
	xorl %eax, %eax
	movl PC, %edx
	movw (%edx), %ax
	addl $2, %edx
	movl %edx, PC
	subl SA1PCBase, %edx
	addl %eax, %edx
	andl $0xffff, %edx
.endm

/************* AbsoluteIndexedIndirect8 ****************/
.macro AbsoluteIndexedIndirect8 K
	xorl %edx, %edx
	movw XX, %dx
	addw (PC), %dx
	addl $2, PC
	orl  SA1ShiftedPB, %edx
	call S9xSA1GetWord
	movl %eax, %edx
	andl $0xffff, %edx
.endm

/************* AbsoluteIndirectLong8 ****************/
.macro AbsoluteIndirectLong8 K
	movw (PC), %dx
	addl $2, PC
	andl $0xffff, %edx
	pushl %edx
	call S9xSA1GetWord
	popl %edx
	pushl %eax
	addl $2, %edx
	call S9xSA1GetByte
	popl %edx
	andl $0xff, %eax
	andl $0xffff, %edx
	sall $16, %eax
	orl  %eax, %edx
.endm

.macro AbsoluteIndirect8 K
	movw (PC), %dx
	addl $2, PC
	andl $0xffff, %edx
	call S9xSA1GetWord
	movl %eax, %edx
	andl $0xffff, %edx
.endm

.macro Absolute8 K
	movw (PC), %dx
	addl $2, PC
	andl $0xffff, %edx
	orl  SA1ShiftedDB, %edx
.endm

.macro AbsoluteLong8 K
	movl (PC), %edx
	add $3, PC
	andl $0xffffff, %edx
.endm

.macro Direct8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw DD, %dx
	incl PC
.endm

.macro DirectIndirectIndexed8 K
	xorl %edx, %edx
	movb (PC), %dl
	incl PC
	addw DD, %dx
	call S9xSA1GetWord
	movl SA1ShiftedDB, %edx
	movw %ax, %dx
	xorl %eax, %eax
	movw YY, %ax
	addl %eax, %edx
.endm

.macro DirectIndirectIndexedLong8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw DD, %dx
	incl PC
	pushl %edx
	call S9xSA1GetWord
	popl %edx
	pushw %ax
	addw $2, %dx
	call S9xSA1GetByte
	andl $0xff, %eax
	sall $16, %eax
	xorl %edx, %edx
	popw %ax
	movw YY, %dx
	addl %eax, %edx
	andl $0xffffff, %edx
.endm

.macro DirectIndexedIndirect8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw DD, %dx
	incl PC
	addw XX, %dx
	call S9xSA1GetWord
	movl SA1ShiftedDB, %edx
	movw %ax, %dx
.endm

.macro DirectIndexedX8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw DD, %dx
	incl PC
	addw XX, %dx
.endm

.macro DirectIndexedY8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw DD, %dx
	incl PC
	addw YY, %dx
.endm

.macro AbsoluteIndexedX8 K
	movl SA1ShiftedDB, %edx
	movw (PC), %dx
	xorl %eax, %eax
	addl $2, PC
	movw XX, %ax
	addl %eax, %edx
	andl $0xffffff, %edx
.endm

.macro AbsoluteIndexedY8 K
	movl SA1ShiftedDB, %edx
	movw (PC), %dx
	xorl %eax, %eax
	addl $2, PC
	movw YY, %ax
	addl %eax, %edx
	andl $0xffffff, %edx
.endm

.macro AbsoluteLongIndexedX8 K
	movl (PC), %edx
	xorl %eax, %eax
	addl $3, PC
	movw XX, %ax
	addl %eax, %edx
	andl $0xffffff, %edx
.endm

.macro DirectIndirect8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw DD, %dx
	incl PC
	call S9xSA1GetWord
	movl SA1ShiftedDB, %edx
	movw %ax, %dx
.endm

.macro DirectIndirectLong8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw DD, %dx
	incl PC
	pushl %edx
	call S9xSA1GetWord
	popl %edx
	pushw %ax
	addw $2, %dx
	call S9xSA1GetByte
	andl $0xff, %eax
	sall $16, %eax
	popw %ax
	movl %eax, %edx
.endm

.macro StackRelative8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw SS, %dx
	incl PC
.endm

.macro StackRelativeIndirectIndexed8 K
	xorl %edx, %edx
	movb (PC), %dl
	addw SS, %dx
	incl PC
	call S9xSA1GetWord
	movl SA1ShiftedDB, %edx
	movw %ax, %dx
	xorl %eax, %eax
	movw YY, %ax
	addl %eax, %edx
	andl $0xffffff, %edx
.endm

#if 0
.macro PushByte K
	movl SS, %edx
	andl $0xffff, %edx
	call S9xSA1SetByte
	decw SS
.endm

.macro PushWord K
	movl SS, %edx
	decl %edx
	andl $0xffff, %edx
	call S9xSA1SetWord
	movl SS, %edx
	subl $2, %edx
	movw %dx, SS
.endm

.macro PullByte K
	movl SS, %edx
	incl %edx
	movw %dx, SS
	andl $0xffff, %edx
	call S9xSA1GetByte
.endm

.macro PullWord K
	movl SS, %edx
	addw $2, %dx
	movw %dx, SS
	decl %edx
	andl $0xffff, %edx
	call S9xSA1GetWord
.endm
#endif

.macro PushByte K
	movl SS, %edx
	andl $0xffff, %edx
	call S9xSA1SetByte
	decw SS
.endm

.macro PushByteE K
	movl SS, %edx
	andl $0xffff, %edx
	call S9xSA1SetByte
	decb SS
.endm

.macro PushWord K
	xorl %edx, %edx
	movw SS, %dx
	pushl %eax
	pushl %edx
	movb %ah, %al
	call S9xSA1SetByte
	popl %edx
	popl %eax
	decw %dx
	call S9xSA1SetByte         
	subw $2, SS      
.endm

.macro PushWordE K
	xorl %edx, %edx
	movw SS, %dx
	pushl %eax
	pushl %edx
	movb %ah, %al
	call S9xSA1SetByte
	popl %edx
	popl %eax
	decb %dl
	call S9xSA1SetByte         
	subb $2, SS      
.endm

.macro PushWordENew K
	xorl %edx, %edx
	movw SS, %dx
	pushl %eax
	pushl %edx
	movb %ah, %al
	call S9xSA1SetByte
	popl %edx
	popl %eax
	decw %dx
	call S9xSA1SetByte
	movb $0x01, %dh
	subw $2, SS      
.endm

.macro PullByte K
	movl SS, %edx
	incl %edx
	movw %dx, SS
	andl $0xffff, %edx
	call S9xSA1GetByte
.endm

.macro PullByteE K
	movl SS, %edx
	incb %dl
	movw %dx, SS
	andl $0xffff, %edx
	call S9xSA1GetByte
.endm

.macro PullWord K
	movl SS, %edx
	incw %dx
	andl $0xffff, %edx 
	pushl %edx
	call S9xSA1GetByte
	movb %al, OpenBus
	popl %edx
	incw %dx
	movw %dx, SS
	call S9xSA1GetByte
	movb %al, %ah
	movb OpenBus, %al
.endm

.macro PullWordE K
	movl SS, %edx
	incb %dl
	andl $0xffff, %edx
	pushl %edx
	call S9xSA1GetByte
	movb %al, OpenBus
	popl %edx
	incb %dl
	movw %dx, SS
	call S9xSA1GetByte
	movb %al, %ah
	movb OpenBus, %al
.endm

.macro PullWordENew K
	movl SS, %edx
	incw %dx
	andl $0xffff, %edx 
	pushl %edx
	call S9xSA1GetByte
	movb %al, OpenBus
	popl %edx
	incw %dx
	call S9xSA1GetByte
	movb %al, %ah
	movb OpenBus, %al
	movb $0x01, %dh
	movw %dx, SS
.endm

