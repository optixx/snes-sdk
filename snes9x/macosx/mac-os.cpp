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
#include "movie.h"
#include "gfx.h"
#include "apu.h"
#include "cpuexec.h"
#include "cheats.h"
#include "controls.h"
#include "crosshairs.h"
#include "display.h"
#include "ppu.h"
#include "soundux.h"
#include "spc7110.h"

#ifdef DEBUGGER
#include "debug.h"
#endif

#include <QuickTime/QuickTime.h>
#include <unistd.h>
#include <pthread.h>

#include "mac-prefix.h"
#include "mac-appleevent.h"
#include "mac-audio.h"
#include "mac-cheat.h"
#include "mac-cheatfinder.h"
#include "mac-controls.h"
#include "mac-dialog.h"
#include "mac-file.h"
#include "mac-gworld.h"
#include "mac-joypad.h"
#include "mac-keyboard.h"
#include "mac-musicbox.h"
#include "mac-prefs.h"
#include "mac-quicktime.h"
#include "mac-render.h"
#include "mac-snes9x.h"
#include "mac-screenshot.h"
#include "mac-stringtools.h"
#include "mac-os.h"

#ifdef MAC_NETPLAY_SUPPORT
#include "mac-netplay.h"
#include "mac-client.h"
#include "mac-server.h"
#endif

#ifdef MAC_COREIMAGE_SUPPORT
#include "mac-coreimage.h"
#endif

#define	kRecentMenu_MAX		20
#define KeyIsPressed(km, k)	(1 & (((unsigned char *) km) [(k) >> 3] >> ((k) & 7)))

extern int			spc_is_dumping;

volatile bool8		running             = false;
volatile bool8		s9xthreadrunning    = false;

volatile bool8		eventQueued         = false;
volatile bool8		rejectinput         = false;

volatile int		windowResizeCount   = 1;
volatile bool8		windowExtend        = true;

uint32				controlPad[MAC_MAX_PLAYERS];

uint8				romDetect           = kAutoROMType,
					interleaveDetect    = kAutoInterleave,
					videoDetect         = kAutoVideo,
					headerDetect        = kAutoHeader;

WindowRef			gWindow             = nil;
CGrafPtr			gWindowPort         = nil;
RgnHandle			gWindowRgn          = nil;
Rect				gWindowRect;
int					gWindowBarHeight;
int					glScreenW,
					glScreenH;
CGRect				glScreenBounds;
Point				windowPos[kWindowCount];
CGSize				windowSize[kWindowCount];

int					macFrameSkip        = -1;
int32				skipFrames          = 3;
int64				lastFrame           = 0;

long				lastDrawingMethod   = 0;

unsigned long		spcFileCount        = 0,
					pngFileCount        = 0;				
				
long				systemVersion,
					qtVersion,
					hiToolboxVersion;

bool8				finished            = false,
					cartOpen            = false,
					autofire            = false,
					hidExist            = true,
					folderWarning       = false,
					lockedROMMedia      = false,
					directDisplay       = false;

bool8				fullscreen          = false,
					autoRes             = false,
					doubleSize          = true,
					tvMode              = false,
					smoothMode          = true,
					eagleMode           = false,
					saiMode             = false,
					supsaiMode          = false,
					epxMode             = false,
					hq2xMode            = false,
					glstretch           = true,
					gl32bit             = true,
					vsync               = true,
					drawoverscan        = false,
					screencurvature     = false,
					multiprocessor      = false,
					ciFilterEnable      = false;
long				drawingMethod       = kDrawingOpenGL;

float				macSoundPitch       = 1.0,
					macSoundPitchMaster = 1.0;
SInt32				macSoundVolume      = 80;
int					macSoundInterval  	= 10;
uint16				aueffect            = 0;

uint8				saveInROMFolder     = 0;	// 0 : Snes9x  1 : ROM  2 : Application Support
int					mac7110Load         = 1,
					mac7110Megs         = 5,
					macSDD1Pack         = 1;
					
int					macCurvatureWarp    = 15,
					macAspectRatio      = 0;

bool8				startopendlog       = false,
					showtimeinfrz       = true,
					enabletoggle        = true,
					savewindowpos       = false,
					minimizecpu         = true,
					onscreeninfo        = true;
int					musicboxmode        = kMBXSoundEmulation;

bool8				applycheat          = false;
int					padSetting          = 1,
					deviceSetting       = 1;	
AutoFireState		autofireRec[MAC_MAX_PLAYERS];

int					macControllerOption = SNES_JOYPAD;

char				npServerIP[256];
char				npName[256];

bool8				macQTRecord         = false;
uint16				macQTMovFlag        = 0;

int					macFastForwardRate  = 5;

int					inactiveMode        = 2;

uint16				macRecordFlag       = 0x3,
					macPlayFlag         = 0x1;
wchar_t				macRecordWChar[MOVIE_MAX_METADATA];

bool8				lastoverscan        = false;

CGPoint				unlimitedCursor;

ExtraOption			extraOptions;

static void Initialize(void);
static void Deinitialize(void);
static void InitAutofire(void);
static void InitRecentItems(void);
static void DeinitRecentItems(void);
static void ClearRecentItems(void);
static void InitRecentMenu(void);
static void DeinitRecentMenu(void);
static void PrepareMenus(void);
static void EnableOptionMenus(void);
static void DisableOptionMenus(void);
static void ProcessInput(void);
static void ResizeGameWindow(void);
static void ChangeAutofireSettings(int, int);
static void ChangeTurboRate(int);
static void ForceChangingKeyScript(void);
static void CFTimerCallback(CFRunLoopTimerRef, void *);
static void UpdateFreezeDefrostScreen(int, CGImageRef, uint8 *, CGContextRef);
static void * MacSNES9XThread(void *);
static OSStatus HandleMenuChoice(long, Boolean *);
static inline void EmulationLoop(void);
static pascal OSStatus MainEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus SubEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus GameWindowEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus GameWindowUserPaneEventHandler(EventHandlerCallRef, EventRef, void *);

typedef struct
{
	int	globalLeft;
	int	globalTop;
	int	width;
	int	height;
}	GameViewInfo;
	
static pthread_t		s9xthread;

static MenuRef			recentMenu;
static CFStringRef		recentItem[kRecentMenu_MAX + 1];

static EventHandlerUPP	gameWindowUPP,
						gameWUPaneUPP;
static EventHandlerRef	gameWindowEventRef,
						gameWUPaneEventRef;
static int				windowZoomCount  = 0;

static int				toggleff         = 0;

static int				frameCount       = 0;

static Boolean			frzselecting     = false;

static GameViewInfo		scopeViewInfo;

static uint16			changeAuto[2] = { 0x0000, 0x0000 };

enum
{
	kmF1Key  = 0x7A,
	kmF2Key  = 0x78,
	kmF3Key	 = 0x63,
	kmF4Key	 = 0x76,
	kmF5Key	 = 0x60,
	kmF6Key  = 0x61,
	km1Key   = 0x12,
	km2Key   = 0x13,
	km3Key   = 0x14,
	km4Key   = 0x15,
	km5Key   = 0x17,
	km6Key   = 0x16,
	km7Key   = 0x1A,
	km8Key   = 0x1C,
	km9Key   = 0x19,
	kmAKey   = 0x00,
	kmBKey   = 0x0B,
	kmCKey   = 0x08,
	kmEscKey = 0x35,
	kmCtrKey = 0x3B
};

struct ButtonCommand
{
	char	command[16];
	uint8	keycode;
	bool8	held;
};

static ButtonCommand btncmd[] =
{
	{ "ToggleBG0",       kmF1Key, false },
	{ "ToggleBG1",       kmF2Key, false },
	{ "ToggleBG2",       kmF3Key, false },
	{ "ToggleBG3",       kmF4Key, false },
	{ "ToggleSprites",   kmF5Key, false },
	{ "SwapJoypads",     kmF6Key, false },
	{ "SoundChannel0",   km1Key,  false },
	{ "SoundChannel1",   km2Key,  false },
	{ "SoundChannel2",   km3Key,  false },
	{ "SoundChannel3",   km4Key,  false },
	{ "SoundChannel4",   km5Key,  false },
	{ "SoundChannel5",   km6Key,  false },
	{ "SoundChannel6",   km7Key,  false },
	{ "SoundChannel7",   km8Key,  false },
	{ "SoundChannelsOn", km9Key,  false }
};

#define	kCommandListSize	(sizeof(btncmd) / sizeof(btncmd[0]))

enum
{
	mApple          = 128,
	iAbout          = 1,
	
	mFile           = 129,
	iOpen           = 1,
	iOpenRecent     = 2,
	iRomInfo        = 4,
	
	mControl        = 134,
	iKeyboardLayout = 1,
	iISpLayout      = 2,
	iAutoFire       = 4,
	iISpPreset      = 6,
	
	mEdit           = 130,

	mEmulation      = 131,
	iResume         = 1,
	iSoftReset      = 3,
	iReset          = 4,
	iDevice         = 6,
	
	mCheat          = 132,
	iApplyCheats    = 1,
	iGameGenie      = 3,
	iCheatFinder    = 4,

	mOption         = 133,
	iFreeze         = 1,
	iDefrost        = 2,
	iFreezeTo       = 4,
	iDefrostFrom    = 5,
	iRecordMovie    = 7,
	iPlayMovie      = 8,
	iQTMovie        = 10,
	iSaveSPC        = 12,
	iSaveSRAM       = 13,
	iCIFilter       = 15,
	iMusicBox       = 17,
	
	mNetplay        = 135,
	iServer         = 1,
	iClient         = 2,
	
	mPresets        = 201,
	
	mDevice         = 202,
	iPad            = 1,
	iMouse			= 2,
	iMouse2         = 3,
	iSuperScope     = 4,
	iMultiPlayer5   = 5,
	iMultiPlayer5_2 = 6,
	iJustifier1     = 7,
	iJustifier2     = 8,
	
	mRecentItem     = 203
};

int main(int argc, char **argv)
{
 	#pragma unused (argc, argv)

	OSStatus		err;
	EventHandlerRef	eref;
	EventHandlerUPP	appEventHandler;
	EventTypeSpec	mEvents[] = { { kEventClassCommand, kEventCommandProcess } },
					sEvents[] = { { kEventClassCommand, kEventCommandProcess },
								  { kEventClassMouse,   kEventMouseUp        },
								  { kEventClassMouse,   kEventMouseMoved     },
								  { kEventClassMouse,   kEventMouseDragged   } };
	
	appEventHandler = NewEventHandlerUPP(MainEventHandler);
 	err = InstallApplicationEventHandler(appEventHandler, GetEventTypeCount(mEvents), mEvents, nil, &eref);
	
	Initialize();
   	
	while (!finished)
	{
		if (cartOpen && running)
		{			
			#ifdef DEBUGGER
				CPU.Flags |= DEBUG_MODE_FLAG;
				S9xDoDebug();
			#endif
			
			eventQueued = false;
			
			Microseconds((UnsignedWide *) &lastFrame);
			frameCount = 0;
			if (macFrameSkip < 0)
			{
				printf("Frame skip: auto\n");
				skipFrames = 3;
			}
			else
			{
				printf("Frame skip: %d\n", macFrameSkip - 1);
				skipFrames = macFrameSkip;
			}
						
			CFStringRef	ref;
			MenuRef		menu;
			
			err = RemoveEventHandler(eref);
			DisposeEventHandlerUPP(appEventHandler);
			
			appEventHandler = NewEventHandlerUPP(SubEventHandler);
			err = InstallApplicationEventHandler(appEventHandler, GetEventTypeCount(sEvents), sEvents, nil, &eref);
			
			S9xInitDisplay(nil, nil);
			ClearGFXScreen();

			menu = GetMenuRef(mEmulation);
			
			ref = CFCopyLocalizedString(CFSTR("PauseMenu"), "pause");
			err = SetMenuItemTextWithCFString(menu, iResume, ref);
			CFRelease(ref);
			
			if (!fullscreen)
				ForceChangingKeyScript();
			
		#ifdef MAC_NETPLAY_SUPPORT
			if (Settings.NetPlay)
			{
				if (Settings.NetPlayServer)
				{
					EnableMenuItem (menu, 0);
					EnableMenuItem (menu, iResume);
				}
				else
				{
					DisableMenuItem(menu, 0);
					DisableMenuItem(menu, iResume);
				}
			}
			else
			{
		#endif
				EnableMenuItem (menu, 0);
				EnableMenuItem (menu, iResume);
		#ifdef MAC_NETPLAY_SUPPORT
			}
		#endif
			DisableMenuItem(menu, iSoftReset);
			DisableMenuItem(menu, iReset);
			DisableMenuItem(menu, iDevice);

			if (ciFilterEnable)
				EnableMenuItem(GetMenuRef(mOption), iCIFilter);

			pthread_create(&s9xthread, nil, MacSNES9XThread, nil);
			
			CFRunLoopTimerRef		cftimer    = nil;
			CFRunLoopTimerContext	cftimerctx = { 0, nil, nil, nil, nil };
			
			if (!fullscreen)
			{
				cftimer = CFRunLoopTimerCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent(), 30, 0, 0, CFTimerCallback, &cftimerctx);
				if (cftimer)
					CFRunLoopAddTimer(CFRunLoopGetCurrent(), cftimer, kCFRunLoopCommonModes);
			}
			
			RunApplicationEventLoop();
			
			if (!fullscreen)
			{
				if (cftimer)
				{
					CFRunLoopTimerInvalidate(cftimer);
					CFRelease(cftimer);
					cftimer = nil;
				}
			}
			
			pthread_join(s9xthread, nil);
			
			DisableMenuItem(GetMenuRef(mOption), iCIFilter);
			
			EnableMenuItem(menu, iSoftReset);
			EnableMenuItem(menu, iReset);
			EnableMenuItem(menu, iResume);
			EnableMenuItem(menu, iDevice);
			
			ref = CFCopyLocalizedString(CFSTR("RunMenu"), "run");
			err = SetMenuItemTextWithCFString(menu, iResume, ref);
			CFRelease(ref);
			
			S9xDeinitDisplay();
			
		#ifdef MAC_NETPLAY_SUPPORT
			if (Settings.NetPlay)
			{
				if (!Settings.NetPlayServer)
				{
					DeinitGameWindow();
					cartOpen = false;
				}
				
				Settings.NetPlay = false;
				Settings.NetPlayServer = false;
			}
		#endif

			err = RemoveEventHandler(eref);
			DisposeEventHandlerUPP(appEventHandler);
			
			appEventHandler = NewEventHandlerUPP(MainEventHandler);
			err = InstallApplicationEventHandler(appEventHandler, GetEventTypeCount(mEvents), mEvents, nil, &eref);
		}

		if (!finished)
		{
			EnableMenus();
			RunApplicationEventLoop();
		}
	}
	
	Deinitialize();
	
	err = RemoveEventHandler(eref);
 	DisposeEventHandlerUPP(appEventHandler);

	return 0;
}

static void CFTimerCallback(CFRunLoopTimerRef timer, void *info)
{
	OSStatus	err;
	
	err = UpdateSystemActivity(OverallAct);
}

static void * MacSNES9XThread(void *)
{
	s9xthreadrunning = true;

	EmulationLoop();

	s9xthreadrunning = false;
	
	return nil;
}

static inline void EmulationLoop(void)
{
	bool8   olddisplayframerate = false;
	
	printf("Minimize CPU usage: %s\n", minimizecpu ? "on" : "off");
	
	if (macQTRecord)
	{
		olddisplayframerate = Settings.DisplayFrameRate;
		Settings.DisplayFrameRate = false;
	}
	
	MacStartSound();
	
#ifdef MAC_NETPLAY_SUPPORT
	if (Settings.NetPlay)
	{
		if (Settings.NetPlayServer)
		{
			NPServerDetachNetPlayThread();
			NPServerStartClients();
			
			while (running)
			{
				NPServerProcessInput();
				S9xMainLoop();
			}

			NPServerStopNetPlayThread();
			NPServerStopServer();
		}
		else
		{
			NPClientDetachNetPlayThread();
			NPClientNetPlayWaitStart();
			
			while (running)
			{
				NPClientProcessInput();
				S9xMainLoop();
			}
			
			NPClientStopNetPlayThread();
			NPClientDisconnect();

			NPClientRestoreConfig();
		}
	}
	else
	{
#endif
		while (running)
		{
			ProcessInput();
			S9xMainLoop();
		}
		
#ifdef MAC_NETPLAY_SUPPORT
	}
#endif

	MacStopSound();
	
	if (macQTRecord)
	{
		MacQTStopRecording();
		macQTRecord = false;
		
		Settings.DisplayFrameRate = olddisplayframerate;
	}

	if (S9xMovieActive())
		S9xMovieStop(true);
}

static pascal OSStatus MainEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
    #pragma unused (inHandlerCallRef, inUserData)
    
    OSStatus	err, result = eventNotHandledErr;
	Boolean		done = false;

	if (frzselecting)
		return result;

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
						UInt32	modifierkey;
						
						err = GetEventParameter(inEvent, kEventParamKeyModifiers, typeUInt32, nil, sizeof(UInt32), nil, &modifierkey);
						if (err == noErr)
						{
							if ((cmd.commandID == 'pref') && (modifierkey & optionKey))
								cmd.commandID = 'EXTR';

							result = HandleMenuChoice(cmd.commandID, &done);
						
							if (done)
								QuitApplicationEventLoop();
						}
					}
			}
	}
	
	return result;
}

static pascal OSStatus SubEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
    #pragma unused (inHandlerCallRef, inUserData)
    
    OSStatus	err, result = eventNotHandledErr;
	
	if (frzselecting)
		return result;

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
						switch (cmd.commandID)
						{
							case 'Erun':	// Pause
							case 'SubQ':	// Queue from emulation thread
								running = false;
								while (s9xthreadrunning)
									sleep(0);
								QuitApplicationEventLoop();
								result = noErr;
								
								break;
								
							case 'Ocif':	// Core Image Filter
								HiliteMenu(0);
								ConfigureCoreImageFilter();
								result = noErr;
								
								break;
						}
					}
			}
			
			break;
			
		case kEventClassMouse:
			if (fullscreen)
			{
				if ((macControllerOption == SNES_JOYPAD) || (macControllerOption == SNES_MULTIPLAYER5) || (macControllerOption == SNES_MULTIPLAYER5_2))
				{
				#ifdef MAC_NETPLAY_SUPPORT
					if (!(Settings.NetPlay && !Settings.NetPlayServer))
					{
				#endif
						switch (GetEventKind(inEvent))
						{
							case kEventMouseUp:
								HIPoint	hipt;
								
								err = GetEventParameter(inEvent, kEventParamMouseLocation, typeHIPoint, nil, sizeof(HIPoint), nil, &hipt);
								if (err == noErr)
								{
									if (CGRectContainsPoint(glScreenBounds, hipt))
									{
										printf("Exiting from fullscreen...\n");
										
										running = false;
										while (s9xthreadrunning)
											sleep(0);
										QuitApplicationEventLoop();
										result = noErr;
									}
								}
						}
				#ifdef MAC_NETPLAY_SUPPORT
					}
				#endif
				}
				else
				if ((macControllerOption == SNES_MOUSE) || (macControllerOption == SNES_MOUSE_SWAPPED))
				{
					switch (GetEventKind(inEvent))
					{
						case kEventMouseMoved:
						case kEventMouseDragged:
							HIPoint	hipt;
							
							err = GetEventParameter(inEvent, kEventParamMouseDelta, typeHIPoint, nil, sizeof(HIPoint), nil, &hipt);
							if (err == noErr)
							{
								unlimitedCursor.x += hipt.x;
								unlimitedCursor.y += hipt.y;
							}
					}					
				}
			}
	}
	
	return result;
}

void PostQueueToSubEventLoop(void)
{
	OSStatus	err;
	EventRef	event;
	
	err = CreateEvent(kCFAllocatorDefault, kEventClassCommand, kEventCommandProcess, 0, kEventAttributeUserEvent, &event);
	if (err == noErr)
	{
		HICommand	cmd;
		
		cmd.commandID          = 'SubQ';
		cmd.attributes         = kEventAttributeUserEvent;
		cmd.menu.menuRef       = nil;
		cmd.menu.menuItemIndex = 0;
		
		err = SetEventParameter(event, kEventParamDirectObject, typeHICommand, sizeof(HICommand), &cmd);
		if (err == noErr)
			err = PostEventToQueue(GetMainEventQueue(), event, kEventPriorityStandard);
			
		ReleaseEvent(event);
	}
}

void InitGameWindow(void)
{
	OSStatus			err;
	IBNibRef			nibRef;
	HIViewRef			ctl;
	HIViewID			cid = { 'Pict', 0 };
	CFStringRef			ref;
	WindowAttributes	attr;
	char				drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	EventTypeSpec		wupaneEvents[] = { { kEventClassControl, kEventControlClick            },
										   { kEventClassControl, kEventControlDraw             } },
						windowEvents[] = { { kEventClassWindow,  kEventWindowDeactivated       },
										   { kEventClassWindow,  kEventWindowActivated         },
										   { kEventClassWindow,  kEventWindowBoundsChanging    },
										   { kEventClassWindow,  kEventWindowBoundsChanged     },
										   { kEventClassWindow,  kEventWindowZoom              },
										   { kEventClassWindow,  kEventWindowToolbarSwitchMode } };

	if (gWindow)
		return;
		
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err)
		QuitWithFatalError(err, "os 02");

	err = CreateWindowFromNib(nibRef, CFSTR("GameWindow"), &gWindow);
	if (err)
		QuitWithFatalError(err, "os 03");

	DisposeNibReference(nibRef);

	HIViewFindByID(HIViewGetRoot(gWindow), cid, &ctl);

	gameWindowUPP = NewEventHandlerUPP(GameWindowEventHandler);
	err = InstallWindowEventHandler(gWindow, gameWindowUPP, GetEventTypeCount(windowEvents), windowEvents, (void *) gWindow, &gameWindowEventRef);
	
	gameWUPaneUPP = NewEventHandlerUPP(GameWindowUserPaneEventHandler);
	err = InstallControlEventHandler(ctl, gameWUPaneUPP, GetEventTypeCount(wupaneEvents), wupaneEvents, (void *) gWindow, &gameWUPaneEventRef);

	_splitpath(Memory.ROMFilename, drive, dir, fname, ext);
	ref = CFStringCreateWithCString(kCFAllocatorDefault, fname, MAC_PATH_ENCODING);
	if (ref)
	{
		SetWindowTitleWithCFString(gWindow, ref);
		CFRelease(ref);
	}

	attr = kWindowFullZoomAttribute | kWindowResizableAttribute | kWindowLiveResizeAttribute;
	if ((drawingMethod == kDrawingOpenGL) || (drawingMethod == kDrawingBlitGL))
		err = ChangeWindowAttributes(gWindow, attr, kWindowNoAttributes);
	else
		err = ChangeWindowAttributes(gWindow, kWindowNoAttributes, attr);
	
	attr = kWindowToolbarButtonAttribute;
	if (((drawingMethod == kDrawingOpenGL) || (drawingMethod == kDrawingBlitGL)) && !drawoverscan)
		err = ChangeWindowAttributes(gWindow, attr, kWindowNoAttributes);
	else
		err = ChangeWindowAttributes(gWindow, kWindowNoAttributes, attr);

	if (savewindowpos)
	{
		MoveWindow(gWindow, windowPos[kWindowScreen].h, windowPos[kWindowScreen].v, false);
		
		if ((windowSize[kWindowScreen].width <= 0) || (windowSize[kWindowScreen].height <= 0))
		{
			windowExtend = true;
			windowSize[kWindowScreen].width  = 512;
			windowSize[kWindowScreen].height = kMacWindowHeight;
		}
		
		if (drawingMethod == kDrawingDirect)
		{
			windowExtend = true;
			windowSize[kWindowScreen].width  = 512;
			windowSize[kWindowScreen].height = kMacWindowHeight;
		}
		else
		if (!lastoverscan && !windowExtend && drawoverscan)
		{
			windowExtend = true;
			windowSize[kWindowScreen].height = (int) ((float) (windowSize[kWindowScreen].height + 0.5) * SNES_HEIGHT_EXTENDED / SNES_HEIGHT);
		}
		
		SizeWindow(gWindow, (short) windowSize[kWindowScreen].width, (short) windowSize[kWindowScreen].height, false);
	}
	else
	{
		if ((drawingMethod == kDrawingDirect) || drawoverscan)
			windowExtend = true;

		SizeWindow(gWindow, 512, (windowExtend ? kMacWindowHeight : (SNES_HEIGHT << 1)), false);
		RepositionWindow(gWindow, nil, kWindowCenterOnMainScreen);
	}
	
	windowZoomCount = 0;
	
	GetWindowBounds(gWindow, kWindowContentRgn, &gWindowRect);

	ActivateWindow(gWindow, true);
}

void UpdateGameWindow(void)
{
	OSStatus	err;
	HIViewRef	ctl;
	HIViewID	cid = { 'Pict', 0 };

	if (!gWindow)
		return;

	HIViewFindByID(HIViewGetRoot(gWindow), cid, &ctl);
	err = HIViewSetNeedsDisplay(ctl, true);
}

static void ResizeGameWindow(void)
{
	Rect	rct;
	int		ww = SNES_WIDTH,
			wh = (windowExtend ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT);

	if (!gWindow)
		return;

	GetWindowBounds(gWindow, kWindowContentRgn, &rct);
	
	switch (windowZoomCount)
	{
		case 0:						break;
		case 1: ww <<= 1; wh <<= 1;	break;
		case 2: ww <<= 2; wh <<= 2; break;
	}
	
	rct.right  = rct.left + ww;
	rct.bottom = rct.top  + wh;

	SetWindowBounds(gWindow, kWindowContentRgn, &rct);
	
	windowZoomCount++;
	if (windowZoomCount == 3)
		windowZoomCount = 0;
}

void DeinitGameWindow(void)
{
	OSStatus	err;
	
	if (!gWindow)
		return;
		
	SaveWindowPosition(gWindow, kWindowScreen);
	lastoverscan = drawoverscan;
	
	err = RemoveEventHandler(gameWUPaneEventRef);
	DisposeEventHandlerUPP(gameWUPaneUPP);

	err = RemoveEventHandler(gameWindowEventRef);
	DisposeEventHandlerUPP(gameWindowUPP);
	
	ReleaseWindow(gWindow);
	gWindow = nil;
}

static pascal OSStatus GameWindowEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
    #pragma unused (inHandlerCallRef)

	OSStatus	err, result = eventNotHandledErr;
	WindowRef	window = (WindowRef) inUserData;
	UInt32		attr;
	Rect		rct;

	switch (GetEventClass(inEvent))
	{
		case kEventClassWindow:
			switch (GetEventKind(inEvent))
			{
				case kEventWindowDeactivated:
					if (running)
					{
					#ifdef MAC_NETPLAY_SUPPORT
						if (!(Settings.NetPlay && !Settings.NetPlayServer))
						{
					#endif
							if (inactiveMode == 3)
							{
								running = false;
								while (s9xthreadrunning)
									sleep(0);
								QuitApplicationEventLoop();
								result = noErr;
							}
							else
							if (inactiveMode == 2)
							{
								rejectinput = true;
								result = noErr;
							}
					#ifdef MAC_NETPLAY_SUPPORT
						}
					#endif
					}

					break;

				case kEventWindowActivated:
					if (running)
					{
					#ifdef MAC_NETPLAY_SUPPORT
						if (!(Settings.NetPlay && !Settings.NetPlayServer))
						{
					#endif
							ForceChangingKeyScript();
							
							if (inactiveMode == 2)
							{
								rejectinput = false;
								result = noErr;
							}
					#ifdef MAC_NETPLAY_SUPPORT
						}
					#endif
					}
					
					break;
					
				case kEventWindowBoundsChanging:
					windowResizeCount = 0x7FFFFFFF;
					err = GetEventParameter(inEvent, kEventParamAttributes, typeUInt32, nil, sizeof(UInt32), nil, &attr);
					if ((err == noErr) && (attr & kWindowBoundsChangeSizeChanged))
					{
						err = GetEventParameter(inEvent, kEventParamCurrentBounds, typeQDRectangle, nil, sizeof(Rect), nil, &rct);
						if (err == noErr)
						{
							if (GetCurrentEventKeyModifiers() & shiftKey)
							{
								Rect	origRct;
								
								err = GetEventParameter(inEvent, kEventParamOriginalBounds, typeQDRectangle, nil, sizeof(Rect), nil, &origRct);
								if (err == noErr)
								{
									rct.right = rct.left + (int) ((float) (origRct.right - origRct.left) / (float) (origRct.bottom - origRct.top) * (float) (rct.bottom - rct.top));
									err = SetEventParameter(inEvent, kEventParamCurrentBounds, typeQDRectangle, sizeof(Rect), &rct);
								}
							}
							
							gWindowRect = rct;
						#ifdef MAC_JAGUAR_SUPPORT
							if (systemVersion < 0x1030)
							{
								HIViewRef	ctl;
								HIViewID	cid = { 'Pict', 0 };
								HIRect		bounds;

								bounds.origin.x = 0.0;
								bounds.origin.y = 0.0;
								bounds.size.width  = (float) (rct.right  - rct.left);
								bounds.size.height = (float) (rct.bottom - rct.top );
								
								HIViewFindByID(HIViewGetRoot(window), cid, &ctl);
								HIViewSetFrame(ctl, &bounds);
							}
						#endif
						}
					}

					result = noErr;

					break;
					
				case kEventWindowBoundsChanged:
					windowResizeCount = 3;
					result = noErr;

					break;
				
				case kEventWindowZoom:
					ResizeGameWindow();
					result = noErr;

					break;
					
				case kEventWindowToolbarSwitchMode:
					windowExtend = !windowExtend;

					GetWindowBounds(gWindow, kWindowContentRgn, &rct);
					
					if (windowExtend)
						rct.bottom = rct.top + (int) ((float) (rct.bottom - rct.top + 0.5) * SNES_HEIGHT_EXTENDED / SNES_HEIGHT);
					else
						rct.bottom = rct.top + (int) ((float) (rct.bottom - rct.top + 0.5) * SNES_HEIGHT / SNES_HEIGHT_EXTENDED);
						
					SetWindowBounds(gWindow, kWindowContentRgn, &rct);

					result = noErr;					
			}
	}

	return result;
}

static pascal OSStatus GameWindowUserPaneEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
    #pragma unused (inHandlerCallRef, inUserData)

    OSStatus	err, result = eventNotHandledErr;

	switch (GetEventClass(inEvent))
	{
		case kEventClassControl:
			switch (GetEventKind(inEvent))
			{
				case kEventControlClick:
					if (running)
					{
						if ((macControllerOption == SNES_JOYPAD) || (macControllerOption == SNES_MULTIPLAYER5) || (macControllerOption == SNES_MULTIPLAYER5_2))
						{
						#ifdef MAC_NETPLAY_SUPPORT
							if (!(Settings.NetPlay && !Settings.NetPlayServer))
							{
						#endif
								if (!frzselecting)
								{
									running = false;
									while (s9xthreadrunning)
										sleep(0);
									QuitApplicationEventLoop();
									result = noErr;
								}
						#ifdef MAC_NETPLAY_SUPPORT
							}
						#endif
						}
					}
					else
					{
						UInt32	count;
						
						err = GetEventParameter(inEvent, kEventParamClickCount, typeUInt32, nil, sizeof(UInt32), nil, &count);
						if ((err == noErr) && (count == 2))
						{
							SNES9X_Go();
							QuitApplicationEventLoop();
							result = noErr;
						}
					}
					
					break;

				case kEventControlDraw:
					CGContextRef	ctx;
					HIViewRef		view;
					HIRect			bounds;

					err = GetEventParameter(inEvent, kEventParamDirectObject, typeControlRef, nil, sizeof(typeControlRef), nil, &view);
					if (err == noErr)
					{
						err = GetEventParameter(inEvent, kEventParamCGContextRef, typeCGContextRef, nil, sizeof(CGContextRef), nil, &ctx);
						if (err == noErr)
						{
							if (!running)
							{
								HIViewGetBounds(view, &bounds);
								CGContextTranslateCTM(ctx, 0, bounds.size.height);
								CGContextScaleCTM(ctx, 1.0, -1.0);
								DrawPauseScreen(ctx, bounds);
							}
						}
					}

					result = noErr;
			}
	}

	return result;
}

static void InitRecentItems(void)
{
	CFStringRef	keyRef, pathRef;
	int			count, i;
	char		key[32];
	
	count = 0;
	
	for (i = 0; i <= kRecentMenu_MAX; i++)
		recentItem[i] = nil;
	
	for (i = 0; i <  kRecentMenu_MAX; i++)
	{
		sprintf(key, "RecentItem_%02d", i);
		keyRef = CFStringCreateWithCString(kCFAllocatorDefault, key, CFStringGetSystemEncoding());
		if (keyRef)
		{
			pathRef = (CFStringRef) CFPreferencesCopyAppValue(keyRef, kCFPreferencesCurrentApplication);
			if (pathRef)
			{
				recentItem[count] = pathRef;
				count++;
			}
			
			CFRelease(keyRef);
		}
	}
}

static void DeinitRecentItems(void)
{
	CFStringRef	keyRef;
	int			i;
	char		key[32];

	for (i = 0; i < kRecentMenu_MAX; i++)
	{
		sprintf(key, "RecentItem_%02d", i);
		keyRef = CFStringCreateWithCString(kCFAllocatorDefault, key, CFStringGetSystemEncoding());
		if (keyRef)
		{
			if (recentItem[i])
			{
				CFPreferencesSetAppValue(keyRef, recentItem[i], kCFPreferencesCurrentApplication);
				CFRelease(recentItem[i]);
				recentItem[i] = nil;
			}
			else
				CFPreferencesSetAppValue(keyRef, nil, kCFPreferencesCurrentApplication);
			
			CFRelease(keyRef);
		}
	}
	
	CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
}

static void ClearRecentItems(void)
{
	for (int i = 0; i < kRecentMenu_MAX; i++)
	{
		if (recentItem[i])
		{
			CFRelease(recentItem[i]);
			recentItem[i] = nil;
		}
	}
}

void AddRecentItem(FSRef *ref)
{
	OSStatus	err;
	char		path[PATH_MAX + 1];
	
	err = FSRefMakePath(ref, (unsigned char *) path, PATH_MAX);
	if (err == noErr)
	{
		CFStringRef	pathRef;
		
		pathRef = CFStringCreateWithCString(kCFAllocatorDefault, path, MAC_PATH_ENCODING);
		if (pathRef)
		{
			int	i, j;
			
			for (i = 0; i < kRecentMenu_MAX; i++)
				if (recentItem[i] && (CFStringCompare(pathRef, recentItem[i], 0) == 0))
					break;
					
			if (i == kRecentMenu_MAX)
			{
				for (j = kRecentMenu_MAX - 1; j >= 0; j--)
					recentItem[j + 1] = recentItem[j];
					
				if (recentItem[kRecentMenu_MAX])
				{
					CFRelease(recentItem[kRecentMenu_MAX]);
					recentItem[kRecentMenu_MAX] = nil;
				}
				
				recentItem[0] = pathRef;
			}
			else
			{
				CFRelease(pathRef);
				
				if (i > 0)
				{
					CFStringRef	temp;
					
					temp = recentItem[i];
				
					for (j = i - 1; j >=0; j--)
						recentItem[j + 1] = recentItem[j];
					
					recentItem[0] = temp;
				}
			}
		}
	}
}

static void InitRecentMenu(void)
{
	OSStatus	err;
	
	err = CreateNewMenu(mRecentItem, 0, &recentMenu);
	err = SetMenuItemHierarchicalMenu(GetMenuRef(mFile), iOpenRecent, recentMenu);
}

static void DeinitRecentMenu(void)
{
	ReleaseMenu(recentMenu);
}

void BuildRecentMenu(void)
{
	OSStatus	err;
	CFStringRef	str;
	int			i;
	
	err = DeleteMenuItems(recentMenu, 1, CountMenuItems(recentMenu));
	
	for (i = 0; i < kRecentMenu_MAX; i++)
	{
		if (!recentItem[i])
			break;

		Boolean	r;
		char	path[PATH_MAX + 1];
		
		r = CFStringGetCString(recentItem[i], path, PATH_MAX, MAC_PATH_ENCODING);
		if (r)
		{
			CFStringRef	nameRef;
			char		drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
			
			_splitpath(path, drive, dir, fname, ext);
			strcat(fname, ext);
			nameRef = CFStringCreateWithCString(kCFAllocatorDefault, fname, MAC_PATH_ENCODING);
			if (nameRef)
			{
				err = AppendMenuItemTextWithCFString(recentMenu, nameRef, 0, 'FRe0' + i, nil);
				CFRelease(nameRef);
			}
		}
	}
	
	err = AppendMenuItemTextWithCFString(recentMenu, nil, kMenuItemAttrSeparator, 'FR__', nil);
	
	str = CFCopyLocalizedString(CFSTR("ClearMenu"), "ClearMenu");
	if (str)
	{
		err = AppendMenuItemTextWithCFString(recentMenu, str, 0, 'FRcr', nil);
		CFRelease(str);
	}
}

static void EnableOptionMenus(void)
{
	MenuRef	menu = GetMenuRef(mOption);
	UInt16	n    = CountMenuItems(menu);
	
	for (UInt16 i = 1; i <= n; i++)
		if (i != iCIFilter)
			EnableMenuItem(menu, i);
}

static void DisableOptionMenus(void)
{
	MenuRef	menu = GetMenuRef(mOption);
	UInt16	n    = CountMenuItems(menu);
	
	for (UInt16 i = 1; i <= n; i++)
		if (i != iCIFilter)
			DisableMenuItem(menu, i);
}

void EnableMenus(void)
{
	EnableMenuItem(GetMenuRef(mApple),     iAbout);
	EnableMenuItem(GetMenuRef(mFile),      0);
	EnableMenuItem(GetMenuRef(mEmulation), 0);
	EnableMenuItem(GetMenuRef(mControl),   0);
	EnableMenuItem(GetMenuRef(mCheat),     0);
	EnableMenuItem(GetMenuRef(mNetplay),   0);
	EnableMenuCommand(nil, kHICommandPreferences);
	EnableMenuCommand(nil, kHICommandQuit);
	EnableOptionMenus();
	
	PrepareMenus();
	DrawMenuBar();
}

void DisableMenus(void)
{
	DisableMenuItem(GetMenuRef(mApple),     iAbout);
	DisableMenuItem(GetMenuRef(mFile),      0);
	DisableMenuItem(GetMenuRef(mEmulation), 0);
	DisableMenuItem(GetMenuRef(mControl),   0);
	DisableMenuItem(GetMenuRef(mCheat),     0);
	DisableMenuItem(GetMenuRef(mNetplay),   0);
	DisableMenuCommand(nil, kHICommandPreferences);
	DisableMenuCommand(nil, kHICommandQuit);
	DisableOptionMenus();
	
	DrawMenuBar();
}

static void PrepareMenus(void)
{
	MenuRef	fileMenu, emuMenu, ctrlMenu, cheatMenu, netMenu;

	fileMenu   = GetMenuRef(mFile);
	emuMenu    = GetMenuRef(mEmulation);
	ctrlMenu   = GetMenuRef(mControl);
	cheatMenu  = GetMenuRef(mCheat);
	netMenu    = GetMenuRef(mNetplay);

	if (cartOpen)
	{
		EnableMenuItem(fileMenu,   iRomInfo);
		EnableMenuItem(emuMenu,    iResume);
		EnableMenuItem(emuMenu,    iSoftReset);
		EnableMenuItem(emuMenu,    iReset);
		EnableMenuItem(cheatMenu,  0);
		EnableMenuItem(netMenu,    iServer);
		EnableOptionMenus();
	}
	else
	{
		DisableMenuItem(fileMenu,   iRomInfo);
		DisableMenuItem(emuMenu,    iResume);
		DisableMenuItem(emuMenu,    iSoftReset);
		DisableMenuItem(emuMenu,    iReset);		
		DisableMenuItem(cheatMenu,  0);
		DisableMenuItem(netMenu,    iServer);
		DisableOptionMenus();
	}
	
	if (hidExist)
	{
		EnableMenuItem(ctrlMenu, iISpLayout);
		EnableMenuItem(ctrlMenu, iISpPreset);
	}
	else
	{
		DisableMenuItem(ctrlMenu, iISpLayout);
		DisableMenuItem(ctrlMenu, iISpPreset);
	}
}

static OSStatus HandleMenuChoice(long command, Boolean *done)
{
	OSStatus	err, result = noErr;
	MenuRef		mh;
	int			item;
	bool8		isok = true;
	
	if ((command & 0xFFFFFF00) == 'FRe\0')
	{
		Boolean	r;
		int		index;
		char	path[PATH_MAX + 1];
		
		index = (int) (command & 0x000000FF) - (int) '0';
		r = CFStringGetCString(recentItem[index], path, PATH_MAX, MAC_PATH_ENCODING);
		if (r)
		{
			FSRef	ref;
			
			err = FSPathMakeRef((unsigned char *) path, &ref, nil);
			if (err == noErr)
			{
				if (SNES9X_OpenCart(&ref))
				{
					SNES9X_Go();
					*done = true;
				}
			}
		}
	}
	else
	{
		switch (command)
		{
			case 'abou':	// About SNES9X
				StartCarbonModalDialog();
				AboutDialog();
				FinishCarbonModalDialog();
	
				break;
				
			case 'pref':	// Preferences...
				StartCarbonModalDialog();
				ConfigurePreferences();
				FinishCarbonModalDialog();
			
				break;
				
			case 'EXTR':	// Extra Options...
				StartCarbonModalDialog();
				ConfigureExtraOptions();
				FinishCarbonModalDialog();
			
				break;

			case 'quit':	// Quit SNES9X
				SNES9X_Quit();
				*done = true;
				
				break;
			
			case 'open':	// Open ROM Image...
				if (SNES9X_OpenCart(nil))
				{
					SNES9X_Go();
					*done = true;
				}
				
				break;
				
			case 'FRcr':	// Clear Menu
				ClearRecentItems();
				BuildRecentMenu();
				
				break;
				
			case 'Finf':	// ROM Information
				StartCarbonModalDialog();
				RomInfoDialog();
				FinishCarbonModalDialog();
					
				break;
				
			case 'Ckey':	// Configure Keyboard...
				StartCarbonModalDialog();
				ConfigureKeyboard();
				FinishCarbonModalDialog();
					
				break;
					
			case 'Cpad':	// Configure Controllers...
				StartCarbonModalDialog();	
				ConfigureHID();
				FinishCarbonModalDialog();
						
				break;
						
			case 'Caut':	// Automatic Fire...
				StartCarbonModalDialog();	
				ConfigureAutofire();
				FinishCarbonModalDialog();
	
				break;
	
			case 'Hapl':	// Apply Cheat Entries
				mh = GetMenuRef(mCheat);
				applycheat = !applycheat;
				CheckMenuItem(mh, iApplyCheats, applycheat);
				Settings.ApplyCheats = applycheat;
						
				if (!Settings.ApplyCheats)
					S9xRemoveCheats();
				else
					S9xApplyCheats();
	
				break;
			
			case 'Hent':	// Cheat Entry...
				StartCarbonModalDialog();	
				ConfigureCheat();
				FinishCarbonModalDialog();
						
				break;
	
			case 'Hfnd':	// Cheat Finder...
				StartCarbonModalDialog();	
				CheatFinder();
				FinishCarbonModalDialog();
						
				break;

			case 'Erun':	// Run
				SNES9X_Go();
				*done = true;
						
				break;
			
			case 'Esrs':	// Software Reset
				SNES9X_SoftReset();
				//printf("\nSoft Reset.\n\n");
				SNES9X_Go();
				*done = true;
						
				break;

			case 'Erst':	// Hardware Reset
				SNES9X_Reset();
				//printf("\nHard Reset.\n\n");
				SNES9X_Go();
				*done = true;
						
				break;
									
			case 'Ofrz':	// Freeze State
				isok = SNES9X_Freeze();
				*done = true;
						
				break;
					
			case 'Odfr':	// Defrost state
				isok = SNES9X_Defrost();
				*done = true;
						
				break;
						
			case 'Ofrd':	// Freeze State to...
				StartCarbonModalDialog();
				isok = SNES9X_FreezeTo();
				FinishCarbonModalDialog();
				
				break;
			
			case 'Odfd':	// Defrost State From...
				StartCarbonModalDialog();
				isok = SNES9X_DefrostFrom();
				if (gWindow)
					ActivateWindow(gWindow, true);
				FinishCarbonModalDialog();
				*done = true;
				
				break;

			case 'MVrc':	// Record Movie...
				StartCarbonModalDialog();
				isok = SNES9X_RecordMovie();
				if (gWindow)
					ActivateWindow(gWindow, true);
				FinishCarbonModalDialog();
				*done = true;
				
				break;

			case 'MVpl':	// Play Movie...
				StartCarbonModalDialog();
				isok = SNES9X_PlayMovie();
				if (isok && (macPlayFlag & 0x2))
				{
					running = false;
					isok = SNES9X_QTMovieRecord();
					running = true;
				}

				if (gWindow)
					ActivateWindow(gWindow, true);
				FinishCarbonModalDialog();
				*done = true;
				
				break;
				
			case 'QTmv':	// Record QuickTime Movie...
				StartCarbonModalDialog();
				isok = SNES9X_QTMovieRecord();
				if (gWindow)
					ActivateWindow(gWindow, true);
				FinishCarbonModalDialog();
				*done = true;

				break;
			
			case 'Ospc':	// Save SPC File at Next Note-on
				spc_is_dumping = 1;
						
				break;
						
			case 'Osrm':	// Save SRAM Now
				SNES9X_SaveSRAM();
						
				break;
				
			case 'Ombx':	// Music Box
				StartCarbonModalDialog();
				MusicBoxDialog();
				FinishCarbonModalDialog();
	
				break;
				
		#ifdef MAC_NETPLAY_SUPPORT
			case 'Nser':	// Server...
				bool8	sr;
				
				Settings.NetPlay = false;
				Settings.NetPlayServer = false;
	
				NPServerInit();

				if (!NPServerStartServer(NP_PORT))
				{
					NPServerStopServer();
					break;
				}
	
				StartCarbonModalDialog();
				sr = NPServerDialog();
				FinishCarbonModalDialog();
	
				if (sr)
				{
					SNES9X_Reset();
					SNES9X_Go();
					Settings.NetPlay = true;
					Settings.NetPlayServer = true;

					*done = true;
				}
				else
					NPServerStopServer();

				break;
				
			case 'Ncli':	// Client...
				bool8	cr;
				
				Settings.NetPlay = false;
				Settings.NetPlayServer = false;
	
				NPClientInit();

				StartCarbonModalDialog();
				cr = NPClientDialog();
				FinishCarbonModalDialog();
				
				if (cr)
				{
					SNES9X_Go();
					Settings.NetPlay = true;
					Settings.NetPlayServer = false;

					*done = true;
				}

				break;
		#endif
				
			case 'CPr1':	// Controller Preset
			case 'CPr2':
			case 'CPr3':
			case 'CPr4':
			case 'CPr5':		
				item = (int) (command & 0x000000FF) - (int) '0';
				err = GetMenuItemHierarchicalMenu(GetMenuRef(mControl), iISpPreset, &mh);
				CheckMenuItem(mh, padSetting, false);
				padSetting = item;
				CheckMenuItem(mh, padSetting, true);
				ClearPadSetting();
				Load_Config();
				
				break;
				
			case 'EIp1':	// Input Device
			case 'EIp2':
			case 'EIp3':
			case 'EIp4':
			case 'EIp5':
			case 'EIp6':
			case 'EIp7':
			case 'EIp8':
				item = (int) (command & 0x000000FF) - (int) '0';
				err = GetMenuItemHierarchicalMenu(GetMenuRef(mEmulation), iDevice, &mh);
				CheckMenuItem(mh, deviceSetting, false);
				deviceSetting = item;
				CheckMenuItem(mh, deviceSetting, true);
				ChangeInputDevice();
				
				break;
				
			default:
				result = eventNotHandledErr;
		}
	}
	
	return result;
}

void ChangeInputDevice(void)
{
	switch (deviceSetting)
	{
		case iPad:
			S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
			S9xSetController(1, CTL_JOYPAD,     1, 0, 0, 0);
			macControllerOption = SNES_JOYPAD;
			break;
		
		case iMouse:
			S9xSetController(0, CTL_MOUSE,      0, 0, 0, 0);
			S9xSetController(1, CTL_JOYPAD,     1, 0, 0, 0);
			macControllerOption = SNES_MOUSE;
			break;
			
		case iMouse2:
			S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
			S9xSetController(1, CTL_MOUSE,      1, 0, 0, 0);
			macControllerOption = SNES_MOUSE_SWAPPED;
			break;
			
		case iSuperScope:
			S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
			S9xSetController(1, CTL_SUPERSCOPE, 0, 0, 0, 0);
			macControllerOption = SNES_SUPERSCOPE;
			break;
			
		case iMultiPlayer5:
			S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
			S9xSetController(1, CTL_MP5,        1, 2, 3, 4);
			macControllerOption = SNES_MULTIPLAYER5;
			break;
			
		case iMultiPlayer5_2:
			S9xSetController(0, CTL_MP5,        0, 1, 2, 3);
			S9xSetController(1, CTL_MP5,        4, 5, 6, 7);
			macControllerOption = SNES_MULTIPLAYER5_2;
			break;
			
		case iJustifier1:
			S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
			S9xSetController(1, CTL_JUSTIFIER,  0, 0, 0, 0);
			macControllerOption = SNES_JUSTIFIER;
			break;
			
		case iJustifier2:
			S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
			S9xSetController(1, CTL_JUSTIFIER,  1, 0, 0, 0);
			macControllerOption = SNES_JUSTIFIER_2;
	}
}

int PromptFreezeDefrost(Boolean freezing)
{
	OSStatus			err;
	CGContextRef		ctx;
	CGColorSpaceRef		color;
	CGDataProviderRef	prov;
	CGImageRef			image;
	CGRect				rct;
	CGPoint				pt;
	CFURLRef			url;
	FSCatalogInfo		info;
	FSRef				ref;
	KeyMap				keys;
	SndListHandle		sound;
	UInt64				newestDate, currentDate;
	float				x, y, textw;
	unsigned long		startTime;
	unsigned int		repeatDelay;
	long				newestIndex;
	short				current_selection;
	int					count, result;
	int					oldInactiveMode;
	uint8				*back, *draw;
	char				dateC[256];

	const char			*filename;
	const char			letters[]  = "123456789ABC";
	const int			w = SNES_WIDTH << 1, h = kMacWindowHeight;
	const uint8			keyCheck[] = { kmEscKey, km1Key, km2Key, km3Key, km4Key, km5Key, km6Key,
												 km7Key, km8Key, km9Key, kmAKey, kmBKey, kmCKey };

	if (!directDisplay)
	{
		S9xInitDisplay(nil, nil);
		SNES9X_Go();
	}
	
	frzselecting = true;

	oldInactiveMode = inactiveMode;
	if (inactiveMode == 3)
		inactiveMode = 2;

	S9xSetSoundMute(true);

	back = (uint8 *) malloc(w * h * 2);
	draw = (uint8 *) malloc(w * h * 2);
	if (!back || !draw)
		QuitWithFatalError(0, "os 04");

	color = CGColorSpaceCreateDeviceRGB();
	if (!color)
		QuitWithFatalError(0, "os 05");
	
	ctx = CGBitmapContextCreate(back, w, h, 5, w * 2, color, kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrder16Host : 0));
	if (!ctx)
		QuitWithFatalError(0, "os 06");

	rct = CGRectMake(0.0, 0.0, (float) w, (float) h);
	CGContextClearRect(ctx, rct);

	image = nil;

	if (freezing)
		url = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("logo_freeze"),  CFSTR("png"), nil);
	else
		url = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("logo_defrost"), CFSTR("png"), nil);
	if (url)
	{
		prov = CGDataProviderCreateWithURL(url);
		if (prov)
		{
			image = CGImageCreateWithPNGDataProvider(prov, nil, true, kCGRenderingIntentDefault);
			CGDataProviderRelease(prov);
		}
		
		CFRelease(url);
	}

	if (image)
	{
		rct = CGRectMake(0.0, (float) h - 118.0, 512.0, 118.0);
		CGContextDrawImage(ctx, rct, image);
		CGImageRelease(image);
	}
	
	newestDate  = 0;
	newestIndex = -1;
	
	CGContextSetLineJoin(ctx, kCGLineJoinRound);

	rct = CGRectMake(0.0, (float) h - 238.0, 128.0, 120.0);

	for (count = 0; count < 12; count++)
	{
		filename = S9xGetFreezeFilename(count);
		err = FSPathMakeRef((unsigned char *) filename, &ref, nil);
		if (err == noErr)
		{
			err = FSGetCatalogInfo(&ref, kFSCatInfoContentMod, &info, nil, nil, nil);
			currentDate = (((UInt64) info.contentModDate.highSeconds << 48) |
						   ((UInt64) info.contentModDate.lowSeconds  << 16) |
						    (UInt64) info.contentModDate.fraction);
			
			if (currentDate > newestDate)
			{
				newestIndex = count;
				newestDate  = currentDate;
			}
			
			DrawThumbnailPICTResource(&ref, ctx, rct);

			CGContextSetShouldAntialias(ctx, false);
			CGContextSetLineWidth(ctx, 1.0);

			CGContextSetRGBStrokeColor(ctx, 0.0, 0.0, 0.0, 1.0);
			x = rct.origin.x + rct.size.width - 1.0;
			y = rct.origin.y + 119.0;
			CGContextBeginPath(ctx);
			CGContextMoveToPoint(ctx, x, y);
			CGContextAddLineToPoint(ctx, x,         y - 119.0);
			CGContextAddLineToPoint(ctx, x - 127.0, y - 119.0);
			CGContextStrokePath(ctx);

			CGContextSetShouldAntialias(ctx, true);
			CGContextSetLineWidth(ctx, 3.0);

			CGContextSelectFont(ctx, "Helvetica", 12.0, kCGEncodingMacRoman);
			x = rct.origin.x +   5.0;
			y = rct.origin.y + 107.0;
			CGContextSetTextDrawingMode(ctx, kCGTextStroke);
			CGContextSetRGBStrokeColor(ctx, 0.0, 0.0, 0.0, 0.8);
			CGContextShowTextAtPoint(ctx, x, y, &letters[count], 1);
			CGContextSetTextDrawingMode(ctx, kCGTextFill);
			CGContextSetRGBFillColor(ctx, 1.0, 0.7, 0.7, 1.0);
			CGContextShowTextAtPoint(ctx, x, y, &letters[count], 1);

			if (showtimeinfrz)
			{
				if (systemVersion >= 0x1030)
				{
					CFAbsoluteTime		at;
					CFDateFormatterRef	format;
					CFLocaleRef			locale;
					CFStringRef			datstr;
					Boolean				r;
					
					err = UCConvertUTCDateTimeToCFAbsoluteTime(&(info.contentModDate), &at);
					locale = CFLocaleCopyCurrent();
					format = CFDateFormatterCreate(kCFAllocatorDefault, locale, kCFDateFormatterShortStyle, kCFDateFormatterMediumStyle);
					datstr = CFDateFormatterCreateStringWithAbsoluteTime(kCFAllocatorDefault, format, at);				
					r = CFStringGetCString(datstr, dateC, sizeof(dateC), CFStringGetSystemEncoding());
					CFRelease(datstr);
					CFRelease(format);
					CFRelease(locale);
				}
			#ifdef MAC_JAGUAR_SUPPORT
				else
				{
					LocalDateTime	localtime;
					LongDateTime	ldtime;
					unsigned char	dateP[256], p1[256], p2[256];
					
					ConvertUTCToLocalDateTime(&(info.contentModDate), &localtime);			
					ldtime = (LongDateTime) ((((UInt64) localtime.highSeconds) << 32) | ((UInt64) localtime.lowSeconds));
					LongDateString(&ldtime, shortDate, p1, nil);
					LongTimeString(&ldtime, true, p2, nil);
					JointPStrings(p1, "\p ", p2, dateP);
					ConvertPString(dateP, dateC);
				}
			#endif
				CGContextSelectFont(ctx, "Helvetica", 10.0, kCGEncodingMacRoman);
				x = rct.origin.x +  20.0;
				y = rct.origin.y + 107.0;
				CGContextSetTextDrawingMode(ctx, kCGTextInvisible);
				CGContextShowTextAtPoint(ctx, x, y, dateC, strlen(dateC));
				pt = CGContextGetTextPosition(ctx);
				textw = pt.x - x;
				x = rct.origin.x + 122.0 - textw;
				CGContextSetTextDrawingMode(ctx, kCGTextStroke);
				CGContextSetRGBStrokeColor(ctx, 0.0, 0.0, 0.0, 0.8);
				CGContextShowTextAtPoint(ctx, x, y, dateC, strlen(dateC));
				CGContextSetTextDrawingMode(ctx, kCGTextFill);
				CGContextSetRGBFillColor(ctx, 1.0, 1.0, 1.0, 1.0);
				CGContextShowTextAtPoint(ctx, x, y, dateC, strlen(dateC));
			}
		}
		else
		{
			CGContextSelectFont(ctx, "Helvetica", 12.0, kCGEncodingMacRoman);
			x = rct.origin.x +   5.0;
			y = rct.origin.y + 107.0;
			CGContextSetTextDrawingMode(ctx, kCGTextFill);
			CGContextSetRGBFillColor(ctx, 0.7, 0.7, 0.7, 1.0);
			CGContextShowTextAtPoint(ctx, x, y, &letters[count], 1);
		}

		if ((count % 4) == 3)
			rct = CGRectOffset(rct, -128.0 * 3.0, -120.0);
		else
			rct = CGRectOffset(rct, 128.0, 0.0);
	}

	if (newestIndex < 0)
		newestIndex = 0;

	CGContextRelease(ctx);

	image = nil;

	prov = CGDataProviderCreateWithData(nil, back, w * h * 2, nil);
	if (prov)
	{
		image = CGImageCreate(w, h, 5, 16, w * 2, color, kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrder16Host : 0), prov, nil, 0, kCGRenderingIntentDefault);
		CGDataProviderRelease(prov);
	}

	if (!image)
		QuitWithFatalError(0, "os 07");

	ctx = CGBitmapContextCreate(draw, w, h, 5, w * 2, color, kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrder16Host : 0));
	if (!ctx)
		QuitWithFatalError(0, "os 08");

	CGContextSetShouldAntialias(ctx, false);

	UpdateFreezeDefrostScreen(newestIndex, image, draw, ctx);

	sound = (SndListHandle) GetResource('snd ', 128);
	if (sound)
	{
		SndPlay(nil, sound, false);
		ReleaseResource((Handle) sound);
	}
	
	result = -2;
	repeatDelay = 10;
	current_selection = (newestIndex == -1 ? 0 : newestIndex);

	do
	{
		if (!rejectinput)
		{
			GetKeys(keys);
			for (count = 0; count <= 12; count++)
			{
				while (KeyIsPressed(keys, keyCheck[count]))
				{
					result = count - 1;
					GetKeys(keys);
				}	
			}
			
			while (KeyIsPressed(keys, keyCode[k1PRight]))
			{
				startTime = TickCount();
				current_selection += 1;
				if (current_selection > 11)
					current_selection -= 12;
				UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
				while (KeyIsPressed(keys, keyCode[k1PRight]) && (TickCount() < (startTime + repeatDelay)))
					GetKeys(keys);
			}

			while (KeyIsPressed(keys, keyCode[k1PLeft]))
			{
				startTime = TickCount();
				current_selection -= 1;
				if (current_selection < 0)
					current_selection += 12;
				UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
				while (KeyIsPressed(keys, keyCode[k1PLeft]) && (TickCount() < (startTime + repeatDelay)))
					GetKeys(keys);
			}
			
			while (KeyIsPressed(keys, keyCode[k1PDown]))
			{
				startTime = TickCount();
				current_selection += 4;
				if (current_selection > 11)
					current_selection -= 12;
				UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
				while (KeyIsPressed(keys, keyCode[k1PDown]) && (TickCount() < (startTime + repeatDelay)))
					GetKeys(keys);
			}
			
			while (KeyIsPressed(keys, keyCode[k1PUp]))
			{
				startTime = TickCount();
				current_selection -= 4;
				if (current_selection < 0)
					current_selection += 12;
				UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
				while (KeyIsPressed(keys, keyCode[k1PUp]) && (TickCount() < (startTime + repeatDelay)))
					GetKeys(keys);
			}

			while (KeyIsPressed(keys, keyCode[k1PA]     ) ||
				   KeyIsPressed(keys, keyCode[k2PA]     ) ||
				   KeyIsPressed(keys, keyCode[k1PB]     ) ||
				   KeyIsPressed(keys, keyCode[k2PB]     ) ||
				   KeyIsPressed(keys, keyCode[k1PX]     ) ||
				   KeyIsPressed(keys, keyCode[k2PX]     ) ||
				   KeyIsPressed(keys, keyCode[k1PY]     ) ||
				   KeyIsPressed(keys, keyCode[k2PY]     ) ||
				   KeyIsPressed(keys, keyCode[k1PStart] ) ||
				   KeyIsPressed(keys, keyCode[k1PSelect]) ||
				   KeyIsPressed(keys, keyCode[k2PStart] ) ||
				   KeyIsPressed(keys, keyCode[k2PSelect]))
			{
				GetKeys(keys);
				result = current_selection;
			}
			
			uint32	pad1 = 0, pad2 = 0;
			
			while (ISpKeyIsPressed(kISpEsc    ) ||
				   ISpKeyIsPressed(kISp1PStart) ||
				   ISpKeyIsPressed(kISp2PStart))
				result = -1;
			
			pad1 = pad2 = 0;
			JoypadScanDirection(0, &pad1);
			JoypadScanDirection(1, &pad2);
			while ((pad1 & 0x0100) || (pad2 & 0x0100))	// Rt
			{
				startTime = TickCount();
				current_selection += 1;
				if (current_selection > 11)
					current_selection -= 12;
				UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
				do
				{
					pad1 = pad2 = 0;
					JoypadScanDirection(0, &pad1);
					JoypadScanDirection(1, &pad2);
				}	
				while (((pad1 & 0x0100) || (pad2 & 0x0100)) && (TickCount() < (startTime + repeatDelay)));
			}
			
			pad1 = pad2 = 0;
			JoypadScanDirection(0, &pad1);
			JoypadScanDirection(1, &pad2);
			while ((pad1 & 0x0200) || (pad2 & 0x0200))	// Lf
			{
				startTime = TickCount();
				current_selection -= 1;
				if (current_selection < 0) 
					current_selection += 12;
				UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
				do
				{
					pad1 = pad2 = 0;
					JoypadScanDirection(0, &pad1);
					JoypadScanDirection(1, &pad2);
				}	
				while (((pad1 & 0x0200) || (pad2 & 0x0200)) && (TickCount() < (startTime + repeatDelay)));
			}
			
			pad1 = pad2 = 0;
			JoypadScanDirection(0, &pad1);
			JoypadScanDirection(1, &pad2);
			while ((pad1 & 0x0800) || (pad2 & 0x0800))	// Up
			{
				startTime = TickCount();
				current_selection -= 4;
				if (current_selection < 0) 
					current_selection += 12;
				UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
				do
				{
					pad1 = pad2 = 0;
					JoypadScanDirection(0, &pad1);
					JoypadScanDirection(1, &pad2);
				}	
				while (((pad1 & 0x0800) || (pad2 & 0x0800)) && (TickCount() < (startTime + repeatDelay)));
			}
				
			pad1 = pad2 = 0;
			JoypadScanDirection(0, &pad1);
			JoypadScanDirection(1, &pad2);
			while ((pad1 & 0x0400) || (pad2 & 0x0400))	// Dn
			{
				startTime = TickCount();
				current_selection += 4;
				if (current_selection > 11)
					current_selection -= 12;
				UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
				do
				{
					pad1 = pad2 = 0;
					JoypadScanDirection(0, &pad1);
					JoypadScanDirection(1, &pad2);
				}	
				while (((pad1 & 0x0400) || (pad2 & 0x0400)) && (TickCount() < (startTime + repeatDelay)));
			}
			
			while (ISpKeyIsPressed(kISp1PA) ||
				   ISpKeyIsPressed(kISp2PA) ||
				   ISpKeyIsPressed(kISp1PB) ||
				   ISpKeyIsPressed(kISp2PB) ||
				   ISpKeyIsPressed(kISp1PX) ||
				   ISpKeyIsPressed(kISp2PX) ||
				   ISpKeyIsPressed(kISp1PY) ||
				   ISpKeyIsPressed(kISp2PY))
				result = current_selection;
		}

		usleep(30000);
		
		windowResizeCount = 2;
		UpdateFreezeDefrostScreen(current_selection, image, draw, ctx);
	}
	while (result == -2);
	
	sound = (SndListHandle) GetResource('snd ', 128);
	if (sound)
	{
		SndPlay(nil, sound, false);
		ReleaseResource((Handle) sound);
	}

	CGContextRelease(ctx);
	CGImageRelease(image);
	CGColorSpaceRelease(color);
	free(draw);
	free(back);

	S9xSetSoundMute(!Settings.APUEnabled);
	
	frzselecting = false;
	
	inactiveMode = oldInactiveMode;
	
	return result;
}

static void UpdateFreezeDefrostScreen(int newIndex, CGImageRef image, uint8 *draw, CGContextRef ctx)
{
	if (newIndex >= 0 && newIndex < 12)
	{
		CGRect	rct;

		const int	w = SNES_WIDTH << 1, h = kMacWindowHeight;

		CGContextSetLineWidth(ctx, 1.0);

		rct = CGRectMake(0.0, 0.0, (float) w, (float) h);
		CGContextDrawImage(ctx, rct, image);

		rct = CGRectMake(0.0, (float) h - 238.0, 128.0, 120.0);
		rct = CGRectOffset(rct, (float) (128 * (newIndex % 4)), (float) (-120 * (newIndex / 4)));
		rct.size.width  -= 1.0;
		rct.size.height -= 1.0;

		CGContextSetRGBStrokeColor(ctx, 1.0, 1.0, 0.0, 1.0);
		CGContextStrokeRect(ctx, rct);
		rct = CGRectInset(rct, 1.0, 1.0);
		CGContextSetRGBStrokeColor(ctx, 0.0, 0.0, 0.0, 1.0);
		CGContextStrokeRect(ctx, rct);
	}

	DrawFreezeDefrostScreen(draw);
}

static void ProcessInput(void)
{
	KeyMap	myKeys;
	bool8   isok;
	bool8	fnbtn, altbtn, tcbtn;
	
	static bool8	lastTimeTT = false,
					lastTimeFn = false,
					ffUp       = false,
					ffDown     = false,
					ffUpSp     = false,
					ffDownSp   = false;
	
	if (rejectinput)
		return;
	
	if (ISpKeyIsPressed(kISpEsc))
	{
		if (s9xthreadrunning)
		{
			if (!eventQueued)
			{
				PostQueueToSubEventLoop();
				eventQueued = true;
			}
		}
		else
			running = false;
		
		return;
	}

	if (ISpKeyIsPressed(kISpFreeze))
	{
		MacStopSound();
		while (ISpKeyIsPressed(kISpFreeze));
		
		isok = SNES9X_Freeze();
		ClearGFXScreen();
		return;
	}

	if (ISpKeyIsPressed(kISpDefrost))
	{
		MacStopSound();
		while (ISpKeyIsPressed(kISpDefrost));
		
		isok = SNES9X_Defrost();
		ClearGFXScreen();
		return;
	}

	if (ISpKeyIsPressed(kISpScreenshot))
	{
		Settings.TakeScreenshot = true;
		while (ISpKeyIsPressed(kISpScreenshot));
	}
	
	if (ISpKeyIsPressed(kISpSPC))
	{
		spc_is_dumping = 1;
		while (ISpKeyIsPressed(kISpSPC));
	}
	
	if (ISpKeyIsPressed(kISpFFUp))
	{
		if (!ffUpSp)
		{
			ChangeTurboRate(+1);
			ffUpSp = true;
		}
	}
	else
		ffUpSp = false;
	
	if (ISpKeyIsPressed(kISpFFDown))
	{
		if (!ffDownSp)
		{
			ChangeTurboRate(-1);
			ffDownSp = true;
		}
	}
	else
		ffDownSp = false;

	controlPad[0] = controlPad[1] = 0;

	JoypadScanDirection(0, &(controlPad[0]));
	if (ISpKeyIsPressed(kISp1PR     ))	controlPad[0] |= 0x0010;
	if (ISpKeyIsPressed(kISp1PL     ))	controlPad[0] |= 0x0020;
	if (ISpKeyIsPressed(kISp1PX     ))	controlPad[0] |= 0x0040;
	if (ISpKeyIsPressed(kISp1PA     ))	controlPad[0] |= 0x0080;
	if (ISpKeyIsPressed(kISp1PStart ))	controlPad[0] |= 0x1000;
	if (ISpKeyIsPressed(kISp1PSelect))	controlPad[0] |= 0x2000;
	if (ISpKeyIsPressed(kISp1PY     ))	controlPad[0] |= 0x4000;
	if (ISpKeyIsPressed(kISp1PB     ))	controlPad[0] |= 0x8000; 

	JoypadScanDirection(1, &(controlPad[1]));
	if (ISpKeyIsPressed(kISp2PR     ))	controlPad[1] |= 0x0010;
	if (ISpKeyIsPressed(kISp2PL     ))	controlPad[1] |= 0x0020;
	if (ISpKeyIsPressed(kISp2PX     ))	controlPad[1] |= 0x0040;
	if (ISpKeyIsPressed(kISp2PA     ))	controlPad[1] |= 0x0080;
	if (ISpKeyIsPressed(kISp2PStart ))	controlPad[1] |= 0x1000;
	if (ISpKeyIsPressed(kISp2PSelect))	controlPad[1] |= 0x2000;
	if (ISpKeyIsPressed(kISp2PY     ))	controlPad[1] |= 0x4000;
	if (ISpKeyIsPressed(kISp2PB     ))	controlPad[1] |= 0x8000;
		
	if (((macControllerOption == SNES_MULTIPLAYER5) || (macControllerOption == SNES_MULTIPLAYER5_2)) && Settings.MultiPlayer5Master)
	{
		controlPad[2] = controlPad[3] = controlPad[4] = 0;

		JoypadScanDirection(2, &(controlPad[2]));
		if (ISpKeyIsPressed(kISp3PR     ))	controlPad[2] |= 0x0010;
		if (ISpKeyIsPressed(kISp3PL     ))	controlPad[2] |= 0x0020;
		if (ISpKeyIsPressed(kISp3PX     ))	controlPad[2] |= 0x0040;
		if (ISpKeyIsPressed(kISp3PA     ))	controlPad[2] |= 0x0080;
		if (ISpKeyIsPressed(kISp3PStart ))	controlPad[2] |= 0x1000;
		if (ISpKeyIsPressed(kISp3PSelect))	controlPad[2] |= 0x2000;
		if (ISpKeyIsPressed(kISp3PY     ))	controlPad[2] |= 0x4000;
		if (ISpKeyIsPressed(kISp3PB     ))	controlPad[2] |= 0x8000;
	
		JoypadScanDirection(3, &(controlPad[3]));
		if (ISpKeyIsPressed(kISp4PR     ))	controlPad[3] |= 0x0010;
		if (ISpKeyIsPressed(kISp4PL     ))	controlPad[3] |= 0x0020;
		if (ISpKeyIsPressed(kISp4PX     ))	controlPad[3] |= 0x0040;
		if (ISpKeyIsPressed(kISp4PA     ))	controlPad[3] |= 0x0080;
		if (ISpKeyIsPressed(kISp4PStart ))	controlPad[3] |= 0x1000;
		if (ISpKeyIsPressed(kISp4PSelect))	controlPad[3] |= 0x2000;
		if (ISpKeyIsPressed(kISp4PY     ))	controlPad[3] |= 0x4000;
		if (ISpKeyIsPressed(kISp4PB     ))	controlPad[3] |= 0x8000;

		JoypadScanDirection(4, &(controlPad[4]));
		if (ISpKeyIsPressed(kISp5PR     ))	controlPad[4] |= 0x0010;
		if (ISpKeyIsPressed(kISp5PL     ))	controlPad[4] |= 0x0020;
		if (ISpKeyIsPressed(kISp5PX     ))	controlPad[4] |= 0x0040;
		if (ISpKeyIsPressed(kISp5PA     ))	controlPad[4] |= 0x0080;
		if (ISpKeyIsPressed(kISp5PStart ))	controlPad[4] |= 0x1000;
		if (ISpKeyIsPressed(kISp5PSelect))	controlPad[4] |= 0x2000;
		if (ISpKeyIsPressed(kISp5PY     ))	controlPad[4] |= 0x4000;
		if (ISpKeyIsPressed(kISp5PB     ))	controlPad[4] |= 0x8000;
		
		ControlPadFlagsToS9xReportButtons(2, controlPad[2]);
		ControlPadFlagsToS9xReportButtons(3, controlPad[3]);
		ControlPadFlagsToS9xReportButtons(4, controlPad[4]);
		
		if (macControllerOption == SNES_MULTIPLAYER5_2)
		{
			controlPad[5] = controlPad[6] = controlPad[7] = 0;

			JoypadScanDirection(5, &(controlPad[5]));
			if (ISpKeyIsPressed(kISp6PR     ))	controlPad[5] |= 0x0010;
			if (ISpKeyIsPressed(kISp6PL     ))	controlPad[5] |= 0x0020;
			if (ISpKeyIsPressed(kISp6PX     ))	controlPad[5] |= 0x0040;
			if (ISpKeyIsPressed(kISp6PA     ))	controlPad[5] |= 0x0080;
			if (ISpKeyIsPressed(kISp6PStart ))	controlPad[5] |= 0x1000;
			if (ISpKeyIsPressed(kISp6PSelect))	controlPad[5] |= 0x2000;
			if (ISpKeyIsPressed(kISp6PY     ))	controlPad[5] |= 0x4000;
			if (ISpKeyIsPressed(kISp6PB     ))	controlPad[5] |= 0x8000;
		
			JoypadScanDirection(6, &(controlPad[6]));
			if (ISpKeyIsPressed(kISp7PR     ))	controlPad[6] |= 0x0010;
			if (ISpKeyIsPressed(kISp7PL     ))	controlPad[6] |= 0x0020;
			if (ISpKeyIsPressed(kISp7PX     ))	controlPad[6] |= 0x0040;
			if (ISpKeyIsPressed(kISp7PA     ))	controlPad[6] |= 0x0080;
			if (ISpKeyIsPressed(kISp7PStart ))	controlPad[6] |= 0x1000;
			if (ISpKeyIsPressed(kISp7PSelect))	controlPad[6] |= 0x2000;
			if (ISpKeyIsPressed(kISp7PY     ))	controlPad[6] |= 0x4000;
			if (ISpKeyIsPressed(kISp7PB     ))	controlPad[6] |= 0x8000;

			JoypadScanDirection(7, &(controlPad[7]));
			if (ISpKeyIsPressed(kISp8PR     ))	controlPad[7] |= 0x0010;
			if (ISpKeyIsPressed(kISp8PL     ))	controlPad[7] |= 0x0020;
			if (ISpKeyIsPressed(kISp8PX     ))	controlPad[7] |= 0x0040;
			if (ISpKeyIsPressed(kISp8PA     ))	controlPad[7] |= 0x0080;
			if (ISpKeyIsPressed(kISp8PStart ))	controlPad[7] |= 0x1000;
			if (ISpKeyIsPressed(kISp8PSelect))	controlPad[7] |= 0x2000;
			if (ISpKeyIsPressed(kISp8PY     ))	controlPad[7] |= 0x4000;
			if (ISpKeyIsPressed(kISp8PB     ))	controlPad[7] |= 0x8000;
			
			ControlPadFlagsToS9xReportButtons(5, controlPad[5]);
			ControlPadFlagsToS9xReportButtons(6, controlPad[6]);
			ControlPadFlagsToS9xReportButtons(7, controlPad[7]);
		}		
	}

	GetKeys(myKeys);

	fnbtn  = (KeyIsPressed(myKeys, keyCode[kKeyFunction]) || ISpKeyIsPressed(kISpFunction));
	altbtn = (KeyIsPressed(myKeys, keyCode[kKeyAlt]     ) || ISpKeyIsPressed(kISpAlt)     );

	if (fnbtn)
	{
		if (!lastTimeFn)
		{
			for (uint32 i = 0; i < kCommandListSize; i++)
				btncmd[i].held = false;
		}
		
		lastTimeFn = true;
		lastTimeTT = false;
		ffUp = ffDown = false;
		
		for (uint32 i = 0; i < kCommandListSize; i++)
		{    
			if (KeyIsPressed(myKeys, btncmd[i].keycode))
			{
				if (!(btncmd[i].held))
				{
					btncmd[i].held = true;
					
					s9xcommand_t	s9xcmd;
					
					s9xcmd = S9xGetCommandT(btncmd[i].command);
					S9xApplyCommand(s9xcmd, 1, 0);
				}
			}
			else
				btncmd[i].held = false;
		}
	}
	else
	{
		lastTimeFn = false;
	
		if (KeyIsPressed(myKeys, keyCode[kKeyEsc]))
		{	
			if (s9xthreadrunning)
			{
				if (!eventQueued)
				{
					PostQueueToSubEventLoop();
					eventQueued = true;
				}
			}
			else
				running = false;
	
			return;
		}
		
		if (KeyIsPressed(myKeys, keyCode[kKeyFreeze]))
		{
			MacStopSound();
			while (KeyIsPressed(myKeys, keyCode[kKeyFreeze]))
				GetKeys(myKeys);
			
			isok = SNES9X_Freeze();
			ClearGFXScreen();
			return;
		}
			
		if (KeyIsPressed(myKeys, keyCode[kKeyDefrost]))
		{
			MacStopSound();
			while (KeyIsPressed(myKeys, keyCode[kKeyDefrost]))
				GetKeys(myKeys);
			
			isok = SNES9X_Defrost();
			ClearGFXScreen();
			return;
		}

		if (KeyIsPressed(myKeys, keyCode[kKeyScreenshot]))
		{
			Settings.TakeScreenshot = true;
			while (KeyIsPressed(myKeys, keyCode[kKeyScreenshot]))
				GetKeys(myKeys);
		}
		
		if (KeyIsPressed(myKeys, keyCode[kKeySPC]))
		{
			spc_is_dumping = 1;
			while (KeyIsPressed(myKeys, keyCode[kKeySPC]))
				GetKeys(myKeys);
		}
		
		if (KeyIsPressed(myKeys, keyCode[kKeyFFUp]))
		{
			if (!ffUp)
			{
				ChangeTurboRate(+1);
				ffUp = true;
			}
		}
		else
			ffUp = false;

		if (KeyIsPressed(myKeys, keyCode[kKeyFFDown]))
		{
			if (!ffDown)
			{
				ChangeTurboRate(-1);
				ffDown = true;
			}
		}
		else
			ffDown = false;

		if (KeyIsPressed(myKeys, keyCode[k1PR]     ))	controlPad[0] |= 0x0010;
		if (KeyIsPressed(myKeys, keyCode[k1PL]     ))	controlPad[0] |= 0x0020;
		if (KeyIsPressed(myKeys, keyCode[k1PX]     ))	controlPad[0] |= 0x0040;
		if (KeyIsPressed(myKeys, keyCode[k1PA]     ))	controlPad[0] |= 0x0080;
		if (KeyIsPressed(myKeys, keyCode[k1PRight] ))	controlPad[0] |= 0x0100;
		if (KeyIsPressed(myKeys, keyCode[k1PLeft]  ))	controlPad[0] |= 0x0200;
		if (KeyIsPressed(myKeys, keyCode[k1PDown]  ))	controlPad[0] |= 0x0400;
		if (KeyIsPressed(myKeys, keyCode[k1PUp]    ))	controlPad[0] |= 0x0800;
		if (KeyIsPressed(myKeys, keyCode[k1PStart] ))	controlPad[0] |= 0x1000;
		if (KeyIsPressed(myKeys, keyCode[k1PSelect]))	controlPad[0] |= 0x2000;
		if (KeyIsPressed(myKeys, keyCode[k1PY]     ))	controlPad[0] |= 0x4000;
		if (KeyIsPressed(myKeys, keyCode[k1PB]     ))	controlPad[0] |= 0x8000;
		
		if (KeyIsPressed(myKeys, keyCode[k2PR]     ))	controlPad[1] |= 0x0010;
		if (KeyIsPressed(myKeys, keyCode[k2PL]     ))	controlPad[1] |= 0x0020;
		if (KeyIsPressed(myKeys, keyCode[k2PX]     ))	controlPad[1] |= 0x0040;
		if (KeyIsPressed(myKeys, keyCode[k2PA]     ))	controlPad[1] |= 0x0080;
		if (KeyIsPressed(myKeys, keyCode[k2PRight] ))	controlPad[1] |= 0x0100;
		if (KeyIsPressed(myKeys, keyCode[k2PLeft]  ))	controlPad[1] |= 0x0200;
		if (KeyIsPressed(myKeys, keyCode[k2PDown]  ))	controlPad[1] |= 0x0400;
		if (KeyIsPressed(myKeys, keyCode[k2PUp]    ))	controlPad[1] |= 0x0800;
		if (KeyIsPressed(myKeys, keyCode[k2PStart] ))	controlPad[1] |= 0x1000;
		if (KeyIsPressed(myKeys, keyCode[k2PSelect]))	controlPad[1] |= 0x2000;
		if (KeyIsPressed(myKeys, keyCode[k2PY]     ))	controlPad[1] |= 0x4000;
		if (KeyIsPressed(myKeys, keyCode[k2PB]     ))	controlPad[1] |= 0x8000;
		
		if (altbtn)
		{
			if (!lastTimeTT)
				changeAuto[0] = changeAuto[1] = 0;
			
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 12; j++)
				{
					uint16	mask = 0x0010 << j;
					
					if (controlPad[i] & mask & autofireRec[i].toggleMask)
					{
						controlPad[i] &= ~mask;
						
						if (!(changeAuto[i] & mask))
						{
							changeAuto[i] |= mask;
							ChangeAutofireSettings(i, j);
						}
					}
					else
						changeAuto[i] &= ~mask;
				}
			}
			
			lastTimeTT = true;
		}
		else
			lastTimeTT = false;
	}

	if (enabletoggle)
	{
		if ((ISpKeyIsPressed(kISpFastForward) || KeyIsPressed(myKeys, keyCode[kKeyFastForward])) && !fnbtn)
		{
			if (!toggleff)
			{
				toggleff = 1;
				Settings.TurboMode = !Settings.TurboMode;
				S9xSetInfoString(Settings.TurboMode ? "Turbo mode on" : "Turbo mode off");
			}
		}
		else
			toggleff = 0;
	}
	else
		Settings.TurboMode = ((ISpKeyIsPressed(kISpFastForward) || KeyIsPressed(myKeys, keyCode[kKeyFastForward])) && !fnbtn) ? true : false;

	for (int i = 0; i < 2; i++)
		controlPad[i] ^= autofireRec[i].invertMask;

	if (autofire)
	{
		long long	currentTime;
		uint16		changeMask;
		
		Microseconds((UnsignedWide *) &currentTime);
		tcbtn  = (KeyIsPressed(myKeys, keyCode[kKeyTC]) || ISpKeyIsPressed(kISpTC));
		
		for (int i = 0; i < 2; i++)
		{
			changeMask = (lastTimeTT ? (~changeAuto[i]) : 0xFFFF);
			
			for (int j = 0; j < 12; j++)
			{
				uint16	mask = (0x0010 << j) & changeMask;
				
				if (autofireRec[i].tcMask & mask)
				{
					if (!tcbtn)
						continue;
				}
				
				if (autofireRec[i].buttonMask & mask)
				{
					if (controlPad[i] & mask)
					{
						if (currentTime > autofireRec[i].nextTime[j])
						{
							if (Settings.TurboMode)
								autofireRec[i].nextTime[j] = currentTime + (long long) ((1.0 / (float) autofireRec[i].frequency) * 1000000.0 / macFastForwardRate);
							else
								autofireRec[i].nextTime[j] = currentTime + (long long) ((1.0 / (float) autofireRec[i].frequency) * 1000000.0);
						}
						else
							controlPad[i] &= ~mask;
					}
				}
			}
		}
	}
	
	ControlPadFlagsToS9xReportButtons(0, controlPad[0]);
	ControlPadFlagsToS9xReportButtons(1, controlPad[1]);
	
	if (macControllerOption == SNES_JUSTIFIER_2)
		ControlPadFlagsToS9xPseudoPointer(controlPad[1]);
}

static void ChangeAutofireSettings(int player, int btn)
{
	uint16		mask, m;
	static char	msg[64];
	
	mask = 0x0010 << btn;
	autofireRec[player].buttonMask ^= mask;
	autofire = (autofireRec[0].buttonMask || autofireRec[1].buttonMask);
	
	m = autofireRec[player].buttonMask;
	if (m)
		snprintf(msg, sizeof(msg), "Autofire %d:%s%s%s%s%s%s%s%s%s%s%s%s%s", player + 1,
			(m & 0xC0F0 ?   " " : ""),
			(m & 0x0080 ?   "A" : ""),
			(m & 0x8000 ?   "B" : ""),
			(m & 0x0040 ?   "X" : ""),
			(m & 0x4000 ?   "Y" : ""),
			(m & 0x0020 ?   "L" : ""),
			(m & 0x0010 ?   "R" : ""),
			(m & 0x0800 ? " Up" : ""),
			(m & 0x0400 ? " Dn" : ""),
			(m & 0x0200 ? " Lf" : ""),
			(m & 0x0100 ? " Rt" : ""),
			(m & 0x1000 ? " St" : ""),
			(m & 0x2000 ? " Se" : ""));
	else
		snprintf(msg, sizeof(msg), "Autofire %d: Off", player + 1);
	
	S9xSetInfoString(msg);
}

static void ChangeTurboRate(int d)
{
	static char	msg[64];
	
	macFastForwardRate += d;
	if (macFastForwardRate < 1)
		macFastForwardRate = 1;
	else
	if (macFastForwardRate > 15)
		macFastForwardRate = 15;
	
	snprintf(msg, sizeof(msg), "Turbo Rate: %d", macFastForwardRate);
	S9xSetInfoString(msg);
}

void GetGameScreenPointer(int16 *x, int16 *y, bool fullmouse)
{
	int	ph;
	
	ph = !drawoverscan ? ((IPPU.RenderedScreenHeight > 256) ? IPPU.RenderedScreenHeight : (IPPU.RenderedScreenHeight << 1)) : (SNES_HEIGHT_EXTENDED << 1);
	
	if ((drawingMethod == kDrawingOpenGL) || (drawingMethod == kDrawingBlitGL))
	{
		if (fullscreen)
		{
			if (glstretch)
			{
				float   fpw = (float) glScreenH / (float) ph * 512.0;
				
				scopeViewInfo.width      = (int) (fpw + ((float) glScreenW - fpw) * (float) macAspectRatio / 100.0);
				scopeViewInfo.height     = glScreenH;
				scopeViewInfo.globalLeft = (int) glScreenBounds.origin.x + ((glScreenW - scopeViewInfo.width) >> 1);
				scopeViewInfo.globalTop  = (int) glScreenBounds.origin.y;
			}
			else
			{
				scopeViewInfo.width      = 512;
				scopeViewInfo.height     = ph;
				scopeViewInfo.globalLeft = (int) glScreenBounds.origin.x + ((glScreenW - 512) >> 1);
				scopeViewInfo.globalTop  = (int) glScreenBounds.origin.y + ((glScreenH - ph ) >> 1);
			}
		}
		else
		{
			GetWindowBounds(gWindow, kWindowContentRgn, &gWindowRect);

			int	ww = gWindowRect.right  - gWindowRect.left,
				wh = gWindowRect.bottom - gWindowRect.top;
			
			scopeViewInfo.width      = ww;
			scopeViewInfo.globalLeft = gWindowRect.left;
			
			if (windowExtend)
			{
				scopeViewInfo.height    = ph * wh / kMacWindowHeight;
				scopeViewInfo.globalTop = gWindowRect.top + ((kMacWindowHeight - ph) >> 1) * wh / kMacWindowHeight;
			}
			else
			{
				scopeViewInfo.height    = wh;
				scopeViewInfo.globalTop = gWindowRect.top;
			}
		}
	}
	else
	{
		if (fullscreen)
		{
			if ((IPPU.RenderedScreenWidth <= 256) && !doubleSize)
			{
				scopeViewInfo.width      = 256;
				scopeViewInfo.height     = !drawoverscan ? IPPU.RenderedScreenHeight : SNES_HEIGHT_EXTENDED;
				scopeViewInfo.globalLeft = (int) glScreenBounds.origin.x + ((glScreenW - 256) >> 1);
				scopeViewInfo.globalTop  = (int) glScreenBounds.origin.y + ((glScreenH - scopeViewInfo.height) >> 1);
			}
			else
			{
				scopeViewInfo.width      = 512;
				scopeViewInfo.height     = ph;
				scopeViewInfo.globalLeft = (int) glScreenBounds.origin.x + ((glScreenW - 512) >> 1);
				scopeViewInfo.globalTop  = (int) glScreenBounds.origin.y + ((glScreenH - ph ) >> 1);
			}
		}
		else
		{
			GetWindowBounds(gWindow, kWindowContentRgn, &gWindowRect);
			
			if ((IPPU.RenderedScreenWidth <= 256) && !doubleSize)
			{
				scopeViewInfo.width      = 256;
				scopeViewInfo.height     = !drawoverscan ? IPPU.RenderedScreenHeight : SNES_HEIGHT_EXTENDED;
				scopeViewInfo.globalLeft = gWindowRect.left + 128;
				scopeViewInfo.globalTop  = gWindowRect.top  + 120 + ((SNES_HEIGHT_EXTENDED - scopeViewInfo.height) >> 1);
			}
			else
			{
				scopeViewInfo.width      = 512;
				scopeViewInfo.height     = ph;
				scopeViewInfo.globalLeft = gWindowRect.left;
				scopeViewInfo.globalTop  = gWindowRect.top + ((kMacWindowHeight - ph) >> 1);
			}
		}
	}
	
	if (!fullmouse)
	{
		Point	pos;
		
		GetGlobalMouse(&pos);

		*x = (int16) (((float) (pos.h - scopeViewInfo.globalLeft)) / ((float) scopeViewInfo.width ) * (float) IPPU.RenderedScreenWidth);
		*y = (int16) (((float) (pos.v - scopeViewInfo.globalTop )) / ((float) scopeViewInfo.height) * (float) (!drawoverscan ? IPPU.RenderedScreenHeight : SNES_HEIGHT_EXTENDED));
	}
	else
	{
		*x = (int16) (unlimitedCursor.x / (float) scopeViewInfo.width  * (float) IPPU.RenderedScreenWidth);
		*y = (int16) (unlimitedCursor.y / (float) scopeViewInfo.height * (float) (!drawoverscan ? IPPU.RenderedScreenHeight : SNES_HEIGHT_EXTENDED));
	}
}

static void Initialize(void)
{
	OSStatus	err;
	IBNibRef	menuNibRef;
	MenuRef		menu;
	int			a;
	
	printf("\nSnes9x for Mac OS X %s (%s), ", VERSION, MAC_VERSION);
#ifdef __BIG_ENDIAN__
	printf("PowerPC\n\n");
#else
	printf("Intel\n\n");
#endif

	InitCursor();
	FlushEvents(everyEvent, 0L);
	
	err = Gestalt(gestaltSystemVersion, &systemVersion);
	err = Gestalt(gestaltQuickTimeVersion, &qtVersion);
	hiToolboxVersion = GetHIToolboxVersion();

	if (qtVersion < 0x06408000)
	{
		AppearanceAlert(kAlertStopAlert, kRequiredSystemWarning, kRequiredSystemHint);
		QuitWithFatalError(0, "os 09");
	}

	printf("OS: %x  QuickTime: %x  HIToolbox: %x\n\n", (unsigned) systemVersion, (unsigned) qtVersion, (unsigned) hiToolboxVersion);
	
    ZeroMemory(&Settings, sizeof(Settings));
	
    Settings.ShutdownMaster = false;
    Settings.CyclesPercentage = 100;
	Settings.SoundSkipMethod = 0;
    Timings.H_Max = SNES_CYCLES_PER_SCANLINE;
    Timings.HBlankStart = SNES_HBLANK_START_HC;
    Settings.DisableIRQ = false;
	Settings.Paused = false;
	Settings.ForcedPause = false;
	Settings.StopEmulation = false;
	Settings.FrameAdvance = false;

    Settings.FrameTimePAL = 20000;
    Settings.FrameTimeNTSC = 16667;
    Settings.FrameTime = Settings.FrameTimeNTSC;

	Settings.MouseMaster = true;
	Settings.SuperScopeMaster = true;
	Settings.MultiPlayer5Master = true;
	Settings.JustifierMaster = true;
	
    Settings.APUEnabled = true;
    Settings.SoundPlaybackRate = 32000;
    Settings.Stereo = true;
    Settings.ReverseStereo = false;
    Settings.SixteenBitSound = true;
	Settings.SoundEnvelopeHeightReading = true;
    Settings.DisableSampleCaching = true;
	Settings.AltSampleDecode = 0;
    Settings.InterpolatedSound = true;
    Settings.NextAPUEnabled = true;
	Settings.DisableSoundEcho = false;
	Settings.DisableMasterVolume = false;
	Settings.SoundSync = false;
	Settings.SampleCatchup = false;

    Settings.Transparency = true;
    Settings.SupportHiRes = true;
    Settings.Mode7Interpolate = false;

	Settings.NetPlay = false;
	Settings.NetPlayServer = false;
    Settings.ServerName[0] = 0;
	Settings.ApplyCheats = true;
	Settings.AutoSaveDelay = 0;
	Settings.TakeScreenshot = false;
	Settings.SDD1Pack = true;
	Settings.NoPatch = false;
	Settings.TurboMode = false;
	
	Settings.BSXBootup = false;
	
	S9xMovieInit();
	
	err = RegisterAppearanceClient();
	
	for (a = 0; a < kWindowCount; a++)
	{
		windowPos[a].h = 40;
		windowPos[a].v = 80;
		windowSize[a].width  = -1.0;
		windowSize[a].height = -1.0;
	}
	
	extraOptions.benchmark = false;
	extraOptions.glForceNoTextureRectangle = false;
	extraOptions.glUseClientStrageApple = true;
	extraOptions.glUseTexturePriority = false;
	extraOptions.glStorageHint = 2;
	
	npServerIP[0] = 0;
	npName[0] = 0;
	
	InitAppleEvents();
	InitKeyboard();
	InitGraphics();
	InitAutofire();
	InitCheatFinder();
	
   	LoadPrefs();
	
	InitMacSound();
	SetUpHID();
	
	RegisterHelpBook();
	
	if (systemVersion < 0x1040)
		ciFilterEnable = false;

	err = CreateNibReference(kMacS9XCFString, &menuNibRef);
	err = SetMenuBarFromNib(menuNibRef, CFSTR("MenuBar"));
	DisposeNibReference(menuNibRef);

	EnableMenuCommand(nil, kHICommandPreferences);
	DisableMenuItem(GetMenuRef(mEdit), 0);
	
	CheckMenuItem(GetMenuRef(mCheat), iApplyCheats, applycheat);
	Settings.ApplyCheats = applycheat;
	
	err = GetMenuItemHierarchicalMenu(GetMenuRef(mControl), iISpPreset, &menu);
	CheckMenuItem(menu, padSetting, true);
	
	err = GetMenuItemHierarchicalMenu(GetMenuRef(mEmulation), iDevice, &menu);
	CheckMenuItem(menu, deviceSetting, true);
	
	DisableMenuItem(GetMenuRef(mOption), iCIFilter);
	
	InitRecentItems();
	InitRecentMenu();
	BuildRecentMenu();
	
	DrawMenuBar();
	
	autofire = (autofireRec[0].buttonMask || autofireRec[1].buttonMask) ? true : false;
	for (a = 0; a < MAC_MAX_PLAYERS; a++)
		for (int b = 0; b < 12; b++)
			autofireRec[a].nextTime[b] = 0;
	
	S9xUnmapAllControls();
	S9xSetupDefaultKeymap();
	ChangeInputDevice();

	EnterMovies(); 
	
	if (!Memory.Init() || !S9xInitAPU() || !S9xGraphicsInit())
		QuitWithFatalError(err, "os 01");
		
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

	Settings.DisableSampleCaching = true; // force true from 1.42
	
	frzselecting = false;
	
	S9xSetControllerCrosshair(X_MOUSE1, 0, nil, nil);
	S9xSetControllerCrosshair(X_MOUSE2, 0, nil, nil);
	
#ifdef MAC_COREIMAGE_SUPPORT
	if (systemVersion >= 0x1040)
	{
		InitCoreImage();
		InitCoreImageFilter();
	}
#endif
}

static void Deinitialize(void)
{
#ifdef MAC_COREIMAGE_SUPPORT
	if (systemVersion >= 0x1040)
	{
		DeinitCoreImageFilter();
		DeinitCoreImage();
	}
#endif

	ExitMovies();
	DeinitRecentMenu();
	DeinitRecentItems();
	SavePrefs();	
	ReleaseHID();
	DeinitCheatFinder();
	DeinitGraphics();
	DeinitKeyboard();		
	DeinitMacSound();
	DeinitAppleEvents();
	
	S9xGraphicsDeinit();
	S9xDeinitAPU();
	Memory.Deinit();
}

static void InitAutofire(void)
{
	autofire = false;
	
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 12; j++)
			autofireRec[i].nextTime[j] = 0;
		autofireRec[i].buttonMask = 0x0000;
		autofireRec[i].toggleMask = 0xFFF0;
		autofireRec[i].tcMask     = 0x0000;
		autofireRec[i].invertMask = 0x0000;
		autofireRec[i].frequency  = 10;
	}
}

static void ForceChangingKeyScript(void)
{
#if 0
	ScriptLanguageRecord	im;
	
	err = GetTextServiceLanguage(&im);
	//printf("S:%d, L:%d\n", im.fScript, im.fLanguage);
	if (err == noErr)
	{
		if ((im.fScript == smJapanese) && (im.fLanguage == langJapanese))
		{
			im.fScript   = smRoman;
			im.fLanguage = langEnglish;
			err = SetTextServiceLanguage(&im);
		}
	}
#else
	long	script;
	
	script = GetScriptManagerVariable(smKeyScript);
	//printf("S:%ld\n", script);
	if (script == smJapanese)
		KeyScript(smRoman | smKeyForceKeyScriptMask);
#endif
}	

void S9xSyncSpeed(void)
{
	long long	currentFrame, adjustment;
	
	if (directDisplay)
	{
		if (extraOptions.benchmark)
			IPPU.RenderThisFrame = true;
		else
		if (!macQTRecord)
		{
			if (macFrameSkip < 0)	// auto skip
			{
				skipFrames--;
				
				if (skipFrames <= 0)
				{
					adjustment = (Settings.TurboMode ? (1000000 / macFastForwardRate) : 1000000) / Memory.ROMFramesPerSecond;
					Microseconds((UnsignedWide *) &currentFrame);
					
					skipFrames = (currentFrame - lastFrame) / adjustment;
					lastFrame += frameCount * adjustment;

					if (skipFrames < 1)
						skipFrames = 1;
					else
					if (skipFrames > 7)
					{
						skipFrames = 7;
						Microseconds((UnsignedWide *) &lastFrame);
					}

					frameCount = skipFrames;
					
					if (!minimizecpu)
					{
						while (lastFrame > currentFrame)
						{
							MPYield();
							Microseconds((UnsignedWide *) &currentFrame);
						}
					}
					else
					{
						if (lastFrame > currentFrame)
							usleep(lastFrame - currentFrame);
					}

					IPPU.RenderThisFrame = true;
				}
				else
					IPPU.RenderThisFrame = false;
			}
			else					// constant
			{
				skipFrames--;
				
				if (skipFrames <= 0)
				{
					adjustment = 1000000 * macFrameSkip / Memory.ROMFramesPerSecond;
					Microseconds((UnsignedWide *) &currentFrame);
					
					if (!minimizecpu)
					{
						while (currentFrame - lastFrame < adjustment)
						{
							MPYield();
							Microseconds((UnsignedWide *) &currentFrame);
						}
					}
					else
					{
						if (currentFrame - lastFrame < adjustment)
						{
							usleep(adjustment + lastFrame - currentFrame);
							Microseconds((UnsignedWide *) &currentFrame);
						}
					}

					lastFrame = currentFrame;
					skipFrames = macFrameSkip;
					if (Settings.TurboMode)
						skipFrames *= macFastForwardRate;
					
					IPPU.RenderThisFrame = true;
				}
				else
					IPPU.RenderThisFrame = false;
			}
		}
		else
		{
			MacQTRecordFrame(IPPU.RenderedScreenWidth, IPPU.RenderedScreenHeight);

			adjustment = 1000000 / Memory.ROMFramesPerSecond;
			Microseconds((UnsignedWide *) &currentFrame);

			if (!minimizecpu)
			{
				while (currentFrame - lastFrame < adjustment)
				{
					MPYield();
					Microseconds((UnsignedWide *) &currentFrame);
				}
			}
			else
			{
				if (currentFrame - lastFrame < adjustment)
					usleep(adjustment + lastFrame - currentFrame);
			}

			lastFrame = currentFrame;
			
			IPPU.RenderThisFrame = true;
		}
	}
	else
		IPPU.RenderThisFrame = false;
}

void S9xAutoSaveSRAM(void)
{
    SNES9X_SaveSRAM();
}

void S9xMessage(int type, int number, const char *message)
{
	static char mes[256];
	
	if (!onscreeninfo)
	{
		printf("%s\n", message);
		
		if ((type == S9X_INFO) && (number == S9X_ROM_INFO))
			if (strstr(message, "checksum ok") == nil)
				AppearanceAlert(kAlertCautionAlert, kBadRomWarning, kBadRomHint);
	}
	else
	{
		strncpy(mes, message, 255);		
		S9xSetInfoString(mes);
	}
}

void S9xToggleSoundChannel(int c)
{
    static int	channel_enable = 255;
    
	if (c == 8)
		channel_enable = 255;
    else
		channel_enable ^= 1 << c;
    
	S9xSetSoundControl(channel_enable);
}

void S9xExit(void)
{
	SysBeep(1);
	
	running = false;
	cartOpen = false;

	QuitApplicationEventLoop();
}

void QuitWithFatalError(OSStatus err, const char * msg)
{
	printf("Quit. %s  err: %ld\n", msg, err);
	ExitToShell();
}
