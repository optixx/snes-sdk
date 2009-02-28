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
#include "spc7110.h"

#include "mac-prefix.h"
#include "mac-audio.h"
#include "mac-dialog.h"
#include "mac-keyboard.h"
#include "mac-os.h"
#include "mac-snes9x.h"
#include "mac-stringtools.h"
#include "mac-prefs.h"

#ifdef MAC_COREIMAGE_SUPPORT
#include <OpenGL/OpenGL.h>
#include "mac-coreimage.h"
#endif

static void SelectTabPane(HIViewRef, SInt16);
static pascal void LittleArrowsActionProc(HIViewRef, HIViewPartCode);
static pascal OSStatus TabEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus PreferencesEventHandler(EventHandlerCallRef, EventRef, void *);

static int	lastTabIndex = 1;
static int	tabList[]    = { 5, 129, 130, 131, 132, 133 };

struct PrefList
{
	long	itemName;
	void	*itemPointer;
	short	size;
};

static PrefList	prefList[] =
{
	{ 'flsc', &fullscreen,									sizeof(bool8      ) },
	{ 'reso', &autoRes,										sizeof(bool8      ) },
	{ 'dfps', &Settings.DisplayFrameRate,					sizeof(bool8      ) },
	{ '7int', &Settings.Mode7Interpolate,					sizeof(bool8      ) },
	{ 'tran', &Settings.Transparency,						sizeof(bool8      ) },
	{ 'gl32', &gl32bit,										sizeof(bool8      ) },
	{ 'glst', &glstretch,									sizeof(bool8      ) },
	{ 'draw', &drawingMethod,								sizeof(long       ) },
	{ '2xgr', &doubleSize,									sizeof(bool8      ) },
	{ 't.v.', &tvMode,										sizeof(bool8      ) },
	{ 'fltr', &smoothMode,									sizeof(bool8      ) },
	{ 'eagl', &eagleMode,									sizeof(bool8      ) },
	{ 'sai ', &saiMode,										sizeof(bool8      ) },
	{ 'sSai', &supsaiMode,									sizeof(bool8      ) },
	{ 'epx_', &epxMode,										sizeof(bool8      ) },
	{ 'hq2x', &hq2xMode,									sizeof(bool8      ) },
	{ 'MPmt', &multiprocessor,								sizeof(bool8      ) },
	{ 'VSNC', &vsync,										sizeof(bool8      ) },
	{ 'H239', &drawoverscan,								sizeof(bool8      ) },
	{ 'SCur', &screencurvature,								sizeof(bool8      ) },
	{ 'SCuW', &macCurvatureWarp,							sizeof(int        ) },
	{ 'ASPe', &macAspectRatio,								sizeof(int        ) },
	{ 'CIFl', &ciFilterEnable,							    sizeof(bool8      ) },
	
	{ 'audi', &Settings.APUEnabled,							sizeof(bool8      ) },
	{ 'so16', &Settings.SixteenBitSound,					sizeof(bool8      ) },
	{ 'ster', &Settings.Stereo,								sizeof(bool8      ) },
	{ 'rbst', &Settings.ReverseStereo,						sizeof(bool8      ) },
	{ 'sint', &Settings.InterpolatedSound,					sizeof(bool8      ) },
	{ 'ehgt', &Settings.SoundEnvelopeHeightReading,			sizeof(bool8      ) },
	{ 'echo', &Settings.DisableSoundEcho,					sizeof(bool8      ) },
	{ 'mstv', &Settings.DisableMasterVolume,				sizeof(bool8      ) },
	{ 'srat', &Settings.SoundPlaybackRate,					sizeof(uint32     ) },
	{ 'deco', &Settings.AltSampleDecode,					sizeof(uint8      ) },
	{ 'pich', &macSoundPitch,								sizeof(float      ) },
	{ 'Volm', &macSoundVolume,								sizeof(SInt32     ) },
	{ 'TMiv', &macSoundInterval,							sizeof(int        ) },
	{ 'AUef', &aueffect,									sizeof(uint16     ) },
	{ 'AUce', &cureffect,									sizeof(int        ) },
		
	{ 'romf', &saveInROMFolder,								sizeof(uint8      ) },
	{ 'atsv', &Settings.AutoSaveDelay,						sizeof(int32      ) },
	{ '7110', &mac7110Load,									sizeof(int        ) },
	{ '711M', &mac7110Megs,									sizeof(int        ) },
	{ 'SD1P', &macSDD1Pack,									sizeof(int        ) },
	{ 'RFlg', &macRecordFlag,								sizeof(uint16     ) },
	{ 'PFlg', &macPlayFlag,									sizeof(uint16     ) },
	{ 'QTfg', &macQTMovFlag,								sizeof(uint16     ) },

	{ 'CPUc', &Settings.CyclesPercentage,					sizeof(int32      ) },
	{ 'stdn', &Settings.ShutdownMaster,						sizeof(bool8      ) },
	{ 'TbRt', &macFastForwardRate,							sizeof(int        ) },
	{ 'FSkp', &macFrameSkip,							    sizeof(int        ) },
	
	{ 'StOp', &startopendlog,								sizeof(bool8      ) },
	{ 'STiF', &showtimeinfrz,								sizeof(bool8      ) },
	{ 'Togl', &enabletoggle,								sizeof(bool8      ) },
	{ 'SvWp', &savewindowpos,								sizeof(bool8      ) },		
	{ 'OnSc', &onscreeninfo,								sizeof(bool8      ) },
	{ 'NIPS', &Settings.NoPatch,							sizeof(bool8      ) },
	{ 'BSX_', &Settings.BSXBootup,							sizeof(bool8      ) },
	{ 'MzCP', &minimizecpu,									sizeof(bool8      ) },
	{ 'MbxM', &musicboxmode,								sizeof(int        ) },
	{ 'InAc', &inactiveMode,								sizeof(int        ) },
	
	{ 'tab ', &lastTabIndex,								sizeof(int        ) },
	{ 'Ftab', &autofireLastTabIndex,						sizeof(int        ) },
	{ 'keyb', keyCode,										sizeof(keyCode    ) },
	{ 'pset', &padSetting,									sizeof(int        ) },
	{ 'dset', &deviceSetting,								sizeof(int        ) },
	{ 'chea', &applycheat,									sizeof(bool8      ) },
	
	{ 'ARec', autofireRec,									sizeof(autofireRec) },
	{ 'WPos', windowPos,									sizeof(windowPos  ) },
	{ 'WSiz', windowSize,									sizeof(windowSize ) },
	{ 'WExt', (void *) &windowExtend,						sizeof(bool8      ) },
	
	{ 'NPIP', npServerIP,									sizeof(npServerIP ) },
	{ 'NPNM', npName,										sizeof(npName     ) },
	
	{ 'L239', &lastoverscan,								sizeof(bool8      ) },
	
	{ 'ExT1', &(extraOptions.benchmark),					sizeof(bool8      ) },
	{ 'ExT2', &(extraOptions.glForceNoTextureRectangle),	sizeof(bool8      ) },
	{ 'ExT3', &(extraOptions.glUseClientStrageApple),		sizeof(bool8      ) },
	{ 'ExT4', &(extraOptions.glUseTexturePriority),			sizeof(bool8      ) },
	{ 'ExT5', &(extraOptions.glStorageHint),				sizeof(int        ) }
};

#define	kPrefListSize	(sizeof(prefList) / sizeof(prefList[0]))

enum
{
	iNibGFullScreen = 1,
	iNibGSwitchResolution,
	iNibGShowFrameRate,
	iNibGMode7Smoothing,
	iNibGTransparency,
	iNibGGL32bit,
	iNibGGLStretch,
	iNibGReserved1,	// unused
	iNibGVideoMode,
	iNibGDirectMP,
	iNibGGLVSync,
	iNibGDrawOverscan,
	iNibGScreenCurvature,
	iNibGCurvatureWarp,
	iNibGAspectRatio,
	iNibGCoreImageFilter,
	iNibGCIFilterEnable,
	
	iNibSEnableSound = 201,
	iNibS16BitPlayback,
	iNibSStereo,
	iNibSReverseStereo,
	iNibSInterpolation,
	iNibSHeightReading,
	iNibSEcho,
	iNibSReserved1,	// unused
	iNibSMasterVolume,
	iNibSPlaybackRate,
	iNibSSampleDecoder,
	iNibSReserved2,	// unused
	iNibSReserved3,	// unused
	iNibSPitch,
	iNibSVolume,
	iNibSMixInterval,
	
	iNibOSaveFolder = 401,
	iNibOReserved1,	// unused
	iNibOReserved2,	// unused
	iNibOAutoSaveInterval,
	iNibOSPC7110Pack,
	iNibOSPC7110Megs,
	iNibOSDD1Pack,
	
	iNibMCPUCycles = 601,
	iNibMShutdownMaster,
	iNibMTurboSkipArrows,
	iNibMTurboSkipText,
	iNibMFrameSkip,
	
	iNibXStartOpenDialog = 801,
	iNibXShowTimeInFrz,
	iNibXMusicBoxMode,
	iNibXEnableToggle,
	iNibXReserved1,	// unused
	iNibXSaveWindowPos,
	iNibXUseIPSPatch,
	iNibXOnScreenInfo,
	iNibXMinimizeCPUUsage,
	iNibXInactiveMode,
	iNibXBSXBootup,
	
	iDirectSmall = 1,
	iDirectBlocky,
	iDirectTVMode,
	iDirectSmoothMode,
	iDirectEAGLEMode,
	iDirect2xSAIMode,
	iDirectSuper2xSAIMode,
	iDirectEPXMode,
	iDirectHQ2xMode,
	iOpenGLBlocky = 11,
	iOpenGLTVMode,
	iOpenGLSmoothMode,
	iOpenGLEagleMode,
	iOpenGL2xSAIMode,
	iOpenGLSuper2xSAIMode,
	iOpenGLEPXMode,
	iOpenGLHQ2xMode,
	
	iBufferSmall = 1,
	iBufferLarge = 2
};

static int	grouplist1[] =
{
	iNibS16BitPlayback,
	iNibSStereo,
	iNibSInterpolation,
	iNibSHeightReading,
	iNibSEcho,
	iNibSMasterVolume,
	iNibSPlaybackRate,
	iNibSSampleDecoder,
	iNibSPitch,
	iNibSMixInterval
};

#define	kGroupListSize1 ((int) (sizeof(grouplist1) / sizeof(grouplist1[0])))

void SavePrefs(void)
{
	CFMutableStringRef	mref;
	CFStringRef			sref;
	CFDataRef			data;
	
	for (unsigned int i = 0; i < kPrefListSize; i++)
	{		
		mref = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, CFSTR("Preferences_"));
		if (mref)
		{
			sref = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8 *) &(prefList[i].itemName), sizeof(long), kCFStringEncodingMacRoman, false);
			if (sref)
			{
				CFStringAppend(mref, sref);

				data = CFDataCreate(kCFAllocatorDefault, (UInt8 *) prefList[i].itemPointer, prefList[i].size);
				if (data)
				{
					CFPreferencesSetAppValue(mref, data, kCFPreferencesCurrentApplication);
					CFRelease(data);
				}

				CFRelease(sref);
			}

			CFRelease(mref);
		}
	}
	
	CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
}

void LoadPrefs(void)
{
	CFMutableStringRef	mref;
	CFStringRef			sref;
	CFDataRef			data;
	
	for (unsigned int i = 0; i < kPrefListSize; i++)
	{		
		mref = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, CFSTR("Preferences_"));
		if (mref)
		{
			sref = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8 *) &(prefList[i].itemName), sizeof(long), kCFStringEncodingMacRoman, false);
			if (sref)
			{
				CFStringAppend(mref, sref);
				
				data = (CFDataRef) CFPreferencesCopyAppValue(mref, kCFPreferencesCurrentApplication);
				if (data)
				{
					if (CFDataGetLength(data) == prefList[i].size)
						CFDataGetBytes(data, CFRangeMake(0, prefList[i].size), (UInt8 *) prefList[i].itemPointer);
					CFRelease(data);
				}
				
				CFRelease(sref);
			}
			
			CFRelease(mref);
		}
	}
}

void ConfigurePreferences(void)
{
	OSStatus	err;
	IBNibRef	nibRef;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		WindowRef	tWindowRef;
		uint32 		oldPlaybackRate;
		bool8		oldEnableSound, old16BitPlayback, oldStereo, oldFullscreen;
		int			oldMixInterval;
		SInt32		oldVolume;
		
		oldEnableSound   = Settings.APUEnabled;
		old16BitPlayback = Settings.SixteenBitSound;
		oldStereo        = Settings.Stereo;
		oldPlaybackRate  = Settings.SoundPlaybackRate;
		oldMixInterval   = macSoundInterval;
		oldVolume        = macSoundVolume;
		oldFullscreen    = fullscreen;

		if (cartOpen)
			DeinitGameWindow();

		S9xGraphicsDeinit();
		
		err = CreateWindowFromNib(nibRef, CFSTR("Preferences"), &tWindowRef);
		if (err == noErr)
		{
			HIViewRef			ctl, root;
			HIViewID			cid;
			ControlActionUPP	actionUPP;
			EventHandlerUPP		tabEventProc, prefEventProc;
			EventHandlerRef		tabHandlerRef, prefHandlerRef;
			EventTypeSpec		tabControlSpec[]  = { { kEventClassControl, kEventControlHit  } },
								prefControlSpec[] = { { kEventClassWindow,  kEventWindowClose }, { kEventClassCommand, kEventCommandProcess } };
			MenuRef				menu;
			char				num[16];
			unsigned char		numP[16];
			int					i;

			root = HIViewGetRoot(tWindowRef);

			actionUPP = NewControlActionUPP(LittleArrowsActionProc);

			cid.signature = 'tabs';
			cid.id = 128;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, lastTabIndex);
			SelectTabPane(ctl, lastTabIndex);
			tabEventProc = NewEventHandlerUPP(TabEventHandler);
			err = InstallControlEventHandler(ctl, tabEventProc, GetEventTypeCount(tabControlSpec), tabControlSpec, 0, &tabHandlerRef);
			
			prefEventProc = NewEventHandlerUPP(PreferencesEventHandler);
			err = InstallWindowEventHandler(tWindowRef, prefEventProc, GetEventTypeCount(prefControlSpec), prefControlSpec, (void *) tWindowRef, &prefHandlerRef);

			cid.signature = 'grap';

			cid.id = iNibGFullScreen;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, fullscreen);
			
			cid.id = iNibGSwitchResolution;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, autoRes);
			
			cid.id = iNibGShowFrameRate;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.DisplayFrameRate);
			
			cid.id = iNibGMode7Smoothing;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.Mode7Interpolate);
			
			cid.id = iNibGTransparency;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.Transparency);
			
			cid.id = iNibGGL32bit;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, gl32bit);
			
			cid.id = iNibGGLStretch;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, glstretch);

			cid.id = iNibGVideoMode;
			HIViewFindByID(root, cid, &ctl);
			menu = GetControlPopupMenuHandle(ctl);
			for (i = 1; i <= CountMenuItems(menu); i++)
				CheckMenuItem(menu, i, false);
			switch (drawingMethod)
			{
				case kDrawingDirect:
					if (smoothMode)
					{
						CheckMenuItem(menu, iDirectSmoothMode, true);
						SetControl32BitValue(ctl, iDirectSmoothMode);
					}
					else
					if (eagleMode)
					{
						CheckMenuItem(menu, iDirectEAGLEMode, true);
						SetControl32BitValue(ctl, iDirectEAGLEMode);
					}
					else
					if (supsaiMode)
					{
						CheckMenuItem(menu, iDirectSuper2xSAIMode, true);
						SetControl32BitValue(ctl, iDirectSuper2xSAIMode);
					}
					else
					if (saiMode)
					{
						CheckMenuItem(menu, iDirect2xSAIMode, true);
						SetControl32BitValue(ctl, iDirect2xSAIMode);
					}
					else
					if (epxMode)
					{
						CheckMenuItem(menu, iDirectEPXMode, true);
						SetControl32BitValue(ctl, iDirectEPXMode);
					}
					else
					if (hq2xMode)
					{
						CheckMenuItem(menu, iDirectHQ2xMode, true);
						SetControl32BitValue(ctl, iDirectHQ2xMode);
					}
					else
					if (tvMode)
					{
						CheckMenuItem(menu, iDirectTVMode, true);
						SetControl32BitValue(ctl, iDirectTVMode);
					}
					else
					if (doubleSize)
					{
						CheckMenuItem(menu, iDirectBlocky, true);
						SetControl32BitValue(ctl, iDirectBlocky);
					}
					else
					{
						CheckMenuItem(menu, iDirectSmall, true);
						SetControl32BitValue(ctl, iDirectSmall);
					}
					
					break;
				
				case kDrawingOpenGL:
					if (smoothMode)
					{
						CheckMenuItem(menu, iOpenGLSmoothMode, true);
						SetControl32BitValue(ctl, iOpenGLSmoothMode);
					}
					else
					{
						CheckMenuItem(menu, iOpenGLBlocky, true);
						SetControl32BitValue(ctl, iOpenGLBlocky);
					}
					
					break;
					
				case kDrawingBlitGL:
					if (eagleMode)
					{
						CheckMenuItem(menu, iOpenGLEagleMode, true);
						SetControl32BitValue(ctl, iOpenGLEagleMode);
					}
					else
					if (supsaiMode)
					{
						CheckMenuItem(menu, iOpenGLSuper2xSAIMode, true);
						SetControl32BitValue(ctl, iOpenGLSuper2xSAIMode);
					}
					else
					if (saiMode)
					{
						CheckMenuItem(menu, iOpenGL2xSAIMode, true);
						SetControl32BitValue(ctl, iOpenGL2xSAIMode);
					}
					else
					if (epxMode)
					{
						CheckMenuItem(menu, iOpenGLEPXMode, true);
						SetControl32BitValue(ctl, iOpenGLEPXMode);
					}
					else
					if (hq2xMode)
					{
						CheckMenuItem(menu, iOpenGLHQ2xMode, true);
						SetControl32BitValue(ctl, iOpenGLHQ2xMode);
					}
					else					
					{
						CheckMenuItem(menu, iOpenGLTVMode, true);
						SetControl32BitValue(ctl, iOpenGLTVMode);
					}
			}

			cid.id = iNibGDirectMP;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, multiprocessor);
			
			cid.id = iNibGGLVSync;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, vsync);

			cid.id = iNibGDrawOverscan;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, drawoverscan);

			cid.id = iNibGScreenCurvature;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, screencurvature);

			cid.id = iNibGCurvatureWarp;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, macCurvatureWarp);
			if (!screencurvature)
				DeactivateControl(ctl);

			cid.id = iNibGAspectRatio;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, macAspectRatio);
			if (!glstretch)
				DeactivateControl(ctl);

			cid.id = iNibGCIFilterEnable;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, ciFilterEnable);
			if (systemVersion < 0x1040)
				DisableControl(ctl);

			if (systemVersion < 0x1040)
			{
				cid.id = iNibGCoreImageFilter;
				HIViewFindByID(root, cid, &ctl);
				DisableControl(ctl);
			}
			
			cid.signature = 'snd_';
			
			cid.id = iNibSEnableSound;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.APUEnabled);
			
			cid.id = iNibS16BitPlayback;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.SixteenBitSound);
			
			cid.id = iNibSStereo;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.Stereo);
			
			cid.id = iNibSReverseStereo;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.ReverseStereo);
			if (!Settings.APUEnabled || !Settings.Stereo)
				DeactivateControl(ctl);
		
			cid.id = iNibSInterpolation;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.InterpolatedSound);
			
			cid.id = iNibSHeightReading;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.SoundEnvelopeHeightReading);

			cid.id = iNibSEcho;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, !Settings.DisableSoundEcho);

			cid.id = iNibSMasterVolume;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, !Settings.DisableMasterVolume);
			
			cid.id = iNibSPlaybackRate;
			HIViewFindByID(root, cid, &ctl);
			menu = GetControlPopupMenuHandle(ctl);
			for (i = 1; i <= CountMenuItems(menu); i++)
				CheckMenuItem(menu, i, false);
			switch (Settings.SoundPlaybackRate)
			{
				case 48000:
					CheckMenuItem(menu, 1, true);
					SetControl32BitValue(ctl, 1);
					break;
				
				case 44100:
					CheckMenuItem(menu, 2, true);
					SetControl32BitValue(ctl, 2);
					break;
				
				case 35000:
					CheckMenuItem(menu, 3, true);
					SetControl32BitValue(ctl, 3);
					break;
				
				case 32000:
					CheckMenuItem(menu, 4, true);
					SetControl32BitValue(ctl, 4);
					break;
				
				case 30000:
					CheckMenuItem(menu, 5, true);
					SetControl32BitValue(ctl, 5);
					break;
				
				case 22050:
					CheckMenuItem(menu, 6, true);
					SetControl32BitValue(ctl, 6);
					break;
				
				case 16000:
					CheckMenuItem(menu, 7, true);
					SetControl32BitValue(ctl, 7);
					break;
					
				case 11025:
					CheckMenuItem(menu, 8, true);
					SetControl32BitValue(ctl, 8);
					break;
					
				case 8000:
					CheckMenuItem(menu, 9, true);
					SetControl32BitValue(ctl, 9);
			}

			cid.id = iNibSSampleDecoder;
			HIViewFindByID(root, cid, &ctl);
			menu = GetControlPopupMenuHandle(ctl);
			for (i = 1; i <= CountMenuItems(menu); i++)
				CheckMenuItem(menu, i, false);
			switch (Settings.AltSampleDecode)
			{
				case 0:
					CheckMenuItem(menu, 1, true);
					SetControl32BitValue(ctl, 1);
					break;
					
				case 1:
				case 2:
					CheckMenuItem(menu, 2, true);
					SetControl32BitValue(ctl, 2);
					break;
					
				case 3:
					CheckMenuItem(menu, 3, true);
					SetControl32BitValue(ctl, 3);
			}

			cid.id = iNibSMixInterval;
			HIViewFindByID(root, cid, &ctl);
			sprintf(num, "%d", macSoundInterval);
			ConvertCString(num, numP);
			SetEditTextText(ctl, numP, false);

			cid.id = iNibSPitch;
			HIViewFindByID(root, cid, &ctl);
			sprintf(num, "%2.4f", macSoundPitch);
			ConvertCString(num, numP);
			SetEditTextText(ctl, numP, false);
			
			cid.id = iNibSVolume;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, macSoundVolume);
			
			for (i = 0; i < kGroupListSize1; i++)
			{
				cid.id = grouplist1[i];
				HIViewFindByID(root, cid, &ctl);
				if (!Settings.APUEnabled)
					DeactivateControl(ctl);
			}
			
			cid.signature = 'othe';
			
			cid.id = iNibOSaveFolder;
			HIViewFindByID(root, cid, &ctl);		
			menu = GetControlPopupMenuHandle(ctl);
			for (i = 1; i <= CountMenuItems(menu); i++)
				CheckMenuItem(menu, i, false);
			if (saveInROMFolder == 1)
			{
				CheckMenuItem(menu, 2, true);
				SetControl32BitValue(ctl, 2);
			}
			else
			if (saveInROMFolder == 0)
			{
				CheckMenuItem(menu, 1, true);
				SetControl32BitValue(ctl, 1);
			}
			else
			{
				CheckMenuItem(menu, 3, true);
				SetControl32BitValue(ctl, 3);
			}			

			cid.id = iNibOAutoSaveInterval;
			HIViewFindByID(root, cid, &ctl);
			sprintf(num, "%ld", (long) Settings.AutoSaveDelay);
			ConvertCString(num, numP);
			SetEditTextText(ctl, numP, false);

			cid.id = iNibOSPC7110Pack;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, mac7110Load);

			cid.id = iNibOSPC7110Megs;
			HIViewFindByID(root, cid, &ctl);
			sprintf(num, "%d", mac7110Megs);
			ConvertCString(num, numP);
			SetEditTextText(ctl, numP, false);

			cid.id = iNibOSDD1Pack;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, macSDD1Pack);

			cid.signature = 'msc2';

			cid.id = iNibMCPUCycles;
			HIViewFindByID(root, cid, &ctl);
			sprintf(num, "%ld", Settings.CyclesPercentage);
			ConvertCString(num, numP);
			SetEditTextText(ctl, numP, false);
			
			cid.id = iNibMShutdownMaster;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.ShutdownMaster);
	
			cid.id = iNibMTurboSkipArrows;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, macFastForwardRate);
			SetControlAction(ctl, actionUPP);

			cid.id = iNibMTurboSkipText;
			HIViewFindByID(root, cid, &ctl);
			sprintf(num, "%d", macFastForwardRate);
			ConvertCString(num, numP);
			SetStaticTextText(ctl, numP, false);

			cid.id = iNibMFrameSkip;
			HIViewFindByID(root, cid, &ctl);
			menu = GetControlPopupMenuHandle(ctl);
			for (i = 1; i <= CountMenuItems(menu); i++)
				CheckMenuItem(menu, i, false);
			CheckMenuItem(menu, macFrameSkip + 2, true);
			SetControl32BitValue(ctl, macFrameSkip + 2);

			cid.signature = 'osx_';
			
			cid.id = iNibXStartOpenDialog;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, startopendlog);
			
			cid.id = iNibXShowTimeInFrz;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, showtimeinfrz);
			
			cid.id = iNibXMusicBoxMode;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, musicboxmode);

			cid.id = iNibXEnableToggle;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, enabletoggle);

			cid.id = iNibXSaveWindowPos;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, savewindowpos);

			cid.id = iNibXUseIPSPatch;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, !Settings.NoPatch);

			cid.id = iNibXOnScreenInfo;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, onscreeninfo);

			cid.id = iNibXMinimizeCPUUsage;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, minimizecpu);

			cid.id = iNibXInactiveMode;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, inactiveMode);

			cid.id = iNibXBSXBootup;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, Settings.BSXBootup);

			MoveWindowPosition(tWindowRef, kWindowPreferences, false);
			ShowWindow(tWindowRef);
			err = RunAppModalLoopForWindow(tWindowRef);
			HideWindow(tWindowRef);
			SaveWindowPosition(tWindowRef, kWindowPreferences);
			
			cid.signature = 'grap';
			
			cid.id = iNibGFullScreen;
			HIViewFindByID(root, cid, &ctl);
			fullscreen = GetControl32BitValue(ctl) ? true : false;
	
			cid.id = iNibGSwitchResolution;
			HIViewFindByID(root, cid, &ctl);
			autoRes = GetControl32BitValue(ctl) ? true : false;
		
			cid.id = iNibGShowFrameRate;
			HIViewFindByID(root, cid, &ctl);
			Settings.DisplayFrameRate = GetControl32BitValue(ctl) ? true : false;
		
			cid.id = iNibGMode7Smoothing;
			HIViewFindByID(root, cid, &ctl);
			Settings.Mode7Interpolate = GetControl32BitValue(ctl) ? true : false;
			
			cid.id = iNibGTransparency;
			HIViewFindByID(root, cid, &ctl);
			Settings.Transparency = GetControl32BitValue(ctl) ? true : false;
			
			cid.id = iNibGGL32bit;
			HIViewFindByID(root, cid, &ctl);
			gl32bit = GetControl32BitValue(ctl) ? true : false;
			
			cid.id = iNibGGLStretch;
			HIViewFindByID(root, cid, &ctl);
			glstretch = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibGVideoMode;
			HIViewFindByID(root, cid, &ctl);
			switch (GetControl32BitValue(ctl))
			{
				case iDirectSmall:
					drawingMethod = kDrawingDirect;
					doubleSize    = false;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iDirectBlocky:
					drawingMethod = kDrawingDirect;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iDirectTVMode:
					drawingMethod = kDrawingDirect;
					doubleSize    = true;
					tvMode        = true;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iDirectSmoothMode:
					drawingMethod = kDrawingDirect;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = true;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iDirectEAGLEMode:
					drawingMethod = kDrawingDirect;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = true;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;
				
				case iDirect2xSAIMode:
					drawingMethod = kDrawingDirect;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = true;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;
					
				case iDirectSuper2xSAIMode:
					drawingMethod = kDrawingDirect;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = true;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iDirectEPXMode:
					drawingMethod = kDrawingDirect;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = true;
					hq2xMode	  = false;
					break;

				case iDirectHQ2xMode:
					drawingMethod = kDrawingDirect;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = true;
					break;

				case iOpenGLBlocky:
					drawingMethod = kDrawingOpenGL;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iOpenGLSmoothMode:
					drawingMethod = kDrawingOpenGL;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = true;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;
					
				case iOpenGLTVMode:
					drawingMethod = kDrawingBlitGL;
					doubleSize    = true;
					tvMode        = true;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iOpenGLEagleMode:
					drawingMethod = kDrawingBlitGL;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = true;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iOpenGL2xSAIMode:
					drawingMethod = kDrawingBlitGL;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = true;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = false;
					break;
					
				case iOpenGLSuper2xSAIMode:
					drawingMethod = kDrawingBlitGL;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = true;
					epxMode       = false;
					hq2xMode	  = false;
					break;

				case iOpenGLEPXMode:
					drawingMethod = kDrawingBlitGL;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = true;
					hq2xMode	  = false;
					break;

				case iOpenGLHQ2xMode:
					drawingMethod = kDrawingBlitGL;
					doubleSize    = true;
					tvMode        = false;
					smoothMode    = false;
					eagleMode     = false;
					saiMode       = false;
					supsaiMode    = false;
					epxMode       = false;
					hq2xMode	  = true;
			}
			
			cid.id = iNibGDirectMP;
			HIViewFindByID(root, cid, &ctl);
			multiprocessor = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibGGLVSync;
			HIViewFindByID(root, cid, &ctl);
			vsync = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibGDrawOverscan;
			HIViewFindByID(root, cid, &ctl);
			drawoverscan = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibGScreenCurvature;
			HIViewFindByID(root, cid, &ctl);
			screencurvature = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibGCurvatureWarp;
			HIViewFindByID(root, cid, &ctl);
			macCurvatureWarp = GetControl32BitValue(ctl);

			cid.id = iNibGAspectRatio;
			HIViewFindByID(root, cid, &ctl);
			macAspectRatio = GetControl32BitValue(ctl);

			cid.id = iNibGCIFilterEnable;
			HIViewFindByID(root, cid, &ctl);
			ciFilterEnable = GetControl32BitValue(ctl) ? true : false;

			cid.signature = 'snd_';
			
			cid.id = iNibSEnableSound;
			HIViewFindByID(root, cid, &ctl);
			Settings.APUEnabled = Settings.NextAPUEnabled = GetControl32BitValue(ctl) ? true : false;
	
			cid.id = iNibS16BitPlayback;
			HIViewFindByID(root, cid, &ctl);
			Settings.SixteenBitSound = GetControl32BitValue(ctl) ? true : false;
			
			cid.id = iNibSStereo;
			HIViewFindByID(root, cid, &ctl);
			Settings.Stereo = GetControl32BitValue(ctl) ? true : false;
		
			cid.id = iNibSReverseStereo;
			HIViewFindByID(root, cid, &ctl);
			Settings.ReverseStereo = GetControl32BitValue(ctl) ? true : false;
		
			cid.id = iNibSInterpolation;
			HIViewFindByID(root, cid, &ctl);
			Settings.InterpolatedSound = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibSHeightReading;
			HIViewFindByID(root, cid, &ctl);
			Settings.SoundEnvelopeHeightReading = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibSEcho;
			HIViewFindByID(root, cid, &ctl);
			Settings.DisableSoundEcho = GetControl32BitValue(ctl) ? false : true;

			cid.id = iNibSMasterVolume;
			HIViewFindByID(root, cid, &ctl);
			Settings.DisableMasterVolume = GetControl32BitValue(ctl) ? false : true;

			cid.id = iNibSPlaybackRate;
			HIViewFindByID(root, cid, &ctl);
			switch (GetControl32BitValue(ctl))
			{
				case 1:
					Settings.SoundPlaybackRate = 48000;
					break;
					
				case 2:
					Settings.SoundPlaybackRate = 44100;
					break;
					
				case 3:
					Settings.SoundPlaybackRate = 35000;
					break;
					
				case 4:
					Settings.SoundPlaybackRate = 32000;
					break;
					
				case 5:
					Settings.SoundPlaybackRate = 30000;
					break;
					
				case 6:
					Settings.SoundPlaybackRate = 22050;
					break;
					
				case 7:
					Settings.SoundPlaybackRate = 16000;
					break;
					
				case 8:
					Settings.SoundPlaybackRate = 11025;
					break;
					
				case 9:
					Settings.SoundPlaybackRate = 8000;
			}

			cid.id = iNibSSampleDecoder;
			HIViewFindByID(root, cid, &ctl);
			switch (GetControl32BitValue(ctl))
			{
				case 1:
					Settings.AltSampleDecode = 0;
					break;
				
				case 2:
					Settings.AltSampleDecode = 1;
					break;
					
				case 3:
					Settings.AltSampleDecode = 3;
			}
		
			cid.id = iNibSMixInterval;
			HIViewFindByID(root, cid, &ctl);
			GetEditTextText(ctl, numP);
			ConvertPString(numP, num);
			macSoundInterval = atoi(num);
			if (macSoundInterval < 0)
				macSoundInterval = 0;
			if (macSoundInterval > 30)
				macSoundInterval = 30;

			cid.id = iNibSPitch;
			HIViewFindByID(root, cid, &ctl);
			GetEditTextText(ctl, numP);
			ConvertPString(numP, num);
			macSoundPitch = (float) atof(num);
			
			cid.id = iNibSVolume;
			HIViewFindByID(root, cid, &ctl);
			macSoundVolume = GetControl32BitValue(ctl);
				
			cid.signature = 'othe';
			
			cid.id = iNibOSaveFolder;
			HIViewFindByID(root, cid, &ctl);
			int	saveto = GetControl32BitValue(ctl);
			if (saveto == 2)
				saveInROMFolder = 1;
			else
			if (saveto == 1)
				saveInROMFolder = 0;
			else
				saveInROMFolder = 3;

			cid.id = iNibOAutoSaveInterval;
			HIViewFindByID(root, cid, &ctl);
			GetEditTextText(ctl, numP);
			ConvertPString(numP, num);
			Settings.AutoSaveDelay = atoi(num);
			
			cid.id = iNibOSPC7110Pack;
			HIViewFindByID(root, cid, &ctl);
			mac7110Load = GetControl32BitValue(ctl);

			cid.id = iNibOSPC7110Megs;
			HIViewFindByID(root, cid, &ctl);
			GetEditTextText(ctl, numP);
			ConvertPString(numP, num);
			mac7110Megs = atoi(num);
			if (mac7110Megs < 1)
				mac7110Megs = 1;

			cid.id = iNibOSDD1Pack;
			HIViewFindByID(root, cid, &ctl);
			macSDD1Pack = GetControl32BitValue(ctl);

			cid.signature = 'msc2';
			
			cid.id = iNibMCPUCycles;
			HIViewFindByID(root, cid, &ctl);
			GetEditTextText(ctl, numP);
			ConvertPString(numP, num);
			Settings.CyclesPercentage = atoi(num);
			if ((Settings.CyclesPercentage <= 0) || (Settings.CyclesPercentage >= 200))
				Settings.CyclesPercentage = 100;
				
			cid.id = iNibMShutdownMaster;
			HIViewFindByID(root, cid, &ctl);
			Settings.ShutdownMaster = GetControl32BitValue(ctl) ? true : false;
			
			cid.id = iNibMTurboSkipArrows;
			HIViewFindByID(root, cid, &ctl);
			macFastForwardRate = GetControl32BitValue(ctl);

			cid.id = iNibMFrameSkip;
			HIViewFindByID(root, cid, &ctl);
			macFrameSkip = GetControl32BitValue(ctl) - 2;

			cid.signature = 'osx_';
			
			cid.id = iNibXStartOpenDialog;
			HIViewFindByID(root, cid, &ctl);
			startopendlog = GetControl32BitValue(ctl) ? true : false;
			
			cid.id = iNibXShowTimeInFrz;
			HIViewFindByID(root, cid, &ctl);
			showtimeinfrz = GetControl32BitValue(ctl) ? true : false;
			
			cid.id = iNibXMusicBoxMode;
			HIViewFindByID(root, cid, &ctl);
			musicboxmode = GetControl32BitValue(ctl);

			cid.id = iNibXEnableToggle;
			HIViewFindByID(root, cid, &ctl);
			enabletoggle = GetControl32BitValue(ctl) ? true : false;
			
			cid.id = iNibXSaveWindowPos;
			HIViewFindByID(root, cid, &ctl);
			savewindowpos = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibXUseIPSPatch;
			HIViewFindByID(root, cid, &ctl);
			Settings.NoPatch = GetControl32BitValue(ctl) ? false : true;

			cid.id = iNibXOnScreenInfo;
			HIViewFindByID(root, cid, &ctl);
			onscreeninfo = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibXMinimizeCPUUsage;
			HIViewFindByID(root, cid, &ctl);
			minimizecpu = GetControl32BitValue(ctl) ? true : false;

			cid.id = iNibXInactiveMode;
			HIViewFindByID(root, cid, &ctl);
			inactiveMode = GetControl32BitValue(ctl);

			cid.id = iNibXBSXBootup;
			HIViewFindByID(root, cid, &ctl);
			Settings.BSXBootup = GetControl32BitValue(ctl) ? true : false;

			err = RemoveEventHandler(prefHandlerRef);
			DisposeEventHandlerUPP(prefEventProc);
			
			err = RemoveEventHandler(tabHandlerRef);
			DisposeEventHandlerUPP(tabEventProc);
			
			DisposeControlActionUPP(actionUPP);
			
			ReleaseWindow(tWindowRef);
		}

		DisposeNibReference(nibRef);
		
		S9xGraphicsInit();
		
		macSoundPitchMaster = macSoundPitch;
		
		switch (mac7110Load)
		{
			case 1:
				LoadUp7110 = &SPC7110Load;
				break;
			
			case 2:
				LoadUp7110 = &SPC7110Grab;
				break;
				
			case 3:
				LoadUp7110 = &SPC7110Open;
		}
		
		cacheMegs = mac7110Megs;

		if (((old16BitPlayback != Settings.SixteenBitSound  ) ||
			 (oldStereo        != Settings.Stereo           ) ||
		     (oldPlaybackRate  != Settings.SoundPlaybackRate) ||
			 (oldMixInterval   != macSoundInterval          ) ||
			 (oldVolume        != macSoundVolume            )) && cartOpen)
			SNES9X_InitSound();
		
		if ((oldEnableSound != Settings.APUEnabled) && cartOpen)
			AppearanceAlert(kAlertCautionAlert, kSoundWarning, kSoundHint);

		if (!fullscreen && cartOpen)
		{
			InitGameWindow();
			ShowWindow(gWindow);
		}
	}
}

static void SelectTabPane(HIViewRef tabControl, SInt16 index)
{
	HIViewRef	sup, userPane, selectedPane = nil;
	HIViewID	cid;
	
	lastTabIndex = index;

	sup = HIViewGetSuperview(tabControl);
	cid.signature = 'tabs';
	
	for (int i = 1; i < tabList[0] + 1; i++)
	{
		cid.id = tabList[i];
		HIViewFindByID(sup, cid, &userPane);
		
		if (i == index)
			selectedPane = userPane;
		else
			HIViewSetVisible(userPane, false);
	}
	
	if (selectedPane != nil)
		HIViewSetVisible(selectedPane, true);
	
	HIViewSetNeedsDisplay(tabControl, true);
}

static pascal void LittleArrowsActionProc(HIViewRef arrows, HIViewPartCode partCode)
{
	HIViewRef		ctl;
	char			num[8];
	unsigned char	numP[8];

	const HIViewID	cid = { 'msc2', iNibMTurboSkipText };
	
	if (partCode == kControlUpButtonPart)
		SetControl32BitValue(arrows, GetControl32BitValue(arrows) + 1);
	else
	if (partCode == kControlDownButtonPart)
		SetControl32BitValue(arrows, GetControl32BitValue(arrows) - 1);

	HIViewFindByID(HIViewGetSuperview(arrows), cid, &ctl);
	sprintf(num, "%ld", GetControl32BitValue(arrows));
	ConvertCString(num, numP);
	SetStaticTextText(ctl, numP, true);
}

static pascal OSStatus TabEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerRef, inUserData)
	
	OSStatus	result = eventNotHandledErr;
	HIViewRef	ctl;
	HIViewID	cid;
	SInt32		value;
	
	GetEventParameter(inEvent, kEventParamDirectObject, typeControlRef, nil, sizeof(ControlRef), nil, &ctl);
	GetControlID(ctl, &cid);
	value = GetControl32BitValue(ctl);
	
	if ((cid.id == 128) && (value != lastTabIndex))
	{
		SelectTabPane(ctl, value);
		result = noErr;
	}    
	
	return result;
}

static pascal OSStatus PreferencesEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerRef)
	
	OSStatus	err, result = eventNotHandledErr;
	WindowRef	tWindowRef = (WindowRef) inUserData;

	switch (GetEventClass(inEvent))
	{
		case kEventClassWindow:
			switch (GetEventKind(inEvent))
			{
				case kEventWindowClose:
					QuitAppModalLoopForWindow(tWindowRef);
					result = noErr;
			}
			
			break;

		case kEventClassCommand:
			switch (GetEventKind(inEvent))
			{
				case kEventCommandProcess:
					HICommand	tHICommand;
					HIViewRef	ctl, root;
					HIViewID	cid;
					SInt32		value, value2;
					
					root = HIViewGetRoot(tWindowRef);

					err = GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, nil, sizeof(HICommand), nil, &tHICommand);
					if (err == noErr)
					{
						switch (tHICommand.commandID)
						{
							case 'S_EF':
								HideWindow(tWindowRef);
								ConfigureSoundEffects();
								ShowWindow(tWindowRef);

								result = noErr;
								break;
						
						#ifdef MAC_COREIMAGE_SUPPORT
							case 'G_FL':
								if (systemVersion >= 0x1040)
								{
									HideWindow(tWindowRef);
									ConfigureCoreImageFilter();
									ShowWindow(tWindowRef);
								}

								result = noErr;
								break;
						#endif
								
							case 'G__7':
								cid.signature = 'grap';
								cid.id        = iNibGGLStretch;
								HIViewFindByID(root, cid, &ctl);
								value = GetControl32BitValue(ctl);
								
								cid.id        = iNibGAspectRatio;
								HIViewFindByID(root, cid, &ctl);
								if (value)
									ActivateControl(ctl);
								else
									DeactivateControl(ctl);
								
								result = noErr;
								break;

							case 'G_13':
								cid.signature = 'grap';
								cid.id        = iNibGScreenCurvature;
								HIViewFindByID(root, cid, &ctl);
								value = GetControl32BitValue(ctl);
								
								cid.id        = iNibGCurvatureWarp;
								HIViewFindByID(root, cid, &ctl);
								if (value)
									ActivateControl(ctl);
								else
									DeactivateControl(ctl);
								
								result = noErr;
								break;
								
							case 'S__3':
								cid.signature = 'snd_';
								cid.id        = iNibSStereo;
								HIViewFindByID(root, cid, &ctl);
								value = GetControl32BitValue(ctl);
								
								cid.id        = iNibSReverseStereo;
								HIViewFindByID(root, cid, &ctl);
								if (value)
									ActivateControl(ctl);
								else
									DeactivateControl(ctl);
								
								result = noErr;
								break;
								
							case 'S__1':
								cid.signature = 'snd_';
								cid.id        = iNibSEnableSound;
								HIViewFindByID(root, cid, &ctl);
								value = GetControl32BitValue(ctl);
								
								for (int i = 0; i < kGroupListSize1; i++)
								{
									cid.id = grouplist1[i];
									HIViewFindByID(root, cid, &ctl);
									if (value)
										ActivateControl(ctl);
									else
										DeactivateControl(ctl);
								}
								
								cid.id = iNibSStereo;
								HIViewFindByID(root, cid, &ctl);
								value2 = GetControl32BitValue(ctl);
									
								cid.id = iNibSReverseStereo;
								HIViewFindByID(root, cid, &ctl);
								if (value && value2)
									ActivateControl(ctl);
								else
									DeactivateControl(ctl);

								result = noErr;
						}
					}
			}
	}

	return result;
}

void ConfigureExtraOptions(void)
{
	OSStatus	err;
	IBNibRef	nibRef;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		WindowRef	tWindowRef;

		err = CreateWindowFromNib(nibRef, CFSTR("ExtraOptions"), &tWindowRef);
		if (err == noErr)
		{
			EventHandlerRef		eref;
			EventHandlerUPP		eventUPP;
			EventTypeSpec		windowEvents[] = { { kEventClassWindow, kEventWindowClose } };
			HIViewRef			ctl, root;
			HIViewID			cid;
			MenuRef				menu;

			root = HIViewGetRoot(tWindowRef);
			cid.id = 0;

			cid.signature = 'BMrk';
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, extraOptions.benchmark);

			cid.signature = 'NoTR';
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, extraOptions.glForceNoTextureRectangle);

			cid.signature = 'CSAp';
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, extraOptions.glUseClientStrageApple);

			cid.signature = 'AGPT';
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, extraOptions.glUseTexturePriority);

			cid.signature = 'Hint';
			HIViewFindByID(root, cid, &ctl);
			menu = GetControlPopupMenuHandle(ctl);
			for (int i = 1; i <= CountMenuItems(menu); i++)
				CheckMenuItem(menu, i, false);
			CheckMenuItem(menu, extraOptions.glStorageHint, true);
			SetControl32BitValue(ctl, extraOptions.glStorageHint);

			eventUPP = NewEventHandlerUPP(DefaultEventHandler);
			err = InstallWindowEventHandler(tWindowRef, eventUPP, GetEventTypeCount(windowEvents), windowEvents, (void *) tWindowRef, &eref);

			MoveWindowPosition(tWindowRef, kWindowExtra, false);
			ShowWindow(tWindowRef);
			err = RunAppModalLoopForWindow(tWindowRef);
			HideWindow(tWindowRef);
			SaveWindowPosition(tWindowRef, kWindowExtra);

			cid.signature = 'BMrk';
			HIViewFindByID(root, cid, &ctl);
			extraOptions.benchmark = GetControl32BitValue(ctl) ? true : false;

			cid.signature = 'NoTR';
			HIViewFindByID(root, cid, &ctl);
			extraOptions.glForceNoTextureRectangle = GetControl32BitValue(ctl) ? true : false;

			cid.signature = 'CSAp';
			HIViewFindByID(root, cid, &ctl);
			extraOptions.glUseClientStrageApple = GetControl32BitValue(ctl) ? true : false;

			cid.signature = 'AGPT';
			HIViewFindByID(root, cid, &ctl);
			extraOptions.glUseTexturePriority = GetControl32BitValue(ctl) ? true : false;

			cid.signature = 'Hint';
			HIViewFindByID(root, cid, &ctl);
			extraOptions.glStorageHint = GetControl32BitValue(ctl);

			err = RemoveEventHandler(eref);
			DisposeEventHandlerUPP(eventUPP);
			
			ReleaseWindow(tWindowRef);
		}
		
		DisposeNibReference(nibRef);
	}
}
