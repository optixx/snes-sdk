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


/*******************************************************************************
  SNES9X for Mac OS (c) Copyright John Stiles
  
  Snes9x for Mac OS X

  (c) Copyright 2001 - 2006 zones
  (c) Copyright 2002 - 2006 107
  (c) Copyright 2002        PB1400c
  (c) Copyright 2004        Alexander and Sander
  (c) Copyright 2004 - 2005 Steven Seeger
  (c) Copyright 2005        Ryan Vogt
*******************************************************************************/

#include "port.h"
#include "mac-hq2x.h"

#define	Ymask	0xFF0000
#define	Umask	0x00FF00
#define	Vmask	0x0000FF
#define	trY		0x300000
#define	trU		0x000700
#define	trV		0x000006

#define	Mask_2	0x03E0	// 00000 11111 00000
#define	Mask13	0x7C1F	// 11111 00000 11111

#define Interp01(c1, c2) \
(c1 == c2) ? c1 : \
(((((c1 & Mask_2) *  3 + (c2 & Mask_2)) >> 2) & Mask_2) + \
 ((((c1 & Mask13) *  3 + (c2 & Mask13)) >> 2) & Mask13))
  
#define Interp05(c1, c2) \
(c1 == c2) ? c1 : \
(((((c1 & Mask_2)      + (c2 & Mask_2)) >> 1) & Mask_2) + \
 ((((c1 & Mask13)      + (c2 & Mask13)) >> 1) & Mask13))
 
#define Interp02(c1, c2, c3) \
(((((c1 & Mask_2) *  2 + (c2 & Mask_2)     + (c3 & Mask_2)    ) >> 2) & Mask_2) + \
 ((((c1 & Mask13) *  2 + (c2 & Mask13)     + (c3 & Mask13)    ) >> 2) & Mask13))

#define Interp06(c1, c2, c3) \
(((((c1 & Mask_2) *  5 + (c2 & Mask_2) * 2 + (c3 & Mask_2)    ) >> 3) & Mask_2) + \
 ((((c1 & Mask13) *  5 + (c2 & Mask13) * 2 + (c3 & Mask13)    ) >> 3) & Mask13))

#define Interp07(c1, c2, c3) \
(((((c1 & Mask_2) *  6 + (c2 & Mask_2)     + (c3 & Mask_2)    ) >> 3) & Mask_2) + \
 ((((c1 & Mask13) *  6 + (c2 & Mask13)     + (c3 & Mask13)    ) >> 3) & Mask13))

#define Interp09(c1, c2, c3) \
(((((c1 & Mask_2) *  2 + (c2 & Mask_2) * 3 + (c3 & Mask_2) * 3) >> 3) & Mask_2) + \
 ((((c1 & Mask13) *  2 + (c2 & Mask13) * 3 + (c3 & Mask13) * 3) >> 3) & Mask13))

#define Interp10(c1, c2, c3) \
(((((c1 & Mask_2) * 14 + (c2 & Mask_2)     + (c3 & Mask_2)    ) >> 4) & Mask_2) + \
 ((((c1 & Mask13) * 14 + (c2 & Mask13)     + (c3 & Mask13)    ) >> 4) & Mask13))

#define PIXEL00_0		*(dp) = w5;
#define PIXEL00_10		*(dp) = Interp01(w5, w1);
#define PIXEL00_11		*(dp) = Interp01(w5, w4);
#define PIXEL00_12		*(dp) = Interp01(w5, w2);
#define PIXEL00_20		*(dp) = Interp02(w5, w4, w2);
#define PIXEL00_21		*(dp) = Interp02(w5, w1, w2);
#define PIXEL00_22		*(dp) = Interp02(w5, w1, w4);
#define PIXEL00_60		*(dp) = Interp06(w5, w2, w4);
#define PIXEL00_61		*(dp) = Interp06(w5, w4, w2);
#define PIXEL00_70		*(dp) = Interp07(w5, w4, w2);
#define PIXEL00_90		*(dp) = Interp09(w5, w4, w2);
#define PIXEL00_100		*(dp) = Interp10(w5, w4, w2);

#define PIXEL01_0		*(dp + 1) = w5;
#define PIXEL01_10		*(dp + 1) = Interp01(w5, w3);
#define PIXEL01_11		*(dp + 1) = Interp01(w5, w2);
#define PIXEL01_12		*(dp + 1) = Interp01(w5, w6);
#define PIXEL01_20		*(dp + 1) = Interp02(w5, w2, w6);
#define PIXEL01_21		*(dp + 1) = Interp02(w5, w3, w6);
#define PIXEL01_22		*(dp + 1) = Interp02(w5, w3, w2);
#define PIXEL01_60		*(dp + 1) = Interp06(w5, w6, w2);
#define PIXEL01_61		*(dp + 1) = Interp06(w5, w2, w6);
#define PIXEL01_70		*(dp + 1) = Interp07(w5, w2, w6);
#define PIXEL01_90		*(dp + 1) = Interp09(w5, w2, w6);
#define PIXEL01_100		*(dp + 1) = Interp10(w5, w2, w6);

#define PIXEL10_0		*(dp + dst1line) = w5;
#define PIXEL10_10		*(dp + dst1line) = Interp01(w5, w7);
#define PIXEL10_11		*(dp + dst1line) = Interp01(w5, w8);
#define PIXEL10_12		*(dp + dst1line) = Interp01(w5, w4);
#define PIXEL10_20		*(dp + dst1line) = Interp02(w5, w8, w4);
#define PIXEL10_21		*(dp + dst1line) = Interp02(w5, w7, w4);
#define PIXEL10_22		*(dp + dst1line) = Interp02(w5, w7, w8);
#define PIXEL10_60		*(dp + dst1line) = Interp06(w5, w4, w8);
#define PIXEL10_61		*(dp + dst1line) = Interp06(w5, w8, w4);
#define PIXEL10_70		*(dp + dst1line) = Interp07(w5, w8, w4);
#define PIXEL10_90		*(dp + dst1line) = Interp09(w5, w8, w4);
#define PIXEL10_100		*(dp + dst1line) = Interp10(w5, w8, w4);

#define PIXEL11_0		*(dp + dst1line + 1) = w5;
#define PIXEL11_10		*(dp + dst1line + 1) = Interp01(w5, w9);
#define PIXEL11_11		*(dp + dst1line + 1) = Interp01(w5, w6);
#define PIXEL11_12		*(dp + dst1line + 1) = Interp01(w5, w8);
#define PIXEL11_20		*(dp + dst1line + 1) = Interp02(w5, w6, w8);
#define PIXEL11_21		*(dp + dst1line + 1) = Interp02(w5, w9, w8);
#define PIXEL11_22		*(dp + dst1line + 1) = Interp02(w5, w9, w6);
#define PIXEL11_60		*(dp + dst1line + 1) = Interp06(w5, w8, w6);
#define PIXEL11_61		*(dp + dst1line + 1) = Interp06(w5, w6, w8);
#define PIXEL11_70		*(dp + dst1line + 1) = Interp07(w5, w6, w8);
#define PIXEL11_90		*(dp + dst1line + 1) = Interp09(w5, w6, w8);
#define PIXEL11_100		*(dp + dst1line + 1) = Interp10(w5, w6, w8);

#define Absolute(c) \
(!(c & (1 << 31)) ? c : (~c + 1))

static int	RGBtoYUV[32768];

static inline bool Diff(int c1, int c2)
{
	if (Absolute((c1 & Ymask) - (c2 & Ymask)) > trY) return true;
	if (Absolute((c1 & Umask) - (c2 & Umask)) > trU) return true;
	if (Absolute((c1 & Vmask) - (c2 & Vmask)) > trV) return true;
	
	return false;
}

void InitLUTs(void)
{
	int	i, j, k, r, g, b, y, u, v;

	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			for (k = 0; k < 32; k++)
  			{
    			r = i << 3;
    			g = j << 3;
    			b = k << 3;
    			y = (r + g + b) >> 2;
    			u = 128 + ((r - b) >> 2);
    			v = 128 + ((-r + 2 * g - b) >> 3);
    			
				RGBtoYUV[(i << 10) + (j << 5) + k] = (y << 16) + (u << 8) + v;
			}
		}
	}
}

void HQ2X_16(uint8 *srcPtr, uint32 srcPitch, uint8 *dstPtr, uint32 dstPitch, int width, int height)
{
	register int	w1, w2, w3, w4, w5, w6, w7, w8, w9;

	register uint32	src1line = srcPitch >> 1;
	register uint32	dst1line = dstPitch >> 1;
	register uint16	*sp = (uint16 *) srcPtr;
	register uint16	*dp = (uint16 *) dstPtr;

	uint32  pattern;
	int		l, y;
	
	while (height--)
	{
		sp--;
	
		w1 = *(sp - src1line);
		w4 = *(sp);
		w7 = *(sp + src1line);

		sp++;

		w2 = *(sp - src1line);
		w5 = *(sp);
		w8 = *(sp + src1line);
		
		for (l = width; l; l--)
		{	
			sp++;

			w3 = *(sp - src1line);
			w6 = *(sp);
			w9 = *(sp + src1line);
			
			y = RGBtoYUV[w5];
			pattern = 0;
			
			if ((w1 != w5) && (Diff(y, RGBtoYUV[w1]))) pattern |= (1 << 0);
			if ((w2 != w5) && (Diff(y, RGBtoYUV[w2]))) pattern |= (1 << 1);
			if ((w3 != w5) && (Diff(y, RGBtoYUV[w3]))) pattern |= (1 << 2);
			if ((w4 != w5) && (Diff(y, RGBtoYUV[w4]))) pattern |= (1 << 3);
			if ((w6 != w5) && (Diff(y, RGBtoYUV[w6]))) pattern |= (1 << 4);
			if ((w7 != w5) && (Diff(y, RGBtoYUV[w7]))) pattern |= (1 << 5);
			if ((w8 != w5) && (Diff(y, RGBtoYUV[w8]))) pattern |= (1 << 6);
			if ((w9 != w5) && (Diff(y, RGBtoYUV[w9]))) pattern |= (1 << 7);
	
			switch (pattern)
			{
			    case 0:
			    case 1:
			    case 4:
			    case 32:
			    case 128:
			    case 5:
			    case 132:
			    case 160:
			    case 33:
			    case 129:
			    case 36:
			    case 133:
			    case 164:
			    case 161:
			    case 37:
			    case 165:
			        {
			            PIXEL00_20
			            PIXEL01_20
			            PIXEL10_20
			            PIXEL11_20
			            break;
			        }
			    case 2:
			    case 34:
			    case 130:
			    case 162:
			        {
			            PIXEL00_22
			            PIXEL01_21
			            PIXEL10_20
			            PIXEL11_20
			            break;
			        }
			    case 16:
			    case 17:
			    case 48:
			    case 49:
			        {
			            PIXEL00_20
			            PIXEL01_22
			            PIXEL10_20
			            PIXEL11_21
			            break;
			        }
			    case 64:
			    case 65:
			    case 68:
			    case 69:
			        {
			            PIXEL00_20
			            PIXEL01_20
			            PIXEL10_21
			            PIXEL11_22
			            break;
			        }
			    case 8:
			    case 12:
			    case 136:
			    case 140:
			        {
			            PIXEL00_21
			            PIXEL01_20
			            PIXEL10_22
			            PIXEL11_20
			            break;
			        }
			    case 3:
			    case 35:
			    case 131:
			    case 163:
			        {
			            PIXEL00_11
			            PIXEL01_21
			            PIXEL10_20
			            PIXEL11_20
			            break;
			        }
			    case 6:
			    case 38:
			    case 134:
			    case 166:
			        {
			            PIXEL00_22
			            PIXEL01_12
			            PIXEL10_20
			            PIXEL11_20
			            break;
			        }
			    case 20:
			    case 21:
			    case 52:
			    case 53:
			        {
			            PIXEL00_20
			            PIXEL01_11
			            PIXEL10_20
			            PIXEL11_21
			            break;
			        }
			    case 144:
			    case 145:
			    case 176:
			    case 177:
			        {
			            PIXEL00_20
			            PIXEL01_22
			            PIXEL10_20
			            PIXEL11_12
			            break;
			        }
			    case 192:
			    case 193:
			    case 196:
			    case 197:
			        {
			            PIXEL00_20
			            PIXEL01_20
			            PIXEL10_21
			            PIXEL11_11
			            break;
			        }
			    case 96:
			    case 97:
			    case 100:
			    case 101:
			        {
			            PIXEL00_20
			            PIXEL01_20
			            PIXEL10_12
			            PIXEL11_22
			            break;
			        }
			    case 40:
			    case 44:
			    case 168:
			    case 172:
			        {
			            PIXEL00_21
			            PIXEL01_20
			            PIXEL10_11
			            PIXEL11_20
			            break;
			        }
			    case 9:
			    case 13:
			    case 137:
			    case 141:
			        {
			            PIXEL00_12
			            PIXEL01_20
			            PIXEL10_22
			            PIXEL11_20
			            break;
			        }
			    case 18:
			    case 50:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_20
			            PIXEL11_21
			            break;
			        }
			    case 80:
			    case 81:
			        {
			            PIXEL00_20
			            PIXEL01_22
			            PIXEL10_21
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 72:
			    case 76:
			        {
			            PIXEL00_21
			            PIXEL01_20
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_22
			            break;
			        }
			    case 10:
			    case 138:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_21
			            PIXEL10_22
			            PIXEL11_20
			            break;
			        }
			    case 66:
			        {
			            PIXEL00_22
			            PIXEL01_21
			            PIXEL10_21
			            PIXEL11_22
			            break;
			        }
			    case 24:
			        {
			            PIXEL00_21
			            PIXEL01_22
			            PIXEL10_22
			            PIXEL11_21
			            break;
			        }
			    case 7:
			    case 39:
			    case 135:
			        {
			            PIXEL00_11
			            PIXEL01_12
			            PIXEL10_20
			            PIXEL11_20
			            break;
			        }
			    case 148:
			    case 149:
			    case 180:
			        {
			            PIXEL00_20
			            PIXEL01_11
			            PIXEL10_20
			            PIXEL11_12
			            break;
			        }
			    case 224:
			    case 228:
			    case 225:
			        {
			            PIXEL00_20
			            PIXEL01_20
			            PIXEL10_12
			            PIXEL11_11
			            break;
			        }
			    case 41:
			    case 169:
			    case 45:
			        {
			            PIXEL00_12
			            PIXEL01_20
			            PIXEL10_11
			            PIXEL11_20
			            break;
			        }
			    case 22:
			    case 54:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_20
			            PIXEL11_21
			            break;
			        }
			    case 208:
			    case 209:
			        {
			            PIXEL00_20
			            PIXEL01_22
			            PIXEL10_21
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 104:
			    case 108:
			        {
			            PIXEL00_21
			            PIXEL01_20
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_22
			            break;
			        }
			    case 11:
			    case 139:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_21
			            PIXEL10_22
			            PIXEL11_20
			            break;
			        }
			    case 19:
			    case 51:
			        {
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL00_11
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL00_60
			                PIXEL01_90
			            }
			            PIXEL10_20
			            PIXEL11_21
			            break;
			        }
			    case 146:
			    case 178:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			                PIXEL11_12
			            }
			            else
			            {
			                PIXEL01_90
			                PIXEL11_61
			            }
			            PIXEL10_20
			            break;
			        }
			    case 84:
			    case 85:
			        {
			            PIXEL00_20
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL01_11
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL01_60
			                PIXEL11_90
			            }
			            PIXEL10_21
			            break;
			        }
			    case 112:
			    case 113:
			        {
			            PIXEL00_20
			            PIXEL01_22
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL10_12
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL10_61
			                PIXEL11_90
			            }
			            break;
			        }
			    case 200:
			    case 204:
			        {
			            PIXEL00_21
			            PIXEL01_20
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			                PIXEL11_11
			            }
			            else
			            {
			                PIXEL10_90
			                PIXEL11_60
			            }
			            break;
			        }
			    case 73:
			    case 77:
			        {
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL00_12
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL00_61
			                PIXEL10_90
			            }
			            PIXEL01_20
			            PIXEL11_22
			            break;
			        }
			    case 42:
			    case 170:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			                PIXEL10_11
			            }
			            else
			            {
			                PIXEL00_90
			                PIXEL10_60
			            }
			            PIXEL01_21
			            PIXEL11_20
			            break;
			        }
			    case 14:
			    case 142:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			                PIXEL01_12
			            }
			            else
			            {
			                PIXEL00_90
			                PIXEL01_61
			            }
			            PIXEL10_22
			            PIXEL11_20
			            break;
			        }
			    case 67:
			        {
			            PIXEL00_11
			            PIXEL01_21
			            PIXEL10_21
			            PIXEL11_22
			            break;
			        }
			    case 70:
			        {
			            PIXEL00_22
			            PIXEL01_12
			            PIXEL10_21
			            PIXEL11_22
			            break;
			        }
			    case 28:
			        {
			            PIXEL00_21
			            PIXEL01_11
			            PIXEL10_22
			            PIXEL11_21
			            break;
			        }
			    case 152:
			        {
			            PIXEL00_21
			            PIXEL01_22
			            PIXEL10_22
			            PIXEL11_12
			            break;
			        }
			    case 194:
			        {
			            PIXEL00_22
			            PIXEL01_21
			            PIXEL10_21
			            PIXEL11_11
			            break;
			        }
			    case 98:
			        {
			            PIXEL00_22
			            PIXEL01_21
			            PIXEL10_12
			            PIXEL11_22
			            break;
			        }
			    case 56:
			        {
			            PIXEL00_21
			            PIXEL01_22
			            PIXEL10_11
			            PIXEL11_21
			            break;
			        }
			    case 25:
			        {
			            PIXEL00_12
			            PIXEL01_22
			            PIXEL10_22
			            PIXEL11_21
			            break;
			        }
			    case 26:
			    case 31:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_22
			            PIXEL11_21
			            break;
			        }
			    case 82:
			    case 214:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_21
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 88:
			    case 248:
			        {
			            PIXEL00_21
			            PIXEL01_22
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 74:
			    case 107:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_21
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_22
			            break;
			        }
			    case 27:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_10
			            PIXEL10_22
			            PIXEL11_21
			            break;
			        }
			    case 86:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_21
			            PIXEL11_10
			            break;
			        }
			    case 216:
			        {
			            PIXEL00_21
			            PIXEL01_22
			            PIXEL10_10
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 106:
			        {
			            PIXEL00_10
			            PIXEL01_21
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_22
			            break;
			        }
			    case 30:
			        {
			            PIXEL00_10
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_22
			            PIXEL11_21
			            break;
			        }
			    case 210:
			        {
			            PIXEL00_22
			            PIXEL01_10
			            PIXEL10_21
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 120:
			        {
			            PIXEL00_21
			            PIXEL01_22
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_10
			            break;
			        }
			    case 75:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_21
			            PIXEL10_10
			            PIXEL11_22
			            break;
			        }
			    case 29:
			        {
			            PIXEL00_12
			            PIXEL01_11
			            PIXEL10_22
			            PIXEL11_21
			            break;
			        }
			    case 198:
			        {
			            PIXEL00_22
			            PIXEL01_12
			            PIXEL10_21
			            PIXEL11_11
			            break;
			        }
			    case 184:
			        {
			            PIXEL00_21
			            PIXEL01_22
			            PIXEL10_11
			            PIXEL11_12
			            break;
			        }
			    case 99:
			        {
			            PIXEL00_11
			            PIXEL01_21
			            PIXEL10_12
			            PIXEL11_22
			            break;
			        }
			    case 57:
			        {
			            PIXEL00_12
			            PIXEL01_22
			            PIXEL10_11
			            PIXEL11_21
			            break;
			        }
			    case 71:
			        {
			            PIXEL00_11
			            PIXEL01_12
			            PIXEL10_21
			            PIXEL11_22
			            break;
			        }
			    case 156:
			        {
			            PIXEL00_21
			            PIXEL01_11
			            PIXEL10_22
			            PIXEL11_12
			            break;
			        }
			    case 226:
			        {
			            PIXEL00_22
			            PIXEL01_21
			            PIXEL10_12
			            PIXEL11_11
			            break;
			        }
			    case 60:
			        {
			            PIXEL00_21
			            PIXEL01_11
			            PIXEL10_11
			            PIXEL11_21
			            break;
			        }
			    case 195:
			        {
			            PIXEL00_11
			            PIXEL01_21
			            PIXEL10_21
			            PIXEL11_11
			            break;
			        }
			    case 102:
			        {
			            PIXEL00_22
			            PIXEL01_12
			            PIXEL10_12
			            PIXEL11_22
			            break;
			        }
			    case 153:
			        {
			            PIXEL00_12
			            PIXEL01_22
			            PIXEL10_22
			            PIXEL11_12
			            break;
			        }
			    case 58:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_11
			            PIXEL11_21
			            break;
			        }
			    case 83:
			        {
			            PIXEL00_11
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_21
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 92:
			        {
			            PIXEL00_21
			            PIXEL01_11
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 202:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            PIXEL01_21
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            PIXEL11_11
			            break;
			        }
			    case 78:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            PIXEL01_12
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            PIXEL11_22
			            break;
			        }
			    case 154:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_22
			            PIXEL11_12
			            break;
			        }
			    case 114:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_12
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 89:
			        {
			            PIXEL00_12
			            PIXEL01_22
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 90:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 55:
			    case 23:
			        {
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL00_11
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL00_60
			                PIXEL01_90
			            }
			            PIXEL10_20
			            PIXEL11_21
			            break;
			        }
			    case 182:
			    case 150:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			                PIXEL11_12
			            }
			            else
			            {
			                PIXEL01_90
			                PIXEL11_61
			            }
			            PIXEL10_20
			            break;
			        }
			    case 213:
			    case 212:
			        {
			            PIXEL00_20
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL01_11
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL01_60
			                PIXEL11_90
			            }
			            PIXEL10_21
			            break;
			        }
			    case 241:
			    case 240:
			        {
			            PIXEL00_20
			            PIXEL01_22
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL10_12
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL10_61
			                PIXEL11_90
			            }
			            break;
			        }
			    case 236:
			    case 232:
			        {
			            PIXEL00_21
			            PIXEL01_20
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			                PIXEL11_11
			            }
			            else
			            {
			                PIXEL10_90
			                PIXEL11_60
			            }
			            break;
			        }
			    case 109:
			    case 105:
			        {
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL00_12
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL00_61
			                PIXEL10_90
			            }
			            PIXEL01_20
			            PIXEL11_22
			            break;
			        }
			    case 171:
			    case 43:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			                PIXEL10_11
			            }
			            else
			            {
			                PIXEL00_90
			                PIXEL10_60
			            }
			            PIXEL01_21
			            PIXEL11_20
			            break;
			        }
			    case 143:
			    case 15:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			                PIXEL01_12
			            }
			            else
			            {
			                PIXEL00_90
			                PIXEL01_61
			            }
			            PIXEL10_22
			            PIXEL11_20
			            break;
			        }
			    case 124:
			        {
			            PIXEL00_21
			            PIXEL01_11
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_10
			            break;
			        }
			    case 203:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_21
			            PIXEL10_10
			            PIXEL11_11
			            break;
			        }
			    case 62:
			        {
			            PIXEL00_10
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_11
			            PIXEL11_21
			            break;
			        }
			    case 211:
			        {
			            PIXEL00_11
			            PIXEL01_10
			            PIXEL10_21
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 118:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_12
			            PIXEL11_10
			            break;
			        }
			    case 217:
			        {
			            PIXEL00_12
			            PIXEL01_22
			            PIXEL10_10
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 110:
			        {
			            PIXEL00_10
			            PIXEL01_12
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_22
			            break;
			        }
			    case 155:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_10
			            PIXEL10_22
			            PIXEL11_12
			            break;
			        }
			    case 188:
			        {
			            PIXEL00_21
			            PIXEL01_11
			            PIXEL10_11
			            PIXEL11_12
			            break;
			        }
			    case 185:
			        {
			            PIXEL00_12
			            PIXEL01_22
			            PIXEL10_11
			            PIXEL11_12
			            break;
			        }
			    case 61:
			        {
			            PIXEL00_12
			            PIXEL01_11
			            PIXEL10_11
			            PIXEL11_21
			            break;
			        }
			    case 157:
			        {
			            PIXEL00_12
			            PIXEL01_11
			            PIXEL10_22
			            PIXEL11_12
			            break;
			        }
			    case 103:
			        {
			            PIXEL00_11
			            PIXEL01_12
			            PIXEL10_12
			            PIXEL11_22
			            break;
			        }
			    case 227:
			        {
			            PIXEL00_11
			            PIXEL01_21
			            PIXEL10_12
			            PIXEL11_11
			            break;
			        }
			    case 230:
			        {
			            PIXEL00_22
			            PIXEL01_12
			            PIXEL10_12
			            PIXEL11_11
			            break;
			        }
			    case 199:
			        {
			            PIXEL00_11
			            PIXEL01_12
			            PIXEL10_21
			            PIXEL11_11
			            break;
			        }
			    case 220:
			        {
			            PIXEL00_21
			            PIXEL01_11
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 158:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_22
			            PIXEL11_12
			            break;
			        }
			    case 234:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            PIXEL01_21
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_11
			            break;
			        }
			    case 242:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_12
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 59:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_11
			            PIXEL11_21
			            break;
			        }
			    case 121:
			        {
			            PIXEL00_12
			            PIXEL01_22
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 87:
			        {
			            PIXEL00_11
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_21
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 79:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_12
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            PIXEL11_22
			            break;
			        }
			    case 122:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 94:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 218:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 91:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 229:
			        {
			            PIXEL00_20
			            PIXEL01_20
			            PIXEL10_12
			            PIXEL11_11
			            break;
			        }
			    case 167:
			        {
			            PIXEL00_11
			            PIXEL01_12
			            PIXEL10_20
			            PIXEL11_20
			            break;
			        }
			    case 173:
			        {
			            PIXEL00_12
			            PIXEL01_20
			            PIXEL10_11
			            PIXEL11_20
			            break;
			        }
			    case 181:
			        {
			            PIXEL00_20
			            PIXEL01_11
			            PIXEL10_20
			            PIXEL11_12
			            break;
			        }
			    case 186:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_11
			            PIXEL11_12
			            break;
			        }
			    case 115:
			        {
			            PIXEL00_11
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_12
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 93:
			        {
			            PIXEL00_12
			            PIXEL01_11
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 206:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            PIXEL01_12
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            PIXEL11_11
			            break;
			        }
			    case 205:
			    case 201:
			        {
			            PIXEL00_12
			            PIXEL01_20
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_10
			            }
			            else
			            {
			                PIXEL10_70
			            }
			            PIXEL11_11
			            break;
			        }
			    case 174:
			    case 46:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_10
			            }
			            else
			            {
			                PIXEL00_70
			            }
			            PIXEL01_12
			            PIXEL10_11
			            PIXEL11_20
			            break;
			        }
			    case 179:
			    case 147:
			        {
			            PIXEL00_11
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_10
			            }
			            else
			            {
			                PIXEL01_70
			            }
			            PIXEL10_20
			            PIXEL11_12
			            break;
			        }
			    case 117:
			    case 116:
			        {
			            PIXEL00_20
			            PIXEL01_11
			            PIXEL10_12
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_10
			            }
			            else
			            {
			                PIXEL11_70
			            }
			            break;
			        }
			    case 189:
			        {
			            PIXEL00_12
			            PIXEL01_11
			            PIXEL10_11
			            PIXEL11_12
			            break;
			        }
			    case 231:
			        {
			            PIXEL00_11
			            PIXEL01_12
			            PIXEL10_12
			            PIXEL11_11
			            break;
			        }
			    case 126:
			        {
			            PIXEL00_10
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_10
			            break;
			        }
			    case 219:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_10
			            PIXEL10_10
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 125:
			        {
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL00_12
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL00_61
			                PIXEL10_90
			            }
			            PIXEL01_11
			            PIXEL11_10
			            break;
			        }
			    case 221:
			        {
			            PIXEL00_12
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL01_11
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL01_60
			                PIXEL11_90
			            }
			            PIXEL10_10
			            break;
			        }
			    case 207:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			                PIXEL01_12
			            }
			            else
			            {
			                PIXEL00_90
			                PIXEL01_61
			            }
			            PIXEL10_10
			            PIXEL11_11
			            break;
			        }
			    case 238:
			        {
			            PIXEL00_10
			            PIXEL01_12
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			                PIXEL11_11
			            }
			            else
			            {
			                PIXEL10_90
			                PIXEL11_60
			            }
			            break;
			        }
			    case 190:
			        {
			            PIXEL00_10
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			                PIXEL11_12
			            }
			            else
			            {
			                PIXEL01_90
			                PIXEL11_61
			            }
			            PIXEL10_11
			            break;
			        }
			    case 187:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			                PIXEL10_11
			            }
			            else
			            {
			                PIXEL00_90
			                PIXEL10_60
			            }
			            PIXEL01_10
			            PIXEL11_12
			            break;
			        }
			    case 243:
			        {
			            PIXEL00_11
			            PIXEL01_10
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL10_12
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL10_61
			                PIXEL11_90
			            }
			            break;
			        }
			    case 119:
			        {
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL00_11
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL00_60
			                PIXEL01_90
			            }
			            PIXEL10_12
			            PIXEL11_10
			            break;
			        }
			    case 237:
			    case 233:
			        {
			            PIXEL00_12
			            PIXEL01_20
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_100
			            }
			            PIXEL11_11
			            break;
			        }
			    case 175:
			    case 47:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_100
			            }
			            PIXEL01_12
			            PIXEL10_11
			            PIXEL11_20
			            break;
			        }
			    case 183:
			    case 151:
			        {
			            PIXEL00_11
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_100
			            }
			            PIXEL10_20
			            PIXEL11_12
			            break;
			        }
			    case 245:
			    case 244:
			        {
			            PIXEL00_20
			            PIXEL01_11
			            PIXEL10_12
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_100
			            }
			            break;
			        }
			    case 250:
			        {
			            PIXEL00_10
			            PIXEL01_10
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 123:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_10
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_10
			            break;
			        }
			    case 95:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_10
			            PIXEL11_10
			            break;
			        }
			    case 222:
			        {
			            PIXEL00_10
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_10
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 252:
			        {
			            PIXEL00_21
			            PIXEL01_11
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_100
			            }
			            break;
			        }
			    case 249:
			        {
			            PIXEL00_12
			            PIXEL01_22
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_100
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 235:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_21
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_100
			            }
			            PIXEL11_11
			            break;
			        }
			    case 111:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_100
			            }
			            PIXEL01_12
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_22
			            break;
			        }
			    case 63:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_100
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_11
			            PIXEL11_21
			            break;
			        }
			    case 159:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_100
			            }
			            PIXEL10_22
			            PIXEL11_12
			            break;
			        }
			    case 215:
			        {
			            PIXEL00_11
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_100
			            }
			            PIXEL10_21
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 246:
			        {
			            PIXEL00_22
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            PIXEL10_12
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_100
			            }
			            break;
			        }
			    case 254:
			        {
			            PIXEL00_10
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_100
			            }
			            break;
			        }
			    case 253:
			        {
			            PIXEL00_12
			            PIXEL01_11
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_100
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_100
			            }
			            break;
			        }
			    case 251:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            PIXEL01_10
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_100
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 239:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_100
			            }
			            PIXEL01_12
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_100
			            }
			            PIXEL11_11
			            break;
			        }
			    case 127:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_100
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_20
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_20
			            }
			            PIXEL11_10
			            break;
			        }
			    case 191:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_100
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_100
			            }
			            PIXEL10_11
			            PIXEL11_12
			            break;
			        }
			    case 223:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_20
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_100
			            }
			            PIXEL10_10
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_20
			            }
			            break;
			        }
			    case 247:
			        {
			            PIXEL00_11
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_100
			            }
			            PIXEL10_12
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_100
			            }
			            break;
			        }
			    case 255:
			        {
			            if (Diff(RGBtoYUV[w4], RGBtoYUV[w2]))
			            {
			                PIXEL00_0
			            }
			            else
			            {
			                PIXEL00_100
			            }
			            if (Diff(RGBtoYUV[w2], RGBtoYUV[w6]))
			            {
			                PIXEL01_0
			            }
			            else
			            {
			                PIXEL01_100
			            }
			            if (Diff(RGBtoYUV[w8], RGBtoYUV[w4]))
			            {
			                PIXEL10_0
			            }
			            else
			            {
			                PIXEL10_100
			            }
			            if (Diff(RGBtoYUV[w6], RGBtoYUV[w8]))
			            {
			                PIXEL11_0
			            }
			            else
			            {
			                PIXEL11_100
			            }
			            break;
			        }
			}
			
			w1 = w2; w4 = w5; w7 = w8;
			w2 = w3; w5 = w6; w8 = w9;

			dp += 2;
		}
		
		dp += ((dst1line - width) << 1);
		sp +=  (src1line - width);
	}
}
