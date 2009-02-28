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
#include "spcops.h"

.section	.rodata
.LC2:
	.string	"Sound CPU in unknown state executing %s at %04X\n%s\n"
.text

STOP:
	subl $100,%esp
	pushl %esi
	pushl %ebx
	movl APUPC,%edx
	movl APURAM,%eax
	subl %eax,%edx
	movl %edx,%eax
	andl $65535,%eax
	movl 112(%esp),%esi
	pushl %eax
	leal 12(%esp),%ebx
	pushl %ebx
#ifdef DEBUGGER
	call S9xAPUOPrint
#endif
	pushl %ebx
	movl APUPC,%edx
	movl APURAM,%eax
	subl %eax,%edx
	movl %edx,%eax
	pushl %eax
	pushl %esi
	pushl $.LC2
	pushl $String
	call sprintf
	pushl $String
	pushl $5
	pushl $4
	call S9xMessage
	xorl %edx,%edx
	movl CPU,%ecx
	addl $40,%esp
	movb %dl,APUTimerEnabled+2
	movb %dl,APUTimerEnabled+1
	popl %ebx
	movb %dl,APUTimerEnabled
	orl $1,%ecx
	popl %esi
	movb %dl,APUExecuting
	movl %ecx,CPU
	addl $100,%esp
	ret

ApuOp00:
	movl APUPC,%eax
	incl %eax
	movl %eax,APUPC
	ret

ApuOp01:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb ApuS,%dl
	addb $-2,%dl
	movb APUExtraRAM+31,%al
	movb %dl,ApuS
	andl $255,%eax
	movb APUExtraRAM+30,%dl
	sall $8,%eax
	andl $255,%edx
	movl APURAM,%ecx
	addl %eax,%edx
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp11:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+29,%al
	movb APUExtraRAM+28,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp21:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+27,%al
	movb APUExtraRAM+26,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp31:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+25,%al
	movb APUExtraRAM+24,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp41:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+23,%al
	movb APUExtraRAM+22,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp51:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+21,%al
	movb APUExtraRAM+20,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp61:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+19,%al
	movb APUExtraRAM+18,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp71:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+17,%al
	movb APUExtraRAM+16,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp81:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+15,%al
	movb APUExtraRAM+14,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp91:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+13,%al
	movb APUExtraRAM+12,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOpA1:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+11,%al
	movb APUExtraRAM+10,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOpB1:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+9,%al
	movb APUExtraRAM+8,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOpC1:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+7,%al
	movb APUExtraRAM+6,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOpD1:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+5,%al
	movb APUExtraRAM+4,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOpE1:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+3,%al
	movb APUExtraRAM+2,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOpF1:
	pushl %ebx
	movl APURAM,%edx
	movl APUPC,%ebx
	movl %edx,%eax
	subl %eax,%ebx
	movb ApuS,%cl
	movl %ebx,%eax
	andl $255,%ecx
	incl %eax
	movw %ax,255(%edx,%ecx)
	movb APUExtraRAM+1,%al
	movb APUExtraRAM+0,%dl
	andl $255,%eax
	movb ApuS,%bl
	andl $255,%edx
	sall $8,%eax
	movl APURAM,%ecx
	addb $-2,%bl
	addl %eax,%edx
	movb %bl,ApuS
	addl %ecx,%edx
	popl %ebx
	movl %edx,APUPC
	ret

ApuOp3F:
	pushl %ebx
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl APURAM,%edx
	movl %eax,APUAddress
	movl %edx,%eax
	movb ApuS,%cl
	movl APUPC,%ebx
	addl $-3,%eax
	andl $255,%ecx
	subl %eax,%ebx
	movw %bx,255(%edx,%ecx)
	movb ApuS,%bl
	movl APUAddress,%eax
	movl APURAM,%edx
	addb $-2,%bl
	addl %edx,%eax
	movb %bl,ApuS
	movl %eax,APUPC
	popl %ebx
	ret

ApuOp4F:
	pushl %ebx
	movl APUPC,%eax
	movb 1(%eax),%cl
	movl APURAM,%edx
	movb %cl,Work8
	movl %edx,%eax
	movl APUPC,%ebx
	movb ApuS,%cl
	addl $-2,%eax
	andl $255,%ecx
	subl %eax,%ebx
	movw %bx,255(%edx,%ecx)
	movb Work8,%al
	movb ApuS,%bl
	andl $255,%eax
	movl APURAM,%edx
	addb $-2,%bl
	addl $65280,%eax
	movb %bl,ApuS
	addl %edx,%eax
	popl %ebx
	movl %eax,APUPC
	ret

ApuOp02:
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb $1,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret

ApuOp22:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb $2,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp42:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb $4,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp62:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb $8,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp82:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb $16,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOpA2:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb $32,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOpC2:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb $64,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOpE2:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb $128,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp12:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $254,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp32:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $253,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp52:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $251,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp72:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $247,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp92:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $239,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOpB2:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $223,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOpD2:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $191,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOpF2:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $127,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret

ApuOp03:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $1,%al
	je .L292
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L292:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp23:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $2,%al
	je .L300
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L300:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp43:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $4,%al
	je .L308
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L308:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp63:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $8,%al
	je .L316
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L316:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp83:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $16,%al
	je .L324
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L324:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOpA3:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $32,%al
	je .L332
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L332:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOpC3:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $64,%al
	je .L340
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L340:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOpE3:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb %al,%al
	jge .L348
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L348:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp13:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $1,%al
	jne .L356
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L356:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp33:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $2,%al
	jne .L364
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L364:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp53:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $4,%al
	jne .L372
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L372:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp73:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $8,%al
	jne .L380
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L380:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp93:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $16,%al
	jne .L388
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L388:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOpB3:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $32,%al
	jne .L396
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L396:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOpD3:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb $64,%al
	jne .L404
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L404:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOpF3:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	testb %al,%al
	jl .L412
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L412:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp04:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%cl
	movl APUPC,%edx
	orb %cl,%al
	addl $4,%esp
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret

ApuOp05:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%cl
	movl APUPC,%edx
	orb %cl,%al
	addl $4,%esp
	addl $3,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret

ApuOp06:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%cl
	movl APUPC,%edx
	orb %cl,%al
	addl $4,%esp
	incl %edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret

ApuOp07:
	movb ApuX,%al
	movl APUPC,%edx
	andl $255,%eax
	movb 1(%edx),%cl
	addb %cl,%al
	xorl %edx,%edx
	movb %al,%dl
	movl APUDirectPage,%eax
	movw (%edx,%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	orb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret

ApuOp08:
	movl APUPC,%edx
	movb ApuA,%al
	movb 1(%edx),%cl
	addl $2,%edx
	orb %cl,%al
	movl %edx,APUPC
	movb %al,ApuA
	movb %al,APU_Zero
	ret

ApuOp09:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb Work8,%dl
	orb %al,%dl
	movb %dl,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%edx
	movb Work8,%al
	addl $3,%edx
	addl $16,%esp
	movb %al,APU_Zero
	movl %edx,APUPC
	ret

ApuOp14:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%dl
	movl APUPC,%ecx
	orb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret

ApuOp15:
	movl APUPC,%eax
	movb ApuX,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	orb %dl,%al
	addl $4,%esp
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret

ApuOp16:
	movl APUPC,%eax
	movb ApuY,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	orb %dl,%al
	addl $4,%esp
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret

ApuOp17:
	movl APUPC,%eax
	movb 1(%eax),%dl
	movl APUDirectPage,%eax
	andl $255,%edx
	movw (%edx,%eax),%ax
	movb ApuY,%dl
	andl $65535,%eax
	andl $255,%edx
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	orb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret

ApuOp18:
	movl APUPC,%edx
	movb 1(%edx),%al
	movb %al,Work8
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb Work8,%dl
	orb %al,%dl
	movb %dl,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%edx
	movb Work8,%al
	addl $3,%edx
	addl $12,%esp
	movb %al,APU_Zero
	movl %edx,APUPC
	ret

ApuOp19:
	pushl %ebx
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%bl
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	orb %bl,%al
	movb %al,Work8
	movb %al,APU_Zero
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $16,%esp
	incl %ecx
	popl %ebx
	movl %ecx,APUPC
	ret

ApuOp0A:
	movb APU_Carry,%dl
	testb %dl,%dl
	jnz .L468
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,%edx
	movl %eax,APUAddress
	shrl $13,%edx
	andl $8191,%eax
	movb %dl,APUBit
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	xorl %edx,%edx
	movb %al,%dl
	movb APUBit,%al
	andl $255,%eax
	addl $4,%esp
	btl %eax,%edx
	jnc .L468
	movb $1,APU_Carry
.L468:
	addl $3,APUPC
	ret

ApuOp2A:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,%edx
	movl %eax,APUAddress
	shrl $13,%edx
	andl $8191,%eax
	movb %dl,APUBit
	movl %eax,APUAddress
	movb APU_Carry,%dl
	testb %dl,%dl
	jne .L476
	pushl %eax
	call S9xAPUGetByte__FUl
	xorl %edx,%edx
	movb %al,%dl
	movb APUBit,%al
	andl $255,%eax
	addl $4,%esp
	btl %eax,%edx
	jc .L476
	movl $1,%ecx
	movb %cl,APU_Carry
.L476:
	movl APUPC,%eax
	addl $3,%eax
	movl %eax,APUPC
	ret

ApuOp4A:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,%edx
	movl %eax,APUAddress
	shrl $13,%edx
	andl $8191,%eax
	movb %dl,APUBit
	movl %eax,APUAddress
	movb APU_Carry,%dl
	testb %dl,%dl
	je .L484
	pushl %eax
	call S9xAPUGetByte__FUl
	movb APUBit,%cl
	andl $255,%eax
	andl $255,%ecx
	sarl %cl,%eax
	andl $1,%eax
	addl $4,%esp
	testl %eax,%eax
	jne .L484
	movb %al,APU_Carry
.L484:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret

ApuOp6A:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,%edx
	movl %eax,APUAddress
	shrl $13,%edx
	andl $8191,%eax
	movb %dl,APUBit
	movl %eax,APUAddress
	movb APU_Carry,%dl
	testb %dl,%dl
	je .L492
	pushl %eax
	call S9xAPUGetByte__FUl
	xorl %edx,%edx
	movb %al,%dl
	movb APUBit,%al
	andl $255,%eax
	addl $4,%esp
	btl %eax,%edx
	jnc .L492
	xorl %ecx,%ecx
	movb %cl,APU_Carry
.L492:
	movl APUPC,%eax
	addl $3,%eax
	movl %eax,APUPC
	ret

ApuOp8A:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,%edx
	movl %eax,APUAddress
	shrl $13,%edx
	andl $8191,%eax
	movb %dl,APUBit
	movl %eax,APUAddress
	movb APU_Carry,%dl
	testb %dl,%dl
	je .L502
	pushl %eax
	call S9xAPUGetByte__FUl
	xorl %edx,%edx
	movb %al,%dl
	movb APUBit,%al
	andl $255,%eax
	addl $4,%esp
	btl %eax,%edx
	jnc .L504
	xorl %ecx,%ecx
	movb %cl,APU_Carry
	jmp .L504
.L502:
	pushl %eax
	call S9xAPUGetByte__FUl
	xorl %edx,%edx
	movb %al,%dl
	movb APUBit,%al
	andl $255,%eax
	addl $4,%esp
	btl %eax,%edx
	jnc .L504
	movl $1,%eax
	movb %al,APU_Carry
.L504:
	movl APUPC,%edx
	addl $3,%edx
	movl %edx,APUPC
	ret

ApuOpAA:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,%edx
	movl %eax,APUAddress
	shrl $13,%edx
	andl $8191,%eax
	movb %dl,APUBit
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb APUBit,%cl
	andl $255,%eax
	andl $255,%ecx
	sarl %cl,%eax
	andl $1,%eax
	addl $4,%esp
	testb %al,%al
	je .L512
	movl $1,%ecx
	movb %cl,APU_Carry
	jmp .L513
.L512:
	movb %al,APU_Carry
.L513:
	movl APUPC,%eax
	addl $3,%eax
	movl %eax,APUPC
	ret

ApuOpCA:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,%edx
	movl %eax,APUAddress
	shrl $13,%edx
	andl $8191,%eax
	movb %dl,APUBit
	movl %eax,APUAddress
	movb APU_Carry,%dl
	testb %dl,%dl
	je .L520
	pushl %eax
	pushl %eax
	call S9xAPUGetByte__FUl
	movb APUBit,%cl
	movb %al,%dl
	movl $1,%eax
	andl $255,%ecx
	sall %cl,%eax
	orb %dl,%al
	jmp .L523
.L520:
	pushl %eax
	pushl %eax
	call S9xAPUGetByte__FUl
	movb APUBit,%cl
	movb %al,%dl
	movl $-2,%eax
	andl $255,%ecx
	roll %cl,%eax
	andb %dl,%al
.L523:
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	addl $8,%esp
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret
ApuOpEA:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	movl %eax,%edx
	andl $8191,%eax
	shrl $13,%edx
	pushl %eax
	movb %dl,APUBit
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb APUBit,%cl
	movb %al,%dl
	movl $1,%eax
	andl $255,%ecx
	sall %cl,%eax
	xorb %dl,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%eax
	addl $3,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOp0B:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movb %al,%dl
	addb %al,%al
	shrb $7,%dl
	movb %al,Work8
	movb %al,APU_Zero
	movb %dl,APU_Carry
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $12,%esp
	movl %eax,APUPC
	ret
ApuOp0C:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb %al,Work8
	shrb $7,%dl
	addb %al,%al
	movb %dl,APU_Carry
	movb %al,Work8
	movb %al,APU_Zero
	movl APUAddress,%edx
	andl $255,%eax
	pushl %edx
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%ecx
	addl $3,%ecx
	addl $12,%esp
	movl %ecx,APUPC
	ret
ApuOp1B:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movb %al,%cl
	addb %al,%al
	movl APUPC,%edx
	shrb $7,%cl
	movb %al,Work8
	movb %al,APU_Zero
	movb %cl,APU_Carry
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $12,%esp
	movl %eax,APUPC
	ret
ApuOp1C:
	movb ApuA,%al
	movb %al,%dl
	shrb $7,%dl
	movb %dl,APU_Carry
	movl APUPC,%edx
	addb %al,%al
	incl %edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp0D:
	pushl %edi
	pushl %esi
	pushl %ebx
	movb ApuP,%cl
	movb APU_Zero,%dl
	andb $60,%cl
	movb %dl,%bl
	movb %cl,ApuP
	movzbl APU_Overflow,%esi
	andb $128,%bl
	movzbl APU_Carry,%edi
	sall $6,%esi
	andl $255,%ebx
	andl $255,%ecx
	testb %dl,%dl
	jne .L552
	movl %ebx,%eax
	orb $2,%al
	orl %edi,%eax
	jmp .L556
.L552:
	movl %edi,%eax
	orl %ebx,%eax
.L556:
	orl %esi,%eax
	orl %ecx,%eax
	movb ApuS,%cl
	movl APURAM,%edx
	movb %al,ApuP
	andl $255,%ecx
	movb %al,256(%edx,%ecx)
	movb ApuS,%cl
	movl APUPC,%eax
	popl %ebx
	decb %cl
	incl %eax
	popl %esi
	movb %cl,ApuS
	movl %eax,APUPC
	popl %edi
	ret
ApuOp2D:
	movb ApuS,%cl
	movl APURAM,%edx
	movb ApuA,%al
	andl $255,%ecx
	movb %al,256(%edx,%ecx)
	movb ApuS,%dl
	movl APUPC,%ecx
	decb %dl
	incl %ecx
	movb %dl,ApuS
	movl %ecx,APUPC
	ret
ApuOp4D:
	movb ApuS,%cl
	movl APURAM,%edx
	movb ApuX,%al
	andl $255,%ecx
	movb %al,256(%edx,%ecx)
	movb ApuS,%al
	movl APUPC,%edx
	decb %al
	incl %edx
	movb %al,ApuS
	movl %edx,APUPC
	ret
ApuOp6D:
	movb ApuS,%cl
	movl APURAM,%edx
	movb ApuY,%al
	andl $255,%ecx
	movb %al,256(%edx,%ecx)
	movb ApuS,%cl
	movl APUPC,%eax
	decb %cl
	incl %eax
	movb %cl,ApuS
	movl %eax,APUPC
	ret
ApuOp8E:
	pushl %esi
	pushl %ebx
	movb ApuS,%dl
	incb %dl
	xorl %eax,%eax
	movl APURAM,%ebx
	movb %dl,%al
	movb %dl,ApuS
	movb 256(%ebx,%eax),%dl
	movb %dl,%al
	shrb $1,%al
	movb %dl,%cl
	notl %eax
	andb $128,%cl
	andl $1,%eax
	movl %ecx,%esi
	orb %al,%cl
	movb %cl,APU_Zero
	movb %dl,%al
	movb %dl,%cl
	andb $64,%al
	andb $1,%cl
	shrb $6,%al
	movb %dl,ApuP
	movb %cl,APU_Carry
	movb %al,APU_Overflow
	testb $32,%dl
	je .L578
	addl $256,%ebx
.L578:
	movl %ebx,APUDirectPage
	movl APUPC,%ecx
	popl %ebx
	incl %ecx
	popl %esi
	movl %ecx,APUPC
	ret
ApuOpAE:
	movb ApuS,%dl
	incb %dl
	xorl %eax,%eax
	movb %dl,ApuS
	movb %dl,%al
	movl APURAM,%ecx
	movl APUPC,%edx
	movb 256(%ecx,%eax),%al
	incl %edx
	movb %al,ApuA
	movl %edx,APUPC
	ret
ApuOpCE:
	movb ApuS,%cl
	incb %cl
	xorl %eax,%eax
	movl APURAM,%edx
	movb %cl,ApuS
	movb %cl,%al
	movb 256(%edx,%eax),%al
	movl APUPC,%edx
	incl %edx
	movb %al,ApuX
	movl %edx,APUPC
	ret
ApuOpEE:
	movb ApuS,%cl
	incb %cl
	xorl %eax,%eax
	movl APURAM,%edx
	movb %cl,ApuS
	movb %cl,%al
	movb 256(%edx,%eax),%al
	movl APUPC,%edx
	incl %edx
	movb %al,ApuY
	movl %edx,APUPC
	ret
ApuOp0E:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUAddress,%ecx
	movb %al,%dl
	pushl %ecx
	movb ApuA,%cl
	orb %cl,%al
	andl $255,%eax
	movb %dl,Work8
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movb Work8,%al
	movb ApuA,%dl
	movl APUPC,%ecx
	andb %dl,%al
	addl $12,%esp
	addl $3,%ecx
	movb %al,Work8
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp4E:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUAddress,%ecx
	pushl %ecx
	movb %al,%dl
	movb ApuA,%al
	notb %al
	andb %dl,%al
	andl $255,%eax
	movb %dl,Work8
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movb Work8,%al
	movb ApuA,%cl
	movl APUPC,%edx
	andb %cl,%al
	addl $12,%esp
	addl $3,%edx
	movb %al,Work8
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp0F:
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl APURAM,%edx
	movl %edx,%eax
	movb ApuS,%cl
	movl APUPC,%ebx
	decl %eax
	andl $255,%ecx
	subl %eax,%ebx
	movw %bx,255(%edx,%ecx)
	movb ApuS,%al
	addb $-2,%al
	movb APU_Zero,%dl
	movb %al,ApuS
	movb %dl,%al
	movb ApuP,%cl
	movzbl APU_Carry,%edi
	andb $128,%al
	andb $60,%cl
	movzbl APU_Overflow,%esi
	movl %eax,%ebp
	movb %cl,ApuP
	sall $6,%esi
	andl $255,%ebp
	andl $255,%ecx
	testb %dl,%dl
	jne .L603
	movl %ebp,%eax
	orb $2,%al
	orl %edi,%eax
	jmp .L607
.L603:
	movl %edi,%eax
	orl %ebp,%eax
.L607:
	orl %esi,%eax
	orl %ecx,%eax
	movb ApuS,%cl
	movl APURAM,%edx
	movb %al,ApuP
	andl $255,%ecx
	movb %al,256(%edx,%ecx)
	movb ApuP,%al
	movb ApuS,%dl
	orb $16,%al
	decb %dl
	andb $251,%al
	movb %dl,ApuS
	movb %al,ApuP
	movl APURAM,%ecx
	movb APUExtraRAM+0x20,%dl
	movb APUExtraRAM+0x21,%al
	popl %ebx
	andl $255,%edx
	andl $255,%eax
	popl %esi
	addl %ecx,%edx
	sall $8,%eax
	popl %edi
	addl %eax,%edx
	popl %ebp
	movl %edx,APUPC
	ret
ApuOpEF:
	movl APUPC,%edx
	xorl %eax,%eax
	incl %edx
	movb %al,APUExecuting
	movl %edx,APUPC
	ret
ApuOpFF:
	movl APUPC,%eax
	xorl %ecx,%ecx
	incl %eax
	movb %cl,APUExecuting
	movl %eax,APUPC
	ret
ApuOp10:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movw %dx,Int16
	movb APU_Zero,%al
	testb %al,%al
	jl .L617
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	testb $0xff,Shutdown
	jz .L627
	movl APUWaitAddress1,%ecx
	cmpl %ecx,%eax
	je .L619
	movl APUWaitAddress2,%edx
	cmpl %edx,%eax
	jne .L627
.L619:
	movl APUWaitCounter,%eax
	testl %eax,%eax
	jne .L620
	movb CPUExecuting,%cl
	testb %cl,%cl
	jne .L621
	movl NextEvent,%eax
	movl %eax,CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer 
	ret
.L621:
	movb %al,APUExecuting
	ret
.L620:
	cmpl $1,%eax
	jbe .L624
	movl $1,%eax
	movl %eax,APUWaitCounter
	ret
.L624:
	decl %eax
	movl %eax,APUWaitCounter
	ret
.L617:
	addl $2,%ecx
	movl %ecx,APUPC
.L627:
	ret
ApuOp30:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movw %dx,Int16
	movb APU_Zero,%al
	testb %al,%al
	jge .L629
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	movb Shutdown,%dl
	testb %dl,%dl
	je .L639
	movl APUWaitAddress1,%ecx
	cmpl %ecx,%eax
	je .L631
	movl APUWaitAddress2,%edx
	cmpl %edx,%eax
	jne .L639
.L631:
	movl APUWaitCounter,%eax
	testl %eax,%eax
	jne .L632
	movb CPUExecuting,%cl
	testb %cl,%cl
	jne .L633
	movl NextEvent,%eax
	movl %eax,CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer 
	ret
.L633:
	movb %al,APUExecuting
	ret
.L632:
	cmpl $1,%eax
	jbe .L636
	movl $1,%eax
	movl %eax,APUWaitCounter
	ret
.L636:
	decl %eax
	movl %eax,APUWaitCounter
	ret
.L629:
	addl $2,%ecx
	movl %ecx,APUPC
.L639:
	ret
ApuOp90:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movw %dx,Int16
	movb APU_Carry,%al
	testb %al,%al
	jne .L641
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	movb Shutdown,%dl
	testb %dl,%dl
	je .L651
	movl APUWaitAddress1,%ecx
	cmpl %ecx,%eax
	je .L643
	movl APUWaitAddress2,%edx
	cmpl %edx,%eax
	jne .L651
.L643:
	movl APUWaitCounter,%eax
	testl %eax,%eax
	jne .L644
	movb CPUExecuting,%cl
	testb %cl,%cl
	jne .L645
	movl NextEvent,%eax
	movl %eax,CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer 	
	ret
.L645:
	movb %al,APUExecuting
	ret
.L644:
	cmpl $1,%eax
	jbe .L648
	movl $1,%eax
	movl %eax,APUWaitCounter
	ret
.L648:
	decl %eax
	movl %eax,APUWaitCounter
	ret
.L641:
	addl $2,%ecx
	movl %ecx,APUPC
.L651:
	ret
ApuOpB0:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movw %dx,Int16
	movb APU_Carry,%al
	testb %al,%al
	je .L653
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	movb Shutdown,%dl
	testb %dl,%dl
	je .L663
	movl APUWaitAddress1,%ecx
	cmpl %ecx,%eax
	je .L655
	movl APUWaitAddress2,%edx
	cmpl %edx,%eax
	jne .L663
.L655:
	movl APUWaitCounter,%eax
	testl %eax,%eax
	jne .L656
	movb CPUExecuting,%cl
	testb %cl,%cl
	jne .L657
	movl NextEvent,%eax
	movl %eax,CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer
	ret
.L657:
	movb %al,APUExecuting
	ret
.L656:
	cmpl $1,%eax
	jbe .L660
	movl $1,%eax
	movl %eax,APUWaitCounter
	ret
.L660:
	decl %eax
	movl %eax,APUWaitCounter
	ret
.L653:
	addl $2,%ecx
	movl %ecx,APUPC
.L663:
	ret
ApuOpD0:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movw %dx,Int16
	movb APU_Zero,%al
	testb %al,%al
	je .L665
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	movb Shutdown,%dl
	testb %dl,%dl
	je .L675
	movl APUWaitAddress1,%ecx
	cmpl %ecx,%eax
	je .L667
	movl APUWaitAddress2,%edx
	cmpl %edx,%eax
	jne .L675
.L667:
	movl APUWaitCounter,%eax
	testl %eax,%eax
	jne .L668
	movb CPUExecuting,%cl
	testb %cl,%cl
	jne .L669
	movl NextEvent,%eax
	movl %eax,CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer 	
	ret
.L669:
	movb %al,APUExecuting
	ret
.L668:
	cmpl $1,%eax
	jbe .L672
	movl $1,%eax
	movl %eax,APUWaitCounter
	ret
.L672:
	decl %eax
	movl %eax,APUWaitCounter
	ret
.L665:
	addl $2,%ecx
	movl %ecx,APUPC
.L675:
	ret
ApuOpF0:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movw %dx,Int16
	movb APU_Zero,%al
	testb %al,%al
	jne .L677
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	movb Shutdown,%dl
	testb %dl,%dl
	je .L687
	movl APUWaitAddress1,%ecx
	cmpl %ecx,%eax
	je .L679
	movl APUWaitAddress2,%edx
	cmpl %edx,%eax
	jne .L687
.L679:
	movl APUWaitCounter,%eax
	testl %eax,%eax
	jne .L680
	movb CPUExecuting,%cl
	testb %cl,%cl
	jne .L681
	movl NextEvent,%eax
	movl %eax,CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer	
	ret
.L681:
	movb %al,APUExecuting
	ret
.L680:
	cmpl $1,%eax
	jbe .L684
	movl $1,%eax
	movl %eax,APUWaitCounter
	ret
.L684:
	decl %eax
	movl %eax,APUWaitCounter
	ret
.L677:
	addl $2,%ecx
	movl %ecx,APUPC
.L687:
	ret
ApuOp50:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movw %dx,Int16
	movb APU_Overflow,%al
	testb %al,%al
	jne .L693
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	ret
.L693:
	addl $2,%ecx
	movl %ecx,APUPC
	ret
ApuOp70:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movw %dx,Int16
	movb APU_Overflow,%al
	testb %al,%al
	je .L701
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	ret
.L701:
	addl $2,%ecx
	movl %ecx,APUPC
	ret
ApuOp2F:
	movl APUPC,%eax
	movb 1(%eax),%dl
	addl $2,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	movl APURAM,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUPC
	ret
ApuOp80:
	movl APUPC,%eax
	movl $1,%ecx
	incl %eax
	movb %cl,APU_Carry
	movl %eax,APUPC
	ret
ApuOpED:
	movb APU_Carry,%dl
	movl APUPC,%ecx
	xorb $1,%dl
	incl %ecx
	movb %dl,APU_Carry
	movl %ecx,APUPC
	ret
ApuOp40:
	movb ApuP,%dl
	movl APURAM,%eax
	movl APUPC,%ecx
	orb $32,%dl
	addl $256,%eax
	incl %ecx
	movb %dl,ApuP
	movl %eax,APUDirectPage
	movl %ecx,APUPC
	ret
ApuOp1A:
	pushl %ebx
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%bl
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%dl
	xorl %eax,%eax
	movb %bl,%al
	salw $8,%dx
	addl %edx,%eax
	decl %eax
	movl APUPC,%edx
	movw %ax,Work16
	movb 1(%edx),%al
	andl $255,%eax
	pushl %eax
	movb Work16,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	movb Work16+1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movb Work16+1,%al
	addl $24,%esp
	andl $255,%eax
	cmpw $0,Work16
	je .L724
	orb $1,%al
.L724:
	movl APUPC,%edx
	addl $2,%edx
	movb %al,APU_Zero
	popl %ebx
	movl %edx,APUPC
	ret
ApuOp5A:
	pushl %ebx
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%bl
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	xorl %edx,%edx
	movb %bl,%dl
	salw $8,%ax
	addl %eax,%edx
	movw %dx,Work16
	xorl %eax,%eax
	movw ApuA,%ax
	andl $65535,%edx
	subl %edx,%eax
	movl %eax,Int32
	notl %eax
	shrl $31,%eax
	movb %al,APU_Carry
	movb Int32+1,%al
	addl $8,%esp
	andl $255,%eax
	cmpw $0,Int32
	je .L730
	orb $1,%al
.L730:
	movl APUPC,%ecx
	addl $2,%ecx
	movb %al,APU_Zero
	popl %ebx
	movl %ecx,APUPC
	ret
ApuOp3A:
	pushl %ebx
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%bl
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%dl
	xorl %eax,%eax
	movb %bl,%al
	salw $8,%dx
	addl %edx,%eax
	incl %eax
	movl APUPC,%edx
	movw %ax,Work16
	movb 1(%edx),%al
	andl $255,%eax
	pushl %eax
	movb Work16,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	movb Work16+1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movb Work16+1,%al
	addl $24,%esp
	andl $255,%eax
	cmpw $0,Work16
	je .L736
	orb $1,%al
.L736:
	movl APUPC,%edx
	addl $2,%edx
	movb %al,APU_Zero
	popl %ebx
	movl %edx,APUPC
	ret
ApuOp7A:
	pushl %ebx
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%bl
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	xorl %edx,%edx
	movb %bl,%dl
	salw $8,%ax
	xorl %ecx,%ecx
	addl %eax,%edx
	movw ApuA,%cx
	xorl %eax,%eax
	movw %dx,%ax
	addl %eax,%ecx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpl $65535,%ecx
	seta %al
	movb %al,APU_Carry
	movw ApuA,%ax
	movw %dx,Work16
	xorl %edx,%eax
	xorl %ecx,%edx
	notw %ax
	andl %edx,%eax
	addl $8,%esp
	movl %ecx,Work32
	andl $-32768,%eax
	testw %ax,%ax
	je .L739
	movl $1,%edx
	movb %dl,APU_Overflow
	jmp .L740
.L739:
	movb %al,APU_Overflow
.L740:
	movw Work32,%ax
	movl %eax,%edx
	shrw $8,%dx
	movw %ax,ApuA
	andl $65535,%edx
	testw %ax,%ax
	je .L741
	orl $1,%edx
.L741:
	movl APUPC,%ecx
	addl $2,%ecx
	movb %dl,APU_Zero
	popl %ebx
	movl %ecx,APUPC
	ret
ApuOp9A:
	pushl %ebx
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%bl
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	xorl %ecx,%ecx
	movb %bl,%cl
	salw $8,%ax
	xorl %edx,%edx
	addl %eax,%ecx
	xorl %eax,%eax
	movw ApuA,%ax
	movw %cx,%dx
	subl %edx,%eax
	movb ApuP,%dl
	movl %eax,Int32
	andb $247,%dl
	notl %eax
	movb %dl,ApuP
	shrl $31,%eax
	movw ApuA,%dx
	movb %al,APU_Carry
	movl %edx,%eax
	movw %cx,Work16
	xorl %ecx,%eax
	addl $8,%esp
	testw %ax,%ax
	jge .L744
	movl %edx,%eax
	xorw Int32,%ax
	jns .L744
	movl $1,%ecx
	movb %cl,APU_Overflow
	jmp .L745
.L744:
	xorl %eax,%eax
	movb %al,APU_Overflow
.L745:
	movb ApuA,%dl
	movb Work16,%cl
	movb %dl,%al
	xorb %cl,%al
	jns .L746
	movb %dl,%al
	movb Int32,%cl
	xorb %cl,%al
	jns .L746
	movb ApuP,%al
	orb $8,%al
	movb %al,ApuP
.L746:
	movw Int32,%ax
	movl %eax,%edx
	shrw $8,%dx
	movw %ax,ApuA
	andl $65535,%edx
	testw %ax,%ax
	je .L747
	orl $1,%edx
.L747:
	movl APUPC,%ecx
	addl $2,%ecx
	movb %dl,APU_Zero
	popl %ebx
	movl %ecx,APUPC
	ret
ApuOpBA:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,ApuA
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,ApuY
	addl $8,%esp
	andl $255,%eax
	cmpw $0,ApuA
	je .L753
	orb $1,%al
.L753:
	movl APUPC,%edx
	addl $2,%edx
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOpDA:
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	movb 1(%eax),%al
	incb %al
	andl $255,%eax
	pushl %eax
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $16,%esp
	movl %ecx,APUPC
	ret
ApuOp64:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movb %al,APU_Carry
	movl APUPC,%eax
	movb Int16,%cl
	addl $2,%eax
	addl $4,%esp
	movb %cl,APU_Zero
	movl %eax,APUPC
	ret
ApuOp65:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	movl APUPC,%ecx
	addl $4,%esp
	notw %ax
	addl $3,%ecx
	movb Int16,%dl
	shrw $15,%ax
	movl %ecx,APUPC
	movb %dl,APU_Zero
	movb %al,APU_Carry
	ret
ApuOp66:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movl APUPC,%edx
	movb %al,APU_Carry
	incl %edx
	movb Int16,%al
	addl $4,%esp
	movl %edx,APUPC
	movb %al,APU_Zero
	ret
ApuOp67:
	movb ApuX,%al
	movl APUPC,%edx
	andl $255,%eax
	movb 1(%edx),%cl
	addb %cl,%al
	xorl %edx,%edx
	movb %al,%dl
	movl APUDirectPage,%eax
	movw (%edx,%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	movl APUPC,%ecx
	addl $4,%esp
	notw %ax
	addl $2,%ecx
	movb Int16,%dl
	shrw $15,%ax
	movl %ecx,APUPC
	movb %dl,APU_Zero
	movb %al,APU_Carry
	ret
ApuOp68:
	movl APUPC,%ecx
	movb 1(%ecx),%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movb %al,APU_Carry
	addl $2,%ecx
	movb Int16,%al
	movl %ecx,APUPC
	movb %al,APU_Zero
	ret
ApuOp69:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,W1
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movb W1,%dl
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movl APUPC,%edx
	movb %al,APU_Carry
	addl $3,%edx
	movb Int16,%al
	addl $8,%esp
	movl %edx,APUPC
	movb %al,APU_Zero
	ret
ApuOp74:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	movl APUPC,%ecx
	addl $4,%esp
	notw %ax
	addl $2,%ecx
	movb Int16,%dl
	shrw $15,%ax
	movl %ecx,APUPC
	movb %dl,APU_Zero
	movb %al,APU_Carry
	ret
ApuOp75:
	movl APUPC,%eax
	movb ApuX,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movl APUPC,%edx
	movb %al,APU_Carry
	addl $3,%edx
	movb Int16,%al
	addl $4,%esp
	movl %edx,APUPC
	movb %al,APU_Zero
	ret
ApuOp76:
	movl APUPC,%eax
	movb ApuY,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movb %al,APU_Carry
	movl APUPC,%eax
	movb Int16,%cl
	addl $3,%eax
	addl $4,%esp
	movb %cl,APU_Zero
	movl %eax,APUPC
	ret
ApuOp77:
	movl APUPC,%eax
	movb 1(%eax),%dl
	movl APUDirectPage,%eax
	andl $255,%edx
	movw (%edx,%eax),%ax
	movb ApuY,%dl
	andl $65535,%eax
	andl $255,%edx
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb ApuA,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	movl APUPC,%ecx
	addl $4,%esp
	notw %ax
	addl $2,%ecx
	movb Int16,%dl
	shrw $15,%ax
	movl %ecx,APUPC
	movb %dl,APU_Zero
	movb %al,APU_Carry
	ret
ApuOp78:
	movl APUPC,%edx
	movb 1(%edx),%al
	movb %al,Work8
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,W1
	movb Work8,%dl
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movl APUPC,%edx
	movb %al,APU_Carry
	addl $3,%edx
	movb Int16,%al
	addl $4,%esp
	movl %edx,APUPC
	movb %al,APU_Zero
	ret
ApuOp79:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,W1
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%dl
	movb W1,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movb %al,APU_Carry
	movl APUPC,%eax
	movb Int16,%cl
	incl %eax
	addl $8,%esp
	movb %cl,APU_Zero
	movl %eax,APUPC
	ret
ApuOp1E:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb ApuX,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	movl APUPC,%ecx
	addl $4,%esp
	notw %ax
	addl $3,%ecx
	movb Int16,%dl
	shrw $15,%ax
	movl %ecx,APUPC
	movb %dl,APU_Zero
	movb %al,APU_Carry
	ret
ApuOp3E:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%dl
	movb ApuX,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movb %al,APU_Carry
	movl APUPC,%eax
	movb Int16,%cl
	addl $2,%eax
	addl $4,%esp
	movb %cl,APU_Zero
	movl %eax,APUPC
	ret
ApuOpC8:
	movl APUPC,%ecx
	movb ApuX,%al
	movb 1(%ecx),%dl
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movb %al,APU_Carry
	addl $2,%ecx
	movb Int16,%al
	movl %ecx,APUPC
	movb %al,APU_Zero
	ret
ApuOp5E:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb ApuY,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	movl APUPC,%ecx
	addl $4,%esp
	notw %ax
	addl $3,%ecx
	movb Int16,%dl
	shrw $15,%ax
	movl %ecx,APUPC
	movb %dl,APU_Zero
	movb %al,APU_Carry
	ret
ApuOp7E:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%dl
	movb ApuY,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	shrw $15,%ax
	movb %al,APU_Carry
	movl APUPC,%eax
	movb Int16,%cl
	addl $2,%eax
	addl $4,%esp
	movb %cl,APU_Zero
	movl %eax,APUPC
	ret
ApuOpAD:
	movl APUPC,%ecx
	movb 1(%ecx),%dl
	movb ApuY,%al
	movb %dl,Work8
	andl $255,%eax
	andl $255,%edx
	subl %edx,%eax
	movw %ax,Int16
	notw %ax
	addl $2,%ecx
	movb Int16,%dl
	shrw $15,%ax
	movl %ecx,APUPC
	movb %dl,APU_Zero
	movb %al,APU_Carry
	ret
ApuOp1F:
	pushl %ebx
	movl APUPC,%eax
	movw 1(%eax),%ax
	movb ApuX,%dl
	andl $65535,%eax
	andl $255,%edx
	movl %eax,APUAddress
	addl %edx,%eax
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%bl
	movb ApuX,%al
	movl APUAddress,%ecx
	andl $255,%eax
	addl %ecx,%eax
	incl %eax
	pushl %eax
	call S9xAPUGetByte__FUl
	xorl %edx,%edx
	movb %bl,%dl
	movl APURAM,%ecx
	andl $255,%eax
	addl %ecx,%edx
	sall $8,%eax
	addl $8,%esp
	addl %eax,%edx
	popl %ebx
	movl %edx,APUPC
	ret
ApuOp5F:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl APURAM,%edx
	movl %eax,APUAddress
	addl %edx,%eax
	movl %eax,APUPC
	ret
ApuOp20:
	movb ApuP,%cl
	movl APUPC,%edx
	movl APURAM,%eax
	andb $223,%cl
	incl %edx
	movl %eax,APUDirectPage
	movb %cl,ApuP
	movl %edx,APUPC
	ret
ApuOp60:
	movl APUPC,%eax
	xorl %ecx,%ecx
	incl %eax
	movb %cl,APU_Carry
	movl %eax,APUPC
	ret
ApuOpE0:
	movb ApuP,%dl
	movl APUPC,%eax
	xorl %ecx,%ecx
	andb $247,%dl
	incl %eax
	movb %cl,APU_Overflow
	movb %dl,ApuP
	movl %eax,APUPC
	ret
ApuOp24:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%cl
	movl APUPC,%edx
	andb %cl,%al
	addl $4,%esp
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp25:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%cl
	movl APUPC,%edx
	andb %cl,%al
	addl $4,%esp
	addl $3,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp26:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%cl
	movl APUPC,%edx
	andb %cl,%al
	addl $4,%esp
	incl %edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp27:
	movb ApuX,%al
	movl APUPC,%edx
	andl $255,%eax
	movb 1(%edx),%cl
	addb %cl,%al
	xorl %edx,%edx
	movb %al,%dl
	movl APUDirectPage,%eax
	movw (%edx,%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	andb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp28:
	movl APUPC,%edx
	movb ApuA,%al
	movb 1(%edx),%cl
	addl $2,%edx
	andb %cl,%al
	movl %edx,APUPC
	movb %al,ApuA
	movb %al,APU_Zero
	ret
ApuOp29:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb Work8,%dl
	andb %al,%dl
	movb %dl,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%edx
	movb Work8,%al
	addl $3,%edx
	addl $16,%esp
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp34:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%dl
	movl APUPC,%ecx
	andb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp35:
	movl APUPC,%eax
	movb ApuX,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	andb %dl,%al
	addl $4,%esp
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp36:
	movl APUPC,%eax
	movb ApuY,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	andb %dl,%al
	addl $4,%esp
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp37:
	movl APUPC,%eax
	movb 1(%eax),%dl
	movl APUDirectPage,%eax
	andl $255,%edx
	movw (%edx,%eax),%ax
	movb ApuY,%dl
	andl $65535,%eax
	andl $255,%edx
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	andb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp38:
	movl APUPC,%edx
	movb 1(%edx),%al
	movb %al,Work8
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb Work8,%dl
	andb %al,%dl
	movb %dl,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%edx
	movb Work8,%al
	addl $3,%edx
	addl $12,%esp
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp39:
	pushl %ebx
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%bl
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	andb %bl,%al
	movb %al,Work8
	movb %al,APU_Zero
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $16,%esp
	incl %ecx
	popl %ebx
	movl %ecx,APUPC
	ret
ApuOp2B:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movb APU_Carry,%dl
	andl $255,%eax
	andl $255,%edx
	addw %ax,%ax
	orl %edx,%eax
	movw %ax,Work16
	cmpw $255,%ax
	seta %al
	movb %al,APU_Carry
	movb Work16,%al
	movl APUPC,%ecx
	movb %al,Work8
	movb %al,APU_Zero
	movb 1(%ecx),%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOp2C:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,Work8
	movb APU_Carry,%dl
	andl $255,%eax
	andl $255,%edx
	addw %ax,%ax
	orl %edx,%eax
	movw %ax,Work16
	cmpw $255,%ax
	seta %al
	movb %al,APU_Carry
	movb Work16,%al
	movb %al,Work8
	movb %al,APU_Zero
	addl $4,%esp
	movl APUAddress,%edx
	movb Work8,%al
	pushl %edx
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%ecx
	addl $3,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOp3B:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movb APU_Carry,%dl
	andl $255,%eax
	andl $255,%edx
	addw %ax,%ax
	orl %edx,%eax
	movw %ax,Work16
	cmpw $255,%ax
	seta %al
	movb %al,APU_Carry
	movb Work16,%al
	movb %al,Work8
	movb %al,APU_Zero
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOp3C:
	movb ApuA,%al
	movb APU_Carry,%dl
	andl $255,%eax
	andl $255,%edx
	addw %ax,%ax
	orl %edx,%eax
	movw %ax,Work16
	cmpw $255,%ax
	seta %al
	movl APUPC,%edx
	movb %al,APU_Carry
	incl %edx
	movb Work16,%al
	movl %edx,APUPC
	movb %al,ApuA
	movb %al,APU_Zero
	ret
ApuOp2E:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	movb ApuA,%cl
	cmpb %cl,%al
	je .L916
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	movb Shutdown,%cl
	testb %cl,%cl
	je .L926
	movl APUWaitAddress1,%edx
	cmpl %edx,%eax
	je .L918
	movl APUWaitAddress2,%ecx
	cmpl %ecx,%eax
	jne .L926
.L918:
	movl APUWaitCounter,%eax
	testl %eax,%eax
	jne .L919
	movb CPUExecuting,%dl
	testb %dl,%dl
	jne .L920
	movl NextEvent,%eax
	movl %eax,CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer	
	ret
.L920:
	movb %al,APUExecuting
	ret
.L919:
	cmpl $1,%eax
	jbe .L923
	movl $1,%ecx
	movl %ecx,APUWaitCounter
	ret
.L923:
	decl %eax
	movl %eax,APUWaitCounter
	ret
.L916:
	movl APUPC,%eax
	addl $3,%eax
	movl %eax,APUPC
.L926:
	ret
ApuOpDE:
	movl APUPC,%ecx
	movb ApuX,%al
	movb 1(%ecx),%dl
	andl $255,%eax
	andl $255,%edx
	addl %eax,%edx
	movw %dx,Work16
	movl APUPC,%eax
	movb 2(%ecx),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	movb Work16,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	movb ApuA,%dl
	cmpb %dl,%al
	je .L928
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	movb Shutdown,%dl
	testb %dl,%dl
	je .L938
	movl APUWaitAddress1,%ecx
	cmpl %ecx,%eax
	je .L930
	movl APUWaitAddress2,%edx
	cmpl %edx,%eax
	jne .L938
.L930:
	movl APUWaitCounter,%eax
	testl %eax,%eax
	jne .L931
	movb CPUExecuting,%cl
	testb %cl,%cl
	jne .L932
	movl NextEvent,%eax
	movl %eax,CYCLES
	SAVE_CYCLES
	ccall S9xUpdateAPUTimer	
	ret
.L932:
	movb %al,APUExecuting
	ret
.L931:
	cmpl $1,%eax
	jbe .L935
	movl $1,%eax
	movl %eax,APUWaitCounter
	ret
.L935:
	decl %eax
	movl %eax,APUWaitCounter
	ret
.L928:
	movl APUPC,%edx
	addl $3,%edx
	movl %edx,APUPC
.L938:
	ret
ApuOp3D:
	movb ApuX,%al
	incb %al
	movb %al,ApuX
	movb %al,APU_Zero
	movl APUWaitCounter,%ecx
	movl APUPC,%eax
	incl %ecx
	incl %eax
	movl %ecx,APUWaitCounter
	movl %eax,APUPC
	ret
ApuOpFC:
	movb ApuY,%al
	movl APUWaitCounter,%edx
	movl APUPC,%ecx
	incb %al
	incl %edx
	incl %ecx
	movb %al,ApuY
	movb %al,APU_Zero
	movl %edx,APUWaitCounter
	movl %ecx,APUPC
	ret
ApuOp1D:
	movb ApuX,%al
	movl APUWaitCounter,%edx
	movl APUPC,%ecx
	decb %al
	incl %edx
	incl %ecx
	movb %al,ApuX
	movb %al,APU_Zero
	movl %edx,APUWaitCounter
	movl %ecx,APUPC
	ret
ApuOpDC:
	movb ApuY,%al
	movl APUWaitCounter,%edx
	movl APUPC,%ecx
	decb %al
	incl %edx
	incl %ecx
	movb %al,ApuY
	movb %al,APU_Zero
	movl %edx,APUWaitCounter
	movl %ecx,APUPC
	ret
ApuOpAB:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	incb %al
	movb %al,Work8
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movb Work8,%dl
	movl APUWaitCounter,%ecx
	movl APUPC,%eax
	addl $12,%esp
	incl %ecx
	addl $2,%eax
	movb %dl,APU_Zero
	movl %ecx,APUWaitCounter
	movl %eax,APUPC
	ret
ApuOpAC:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	incb %al
	movl APUAddress,%edx
	movb %al,Work8
	pushl %edx
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movb Work8,%al
	movb %al,APU_Zero
	movl APUWaitCounter,%ecx
	movl APUPC,%eax
	incl %ecx
	addl $3,%eax
	addl $12,%esp
	movl %ecx,APUWaitCounter
	movl %eax,APUPC
	ret
ApuOpBB:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	incb %al
	movb %al,Work8
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movb Work8,%dl
	movl APUWaitCounter,%ecx
	movl APUPC,%eax
	addl $12,%esp
	incl %ecx
	addl $2,%eax
	movb %dl,APU_Zero
	movl %ecx,APUWaitCounter
	movl %eax,APUPC
	ret
ApuOpBC:
	movb ApuA,%al
	movl APUWaitCounter,%edx
	movl APUPC,%ecx
	incb %al
	incl %edx
	incl %ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUWaitCounter
	movl %ecx,APUPC
	ret
ApuOp8B:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	decb %al
	movb %al,Work8
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movb Work8,%dl
	movl APUWaitCounter,%ecx
	movl APUPC,%eax
	addl $12,%esp
	incl %ecx
	addl $2,%eax
	movb %dl,APU_Zero
	movl %ecx,APUWaitCounter
	movl %eax,APUPC
	ret
ApuOp8C:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	decb %al
	movl APUAddress,%edx
	movb %al,Work8
	pushl %edx
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movb Work8,%al
	movb %al,APU_Zero
	movl APUWaitCounter,%ecx
	movl APUPC,%eax
	incl %ecx
	addl $3,%eax
	addl $12,%esp
	movl %ecx,APUWaitCounter
	movl %eax,APUPC
	ret
ApuOp9B:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	decb %al
	movb %al,Work8
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movb Work8,%dl
	movl APUWaitCounter,%ecx
	movl APUPC,%eax
	addl $12,%esp
	incl %ecx
	addl $2,%eax
	movb %dl,APU_Zero
	movl %ecx,APUWaitCounter
	movl %eax,APUPC
	ret
ApuOp9C:
	movb ApuA,%al
	movl APUWaitCounter,%edx
	movl APUPC,%ecx
	decb %al
	incl %edx
	incl %ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUWaitCounter
	movl %ecx,APUPC
	ret
ApuOp44:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%cl
	movl APUPC,%edx
	xorb %cl,%al
	addl $4,%esp
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp45:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%cl
	movl APUPC,%edx
	xorb %cl,%al
	addl $4,%esp
	addl $3,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp46:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%cl
	movl APUPC,%edx
	xorb %cl,%al
	addl $4,%esp
	incl %edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp47:
	movb ApuX,%al
	movl APUPC,%edx
	andl $255,%eax
	movb 1(%edx),%cl
	addb %cl,%al
	xorl %edx,%edx
	movb %al,%dl
	movl APUDirectPage,%eax
	movw (%edx,%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	xorb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp48:
	movl APUPC,%edx
	movb ApuA,%al
	movb 1(%edx),%cl
	addl $2,%edx
	xorb %cl,%al
	movl %edx,APUPC
	movb %al,ApuA
	movb %al,APU_Zero
	ret
ApuOp49:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb Work8,%dl
	xorb %al,%dl
	movb %dl,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%edx
	movb Work8,%al
	addl $3,%edx
	addl $16,%esp
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp54:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%dl
	movl APUPC,%ecx
	xorb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp55:
	movl APUPC,%eax
	movb ApuX,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	xorb %dl,%al
	addl $4,%esp
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp56:
	movl APUPC,%eax
	movb ApuY,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	xorb %dl,%al
	addl $4,%esp
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp57:
	movl APUPC,%eax
	movb 1(%eax),%dl
	movl APUDirectPage,%eax
	andl $255,%edx
	movw (%edx,%eax),%ax
	movb ApuY,%dl
	andl $65535,%eax
	andl $255,%edx
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movl APUPC,%ecx
	xorb %dl,%al
	addl $4,%esp
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOp58:
	movl APUPC,%edx
	movb 1(%edx),%al
	movb %al,Work8
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb Work8,%dl
	xorb %al,%dl
	movb %dl,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%edx
	movb Work8,%al
	addl $3,%edx
	addl $12,%esp
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp59:
	pushl %ebx
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%bl
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	xorb %bl,%al
	movb %al,Work8
	movb %al,APU_Zero
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $16,%esp
	incl %ecx
	popl %ebx
	movl %ecx,APUPC
	ret
ApuOp4B:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movb %al,%dl
	shrb $1,%al
	andb $1,%dl
	movb %al,Work8
	movb %al,APU_Zero
	movb %dl,APU_Carry
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $12,%esp
	movl %eax,APUPC
	ret
ApuOp4C:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%dl
	movb %al,Work8
	andb $1,%dl
	shrb $1,%al
	movb %dl,APU_Carry
	movb %al,Work8
	movb %al,APU_Zero
	movl APUAddress,%edx
	andl $255,%eax
	pushl %edx
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%ecx
	addl $3,%ecx
	addl $12,%esp
	movl %ecx,APUPC
	ret
ApuOp5B:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movb %al,%cl
	shrb $1,%al
	movl APUPC,%edx
	andb $1,%cl
	movb %al,Work8
	movb %al,APU_Zero
	movb %cl,APU_Carry
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $12,%esp
	movl %eax,APUPC
	ret
ApuOp5C:
	movb ApuA,%al
	movb %al,%dl
	andb $1,%dl
	movb %dl,APU_Carry
	movl APUPC,%edx
	shrb $1,%al
	incl %edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp7D:
	movl APUPC,%ecx
	movb ApuX,%al
	incl %ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpDD:
	movl APUPC,%edx
	movb ApuY,%al
	incl %edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp5D:
	movl APUPC,%ecx
	movb ApuA,%al
	incl %ecx
	movb %al,ApuX
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpFD:
	movl APUPC,%edx
	movb ApuA,%al
	incl %edx
	movb %al,ApuY
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp9D:
	movl APUPC,%ecx
	movb ApuS,%al
	incl %ecx
	movb %al,ApuX
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpBD:
	movl APUPC,%edx
	movb ApuX,%al
	incl %edx
	movb %al,ApuS
	movl %edx,APUPC
	ret
ApuOp6B:
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	xorl %edx,%edx
	movb %al,%dl
	movb APU_Carry,%al
	andl $255,%eax
	salw $8,%ax
	orl %eax,%edx
	movw %dx,Work16
	movb %dl,%cl
	shrw $1,%dx
	andb $1,%cl
	movw %dx,Work16
	movb %dl,%al
	movb %cl,APU_Carry
	movl APUPC,%edx
	movb %al,Work8
	movb %al,APU_Zero
	movb 1(%edx),%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $12,%esp
	movl %ecx,APUPC
	ret
ApuOp6C:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,Work8
	xorl %edx,%edx
	movb %al,%dl
	movb APU_Carry,%al
	andl $255,%eax
	salw $8,%ax
	orl %eax,%edx
	movw %dx,Work16
	movb %dl,%cl
	shrw $1,%dx
	movb %dl,%al
	movb %al,Work8
	movb %al,APU_Zero
	movl APUAddress,%eax
	pushl %eax
	movb Work8,%al
	andb $1,%cl
	andl $255,%eax
	movb %cl,APU_Carry
	movw %dx,Work16
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%edx
	addl $3,%edx
	addl $12,%esp
	movl %edx,APUPC
	ret
ApuOp7B:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	xorl %edx,%edx
	movb %al,%dl
	movb APU_Carry,%al
	andl $255,%eax
	salw $8,%ax
	orl %eax,%edx
	movw %dx,Work16
	movb %dl,%cl
	shrw $1,%dx
	andb $1,%cl
	movb %dl,%al
	movb %cl,APU_Carry
	movw %dx,Work16
	movb %al,Work8
	movb %al,APU_Zero
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $12,%esp
	movl %eax,APUPC
	ret
ApuOp7C:
	movb APU_Carry,%dl
	movb ApuA,%al
	andl $255,%edx
	andl $255,%eax
	salw $8,%dx
	orl %edx,%eax
	movl APUPC,%edx
	movb %al,%cl
	movw %ax,Work16
	incl %edx
	andb $1,%cl
	shrw $1,%ax
	movl %edx,APUPC
	movb %cl,APU_Carry
	movw %ax,Work16
	movb %al,ApuA
	movb %al,APU_Zero
	ret
ApuOp6E:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb %cl,Work8
	movb 2(%eax),%dl
	addl $3,%eax
	movb %dl,Int8
	subw APURAM,%ax
	movsbw %dl,%dx
	addl %edx,%eax
	movw %ax,Int16
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	decb %al
	movb %al,W1
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	movb W1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	addl $12,%esp
	movb W1,%cl
	testb %cl,%cl
	je .L1096
	xorl %eax,%eax
	movw Int16,%ax
	movl APURAM,%edx
	addl %edx,%eax
	movl %eax,APUPC
	ret
.L1096:
	movl APUPC,%ecx
	addl $3,%ecx
	movl %ecx,APUPC
	ret
ApuOpFE:
	movl APUPC,%ecx
	movl %ecx,%edx
	movb 1(%ecx),%al
	addl $2,%edx
	movb %al,Int8
	subw APURAM,%dx
	cbtw
	addl %eax,%edx
	movb ApuY,%al
	decb %al
	movw %dx,Int16
	movb %al,ApuY
	je .L1104
	xorl %eax,%eax
	movw %dx,%ax
	movl APURAM,%ecx
	addl %ecx,%eax
	movl %eax,APUPC
	ret
.L1104:
	addl $2,%ecx
	movl %ecx,APUPC
	ret
ApuOp6F:
	movb ApuS,%al
	addb $2,%al
	movb %al,ApuS
	movl APURAM,%edx
	andl $255,%eax
	movw 255(%edx,%eax),%ax
	movw %ax,ApuPC
	andl $65535,%eax
	addl %eax,%edx
	movl %edx,APUPC
	ret
ApuOp7F:
	pushl %esi
	pushl %ebx
	movb ApuS,%al
	incb %al
	movb %al,ApuS
	movl APURAM,%ebx
	andl $255,%eax
	movb 256(%ebx,%eax),%dl
	movb %dl,%al
	shrb $1,%al
	movb %dl,%cl
	notl %eax
	andb $128,%cl
	andl $1,%eax
	movl %ecx,%esi
	orb %al,%cl
	movb ApuS,%al
	movb %cl,APU_Zero
	movb %dl,ApuP
	addb $2,%al
	movb %dl,%cl
	andb $64,%dl
	movb %al,ApuS
	andb $1,%cl
	shrb $6,%dl
	andl $255,%eax
	movb %cl,APU_Carry
	movb %dl,APU_Overflow
	movw 255(%ebx,%eax),%ax
	movw %ax,ApuPC
	andl $65535,%eax
	addl %eax,%ebx
	movl %ebx,APUPC
	popl %ebx
	popl %esi
	ret
ApuOp84:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%cl
	movb ApuA,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb ApuA,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1124
	movl $1,%eax
.L1124:
	movb %al,APU_Overflow
	movl APUPC,%edx
	movb Work16,%al
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp85:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%cl
	movb ApuA,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb ApuA,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1132
	movl $1,%eax
.L1132:
	movb %al,APU_Overflow
	movl APUPC,%edx
	movb Work16,%al
	addl $3,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp86:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%cl
	movb ApuA,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb ApuA,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1140
	movl $1,%eax
.L1140:
	movb %al,APU_Overflow
	movl APUPC,%edx
	movb Work16,%al
	incl %edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp87:
	movb ApuX,%al
	movl APUPC,%edx
	andl $255,%eax
	movb 1(%edx),%cl
	addb %cl,%al
	xorl %edx,%edx
	movb %al,%dl
	movl APUDirectPage,%eax
	movw (%edx,%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%cl
	movb ApuA,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb ApuA,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1144
	movl $1,%eax
.L1144:
	movb %al,APU_Overflow
	movl APUPC,%edx
	movb Work16,%al
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp88:
	movl APUPC,%eax
	movb 1(%eax),%cl
	movb ApuA,%al
	xorl %edx,%edx
	andl $255,%eax
	movb %cl,%dl
	addl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	movw %ax,Work16
	movb ApuP,%dl
	cmpw $255,%ax
	seta %al
	andb $247,%dl
	movb %al,APU_Carry
	movb %dl,ApuP
	movb ApuA,%al
	movb %cl,Work8
	movb Work16,%dl
	xorb %cl,%al
	xorb %dl,%cl
	notb %al
	andb %cl,%al
	andb $-128,%al
	je .L1152
	movl $1,%ecx
	movb %cl,APU_Overflow
	jmp .L1153
.L1152:
	movb %al,APU_Overflow
.L1153:
	movl APUPC,%edx
	movb Work16,%al
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp89:
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movl APUPC,%eax
	movb 2(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%cl
	movb Work8,%al
	xorl %edx,%edx
	movb %cl,%dl
	andl $255,%eax
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movw %dx,Work16
	movb %al,APU_Carry
	movb %cl,W1
	movb Work8,%dl
	movb %cl,%al
	movb Work16,%cl
	xorb %dl,%al
	xorb %cl,%dl
	notb %al
	andb %dl,%al
	addl $8,%esp
	andb $-128,%al
	je .L1156
	movl $1,%eax
.L1156:
	movb %al,APU_Overflow
	movb Work16,%al
	movl APUPC,%edx
	movb %al,W1
	movb %al,APU_Zero
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	movb W1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $3,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOp94:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%cl
	movb ApuA,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb ApuA,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1164
	movl $1,%eax
.L1164:
	movb %al,APU_Overflow
	movl APUPC,%edx
	movb Work16,%al
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp95:
	movl APUPC,%eax
	movb ApuX,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%cl
	movb ApuA,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb ApuA,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1168
	movl $1,%eax
.L1168:
	movb %al,APU_Overflow
	movl APUPC,%edx
	movb Work16,%al
	addl $3,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp96:
	movl APUPC,%eax
	movb ApuY,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%cl
	movb ApuA,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb ApuA,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1172
	movl $1,%eax
.L1172:
	movb %al,APU_Overflow
	movl APUPC,%edx
	movb Work16,%al
	addl $3,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp97:
	movl APUPC,%eax
	movb 1(%eax),%dl
	movl APUDirectPage,%eax
	andl $255,%edx
	movw (%edx,%eax),%ax
	movb ApuY,%dl
	andl $65535,%eax
	andl $255,%edx
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb %al,%cl
	movb ApuA,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb ApuA,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1176
	movl $1,%eax
.L1176:
	movb %al,APU_Overflow
	movl APUPC,%edx
	movb Work16,%al
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp98:
	movl APUPC,%edx
	movb 1(%edx),%al
	movb %al,Work8
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%cl
	movb Work8,%al
	xorl %edx,%edx
	movb %cl,%dl
	andl $255,%eax
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movw %dx,Work16
	movb %al,APU_Carry
	movb %cl,W1
	movb Work8,%dl
	movb %cl,%al
	movb Work16,%cl
	xorb %dl,%al
	xorb %cl,%dl
	notb %al
	andb %dl,%al
	addl $4,%esp
	andb $-128,%al
	je .L1180
	movl $1,%eax
.L1180:
	movb %al,APU_Overflow
	movb Work16,%al
	movl APUPC,%edx
	movb %al,W1
	movb %al,APU_Zero
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	movb W1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $3,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOp99:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,W1
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%cl
	movb W1,%al
	xorl %edx,%edx
	movb %al,%dl
	xorl %eax,%eax
	movb %cl,%al
	addl %eax,%edx
	movb APU_Carry,%al
	andl $255,%eax
	addl %eax,%edx
	movb ApuP,%al
	andb $247,%al
	movb %al,ApuP
	cmpw $255,%dx
	seta %al
	movb %al,APU_Carry
	movw %dx,Work16
	movb W1,%al
	movb %cl,Work8
	movb %cl,%dl
	xorb %cl,%al
	movb Work16,%cl
	notb %al
	xorb %cl,%dl
	andb %dl,%al
	addl $8,%esp
	andb $-128,%al
	je .L1184
	movl $1,%eax
.L1184:
	movb %al,APU_Overflow
	movb Work16,%al
	movb %al,W1
	movb %al,APU_Zero
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	movb W1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%edx
	incl %edx
	addl $8,%esp
	movl %edx,APUPC
	ret
ApuOp8D:
	movl APUPC,%edx
	movb 1(%edx),%al
	addl $2,%edx
	movb %al,ApuY
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp8F:
	movl APUPC,%edx
	movb 1(%edx),%al
	movb %al,Work8
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	movb Work8,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $3,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOp9E:
	movb ApuX,%al
	testb %al,%al
	jne .L1200
	movl $1,%edx
	movl $255,%ecx
	movb %dl,APU_Overflow
	movb %cl,ApuY
	jmp .L1203
.L1200:
	xorl %eax,%eax
	movb ApuX,%cl
	movb %al,APU_Overflow
	andl $255,%ecx
	movw ApuA,%ax
	movl %eax,%edx
	sarl $31,%edx
	idivl %ecx
	movl %eax,%ecx
	movb %dl,ApuY
	movb %cl,Work8
.L1203:
	movb %cl,ApuA
	movl APUPC,%edx
	movb ApuA,%al
	incl %edx
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOp9F:
	movb ApuA,%al
	movl APUPC,%ecx
	rolb $4,%al
	incl %ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpA4:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1213
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1213
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1214
.L1213:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1214:
	movl APUPC,%ecx
	movb Int16,%al
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpA5:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1217
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1217
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1218
.L1217:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1218:
	movl APUPC,%ecx
	movb Int16,%al
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpA6:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1225
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1225
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1226
.L1225:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1226:
	movl APUPC,%ecx
	movb Int16,%al
	incl %ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpA7:
	movb ApuX,%al
	movl APUPC,%edx
	andl $255,%eax
	movb 1(%edx),%cl
	addb %cl,%al
	xorl %edx,%edx
	movb %al,%dl
	movl APUDirectPage,%eax
	movw (%edx,%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1229
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1229
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1230
.L1229:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1230:
	movl APUPC,%ecx
	movb Int16,%al
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpA8:
	movl APUPC,%eax
	movb ApuA,%dl
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	jns .L1237
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1237
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1238
.L1237:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1238:
	movl APUPC,%ecx
	movb Int16,%al
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpA9:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,Work8
	movl APUPC,%eax
	movb 2(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%cl
	movb Work8,%dl
	xorl %eax,%eax
	movb %cl,%al
	andl $255,%edx
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movw %ax,Int16
	movb ApuP,%dl
	notw %ax
	andb $247,%dl
	shrw $15,%ax
	movb %dl,ApuP
	movb %al,APU_Carry
	movb Work8,%dl
	movb %cl,%al
	movb %cl,W1
	xorb %dl,%al
	addl $8,%esp
	testb %al,%al
	jge .L1241
	movb %cl,%al
	movb Int16,%dl
	xorb %dl,%al
	jns .L1241
	movl $1,%ecx
	movb %cl,APU_Overflow
	jmp .L1242
.L1241:
	xorl %eax,%eax
	movb %al,APU_Overflow
.L1242:
	movb Int16,%al
	movl APUPC,%edx
	movb %al,W1
	movb %al,APU_Zero
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	movb W1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $3,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOpB4:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb ApuA,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1245
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1245
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1246
.L1245:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1246:
	movl APUPC,%ecx
	movb Int16,%al
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpB5:
	movl APUPC,%eax
	movb ApuX,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1249
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1249
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1250
.L1249:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1250:
	movl APUPC,%ecx
	movb Int16,%al
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpB6:
	movl APUPC,%eax
	movb ApuY,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1253
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1253
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1254
.L1253:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1254:
	movl APUPC,%ecx
	movb Int16,%al
	addl $3,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpB7:
	movl APUPC,%eax
	movb 1(%eax),%dl
	movl APUDirectPage,%eax
	andl $255,%edx
	movw (%edx,%eax),%ax
	movb ApuY,%dl
	andl $65535,%eax
	andl $255,%edx
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movb ApuA,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb ApuA,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1257
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1257
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1258
.L1257:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1258:
	movl APUPC,%ecx
	movb Int16,%al
	addl $2,%ecx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpB8:
	movl APUPC,%edx
	movb 1(%edx),%al
	movb %al,Work8
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,%cl
	movb Work8,%dl
	xorl %eax,%eax
	movb %cl,%al
	andl $255,%edx
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movw %ax,Int16
	movb ApuP,%dl
	notw %ax
	andb $247,%dl
	shrw $15,%ax
	movb %dl,ApuP
	movb %al,APU_Carry
	movb Work8,%dl
	movb %cl,%al
	movb %cl,W1
	xorb %dl,%al
	addl $4,%esp
	testb %al,%al
	jge .L1261
	movb %cl,%al
	movb Int16,%dl
	xorb %dl,%al
	jns .L1261
	movl $1,%ecx
	movb %cl,APU_Overflow
	jmp .L1262
.L1261:
	xorl %eax,%eax
	movb %al,APU_Overflow
.L1262:
	movb Int16,%al
	movl APUPC,%edx
	movb %al,W1
	movb %al,APU_Zero
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	movb W1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $3,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOpB9:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb %al,W1
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movb W1,%dl
	movb %al,%cl
	xorl %eax,%eax
	movb %dl,%al
	xorl %edx,%edx
	movb %cl,%dl
	subl %edx,%eax
	movb APU_Carry,%dl
	andl $255,%edx
	addl %edx,%eax
	decl %eax
	movb ApuP,%dl
	movw %ax,Int16
	andb $247,%dl
	notw %ax
	movb %dl,ApuP
	shrw $15,%ax
	movb W1,%dl
	movb %al,APU_Carry
	movb %dl,%al
	movb %cl,Work8
	xorb %cl,%al
	addl $8,%esp
	testb %al,%al
	jge .L1265
	movb %dl,%al
	movb Int16,%cl
	xorb %cl,%al
	jns .L1265
	movl $1,%eax
	movb %al,APU_Overflow
	jmp .L1266
.L1265:
	xorl %edx,%edx
	movb %dl,APU_Overflow
.L1266:
	movb Int16,%al
	movb %al,W1
	movb %al,APU_Zero
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	movb W1,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	incl %ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOpAF:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	movb ApuX,%dl
	movb ApuA,%al
	incb %dl
	andl $255,%eax
	movb %dl,ApuX
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	incl %ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOpBE:
	movl APUPC,%eax
	incl %eax
	movl %eax,APUPC
	ret
ApuOpBF:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	movb ApuX,%dl
	incb %dl
	movb %dl,ApuX
	call S9xAPUGetByteZ__FUc
	movl APUPC,%ecx
	movb %al,ApuA
	incl %ecx
	movb %al,APU_Zero
	addl $4,%esp
	movl %ecx,APUPC
	ret
ApuOpC0:
	movb ApuP,%al
	movl APUPC,%edx
	andb $251,%al
	incl %edx
	movb %al,ApuP
	movl %edx,APUPC
	ret
ApuOpA0:
	movb ApuP,%cl
	movl APUPC,%eax
	orb $4,%cl
	incl %eax
	movb %cl,ApuP
	movl %eax,APUPC
	ret
ApuOpC4:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOpC5:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%eax
	addl $3,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOpC6:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%edx
	incl %edx
	addl $8,%esp
	movl %edx,APUPC
	ret
ApuOpC7:
	movb ApuX,%al
	movl APUPC,%edx
	andl $255,%eax
	movb 1(%edx),%cl
	addb %cl,%al
	xorl %edx,%edx
	movb %al,%dl
	movl APUDirectPage,%eax
	movw (%edx,%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOpC9:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%edx
	addl $3,%edx
	addl $8,%esp
	movl %edx,APUPC
	ret
ApuOpCB:
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOpCC:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%edx
	addl $3,%edx
	addl $8,%esp
	movl %edx,APUPC
	ret
ApuOpCD:
	movl APUPC,%edx
	movb 1(%edx),%al
	addl $2,%edx
	movb %al,ApuX
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOpCF:
	movb ApuA,%al
	mulb ApuY
	movl %eax,%edx
	shrw $8,%ax
	movw %dx,ApuA
	andl $65535,%eax
	testw %dx,%dx
	je .L1324
	orb $1,%al
.L1324:
	movl APUPC,%ecx
	incl %ecx
	movb %al,APU_Zero
	movl %ecx,APUPC
	ret
ApuOpD4:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOpD5:
	movl APUPC,%eax
	movb ApuX,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%edx
	addl $3,%edx
	addl $8,%esp
	movl %edx,APUPC
	ret
ApuOpD6:
	movl APUPC,%eax
	movb ApuY,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%ecx
	addl $3,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOpD7:
	movl APUPC,%eax
	movb 1(%eax),%dl
	movl APUDirectPage,%eax
	andl $255,%edx
	movw (%edx,%eax),%ax
	movb ApuY,%dl
	andl $65535,%eax
	andl $255,%edx
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	movb ApuA,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByte__FUcUl
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOpD8:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $2,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOpD9:
	movl APUPC,%edx
	movb ApuY,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOpDB:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	movb ApuY,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%eax
	addl $2,%eax
	addl $8,%esp
	movl %eax,APUPC
	ret
ApuOpDF:
	movb ApuA,%al
	movb %al,%dl
	xorl %ecx,%ecx
	shrb $4,%al
	andb $15,%dl
	movb %cl,APU_Carry
	movb %al,W2
	movb %dl,W1
	cmpb $9,%dl
	jbe .L1359
	addb $-10,%dl
	incb %al
	movb %dl,W1
	movb %al,W2
.L1359:
	movb W2,%al
	cmpb $9,%al
	jbe .L1360
	addb $-10,%al
	movl $1,%edx
	movb %al,W2
	movb %dl,APU_Carry
.L1360:
	movb W2,%al
	movb W1,%cl
	movl APUPC,%edx
	salb $4,%al
	incl %edx
	orb %cl,%al
	movl %edx,APUPC
	movb %al,ApuA
	movb %al,APU_Zero
	ret
ApuOpE4:
	movl APUPC,%eax
	movb 1(%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movl APUPC,%edx
	movb %al,ApuA
	addl $2,%edx
	movb %al,APU_Zero
	addl $4,%esp
	movl %edx,APUPC
	ret
ApuOpE5:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUPC,%ecx
	movb %al,ApuA
	addl $3,%ecx
	movb %al,APU_Zero
	addl $4,%esp
	movl %ecx,APUPC
	ret
ApuOpE6:
	movb ApuX,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movl APUPC,%edx
	movb %al,ApuA
	incl %edx
	movb %al,APU_Zero
	addl $4,%esp
	movl %edx,APUPC
	ret
ApuOpE7:
	movb ApuX,%al
	movl APUPC,%edx
	andl $255,%eax
	movb 1(%edx),%cl
	addb %cl,%al
	xorl %edx,%edx
	movb %al,%dl
	movl APUDirectPage,%eax
	movw (%edx,%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUPC,%edx
	movb %al,ApuA
	addl $2,%edx
	movb %al,APU_Zero
	addl $4,%esp
	movl %edx,APUPC
	ret
ApuOpE8:
	movl APUPC,%edx
	movb 1(%edx),%al
	addl $2,%edx
	movb %al,ApuA
	movb %al,APU_Zero
	movl %edx,APUPC
	ret
ApuOpE9:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUPC,%ecx
	movb %al,ApuX
	addl $3,%ecx
	movb %al,APU_Zero
	addl $4,%esp
	movl %ecx,APUPC
	ret
ApuOpEB:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movl APUPC,%ecx
	movb %al,ApuY
	addl $2,%ecx
	movb %al,APU_Zero
	addl $4,%esp
	movl %ecx,APUPC
	ret
ApuOpEC:
	movl APUPC,%eax
	movw 1(%eax),%ax
	andl $65535,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUPC,%edx
	movb %al,ApuY
	addl $3,%edx
	movb %al,APU_Zero
	addl $4,%esp
	movl %edx,APUPC
	ret
ApuOpF4:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movl APUPC,%edx
	movb %al,ApuA
	addl $2,%edx
	movb %al,APU_Zero
	addl $4,%esp
	movl %edx,APUPC
	ret
ApuOpF5:
	movl APUPC,%eax
	movb ApuX,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUPC,%ecx
	movb %al,ApuA
	addl $3,%ecx
	movb %al,APU_Zero
	addl $4,%esp
	movl %ecx,APUPC
	ret
ApuOpF6:
	movl APUPC,%eax
	movb ApuY,%dl
	movw 1(%eax),%ax
	andl $255,%edx
	andl $65535,%eax
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUPC,%edx
	movb %al,ApuA
	addl $3,%edx
	movb %al,APU_Zero
	addl $4,%esp
	movl %edx,APUPC
	ret
ApuOpF7:
	movl APUPC,%eax
	movb 1(%eax),%dl
	movl APUDirectPage,%eax
	andl $255,%edx
	movw (%edx,%eax),%ax
	movb ApuY,%dl
	andl $65535,%eax
	andl $255,%edx
	addl %edx,%eax
	movl %eax,APUAddress
	pushl %eax
	call S9xAPUGetByte__FUl
	movl APUPC,%ecx
	movb %al,ApuA
	addl $2,%ecx
	movb %al,APU_Zero
	addl $4,%esp
	movl %ecx,APUPC
	ret
ApuOpF8:
	movl APUPC,%eax
	movb 1(%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movl APUPC,%ecx
	movb %al,ApuX
	addl $2,%ecx
	movb %al,APU_Zero
	addl $4,%esp
	movl %ecx,APUPC
	ret
ApuOpF9:
	movl APUPC,%edx
	movb ApuY,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movl APUPC,%edx
	movb %al,ApuX
	addl $2,%edx
	movb %al,APU_Zero
	addl $4,%esp
	movl %edx,APUPC
	ret
ApuOpFA:
	movl APUPC,%edx
	movb 2(%edx),%al
	andl $255,%eax
	pushl %eax
	movb 1(%edx),%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	addl $4,%esp
	andl $255,%eax
	pushl %eax
	call S9xAPUSetByteZ__FUcUc
	movl APUPC,%ecx
	addl $3,%ecx
	addl $8,%esp
	movl %ecx,APUPC
	ret
ApuOpFB:
	movl APUPC,%edx
	movb ApuX,%al
	movb 1(%edx),%cl
	addb %cl,%al
	andl $255,%eax
	pushl %eax
	call S9xAPUGetByteZ__FUc
	movl APUPC,%edx
	movb %al,ApuY
	addl $2,%edx
	movb %al,APU_Zero
	addl $4,%esp
	movl %edx,APUPC
	ret

#if 0
S9xAPUGetByteZ__FUc:
	movb 4(%esp),%dl
	cmpb $239,%dl
	jbe .L1434
	movl APUDirectPage,%ecx
	movl APURAM,%eax
	cmpl %eax,%ecx
	jne .L1434
	movb %dl,%al
	addb $12,%al
	cmpb $3,%al
	ja .L1435
	movl APUWaitAddress1,%eax
	movl %eax,APUWaitAddress2
	movl APUPC,%eax
	movl %eax,APUWaitAddress1
.L1441:
	xorl %eax,%eax
	movb %dl,%al
	movb (%eax,%ecx),%dl
	xorl %eax,%eax
	movb %dl,%al
	ret
.L1435:
	cmpb $243,%dl
	jne .L1436
	call S9xGetAPUDSP
	andl $255,%eax
	ret
.L1436:
	cmpb $252,%dl
	jbe .L1441
	movl APUWaitAddress1,%eax
	movl %eax,APUWaitAddress2
	movl APUPC,%eax
	andl $255,%edx
	movl %eax,APUWaitAddress1
	movb (%edx,%ecx),%al
	movb %al,W4
	xorl %eax,%eax
	movb %al,(%edx,%ecx)
	movb W4,%al
	andl $255,%eax
	ret
.L1434:
	andl $255,%edx
	movl APUDirectPage,%eax
	movb (%edx,%eax),%cl
	xorl %eax,%eax
	movb %cl,%al
	ret
S9xAPUSetByteZ__FUcUc:
	pushl %ebx
	movl 8(%esp),%ebx
	movb 12(%esp),%dl
	movb %bl,%cl
	cmpb $239,%dl
	jbe .L1454
	movl APURAM,%eax
	cmpl %eax,APUDirectPage
	jne .L1454
	movb %dl,%al
	addb $12,%al
	cmpb $3,%al
	ja .L1455
	xorl %eax,%eax
	movb %dl,%al
	movb %bl,APU-237(%eax)
	jmp .L1463
.L1455:
	cmpb $241,%dl
	jne .L1457
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xSetAPUControl
	addl $4,%esp
	jmp .L1463
.L1457:
	cmpb $243,%dl
	jne .L1459
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	call S9xSetAPUDSP
	addl $4,%esp
	jmp .L1463
.L1459:
	cmpb $252,%dl
	ja .L1463
	xorl %eax,%eax
	movb %cl,%al
	pushl %eax
	xorl %eax,%eax
	movb %dl,%al
	pushl %eax
	call S9xSetAPUTimer
	addl $8,%esp
	jmp .L1463
.L1454:
	andl $255,%edx
	movl APUDirectPage,%eax
	movb %cl,(%edx,%eax)
.L1463:
	popl %ebx
	ret

S9xAPUGetByte__FUl:
	xorl %ecx,%ecx
	movw 4(%esp),%cx
	leal -240(%ecx),%eax
	cmpl $15,%eax
	ja .L1465
	leal -244(%ecx),%eax
	cmpl $3,%eax
	ja .L1466
	movl APUWaitAddress1,%eax
	movl APUPC,%edx
	movl %eax,APUWaitAddress2
	movl %edx,APUWaitAddress1
.L1477:
	movl APURAM,%eax
	movb (%ecx,%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	ret
.L1466:
	cmpl $243,%ecx
	jne .L1467
	call S9xGetAPUDSP
	andl $255,%eax
	ret
.L1467:
	cmpl $252,%ecx
	jbe .L1477
	movl APUPC,%edx
	movl APUWaitAddress1,%eax
	movl %edx,APUWaitAddress1
	movl %eax,APUWaitAddress2
	movl APURAM,%edx
	movb (%ecx,%edx),%al
	movb %al,W4
	xorl %eax,%eax
	movb %al,(%ecx,%edx)
	movb W4,%al
	andl $255,%eax
	ret
.L1465:
	cmpl $65471,%ecx
	jbe .L1471
	movb APUShowROM,%al
	testb %al,%al
	je .L1472
	movb APUROM-65472(%ecx),%al
	andl $255,%eax
	ret
.L1472:
	movb APUExtraRAM-0xffc0(%ecx),%al
	andl $255,%eax
	ret
.L1471:
	movl APURAM,%eax
	movb (%ecx,%eax),%dl
	xorl %eax,%eax
	movb %dl,%al
	ret

S9xAPUSetByte__FUcUl:
	pushl %ebx
	xorl %edx,%edx
	movl 8(%esp),%ecx
	movw 12(%esp),%dx
	movb %cl,%bl
	leal -240(%edx),%eax
	cmpl $15,%eax
	ja .L1492
	leal -244(%edx),%eax
	cmpl $3,%eax
	ja .L1493
	movb %cl,APUOutPorts-0xf4(%edx)
	jmp .L1503
.L1493:
	cmpl $241,%edx
	jne .L1495
	xorl %eax,%eax
	movb %bl,%al
	pushl %eax
	call S9xSetAPUControl
	addl $4,%esp
	jmp .L1503
.L1495:
	cmpl $243,%edx
	jne .L1497
	xorl %eax,%eax
	movb %bl,%al
	pushl %eax
	call S9xSetAPUDSP
	addl $4,%esp
	jmp .L1503
.L1497:
	cmpl $252,%edx
	ja .L1503
	xorl %eax,%eax
	movb %bl,%al
	pushl %eax
	pushl %edx
	call S9xSetAPUTimer
	addl $8,%esp
	jmp .L1503
.L1492:
	cmpl $65471,%edx
	ja .L1501
	movl APURAM,%eax
	movb %cl,(%edx,%eax)
	jmp .L1503
.L1501:
	movb %cl,APUExtraRAM-0xffc0(%edx)
.L1503:
	popl %ebx
	ret
#else

S9xAPUGetByteZ__FUc:
	movb 4(%esp),%dl
	movl APUDirectPage,%ecx
	cmpb $239,%dl
	jbe .L1435
	movl APURAM,%eax
	cmpl %eax,%ecx
	jne .L1435
	movb %dl,%al
	addb $12,%al
	cmpb $3,%al
	ja .L1436
	movl APUWaitAddress1,%eax
	movl %eax,APUWaitAddress2
	movl APUPC,APUWaitAddress1
.L1444:
	xorl %eax,%eax
	movb %dl,%al
	movb (%eax,%ecx),%dl
.L1443:
	xorl %eax,%eax
	movb %dl,%al
	ret
.L1436:
	cmpb $252,%dl
	jbe .L1437
	movl APUWaitAddress1,%eax
	movl APUPC,APUWaitAddress1
	movl %eax,APUWaitAddress2
	xorl %eax,%eax
	movb %dl,%al
	movb (%eax,%ecx),%dl
	movb $0,(%eax,%ecx)
	jmp .L1443
.L1437:
	cmpb $243,%dl
	jne .L1444
	call S9xGetAPUDSP
	ret
.L1435:
	andl $255,%edx
	xorl %eax,%eax
	movb (%edx,%ecx),%al
	ret

S9xAPUSetByteZ__FUcUc:
	pushl %eax
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 20(%esp),%ebx
	movb 24(%esp),%dl
	movb %bl,15(%esp)
	cmpb $239,%dl
	jbe .L1460
	movl APUDirectPage,%esi
	movl APURAM,%eax
	cmpl %eax,%esi
	jne .L1460
	cmpb $243,%dl
	jne .L1461
	xorl %eax,%eax
	movb %bl,%al
	pushl %eax
	call S9xSetAPUDSP
	addl $4,%esp
	jmp .L1472
.L1461:
	movb %dl,%al
	addb $12,%al
	cmpb $3,%al
	ja .L1463
	xorl %eax,%eax
	movb %dl,%al
	movb %bl,APUOutPorts-0xf4(%eax)
	jmp .L1472
.L1463:
	cmpb $241,%dl
	jne .L1465
	movb 15(%esp),%al
	andl $255,%eax
	pushl %eax
	call S9xSetAPUControl
	addl $4,%esp
	jmp .L1472
.L1465:
	cmpb $252,%dl
	ja .L1472
	movl %edx,%edi
	andl $255,%edi
	movb %bl,(%edi,%esi)
	cmpb $249,%dl
	jbe .L1472
	testb %bl,%bl
	jne .L1469
	movw $256,APUTimerTarget-0xfa * 2(,%edi,2)
	jmp .L1472
.L1469:
	movb 15(%esp),%al
	andl $255,%eax
	movw %ax,APUTimerTarget-0xfa * 2(,%edi,2)
	jmp .L1472
.L1460:
	andl $255,%edx
	movl APUDirectPage,%eax
	movb 15(%esp),%cl
	movb %cl,(%edx,%eax)
.L1472:
	popl %ebx
	popl %esi
	popl %edi
	addl $4,%esp
	ret

S9xAPUGetByte__FUl:
	movl 4(%esp),%ecx
	andl $0xffff,%eax
	leal -240(%ecx),%eax
	cmpl $15,%eax
	ja .L1474
	leal -244(%ecx),%eax
	cmpl $3,%eax
	ja .L1475
	movl APUWaitAddress1,%eax
	movl APUPC,APUWaitAddress2
	movl %edx,APUWaitAddress1
.L1487:
	xorl %eax,%eax
	movl APURAM,%edx
	movb (%ecx,%edx),%al
	ret
.L1475:
	cmpl $243,%ecx
	jne .L1476
	call S9xGetAPUDSP
	ret
.L1476:
	cmpl $252,%ecx
	jbe .L1487
	movl APUWaitAddress1,%eax
	movl APUPC,APUWaitAddress1
	movl %eax,APUWaitAddress2
	xorl %eax,%eax
	movl APURAM,%edx
	movb (%ecx,%edx),%al
	movb $0,(%ecx,%edx)
	ret
.L1474:
	cmpl $65471,%ecx
	jbe .L1480
	movb APUShowROM,%al
	testb %al,%al
	je .L1481
	movb APUROM-65472(%ecx),%al
	andl $255,%eax
	ret
.L1481:
	xorl %eax,%eax
	movb APUExtraRAM-0xffc0(%ecx),%al
	ret
.L1480:
	xorl %eax,%eax
	movl APURAM,%edx
	movb (%ecx,%edx),%al
	ret

S9xAPUSetByte__FUcUl:
	pushl %ebx
	xorl %edx,%edx
	movl 8(%esp),%ecx
	movw 12(%esp),%dx
	movb %cl,%bl
	leal -240(%edx),%eax
	cmpl $15,%eax
	ja .L1505
	cmpl $243,%edx
	jne .L1506
	xorl %eax,%eax
	movb %bl,%al
	pushl %eax
	call S9xSetAPUDSP
	addl $4,%esp
	jmp .L1519
.L1506:
	leal -244(%edx),%eax
	cmpl $3,%eax
	ja .L1508
	movb %cl,APUOutPorts-0xf4(%edx)
	jmp .L1519
.L1508:
	cmpl $241,%edx
	jne .L1510
	xorl %eax,%eax
	movb %bl,%al
	pushl %eax
	call S9xSetAPUControl
	addl $4,%esp
	jmp .L1519
.L1510:
	cmpl $252,%edx
	ja .L1519
	movl APURAM,%eax
	movb %cl,(%edx,%eax)
	cmpl $249,%edx
	jbe .L1519
	testb %cl,%cl
	jne .L1514
	movw $256,APUTimerTarget-0xfa*2(,%edx,2)
	jmp .L1519
.L1514:
	xorl %eax,%eax
	movb %bl,%al
	movw %ax,APUTimerTarget-0xfa*2(,%edx,2)
	jmp .L1519
.L1505:
	cmpl $0xffbf,%edx
	ja .L1517
	movl APURAM,%eax
	movb %cl,(%edx,%eax)
	jmp .L1519
.L1517:
	movb %cl,APUExtraRAM - 0xffc0(%edx)
.L1519:
	popl %ebx
	ret
#endif

.data
	.align 4
.globl S9xApuOpcodes	
S9xApuOpcodes:
	.long ApuOp00
	.long ApuOp01
	.long ApuOp02
	.long ApuOp03
	.long ApuOp04
	.long ApuOp05
	.long ApuOp06
	.long ApuOp07
	.long ApuOp08
	.long ApuOp09
	.long ApuOp0A
	.long ApuOp0B
	.long ApuOp0C
	.long ApuOp0D
	.long ApuOp0E
	.long ApuOp0F
	.long ApuOp10
	.long ApuOp11
	.long ApuOp12
	.long ApuOp13
	.long ApuOp14
	.long ApuOp15
	.long ApuOp16
	.long ApuOp17
	.long ApuOp18
	.long ApuOp19
	.long ApuOp1A
	.long ApuOp1B
	.long ApuOp1C
	.long ApuOp1D
	.long ApuOp1E
	.long ApuOp1F
	.long ApuOp20
	.long ApuOp21
	.long ApuOp22
	.long ApuOp23
	.long ApuOp24
	.long ApuOp25
	.long ApuOp26
	.long ApuOp27
	.long ApuOp28
	.long ApuOp29
	.long ApuOp2A
	.long ApuOp2B
	.long ApuOp2C
	.long ApuOp2D
	.long ApuOp2E
	.long ApuOp2F
	.long ApuOp30
	.long ApuOp31
	.long ApuOp32
	.long ApuOp33
	.long ApuOp34
	.long ApuOp35
	.long ApuOp36
	.long ApuOp37
	.long ApuOp38
	.long ApuOp39
	.long ApuOp3A
	.long ApuOp3B
	.long ApuOp3C
	.long ApuOp3D
	.long ApuOp3E
	.long ApuOp3F
	.long ApuOp40
	.long ApuOp41
	.long ApuOp42
	.long ApuOp43
	.long ApuOp44
	.long ApuOp45
	.long ApuOp46
	.long ApuOp47
	.long ApuOp48
	.long ApuOp49
	.long ApuOp4A
	.long ApuOp4B
	.long ApuOp4C
	.long ApuOp4D
	.long ApuOp4E
	.long ApuOp4F
	.long ApuOp50
	.long ApuOp51
	.long ApuOp52
	.long ApuOp53
	.long ApuOp54
	.long ApuOp55
	.long ApuOp56
	.long ApuOp57
	.long ApuOp58
	.long ApuOp59
	.long ApuOp5A
	.long ApuOp5B
	.long ApuOp5C
	.long ApuOp5D
	.long ApuOp5E
	.long ApuOp5F
	.long ApuOp60
	.long ApuOp61
	.long ApuOp62
	.long ApuOp63
	.long ApuOp64
	.long ApuOp65
	.long ApuOp66
	.long ApuOp67
	.long ApuOp68
	.long ApuOp69
	.long ApuOp6A
	.long ApuOp6B
	.long ApuOp6C
	.long ApuOp6D
	.long ApuOp6E
	.long ApuOp6F
	.long ApuOp70
	.long ApuOp71
	.long ApuOp72
	.long ApuOp73
	.long ApuOp74
	.long ApuOp75
	.long ApuOp76
	.long ApuOp77
	.long ApuOp78
	.long ApuOp79
	.long ApuOp7A
	.long ApuOp7B
	.long ApuOp7C
	.long ApuOp7D
	.long ApuOp7E
	.long ApuOp7F
	.long ApuOp80
	.long ApuOp81
	.long ApuOp82
	.long ApuOp83
	.long ApuOp84
	.long ApuOp85
	.long ApuOp86
	.long ApuOp87
	.long ApuOp88
	.long ApuOp89
	.long ApuOp8A
	.long ApuOp8B
	.long ApuOp8C
	.long ApuOp8D
	.long ApuOp8E
	.long ApuOp8F
	.long ApuOp90
	.long ApuOp91
	.long ApuOp92
	.long ApuOp93
	.long ApuOp94
	.long ApuOp95
	.long ApuOp96
	.long ApuOp97
	.long ApuOp98
	.long ApuOp99
	.long ApuOp9A
	.long ApuOp9B
	.long ApuOp9C
	.long ApuOp9D
	.long ApuOp9E
	.long ApuOp9F
	.long ApuOpA0
	.long ApuOpA1
	.long ApuOpA2
	.long ApuOpA3
	.long ApuOpA4
	.long ApuOpA5
	.long ApuOpA6
	.long ApuOpA7
	.long ApuOpA8
	.long ApuOpA9
	.long ApuOpAA
	.long ApuOpAB
	.long ApuOpAC
	.long ApuOpAD
	.long ApuOpAE
	.long ApuOpAF
	.long ApuOpB0
	.long ApuOpB1
	.long ApuOpB2
	.long ApuOpB3
	.long ApuOpB4
	.long ApuOpB5
	.long ApuOpB6
	.long ApuOpB7
	.long ApuOpB8
	.long ApuOpB9
	.long ApuOpBA
	.long ApuOpBB
	.long ApuOpBC
	.long ApuOpBD
	.long ApuOpBE
	.long ApuOpBF
	.long ApuOpC0
	.long ApuOpC1
	.long ApuOpC2
	.long ApuOpC3
	.long ApuOpC4
	.long ApuOpC5
	.long ApuOpC6
	.long ApuOpC7
	.long ApuOpC8
	.long ApuOpC9
	.long ApuOpCA
	.long ApuOpCB
	.long ApuOpCC
	.long ApuOpCD
	.long ApuOpCE
	.long ApuOpCF
	.long ApuOpD0
	.long ApuOpD1
	.long ApuOpD2
	.long ApuOpD3
	.long ApuOpD4
	.long ApuOpD5
	.long ApuOpD6
	.long ApuOpD7
	.long ApuOpD8
	.long ApuOpD9
	.long ApuOpDA
	.long ApuOpDB
	.long ApuOpDC
	.long ApuOpDD
	.long ApuOpDE
	.long ApuOpDF
	.long ApuOpE0
	.long ApuOpE1
	.long ApuOpE2
	.long ApuOpE3
	.long ApuOpE4
	.long ApuOpE5
	.long ApuOpE6
	.long ApuOpE7
	.long ApuOpE8
	.long ApuOpE9
	.long ApuOpEA
	.long ApuOpEB
	.long ApuOpEC
	.long ApuOpED
	.long ApuOpEE
	.long ApuOpEF
	.long ApuOpF0
	.long ApuOpF1
	.long ApuOpF2
	.long ApuOpF3
	.long ApuOpF4
	.long ApuOpF5
	.long ApuOpF6
	.long ApuOpF7
	.long ApuOpF8
	.long ApuOpF9
	.long ApuOpFA
	.long ApuOpFB
	.long ApuOpFC
	.long ApuOpFD
	.long ApuOpFE
	.long ApuOpFF
