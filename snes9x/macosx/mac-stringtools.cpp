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

#include "mac-prefix.h"
#include "mac-stringtools.h"

void ConvertPString(const unsigned char *pS, char *cS)
{
	short	size = *pS++;
	
	while (size--)
		*cS++ = *pS++;
	*cS = 0;
}

void ConvertCString(const char *cS, unsigned char *pS)
{
	unsigned char	*pP = ++pS;
	
	while (*cS)
		*pP++ = *cS++;
	*(pS - 1) = pP - pS;
}

void JointPStrings(const unsigned char *s1, const unsigned char *s2, const unsigned char *s3, unsigned char *result)
{
	short 	i;
	
	if (((short) s1[0] + (short) s2[0] + (short) s3[0]) > 255)
		return;
	
	result[0] = s1[0] + s2[0] + s3[0];
	
	for (i = 1; i <= s1[0]; i++)
		result[i] = s1[i];
	for (i = s1[0] + 1; i <= s1[0] + s2[0]; i++)
		result[i] = s2[i - s1[0]];
	for (i = s1[0] + s2[0] + 1; i <= s1[0] + s2[0] + s3[0]; i++)
		result[i] = s3[i - s1[0] - s2[0]];
}

CFStringRef CopyFixNameStrings(const char * itext, short romreg)
{
	if (romreg != 0)	// non-Japanese
	{
		char	btext[256];
		
		for (uint i = 0; i < strlen(itext) ; i++)
		{
			if (!isgraph(btext[i] = itext[i]))
				btext[i] = ' ';
		}

		return CFStringCreateWithCString(kCFAllocatorDefault, btext, kCFStringEncodingNonLossyASCII);
	}
	else
		return CFStringCreateWithCString(kCFAllocatorDefault, itext, kCFStringEncodingMacJapanese);		// assuming Shift-JIS
	
	return NULL;	
}

OSStatus CreateATSUIStyleFromFontFullNameAndSize(Str255 fontName, short fontSize, ATSUStyle *theStyle)
{
	OSStatus	err;
	ATSUStyle	localStyle;
	ATSUFontID	atsuFont;
	Fixed		atsuSize;

	ATSUAttributeTag		theTags[]   = { kATSUFontTag,       kATSUSizeTag  };
	ByteCount				theSizes[]  = { sizeof(ATSUFontID), sizeof(Fixed) };
	ATSUAttributeValuePtr	theValues[] = { nil,                nil           };

	atsuFont = 0;
	atsuSize = FixRatio(fontSize, 1);
	localStyle = NULL;

	theValues[0] = &atsuFont;
	theValues[1] = &atsuSize;

	err = ATSUFindFontFromName((Ptr) fontName + 1, (long) fontName[0], kFontFullName, (unsigned long) kFontNoPlatform, (unsigned long) kFontNoScript, (unsigned long) kFontNoLanguage, &atsuFont);
	if (err == noErr)
	{
		err = ATSUCreateStyle(&localStyle);
		if (err == noErr)
		{
			err = ATSUSetAttributes(localStyle, sizeof(theTags) / sizeof(theTags[0]), theTags, theSizes, theValues);
			if (err == noErr)
			{
				*theStyle = localStyle;
				return (noErr);
			}
		}
	}

	if (localStyle != nil)
		err = ATSUDisposeStyle(localStyle);

    return (err);
}

const char * GetMultiByteCharacters(CFStringRef inText, int cutAt)
{
	CFStringRef	str;
	static char	buf[256];	
	
	buf[0] = 0;
	
	if (!inText || (cutAt < 1))
		return (buf);
	
	if (CFStringGetCString(inText, buf, sizeof(buf), CFStringGetSystemEncoding()))
	{
		buf[cutAt] = 0;
		
		str = CFStringCreateWithCString(kCFAllocatorDefault, buf, CFStringGetSystemEncoding());
		if (str)
			CFRelease(str);
		else
			buf[cutAt - 1] = 0;
	}
	
	return (buf);
}
