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

#define Registers SA1Registers
#include "sa1struc.h"

#if 0
#define PCS SA1PCS
#define PB SA1PB
#define PP SA1PP
#define PL SA1PL
#define PH SA1PH
#define AA SA1AA
#define AL SA1AL
#define AH SA1AH
#define DD SA1DD
#define DL SA1DL
#define DH SA1DH
#define SS SA1SS
#define SL SA1SL
#define SH SA1SH
#define XX SA1XX
#define XL SA1XL
#define XH SA1XH
#define YY SA1YY
#define YL SA1YL
#define YH SA1YH
#endif

#undef _Carry
#define _Carry SA1_Carry
#undef _Zero
#define _Zero SA1_Zero
#undef _Negative
#define _Negative SA1_Negative
#undef _Overflow
#define _Overflow SA1_Overflow

#include "sa1addr.h"
#include "sa1ops.h"
#include "sa1gs.S"

.data
.zzz:
.string "*** IRQ\n"

.text

.globl S9xSA1MainLoop
S9xSA1MainLoop:
	LOAD_REGISTERS
	testl $IRQ_PENDING_FLAG, SA1Flags
	jz .nosa1irq2
	testb $0xff, SA1IRQActive
	jz .nosa1irq1
	testb $IRQ, FLAGS
	jnz .nosa1irq2
	cmpb $0, WaitingForInterrupt
	je .nosa1wait
	incl PC
	movb $0, WaitingForInterrupt
.nosa1wait:
	call S9xSA1Opcode_IRQ
	jmp .nosa1irq2
.nosa1irq1:
	andl $~IRQ_PENDING_FLAG, SA1Flags	
.nosa1irq2:
	movl $3, %eax
.instruction_loop:
	testb $0xff, SA1Executing
	jz .exit
	pushl %eax
#ifdef DEBUGGER
	testb $TRACE_FLAG, SA1Flags
	jz .NO_SA1_TRACE
	STORE_REGISTERS
	ccall S9xSA1Trace
	LOAD_REGISTERS
.NO_SA1_TRACE:
#endif
	xorl %eax, %eax
	movl SA1Opcodes, %ecx
#ifdef CPU_SHUTDOWN
        movl PC, SA1PCAtOpcodeStart
#endif
	movb (PC), %al
	incl PC
	jmp *(%ecx,%eax,4)
.globl S9xSA1MainAsmLoop
S9xSA1MainAsmLoop:
	popl %eax
	decl %eax
	jnz .instruction_loop
.exit:
	STORE_REGISTERS
	ret

/* ADC */
Op69M1:
	Immediate8 ADC
	Adc8 IMM8

Op69M0:
	Immediate16 ADC2
	Adc16 IMM16

Op65M1:
	Direct8 ADC2
	call S9xSA1GetByte
	Adc8 DIR8

Op65M0:
	Direct8 ADC
	call S9xSA1GetWord
	Adc16 DIR16
	
Op75M1:
	DirectIndexedX8 ADC
	call S9xSA1GetByte
	Adc8 DIX8

Op75M0:
	DirectIndexedX8 ADC2
	call S9xSA1GetWord
	Adc16 DIX16

Op72M1:
	DirectIndirect8 ADC
	call S9xSA1GetByte
	Adc8 DI8

Op72M0:
	DirectIndirect8 ADC2
	call S9xSA1GetWord
	Adc16 DI16

Op61M1:
	DirectIndexedIndirect8 ADC
	call S9xSA1GetByte
	Adc8 DII8

Op61M0:
	DirectIndexedIndirect8 ADC2
	call S9xSA1GetWord
	Adc16 DII16

Op71M1:
	DirectIndirectIndexed8 ADC
	call S9xSA1GetByte
	Adc8 DIIY8

Op71M0:
	DirectIndirectIndexed8 ADC2
	call S9xSA1GetWord
	Adc16 DIIY16

Op67M1:
	DirectIndirectLong8 ADC
	call S9xSA1GetByte
	Adc8 DIL8

Op67M0:
	DirectIndirectLong8 ADC2
	call S9xSA1GetWord
	Adc16 DIL16

Op77M1:
	DirectIndirectIndexedLong8 ADC
	call S9xSA1GetByte
	Adc8 DIIL8

Op77M0:
	DirectIndirectIndexedLong8 ADC2
	call S9xSA1GetWord
	Adc16 DIIL8

Op6DM1:
	Absolute8 ADC
	call S9xSA1GetByte
	Adc8 ABS8

Op6DM0:
	Absolute8 ADC2
	call S9xSA1GetWord
	Adc16 ABS16

Op7DM1:
	AbsoluteIndexedX8 ADC
	call S9xSA1GetByte
	Adc8 ABSX8

Op7DM0:
	AbsoluteIndexedX8 ADC2
	call S9xSA1GetWord
	Adc16 ABSX16

Op79M1:
	AbsoluteIndexedY8 ADC
	call S9xSA1GetByte
	Adc8 ABSY8

Op79M0:
	AbsoluteIndexedY8 ADC2
	call S9xSA1GetWord
	Adc16 ABSY16

Op6FM1:
	AbsoluteLong8 ADC
	call S9xSA1GetByte
	Adc8 ABSL8

Op6FM0:
	AbsoluteLong8 ADC2
	call S9xSA1GetWord
	Adc16 ABSL16

Op7FM1:
	AbsoluteLongIndexedX8 ADC
	call S9xSA1GetByte
	Adc8 ALX8

Op7FM0:
	AbsoluteLongIndexedX8 ADC2
	call S9xSA1GetWord
	Adc16 ALX16

Op63M1:
	StackRelative8 ADC
	call S9xSA1GetByte
	Adc8 SREL8

Op63M0:
	StackRelative8 ADC2
	call S9xSA1GetWord
	Adc16 SREL16

Op73M1:
	StackRelativeIndirectIndexed8 ADC
	call S9xSA1GetByte
	Adc8 SRII8

Op73M0:
	StackRelativeIndirectIndexed8 ADC2
	call S9xSA1GetWord
	Adc16 SRII16

/* AND */
Op29M1:
	Immediate8 AND
	And8 IMM8

Op29M0:
	Immediate16 AND
	And16 IMM16

Op25M1:
	Direct8 AND
	call S9xSA1GetByte
	And8 DIR8

Op25M0:
	Direct8 AND2
	call S9xSA1GetWord
	And16 DIR16

Op35M1:
	DirectIndexedX8 AND
	call S9xSA1GetByte
	And8 DIX8

Op35M0:
	DirectIndexedX8 AND2
	call S9xSA1GetWord
	And16 DIX16

Op32M1:
	DirectIndirect8 AND
	call S9xSA1GetByte
	And8 DI8

Op32M0:
	DirectIndirect8 AND2
	call S9xSA1GetWord
	And16 DI16

Op21M1:
	DirectIndexedIndirect8 AND
	call S9xSA1GetByte
	And8 DII8

Op21M0:
	DirectIndexedIndirect8 AND2
	call S9xSA1GetWord
	And16 DII16

Op31M1:
	DirectIndirectIndexed8 AND
	call S9xSA1GetByte
	And8 DIIY8

Op31M0:
	DirectIndirectIndexed8 AND2
	call S9xSA1GetWord
	And16 DIIY16

Op27M1:
	DirectIndirectLong8 AND
	call S9xSA1GetByte
	And8 DIL8

Op27M0:
	DirectIndirectLong8 AND2
	call S9xSA1GetWord
	And16 DIL16

Op37M1:
	DirectIndirectIndexedLong8 AND
	call S9xSA1GetByte
	And8 DIIL8

Op37M0:
	DirectIndirectIndexedLong8 AND2
	call S9xSA1GetWord
	And16 DIIL16

Op2DM1:
	Absolute8 AND
	call S9xSA1GetByte
	And8 ABS8

Op2DM0:
	Absolute8 AND2
	call S9xSA1GetWord
	And16 ABS16

Op3DM1:
	AbsoluteIndexedX8 AND
	call S9xSA1GetByte
	And8 ABSX8

Op3DM0:
	AbsoluteIndexedX8 AND2
	call S9xSA1GetWord
	And16 ABSX16

Op39M1:
	AbsoluteIndexedY8 AND
	call S9xSA1GetByte
	And8 ABSY8

Op39M0:
	AbsoluteIndexedY8 AND2
	call S9xSA1GetWord
	And16 ABSY16

Op2FM1:
	AbsoluteLong8 AND
	call S9xSA1GetByte
	And8 ABSL8

Op2FM0:
	AbsoluteLong8 AND2
	call S9xSA1GetWord
	And16 ABSL16

Op3FM1:
	AbsoluteLongIndexedX8 AND
	call S9xSA1GetByte
	And8 ALX8

Op3FM0:
	AbsoluteLongIndexedX8 AND2
	call S9xSA1GetWord
	And16 ALX16

Op23M1:
	StackRelative8 AND
	call S9xSA1GetByte
	And8 SREL8

Op23M0:
	StackRelative8 AND2
	call S9xSA1GetWord
	And16 SREL16

Op33M1:
	StackRelativeIndirectIndexed8 AND
	call S9xSA1GetByte
	And8 SRII8

Op33M0:
	StackRelativeIndirectIndexed8 AND2
	call S9xSA1GetWord
	And16 SRII16

/* ASL */
Op0AM1:
	movb AL, %al
	salb %al
	movb %al, AL
	SetZNC
	jmp S9xSA1MainAsmLoop

Op0AM0:
	movw AA, %ax
	salw %ax
	movw %ax, AA
	setnz SA1_Zero
	setc SA1_Carry
	movb %ah, SA1_Negative
	jmp S9xSA1MainAsmLoop
	
Op06M1:
	Direct8 ASL
	Asl8 DIR8

Op06M0:
	Direct8 ASL2
	Asl16 DIR16

Op16M1:
	DirectIndexedX8 ASL
	Asl8 DIX

Op16M0:
	DirectIndexedX8 ASL2
	Asl16 DIX

Op0EM1:
	Absolute8 ASL
	Asl8 ABS

Op0EM0:
	Absolute8 ASL2
	Asl16 ABS

Op1EM1:
	AbsoluteIndexedX8 ASL
	Asl8 ABSX

Op1EM0:
	AbsoluteIndexedX8 ASL2
	Asl16 ABSX

/* BIT */
Op89M1:
	Immediate8 BIT
	andb AL, %al
	movb %al, SA1_Zero
	jmp S9xSA1MainAsmLoop

Op89M0:
	Immediate16 BIT
	andw AA, %ax
	setnz SA1_Zero
	jmp S9xSA1MainAsmLoop

Op24M1:
	Direct8 BIT
	Bit8 DIR

Op24M0:
	Direct8 BIT
	Bit16 DIR

Op34M1:
	DirectIndexedX8 BIT
	Bit8 DIX

Op34M0:
	DirectIndexedX8 BIT2
	Bit16 DIX

Op2CM1:
	Absolute8 BIT
	Bit8 ABS

Op2CM0:
	Absolute8 BIT2
	Bit16 ABS

Op3CM1:
	AbsoluteIndexedX8 BIT
	Bit8 ABSX

Op3CM0:
	AbsoluteIndexedX8 BIT2
	Bit16 ABSX

/* CMP */
OpC9M1:
	Immediate8 CMP
	Cmp8 IMM

OpC9M0:
	Immediate16 CMP
	Cmp16 IMM

OpC5M1:
	Direct8 CMP
	call S9xSA1GetByte
	Cmp8 DIR

OpC5M0:
	Direct8 CMP2
	call S9xSA1GetWord
	Cmp16 DIR

OpD5M1:
	DirectIndexedX8 CMP
	call S9xSA1GetByte
	Cmp8 DIX

OpD5M0:
	DirectIndexedX8 CMP2
	call S9xSA1GetWord
	Cmp16 DIX

OpD2M1:
	DirectIndirect8 CMP
	call S9xSA1GetByte
	Cmp8 DI

OpD2M0:
	DirectIndirect8 CMP2
	call S9xSA1GetWord
	Cmp16 DI

OpC1M1:
	DirectIndexedIndirect8 CMP
	call S9xSA1GetByte
	Cmp8 DII

OpC1M0:
	DirectIndexedIndirect8 CMP2
	call S9xSA1GetWord
	Cmp16 DII

OpD1M1:
	DirectIndirectIndexed8 CMP
	call S9xSA1GetByte
	Cmp8 DIIY

OpD1M0:
	DirectIndirectIndexed8 CMP2
	call S9xSA1GetWord
	Cmp16 DIIY

OpC7M1:
	DirectIndirectLong8 CMP
	call S9xSA1GetByte
	Cmp8 DIL

OpC7M0:
	DirectIndirectLong8 CMP2
	call S9xSA1GetWord
	Cmp16 DIL

OpD7M1:
	DirectIndirectIndexedLong8 CMP
	call S9xSA1GetByte
	Cmp8 DIIL

OpD7M0:
	DirectIndirectIndexedLong8 CMP2
	call S9xSA1GetWord
	Cmp16 DIIL

OpCDM1:
	Absolute8 CMP
	call S9xSA1GetByte
	Cmp8 ABS

OpCDM0:
	Absolute8 CMP2
	call S9xSA1GetWord
	Cmp16 ABS

OpDDM1:
	AbsoluteIndexedX8 CMP
	call S9xSA1GetByte
	Cmp8 ABSX

OpDDM0:
	AbsoluteIndexedX8 CMP2
	call S9xSA1GetWord
	Cmp16 ABSX

OpD9M1:
	AbsoluteIndexedY8 CMP
	call S9xSA1GetByte
	Cmp8 ABSY

OpD9M0:
	AbsoluteIndexedY8 CMP2
	call S9xSA1GetWord
	Cmp16 ABSY

OpCFM1:
	AbsoluteLong8 CMP
	call S9xSA1GetByte
	Cmp8 ABSL

OpCFM0:
	AbsoluteLong8 CMP2
	call S9xSA1GetWord
	Cmp16 ABSL

OpDFM1:
	AbsoluteLongIndexedX8 CMP
	call S9xSA1GetByte
	Cmp8 ALX

OpDFM0:
	AbsoluteLongIndexedX8 CMP2
	call S9xSA1GetWord
	Cmp16 ALX

OpC3M1:
	StackRelative8 CMP
	call S9xSA1GetByte
	Cmp8 SREL

OpC3M0:
	StackRelative8 CMP2
	call S9xSA1GetWord
	Cmp16 SREL

OpD3M1:
	StackRelativeIndirectIndexed8 CMP
	call S9xSA1GetByte
	Cmp8 SRII

OpD3M0:
	StackRelativeIndirectIndexed8 CMP2
	call S9xSA1GetWord
	Cmp16 SRII

/* CPX */
OpE0X1:
	Immediate8 CPX
	Cpx8 IMM

OpE0X0:
	Immediate16 CPX
	Cpx16 IMM

OpE4X1:
	Direct8 CPX
	call S9xSA1GetByte
	Cpx8 DIR

OpE4X0:
	Direct8 CPX2
	call S9xSA1GetWord
	Cpx16 DIR

OpECX1:
	Absolute8 CPX
	call S9xSA1GetByte
	Cpx8 ABS

OpECX0:
	Absolute8 CPX2
	call S9xSA1GetWord
	Cpx16 ABS


/* CPY */
OpC0X1:
	Immediate8 CPY
	Cpy8 IMM

OpC0X0:
	Immediate16 CPY
	Cpy16 IMM

OpC4X1:
	Direct8 CPY
	call S9xSA1GetByte
	Cpy8 DIR

OpC4X0:
	Direct8 CPY2
	call S9xSA1GetWord
	Cpy16 DIR

OpCCX1:
	Absolute8 CPY
	call S9xSA1GetByte
	Cpy8 ABS

OpCCX0:
	Absolute8 CPY2
	call S9xSA1GetWord
	Cpy16 ABS

/* DEC */
Op3AM1:
	movb AL, %al
	decb %al
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop

Op3AM0:
	decw AA
	setnz SA1_Zero
	movb AH, %al
	movb %al, SA1_Negative
	jmp S9xSA1MainAsmLoop

OpC6M1:
	Direct8 DEC
	Dec8 DIR

OpC6M0:
	Direct8 DEC2
	Dec16 DIR

OpD6M1:
	DirectIndexedX8 DEC
	Dec8 DIX

OpD6M0:
	DirectIndexedX8 DEC2
	Dec16 DIX

OpCEM1:
	Absolute8 DEC
	Dec8 ABS

OpCEM0:
	Absolute8 DEC2
	Dec16 ABS

OpDEM1:
	AbsoluteIndexedX8 DEC

	Dec8 ABSX

OpDEM0:
	AbsoluteIndexedX8 DEC2
	Dec16 ABSX

/* EOR */
Op49M1:
	Immediate8 EOR
	Eor8 IMM

Op49M0:
	Immediate16 EOR
	Eor16 IMM

Op45M1:
	Direct8 EOR
	call S9xSA1GetByte
	Eor8 DIR

Op45M0:
	Direct8 EOR2
	call S9xSA1GetWord
	Eor16 DIR

Op55M1:
	DirectIndexedX8 EOR
	call S9xSA1GetByte
	Eor8 DIX

Op55M0:
	DirectIndexedX8 EOR2
	call S9xSA1GetWord
	Eor16 DIX

Op52M1:
	DirectIndirect8 EOR
	call S9xSA1GetByte
	Eor8 DI

Op52M0:
	DirectIndirect8 EOR2
	call S9xSA1GetWord
	Eor16 DI

Op41M1:
	DirectIndexedIndirect8 EOR
	call S9xSA1GetByte
	Eor8 DII

Op41M0:
	DirectIndexedIndirect8 EOR2
	call S9xSA1GetWord
	Eor16 DII

Op51M1:
	DirectIndirectIndexed8 EOR
	call S9xSA1GetByte
	Eor8 DIIY

Op51M0:
	DirectIndirectIndexed8 EOR2
	call S9xSA1GetWord
	Eor16 DIIY

Op47M1:
	DirectIndirectLong8 EOR
	call S9xSA1GetByte
	Eor8 DIL

Op47M0:
	DirectIndirectLong8 EOR2
	call S9xSA1GetWord
	Eor16 DIL

Op57M1:
	DirectIndirectIndexedLong8 EOR
	call S9xSA1GetByte
	Eor8 DIIL

Op57M0:
	DirectIndirectIndexedLong8 EOR2
	call S9xSA1GetWord
	Eor16 DIIL

Op4DM1:
	Absolute8 EOR
	call S9xSA1GetByte
	Eor8 ABS

Op4DM0:
	Absolute8 EOR2
	call S9xSA1GetWord
	Eor16 ABS

Op5DM1:
	AbsoluteIndexedX8 EOR
	call S9xSA1GetByte
	Eor8 ABSX

Op5DM0:
	AbsoluteIndexedX8 EOR2
	call S9xSA1GetWord
	Eor16 ABSX

Op59M1:
	AbsoluteIndexedY8 EOR
	call S9xSA1GetByte
	Eor8 ABSY

Op59M0:
	AbsoluteIndexedY8 EOR2
	call S9xSA1GetWord
	Eor16 ABSY

Op4FM1:
	AbsoluteLong8 EOR
	call S9xSA1GetByte
	Eor8 ABSL

Op4FM0:
	AbsoluteLong8 EOR2
	call S9xSA1GetWord
	Eor16 ABSL

Op5FM1:
	AbsoluteLongIndexedX8 EOR
	call S9xSA1GetByte
	Eor8 ALX

Op5FM0:
	AbsoluteLongIndexedX8 EOR2
	call S9xSA1GetWord
	Eor16 ALX

Op43M1:
	StackRelative8 EOR
	call S9xSA1GetByte
	Eor8 SREL

Op43M0:
	StackRelative8 EOR2
	call S9xSA1GetWord
	Eor16 SREL

Op53M1:
	StackRelativeIndirectIndexed8 EOR
	call S9xSA1GetByte
	Eor8 SRII

Op53M0:
	StackRelativeIndirectIndexed8 EOR2
	call S9xSA1GetWord
	Eor16 SRII

/* INC */
Op1AM1:
	movb AL, %al
	incb %al
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop

Op1AM0:
	incw AA
	setnz SA1_Zero
	movb AH, %ah
	movb %ah, SA1_Negative
	jmp S9xSA1MainAsmLoop

OpE6M1:
	Direct8 INC
	Inc8 DIR

OpE6M0:
	Direct8 INC2
	Inc16 DIR
	
OpF6M1:
	DirectIndexedX8 INC
	Inc8 DIX

OpF6M0:
	DirectIndexedX8 INC2
	Inc16 DIX

OpEEM1:
	Absolute8 INC
	Inc8 ABS

OpEEM0:
	Absolute8 INC2
	Inc16 ABS

OpFEM1:
	AbsoluteIndexedX8 INC
	Inc8 ABSX

OpFEM0:
	AbsoluteIndexedX8 INC2
	Inc16 ABSX

/* LDA */
OpA9M1:
	Immediate8 LDA
	Lda8 IMM

OpA9M0:
	Immediate16 LDA
	Lda16 IMM

OpA5M1:
	Direct8 LDA
	call S9xSA1GetByte
	Lda8 DIR

OpA5M0:
	Direct8 LDA2
	call S9xSA1GetWord
	Lda16 DIR

OpB5M1:
	DirectIndexedX8 LDA
	call S9xSA1GetByte
	Lda8 DIX

OpB5M0:
	DirectIndexedX8 LDA2
	call S9xSA1GetWord
	Lda16 DIX

OpB2M1:
	DirectIndirect8 LDA
	call S9xSA1GetByte
	Lda8 DI

OpB2M0:
	DirectIndirect8 LDA2
	call S9xSA1GetWord
	Lda16 DI

OpA1M1:
	DirectIndexedIndirect8 LDA
	call S9xSA1GetByte
	Lda8 DII

OpA1M0:
	DirectIndexedIndirect8 LDA2
	call S9xSA1GetWord
	Lda16 DII

OpB1M1:
	DirectIndirectIndexed8 LDA
	call S9xSA1GetByte
	Lda8 DIIY

OpB1M0:
	DirectIndirectIndexed8 LDA2
	call S9xSA1GetWord
	Lda16 DIIY

OpA7M1:
	DirectIndirectLong8 LDA
	call S9xSA1GetByte
	Lda8 DIL

OpA7M0:
	DirectIndirectLong8 LDA2
	call S9xSA1GetWord
	Lda16 DIL

OpB7M1:
	DirectIndirectIndexedLong8 LDA
	call S9xSA1GetByte
	Lda8 DIIL

OpB7M0:
	DirectIndirectIndexedLong8 LDA2
	call S9xSA1GetWord
	Lda16 DIIL

OpADM1:
	Absolute8 LDA
	call S9xSA1GetByte
	Lda8 ABS

OpADM0:
	Absolute8 LDA2
	call S9xSA1GetWord
	Lda16 ABS

OpBDM1:
	AbsoluteIndexedX8 LDA
	call S9xSA1GetByte
	Lda8 ABSX

OpBDM0:
	AbsoluteIndexedX8 LDA2
	call S9xSA1GetWord
	Lda16 ABSX

OpB9M1:
	AbsoluteIndexedY8 LDA
	call S9xSA1GetByte
	Lda8 ABSY

OpB9M0:
	AbsoluteIndexedY8 LDA2
	call S9xSA1GetWord
	Lda16 ABSY

OpAFM1:
	AbsoluteLong8 LDA
	call S9xSA1GetByte
	Lda8 ABSL

OpAFM0:
	AbsoluteLong8 LDA2
	call S9xSA1GetWord
	Lda16 ABSL

OpBFM1:
	AbsoluteLongIndexedX8 LDA
	call S9xSA1GetByte
	Lda8 ALX

OpBFM0:
	AbsoluteLongIndexedX8 LDA2
	call S9xSA1GetWord
	Lda16 ALX

OpA3M1:
	StackRelative8 LDA
	call S9xSA1GetByte
	Lda8 SREL

OpA3M0:
	StackRelative8 LDA2
	call S9xSA1GetWord
	Lda16 SREL

OpB3M1:
	StackRelativeIndirectIndexed8 LDA
	call S9xSA1GetByte
	Lda8 SRII

OpB3M0:
	StackRelativeIndirectIndexed8 LDA2
	call S9xSA1GetWord
	Lda16 SRII

/* LDX */
OpA2X1:
	Immediate8 LDX
	Ldx8 IMM

OpA2X0:
	Immediate16 LDX
	Ldx16 IMM

OpA6X1:
	Direct8 LDX
	call S9xSA1GetByte
	Ldx8 DIR

OpA6X0:
	Direct8 LDX2
	call S9xSA1GetWord
	Ldx16 DIR

OpB6X1:
	DirectIndexedY8 LDX
	call S9xSA1GetByte
	Ldx8 DIY

OpB6X0:
	DirectIndexedY8 LDX2
	call S9xSA1GetWord
	Ldx16 DIY

OpAEX1:
	Absolute8 LDX
	call S9xSA1GetByte
	Ldx8 ABS

OpAEX0:
	Absolute8 LDX2
	call S9xSA1GetWord
	Ldx16 ABS

OpBEX1:
	AbsoluteIndexedY8 LDX
	call S9xSA1GetByte
	Ldx8 ABSY

OpBEX0:
	AbsoluteIndexedY8 LDX2
	call S9xSA1GetWord
	Ldx16 ABSY

/* LDY */
OpA0X1:
	Immediate8 LDY
	Ldy8 IMM

OpA0X0:
	Immediate16 LDY
	Ldy16 IMM

OpA4X1:
	Direct8 LDY
	call S9xSA1GetByte
	Ldy8 DIR

OpA4X0:
	Direct8 LDY2
	call S9xSA1GetWord
	Ldy16 DIR

OpB4X1:
	DirectIndexedX8 LDY
	call S9xSA1GetByte
	Ldy8 DIX

OpB4X0:
	DirectIndexedX8 LDY2
	call S9xSA1GetWord
	Ldy16 DIX

OpACX1:
	Absolute8 LDY
	call S9xSA1GetByte
	Ldy8 ABS

OpACX0:
	Absolute8 LDY2
	call S9xSA1GetWord
	Ldy16 ABS

OpBCX1:
	AbsoluteIndexedX8 LDY
	call S9xSA1GetByte
	Ldy8 ABSX

OpBCX0:
	AbsoluteIndexedX8 LDY2
	call S9xSA1GetWord
	Ldy16 ABSX

/* LSR */
Op4AM1:
	movb AL, %al
	shrb %al
	movb %al, AL
	SetZNC
	jmp S9xSA1MainAsmLoop

Op4AM0:
	shrw AA
	setnz SA1_Zero
	setc SA1_Carry
	movb AH, %ah
	movb %ah, SA1_Negative
	jmp S9xSA1MainAsmLoop

Op46M1:
	Direct8 LSR
	Lsr8 DIR

Op46M0:
	Direct8 LSR2
	Lsr16 DIR

Op56M1:
	DirectIndexedX8 LSR
	Lsr8 DIX

Op56M0:
	DirectIndexedX8 LSR2
	Lsr16 DIX

Op4EM1:
	Absolute8 LSR
	Lsr8 ABS

Op4EM0:
	Absolute8 LSR2
	Lsr16 ABS

Op5EM1:
	AbsoluteIndexedX8 LSR
	Lsr8 ABSX

Op5EM0:
	AbsoluteIndexedX8 LSR2
	Lsr16 ABSX

/* ORA */
Op09M1:
	Immediate8 ORA
	Ora8 IMM

Op09M0:
	Immediate16 ORA
	Ora16 IMM

Op05M1:
	Direct8 ORA
	call S9xSA1GetByte
	Ora8 DIR

Op05M0:
	Direct8 ORA2
	call S9xSA1GetWord
	Ora16 DIR

Op15M1:
	DirectIndexedX8 ORA
	call S9xSA1GetByte
	Ora8 DIX

Op15M0:
	DirectIndexedX8 ORA2
	call S9xSA1GetWord
	Ora16 DIX

Op12M1:
	DirectIndirect8 ORA
	call S9xSA1GetByte
	Ora8 DI

Op12M0:
	DirectIndirect8 ORA2
	call S9xSA1GetWord
	Ora16 DI

Op01M1:
	DirectIndexedIndirect8 ORA
	call S9xSA1GetByte
	Ora8 DII

Op01M0:
	DirectIndexedIndirect8 ORA2
	call S9xSA1GetWord
	Ora16 DII

Op11M1:
	DirectIndirectIndexed8 ORA
	call S9xSA1GetByte
	Ora8 DIIY

Op11M0:
	DirectIndirectIndexed8 ORA2
	call S9xSA1GetWord
	Ora16 DIIY

Op07M1:
	DirectIndirectLong8 ORA
	call S9xSA1GetByte
	Ora8 DIL

Op07M0:
	DirectIndirectLong8 ORA2
	call S9xSA1GetWord
	Ora16 DIL

Op17M1:
	DirectIndirectIndexedLong8 ORA
	call S9xSA1GetByte
	Ora8 DIIL

Op17M0:
	DirectIndirectIndexedLong8 ORA2
	call S9xSA1GetWord
	Ora16 DIIL

Op0DM1:
	Absolute8 ORA
	call S9xSA1GetByte
	Ora8 ABS

Op0DM0:
	Absolute8 ORA2
	call S9xSA1GetWord
	Ora16 ABS

Op1DM1:
	AbsoluteIndexedX8 ORA
	call S9xSA1GetByte
	Ora8 ABSX

Op1DM0:
	AbsoluteIndexedX8 ORA2
	call S9xSA1GetWord
	Ora16 ABSX

Op19M1:
	AbsoluteIndexedY8 ORA
	call S9xSA1GetByte
	Ora8 ABSY

Op19M0:
	AbsoluteIndexedY8 ORA2
	call S9xSA1GetWord
	Ora16 ABSY

Op0FM1:
	AbsoluteLong8 ORA
	call S9xSA1GetByte
	Ora8 ABSL

Op0FM0:
	AbsoluteLong8 ORA2
	call S9xSA1GetWord
	Ora16 ABSL

Op1FM1:
	AbsoluteLongIndexedX8 ORA
	call S9xSA1GetByte
	Ora8 ALX

Op1FM0:
	AbsoluteLongIndexedX8 ORA2
	call S9xSA1GetWord
	Ora16 ALX

Op03M1:
	StackRelative8 ORA
	call S9xSA1GetByte
	Ora8 SREL

Op03M0:
	StackRelative8 ORA2
	call S9xSA1GetWord
	Ora16 SREL

Op13M1:
	StackRelativeIndirectIndexed8 ORA
	call S9xSA1GetByte
	Ora8 SRII

Op13M0:
	StackRelativeIndirectIndexed8 ORA2
	call S9xSA1GetWord
	Ora16 SRII

/* ROL */
Op2AM1:
	movb AL, %al
	GetCarry
	rclb %al
	movb %al, AL
	SetZNC
	jmp S9xSA1MainAsmLoop

Op2AM0:
	GetCarry
	movw AA, %ax
	rclw %ax
	movw %ax, AA
	setc SA1_Carry
	movb %ah, SA1_Negative
	orb %ah, %al
	movb %al, SA1_Zero
	jmp S9xSA1MainAsmLoop

Op26M1:
	Direct8 ROL
	Rol8 DIR

Op26M0:
	Direct8 ROL2
	Rol16 DIR

Op36M1:
	DirectIndexedX8 ROL
	Rol8 DIX

Op36M0:
	DirectIndexedX8 ROL2
	Rol16 DIX

Op2EM1:
	Absolute8 ROL
	Rol8 ABS

Op2EM0:
	Absolute8 ROL2
	Rol16 ABS

Op3EM1:
	AbsoluteIndexedX8 ROL
	Rol8 ABSX

Op3EM0:
	AbsoluteIndexedX8 ROL2
	Rol16 ABSX

/* ROR */
Op6AM1:
	movb AL, %al
	GetCarry
	rcrb %al
	movb %al, AL
	SetZNC
	jmp S9xSA1MainAsmLoop

Op6AM0:
	GetCarry
	movw AA, %ax
	rcrw %ax
	movw %ax, AA
	setc SA1_Carry
	movb %ah, SA1_Negative
	orb %ah, %al
	movb %al, SA1_Zero
	jmp S9xSA1MainAsmLoop
	
Op66M1:
	Direct8 ROR
	Ror8 DIR

Op66M0:
	Direct8 ROR2
	Ror16 DIR

Op76M1:
	DirectIndexedX8 ROR
	Ror8 DIX

Op76M0:
	DirectIndexedX8 ROR2
	Ror16 DIX

Op6EM1:
	Absolute8 ROR
	Ror8 ABS

Op6EM0:
	Absolute8 ROR2
	Ror16 ABS

Op7EM1:
	AbsoluteIndexedX8 ROR
	Ror8 ABSX

Op7EM0:
	AbsoluteIndexedX8 ROR2
	Ror16 ABSX

/* SBC */
OpE9M1:
	Immediate8 SBC
	Sbc8 IMM

OpE9M0:
	Immediate16 SBC
	Sbc16 IMM

OpE5M1:
	Direct8 SBC
	call S9xSA1GetByte
	Sbc8 DIR

OpE5M0:
	Direct8 SBC2
	call S9xSA1GetWord
	Sbc16 DIR

OpF5M1:
	DirectIndexedX8 SBC
	call S9xSA1GetByte
	Sbc8 DIX

OpF5M0:
	DirectIndexedX8 SBC2
	call S9xSA1GetWord
	Sbc16 DIX

OpF2M1:
	DirectIndirect8 SBC
	call S9xSA1GetByte
	Sbc8 DI

OpF2M0:
	DirectIndirect8 SBC2
	call S9xSA1GetWord
	Sbc16 DI

OpE1M1:
	DirectIndexedIndirect8 SBC
	call S9xSA1GetByte
	Sbc8 DII

OpE1M0:
	DirectIndexedIndirect8 SBC2
	call S9xSA1GetWord
	Sbc16 DII

OpF1M1:
	DirectIndirectIndexed8 SBC
	call S9xSA1GetByte
	Sbc8 DIIY

OpF1M0:
	DirectIndirectIndexed8 SBC2
	call S9xSA1GetWord
	Sbc16 DIIY

OpE7M1:
	DirectIndirectLong8 SBC
	call S9xSA1GetByte
	Sbc8 DIL

OpE7M0:
	DirectIndirectLong8 SBC2
	call S9xSA1GetWord
	Sbc16 DIL

OpF7M1:
	DirectIndirectIndexedLong8 SBC
	call S9xSA1GetByte
	Sbc8 DIIL

OpF7M0:
	DirectIndirectIndexedLong8 SBC2
	call S9xSA1GetWord
	Sbc16 DIIL

OpEDM1:
	Absolute8 SBC
	call S9xSA1GetByte
	Sbc8 ABS

OpEDM0:
	Absolute8 SBC2
	call S9xSA1GetWord
	Sbc16 ABS

OpFDM1:
	AbsoluteIndexedX8 SBC
	call S9xSA1GetByte
	Sbc8 ABSX

OpFDM0:
	AbsoluteIndexedX8 SBC2
	call S9xSA1GetWord
	Sbc16 ABSX

OpF9M1:
	AbsoluteIndexedY8 SBC
	call S9xSA1GetByte
	Sbc8 ABSY

OpF9M0:
	AbsoluteIndexedY8 SBC2
	call S9xSA1GetWord
	Sbc16 ABSY

OpEFM1:
	AbsoluteLong8 SBC
	call S9xSA1GetByte
	Sbc8 ABSL

OpEFM0:
	AbsoluteLong8 SBC2
	call S9xSA1GetWord
	Sbc16 ABSL

OpFFM1:
	AbsoluteLongIndexedX8 SBC
	call S9xSA1GetByte
	Sbc8 ALX

OpFFM0:
	AbsoluteLongIndexedX8 SBC2
	call S9xSA1GetWord
	Sbc16 ALX

OpE3M1:
	StackRelative8 SBC
	call S9xSA1GetByte
	Sbc8 SREL

OpE3M0:
	StackRelative8 SBC2
	call S9xSA1GetWord
	Sbc16 SREL

OpF3M1:
	StackRelativeIndirectIndexed8 SBC
	call S9xSA1GetByte
	Sbc8 SRII

OpF3M0:
	StackRelativeIndirectIndexed8 SBC2
	call S9xSA1GetWord
	Sbc16 SRII

/* STA */
Op85M1:
	Direct8 STA
	Sta8 DIR

Op85M0:
	Direct8 STA2
	Sta16 DIR

Op95M1:
	DirectIndexedX8 STA
	Sta8 DIX

Op95M0:
	DirectIndexedX8 STA2
	Sta16 DIX

Op92M1:
	DirectIndirect8 STA
	Sta8 DI

Op92M0:
	DirectIndirect8 STA2
	Sta16 DI

Op81M1:
	DirectIndexedIndirect8 STA
	Sta8 DII

Op81M0:
	DirectIndexedIndirect8 STA2
	Sta16 DII

Op91M1:
	DirectIndirectIndexed8 STA
	Sta8 DIIY

Op91M0:
	DirectIndirectIndexed8 STA2
	Sta16 DIIY

Op87M1:
	DirectIndirectLong8 STA
	Sta8 DIL

Op87M0:
	DirectIndirectLong8 STA2
	Sta16 DIL

Op97M1:
	DirectIndirectIndexedLong8 STA
	Sta8 DIIL

Op97M0:
	DirectIndirectIndexedLong8 STA2
	Sta16 DIIL

Op8DM1:
	Absolute8 STA
	Sta8 ABS

Op8DM0:
	Absolute8 STA
	Sta16 ABS

Op9DM1:
	AbsoluteIndexedX8 STA
	Sta8 ABSX

Op9DM0:
	AbsoluteIndexedX8 STA2
	Sta16 ABSX

Op99M1:
	AbsoluteIndexedY8 STA
	Sta8 ABSY

Op99M0:
	AbsoluteIndexedY8 STA2
	Sta16 ABSY

Op8FM1:
	AbsoluteLong8 STA
	Sta8 ABSL

Op8FM0:
	AbsoluteLong8 STA2
	Sta16 ABSL

Op9FM1:
	AbsoluteLongIndexedX8 STA
	Sta8 ALX

Op9FM0:
	AbsoluteLongIndexedX8 STA2
	Sta16 ALX

Op83M1:
	StackRelative8 STA
	Sta8 SREL

Op83M0:
	StackRelative8 STA2
	Sta16 SREL

Op93M1:
	StackRelativeIndirectIndexed8 STA
	Sta8 SRII

Op93M0:
	StackRelativeIndirectIndexed8 STA2
	Sta16 SRII

/* STX */
Op86X1:
	Direct8 STX
	Stx8 DIR

Op86X0:
	Direct8 STX2
	Stx16 DIR

Op96X1:
	DirectIndexedY8 STX
	Stx8 DIY

Op96X0:
	DirectIndexedY8 STX2
	Stx16 DIY

Op8EX1:
	Absolute8 STX
	Stx8 ABS

Op8EX0:
	Absolute8 STX2
	Stx16 ABS

/* STY */
Op84X1:
	Direct8 STY
	Sty8 DIR

Op84X0:
	Direct8 STY2
	Sty16 DIR

Op94X1:
	DirectIndexedX8 STY
	Sty8 DIX

Op94X0:
	DirectIndexedX8 STY2
	Sty16 DIX

Op8CX1:
	Absolute8 STY
	Sty8 ABS

Op8CX0:
	Absolute8 STY2
	Sty16 ABS

/* STZ */
Op64M1:
	Direct8 STZ
	Stz8 DIR

Op64M0:
	Direct8 STZ2
	Stz16 DIR

Op74M1:
	DirectIndexedX8 STZ
	Stz8 DIX

Op74M0:
	DirectIndexedX8 STZ2
	Stz16 DIX

Op9CM1:
	Absolute8 STZ
	Stz8 ABS

Op9CM0:
	Absolute8 STZ2
	Stz16 ABS

Op9EM1:
	AbsoluteIndexedX8 STZ
	Stz8 ABSX

Op9EM0:
	AbsoluteIndexedX8 STZ2
	Stz16 ABSX

/* TRB */
Op14M1:
	Direct8 TRB
	Trb8 DIR

Op14M0:
	Direct8 TRB2
	Trb16 DIR

Op1CM1:
	Absolute8 TRB
	Trb8 ABS

Op1CM0:
	Absolute8 TRB2
	Trb16 ABS

/* TSB */
Op04M1:
	Direct8 TSB
	Tsb8 DIR

Op04M0:
	Direct8 TSB2
	Tsb16 DIR

Op0CM1:
	Absolute8 TSB
	Tsb8 ABS

Op0CM0:
	Absolute8 TSB2
	Tsb16 ABS

/* BCC */
Op90:
	Relative
	testb $0xff, SA1_Carry
	jnz .BCC_EXIT
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
.BCC_EXIT:
	jmp S9xSA1MainAsmLoop

/* BCS */
OpB0:
	Relative
	testb $0xff, SA1_Carry
	jz .BCS_EXIT
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
.BCS_EXIT:
	jmp S9xSA1MainAsmLoop

/* BEQ */
OpF0:
	Relative
	testb $0xff, SA1_Zero
	jnz .BEQ_EXIT
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
	CPUShutdown BEQ
.BEQ_EXIT:
	jmp S9xSA1MainAsmLoop

/* BMI */
Op30:
	Relative
	testb $0x80, SA1_Negative
	jz .BMI_EXIT
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
	CPUShutdown BMI
.BMI_EXIT:
	jmp S9xSA1MainAsmLoop

/* BNE */
OpD0:
	Relative
	testb $0xff, SA1_Zero
	jz .BNE_EXIT
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
	CPUShutdown BNE
.BNE_EXIT:
	jmp S9xSA1MainAsmLoop

/* BPL */
Op10:
	Relative
	testb $0x80, SA1_Negative
	jnz .BPL_EXIT
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
	CPUShutdown BPL
.BPL_EXIT:
	jmp S9xSA1MainAsmLoop

/* BRA */
Op80:
	Relative
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
	CPUShutdown BRA
	jmp S9xSA1MainAsmLoop

/* BVC */
Op50:
	Relative
	testb $0xff, SA1_Overflow
	jnz .BVC_EXIT
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
	CPUShutdown BVC
.BVC_EXIT:
	jmp S9xSA1MainAsmLoop

/* BVS */
Op70:
	Relative
	testb $0xff, SA1_Overflow
	jz .BVS_EXIT
	andl $0xffff, %edx
	addl SA1PCBase, %edx
	movl %edx, PC
	CPUShutdown BVS
.BVS_EXIT:
	jmp S9xSA1MainAsmLoop

/* BRL */
Op82:
	RelativeLong BRL
	andl $0xffff, %edx
	orl SA1ShiftedPB, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* CLC */
Op18:
	movb $0, SA1_Carry
	jmp S9xSA1MainAsmLoop

/* CLD */
OpD8:
	andb $~Decimal, FLAGS
	jmp S9xSA1MainAsmLoop

/* CLI */
Op58:
	andb $~IRQ, FLAGS
	cmpb $0, SA1IRQActive
	jz .CLI_EXIT
	/* XXX: test for Settings.DisableIRQ */
	call S9xSA1Opcode_IRQ
.CLI_EXIT:
	jmp S9xSA1MainAsmLoop

/* CLV */
OpB8:
	movb $0, SA1_Overflow
	jmp S9xSA1MainAsmLoop

/* DEX */
OpCAX1:
	movb XL, %al
	decb %al
	movb %al, XL
	SetZN
	jmp S9xSA1MainAsmLoop

OpCAX0:
	decw XX
	setnz SA1_Zero
	movb XH, %al
	movb %al, SA1_Negative
	jmp S9xSA1MainAsmLoop

/* DEY */
Op88X1:
	movb YL, %al
	decb %al
	movb %al, YL
	SetZN
	jmp S9xSA1MainAsmLoop

Op88X0:
	decw YY
	setnz SA1_Zero
	movb YH, %al
	movb %al, SA1_Negative
	jmp S9xSA1MainAsmLoop

/* INX */
OpE8X1:
	movb XL, %al
	incb %al
	movb %al, XL
	SetZN
	jmp S9xSA1MainAsmLoop

OpE8X0:
	incw XX
	setnz SA1_Zero
	movb XH, %al
	movb %al, SA1_Negative
	jmp S9xSA1MainAsmLoop

/* INY */
OpC8X1:
	movb YL, %al
	incb %al
	movb %al, YL
	SetZN
	jmp S9xSA1MainAsmLoop

OpC8X0:
	incw YY
	setnz SA1_Zero
	movb YH, %al
	movb %al, SA1_Negative
	jmp S9xSA1MainAsmLoop

/* NOP */
OpEA:
	jmp S9xSA1MainAsmLoop

/* PEA */
OpF4E1:
	Immediate16 PEA
	PushWordENew PEA
	jmp S9xSA1MainAsmLoop

OpF4:
	Immediate16 PEA
	PushWord PEA
	jmp S9xSA1MainAsmLoop

/* PEI */
OpD4E1:
	DirectIndirect8 PEI
	movl %edx, %eax
	PushWordENew PEI
	jmp S9xSA1MainAsmLoop

OpD4:
	DirectIndirect8 PEI
	movl %edx, %eax
	PushWord PEI
	jmp S9xSA1MainAsmLoop

/* PER */
Op62E1:
	RelativeLong PER
	movl %edx, %eax
	PushWordENew PER
	jmp S9xSA1MainAsmLoop	

Op62:
	RelativeLong PER
	movl %edx, %eax
	PushWord PER
	jmp S9xSA1MainAsmLoop	

/* PHA */
Op48E1:
	movb AL, %al
	PushByteE PHA
	jmp S9xSA1MainAsmLoop

Op48M1:
	movb AL, %al
	PushByte PHA
	jmp S9xSA1MainAsmLoop

Op48M0:
	movw AA, %ax
	PushWord PHA
	jmp S9xSA1MainAsmLoop

/* PHB */
Op8BE1:
	movb DB, %al
	PushByteE PHB
	jmp S9xSA1MainAsmLoop

Op8B:
	movb DB, %al
	PushByte PHB
	jmp S9xSA1MainAsmLoop

/* PHD */
Op0BE1:
	movw DD, %ax
	PushWordENew PHD
	jmp S9xSA1MainAsmLoop

Op0B:
	movw DD, %ax
	PushWord PHD
	jmp S9xSA1MainAsmLoop

/* PHK */
Op4BE1:
	movb PB, %al
	PushByteE PHK
	jmp S9xSA1MainAsmLoop

Op4B:
	movb PB, %al
	PushByte PHK
	jmp S9xSA1MainAsmLoop

/* PHP */
Op08E1:
	S9xSA1PackStatus PHP
	movb FLAGS, %al
	PushByteE PHP
	jmp S9xSA1MainAsmLoop

Op08:
	S9xSA1PackStatus PHP
	movb FLAGS, %al
	PushByte PHP
	jmp S9xSA1MainAsmLoop

/* PHX */
OpDAE1:
	movb XL, %al
	PushByteE PHX
	jmp S9xSA1MainAsmLoop

OpDAX1:
	movb XL, %al
	PushByte PHX
	jmp S9xSA1MainAsmLoop

OpDAX0:
	movw XX, %ax
	PushWord PHX
	jmp S9xSA1MainAsmLoop

/* PHY */
Op5AE1:
	movb YL, %al
	PushByteE PHY
	jmp S9xSA1MainAsmLoop

Op5AX1:
	movb YL, %al
	PushByte PHY
	jmp S9xSA1MainAsmLoop

Op5AX0:
	movw YY, %ax
	PushWord PHY
	jmp S9xSA1MainAsmLoop

/* PLA */
Op68E1:
	PullByteE PLA
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop

Op68M1:
	PullByte PLA
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop

Op68M0:
	PullWord PLA
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* PLB */
OpABE1:
	PullByteE PLB
	movb %al, DB
	SetZN
	andl $0xff, %eax
	sall $16, %eax
	movl %eax, SA1ShiftedDB
	jmp S9xSA1MainAsmLoop

OpAB:
	PullByte PLB
	movb %al, DB
	SetZN
	andl $0xff, %eax
	sall $16, %eax
	movl %eax, SA1ShiftedDB
	jmp S9xSA1MainAsmLoop

/* PLD */
Op2BE1:
	PullWordENew PLD
	movw %ax, DD
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

Op2B:
	PullWord PLD
	movw %ax, DD
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop
	
/* PLP */
Op28E1:
	PullByteE PLPE
	movb %al, FLAGS

	testb $IndexFlag, FLAGS
	jz .PLP16E
	xorb %al, %al
	movb %al, XH
	movb %al, YH
.PLP16E:
	S9xSA1UnpackStatus PLPE
	S9xSA1FixCycles PLPE
	CheckForIrq PLPE
	jmp S9xSA1MainAsmLoop

Op28:
	PullByte PLP
	movb %al, FLAGS

	testb $IndexFlag, FLAGS
	jz .PLP16
	xorb %al, %al
	movb %al, XH
	movb %al, YH
.PLP16:
	S9xSA1UnpackStatus PLP
	S9xSA1FixCycles PLP
	CheckForIrq PLP
	jmp S9xSA1MainAsmLoop

/* PLX */
OpFAE1:
	PullByteE PLX
	movb %al, XL
	SetZN
	jmp S9xSA1MainAsmLoop

OpFAX1:
	PullByte PLX
	movb %al, XL
	SetZN
	jmp S9xSA1MainAsmLoop

OpFAX0:
	PullWord PLX
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* PLY */
Op7AE1:
	PullByteE PLY
	movb %al, YL
	SetZN
	jmp S9xSA1MainAsmLoop
	
Op7AX1:
	PullByte PLY
	movb %al, YL
	SetZN
	jmp S9xSA1MainAsmLoop

Op7AX0:
	PullWord PLY
	movw %ax, YY
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* SEC */
Op38:
	movb $1, SA1_Carry
	jmp S9xSA1MainAsmLoop

/* SED */
OpF8:
	
	orb $Decimal, FLAGS
	jmp S9xSA1MainAsmLoop

/* SEI */
Op78:
	orb $IRQ, FLAGS
	jmp S9xSA1MainAsmLoop

/* TAX */
OpAAX1:
	movb AL, %al
	movb %al, XL
	SetZN
	jmp S9xSA1MainAsmLoop

OpAAX0:
	movw AA, %ax
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TAY */
OpA8X1:
	movb AL, %al
	movb %al, YL
	SetZN
	jmp S9xSA1MainAsmLoop

OpA8X0:
	movw AA, %ax
	movw %ax, YY
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TCD */
Op5B:
	movw AA, %ax
	movw %ax, DD
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TCS */
Op1B:
	movw AA, %ax
	movw %ax, SS
	testw $Emulation, FLAGS16
	jz .TCS_EXIT
	movb $1, SH
.TCS_EXIT:
	jmp S9xSA1MainAsmLoop

/* TDC */
Op7B:
	movw DD, %ax
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TSC */
Op3B:
	movw SS, %ax
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TSX */
OpBAX1:
	movb SL, %al
	movb %al, XL
	SetZN
	jmp S9xSA1MainAsmLoop

OpBAX0:
	movw SS, %ax
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TXA */
Op8AM1:
	movb XL, %al
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop

Op8AM0:
	movw XX, %ax
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TXS */
Op9A:
	movw XX, %ax
	movw %ax, SS
	testw $Emulation, FLAGS16
	jz .TXS_EXIT
	movb $1, SH
.TXS_EXIT:
	jmp S9xSA1MainAsmLoop

/* TXY */
Op9BX1:
	movb XL, %al
	movb %al, YL
	SetZN
	jmp S9xSA1MainAsmLoop

Op9BX0:
	movw XX, %ax
	movw %ax, YY
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TYA */
Op98M1:
	movb YL, %al
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop

Op98M0:
	movw YY, %ax
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* TYX */
OpBBX1:
	movb YL, %al
	movb %al, XL
	SetZN
	jmp S9xSA1MainAsmLoop

OpBBX0:
	movw YY, %ax
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop

/* XCE */
OpFB:
	movw FLAGS16, %ax
	andw $~(Emulation | Carry), FLAGS16
	GetCarry
	jnc .XCE_NO_CARRY
	orw $Emulation, FLAGS16
.XCE_NO_CARRY:
	testw $Emulation, %ax
	setnz SA1_Carry
	testw $Emulation, FLAGS16
	jz .XCE_NO_EMULATION2
	orw $(MemoryFlag | IndexFlag), FLAGS16
	movb $1, SH
.XCE_NO_EMULATION2:
	testw $IndexFlag, FLAGS16
	jz .XCE_NO_INDEX
	xorl %eax, %eax
	movb %al, XH
	movb %al, YH
.XCE_NO_INDEX:
	S9xSA1FixCycles XCE
	jmp S9xSA1MainAsmLoop
		
.data
.LC0:
	.string	"*** BRK"
.text

Op00:
#ifdef DEBUGGER
	testb $TRACE_FLAG, SA1Flags
	je .BRK_NO_TRACE
	pushl $.LC0
	ccall S9xTraceMessage
	addl $4,%esp
.BRK_NO_TRACE:
#endif
	testw $Emulation, FLAGS16
	jnz .BRK_EMULATION
	movb PB, %al
	PushByte BRK1
	movl PC, %eax
	subl SA1PCBase, %eax
	incl %eax
	PushWord BRK2
	
	S9xSA1PackStatus BRK
	movb FLAGS, %al
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte BRK3
	xorl %ecx, %ecx
	movl %ecx, SA1ShiftedPB
	movb %cl, PB
	movl $0xFFE6, %edx
	call S9xSA1GetWord
	movl %eax, %edx
	andl $0xffff, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop
.BRK_EMULATION:
	movl PC, %eax
	subl SA1PCBase, %eax
	incl %eax
	PushWord BRK2
	
	S9xSA1PackStatus BRK2
	movb FLAGS, %al
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte BRK3
	xorl %ecx, %ecx
	movl %ecx, SA1ShiftedPB
	movb %cl, PB
	movl $0xFFFE, %edx
	call S9xSA1GetWord
	movl %eax, %edx
	andl $0xffff, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

.data
.LC1:
	.string	"*** IRQ"
.text
.globl S9xSA1Opcode_IRQ

S9xSA1Opcode_IRQ:
#ifdef DEBUGGER
	testb $TRACE_FLAG, SA1Flags
	je .IRQ_NO_TRACE
	pushl $.LC1
	ccall S9xTraceMessage
	addl $4, %esp
.IRQ_NO_TRACE:
#endif
	testw $Emulation, FLAGS16
	jnz .IRQ_EMULATION
	movb PB, %al
	PushByte IRQ1
	movl PC, %eax
	subl SA1PCBase, %eax
	PushWord IRQ2
	
	S9xSA1PackStatus IRQ
	movb FLAGS, %al
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte IRQ3
	xorl %ecx, %ecx
	movl %ecx, SA1ShiftedPB
	movb %cl, PB
// XXX: FIXME: wrong vector
	xorl %edx, %edx
	movl FillRAM, %eax
	movw 0x2207(%eax), %dx
	jmp S9xSA1SetPCBase
.IRQ_EMULATION:
	movl PC, %eax
	subl SA1PCBase, %eax
	PushWord IRQ4
	
	S9xSA1PackStatus IRQ2
	movb FLAGS, %al
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte IRQ5
	xorl %ecx, %ecx
	movl %ecx, SA1ShiftedPB
	movb %cl, PB
// XXX: FIXME: wrong vector
	xorl %edx, %edx
	movl FillRAM, %eax
	movw 0x2207(%eax), %dx
	jmp S9xSA1SetPCBase

.data
.LC2:
	.string	"*** NMI"
.text
.globl S9xSA1Opcode_NMI

S9xSA1Opcode_NMI:
#ifdef DEBUGGER
	testb $TRACE_FLAG, SA1Flags
	je .NMI_NO_TRACE
	pushl $.LC2
	ccall S9xTraceMessage
	addl $4, %esp
.NMI_NO_TRACE:
#endif
	testw $Emulation, FLAGS16
	jnz .NMI_EMULATION
	movb PB, %al
	PushByte NMI1
	movl PC, %eax
	subl SA1PCBase, %eax
	PushWord NMI2
	
	S9xSA1PackStatus NMI
	movb FLAGS, %al
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte NMI3
	xorl %ecx, %ecx
	movl %ecx, SA1ShiftedPB
	movb %cl, PB
// XXX: FIXME: wrong vector
	xorl %edx, %edx
	movl FillRAM, %eax
	movw 0x2205(%eax), %dx
	jmp S9xSA1SetPCBase
.NMI_EMULATION:
	movl PC, %eax
	subl SA1PCBase, %eax
	PushWord NMI4
	
	S9xSA1PackStatus NMI2
	movb FLAGS, %al
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte NMI5
	xorl %ecx, %ecx
	movl %ecx, SA1ShiftedPB
	movb %cl, PB
// XXX: FIXME: wrong vector
	xorl %edx, %edx
	movl FillRAM, %eax
	movw 0x2205(%eax), %dx
	jmp S9xSA1SetPCBase

.data
.LC3:
	.string	"*** COP"
.text

Op02:
#ifdef DEBUGGER
	testb $TRACE_FLAG, SA1Flags
	je .COP_NO_TRACE
	pushl $.LC3
	ccall S9xTraceMessage
	addl $4,%esp
.COP_NO_TRACE:
#endif
	testw $Emulation, FLAGS16
	jnz .COP_EMULATION
	movb PB, %al
	PushByte COP1
	movl PC, %eax
	subl SA1PCBase, %eax
	incl %eax
	PushWord COP2
	
	S9xSA1PackStatus COP
	movb FLAGS, %al
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte COP3
	xorl %ecx, %ecx
	movl %ecx, SA1ShiftedPB
	movb %cl, PB
	movl $0xFFE4, %edx
	call S9xSA1GetWord
	movl %eax, %edx
	andl $0xffff, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop
.COP_EMULATION:
	movl PC, %eax
	subl SA1PCBase, %eax
	incl %eax
	PushWord COP4
	
	S9xSA1PackStatus COP2
	movb FLAGS, %al
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte COP5
	xorl %ecx, %ecx
	movl %ecx, SA1ShiftedPB
	movb %cl, PB
	movl $0xFFF4, %edx
	call S9xSA1GetWord
	movl %eax, %edx
	andl $0xffff, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* JML */
OpDC:
	AbsoluteIndirectLong8 JML
	movl %edx, %ecx
	andl $0xff0000, %ecx
	movl %ecx, SA1ShiftedPB
	sarl $16, %ecx
	movb %cl, PB
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

Op5C:
	AbsoluteLong8 JML
	movl %edx, %ecx
	andl $0xff0000, %ecx
	movl %ecx, SA1ShiftedPB
	sarl $16, %ecx
	movb %cl, PB
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* JMP */
Op4C:
	Absolute8 JMP
	andl $0xffff, %edx
	orl  SA1ShiftedPB, %edx
	call S9xSA1SetPCBase
	CPUShutdown JMP
	jmp S9xSA1MainAsmLoop

Op6C:
	AbsoluteIndirect8 JMP
	andl $0xffff, %edx
	orl  SA1ShiftedPB, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

Op7C:
	AbsoluteIndexedIndirect8 JMP
	andl $0xffff, %edx
	orl SA1ShiftedPB, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* JSL */
Op22E1:
	movb PB, %al
	PushByte JSL_ABSL_E
	movl PC, %eax
	subl SA1PCBase, %eax
	addl $2, %eax
	PushWordENew JSL_ABSL_E
	AbsoluteLong8 JSL
	movl %edx, %ecx
	andl $0xff0000, %ecx
	movl %ecx, SA1ShiftedPB
	sarl $16, %ecx
	movb %cl, PB
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

Op22:
	movb PB, %al
	PushByte JSL_ABSL
	movl PC, %eax
	subl SA1PCBase, %eax
	addl $2, %eax
	PushWord JSL_ABSL
	AbsoluteLong8 JSL
	movl %edx, %ecx
	andl $0xff0000, %ecx
	movl %ecx, SA1ShiftedPB
	sarl $16, %ecx
	movb %cl, PB
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* RTL */
Op6BE1:
	PullWord RTL_E
	pushl %eax
	PullByte RTL_E
	movl SS, %edx
	addl $0xFF, %edx
	movb $0x01, %dh
	movw %dx, SS
	popl %edx
	movb %al, PB
	incw %dx
	andl $0xff, %eax
	andl $0xffff, %edx
	sall $16, %eax
	movl %eax, SA1ShiftedPB
	orl %eax, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

Op6B:
	PullWord RTL
	pushl %eax
	PullByte RTL
	popl %edx
	movb %al, PB
	incw %dx
	andl $0xff, %eax
	andl $0xffff, %edx
	sall $16, %eax
	movl %eax, SA1ShiftedPB
	orl %eax, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* JSR ABS */
Op20:
	movl PC, %eax
	subl SA1PCBase, %eax
	incl %eax
	PushWord JSR_ABS
	Absolute8 JSR_ABS
	andl $0xffff, %edx
	orl  SA1ShiftedPB, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* JSR ABS INDEXED INDIRECT */
OpFCE1:
	movl PC, %eax
	subl SA1PCBase, %eax
	incl %eax
	PushWordENew JSR_AII_E
	AbsoluteIndexedIndirect8 JSR
	andl $0xffff, %edx
	orl  SA1ShiftedPB, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

OpFC:
	movl PC, %eax
	subl SA1PCBase, %eax
	incl %eax
	PushWord JSR_AII
	AbsoluteIndexedIndirect8 JSR
	andl $0xffff, %edx
	orl  SA1ShiftedPB, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* RTS */
Op60:
	PullWord RTS
	incl %eax
	movl %eax, %edx
	andl $0xffff, %edx
	orl  SA1ShiftedPB, %edx
	call S9xSA1SetPCBase
	jmp S9xSA1MainAsmLoop

/* MVN */
Op54X1:
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, DB
	sall $16, %edx
	movl %edx, SA1ShiftedDB
	movb 1(PC), %dl
	sall $16, %edx
	movw XX, %dx
	call S9xSA1GetByte
	movl SA1ShiftedDB, %edx
	movw YY, %dx
	call S9xSA1SetByte
	incb XL
	incb YL
	movw AA, %ax
	decw %ax
	movw %ax, AA
	cmpw $0xffff, %ax
	je .MVN_EXIT8
	decl PC
	jmp S9xSA1MainAsmLoop
.MVN_EXIT8:
	addl $2, PC
	jmp S9xSA1MainAsmLoop

Op54X0:
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, DB
	sall $16, %edx
	movl %edx, SA1ShiftedDB
	movb 1(PC), %dl
	sall $16, %edx
	movw XX, %dx
	call S9xSA1GetByte
	movl SA1ShiftedDB, %edx
	movw YY, %dx
	call S9xSA1SetByte
	incw XX
	incw YY
	movw AA, %ax
	decw %ax
	movw %ax, AA
	cmpw $0xffff, %ax
	je .MVN_EXIT16
	decl PC
	jmp S9xSA1MainAsmLoop
.MVN_EXIT16:
	addl $2, PC
	jmp S9xSA1MainAsmLoop

/* MVP */
Op44X1:
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, DB
	sall $16, %edx
	movl %edx, SA1ShiftedDB
	movb 1(PC), %dl
	sall $16, %edx
	movw XX, %dx
	call S9xSA1GetByte
	movl SA1ShiftedDB, %edx
	movw YY, %dx
	call S9xSA1SetByte
	decb XL
	decb YL
	movw AA, %ax
	decw %ax
	movw %ax, AA
	cmpw $0xffff, %ax
	je .MVP_EXIT8
	decl PC
	jmp S9xSA1MainAsmLoop
.MVP_EXIT8:
	addl $2, PC
	jmp S9xSA1MainAsmLoop

Op44X0:
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, DB
	sall $16, %edx
	movl %edx, SA1ShiftedDB
	movb 1(PC), %dl
	sall $16, %edx
	movw XX, %dx
	call S9xSA1GetByte
	movl SA1ShiftedDB, %edx
	movw YY, %dx
	call S9xSA1SetByte
	decw XX
	decw YY
	movw AA, %ax
	decw %ax
	movw %ax, AA
	cmpw $0xffff, %ax
	je .MVP_EXIT16
	decl PC
	jmp S9xSA1MainAsmLoop
.MVP_EXIT16:
	addl $2, PC
	jmp S9xSA1MainAsmLoop

/* REP */
OpC2:
	movb (PC), %al
	incl PC
	notb %al
	andb %al, FLAGS
	andb %al, SA1_Negative
	andb %al, SA1_Carry
	testb $Zero, %al
	setz %ah
	orb %ah, SA1_Zero
	shrb $6, %al
	andb %al, SA1_Overflow

	testw $Emulation, FLAGS16
	jz .REP_NO_EMU
	orb $(MemoryFlag | IndexFlag), FLAGS
	
.REP_NO_EMU:
	testb $IndexFlag, FLAGS
	jz .REP16
	xorb %al, %al
	movb %al, XH
	movb %al, YH
.REP16:
	S9xSA1FixCycles REP
	CheckForIrq REP
	jmp S9xSA1MainAsmLoop

/* SEP */
OpE2:
	movb (PC), %al
	incl PC
	orb %al, FLAGS
	orb %al, SA1_Negative
	movb %al, %ah
	shrb $6, %ah
	andb $1, %ah
	orb %ah, SA1_Overflow
	test $Zero, %al
	jz .SEP_NO_ZERO
	movb $0, SA1_Zero
.SEP_NO_ZERO:
	andb $1, %al
	orb %al, SA1_Carry
	testw $Emulation, FLAGS16
	jz .SEP_NO_EMU
	orb $(MemoryFlag | IndexFlag), FLAGS
	
.SEP_NO_EMU:
	testb $IndexFlag, FLAGS
	jz .SEP16
	xorb %al, %al
	movb %al, XH
	movb %al, YH
.SEP16:
	S9xSA1FixCycles SEP
	jmp S9xSA1MainAsmLoop

/* XBA */
OpEB:
	movb AL, %ah
	movb AH, %al
	movb %ah, AH
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop

/* RTI */
Op40:
	PullByte RTI
	movb %al, FLAGS
	PullWord RTI
	andl $0xffff, %eax
	pushl %eax
	testw $Emulation, FLAGS16
	jnz .RTI_EMU
	PullByte RTI2
	andl $0xff, %eax
	jmp .RTI_SKIP_EMU
.RTI_EMU:
	orb $(MemoryFlag | IndexFlag), FLAGS
	/*XXX: HERE xorl %eax, %eax*/
	movl SA1ShiftedPB, %eax
	sarl $16, %eax
.RTI_SKIP_EMU:
	movb %al, PB
	popl %edx
	sall $16, %eax
	orl %eax, %edx
	movl %eax, SA1ShiftedPB
	call S9xSA1SetPCBase
	testb $IndexFlag, FLAGS
	jz .RTI16
	xorb %al, %al
	movb %al, XH
	movb %al, YH
.RTI16:
	S9xSA1UnpackStatus RTI
	S9xSA1FixCycles RTI
	CheckForIrq RTI
	jmp S9xSA1MainAsmLoop

/* WAI */
OpCB:
	movb $1, SA1WaitingForInterrupt
	decl PC
#if 0
// XXX: FIXME
	movb Shutdown, %al
	testb %al, %al
	je .NoShutdown
	movl NextEvent, CYCLES
	cmpb $0, APUExecuting
	je S9xSA1MainAsmLoop
	movb $0, CPUExecuting
.WAITExecAPU:
#ifdef DEBUGGER
	testb $2,APUFlags
	je .WAITNoAPUS9xSA1Trace
	STORE_REGISTERS
	ccall S9xSA1TraceAPU
	LOAD_REGISTERS
.WAITNoAPUS9xSA1Trace:
#endif
	xorl %eax,%eax
#ifdef SPC700_C
	movl APUPC,%edx
	SAVE_CYCLES
	movb (%edx),%al
#else
	movb (APUPC),%al
#endif
	movl S9xAPUCycles(,%eax,4), %edx
	movl S9xApuOpcodes(,%eax,4),%eax
	addl %edx, APUCycles
	call *%eax
#ifdef SPC700_C
        LOAD_CYCLES
#endif
	movl NextEvent, %eax
	cmpl %eax, APUCycles
	jl .WAITExecAPU
	movb $1, CPUExecuting
	jmp S9xSA1MainAsmLoop
#endif
.NoShutdown:
	jmp S9xSA1MainAsmLoop

/* ??? */
OpDB:
	decl PC
	orb $DEBUG_MODE_FLAG, SA1Flags
	jmp S9xSA1MainAsmLoop

Op42:
	jmp S9xSA1MainAsmLoop

.globl S9xSA1OpcodesM1X1
.data
	.align 4
S9xSA1OpcodesM1X1:
	.long Op00
	.long Op01M1
	.long Op02
	.long Op03M1
	.long Op04M1
	.long Op05M1
	.long Op06M1
	.long Op07M1
	.long Op08
	.long Op09M1
	.long Op0AM1
	.long Op0B
	.long Op0CM1
	.long Op0DM1
	.long Op0EM1
	.long Op0FM1
	.long Op10
	.long Op11M1
	.long Op12M1
	.long Op13M1
	.long Op14M1
	.long Op15M1
	.long Op16M1
	.long Op17M1
	.long Op18
	.long Op19M1
	.long Op1AM1
	.long Op1B
	.long Op1CM1
	.long Op1DM1
	.long Op1EM1
	.long Op1FM1
	.long Op20
	.long Op21M1
	.long Op22
	.long Op23M1
	.long Op24M1
	.long Op25M1
	.long Op26M1
	.long Op27M1
	.long Op28
	.long Op29M1
	.long Op2AM1
	.long Op2B
	.long Op2CM1
	.long Op2DM1
	.long Op2EM1
	.long Op2FM1
	.long Op30
	.long Op31M1
	.long Op32M1
	.long Op33M1
	.long Op34M1
	.long Op35M1
	.long Op36M1
	.long Op37M1
	.long Op38
	.long Op39M1
	.long Op3AM1
	.long Op3B
	.long Op3CM1
	.long Op3DM1
	.long Op3EM1
	.long Op3FM1
	.long Op40
	.long Op41M1
	.long Op42
	.long Op43M1
	.long Op44X1
	.long Op45M1
	.long Op46M1
	.long Op47M1
	.long Op48M1
	.long Op49M1
	.long Op4AM1
	.long Op4B
	.long Op4C
	.long Op4DM1
	.long Op4EM1
	.long Op4FM1
	.long Op50
	.long Op51M1
	.long Op52M1
	.long Op53M1
	.long Op54X1
	.long Op55M1
	.long Op56M1
	.long Op57M1
	.long Op58
	.long Op59M1
	.long Op5AX1
	.long Op5B
	.long Op5C
	.long Op5DM1
	.long Op5EM1
	.long Op5FM1
	.long Op60
	.long Op61M1
	.long Op62
	.long Op63M1
	.long Op64M1
	.long Op65M1
	.long Op66M1
	.long Op67M1
	.long Op68M1
	.long Op69M1
	.long Op6AM1
	.long Op6B
	.long Op6C
	.long Op6DM1
	.long Op6EM1
	.long Op6FM1
	.long Op70
	.long Op71M1
	.long Op72M1
	.long Op73M1
	.long Op74M1
	.long Op75M1
	.long Op76M1
	.long Op77M1
	.long Op78
	.long Op79M1
	.long Op7AX1
	.long Op7B
	.long Op7C
	.long Op7DM1
	.long Op7EM1
	.long Op7FM1
	.long Op80
	.long Op81M1
	.long Op82
	.long Op83M1
	.long Op84X1
	.long Op85M1
	.long Op86X1
	.long Op87M1
	.long Op88X1
	.long Op89M1
	.long Op8AM1
	.long Op8B
	.long Op8CX1
	.long Op8DM1
	.long Op8EX1
	.long Op8FM1
	.long Op90
	.long Op91M1
	.long Op92M1
	.long Op93M1
	.long Op94X1
	.long Op95M1
	.long Op96X1
	.long Op97M1
	.long Op98M1
	.long Op99M1
	.long Op9A
	.long Op9BX1
	.long Op9CM1
	.long Op9DM1
	.long Op9EM1
	.long Op9FM1
	.long OpA0X1
	.long OpA1M1
	.long OpA2X1
	.long OpA3M1
	.long OpA4X1
	.long OpA5M1
	.long OpA6X1
	.long OpA7M1
	.long OpA8X1
	.long OpA9M1
	.long OpAAX1
	.long OpAB
	.long OpACX1
	.long OpADM1
	.long OpAEX1
	.long OpAFM1
	.long OpB0
	.long OpB1M1
	.long OpB2M1
	.long OpB3M1
	.long OpB4X1
	.long OpB5M1
	.long OpB6X1
	.long OpB7M1
	.long OpB8
	.long OpB9M1
	.long OpBAX1
	.long OpBBX1
	.long OpBCX1
	.long OpBDM1
	.long OpBEX1
	.long OpBFM1
	.long OpC0X1
	.long OpC1M1
	.long OpC2
	.long OpC3M1
	.long OpC4X1
	.long OpC5M1
	.long OpC6M1
	.long OpC7M1
	.long OpC8X1
	.long OpC9M1
	.long OpCAX1
	.long OpCB
	.long OpCCX1
	.long OpCDM1
	.long OpCEM1
	.long OpCFM1
	.long OpD0
	.long OpD1M1
	.long OpD2M1
	.long OpD3M1
	.long OpD4
	.long OpD5M1
	.long OpD6M1
	.long OpD7M1
	.long OpD8
	.long OpD9M1
	.long OpDAX1
	.long OpDB
	.long OpDC
	.long OpDDM1
	.long OpDEM1
	.long OpDFM1
	.long OpE0X1
	.long OpE1M1
	.long OpE2
	.long OpE3M1
	.long OpE4X1
	.long OpE5M1
	.long OpE6M1
	.long OpE7M1
	.long OpE8X1
	.long OpE9M1
	.long OpEA
	.long OpEB
	.long OpECX1
	.long OpEDM1
	.long OpEEM1
	.long OpEFM1
	.long OpF0
	.long OpF1M1
	.long OpF2M1
	.long OpF3M1
	.long OpF4
	.long OpF5M1
	.long OpF6M1
	.long OpF7M1
	.long OpF8
	.long OpF9M1
	.long OpFAX1
	.long OpFB
	.long OpFC
	.long OpFDM1
	.long OpFEM1
	.long OpFFM1

.globl S9xSA1OpcodesE1
.data
	.align 4
S9xSA1OpcodesE1:
	.long Op00
	.long Op01M1
	.long Op02
	.long Op03M1
	.long Op04M1
	.long Op05M1
	.long Op06M1
	.long Op07M1
	.long Op08E1
	.long Op09M1
	.long Op0AM1
	.long Op0BE1
	.long Op0CM1
	.long Op0DM1
	.long Op0EM1
	.long Op0FM1
	.long Op10
	.long Op11M1
	.long Op12M1
	.long Op13M1
	.long Op14M1
	.long Op15M1
	.long Op16M1
	.long Op17M1
	.long Op18
	.long Op19M1
	.long Op1AM1
	.long Op1B
	.long Op1CM1
	.long Op1DM1
	.long Op1EM1
	.long Op1FM1
	.long Op20
	.long Op21M1
	.long Op22E1
	.long Op23M1
	.long Op24M1
	.long Op25M1
	.long Op26M1
	.long Op27M1
	.long Op28E1
	.long Op29M1
	.long Op2AM1
	.long Op2BE1
	.long Op2CM1
	.long Op2DM1
	.long Op2EM1
	.long Op2FM1
	.long Op30
	.long Op31M1
	.long Op32M1
	.long Op33M1
	.long Op34M1
	.long Op35M1
	.long Op36M1
	.long Op37M1
	.long Op38
	.long Op39M1
	.long Op3AM1
	.long Op3B
	.long Op3CM1
	.long Op3DM1
	.long Op3EM1
	.long Op3FM1
	.long Op40
	.long Op41M1
	.long Op42
	.long Op43M1
	.long Op44X1
	.long Op45M1
	.long Op46M1
	.long Op47M1
	.long Op48E1
	.long Op49M1
	.long Op4AM1
	.long Op4BE1
	.long Op4C
	.long Op4DM1
	.long Op4EM1
	.long Op4FM1
	.long Op50
	.long Op51M1
	.long Op52M1
	.long Op53M1
	.long Op54X1
	.long Op55M1
	.long Op56M1
	.long Op57M1
	.long Op58
	.long Op59M1
	.long Op5AE1
	.long Op5B
	.long Op5C
	.long Op5DM1
	.long Op5EM1
	.long Op5FM1
	.long Op60
	.long Op61M1
	.long Op62E1
	.long Op63M1
	.long Op64M1
	.long Op65M1
	.long Op66M1
	.long Op67M1
	.long Op68E1
	.long Op69M1
	.long Op6AM1
	.long Op6BE1
	.long Op6C
	.long Op6DM1
	.long Op6EM1
	.long Op6FM1
	.long Op70
	.long Op71M1
	.long Op72M1
	.long Op73M1
	.long Op74M1
	.long Op75M1
	.long Op76M1
	.long Op77M1
	.long Op78
	.long Op79M1
	.long Op7AE1
	.long Op7B
	.long Op7C
	.long Op7DM1
	.long Op7EM1
	.long Op7FM1
	.long Op80
	.long Op81M1
	.long Op82
	.long Op83M1
	.long Op84X1
	.long Op85M1
	.long Op86X1
	.long Op87M1
	.long Op88X1
	.long Op89M1
	.long Op8AM1
	.long Op8BE1
	.long Op8CX1
	.long Op8DM1
	.long Op8EX1
	.long Op8FM1
	.long Op90
	.long Op91M1
	.long Op92M1
	.long Op93M1
	.long Op94X1
	.long Op95M1
	.long Op96X1
	.long Op97M1
	.long Op98M1
	.long Op99M1
	.long Op9A
	.long Op9BX1
	.long Op9CM1
	.long Op9DM1
	.long Op9EM1
	.long Op9FM1
	.long OpA0X1
	.long OpA1M1
	.long OpA2X1
	.long OpA3M1
	.long OpA4X1
	.long OpA5M1
	.long OpA6X1
	.long OpA7M1
	.long OpA8X1
	.long OpA9M1
	.long OpAAX1
	.long OpABE1
	.long OpACX1
	.long OpADM1
	.long OpAEX1
	.long OpAFM1
	.long OpB0
	.long OpB1M1
	.long OpB2M1
	.long OpB3M1
	.long OpB4X1
	.long OpB5M1
	.long OpB6X1
	.long OpB7M1
	.long OpB8
	.long OpB9M1
	.long OpBAX1
	.long OpBBX1
	.long OpBCX1
	.long OpBDM1
	.long OpBEX1
	.long OpBFM1
	.long OpC0X1
	.long OpC1M1
	.long OpC2
	.long OpC3M1
	.long OpC4X1
	.long OpC5M1
	.long OpC6M1
	.long OpC7M1
	.long OpC8X1
	.long OpC9M1
	.long OpCAX1
	.long OpCB
	.long OpCCX1
	.long OpCDM1
	.long OpCEM1
	.long OpCFM1
	.long OpD0
	.long OpD1M1
	.long OpD2M1
	.long OpD3M1
	.long OpD4E1
	.long OpD5M1
	.long OpD6M1
	.long OpD7M1
	.long OpD8
	.long OpD9M1
	.long OpDAE1
	.long OpDB
	.long OpDC
	.long OpDDM1
	.long OpDEM1
	.long OpDFM1
	.long OpE0X1
	.long OpE1M1
	.long OpE2
	.long OpE3M1
	.long OpE4X1
	.long OpE5M1
	.long OpE6M1
	.long OpE7M1
	.long OpE8X1
	.long OpE9M1
	.long OpEA
	.long OpEB
	.long OpECX1
	.long OpEDM1
	.long OpEEM1
	.long OpEFM1
	.long OpF0
	.long OpF1M1
	.long OpF2M1
	.long OpF3M1
	.long OpF4E1
	.long OpF5M1
	.long OpF6M1
	.long OpF7M1
	.long OpF8
	.long OpF9M1
	.long OpFAE1
	.long OpFB
	.long OpFCE1
	.long OpFDM1
	.long OpFEM1
	.long OpFFM1
	
	.globl S9xSA1OpcodesM1X0
	.align 4
S9xSA1OpcodesM1X0:
	.long Op00
	.long Op01M1
	.long Op02
	.long Op03M1
	.long Op04M1
	.long Op05M1
	.long Op06M1
	.long Op07M1
	.long Op08
	.long Op09M1
	.long Op0AM1
	.long Op0B
	.long Op0CM1
	.long Op0DM1
	.long Op0EM1
	.long Op0FM1
	.long Op10
	.long Op11M1
	.long Op12M1
	.long Op13M1
	.long Op14M1
	.long Op15M1
	.long Op16M1
	.long Op17M1
	.long Op18
	.long Op19M1
	.long Op1AM1
	.long Op1B
	.long Op1CM1
	.long Op1DM1
	.long Op1EM1
	.long Op1FM1
	.long Op20
	.long Op21M1
	.long Op22
	.long Op23M1
	.long Op24M1
	.long Op25M1
	.long Op26M1
	.long Op27M1
	.long Op28
	.long Op29M1
	.long Op2AM1
	.long Op2B
	.long Op2CM1
	.long Op2DM1
	.long Op2EM1
	.long Op2FM1
	.long Op30
	.long Op31M1
	.long Op32M1
	.long Op33M1
	.long Op34M1
	.long Op35M1
	.long Op36M1
	.long Op37M1
	.long Op38
	.long Op39M1
	.long Op3AM1
	.long Op3B
	.long Op3CM1
	.long Op3DM1
	.long Op3EM1
	.long Op3FM1
	.long Op40
	.long Op41M1
	.long Op42
	.long Op43M1
	.long Op44X0
	.long Op45M1
	.long Op46M1
	.long Op47M1
	.long Op48M1
	.long Op49M1
	.long Op4AM1
	.long Op4B
	.long Op4C
	.long Op4DM1
	.long Op4EM1
	.long Op4FM1
	.long Op50
	.long Op51M1
	.long Op52M1
	.long Op53M1
	.long Op54X0
	.long Op55M1
	.long Op56M1
	.long Op57M1
	.long Op58
	.long Op59M1
	.long Op5AX0
	.long Op5B
	.long Op5C
	.long Op5DM1
	.long Op5EM1
	.long Op5FM1
	.long Op60
	.long Op61M1
	.long Op62
	.long Op63M1
	.long Op64M1
	.long Op65M1
	.long Op66M1
	.long Op67M1
	.long Op68M1
	.long Op69M1
	.long Op6AM1
	.long Op6B
	.long Op6C
	.long Op6DM1
	.long Op6EM1
	.long Op6FM1
	.long Op70
	.long Op71M1
	.long Op72M1
	.long Op73M1
	.long Op74M1
	.long Op75M1
	.long Op76M1
	.long Op77M1
	.long Op78
	.long Op79M1
	.long Op7AX0
	.long Op7B
	.long Op7C
	.long Op7DM1
	.long Op7EM1
	.long Op7FM1
	.long Op80
	.long Op81M1
	.long Op82
	.long Op83M1
	.long Op84X0
	.long Op85M1
	.long Op86X0
	.long Op87M1
	.long Op88X0
	.long Op89M1
	.long Op8AM1
	.long Op8B
	.long Op8CX0
	.long Op8DM1
	.long Op8EX0
	.long Op8FM1
	.long Op90
	.long Op91M1
	.long Op92M1
	.long Op93M1
	.long Op94X0
	.long Op95M1
	.long Op96X0
	.long Op97M1
	.long Op98M1
	.long Op99M1
	.long Op9A
	.long Op9BX0
	.long Op9CM1
	.long Op9DM1
	.long Op9EM1
	.long Op9FM1
	.long OpA0X0
	.long OpA1M1
	.long OpA2X0
	.long OpA3M1
	.long OpA4X0
	.long OpA5M1
	.long OpA6X0
	.long OpA7M1
	.long OpA8X0
	.long OpA9M1
	.long OpAAX0
	.long OpAB
	.long OpACX0
	.long OpADM1
	.long OpAEX0
	.long OpAFM1
	.long OpB0
	.long OpB1M1
	.long OpB2M1
	.long OpB3M1
	.long OpB4X0
	.long OpB5M1
	.long OpB6X0
	.long OpB7M1
	.long OpB8
	.long OpB9M1
	.long OpBAX0
	.long OpBBX0
	.long OpBCX0
	.long OpBDM1
	.long OpBEX0
	.long OpBFM1
	.long OpC0X0
	.long OpC1M1
	.long OpC2
	.long OpC3M1
	.long OpC4X0
	.long OpC5M1
	.long OpC6M1
	.long OpC7M1
	.long OpC8X0
	.long OpC9M1
	.long OpCAX0
	.long OpCB
	.long OpCCX0
	.long OpCDM1
	.long OpCEM1
	.long OpCFM1
	.long OpD0
	.long OpD1M1
	.long OpD2M1
	.long OpD3M1
	.long OpD4
	.long OpD5M1
	.long OpD6M1
	.long OpD7M1
	.long OpD8
	.long OpD9M1
	.long OpDAX0
	.long OpDB
	.long OpDC
	.long OpDDM1
	.long OpDEM1
	.long OpDFM1
	.long OpE0X0
	.long OpE1M1
	.long OpE2
	.long OpE3M1
	.long OpE4X0
	.long OpE5M1
	.long OpE6M1
	.long OpE7M1
	.long OpE8X0
	.long OpE9M1
	.long OpEA
	.long OpEB
	.long OpECX0
	.long OpEDM1
	.long OpEEM1
	.long OpEFM1
	.long OpF0
	.long OpF1M1
	.long OpF2M1
	.long OpF3M1
	.long OpF4
	.long OpF5M1
	.long OpF6M1
	.long OpF7M1
	.long OpF8
	.long OpF9M1
	.long OpFAX0
	.long OpFB
	.long OpFC
	.long OpFDM1
	.long OpFEM1
	.long OpFFM1

.globl S9xSA1OpcodesM0X0
	.align 4
S9xSA1OpcodesM0X0:
	.long Op00
	.long Op01M0
	.long Op02
	.long Op03M0
	.long Op04M0
	.long Op05M0
	.long Op06M0
	.long Op07M0
	.long Op08
	.long Op09M0
	.long Op0AM0
	.long Op0B
	.long Op0CM0
	.long Op0DM0
	.long Op0EM0
	.long Op0FM0
	.long Op10
	.long Op11M0
	.long Op12M0
	.long Op13M0
	.long Op14M0
	.long Op15M0
	.long Op16M0
	.long Op17M0
	.long Op18
	.long Op19M0
	.long Op1AM0
	.long Op1B
	.long Op1CM0
	.long Op1DM0
	.long Op1EM0
	.long Op1FM0
	.long Op20
	.long Op21M0
	.long Op22
	.long Op23M0
	.long Op24M0
	.long Op25M0
	.long Op26M0
	.long Op27M0
	.long Op28
	.long Op29M0
	.long Op2AM0
	.long Op2B
	.long Op2CM0
	.long Op2DM0
	.long Op2EM0
	.long Op2FM0
	.long Op30
	.long Op31M0
	.long Op32M0
	.long Op33M0
	.long Op34M0
	.long Op35M0
	.long Op36M0
	.long Op37M0
	.long Op38
	.long Op39M0
	.long Op3AM0
	.long Op3B
	.long Op3CM0
	.long Op3DM0
	.long Op3EM0
	.long Op3FM0
	.long Op40
	.long Op41M0
	.long Op42
	.long Op43M0
	.long Op44X0
	.long Op45M0
	.long Op46M0
	.long Op47M0
	.long Op48M0
	.long Op49M0
	.long Op4AM0
	.long Op4B
	.long Op4C
	.long Op4DM0
	.long Op4EM0
	.long Op4FM0
	.long Op50
	.long Op51M0
	.long Op52M0
	.long Op53M0
	.long Op54X0
	.long Op55M0
	.long Op56M0
	.long Op57M0
	.long Op58
	.long Op59M0
	.long Op5AX0
	.long Op5B
	.long Op5C
	.long Op5DM0
	.long Op5EM0
	.long Op5FM0
	.long Op60
	.long Op61M0
	.long Op62
	.long Op63M0
	.long Op64M0
	.long Op65M0
	.long Op66M0
	.long Op67M0
	.long Op68M0
	.long Op69M0
	.long Op6AM0
	.long Op6B
	.long Op6C
	.long Op6DM0
	.long Op6EM0
	.long Op6FM0
	.long Op70
	.long Op71M0
	.long Op72M0
	.long Op73M0
	.long Op74M0
	.long Op75M0
	.long Op76M0
	.long Op77M0
	.long Op78
	.long Op79M0
	.long Op7AX0
	.long Op7B
	.long Op7C
	.long Op7DM0
	.long Op7EM0
	.long Op7FM0
	.long Op80
	.long Op81M0
	.long Op82
	.long Op83M0
	.long Op84X0
	.long Op85M0
	.long Op86X0
	.long Op87M0
	.long Op88X0
	.long Op89M0
	.long Op8AM0
	.long Op8B
	.long Op8CX0
	.long Op8DM0
	.long Op8EX0
	.long Op8FM0
	.long Op90
	.long Op91M0
	.long Op92M0
	.long Op93M0
	.long Op94X0
	.long Op95M0
	.long Op96X0
	.long Op97M0
	.long Op98M0
	.long Op99M0
	.long Op9A
	.long Op9BX0
	.long Op9CM0
	.long Op9DM0
	.long Op9EM0
	.long Op9FM0
	.long OpA0X0
	.long OpA1M0
	.long OpA2X0
	.long OpA3M0
	.long OpA4X0
	.long OpA5M0
	.long OpA6X0
	.long OpA7M0
	.long OpA8X0
	.long OpA9M0
	.long OpAAX0
	.long OpAB
	.long OpACX0
	.long OpADM0
	.long OpAEX0
	.long OpAFM0
	.long OpB0
	.long OpB1M0
	.long OpB2M0
	.long OpB3M0
	.long OpB4X0
	.long OpB5M0
	.long OpB6X0
	.long OpB7M0
	.long OpB8
	.long OpB9M0
	.long OpBAX0
	.long OpBBX0
	.long OpBCX0
	.long OpBDM0
	.long OpBEX0
	.long OpBFM0
	.long OpC0X0
	.long OpC1M0
	.long OpC2
	.long OpC3M0
	.long OpC4X0
	.long OpC5M0
	.long OpC6M0
	.long OpC7M0
	.long OpC8X0
	.long OpC9M0
	.long OpCAX0
	.long OpCB
	.long OpCCX0
	.long OpCDM0
	.long OpCEM0
	.long OpCFM0
	.long OpD0
	.long OpD1M0
	.long OpD2M0
	.long OpD3M0
	.long OpD4
	.long OpD5M0
	.long OpD6M0
	.long OpD7M0
	.long OpD8
	.long OpD9M0
	.long OpDAX0
	.long OpDB
	.long OpDC
	.long OpDDM0
	.long OpDEM0
	.long OpDFM0
	.long OpE0X0
	.long OpE1M0
	.long OpE2
	.long OpE3M0
	.long OpE4X0
	.long OpE5M0
	.long OpE6M0
	.long OpE7M0
	.long OpE8X0
	.long OpE9M0
	.long OpEA
	.long OpEB
	.long OpECX0
	.long OpEDM0
	.long OpEEM0
	.long OpEFM0
	.long OpF0
	.long OpF1M0
	.long OpF2M0
	.long OpF3M0
	.long OpF4
	.long OpF5M0
	.long OpF6M0
	.long OpF7M0
	.long OpF8
	.long OpF9M0
	.long OpFAX0
	.long OpFB
	.long OpFC
	.long OpFDM0
	.long OpFEM0
	.long OpFFM0

.globl S9xSA1OpcodesM0X1
	.align 4
S9xSA1OpcodesM0X1:
	.long Op00
	.long Op01M0
	.long Op02
	.long Op03M0
	.long Op04M0
	.long Op05M0
	.long Op06M0
	.long Op07M0
	.long Op08
	.long Op09M0
	.long Op0AM0
	.long Op0B
	.long Op0CM0
	.long Op0DM0
	.long Op0EM0
	.long Op0FM0
	.long Op10
	.long Op11M0
	.long Op12M0
	.long Op13M0
	.long Op14M0
	.long Op15M0
	.long Op16M0
	.long Op17M0
	.long Op18
	.long Op19M0
	.long Op1AM0
	.long Op1B
	.long Op1CM0
	.long Op1DM0
	.long Op1EM0
	.long Op1FM0
	.long Op20
	.long Op21M0
	.long Op22
	.long Op23M0
	.long Op24M0
	.long Op25M0
	.long Op26M0
	.long Op27M0
	.long Op28
	.long Op29M0
	.long Op2AM0
	.long Op2B
	.long Op2CM0
	.long Op2DM0
	.long Op2EM0
	.long Op2FM0
	.long Op30
	.long Op31M0
	.long Op32M0
	.long Op33M0
	.long Op34M0
	.long Op35M0
	.long Op36M0
	.long Op37M0
	.long Op38
	.long Op39M0
	.long Op3AM0
	.long Op3B
	.long Op3CM0
	.long Op3DM0
	.long Op3EM0
	.long Op3FM0
	.long Op40
	.long Op41M0
	.long Op42
	.long Op43M0
	.long Op44X1
	.long Op45M0
	.long Op46M0
	.long Op47M0
	.long Op48M0
	.long Op49M0
	.long Op4AM0
	.long Op4B
	.long Op4C
	.long Op4DM0
	.long Op4EM0
	.long Op4FM0
	.long Op50
	.long Op51M0
	.long Op52M0
	.long Op53M0
	.long Op54X1
	.long Op55M0
	.long Op56M0
	.long Op57M0
	.long Op58
	.long Op59M0
	.long Op5AX1
	.long Op5B
	.long Op5C
	.long Op5DM0
	.long Op5EM0
	.long Op5FM0
	.long Op60
	.long Op61M0
	.long Op62
	.long Op63M0
	.long Op64M0
	.long Op65M0
	.long Op66M0
	.long Op67M0
	.long Op68M0
	.long Op69M0
	.long Op6AM0
	.long Op6B
	.long Op6C
	.long Op6DM0
	.long Op6EM0
	.long Op6FM0
	.long Op70
	.long Op71M0
	.long Op72M0
	.long Op73M0
	.long Op74M0
	.long Op75M0
	.long Op76M0
	.long Op77M0
	.long Op78
	.long Op79M0
	.long Op7AX1
	.long Op7B
	.long Op7C
	.long Op7DM0
	.long Op7EM0
	.long Op7FM0
	.long Op80
	.long Op81M0
	.long Op82
	.long Op83M0
	.long Op84X1
	.long Op85M0
	.long Op86X1
	.long Op87M0
	.long Op88X1
	.long Op89M0
	.long Op8AM0
	.long Op8B
	.long Op8CX1
	.long Op8DM0
	.long Op8EX1
	.long Op8FM0
	.long Op90
	.long Op91M0
	.long Op92M0
	.long Op93M0
	.long Op94X1
	.long Op95M0
	.long Op96X1
	.long Op97M0
	.long Op98M0
	.long Op99M0
	.long Op9A
	.long Op9BX1
	.long Op9CM0
	.long Op9DM0
	.long Op9EM0
	.long Op9FM0
	.long OpA0X1
	.long OpA1M0
	.long OpA2X1
	.long OpA3M0
	.long OpA4X1
	.long OpA5M0
	.long OpA6X1
	.long OpA7M0
	.long OpA8X1
	.long OpA9M0
	.long OpAAX1
	.long OpAB
	.long OpACX1
	.long OpADM0
	.long OpAEX1
	.long OpAFM0
	.long OpB0
	.long OpB1M0
	.long OpB2M0
	.long OpB3M0
	.long OpB4X1
	.long OpB5M0
	.long OpB6X1
	.long OpB7M0
	.long OpB8
	.long OpB9M0
	.long OpBAX1
	.long OpBBX1
	.long OpBCX1
	.long OpBDM0
	.long OpBEX1
	.long OpBFM0
	.long OpC0X1
	.long OpC1M0
	.long OpC2
	.long OpC3M0
	.long OpC4X1
	.long OpC5M0
	.long OpC6M0
	.long OpC7M0
	.long OpC8X1
	.long OpC9M0
	.long OpCAX1
	.long OpCB
	.long OpCCX1
	.long OpCDM0
	.long OpCEM0
	.long OpCFM0
	.long OpD0
	.long OpD1M0
	.long OpD2M0
	.long OpD3M0
	.long OpD4
	.long OpD5M0
	.long OpD6M0
	.long OpD7M0
	.long OpD8
	.long OpD9M0
	.long OpDAX1
	.long OpDB
	.long OpDC
	.long OpDDM0
	.long OpDEM0
	.long OpDFM0
	.long OpE0X1
	.long OpE1M0
	.long OpE2
	.long OpE3M0
	.long OpE4X1
	.long OpE5M0
	.long OpE6M0
	.long OpE7M0
	.long OpE8X1
	.long OpE9M0
	.long OpEA
	.long OpEB
	.long OpECX1
	.long OpEDM0
	.long OpEEM0
	.long OpEFM0
	.long OpF0
	.long OpF1M0
	.long OpF2M0
	.long OpF3M0
	.long OpF4
	.long OpF5M0
	.long OpF6M0
	.long OpF7M0
	.long OpF8
	.long OpF9M0
	.long OpFAX1
	.long OpFB
	.long OpFC
	.long OpFDM0
	.long OpFEM0
	.long OpFFM0
