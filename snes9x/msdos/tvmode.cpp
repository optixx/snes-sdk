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

#include <sys/farptr.h>

#include "snes9x.h"
#include "port.h"
#include "gfx.h"

#include <allegro.h>

extern uint32 screen_width;
extern uint32 screen_height;

void TVMode (uint8 *srcPtr, uint32 srcPitch,
	     uint8 *deltaPtr,
	     BITMAP *dstBitmap, int width, int height)
{
    uint8 *finish;
    uint32 dstPitch = dstBitmap->w * 2;
    uint32 line;
    uint32 x_offset;
    uint32 fraction;
    uint32 error = 0;
    uint32 colorMask = ~(RGB_LOW_BITS_MASK | (RGB_LOW_BITS_MASK << 16));
    uint32 lowPixelMask = RGB_LOW_BITS_MASK;

    if (height * 2 <= screen_height)
    {
	line = (screen_height - height * 2) >> 1;
	fraction = 0x10000;
    }
    else
    {
	line = 0;
	fraction = ((screen_height - height) * 0x10000) / height;
    }

    _farsetsel (dstBitmap->seg);
    
    if (width == 512)
    {
	// Offset into scanline in bytes, since each pixel takes two bytes,
	// no divide by two.
	x_offset = screen_width - width;

	do
	{
	    uint32 *bP = (uint32 *) srcPtr;
	    uint32 *xP = (uint32 *) deltaPtr;
	    uint32 dP = bmp_write_line (dstBitmap, line) + x_offset;
	    uint32 currentPixel;
	    uint32 currentDelta;

	    finish = (uint8 *) bP + ((width + 2) << 1);
	    if ((error += fraction) >= 0x10000)
	    {
		error -= 0x10000;
		do
		{
		    currentPixel = *bP++;

		    if (currentPixel != *xP++)
		    {
			uint32 product, darkened;

			*(xP - 1) = currentPixel;
			_farnspokel (dP, currentPixel);

			darkened = (product = ((currentPixel & colorMask) >> 1));
			darkened += (product = ((product & colorMask) >> 1));
			darkened += (product & colorMask) >> 1;
			_farnspokel (dP + dstPitch, darkened);
		    }

		    dP += 4;
		} while ((uint8 *) bP < finish);
		line += 2;
	    }
	    else
	    {
		do
		{
		    currentPixel = *bP++;

		    if (currentPixel != *xP++)
		    {
			*(xP - 1) = currentPixel;
			_farnspokel (dP, currentPixel);
		    }
		    dP += 4;
		} while ((uint8 *) bP < finish);
		line++;
	    }

	    deltaPtr += srcPitch;
	    srcPtr += srcPitch;
	} while (--height);
    }
    else
    {
	x_offset = (screen_width - width * 2);
	do
	{
	    uint32 *bP = (uint32 *) srcPtr;
	    uint32 *xP = (uint32 *) deltaPtr;
	    uint32 dP = bmp_write_line (dstBitmap, line) + x_offset;
	    uint32 currentPixel;
	    uint32 nextPixel;
	    uint32 currentDelta;
	    uint32 nextDelta;

	    finish = (uint8 *) bP + ((width + 2) << 1);
	    nextPixel = *bP++;
	    nextDelta = *xP++;
	    if ((error += fraction) >= 0x10000)
	    {
		error -= 0x10000;
		do
		{
		    currentPixel = nextPixel;
		    currentDelta = nextDelta;
		    nextPixel = *bP++;
		    nextDelta = *xP++;

		    if ((nextPixel != nextDelta) || (currentPixel != currentDelta))
		    {
			uint32 colorA, colorB, product, darkened;

			*(xP - 2) = currentPixel;
			colorA = currentPixel & 0xffff;

			colorB = (currentPixel & 0xffff0000) >> 16;
			product = colorA |
				  ((((colorA & colorMask) >> 1) +
				    ((colorB & colorMask) >> 1) +
				    (colorA & colorB & lowPixelMask)) << 16);
			_farnspokel (dP, product);

			darkened = (product = ((product & colorMask) >> 1));
			darkened += (product = ((product & colorMask) >> 1));
			darkened += (product & colorMask) >> 1;
			_farnspokel (dP + dstPitch, darkened);

			colorA = nextPixel & 0xffff;
			product = colorB |
				  ((((colorA & colorMask) >> 1) +
				    ((colorB & colorMask) >> 1) +
				    (colorA & colorB & lowPixelMask)) << 16);
			_farnspokel (dP + 4, product);

			darkened = (product = ((product & colorMask) >> 1));
			darkened += (product = ((product & colorMask) >> 1));
			darkened += (product & colorMask) >> 1;
			_farnspokel (dP + dstPitch + 4, darkened);
		    }

		    dP += 8;
		} while ((uint8 *) bP < finish);
		line += 2;
	    }
	    else
	    {
		do
		{
		    currentPixel = nextPixel;
		    currentDelta = nextDelta;
		    nextPixel = *bP++;
		    nextDelta = *xP++;

		    if ((nextPixel != nextDelta) || (currentPixel != currentDelta))
		    {
			uint32 colorA, colorB, product;

			*(xP - 2) = currentPixel;
			colorA = currentPixel & 0xffff;

			colorB = (currentPixel & 0xffff0000) >> 16;
			product = colorA |
				  ((((colorA & colorMask) >> 1) +
				    ((colorB & colorMask) >> 1) +
				    (colorA & colorB & lowPixelMask)) << 16);
			_farnspokel (dP, product);

			colorA = nextPixel & 0xffff;
			product = colorB |
				  ((((colorA & colorMask) >> 1) +
				    ((colorB & colorMask) >> 1) +
				    (colorA & colorB & lowPixelMask)) << 16);
			_farnspokel (dP + 4, product);
		    }
		    dP += 8;
		} while ((uint8 *) bP < finish);
		line++;
	    }

	    deltaPtr += srcPitch;
	    srcPtr += srcPitch;
	} while (--height);
    }
}

