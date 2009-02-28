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

.globl MainAsmLoop

.text
	.align 4
.globl S9xMainLoop
S9xMainLoop:
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	LOAD_REGISTERS
.L9:
	cmpb $0,APUExecuting
	je .L12
.apuloop:
	cmpl CYCLES, APUCycles
	jg .L12
#ifdef DEBUGGER
	testb $2,APUFlags
	je .L14
	STORE_REGISTERS
	ccall S9xTraceAPU
	LOAD_REGISTERS
.L14:
#endif
	xorl %eax,%eax
#ifdef SPC700_C
	movl APUPC, %edx
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
	jmp .apuloop
.L12:
	cmpl $0, Flags
	je .L15
	movl Flags, %eax
	testb %al, %al
	jge .NO_NMI
	decl NMICycleCount
	jnz .NO_NMI
	andb $~NMI_FLAG, %al
	movl %eax, Flags
	cmpb $0, WaitingForInterrupt
	je .L17
	movb $0, WaitingForInterrupt
	incl PC
.L17:
	call S9xOpcode_NMI
.NO_NMI:
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
	testl $IRQ_PENDING_FLAG, Flags
	jz .NO_PENDING_IRQ
	cmpl $0, IRQCycleCount
	jne .DEC_IRQ_CYCLE_COUNT
	testb $0xff, WaitingForInterrupt
	jz .NOT_WAITING
	movb $0, WaitingForInterrupt
	incl PC
.NOT_WAITING:
	testb $0xff, IRQActive
	jz .CLEAR_PENDING_IRQ_FLAG
	testb $0xff, DisableIRQ
	jnz .CLEAR_PENDING_IRQ_FLAG
	testb $IRQ, FLAGS
	jnz .NO_PENDING_IRQ
	call S9xOpcode_IRQ
	jmp .NO_PENDING_IRQ

.CLEAR_PENDING_IRQ_FLAG:
	andl $~IRQ_PENDING_FLAG, Flags
	jmp .NO_PENDING_IRQ

.DEC_IRQ_CYCLE_COUNT:
	decl IRQCycleCount
        jnz .DEC_IRQ_CYCLE_COUNT_DONE
	testb $IRQ, FLAGS
        jz .DEC_IRQ_CYCLE_COUNT_DONE
        movb $1, IRQCycleCount
.DEC_IRQ_CYCLE_COUNT_DONE:

.NO_PENDING_IRQ:
#ifdef DEBUGGER
	movl Flags, %eax
	testb $DEBUG_MODE_FLAG,%al
	jnz .L31
#else
	movl Flags, %eax
#endif
	testb $SCAN_KEYS_FLAG, %al
	jnz .L31
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
	xorl %eax,%eax

#ifdef CPU_SHUTDOWN
	movl PC, PCAtOpcodeStart
#endif	
	movb (PC), %al
	addl MemSpeed, CYCLES
	movl CPUOpcodes, %ecx
	incl PC
	jmp *(%ecx,%eax,4)
MainAsmLoop:
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer
	cmpb $0, SA1Executing
	je .nosa1
	STORE_REGISTERS
	call S9xSA1MainLoop
	LOAD_REGISTERS
.nosa1:
	cmpl NextEvent, CYCLES
	jl .L9
	STORE_REGISTERS
	call S9xDoHBlankProcessing
	LOAD_REGISTERS
	jmp .L9
.L31:
	S9xPackStatus S9xMainLoop
	STORE_REGISTERS
	subl PCBase, PC
	movw %di, PCR
#ifdef SPC700_C
	movl APUPC, %edx
	movl APURAM, %eax
	subl %eax, %edx
	movw %dx, APUPCR
#else
	subl APURAM, APUPC
	movw %bp, APUPCR
#endif
	APUS9xPackStatus S9xMainLoop
	movl Flags, %eax
	testb $SCAN_KEYS_FLAG, %al
	jz .NoScanKeys
	andb $~SCAN_KEYS_FLAG, %al
	movl %eax, Flags
#ifdef DEBUGGER
	testl $FRAME_ADVANCE_FLAG, %eax
	jnz .NoScanKeys
#endif
	ccall S9xSyncSpeed
.NoScanKeys:

#ifdef DETECT_NASTY_FX_INTERLEAVE
	movb BRKTriggered, %al
	andb SuperFXEnabled, %al
	jz .NoSuperFXBrkTest
	andb TriedInterleavedMode2, %al
	jnz .NoSuperFXBrkTest
	movb $1, TriedInterleavedMode2
	movb $0, BRKTriggered
	call S9xDeinterleaveMode2
.NoSuperFXBrkTest:
#endif
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp

	ret

.globl S9xDoHBlankProcessing

S9xDoHBlankProcessing:
	pushl %edi
	pushl %esi
	pushl %ebx
#ifdef CPU_SHUTDOWN
	incl WaitCounter
#endif
	movb WhichEvent,%bl
	cmpb $1,%bl
	je .hblank_end
	jg .L196
	testb %bl,%bl
	jz .hblank_start
	jmp .reschedule
.L196:
	cmpb $3,%bl
	jle .htimer_trigger
	jmp .reschedule
.hblank_start:
	movb HDMA,%dl
	testb %dl,%dl
	je .reschedule
	movl V_Counter, %eax
	cmpw ScreenHeight, %ax
	ja .reschedule 
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	ccall S9xDoHDMA
	movb %al,HDMA
	addl $4,%esp
	jmp .reschedule
.hblank_end:
	ccall S9xSuperFXExec
	testb $0xff, SoundSync
	jz .nosoundsync
	ccall S9xGenerateSound
.nosoundsync:
	movl H_Max,%eax
	subl %eax, Cycles
	imull $10000, %eax, %ecx
	subl %ecx, NextAPUTimerPos
	cmpb $0, APUExecuting
	je .apunotrunning
	subl %eax, APUCycles
//	addl %eax, smpcyc
	jmp .apucycleskip
.apunotrunning:
	movl $0, APUCycles
.apucycleskip:	
	movl V_Counter,%ecx
	incl %ecx
	incl Scanline
	movl $-1,NextEvent
	movl %ecx,V_Counter
	testb $0xff, PAL
	jz .ntsc_tv
	cmpl $312,%ecx
	jb .L161
	jmp .endofframe
.ntsc_tv:
	cmpl $262,%ecx
	jb .L161
.endofframe:
	xorl %edx, %edx
	movl Flags,%ecx
	movb %dl,NMIActive
	orl $16,%ecx
	movl %edx,V_Counter
	movb %dl,HVBeamCounterLatched
	movl %ecx,Flags
	movb %dl,RangeTimeOver

	movl FillRAM, %ecx
	xorb $0x80,0x213F(%ecx)

	ccall S9xStartHDMA
.L161:
	movb VTimerEnabled,%al
	testb %al,%al
	je .L162
	movb HTimerEnabled,%dl
	testb %dl,%dl
	jne .L162
	xorl %eax,%eax
	movw IRQVBeamPos,%ax
	movl V_Counter,%ecx
	cmpl %eax,%ecx
	jne .L162
	
	pushl $2
	call S9xSetIRQ
	addl $4, %esp

.L162:
	xorl %eax,%eax
	movl V_Counter,%edx
	movw ScreenHeight,%ax
	incl %eax
	cmpl %eax,%edx
	jne .L165
	ccall S9xEndScreenRefresh
	movb Brightness,%al
	xorl %ecx,%ecx
	movb %al,MaxBrightness
	movl FillRAM,%eax
	movb %cl,HDMA
	movb 0x2100(%eax),%cl
	movb 0x4200(%eax),%dl
	shrb $7,%cl
	movb %cl,ForcedBlanking
	movb $0x80, 0x4210(%eax)

        testb $0xff, ForcedBlanking
	jnz .noOAMreset
	movw SavedOAMAddr,%ax
	movw %ax,OAMAddr
	movb %ch,FirstSprite
	testb $0xff, OAMPriorityRotation
	jz .noOAMreset
	shrw %ax
	andb $0x7F,%al
        testb $0x01,OAMFlip
        jne .yesOAMReset
        cmpb %al,FirstSprite
        je .noOAMreset
.yesOAMReset:
	movb %al,FirstSprite
        movb $0xff,OBJChanged
.noOAMreset:
	movb %ch,OAMFlip

	testb %dl,%dl
	jge .L165
	orl $NMI_FLAG, Flags
	movb $1, NMIActive
	movl NMITriggerPoint, %eax
	movl %eax, NMICycleCount
.L165:
	xorl %eax, %eax
	movw ScreenHeight,%ax
	addl $3,%eax
	cmpl V_Counter,%eax
	jne .NoJoypadUpdate
	ccall S9xDoAutoJoypad
.NoJoypadUpdate:
	movl V_Counter,%eax
	cmpl $1,%eax
	jne .L177
	movl FillRAM,%eax
	andl $~NMI_FLAG,Flags
	movb $0,0x4210(%eax)
	ccall S9xStartScreenRefresh

.L177:
	movl V_Counter,%edx
	testl %edx,%edx
	je .L178
	xorl %eax,%eax
	movw ScreenHeight,%ax
	incl %eax
	cmpl %eax,%edx
	jae .L178
	movb V_Counter,%al
	decb %al
	andl $255,%eax
	pushl %eax
	ccall RenderLine
	addl $4,%esp
.L178:
#if 0
	movl APUTimerErrorCounter,%eax
	incl %eax
	movl %eax,APUTimerErrorCounter
	andl $31,%eax
	jz .reschedule
#endif

#if 0
	movb APUTimerEnabled + 2,%cl
	testb %cl,%cl
	je .L179
	movw APUTimer + 4,%ax
	addl $4,%eax
	movw %ax,APUTimer + 4
	cmpw %ax,APUTimerTarget + 4
	ja .L179
.L182:
	movl APURAM,%edx
	movb 255(%edx),%al
	incb %al
	andb $15,%al
	movb %al,255(%edx)
	movw APUTimerTarget + 4,%dx
	movw APUTimer + 4,%ax
	movl APUWaitCounter,%ecx
	subl %edx,%eax
	incl %ecx
	movb $1,APUExecuting
	movw %ax,APUTimer + 4
	movl %ecx,APUWaitCounter
	cmpw %dx,%ax
	jae .L182
.L179:
	testb $1,V_Counter
	je .reschedule
	movb APUTimerEnabled,%al
	testb %al,%al
	je .L185
	incw APUTimer
	movw APUTimerTarget,%ax
	cmpw %ax,APUTimer
	jb .L185
	movl APURAM,%edx
	movb 253(%edx),%al
	incb %al
	andb $15,%al
	movb %al,253(%edx)
	movl APUWaitCounter,%edx
	incl %edx
	movw $0,APUTimer
	movb $1,APUExecuting
	movl %edx,APUWaitCounter
.L185:
	movb APUTimerEnabled + 1,%al
	testb %al,%al
	je .reschedule
	incw APUTimer + 2
	movw APUTimerTarget + 2,%ax
	cmpw %ax,APUTimer + 2
	jb .reschedule
	movl APURAM,%edx
	movb 254(%edx),%al
	incb %al
	andb $15,%al
	movb %al,254(%edx)
	movl APUWaitCounter,%edx
	incl %edx
	movw $0,APUTimer + 2
	movb $1,APUExecuting
	movl %edx,APUWaitCounter
#endif
	jmp .reschedule
.htimer_trigger:
	cmpb $0, HTimerEnabled
	je .reschedule
	cmpb $0, VTimerEnabled
	je .L191
	xorl %eax,%eax
	movw IRQVBeamPos,%ax
	movl V_Counter,%edx
	cmpl %eax,%edx
	jne .reschedule
.L191:
	// CHANGED: 20/11/00
	pushl $1
	call S9xSetIRQ
	addl $4, %esp

.reschedule:
	cmpb $0, WhichEvent
	je .next_is_hblank_end
	cmpb $3, WhichEvent
	jne .next_is_hblank_start
.next_is_hblank_end:
	movb $1,%bl
	movl H_Max,%edx
	jmp .skip
.next_is_hblank_start:
	xorl %ebx,%ebx
	movl HBlankStart,%edx
.skip:
	cmpb $0, HTimerEnabled
	je .not_enabled
	movswl HTimerPosition,%esi
	cmpl %edx,%esi
	jge .not_enabled
	movl NextEvent,%eax
	cmpl %eax,%esi
	jle .not_enabled
	cmpb $0, VTimerEnabled
	je .enabled
	xorl %eax,%eax
	movw IRQVBeamPos,%ax
	movl V_Counter,%ecx
	cmpl %eax,%ecx
	jne .not_enabled
.enabled:
	movb $2,%bl
	cmpl %esi, HBlankStart
	jg .before
	movb $3,%bl
.before:
	movl %esi,%edx
.not_enabled:
	movb %bl,WhichEvent
	popl %ebx
	popl %esi
	movl %edx,NextEvent
	popl %edi
	ret

.text
	.align 4
.globl S9xSetIRQ
S9xSetIRQ:
	movl 4(%esp), %eax
	orb %al,IRQActive
	// CHANGED: 20/11/00
	movl $3, IRQCycleCount
	orl $IRQ_PENDING_FLAG, Flags
	cmpb $0, WaitingForInterrupt
	je .NoIncPC
	movb $0, WaitingForInterrupt
	// IRQ must trigger immediately after a WAI instruction - 
	// Final Fantasy Mystic Quest requires this.
	movl $0, IRQCycleCount
	incl PCS
.NoIncPC:
	ret

.globl S9xClearIRQ
S9xClearIRQ:
	movl 4(%esp), %eax
	xorl $~0, %eax
	andb IRQActive, %al
	movb %al, IRQActive
	jnz .irqsstillpending
	andl $~IRQ_PENDING_FLAG, Flags	
.irqsstillpending:
	ret
