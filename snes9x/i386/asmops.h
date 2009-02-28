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
	jmp MainAsmLoop

.ADC8Decimal\K:
	mov %al, %ah
	mov AL, %dl
	mov %al, %dh
	andb $0x0f, %dl
	andb $0x0f, %dh
	add %dl, %dh
	GetCarry
	jnc .ADC8DecimalNoCarry\K
	inc %dh
.ADC8DecimalNoCarry\K:
	cmp $0x1a, %dh
	jb .ADC8DecimalNoAdj\K
	andb $0xf7, AL
	andb $0xf7, %al
.ADC8DecimalNoAdj\K:
	GetCarry
	adcb AL, %al
	daa
	movb AL, %dl
	movb %al, AL
	SetZNC
	xor %dl, %ah
	not %ah
	xor %al, %dl
	and %dl, %ah
	and $0x80, %ah
	shr $7, %ah
	mov %ah, _Overflow
	jmp MainAsmLoop
.endm

.macro Adc16 K
	testb $Decimal, FLAGS
	jnz .ADC16Decimal\K
	GetCarry
	adcw AA, %ax
	movw %ax, AA
	setc _Carry
	seto _Overflow
	setnz _Zero
	movb %ah, _Negative
	jmp MainAsmLoop

.ADC16Decimal\K:

	push	%ecx
	push	%esi
	movsx	%ax, %edx
	push	%edx
	push	%edi
	movw	AA, %si
	and	$0xf000, %eax
	mov	%edx, %edi
	shl	$4, %eax
	and	$0x0f00, %edi
	or	%edi, %eax
	mov	%edx, %edi
	shl	$4, %eax
	and	$0x00f0, %edi
	and	$0x000f, %edx 
	or	%edi, %eax
	pop	%edi

	movsx	%si, %ecx
	shl	$4, %eax
	or	%edx, %eax
	and	$0xf000, %esi
	mov	%ecx, %edx
	shl	$4, %esi
	and	$0x0f00, %edx
	or	%edx, %esi
	mov	%ecx, %edx
	shl	$4, %esi
	and	$0x00f0, %edx
	and	$0x000f, %ecx
	or	%edx, %esi
	shl	$4, %esi
	or	%ecx, %esi
	mov	%esi, %ecx

	or	$0x90909090, %eax
	mov	$4, %esi
.ADC16DecimalAddLoop\K:
	GetCarry
	adcb	%cl, %al
	daa
	setc	_Carry
	shr	$8, %ecx
	shrd	$4, %eax, %edx
	shr	$8, %eax
	dec	%esi
	jne	.ADC16DecimalAddLoop\K
	shr	$16, %edx
	pop	%ecx
	mov	AA, %ax

	xor %cx, %ax
	not %ax
	xor %dx, %cx
	and %cx, %ax
	and $0x8000, %ax
	shr $7, %ah
	mov %ah, _Overflow

	movw	%dx, AA
	movb	%dh, _Negative
	orb	%dh, %dl
	movb	%dl, _Zero
	pop	%esi
	pop	%ecx
	jmp MainAsmLoop
.endm

.macro Sbc8 K
	testb $Decimal, FLAGS
	jnz .SBC8Decimal\K
	GetNotCarry
	sbbb %al, AL
	movb AL, %al
	setnc _Carry
	SetZNV
	jmp MainAsmLoop

.SBC8Decimal\K:
	GetNotCarry
	movb %al, %dl
	movb AL, %al
	sbbb %dl, %al
	das
	movb %al, AL
	setnc _Carry
	SetZNV
	jmp MainAsmLoop
.endm

.macro Sbc16 K
	testb $Decimal, FLAGS
	jnz .SBC16Decimal\K
	GetNotCarry
	sbbw %ax, AA
	setnc _Carry
	setnz _Zero
	seto _Overflow
	movb AH, %al
	movb %al, _Negative
	jmp MainAsmLoop

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
	setnc _Carry
	seto _Overflow
	movw AA, %ax
	movb %ah, _Negative
	orb %ah, %al
	movb %al, _Zero
	jmp MainAsmLoop
.endm

.macro Ora8 K
	orb AL, %al
	movb %al, AL
	SetZN
	jmp MainAsmLoop
.endm

.macro Ora16 K
	orw AA, %ax
	movw %ax, AA
	Set16ZN
	jmp MainAsmLoop
.endm

.macro Eor8 K
	xorb AL, %al
	movb %al, AL
	SetZN
	jmp MainAsmLoop
.endm

.macro Eor16 K
	xorw AA, %ax
	movw %ax, AA
	Set16ZN
	jmp MainAsmLoop
.endm

.macro And8 K
	andb AL, %al
	movb %al, AL
	SetZN
	jmp MainAsmLoop
.endm

.macro And16 K
	andw AA, %ax
	movw %ax, AA
	Set16ZN
	jmp MainAsmLoop
.endm

.macro Lda8 K
	movb %al, AL
	SetZN
	jmp MainAsmLoop
.endm

.macro Lda16 K
	movw %ax, AA
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop
.endm

.macro Ldx8 K
	movb %al, XL
	SetZN
	jmp MainAsmLoop
.endm

.macro Ldx16 K
	movw %ax, XX
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop
.endm

.macro Ldy8 K
	movb %al, YL
	SetZN
	jmp MainAsmLoop
.endm

.macro Ldy16 K
	movw %ax, YY
	testw %ax, %ax
	Set16ZN
	jmp MainAsmLoop
.endm

.macro Cmp8 K
	movb AL, %ah
	subb %al, %ah
	setnc _Carry
	movb %ah, _Negative
	movb %ah, _Zero
	jmp MainAsmLoop
.endm

.macro Cmp16 K
	movw AA, %dx
	subw %ax, %dx
	setnc _Carry
	setnz _Zero
	movb %dh, _Negative
	jmp MainAsmLoop
.endm

.macro Cpx8 K
	movb XL, %ah
	subb %al, %ah
	setnc _Carry
	movb %ah, _Negative
	movb %ah, _Zero
	jmp MainAsmLoop
.endm

.macro Cpx16 K
	movw XX, %dx
	subw %ax, %dx
	setnc _Carry
	setnz _Zero
	movb %dh, _Negative
	jmp MainAsmLoop
.endm

.macro Cpy8 K
	movb YL, %ah
	subb %al, %ah
	setnc _Carry
	movb %ah, _Negative
	movb %ah, _Zero
	jmp MainAsmLoop
.endm

.macro Cpy16 K
	movw YY, %dx
	subw %ax, %dx
	setnc _Carry
	setnz _Zero
	movb %dh, _Negative
	jmp MainAsmLoop
.endm

.macro Asl8 K
	pushl %edx
	call S9xGetByte
	addl $6, CYCLES
	salb %al
	SetZNC
	popl %edx
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Asl16 K
	pushl %edx
	call S9xGetWord
	addl $6, CYCLES
	salw %ax
	Set16ZNC
	popl %edx
#if 0
	call S9xSetWord
#else
	pushl %eax
	pushl %edx
	incl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	call S9xSetByte
#endif
	jmp MainAsmLoop
.endm

.macro Lsr8 K
	pushl %edx
	call S9xGetByte
	addl $6, CYCLES
	shrb %al
	SetZNC
	popl %edx
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Lsr16 K
	pushl %edx
	call S9xGetWord
	addl $6, CYCLES
	shrw %ax
	Set16ZNC
	popl %edx
#if 0
	call S9xSetWord
#else
	pushl %eax
	pushl %edx
	incl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	call S9xSetByte
#endif
	jmp MainAsmLoop
.endm

.macro Rol8 K
	pushl %edx
	call S9xGetByte
	addl $6, CYCLES
	GetCarry
	rclb %al
	SetZNC
	popl %edx
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Rol16 K
	pushl %edx
	call S9xGetWord
	addl $6, CYCLES
	GetCarry
	rclw %ax
	setc _Carry
	movb %ah, _Negative
	testw %ax, %ax
	setnz _Zero
	popl %edx
#if 0
	call S9xSetWord
#else
	pushl %eax
	pushl %edx
	incl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	call S9xSetByte
#endif
	jmp MainAsmLoop
.endm

.macro Ror8 K
	pushl %edx
	call S9xGetByte
	addl $6, CYCLES
	GetCarry
	rcrb %al
	SetZNC
	popl %edx
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Ror16 K
	pushl %edx
	call S9xGetWord
	addl $6, CYCLES
	GetCarry
	rcrw %ax
	setc _Carry
	movb %ah, _Negative
	testw %ax, %ax
	setnz _Zero
	popl %edx
#if 0
	call S9xSetWord
#else
	pushl %eax
	pushl %edx
	incl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	call S9xSetByte
#endif
	jmp MainAsmLoop
.endm

.macro Bit8 K
	call S9xGetByte
	movb %al, _Negative
	testb $0x40, %al
	setnz _Overflow
	andb AL, %al
	movb %al, _Zero
	jmp MainAsmLoop
.endm

.macro Bit16 K
	call S9xGetWord
	movb %ah, _Negative
	testb $0x40, %ah
	setnz _Overflow
	andw AA, %ax
	setnz _Zero
	jmp MainAsmLoop
.endm

.macro Dec8 K
	pushl %edx
	addl $6, CYCLES
	call S9xGetByte
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	decb %al
	SetZN
	popl %edx
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Dec16 K
	pushl %edx
	addl $6, CYCLES
	call S9xGetWord
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	decw %ax
	Set16ZN
	popl %edx
#if 0
	call S9xSetWord
#else
	pushl %eax
	pushl %edx
	incl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	call S9xSetByte
#endif
	jmp MainAsmLoop
.endm

.macro Inc8 K
	pushl %edx
	addl $6, CYCLES
	call S9xGetByte
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	incb %al
	SetZN
	popl %edx
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Inc16 K
	pushl %edx
	addl $6, CYCLES
	call S9xGetWord
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif
	incw %ax
	Set16ZN
	popl %edx
#if 0
	call S9xSetWord
#else
	pushl %eax
	pushl %edx
	incl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	call S9xSetByte
#endif
	jmp MainAsmLoop
.endm

.macro Sta8 K
	movb AL, %al
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Sta16 K
	movw AA, %ax
	call S9xSetWord
	jmp MainAsmLoop
.endm

.macro Stx8 K
	movb XL, %al
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Stx16 K
	movw XX, %ax
	call S9xSetWord
	jmp MainAsmLoop
.endm

.macro Sty8 K
	movb YL, %al
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Sty16 K
	movw YY, %ax
	call S9xSetWord
	jmp MainAsmLoop
.endm

.macro Stz8 K
	xorb %al, %al
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Stz16 K
	xorl %eax, %eax
	call S9xSetWord
	jmp MainAsmLoop
.endm

.macro Trb8 K
	pushl %edx
	addl $6, CYCLES
	call S9xGetByte
	movb AL, %dl
	testb %al, %dl
	setnz _Zero
	xorb $0xff, %dl
	andb %dl, %al
	popl %edx
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Trb16 K
	pushl %edx
	addl $6, CYCLES
	call S9xGetWord
	movw AA, %dx
	testw %ax, %dx
	setnz _Zero
	xorl $0xffffffff, %edx
	andl %edx, %eax
	popl %edx
#if 0
	call S9xSetWord
#else
	pushl %eax
	pushl %edx
	incl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	call S9xSetByte
#endif
	jmp MainAsmLoop
.endm

.macro Tsb8 K
	pushl %edx
	addl $6, CYCLES
	call S9xGetByte
	movb AL, %dl
	testb %al, %dl
	setnz _Zero
	orb %dl, %al
	popl %edx
	call S9xSetByte
	jmp MainAsmLoop
.endm

.macro Tsb16 K
	pushl %edx
	addl $6, CYCLES
	call S9xGetWord
	movw AA, %dx
	testw %ax, %dx
	setnz _Zero
	orw %dx, %ax
	popl %edx
#if 0
	call S9xSetWord
#else
	pushl %eax
	pushl %edx
	incl %edx
	movb %ah, %al
	call S9xSetByte
	popl %edx
	popl %eax
	call S9xSetByte
#endif
	jmp MainAsmLoop
.endm

.macro CheckForIrq K
	testb $0xff, IRQActive
	jz .CheckForIrqS9xExit\K
	testb $IRQ, FLAGS
	jnz .CheckForIrqS9xExit\K
	testb $0xff, DisableIRQ
	jnz .CheckForIrqS9xExit\K
	call S9xOpcode_IRQ
.CheckForIrqS9xExit\K:
.endm

.macro S9xFixCycles K
	testw $Emulation, FLAGS16
	jz .S9xFixCyclesNoEmulation\K
	movl $S9xOpcodesE1, CPUOpcodes
	jmp .S9xFixCyclesS9xExit\K

.S9xFixCyclesNoEmulation\K:	
	testb $MemoryFlag, FLAGS
	jz .S9xFixCyclesNoMemory\K
	testb $IndexFlag, FLAGS
	jz .S9xFixCyclesNoIndex\K
	movl $S9xOpcodesM1X1, CPUOpcodes
	jmp .S9xFixCyclesS9xExit\K
.S9xFixCyclesNoIndex\K:
	movl $S9xOpcodesM1X0, CPUOpcodes
	jmp .S9xFixCyclesS9xExit\K

.S9xFixCyclesNoMemory\K:
	testb $IndexFlag, FLAGS
	jz .S9xFixCyclesNoIndex2\K
	movl $S9xOpcodesM0X1, CPUOpcodes
	jmp .S9xFixCyclesS9xExit\K
.S9xFixCyclesNoIndex2\K:
	movl $S9xOpcodesM0X0, CPUOpcodes
.S9xFixCyclesS9xExit\K:
.endm

.macro S9xUnpackStatus K
	movb FLAGS, %al
	movb %al, _Negative
	testb $Zero, %al
	setz _Zero
	testb $Carry, FLAGS
	setnz _Carry
	testb $Overflow, FLAGS
	setnz _Overflow
.endm

.macro S9xPackStatus K
//	movb _Carry, %al
	andb $~(Zero | Negative | Carry | Overflow), FLAGS
//	orb %al, FLAGS
	movb _Negative, %al
	andb $0x80, %al
	orb _Carry, %al
	orb %al, FLAGS
	movb _Overflow, %al
	salb $6, %al
	orb %al, FLAGS
	testb $0xff, _Zero
	setz %al
	salb %al
	orb %al, FLAGS
.endm

.macro CPUShutdown K
#ifdef CPU_SHUTDOWN
	testb $0xff, Shutdown
	jz .NoCPUShutdown\K
	cmpl PC, WaitAddress
	jne .NoCPUShutdown\K
	cmpl $0, WaitCounter
	jnz .CPUShutdownNotYet\K
	movl Flags, %eax
	andl $NMI_FLAG | IRQ_PENDING_FLAG, %eax
	jnz .CPUShutdownNotYet\K
	
	movl $0, WaitAddress
	movl NextEvent, CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer
	testb $0xff, APUExecuting
	jz .NoCPUShutdown\K

	pushl %edx
	movb $0, CPUExecuting
.CPUShutdownAPUExecLoop\K:
#ifdef DEBUGGER
	testb $2,APUFlags
	je .CPUShutdownNoAPUTrace\K
	STORE_REGISTERS
	ccall S9xTraceAPU
	LOAD_REGISTERS
.CPUShutdownNoAPUTrace\K:
#endif
	xorl %eax,%eax
#ifdef SPC700_C
	movl APUPC, %edx
	SAVE_CYCLES
	movb (%edx),%al
#else
	movb (APUPC), %al
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
	jl .CPUShutdownAPUExecLoop\K
	
	popl %edx
	movb $1, CPUExecuting
	jmp .NoCPUShutdown\K

.CPUShutdownNotYet\K:
	cmpl $2, WaitCounter
	jb .CPUShutdownDecCounter\K
	movl $1, WaitCounter
	jp .NoCPUShutdown\K
.CPUShutdownDecCounter\K:
	decl WaitCounter
.NoCPUShutdown\K:	
#endif
.endm

