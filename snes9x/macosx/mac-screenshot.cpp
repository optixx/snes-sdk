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

#include "gfx.h"
#include "ppu.h"
#include "screenshot.h"

#include <QuickTime/QuickTime.h>

#include "mac-prefix.h"
#include "mac-file.h"
#include "mac-gworld.h"
#include "mac-os.h"
#include "mac-render.h"
#include "mac-screenshot.h"

static void ExportCGImageToPNGFile(CGImageRef, const char *);

PicHandle GetScreenAsPicHandle(int width, int height, int destWidth, int destHeight)
{
	PicHandle	myPicture;
	Rect		drawSize, scaleSize;
	GWorldPtr	drawWorld, scaleWorld;
	Byte		*graphicsIn, *graphicsOut; 
	int			row, graphicsRowBytes;

	SetRect(&drawSize,  0, 0, width,     height);
	SetRect(&scaleSize, 0, 0, destWidth, destHeight);
	
	InitGWorld(&drawWorld,  &drawSize,  16);
	InitGWorld(&scaleWorld, &scaleSize, 16);

	graphicsIn  = (Byte *) GFX.Screen;
	graphicsOut = (Byte *) GetPixBaseAddr(GetGWorldPixMap(drawWorld));
	graphicsRowBytes = GetPixRowBytes(GetGWorldPixMap(drawWorld));
	
	for (row = 0; row < height; row++)
	{
		memcpy(graphicsOut, graphicsIn, width * 2);
		
		if (directDisplay)
		{
			if (drawingMethod != kDrawingOpenGL)
				graphicsIn += 512 * 2;
			else
				graphicsIn += width * 2;
		}
		else
		{
			if (lastDrawingMethod != kDrawingOpenGL)
				graphicsIn += 512 * 2;
			else
				graphicsIn += width * 2;
		}
		
		graphicsOut += graphicsRowBytes;
	}

	if ((scaleSize.right * scaleSize.bottom) < (drawSize.right * drawSize.bottom))
	{
		PrepareForGDrawing(drawWorld);
		CopyBits(GetPortBitMapForCopyBits(drawWorld),  GetPortBitMapForCopyBits(scaleWorld), &drawSize,  &scaleSize, srcCopy | ditherCopy, nil);
		FinishGDrawing(drawWorld);

		PrepareForGDrawing(scaleWorld);
		myPicture = OpenPicture(&scaleSize);
		CopyBits(GetPortBitMapForCopyBits(scaleWorld), GetPortBitMapForCopyBits(scaleWorld), &scaleSize, &scaleSize, srcCopy, nil);
		ClosePicture();
		FinishGDrawing(scaleWorld);
	}
	else
	{
		PrepareForGDrawing(scaleWorld);
		myPicture = OpenPicture(&scaleSize);
		CopyBits(GetPortBitMapForCopyBits(drawWorld),  GetPortBitMapForCopyBits(scaleWorld), &drawSize,  &scaleSize, srcCopy, nil);
		ClosePicture();
		FinishGDrawing(scaleWorld);
	}

	DisposeGWorld(drawWorld);
	DisposeGWorld(scaleWorld);

	return myPicture;
}

void WriteThumbnailToResourceFork(FSRef *ref, int destWidth, int destHeight)
{
	OSStatus		err;
	PicHandle		pict;
	HFSUniStr255	fork;
	SInt16			resf;
	
	err = FSGetResourceForkName(&fork);
	if (err == noErr)
	{
		err = FSCreateResourceFork(ref, fork.length, fork.unicode, 0);
		if ((err == noErr) || (err == errFSForkExists))
		{
			err = FSOpenResourceFile(ref, fork.length, fork.unicode, fsWrPerm, &resf);
			if (err == noErr)
			{
				pict = GetScreenAsPicHandle(IPPU.RenderedScreenWidth, IPPU.RenderedScreenHeight, destWidth, destHeight);
				if (pict)
				{
					AddResource((Handle) pict, 'PICT', 128, "\p");
					WriteResource((Handle) pict);
					ReleaseResource((Handle) pict);
				}
				
				CloseResFile(resf);
			}
		}
	}
}

static void ExportCGImageToPNGFile(CGImageRef image, const char *path)
{
	OSStatus				err;
	GraphicsExportComponent	exporter;
	CFStringRef				str;
	CFURLRef				url;
	Handle					dataRef;
	OSType					dataRefType;
	
	str = CFStringCreateWithCString(kCFAllocatorDefault, path, MAC_PATH_ENCODING);
	if (str)
	{
		url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, str, kCFURLPOSIXPathStyle, false);
		if (url)
		{
			err = QTNewDataReferenceFromCFURL(url, 0, &dataRef, &dataRefType);
			if (err == noErr)
			{
				err = OpenADefaultComponent(GraphicsExporterComponentType, kQTFileTypePNG, &exporter);
				if (err == noErr)
				{
					err = GraphicsExportSetInputCGImage(exporter, image);
					if (err == noErr)
					{
						err = GraphicsExportSetOutputDataReference(exporter, dataRef, dataRefType);
						if (err == noErr)
							err = GraphicsExportDoExport(exporter, nil);
					}
					
					CloseComponent(exporter);
				}
				
				DisposeHandle(dataRef);
			}
			
			CFRelease(url);
		}
		
		CFRelease(str);
	}
}

CGImageRef CreateGameScreenCGImage(void)
{
	CGDataProviderRef	prov;
	CGColorSpaceRef		color;
	CGImageRef			image = nil;
	int					rowbytes;

	rowbytes = (((directDisplay ? drawingMethod : lastDrawingMethod) == kDrawingOpenGL) ? IPPU.RenderedScreenWidth : 512) * 2;

	prov = CGDataProviderCreateWithData(nil, GFX.Screen, 512 * 2 * 478, nil);
	if (prov)
	{		
		color = CGColorSpaceCreateDeviceRGB();
		if (color)
		{
			image = CGImageCreate(IPPU.RenderedScreenWidth, IPPU.RenderedScreenHeight, 5, 16, rowbytes, color, kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrder16Host : 0), prov, nil, 1, kCGRenderingIntentDefault);
			CGColorSpaceRelease(color);
		}

		CGDataProviderRelease(prov);
	}
	
	return (image);
}

CGImageRef CreateBlitScreenCGImage(int height, uint8 *buffer)
{
	CGDataProviderRef	prov;
	CGColorSpaceRef		color;
	CGImageRef			image = nil;
	
	prov = CGDataProviderCreateWithData(nil, buffer, 512 * 2 * 478, nil);
	if (prov)
	{		
		color = CGColorSpaceCreateDeviceRGB();
		if (color)
		{
			image = CGImageCreate(512, height, 5, 16, 512 * 2, color, kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrder16Host : 0), prov, nil, 1, kCGRenderingIntentDefault);
			CGColorSpaceRelease(color);
		}
		
		CGDataProviderRelease(prov);
	}
	
	return (image);
}

void DrawThumbnailPICTResource(FSRef *ref, CGContextRef ctx, CGRect bounds)
{
	OSStatus			err;
	CGDataProviderRef	prov;
	QDPictRef			qdpr;
	PicHandle			pict;
	HFSUniStr255		fork;
	SInt16				resf;
	Size				size;
	
	CGContextSaveGState(ctx);
	
	CGContextSetRGBFillColor(ctx, 0.0, 0.0, 0.0, 1.0);
	CGContextFillRect(ctx, bounds);
	
	err = FSGetResourceForkName(&fork);
	if (err == noErr)
	{
		err = FSOpenResourceFile(ref, fork.length, fork.unicode, fsRdPerm, &resf);
		if (err == noErr)
		{
			pict = (PicHandle) Get1Resource('PICT', 128);
			if (pict)
			{
				HLock((Handle) pict);
				
				size = GetHandleSize((Handle) pict);
				prov = CGDataProviderCreateWithData(nil, (void *) *pict, size, nil);
				if (prov)
				{
					qdpr = QDPictCreateWithProvider(prov);
					if (qdpr)
					{
						QDPictDrawToCGContext(ctx, bounds, qdpr);
						QDPictRelease(qdpr);
					}
					
					CGDataProviderRelease(prov);
				}
				
				HUnlock((Handle) pict);
				ReleaseResource((Handle) pict);
			}
			
			CloseResFile(resf);
		}
	}
	
	CGContextRestoreGState(ctx);
}

bool8 S9xDoScreenshot(int width, int height)
{
	CGImageRef	image, resizedImage;
	CGSize		size;
	Boolean		r;
	void		*bitmap;
	
	Settings.TakeScreenshot = false;
	
	size = CGSizeMake((float) (width * ((width <= SNES_WIDTH) ? 2 : 1)), (float) (height * ((height <= SNES_HEIGHT_EXTENDED) ? 2 : 1)));
	
	image = CreateGameScreenCGImage();
	if (image)
	{
		r = CreateResizedBitmapAndCGImage(image, &resizedImage, &bitmap, size, (CGImageAlphaInfo) (kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrderDefault : 0)), 16, false);
		if (r)
		{
			ExportCGImageToPNGFile(resizedImage, S9xGetPNGFilename());
			
			CGImageRelease(resizedImage);
			free(bitmap);
		}
		
		CGImageRelease(image);
	}
	
	return true;
}
