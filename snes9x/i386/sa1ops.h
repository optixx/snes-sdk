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

.macro Adc8 K
	testb $Decimal, FLAGS
	jnz .ADC8Decimal\K
	GetCarry
	adcb AL, %al
	SetZNCV
	movb %al, AL
	jmp S9xSA1MainAsmLoop

.ADC8Decimal\K:
	GetCarry
	adcb AL, %al
	daa
	movb %al, AL
	SetZNCV
	jmp S9xSA1MainAsmLoop
.endm

.macro Adc16 K
	testb $Decimal, FLAGS
	jnz .ADC16Decimal\K
	GetCarry
	adcw AA, %ax
	movw %ax, AA
	setc SA1_Carry
	seto SA1_Overflow
	setnz SA1_Zero
	movb %ah, SA1_Negative
	jmp S9xSA1MainAsmLoop

.ADC16Decimal\K:
	GetCarry
	adcb AL, %al
	daa
	movb %al, AL
	movb %ah, %al
	adcb AH, %al
	daa
	movb %al, AH
	setc SA1_Carry
	seto SA1_Overflow
	movw AA, %ax
	movb %ah, SA1_Negative
	orb %ah, %al
	movb %al, SA1_Zero
	jmp S9xSA1MainAsmLoop
.endm

.macro Sbc8 K
	testb $Decimal, FLAGS
	jnz .SBC8Decimal\K
	GetNotCarry
	sbbb %al, AL
	movb AL, %al
	setnc SA1_Carry
	SetZNV
	jmp S9xSA1MainAsmLoop

.SBC8Decimal\K:
	GetNotCarry
	movb %al, %dl
	movb AL, %al
	sbbb %dl, %al
	das
	movb %al, AL
	setnc SA1_Carry
	SetZNV
	jmp S9xSA1MainAsmLoop
.endm

.macro Sbc16 K
	testb $Decimal, FLAGS
	jnz .SBC16Decimal\K
	GetNotCarry
	sbbw %ax, AA
	setnc SA1_Carry
	setnz SA1_Zero
	seto SA1_Overflow
	movb AH, %al
	movb %al, SA1_Negative
	jmp S9xSA1MainAsmLoop

.SBC16Decimal\K:
	movl %eax, %edx
	movw AA, %ax
	GetNotCarry
	sbbb %dl, %al
	das
	movb %al, AL
	movb %ah, %al
	sbbb %dh, %al
	das
	movb %al, AH
	setnc SA1_Carry
	seto SA1_Overflow
	movw AA, %ax
	movb %ah, SA1_Negative
	orb %ah, %al
	movb %al, SA1_Zero
	jmp S9xSA1MainAsmLoop
.endm

.macro Ora8 K
	orb AL, %al
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Ora16 K
	orw AA, %ax
	movw %ax, AA
	Set16ZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Eor8 K
	xorb AL, %al
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Eor16 K
	xorw AA, %ax
	movw %ax, AA
	Set16ZN
	jmp S9xSA1MainAsmLoop
.endm

.macro And8 K
	andb AL, %al
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop
.endm

.macro And16 K
	andw AA, %ax
	movw %ax, AA
	Set16ZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Lda8 K
	movb %al, AL
	SetZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Lda16 K
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Ldx8 K
	movb %al, XL
	SetZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Ldx16 K
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Ldy8 K
	movb %al, YL
	SetZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Ldy16 K
	movw %ax, YY
	testw %ax, %ax
	Set16ZN
	jmp S9xSA1MainAsmLoop
.endm

.macro Cmp8 K
	movb AL, %ah
	subb %al, %ah
	setnc SA1_Carry
	movb %ah, SA1_Negative
	movb %ah, SA1_Zero
	jmp S9xSA1MainAsmLoop
.endm

.macro Cmp16 K
	movw AA, %dx
	subw %ax, %dx
	setnc SA1_Carry
	setnz SA1_Zero
	movb %dh, SA1_Negative
	jmp S9xSA1MainAsmLoop
.endm

.macro Cpx8 K
	movb XL, %ah
	subb %al, %ah
	setnc SA1_Carry
	movb %ah, SA1_Negative
	movb %ah, SA1_Zero
	jmp S9xSA1MainAsmLoop
.endm

.macro Cpx16 K
	movw XX, %dx
	subw %ax, %dx
	setnc SA1_Carry
	setnz SA1_Zero
	movb %dh, SA1_Negative
	jmp S9xSA1MainAsmLoop
.endm

.macro Cpy8 K
	movb YL, %ah
	subb %al, %ah
	setnc SA1_Carry
	movb %ah, SA1_Negative
	movb %ah, SA1_Zero
	jmp S9xSA1MainAsmLoop
.endm

.macro Cpy16 K
	movw YY, %dx
	subw %ax, %dx
	setnc SA1_Carry
	setnz SA1_Zero
	movb %dh, SA1_Negative
	jmp S9xSA1MainAsmLoop
.endm

.macro Asl8 K
	pushl %edx
	call S9xSA1GetByte
	salb %al
	SetZNC
	popl %edx
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Asl16 K
	pushl %edx
	call S9xSA1GetWord
	salw %ax
	Set16ZNC
	popl %edx
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Lsr8 K
	pushl %edx
	call S9xSA1GetByte
	shrb %al
	SetZNC
	popl %edx
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Lsr16 K
	pushl %edx
	call S9xSA1GetWord
	shrw %ax
	Set16ZNC
	popl %edx
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Rol8 K
	pushl %edx
	call S9xSA1GetByte
	GetCarry
	rclb %al
	SetZNC
	popl %edx
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Rol16 K
	pushl %edx
	call S9xSA1GetWord
	GetCarry
	rclw %ax
	setc SA1_Carry
	movb %ah, SA1_Negative
	testw %ax, %ax
	setnz SA1_Zero
	popl %edx
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Ror8 K
	pushl %edx
	call S9xSA1GetByte
	GetCarry
	rcrb %al
	SetZNC
	popl %edx
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Ror16 K
	pushl %edx
	call S9xSA1GetWord
	GetCarry
	rcrw %ax
	setc SA1_Carry
	movb %ah, SA1_Negative
	testw %ax, %ax
	setnz SA1_Zero
	popl %edx
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Bit8 K
	call S9xSA1GetByte
	movb %al, SA1_Negative
	testb $0x40, %al
	setnz SA1_Overflow
	andb AL, %al
	movb %al, SA1_Zero
	jmp S9xSA1MainAsmLoop
.endm

.macro Bit16 K
	call S9xSA1GetWord
	movb %ah, SA1_Negative
	testb $0x40, %ah
	setnz SA1_Overflow
	andw AA, %ax
	setnz SA1_Zero
	jmp S9xSA1MainAsmLoop
.endm

.macro Dec8 K
	pushl %edx
	call S9xSA1GetByte
	decb %al
	SetZN
	popl %edx
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Dec16 K
	pushl %edx
	call S9xSA1GetWord
	decw %ax
	Set16ZN
	popl %edx
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Inc8 K
	pushl %edx
	call S9xSA1GetByte
	incb %al
	SetZN
	popl %edx
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Inc16 K
	pushl %edx
	call S9xSA1GetWord
	incw %ax
	Set16ZN
	popl %edx
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Sta8 K
	movb AL, %al
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Sta16 K
	movw AA, %ax
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Stx8 K
	movb XL, %al
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Stx16 K
	movw XX, %ax
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Sty8 K
	movb YL, %al
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Sty16 K
	movw YY, %ax
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Stz8 K
	xorb %al, %al
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Stz16 K
	xorl %eax, %eax
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Trb8 K
	pushl %edx
	call S9xSA1GetByte
	movb AL, %dl
	testb %al, %dl
	setnz SA1_Zero
	xorb $0xff, %dl
	andb %dl, %al
	popl %edx
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Trb16 K
	pushl %edx
	call S9xSA1GetWord
	movw AA, %dx
	testw %ax, %dx
	setnz SA1_Zero
	xorl $0xffffffff, %edx
	andl %edx, %eax
	popl %edx
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro Tsb8 K
	pushl %edx
	call S9xSA1GetByte
	movb AL, %dl
	testb %al, %dl
	setnz SA1_Zero
	orb %dl, %al
	popl %edx
	call S9xSA1SetByte
	jmp S9xSA1MainAsmLoop
.endm

.macro Tsb16 K
	pushl %edx
	call S9xSA1GetWord
	movw AA, %dx
	testw %ax, %dx
	setnz SA1_Zero
	orw %dx, %ax
	popl %edx
	call S9xSA1SetWord
	jmp S9xSA1MainAsmLoop
.endm

.macro CheckForIrq K
	testb $0xff, SA1IRQActive
	jz .CheckForIrqS9xSA1Exit\K
	testb $IRQ, FLAGS
	jnz .CheckForIrqS9xSA1Exit\K
	call S9xSA1Opcode_IRQ
.CheckForIrqS9xSA1Exit\K:
.endm

.macro S9xSA1FixCycles K
	testw $Emulation, FLAGS16
	jz .S9xSA1FixCyclesNoEmulation\K
	movl $S9xSA1OpcodesM1X1, SA1Opcodes
	jmp .S9xSA1FixCyclesS9xSA1Exit\K

.S9xSA1FixCyclesNoEmulation\K:	
	testb $MemoryFlag, FLAGS
	jz .S9xSA1FixCyclesNoMemory\K
	testb $IndexFlag, FLAGS
	jz .S9xSA1FixCyclesNoIndex\K
	movl $S9xSA1OpcodesM1X1, SA1Opcodes
	jmp .S9xSA1FixCyclesS9xSA1Exit\K
.S9xSA1FixCyclesNoIndex\K:
	movl $S9xSA1OpcodesM1X0, SA1Opcodes
	jmp .S9xSA1FixCyclesS9xSA1Exit\K

.S9xSA1FixCyclesNoMemory\K:
	testb $IndexFlag, FLAGS
	jz .S9xSA1FixCyclesNoIndex2\K
	movl $S9xSA1OpcodesM0X1, SA1Opcodes
	jmp .S9xSA1FixCyclesS9xSA1Exit\K
.S9xSA1FixCyclesNoIndex2\K:
	movl $S9xSA1OpcodesM0X0, SA1Opcodes
.S9xSA1FixCyclesS9xSA1Exit\K:
.endm

.macro S9xSA1UnpackStatus K
	movb FLAGS, %al
	movb %al, SA1_Negative
	testb $Zero, %al
	setz SA1_Zero
	testb $Carry, FLAGS
	setnz SA1_Carry
	testb $Overflow, FLAGS
	setnz SA1_Overflow
.endm

.macro S9xSA1PackStatus K
	movb SA1_Carry, %al
	andb $~(Zero | Negative | Carry | Overflow), FLAGS
	orb %al, FLAGS
	movb SA1_Negative, %al
	andb $0x80, %al
	orb %al, FLAGS
	movb SA1_Overflow, %al
	salb $6, %al
	orb %al, FLAGS
	testb $0xff, SA1_Zero
	setz %al
	salb %al
	orb %al, FLAGS
.endm

.macro CPUShutdown K
//	cmpb $0, Shutdown
//	jz .NoShutdown\K
	cmpl PC, SA1WaitAddress
	jnz .NoShutdown\K
	cmpl $1, SA1WaitCounter
	jl .NotYet\K
	movb $0, SA1Executing
	jp .NoShutdown\K
.NotYet\K:
	incl SA1WaitCounter
.NoShutdown\K:
.endm

