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

.data
.BankSave:
	.long 0
.text

S9xSA1GetByte:
	movl %edx, %eax
	and $0xffffff, %eax
	shrl $MEMMAP_SHIFT, %eax
	movl SA1Map(, %eax, 4), %eax
	cmpl $10, %eax
	jbe .GBSpecial
	andl $0xffff, %edx
	movb (%eax, %edx), %al
	ret

	.align 4
.GBSpecial:
	jmp *.GBJmpTable(, %eax, 4)
.data
	.align 4
.GBJmpTable:
	.long .GBPPU
	.long .GBCPU
	.long .GBDSP
	.long .GBLSRAM
	.long .GBHSRAM
	.long .GBNONE
	.long .GBDEBUG
	.long .GBNONE
	.long .GBBWRAM
	.long .GBBWBITMAP
	.long .GBBWBITMAP2

.text
.GBPPU: /* MAP_PPU */
	andl $0xffff, %edx
	pushl %edx
	ccall S9xGetSA1
	popl %edx
	ret

.GBCPU: /* MAP_CPU */

.GBDSP: /* MAP_DSP */
	mov $0, %al
	ret

.GBLSRAM: /* MAP_LOROM_SRAM */
	andl $0xffff, %edx
	movl SRAM, %ecx
	andw SRAMMask, %dx
	movb (%ecx, %edx), %al
	ret

.GBHSRAM: /* MAP_HIROM_SRAM */
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAM, %eax
	andw SRAMMask, %dx
	movb (%eax, %edx), %al
	ret

.GBNONE:
.GBDEBUG: /* MAP_DEBUG */
	xorl %eax, %eax
	movb OpenBus, %al
	ret

.GBBWRAM: /* MAP_BWRAM */
	andl $0x7fff, %edx
	addl $-24576, %edx
	movl SA1BWRAM, %eax
	movb (%eax, %edx), %al
	ret

.GBBWBITMAP:
	movl FillRAM, %eax
	subl $0x600000, %edx
	testb $0x80, 0x223f(%eax)
	movl %edx, %eax
	jz .GB4

	// Depth 2
	movb %dl, %cl
	shrl $2, %eax
	andb $3, %cl
	andl $0xffff, %eax
	addl SRAM, %eax
	shlb $1, %cl
	movb (%eax), %al
	shrb %cl, %al
	andb $3, %al
	ret	
.GB4:
	movb %dl, %cl
	shrl $1, %eax
	andb $1, %cl
	andl $0xffff, %eax
	addl SRAM, %eax
	shlb $2, %cl
	movb (%eax), %al
	shrb %cl, %al
	andb $15, %al
	ret	

.GBBWBITMAP2:
	andl $0xffff, %edx
	movl FillRAM, %eax
	subl $0x6000, %edx
	testb $0x80, 0x223f(%eax)
	movl %edx, %eax
	jz .GB42

	// Depth 2
	movb %dl, %cl
	shrl $2, %eax
	andb $3, %cl
	andl $0xffff, %eax
	addl SA1BWRAM, %eax
	shlb $1, %cl
	movb (%eax), %al
	shrb %cl, %al
	andb $3, %al
	ret	
.GB42:
	movb %dl, %cl
	shrl $1, %eax
	andb $1, %cl
	andl $0xffff, %eax
	addl SA1BWRAM, %eax
	shlb $2, %cl
	movb (%eax), %al
	shrb %cl, %al
	andb $15, %al
	ret	


S9xSA1GetWord:
	pushl %edx
	call S9xSA1GetByte
	popl %edx
	movb %al, OpenBus
	incl %edx
	call S9xSA1GetByte
	movb %al, %ah
	movb OpenBus, %al
	ret

S9xSA1SetByte:
	pushl %eax
	movl %edx, %eax
	and $0xffffff, %eax
	shrl $MEMMAP_SHIFT, %eax
	movl SA1WriteMap(, %eax, 4), %eax
	cmpl $10, %eax
	jbe .SBSpecial
	andl $0xffff, %edx
	popl %ecx
	movb %cl, (%eax, %edx)
	ret

.SBSpecial:
	jmp *.SBJmpTable(, %eax, 4)
.data
	.align 4
.SBJmpTable:
	.long .SBPPU
	.long .SBCPU
	.long .SBDSP
	.long .SBLSRAM
	.long .SBHSRAM
	.long .SBNONE
	.long .SBDEBUG
	.long .SBNONE
	.long .SBBWRAM
	.long .SBBWBITMAP
	.long .SBBWBITMAP2

.text
.SBPPU: /* MAP_PPU */
	andl $0xffff, %edx
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetSA1
	addl $8, %esp
	ret

.SBCPU: /* MAP_CPU */
	popl %eax
	ret

.SBDSP: /* MAP_DSP */
	popl %eax
	ret

.SBLSRAM: /* MAP_LOROM_SRAM */
	popl %eax
	movw SRAMMask, %cx
	orw %cx, %cx
	jz .SBLSRAM_SKIP
	andl $0xffff, %edx
	andw %cx, %dx
	movl SRAM, %ecx
	movb %al, (%ecx, %edx)
.SBLSRAM_SKIP:
	ret

.SBHSRAM: /* MAP_HIROM_SRAM */
	popl %eax
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movw SRAMMask, %cx
	orw %cx, %cx
	jz .SBHSRAM_SKIP
	andw %cx, %dx
	movl SRAM, %ecx
	movb %al, (%ecx, %edx)
.SBHSRAM_SKIP:
	ret

.SBNONE:
.SBDEBUG: /* MAP_DEBUG */
	popl %eax
	ret

.SBBWRAM: /* MAP_BWRAM */
	andl $0x7fff, %edx
	popl %eax
	addl $-24576, %edx
	movl SA1BWRAM, %ecx
	movb %al, (%ecx, %edx)
	ret

.SBBWBITMAP:
	movl FillRAM, %eax
	subl $0x600000, %edx
	movb 0x223f(%eax), %al
	testb $0x80, %al
	movl %edx, %eax
	jz .SB4

	// Depth 2
	shrl $2, %eax
	movb %dl, %cl
	andl $0xffff, %eax
	addl SRAM, %eax
	andb $3, %cl
	popl %edx
	shlb $1, %cl
	movb %dl, %dh
	movb $3, %dl
	shlb %cl, %dl
	notb %dl
	andb $3, %dh
	andb (%eax), %dl
	shlb %cl, %dh
	orb %dh, %dl
	movb %dl, (%eax)
	ret	
.SB4:
	shrl $1, %eax
	movb %dl, %cl
	andl $0xffff, %eax
	addl SRAM, %eax
	andb $1, %cl
	popl %edx
	shlb $2, %cl
	movb %dl, %dh
	movb $15, %dl
	shlb %cl, %dl
	notb %dl
	andb $15, %dh
	andb (%eax), %dl
	shlb %cl, %dh
	orb %dh, %dl
	movb %dl, (%eax)
	ret	

.SBBWBITMAP2:
	andl $0xffff, %edx
	subl $0x6000, %edx
	movl FillRAM, %eax
	testb $0x80, 0x223f(%eax)
	jz .SB42

	// Depth 2
	movl %edx, %eax
	shrl $2, %eax
	andl $0xffff, %eax
	addl SA1BWRAM, %eax

	movb %dl, %cl
	popl %edx
	andb $3, %cl
	movb %dl, %dh
	shlb $1, %cl
	movb $3, %dl
	andb $3, %dh
	shlb %cl, %dl
	shlb %cl, %dh
	notb %dl
	andb (%eax), %dl
	orb %dh, %dl
	movb %dl, (%eax)
	ret	
.SB42:
	movl %edx, %eax
	shrl $1, %eax
	andl $0xffff, %eax
	addl SA1BWRAM, %eax
	movb %dl, %cl
	andb $1, %cl
	shlb $2, %cl
	popl %edx
	movb %dl, %dh
	movb $15, %dl
	shlb %cl, %dl
	notb %dl
	andb (%eax), %dl
	andb $15, %dh
	shlb %cl, %dh
	orb %dh, %dl
	movb %dl, (%eax)
	ret	

S9xSA1SetWord:
	pushl %eax
	pushl %edx
	call S9xSA1SetByte
	popl %edx
	popl %eax
	incl %edx
	movb %ah, %al
	jmp S9xSA1SetByte

S9xSA1SetPCBase:
	movl %edx, %eax
	and $0xffffff, %eax
	shrl $MEMMAP_SHIFT, %eax
	movl SA1Map(, %eax, 4), %eax
	cmpl $10, %eax
	jbe .SPCSpecial
	andl $0xffff, %edx
	movl %eax, SA1PCBase
	addl %edx, %eax
	movl %eax, PC
	ret

	.align 4
.SPCSpecial:
	jmp *.SPCJmpTable(, %eax, 4)
.data
	.align 4
.SPCJmpTable:
	.long .SPCPPU
	.long .SPCCPU
	.long .SPCDSP
	.long .SPCLSRAM
	.long .SPCHSRAM
	.long .SPCNONE
	.long .SPCDEBUG
	.long .SPCNONE
	.long .SPCBWRAM
	.long .SPCNONE
	.long .SPCNONE

.text
.SPCPPU: /* MAP_PPU */
	movl FillRAM, %ecx
	andl $0xffff, %edx
	movl %ecx, SA1PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCCPU: /* MAP_CPU */
	movl FillRAM, %ecx
	andl $0xffff, %edx
	movl %ecx, SA1PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCDSP: /* MAP_DSP */
	movl FillRAM, %ecx
	andl $0xffff, %edx
	movl %ecx, SA1PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCLSRAM: /* MAP_LOROM_SRAM */
	movl SRAM, %ecx
	andl $0xffff, %edx
	movl %ecx, SA1PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCHSRAM: /* MAP_HIROM_SRAM */
	movl SRAM, %eax
	andl $0xffff, %edx
	addl $-24576, %eax
	movl %eax, SA1PCBase
	addl %eax, %edx
	movl %edx, PC
	ret

.SPCNONE:
.SPCDEBUG: /* MAP_DEBUG */
	movl SRAM, %eax
	movl %eax, SA1PCBase
	movl %eax, PC
	ret

.SPCBWRAM: /* MAP_BWRAM */
	movl SA1BWRAM, %eax
	andl $0xffff, %edx
	addl $-24576, %eax
	movl %eax, SA1PCBase
	addl %eax, %edx
	movl %edx, PC
	ret
