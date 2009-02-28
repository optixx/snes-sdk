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

#include "memmap.h"
#include "gfx.h"
#include "apu.h"
#include "bsx.h"
#include "cpuexec.h"
#include "cheats.h"
#include "display.h"
#include "movie.h"
#include "snapshot.h"
#include "spc7110.h"

#include <wchar.h>

#include "mac-prefix.h"
#include "mac-audio.h"
#include "mac-cart.h"
#include "mac-cheatfinder.h"
#include "mac-dialog.h"
#include "mac-file.h"
#include "mac-os.h"
#include "mac-quicktime.h"
#include "mac-screenshot.h"
#include "mac-snes9x.h"
#include "mac-stringtools.h"

extern wchar_t  macRecordWChar[MOVIE_MAX_METADATA];

void SNES9X_Go(void)
{
	if (cartOpen)
		running = true;
}

bool8 SNES9X_OpenCart(FSRef *inRef)
{
	OSStatus		err;
	FSCatalogInfo	info;
	FSRef			cartRef;
	char			filename[PATH_MAX + 1];
	
	DeinitGameWindow();	

	if (cartOpen)
	{
		SNES9X_SaveSRAM();
		S9xSaveCheatFile(S9xGetFilename(".cht", PATCH_DIR));
	}

	Settings.MouseMaster = true;
	Settings.SuperScopeMaster = true;
	Settings.MultiPlayer5Master = true;
	Settings.JustifierMaster = true;

	ResetCheatFinder();
	
	if (!inRef)
	{
		DisableMenus();
		
		if (!NavOpenROMImage(&cartRef))
		{
			cartOpen = false;
			EnableMenus();
			return false;
		}
		
		EnableMenus();
	}
	else
		cartRef = *inRef;
		
	spcFileCount = pngFileCount = 0;

	err = FSGetCatalogInfo(&cartRef, kFSCatInfoVolume, &info, nil, nil, nil);
	lockedROMMedia = IsLockedMedia(info.volume);

	Settings.ForceLoROM          = (romDetect        == kLoROMForce       );
	Settings.ForceHiROM          = (romDetect        == kHiROMForce       );
	Settings.ForceNotInterleaved = (interleaveDetect == kNoInterleaveForce);
	Settings.ForceInterleaved    = (interleaveDetect == kInterleaveForce  );
	Settings.ForceInterleaved2   = (interleaveDetect == kInterleave2Force );
	Settings.ForceInterleaveGD24 = (interleaveDetect == kInterleaveGD24   );
	Settings.ForcePAL            = (videoDetect      == kPALForce         );
	Settings.ForceNTSC           = (videoDetect      == kNTSCForce        );
	Settings.ForceHeader         = (headerDetect     == kHeaderForce      );
	Settings.ForceNoHeader       = (headerDetect     == kNoHeaderForce    );
	
	Settings.ForceSuperFX = Settings.ForceNoSuperFX = false;
	Settings.ForceDSP1    = Settings.ForceNoDSP1    = false;
	Settings.ForceSA1     = Settings.ForceNoSA1     = false;
	Settings.ForceC4      = Settings.ForceNoC4      = false;
	Settings.ForceSDD1    = Settings.ForceNoSDD1    = false;
	
	GFX.InfoString = nil;
	GFX.InfoStringTimeout = 0;
	
	err = FSRefMakePath(&cartRef, (unsigned char *) filename, PATH_MAX);
		
	if (Memory.LoadROM(filename))
	{
		cartOpen = true;

		SNES9X_InitSound();
		SNES9X_LoadSRAM();
		
		ChangeTypeAndCreator(filename, 'CART', '~9X~');
		
		AddRecentItem(&cartRef);
		BuildRecentMenu();
		
		for (int a = 0; a < MAC_MAX_PLAYERS; a++)
			for (int b = 0; b < 12; b++)
				autofireRec[a].nextTime[b] = 0;

		return true;
	}
	else
	{
		cartOpen = false;
		return false;
	}
}

void SNES9X_LoadSRAM(void)
{
	if (cartOpen)
		Memory.LoadSRAM(S9xGetFilename(".srm", SRAM_DIR));
}

void SNES9X_SaveSRAM(void)
{
	const char	*sramFilename;
	
	if (cartOpen)
	{
		sramFilename = S9xGetFilename(".srm", SRAM_DIR);
		Memory.SaveSRAM(sramFilename);
		
		sramFilename = S9xGetFilename(".srm", SRAM_DIR);
		ChangeTypeAndCreator(sramFilename, 'SRAM', '~9X~');
	}
}

void SNES9X_Reset(void)		// Hardware Reset
{
	if (cartOpen)
	{
		SNES9X_SaveSRAM();
		S9xReset();
		SNES9X_LoadSRAM();
	}
}

void SNES9X_SoftReset(void)	// Software Reset
{
	if (cartOpen)
	{
		SNES9X_SaveSRAM();		
		S9xSoftReset();
		SNES9X_LoadSRAM();
	}
}

bool8 SNES9X_Freeze(void)
{
	OSStatus	err;
	FSRef		ref;
	int			which;
    const char	*filename;
	
	if (cartOpen)
	{		
		MacStopSound();

		which = PromptFreezeDefrost(true);
		
		if (which >= 0)
		{
			filename = S9xGetFreezeFilename(which);
			
			err = FSPathMakeRef((unsigned char *) filename, &ref, nil);
			if (!err)
				FSDeleteObject(&ref);
				
			S9xFreezeGame(filename);
			ChangeTypeAndCreator(filename, 'SAVE', '~9X~');
			err = FSPathMakeRef((unsigned char *) filename, &ref, nil);
			WriteThumbnailToResourceFork(&ref, 128, 120);
			
			SNES9X_Go();
			
			MacStartSound();
			return true;
		}
		else
		{
			MacStartSound();
			return false;
		}
	}
	else
		return false;
}

bool8 SNES9X_Defrost(void)
{
	int			which;
	const char	*filename;
	
	if (cartOpen)
	{		
		MacStopSound();

		which = PromptFreezeDefrost(false);
		
		if (which >= 0)
		{
			filename = S9xGetFreezeFilename(which);
			S9xUnfreezeGame(filename);

			SNES9X_Go();
			
			MacStartSound();
			return true;
		}
		else
		{
			MacStartSound();			
			return false;
		}
	}
	else
		return false;
}

bool8 SNES9X_FreezeTo(void)
{
	OSStatus	err;
	FSRef		ref;
    char		filename[PATH_MAX + 1];
	
	if (cartOpen)
	{		
		if (NavFreezeTo(filename))
		{			
			err = FSPathMakeRef((unsigned char *) filename, &ref, nil);
			if (!err)
				FSDeleteObject(&ref);

			S9xFreezeGame(filename);
			ChangeTypeAndCreator(filename, 'SAVE', '~9X~');
			err = FSPathMakeRef((unsigned char *) filename, &ref, nil);
			WriteThumbnailToResourceFork(&ref, 128, 120);

			return true;
		}
	}

	return false;
}

bool8 SNES9X_DefrostFrom(void)
{
	char	filename[PATH_MAX + 1];
	
	if (cartOpen)
	{		
		if (NavDefrostFrom(filename))
		{
			S9xUnfreezeGame(filename);

			SNES9X_Go();

			return true;
		}
	}

	return false;
}

bool8 SNES9X_RecordMovie(void)
{
	OSStatus	err;
	FSRef		ref;
    char		filename[PATH_MAX + 1];
	
	if (cartOpen)
	{		
		if (NavRecordMovieTo(filename))
		{			
			err = FSPathMakeRef((unsigned char *) filename, &ref, nil);
			if (!err)
				FSDeleteObject(&ref);
				
			int		r;
			uint8   opt = 0, mask = 0;
			
			for (int i = 0; i <= 4; i++)
				if (macRecordFlag & (1 << i))
					mask |= (1 << i);

			if (macRecordFlag & (1 << 5))
				opt |= MOVIE_OPT_FROM_RESET;
			else
				opt |= MOVIE_OPT_FROM_SNAPSHOT;

			r = S9xMovieCreate(filename, mask, opt, macRecordWChar, wcslen(macRecordWChar));
			if (r == SUCCESS)
			{
				ChangeTypeAndCreator(filename, 'SMOV', '~9X~');

				if ((macRecordFlag & (1 << 5)) == 0)
				{
					err = FSPathMakeRef((unsigned char *) filename, &ref, nil);
					WriteThumbnailToResourceFork(&ref, 128, 120);
				}

				SNES9X_Go();
				
				return true;
			}
		}
	}

	return false;
}

bool8 SNES9X_PlayMovie(void)
{
	char	filename[PATH_MAX + 1];
	
	if (cartOpen)
	{		
		if (NavPlayMovieFrom(filename))
		{
			int r;
			
			r = S9xMovieOpen(filename, macPlayFlag & 1);
			if (r == SUCCESS)
			{
				SNES9X_Go();

				return true;
			}
		}
	}

	return false;
}

bool8 SNES9X_QTMovieRecord(void)
{
    char	filename[PATH_MAX + 1];
	
	if (cartOpen)
	{		
		if (NavQTMovieRecordTo(filename))
		{			
			MacQTStartRecording(filename);
			macQTRecord = true;
			SNES9X_Go();

			return true;
		}
	}

	macQTRecord = false;
	
	return false;
}

void SNES9X_Quit(void)
{
	DeinitGameWindow();

	if (cartOpen)
	{
		SNES9X_SaveSRAM();
		S9xSaveCheatFile(S9xGetFilename(".cht", PATCH_DIR));
	}

	if (Settings.SPC7110)
		(*CleanUp7110)();
	
	finished = true;
}

void SNES9X_InitSound(void)
{
	S9xInitSound(1, Settings.Stereo, 2048);
}
