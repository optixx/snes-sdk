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

#include "asmstruc.h"
#include "asmaddr.h"
#include "asmops.h"
#include "getset.S"

/* ADC */
Op69M1:
	Immediate8 ADC READ
	Adc8 IMM8

Op69M0:
	Immediate16 ADC2 READ
	Adc16 IMM16

Op65M1:
	Direct8 ADC2 READ
	call S9xGetByte
	Adc8 DIR8

Op65M0:
	Direct8 ADC READ
	call S9xGetWord
	Adc16 DIR16
	
Op75M1:
	DirectIndexedX8 ADC READ
	call S9xGetByte
	Adc8 DIX8

Op75M0:
	DirectIndexedX8 ADC2 READ
	call S9xGetWord
	Adc16 DIX16

Op72M1:
	DirectIndirect8 ADC READ
	call S9xGetByte
	Adc8 DI8

Op72M0:
	DirectIndirect8 ADC2 READ
	call S9xGetWord
	Adc16 DI16

Op61M1:
	DirectIndexedIndirect8 ADC READ
	call S9xGetByte
	Adc8 DII8

Op61M0:
	DirectIndexedIndirect8 ADC2 READ
	call S9xGetWord
	Adc16 DII16

Op71M1:
	DirectIndirectIndexed8 ADC READ
	call S9xGetByte
	Adc8 DIIY8

Op71M0:
	DirectIndirectIndexed8 ADC2 READ
	call S9xGetWord
	Adc16 DIIY16

Op67M1:
	DirectIndirectLong8 ADC READ
	call S9xGetByte
	Adc8 DIL8

Op67M0:
	DirectIndirectLong8 ADC2 READ
	call S9xGetWord
	Adc16 DIL16

Op77M1:
	DirectIndirectIndexedLong8 ADC READ
	call S9xGetByte
	Adc8 DIIL8

Op77M0:
	DirectIndirectIndexedLong8 ADC2 READ
	call S9xGetWord
	Adc16 DIIL8

Op6DM1:
	Absolute8 ADC READ
	call S9xGetByte
	Adc8 ABS8

Op6DM0:
	Absolute8 ADC2 READ
	call S9xGetWord
	Adc16 ABS16

Op7DM1:
	AbsoluteIndexedX8 ADC READ
	call S9xGetByte
	Adc8 ABSX8

Op7DM0:
	AbsoluteIndexedX8 ADC2 READ
	call S9xGetWord
	Adc16 ABSX16

Op79M1:
	AbsoluteIndexedY8 ADC READ
	call S9xGetByte
	Adc8 ABSY8

Op79M0:
	AbsoluteIndexedY8 ADC2 READ
	call S9xGetWord
	Adc16 ABSY16

Op6FM1:
	AbsoluteLong8 ADC READ
	call S9xGetByte
	Adc8 ABSL8

Op6FM0:
	AbsoluteLong8 ADC2 READ
	call S9xGetWord
	Adc16 ABSL16

Op7FM1:
	AbsoluteLongIndexedX8 ADC READ
	call S9xGetByte
	Adc8 ALX8

Op7FM0:
	AbsoluteLongIndexedX8 ADC2 READ
	call S9xGetWord
	Adc16 ALX16

Op63M1:
	StackRelative8 ADC READ
	call S9xGetByte
	Adc8 SREL8

Op63M0:
	StackRelative8 ADC2 READ
	call S9xGetWord
	Adc16 SREL16

Op73M1:
	StackRelativeIndirectIndexed8 ADC READ
	call S9xGetByte
	Adc8 SRII8

Op73M0:
	StackRelativeIndirectIndexed8 ADC2 READ
	call S9xGetWord
	Adc16 SRII16

/* AND */
Op29M1:
	Immediate8 AND READ
	And8 IMM8

Op29M0:
	Immediate16 AND READ
	And16 IMM16

Op25M1:
	Direct8 AND READ
	call S9xGetByte
	And8 DIR8

Op25M0:
	Direct8 AND2 READ
	call S9xGetWord
	And16 DIR16

Op35M1:
	DirectIndexedX8 AND READ
	call S9xGetByte
	And8 DIX8

Op35M0:
	DirectIndexedX8 AND2 READ
	call S9xGetWord
	And16 DIX16

Op32M1:
	DirectIndirect8 AND READ
	call S9xGetByte
	And8 DI8

Op32M0:
	DirectIndirect8 AND2 READ
	call S9xGetWord
	And16 DI16

Op21M1:
	DirectIndexedIndirect8 AND READ
	call S9xGetByte
	And8 DII8

Op21M0:
	DirectIndexedIndirect8 AND2 READ
	call S9xGetWord
	And16 DII16

Op31M1:
	DirectIndirectIndexed8 AND READ
	call S9xGetByte
	And8 DIIY8

Op31M0:
	DirectIndirectIndexed8 AND2 READ
	call S9xGetWord
	And16 DIIY16

Op27M1:
	DirectIndirectLong8 AND READ
	call S9xGetByte
	And8 DIL8

Op27M0:
	DirectIndirectLong8 AND2 READ
	call S9xGetWord
	And16 DIL16

Op37M1:
	DirectIndirectIndexedLong8 AND READ
	call S9xGetByte
	And8 DIIL8

Op37M0:
	DirectIndirectIndexedLong8 AND2 READ
	call S9xGetWord
	And16 DIIL16

Op2DM1:
	Absolute8 AND READ
	call S9xGetByte
	And8 ABS8

Op2DM0:
	Absolute8 AND2 READ
	call S9xGetWord
	And16 ABS16

Op3DM1:
	AbsoluteIndexedX8 AND READ
	call S9xGetByte
	And8 ABSX8

Op3DM0:
	AbsoluteIndexedX8 AND2 READ
	call S9xGetWord
	And16 ABSX16

Op39M1:
	AbsoluteIndexedY8 AND READ
	call S9xGetByte
	And8 ABSY8

Op39M0:
	AbsoluteIndexedY8 AND2 READ
	call S9xGetWord
	And16 ABSY16

Op2FM1:
	AbsoluteLong8 AND READ
	call S9xGetByte
	And8 ABSL8

Op2FM0:
	AbsoluteLong8 AND2 READ
	call S9xGetWord
	And16 ABSL16

Op3FM1:
	AbsoluteLongIndexedX8 AND READ
	call S9xGetByte
	And8 ALX8

Op3FM0:
	AbsoluteLongIndexedX8 AND2 READ
	call S9xGetWord
	And16 ALX16

Op23M1:
	StackRelative8 AND READ
	call S9xGetByte
	And8 SREL8

Op23M0:
	StackRelative8 AND2 READ
	call S9xGetWord
	And16 SREL16

Op33M1:
	StackRelativeIndirectIndexed8 AND READ
	call S9xGetByte
	And8 SRII8

Op33M0:
	StackRelativeIndirectIndexed8 AND2 READ
	call S9xGetWord
	And16 SRII16

/* ASL */
Op0AM1:
	movb AL, %al
	addl $6, CYCLES
	salb %al
	movb %al, AL
	SetZNC
	jmp MainAsmLoop

Op0AM0:
	movw AA, %ax
	addl $6, CYCLES
	salw %ax
	movw %ax, AA
	setnz _Zero
	setc _Carry
	movb %ah, _Negative
	jmp MainAsmLoop
	
Op06M1:
	Direct8 ASL MODIFY
	Asl8 DIR8

Op06M0:
	Direct8 ASL2 MODIFY
	Asl16 DIR16

Op16M1:
	DirectIndexedX8 ASL MODIFY
	Asl8 DIX

Op16M0:
	DirectIndexedX8 ASL2 MODIFY
	Asl16 DIX

Op0EM1:
	Absolute8 ASL MODIFY
	Asl8 ABS

Op0EM0:
	Absolute8 ASL2 MODIFY
	Asl16 ABS

Op1EM1:
	AbsoluteIndexedX8 ASL MODIFY
	Asl8 ABSX

Op1EM0:
	AbsoluteIndexedX8 ASL2 MODIFY
	Asl16 ABSX

/* BIT */
Op89M1:
	Immediate8 BIT READ
	andb AL, %al
	movb %al, _Zero
	jmp MainAsmLoop

Op89M0:
	Immediate16 BIT READ
	andw AA, %ax
	setnz _Zero
	jmp MainAsmLoop

Op24M1:
	Direct8 BIT READ
	Bit8 DIR

Op24M0:
	Direct8 BIT READ
	Bit16 DIR

Op34M1:
	DirectIndexedX8 BIT READ
	Bit8 DIX

Op34M0:
	DirectIndexedX8 BIT2 READ
	Bit16 DIX

Op2CM1:
	Absolute8 BIT READ
	Bit8 ABS

Op2CM0:
	Absolute8 BIT2 READ
	Bit16 ABS

Op3CM1:
	AbsoluteIndexedX8 BIT READ
	Bit8 ABSX

Op3CM0:
	AbsoluteIndexedX8 BIT2 READ
	Bit16 ABSX

/* CMP */
OpC9M1:
	Immediate8 CMP READ
	Cmp8 IMM

OpC9M0:
	Immediate16 CMP READ
	Cmp16 IMM

OpC5M1:
	Direct8 CMP READ
	call S9xGetByte
	Cmp8 DIR

OpC5M0:
	Direct8 CMP2 READ
	call S9xGetWord
	Cmp16 DIR

OpD5M1:
	DirectIndexedX8 CMP READ
	call S9xGetByte
	Cmp8 DIX

OpD5M0:
	DirectIndexedX8 CMP2 READ
	call S9xGetWord
	Cmp16 DIX

OpD2M1:
	DirectIndirect8 CMP READ
	call S9xGetByte
	Cmp8 DI

OpD2M0:
	DirectIndirect8 CMP2 READ
	call S9xGetWord
	Cmp16 DI

OpC1M1:
	DirectIndexedIndirect8 CMP READ
	call S9xGetByte
	Cmp8 DII

OpC1M0:
	DirectIndexedIndirect8 CMP2 READ
	call S9xGetWord
	Cmp16 DII

OpD1M1:
	DirectIndirectIndexed8 CMP READ
	call S9xGetByte
	Cmp8 DIIY

OpD1M0:
	DirectIndirectIndexed8 CMP2 READ
	call S9xGetWord
	Cmp16 DIIY

OpC7M1:
	DirectIndirectLong8 CMP READ
	call S9xGetByte
	Cmp8 DIL

OpC7M0:
	DirectIndirectLong8 CMP2 READ
	call S9xGetWord
	Cmp16 DIL

OpD7M1:
	DirectIndirectIndexedLong8 CMP READ
	call S9xGetByte
	Cmp8 DIIL

OpD7M0:
	DirectIndirectIndexedLong8 CMP2 READ
	call S9xGetWord
	Cmp16 DIIL

OpCDM1:
	Absolute8 CMP READ
	call S9xGetByte
	Cmp8 ABS

OpCDM0:
	Absolute8 CMP2 READ
	call S9xGetWord
	Cmp16 ABS

OpDDM1:
	AbsoluteIndexedX8 CMP READ
	call S9xGetByte
	Cmp8 ABSX

OpDDM0:
	AbsoluteIndexedX8 CMP2 READ
	call S9xGetWord
	Cmp16 ABSX

OpD9M1:
	AbsoluteIndexedY8 CMP READ
	call S9xGetByte
	Cmp8 ABSY

OpD9M0:
	AbsoluteIndexedY8 CMP2 READ
	call S9xGetWord
	Cmp16 ABSY

OpCFM1:
	AbsoluteLong8 CMP READ
	call S9xGetByte
	Cmp8 ABSL

OpCFM0:
	AbsoluteLong8 CMP2 READ
	call S9xGetWord
	Cmp16 ABSL

OpDFM1:
	AbsoluteLongIndexedX8 CMP READ
	call S9xGetByte
	Cmp8 ALX

OpDFM0:
	AbsoluteLongIndexedX8 CMP2 READ
	call S9xGetWord
	Cmp16 ALX

OpC3M1:
	StackRelative8 CMP READ
	call S9xGetByte
	Cmp8 SREL

OpC3M0:
	StackRelative8 CMP2 READ
	call S9xGetWord
	Cmp16 SREL

OpD3M1:
	StackRelativeIndirectIndexed8 CMP READ
	call S9xGetByte
	Cmp8 SRII

OpD3M0:
	StackRelativeIndirectIndexed8 CMP2 READ
	call S9xGetWord
	Cmp16 SRII

/* CPX */
OpE0X1:
	Immediate8 CPX READ
	Cpx8 IMM

OpE0X0:
	Immediate16 CPX READ
	Cpx16 IMM

OpE4X1:
	Direct8 CPX READ
	call S9xGetByte
	Cpx8 DIR

OpE4X0:
	Direct8 CPX2 READ
	call S9xGetWord
	Cpx16 DIR

OpECX1:
	Absolute8 CPX READ
	call S9xGetByte
	Cpx8 ABS

OpECX0:
	Absolute8 CPX2 READ
	call S9xGetWord
	Cpx16 ABS


/* CPY */
OpC0X1:
	Immediate8 CPY READ
	Cpy8 IMM

OpC0X0:
	Immediate16 CPY READ
	Cpy16 IMM

OpC4X1:
	Direct8 CPY READ
	call S9xGetByte
	Cpy8 DIR

OpC4X0:
	Direct8 CPY2 READ
	call S9xGetWord
	Cpy16 DIR

OpCCX1:
	Absolute8 CPY READ
	call S9xGetByte
	Cpy8 ABS

OpCCX0:
	Absolute8 CPY2 READ
	call S9xGetWord
	Cpy16 ABS

/* DEC */
Op3AM1:
	movb AL, %al
	addl $6, CYCLES
	decb %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, AL
	SetZN
	jmp MainAsmLoop

Op3AM0:
	addl $6, CYCLES
	decw AA
	setnz _Zero
	movb AH, %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, _Negative
	jmp MainAsmLoop

OpC6M1:
	Direct8 DEC MODIFY
	Dec8 DIR

OpC6M0:
	Direct8 DEC2 MODIFY
	Dec16 DIR

OpD6M1:
	DirectIndexedX8 DEC MODIFY
	Dec8 DIX

OpD6M0:
	DirectIndexedX8 DEC2 MODIFY
	Dec16 DIX

OpCEM1:
	Absolute8 DEC MODIFY
	Dec8 ABS

OpCEM0:
	Absolute8 DEC2 MODIFY
	Dec16 ABS

OpDEM1:
	AbsoluteIndexedX8 DEC MODIFY

	Dec8 ABSX

OpDEM0:
	AbsoluteIndexedX8 DEC2 MODIFY
	Dec16 ABSX

/* EOR */
Op49M1:
	Immediate8 EOR READ
	Eor8 IMM

Op49M0:
	Immediate16 EOR READ
	Eor16 IMM

Op45M1:
	Direct8 EOR READ
	call S9xGetByte
	Eor8 DIR

Op45M0:
	Direct8 EOR2 READ
	call S9xGetWord
	Eor16 DIR

Op55M1:
	DirectIndexedX8 EOR READ
	call S9xGetByte
	Eor8 DIX

Op55M0:
	DirectIndexedX8 EOR2 READ
	call S9xGetWord
	Eor16 DIX

Op52M1:
	DirectIndirect8 EOR READ
	call S9xGetByte
	Eor8 DI

Op52M0:
	DirectIndirect8 EOR2 READ
	call S9xGetWord
	Eor16 DI

Op41M1:
	DirectIndexedIndirect8 EOR READ
	call S9xGetByte
	Eor8 DII

Op41M0:
	DirectIndexedIndirect8 EOR2 READ
	call S9xGetWord
	Eor16 DII

Op51M1:
	DirectIndirectIndexed8 EOR READ
	call S9xGetByte
	Eor8 DIIY

Op51M0:
	DirectIndirectIndexed8 EOR2 READ
	call S9xGetWord
	Eor16 DIIY

Op47M1:
	DirectIndirectLong8 EOR READ
	call S9xGetByte
	Eor8 DIL

Op47M0:
	DirectIndirectLong8 EOR2 READ
	call S9xGetWord
	Eor16 DIL

Op57M1:
	DirectIndirectIndexedLong8 EOR READ
	call S9xGetByte
	Eor8 DIIL

Op57M0:
	DirectIndirectIndexedLong8 EOR2 READ
	call S9xGetWord
	Eor16 DIIL

Op4DM1:
	Absolute8 EOR READ
	call S9xGetByte
	Eor8 ABS

Op4DM0:
	Absolute8 EOR2 READ
	call S9xGetWord
	Eor16 ABS

Op5DM1:
	AbsoluteIndexedX8 EOR READ
	call S9xGetByte
	Eor8 ABSX

Op5DM0:
	AbsoluteIndexedX8 EOR2 READ
	call S9xGetWord
	Eor16 ABSX

Op59M1:
	AbsoluteIndexedY8 EOR READ
	call S9xGetByte
	Eor8 ABSY

Op59M0:
	AbsoluteIndexedY8 EOR2 READ
	call S9xGetWord
	Eor16 ABSY

Op4FM1:
	AbsoluteLong8 EOR READ
	call S9xGetByte
	Eor8 ABSL

Op4FM0:
	AbsoluteLong8 EOR2 READ
	call S9xGetWord
	Eor16 ABSL

Op5FM1:
	AbsoluteLongIndexedX8 EOR READ
	call S9xGetByte
	Eor8 ALX

Op5FM0:
	AbsoluteLongIndexedX8 EOR2 READ
	call S9xGetWord
	Eor16 ALX

Op43M1:
	StackRelative8 EOR READ
	call S9xGetByte
	Eor8 SREL

Op43M0:
	StackRelative8 EOR2 READ
	call S9xGetWord
	Eor16 SREL

Op53M1:
	StackRelativeIndirectIndexed8 EOR READ
	call S9xGetByte
	Eor8 SRII

Op53M0:
	StackRelativeIndirectIndexed8 EOR2 READ
	call S9xGetWord
	Eor16 SRII

/* INC */
Op1AM1:
	movb AL, %al
	addl $6, CYCLES
	incb %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, AL
	SetZN
	jmp MainAsmLoop

Op1AM0:
	addl $6, CYCLES
	incw AA
	setnz _Zero
	movb AH, %ah
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %ah, _Negative
	jmp MainAsmLoop

OpE6M1:
	Direct8 INC MODIFY
	Inc8 DIR

OpE6M0:
	Direct8 INC2 MODIFY
	Inc16 DIR
	
OpF6M1:
	DirectIndexedX8 INC MODIFY
	Inc8 DIX

OpF6M0:
	DirectIndexedX8 INC2 MODIFY
	Inc16 DIX

OpEEM1:
	Absolute8 INC MODIFY
	Inc8 ABS

OpEEM0:
	Absolute8 INC2 MODIFY
	Inc16 ABS

OpFEM1:
	AbsoluteIndexedX8 INC MODIFY
	Inc8 ABSX

OpFEM0:
	AbsoluteIndexedX8 INC2 MODIFY
	Inc16 ABSX

/* LDA */
OpA9M1:
	Immediate8 LDA READ
	Lda8 IMM

OpA9M0:
	Immediate16 LDA READ
	Lda16 IMM

OpA5M1:
	Direct8 LDA READ
	call S9xGetByte
	Lda8 DIR

OpA5M0:
	Direct8 LDA2 READ
	call S9xGetWord
	Lda16 DIR

OpB5M1:
	DirectIndexedX8 LDA READ
	call S9xGetByte
	Lda8 DIX

OpB5M0:
	DirectIndexedX8 LDA2 READ
	call S9xGetWord
	Lda16 DIX

OpB2M1:
	DirectIndirect8 LDA READ
	call S9xGetByte
	Lda8 DI

OpB2M0:
	DirectIndirect8 LDA2 READ
	call S9xGetWord
	Lda16 DI

OpA1M1:
	DirectIndexedIndirect8 LDA READ
	call S9xGetByte
	Lda8 DII

OpA1M0:
	DirectIndexedIndirect8 LDA2 READ
	call S9xGetWord
	Lda16 DII

OpB1M1:
	DirectIndirectIndexed8 LDA READ
	call S9xGetByte
	Lda8 DIIY

OpB1M0:
	DirectIndirectIndexed8 LDA2 READ
	call S9xGetWord
	Lda16 DIIY

OpA7M1:
	DirectIndirectLong8 LDA READ
	call S9xGetByte
	Lda8 DIL

OpA7M0:
	DirectIndirectLong8 LDA2 READ
	call S9xGetWord
	Lda16 DIL

OpB7M1:
	DirectIndirectIndexedLong8 LDA READ
	call S9xGetByte
	Lda8 DIIL

OpB7M0:
	DirectIndirectIndexedLong8 LDA2 READ
	call S9xGetWord
	Lda16 DIIL

OpADM1:
	Absolute8 LDA READ
	call S9xGetByte
	Lda8 ABS

OpADM0:
	Absolute8 LDA2 READ
	call S9xGetWord
	Lda16 ABS

OpBDM1:
	AbsoluteIndexedX8 LDA READ
	call S9xGetByte
	Lda8 ABSX

OpBDM0:
	AbsoluteIndexedX8 LDA2 READ
	call S9xGetWord
	Lda16 ABSX

OpB9M1:
	AbsoluteIndexedY8 LDA READ
	call S9xGetByte
	Lda8 ABSY

OpB9M0:
	AbsoluteIndexedY8 LDA2 READ
	call S9xGetWord
	Lda16 ABSY

OpAFM1:
	AbsoluteLong8 LDA READ
	call S9xGetByte
	Lda8 ABSL

OpAFM0:
	AbsoluteLong8 LDA2 READ
	call S9xGetWord
	Lda16 ABSL

OpBFM1:
	AbsoluteLongIndexedX8 LDA READ
	call S9xGetByte
	Lda8 ALX

OpBFM0:
	AbsoluteLongIndexedX8 LDA2 READ
	call S9xGetWord
	Lda16 ALX

OpA3M1:
	StackRelative8 LDA READ
	call S9xGetByte
	Lda8 SREL

OpA3M0:
	StackRelative8 LDA2 READ
	call S9xGetWord
	Lda16 SREL

OpB3M1:
	StackRelativeIndirectIndexed8 LDA READ
	call S9xGetByte
	Lda8 SRII

OpB3M0:
	StackRelativeIndirectIndexed8 LDA2 READ
	call S9xGetWord
	Lda16 SRII

/* LDX */
OpA2X1:
	Immediate8 LDX READ
	Ldx8 IMM

OpA2X0:
	Immediate16 LDX READ
	Ldx16 IMM

OpA6X1:
	Direct8 LDX READ
	call S9xGetByte
	Ldx8 DIR

OpA6X0:
	Direct8 LDX2 READ
	call S9xGetWord
	Ldx16 DIR

OpB6X1:
	DirectIndexedY8 LDX READ
	call S9xGetByte
	Ldx8 DIY

OpB6X0:
	DirectIndexedY8 LDX2 READ
	call S9xGetWord
	Ldx16 DIY

OpAEX1:
	Absolute8 LDX READ
	call S9xGetByte
	Ldx8 ABS

OpAEX0:
	Absolute8 LDX2 READ
	call S9xGetWord
	Ldx16 ABS

OpBEX1:
	AbsoluteIndexedY8 LDX READ
	call S9xGetByte
	Ldx8 ABSY

OpBEX0:
	AbsoluteIndexedY8 LDX2 READ
	call S9xGetWord
	Ldx16 ABSY

/* LDY */
OpA0X1:
	Immediate8 LDY READ
	Ldy8 IMM

OpA0X0:
	Immediate16 LDY READ
	Ldy16 IMM

OpA4X1:
	Direct8 LDY READ
	call S9xGetByte
	Ldy8 DIR

OpA4X0:
	Direct8 LDY2 READ
	call S9xGetWord
	Ldy16 DIR

OpB4X1:
	DirectIndexedX8 LDY READ
	call S9xGetByte
	Ldy8 DIX

OpB4X0:
	DirectIndexedX8 LDY2 READ
	call S9xGetWord
	Ldy16 DIX

OpACX1:
	Absolute8 LDY READ
	call S9xGetByte
	Ldy8 ABS

OpACX0:
	Absolute8 LDY2 READ
	call S9xGetWord
	Ldy16 ABS

OpBCX1:
	AbsoluteIndexedX8 LDY READ
	call S9xGetByte
	Ldy8 ABSX

OpBCX0:
	AbsoluteIndexedX8 LDY2 READ
	call S9xGetWord
	Ldy16 ABSX

/* LSR */
Op4AM1:
	movb AL, %al
	addl $6, CYCLES
	shrb %al
	movb %al, AL
	SetZNC
	jmp MainAsmLoop

Op4AM0:
	addl $6, CYCLES
	shrw AA
	setnz _Zero
	setc _Carry
	movb AH, %ah
	movb %ah, _Negative
	jmp MainAsmLoop

Op46M1:
	Direct8 LSR MODIFY
	Lsr8 DIR

Op46M0:
	Direct8 LSR2 MODIFY
	Lsr16 DIR

Op56M1:
	DirectIndexedX8 LSR MODIFY
	Lsr8 DIX

Op56M0:
	DirectIndexedX8 LSR2 MODIFY
	Lsr16 DIX

Op4EM1:
	Absolute8 LSR MODIFY
	Lsr8 ABS

Op4EM0:
	Absolute8 LSR2 MODIFY
	Lsr16 ABS

Op5EM1:
	AbsoluteIndexedX8 LSR MODIFY
	Lsr8 ABSX

Op5EM0:
	AbsoluteIndexedX8 LSR2 MODIFY
	Lsr16 ABSX

/* ORA */
Op09M1:
	Immediate8 ORA READ
	Ora8 IMM

Op09M0:
	Immediate16 ORA READ
	Ora16 IMM

Op05M1:
	Direct8 ORA READ
	call S9xGetByte
	Ora8 DIR

Op05M0:
	Direct8 ORA2 READ
	call S9xGetWord
	Ora16 DIR

Op15M1:
	DirectIndexedX8 ORA READ
	call S9xGetByte
	Ora8 DIX

Op15M0:
	DirectIndexedX8 ORA2 READ
	call S9xGetWord
	Ora16 DIX

Op12M1:
	DirectIndirect8 ORA READ
	call S9xGetByte
	Ora8 DI

Op12M0:
	DirectIndirect8 ORA2 READ
	call S9xGetWord
	Ora16 DI

Op01M1:
	DirectIndexedIndirect8 ORA READ
	call S9xGetByte
	Ora8 DII

Op01M0:
	DirectIndexedIndirect8 ORA2 READ
	call S9xGetWord
	Ora16 DII

Op11M1:
	DirectIndirectIndexed8 ORA READ
	call S9xGetByte
	Ora8 DIIY

Op11M0:
	DirectIndirectIndexed8 ORA2 READ
	call S9xGetWord
	Ora16 DIIY

Op07M1:
	DirectIndirectLong8 ORA READ
	call S9xGetByte
	Ora8 DIL

Op07M0:
	DirectIndirectLong8 ORA2 READ
	call S9xGetWord
	Ora16 DIL

Op17M1:
	DirectIndirectIndexedLong8 ORA READ
	call S9xGetByte
	Ora8 DIIL

Op17M0:
	DirectIndirectIndexedLong8 ORA2 READ
	call S9xGetWord
	Ora16 DIIL

Op0DM1:
	Absolute8 ORA READ
	call S9xGetByte
	Ora8 ABS

Op0DM0:
	Absolute8 ORA2 READ
	call S9xGetWord
	Ora16 ABS

Op1DM1:
	AbsoluteIndexedX8 ORA READ
	call S9xGetByte
	Ora8 ABSX

Op1DM0:
	AbsoluteIndexedX8 ORA2 READ
	call S9xGetWord
	Ora16 ABSX

Op19M1:
	AbsoluteIndexedY8 ORA READ
	call S9xGetByte
	Ora8 ABSY

Op19M0:
	AbsoluteIndexedY8 ORA2 READ
	call S9xGetWord
	Ora16 ABSY

Op0FM1:
	AbsoluteLong8 ORA READ
	call S9xGetByte
	Ora8 ABSL

Op0FM0:
	AbsoluteLong8 ORA2 READ
	call S9xGetWord
	Ora16 ABSL

Op1FM1:
	AbsoluteLongIndexedX8 ORA READ
	call S9xGetByte
	Ora8 ALX

Op1FM0:
	AbsoluteLongIndexedX8 ORA2 READ
	call S9xGetWord
	Ora16 ALX

Op03M1:
	StackRelative8 ORA READ
	call S9xGetByte
	Ora8 SREL

Op03M0:
	StackRelative8 ORA2 READ
	call S9xGetWord
	Ora16 SREL

Op13M1:
	StackRelativeIndirectIndexed8 ORA READ
	call S9xGetByte
	Ora8 SRII

Op13M0:
	StackRelativeIndirectIndexed8 ORA2 READ
	call S9xGetWord
	Ora16 SRII

/* ROL */
Op2AM1:
	addl $6, CYCLES
	movb AL, %al
	GetCarry
	rclb %al
	movb %al, AL
	SetZNC
	jmp MainAsmLoop

Op2AM0:
	addl $6, CYCLES
	GetCarry
	movw AA, %ax
	rclw %ax
	movw %ax, AA
	setc _Carry
	movb %ah, _Negative
	orb %ah, %al
	movb %al, _Zero
	jmp MainAsmLoop

Op26M1:
	Direct8 ROL MODIFY
	Rol8 DIR

Op26M0:
	Direct8 ROL2 MODIFY
	Rol16 DIR

Op36M1:
	DirectIndexedX8 ROL MODIFY
	Rol8 DIX

Op36M0:
	DirectIndexedX8 ROL2 MODIFY
	Rol16 DIX

Op2EM1:
	Absolute8 ROL MODIFY
	Rol8 ABS

Op2EM0:
	Absolute8 ROL2 MODIFY
	Rol16 ABS

Op3EM1:
	AbsoluteIndexedX8 ROL MODIFY
	Rol8 ABSX

Op3EM0:
	AbsoluteIndexedX8 ROL2 MODIFY
	Rol16 ABSX

/* ROR */
Op6AM1:
	addl $6, CYCLES
	movb AL, %al
	GetCarry
	rcrb %al
	movb %al, AL
	SetZNC
	jmp MainAsmLoop

Op6AM0:
	addl $6, CYCLES
	GetCarry
	movw AA, %ax
	rcrw %ax
	movw %ax, AA
	setc _Carry
	movb %ah, _Negative
	orb %ah, %al
	movb %al, _Zero
	jmp MainAsmLoop
	
Op66M1:
	Direct8 ROR MODIFY
	Ror8 DIR

Op66M0:
	Direct8 ROR2 MODIFY
	Ror16 DIR

Op76M1:
	DirectIndexedX8 ROR MODIFY
	Ror8 DIX

Op76M0:
	DirectIndexedX8 ROR2 MODIFY
	Ror16 DIX

Op6EM1:
	Absolute8 ROR MODIFY
	Ror8 ABS

Op6EM0:
	Absolute8 ROR2 MODIFY
	Ror16 ABS

Op7EM1:
	AbsoluteIndexedX8 ROR MODIFY
	Ror8 ABSX

Op7EM0:
	AbsoluteIndexedX8 ROR2 MODIFY
	Ror16 ABSX

/* SBC */
OpE9M1:
	Immediate8 SBC READ
	Sbc8 IMM

OpE9M0:
	Immediate16 SBC READ
	Sbc16 IMM

OpE5M1:
	Direct8 SBC READ
	call S9xGetByte
	Sbc8 DIR

OpE5M0:
	Direct8 SBC2 READ
	call S9xGetWord
	Sbc16 DIR

OpF5M1:
	DirectIndexedX8 SBC READ
	call S9xGetByte
	Sbc8 DIX

OpF5M0:
	DirectIndexedX8 SBC2 READ
	call S9xGetWord
	Sbc16 DIX

OpF2M1:
	DirectIndirect8 SBC READ
	call S9xGetByte
	Sbc8 DI

OpF2M0:
	DirectIndirect8 SBC2 READ
	call S9xGetWord
	Sbc16 DI

OpE1M1:
	DirectIndexedIndirect8 SBC READ
	call S9xGetByte
	Sbc8 DII

OpE1M0:
	DirectIndexedIndirect8 SBC2 READ
	call S9xGetWord
	Sbc16 DII

OpF1M1:
	DirectIndirectIndexed8 SBC READ
	call S9xGetByte
	Sbc8 DIIY

OpF1M0:
	DirectIndirectIndexed8 SBC2 READ
	call S9xGetWord
	Sbc16 DIIY

OpE7M1:
	DirectIndirectLong8 SBC READ
	call S9xGetByte
	Sbc8 DIL

OpE7M0:
	DirectIndirectLong8 SBC2 READ
	call S9xGetWord
	Sbc16 DIL

OpF7M1:
	DirectIndirectIndexedLong8 SBC READ
	call S9xGetByte
	Sbc8 DIIL

OpF7M0:
	DirectIndirectIndexedLong8 SBC2 READ
	call S9xGetWord
	Sbc16 DIIL

OpEDM1:
	Absolute8 SBC READ
	call S9xGetByte
	Sbc8 ABS

OpEDM0:
	Absolute8 SBC2 READ
	call S9xGetWord
	Sbc16 ABS

OpFDM1:
	AbsoluteIndexedX8 SBC READ
	call S9xGetByte
	Sbc8 ABSX

OpFDM0:
	AbsoluteIndexedX8 SBC2 READ
	call S9xGetWord
	Sbc16 ABSX

OpF9M1:
	AbsoluteIndexedY8 SBC READ
	call S9xGetByte
	Sbc8 ABSY

OpF9M0:
	AbsoluteIndexedY8 SBC2 READ
	call S9xGetWord
	Sbc16 ABSY

OpEFM1:
	AbsoluteLong8 SBC READ
	call S9xGetByte
	Sbc8 ABSL

OpEFM0:
	AbsoluteLong8 SBC2 READ
	call S9xGetWord
	Sbc16 ABSL

OpFFM1:
	AbsoluteLongIndexedX8 SBC READ
	call S9xGetByte
	Sbc8 ALX

OpFFM0:
	AbsoluteLongIndexedX8 SBC2 READ
	call S9xGetWord
	Sbc16 ALX

OpE3M1:
	StackRelative8 SBC READ
	call S9xGetByte
	Sbc8 SREL

OpE3M0:
	StackRelative8 SBC2 READ
	call S9xGetWord
	Sbc16 SREL

OpF3M1:
	StackRelativeIndirectIndexed8 SBC READ
	call S9xGetByte
	Sbc8 SRII

OpF3M0:
	StackRelativeIndirectIndexed8 SBC2 READ
	call S9xGetWord
	Sbc16 SRII

/* STA */
Op85M1:
	Direct8 STA WRITE
	Sta8 DIR

Op85M0:
	Direct8 STA2 WRITE
	Sta16 DIR

Op95M1:
	DirectIndexedX8 STA WRITE
	Sta8 DIX

Op95M0:
	DirectIndexedX8 STA2 WRITE
	Sta16 DIX

Op92M1:
	DirectIndirect8 STA WRITE
	Sta8 DI

Op92M0:
	DirectIndirect8 STA2 WRITE
	Sta16 DI

Op81M1:
	DirectIndexedIndirect8 STA WRITE
	Sta8 DII

Op81M0:
	DirectIndexedIndirect8 STA2 WRITE
	Sta16 DII

Op91M1:
	DirectIndirectIndexed8 STA WRITE
	Sta8 DIIY

Op91M0:
	DirectIndirectIndexed8 STA2 WRITE
	Sta16 DIIY

Op87M1:
	DirectIndirectLong8 STA WRITE
	Sta8 DIL

Op87M0:
	DirectIndirectLong8 STA2 WRITE
	Sta16 DIL

Op97M1:
	DirectIndirectIndexedLong8 STA WRITE
	Sta8 DIIL

Op97M0:
	DirectIndirectIndexedLong8 STA2 WRITE
	Sta16 DIIL

Op8DM1:
	Absolute8 STA WRITE
	Sta8 ABS

Op8DM0:
	Absolute8 STA WRITE
	Sta16 ABS

Op9DM1:
	AbsoluteIndexedX8 STA WRITE
	Sta8 ABSX

Op9DM0:
	AbsoluteIndexedX8 STA2 WRITE
	Sta16 ABSX

Op99M1:
	AbsoluteIndexedY8 STA WRITE
	Sta8 ABSY

Op99M0:
	AbsoluteIndexedY8 STA2 WRITE
	Sta16 ABSY

Op8FM1:
	AbsoluteLong8 STA WRITE
	Sta8 ABSL

Op8FM0:
	AbsoluteLong8 STA2 WRITE
	Sta16 ABSL

Op9FM1:
	AbsoluteLongIndexedX8 STA WRITE
	Sta8 ALX

Op9FM0:
	AbsoluteLongIndexedX8 STA2 WRITE
	Sta16 ALX

Op83M1:
	StackRelative8 STA WRITE
	Sta8 SREL

Op83M0:
	StackRelative8 STA2 WRITE
	Sta16 SREL

Op93M1:
	StackRelativeIndirectIndexed8 STA WRITE
	Sta8 SRII

Op93M0:
	StackRelativeIndirectIndexed8 STA2 WRITE
	Sta16 SRII

/* STX */
Op86X1:
	Direct8 STX WRITE
	Stx8 DIR

Op86X0:
	Direct8 STX2 WRITE
	Stx16 DIR

Op96X1:
	DirectIndexedY8 STX WRITE
	Stx8 DIY

Op96X0:
	DirectIndexedY8 STX2 WRITE
	Stx16 DIY

Op8EX1:
	Absolute8 STX WRITE
	Stx8 ABS

Op8EX0:
	Absolute8 STX2 WRITE
	Stx16 ABS

/* STY */
Op84X1:
	Direct8 STY WRITE
	Sty8 DIR

Op84X0:
	Direct8 STY2 WRITE
	Sty16 DIR

Op94X1:
	DirectIndexedX8 STY WRITE
	Sty8 DIX

Op94X0:
	DirectIndexedX8 STY2 WRITE
	Sty16 DIX

Op8CX1:
	Absolute8 STY WRITE
	Sty8 ABS

Op8CX0:
	Absolute8 STY2 WRITE
	Sty16 ABS

/* STZ */
Op64M1:
	Direct8 STZ WRITE
	Stz8 DIR

Op64M0:
	Direct8 STZ2 WRITE
	Stz16 DIR

Op74M1:
	DirectIndexedX8 STZ WRITE
	Stz8 DIX

Op74M0:
	DirectIndexedX8 STZ2 WRITE
	Stz16 DIX

Op9CM1:
	Absolute8 STZ WRITE
	Stz8 ABS

Op9CM0:
	Absolute8 STZ2 WRITE
	Stz16 ABS

Op9EM1:
	AbsoluteIndexedX8 STZ WRITE
	Stz8 ABSX

Op9EM0:
	AbsoluteIndexedX8 STZ2 WRITE
	Stz16 ABSX

/* TRB */
Op14M1:
	Direct8 TRB MODIFY
	Trb8 DIR

Op14M0:
	Direct8 TRB2 MODIFY
	Trb16 DIR

Op1CM1:
	Absolute8 TRB MODIFY
	Trb8 ABS

Op1CM0:
	Absolute8 TRB2 MODIFY
	Trb16 ABS

/* TSB */
Op04M1:
	Direct8 TSB MODIFY
	Tsb8 DIR

Op04M0:
	Direct8 TSB2 MODIFY
	Tsb16 DIR

Op0CM1:
	Absolute8 TSB MODIFY
	Tsb8 ABS

Op0CM0:
	Absolute8 TSB2 MODIFY
	Tsb16 ABS

/* BCC */
Op90:
	Relative JUMP
	BranchCheck0 BCC
	testb $0xff, _Carry
	jnz .BCC_EXIT
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
.BCC_EXIT:
	jmp MainAsmLoop

/* BCS */
OpB0:
	Relative JUMP
	BranchCheck0 BCS
	testb $0xff, _Carry
	jz .BCS_EXIT
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
.BCS_EXIT:
	jmp MainAsmLoop

/* BEQ */
OpF0:
	Relative JUMP
	BranchCheck2 BEQ
	testb $0xff, _Zero
	jnz .BEQ_EXIT
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
	CPUShutdown BEQ
.BEQ_EXIT:
	jmp MainAsmLoop

/* BMI */
Op30:
	Relative JUMP
	BranchCheck1 BMI
	testb $0x80, _Negative
	jz .BMI_EXIT
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
	CPUShutdown BMI
.BMI_EXIT:
	jmp MainAsmLoop

/* BNE */
OpD0:
	Relative JUMP
	BranchCheck1 BNE
	testb $0xff, _Zero
	jz .BNE_EXIT
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
	CPUShutdown BNE
.BNE_EXIT:
	jmp MainAsmLoop

/* BPL */
Op10:
	Relative JUMP
	BranchCheck1 BPL
	testb $0x80, _Negative
	jnz .BPL_EXIT
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
	CPUShutdown BPL
.BPL_EXIT:
	jmp MainAsmLoop

/* BRA */
Op80:
	Relative JUMP
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
	jmp MainAsmLoop

/* BVC */
Op50:
	Relative JUMP
	BranchCheck0 BVC
	testb $0xff, _Overflow
	jnz .BVC_EXIT
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
	CPUShutdown BVC
.BVC_EXIT:
	jmp MainAsmLoop

/* BVS */
Op70:
	Relative JUMP
	BranchCheck0 BVS
	testb $0xff, _Overflow
	jz .BVS_EXIT
	addl $6, CYCLES
	andl $0xffff, %edx
	addl PCBase, %edx
	movl %edx, PC
	CPUShutdown BVS
.BVS_EXIT:
	jmp MainAsmLoop

/* BRL */
Op82:
	RelativeLong BRL JUMP
	andl $0xffff, %edx
	orl ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

/* CLC */
Op18:
	movb $0, _Carry
	addl $6, CYCLES
	jmp MainAsmLoop

/* CLD */
OpD8:
	andb $~Decimal, FLAGS
	addl $6, CYCLES
	jmp MainAsmLoop

/* CLI */
Op58:
	andb $~IRQ, FLAGS
	addl $6, CYCLES
#if 0 /* C version has this commented out? */
	cmpb $0, IRQActive
	jz .CLI_EXIT
	/* XXX: test for Settings.DisableIRQ */
	call S9xOpcode_IRQ
#endif
.CLI_EXIT:
	jmp MainAsmLoop

/* CLV */
OpB8:
	movb $0, _Overflow
	addl $6, CYCLES
	jmp MainAsmLoop

/* DEX */
OpCAX1:
	addl $6, CYCLES
	movb XL, %al
	decb %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, XL
	SetZN
	jmp MainAsmLoop

OpCAX0:
	addl $6, CYCLES
	decw XX
	setnz _Zero
	movb XH, %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, _Negative
	jmp MainAsmLoop

/* DEY */
Op88X1:
	addl $6, CYCLES
	movb YL, %al
	decb %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, YL
	SetZN
	jmp MainAsmLoop

Op88X0:
	addl $6, CYCLES
	decw YY
	setnz _Zero
	movb YH, %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, _Negative
	jmp MainAsmLoop

/* INX */
OpE8X1:
	addl $6, CYCLES
	movb XL, %al
	incb %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, XL
	SetZN
	jmp MainAsmLoop

OpE8X0:
	addl $6, CYCLES
	incw XX
	setnz _Zero
	movb XH, %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, _Negative
	jmp MainAsmLoop

/* INY */
OpC8X1:
	addl $6, CYCLES
	movb YL, %al
	incb %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, YL
	SetZN
	jmp MainAsmLoop

OpC8X0:
	addl $6, CYCLES
	incw YY
	setnz _Zero
	movb YH, %al
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	movb %al, _Negative
	jmp MainAsmLoop

/* NOP */
OpEA:
	addl $6,CYCLES
	jmp MainAsmLoop

/* PEA */
OpF4E1:
	Immediate16 PEA NONE
	PushWordENew PEA
	jmp MainAsmLoop

OpF4:
	Immediate16 PEA NONE
	PushWord PEA
	jmp MainAsmLoop

/* PEI */
OpD4E1:
	DirectIndirect8 PEI NONE
	movl %edx, %eax
	PushWordENew PEI
	jmp MainAsmLoop

OpD4:
	DirectIndirect8 PEI NONE
	movl %edx, %eax
	PushWord PEI
	jmp MainAsmLoop

/* PER */
Op62E1:
	RelativeLong PER NONE
	movl %edx, %eax
	PushWordENew PER
	jmp MainAsmLoop	

Op62:
	RelativeLong PER NONE
	movl %edx, %eax
	PushWord PER
	jmp MainAsmLoop	

/* PHA */
Op48E1:
	addl $6, CYCLES
	movb AL, %al
	PushByteE PHA
	jmp MainAsmLoop

Op48M1:
	addl $6, CYCLES
	movb AL, %al
	PushByte PHA
	jmp MainAsmLoop

Op48M0:
	addl $6, CYCLES
	movw AA, %ax
	PushWord PHA
	jmp MainAsmLoop

/* PHB */
Op8BE1:
	addl $6, CYCLES
	movb DB, %al
	PushByteE PHB
	jmp MainAsmLoop

Op8B:
	addl $6, CYCLES
	movb DB, %al
	PushByte PHB
	jmp MainAsmLoop

/* PHD */
Op0BE1:
	addl $6, CYCLES
	movw DD, %ax
	PushWordENew PHD
	jmp MainAsmLoop

Op0B:
	addl $6, CYCLES
	movw DD, %ax
	PushWord PHD
	jmp MainAsmLoop

/* PHK */
Op4BE1:
	addl $6, CYCLES
	movb PB, %al
	PushByteE PHK
	jmp MainAsmLoop

Op4B:
	addl $6, CYCLES
	movb PB, %al
	PushByte PHK
	jmp MainAsmLoop

/* PHP */
Op08E1:
	addl $6, CYCLES
	S9xPackStatus PHP
	movb FLAGS, %al
	PushByteE PHP
	jmp MainAsmLoop

Op08:
	addl $6, CYCLES
	S9xPackStatus PHP
	movb FLAGS, %al
	PushByte PHP
	jmp MainAsmLoop

/* PHX */
OpDAE1:
	addl $6, CYCLES
	movb XL, %al
	PushByteE PHX
	jmp MainAsmLoop

OpDAX1:
	addl $6, CYCLES
	movb XL, %al
	PushByte PHX
	jmp MainAsmLoop

OpDAX0:
	addl $6, CYCLES
	movw XX, %ax
	PushWord PHX
	jmp MainAsmLoop

/* PHY */
Op5AE1:
	addl $6, CYCLES
	movb YL, %al
	PushByteE PHY
	jmp MainAsmLoop

Op5AX1:
	addl $6, CYCLES
	movb YL, %al
	PushByte PHY
	jmp MainAsmLoop

Op5AX0:
	addl $6, CYCLES
	movw YY, %ax
	PushWord PHY
	jmp MainAsmLoop

/* PLA */
Op68E1:
	addl $12, CYCLES
	PullByteE PLA
	movb %al, AL
	SetZN
	jmp MainAsmLoop

Op68M1:
	addl $12, CYCLES
	PullByte PLA
	movb %al, AL
	SetZN
	jmp MainAsmLoop

Op68M0:
	addl $12, CYCLES
	PullWord PLA
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* PLB */
OpABE1:
	addl $12, CYCLES
	PullByteE PLB
	movb %al, DB
	SetZN
	andl $0xff, %eax
	sall $16, %eax
	movl %eax, ShiftedDB
	jmp MainAsmLoop

OpAB:
	addl $12, CYCLES
	PullByte PLB
	movb %al, DB
	SetZN
	andl $0xff, %eax
	sall $16, %eax
	movl %eax, ShiftedDB
	jmp MainAsmLoop

/* PLD */
Op2BE1:
	addl $12, CYCLES
	PullWordENew PLD
	movw %ax, DD
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

Op2B:
	addl $12, CYCLES
	PullWord PLD
	movw %ax, DD
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop
	
/* PLP */
Op28E1:
	addl $12, CYCLES
	PullByteE PLPE
	movb %al, FLAGS

	testb $IndexFlag, FLAGS
	jz .PLP16E
	xorb %al, %al
	movb %al, XH
	movb %al, YH
.PLP16E:
	S9xUnpackStatus PLPE
	S9xFixCycles PLPE
/*	CheckForIrq PLP */
	jmp MainAsmLoop

Op28:
	addl $12, CYCLES
	PullByte PLP
	movb %al, FLAGS

	testb $IndexFlag, FLAGS
	jz .PLP16
	xorb %al, %al
	movb %al, XH
	movb %al, YH
.PLP16:
	S9xUnpackStatus PLP
	S9xFixCycles PLP
/*	CheckForIrq PLP */
	jmp MainAsmLoop

/* PLX */
OpFAE1:
	addl $12, CYCLES
	PullByteE PLX
	movb %al, XL
	SetZN
	jmp MainAsmLoop

OpFAX1:
	addl $12, CYCLES
	PullByte PLX
	movb %al, XL
	SetZN
	jmp MainAsmLoop

OpFAX0:
	addl $12, CYCLES
	PullWord PLX
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* PLY */
Op7AE1:
	addl $12, CYCLES
	PullByteE PLY
	movb %al, YL
	SetZN
	jmp MainAsmLoop

Op7AX1:
	addl $12, CYCLES
	PullByte PLY
	movb %al, YL
	SetZN
	jmp MainAsmLoop

Op7AX0:
	addl $12, CYCLES
	PullWord PLY
	movw %ax, YY
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* SEC */
Op38:
	movb $1, _Carry
	addl $6, CYCLES
	jmp MainAsmLoop

/* SED */
OpF8:
	
	orb $Decimal, FLAGS
	addl $6, CYCLES
	jmp MainAsmLoop

/* SEI */
Op78:
	orb $IRQ, FLAGS
	addl $6, CYCLES
	jmp MainAsmLoop

/* TAX */
OpAAX1:
	addl $6, CYCLES
	movb AL, %al
	movb %al, XL
	SetZN
	jmp MainAsmLoop

OpAAX0:
	addl $6, CYCLES
	movw AA, %ax
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TAY */
OpA8X1:
	addl $6, CYCLES
	movb AL, %al
	movb %al, YL
	SetZN
	jmp MainAsmLoop

OpA8X0:
	addl $6, CYCLES
	movw AA, %ax
	movw %ax, YY
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TCD */
Op5B:
	addl $6, CYCLES
	movw AA, %ax
	movw %ax, DD
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TCS */
Op1B:
	addl $6, CYCLES
	movw AA, %ax
	movw %ax, SS
	testw $Emulation, FLAGS16
	jz .TCS_EXIT
	movb $1, SH
.TCS_EXIT:
	jmp MainAsmLoop

/* TDC */
Op7B:
	addl $6, CYCLES
	movw DD, %ax
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TSC */
Op3B:
	addl $6, CYCLES
	movw SS, %ax
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TSX */
OpBAX1:
	addl $6, CYCLES
	movb SL, %al
	movb %al, XL
	SetZN
	jmp MainAsmLoop

OpBAX0:
	addl $6, CYCLES
	movw SS, %ax
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TXA */
Op8AM1:
	addl $6, CYCLES
	movb XL, %al
	movb %al, AL
	SetZN
	jmp MainAsmLoop

Op8AM0:
	addl $6, CYCLES
	movw XX, %ax
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TXS */
Op9A:
	addl $6, CYCLES
	movw XX, %ax
	movw %ax, SS
	testw $Emulation, FLAGS16
	jz .TXS_EXIT
	movb $1, SH
.TXS_EXIT:
	jmp MainAsmLoop

/* TXY */
Op9BX1:
	addl $6, CYCLES
	movb XL, %al
	movb %al, YL
	SetZN
	jmp MainAsmLoop

Op9BX0:
	addl $6, CYCLES
	movw XX, %ax
	movw %ax, YY
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TYA */
Op98M1:
	addl $6, CYCLES
	movb YL, %al
	movb %al, AL
	SetZN
	jmp MainAsmLoop

Op98M0:
	addl $6, CYCLES
	movw YY, %ax
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* TYX */
OpBBX1:
	addl $6, CYCLES
	movb YL, %al
	movb %al, XL
	SetZN
	jmp MainAsmLoop

OpBBX0:
	addl $6, CYCLES
	movw YY, %ax
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop

/* XCE */
OpFB:
	addl $6, CYCLES
	movw FLAGS16, %ax
	andw $~(Emulation | Carry), FLAGS16
	GetCarry
	jnc .XCE_NO_CARRY
	orw $Emulation, FLAGS16
.XCE_NO_CARRY:
	testw $Emulation, %ax
	setnz _Carry
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
	S9xFixCycles XCE
	jmp MainAsmLoop
		
.data
.LC0:
	.string	"*** BRK"
.text

Op00:
#ifdef DEBUGGER
	testb $TRACE_FLAG, Flags
	je .BRK_NO_TRACE
	pushl $.LC0
	ccall S9xTraceMessage
	addl $4,%esp
.BRK_NO_TRACE:
#endif
	movb $1, BRKTriggered
	addl $8, CYCLES
	testw $Emulation, FLAGS16
	jnz .BRK_EMULATION
	movb PB, %al
	PushByte BRK1
	movl PC, %eax
	subl PCBase, %eax
	incl %eax
	PushWord BRK2
	
	S9xPackStatus BRK
	movb FLAGS, %al
	movb %al, OpenBus
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte BRK3
	xorl %ecx, %ecx
	movl %ecx, ShiftedPB
	movb %cl, PB
	movl $0xFFE6, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
	call S9xSetPCBase
	jmp MainAsmLoop
.BRK_EMULATION:
	movl PC, %eax
	subl PCBase, %eax
	incl %eax
	PushWordE BRK2
	
	S9xPackStatus BRK2
	movb FLAGS, %al
	movb %al, OpenBus
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByteE BRK3
	xorl %ecx, %ecx
	movl %ecx, ShiftedPB
	movb %cl, PB
	movl $0xFFFE, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

.data
.LC1:
	.string	"*** IRQ"
.text
.globl S9xOpcode_IRQ

S9xOpcode_IRQ:
#ifdef DEBUGGER
	testb $TRACE_FLAG, Flags
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
	subl PCBase, %eax
	PushWord IRQ2
	
	S9xPackStatus IRQ
	movb FLAGS, %al
	movb %al, OpenBus
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte IRQ3
	xorl %ecx, %ecx
	movl %ecx, ShiftedPB
	movb %cl, PB
	addl $12, CYCLES
	testb $0xff, SA1Enabled
	jz .noirqsa1vector
	movl FillRAM, %eax
	movb 0x2209(%eax), %dl
	testb $0x40, %dl
	jz .noirqsa1vector
	xorl %edx, %edx
	movw 0x220e(%eax), %dx
	jmp S9xSetPCBase
.noirqsa1vector:
	movl $0xFFEE, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
	jmp S9xSetPCBase
.IRQ_EMULATION:
	movl PC, %eax
	subl PCBase, %eax
	PushWord IRQ4
	
	S9xPackStatus IRQ2
	movb FLAGS, %al
	movb %al, OpenBus
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte IRQ5
	xorl %ecx, %ecx
	movl %ecx, ShiftedPB
	movb %cl, PB
	addl $6, CYCLES
	movl $0xFFFE, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
	jmp S9xSetPCBase

.data
.LC2:
	.string	"*** NMI"
.text
.globl S9xOpcode_NMI

S9xOpcode_NMI:
#ifdef DEBUGGER
	testb $TRACE_FLAG, Flags
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
	subl PCBase, %eax
	PushWord NMI2
	
	S9xPackStatus NMI
	movb FLAGS, %al
	movb %al, OpenBus
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte NMI3
	xorl %ecx, %ecx
	movl %ecx, ShiftedPB
	movb %cl, PB
	addl $12, CYCLES
	movl $0xFFEA, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
	jmp S9xSetPCBase
.NMI_EMULATION:
	movl PC, %eax
	subl PCBase, %eax
	PushWord NMI4
	
	S9xPackStatus NMI2
	movb FLAGS, %al
	movb %al, OpenBus
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte NMI5
	xorl %ecx, %ecx
	movl %ecx, ShiftedPB
	movb %cl, PB
	addl $6, CYCLES
	movl $0xFFFA, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
	jmp S9xSetPCBase

.data
.LC3:
	.string	"*** COP"
.text

Op02:
#ifdef DEBUGGER
	testb $TRACE_FLAG, Flags
	je .COP_NO_TRACE
	pushl $.LC3
	ccall S9xTraceMessage
	addl $4,%esp
.COP_NO_TRACE:
#endif
	addl $8, CYCLES
	testw $Emulation, FLAGS16
	jnz .COP_EMULATION
	movb PB, %al
	PushByte COP1
	movl PC, %eax
	subl PCBase, %eax
	incl %eax
	PushWord COP2
	
	S9xPackStatus COP
	movb FLAGS, %al
	movb %al, OpenBus
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByte COP3
	xorl %ecx, %ecx
	movl %ecx, ShiftedPB
	movb %cl, PB
	movl $0xFFE4, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
	call S9xSetPCBase
	jmp MainAsmLoop
.COP_EMULATION:
	movl PC, %eax
	subl PCBase, %eax
	incl %eax
	PushWordE COP4
	
	S9xPackStatus COP2
	movb FLAGS, %al
	movb %al, OpenBus
	andb $~Decimal, FLAGS
	orb $IRQ, FLAGS
	
	PushByteE COP5
	xorl %ecx, %ecx
	movl %ecx, ShiftedPB
	movb %cl, PB
	movl $0xFFF4, %edx
	call S9xGetWord
	movl %eax, %edx
	andl $0xffff, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

/* JML */
OpDC:
	AbsoluteIndirectLong8 JML JUMP
	movl %edx, %ecx
	andl $0xff0000, %ecx
	movl %ecx, ShiftedPB
	sarl $16, %ecx
	movb %cl, PB
	addl $12, CYCLES
	call S9xSetPCBase
	jmp MainAsmLoop

Op5C:
	AbsoluteLong8 JML JUMP
	movl %edx, %ecx
	andl $0xff0000, %ecx
	movl %ecx, ShiftedPB
	sarl $16, %ecx
	movb %cl, PB
	call S9xSetPCBase
	jmp MainAsmLoop

/* JMP */
Op4C:
	Absolute8 JMP JUMP
	andl $0xffff, %edx
	orl  ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

Op6C:
	AbsoluteIndirect8 JMP JUMP
	andl $0xffff, %edx
	orl  ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

Op7C:
	AbsoluteIndexedIndirect8 JMP JUMP
	addl $6, CYCLES
	andl $0xffff, %edx
	orl ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

/* JSL */
Op22E1:
	movb PB, %al
	PushByte JSL_ABSL_E
	movl PC, %eax
	subl PCBase, %eax
	addl $2, %eax
	PushWordENew JSL_ABSL_E
	AbsoluteLong8 JSL JUMP
	movl %edx, %ecx
	andl $0xff0000, %ecx
	movl %ecx, ShiftedPB
	sarl $16, %ecx
	movb %cl, PB
	call S9xSetPCBase
	jmp MainAsmLoop

Op22:
	movb PB, %al
	PushByte JSL_ABSL
	movl PC, %eax
	subl PCBase, %eax
	addl $2, %eax
	PushWord JSL_ABSL
	AbsoluteLong8 JSL JUMP
	movl %edx, %ecx
	andl $0xff0000, %ecx
	movl %ecx, ShiftedPB
	sarl $16, %ecx
	movb %cl, PB
	call S9xSetPCBase
	jmp MainAsmLoop

/* RTL */
Op6BE1:
	addl $12, CYCLES
	PullWordE RTL
	pushl %eax
	PullByteE RTL
	popl %edx
	movb %al, PB
	incw %dx
	andl $0xff, %eax
	andl $0xffff, %edx
	sall $16, %eax
	movl %eax, ShiftedPB
	orl %eax, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

Op6B:
	addl $12, CYCLES
	PullWord RTL
	pushl %eax
	PullByte RTL
	popl %edx
	movb %al, PB
	incw %dx
	andl $0xff, %eax
	andl $0xffff, %edx
	sall $16, %eax
	movl %eax, ShiftedPB
	orl %eax, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

/* JSR ABS */
Op20:
	addl $6, CYCLES
	movl PC, %eax
	subl PCBase, %eax
	incl %eax
	PushWord JSR_ABS
	Absolute8 JSR_ABS JUMP
	andl $0xffff, %edx
	orl  ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

/* JSR ABS INDEXED INDIRECT */
OpFCE1:
	addl $6, CYCLES
	movl PC, %eax
	subl PCBase, %eax
	incl %eax
	PushWordENew JSR_AII_E
	AbsoluteIndexedIndirect8 JSR JUMP
	andl $0xffff, %edx
	orl  ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

OpFC:
	addl $6, CYCLES
	movl PC, %eax
	subl PCBase, %eax
	incl %eax
	PushWord JSR_AII
	AbsoluteIndexedIndirect8 JSR JUMP
	andl $0xffff, %edx
	orl  ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

/* RTS */
Op60:
	addl $6 * 2, CYCLES
	PullWord RTS
	addl $6, CYCLES
	incl %eax
	movl %eax, %edx
	andl $0xffff, %edx
	orl  ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

Op60E1:
	addl $6 * 2, CYCLES
	PullWordE RTS
	addl $6, CYCLES
	incl %eax
	movl %eax, %edx
	andl $0xffff, %edx
	orl  ShiftedPB, %edx
	call S9xSetPCBase
	jmp MainAsmLoop

/* MVN */
Op54X1:
	addl MemSpeedx2, CYCLES
	addl $12, CYCLES
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, DB
	sall $16, %edx
	movl %edx, ShiftedDB
	movb 1(PC), %dl
	movb %dl, OpenBus
	sall $16, %edx
	movw XX, %dx
	call S9xGetByte
	movl ShiftedDB, %edx
	movw YY, %dx
	call S9xSetByte
	incb XL
	incb YL
	movw AA, %ax
	decw %ax
	movw %ax, AA
	cmpw $0xffff, %ax
	je .MVN_EXIT8
	decl PC
	jmp MainAsmLoop
.MVN_EXIT8:
	addl $2, PC
	jmp MainAsmLoop

Op54X0:
	addl MemSpeedx2, CYCLES
	addl $12, CYCLES
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, DB
	sall $16, %edx
	movl %edx, ShiftedDB
	movb 1(PC), %dl
	movb %dl, OpenBus
	sall $16, %edx
	movw XX, %dx
	call S9xGetByte
	movl ShiftedDB, %edx
	movw YY, %dx
	call S9xSetByte
	incw XX
	incw YY
	movw AA, %ax
	decw %ax
	movw %ax, AA
	cmpw $0xffff, %ax
	je .MVN_EXIT16
	decl PC
	jmp MainAsmLoop
.MVN_EXIT16:
	addl $2, PC
	jmp MainAsmLoop

/* MVP */
Op44X1:
	addl MemSpeedx2, CYCLES
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, DB
	sall $16, %edx
	movl %edx, ShiftedDB
	movb 1(PC), %dl
	movb %dl, OpenBus
	sall $16, %edx
	movw XX, %dx
	call S9xGetByte
	movl ShiftedDB, %edx
	movw YY, %dx
	call S9xSetByte
	decb XL
	decb YL
	movw AA, %ax
	decw %ax
	movw %ax, AA
	addl $12, CYCLES
	cmpw $0xffff, %ax
	je .MVP_EXIT8
	decl PC
	jmp MainAsmLoop
.MVP_EXIT8:
	addl $2, PC
	jmp MainAsmLoop

Op44X0:
	addl MemSpeedx2, CYCLES
	xorl %edx, %edx
	movb (PC), %dl
	movb %dl, DB
	sall $16, %edx
	movl %edx, ShiftedDB
	movb 1(PC), %dl
	movb %dl, OpenBus
	sall $16, %edx
	movw XX, %dx
	call S9xGetByte
	movl ShiftedDB, %edx
	movw YY, %dx
	call S9xSetByte
	decw XX
	decw YY
	movw AA, %ax
	decw %ax
	movw %ax, AA
	addl $12, CYCLES
	cmpw $0xffff, %ax
	je .MVP_EXIT16
	decl PC
	jmp MainAsmLoop
.MVP_EXIT16:
	addl $2, PC
	jmp MainAsmLoop

/* REP */
OpC2:
	addl MemSpeed, CYCLES
	addl $6, CYCLES
	movb (PC), %al
	incl PC
	notb %al
	andb %al, FLAGS
	andb %al, _Negative
	andb %al, _Carry
	testb $Zero, %al
	setz %ah
	orb %ah, _Zero
	shrb $6, %al
	andb %al, _Overflow

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
	S9xFixCycles REP
/*	CheckForIrq REP */
	jmp MainAsmLoop

/* SEP */
OpE2:
	addl MemSpeed, CYCLES
	addl $6, CYCLES
	movb (PC), %al
	incl PC
	orb %al, FLAGS
	orb %al, _Negative
	movb %al, %ah
	shrb $6, %ah
	andb $1, %ah
	orb %ah, _Overflow
	test $Zero, %al
	jz .SEP_NO_ZERO
	movb $0, _Zero
.SEP_NO_ZERO:
	andb $1, %al
	orb %al, _Carry
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
	S9xFixCycles SEP
	jmp MainAsmLoop

/* XBA */
OpEB:
	addl $12, CYCLES
	movb AL, %ah
	movb AH, %al
	movb %ah, AH
	movb %al, AL
	SetZN
	jmp MainAsmLoop

/* RTI */
Op40:
	addl $12, CYCLES
	testw $Emulation, FLAGS16   // FIXME: Split into Op40E1 and Op40E0?
	jnz .RTI_EMU
	PullByte RTI
	movb %al, FLAGS
	PullWord RTI
	andl $0xffff, %eax
	pushl %eax
	PullByte RTI2
	andl $0xff, %eax
	jmp .RTI_SKIP_EMU
.RTI_EMU:
	PullByteE RTI
	movb %al, FLAGS
	PullWordE RTI
	andl $0xffff, %eax
	pushl %eax
	orb $(MemoryFlag | IndexFlag), FLAGS
	/*XXX: HERE xorl %eax, %eax*/
	movl ShiftedPB, %eax
	sarl $16, %eax
.RTI_SKIP_EMU:
	movb %al, PB
	popl %edx
	sall $16, %eax
	orl %eax, %edx
	movl %eax, ShiftedPB

	call S9xSetPCBase
	testb $IndexFlag, FLAGS
	jz .RTI16
	xorb %al, %al
	movb %al, XH
	movb %al, YH
.RTI16:
	S9xUnpackStatus RTI
	S9xFixCycles RTI
/*	CheckForIrq RTI */
	jmp MainAsmLoop

/* WAI */
OpCB:
#if 0
	cmpb $0, IRQActive
	je .L5334
	addl $12, CYCLES
	jmp MainAsmLoop
.L5334:
#endif
	movb $1, WaitingForInterrupt
	decl PC
	movb Shutdown, %al
	testb %al, %al
	je .NoShutdown
	movl NextEvent, CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer
	cmpb $0, APUExecuting
	je MainAsmLoop
	movb $0, CPUExecuting
.WAITExecAPU:
#ifdef DEBUGGER
	testb $2,APUFlags
	je .WAITNoAPUS9xTrace
	STORE_REGISTERS
	ccall S9xTraceAPU
	LOAD_REGISTERS
.WAITNoAPUS9xTrace:
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
	jmp MainAsmLoop
.NoShutdown:
	addl $12, CYCLES
	jmp MainAsmLoop

/* STP */
OpDB:
	decl PC
	orb $DEBUG_MODE_FLAG, Flags
	jmp MainAsmLoop

/* Reserved opcode (WDM) */

Op42:
	jmp MainAsmLoop

.globl S9xOpcodesM1X1
.data
	.align 4
S9xOpcodesM1X1:
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

.globl S9xOpcodesE1
.data
	.align 4
S9xOpcodesE1:
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
	.long Op60E1
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

.globl S9xOpcodesM1X0
	.align 4
S9xOpcodesM1X0:
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

.globl S9xOpcodesM0X0
	.align 4
S9xOpcodesM0X0:
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

.globl S9xOpcodesM0X1
	.align 4
S9xOpcodesM0X1:
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
