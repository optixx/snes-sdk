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
#include "ppu.h"
#include "cheats.h"
#include "font.h"

#include "mac-prefix.h"
#include "mac-dialog.h"
#include "mac-os.h"
#include "mac-screenshot.h"
#include "mac-stringtools.h"
#include "mac-cheatfinder.h"

#define	kCFNumBytesPop			'Size'
#define	kCFViewModeRad			'Mode'
#define	kCFCompModePop			'Math'
#define kCFCompStoredRad		'RSto'
#define	kCFCompLastRad			'RLst'
#define	kCFCompThisRad			'RThs'
#define	kCFCompValueTxt			'CTxt'
#define	kCFSearchBtn			'BSea'
#define	kCFStoreValueBtn		'BSto'
#define	kCFWatchBtn				'BWat'
#define kCFDrawerBtn			'Drwr'
#define	kCFWatchAddrTxt			'WTxt'
#define	kCFRestoreBtn			'BRes'
#define	kCFRemoveBtn			'BRem'
#define	kCFAddEntryBtn			'BAdd'
#define kCFUserPane				'Pane'
#define	kCFSheetAddrTxt			'AEad'
#define	kCFSheetCurrentValueTxt	'AEcv'
#define	kCFSheetCheetValueTxt	'AEtx'
#define	kCFSheetDescriptionTxt	'AEde'
#define	kCFSheetAddBtn			'SHTa'
#define	kCFSheetCancelBtn		'SHTc'
#define	kCFListView				'List'
#define	kCFUpperViews			'UI_T'
#define	kCFLowerViews			'UI_B'

enum
{
	kCFHexadecimal = 1,
	kCFSignedDecimal,	
	kCFUnsignedDecimal
};

enum
{
	kCFCompWithStored = 1,
	kCFCompWithLast,
	kCFCompWithThis
};

enum
{
	kCFSearchEqual = 1,
	kCFSearchNotEqual,
	kCFSearchGreater,
	kCFSearchGreaterOrEqual,
	kCFSearchLess,
	kCFSearchLessOrEqual
};

Boolean	cfIsWatching = false;

extern SCheatData	Cheat;

void DisplayChar(uint16 *, uint8);

#define	MAIN_MEMORY_SIZE	0x20000

#define kControlListLinePart		172
#define kEventScrollableScrollThere	'ESST'
#define kEventCheatFinderList		'ECFL'

#define	kCheatFinderListViewClassID	CFSTR("com.snes9x.macos.snes9x.cheatfinder")

static const HIViewID	kCheatFinderListViewID = { 'CHET', 'FNDR' };

typedef struct
{
	WindowRef	main;
	WindowRef	sheet;
	WindowRef	drawer;
	HIViewRef	list;
	HIViewRef	scroll;
}	WindowData;

typedef struct
{
	HIViewRef	view;
	HIPoint		originPoint;
	HISize		lineSize;
	Boolean		inFocus;
}	ListViewData;

static void CheatFinderSearch(WindowData *);
static void CheatFinderRestoreList(WindowData *);
static void CheatFinderRemoveFromList(WindowData *);
static void CheatFinderAdjustButtons(WindowData *);
static void CheatFinderBuildResultList(void);
static void CheatFinderHandleAddEntryButton(WindowData *);
static void CheatFinderMakeValueFormat(char *);
static void CheatFinderAddEntry(SInt64, char *);
static void CheatFinderBeginAddEntrySheet(WindowData *);
static void CheatFinderListViewScrollToThere(float, ListViewData *);
static void CheatFinderListViewDraw(CGContextRef, HIRect *, ListViewData *);
static float CheatFinderListViewSanityCheck(float, ListViewData *);
static SInt64 CheatFinderReadBytes(UInt8 *, UInt32);
static SInt64 CheatFinderGetValueEditText(ControlRef);
static Boolean CheatFinderCompare(SInt64, SInt64);
static HIViewPartCode CheatFinderListViewFindPart(EventRef, ListViewData *, SInt32 *);
static pascal OSStatus CheatFinderListViewHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus CheatFinderSheetEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus CheatFinderWindowEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus CheatFinderListFrameEventHandler(EventHandlerCallRef, EventRef, void *);

static UInt8	*cfStoredRAM;
static UInt8	*cfLastRAM;
static UInt8	*cfCurrentRAM;
static UInt8	*cfStatusFlag;
static UInt32	*cfAddress;

static SInt32	cfNumRows;
static SInt32	cfListSelection;

static SInt32	cfViewMode;
static SInt32	cfCompMode;
static SInt32	cfCompWith;
static UInt32	cfViewNumBytes;
static UInt32	cfWatchAddr;
static Boolean	cfIsNewGame;
static Boolean	cfIsStored;
static Boolean	cfDrawerShow;

static int			cfListAddrColumnWidth;
static char			cfWatchTextFormat[32];
static ATSUStyle	cfListLineATSUStyle;


void InitCheatFinder(void)
{
	OSStatus	err;
	
	cfStoredRAM  = new UInt8  [MAIN_MEMORY_SIZE + 10];
	cfLastRAM    = new UInt8  [MAIN_MEMORY_SIZE + 10];
	cfCurrentRAM = new UInt8  [MAIN_MEMORY_SIZE + 10];
	cfStatusFlag = new UInt8  [MAIN_MEMORY_SIZE + 10];
	cfAddress    = new UInt32 [MAIN_MEMORY_SIZE + 10];
	
	if (!cfStoredRAM || !cfLastRAM || !cfCurrentRAM || !cfStatusFlag || !cfAddress)
		QuitWithFatalError(0, "cheatfinder 01");
		
	memset(cfCurrentRAM, 0x00, MAIN_MEMORY_SIZE + 10);

	cfViewMode     = kCFUnsignedDecimal;
	cfViewNumBytes = 2;
	cfCompMode     = kCFSearchEqual;
	cfCompWith     = kCFCompWithThis;
		
	err = CreateATSUIStyleFromFontFullNameAndSize((unsigned char *) "\pLucida Sans Typewriter Regular", 11, &cfListLineATSUStyle);
	if (err)
	{
		err = CreateATSUIStyleFromFontFullNameAndSize((unsigned char *) "\pMonaco", 11, &cfListLineATSUStyle);
		if (err)
			QuitWithFatalError(0, "cheatfinder 02");
	}
}

void ResetCheatFinder(void)
{	
	memset(cfStoredRAM,  0x00, MAIN_MEMORY_SIZE);
	memset(cfLastRAM,    0x00, MAIN_MEMORY_SIZE);
	memset(cfStatusFlag, 0xFF, MAIN_MEMORY_SIZE);
	
	cfWatchAddr  = 0;
	cfIsNewGame  = true;
	cfIsWatching = false;
	cfIsStored   = false;
	cfDrawerShow = false;
	
	CheatFinderMakeValueFormat(cfWatchTextFormat);
}

void DeinitCheatFinder(void)
{
	OSStatus	err;
	
	err = ATSUDisposeStyle(cfListLineATSUStyle);

	delete [] cfStoredRAM;
	delete [] cfLastRAM;
	delete [] cfCurrentRAM;
	delete [] cfStatusFlag;
	delete [] cfAddress;
}

void CheatFinder(void)
{
	OSStatus		err;
	IBNibRef		nibRef;
	HIViewRef		ctl;
	HIViewID		cid;
	MenuRef			menu;
	char			num[256];
	unsigned char	numP[256];
	WindowData		cf;
	EventHandlerRef	wEref, sEref, pEref;
	EventHandlerUPP	wUPP, sUPP, pUPP;
	EventTypeSpec	wEvents[] = { { kEventClassCommand,    kEventCommandProcess           },
								  { kEventClassWindow,     kEventWindowClose              },
								  { kEventClassWindow,     kEventWindowBoundsChanging     } },
					sEvents[] = { { kEventClassCommand,    kEventCommandProcess           } },
					pEvents[] = { { kEventClassControl,    kEventControlDraw              } },
					cEvents[] = { { kEventClassHIObject,   kEventHIObjectConstruct        },
								  { kEventClassHIObject,   kEventHIObjectInitialize       },
								  { kEventClassHIObject,   kEventHIObjectDestruct         },
								  { kEventClassScrollable, kEventScrollableGetInfo        },
								  { kEventClassScrollable, kEventScrollableScrollTo       },
								  { kEventCheatFinderList, kEventScrollableScrollThere    },
								  { kEventClassControl,    kEventControlHitTest           },
								  { kEventClassControl,    kEventControlTrack             },
								  { kEventClassControl,    kEventControlValueFieldChanged },
								  { kEventClassControl,    kEventControlDraw              } };

	static HIObjectClassRef	cfListViewClass = nil;

	if (!cartOpen)
		return;

	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		err = CreateWindowFromNib(nibRef, CFSTR("CheatFinder"), &(cf.main));
		if (err == noErr)
		{
			err = CreateWindowFromNib(nibRef, CFSTR("CFAddEntry"), &(cf.sheet));
			if (err == noErr)
			{
				err = CreateWindowFromNib(nibRef, CFSTR("CFDrawer"), &(cf.drawer));
				if (err == noErr)			
				{
					memcpy(cfCurrentRAM, Memory.RAM, MAIN_MEMORY_SIZE);
					CheatFinderBuildResultList();

					err = noErr;
					if (!cfListViewClass)
						err = HIObjectRegisterSubclass(kCheatFinderListViewClassID, kHIViewClassID, 0, CheatFinderListViewHandler, GetEventTypeCount(cEvents), cEvents, nil, &cfListViewClass);
					if (err == noErr)
					{
						HIObjectRef	hiObject;
						HIViewRef	userpane, scrollview, listview, imageview, root;
						HIRect		frame;
						HISize		minSize;
						CGImageRef	image;
						Rect		rct;
						float		pich;

						GetWindowBounds(cf.main, kWindowContentRgn, &rct);

						minSize.width  = (float) (rct.right  - rct.left);
						minSize.height = (float) (rct.bottom - rct.top );
						err = SetWindowResizeLimits(cf.main, &minSize, nil);
						
						root = HIViewGetRoot(cf.main);
						cid.id = 0;
						cid.signature = kCFUserPane;
						HIViewFindByID(root, cid, &userpane);
						
						err = HIScrollViewCreate(kHIScrollViewOptionsVertScroll, &scrollview);
						HIViewAddSubview(userpane, scrollview);
						HIViewGetBounds(userpane, &frame);
						cfListAddrColumnWidth = (int) (frame.size.width * 0.4);
						frame.origin.y    += 16.0;
						frame.size.height -= 16.0;
						frame = CGRectInset(frame, 1.0, 1.0);
						HIViewSetFrame(scrollview, &frame);
						HIViewSetVisible(scrollview, true);
						cf.scroll = scrollview;
						
						if (hiToolboxVersion >= 0x130)
						{
							HILayoutInfo	layoutinfo;

							layoutinfo.version = kHILayoutInfoVersionZero;
							HIViewGetLayoutInfo(scrollview, &layoutinfo);

							layoutinfo.binding.top.toView    = userpane;
							layoutinfo.binding.top.kind      = kHILayoutBindTop;
							layoutinfo.binding.bottom.toView = userpane;
							layoutinfo.binding.bottom.kind   = kHILayoutBindBottom;
							layoutinfo.binding.left.toView   = userpane;
							layoutinfo.binding.left.kind     = kHILayoutBindLeft;
							layoutinfo.binding.right.toView  = userpane;
							layoutinfo.binding.right.kind    = kHILayoutBindRight;
							HIViewSetLayoutInfo(scrollview, &layoutinfo);
						}

						err = HIObjectCreate(kCheatFinderListViewClassID, nil, &hiObject);
						listview = (HIViewRef) hiObject;
						HIViewAddSubview(scrollview, listview);
						SetControl32BitMinimum(listview, 1);
						SetControl32BitMaximum(listview, cfNumRows);
						SetControl32BitValue(listview, 1);
						HIViewSetVisible(listview, true);
						cf.list = listview;
						
						cid.signature = kCFNumBytesPop;
						HIViewFindByID(root, cid, &ctl);
						menu = GetControlPopupMenuHandle(ctl);
						for (int i = 1; i <= CountMenuItems(menu); i++)
							CheckMenuItem(menu, i, false);
						CheckMenuItem(menu, cfViewNumBytes, true);
						SetControl32BitValue(ctl, cfViewNumBytes);

						cid.signature = kCFViewModeRad;
						HIViewFindByID(root, cid, &ctl);
						SetControl32BitValue(ctl, cfViewMode);

						cid.signature = kCFCompModePop;
						HIViewFindByID(root, cid, &ctl);
						menu = GetControlPopupMenuHandle(ctl);
						for (int i = 1; i <= CountMenuItems(menu); i++)
							CheckMenuItem(menu, i, false);
						CheckMenuItem(menu, cfCompMode, true);
						SetControl32BitValue(ctl, cfCompMode);

						if (cfIsNewGame || (!cfIsStored && (cfCompWith == kCFCompWithStored)))
							cfCompWith = kCFCompWithThis;

						cid.signature = kCFCompStoredRad;
						HIViewFindByID(root, cid, &ctl);
						SetControl32BitValue(ctl, cfCompWith == kCFCompWithStored);
						if (cfIsStored)
							ActivateControl(ctl);
						else
							DeactivateControl(ctl);
						
						cid.signature = kCFCompLastRad;
						HIViewFindByID(root, cid, &ctl);
						SetControl32BitValue(ctl, cfCompWith == kCFCompWithLast);
						if (!cfIsNewGame)
							ActivateControl(ctl);
						else
							DeactivateControl(ctl);

						cid.signature = kCFCompThisRad;
						HIViewFindByID(root, cid, &ctl);
						SetControl32BitValue(ctl, cfCompWith == kCFCompWithThis);
						
						cid.signature = kCFCompValueTxt;
						HIViewFindByID(root, cid, &ctl);
						SetEditTextCFString(ctl, CFSTR(""), false);
						err = SetKeyboardFocus(cf.main, ctl, kControlFocusNextPart);

						cid.signature = kCFWatchBtn;
						HIViewFindByID(root, cid, &ctl);
						SetControl32BitValue(ctl, cfIsWatching);
						
						cid.signature = kCFDrawerBtn;
						HIViewFindByID(root, cid, &ctl);
						SetControl32BitValue(ctl, cfDrawerShow);

						cid.signature = kCFWatchAddrTxt;
						HIViewFindByID(root, cid, &ctl);
						if (cfIsWatching)
						{
							sprintf(num, "%06lX", cfWatchAddr + 0x7E0000);
							ConvertCString(num, numP);
							SetStaticTextText(ctl, numP, false);
						}
						else
							SetStaticTextCFString(ctl, CFSTR(""), false);

						CheatFinderAdjustButtons(&cf);

						pUPP = NewEventHandlerUPP(CheatFinderListFrameEventHandler);
						err = InstallControlEventHandler(userpane, pUPP, GetEventTypeCount(pEvents), pEvents, (void *) userpane, &pEref);

						wUPP = NewEventHandlerUPP(CheatFinderWindowEventHandler);
						err = InstallWindowEventHandler (cf.main,  wUPP, GetEventTypeCount(wEvents), wEvents, (void *) &cf,      &wEref);

						sUPP = NewEventHandlerUPP(CheatFinderSheetEventHandler);
						err = InstallWindowEventHandler (cf.sheet, sUPP, GetEventTypeCount(sEvents), sEvents, (void *) &cf,      &sEref);

						pich = (float) (IPPU.RenderedScreenHeight >> ((IPPU.RenderedScreenHeight > 256) ? 1 : 0));

						err = SetDrawerParent(cf.drawer, cf.main);
						err = SetDrawerOffsets(cf.drawer, 0.0, (float) ((rct.bottom - rct.top) - (pich + 37)));

						image = CreateGameScreenCGImage();
						if (image)
						{
							err = HIImageViewCreate(image, &imageview);
							if (err == noErr)
							{
								HIViewFindByID(HIViewGetRoot(cf.drawer), kHIViewWindowContentID, &ctl);
								
								HIViewAddSubview(ctl, imageview);								
								HIImageViewSetOpaque(imageview, false);
								HIImageViewSetScaleToFit(imageview, true);
								HIViewSetVisible(imageview, true);
								
								frame.origin.x = 8.0;
								frame.origin.y = 8.0;
								frame.size.width  = (float) SNES_WIDTH;
								frame.size.height = pich;
								HIViewSetFrame(imageview, &frame);
							}
						}
						
						MoveWindowPosition(cf.main, kWindowCheatFinder, true);
						ShowWindow(cf.main);
						
						if (cfDrawerShow)
							err = OpenDrawer(cf.drawer, kWindowEdgeDefault, false);
							
						err = RunAppModalLoopForWindow(cf.main);
						
						HideWindow(cf.main);
						SaveWindowPosition(cf.main, kWindowCheatFinder);
			
						err = RemoveEventHandler(pEref);
						DisposeEventHandlerUPP(pUPP);
						
						err = RemoveEventHandler(sEref);
						DisposeEventHandlerUPP(sUPP);

						err = RemoveEventHandler(wEref);
						DisposeEventHandlerUPP(wUPP);

						if (image)
							CGImageRelease(image);
					}
		
					ReleaseWindow(cf.drawer);
				}
				
				ReleaseWindow(cf.sheet);
			}
			
			ReleaseWindow(cf.main);
		}
		
		DisposeNibReference(nibRef);

		memcpy(cfLastRAM, Memory.RAM, MAIN_MEMORY_SIZE);
		cfIsNewGame = false;
	}
}

static SInt64 CheatFinderReadBytes(UInt8 *mem, UInt32 addr)
{
	switch (cfViewMode)
	{
		case kCFSignedDecimal:
		{
			switch (cfViewNumBytes)
			{
				case 1:	return (SInt64) (SInt8)      mem[addr];
				case 2:	return (SInt64) (SInt16)    (mem[addr] | (mem[addr + 1] << 8));
				case 4:	return (SInt64) (SInt32)    (mem[addr] | (mem[addr + 1] << 8) | (mem[addr + 2] << 16) | (mem[addr + 3] << 24));
				case 3:	return (SInt64) (((SInt32) ((mem[addr] | (mem[addr + 1] << 8) | (mem[addr + 2] << 16)) << 8)) >> 8);
			}
			
			break;
		}
		
		case kCFUnsignedDecimal:
		case kCFHexadecimal:
		{
			switch (cfViewNumBytes)
			{
				case 1:	return (SInt64) (UInt8)      mem[addr];
				case 2:	return (SInt64) (UInt16)    (mem[addr] | (mem[addr + 1] << 8));
				case 3:	return (SInt64) (UInt32)    (mem[addr] | (mem[addr + 1] << 8) | (mem[addr + 2] << 16));
				case 4:	return (SInt64) (UInt32)    (mem[addr] | (mem[addr + 1] << 8) | (mem[addr + 2] << 16) | (mem[addr + 3] << 24));
			}
			
			break;
		}
	}
	
	return 0;
}

static pascal OSStatus CheatFinderWindowEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerCallRef)
	
	OSStatus				err, result;
	WindowData	*cf;
	HIViewRef				control, ctl, root;
	HIViewID				controlID, cid;
	Boolean					update = false;
	char					num[256];
	unsigned char			numP[256];

	result = eventNotHandledErr;
	cf = (WindowData *) inUserData;
	
	switch (GetEventClass(inEvent))
	{
		case kEventClassWindow:
		{
			switch (GetEventKind(inEvent))
			{
				case kEventWindowClose:
				{
					QuitAppModalLoopForWindow(cf->main);
					result = noErr;
					break;
				}

				case kEventWindowBoundsChanging:
				{
				#ifdef MAC_JAGUAR_SUPPORT
					if (systemVersion < 0x1030)
					{
						Rect	winBounds;
						UInt32	attr;
						
						err = GetEventParameter(inEvent, kEventParamAttributes, typeUInt32, nil, sizeof(UInt32), nil, &attr);
						if ((err == noErr) && (attr & kWindowBoundsChangeSizeChanged))
						{
							err = GetEventParameter(inEvent, kEventParamCurrentBounds, typeQDRectangle, nil, sizeof(Rect), nil, &winBounds);
							if (err == noErr)
							{
								HIRect	frame, pbounds;

								pbounds.origin.x = 0.0;
								pbounds.origin.y = 0.0;
								pbounds.size.width  = (float) (winBounds.right  - winBounds.left);
								pbounds.size.height = (float) (winBounds.bottom - winBounds.top );
								
								root = HIViewGetRoot(cf->main);
								cid.id = 0;

								cid.signature = kCFUpperViews;
								HIViewFindByID(root, cid, &ctl);
								HIViewGetFrame(ctl, &frame);
								frame.origin.x = pbounds.size.width  - frame.size.width;
								frame.origin.y = 0;
								HIViewSetFrame(ctl, &frame);
						
								cid.signature = kCFLowerViews;
								HIViewFindByID(root, cid, &ctl);
								HIViewGetFrame(ctl, &frame);
								frame.origin.x = pbounds.size.width  - frame.size.width;
								frame.origin.y = pbounds.size.height - frame.size.height;
								HIViewSetFrame(ctl, &frame);

								cid.signature = kCFUserPane;
								HIViewFindByID(root, cid, &ctl);
								HIViewGetFrame(ctl, &frame);
								frame.size.height = pbounds.size.height -  40.0;
								frame.size.width  = pbounds.size.width  - 236.0;
								HIViewSetFrame(ctl, &frame);

								HIViewGetBounds(ctl, &frame);
								frame.origin.y    += 16.0;
								frame.size.height -= 16.0;
								frame = CGRectInset(frame, 1.0, 1.0);
								HIViewSetFrame(cf->scroll, &frame);

								result = noErr;
							}
						}
					}
				#endif
					break;
				}
			}
			
			break;
		}

		case kEventClassCommand:
		{
			switch (GetEventKind(inEvent))
			{
				case kEventCommandProcess:
				{
					HICommand	tHICommand;

					err = GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, nil, sizeof(HICommand), nil, &tHICommand);
					if (err == noErr)
					{
						root = HIViewGetRoot(cf->main);
						controlID.id = 0;
						controlID.signature = tHICommand.commandID;
						HIViewFindByID(root, controlID, &control);
						
						switch (tHICommand.commandID)
						{
							case kCFNumBytesPop:
							{
								cfViewNumBytes = GetControl32BitValue(control);
								update = true;
								result = noErr;
								break;
							}

							case kCFViewModeRad:
							{
								cfViewMode     = GetControl32BitValue(control);
								update = true;
								result = noErr;
								break;
							}

							case kCFCompModePop:
							{
								cfCompMode     = GetControl32BitValue(control);
								result = noErr;
								break;
							}

							case kCFCompStoredRad:
							case kCFCompLastRad:
							case kCFCompThisRad:
							{
								if (tHICommand.commandID == kCFCompStoredRad)
									cfCompWith = kCFCompWithStored;
								else
								if (tHICommand.commandID == kCFCompLastRad)
									cfCompWith = kCFCompWithLast;
								else
									cfCompWith = kCFCompWithThis;
								
								cid.id = 0;
								
								cid.signature = kCFCompStoredRad;
								HIViewFindByID(root, cid, &ctl);
								SetControl32BitValue(ctl, cfCompWith == kCFCompWithStored);
								
								cid.signature = kCFCompLastRad;
								HIViewFindByID(root, cid, &ctl);
								SetControl32BitValue(ctl, cfCompWith == kCFCompWithLast);

								cid.signature = kCFCompThisRad;
								HIViewFindByID(root, cid, &ctl);
								SetControl32BitValue(ctl, cfCompWith == kCFCompWithThis);
								
								result = noErr;
								break;
							}

							case kCFStoreValueBtn:
							{
								memcpy(cfStoredRAM, Memory.RAM, MAIN_MEMORY_SIZE);
								cfIsStored = true;
								
								cid.id = 0;
								cid.signature = kCFCompStoredRad;
								HIViewFindByID(root, cid, &ctl);
								ActivateControl(ctl);

								result = noErr;
								break;
							}

							case kCFSearchBtn:
							{
								CheatFinderSearch(cf);
								update = true;
								result = noErr;
								break;
							}
								
							case kCFRemoveBtn:
							{
								CheatFinderRemoveFromList(cf);
								update = true;
								result = noErr;
								break;
							}

							case kCFRestoreBtn:
							{
								CheatFinderRestoreList(cf);
								update = true;
								result = noErr;
								break;
							}

							case kCFAddEntryBtn:
							{
								CheatFinderHandleAddEntryButton(cf);
								result = noErr;
								break;
							}

							case kCFDrawerBtn:
							{
								cfDrawerShow = !cfDrawerShow;
								
								if (cfDrawerShow)
									err = OpenDrawer(cf->drawer, kWindowEdgeDefault, false);
								else
									err = CloseDrawer(cf->drawer, false);
								
								result = noErr;
								break;
							}
								
							case kCFWatchBtn:
							{
								cid.id = 0;
								cid.signature = kCFWatchAddrTxt;
								HIViewFindByID(root, cid, &ctl);
									
								if (!cfIsWatching)
								{
									if (cfAddress[cfListSelection] > (0x20000 - cfViewNumBytes))
									{
										SysBeep(10);
										SetControl32BitValue(control, 0);
									}
									else
									{									
										cfWatchAddr = cfAddress[cfListSelection];
										cfIsWatching = true;
										
										sprintf(num, "%06lX", cfWatchAddr + 0x7E0000);
										ConvertCString(num, numP);
										SetStaticTextText(ctl, numP, true);
									}
								}
								else
								{
									cfWatchAddr = 0;
									cfIsWatching = false;
									
									SetControl32BitValue(control, 0);
									SetStaticTextCFString(ctl, CFSTR(""), true);
									
									CheatFinderAdjustButtons(cf);
								}
								
								result = noErr;
								break;
							}						
						}
						
						if (update)
						{						
							HIViewSetNeedsDisplay(cf->list, true);
							CheatFinderAdjustButtons(cf);
						}
						
						CheatFinderMakeValueFormat(cfWatchTextFormat);
					}
					
					break;
				}
			}
			
			break;
		}
	}
	
	return (result);
}

static SInt64 CheatFinderGetValueEditText(HIViewRef control)
{
	SInt64			result = 0;
	UInt32			uvalue;
	SInt32			svalue;	
	char			num[256];
	unsigned char	numP[256];
	
	GetEditTextText(control, numP);
	if (numP[0] == 0)
	{
		SetEditTextCFString(control, CFSTR("0"), true);
		return 0;
	}

	ConvertPString(numP, num);
	
	switch (cfViewMode)
	{
		case kCFSignedDecimal:
		{
			if (sscanf(num, "%ld", &svalue) == 1)
			{
				switch (cfViewNumBytes)
				{
					case 1:
					{
						if (svalue >  127)
						{
							svalue =  127;
							SetEditTextCFString(control, CFSTR("127"), true);
						}
						else
						if (svalue < -128)
						{
							svalue = -128;
							SetEditTextCFString(control, CFSTR("-128"), true);
						}
						
						break;
					}
						
					case 2:
					{
						if (svalue >  32767)
						{
							svalue =  32767;
							SetEditTextCFString(control, CFSTR("32767"), true);
						}
						else
						if (svalue < -32768)
						{
							svalue = -32768;
							SetEditTextCFString(control, CFSTR("-32768"), true);
						}
						
						break;
					}
						
					case 3:
					{
						if (svalue >  8388607)
						{
							svalue =  8388607;
							SetEditTextCFString(control, CFSTR("8388607"), true);
						}
						else
						if (svalue < -8388608)
						{
							svalue = -8388608;
							SetEditTextCFString(control, CFSTR("-8388608"), true);
						}
						
						break;
					}
				}
			}
			else
			{
				svalue = 0;
				SetEditTextCFString(control, CFSTR("0"), true);
			}
			
			result = (SInt64) svalue;
			
			break;
		}
			
		case kCFUnsignedDecimal:
		{
			if (sscanf(num, "%lu", &uvalue) == 1)
			{
				switch (cfViewNumBytes)
				{
					case 1:
					{
						if (uvalue > 255)
						{
							uvalue = 255;
							SetEditTextCFString(control, CFSTR("255"), true);
						}
						
						break;
					}
						
					case 2:
					{
						if (uvalue > 65535)
						{
							uvalue = 65535;
							SetEditTextCFString(control, CFSTR("65535"), true);
						}
						
						break;
					}
						
					case 3:
					{
						if (uvalue > 16777215)
						{
							uvalue = 16777215;
							SetEditTextCFString(control, CFSTR("16777215"), true);
						}
						
						break;
					}
				}
			}
			else
			{
				uvalue = 0;
				SetEditTextCFString(control, CFSTR("0"), true);
			}
			
			result = (SInt64) uvalue;
			
			break;
		}

		case kCFHexadecimal:
		{
			if (sscanf(num, "%lx", &uvalue) == 1)
			{
				switch (cfViewNumBytes)
				{
					case 1:
					{
						if (uvalue > 0xFF)
						{
							uvalue = 0xFF;
							SetEditTextCFString(control, CFSTR("FF"), true);
						}
						
						break;
					}
					
					case 2:
					{
						if (uvalue > 0xFFFF)
						{
							uvalue = 0xFFFF;
							SetEditTextCFString(control, CFSTR("FFFF"), true);
						}
						
						break;
					}
						
					case 3:
					{
						if (uvalue > 0xFFFFFF)
						{
							uvalue = 0xFFFFFF;
							SetEditTextCFString(control, CFSTR("FFFFFF"), true);
						}
						
						break;
					}
				}
			}
			else
			{
				uvalue = 0;
				SetEditTextCFString(control, CFSTR("0"), true);
			}
			
			result = (SInt64) uvalue;
			
			break;
		}
	}
	
	return (result);
}

static void CheatFinderSearch(WindowData *cf)
{
	UInt8	*mem;
	SInt64	cmpvalue;
	SInt32	i;
	
	if (cfCompWith == kCFCompWithThis)
	{
		HIViewRef	ctl;
		HIViewID	cid = { kCFCompValueTxt, 0 };
		
		HIViewFindByID(HIViewGetRoot(cf->main), cid, &ctl);
		cmpvalue = CheatFinderGetValueEditText(ctl);
			
		for (i = 0; i < cfNumRows; i++)
			if (!CheatFinderCompare(CheatFinderReadBytes(cfCurrentRAM, cfAddress[i]), cmpvalue))
				cfStatusFlag[cfAddress[i]] = 0;
	}
	else
	{
		mem = (cfCompWith == kCFCompWithStored) ? cfStoredRAM : cfLastRAM;
	
		for (i = 0; i < cfNumRows; i++)
			if (!CheatFinderCompare(CheatFinderReadBytes(cfCurrentRAM, cfAddress[i]), CheatFinderReadBytes(mem, cfAddress[i])))
				cfStatusFlag[cfAddress[i]] = 0;
	}
	
	CheatFinderBuildResultList();

	SetControl32BitMaximum(cf->list, cfNumRows);
	SetControl32BitValue(cf->list, 1);
}

static Boolean CheatFinderCompare(SInt64 ramvalue, SInt64 cmpvalue)
{
	switch (cfCompMode)
	{
		case kCFSearchEqual:			return (ramvalue == cmpvalue);
		case kCFSearchNotEqual:			return (ramvalue != cmpvalue);
		case kCFSearchGreater:			return (ramvalue >  cmpvalue);
		case kCFSearchGreaterOrEqual:	return (ramvalue >= cmpvalue);
		case kCFSearchLess:				return (ramvalue <  cmpvalue);
		case kCFSearchLessOrEqual:		return (ramvalue <= cmpvalue);
	}
	
	return (false);
}

static void CheatFinderBuildResultList(void)
{
	cfNumRows = 0;
	
	for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
	{
		if (cfStatusFlag[i] == 0xFF)
		{
			cfAddress[cfNumRows] = i;
			cfNumRows++;
		}
	}
	
	cfListSelection = 0;
}

static void CheatFinderAdjustButtons(WindowData *cf)
{
	HIViewRef	ctl, root;
	HIViewID	cid;
	
	cid.id = 0;
	root = HIViewGetRoot(cf->main);
		
	if (cfNumRows > 0)
	{		
		cid.signature = kCFAddEntryBtn;
		HIViewFindByID(root, cid, &ctl);
		ActivateControl(ctl);
		
		cid.signature = kCFRemoveBtn;
		HIViewFindByID(root, cid, &ctl);
		ActivateControl(ctl);

		cid.signature = kCFWatchBtn;
		HIViewFindByID(root, cid, &ctl);
		ActivateControl(ctl);
	}
	else
	{		
		cid.signature = kCFAddEntryBtn;
		HIViewFindByID(root, cid, &ctl);
		DeactivateControl(ctl);
		
		cid.signature = kCFRemoveBtn;
		HIViewFindByID(root, cid, &ctl);
		DeactivateControl(ctl);

		if (!cfIsWatching)
		{
			cid.signature = kCFWatchBtn;
			HIViewFindByID(root, cid, &ctl);
			DeactivateControl(ctl);
		}
	}
}

static void CheatFinderRemoveFromList(WindowData *cf)
{
	if (cfNumRows > 0)
	{
		cfStatusFlag[cfAddress[cfListSelection]] = 0;

		if (cfNumRows == 1)
		{			
			cfNumRows = 0;
			
			SetControl32BitMaximum(cf->list, 0);
			SetControl32BitValue(cf->list, 1);
		}
		else
		{
			for (SInt32 i = cfListSelection; i < cfNumRows - 1; i++)
				cfAddress[i] = cfAddress[i + 1];
			
			cfNumRows--;
			if (cfListSelection >= cfNumRows)
				cfListSelection = cfNumRows - 1;
			
			SetControl32BitMaximum(cf->list, cfNumRows);
			SetControl32BitValue(cf->list, cfListSelection + 1);
		}
	}
}

static void CheatFinderRestoreList(WindowData *cf)
{
	memset(cfStatusFlag, 0xFF, MAIN_MEMORY_SIZE);
	CheatFinderBuildResultList();	

	SetControl32BitMaximum(cf->list, cfNumRows);
	SetControl32BitValue(cf->list, 1);
}

static void CheatFinderMakeValueFormat(char *text)
{
	switch (cfViewMode)
	{
		case kCFSignedDecimal:
		case kCFUnsignedDecimal:
		{
			strcpy(text, "%lld");
			break;
		}
		
		case kCFHexadecimal:
		{
			sprintf(text, "%%0%lullX", cfViewNumBytes * 2);
			break;
		}
	}
}		

void CheatFinderDrawWatchAddr(void)
{
	uint16	*basePtr;
	int		len;
	
	static char	code[256];
	
	sprintf(code, cfWatchTextFormat, CheatFinderReadBytes(Memory.RAM, cfWatchAddr));

	basePtr = GFX.Screen + 1;
	len = strlen(code);
	
	for (int i = 0; i < len; i++)
	{
		DisplayChar(basePtr, code[i]);
		basePtr += (font_width - 1);
    }
}

static void CheatFinderHandleAddEntryButton(WindowData *cf)
{
	if (cfAddress[cfListSelection] > (0x20000 - cfViewNumBytes))	
		SysBeep(10);
	else
	if (Cheat.num_cheats + cfViewNumBytes > MAX_CHEATS)
		AppearanceAlert(kAlertCautionAlert, kCFCantAddWarning, kCFCantAddHint);
	else
		CheatFinderBeginAddEntrySheet(cf);
}

static void CheatFinderBeginAddEntrySheet(WindowData *cf)
{
	OSStatus	err;
	HIViewRef	ctl, root;
	HIViewID	cid;
	UInt32		addr;
	char		buf1[256], buf2[256];
		
	addr = cfAddress[cfListSelection];
	
	root = HIViewGetRoot(cf->sheet);
	cid.id = 0;
	
	cid.signature = kCFSheetAddrTxt;
	HIViewFindByID(root, cid, &ctl);
	sprintf(buf1, "%06lX", addr + 0x7E0000);
	ConvertCString(buf1, (unsigned char *) buf2);
	SetStaticTextText(ctl, (unsigned char *) buf2, false);
	
	cid.signature = kCFSheetCurrentValueTxt;
	HIViewFindByID(root, cid, &ctl);
	CheatFinderMakeValueFormat(buf2);
	sprintf(buf1, buf2, CheatFinderReadBytes(cfCurrentRAM, addr));
	ConvertCString(buf1, (unsigned char *) buf2);
	SetStaticTextText(ctl, (unsigned char *) buf2, false);
	
	cid.signature = kCFSheetCheetValueTxt;
	HIViewFindByID(root, cid, &ctl);
	SetEditTextText(ctl, (unsigned char *) buf2, false);

	err = ClearKeyboardFocus(cf->sheet);
	err = SetKeyboardFocus(cf->sheet, ctl, kControlFocusNextPart);
	
	cid.signature = kCFSheetDescriptionTxt;
	HIViewFindByID(root, cid, &ctl);
	sprintf(buf1, "%06lX-%06lX", addr + 0x7E0000, addr + cfViewNumBytes - 1 + 0x7E0000);
	ConvertCString(buf1, (unsigned char *) buf2);
	SetEditTextText(ctl, (unsigned char *) buf2, false);

	err = ShowSheetWindow(cf->sheet, cf->main);
}

static pascal OSStatus CheatFinderSheetEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerCallRef)
	
	OSStatus	err, result = eventNotHandledErr;
	WindowData	*cf = (WindowData *) inUserData;
	
	switch (GetEventClass(inEvent))
	{
		case kEventClassCommand:
		{
			switch (GetEventKind(inEvent))
			{
				case kEventCommandProcess:
				{
					HICommand	tHICommand;

					err = GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, nil, sizeof(HICommand), nil, &tHICommand);
					if (err == noErr)
					{
						switch (tHICommand.commandID)
						{
							case kCFSheetAddBtn:
							{
								HIViewRef	ctl, root;
								HIViewID	cid;
								CFStringRef	str;
								SInt64		rv;
								char		buf[256];
								
								root = HIViewGetRoot(cf->sheet);
								cid.id = 0;
								
								cid.signature = kCFSheetCheetValueTxt;
								HIViewFindByID(root, cid, &ctl);
								rv = CheatFinderGetValueEditText(ctl);
								
								cid.signature = kCFSheetDescriptionTxt;
								HIViewFindByID(root, cid, &ctl);
								
								buf[0] = 0;
								CopyEditTextCFString(ctl, &str);
								if (str)
								{
									strcpy(buf, GetMultiByteCharacters(str, 19));
									CFRelease(str);
								}
								
								if (buf[0] == 0)
								{
									buf[0] = ' ';
									buf[1] = 0;
								}
								
								CheatFinderAddEntry(rv, buf);
								
								err = HideSheetWindow(cf->sheet);								
								
								result = noErr;
								break;
							}

							case kCFSheetCancelBtn:
							{
								err = HideSheetWindow(cf->sheet);								
								result = noErr;
								break;
							}
						}
					}
					
					break;
				}
			}
			
			break;
		}
	}
	
	return (result);
}	

static void CheatFinderAddEntry(SInt64 value, char *description)
{
	UInt32	addr, v, i;
	
	addr = cfAddress[cfListSelection];
	v = (UInt32) (SInt32) value;

	for (i = 0; i < cfViewNumBytes; i++)
	{
		strcpy(Cheat.c[Cheat.num_cheats].name, description);			
		S9xAddCheat(true, true, addr + i + 0x7E0000, (UInt8) ((v & (0x000000FF << (i * 8))) >> (i * 8)));
	}
	
	S9xApplyCheats();
}

static pascal OSStatus CheatFinderListFrameEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *userData)
{
	#pragma unused (inHandlerCallRef)

	OSStatus	err, result = eventNotHandledErr;
	HIViewRef	view = (HIViewRef) userData;

	switch (GetEventClass(inEvent))
	{
		case kEventClassControl:
		{
			switch (GetEventKind(inEvent))
			{
				case kEventControlDraw:
				{
					CGContextRef	ctx;
					HIRect			bounds;

					HIViewGetBounds(view, &bounds);
					cfListAddrColumnWidth = (int) (bounds.size.width * 0.4);

					err = GetEventParameter(inEvent, kEventParamCGContextRef, typeCGContextRef, nil, sizeof(CGContextRef), nil, &ctx);
					if (err == noErr)
					{
						HIRect	drawBounds;

						if (systemVersion >= 0x1040)
						{
							HIShapeRef	shape = nil;

							err = GetEventParameter(inEvent, kEventParamShape, typeHIShapeRef, nil, sizeof(HIShapeRef), nil, &shape);
							if (err == noErr)
							{
								if (shape)
									HIShapeGetBounds(shape, &drawBounds);
								else
									drawBounds = bounds;
							}
						}
					#ifdef MAC_PANTHER_JAGUAR_SUPPORT
						else
						{
							RgnHandle	rgn = nil;

							err = GetEventParameter(inEvent, kEventParamRgnHandle, typeQDRgnHandle, nil, sizeof(RgnHandle), nil, &rgn);
							if (err == noErr)
							{
								if (rgn)
								{
									Rect	rgnBounds;

									GetRegionBounds(rgn, &rgnBounds);
									drawBounds.origin.x    = (float)  rgnBounds.left;
									drawBounds.origin.y    = (float)  rgnBounds.top;
									drawBounds.size.width  = (float) (rgnBounds.right  - rgnBounds.left);
									drawBounds.size.height = (float) (rgnBounds.bottom - rgnBounds.top );
								}
								else
									drawBounds = bounds;
							}
						}
					#endif
						if (err == noErr)
						{
							if (CGRectContainsRect(drawBounds, bounds))
							{
								if (hiToolboxVersion >= 0x130)
								{
									HIRect					rct;
									HIThemeFrameDrawInfo	frameinfo;
									HIThemeButtonDrawInfo	buttoninfo;
									HIThemeTextInfo			textinfo;
									CFStringRef				str;

									buttoninfo.version          = 0;
									buttoninfo.state            = kThemeStateActive;
									buttoninfo.kind             = kThemeListHeaderButton;
									buttoninfo.value            = kThemeButtonOff;
									buttoninfo.adornment        = kThemeAdornmentNone;

									frameinfo.version           = 0;
									frameinfo.state             = kThemeStateActive;
									frameinfo.kind              = kHIThemeFrameListBox;
									frameinfo.isFocused         = false;

									textinfo.version            = 0;
									textinfo.state              = kThemeStateActive;
									textinfo.fontID             = kThemeSmallSystemFont;
									textinfo.verticalFlushness  = kHIThemeTextVerticalFlushCenter;
									textinfo.options            = 0;
									textinfo.truncationPosition = kHIThemeTextTruncationNone;
									textinfo.truncationMaxLines = 0;
									textinfo.truncationHappened = false;

									CGContextSaveGState(ctx);
									
									rct = CGRectInset(bounds, 1.0, 1.0);
									err = HIThemeDrawFrame(&rct, &frameinfo, ctx, kHIThemeOrientationNormal);

									rct = bounds;
									rct.size.height = 16.0;
									rct.size.width  = (float) cfListAddrColumnWidth + 1.0;
									err = HIThemeDrawButton(&rct, &buttoninfo, ctx, kHIThemeOrientationNormal, nil);
									
									CGContextSetRGBFillColor(ctx, 0.0, 0.0, 0.0, 1.0);
									
									rct.size.width  = (float) cfListAddrColumnWidth;
									rct.origin.y++;
									textinfo.horizontalFlushness = kHIThemeTextHorizontalFlushCenter;
									str = CFCopyLocalizedString(CFSTR("Address"), "address");
									if (str)
									{
										err = HIThemeDrawTextBox(str, &rct, &textinfo, ctx, kHIThemeOrientationNormal);
										CFRelease(str);
									}

									rct = bounds;
									rct.size.height = 16.0;
									rct.size.width -= (float) cfListAddrColumnWidth;
									rct.origin.x   += (float) cfListAddrColumnWidth;
									err = HIThemeDrawButton(&rct, &buttoninfo, ctx, kHIThemeOrientationNormal, nil);
									
									CGContextSetRGBFillColor(ctx, 0.0, 0.0, 0.0, 1.0);
									
									rct.size.width -= 28.0;
									rct.origin.y++;
									textinfo.horizontalFlushness = kHIThemeTextHorizontalFlushRight;
									str = CFCopyLocalizedString(CFSTR("Value"), "value");
									if (str)
									{
										err = HIThemeDrawTextBox(str, &rct, &textinfo, ctx, kHIThemeOrientationNormal);
										CFRelease(str);
									}

									rct = bounds;
									rct.size.height = 16.0;
									rct.size.width  = 16.0;
									rct.origin.x    = bounds.size.width - 16.0;
									err = HIThemeDrawButton(&rct, &buttoninfo, ctx, kHIThemeOrientationNormal, nil);
									
									CGContextRestoreGState(ctx);
								}
							#ifdef MAC_JAGUAR_SUPPORT
								else
								{
									Rect				qdbounds, rct;
									ThemeButtonDrawInfo	buttoninfo;
									CFStringRef			str;
									CGrafPtr			old;

									GetPort(&old);
									SetPortWindowPort(GetControlOwner(view));

									buttoninfo.state     = kThemeStateActive;
									buttoninfo.value     = kThemeButtonOff;
									buttoninfo.adornment = kThemeAdornmentNone;

									GetControlBounds(view, &qdbounds);

									rct = qdbounds;
									InsetRect(&rct, 1, 1);
									err = DrawThemeListBoxFrame(&rct, kThemeStateActive);

									rct = qdbounds;
									rct.bottom = rct.top  + 16;
									rct.right  = rct.left + cfListAddrColumnWidth + 1;
									err = DrawThemeButton(&rct, kThemeListHeaderButton, &buttoninfo, nil, nil, nil, 0);

									rct.right  = rct.left + cfListAddrColumnWidth;
									OffsetRect(&rct, 0, 1);
									str = CFCopyLocalizedString(CFSTR("Address"), "address");
									if (str)
									{
										err = DrawThemeTextBox(str, kThemeSmallSystemFont, kThemeStateActive, false, &rct, teCenter, ctx);
										CFRelease(str);
									}

									rct = qdbounds;
									rct.bottom = rct.top + 16;
									rct.left  += cfListAddrColumnWidth;
									err = DrawThemeButton(&rct, kThemeListHeaderButton, &buttoninfo, nil, nil, nil, 0);

									rct.right -= 28;
									OffsetRect(&rct, 0, 1);
									str = CFCopyLocalizedString(CFSTR("Value"), "value");
									if (str)
									{
										err = DrawThemeTextBox(str, kThemeSmallSystemFont, kThemeStateActive, false, &rct, teFlushRight, ctx);
										CFRelease(str);
									}

									rct = qdbounds;
									rct.bottom = rct.top   + 16;
									rct.left   = rct.right - 16;
									err = DrawThemeButton(&rct, kThemeListHeaderButton, &buttoninfo, nil, nil, nil, 0);

									SetPort(old);
								}
							#endif
							}
							
							result = noErr;
						}
					}

					break;
				}
			}

			break;
		}
	}

	return (result);
}

static void CheatFinderListViewDraw(CGContextRef ctx, HIRect *bounds, ListViewData *myData)
{
	static Boolean	init = true;
	static Rect		aRect = { 0, 0, 0, 0 }, vRect = { 0, 0, 0, 0 };

	OSStatus		err;
	ATSUTextLayout	layout;
	HIRect			lineBounds;
	UniCharCount	runLength[1], len;
	SInt32			start, end, val, max;
	Fixed			ax, vx, f;
	float			y;
	UniChar			unistr[64];
	char			format[32], t1[64], t2[64];

	ATSUAttributeTag		theTags[]   = { kATSUCGContextTag    };
	ByteCount				theSizes[]  = { sizeof(CGContextRef) };
	ATSUAttributeValuePtr	theValues[] = { &ctx                 };

	CheatFinderMakeValueFormat(format);

	start = (SInt32)  (myData->originPoint.y / myData->lineSize.height);
	end   = (SInt32) ((myData->originPoint.y + bounds->size.height) / myData->lineSize.height) + 1;

	y = start * myData->lineSize.height - myData->originPoint.y;

	lineBounds = *bounds;
	lineBounds.size.height = myData->lineSize.height;
	lineBounds.origin.y = y;

	val = GetControl32BitValue(myData->view) - 1;
	max = GetControl32BitMaximum(myData->view);

	if (init)
	{
		f = Long2Fix(0);
		for (unsigned int n = 0; n < 11; n++)
			unistr[n] = 'F';

		len = runLength[0] = 6;
		err = ATSUCreateTextLayoutWithTextPtr(unistr, kATSUFromTextBeginning, kATSUToTextEnd, len, 1, runLength, &cfListLineATSUStyle, &layout); 
		err = ATSUSetLayoutControls(layout, sizeof(theTags) / sizeof(theTags[0]), theTags, theSizes, theValues);
		err = ATSUMeasureTextImage(layout, kATSUFromTextBeginning, kATSUToTextEnd, f, f, &aRect);
		err = ATSUDisposeTextLayout(layout);

		len = runLength[0] = 11;
		err = ATSUCreateTextLayoutWithTextPtr(unistr, kATSUFromTextBeginning, kATSUToTextEnd, len, 1, runLength, &cfListLineATSUStyle, &layout); 
		err = ATSUSetLayoutControls(layout, sizeof(theTags) / sizeof(theTags[0]), theTags, theSizes, theValues);
		err = ATSUMeasureTextImage(layout, kATSUFromTextBeginning, kATSUToTextEnd, f, f, &vRect);
		err = ATSUDisposeTextLayout(layout);

		init = false;
	}

	ax = Long2Fix((cfListAddrColumnWidth - 2 - (aRect.right - aRect.left)) >> 1);
	vx = Long2Fix((int) (lineBounds.origin.x + lineBounds.size.width) - (vRect.right - vRect.left) - 13);

	for (int i = start; i <= end; i++)
	{
		if ((i == val) && cfNumRows)
			CGContextSetRGBFillColor(ctx,  59.0 / 256.0, 124.0 / 256.0, 212.0 / 256.0, 1.0);
		else
		if ((i - start) % 2 == 0)
			CGContextSetRGBFillColor(ctx, 256.0 / 256.0, 256.0 / 256.0, 256.0 / 256.0, 1.0);
		else
			CGContextSetRGBFillColor(ctx, 237.0 / 256.0, 244.0 / 256.0, 254.0 / 256.0, 1.0);

		CGContextFillRect(ctx, lineBounds);

		if (i < max)
		{
			CGContextScaleCTM(ctx, 1, -1);
			
			if (i == val)
				CGContextSetRGBFillColor(ctx, 1.0, 1.0, 1.0, 1.0);
			else
				CGContextSetRGBFillColor(ctx, 0.0, 0.0, 0.0, 1.0);

			f = Long2Fix(-((int) y + 12));

			sprintf(t1, "%06lX", cfAddress[i] + 0x7E0000);
			len = runLength[0] = strlen(t1);
			for (unsigned int n = 0; n < len; n++)
				unistr[n] = t1[n];
			err = ATSUCreateTextLayoutWithTextPtr(unistr, kATSUFromTextBeginning, kATSUToTextEnd, len, 1, runLength, &cfListLineATSUStyle, &layout); 
			err = ATSUSetLayoutControls(layout, sizeof(theTags) / sizeof(theTags[0]), theTags, theSizes, theValues);
			err = ATSUDrawText(layout, kATSUFromTextBeginning, kATSUToTextEnd, ax, f);
			err = ATSUDisposeTextLayout(layout);

			sprintf(t2, format, CheatFinderReadBytes(cfCurrentRAM, cfAddress[i]));
			strcpy(t1, "            ");
			t1[11 - strlen(t2)] = 0;
			strcat(t1, t2);
			len = runLength[0] = strlen(t1);
			for (unsigned int n = 0; n < len; n++)
				unistr[n] = t1[n];
			err = ATSUCreateTextLayoutWithTextPtr(unistr, kATSUFromTextBeginning, kATSUToTextEnd, len, 1, runLength, &cfListLineATSUStyle, &layout); 
			err = ATSUSetLayoutControls(layout, sizeof(theTags) / sizeof(theTags[0]), theTags, theSizes, theValues);
			err = ATSUDrawText(layout, kATSUFromTextBeginning, kATSUToTextEnd, vx, f);
			err = ATSUDisposeTextLayout(layout);

			CGContextScaleCTM(ctx, 1, -1);
		}

		y += myData->lineSize.height;
		lineBounds.origin.y += myData->lineSize.height;
	}
}

static HIViewPartCode CheatFinderListViewFindPart(EventRef inEvent, ListViewData *myData, SInt32 *whichLine)
{
	OSStatus		err;
	HIViewPartCode	part;
	HIPoint			hipt;
	SInt32			start, line;
	float			y;
	
	part = kControlNoPart;

	start = (SInt32) (myData->originPoint.y / myData->lineSize.height);
	y = start * myData->lineSize.height - myData->originPoint.y;

	err = GetEventParameter(inEvent, kEventParamMouseLocation, typeHIPoint, nil, sizeof(hipt), nil, &hipt);
	if (err == noErr)
	{
		line = start + (SInt32) ((hipt.y - y - 1) / myData->lineSize.height) + 1;

		if (line <= GetControl32BitMaximum(myData->view))
			part = kControlListLinePart;
	
		if (whichLine != nil)
			*whichLine = line;
	}
	
	return (part);
}

static float CheatFinderListViewSanityCheck(float where, ListViewData *myData)
{
	HIRect	bounds;
	HISize	imageSize;
	
	HIViewGetBounds(myData->view, &bounds);
	imageSize = myData->lineSize;
	imageSize.height *= GetControl32BitMaximum(myData->view);

	if (where + bounds.size.height > imageSize.height) 
		where = imageSize.height - bounds.size.height;
	if (where < 0)
		where = 0;
	
	return (where);
}

static void CheatFinderListViewScrollToThere(float where, ListViewData *myData)
{
	OSStatus	err;
	EventRef	theEvent;
	HIPoint		whereP = { 0.0, where };
	
	err = CreateEvent(kCFAllocatorDefault, kEventCheatFinderList, kEventScrollableScrollThere, GetCurrentEventTime(), kEventAttributeUserEvent, &theEvent);
	if (err == noErr)
	{
		err = SetEventParameter(theEvent, kEventParamOrigin, typeHIPoint, sizeof(whereP), &whereP);
		if (err == noErr)
			err = SendEventToEventTarget(theEvent, GetControlEventTarget(myData->view));

		ReleaseEvent(theEvent);
	}
}

static pascal OSStatus CheatFinderListViewHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
	OSStatus				err, result = eventNotHandledErr;
	ListViewData	*myData = (ListViewData *) inUserData;

	switch (GetEventClass(inEvent))
	{
		case kEventClassHIObject:
		{
			switch (GetEventKind(inEvent))
			{
				case kEventHIObjectConstruct:
				{
					ListViewData	*data;
					
					data = (ListViewData *) calloc(1, sizeof(ListViewData));
					if (data)
					{
						HIViewRef	epView;
						
						err = GetEventParameter(inEvent, kEventParamHIObjectInstance, typeHIObjectRef, nil, sizeof(epView), nil, &epView);
						if (err == noErr)
						{					
							data->view = epView;
							data->originPoint.x   =  0.0;
							data->originPoint.y   =  0.0;
							data->lineSize.width  =  0.0;
							data->lineSize.height = 16.0;
							data->inFocus = false;

							SetControlID(epView, &kCheatFinderListViewID);

							result = SetEventParameter(inEvent, kEventParamHIObjectInstance, typeVoidPtr, sizeof(data), &data);
						}
					}

					break;
				}
				
				case kEventHIObjectDestruct:
				{
					if (myData)
						free(myData);

					result = noErr;
					break;
				}

				case kEventHIObjectInitialize:
				{
					result = CallNextEventHandler(inHandlerCallRef, inEvent);
					break;
				}
			}

			break;
		}
		
		case kEventClassScrollable:
		{
			switch (GetEventKind(inEvent))
			{
				case kEventScrollableGetInfo:
				{
					HIRect	bounds;
					HISize	imageSize;
					
					HIViewGetBounds(myData->view, &bounds);
					imageSize = myData->lineSize;
					imageSize.height *= GetControl32BitMaximum(myData->view);

					err = SetEventParameter(inEvent, kEventParamImageSize, typeHISize,  sizeof(imageSize),           &imageSize);
					err = SetEventParameter(inEvent, kEventParamViewSize,  typeHISize,  sizeof(bounds.size),         &bounds.size);
					err = SetEventParameter(inEvent, kEventParamLineSize,  typeHISize,  sizeof(myData->lineSize),    &myData->lineSize);
					err = SetEventParameter(inEvent, kEventParamOrigin,    typeHIPoint, sizeof(myData->originPoint), &myData->originPoint);

					result = noErr;
					break;
				}

				case kEventScrollableScrollTo:
				{
					HIPoint	where;

					err = GetEventParameter(inEvent, kEventParamOrigin, typeHIPoint, nil, sizeof(where), nil, &where);
					if (err == noErr)
					{
						where.y = CheatFinderListViewSanityCheck(where.y, myData);
						if ((int) myData->originPoint.y == (int) where.y)
							break;					
						myData->originPoint.y = where.y;

						HIViewSetNeedsDisplay(myData->view, true);
					}
					
					break;
				}
			}
			
			break;
		}
		
		case kEventCheatFinderList:
		{
			switch (GetEventKind(inEvent))
			{
				case kEventScrollableScrollThere:
				{
					HIPoint	where;
					
					err = GetEventParameter(inEvent, kEventParamOrigin, typeHIPoint, nil, sizeof(where), nil, &where);
					if (err == noErr)
					{
						where.y = CheatFinderListViewSanityCheck(where.y, myData);
						if ((int) myData->originPoint.y != (int) where.y)
						{
							myData->originPoint.y = where.y;
							SetControl32BitValue(HIViewGetNextView(myData->view), (SInt32) myData->originPoint.y);
					
							HIViewSetNeedsDisplay(myData->view, true);
						}
					}

					break;
				}
			}
			
			break;
		}
		
		case kEventClassControl:
		{
			switch (GetEventKind(inEvent))
			{
				case kEventControlDraw:
				{
					CGContextRef	ctx;

					err = GetEventParameter(inEvent, kEventParamCGContextRef, typeCGContextRef, nil, sizeof(ctx), nil, &ctx);
					if (err == noErr)
					{
						HIRect	bounds;
					
						HIViewGetBounds(myData->view, &bounds);
						
						CGContextSaveGState(ctx);

						CheatFinderListViewDraw(ctx, &bounds, myData);

						CGContextRestoreGState(ctx);

						result = noErr;
					}

					break;
				}

				case kEventControlHitTest:
				{
					HIViewPartCode	part;
					
					part = CheatFinderListViewFindPart(inEvent, myData, nil);

					result = SetEventParameter(inEvent, kEventParamControlPart, typeControlPartCode, sizeof(part), &part);

					break;
				}
				
				case kEventControlTrack:
				{
					MouseTrackingResult	trackResult;
					WindowRef			window;
					HIViewRef			contentView;
					HIViewPartCode		part;
					HIRect				frame;
					CGrafPtr			oldPort;
					SInt32				whichLine;
					
					part = CheatFinderListViewFindPart(inEvent, myData, &whichLine);
					if (part == kControlListLinePart)
						SetControl32BitValue(myData->view, whichLine);

					window = GetControlOwner(myData->view);

					HIViewFindByID(HIViewGetRoot(window), kHIViewWindowContentID, &contentView);
					HIViewGetFrame(myData->view, &frame);
					HIViewConvertRect(&frame, myData->view, contentView);
					
					Boolean	portChanged;
					Point	qdpt;
					
					portChanged = QDSwapPort(GetWindowPort(window), &oldPort);

					TrackMouseLocation(nil, &qdpt, &trackResult);

					while (trackResult != kMouseTrackingMouseUp)
					{
						SInt32	start, line;
						float	y;
						
						start = (SInt32) (myData->originPoint.y / myData->lineSize.height);
						y = start * myData->lineSize.height - myData->originPoint.y;
						line = (SInt32) ((qdpt.v - frame.origin.y - y - 1) / myData->lineSize.height) + 1;
						if (qdpt.v < frame.origin.y)
							line--;
						line += start;

						if (line != whichLine)
						{
							whichLine = line;
							SetControl32BitValue(myData->view, whichLine);

							SInt32	goThere = -1;

							if (whichLine <= start)
								goThere = whichLine - 1;
							else
							if (whichLine >=  start + (SInt32) (frame.size.height / myData->lineSize.height) + 1)
								goThere = whichLine - (SInt32) (frame.size.height / myData->lineSize.height) - 1;
								
					 		if (goThere != -1)
							{
								float	where;
								
								where = goThere * myData->lineSize.height;
								where = CheatFinderListViewSanityCheck(where, myData);
								if ((int) where != (int) myData->originPoint.y)
									CheatFinderListViewScrollToThere(where, myData);
							}
						}

						TrackMouseLocation(nil, &qdpt, &trackResult);
					}
					
					if (portChanged)
						QDSwapPort(oldPort, nil);

					err = SetEventParameter(inEvent, kEventParamControlPart, typeControlPartCode, sizeof(part), &part);

					cfListSelection = GetControl32BitValue(myData->view) - 1;
					
					result = noErr;
					break;
				}

				case kEventControlValueFieldChanged:
				{
					EventRef	theEvent;
					
					myData->originPoint.y = CheatFinderListViewSanityCheck(myData->originPoint.y, myData);

					err = CreateEvent(kCFAllocatorDefault, kEventClassScrollable, kEventScrollableInfoChanged, GetCurrentEventTime(), kEventAttributeUserEvent, &theEvent);
					if (err == noErr)
					{
						err = SendEventToEventTarget(theEvent, GetControlEventTarget(myData->view));
						ReleaseEvent(theEvent);
						
						HIViewSetNeedsDisplay(myData->view, true);
					}
					
					break;
				}
			}
		}
	}
	
	return (result);
}
