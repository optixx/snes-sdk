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


#define NONE 0
#define READ 1
#define WRITE 2
#define MODIFY 3
#define JUMP 4

.macro GetCarry
	movb _Carry, %dl
	shrb %dl
.endm

.macro GetNotCarry
	cmpb $1, _Carry
.endm

.macro SetZN
	movb %al, _Zero
	movb %al, _Negative
.endm

.macro Set16ZN
	movb %ah, _Negative
	setnz _Zero
.endm

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


/************* IMMEDIATE8 ****************/
.Macro Immediate8 K A
	movb (PC), %al
	addl MemSpeed, CYCLES
	incl PC
.endm

/************* IMMEDIATE16 ****************/
.macro Immediate16 K A
	movw (PC), %ax
	addl MemSpeedx2, CYCLES
	addl $2, PC
.endm

/************* Relative ****************/
.macro Relative K A
	movsbl (PC), %eax
	incl PC
	addl MemSpeed, CYCLES
	movl PC, %edx
	subl PCBase, %edx
	addl %eax, %edx
	andl $0xffff, %edx
.endm

/************* RelativeLong ****************/
.macro RelativeLong K A
	xorl %eax, %eax
	addl MemSpeedx2, CYCLES
	movw (PC), %ax
	addl $6, CYCLES
	addl $2, PC
	movl PC, %edx
	subl PCBase, %edx
	addl %eax, %edx
	andl $0xffff, %edx
.endm

/************* AbsoluteIndexedIndirect8 ****************/
.macro AbsoluteIndexedIndirect8 K A
	xorl %edx, %edx
	addl MemSpeedx2, CYCLES
	movw (PC), %dx
	movb %dh, OpenBus
	addw XX, %dx
	orl  ShiftedPB, %edx
	addl $2, PC
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
.if \A&READ
	movb %ah, OpenBus
.endif
.endm

/************* AbsoluteIndirectLong8 ****************/
.macro AbsoluteIndirectLong8 K A
	movw (PC), %dx
	addl MemSpeedx2, CYCLES
	movb %dh, OpenBus
	addl $2, PC
	andl $0xffff, %edx
	pushl %edx
	call S9xGetWord
	popl %edx
	pushl %eax
	addl $2, %edx
	call S9xGetByte
.if \A&READ
	movb %al, OpenBus
.endif
	popl %edx
	andl $0xff, %eax
	andl $0xffff, %edx
	sall $16, %eax
	orl  %eax, %edx
.endm

.macro AbsoluteIndirect8 K A
	movw (PC), %dx
	addl MemSpeedx2, CYCLES
	movb %dh, OpenBus
	addl $2, PC
	andl $0xffff, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
.if \A&READ
	movb %ah, OpenBus
.endif
.endm

.macro Absolute8 K A
	movw (PC), %dx
	addl MemSpeedx2, CYCLES
.if \A&READ
	movb %dh, OpenBus
.endif
	addl $2, PC
	andl $0xffff, %edx
	orl  ShiftedDB, %edx
.endm

.macro AbsoluteLong8 K A
.if \A&READ
	movb 2(PC), %dl
	movb %dl, OpenBus
.endif
	movl (PC), %edx
	addl MemSpeedx2, CYCLES
	andl $0xffffff, %edx
	addl MemSpeed, CYCLES
	add $3, PC
.endm

.macro Direct8 K A
	addl MemSpeed, CYCLES
#if 0
	cmpb $0, DL
	je .Direct8\K
	addl $6, CYCLES
.Direct8\K:
#endif
	xorl %edx, %edx
	movb (PC), %dl
.if \A&READ
	movb %dl, OpenBus
.endif
	addw DD, %dx
	incl PC
.endm

.macro DirectIndirectIndexed8 K A
	addl MemSpeed, CYCLES
#if 0
	cmpb $0, DD
	je .DirectIndirectIndexed8\K
	addl $6, CYCLES
.DirectIndirectIndexed8\K:
#endif
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, OpenBus
	addw DD, %dx
	incl PC
	call S9xGetWord
.if \A&READ
	movb %ah, OpenBus
.endif
	movl ShiftedDB, %edx
	movw %ax, %dx
	xorl %eax, %eax
	movw YY, %ax
	addl %eax, %edx
.endm

.macro DirectIndirectIndexedLong8 K A
	addl MemSpeed, CYCLES
#if 0
	cmpb $0, DD
	je .DirectIndirectIndexedLong8\K
	addl $6, CYCLES
.DirectIndirectIndexedLong8\K:
#endif
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, OpenBus
	addw DD, %dx
	incl PC
	pushl %edx
	call S9xGetWord
	popl %edx
	pushw %ax
	addw $2, %dx
	call S9xGetByte
.if \A&READ
	movb %al, OpenBus
.endif
	andl $0xff, %eax
	sall $16, %eax
	xorl %edx, %edx
	popw %ax
	movw YY, %dx
	addl %eax, %edx
	andl $0xffffff, %edx
.endm

.macro DirectIndexedIndirect8 K A
	addl MemSpeed, CYCLES
	addl $6, CYCLES
#if 0
	cmpb $0, DD
	je .DirectIndexedIndirect8\K
	addl $6, CYCLES
.DirectIndexedIndirect8\K:
#endif
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, OpenBus
	addw DD, %dx
	incl PC
	addw XX, %dx
	call S9xGetWord
.if \A&READ
	movb %ah, OpenBus
.endif
	movl ShiftedDB, %edx
	movw %ax, %dx
.endm

.macro DirectIndexedX8 K A
	addl MemSpeed, CYCLES
	addl $6, CYCLES
#if 0
	cmpb $0, DD
	je .DirectIndexedX8\K
	addl $6, CYCLES
.DirectIndexedX8\K:
#endif
	xorl %edx, %edx
	movb (PC), %dl
	addw DD, %dx
	incl PC
.if \A&READ
 	movb %dl, OpenBus
.endif

	addw XX, %dx
   testw $Emulation, FLAGS16
	jz .DirectIndexedX8done\K
   andw $0x00ff, %dx     /* Zero page indexed wraps in emulation mode */
.DirectIndexedX8done\K:
.endm

.macro DirectIndexedY8 K A
	addl MemSpeed, CYCLES
	addl $6, CYCLES
#if 0
	cmpb $0, DD
	je .DirectIndexedY8\K
	addl $6, CYCLES
.DirectIndexedY8\K:
#endif
	xorl %edx, %edx
	movb (PC), %dl
.if \A&READ
	movb %dl, OpenBus
.endif	
	addw DD, %dx
	incl PC
	addw YY, %dx
    testw $Emulation, FLAGS16
	jz .DirectIndexedY8done\K
    andw $0x00ff, %dx
.DirectIndexedY8done\K:

.endm

.macro AbsoluteIndexedX8 K A
	movl ShiftedDB, %edx
	xorl %eax, %eax
	movw (PC), %dx
	movw XX, %ax
.if \A&READ
	movb %dh, OpenBus
.endif
	addl $2, PC
	addl %eax, %edx
	addl MemSpeedx2, CYCLES
	andl $0xffffff, %edx
.endm

.macro AbsoluteIndexedY8 K A
	movl ShiftedDB, %edx
	xorl %eax, %eax
	movw (PC), %dx
	movw YY, %ax
.if \A&READ
	movb %dh, OpenBus
.endif
	addl $2, PC
	addl %eax, %edx
	addl MemSpeedx2, CYCLES
	andl $0xffffff, %edx
.endm

.macro AbsoluteLongIndexedX8 K A
.if \A&READ
	movb 2(PC), %dl
	movb %dl, OpenBus
.endif
	movl (PC), %edx
	xorl %eax, %eax
	addl $3, PC
	movw XX, %ax
	addl MemSpeedx2, CYCLES
	addl %eax, %edx
	addl MemSpeed, CYCLES
	andl $0xffffff, %edx
.endm

.macro DirectIndirect8 K A
#if 0
	cmpb $0, DL
	je .DirectIndirect8\K
	addl $6, CYCLES
.DirectIndirect8\K:
#endif
	xorl %edx, %edx
	addl MemSpeed, CYCLES
	movb (PC), %dl
	movb %dl, OpenBus
	addw DD, %dx
	incl PC
	call S9xGetWord
.if \A&READ
	movb %ah, OpenBus
.endif
	movl ShiftedDB, %edx
	movw %ax, %dx
.endm

.macro DirectIndirectLong8 K A
#if 0
	cmpb $0, DL
	je .DirectIndirectLong8\K
	addl $6, CYCLES
.DirectIndirectLong8\K:
#endif
	xorl %edx, %edx
	addl MemSpeed, CYCLES
	movb (PC), %dl
	movb %dl, OpenBus
	addw DD, %dx
	incl PC
	pushl %edx
	call S9xGetWord
	popl %edx
	pushw %ax
	addw $2, %dx
	call S9xGetByte
.if \A&READ
	movb %al, OpenBus
.endif
	andl $0xff, %eax
	sall $16, %eax
	popw %ax
	movl %eax, %edx
.endm

.macro StackRelative8 K A
	xorl %edx, %edx
	addl MemSpeed, CYCLES
	movb (PC), %dl
.if \A&READ
	movb %dl, OpenBus
.endif
	addl $6, CYCLES
	addw SS, %dx
	incl PC
.endm

.macro StackRelativeIndirectIndexed8 K A
	xorl %edx, %edx
	addl MemSpeed, CYCLES
	movb (PC), %dl
	movb %dl, OpenBus
	addl $12, CYCLES
	addw SS, %dx
	incl PC
	call S9xGetWord
.if \A&READ
	movb %ah, OpenBus
.endif
	movl ShiftedDB, %edx
	movw %ax, %dx
	xorl %eax, %eax
	movw YY, %ax
	addl %eax, %edx
	andl $0xffffff, %edx
.endm

.macro BranchCheck0 K
	cmpb $0, BranchSkip
	jz .BranchCheck0S9xExit\K
	movb $0, BranchSkip
	cmpb $0, SoundSkipMethod
	jne .BranchCheck0S9xExit\K
	movl PC, %eax
	subl PCBase, %eax
	cmpw %dx, %ax
	jbe .BranchCheck0S9xExit\K
	jmp MainAsmLoop
.BranchCheck0S9xExit\K:
.endm

.macro BranchCheck1 K
	cmpb $0, BranchSkip
	jz .BranchCheck1S9xExit\K
	movb $0, BranchSkip
	movb SoundSkipMethod, %al
	cmpb $0, %al
	jne .BranchCheck1Not0\K
	movl PC, %eax
	subl PCBase, %eax
	cmpw %dx, %ax
	jbe .BranchCheck1S9xExit\K
	jmp MainAsmLoop
.BranchCheck1Not0\K:
	cmpb $1, %al
	jne .BranchCheck1Not1\K
	jmp MainAsmLoop
.BranchCheck1Not1\K:
	cmpb $3, %al
	jne .BranchCheck1S9xExit\K
	movl PC, %eax
	subl PCBase, %eax
	cmpw %dx, %ax
	jbe .BranchCheck13a\K
	jmp MainAsmLoop
.BranchCheck13a\K:
	movl PCBase, %eax
	andl $0xffff, %edx
	addl %edx, %eax
	movl %eax, PC
.BranchCheck1S9xExit\K:
.endm

.macro BranchCheck2 K
	cmpb $0, BranchSkip
	jz .BranchCheck2S9xExit\K
	movb $0, BranchSkip
	movb SoundSkipMethod, %al
	cmpb $0, %al
	jne .BranchCheck2Not0\K
	movl PC, %eax
	subl PCBase, %eax
	cmpw %dx, %ax
	jbe .BranchCheck2S9xExit\K
	jmp MainAsmLoop
.BranchCheck2Not0\K:
	cmpb $1, %al
	jne .BranchCheck2Not1\K
	movl PCBase, %eax
	andl $0xffff, %edx
	addl %edx, %eax
	movl %eax, PC
	jmp MainAsmLoop
.BranchCheck2Not1\K:
	cmpb $3, %al
	jne .BranchCheck2S9xExit\K
	movl PC, %eax
	subl PCBase, %eax
	cmpw %dx, %ax
	jbe .BranchCheck23a\K
	jmp MainAsmLoop
.BranchCheck23a\K:
	movl PCBase, %eax
	andl $0xffff, %edx
	addl %edx, %eax
	movl %eax, PC
.BranchCheck2S9xExit\K:
.endm

#if 0
.macro PushByte K
	addl $8, CYCLES
	xorl %edx, %edx
	movl RAM, %ecx
	movw SS, %dx
	movb %al, (%ecx, %edx)
	decw SS
.endm

.macro PushWord K
	addl $16, CYCLES
	xorl %edx, %edx
	movl RAM, %ecx
	movw SS, %dx
	decl %ecx
	movw %ax, (%ecx, %edx)
	subw $2, %dx
	movw %dx, SS
.endm

.macro PullByte K
	addl $8, CYCLES
	xorl %edx, %edx
	movl RAM, %eax
	movw SS, %dx
	incl %edx
	movw %dx, SS
	movb (%eax, %edx), %al
.endm

.macro PullWord K
	addl $16, CYCLES
	xorl %edx, %edx
	movl RAM, %eax
	movw SS, %edx
	addw $2, %dx
	movw %dx, SS
	movw -1(%eax, %edx), %ax
.endm
#else
.macro PushByte K
	movl SS, %edx
	andl $0xffff, %edx
	call S9xSetByte
	decw SS
.endm

.macro PushByteE K
	movl SS, %edx
	andl $0xffff, %edx
	call S9xSetByte
	decb SS
.endm

#if 0
.macro PushWord K
	movl SS, %edx
	decl %edx
	andl $0xffff, %edx
	call S9xSetWord
	subw $2, SS
.endm
#endif
#if 1
.macro PushWord K
	xorl %edx, %edx
	movw SS, %dx
	pushl %eax
	pushl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	decw %dx
	call S9xSetByte         
	subw $2, SS      
.endm
#endif

.macro PushWordE K
	xorl %edx, %edx
	movw SS, %dx
	pushl %eax
	pushl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	decb %dl
	call S9xSetByte         
	subb $2, SS
.endm

.macro PushWordENew K
	xorl %edx, %edx
	movw SS, %dx
	pushl %eax
	pushl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	decw %dx
	call S9xSetByte
	movb $0x01, %dh
	subw $2, SS      
.endm

.macro PullByte K
	movl SS, %edx
	incl %edx
	movw %dx, SS
	andl $0xffff, %edx
	call S9xGetByte
.endm

.macro PullByteE K
	movl SS, %edx
	incb %dl
	movw %dx, SS
	andl $0xffff, %edx
	call S9xGetByte
.endm

#if 0
.macro PullWord K
          movl SS, %edx 
           incl %edx 
           andl $0xffff, %edx 
           call S9xGetWord 
           addw $2, SS 
.endm
#else

.macro PullWord K
	movl SS, %edx
	incw %dx
	andl $0xffff, %edx 
	pushl %edx
	call S9xGetByte
	movb %al, OpenBus
	popl %edx
	incw %dx
	movw %dx, SS
	call S9xGetByte
	movb %al, %ah
	movb OpenBus, %al
.endm
#endif

.macro PullWordE K
	movl SS, %edx
	incb %dl
	andl $0xffff, %edx
	pushl %edx
	call S9xGetByte
	movb %al, OpenBus
	popl %edx
	incb %dl
	movw %dx, SS
	call S9xGetByte
	movb %al, %ah
	movb OpenBus, %al
.endm

.macro PullWordENew K
	movl SS, %edx
	incw %dx
	andl $0xffff, %edx 
	pushl %edx
	call S9xGetByte
	movb %al, OpenBus
	popl %edx
	incw %dx
	call S9xGetByte
	movb %al, %ah
	movb OpenBus, %al
	movb $0x01, %dh
	movw %dx, SS
.endm
#endif

