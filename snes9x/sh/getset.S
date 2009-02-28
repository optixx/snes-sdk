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

.globl S9xGetByte_r

	! GetByte preserves r4

S9xGetByte_r:
	mov r4,r0
	mov #-MEMMAP_SHIFT,r3
	shld r3,r0
	mov.w .MEMMAP_MSK1,r3
	and r3,r0
#ifdef VAR_CYCLES
	mov.l .MemorySpeed0,r3
	mov.b @(r0,r3),r1
#endif
#ifdef CPU_SHUTDOWN
	mov.l .BlockIsRAM0,r3
	mov.b @(r0,r3),r2
#endif	
	shll2 r0
	mov.l .Map0,r3
	mov.l @(r0,r3),r3
	cmp/pz r3
	bt .GBSpecial
#ifdef CPU_SHUTDOWN
	tst r2,r2
	bt .GBNotRAM
	mov.l PCAtOpcodeStart,r2
	mov.l r2,WaitAddress
.GBNotRAM:
#endif	
	extu.w r4,r2
#ifdef VAR_CYCLES
	add r1,CYCLES
#endif
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
#ifdef VAR_CYCLES
	mov.b InDMA,r0
	tst r0,r0
	bf .GBPPU_NOADD
	add #6, CYCLES
.GBPPU_NOADD:
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetPPU,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r4
	extu.b r0,r0
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	LOAD_CYCLES


.GBCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetCPU,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r4
	extu.b r0,r0
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	LOAD_CYCLES


.GBDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	add #8, CYCLES
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetDSP,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r4
	extu.b r0,r0
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	LOAD_CYCLES


.GBLSRAM: /* MAP_LOROM_SRAM */
#ifdef VAR_CYCLES
	add #8, CYCLES
#endif
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
#ifdef VAR_CYCLES
	add #8, CYCLES
#endif
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
	extu.w	r4,r0
#ifdef VAR_CYCLES
	add #8, CYCLES
#endif
	rts
	shlr8	r0

.GBDEBUG: /* MAP_DEBUG */
#ifdef VAR_CYCLES
	add #8, CYCLES
#endif
	rts
	mov #0,r0

.GBC4: /* MAP_C4 */
#ifdef VAR_CYCLES
	add #8, CYCLES
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetC4,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r4
	extu.b r0,r0
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	LOAD_CYCLES
	
.GBBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	add #8, CYCLES
#endif
	mov.w .n7fffA,r3
	and r4,r3
	mov.w .n6000A,r2
	sub r2,r3
	mov.l .BWRAM0,r2
	mov.l @r2,r0
	mov.b @(r0,r3),r0
	rts
	extu.b r0,r0

.n7fffA:
	.word 0x7fff
.n6000A:
	.word 0x6000

.globl S9xGetWord_r


	! GetWord preserves r4

S9xGetWord_r:
	mov r4,r2
	mov.w .n1fff,r3
	and r3,r2
	cmp/eq r3,r2
	bf .GWNotAtBlockBoundary
	sts.l pr,@-r15
	bsr S9xGetByte_r
	nop
	mov.l r0,@-r15
	bsr S9xGetByte_r
	add #1,r4
	mov.l @r15+,r2
	shll8 r0
	lds.l @r15+,pr
	add #-1,r4
	rts
	or r2,r0

.n1fff:
	.word	0x1fff

.GWNotAtBlockBoundary:
	mov r4,r0
	mov #-MEMMAP_SHIFT,r3
	shld r3,r0
	mov.w .MEMMAP_MSK15,r3
	and r3,r0
#ifdef VAR_CYCLES
	mov.l .MemorySpeed0,r3
	mov.b @(r0,r3),r1
#endif
#ifdef CPU_SHUTDOWN
	mov.l .BlockIsRAM0,r3
	mov.b @(r0,r3),r2
#endif	
	shll2 r0
	mov.l .Map0,r3
	mov.l @(r0,r3),r3
	cmp/pz r3
	bt .GWSpecial
#ifdef CPU_SHUTDOWN
	tst r2,r2
	bt .GWNotRAM
	mov.l PCAtOpcodeStart,r2
	mov.l r2,WaitAddress
.GWNotRAM:
#endif	
	extu.w r4,r2
#ifdef VAR_CYCLES
	add r1,CYCLES
	add r1,CYCLES
#endif
	add r2,r3
	mov.b @r3+,r2
	mov.b @r3,r0
	extu.b r2,r2
	shll8 r0
	or r2,r0
	rts
	extu.w r0,r0

.MEMMAP_MSK15:	
	.word MEMMAP_MASK

.GWSpecial:
	mova .GWJmpTable,r0
	shll2 r3
	add r3,r0
	mov.l @r0,r3
	jmp @r3
	nop


.GWPPU: /* MAP_PPU */
#ifdef VAR_CYCLES
	mov.b InDMA,r0
	tst r0,r0
	bf .GWPPU_NOADD
	add #12, CYCLES
.GWPPU_NOADD:
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetPPU,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15,r4
	mov.l r0,@-r15
	mov.l .S9xGetPPU,r3
	add #1,r4
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r1
	shll8 r0
	mov.l @r15+,r4
	or r1,r0
	lds.l @r15+,pr
	POP_REGISTERS
	LOAD_CYCLES
	rts
	extu.w r0,r0

.GWCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetCPU,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15,r4
	mov.l r0,@-r15
	mov.l .S9xGetCPU,r3
	add #1,r4
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r1
	shll8 r0
	mov.l @r15+,r4
	or r1,r0
	lds.l @r15+,pr
	POP_REGISTERS
	LOAD_CYCLES
	rts
	extu.w r0,r0

.GWDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	add #16, CYCLES
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetDSP,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15,r4
	mov.l r0,@-r15
	mov.l .S9xGetDSP,r3
	add #1,r4
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r1
	shll8 r0
	mov.l @r15+,r4
	or r1,r0
	lds.l @r15+,pr
	POP_REGISTERS
	LOAD_CYCLES
	rts
	extu.w r0,r0

.GWLSRAM: /* MAP_LOROM_SRAM */
#ifdef VAR_CYCLES
	add #16, CYCLES
#endif
	mov.l .SRAM0,r3
	mov.l @r3,r0
	mov.l .SRAMMask0,r2
	mov.w @r2,r2
	extu.w r2,r2
	mov r2,r1
	and r4,r1
	mov.b @(r0,r1),r1
	add #1,r4
	extu.b r1,r1
	and r4,r2
	add #-1,r4
	mov.b @(r0,r2),r0
	shll8 r0
	or r1,r0
	rts
	extu.w r0,r0

.GWHSRAM: /* MAP_HIROM_SRAM */
#ifdef VAR_CYCLES
	add #16, CYCLES
#endif
	mov.w .n7fff,r3
	and r4,r3
	mov.w .n6000,r2
	sub r2,r3
	mov.l .nf0000,r2
	and r4,r2
	shlr2 r2
	shlr r2
	add r2,r3
	mov.l .SRAM0,r1
	mov.l @r1,r1
	mov.l .SRAMMask0,r2
	mov.w @r2,r2
	extu.w r2,r2
	and r3,r2
	add r2,r1
	mov.b @r1+,r2
	mov.b @r1,r0
	extu.b r2,r2
	shll8 r0
	or r2,r0
	rts
	extu.w r0,r0

.GWNONE:
	extu.w	r4,r0
	extu.b	r4,r1
	mov r0,r2
#ifdef VAR_CYCLES
	add #16, CYCLES
#endif
	shlr8	r0
	add	r2,r0
	rts
	sub	r1,r0

	
.GWDEBUG: /* MAP_DEBUG */
#ifdef VAR_CYCLES
	add #16, CYCLES
#endif
	rts
	mov #0,r0

.GWC4:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xGetC4,r3
	mov.l r4,@-r15
	jsr @r3
	extu.w r4,r4
	mov.l @r15,r4
	mov.l r0,@-r15
	mov.l .S9xGetC4,r3
	add #1,r4
	jsr @r3
	extu.w r4,r4
	mov.l @r15+,r1
	shll8 r0
	mov.l @r15+,r4
	or r1,r0
	lds.l @r15+,pr
	POP_REGISTERS
	LOAD_CYCLES
	rts
	extu.w r0,r0

	
.GWBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	add #16, CYCLES
#endif
	mov.w .n7fff,r3
	and r4,r3
	mov.w .n6000,r2
	sub r2,r3
	mov.l .BWRAM0,r2
	mov.l @r2,r2
	add r2,r3
	mov.b @r3+,r2
	mov.b @r3,r0
	extu.b r2,r2
	shll8 r0
	or r2,r0
	rts
	extu.w r0,r0

	.align 4

#ifdef VAR_CYCLES
.MemorySpeed0:
	.long MemorySpeed
#endif
#ifdef CPU_SHUTDOWN
.BlockIsRAM0:
	.long BlockIsRAM
#endif
.Map0:
	.long Map
	
.GBJmpTable:
	.long .GBPPU
	.long .GBCPU
	.long .GBDSP
	.long .GBLSRAM
	.long .GBHSRAM
	.long .GBNONE
	.long .GBDEBUG
	.long .GBC4
	.long .GBBWRAM
	.long .GBNONE
	.long .GBNONE
	.long .GBNONE

.GWJmpTable:
	.long .GWPPU
	.long .GWCPU
	.long .GWDSP
	.long .GWLSRAM
	.long .GWHSRAM
	.long .GWNONE
	.long .GWDEBUG
	.long .GWC4
	.long .GWBWRAM
	.long .GWNONE
	.long .GWNONE
	.long .GWNONE

.S9xGetPPU:
	.long _S9xGetPPU
.S9xGetCPU:
	.long _S9xGetCPU
.S9xGetDSP:
	.long _S9xGetDSP
.S9xGetC4:
	.long _S9xGetC4
.BWRAM0:	
	.long BWRAM
.SRAM0:
	.long SRAM
.SRAMMask0:
	.long SRAMMask
.nf0000:
	.long 0xf0000
.n7fff:
	.word 0x7fff
.n6000:
	.word 0x6000


.globl S9xSetByte_r
S9xSetByte_r:
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif	
	mov r4,r2
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov #-MEMMAP_SHIFT,r3
	shld r3,r2
	mov.w .MEMMAP_MSK175,r3
	and r3,r2
#ifdef VAR_CYCLES
	mov.l .MemorySpeed2,r3
	add r2,r3
	mov.b @r3,r5
#endif
	shll2 r2
	mov.l .WriteMap0,r3
	add r2,r3
	mov.l @r3,r3
	cmp/pz r3
	bt .SBSpecial
	extu.w r4,r2
#ifdef VAR_CYCLES
	add r5, CYCLES
#endif
#ifdef CPU_SHUTDOWN
	mov.l SA1,r1
	add r2,r3
	mov.l SA1WaitByteAddress1,r2
	cmp/eq r2,r3
	bt .Matched
	mov.l SA1WaitByteAddress2,r2
	cmp/eq r2,r3
	bf .NoMatch
.Matched:
	mov.l SA1Opcodes,r2
	mov.b r0,@r3
	mov #0,r0
	cmp/hi r0,r2
	mov.l r0,SA1WaitCounter
	rotcl r0
	rts
	mov.b r0,SA1Executing
.NoMatch:
	rts
	mov.b r0,@r3
#else
	add r2,r3
	rts
	mov.b r0,@r3
#endif
	
.MEMMAP_MSK175:	
	.word MEMMAP_MASK

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
#ifdef VAR_CYCLES
	mov.b InDMA,r0
	tst r0,r0
	bf .SBPPU_NOADD
	add #6, CYCLES
.SBPPU_NOADD:
#endif
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetPPU,r3
	jsr @r3
	nop
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	nop

.SBCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetCPU,r3
	SAVE_CYCLES
	jsr @r3
	nop
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	LOAD_CYCLES


.SBDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	add #8, CYCLES
#endif
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetDSP,r3
	SAVE_CYCLES
	jsr @r3
	nop
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	LOAD_CYCLES

.SBLSRAM: /* MAP_LOROM_SRAM */
	mov.l .SRAM2,r3
	mov.l @r3,r0
	mov.l .SRAMMask2,r5
	mov.w @r5,r5
	extu.w r5,r5
	tst r5,r5
	and r5,r2
	bt .SBLSRAM_SKIP
	mov.b r4,@(r0,r2)
//	movb $1, SRAMModified
.SBLSRAM_SKIP:
	rts
#ifdef VAR_CYCLES
	add #8, CYCLES
#else
	nop
#endif

.SBHSRAM: /* MAP_HIROM_SRAM */
	mov.w .n7fff15,r3
	and r2,r3
	mov.w .n600015,r5
	sub r5,r3
	mov.l .nf00002,r5
	and r5,r2
	shlr2 r2
	shlr r2
	add r2,r3
	mov.l .SRAM2,r0
	mov.l @r0,r0
	mov.l .SRAMMask2,r2
	mov.w @r2,r2
	extu.w r2,r2
	tst r2,r2
	and r3,r2
	bt .SBHSRAM_SKIP
	mov.b r4,@(r0,r2)
//	movb $1, SRAMModified
.SBHSRAM_SKIP:
	rts
#ifdef VAR_CYCLES
	add #8, CYCLES
#else
	nop
#endif

.SBNONE:
.SBDEBUG: /* MAP_DEBUG */
	rts
#ifdef VAR_CYCLES
	add #8, CYCLES
#else
	nop
#endif

.SBC4: /* MAP_C4 */
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetC4,r3
	jsr @r3
	nop
	lds.l @r15+,pr
	POP_REGISTERS
	rts
#ifdef VAR_CYCLES
	add #6, CYCLES
#else
	nop
#endif


.SBBWRAM: /* MAP_BWRAM */
	mov.w .n7fff15,r0
	and r2,r0
	mov.w .n600015,r2
	sub r2,r0
	mov.l .BWRAM2,r2
	mov.l @r2,r2
//	movb $1, SRAMModified
	mov.b r4,@(r0,r2)
	rts
#ifdef VAR_CYCLES
	add #8, CYCLES
#else
	nop
#endif

.n7fff15:
	.word 0x7fff
.n600015:
	.word 0x6000

.globl S9xSetWord_r

S9xSetWord_r:
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif	
	mov r4,r2
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov #-MEMMAP_SHIFT,r3
	shld r3,r2
	mov.w .MEMMAP_MSK19,r3
	and r3,r2
#ifdef VAR_CYCLES
	mov.l .MemorySpeed2,r3
	add r2,r3
	mov.b @r3,r5
#endif
	shll2 r2
	mov.l .WriteMap0,r3
	add r2,r3
	mov.l @r3,r3
	cmp/pz r3
	bt .SWSpecial
	extu.w r4,r2
#ifdef VAR_CYCLES
	add r5, CYCLES
	add r5, CYCLES
#endif
#ifdef CPU_SHUTDOWN
	mov.l SA1,r1
	add r2,r3
	mov.l SA1WaitByteAddress1,r2
	mov.b r0,@r3
	cmp/eq r2,r3
	bt/s .Matched2
	shlr8 r0
	mov.l SA1WaitByteAddress2,r2
	cmp/eq r2,r3
	bf .NoMatch2
.Matched2:
	mov.l SA1Opcodes,r2
	mov.b r0,@(1,r3)
	mov #0,r0
	cmp/hi r0,r2
	mov.l r0,SA1WaitCounter
	rotcl r0
	rts
	mov.b r0,SA1Executing
.NoMatch2:
	rts
	mov.b r0,@(1,r3)
#else
	add r2,r3
	mov.b r0,@r3
	shlr8 r0
	rts
	mov.b r0,@(1,r3)
#endif
	
.MEMMAP_MSK19:	
	.word MEMMAP_MASK

.SWSpecial:
	mov r4,r2
	extu.w r0,r4
	mova .SWJmpTable,r0
	shll2 r3
	add r3,r0
	mov.l @r0,r3
	jmp @r3
	extu.w r2,r5



.SWPPU: /* MAP_PPU */
#ifdef VAR_CYCLES
	mov.b InDMA,r0
	tst r0,r0
	bf .SWPPU_NOADD
	add #12, CYCLES
.SWPPU_NOADD:
#endif
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetPPU,r3
	mov.l r4,@-r15
	mov.l r5,@-r15
	jsr @r3
	extu.b r4,r4
	mov.l .S9xSetPPU,r3
	mov.l @r15+,r5
	mov.l @r15+,r4
	add #1,r5
	shlr8 r4
	jsr @r3
	extu.w r5,r5
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	nop

.SWCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetCPU,r3
	mov.l r4,@-r15
	SAVE_CYCLES
	mov.l r5,@-r15
	jsr @r3
	extu.b r4,r4
	mov.l .S9xSetCPU,r3
	mov.l @r15+,r5
	mov.l @r15+,r4
	add #1,r5
	shlr8 r4
	jsr @r3
	extu.w r5,r5
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	LOAD_CYCLES
	
.SWDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	add #16, CYCLES
#endif
	SAVE_CYCLES
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetDSP,r3
	mov.l r4,@-r15
	mov.l r5,@-r15
	jsr @r3
	extu.b r4,r4
	mov.l .S9xSetDSP,r3
	mov.l @r15+,r5
	mov.l @r15+,r4
	add #1,r5
	shlr8 r4
	jsr @r3
	extu.w r5,r5
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	LOAD_CYCLES

.SWLSRAM: /* MAP_LOROM_SRAM */
	mov.l .SRAM2,r3
	mov.l @r3,r0
	mov.l .SRAMMask2,r5
	mov.w @r5,r5
	extu.w r5,r1
	tst r1,r1
	mov r1,r5
	and r2,r1
	bt/s .SWLSRAM_SKIP
	mov.b r4,@(r0,r1)
	add #1,r2
	shlr8 r4
	and r2,r5
//	movb $1, SRAMModified
	rts
	mov.b r4,@(r0,r5)
.SWLSRAM_SKIP:
	rts
#ifdef VAR_CYCLES
	add #16, CYCLES
#else
	nop
#endif

.SWHSRAM: /* MAP_HIROM_SRAM */
	mov.w .n7fff2,r3
	and r2,r3
	mov.w .n60003,r5
	sub r5,r3
	mov.l .nf00002,r5
	and r5,r2
	shlr2 r2
	shlr r2
	add r2,r3
	mov.l .SRAM2,r0
	mov.l @r0,r0
	mov.l .SRAMMask2,r2
	mov.w @r2,r2
	extu.w r2,r2
	tst r2,r2
	and r3,r2
	bt .SWHSRAM_SKIP
	mov.b r4,@(r0,r2)
	add #1,r2
	shlr8 r4
//	movb $1, SRAMModified
	rts
	mov.b r4,@(r0,r2)
.SWHSRAM_SKIP:
	rts
#ifdef VAR_CYCLES
	add #16, CYCLES
#else
	nop
#endif

.SWNONE:
.SWDEBUG: /* MAP_DEBUG */
	rts
#ifdef VAR_CYCLES
	add #16, CYCLES
#else
	nop
#endif

.SWC4: /* MAP_C4 */
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PUSH_REGISTERS
	sts.l pr,@-r15
	mov.l .S9xSetC4,r3
	mov.l r4,@-r15
	mov.l r5,@-r15
	jsr @r3
	extu.b r4,r4
	mov.l .S9xSetC4,r3
	mov.l @r15+,r5
	mov.l @r15+,r4
	add #1,r5
	shlr8 r4
	jsr @r3
	extu.w r5,r5
	lds.l @r15+,pr
	POP_REGISTERS
	rts
	nop

.SWBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	add #16, CYCLES
#endif
	mov.w .n7fff2,r3
	and r2,r3
	mov.w .n60002,r2
	sub r2,r3
	mov.l .BWRAM2,r2
	mov.l @r2,r0
	mov.b r4,@(r0,r3)
	add #1,r3
	shlr8 r4
//	movb $1, SRAMModified
	rts
	mov.b r4,@(r0,r3)


	
.globl S9xSetPCBase_r

S9xSetPCBase_r:
	mov r4,r2
	mov #-MEMMAP_SHIFT,r3
	shld r3,r2
	mov.w .MEMMAP_MSK2,r3
	and r3,r2
#ifdef VAR_CYCLES
	mov.l .MemorySpeed2,r3
	add r2,r3
	mov.b @r3,r1
#endif
	shll2 r2
	mov.l .Map2,r3
	add r2,r3
	mov.l @r3,r3
	cmp/pz r3
	bt .SPCSpecial
	extu.w r4,r2
#ifdef VAR_CYCLES
	mov.l r1,MemSpeed
	shll r1
	mov.l r1,MemSpeedx2
#endif
	mov.l r3,PCBase
	add r2,r3
	rts
	mov r3,PC

.SPCSpecial:
	mova .SPCJmpTable,r0
	shll2 r3
	add r3,r0
	mov.l @r0,r3
	jmp @r3
	nop

	.align 2
#ifdef VAR_CYCLES
.MemorySpeed2:
	.long MemorySpeed
#endif
.WriteMap0:
	.long WriteMap
.n7fff2:
	.word 0x7fff
.n60003:
	.word 0x6000

.SPCPPU: /* MAP_PPU */
#ifdef VAR_CYCLES
	mov #6,r1
	mov.l r1, MemSpeed
	mov #12,r1
	mov.l r1, MemSpeedx2
#endif
	mov.l .FillRAM0,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,PCBase
	rts
	add r2,PC

.SPCCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	mov #6,r1
	mov.l r1, MemSpeed
	mov #12,r1
	mov.l r1, MemSpeedx2
#endif
	mov.l .FillRAM0,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,PCBase
	rts
	add r2,PC

.SPCDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	mov #8,r1
	mov.l r1, MemSpeed
	mov #16,r1
	mov.l r1, MemSpeedx2
#endif
	mov.l .FillRAM0,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,PCBase
	rts
	add r2,PC

.SPCLSRAM: /* MAP_LOROM_SRAM */
#ifdef VAR_CYCLES
	mov #8,r1
	mov.l r1, MemSpeed
	mov #16,r1
	mov.l r1, MemSpeedx2
#endif
	mov.l .SRAM2,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,PCBase
	rts
	add r2,PC

.SPCHSRAM: /* MAP_HIROM_SRAM */
#ifdef VAR_CYCLES
	mov #8,r1
	mov.l r1, MemSpeed
	mov #16,r1
	mov.l r1, MemSpeedx2
#endif
	mov.l .SRAM2,r3
	mov.w .n60002,r2
	mov.l @r3,PC
	add r2,PC
	extu.w r4,r2
	mov.l PC,PCBase
	rts
	add r2,PC

.SPCNONE:
.SPCDEBUG: /* MAP_DEBUG */
#ifdef VAR_CYCLES
	mov #8,r1
	mov.l r1, MemSpeed
	mov #16,r1
	mov.l r1, MemSpeedx2
#endif
	mov.l .SRAM2,PC
	extu.w r4,r2
	mov.l PC,PCBase
	rts
	add r2,PC

.SPCC4: /* MAP_C4 */
#ifdef VAR_CYCLES
	mov #8,r1
	mov.l r1, MemSpeed
	mov #16,r1
	mov.l r1, MemSpeedx2
#endif
	mov.l .C4RAM0, r3
	mov.w .n60002,r2
	mov.l @r3,PC
	sub r2,PC
	extu.w r4,r2
	mov.l PC,PCBase
	rts
	add r2,PC

.SPCBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	mov #8,r1
	mov.l r1, MemSpeed
	mov #16,r1
	mov.l r1, MemSpeedx2
#endif
	mov.l .SRAM2,r3
	mov.w .n60002,r2
	add r2,r3
	extu.w r4,r2
	mov.l @r3,PC
	mov.l PC,PCBase
	rts
	add r2,PC


	
	.align 4

.Map2:
	.long Map

.SBJmpTable:
	.long .SBPPU
	.long .SBCPU
	.long .SBDSP
	.long .SBLSRAM
	.long .SBHSRAM
	.long .SBNONE
	.long .SBDEBUG
	.long .SBC4
	.long .SBBWRAM
	.long .SBNONE
	.long .SBNONE
	.long .SBNONE

.SWJmpTable:
	.long .SWPPU
	.long .SWCPU
	.long .SWDSP
	.long .SWLSRAM
	.long .SWHSRAM
	.long .SWNONE
	.long .SWDEBUG
	.long .SWC4
	.long .SWBWRAM
	.long .SWNONE
	.long .SWNONE
	.long .SWNONE

.SPCJmpTable:
	.long .SPCPPU
	.long .SPCCPU
	.long .SPCDSP
	.long .SPCLSRAM
	.long .SPCHSRAM
	.long .SPCNONE
	.long .SPCDEBUG
	.long .SPCC4
	.long .SPCBWRAM
	.long .SPCNONE
	.long .SPCNONE
	.long .SPCNONE

.S9xSetPPU:
	.long _S9xSetPPU
.S9xSetCPU:
	.long _S9xSetCPU
.S9xSetDSP:
	.long _S9xSetDSP
.S9xSetC4:
	.long _S9xSetC4
.BWRAM2:
	.long BWRAM
.SRAM2:
	.long SRAM
.SRAMMask2:
	.long SRAMMask
.FillRAM0:
	.long FillRAM
.C4RAM0:
	.long C4RAM
.nf00002:
	.long 0xf0000
.n60002:
	.word 0x6000
.MEMMAP_MSK2:	
	.word MEMMAP_MASK


