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

#include "asmstruc.h"
#include "asmaddr.h"
#include "asmops.h"
#include "getset.S"


.macro call_S9xGetByte
	mov.l S9xGetByte,r3
	jsr @r3
	nop
.endm

.macro call_S9xGetWord
	mov.l S9xGetWord,r3
	jsr @r3
	nop
.endm

	
/* ADC */
Op69M1:
	Immediate8 ADC
	Adc8 IMM8

Op69M0:
	Immediate16 ADC2
	Adc16 IMM16

Op65M1:
	Direct8 ADC2
	call_S9xGetByte
	Adc8 DIR8

Op65M0:
	Direct8 ADC
	call_S9xGetWord
	Adc16 DIR16
	
Op75M1:
	DirectIndexedX8 ADC
	call_S9xGetByte
	Adc8 DIX8

Op75M0:
	DirectIndexedX8 ADC2
	call_S9xGetWord
	Adc16 DIX16

Op72M1:
	DirectIndirect8 ADC
	call_S9xGetByte
	Adc8 DI8

Op72M0:
	DirectIndirect8 ADC2
	call_S9xGetWord
	Adc16 DI16

Op61M1:
	DirectIndexedIndirect8 ADC
	call_S9xGetByte
	Adc8 DII8

Op61M0:
	DirectIndexedIndirect8 ADC2
	call_S9xGetWord
	Adc16 DII16

Op71M1:
	DirectIndirectIndexed8 ADC
	call_S9xGetByte
	Adc8 DIIY8

Op71M0:
	DirectIndirectIndexed8 ADC2
	call_S9xGetWord
	Adc16 DIIY16

Op67M1:
	DirectIndirectLong8 ADC
	call_S9xGetByte
	Adc8 DIL8

Op67M0:
	DirectIndirectLong8 ADC2
	call_S9xGetWord
	Adc16 DIL16

Op77M1:
	DirectIndirectIndexedLong8 ADC
	call_S9xGetByte
	Adc8 DIIL8

Op77M0:
	DirectIndirectIndexedLong8 ADC2
	call_S9xGetWord
	Adc16 DIIL8

Op6DM1:
	Absolute8 ADC
	call_S9xGetByte
	Adc8 ABS8

Op6DM0:
	Absolute8 ADC2
	call_S9xGetWord
	Adc16 ABS16

Op7DM1:
	AbsoluteIndexedX8 ADC
	call_S9xGetByte
	Adc8 ABSX8

Op7DM0:
	AbsoluteIndexedX8 ADC2
	call_S9xGetWord
	Adc16 ABSX16

Op79M1:
	AbsoluteIndexedY8 ADC
	call_S9xGetByte
	Adc8 ABSY8

Op79M0:
	AbsoluteIndexedY8 ADC2
	call_S9xGetWord
	Adc16 ABSY16

Op6FM1:
	AbsoluteLong8 ADC
	call_S9xGetByte
	Adc8 ABSL8

Op6FM0:
	AbsoluteLong8 ADC2
	call_S9xGetWord
	Adc16 ABSL16

Op7FM1:
	AbsoluteLongIndexedX8 ADC
	call_S9xGetByte
	Adc8 ALX8

Op7FM0:
	AbsoluteLongIndexedX8 ADC2
	call_S9xGetWord
	Adc16 ALX16

Op63M1:
	StackRelative8 ADC
	call_S9xGetByte
	Adc8 SREL8

Op63M0:
	StackRelative8 ADC2
	call_S9xGetWord
	Adc16 SREL16

Op73M1:
	StackRelativeIndirectIndexed8 ADC
	call_S9xGetByte
	Adc8 SRII8

Op73M0:
	StackRelativeIndirectIndexed8 ADC2
	call_S9xGetWord
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
	call_S9xGetByte
	And8 DIR8

Op25M0:
	Direct8 AND2
	call_S9xGetWord
	And16 DIR16

Op35M1:
	DirectIndexedX8 AND
	call_S9xGetByte
	And8 DIX8

Op35M0:
	DirectIndexedX8 AND2
	call_S9xGetWord
	And16 DIX16

Op32M1:
	DirectIndirect8 AND
	call_S9xGetByte
	And8 DI8

Op32M0:
	DirectIndirect8 AND2
	call_S9xGetWord
	And16 DI16

Op21M1:
	DirectIndexedIndirect8 AND
	call_S9xGetByte
	And8 DII8

Op21M0:
	DirectIndexedIndirect8 AND2
	call_S9xGetWord
	And16 DII16

Op31M1:
	DirectIndirectIndexed8 AND
	call_S9xGetByte
	And8 DIIY8

Op31M0:
	DirectIndirectIndexed8 AND2
	call_S9xGetWord
	And16 DIIY16

Op27M1:
	DirectIndirectLong8 AND
	call_S9xGetByte
	And8 DIL8

Op27M0:
	DirectIndirectLong8 AND2
	call_S9xGetWord
	And16 DIL16

Op37M1:
	DirectIndirectIndexedLong8 AND
	call_S9xGetByte
	And8 DIIL8

Op37M0:
	DirectIndirectIndexedLong8 AND2
	call_S9xGetWord
	And16 DIIL16

Op2DM1:
	Absolute8 AND
	call_S9xGetByte
	And8 ABS8

Op2DM0:
	Absolute8 AND2
	call_S9xGetWord
	And16 ABS16

Op3DM1:
	AbsoluteIndexedX8 AND
	call_S9xGetByte
	And8 ABSX8

Op3DM0:
	AbsoluteIndexedX8 AND2
	call_S9xGetWord
	And16 ABSX16

Op39M1:
	AbsoluteIndexedY8 AND
	call_S9xGetByte
	And8 ABSY8

Op39M0:
	AbsoluteIndexedY8 AND2
	call_S9xGetWord
	And16 ABSY16

Op2FM1:
	AbsoluteLong8 AND
	call_S9xGetByte
	And8 ABSL8

Op2FM0:
	AbsoluteLong8 AND2
	call_S9xGetWord
	And16 ABSL16

Op3FM1:
	AbsoluteLongIndexedX8 AND
	call_S9xGetByte
	And8 ALX8

Op3FM0:
	AbsoluteLongIndexedX8 AND2
	call_S9xGetWord
	And16 ALX16

Op23M1:
	StackRelative8 AND
	call_S9xGetByte
	And8 SREL8

Op23M0:
	StackRelative8 AND2
	call_S9xGetWord
	And16 SREL16

Op33M1:
	StackRelativeIndirectIndexed8 AND
	call_S9xGetByte
	And8 SRII8

Op33M0:
	StackRelativeIndirectIndexed8 AND2
	call_S9xGetWord
	And16 SRII16

/* ASL */
Op0AM1:
	extu.b A,r1
	sub r1,A
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	shll r1
	extu.b r1,r2
	mov #Carry,r0
	or r2,A
	cmp/eq r1,r2
	bf/s .Asl8Carry
	or r0,FLAGS
	add #-Carry,FLAGS
.Asl8Carry:
	SetZNjmp A

Op0AM0:
	extu.w A,r1
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	shll r1
	mov #Carry,r0
	extu.w r1,A
	cmp/eq r1,A
	bf/s .Asl16Carry
	or r0,FLAGS
	add #-Carry,FLAGS
.Asl16Carry:
	Set16ZNjmp A
	
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
	and A, r0
	extu.w r7,r3
	sub r3,r7
	jmp @r13
	or r0,r7

Op89M0:
	Immediate16 BIT
	and A, r0
	extu.w r7,r3
	sub r3,r7
	jmp @r13
	or r0,r7

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
	call_S9xGetByte
	Cmp8 DIR

OpC5M0:
	Direct8 CMP2
	call_S9xGetWord
	Cmp16 DIR

OpD5M1:
	DirectIndexedX8 CMP
	call_S9xGetByte
	Cmp8 DIX

OpD5M0:
	DirectIndexedX8 CMP2
	call_S9xGetWord
	Cmp16 DIX

OpD2M1:
	DirectIndirect8 CMP
	call_S9xGetByte
	Cmp8 DI

OpD2M0:
	DirectIndirect8 CMP2
	call_S9xGetWord
	Cmp16 DI

OpC1M1:
	DirectIndexedIndirect8 CMP
	call_S9xGetByte
	Cmp8 DII

OpC1M0:
	DirectIndexedIndirect8 CMP2
	call_S9xGetWord
	Cmp16 DII

OpD1M1:
	DirectIndirectIndexed8 CMP
	call_S9xGetByte
	Cmp8 DIIY

OpD1M0:
	DirectIndirectIndexed8 CMP2
	call_S9xGetWord
	Cmp16 DIIY

OpC7M1:
	DirectIndirectLong8 CMP
	call_S9xGetByte
	Cmp8 DIL

OpC7M0:
	DirectIndirectLong8 CMP2
	call_S9xGetWord
	Cmp16 DIL

OpD7M1:
	DirectIndirectIndexedLong8 CMP
	call_S9xGetByte
	Cmp8 DIIL

OpD7M0:
	DirectIndirectIndexedLong8 CMP2
	call_S9xGetWord
	Cmp16 DIIL

OpCDM1:
	Absolute8 CMP
	call_S9xGetByte
	Cmp8 ABS

OpCDM0:
	Absolute8 CMP2
	call_S9xGetWord
	Cmp16 ABS

OpDDM1:
	AbsoluteIndexedX8 CMP
	call_S9xGetByte
	Cmp8 ABSX

OpDDM0:
	AbsoluteIndexedX8 CMP2
	call_S9xGetWord
	Cmp16 ABSX

OpD9M1:
	AbsoluteIndexedY8 CMP
	call_S9xGetByte
	Cmp8 ABSY

OpD9M0:
	AbsoluteIndexedY8 CMP2
	call_S9xGetWord
	Cmp16 ABSY

OpCFM1:
	AbsoluteLong8 CMP
	call_S9xGetByte
	Cmp8 ABSL

OpCFM0:
	AbsoluteLong8 CMP2
	call_S9xGetWord
	Cmp16 ABSL

OpDFM1:
	AbsoluteLongIndexedX8 CMP
	call_S9xGetByte
	Cmp8 ALX

OpDFM0:
	AbsoluteLongIndexedX8 CMP2
	call_S9xGetWord
	Cmp16 ALX

OpC3M1:
	StackRelative8 CMP
	call_S9xGetByte
	Cmp8 SREL

OpC3M0:
	StackRelative8 CMP2
	call_S9xGetWord
	Cmp16 SREL

OpD3M1:
	StackRelativeIndirectIndexed8 CMP
	call_S9xGetByte
	Cmp8 SRII

OpD3M0:
	StackRelativeIndirectIndexed8 CMP2
	call_S9xGetWord
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
	call_S9xGetByte
	Cpx8 DIR

OpE4X0:
	Direct8 CPX2
	call_S9xGetWord
	Cpx16 DIR

OpECX1:
	Absolute8 CPX
	call_S9xGetByte
	Cpx8 ABS

OpECX0:
	Absolute8 CPX2
	call_S9xGetWord
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
	call_S9xGetByte
	Cpy8 DIR

OpC4X0:
	Direct8 CPY2
	call_S9xGetWord
	Cpy16 DIR

OpCCX1:
	Absolute8 CPY
	call_S9xGetByte
	Cpy8 ABS

OpCCX0:
	Absolute8 CPY2
	call_S9xGetWord
	Cpy16 ABS

/* DEC */
Op3AM1:
	extu.b A,r0
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	sub r0,A
	add #-1,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	extu.b r0,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	or r0,A
	SetZNjmp r0

Op3AM0:
	add #-1,A
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	extu.w A,A
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	Set16ZNjmp A

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
	call_S9xGetByte
	Eor8 DIR

Op45M0:
	Direct8 EOR2
	call_S9xGetWord
	Eor16 DIR

Op55M1:
	DirectIndexedX8 EOR
	call_S9xGetByte
	Eor8 DIX

Op55M0:
	DirectIndexedX8 EOR2
	call_S9xGetWord
	Eor16 DIX

Op52M1:
	DirectIndirect8 EOR
	call_S9xGetByte
	Eor8 DI

Op52M0:
	DirectIndirect8 EOR2
	call_S9xGetWord
	Eor16 DI

Op41M1:
	DirectIndexedIndirect8 EOR
	call_S9xGetByte
	Eor8 DII

Op41M0:
	DirectIndexedIndirect8 EOR2
	call_S9xGetWord
	Eor16 DII

Op51M1:
	DirectIndirectIndexed8 EOR
	call_S9xGetByte
	Eor8 DIIY

Op51M0:
	DirectIndirectIndexed8 EOR2
	call_S9xGetWord
	Eor16 DIIY

Op47M1:
	DirectIndirectLong8 EOR
	call_S9xGetByte
	Eor8 DIL

Op47M0:
	DirectIndirectLong8 EOR2
	call_S9xGetWord
	Eor16 DIL

Op57M1:
	DirectIndirectIndexedLong8 EOR
	call_S9xGetByte
	Eor8 DIIL

Op57M0:
	DirectIndirectIndexedLong8 EOR2
	call_S9xGetWord
	Eor16 DIIL

Op4DM1:
	Absolute8 EOR
	call_S9xGetByte
	Eor8 ABS

Op4DM0:
	Absolute8 EOR2
	call_S9xGetWord
	Eor16 ABS

Op5DM1:
	AbsoluteIndexedX8 EOR
	call_S9xGetByte
	Eor8 ABSX

Op5DM0:
	AbsoluteIndexedX8 EOR2
	call_S9xGetWord
	Eor16 ABSX

Op59M1:
	AbsoluteIndexedY8 EOR
	call_S9xGetByte
	Eor8 ABSY

Op59M0:
	AbsoluteIndexedY8 EOR2
	call_S9xGetWord
	Eor16 ABSY

Op4FM1:
	AbsoluteLong8 EOR
	call_S9xGetByte
	Eor8 ABSL

Op4FM0:
	AbsoluteLong8 EOR2
	call_S9xGetWord
	Eor16 ABSL

Op5FM1:
	AbsoluteLongIndexedX8 EOR
	call_S9xGetByte
	Eor8 ALX

Op5FM0:
	AbsoluteLongIndexedX8 EOR2
	call_S9xGetWord
	Eor16 ALX

Op43M1:
	StackRelative8 EOR
	call_S9xGetByte
	Eor8 SREL

Op43M0:
	StackRelative8 EOR2
	call_S9xGetWord
	Eor16 SREL

Op53M1:
	StackRelativeIndirectIndexed8 EOR
	call_S9xGetByte
	Eor8 SRII

Op53M0:
	StackRelativeIndirectIndexed8 EOR2
	call_S9xGetWord
	Eor16 SRII

/* INC */
Op1AM1:
	extu.b A,r0
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	sub r0,A
	add #1,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	extu.b r0,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	or r0,A
	SetZNjmp r0

Op1AM0:
	add #1,A
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	extu.w A,A
	Set16ZNjmp A

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
	call_S9xGetByte
	Lda8 DIR

OpA5M0:
	Direct8 LDA2
	call_S9xGetWord
	Lda16 DIR

OpB5M1:
	DirectIndexedX8 LDA
	call_S9xGetByte
	Lda8 DIX

OpB5M0:
	DirectIndexedX8 LDA2
	call_S9xGetWord
	Lda16 DIX

OpB2M1:
	DirectIndirect8 LDA
	call_S9xGetByte
	Lda8 DI

OpB2M0:
	DirectIndirect8 LDA2
	call_S9xGetWord
	Lda16 DI

OpA1M1:
	DirectIndexedIndirect8 LDA
	call_S9xGetByte
	Lda8 DII

OpA1M0:
	DirectIndexedIndirect8 LDA2
	call_S9xGetWord
	Lda16 DII

OpB1M1:
	DirectIndirectIndexed8 LDA
	call_S9xGetByte
	Lda8 DIIY

OpB1M0:
	DirectIndirectIndexed8 LDA2
	call_S9xGetWord
	Lda16 DIIY

OpA7M1:
	DirectIndirectLong8 LDA
	call_S9xGetByte
	Lda8 DIL

OpA7M0:
	DirectIndirectLong8 LDA2
	call_S9xGetWord
	Lda16 DIL

OpB7M1:
	DirectIndirectIndexedLong8 LDA
	call_S9xGetByte
	Lda8 DIIL

OpB7M0:
	DirectIndirectIndexedLong8 LDA2
	call_S9xGetWord
	Lda16 DIIL

OpADM1:
	Absolute8 LDA
	call_S9xGetByte
	Lda8 ABS

OpADM0:
	Absolute8 LDA2
	call_S9xGetWord
	Lda16 ABS

OpBDM1:
	AbsoluteIndexedX8 LDA
	call_S9xGetByte
	Lda8 ABSX

OpBDM0:
	AbsoluteIndexedX8 LDA2
	call_S9xGetWord
	Lda16 ABSX

OpB9M1:
	AbsoluteIndexedY8 LDA
	call_S9xGetByte
	Lda8 ABSY

OpB9M0:
	AbsoluteIndexedY8 LDA2
	call_S9xGetWord
	Lda16 ABSY

OpAFM1:
	AbsoluteLong8 LDA
	call_S9xGetByte
	Lda8 ABSL

OpAFM0:
	AbsoluteLong8 LDA2
	call_S9xGetWord
	Lda16 ABSL

OpBFM1:
	AbsoluteLongIndexedX8 LDA
	call_S9xGetByte
	Lda8 ALX

OpBFM0:
	AbsoluteLongIndexedX8 LDA2
	call_S9xGetWord
	Lda16 ALX

OpA3M1:
	StackRelative8 LDA
	call_S9xGetByte
	Lda8 SREL

OpA3M0:
	StackRelative8 LDA2
	call_S9xGetWord
	Lda16 SREL

OpB3M1:
	StackRelativeIndirectIndexed8 LDA
	call_S9xGetByte
	Lda8 SRII

OpB3M0:
	StackRelativeIndirectIndexed8 LDA2
	call_S9xGetWord
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
	call_S9xGetByte
	Ldx8 DIR

OpA6X0:
	Direct8 LDX2
	call_S9xGetWord
	Ldx16 DIR

OpB6X1:
	DirectIndexedY8 LDX
	call_S9xGetByte
	Ldx8 DIY

OpB6X0:
	DirectIndexedY8 LDX2
	call_S9xGetWord
	Ldx16 DIY

OpAEX1:
	Absolute8 LDX
	call_S9xGetByte
	Ldx8 ABS

OpAEX0:
	Absolute8 LDX2
	call_S9xGetWord
	Ldx16 ABS

OpBEX1:
	AbsoluteIndexedY8 LDX
	call_S9xGetByte
	Ldx8 ABSY

OpBEX0:
	AbsoluteIndexedY8 LDX2
	call_S9xGetWord
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
	call_S9xGetByte
	Ldy8 DIR

OpA4X0:
	Direct8 LDY2
	call_S9xGetWord
	Ldy16 DIR

OpB4X1:
	DirectIndexedX8 LDY
	call_S9xGetByte
	Ldy8 DIX

OpB4X0:
	DirectIndexedX8 LDY2
	call_S9xGetWord
	Ldy16 DIX

OpACX1:
	Absolute8 LDY
	call_S9xGetByte
	Ldy8 ABS

OpACX0:
	Absolute8 LDY2
	call_S9xGetWord
	Ldy16 ABS

OpBCX1:
	AbsoluteIndexedX8 LDY
	call_S9xGetByte
	Ldy8 ABSX

OpBCX0:
	AbsoluteIndexedX8 LDY2
	call_S9xGetWord
	Ldy16 ABSX

/* LSR */
Op4AM1:
	extu.b A,r1
	sub r1,A
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov #Carry,r0
	shlr r1
	bt/s .Lsr8Carry
	or r0,FLAGS
	add #-Carry,FLAGS
.Lsr8Carry:
	or r1,A
	SetZNjmp r1

Op4AM0:
	extu.w A,A
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	shlr A
	mov #Carry,r0
	bt/s .Lsr16Carry
	or r0,FLAGS
	add #-Carry,FLAGS
.Lsr16Carry:
	Set16ZNjmp A

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
	call_S9xGetByte
	Ora8 DIR

Op05M0:
	Direct8 ORA2
	call_S9xGetWord
	Ora16 DIR

Op15M1:
	DirectIndexedX8 ORA
	call_S9xGetByte
	Ora8 DIX

Op15M0:
	DirectIndexedX8 ORA2
	call_S9xGetWord
	Ora16 DIX

Op12M1:
	DirectIndirect8 ORA
	call_S9xGetByte
	Ora8 DI

Op12M0:
	DirectIndirect8 ORA2
	call_S9xGetWord
	Ora16 DI

Op01M1:
	DirectIndexedIndirect8 ORA
	call_S9xGetByte
	Ora8 DII

Op01M0:
	DirectIndexedIndirect8 ORA2
	call_S9xGetWord
	Ora16 DII

Op11M1:
	DirectIndirectIndexed8 ORA
	call_S9xGetByte
	Ora8 DIIY

Op11M0:
	DirectIndirectIndexed8 ORA2
	call_S9xGetWord
	Ora16 DIIY

Op07M1:
	DirectIndirectLong8 ORA
	call_S9xGetByte
	Ora8 DIL

Op07M0:
	DirectIndirectLong8 ORA2
	call_S9xGetWord
	Ora16 DIL

Op17M1:
	DirectIndirectIndexedLong8 ORA
	call_S9xGetByte
	Ora8 DIIL

Op17M0:
	DirectIndirectIndexedLong8 ORA2
	call_S9xGetWord
	Ora16 DIIL

Op0DM1:
	Absolute8 ORA
	call_S9xGetByte
	Ora8 ABS

Op0DM0:
	Absolute8 ORA2
	call_S9xGetWord
	Ora16 ABS

Op1DM1:
	AbsoluteIndexedX8 ORA
	call_S9xGetByte
	Ora8 ABSX

Op1DM0:
	AbsoluteIndexedX8 ORA2
	call_S9xGetWord
	Ora16 ABSX

Op19M1:
	AbsoluteIndexedY8 ORA
	call_S9xGetByte
	Ora8 ABSY

Op19M0:
	AbsoluteIndexedY8 ORA2
	call_S9xGetWord
	Ora16 ABSY

Op0FM1:
	AbsoluteLong8 ORA
	call_S9xGetByte
	Ora8 ABSL

Op0FM0:
	AbsoluteLong8 ORA2
	call_S9xGetWord
	Ora16 ABSL

Op1FM1:
	AbsoluteLongIndexedX8 ORA
	call_S9xGetByte
	Ora8 ALX

Op1FM0:
	AbsoluteLongIndexedX8 ORA2
	call_S9xGetWord
	Ora16 ALX

Op03M1:
	StackRelative8 ORA
	call_S9xGetByte
	Ora8 SREL

Op03M0:
	StackRelative8 ORA2
	call_S9xGetWord
	Ora16 SREL

Op13M1:
	StackRelativeIndirectIndexed8 ORA
	call_S9xGetByte
	Ora8 SRII

Op13M0:
	StackRelativeIndirectIndexed8 ORA2
	call_S9xGetWord
	Ora16 SRII

/* ROL */
Op2AM1:
	GetCarry
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.b A,r0
	mov #1,r2
	sub r0,A
	or r2,FLAGS
	rotcl r0
	shll8 r2
	tst r2,r0
	extu.b r0,r0
	bf/s .Rol8Carry
	or r0,A
	add #-Carry,FLAGS
.Rol8Carry:
	SetZNjmp r0

Op2AM0:
	GetCarry
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov #1,r2
	rotcl A
	or r2,FLAGS
	shll16 r2
	tst r2,A
	bf/s .Rol16Carry
	extu.w A,A
	add #-Carry,FLAGS
.Rol16Carry:	
	Set16ZNjmp A

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
	mov #1,r1
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	and FLAGS,r1
	extu.b A,r0
	shll8 r1
	sub r0,A
	or r1,r0
	shlr r0
	mov #Carry,r1
	bt/s .Ror8Carry
	or r1,FLAGS
	add #-Carry,FLAGS
.Ror8Carry:	
	or r0,A
	SetZNjmp r0

Op6AM0:
	mov #1,r1
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	and FLAGS,r1
	extu.w A,A
	shll16 r1
	or r1,A
	shlr A
	mov #Carry,r1
	bt/s .Ror16Carry
	or r1,FLAGS
	add #-Carry,FLAGS
.Ror16Carry:	
	Set16ZNjmp A
	
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
	call_S9xGetByte
	Sbc8 DIR

OpE5M0:
	Direct8 SBC2
	call_S9xGetWord
	Sbc16 DIR

OpF5M1:
	DirectIndexedX8 SBC
	call_S9xGetByte
	Sbc8 DIX

OpF5M0:
	DirectIndexedX8 SBC2
	call_S9xGetWord
	Sbc16 DIX

OpF2M1:
	DirectIndirect8 SBC
	call_S9xGetByte
	Sbc8 DI

OpF2M0:
	DirectIndirect8 SBC2
	call_S9xGetWord
	Sbc16 DI

OpE1M1:
	DirectIndexedIndirect8 SBC
	call_S9xGetByte
	Sbc8 DII

OpE1M0:
	DirectIndexedIndirect8 SBC2
	call_S9xGetWord
	Sbc16 DII

OpF1M1:
	DirectIndirectIndexed8 SBC
	call_S9xGetByte
	Sbc8 DIIY

OpF1M0:
	DirectIndirectIndexed8 SBC2
	call_S9xGetWord
	Sbc16 DIIY

OpE7M1:
	DirectIndirectLong8 SBC
	call_S9xGetByte
	Sbc8 DIL

OpE7M0:
	DirectIndirectLong8 SBC2
	call_S9xGetWord
	Sbc16 DIL

OpF7M1:
	DirectIndirectIndexedLong8 SBC
	call_S9xGetByte
	Sbc8 DIIL

OpF7M0:
	DirectIndirectIndexedLong8 SBC2
	call_S9xGetWord
	Sbc16 DIIL

OpEDM1:
	Absolute8 SBC
	call_S9xGetByte
	Sbc8 ABS

OpEDM0:
	Absolute8 SBC2
	call_S9xGetWord
	Sbc16 ABS

OpFDM1:
	AbsoluteIndexedX8 SBC
	call_S9xGetByte
	Sbc8 ABSX

OpFDM0:
	AbsoluteIndexedX8 SBC2
	call_S9xGetWord
	Sbc16 ABSX

OpF9M1:
	AbsoluteIndexedY8 SBC
	call_S9xGetByte
	Sbc8 ABSY

OpF9M0:
	AbsoluteIndexedY8 SBC2
	call_S9xGetWord
	Sbc16 ABSY

OpEFM1:
	AbsoluteLong8 SBC
	call_S9xGetByte
	Sbc8 ABSL

OpEFM0:
	AbsoluteLong8 SBC2
	call_S9xGetWord
	Sbc16 ABSL

OpFFM1:
	AbsoluteLongIndexedX8 SBC
	call_S9xGetByte
	Sbc8 ALX

OpFFM0:
	AbsoluteLongIndexedX8 SBC2
	call_S9xGetWord
	Sbc16 ALX

OpE3M1:
	StackRelative8 SBC
	call_S9xGetByte
	Sbc8 SREL

OpE3M0:
	StackRelative8 SBC2
	call_S9xGetWord
	Sbc16 SREL

OpF3M1:
	StackRelativeIndirectIndexed8 SBC
	call_S9xGetByte
	Sbc8 SRII

OpF3M0:
	StackRelativeIndirectIndexed8 SBC2
	call_S9xGetWord
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
	BranchCheck0 BCC
	GetNotCarry
	bf .BCC_EXIT
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	mov.l PCBase,r3
	extu.w r4,PC
	add r3,PC
	CPUShutdown BCC
.BCC_EXIT:
	jmp @r13
	nop

/* BCS */
OpB0:
	Relative
	BranchCheck0 BCS
	GetNotCarry
	bt .BCS_EXIT
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	mov.l PCBase,r3
	extu.w r4,PC
	add r3,PC
	CPUShutdown BCS
.BCS_EXIT:
	jmp @r13
	nop

/* BEQ */
OpF0:
	Relative
	BranchCheck2 BEQ
	extu.w r7,r3
	tst r3,r3
	bf .BEQ_EXIT
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	mov.l PCBase,r3
	extu.w r4,PC
	add r3,PC
	CPUShutdown BEQ
.BEQ_EXIT:
	jmp @r13
	nop

/* BMI */
Op30:
	Relative
	BranchCheck1 BMI
	cmp/pz r7
	bt .BMI_EXIT
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	mov.l PCBase,r3
	extu.w r4,PC
	add r3,PC
	CPUShutdown BMI
.BMI_EXIT:
	jmp @r13
	nop

/* BNE */
OpD0:
	Relative
	BranchCheck1 BNE
	extu.w r7,r3
	tst r3,r3
	bt .BNE_EXIT
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	mov.l PCBase,r3
	extu.w r4,PC
	add r3,PC
	CPUShutdown BNE
.BNE_EXIT:
	jmp @r13
	nop

/* BPL */
Op10:
	Relative
	BranchCheck1 BPL
	cmp/pz r7
	bf .BPL_EXIT
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	mov.l PCBase,r3
	extu.w r4,PC
	add r3,PC
	CPUShutdown BPL
.BPL_EXIT:
	jmp @r13
	nop

/* BRA */
Op80:
	Relative
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	extu.w r4,PC
	mov.l PCBase,r3
	jmp @r13
	add r3,PC

/* BVC */
Op50:
	Relative
	BranchCheck0 BVC
	mov #Overflow,r1
	tst r1,FLAGS
	bf .BVC_EXIT:
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	mov.l PCBase,r3
	extu.w r4,PC
	add r3,PC
	CPUShutdown BVC
.BVC_EXIT:
	jmp @r13
	nop

/* BVS */
Op70:
	Relative
	BranchCheck0 BVS
	mov #Overflow,r1
	tst r1,FLAGS
	bt .BVS_EXIT:
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	add #1,CYCLES
#endif
	mov.l PCBase,r3
	extu.w r4,PC
	add r3,PC
	CPUShutdown BVS
.BVS_EXIT:
	jmp @r13
	nop
	
/* BRL */
Op82:
	RelativeLong BRL
	extu.w r4,r4
	mov.l ICPU,r0
	mov.l @(16,r0),r2
	mov.l S9xSetPCBase,r3
	or r2,r4
	Xlink

/* CLC */
Op18:
	mov #~Carry,r0
#ifdef VAR_CYCLES
	add #6,CYCLES
#endif
	jmp @r13
	and r0,FLAGS

/* CLD */
OpD8:
	mov #~Decimal,r0
#ifdef VAR_CYCLES
	add #6,CYCLES
#endif
	jmp @r13
	and r0,FLAGS

/* CLI */
Op58:
	mov #~IRQ,r2
#ifdef VAR_CYCLES
	add #6,CYCLES
#endif
	mov.b IRQActive,r0
	tst r0,r0
	bt .CLI_EXIT
	/* XXX: test for Settings.DisableIRQ */
	mov.l S9xOpcode_IRQ,r3
	and r2,FLAGS
	Xlink
.CLI_EXIT:
	jmp @r13
	and r2,FLAGS

/* CLV */
OpB8:
	mov #~Overflow,r0
#ifdef VAR_CYCLES
	add #6,CYCLES
#endif
	jmp @r13
	and r0,FLAGS

/* DEX */
OpCAX1:
#ifdef VAR_CYCLES
	add #6,CYCLES
#endif
	mov.b XL,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	add #-1,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov.b r0,XL
	SetZNjmp r0

OpCAX0:
#ifdef VAR_CYCLES
	add #6,CYCLES
#endif
	mov.w XX,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	add #-1,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov.w r0,XX
	Set16ZNjmp r0

/* DEY */
Op88X1:
#ifdef VAR_CYCLES
	add #6,CYCLES
#endif
	mov.b YL,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	add #-1,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov.b r0,YL
	SetZNjmp r0

Op88X0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w YY,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	add #-1,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov.w r0,YY
	Set16ZNjmp r0

/* INX */
OpE8X1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b XL,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	add #1,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov.b r0, XL
	SetZNjmp r0

OpE8X0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w XX,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	add #1,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov.w r0, XX
	Set16ZNjmp r0

/* INY */
OpC8X1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b YL,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	add #1,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov.b r0, YL
	SetZNjmp r0

OpC8X0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w YY,r0
#ifdef CPU_SHUTDOWN
	mov #0,r3
#endif
	add #1,r0
#ifdef CPU_SHUTDOWN
	mov.l r3,WaitAddress
#endif
	mov.w r0,YY
	Set16ZNjmp r0

/* NOP */
OpEA:
	jmp @r13
#ifdef VAR_CYCLES
	add #6,CYCLES
#else
	nop
#endif

/* PEA */
OpF4:
	Immediate16 PEA
	PushWordjmp PEA

/* PEI */
OpD4:
	DirectIndirect8 PEI
	extu.w r4,r0
	PushWordjmp PEI

/* PER */
Op62:
	RelativeLong PER
	extu.w r4,r0
	PushWordjmp PER

/* PHA */
Op48M1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.b A,r0
	PushBytejmp PHA

Op48M0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.w A,r0
	PushWordjmp PHA

/* PHB */
Op8B:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b DB, r0
	PushBytejmp PHB

/* PHD */
Op0B:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w DD, r0
	PushWordjmp PHD

/* PHK */
Op4B:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b PB, r0
	PushBytejmp PHK

/* PHP */
Op08:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	S9xPackStatus PHP
	extu.b FLAGS, r0
	PushBytejmp PHP

/* PHX */
OpDAX1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b XL, r0
	PushBytejmp PHX

OpDAX0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w XX, r0
	PushWordjmp PHX

/* PHY */
Op5AX1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b YL, r0
	PushBytejmp PHY

Op5AX0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w YY, r0
	PushWordjmp PHY

/* PLA */
Op68M1:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullByte PLA
	extu.b A,r1
	add r0,A
	sub r1,A
	SetZNjmp A

Op68M0:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullWord PLA
	extu.w r0,A
	Set16ZNjmp A

/* PLB */
OpAB:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullByte PLB
	mov.b r0, DB
	extu.b r0,r1
	mov.l ICPU,r2
	shll16 r1
	mov.l r1,@(20,r2)
	SetZNjmp r0

/* PLD */
Op2B:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullWord PLD
	mov.w r0 DD
	Set16ZNjmp r0
	
/* PLP */
Op28:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullByte PLP
	extu.b FLAGS,r1
	add r0,FLAGS
	mov #IndexFlag,r2
	sub r1,FLAGS
	tst r2,r0
	bt .PLP16
	mov #0,r0
	mov.b r0,XH
	mov.b r0,YH
.PLP16:
	S9xUnpackStatus PLP
	S9xFixCycles PLP
	CheckForIrqjmp PLP /* Removed in 1.39. See asmops.h */

/* PLX */
OpFAX1:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullByte PLX
	mov.b r0, XL
	SetZNjmp r0

OpFAX0:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullWord PLX
	mov.w r0, XX
	Set16ZNjmp r0

/* PLY */
Op7AX1:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullByte PLY
	mov.b r0, YL
	SetZNjmp r0

Op7AX0:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullWord PLY
	mov.w r0, YY
	Set16ZNjmp r0

/* SEC */
Op38:
	mov #Carry,r0
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	jmp @r13
	or r0,FLAGS

/* SED */
OpF8:
	
	mov #Decimal,r0
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	jmp @r13
	or r0,FLAGS

/* SEI */
Op78:
	mov #IRQ,r0
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	jmp @r13
	or r0,FLAGS

/* TAX */
OpAAX1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.b A,r0
	mov.b r0, XL
	SetZNjmp r0

OpAAX0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.w A,r0
	mov.w r0, XX
	Set16ZNjmp r0

/* TAY */
OpA8X1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.b A,r0
	mov.b r0, YL
	SetZNjmp r0

OpA8X0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.w A,r0
	mov.w r0, YY
	Set16ZNjmp r0

/* TCD */
Op5B:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.w A,r0
	mov.w r0, DD
	Set16ZNjmp r0

/* TCS */
Op1B:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	extu.w A,r0
	mov #Emulation/4,r1
	mov.w r0, SS
	shll2 r1
	tst r1,FLAGS16
	bt .TCS_EXIT
	mov #1,r0
	mov.b r0,SH
.TCS_EXIT:
	jmp @r13
	nop

/* TDC */
Op7B:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w DD, r0
	extu.w r0,A
	Set16ZNjmp A

/* TSC */
Op3B:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w SS, r0
	extu.w r0,A
	Set16ZNjmp A

/* TSX */
OpBAX1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b SL, r0
	mov.b r0, XL
	SetZNjmp r0

OpBAX0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w SS, r0
	mov.w r0, XX
	Set16ZNjmp r0

/* TXA */
Op8AM1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b XL, r0
	extu.b A,r1
	extu.b r0,r0
	sub r1,A
	add r0,A
	SetZNjmp A

Op8AM0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w XX, r0
	extu.w r0,A
	Set16ZNjmp A

/* TXS */
Op9A:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w XX, r0
	mov #Emulation/4,r1
	mov.w r0, SS
	shll2 r1
	tst r1,FLAGS16
	bt .TXS_EXIT
	mov #1,r0
	mov.b r0,SH
.TXS_EXIT:
	jmp @r13
	nop

/* TXY */
Op9BX1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b XL, r0
	mov.b r0, YL
	SetZNjmp r0

Op9BX0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w XX, r0
	mov.w r0, YY
	Set16ZNjmp r0

/* TYA */
Op98M1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b YL, r0
	extu.b A,r1
	extu.b r0,r0
	sub r1,A
	add r0,A
	SetZNjmp A

Op98M0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w YY, r0
	extu.w r0,A
	Set16ZNjmp A

/* TYX */
OpBBX1:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.b YL, r0
	mov.b r0, XL
	SetZNjmp r0

OpBBX0:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.w YY, r0
	mov.w r0, XX
	Set16ZNjmp r0

/* XCE */
OpFB:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov FLAGS16, r0
	mov.w .notemcymask,r1
	and r1,FLAGS16
	tst #Carry,r0
	mov.w .emflag0,r1
	bt .XCE_NO_CARRY
	or r1,FLAGS16
.XCE_NO_CARRY:
	tst r1,r0
	bt/s .XCE_NO_EMU
	tst r1,FLAGS
	add #Carry,FLAGS
.XCE_NO_EMU:
	bt/s .XCE_NO_EMULATION2
	mov #IndexFlag,r1
	mov #MemoryFlag|IndexFlag,r0
	or r0,FLAGS
	mov #1,r0
	mov.b r0,SH
.XCE_NO_EMULATION2:
	tst r1,FLAGS
	bt .XCE_NO_INDEX
	mov #0,r0
	mov.b r0, XH
	mov.b r0, YH
.XCE_NO_INDEX:
	S9xFixCycles XCE
	jmp @r13
	nop

.emflag0:
	.word	Emulation

Op00:
#ifdef DEBUGGER
	testb $TRACE_FLAG, Flags
	je .BRK_NO_TRACE
	pushl $.LC0
	ccall S9xTraceMessage
	addl $4,%esp
.BRK_NO_TRACE:
#endif
	mov #1,r0
	mov.b r0,BRKTriggered
	mov.w .emflag,r0
	tst r0,FLAGS16
	bf .BRK_EMULATION
	mov.b PB, r0
	PushByte BRK1
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	add #1,r0
	PushWord BRK2
	
	S9xPackStatus BRK
	extu.b FLAGS, r0
	mov #Decimal|IRQ,r1
	or r1,FLAGS
	add #-Decimal, FLAGS
	
	PushByte BRK3
	mov.l ICPU,r3
	mov #0,r0
	mov.l r0,@(16,r3)
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #12, CYCLES
#else
	add #8, CYCLES
#endif
	mov.l S9xGetWord,r3
	mov #-0x1a,r4
	jsr @r3
	extu.w r4,r4
	mov.l S9xSetPCBase,r3
	extu.w r0,r4
	Xlink
.BRK_EMULATION:
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	add #1,r0
	PushWord BRK2
	
	S9xPackStatus BRK2
	extu.b FLAGS, r0
	mov #Decimal|IRQ,r1
	or r1,FLAGS
	add #-Decimal, FLAGS
	
	PushByte BRK3
	mov.l ICPU,r3
	mov #0,r0
	mov.l r0,@(16,r3)
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #6, CYCLES
#else
	add #6, CYCLES
#endif
	mov.l S9xGetWord,r3
	mov #-2,r4
	jsr @r3
	extu.w r4,r4
	mov.l S9xSetPCBase,r3
	extu.w r0,r4
	Xlink

.notemcymask:
	.word	~(Emulation|Carry)
.emflag:
	.word	Emulation

#ifdef DEBUGGER
	.align 4
.LC0:
	.ascii	"*** BRK"
	.byte 0
#endif

	
.globl S9xOpcode_IRQ_r
S9xOpcode_IRQ_r:
#ifdef DEBUGGER
	testb $TRACE_FLAG, Flags
	je .IRQ_NO_TRACE
	pushl $.LC1
	ccall S9xTraceMessage
	addl $4, %esp
.IRQ_NO_TRACE:
#endif
	mov.w .emflag1,r0
	sts.l pr,@-r15
	tst r0,FLAGS16
	bf .IRQ_EMULATION
	mov.b PB, r0
	PushByte IRQ1
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	PushWord IRQ2
	
	S9xPackStatus IRQ
	extu.b FLAGS, r0
	mov #Decimal|IRQ,r1
	or r1,FLAGS
	add #-Decimal, FLAGS
	
	PushByte IRQ3
	mov.l ICPU,r3
	mov #0,r0
	mov.l r0,@(16,r3)
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #12, CYCLES
#else
	add #8, CYCLES
#endif
	mov.l Settings,r3
	add #82,r3	! Settings.SA1
	mov.b @r3,r0
	tst r0,r0
	bt .noirqsa1vector
	mov.l .FillRAM2,r3
	mov.l @r3,r3
	mov.w .n2209,r0
	add r0,r3
	mov.b @r3,r0
	tst #0x40,r0
	bt .noirqsa1vector
	add #5,r3
	lds.l @r15+,pr
	mov.w @r3,r4
	mov.l S9xSetPCBase,r3
	jmp @r3
	extu.w r4,r4
.noirqsa1vector:
	mov.l S9xGetWord,r3
	mov #-0x12,r4
	jsr @r3
	extu.w r4,r4
	mov.l S9xSetPCBase,r3
	lds.l @r15+,pr
	jmp @r3
	extu.w r0,r4
.IRQ_EMULATION:
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	PushWord IRQ4
	
	S9xPackStatus IRQ2
	extu.b FLAGS, r0
	mov #Decimal|IRQ,r1
	or r1,FLAGS
	add #-Decimal, FLAGS
	
	PushByte IRQ5
	mov.l ICPU,r3
	mov #0,r0
	mov.l r0,@(16,r3)
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #6, CYCLES
#else
	add #6, CYCLES
#endif
	mov.l S9xGetWord,r3
	mov #-0x2,r4
	jsr @r3
	extu.w r4,r4
	mov.l S9xSetPCBase,r3
	lds.l @r15+,pr
	jmp @r3
	extu.w r0,r4

.emflag1:
	.word	Emulation

#ifdef DEBUGGER
	.align 4
.LC1:
	.ascii	"*** IRQ"
	.byte 0
#endif
	.align 4
.FillRAM2:
	.long FillRAM
.n2209:
	.word 0x2209

	
.globl S9xOpcode_NMI_r

S9xOpcode_NMI_r:
#if 0
	PUSH_REGISTERS
	sts.l pr,@-r15
	mova .foofoo,r0
	mov.l @r0+,r1
	jsr @r1
	mov r0,r4
	lds.l @r15+,pr
	POP_REGISTERS
#endif
#ifdef DEBUGGER
	testb $TRACE_FLAG, Flags
	je .NMI_NO_TRACE
	pushl $.LC2
	ccall S9xTraceMessage
	addl $4, %esp
.NMI_NO_TRACE:
#endif
	mov.w .emflag2,r0
	sts.l pr,@-r15
	tst r0,FLAGS16
	bf .NMI_EMULATION
	mov.b PB, r0
	PushByte NMI1
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	PushWord NMI2

	S9xPackStatus NMI
	extu.b FLAGS, r0
	mov #Decimal|IRQ,r1
	or r1,FLAGS
	add #-Decimal, FLAGS
	
	PushByte NMI3
	mov.l ICPU,r3
	mov #0,r0
	mov.l r0,@(16,r3)
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #12, CYCLES
#else
	add #8, CYCLES
#endif
	mov.l S9xGetWord,r3
	mov #-0x16,r4
	jsr @r3
	extu.w r4,r4
	mov.l S9xSetPCBase,r3
	lds.l @r15+,pr
	jmp @r3
	extu.w r0,r4
.NMI_EMULATION:
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	PushWord NMI4
	
	S9xPackStatus NMI2
	extu.b FLAGS, r0
	mov #Decimal|IRQ,r1
	or r1,FLAGS
	add #-Decimal, FLAGS
	
	PushByte NMI5
	mov.l ICPU,r3
	mov #0,r0
	mov.l r0,@(16,r3)
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #6, CYCLES
#else
	add #6, CYCLES
#endif
	mov.l S9xGetWord,r3
	mov #-0x6,r4
	jsr @r3
	extu.w r4,r4
	mov.l S9xSetPCBase,r3
	lds.l @r15+,pr
	jmp @r3
	extu.w r0,r4

.emflag2:
	.word	Emulation

#ifdef DEBUGGER
	.align 4
.LC2:
	.ascii	"*** NMI"
	.byte 0
#endif

	.align 2
.foofoo:
	.long _reportf
	.ascii "[NMI]"
	.byte 10,0
	.align 1
	
Op02:
#ifdef DEBUGGER
	testb $TRACE_FLAG, Flags
	je .COP_NO_TRACE
	pushl $.LC3
	ccall S9xTraceMessage
	addl $4,%esp
.COP_NO_TRACE:
#endif
	mov.w .emflag25,r0
	tst r0,FLAGS16
	bf .COP_EMULATION
	mov.b PB, r0
	PushByte COP1
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	add #1,r0
	PushWord COP2
	
	S9xPackStatus COP
	extu.b FLAGS, r0
	mov #Decimal|IRQ,r1
	or r1,FLAGS
	add #-Decimal, FLAGS
	
	PushByte COP3
	mov.l ICPU,r3
	mov #0,r0
	mov.l r0,@(16,r3)
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #12, CYCLES
#else
	add #8, CYCLES
#endif
	mov.l S9xGetWord,r3
	mov #-0x1c,r4
	jsr @r3
	extu.w r4,r4
	mov.l S9xSetPCBase,r3
	extu.w r0,r4
	Xlink
.COP_EMULATION:
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	add #1,r0
	PushWord COP4
	
	S9xPackStatus COP2
	extu.b FLAGS, r0
	mov #Decimal|IRQ,r1
	or r1,FLAGS
	add #-Decimal, FLAGS
	
	PushByte COP5
	mov.l ICPU,r3
	mov #0,r0
	mov.l r0,@(16,r3)
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #6, CYCLES
#else
	add #6, CYCLES
#endif
	mov.l S9xGetWord,r3
	mov #-0xc,r4
	jsr @r3
	extu.w r4,r4
	mov.l S9xSetPCBase,r3
	extu.w r0,r4
	Xlink

.emflag25:
	.word	Emulation

	
#ifdef DEBUGGER
	.align 4
.LC3:
	.ascii	"*** COP"
	.byte 0
#endif
	
/* JML */
OpDC:
	AbsoluteIndirectLong8 JML
	mov.l ICPU,r3
	mov.l r0,@(16,r3)
	shlr16 r0
	mov.l S9xSetPCBase,r3
	mov.b r0,PB
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	Xlink

Op5C:
	AbsoluteLong8 JML
	mov.l ICPU,r3
	mov.l r0,@(16,r3)
	shlr16 r0
	mov.l S9xSetPCBase,r3
	mov.b r0,PB
	Xlink

/* JMP */
Op4C:
	Absolute8 JMP
	mov.l ICPU,r3
	extu.w r4,r4
	mov.l @(16,r3),r0
	mov.l S9xSetPCBase,r3
	or r0,r4
	Xlink

Op6C:
	AbsoluteIndirect8 JMP
	mov.l ICPU,r3
	extu.w r4,r4
	mov.l @(16,r3),r0
	mov.l S9xSetPCBase,r3
	or r0,r4
	Xlink

Op7C:
	AbsoluteIndexedIndirect8 JMP
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.l ICPU,r3
	extu.w r4,r4
	mov.l @(16,r3),r0
	mov.l S9xSetPCBase,r3
	or r0,r4
	Xlink

/* JSL */
Op22:
	mov.b PB, r0
	PushByte JSL_ABSL
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	add #2,r0
	PushWord JSL_ABSL
	AbsoluteLong8 JSL
	mov.l ICPU,r3
	mov.l r0,@(16,r3)
	shlr16 r0
	mov.l S9xSetPCBase,r3
	mov.b r0,PB
	Xlink

/* RTL */
Op6B:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullWord RTL
	mov.l r0,@-r15
	PullByte RTL
	mov.l @r15+,r4
	mov.b r0, PB
	add #1,r4
	shll16 r0
	extu.w r4,r4
	mov.l ICPU,r3
	mov.l r0,@(16,r3)
	mov.l S9xSetPCBase,r3
	or r0,r4
	Xlink

/* JSR ABS */
Op20:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	add #1,r0
	PushWord JSR_ABS
	Absolute8 JSR_ABS
	mov.l ICPU,r3
	extu.w r4,r4
	mov.l @(16,r3),r0
	mov.l S9xSetPCBase,r3
	or r0,r4
	Xlink

/* JSR ABS INDEXED INDIRECT */
OpFC:
#ifdef VAR_CYCLES
	add #6, CYCLES
#endif
	mov.l PCBase,r0
	neg r0,r0
	add PC, r0
	add #1,r0
	PushWord JSR_AII
	AbsoluteIndexedIndirect8 JSR
	mov.l ICPU,r3
	extu.w r4,r4
	mov.l @(16,r3),r0
	mov.l S9xSetPCBase,r3
	or r0,r4
	Xlink

/* RTS */
Op60:
#ifdef VAR_CYCLES
	add #6 * 3, CYCLES
#endif
	PullWord RTS
	add #1,r0
	mov.l ICPU,r3
	extu.w r0,r4
	mov.l @(16,r3),r0
	mov.l S9xSetPCBase,r3
	or r0,r4
	Xlink

/* MVN */
Op54X1:
#ifdef VAR_CYCLES
	mov.l MemSpeedx2,r2
	add r2,CYCLES
	add #12,CYCLES
#endif
	mov.b @PC+,r0
	mov.b r0,DB
	extu.b r0,r0
	mov.l ICPU,r1
	shll16 r0
	mov.l r0,ShiftedDB
	mov.l r0,@-r15
	mov.b @PC+,r4
	mov.l S9xGetByte,r3
	extu.b r4,r4
	mov.w XX,r0
	shll16 r4
	extu.w r0,r0
	jsr @r3
	add r0,r4
	mov r0,r5
	mov.w YY,r0
	mov.l S9xSetByte,r3
	mov.l @r15+,r4
	extu.w r0,r1
	mov r5,r0
	jsr @r3
	add r1,r4
	mov.b XL,r0
	add #1,r0
	mov.b r0,XL
	mov.b YL,r0
	add #1,r0
	mov.b r0,YL
	add #-1,A
	cmp/pz A
	bf/s .MVN_EXIT8
	extu.w A,A
	jmp @r13
	add #-3,PC
.MVN_EXIT8:
	jmp @r13
	nop

Op54X0:
#ifdef VAR_CYCLES
	mov.l MemSpeedx2,r2
	add r2,CYCLES
	add #12,CYCLES
#endif
	mov.b @PC+,r0
	mov.b r0,DB
	extu.b r0,r0
	mov.l ICPU,r1
	shll16 r0
	mov.l r0,ShiftedDB
	mov.l r0,@-r15
	mov.b @PC+,r4
	mov.l S9xGetByte,r3
	extu.b r4,r4
	mov.w XX,r0
	shll16 r4
	extu.w r0,r1
	add #1,r0
	add r1,r4
	jsr @r3
	mov.w r0,XX
	mov r0,r5
	mov.w YY,r0
	mov.l S9xSetByte,r3
	mov.l @r15+,r4
	extu.w r0,r1
	add #1,r0
	add r1,r4
	mov.w r0,YY
	jsr @r3
	mov r5,r0
	add #-1,A
	cmp/pz A
	bf/s .MVN_EXIT16
	extu.w A,A
	jmp @r13
	add #-3,PC
.MVN_EXIT16:
	jmp @r13
	nop

/* MVP */
Op44X1:
#ifdef VAR_CYCLES
	mov.l MemSpeedx2,r2
	add r2,CYCLES
	add #12,CYCLES
#endif
	mov.b @PC+,r0
	mov.b r0,DB
	extu.b r0,r0
	mov.l ICPU,r1
	shll16 r0
	mov.l r0,ShiftedDB
	mov.l r0,@-r15
	mov.b @PC+,r4
	mov.l S9xGetByte,r3
	extu.b r4,r4
	mov.w XX,r0
	shll16 r4
	extu.w r0,r0
	jsr @r3
	add r0,r4
	mov r0,r5
	mov.w YY,r0
	mov.l S9xSetByte,r3
	mov.l @r15+,r4
	extu.w r0,r1
	mov r5,r0
	jsr @r3
	add r1,r4
	mov.b XL,r0
	add #-1,r0
	mov.b r0,XL
	mov.b YL,r0
	add #-1,r0
	mov.b r0,YL
	add #-1,A
	cmp/pz A
	bf/s .MVP_EXIT8
	extu.w A,A
	jmp @r13
	add #-3,PC
.MVP_EXIT8:
	jmp @r13
	nop

Op44X0:
#ifdef VAR_CYCLES
	mov.l MemSpeedx2,r2
	add r2,CYCLES
	add #12,CYCLES
#endif
	mov.b @PC+,r0
	mov.b r0,DB
	extu.b r0,r0
	mov.l ICPU,r1
	shll16 r0
	mov.l r0,ShiftedDB
	mov.l r0,@-r15
	mov.b @PC+,r4
	mov.l S9xGetByte,r3
	extu.b r4,r4
	mov.w XX,r0
	shll16 r4
	extu.w r0,r1
	add #-1,r0
	add r1,r4
	jsr @r3
	mov.w r0,XX
	mov r0,r5
	mov.w YY,r0
	mov.l S9xSetByte,r3
	mov.l @r15+,r4
	extu.w r0,r1
	add #-1,r0
	add r1,r4
	mov.w r0,YY
	jsr @r3
	mov r5,r0
	add #-1,A
	cmp/pz A
	bf/s .MVP_EXIT16
	extu.w A,A
	jmp @r13
	add #-3,PC
.MVP_EXIT16:
	jmp @r13
	nop

/* REP */
OpC2:
#ifdef VAR_CYCLES
	mov.l MemSpeed,r2
	add r2,CYCLES
	add #6,CYCLES
#endif
	mov.b @PC+,r0
	mov #Zero,r1
	extu.b r0,r0
	mov #-1,r2
	and r0,r1
	not r0,r0
	or r1,r7
	and r0,FLAGS
	extu.w r2,r2
	exts.b r0,r1
	mov.w .emflag3,r0
	or r2,r1
	tst r0,FLAGS16
	bt/s .REP_NO_EMU
	and r1,r7
	mov #(MemoryFlag | IndexFlag), r0
	or r0, FLAGS
	
.REP_NO_EMU:
	mov #IndexFlag,r0
	tst r0,FLAGS
	bt .REP16
	mov #0,r0
	mov.b r0,XH
	mov.b r0,YH
.REP16:
	S9xFixCycles REP
	CheckForIrqjmp REP /* Removed in 1.39. See asmops.h */

/* SEP */
OpE2:
#ifdef VAR_CYCLES
	mov.l MemSpeed,r2
	add r2,CYCLES
	add #6,CYCLES
#endif
	mov.b @PC+,r0
	mov #Zero,r1
	extu.b r0,r0
	and r0,r1
	exts.b r0,r2
	shll8 r1
	or r0,FLAGS
	shlr2 r1
	extu.w r2,r0
	exts.b r1,r1
	sub r0,r2
	shlr16 r1
	or r2,r7
	mov.w .emflag3,r0
	not r1,r1
	tst r0,FLAGS16
	bt/s .SEP_NO_EMU
	and r1,r7

	mov #(MemoryFlag | IndexFlag), r0
	or r0, FLAGS
	
.SEP_NO_EMU:
	mov #IndexFlag,r0
	tst r0,FLAGS
	bt .SEP16
	mov #0,r0
	mov.b r0,XH
	mov.b r0,YH
.SEP16:
	S9xFixCycles SEP
	jmp @r13
	nop


/* XBA */
OpEB:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	swap.b A,A
	SetZNjmp A

/* RTI */
Op40:
#ifdef VAR_CYCLES
	add #12, CYCLES
#endif
	PullByte RTI
	extu.b FLAGS,r1
	add r0,FLAGS
	sub r1,FLAGS
	PullWord RTI
	mov.w .emflag3,r1
	mov.l r0,@-r15
	tst r1, FLAGS16
	bf .RTI_EMU
	PullByte RTI2
	bra .RTI_SKIP_EMU
	extu.b r0,r0
.RTI_EMU:
	mov #(MemoryFlag | IndexFlag), r0
	mov.l ICPU,r3
	or r0,FLAGS
	/*XXX: HERE xorl %eax, %eax*/
	mov.l @(16,r3),r0
	shlr16 r0
.RTI_SKIP_EMU:
	mov.b r0,PB
	mov.l @r15+,r4
	shll16 r0
	mov.l ICPU,r3
	mov.l r0,@(16,r3)

	mov.l S9xSetPCBase,r3
	jsr @r3
	or r0,r4
	mov #IndexFlag,r0
	tst r0,FLAGS
	bt .RTI16
	mov #0,r0
	mov.b r0,XH
	mov.b r0,YH
.RTI16:
	S9xUnpackStatus RTI
	S9xFixCycles RTI
	CheckForIrqjmp RTI /* Removed in 1.39. See asmops.h */

.emflag3:
	.word	Emulation

/* WAI */
OpCB:
#if 0
	cmpb $0, IRQActive
	je .L5334
#ifdef VAR_CYCLES
	addl $12, CYCLES
#else
	addl $2, CYCLES
#endif
	jmp MainAsmLoop
.L5334:
#endif
	mov #1,r0
	add #-1,PC
	mov.b r0, WaitingForInterrupt

	mov.l Settings,r1
	mov.b Shutdown,r0
	tst r0,r0
	bt .NoShutdown

#ifdef ARMAPU
	mov #0,r2
	mov.l NextEvent, CYCLES
	jmp @r13
	mov.l r2,WaitAddress
#else
	mov.l IAPU,r1
	mov #0,r2
	mov.b APUExecuting,r0
	mov.l NextEvent, CYCLES
	tst r0,r0
	mov.l r2,WaitAddress
	bt .EndWAIShutdown

	mov.l ICPU,r1
	mov #0,r0
	mov.b r0, CPUExecuting
.WAITExecAPU:
#ifdef DEBUGGER
	testb $2,APUFlags
	je .WAITNoAPUS9xTrace
	STORE_REGISTERS
	ccall S9xTraceAPU
	LOAD_REGISTERS
.WAITNoAPUS9xTrace:
#endif
#ifdef SPC700_C
	mov.l IAPU,r1
	SAVE_CYCLES
	mov.l APUPC,r0

	PUSH_REGISTERS
	mov.b @r0,r0
#else
	mov.b @APUPC,r0
#endif
	mov.l .WAIAPUCycles,r1
	extu.b r0,r0
	mov.l .WAIApuOpcodes,r2
	shll2 r0
	mov.l @(r0,r1),r4
	mov.l @(r0,r2),r0
	mov.l APU,r1
	mov.l APUCycles,r2
	add r4,r2
	jsr @r0
	mov.l r2,APUCycles
#ifdef SPC700_C
	POP_REGISTERS
	LOAD_CYCLES
#endif
	mov.l APU,r1
	mov.l NextEvent,r2
	mov.l APUCycles,r3
	cmp/ge r2,r3
	bf .WAITExecAPU
	mov.l ICPU,r1
	mov #1,r0
	jmp @r13
	mov.b r0, CPUExecuting

.EndWAIShutdown:
	jmp @r13
	nop

	.align 2
.WAIAPUCycles:	
	.long _S9xAPUCycles
.WAIApuOpcodes:
	.long _S9xApuOpcodes
#endif /* ARMAPU */

.NoShutdown:
	jmp @r13
#ifdef VAR_CYCLES
	add #12, CYCLES
#else
	add #2, CYCLES
#endif

/* ??? */
OpDB:
	mov.l Flags,r0
	add #-1,PC
	or #DEBUG_MODE_FLAG, r0
	jmp @r13
	mov.l r0,Flags

Op42:
	jmp @r13
	nop

.globl _S9xOpcodesM1X1

	.align 4
_S9xOpcodesM1X1:
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

.globl _S9xOpcodesM1X0
	.align 4
_S9xOpcodesM1X0:
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

.globl _S9xOpcodesM0X0
	.align 4
_S9xOpcodesM0X0:
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

.globl _S9xOpcodesM0X1
	.align 4
_S9xOpcodesM0X1:
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




	

.globl _DataWatch
TraceFunc:
	mov.l r13,@-r15
	sts.l pr,@-r15
	mov.l r12,@-r15
	mov.l r11,@-r15
	mov.l r14,@-r15
	mov.l r10,@-r15
	mov.l r9,@-r15
	mov.l r8,@-r15
	SETUP_PREDEF
	LOAD_REGISTERS CPUTrace

	mov.w DD,r0
	cmp/eq #1,r0
	bf 4f

	mov FLAGS16,r7
	mov.l PCBase,r0
	mov PC,r1
	sub r0,r1
	mov.l ICPU,r0
	mov.l @(16,r0),r0
	add r0,r1
	mov r1,r6
	mova .zfoorpt,r0
	mov.l @r0+,r1
	mov r4,r5
	jsr @r1
	mov r0,r4
4:	
	mov.l @r15+,r8
	mov.l @r15+,r9
	mov.l @r15+,r10
	mov.l @r15+,r14
	mov.l @r15+,r11
	mov.l @r15+,r12
	lds.l @r15+,pr
	rts
	mov.l @r15+,r13

	MISC

	.align 2
.zfoorpt:
	.long _reportf
	.ascii "@%x: PC = %x, flags = %x\n"
	.byte 0

	.align 1



_DataWatch:
	mova	exc_handler,r0
	tst	r4,r4
	.word	0x40fa		! ldc	r0,dbr
	bt	trace_off
	
trace_on:
	mov.l   .DDReg,r0
	mov.l	ubcbase,r1
	mov.l	r0,@r1
	mov	#0,r0
	mov.b	r0,@(4,r1)
	add	#8,r1
	mov.w	crval,r0
	mov.w	r0,@(0x18, r1)
	mov	#0x28,r0
	mov.w	r0,@r1
	mov	#0x1a,r0
	mov.w	r0,@(0xc,r1)
	mov.l	srmask,r1
	stc	sr,r0
	and	r1,r0
	ldc	r0,sr
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	rts
	nop


trace_off:
!	mov.l	srset,r1
!	stc	sr,r0
!	or	r1,r0
!	ldc	r0,sr
	mov.l	ubcbase,r1
	mov	#0x1a,r0
	mov.w	r0,@(8,r1)
	add	#0x20,r1
	mov.w	mask,r2
	mov.w	@r1,r0
	and	r2,r0
	mov.w	r0,@r1
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	rts
	nop


	.align	4	

exc_handler:
	sts.l	pr,@-r15
	sts.l	mach,@-r15
	sts.l	macl,@-r15
	mov.l	ubcbase,r1
	stc	spc,r0
!	mov.l	@r1,r4
!	mov.l	r0,@r1
	bsr TraceFunc
	mov	r0,r4
	lds.l	@r15+,macl
	lds.l	@r15+,mach
	lds.l	@r15+,pr
	mov.l	ubcbase,r0
	add	#0x20,r0
	mov.w	mask,r2
	mov.w	@r0,r1
	and	r2,r1
	mov.w	r1,@r0
	stc	sgr,r15
	rte
	nop	

	.align	4	

.DDReg:
	.long	_Registers+6
ubcbase:
	.long	0xff200000
srmask:
	.long	0xcfffffff
!srset:
!	.long	0x10000000
crval:	
	.word	0x401
mask:
	.word	0x3fff
