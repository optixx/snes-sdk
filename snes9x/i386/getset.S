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

.globl S9xGetByte

S9xGetByte:
	movl %edx, %eax
	and $0xffffff, %eax
	shrl $MEMMAP_SHIFT, %eax

	movb InDMA, %cl
	testb %cl, %cl
	jne .GB_NOADD
	movb MemorySpeed(%eax), %cl
	andl $0xff, %ecx
	addl %ecx, CYCLES
	SAVE_CYCLES /*save now, so we don't need to necessarily save later*/
.GB_NOADD:
#ifdef CPU_SHUTDOWN
	movb BlockIsRAM(%eax), %ch
#endif	
	movl Map(, %eax, 4), %eax
	cmpl $18, %eax /* MAP_LAST */
	jb .GBSpecial
	andl $0xffff, %edx
#ifdef CPU_SHUTDOWN
	testb %ch, %ch
	jz .GBNotRAM
	pushl %eax
	movl PCAtOpcodeStart, %eax
	movl %eax, WaitAddress
	popl %eax
.GBNotRAM:
#endif
	movb (%eax, %edx), %al
	ret

	.align 4
.GBSpecial:
	jmp *.GBJmpTable(, %eax, 4)
.data
	.align 4
.GBJmpTable:
	.long .GBPPU    /* MAP_PPU */
	.long .GBCPU    /* MAP_CPU */
	.long .GBDSP    /* MAP_DSP */
	.long .GBLSRAM  /* MAP_LOROM_SRAM */
	.long .GBHSRAM  /* MAP_HIROM_SRAM */
	.long .GBNONE   /* MAP_NONE */
	.long .GBDEBUG  /* MAP_DEBUG */
	.long .GBC4     /* MAP_C4 */
	.long .GBBWRAM  /* MAP_BWRAM */
	.long .GBNONE   /* MAP_BWRAM_BITMAP */
	.long .GBNONE   /* MAP_BWRAM_BITMAP2 */
	.long .GBNONE   /* MAP_SA1RAM */
	.long .GB7ROM   /* MAP_SPC7110_ROM */
	.long .GB7RAM   /* MAP_SPC7110_DRAM */
	.long .GB7SRM   /* MAP_RONLY_SRAM */
	.long .GBOBC1   /* MAP_OBC_RAM */
	.long .GBSDSP   /* MAP_SETA_DSP */
	.long .GBSRISC  /* MAP_SETA_RISC */

.text
.GBPPU: /* MAP_PPU */
	movb InDMA, %al
	testb %al, %al
        jz .GBPPUOK
        cmpb $%0x21, %dh
        je .GBNONE
.GBPPUOK:
	pushl %edx
	ccall S9xGetPPU
	popl %edx
	LOAD_CYCLES
	ret

.GBCPU: /* MAP_CPU */
	pushl %edx
	ccall S9xGetCPU
	popl %edx
	LOAD_CYCLES
	ret

.GBDSP: /* MAP_DSP */
	pushl %edx
	ccall S9xGetDSP
	LOAD_CYCLES
	popl %edx
	ret

.GBLSRAM: /* MAP_LOROM_SRAM */
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
	movb (%ecx, %edx), %al
	ret

.GB7SRM: /* MAP_RONLY_SRAM */
.GBHSRAM: /* MAP_HIROM_SRAM */
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAM, %eax
	andl SRAMMask, %edx
	movb (%eax, %edx), %al
	ret

.GBNONE: /* MAP_NONE */
	xorl %eax, %eax
	movb OpenBus, %al
	ret

.GBDEBUG: /* MAP_DEBUG */
	xorl %eax, %eax
	movb OpenBus, %al
	ret

.GBC4: /* MAP_C4 */
	pushl %edx
	ccall S9xGetC4
	popl %edx
	LOAD_CYCLES
	ret

.GBBWRAM: /* MAP_BWRAM */
	andl $0x7fff, %edx
	addl $-24576, %edx
	movl BWRAM, %eax
	movb (%eax, %edx), %al
	ret

.GB7ROM: /* MAP_SPC7110_ROM */
	pushl %edx
	ccall S9xGetSPC7110Byte
	popl %edx
	LOAD_CYCLES
	ret

.GB7RAM: /* MAP_SPC7110_DRAM */
	movl $0x4800, %edx
	pushl %edx
	ccall S9xGetSPC7110
	popl %edx
	LOAD_CYCLES
	ret

.GBOBC1: /* MAP_OBC_RAM */
	pushl %edx
	ccall GetOBC1
	popl %edx
	LOAD_CYCLES
	ret

.GBSDSP: /* MAP_SETA_DSP */
	pushl %edx
	ccall S9xGetSetaDSP
	popl %edx
	LOAD_CYCLES
	ret

.GBSRISC: /* MAP_SETA_RISC */
	pushl %edx
	ccall S9xGetST018
	popl %edx
	LOAD_CYCLES
	ret


.globl S9xGetWord

S9xGetWord:
	movl %edx, %eax
	/*MK: blocks can be 0x1000 bytes, not a 0x2000 minimum*/
	andl $MEMMAP_MASK, %eax
	cmpl $MEMMAP_MASK, %eax
	/*MK*/
	jne .GWNotAtBlockBoundary
.GWAtBlockBoundary:
	pushl %edx
	call S9xGetByte
	popl %edx
        movb %al, OpenBus
	incl %edx
	call S9xGetByte
        movb %al, %ah
	movb OpenBus, %al
	ret

.GWNotAtBlockBoundary:
	movl %edx, %eax
	and $0xffffff, %eax
	shrl $MEMMAP_SHIFT, %eax

	movb InDMA, %cl
	testb %cl, %cl
	jne .GW_NOADD
	movb MemorySpeed(%eax), %cl
	andl $0xff, %ecx
	addl %ecx, CYCLES
	addl %ecx, CYCLES
	SAVE_CYCLES
.GW_NOADD:

#ifdef CPU_SHUTDOWN
	movb BlockIsRAM(%eax), %ch
#endif	
	movl Map(, %eax, 4), %eax
	cmpl $18, %eax  /* MAP_LAST */
	jb .GWSpecial
	andl $0xffff, %edx
#ifdef CPU_SHUTDOWN
	testb %ch, %ch
	jz .GWNotRAM
	pushl %eax
	movl PCAtOpcodeStart, %eax
	movl %eax, WaitAddress
	popl %eax
.GWNotRAM:
#endif	
	movw (%eax, %edx), %ax
	ret

	.align 4
.GWSpecial:
	jmp *.GWJmpTable(, %eax, 4)

.data
	.align 4
.GWJmpTable:
	.long .GWPPU    /* MAP_PPU */
	.long .GWCPU    /* MAP_CPU */
	.long .GWDSP    /* MAP_DSP */
	.long .GWLSRAM  /* MAP_LOROM_SRAM */
	.long .GWHSRAM  /* MAP_HIROM_SRAM */
	.long .GWNONE   /* MAP_NONE */
	.long .GWDEBUG  /* MAP_DEBUG */
	.long .GWC4     /* MAP_C4 */
	.long .GWBWRAM  /* MAP_BWRAM */
	.long .GWNONE   /* MAP_BWRAM_BITMAP */
	.long .GWNONE   /* MAP_BWRAM_BITMAP2 */
	.long .GWNONE   /* MAP_SA1RAM */
	.long .GW7ROM   /* MAP_SPC7110_ROM */
	.long .GW7RAM   /* MAP_SPC7110_DRAM */
	.long .GW7SRM   /* MAP_RONLY_SRAM */
	.long .GWOBC1   /* MAP_OBC_RAM */
	.long .GWSDSP   /* MAP_SETA_DSP */
	.long .GWSRISC  /* MAP_SETA_RISC */

.text
.GWPPU: /* MAP_PPU */
	movb InDMA, %al
	testb %al, %al
        jnz .GWAtBlockBoundary
	pushl %edx
	ccall S9xGetPPU
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetPPU
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWCPU: /* MAP_CPU */
	pushl %edx
	ccall S9xGetCPU
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetCPU
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWDSP: /* MAP_DSP */
	pushl %edx
	ccall S9xGetDSP
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetDSP
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWLSRAM: /* MAP_LOROM_SRAM */
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
	movb (%ecx, %edx), %al
	incl %edx
	andl SRAMMask, %edx
	movb (%ecx, %edx), %ah
	ret

.GW7SRM:  /* MAP_RONLY_SRAM */
.GWHSRAM: /* MAP_HIROM_SRAM */
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAM, %ecx
	movb (%ecx, %edx), %al
	incl %edx
	andl SRAMMask, %edx
	movb (%ecx, %edx), %ah
	ret

.GWNONE:
	xorl %eax, %eax
	movb OpenBus, %ah
	movb OpenBus, %al
	ret

.GWDEBUG: /* MAP_DEBUG */
	xorl %eax, %eax
	movb OpenBus, %ah
	movb OpenBus, %al
	ret

.GWC4: /* MAP_C4 */
	pushl %edx
	ccall S9xGetC4
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetC4
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWBWRAM: /* MAP_BWRAM */
	andl $0x7fff, %edx
	addl $-24576, %edx
	movl BWRAM, %eax
	movw (%eax, %edx), %ax
	ret

.GW7ROM: /* MAP_SPC7110_ROM */
	pushl %edx
	ccall S9xGetSPC7110Byte
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetSPC7110Byte
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GW7RAM: /* MAP_SPC7110_DRAM */
	movl $0x4800, %edx
	pushl %edx
	ccall S9xGetSPC7110
	popl %edx
	pushl %eax
	pushl %edx
	ccall S9xGetSPC7110
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWOBC1: /* MAP_OBC_RAM */
	pushl %edx
	ccall GetOBC1
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall GetOBC1
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWSDSP: /* MAP_SETA_DSP */
	pushl %edx
	ccall S9xGetSetaDSP
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetSetaDSP
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWSRISC: /* MAP_SETA_RISC */
	pushl %edx
	ccall S9xGetST018
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetST018
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret


.globl S9xSetByte
S9xSetByte:
	pushl %eax
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif	
	movl %edx, %eax
	and $0xffffff, %eax
	shrl $MEMMAP_SHIFT, %eax

	movb InDMA, %cl
	testb %cl, %cl
	jne .SB_NOADD
	movb MemorySpeed(%eax), %cl
	andl $0xff, %ecx
	addl %ecx, CYCLES
	SAVE_CYCLES
.SB_NOADD:
	
	movl WriteMap(, %eax, 4), %eax
	cmpl $18, %eax  /* MAP_LAST */
	jb .SBSpecial
	andl $0xffff, %edx
#ifdef CPU_SHUTDOWN
	addl %eax, %edx
	cmpl SA1WaitByteAddress1, %edx
	jz .Matched
	cmpl SA1WaitByteAddress2, %edx
	jnz .NoMatch
.Matched:
	cmpl $0, SA1Opcodes
	movl $0, SA1WaitCounter
	setnz %al
	movb %al, SA1Executing
.NoMatch:
	popl %ecx
	movb %cl, (%edx)
#else
	popl %ecx
	movb %cl, (%eax, %edx)
#endif
	ret

.SBSpecial:
	jmp *.SBJmpTable(, %eax, 4)
.data
	.align 4
.SBJmpTable:
	.long .SBPPU    /* MAP_PPU */
	.long .SBCPU    /* MAP_CPU */
	.long .SBDSP    /* MAP_DSP */
	.long .SBLSRAM  /* MAP_LOROM_SRAM */
	.long .SBHSRAM  /* MAP_HIROM_SRAM */
	.long .SBNONE   /* MAP_NONE */
	.long .SBDEBUG  /* MAP_DEBUG */
	.long .SBC4     /* MAP_C4 */
	.long .SBBWRAM  /* MAP_BWRAM */
	.long .SBNONE   /* MAP_BWRAM_BITMAP */
	.long .SBNONE   /* MAP_BWRAM_BITMAP2 */
	.long .SBNONE   /* MAP_SA1RAM */
	.long .SBNONE   /* MAP_SPC7110_ROM */
	.long .SBNONE   /* MAP_SPC7110_DRAM */
	.long .SBNONE   /* MAP_RONLY_SRAM */
	.long .SBOBC1   /* MAP_OBC_RAM */
	.long .SBSDSP   /* MAP_SETA_DSP */
	.long .SBSRISC  /* MAP_SETA_RISC */

.text
.SBPPU: /* MAP_PPU */
	movb InDMA, %cl
	testb %cl, %cl
        jz .SBPPUOK
        cmpb $%0x21, %dh
        je .SBNONE
.SBPPUOK:
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetPPU can stomp it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetPPU
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SBCPU: /* MAP_CPU */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetCPU can stomp it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetCPU
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SBDSP: /* MAP_DSP */
	popl %eax
	pushl %edx	/* Save Address because S9xSetDSP can stomp it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetDSP
	LOAD_CYCLES
	addl $8, %esp
	popl %edx
	ret

.SBLSRAM: /* MAP_LOROM_SRAM */
	popl %eax
	movl SRAMMask, %ecx
	orl %ecx, %ecx
	jz .SBLSRAM_SKIP
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
	movb %al, (%ecx, %edx)
	movb $1, SRAMModified
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
	movl SRAMMask, %ecx
	orl %ecx, %ecx
	jz .SBHSRAM_SKIP
	andl %ecx, %edx
	movl SRAM, %ecx
	movb %al, (%ecx, %edx)
	movb $1, SRAMModified
.SBHSRAM_SKIP:
	ret

.SBNONE:  /* MAP_NONE */
.SBDEBUG: /* MAP_DEBUG */
	popl %eax
	ret

.SBC4: /* MAP_C4 */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetC4 can stomp it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetC4
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SBBWRAM: /* MAP_BWRAM */
	andl $0x7fff, %edx
	popl %eax
	addl $-24576, %edx
	movl BWRAM, %ecx
	movb %al, (%ecx, %edx)
	movb $1, SRAMModified
	ret

.SBOBC1: /* MAP_OBC_RAM */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because SetOBC1 can stomp it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall SetOBC1
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SBSDSP: /* MAP_SETA_DSP */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetSetaDSP can stomp it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetSetaDSP
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SBSRISC: /* MAP_SETA_RISC */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetST018 can stomp it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetST018
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.globl S9xSetWord

S9xSetWord:
	pushl %eax

	movl %edx, %eax
	andl $MEMMAP_MASK, %eax
	cmpl $MEMMAP_MASK, %eax
	jne .SWNotAtBlockBoundary
.SWAtBlockBoundary:
	xorl %eax, %eax
	pushl %edx
	movb 4(%esp), %al
	call S9xSetByte
	popl %edx
	xorl %eax, %eax
	incl %edx
	movb 1(%esp), %al
	call S9xSetByte
	popl %ecx
	ret

.SWNotAtBlockBoundary:

#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif	
	movl %edx, %eax
	and $0xffffff, %eax
	shrl $MEMMAP_SHIFT, %eax

	movb InDMA, %cl
	testb %cl, %cl
	jne .SW_NOADD
	movb MemorySpeed(%eax), %cl
	andl $0xff, %ecx
	addl %ecx, CYCLES
	addl %ecx, CYCLES
	SAVE_CYCLES
.SW_NOADD:
	movl WriteMap(, %eax, 4), %eax
	cmpl $18, %eax  /* MAP_LAST */
	jb .SWSpecial
	andl $0xffff, %edx
#ifdef CPU_SHUTDOWN
	addl %eax, %edx
	cmpl SA1WaitByteAddress1, %edx
	jz .Matched2
	cmpl SA1WaitByteAddress2, %edx
	jnz .NoMatch2
.Matched2:
	cmpl $0, SA1Opcodes
	movl $0, SA1WaitCounter
	setnz %al
	movb %al, SA1Executing
.NoMatch2:
	popl %ecx
	movw %cx, (%edx)
#else
	popl %ecx
	movw %cx, (%eax, %edx)
#endif
	ret

.SWSpecial:
	jmp *.SWJmpTable(, %eax, 4)
.data
	.align 4
.SWJmpTable:
	.long .SWPPU    /* MAP_PPU */
	.long .SWCPU    /* MAP_CPU */
	.long .SWDSP    /* MAP_DSP */
	.long .SWLSRAM  /* MAP_LOROM_SRAM */
	.long .SWHSRAM  /* MAP_HIROM_SRAM */
	.long .SWNONE   /* MAP_NONE */
	.long .SWDEBUG  /* MAP_DEBUG */
	.long .SWC4     /* MAP_C4 */
	.long .SWBWRAM  /* MAP_BWRAM */
	.long .SWNONE   /* MAP_BWRAM_BITMAP */
	.long .SWNONE   /* MAP_BWRAM_BITMAP2 */
	.long .SWNONE   /* MAP_SA1RAM */
	.long .SWNONE   /* MAP_SPC7110_ROM */
	.long .SWNONE   /* MAP_SPC7110_DRAM */
	.long .SWNONE   /* MAP_RONLY_SRAM */
	.long .SWOBC1   /* MAP_OBC_RAM */
	.long .SWSDSP   /* MAP_SETA_DSP */
	.long .SWSRISC  /* MAP_SETA_RISC */

.text
.SWPPU: /* MAP_PPU */
	movb InDMA, %cl
	testb %cl, %cl
        jnz .SWAtBlockBoundary
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetPPU will use it with certain optimizations enabled */
	pushl %eax	/* Save Byte because S9xSetPPU will use it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetPPU
	popl %eax
	popl %edx
	popl %eax
	popl %edx
	pushl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetPPU
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SWCPU: /* MAP_CPU */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetCPU will use it with certain optimizations enabled */
	pushl %eax	/* Save Byte because S9xSetCPU will use it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetCPU
	popl %eax
	popl %edx
	popl %eax
	popl %edx
	pushl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetCPU
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SWDSP: /* MAP_DSP */
	popl %eax
	pushl %edx	/* Save Address because S9xSetDSP will use it with certain optimizations enabled */
	pushl %eax	/* Save Byte because S9xSetDSP will use it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetDSP
	popl %eax
	popl %edx
	popl %eax
	popl %edx
	pushl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetDSP
	LOAD_CYCLES
	addl $8, %esp
	popl %edx
	ret

.SWLSRAM: /* MAP_LOROM_SRAM */
	popl %eax
	movl SRAMMask, %ecx
	orl %ecx, %ecx
	jz .SWLSRAM_SKIP
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
	movb %al, (%ecx, %edx)
	incl %edx
	andl SRAMMask, %edx
	movb %ah, (%ecx, %edx)
	movb $1, SRAMModified
.SWLSRAM_SKIP:
	ret

.SWHSRAM: /* MAP_HIROM_SRAM */
	popl %eax
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAMMask, %ecx
	orl %ecx, %ecx
	jz .SWHSRAM_SKIP
	andl %ecx, %edx
	movl SRAM, %ecx
	movb %al, (%ecx, %edx)
	incl %edx
	andl SRAMMask, %edx
	movb %ah, (%ecx, %edx)
	movb $1, SRAMModified
.SWHSRAM_SKIP:
	ret

.SWNONE:
.SWDEBUG: /* MAP_DEBUG */
	popl %eax
	ret

.SWC4: /* MAP_C4 */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetC4 will use it with certain optimizations enabled */
	pushl %eax	/* Save Byte because S9xSetC4 will use it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetC4
	popl %eax
	popl %edx
	popl %eax
	popl %edx
	pushl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetC4
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SWBWRAM: /* MAP_BWRAM */
	andl $0x7fff, %edx
	popl %eax
	addl $-24576, %edx
	movl BWRAM, %ecx
	movw %ax, (%ecx, %edx)
	movb $1, SRAMModified
	ret

.SWOBC1: /* MAP_OBC1 */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetOBC1 will use it with certain optimizations enabled */
	pushl %eax	/* Save Byte because S9xSetOBC1 will use it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall SetOBC1
	popl %eax
	popl %edx
	popl %eax
	popl %edx
	pushl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall SetOBC1
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SWSDSP: /* MAP_SETA_DSP */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetSetaDSP will use it with certain optimizations enabled */
	pushl %eax	/* Save Byte because S9xSetSetaDSP will use it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetSetaDSP
	popl %eax
	popl %edx
	popl %eax
	popl %edx
	pushl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetSetaDSP
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret

.SWSRISC: /* MAP_SETA_RISC */
	STORE_REGISTERS
	popl %eax
	pushl %edx	/* Save Address because S9xSetST018 will use it with certain optimizations enabled */
	pushl %eax	/* Save Byte because S9xSetST018 will use it with certain optimizations enabled */
	pushl %edx
	pushl %eax
	ccall S9xSetST018
	popl %eax
	popl %edx
	popl %eax
	popl %edx
	pushl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetST018
	LOAD_REGISTERS
	addl $8, %esp
	popl %edx
	ret


.globl S9xSetPCBase

S9xSetPCBase:
	movl %edx, %eax
	and $0xffffff, %eax
	shrl $MEMMAP_SHIFT, %eax
	movb MemorySpeed(%eax), %cl
	andl $0xff, %ecx
	movl %ecx, MemSpeed
	addl %ecx, %ecx
	movl %ecx, MemSpeedx2
	movl Map(, %eax, 4), %eax
	cmpl $18, %eax  /* MAP_LAST */
	jb .SPCSpecial
	andl $0xffff, %edx
	movl %eax, PCBase
	addl %edx, %eax
	movl %eax, PC
	ret

	.align 4
.SPCSpecial:
	jmp *.SPCJmpTable(, %eax, 4)
.data
	.align 4
.SPCJmpTable:
	.long .SPCNONE   /* MAP_PPU */
	.long .SPCNONE   /* MAP_CPU */
	.long .SPCNONE   /* MAP_DSP */
	.long .SPCLSRAM  /* MAP_LOROM_SRAM */
	.long .SPCHSRAM  /* MAP_HIROM_SRAM */
	.long .SPCNONE   /* MAP_NONE */
	.long .SPCDEBUG  /* MAP_DEBUG */
	.long .SPCC4     /* MAP_C4 */
	.long .SPCBWRAM  /* MAP_BWRAM */
	.long .SPCNONE   /* MAP_BWRAM_BITMAP */
	.long .SPCNONE   /* MAP_BWRAM_BITMAP2 */
	.long .SPCNONE   /* MAP_SA1RAM */
	.long .SPC7110   /* MAP_SPC7110_ROM */
	.long .SPCNONE   /* MAP_SPC7110_DRAM */
	.long .SPCNONE   /* MAP_RONLY_SRAM */
	.long .SPCOBC1   /* MAP_OBC_RAM */
	.long .SPCNONE   /* MAP_SETA_DSP */
	.long .SPCNONE   /* MAP_SETA_RISC */

.text
.SPCLSRAM: /* MAP_LOROM_SRAM */
	movl SRAMMask, %ecx
	andl $MEMMAP_MASK, %ecx
        cmpl $MEMMAP_MASK, %ecx
        jne .SPCNONE

        pushl %edx
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
        addl %edx, %ecx
	movl %ecx, PC
        popl %edx
        andl $0xffff, %edx
        subl %edx, %ecx
	movl %ecx, PCBase
	ret

.SPCHSRAM: /* MAP_HIROM_SRAM */
	movl SRAMMask, %ecx
	andl $MEMMAP_MASK, %ecx
        cmpl $MEMMAP_MASK, %ecx
        jne .SPCNONE

        pushl %edx
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAM, %eax
	andl SRAMMask, %edx
        addl %edx, %ecx
	movl %ecx, PC
        popl %edx
        andl $0xffff, %edx
        subl %edx, %ecx
	movl %ecx, PCBase
	ret

.SPCNONE:
.SPCDEBUG: /* MAP_DEBUG */
	movl $0, %eax
	movl %eax, PCBase
	movl %eax, PC
	ret

.SPCC4: /* MAP_C4 */
	pushl %edx
	pushl %edx
	ccall S9xGetBasePointerC4
	popl %edx
        popl %edx
        movl %eax, PC
        testl %eax, %eax
        jz .SPCC4NULL
	andl $0xffff, %edx
        subl %edx, %eax
.SPCC4NULL:
        movl %eax, PCBase
	ret

.SPCBWRAM: /* MAP_BWRAM */
	movl BWRAM, %eax
	addl $-24576, %eax
        movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x8000, %ecx
        addl %eax, %edx
        subl %ecx, %eax
	movl %eax, PCBase
	movl %edx, PC
	ret

.SPCOBC1: /* MAP_OBC1 */
	pushl %edx
	pushl %edx
	ccall GetBasePointerOBC1
	popl %edx
        popl %edx
        movl %eax, PC
        testl %eax, %eax
        jz .SPCOBC1NULL
	andl $0xffff, %edx
        subl %edx, %eax
.SPCOBC1NULL:
        movl %eax, PCBase
	ret

.SPC7110: /* MAP_SPC7110 */
	pushl %edx
	pushl %edx
	ccall Get7110BasePtr
	popl %edx
        popl %edx
        movl %eax, PC
        testl %eax, %eax
        jz .SPCC4NULL
	andl $0xffff, %edx
        subl %edx, %eax
.SPC7110NULL:
        movl %eax, PCBase
	ret
