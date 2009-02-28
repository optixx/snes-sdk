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

#include "mac-prefix.h"
#include "mac-os.h"
#include "mac-gworld.h"

static GDHandle		oldGD;
static GWorldPtr	oldGW;

void InitGWorld(GWorldPtr *world, const Rect *worldSize, short depth)
{
	QDErr	err;
	
	err = NewGWorld(world, depth, worldSize, nil, nil, kNativeEndianPixMap);
	if (err)
		QuitWithFatalError(err, "gworld 01");
	
	LockPixels(GetGWorldPixMap(*world));
	
	PrepareForGDrawing(*world);
	EraseRect(worldSize);
	FinishGDrawing(*world);
}

void PrepareForGDrawing(GWorldPtr world)
{
	GetGWorld(&oldGW, &oldGD);
	SetGWorld(world, nil);
}

void FinishGDrawing(GWorldPtr)
{
	SetGWorld(oldGW, oldGD);
}

void DrawSubCGImage(CGContextRef ctx, CGImageRef image, CGRect src, CGRect dst)
{
    float	w = (float) CGImageGetWidth(image);
    float	h = (float) CGImageGetHeight(image);

	CGRect	drawRect = CGRectMake(0.0, 0.0, w, h);

	if (!CGRectEqualToRect(src, dst))
	{ 
		float	sx = CGRectGetWidth(dst)  / CGRectGetWidth(src);
		float	sy = CGRectGetHeight(dst) / CGRectGetHeight(src);
		float	dx = CGRectGetMinX(dst) - (CGRectGetMinX(src) * sx);
		float	dy = CGRectGetMinY(dst) - (CGRectGetMinY(src) * sy);

		drawRect = CGRectMake(dx, dy, w * sx, h * sy);
	}

	CGContextSaveGState(ctx);
	CGContextClipToRect(ctx, dst);
	CGContextDrawImage(ctx, drawRect, image);
	CGContextRestoreGState(ctx);
}

CGImageRef CreateCGImageFromGWorld(GWorldPtr world)
{
	CGDataProviderRef	prov;
	CGColorSpaceRef		color;
	CGImageRef			image = nil;
	PixMapHandle		pixmap;
	Rect				bounds;
	long				rowbytes, w, h;
	char				*addr, *buf;
	
	if (!world)
		return (nil);
	
	pixmap = GetGWorldPixMap(world);
	if (GetPixDepth(pixmap) != 32)
		return (nil);

	rowbytes = GetPixRowBytes(pixmap);
	addr     = GetPixBaseAddr(pixmap);
	GetPixBounds(pixmap, &bounds);
	
	w = bounds.right  - bounds.left;
	h = bounds.bottom - bounds.top;
	
	buf = (char *) malloc(rowbytes);
	if (buf)
	{
		for (int i = 0; i < (h >> 1); i++)
		{
			memcpy(buf, addr + i * rowbytes, rowbytes);
			memcpy(addr + i * rowbytes, addr + (h - i - 1) * rowbytes, rowbytes);
			memcpy(addr + (h - i - 1) * rowbytes, buf, rowbytes);
		}

		free(buf);
	}

	prov = CGDataProviderCreateWithData(nil, addr, rowbytes * h, nil);
	if (prov)
	{	
		color = CGColorSpaceCreateDeviceRGB();
		if (color)
		{
			image = CGImageCreate(w, h, 8, 32, rowbytes, color, kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrderDefault : 0), prov, nil, 0, kCGRenderingIntentDefault);
			CGColorSpaceRelease(color);
		}
		
		CGDataProviderRelease(prov);
	}
	
	return (image);
}

Boolean CreateResizedBitmapAndCGImage(const CGImageRef inImage, CGImageRef *outImage, void **outBitmap, CGSize size, CGImageAlphaInfo alpha, int depth, Boolean smoothing)
{
	CGContextRef		ctx;
	CGColorSpaceRef		color;
	CGDataProviderRef	prov;
	int					bytesPerPixel, bitsPerComponent, w, h;
	
	bytesPerPixel = depth / 8;
	bitsPerComponent = (depth == 16 ? 5 : 8);
	w = (int) size.width;
	h = (int) size.height;
	
	*outImage = nil;
	
	*outBitmap = calloc(w * h * bytesPerPixel, 1);
	if (*outBitmap)
	{
		color = CGColorSpaceCreateDeviceRGB();
		if (color)
		{
			ctx = CGBitmapContextCreate(*outBitmap, w, h, bitsPerComponent, w * bytesPerPixel, color, alpha);
			if (ctx)
			{
				CGContextSetShouldAntialias(ctx, smoothing);
				CGContextDrawImage(ctx, CGRectMake(0, 0, size.width, size.height), inImage);
				CGContextRelease(ctx);
			}
			
			prov = CGDataProviderCreateWithData(nil, *outBitmap, w * h * bytesPerPixel, nil);
			if (prov)
			{	
				*outImage = CGImageCreate(w, h, bitsPerComponent, depth, w * bytesPerPixel, color, alpha, prov, nil, 0, kCGRenderingIntentDefault);
				CGDataProviderRelease(prov);
			}
			
			CGColorSpaceRelease(color);
		}
	}
	
	if (*outImage == nil)
	{
		if (*outBitmap)
			free(*outBitmap);
		return (false);
	}
	else
		return (true);
}
