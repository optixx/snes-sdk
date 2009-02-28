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
#include "asmops.h"
#include "spcops.h"

#ifdef SA1TRACE
#undef OOBTRACE
#endif
	
.globl MainAsmLoop

.text
	.align 4
.globl _S9xMainLoop
_S9xMainLoop:
	mov.l r14,@-r15
	sts.l pr,@-r15
	mov.l r13,@-r15
	mov.l r12,@-r15
	mov.l r11,@-r15
	mov.l r10,@-r15
	mov.l r9,@-r15
	mov.l r8,@-r15
	SETUP_PREDEF
	LOAD_REGISTERS MainLoop
	bra .L9
	nop
	
MainAsmLoop:
#if 0
	mova .stuff,r0
	extu.w A,r1
	cmp/eq A,r1
	bt 5f

	PUSH_REGISTERS
	mov A,r7
	mov.l @r0,r5
	mov.l @(4,r0),r6
	mov r0,r4
	mov.l @(8,r0),r0
	jsr @r0
	add #12,r4
	POP_REGISTERS
	mova .stuff,r0
	bra 5f
	extu.w A,A

	.align 2
.stuff:	
	.long 0,0
	.long _reportf
	.ascii "FAA!  lastPC=%x op=%x A=%x\n"
	.byte 0
	.align 2		
5:	
	mov.l PCBase,r2
	mov PC,r4
	sub r2,r4
	mov.l ICPU,r1
	mov.l ShiftedPB,r2
	add r2,r4
	mov.l r4,@r0
	mov.b @PC,r1
	mov.l r1,@(4,r0)
#endif
#if 0
#if 0
	mov.l Settings,r1
	mov.l CyclesPercentage,r0
	shlr8 r0
	cmp/eq #4,r0
	bf 5f
#endif
	PUSH_REGISTERS
	mov.l PCBase,r0
	mov PC,r1
	sub r0,r1
	mov.l ICPU,r0
	mov.l @(16,r0),r0
	add r0,r1
#if 0
	mov.l .lowpc,r0
	cmp/ge r0,r1
	bf 4f
	mov.l .hipc,r0
	cmp/ge r0,r1
	bt 4f
#endif
	mov r1,r5
	mova .foorpt,r0
	mov.l @r0+,r1
	jsr @r1
	mov.l r0,r4
4:	
	POP_REGISTERS
5:	
#endif
#if 0
	mov.l PCBase,r0
	mov PC,r1
	sub r0,r1
	mov.l ICPU,r0
	mov.l @(16,r0),r0
	add r0,r1
	mov.l .trapaddr,r0
	cmp/eq r0,r1
	bf .notrap
#if 1
	STORE_REGISTERS trap
	mov.l .bartrp,r0
	jsr @r0
	nop
	LOAD_REGISTERS trap
#else
	mov r1,r5
	mova .footrp,r0
	mov.l @r0+,r1
	jsr @r1
	mov.l r0,r4
#endif
.notrap:	
#endif	
	mov.l SA1,r1
	mov.b SA1Executing,r0
	tst r0,r0
	bt .nosa1
	STORE_REGISTERS	MAL1
	mov.l .S9xSA1MainLoop,r0
	jsr @r0
	nop
	LOAD_REGISTERS MAL1
.nosa1:
	mov.l NextEvent,r0
	cmp/ge r0,CYCLES
	bf .L9
	STORE_REGISTERS MAL2
	bsr S9xDoHBlankProcessing
	nop
	LOAD_REGISTERS MAL2

.L9:
#ifndef ARMAPU
	mov.l IAPU,r1
	mov.b APUExecuting,r0
	tst r0,r0
	bt .L12
.apuloop:
	mov.l APU,r1
	mov.l APUCycles,r0
	cmp/gt CYCLES,r0
	bt .L12
#ifdef DEBUGGER
	testb $2,APUFlags
	je .L14
	STORE_REGISTERS
	ccall S9xTraceAPU
	LOAD_REGISTERS
.L14:
#endif
#ifdef SPC700_C
	mov.l IAPU,r1
	SAVE_CYCLES
	mov.l APUPC,r0

#if 0
	PUSH_REGISTERS
	mov.l r0,@-r15
	mov.l APURAM,r1
	sub r1,r0
	mov r0,r5
	mova .foorpt2,r0
	mov.l @r0+,r1
	jsr @r1
	mov.l r0,r4
	mov.l @r15+,r0
	POP_REGISTERS
#endif	
	
	PUSH_REGISTERS
	mov.b @r0,r0
#else
	mov.b @APUPC,r0
#endif
	mov.l .S9xAPUCycles,r1
	extu.b r0,r0
	mov.l .S9xApuOpcodes,r2
	shll2 r0
	mov.l @(r0,r1),r4
	mov.l @(r0,r2),r0
	mov.l APU,r1
	mov.l APUCycles,r2
	add r4,r2
	jsr @r0
	mov.l r2,APUCycles
#ifdef SPC700_C
#if 0
	mov.l .myaputrace,r0
	mov.l PCBase,r1
	mov PC,r4
	jsr @r0
	sub r1,r4
#endif
	POP_REGISTERS
	bra .apuloop
	LOAD_CYCLES
#else
	bra .apuloop
	nop
#endif

	.align 2
#if 0
.myaputrace:
	.long _myaputrace
	
.foorpt2:	
	.long _reportf
	.ascii "A[%x]\n"
	.byte 0
	.align 1
#endif
#endif	/* ARMAPU */

.L12:
	mov.l Flags,r0
	tst r0,r0
	bt .L15
	tst #128,r0
	bt .NO_NMI
	mov.l NMICycleCount,r1
	dt r1
	bf/s .NO_NMI
	mov.l r1,NMICycleCount
	add #-NMI_FLAG,r0
	mov.l r0,Flags
	mov.b WaitingForInterrupt,r0
	tst r0,r0
	bt .L17
	mov #0,r0
	mov.b r0,WaitingForInterrupt
	add #1,PC
.L17:
	mov.l S9xOpcode_NMI,r0
	jsr @r0
	nop
	mov.l Flags,r0
.NO_NMI:
!	tst #DELAYED_NMI_FLAG2,r0
!	bt .NO_DELAYED_NMI
!	and #~DELAYED_NMI_FLAG2,r0
!	or #NMI_FLAG, r0
!	mov.l r0, Flags
!.NO_DELAYED_NMI:
!	tst #DELAYED_NMI_FLAG, r0
!	bt .NO_DELAYED_NMI2
!	and #~DELAYED_NMI_FLAG, r0
!	or #DELAYED_NMI_FLAG2, r0
!	mov.l r0, Flags
!.NO_DELAYED_NMI2:	
#ifdef DEBUGGER
	testb $BREAK_FLAG, Flags
	jz .NO_BREAK_POINTS
	pushl %esi
	pushl %ebx
	movl $S9xBreakpoint, %esi
	movb PB, %bl
	xorl %edx, %edx
	movl PC, %ecx
	subl PCBase, %ecx

.BREAK_CHECK_LOOP:
	movzwl %dx, %eax
	sall $2, %eax
	cmpb $0, S9xBreakpoint(%eax)
	je .BREAK_MATCH_FAILED
	cmpb %bl, 1(%esi, %eax)
	jne .BREAK_MATCH_FAILED
	movzwl 2(%esi, %eax), %eax
	cmpl %ecx, %eax
	jne .BREAK_MATCH_FAILED
	orb $DEBUG_MODE_FLAG, Flags
.BREAK_MATCH_FAILED:
	incw %dx
	cmpw $6, %dx
	jne .BREAK_CHECK_LOOP
	popl %ebx
	popl %esi
.NO_BREAK_POINTS:
#endif

	mov.w .pIRQ_PENDING_FLAG,r1
	tst r1, r0
	bt .NO_PENDING_IRQ
	mov.l IRQCycleCount,r0
	tst r0,r0
	bf/s .DEC_IRQ_CYCLE_COUNT
	add #-1,r0
	mov.b WaitingForInterrupt,r0
	tst r0,r0
	bt .NOT_WAITING
	mov #0,r0
	mov.b r0,WaitingForInterrupt
	add #1,PC
.NOT_WAITING:	
	mov.b IRQActive,r0
	tst r0,r0
	bt .CLEAR_PENDING_IRQ_FLAG
	/* XXX:	test for Settings.DisableIRQ */
	mov #IRQ,r0
	tst r0,FLAGS
	bf .NO_PENDING_IRQ
	mov.l S9xOpcode_IRQ,r3
	jsr @r3
	nop
	bra .NO_PENDING_IRQ
	nop
.CLEAR_PENDING_IRQ_FLAG:
	mov.l Flags,r0
	mov.w .nIRQ_PENDING_FLAG,r1
	and r1,r0
	bra .NO_PENDING_IRQ
	mov.l r0,Flags

.pIRQ_PENDING_FLAG:
	.word IRQ_PENDING_FLAG
.nIRQ_PENDING_FLAG:
	.word ~IRQ_PENDING_FLAG


.DEC_IRQ_CYCLE_COUNT:
	mov.l r0,IRQCycleCount

.NO_PENDING_IRQ:
#ifdef DEBUGGER
	movl Flags, %eax
	testb $DEBUG_MODE_FLAG,%al
	jnz .L31
#else
	mov.l Flags,r0
#endif
	tst #SCAN_KEYS_FLAG, r0
	bf .L31
.L28:
#ifdef DEBUGGER
	testb $TRACE_FLAG, %al
	jz .NO_TRACE
	STORE_REGISTERS
	ccall S9xTrace
	LOAD_REGISTERS
.NO_TRACE:
	movl Flags, %eax
	testb $SINGLE_STEP_FLAG, %al
	jz .L15
	andb $~SINGLE_STEP_FLAG, %al
	orb $DEBUG_MODE_FLAG, %al
	movl %eax, Flags
#endif
.L15:
#ifdef OOBTRACE
	PUSH_REGISTERS
	S9xPackStatus OOBTrace
	mov.l PCBase,r0
	mov PC,r4
	sub r0,r4
	mov A,r0
	mov.l ICPU,r1
        mov.w r0,AA
	mov.l ShiftedPB,r0
	add r0,r4
	mov.l .oobtrace,r0
	jsr @r0
	mov r11,r5
	POP_REGISTERS
#endif	
	mov.l ICPU,r1
#ifdef VAR_CYCLES
	mov.l MemSpeed, r2
	mov.b @PC,r0
	add r2,CYCLES
	extu.b r0,r0
#else
	mov.b @PC,r0
	mov.l CPUSpeed,r4
	extu.b r0,r0
	mov.b @(r0,r4),r2
	add r2,CYCLES
#endif
	mov.l CPUOpcodes,r2
	shll2 r0
	mov.l @(r0,r2),r0
#ifdef CPU_SHUTDOWN
	mov.l PC, PCAtOpcodeStart
#endif	
	jmp @r0
	add #1,PC

	.align 2
.S9xSA1MainLoop:
	.long _S9xSA1MainLoop

#ifdef OOBTRACE
.oobtrace:	
	.long __Z8oobtraceiP10SRegistersi
#endif
	
#if 0
.trapaddr:
	.long 0x0858502
.lowpc:
	.long 0x08583a0
.hipc:
	.long 0x0858402
.foorpt:
	.long _reportf
	.ascii "*%x\n"
	.byte 0
	.align 2
.footrp:	
	.long _reportf
	.ascii "WATCH AT %x\n"
	.byte 0
	.align 1
#if 1
	.align 2
.bartrp:
	.long _gurktrap
#endif
#endif
		
.L31:
	S9xPackStatus S9xMainLoop
	STORE_REGISTERS
	mov.l PCBase,r0
	neg r0,r0
	add PC,r0
	mov.w r0,PCR
#ifndef ARMAPU
#ifdef SPC700_C
	mov.l IAPU,r1
	mov.l APUPC, r0
	mov.l APURAM, r1
	sub r1,r0
	mov.l APURegisters,r1
	mov.w r0,APUPCR
#else
	mov.l APURAM,r1
	sub r1,APUPC
	mov.w APUPC, APUPCR
#endif
	APUS9xPackStatus S9xMainLoop
#endif	/* ARMAPU */
	mov.l Flags, r0
	tst #SCAN_KEYS_FLAG,r0
	bt .NoScanKeys
	add #-SCAN_KEYS_FLAG,r0
	mov.l r0, Flags
#ifdef DEBUGGER
	testl $FRAME_ADVANCE_FLAG, %eax
	jnz .NoScanKeys
#endif
	mov.l .S9xSyncSpeed,r3
	jsr @r3
	nop
.NoScanKeys:
#if 0
	mov.l Settings,r1
	mov.b BRKTriggered,r0
	mov r0,r2
	mov.b SuperFXEnabled,r0
	tst r2,r0
	bt .NoSuperFXBrkTest
	and r0,r2
	mov.b TriedInterleavedMode2,r0
	tst r2,r0
	bf .NoSuperFXBrkTest
	mov #1,r0
	mov.l .S9xDeinterleaveMode2,r2
	mov.b r0,TriedInterleavedMode2
	mov #0,r0
	jsr @r2
	mov.b r0,BRKTriggered
#endif
.NoSuperFXBrkTest:	
	mov.l @r15+,r8
	mov.l @r15+,r9
	mov.l @r15+,r10
	mov.l @r15+,r11
	mov.l @r15+,r12
	mov.l @r15+,r13
	lds.l @r15+,pr

	rts
	mov.l @r15+,r14

	.align 2

#ifndef ARMAPU
.S9xAPUCycles:
	.long _S9xAPUCycles
.S9xApuOpcodes:
	.long _S9xApuOpcodes
#endif	/* ARMAPU */

.S9xSyncSpeed:
	.long _S9xSyncSpeed

.globl _S9xDoHBlankProcessing
_S9xDoHBlankProcessing:
	mov.l r14,@-r15
	mov.l r13,@-r15
	sts.l pr,@-r15
	mov.l r12,@-r15
	mov.l r11,@-r15
	SETUP_PREDEF
	bsr S9xDoHBlankProcessing
	nop
	mov.l @r15+,r11
	mov.l @r15+,r12
	lds.l @r15+,pr
	mov.l @r15+,r13
	rts
	mov.l @r15+,r14

	MISC

	
S9xDoHBlankProcessing:
#if 0
	sts.l pr,@-r15

	mov.l .hbtrace,r0
	mov r11,r4
	jsr @r0
	mov r12,r5
	
	bsr 6f
	nop

	mov.l .hbtrace,r0
	mov r11,r4
	jsr @r0
	mov r12,r5
	
	lds.l @r15+,pr
	rts
	nop

	.align 2
.hbtrace: .long __Z8hb_traceP10SRegistersP9SCPUState
	
6:		
#endif
	sts.l pr,@-r15

#ifdef CPU_SHUTDOWN
	mov.l WaitCounter,r1
#endif
	mov.b WhichEvent,r0
#ifdef CPU_SHUTDOWN
	add #1,r1
#endif
	tst r0,r0
#ifdef CPU_SHUTDOWN
	mov.l r1,WaitCounter
#endif
	bt/s .hblank_start
	cmp/eq #1,r0
	bt/s .hblank_end
	mov #3,r1
	cmp/hi r1,r0
	bt .reschedule

.htimer_trigger:
	mov.l PPU,r1
	mov.b HTimerEnabled,r0
	tst r0,r0
	bt .reschedule
	mov.b VTimerEnabled,r0
	tst r0,r0
	bt .L191
	mov.w IRQVBeamPos,r0
	mov.l V_Counter,r3
	extu.w r0,r0
	cmp/eq r0,r3
	bf .reschedule
.L191:
	// CHANGED: 20/11/00
	bsr _S9xSetIRQ
	mov #1,r4

	bra .reschedule
	nop

.hblank_start:		
	mov.l IPPU,r1
	mov.b HDMA,r0
	mov r0,r4
	tst r0,r0
	bt .reschedule
	mov.l PPU,r1
	mov.w ScreenHeight,r0
	mov.l V_Counter,r1
	cmp/hi r0,r1
	bt .reschedule
	mov.l .S9xDoHDMA,r3
	jsr @r3
	extu.b r4,r4
	mov.l IPPU,r1
	mov.b r0,HDMA

.reschedule:
	mov.b WhichEvent,r0
	tst r0,r0
	bt/s .next_is_hblank_end
	mov.l Settings,r1
	cmp/eq #3,r0
	bf .next_is_hblank_start
.next_is_hblank_end:
	mov #1,r4
	bra .skip
	mov.l H_Max,r5
.next_is_hblank_start:
	mov #0,r4
	mov.l HBlankStart,r5
.skip:
	mov.l PPU,r1
	mov.b HTimerEnabled,r0
	tst r0,r0
	bt .not_enabled
	mov.w HTimerPosition,r0
	cmp/ge r5,r0
	bt .not_enabled
	mov.l NextEvent,r3
	mov r0,r6
	cmp/gt r3,r0
	bf .not_enabled
	mov.b VTimerEnabled,r0
	tst r0,r0
	bt .enabled
	mov.w IRQVBeamPos,r0
	mov.l V_Counter,r2
	extu.w r0,r0
	cmp/eq r0,r2
	bf .not_enabled
.enabled:
	mov.l Settings,r1
	mov #2,r4
	mov.l HBlankStart,r0
	cmp/gt r6,r0
	bt/s .before
	mov r6,r5
	mov #3,r4
.before:
.not_enabled:
	mov r4,r0
	mov.l r5,NextEvent
	lds.l @r15+,pr
	rts
	mov.b r0,WhichEvent

.hblank_end:
	mov.l .S9xSuperFXExec,r3
	jsr @r3
	nop
/*	testb $0xff, SoundSync
	jz .nosoundsync */
	mov.l .S9xGenerateSound,r3
	jsr @r3
	nop
.nosoundsync:
	mov.l Settings,r1
	mov.l Cycles,r0
	mov.l H_Max,r2
	sub r2,r0
	mov.l r0,Cycles
#ifndef ARMAPU
	mov.l IAPU,r1
	mov.b APUExecuting,r0
	tst r0,r0
	bt/s .apunotrunning
	mov.l APU,r1
	mov.l APUCycles,r0
	sub r2,r0
/*	bra .apucycleskip
	mov.l r0,APUCycles */

.apunotrunning:
	mov.l r0,APUCycles
.apucycleskip:	
#endif	/* ARMAPU */
	mov.l PPU,r1
	mov.l V_Counter,r0
	add #1,r0
	mov.l ICPU,r1
	mov.l Scanline,r2
	add #1,r2
	mov.l r2,Scanline
	mov #-1,r2
	mov.l r2,NextEvent
	mov.l PPU,r1
	mov.l r0,V_Counter
	mov.w .n262,r3
	cmp/hi r3,r0
	bf .L161
	mov.w .n312,r3
	cmp/hi r3,r0
	mov.l Settings,r1
	bt .ntsc_tv
	mov.b PAL,r0
	tst r0,r0
	bf .L161

.ntsc_tv:
	mov.l PPU,r1
	mov.w SavedOAMAddr,r0
	mov #0,r4
	mov.l Flags,r3
	mov.w r0,OAMAddr
	mov r4,r0
	mov.b r0,OAMFlip
	mov.b r0,NMIActive
	mov #16,r0
	or r0,r3
	mov.l r4,V_Counter
	mov r4,r0
	mov.l .S9xStartHDMA,r2
	mov.b r0,HVBeamCounterLatched
	jsr @r2
	mov.l r3,Flags
.L161:
	mov.l PPU,r1
	mov.b VTimerEnabled,r0
	tst r0,r0
	bt .L162
	mov.b HTimerEnabled,r0
	tst r0,r0
	bf .L162
	mov.w IRQVBeamPos,r0
	mov.l V_Counter,r2
	extu.w r0,r0
	cmp/eq r0,r2
	bf .L162
	mov.b IRQActive,r0
	or #2,r0
	mov.b r0,IRQActive
	mov.b WaitingForInterrupt,r0
	tst r0,r0
	bt .L163
	mov.l PCS,r0
	add #1,r0
	mov.l r0,PCS
	mov #0,r0
	mov.b r0,WaitingForInterrupt
.L163:
/*
	movb DisableIRQ,%al
	testb %al,%al
	jne .L162
*/
#if 0
	mov.b PP,r0
	tst #4,r0
	bf .L162
	mov.l S9xOpcode_IRQ,r3
	/* PUSH_REGISTERS */
	LOAD_REGISTERS IRQ
	jsr @r3
	nop
	STORE_REGISTERS IRQ
	/* POP_REGISTERS */
#endif
	bsr _S9xSetIRQ
	mov #2,r4
.L162:
	mov.l PPU,r1
	mov.w ScreenHeight,r0
	mov.l V_Counter,r2
	add #1,r0
	cmp/eq r0,r2
	bf .L165
	mov.l .S9xEndScreenRefresh,r0
	jsr @r0
	nop

	mov.l PPU,r1
	mov #0,r0
	mov.b r0,FirstSprite
	mov.b Brightness,r0
	mov.l IPPU,r1
	mov.b r0,MaxBrightness
	mov #0,r0
	mov.b r0,HDMA
	mov.l .FillRAM0,r0
	mov.w .nx2100,r2
	mov.l @r0,r0
	mov.b @(r0,r2),r3
	shll r2
	shll r3
	mov.l PPU,r1
	shlr8 r3
	mov #1,r0
	and r3,r0
	mov.b r0,ForcedBlanking
	mov.l .FillRAM0,r0
	mov.l @r0,r0
	mov.b @(r0,r2),r3
	add #0x10,r2
	cmp/pz r3
	mov #0x80,r3
	bt/s .L165
	mov.b r3,@(r0,r2)
	mov.l Flags,r0
	or #NMI_FLAG,r0
	mov.l r0,Flags
	mov #NMITriggerPoint_offs,r0
	mov.l @(r0,r12),r2
	mov #1,r0
	mov.l r2,NMICycleCount
	mov.b r0,NMIActive
.L165:
	mov.w ScreenHeight,r0
	add #3,r0
	mov.l V_Counter,r2
	cmp/eq r0,r2
	bf .NoJoypadUpdate
	mov.l .S9xDoAutoJoypad,r3
	jsr @r3
	nop
.NoJoypadUpdate:
	mov.l V_Counter,r0
	cmp/eq #1,r0
	bf .L177
	mov.l .FillRAM0,r3
	mov #0,r2
	mov.l @r3,r3
	mov.w .n4210,r1
	mov.l Flags,r0
	add r1,r3
	mov.b r2,@r3
	or #NMI_FLAG,r0
	mov.l .S9xStartScreenRefresh,r3
	add #-NMI_FLAG,r0
	jsr @r3
	mov.l r0,Flags
.L177:
	mov.l V_Counter,r3
	mov.l PPU,r1
	tst r3,r3
	bt .L178
	mov.w ScreenHeight,r0
	extu.w r0,r0
	add #1,r0
	cmp/ge r0,r3
	bt .L178
	mov.l V_Counter,r0
	mov.l .RenderLine,r3
	add #-1,r0
	jsr @r3
	extu.b r0,r4
.L178:
#ifndef ARMAPU
#if 0
	movl APUTimerErrorCounter,%eax
	incl %eax
	movl %eax,APUTimerErrorCounter
	andl $31,%eax
	jz .reschedule
#endif
	mov.l .APUTimer_ptr,r2
	mov.l IAPU,r1
	mov.b @(APUTimerEnabled - APUTimer + 2,r2),r0
	tst r0,r0
	bt .L179
	mov.w @(4,r2),r0
	add #4,r0
	mov.w r0,@(4,r2)
	extu.w r0,r3
	mov.w @(APUTimerTarget - APUTimer + 4,r2),r0
	extu.w r0,r0
	cmp/hs r0,r3
	bf .L179
.L182:
	mov.w .n255,r0
	mov.l APURAM,r3
	add r0,r3
	mov.b @r3,r0
	add #1,r0
	and #15,r0
	mov.b r0,@r3
	mov.w @(APUTimerTarget - APUTimer + 4,r2),r0
	extu.w r0,r3
	mov.w @(4,r2),r0
#ifdef SPC700_SHUTDOWN
	mov.l APUWaitCounter,r4
	sub r3,r0
	add #1,r4
	mov.w r0,@(4,r2)
	extu.w r0,r0
	mov.l r4,APUWaitCounter
	cmp/hs r3,r0
	mov #1,r0
	bt/s .L182
	mov.b r0,APUExecuting
#else
	sub r3,r0
	mov.w r0,@(4,r2)
	extu.w r0,r0
	cmp/hs r3,r0
	bt .L182
#endif
.L179:
	mov.l V_Counter,r0
	tst #1,r0
	bt 1f
	mov.b @(APUTimerEnabled - APUTimer,r2),r0
	tst r0,r0
	bt .L185
	mov.w @r2,r0
	add #1,r0
	mov.w r0,@r2
	extu.w r0,r3
	mov.w @(APUTimerTarget - APUTimer,r2),r0
	extu.w r0,r0
	cmp/hs r0,r3
	bf .L185
	mov.w .n253,r0
	mov.l APURAM,r3
	add r0,r3
	mov.b @r3,r0
	add #1,r0
	and #15,r0
	mov.b r0,@r3
#ifdef SPC700_SHUTDOWN
	mov.l APUWaitCounter,r3
	mov #0,r0
	add #1,r3
	mov.w r0,@r2
	add #1,r0
	mov.l r3,APUWaitCounter
	mov.b r0,APUExecuting
#else
	mov #0,r0
	mov.w r0,@r2
#endif
.L185:
	mov.b @(APUTimerEnabled - APUTimer + 1,r2),r0
	tst r0,r0
	bt 1f
	mov.w @(2, r2),r0
	add #1,r0
	mov.w r0,@(2, r2)
	extu.w r0,r3
	mov.w @(APUTimerTarget - APUTimer + 2,r2),r0
	extu.w r0,r0
	cmp/hs r0,r3
	bf 1f
	mov.w .n254,r0
	mov.l APURAM,r3
	add r0,r3
	mov.b @r3,r0
	add #1,r0
	and #15,r0
	mov.b r0,@r3
#ifdef SPC700_SHUTDOWN
	mov.l APUWaitCounter,r3
	mov #0,r0
	add #1,r3
	mov.w r0,@(2,r2)
	add #1,r0
	mov.l r3,APUWaitCounter
	bra .reschedule
	mov.b r0,APUExecuting
#else
	mov #0,r0
	bra .reschedule
	mov.w r0,@(2,r2)
#endif
#endif	/* ARMAPU */

1:	bra .reschedule
	nop

	.align 2

.S9xDoHDMA:
	.long _S9xDoHDMA
.S9xSuperFXExec:	
	.long _S9xSuperFXExec	
.S9xGenerateSound:
	.long _S9xGenerateSound
	
#ifndef ARMAPU
.APUTimer_ptr:
	.long _APU+APUTimer
#endif	/* ARMAPU */

.S9xStartHDMA:	
	.long _S9xStartHDMA
.S9xEndScreenRefresh:	
	.long _S9xEndScreenRefresh
.FillRAM0:	
	.long FillRAM
.S9xDoAutoJoypad:	
	.long _S9xDoAutoJoypad
.S9xStartScreenRefresh:
	.long _S9xStartScreenRefresh
.RenderLine:
	.long _RenderLine
.n4210:
	.word 0x4210
.nx2100:
	.word 0x2100
.n253:
	.word 253
.n254:
	.word 254
.n255:
	.word 255
.n262:
	.word 262
.n312:
	.word 312

.text
	.align 4
.globl _S9xSetIRQ
_S9xSetIRQ:
	mov.l r12,@-r15
	mov.l ._CPU,r12
	mov.b IRQActive,r0
	// CHANGED: 20/11/00
	mov #3,r1
	or r4,r0
	mov.l r1,IRQCycleCount
	mov.b r0,IRQActive
	mov.l Flags,r2
	mov.w .pIRQ_PENDING_FLAG2,r0
	or r0,r2
	mov.l r2,Flags
	mov.b WaitingForInterrupt,r0
	tst r0,r0
	bt .NoIncPC
	mov #0,r0
	mov.b r0,WaitingForInterrupt
	// IRQ must trigger immediately after a WAI instruction - 
	// Final Fantasy Mystic Quest requires this.
	mov.l PCS,r1
	mov.l r0,IRQCycleCount
	add #1,r1
	mov.l r1,PCS
.NoIncPC:
	rts
	mov.l @r15+,r12


.globl _S9xClearIRQ
_S9xClearIRQ:
	mov.l r12,@-r15
	mov.l ._CPU,r12
	not r4,r3
	mov.b IRQActive,r0
	and r3,r0
	mov.b r0,IRQActive
	tst r0,r0
	bf .irqsstillpending
	mov.w .nIRQ_PENDING_FLAG2,r0
	mov.l Flags,r2
	and r0,r2
	mov.l r2,Flags
.irqsstillpending:
	rts
	mov.l @r15+,r12

	.align 2
._CPU:
	.long _CPU
.pIRQ_PENDING_FLAG2:
	.word IRQ_PENDING_FLAG
.nIRQ_PENDING_FLAG2:
	.word ~IRQ_PENDING_FLAG

