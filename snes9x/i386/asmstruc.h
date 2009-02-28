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

#if defined(__DJGCC) || defined(MICROSOFT_C) || defined(_WINDOWS)
#define S9xTraceMessage _S9xTraceMessage
#define S9xGetPPU _S9xGetPPU
#define S9xSetPPU _S9xSetPPU
#define S9xSetDSP _S9xSetDSP
#define S9xGetDSP _S9xGetDSP
#define S9xGetCPU _S9xGetCPU
#define S9xSetCPU _S9xSetCPU
#define S9xGetC4 _S9xGetC4
#define S9xSetC4 _S9xSetC4
#define S9xSyncSpeed _S9xSyncSpeed
#define S9xTrace _S9xTrace
#define S9xDoHBlankProcessing _S9xDoHBlankProcessing
#define S9xTraceAPU _S9xTraceAPU
#define S9xGetWord _S9xGetWord
#define S9xGetByte _S9xGetByte
#define S9xSetWord _S9xSetWord
#define S9xSetByte _S9xSetByte
#define S9xSetPCBase _S9xSetPCBase
#define memset _memset
#define S9xDoHDMA _S9xDoHDMA
#define S9xStartHDMA _S9xStartHDMA
#define S9xEndScreenRefresh _S9xEndScreenRefresh
#define S9xStartScreenRefresh _S9xStartScreenRefresh
#define S9xSetAPUControl _S9xSetAPUControl
#define S9xSetAPUDSP _S9xSetAPUDSP
#define S9xGetAPUDSP _S9xGetAPUDSP
#define S9xTextMode _S9xTextMode
#define printf _printf
#define S9xSetAPUTimer _S9xSetAPUTimer
#define S9xAPUOPrint _S9xAPUOPrint
#define S9xSuperFXExec _S9xSuperFXExec
#define S9xGenerateSound _S9xGenerateSound
#define S9xDoAutoJoypad _S9xDoAutoJoypad
#define S9xMainLoop _S9xMainLoop
#define S9xDeinterleaveMode2 _S9xDeinterleaveMode2
#define RenderLine _RenderLine
#define S9xSetIRQ _S9xSetIRQ
#define S9xClearIRQ _S9xClearIRQ
#define SA1 _SA1
#define CPU _CPU
#define ICPU _ICPU
#define Settings _Settings
#define PPU _PPU
#define IPPU _IPPU
#define Registers _Registers
#define Memory _Memory
#define APU _APU
#define IAPU _IAPU
#define SoundData _SoundData
#define APURegisters _APURegisters
#define S9xBreakpoint _S9xBreakpoint
#define missing _missing
#define OpenBus _OpenBus
#define S9xApuOpcodes _S9xApuOpcodes
#define S9xAPUCycles _S9xAPUCycles
#define S9xMessage _S9xMessage
#define String _String
#define sprintf _sprintf
#define S9xSA1ExecuteDuringSleep _S9xSA1ExecuteDuringSleep
#define S9xGetSPC7110Byte _S9xGetSPC7110Byte
#define S9xGetSPC7110 _S9xGetSPC7110
#define GetOBC1 _GetOBC1
#define SetOBC1 _SetOBC1
#define S9xGetSetaDSP _S9xGetSetaDSP
#define S9xSetSetaDSP _S9xSetSetaDSP
#define S9xGetST018 _S9xGetST018
#define S9xSetST018 _S9xSetST018
#define S9xUpdateAPUTimer _S9xUpdateAPUTimer

#define A1 _A1
#define A2 _A2
#define A3 _A3
#define A4 _A4
#define Work8 _Work8
#define Work16 _Work16
#define Work32 _Work32
#define Int8 _Int8
#define Int16 _Int16
#define Int32 _Int32
#define S9xE1M1X1 _S9xE1M1X1
#define S9xE0M1X1 _S9xE0M1X1
#define S9xE0M1X0 _S9xE0M1X0
#define S9xE0M0X1 _S9xE0M0X1
#define S9xE0M0X0 _S9xE0M0X0
#define S9xOpcodesE1 _S9xOpcodesE1
#define S9xOpcodesM1X1 _S9xOpcodesM1X1
#define S9xOpcodesM1X0 _S9xOpcodesM1X0
#define S9xOpcodesM0X1 _S9xOpcodesM0X1
#define S9xOpcodesM0X0 _S9xOpcodesM0X0
#define APUROM _APUROM
#define W1 _W1
#define W2 _W2
#define W3 _W3
#define W4 _W4
#define ebx_save _ebx_save
#endif

#include "offsets.h"

#define Carry		1
#define Zero		2
#define IRQ		4
#define Decimal		8
#define IndexFlag	16
#define MemoryFlag	32
#define Overflow	64
#define Negative	128
#define Emulation	256

#define DEBUG_MODE_FLAG	    (1 << 0)
#define TRACE_FLAG	    (1 << 1)
#define SINGLE_STEP_FLAG    (1 << 2)
#define BREAK_FLAG	    (1 << 3)
#define SCAN_KEYS_FLAG	    (1 << 4)
#define SAVE_SNAPSHOT_FLAG  (1 << 5)
#define DELAYED_NMI_FLAG    (1 << 6)
#define NMI_FLAG	    (1 << 7)
#define PROCESS_SOUND_FLAG  (1 << 8)
#define FRAME_ADVANCE_FLAG  (1 << 9)
#define DELAYED_NMI_FLAG2   (1 << 10)
#define IRQ_PENDING_FLAG    (1 << 11)

#define MEMMAP_BLOCK_SIZE (0x1000)
#define MEMMAP_NUM_BLOCKS (0x1000000 / MEMMAP_BLOCK_SIZE)
#define MEMMAP_BLOCKS_PER_BANK (0x10000 / MEMMAP_BLOCK_SIZE)
#define MEMMAP_SHIFT 12
#define MEMMAP_MASK (MEMMAP_BLOCK_SIZE - 1)
#define ONE_CYCLE 6

#define CYCLES %esi
#define PC %edi
#ifdef SPC700_C
#define APUPC APUPCS
#else
#define APUPC %ebp
#endif

#define FLAGS %bl
#define FLAGS16 %bx

#define LOAD_CYCLES movl Cycles, CYCLES
#define SAVE_CYCLES movl CYCLES, Cycles

.macro LOAD_REGISTERS
	LOAD_CYCLES
	movl PCS, PC
#ifndef SPC700_C
	movl APUPCS, APUPC
#endif
	movw PP, FLAGS16
.endm

.macro STORE_REGISTERS
	SAVE_CYCLES
	movl PC, PCS
#ifndef SPC700_C
	movl APUPC, APUPCS
#endif
	movw FLAGS16, PP
.endm

.macro PUSH_REGISTERS
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
.endm

.macro POP_REGISTERS
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
.endm

.macro ccall name
	call \name
.endm

