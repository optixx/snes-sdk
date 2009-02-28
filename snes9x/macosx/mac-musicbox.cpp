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
#include "apu.h"
#include "cpuexec.h"
#include "snapshot.h"
#include "soundux.h"

#include <unistd.h>
#include <pthread.h>

#include "mac-prefix.h"
#include "mac-audio.h"
#include "mac-dialog.h"
#include "mac-file.h"
#include "mac-os.h"
#include "mac-musicbox.h"
#include "mac-stringtools.h"

const float	mbxOffsetX   = 0.0,
			mbxOffsetY   = 0.0,	// 0.5
			mbxBarWidth  = 12.0,
			mbxBarHeight = 128.0,
			mbxBarSpace  = 2.0,
			mbxLRSpace   = 20.0,
			mbxRightBarX = (mbxLRSpace + (mbxBarWidth * 8.0 + mbxBarSpace * 7.0)),
			yyscale      = (128.0 / (sqrt(64.0)));

extern char gMacRomName[ROM_NAME_LEN];

volatile Boolean	mboxPause = false;

static void SPCPlayExec(void);
static void SPCPlayFreeze(void);
static void SPCPlayDefrost(void);
static void MusicBoxForceFreeze(void);
static void MusicBoxForceDefrost(void);
static void MusicBoxInitIndicator(void);
static void MusicBoxDrawIndicator(HIViewRef, CGContextRef);
static void * SoundTask(void *);
static pascal void MusicBoxTimerHandler(EventLoopTimerRef, void *);
static pascal OSStatus MusicBoxEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus IndicatorEventHandler(EventHandlerCallRef, EventRef, void *);

static volatile Boolean	stopNow, showIndicator, mbxFinished;

static pthread_t		mbxThread;

static short			mbxOpenedHeight, mbxClosedHeight;
static float			mbxMarginX, mbxMarginY, mbxViewWidth, mbxViewHeight;

static SAPU				*StoredAPU;
static SAPURegisters	*StoredAPURegisters;
static SSoundData		*StoredSoundData;
static uint8			*StoredIAPURAM;
static int32			oldNextAPUTimerPos, oldAPUTimerCounter;

static short			prevLMax[8], prevRMax[8];
static short			prevLVol[8], prevRVol[8];
static long long		barTimeL[8], barTimeR[8];

void MusicBoxDialog(void)
{
	OSStatus	err;
	IBNibRef	nibRef;

	if (!cartOpen)
		return;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		CFURLRef	iconURL;
		FSRef		iconFSRef;
		IconRef		actIcon;
		WindowRef	tWindowRef;
		
		actIcon = nil;

		if (musicboxmode == kMBXSoundEmulation)
			iconURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("musicbox_ledoff"), CFSTR("icns"), nil);
		else
			iconURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("musicbox_ledon" ), CFSTR("icns"), nil);

		if (iconURL)
		{
			if (CFURLGetFSRef(iconURL, &iconFSRef))
				err = RegisterIconRefFromFSRef('~9X~', 'micn', &iconFSRef, &actIcon);
					
			CFRelease(iconURL);
		}

		err = CreateWindowFromNib(nibRef, CFSTR("MusicBox"), &tWindowRef);
		if (err == noErr)
		{
			EventHandlerRef		mboxRef, paneRef;
			EventHandlerUPP		mboxUPP, paneUPP;
			EventLoopTimerRef	timeRef;
			EventLoopTimerUPP	timeUPP;
			EventTypeSpec		mboxEvents[] = { { kEventClassCommand, kEventCommandProcess } },
								paneEvents[] = { { kEventClassControl, kEventControlDraw    } };
			CFStringRef			sref;
			CGDataProviderRef	prov;
			CGImageRef			ipng;
			CFURLRef			iurl;
			HIViewRef			ctl, root, paneView, imageView, contentView;
			HIViewID			cid;
			HIRect				bounds;
			Rect				windowRect, barRect;
			char				drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

			mboxPause = false;
			mbxFinished = false;
			showIndicator = false;
			so.stereo_switch = ~0;

			for (int i = 0; i < MAC_MAX_PLAYERS; i++)
				controlPad[i] = 0;

			switch (drawingMethod)
			{
				case kDrawingOpenGL:
					Settings.OpenGLEnable = true;
					break;
					
				case kDrawingDirect:
				case kDrawingBlitGL:
					Settings.OpenGLEnable = false;
			}
			
			// 107's enhanced SPC player
			
			root = HIViewGetRoot(tWindowRef);
			cid.id = 0;

			if (musicboxmode == kMBXSoundEmulation)
			{
				cid.signature = 'HEAD';
				HIViewFindByID(root, cid, &ctl);
				EnableControl(ctl);

				StoredAPU          = new SAPU;
				StoredAPURegisters = new SAPURegisters;
				StoredSoundData    = new SSoundData;
				StoredIAPURAM      = new uint8 [0x10000];
				
				SPCPlayFreeze();
			}
			else
				MusicBoxForceFreeze();

			cid.signature = 'Kart';
			HIViewFindByID(root, cid, &ctl);
			SetStaticTextTrunc(ctl, truncEnd, false);	
			_splitpath(Memory.ROMFilename, drive, dir, fname, ext);
			sref = CFStringCreateWithCString(kCFAllocatorDefault, fname, MAC_PATH_ENCODING);
			if (sref)
			{
				SetStaticTextCFString(ctl, sref, false);
				CFRelease(sref);
			}
			
			ipng = nil;
			
			iurl = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("musicbox_indicator"), CFSTR("png"), nil);
			if (iurl)
			{
				prov = CGDataProviderCreateWithURL(iurl);
				if (prov)
				{
					ipng = CGImageCreateWithPNGDataProvider(prov, nil, false, kCGRenderingIntentDefault);
					CGDataProviderRelease(prov);
				}
				
				CFRelease(iurl);
			}
			
			imageView = nil;

			if (ipng)
			{
				HIViewFindByID(root, kHIViewWindowContentID, &contentView);

				err = HIImageViewCreate(ipng, &imageView);
				if (err == noErr)
				{
					bounds = CGRectMake(30, 64, CGImageGetWidth(ipng), CGImageGetHeight(ipng));
					HIViewSetFrame(imageView, &bounds);
					HIImageViewSetOpaque(imageView, false);
					HIViewSetVisible(imageView, true);
					HIViewAddSubview(contentView, imageView);
					cid.signature = 'iMaG';
					SetControlID(imageView, &cid);
				}

				CGImageRelease(ipng);
			}

			cid.signature = 'Pane';
			HIViewFindByID(root, cid, &paneView);
			HIViewGetBounds(paneView, &bounds);
			mbxViewWidth  = bounds.size.width;
			mbxViewHeight = bounds.size.height;
			mbxMarginY = (mbxViewHeight - mbxBarHeight) / 2.0;
			mbxMarginX = (mbxViewWidth - ((mbxBarWidth * 8.0 + mbxBarSpace * 7.0) * 2.0 + mbxLRSpace)) / 2.0;

			if (imageView)
			{
				HIViewSetZOrder(imageView, kHIViewZOrderBelow, paneView);
				HIViewAddSubview(imageView, paneView);
			}

			cid.signature = 'Tr_i';
			HIViewFindByID(root, cid, &ctl);
			HIViewGetFrame(ctl, &bounds);
			GetWindowBounds(tWindowRef, kWindowTitleBarRgn, &barRect);
			mbxClosedHeight = (short) (bounds.origin.y + bounds.size.height + 7.0) + (barRect.bottom - barRect.top);

			GetWindowBounds(tWindowRef, kWindowStructureRgn, &windowRect);
			mbxOpenedHeight = windowRect.bottom - windowRect.top;
			
			windowRect.bottom = windowRect.top + mbxClosedHeight;
			SetWindowBounds(tWindowRef, kWindowStructureRgn, &windowRect);

			paneUPP = NewEventHandlerUPP(IndicatorEventHandler);
			err = InstallControlEventHandler(paneView, paneUPP, GetEventTypeCount(paneEvents), paneEvents, (void *) paneView, &paneRef);

			mboxUPP = NewEventHandlerUPP(MusicBoxEventHandler);
			err = InstallWindowEventHandler(tWindowRef, mboxUPP, GetEventTypeCount(mboxEvents), mboxEvents, (void *) tWindowRef, &mboxRef);
			
			timeUPP = NewEventLoopTimerUPP(MusicBoxTimerHandler);
			err = InstallEventLoopTimer(GetCurrentEventLoop(), kEventDurationNoWait, kEventDurationSecond * 2.0 / (double) Memory.ROMFramesPerSecond, timeUPP, (void *) paneView, &timeRef);

			MusicBoxInitIndicator();
			
			stopNow = false;
			MacStartSound();
			pthread_create(&mbxThread, nil, SoundTask, nil);

			MoveWindowPosition(tWindowRef, kWindowMusicBox, true);
			GetWindowBounds(tWindowRef, kWindowStructureRgn, &windowRect);
			if (windowRect.bottom - windowRect.top > mbxClosedHeight)
			{
				showIndicator = true;
				SetControl32BitValue(ctl, 1);	// Tr_i
			}

			ShowWindow(tWindowRef);
			err = RunAppModalLoopForWindow(tWindowRef);
			HideWindow(tWindowRef);

			SaveWindowPosition(tWindowRef, kWindowMusicBox);

			stopNow = true;
			pthread_join(mbxThread, nil);
			MacStopSound();
			
			err = RemoveEventLoopTimer(timeRef);
			DisposeEventLoopTimerUPP(timeUPP);
			
			err = RemoveEventHandler(mboxRef);
			DisposeEventHandlerUPP(mboxUPP);
			
			err = RemoveEventHandler(paneRef);
			DisposeEventHandlerUPP(paneUPP);

			ReleaseWindow(tWindowRef);

			so.stereo_switch = ~0;

			mbxFinished = true;

			if (musicboxmode == kMBXSoundEmulation)
			{
 				SPCPlayDefrost();

				delete    StoredAPU;
				delete    StoredAPURegisters;
				delete    StoredSoundData;
				delete [] StoredIAPURAM;				
			}
			else
				MusicBoxForceDefrost();

			Settings.OpenGLEnable = false;
		}

		if (actIcon)
			err = UnregisterIconRef('~9X~', 'micn');

		DisposeNibReference(nibRef);
	}
}

static pascal OSStatus MusicBoxEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *userData)
{
    #pragma unused (inHandlerCallRef)
    
    OSStatus	err, result = eventNotHandledErr;
	WindowRef	tWindowRef = (WindowRef) userData;

	switch (GetEventClass(inEvent))
	{
		case kEventClassCommand:
			switch (GetEventKind(inEvent))
			{
				case kEventCommandProcess:
					HICommand	cmd;

					err = GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, nil, sizeof(HICommand), nil, &cmd);
					if (err == noErr)
					{
						HIViewRef	root, c1, c2, c3, c4;
						HIViewID	cid;
						Rect		rct;

						switch (cmd.commandID)
						{
							case 'bar1': so.stereo_switch ^= (1 <<  0);	result = noErr;	break;
							case 'bar2': so.stereo_switch ^= (1 <<  1);	result = noErr;	break;
							case 'bar3': so.stereo_switch ^= (1 <<  2);	result = noErr;	break;
							case 'bar4': so.stereo_switch ^= (1 <<  3);	result = noErr;	break;
							case 'bar5': so.stereo_switch ^= (1 <<  4);	result = noErr;	break;
							case 'bar6': so.stereo_switch ^= (1 <<  5);	result = noErr;	break;
							case 'bar7': so.stereo_switch ^= (1 <<  6);	result = noErr;	break;
							case 'bar8': so.stereo_switch ^= (1 <<  7);	result = noErr;	break;
							case 'bar9': so.stereo_switch ^= (1 <<  8);	result = noErr;	break;
							case 'bara': so.stereo_switch ^= (1 <<  9);	result = noErr;	break;
							case 'barb': so.stereo_switch ^= (1 << 10);	result = noErr;	break;
							case 'barc': so.stereo_switch ^= (1 << 11);	result = noErr;	break;
							case 'bard': so.stereo_switch ^= (1 << 12);	result = noErr;	break;
							case 'bare': so.stereo_switch ^= (1 << 13);	result = noErr;	break;
							case 'barf': so.stereo_switch ^= (1 << 14);	result = noErr;	break;
							case 'bar0': so.stereo_switch ^= (1 << 15);	result = noErr;	break;

							case 'Paus': 
								mboxPause = !mboxPause;
								S9xSetSoundMute(mboxPause);
								result = noErr;
								break;

							case 'Tr_i':
								showIndicator = !showIndicator;
								
								root = HIViewGetRoot(tWindowRef);
								cid.id = 0;
								
								cid.signature = 'Pane';
								HIViewFindByID(root, cid, &c1);
								HIViewSetVisible(c1, false);
								cid.signature = 'iMaG';
								HIViewFindByID(root, cid, &c2);
								HIViewSetVisible(c2, false);
								cid.signature = 'rCTL';
								HIViewFindByID(root, cid, &c3);
								HIViewSetVisible(c3, false);
								cid.signature = 'bCTL';
								HIViewFindByID(root, cid, &c4);
								HIViewSetVisible(c4, false);
																
								GetWindowBounds(tWindowRef, kWindowStructureRgn,  &rct);
								rct.bottom = rct.top + (showIndicator ? mbxOpenedHeight : mbxClosedHeight);
								
								err = TransitionWindow(tWindowRef, kWindowSlideTransitionEffect, kWindowResizeTransitionAction, &rct);
								
								HIViewSetVisible(c1, true);
								HIViewSetVisible(c2, true);
								HIViewSetVisible(c3, true);
								HIViewSetVisible(c4, true);
								
								result = noErr;
								break;
						
							case 'ok  ':
								QuitAppModalLoopForWindow(tWindowRef);
								result = noErr;
								break;

							case 'HEAD':
								showIndicator = !showIndicator;
								SPCPlayDefrost();
								showIndicator = !showIndicator;
								result = noErr;
								break;
								
							case 'S_EF':
								HideWindow(tWindowRef);
								showIndicator = !showIndicator;
								ConfigureSoundEffects();
								showIndicator = !showIndicator;
								ShowWindow(tWindowRef);
								result = noErr;
						}
					}
			}
	}
	
	return result;
}

static void * SoundTask(void *)
{
	long long			curt;
	static long long	last;
	
	Microseconds((UnsignedWide *) &last);
	
	while (!stopNow)
	{
		if (!mboxPause)
		{
			if (musicboxmode == kMBXSoundEmulation)
				SPCPlayExec();
			else
				S9xMainLoop();
		}
		
		last += (1000000 / Memory.ROMFramesPerSecond);
		Microseconds((UnsignedWide *) &curt);
		
		if (last > curt)
			usleep(last - curt);
	}
	
	return nil;
}

static void SPCPlayExec(void)
{
	for (int v = 0; v < Timings.V_Max; v++)
	{	
		while (APU.Cycles < Timings.H_Max)
		{
			S9xUpdateAPUTimer();
			APU_EXECUTE1();
		}

		APU.Cycles -= Timings.H_Max;
		IAPU.NextAPUTimerPos -= (Timings.H_Max << SNES_APUTIMER_ACCURACY);
	}
	
	APURegisters.PC = IAPU.PC - IAPU.RAM;
    S9xAPUPackStatus();
}

static void MusicBoxForceFreeze(void)
{
	char	filename[PATH_MAX + 1];
	
	strcpy(filename, S9xGetFreezeFilename(999));
	strcat(filename, ".tmp");
	
	S9xFreezeGame(filename);
}

static void MusicBoxForceDefrost(void)
{
	char	filename[PATH_MAX + 1];
	
	strcpy(filename, S9xGetFreezeFilename(999));
	strcat(filename, ".tmp");
	
	S9xUnfreezeGame(filename);
	remove(filename);
}

static void SPCPlayFreeze(void)
{
	S9xSetSoundMute(true);
	
	oldNextAPUTimerPos = IAPU.NextAPUTimerPos;
	oldAPUTimerCounter = IAPU.APUTimerCounter;
		
	for (int i = 0; i < 8; i++)
    {
		SoundData.channels[i].previous16[0] = (int16) SoundData.channels[i].previous[0];
		SoundData.channels[i].previous16[1] = (int16) SoundData.channels[i].previous[1];
    }

	memcpy(StoredAPU,          &APU,          sizeof(SAPU));
	memcpy(StoredAPURegisters, &APURegisters, sizeof(SAPURegisters));
	memcpy(StoredSoundData,    &SoundData,    sizeof(SSoundData));
	memcpy(StoredIAPURAM,      IAPU.RAM,      0x10000);

	S9xSetSoundMute(false);
}

static void SPCPlayDefrost(void)
{
	uint16	savedswitch;
	short   i;
	
	savedswitch = so.stereo_switch;
	
	S9xResetAPU();
    S9xSetSoundMute(true);
	
	memcpy(&APU,          StoredAPU,          sizeof(SAPU));
	memcpy(&APURegisters, StoredAPURegisters, sizeof(SAPURegisters));
	memcpy(&SoundData,    StoredSoundData,    sizeof(SSoundData));
	memcpy(IAPU.RAM,      StoredIAPURAM,      0x10000);
	
	if (!mbxFinished)
	{
		for (i = 0; i < 4; i++)
			APU.OutPorts[i] = IAPU.RAM[0xf4 + i];
		
		for (i = 0; i < 3; i++)
		{
			if (IAPU.RAM[0xfa + i] == 0)
				APU.TimerTarget[i] = 0x100;
			else
				APU.TimerTarget[i] = IAPU.RAM[0xfa + i];
		}

		S9xSetAPUControl(IAPU.RAM[0xf1] & 0xcf);   // Don't reset I/O ports
	}

	S9xSetSoundMute(false);

	IAPU.PC = IAPU.RAM + APURegisters.PC;
	S9xAPUUnpackStatus();
	
	if (APUCheckDirectPage())
	    IAPU.DirectPage = IAPU.RAM + 0x100;
	else
	    IAPU.DirectPage = IAPU.RAM;

	IAPU.APUExecuting = true;

	IAPU.NextAPUTimerPos = oldNextAPUTimerPos;
	IAPU.APUTimerCounter = oldAPUTimerCounter;

	S9xFixSoundAfterSnapshotLoad(SNAPSHOT_VERSION);
	
	so.stereo_switch = savedswitch;
}

static void MusicBoxInitIndicator(void)
{
	long long	currentTime;
	
	Microseconds((UnsignedWide *) &currentTime);
	
	for (int i = 0; i < 8; i++)
	{
		prevLMax[i] = prevRMax[i] = 0;
		prevLVol[i] = prevRVol[i] = 0;
		barTimeL[i] = barTimeR[i] = currentTime;
	}
}

static pascal void MusicBoxTimerHandler(EventLoopTimerRef inHandlerCallRef, void *userData)
{
	#pragma unused (inHandlerCallRef)

	OSStatus	err;

	if (showIndicator)
		err = HIViewSetNeedsDisplay((HIViewRef) userData, true);
}

static pascal OSStatus IndicatorEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *userData)
{
    #pragma unused (inHandlerCallRef)
    
    OSStatus	err, result = eventNotHandledErr;
	HIViewRef	view = (HIViewRef) userData;

	switch (GetEventClass(inEvent))
	{
		case kEventClassControl:
			switch (GetEventKind(inEvent))
			{
				case kEventControlDraw:
					CGContextRef	ctx;

					err = GetEventParameter(inEvent, kEventParamCGContextRef, typeCGContextRef, nil, sizeof(CGContextRef), nil, &ctx);
					if (err == noErr)
					{
						HIRect	bounds;

						HIViewGetBounds(view, &bounds);
						CGContextTranslateCTM(ctx, 0, bounds.size.height);
						CGContextScaleCTM(ctx, 1.0, -1.0);
						MusicBoxDrawIndicator(view, ctx);
						
						result = noErr;
					}
			}
	}
	
	return result;
}

static void MusicBoxDrawIndicator(HIViewRef view, CGContextRef mboxctx)
{
	if (!showIndicator)
		return;

	// Bar

	const float	length[] = { 1.0, 1.0 };
	
	CGContextSetLineWidth(mboxctx, mbxBarWidth);
	CGContextSetLineDash(mboxctx, 0, length, 2);
	CGContextSetLineJoin(mboxctx, kCGLineJoinMiter);

	CGContextBeginPath(mboxctx);
	
	float   x = mbxOffsetX + mbxMarginX + mbxBarWidth / 2.0;
	for (int h = 0; h < 8; h++)
	{
		// Inactive
		
		CGContextSetRGBStrokeColor(mboxctx, (196.0 / 256.0), (200.0 / 256.0), (176.0 / 256.0), 1.0);
		
		CGContextMoveToPoint   (mboxctx, x,                mbxOffsetY + mbxMarginY);
		CGContextAddLineToPoint(mboxctx, x,                mbxOffsetY + mbxMarginY + mbxBarHeight);
		
		CGContextMoveToPoint   (mboxctx, x + mbxRightBarX, mbxOffsetY + mbxMarginY);
		CGContextAddLineToPoint(mboxctx, x + mbxRightBarX, mbxOffsetY + mbxMarginY + mbxBarHeight);
	
		CGContextStrokePath(mboxctx);
		
		// Max
		
		Channel	*ch = &SoundData.channels[h];
		
		ch->envx = ch->xenvx >> 4;
		ch-> left_vol_level = (ch->xenvx * ch->volume_left ) >> 11;
		ch->right_vol_level = (ch->xenvx * ch->volume_right) >> 11;

		short		vl = ch-> left_vol_level;
		short		vr = ch->right_vol_level;
		long long	currentTime;
		
		if (vl <= 0) vl = 0; else if (vl > 64) vl = 64; else vl = (short) (yyscale * sqrt((double) vl)) & (~0 << 1);
		if (vr <= 0) vr = 0; else if (vr > 64) vr = 64; else vr = (short) (yyscale * sqrt((double) vr)) & (~0 << 1);
	
		if (vl < prevLVol[h]) vl = ((prevLVol[h] + vl) >> 1);
		if (vr < prevRVol[h]) vr = ((prevRVol[h] + vr) >> 1);

		Microseconds((UnsignedWide *) &currentTime);
		
		// left

		if ((vl >= prevLMax[h]) && (vl > prevLVol[h]))
		{
			barTimeL[h] = currentTime;
			prevLMax[h] = vl;
		}
		else
		if ((prevLMax[h] > 0) && (barTimeL[h] + 1000000 > currentTime))
		{
			CGContextSetRGBStrokeColor(mboxctx, (22.0 / 256.0), (156.0 / 256.0), (20.0 / 256.0), (float) (barTimeL[h] + 1000000 - currentTime) / 1000000.0);
			
			CGContextMoveToPoint   (mboxctx, x, mbxOffsetY + mbxMarginY + (float) (prevLMax[h] - 2));
			CGContextAddLineToPoint(mboxctx, x, mbxOffsetY + mbxMarginY + (float) (prevLMax[h]    ));
			
			CGContextStrokePath(mboxctx);
		}
		else
			prevLMax[h] = 0;
		
		prevLVol[h] = vl;
		
		// right
		
		if ((vr >= prevRMax[h]) && (vr > prevRVol[h]))
		{
			barTimeR[h] = currentTime;
			prevRMax[h] = vr;
		}
		else
		if ((prevRMax[h] > 0) && (barTimeR[h] + 1000000 > currentTime))
		{
			CGContextSetRGBStrokeColor(mboxctx, (22.0 / 256.0), (156.0 / 256.0), (20.0 / 256.0), (float) (barTimeR[h] + 1000000 - currentTime) / 1000000.0);
			
			CGContextMoveToPoint   (mboxctx, x + mbxRightBarX, mbxOffsetY + mbxMarginY + (float) (prevRMax[h] - 2));
			CGContextAddLineToPoint(mboxctx, x + mbxRightBarX, mbxOffsetY + mbxMarginY + (float) (prevRMax[h]    ));
			
			CGContextStrokePath(mboxctx);
		}
		else
			prevRMax[h] = 0;
		
		prevRVol[h] = vr;

		// Active
		
		CGContextSetRGBStrokeColor(mboxctx, (22.0 / 256.0), (22.0 / 256.0), (20.0 / 256.0), 1.0);
		
		CGContextMoveToPoint   (mboxctx, x,                mbxOffsetY + mbxMarginY);
		CGContextAddLineToPoint(mboxctx, x,                mbxOffsetY + mbxMarginY + (float) vl);
		CGContextStrokePath(mboxctx);
		CGContextMoveToPoint   (mboxctx, x + mbxRightBarX, mbxOffsetY + mbxMarginY);
		CGContextAddLineToPoint(mboxctx, x + mbxRightBarX, mbxOffsetY + mbxMarginY + (float) vr);
		CGContextStrokePath(mboxctx);
		
		x += (mbxBarWidth + mbxBarSpace);
	}
}
