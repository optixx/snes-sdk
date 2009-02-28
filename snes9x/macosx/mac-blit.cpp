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
#include "mac-2xsai.h"
#include "mac-epx.h"
#include "mac-hq2x.h"
#include "mac-blit.h"

int						gSrcP = 1024;

static unsigned char	XDelta[1024 * 240];

void BlitUpdateEntireDelta(void)
{
	UInt32	*d = (UInt32 *) XDelta;
	int		x, y;
	
	for (y = 0; y < 240; y++)
		for (x = 0; x < 256; x++)
			*d++ = 0x80008000;
}

void BlitClearExtendArea(register unsigned char *srcPtr, register Rect *srcRect)
{
	unsigned char	*clear;
	UInt16			height = srcRect->bottom - srcRect->top; // 224 or 239. width = 256.

	#define	BYTES_PER_PIXEL	2
	
	clear = srcPtr - (512 * 2 + 2) * BYTES_PER_PIXEL;
	memset(clear, 0, (2 + 256 + 2) * BYTES_PER_PIXEL);
	clear = srcPtr - (512 * 1 + 2) * BYTES_PER_PIXEL;
	memset(clear, 0, (2 + 256 + 2) * BYTES_PER_PIXEL);
	
	clear = srcPtr;
	for (UInt16 y = 0; y < height; y++)
	{
		*(UInt32 *) (clear -   2 * BYTES_PER_PIXEL) = 0;
		*(UInt32 *) (clear + 256 * BYTES_PER_PIXEL) = 0;
		clear += 512 * BYTES_PER_PIXEL;
	}
	
	clear = srcPtr + (512 * height       - 2) * BYTES_PER_PIXEL;
	memset(clear, 0, (2 + 256 + 2) * BYTES_PER_PIXEL);
	clear = srcPtr + (512 * (height + 1) - 2) * BYTES_PER_PIXEL;
	memset(clear, 0, (2 + 256 + 2) * BYTES_PER_PIXEL);
}

void BlitPixHiRes16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	Byte	*finish, *dstPtr2;
	long	lineSize = (srcRect->right  - srcRect->left) << 1;
	long	height   =  srcRect->bottom - srcRect->top;
	
	srcPtr  += (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr  += (2 * dstRect->left) + (dstRowBytes * dstRect->top);
	dstPtr2  = dstPtr + dstRowBytes;
	
	dstRowBytes <<= 1;
	
	do
	{
		double	*dP  = (double *) (dstPtr ),
				*dP2 = (double *) (dstPtr2),
				*bP  = (double *) (srcPtr );

		finish = (Byte *) bP + lineSize;
		
		do
		{
			*dP++ = *dP2++ = *bP++;
			*dP++ = *dP2++ = *bP++;
		}
		while ((Byte *) bP < finish);
		
		srcPtr  += gSrcP;
		dstPtr  += dstRowBytes;
		dstPtr2 += dstRowBytes;
	}
	while (--height);
}

void BlitPixHiResTV16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	Byte	*finish, *dstPtr2;
	long	lineSize  = (srcRect->right  - srcRect->left) << 1;
	long	height    =  srcRect->bottom - srcRect->top;
	long	colorMask = 0x3DEF3DEF;
	
	srcPtr  += (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr  += (2 * dstRect->left) + (dstRowBytes * dstRect->top);
	dstPtr2  = dstPtr + dstRowBytes;
	
	dstRowBytes <<= 1;
	
	do
	{
		unsigned long	*dP  = (unsigned long *) (dstPtr ),
						*dP2 = (unsigned long *) (dstPtr2),
						*bP  = (unsigned long *) (srcPtr ),
						value1, value2, total1, total2;

		finish = (Byte * ) bP + lineSize;
		
		do
		{
			value1 = *dP++ = *bP++;
			value2 = *dP++ = *bP++;
			
			total1  = (value1 = (value1 >> 1) & colorMask);
			total2  = (value2 = (value2 >> 1) & colorMask);
			total1 += (value1 = (value1 >> 1) & colorMask);
			*dP2++  = total1;
			total2 += (value2 = (value2 >> 1) & colorMask);
			*dP2++  = total2;			
		}
		while ((Byte *) bP < finish);
		
		srcPtr  += gSrcP;
		dstPtr  += dstRowBytes;
		dstPtr2 += dstRowBytes;
	}
	while (--height);
}

void BlitPixSmall16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	long	lineSize = (srcRect->right  - srcRect->left) << 1;
	long	height   =  srcRect->bottom - srcRect->top;
	
	srcPtr += (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr += (2 * dstRect->left) + (dstRowBytes * dstRect->top);
	
	do
	{
		memcpy(dstPtr, srcPtr, lineSize);
		
		srcPtr += gSrcP;
		dstPtr += dstRowBytes;
	}

	while (--height);
}

void BlitPixEagle16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	srcPtr += (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr += (2 * dstRect->left) + (dstRowBytes * dstRect->top);

	SuperEagle(srcPtr, gSrcP, dstPtr, dstRowBytes, srcRect->right - srcRect->left, srcRect->bottom - srcRect->top);
}

void BlitPix2xSaI16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	srcPtr += (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr += (2 * dstRect->left) + (dstRowBytes * dstRect->top);

	_2xSaI(srcPtr, gSrcP, dstPtr, dstRowBytes, srcRect->right - srcRect->left, srcRect->bottom - srcRect->top);
}

void BlitPixSuper2xSaI16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	srcPtr += (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr += (2 * dstRect->left) + (dstRowBytes * dstRect->top);

	Super2xSaI(srcPtr, gSrcP, dstPtr, dstRowBytes, srcRect->right - srcRect->left, srcRect->bottom - srcRect->top);
}

void BlitPixEPX16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	srcPtr += (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr += (2 * dstRect->left) + (dstRowBytes * dstRect->top);

	EPX_16(srcPtr, gSrcP, dstPtr, dstRowBytes, srcRect->right - srcRect->left, srcRect->bottom - srcRect->top);
}

void BlitPixHQ2x16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	srcPtr += (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr += (2 * dstRect->left) + (dstRowBytes * dstRect->top);

	HQ2X_16(srcPtr, gSrcP, dstPtr, dstRowBytes, srcRect->right - srcRect->left, srcRect->bottom - srcRect->top);
}

void BlitPixFiltered16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	UInt32	lastLinePix[1024];
	UInt8	lastLineChg[1024];	
	Byte	*prevLine, *deltaPtr, *finish;
	long	lineSize     = (srcRect->right  - srcRect->left) << 1;
	long	height       =  srcRect->bottom - srcRect->top;
	long	dstLineSize  = dstRowBytes << 1;
	
	srcPtr  +=          (2 * srcRect->left) + (gSrcP       * srcRect->top);
	deltaPtr = XDelta + (2 * srcRect->left) + (gSrcP       * srcRect->top);
	prevLine = dstPtr + (2 * dstRect->left) + (dstRowBytes * dstRect->top);
	dstPtr   = prevLine + dstRowBytes;
	
	for (int index = 0; index < gSrcP; index++)
	{
		lastLinePix[index] = 0;
		lastLineChg[index] = 0;
	}

	do
	{
		unsigned char	*lC = (unsigned char *) lastLineChg;
		unsigned long	*bP = (unsigned long *) srcPtr,
						*xP = (unsigned long *) deltaPtr,
						*dP = (unsigned long *) dstPtr,
						*pL = (unsigned long *) prevLine,
						*lL = (unsigned long *) lastLinePix,
						colorMask    = 0x3DEF3DEF,
						lowPixelMask = 0x04210421,
						currentPixel, nextPixel, currentDelta, nextDelta, lastPix, lastChg, thisChg, currentPixA, currentPixB;
		UInt16			savePixel;
							   
		finish = srcPtr + lineSize + 4;
		 
		savePixel = *(UInt16 *) (srcPtr + lineSize);
		*(UInt16 *) (srcPtr + lineSize) = *(UInt16 *) (srcPtr + lineSize - 2);
		*(unsigned long *) (XDelta + lineSize) = *(unsigned long *) (srcPtr + lineSize);
		
		nextPixel = *bP++;
		nextDelta = *xP++;
		
		do
		{
			currentPixel = nextPixel;
			currentDelta = nextDelta;
			nextPixel    = *bP++;
			nextDelta    = *xP++;
			lastChg      = *lC;
			thisChg      = (nextPixel - nextDelta) | (currentPixel - currentDelta);
			
		#ifdef __BIG_ENDIAN__
			unsigned long	colorA = (currentPixel >> 16) & 0xFFFF;
			unsigned long	colorB = (currentPixel      ) & 0xFFFF;
			unsigned long	colorC = (nextPixel    >> 16) & 0xFFFF;
			
			currentPixA = (colorA << 16) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask))      );
			currentPixB = (colorB << 16) | ((((colorC >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorC & colorB & lowPixelMask))      );
		#else
			unsigned long	colorA = (currentPixel      ) & 0xFFFF;
			unsigned long	colorB = (currentPixel >> 16) & 0xFFFF;
			unsigned long	colorC = (nextPixel         ) & 0xFFFF;
			
			currentPixA = (colorA      ) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask)) << 16);
			currentPixB = (colorB      ) | ((((colorC >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorC & colorB & lowPixelMask)) << 16);
		#endif
		
			if (thisChg | lastChg)
			{
				xP[-2]  = currentPixel;
				
				lastPix = lL[0];
				dP[0]   = currentPixA;
				lL[0]   = currentPixA;
				pL[0]   = ((currentPixA >> 1) & colorMask) + ((lastPix >> 1) & colorMask) + (currentPixA & lastPix & lowPixelMask);
				
				lastPix = lL[1];
				dP[1]   = currentPixB;
				lL[1]   = currentPixB;
				pL[1]   = ((currentPixB >> 1) & colorMask) + ((lastPix >> 1) & colorMask) + (currentPixB & lastPix & lowPixelMask);
				
				*lC++   = (thisChg != 0);
			}
			else
			{								
				lL[0]   = currentPixA;
				lL[1]   = currentPixB;
				*lC++   = 0;
			}
			
			lL += 2;
			dP += 2;
			pL += 2;
		}
		while ((Byte *) bP < finish);

		*(UInt16 *) (srcPtr + lineSize) = savePixel;
				
		deltaPtr += gSrcP;
		srcPtr   += gSrcP;
		dstPtr   += dstLineSize;
		prevLine += dstLineSize;
	}
	while (--height);
}

#ifdef MAC_PPCASM_SUPPORT

void BlitPixScaled16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	register unsigned char	*dstPtr2, *deltaPtr;
	
	long	lineSize  = (srcRect->right  - srcRect->left) >> 1;
	long	height    =  srcRect->bottom - srcRect->top;

	srcPtr  +=          (2 * srcRect->left) + (gSrcP       * srcRect->top);
	deltaPtr = XDelta + (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr  +=          (2 * dstRect->left) + (dstRowBytes * dstRect->top);
	dstPtr2  = dstPtr + dstRowBytes;
	
	dstRowBytes <<= 1;
	
	asm volatile
	(
		"1:						\n"
		
		"mr		r26, %5			\n"	//	*dP1 = (unsigned long *) dstPtr
		"mr		r27, %6			\n"	//	*dP2 = (unsigned long *) dstPtr2
		"mr		r28, %7			\n"	//	*bP  = (unsigned long *) srcPtr
		"mr		r29, %8			\n"	//	*xP  = (unsigned long *) deltaPtr
				
		"mtctr	%12				\n"	//	ctr = lineSize

		"2:						\n"
		
		"lwz	r10, 0(r28)		\n"	//	currentPixel = *bP
		"lwz	r9,  0(r29)		\n"	//	lastPixel    = *xP
		"cmplw	r10, r9			\n"	//	if (currentPixel == lastPixel)
		"beq+	0f				\n"	//		goto 0:
				
		"stw	r10, 0(r29)		\n"	//	*xP = *bP
				
		"mr		r9, r10			\n"
		
		"inslwi	r9,  r9,  16, 16\n"	//	r9  = currentPixA
		"stw	r9,  0(r26)		\n"	//	*dP1 = r9
		"stw	r9,  0(r27)		\n"	//	*dP2 = r9
		
		"insrwi	r10, r10, 16, 0	\n"	//	r10 = currentPixB
		"stw	r10, 4(r26)		\n"	//	*(dP1 + 1) = currentPixB
		"stw	r10, 4(r27)		\n"	//	*(dP2 + 1) = currentPixB
				
		"0:						\n"
				
		"addi	r26, r26, 8		\n"	//	dP1 += 2
		"addi	r27, r27, 8		\n"	//	dP2 += 2
		"addi	r28, r28, 4		\n"	//	bP++
		"addi	r29, r29, 4		\n"	//	dP++
		
		"bdnz+	2b				\n"	//	if (--ctr) goto 2:
		
		"add	%0, %5, %11		\n"	//	dstPtr2  += dstRowBytes
		"add	%1, %6, %11		\n"	//	dstPtr   += dstRowBytes
		"add	%2, %7, %10		\n"	//	deltaPtr += gSrcP
		"add	%3, %8, %10		\n"	//	srcPtr   += gSrcP
		
		"subic.	%4, %9, 1		\n"	//	height--
		"bne+	1b				\n"	//	if (height) goto 1:
		
		: "=&r"(dstPtr), "=&r"(dstPtr2), "=&r"(srcPtr), "=&r"(deltaPtr), "=&r"(height)
		: "0"  (dstPtr), "1"  (dstPtr2), "2"  (srcPtr), "3"  (deltaPtr), "4"  (height), "r"(gSrcP), "r"(dstRowBytes), "r"(lineSize)
		: "r9", "r10", "r26", "r27", "r28", "r29", "ctr", "memory"
	); 
}

#else

void BlitPixScaled16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	register unsigned char	*dstPtr2, *deltaPtr;
	
	long	lineSize = srcRect->right  - srcRect->left;
	long	height   = srcRect->bottom - srcRect->top;
	
	srcPtr  +=          (2 * srcRect->left) + (gSrcP       * srcRect->top);
	deltaPtr = XDelta + (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr  +=          (2 * dstRect->left) + (dstRowBytes * dstRect->top);
	dstPtr2  = dstPtr + dstRowBytes;
	
	dstRowBytes <<= 1;
	
	do
	{
		unsigned long	*dP1 = (unsigned long *) dstPtr,
						*dP2 = (unsigned long *) dstPtr2,
						*bP  = (unsigned long *) srcPtr,
						*xP  = (unsigned long *) deltaPtr,
						currentPixel, lastPixel, currentPixA, currentPixB;

		for (long i = 0; i < (lineSize >> 1); i++)
		{
			currentPixel = *bP;
			lastPixel    = *xP;
			
			if (currentPixel != lastPixel)
			{			
			#ifdef __BIG_ENDIAN__
				unsigned long	colorA = (currentPixel >> 16) & 0xFFFF;
				unsigned long	colorB = (currentPixel      ) & 0xFFFF;
			#else
				unsigned long	colorA = (currentPixel      ) & 0xFFFF;
				unsigned long	colorB = (currentPixel >> 16) & 0xFFFF;
			#endif
			
				currentPixA = (colorA << 16) | colorA;
				currentPixB = (colorB << 16) | colorB;
	
				dP1[0] = currentPixA;
				dP1[1] = currentPixB;
				dP2[0] = currentPixA;
				dP2[1] = currentPixB;
				
				*xP = *bP;
			}
			
			bP++;
			xP++;
			dP1 += 2;
			dP2 += 2;
		}
		
		srcPtr   += gSrcP;
		deltaPtr += gSrcP;
		dstPtr   += dstRowBytes;
		dstPtr2  += dstRowBytes;
	}
	while (--height);
}

#endif

#ifdef MAC_PPCASM_SUPPORT

void BlitPixScaledTV16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	register unsigned char	*dstPtr2, *deltaPtr;
	
	long	lineSize = (srcRect->right  - srcRect->left - 2) >> 1;
	long	height   =  srcRect->bottom - srcRect->top;
	
	srcPtr  +=          (2 * srcRect->left) + (gSrcP       * srcRect->top);
	deltaPtr = XDelta + (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr  +=          (2 * dstRect->left) + (dstRowBytes * dstRect->top);
	dstPtr2  = dstPtr + dstRowBytes;
	
	dstRowBytes <<= 1;
	
	asm volatile
	(
		"lis	r8, 0x3DEF		\n"	//	  R     G     B         R     G     B 
		"addi	r8, r8, 0x3DEF	\n"	//	0 01111 01111 01111   0 01111 01111 01111
		
		"1:						\n"
		
		"mr		r26, %5			\n"	//	*dP1 = (unsigned long *) dstPtr
		"mr		r27, %6			\n"	//	*dP2 = (unsigned long *) dstPtr2
		"mr		r28, %7			\n"	//	*bP  = (unsigned long *) srcPtr
		"mr		r29, %8			\n"	//	*xP  = (unsigned long *) deltaPtr

		"mtctr	%12				\n"	//	ctr = lineSize

		"2:						\n"	//	0 - 510 Pixels

		"lwz	r10, 0(r28)		\n"	//	currentPixel = *bP
		"lwz	r9,  0(r29)		\n"	//	currentDelta = *xP
		"lwz	r7,  4(r28)		\n" //	nextPixel    = *(bP + 1)
		"cmplw	r10, r9			\n"	//	if (currentPixel != currentDelta)
		"bne-	3f				\n"	//		goto 3:
		"lwz	r6,  4(r29)		\n"	//	nextDelta    = *(xP + 1)
		"cmplw	r7,  r6			\n"	//	if (nextPixel == nextDelta)
		"beq+	0f				\n"	//		goto 0:
		
		"3:						\n"
		
		"stw	r10, 0(r29)		\n"	//	*xP = *bP
		
		"mr		r9,  r10		\n"
		"srwi	r9,  r9,  16	\n"	//	colorA
		"clrlwi	r10, r10, 16	\n"	//	colorB
		
		"srwi	r6,  r9,  1		\n"	//	r6  = colorA >> 1
		"and	r11, r6,  r8	\n"	//	r11 = r6 & 0x3DEF3DEF
		"srwi   r6,  r10, 1		\n"	//	r6  = colorB >> 1
		"and    r12, r6,  r8	\n"	//	r12 = r6 & 0x3DEF3DEF
		"add	r12, r12, r11	\n"	//	r12 += r11
		"and	r11, r9,  r10	\n"	//	r11 = r9 & r10
		"andi.	r11, r11, 0x0421\n"	//	r11 &= 0x0421
		"add	r12, r12, r11	\n"	//	r12 += r11
		"slwi	r9,  r9,  16	\n"	//	colorA <<= 16
		"add	r12, r12, r9	\n"	//	r12 += colorA
		"stw	r12, 0(r26)		\n"	//	*dP1 = r12
		
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"mr		r11, r12		\n"	//	r11 = r12
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"add	r11, r11, r12	\n"	//	r11 += r12
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"add	r11, r11, r12	\n"	//	r11 += r12
		"stw	r11, 0(r27)		\n"	//	*dP2 = r11
		
		"srwi	r9,  r7,  16	\n"	//	colorA
		
		"srwi	r6,  r9,  1		\n"	//	r6  = colorA >> 1
		"and	r11, r6,  r8	\n"	//	r11 = r6 & 0x3DEF3DEF
		"srwi   r6,  r10, 1		\n"	//	r6  = colorB >> 1
		"and    r12, r6,  r8	\n"	//	r12 = r6 & 0x3DEF3DEF
		"add	r12, r12, r11	\n"	//	r12 += r11
		"and	r11, r9,  r10	\n"	//	r11 = r9 & r10
		"andi.	r11, r11, 0x0421\n"	//	r11 &= 0x0421
		"add	r12, r12, r11	\n"	//	r12 += r11
		"slwi	r10, r10, 16	\n"	//	colorB <<= 16
		"add	r12, r12, r10	\n"	//	r12 += colorB
		"stw	r12, 4(r26)		\n"	//	*(dP1 + 1) = r12
		
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"mr		r11, r12		\n"	//	r11 = r12
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"add	r11, r11, r12	\n"	//	r11 += r12
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"add	r11, r11, r12	\n"	//	r11 += r12
		"stw	r11, 4(r27)		\n"	//	*(dP2 + 1) = r11
		
		"0:						\n"
		
		"addi	r26, r26, 8		\n"	//	dP1 += 2
		"addi	r27, r27, 8		\n"	//	dP2 += 2
		"addi	r28, r28, 4		\n"	//	bP++
		"addi	r29, r29, 4		\n"	//	dP++
		
		"bdnz+	2b				\n"	//	if (--ctr) goto 2:

									//	Last 2 Pixels
				
		"lwz	r10, 0(r28)		\n"	//	currentPixel = *bP
		"lwz	r9,  0(r29)		\n"	//	currentDelta = *xP
		"cmplw	r10, r9			\n"	//	if (currentPixel == currentDelta)
		"beq+	4f				\n"	//		goto 4:
		
		"stw	r10, 0(r29)		\n"	//	*xP = *bP
		
		"mr		r9,  r10		\n"
		"srwi	r9,  r9,  16	\n"	//	colorA
		"clrlwi	r10, r10, 16	\n"	//	colorB
		
		"srwi	r6,  r9,  1		\n"	//	r6  = colorA >> 1
		"and	r11, r6,  r8	\n"	//	r11 = r6 & 0x3DEF3DEF
		"srwi   r6,  r10, 1		\n"	//	r6  = colorB >> 1
		"and    r12, r6,  r8	\n"	//	r12 = r6 & 0x3DEF3DEF
		"add	r12, r12, r11	\n"	//	r12 += r11
		"and	r11, r9,  r10	\n"	//	r11 = r9 & r10
		"andi.	r11, r11, 0x0421\n"	//	r11 &= 0x0421
		"add	r12, r12, r11	\n"	//	r12 += r11
		"slwi	r9,  r9,  16	\n"	//	colorA <<= 16
		"add	r12, r12, r9	\n"	//	r12 += colorA
		"stw	r12, 0(r26)		\n"	//	*dP1 = r12
		
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"mr		r11, r12		\n"	//	r11 = r12
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"add	r11, r11, r12	\n"	//	r11 += r12
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"add	r11, r11, r12	\n"	//	r11 += r12
		"stw	r11, 0(r27)		\n"	//	*dP2 = r11
		
		"slwi	r12, r10, 16	\n"
		"add	r12, r12, r10	\n"	//	r12 = (r10 << 16) | r10
		"stw	r12, 4(r26)		\n"	//	*(dP1 + 1) = r12
		
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"mr		r11, r12		\n"	//	r11 = r12
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"add	r11, r11, r12	\n"	//	r11 += r12
		"srwi	r12, r12, 1		\n"	//	r12 >>= 1
		"and	r12, r12, r8	\n"	//	r12 &= 0x3DEF3DEF
		"add	r11, r11, r12	\n"	//	r11 += r12
		"stw	r11, 4(r27)		\n"	//	*(dP2 + 1) = r11
		
		"4:						\n"
				
		"add	%0, %5, %11		\n"	//	dstPtr2  += dstRowBytes
		"add	%1, %6, %11		\n"	//	dstPtr   += dstRowBytes
		"add	%2, %7, %10		\n"	//	deltaPtr += gSrcP
		"add	%3, %8, %10		\n"	//	srcPtr   += gSrcP
		
		"subic.	%4, %9, 1		\n"	//	height--
		"bne+	1b				\n"	//	if (height) goto 1:

		: "=&r"(dstPtr), "=&r"(dstPtr2), "=&r"(srcPtr), "=&r"(deltaPtr), "=&r"(height)
		: "0"  (dstPtr), "1"  (dstPtr2), "2"  (srcPtr), "3"  (deltaPtr), "4"  (height), "r"(gSrcP), "r"(dstRowBytes), "r"(lineSize)
		: "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r26", "r27", "r28", "r29", "ctr", "memory"
	); 
}

#else

void BlitPixScaledTV16(register unsigned char *srcPtr, register unsigned char *dstPtr, register long dstRowBytes, register Rect *srcRect, register Rect *dstRect)
{
	register unsigned char	*dstPtr2, *deltaPtr;
	
	long		lineSize  = srcRect->right  - srcRect->left;
	long		height    = srcRect->bottom - srcRect->top;
	const long	colorMask = 0x3DEF3DEF, lowPixelMask = 0x0421;
	
	srcPtr  +=          (2 * srcRect->left) + (gSrcP       * srcRect->top);
	deltaPtr = XDelta + (2 * srcRect->left) + (gSrcP       * srcRect->top);
	dstPtr  +=          (2 * dstRect->left) + (dstRowBytes * dstRect->top);
	dstPtr2  = dstPtr + dstRowBytes;
	
	dstRowBytes <<= 1;

	do
	{
		unsigned long	*dP1 = (unsigned long *) dstPtr,
						*dP2 = (unsigned long *) dstPtr2,
						*bP  = (unsigned long *) srcPtr,
						*xP  = (unsigned long *) deltaPtr,
						currentPixel, nextPixel, currentDelta, nextDelta, colorA, colorB, product, darkened;
		
		for (long i = 0; i < (lineSize >> 1) - 1; i++)
		{
			currentPixel = *bP;
			currentDelta = *xP;
			nextPixel    = *(bP + 1);
			nextDelta    = *(xP + 1);

			if ((currentPixel != currentDelta) || (nextPixel != nextDelta))
			{
		    	*xP = *bP;
				
			#ifdef __BIG_ENDIAN__
				colorA = (currentPixel >> 16) & 0xFFFF;
				colorB = (currentPixel      ) & 0xFFFF;
			#else
				colorA = (currentPixel      ) & 0xFFFF;
				colorB = (currentPixel >> 16) & 0xFFFF;
			#endif
			
			#ifdef __BIG_ENDIAN__
		    	*dP1       = product = (colorA << 16) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask))      );
			#else
		    	*dP1       = product = (colorA      ) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask)) << 16);
			#endif
			
			    darkened  = (product = ((product >> 1) & colorMask));
			    darkened += (product = ((product >> 1) & colorMask));
			    darkened +=             (product >> 1) & colorMask;
				
		    	*dP2       = darkened;
				
			#ifdef __BIG_ENDIAN__
				colorA = (nextPixel    >> 16) & 0xFFFF;
			#else
				colorA = (nextPixel         ) & 0xFFFF;
			#endif
			
			#ifdef __BIG_ENDIAN__
				*(dP1 + 1) = product = (colorB << 16) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask))      );
			#else
				*(dP1 + 1) = product = (colorB      ) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask)) << 16);
			#endif
			
			    darkened  = (product = ((product >> 1) & colorMask));
			    darkened += (product = ((product >> 1) & colorMask));
			    darkened +=             (product >> 1) & colorMask;
		    	
				*(dP2 + 1) = darkened;
			}
			
			bP++;
			xP++;
			dP1 += 2;
			dP2 += 2;
	    }

		// Last 2 Pixels
		
		currentPixel = *bP;
		currentDelta = *xP;

		if (currentPixel != currentDelta)
		{
			*xP = *bP;
		
		#ifdef __BIG_ENDIAN__
			colorA = (currentPixel >> 16) & 0xFFFF;
			colorB = (currentPixel      ) & 0xFFFF;
		#else
			colorA = (currentPixel      ) & 0xFFFF;
			colorB = (currentPixel >> 16) & 0xFFFF;
		#endif
		
		#ifdef __BIG_ENDIAN__
			*dP1       = product = (colorA << 16) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask))      );
		#else
			*dP1       = product = (colorA      ) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask)) << 16);
		#endif
		
			darkened  = (product = ((product >> 1) & colorMask));
			darkened += (product = ((product >> 1) & colorMask));
			darkened +=             (product >> 1) & colorMask;
			
			*dP2       = darkened;
			
			*(dP1 + 1) = product = (colorB << 16) | colorB;

			darkened  = (product = ((product >> 1) & colorMask));
			darkened += (product = ((product >> 1) & colorMask));
			darkened +=             (product >> 1) & colorMask;
			
			*(dP2 + 1) = darkened;
		}
			
		srcPtr   += gSrcP;
		deltaPtr += gSrcP;
		dstPtr   += dstRowBytes;
		dstPtr2  += dstRowBytes;
	}
	while (--height);
}

#endif
