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

.text

S9xSA1GetByte_r:
	mov r4,r0
	mov #-MEMMAP_SHIFT,r3
	shld r3,r0
	mov.w .MEMMAP_MSK1,r3
	and r3,r0
	shll2 r0
	mov #SA1Map_offs,r3
	add SA1REG,r3
	mov.l @(r0,r3),r3
	cmp/pz r3
	bt .GBSpecial
	extu.w r4,r2
	add r2,r3
	mov.b @r3,r0
	rts
	extu.b r0,r0

.MEMMAP_MSK1:	
	.word MEMMAP_MASK

.GBSpecial:
	mova .GBJmpTable,r0
	shll2 r3
	add r3,r0
	mov.l @r0,r3
	jmp @r3
	nop


.GBPPU: /* MAP_PPU */
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetSA1,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r4
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	extu.b r0,r0

.GBCPU: /* MAP_CPU */

.GBDSP: /* MAP_DSP */
	rts
	mov #0,r0

.GBLSRAM: /* MAP_LOROM_SRAM */
	mov.l .SRAM0,r3
	mov.l @r3,r3
	mov.l .SRAMMask0,r2
	mov.w @r2,r2
	extu.w r2,r2
	and r4,r2
	add r2,r3
	mov.b @r3,r0
	rts
	extu.b r0,r0

.GBHSRAM: /* MAP_HIROM_SRAM */
	mov.w .n7fffA,r3
	and r4,r3
	mov.w .n6000A,r2
	sub r2,r3
	mov.l .nf0000,r2
	and r4,r2
	shlr2 r2
	shlr r2
	add r2,r3
	mov.l .SRAM0,r1
	mov.l @r1,r0
	mov.l .SRAMMask0,r2
	mov.w @r2,r2
	extu.w r2,r2
	and r3,r2
	mov.b @(r0,r2),r0
	rts
	extu.b r0,r0

.GBNONE:
.GBDEBUG: /* MAP_DEBUG */
	rts
	mov #0,r0

.GBBWRAM: /* MAP_BWRAM */
	mov.w .n7fffA,r3
	and r4,r3
	mov.w .n6000A,r2
	sub r2,r3
	mov.l SA1BWRAM,r0
	mov.b @(r0,r3),r0
	rts
	extu.b r0,r0

.GBBWBITMAP:
	mov.l .FillRAM0,r3
	mov.w .n6000A,r2
	mov.l @r3,r3
	mov.w .n223f0,r0
	shll8 r2
	mov.b @(r0,r3),r0
	mov r4,r3
	tst #0x80,r0
	sub r2,r3
	bt .GB4

	// Depth 2
	mov #3,r2
	shlr2 r3
	and r4,r2
	extu.w r3,r3
	mov.l .SRAM0,r0
	shll r2
	mov.b @(r0,r3),r0
	neg r2,r2
	shld r2,r0
	rts
	and #3,r0

.GB4:
	mov #1,r2
	shlr r3
	and r4,r2
	extu.w r3,r3
	mov.l .SRAM0,r0
	shll2 r2
	mov.b @(r0,r3),r0
	neg r2,r2
	shld r2,r0
	rts
	and #15,r0


.GBBWBITMAP2:
	mov.l .FillRAM0,r3
	mov.w .n6000A,r2
	mov.l @r3,r3
	mov.w .n223f0,r0
	mov.b @(r0,r3),r0
	extu.w r4,r3
	tst #0x80,r0
	sub r2,r3
	bt .GB42

	// Depth 2
	mov #3,r2
	shlr2 r3
	and r4,r2
	extu.w r3,r3
	mov.l SA1BWRAM,r0
	shll r2
	mov.b @(r0,r3),r0
	neg r2,r2
	shld r2,r0
	rts
	and #3,r0

.GB42:
	mov #1,r2
	shlr r3
	and r4,r2
	extu.w r3,r3
	mov.l SA1BWRAM,r0
	shll2 r2
	mov.b @(r0,r3),r0
	neg r2,r2
	shld r2,r0
	rts
	and #15,r0

.n7fffA:
	.word 0x7fff
.n6000A:
	.word 0x6000
.n223f0:
	.word 0x223f


S9xSA1GetWord_r:
	sts.l pr,@-r15
	bsr S9xSA1GetByte_r
	nop
	mov.l r0,@-r15
	bsr S9xSA1GetByte_r
	add #1,r4
	mov.l @r15+,r1
	add #-1,r4
	lds.l @r15+,pr
	shll8 r0
	rts
	or r1,r0

S9xSA1SetByte_r:
	mov r4,r2
	mov #-MEMMAP_SHIFT,r3
	shld r3,r2
	mov.w .MEMMAP_MSK,r3
	and r3,r2

	shll2 r2
	mov.w .wSA1WriteMap_offs,r3
	add SA1REG,r3
	add r2,r3
	mov.l @r3,r3
	cmp/pz r3
	bt .SBSpecial
	extu.w r4,r2
	add r2,r3
	rts
	mov.b r0,@r3

.wSA1WriteMap_offs:
	.word SA1WriteMap_offs

.SBSpecial:
	mov r4,r2
	extu.b r0,r4
	mova .SBJmpTable,r0
	shll2 r3
	add r3,r0
	mov.l @r0,r3
	jmp @r3
	extu.w r2,r5


.SBPPU: /* MAP_PPU */
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetSA1,r3
	jsr @r3
	nop
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	nop
	
.SBCPU: /* MAP_CPU */
	rts
	nop

.SBDSP: /* MAP_DSP */
	rts
	nop

.SBLSRAM: /* MAP_LOROM_SRAM */
	mov.l .SRAM0,r3
	mov.l @r3,r0
	mov.l .SRAMMask0,r5
	mov.w @r5,r5
	extu.w r5,r5
	tst r5,r5
	and r5,r2
	bt .SBLSRAM_SKIP
	mov.b r4,@(r0,r2)
.SBLSRAM_SKIP:
	rts
	nop

.SBHSRAM: /* MAP_HIROM_SRAM */
	mov.w .n7fff,r3
	and r4,r3
	mov.w .n6000,r5
	sub r5,r3
	mov.l .nf0000,r5
	and r5,r2
	shlr2 r2
	shlr r2
	add r2,r3
	mov.l .SRAM0,r0
	mov.l @r0,r0
	mov.l .SRAMMask0,r2
	mov.w @r2,r2
	extu.w r2,r2
	tst r2,r2
	and r3,r2
	bt .SBHSRAM_SKIP
	mov.b r4,@(r0,r2)
.SBHSRAM_SKIP:
	rts
	nop

.SBNONE:
.SBDEBUG: /* MAP_DEBUG */
	rts
	nop

.SBBWRAM: /* MAP_BWRAM */
	mov.w .n7fff,r0
	and r2,r0
	mov.w .n6000,r2
	sub r2,r0
	mov.l SA1BWRAM,r2
	rts
	mov.b r4,@(r0,r2)

.SBBWBITMAP:
	mov.l .FillRAM0,r0
	mov.w .n223f,r2
	mov.l @r0,r0
	mov.l .nnx600000,r1
	mov.b @(r0,r2),r0
	tst #0x80,r0
	bt/s .SB4
	add r2,r1

	// Depth 2
	mov #3,r0
	shlr2 r1
	and r2,r0
	extu.w r1,r1
	mov.l .SRAM0,r2
	shll r0
	add r2,r1
	mov #3,r2
	mov #3,r3
	shld r0,r2
	and r4,r3
	not r2,r2
	shld r0,r3
	mov.b @r1,r0
	and r2,r0
	or r3,r0
	rts
	mov.b r0,@r1

.SB4:
	mov #1,r0
	shlr r1
	and r2,r0
	extu.w r1,r1
	mov.l .SRAM0,r2
	shll2 r0
	add r2,r1
	mov #15,r2
	mov #15,r3
	shld r0,r2
	and r4,r3
	not r2,r2
	shld r0,r3
	mov.b @r1,r0
	and r2,r0
	or r3,r0
	rts
	mov.b r0,@r1


.SBBWBITMAP2:
	mov.l .FillRAM0,r0
	mov.w .n223f,r2
	mov.l @r0,r0
	mov.w .nnx6000,r1
	mov.b @(r0,r2),r0
	tst #0x80,r0
	bt/s .SB42
	add r5,r1

	// Depth 2
	mov #3,r0
	shlr2 r1
	and r5,r0
	extu.w r1,r1
	mov.l SA1BWRAM,r2
	shll r0
	add r2,r1
	mov #3,r2
	mov #3,r3
	shld r0,r2
	and r4,r3
	not r2,r2
	shld r0,r3
	mov.b @r1,r0
	and r2,r0
	or r3,r0
	rts
	mov.b r0,@r1

.SB42:
	mov #1,r0
	shlr r1
	and r5,r0
	extu.w r1,r1
	mov.l SA1BWRAM,r2
	shll2 r0
	add r2,r1
	mov #15,r2
	mov #15,r3
	shld r0,r2
	and r4,r3
	not r2,r2
	shld r0,r3
	mov.b @r1,r0
	and r2,r0
	or r3,r0
	rts
	mov.b r0,@r1

	.align 2

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

.S9xGetSA1:
	.long _S9xGetSA1
.S9xSetSA1:
	.long _S9xSetSA1
.SRAM0:
	.long SRAM
.SRAMMask0:
	.long SRAMMask
.FillRAM0:
	.long FillRAM
.nf0000:
	.long 0xf0000
.nnx600000:
	.long -0x600000
.n7fff:
	.word 0x7fff
.n6000:
	.word 0x6000
.nnx6000:
	.word -0x6000
.n223f:
	.word 0x223f
		
S9xSA1SetWord_r:
	sts.l pr,@-r15
	mov.l r0,@-r15
	bsr S9xSA1SetByte_r
	mov.l r4,@-r15
	mov.l @r15+,r4
	mov.l @r15+,r0
	add #1,r4
	shlr8 r0
	bra S9xSA1SetByte_r
	lds.l @r15+,pr

S9xSA1SetPCBase_r:
	mov r4,r2
	mov #-MEMMAP_SHIFT,r3
	shld r3,r2
	mov.w .MEMMAP_MSK,r3
	and r3,r2
	shll2 r2
	mov #SA1Map_offs,r3
	add SA1REG,r3
	add r2,r3
	mov.l @r3,r3
	cmp/pz r3
	bt .SPCSpecial
	extu.w r4,r2
	mov.l r3,SA1PCBase
	add r2,r3
	rts
	mov r3,PC

.MEMMAP_MSK:	
	.word MEMMAP_MASK

.SPCSpecial:
	mova .SPCJmpTable,r0
	shll2 r3
	add r3,r0
	mov.l @r0,r3
	jmp @r3
	nop

	.align 2
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


.SPCPPU: /* MAP_PPU */
	mov.l .FillRAMB,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,SA1PCBase
	rts
	add r2,PC

.SPCCPU: /* MAP_CPU */
	mov.l .FillRAMB,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,SA1PCBase
	rts
	add r2,PC

.SPCDSP: /* MAP_DSP */
	mov.l .FillRAMB,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,SA1PCBase
	rts
	add r2,PC

.SPCLSRAM: /* MAP_LOROM_SRAM */
	mov.l .SRAM2,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,SA1PCBase
	rts
	add r2,PC

.SPCHSRAM: /* MAP_HIROM_SRAM */
	mov.l .SRAM2,r3
	mov.w .n60002,r2
	mov.l @r3,PC
	add r2,PC
	extu.w r4,r2
	mov.l PC,SA1PCBase
	rts
	add r2,PC

.SPCNONE:
.SPCDEBUG: /* MAP_DEBUG */
	mov.l .SRAM2,PC
	rts
	mov.l PC,SA1PCBase

.SPCBWRAM: /* MAP_BWRAM */
	mov.l SA1BWRAM,r3
	mov.w .n60002,r2
	add r2,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,SA1PCBase
	rts
	add r2,PC

	.align 2

.SRAM2:
	.long SRAM
.FillRAMB:
	.long FillRAM
.n60002:
	.word 0x6000

