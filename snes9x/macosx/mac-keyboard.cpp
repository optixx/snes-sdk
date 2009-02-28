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
#include "mac-dialog.h"
#include "mac-gworld.h"
#include "mac-os.h"
#include "mac-keyboard.h"

#define kmUpArrowKey	0x7E
#define kmDownArrowKey	0x7D
#define kmRightArrowKey	0x7C
#define kmLeftArrowKey	0x7B
#define kmReturnKey		0x24
#define kmTabKey		0x30
#define kmShiftKey		0x38
#define kmControlKey	0x3B
#define kmOptionKey		0x3A
#define kmCommandKey	0x37
#define kmXKey			0x07
#define kmZKey			0x06
#define kmKP2Key		0x54
#define kmKP4Key		0x56
#define kmKP5Key		0x57
#define kmKP6Key		0x58
#define kmKP8Key		0x5B
#define kmKPEnterKey	0x4C
#define kmKPPlusKey		0x45
#define kmKP0Key		0x52
#define kmKPPeriodKey	0x41
#define kmHomeKey		0x73
#define kmPageUpKey		0x74
#define kmEndKey		0x77
#define kmPageDownKey	0x79
#define kmBackslashKey	0x2A 
#define km1Key			0x12
#define km0Key			0x1D
#define kmIKey			0x22
#define kmJKey			0x26
#define kmKKey			0x28
#define kmLKey			0x25
#define kmTildeKey		0x32
#define	kmRKey			0x0F
#define	kmBKey			0x0B
#define	kmNKey			0x2D
#define	kmMKey			0x2E
#define	kmSpaceKey		0x31
#define kmSlashKey		0x2C
#define	kmPeriodKey		0x2F
#define	kmQKey			0x0C
#define	kmWKey			0x0D
#define	kmEscKey		0x35
#define	kmCommaKey		0x2B

#define kIconSize		16
#define kKeySize		24
#define KS				kKeySize

UInt8	keyCode[kKeys] =
{
	kmUpArrowKey,
	kmDownArrowKey,
	kmLeftArrowKey,
	kmRightArrowKey,
	kmShiftKey,
	kmOptionKey,
	kmControlKey,
	kmCommandKey,
	kmZKey,
	kmXKey,
	kmReturnKey,
	kmTabKey,
	
	kmKP8Key,
	kmKP2Key,
	kmKP4Key,
	kmKP6Key,
	kmPageDownKey,
	kmPageUpKey,
	kmEndKey,
	kmHomeKey,
	kmKP0Key,
	kmKPPeriodKey,
	kmKPEnterKey,
	kmKPPlusKey,
	
	kmBackslashKey,
	km1Key,
	km0Key,
	kmTildeKey,
	kmRKey,	
	kmBKey,
	kmNKey,
	kmMKey,
	kmSpaceKey,
	kmSlashKey,
	kmPeriodKey,
	kmQKey,
	kmWKey,
	kmEscKey,
	kmCommaKey
};

typedef struct
{ 
	uint8	keyWidth, keyHeight;
	uint8	scancode;
  	char	*keyLabel;
}	KeyboardLayout;

typedef struct
{
	HIViewRef	view;
}	CustomViewData;

static CGImageRef		iconTableImage;
static CGImageRef		keyLayoutImage;
static CGImageRef		iconPlaceImage;
#ifdef MAC_JAGUAR_SUPPORT
static GWorldPtr		iconTableWorld;	// for 10.2
#endif
static Ptr				iconTableCGWld;	// for 10.3 and higher
static Ptr				keyLayoutWorld;
static Ptr				iconPlaceWorld;

static CGRect			keyRect[0x80][2];
static UInt8			defaultKeys[kKeys];

static HIObjectClassRef	theClass;
static HIViewRef		customView;
static HIPoint			mousePos;
static float			ofsx, ofsy;
static int				dragKey;
static CGPoint			dragKeyOfs;
static CGRect			dragKeyRect;
static volatile Boolean	keyInDrag;

static const int		kKeyLayoutWidth  = kKeySize * 23 + 1,
						kKeyLayoutHeight = kKeySize *  7 + 1;

#define	kCustomLayoutViewClassID	CFSTR("com.snes9x.macos.snes9x.keylayout")
					
static void CreateIconTableImage(void);
static void ReleaseIconTableImage(void);
static void CreateKeyLayoutImage(void);
static void ReleaseKeyLayoutImage(void);
static void CreateIconPlaceImage(void);
static void UpdateIconPlaceImage(void);
static void ReleaseIconPlaceImage(void);
static void DrawPlacedIcon(CGContextRef, int);
static void DrawDraggedIcon(CGContextRef, int, CGPoint *);
static Boolean KeyCodeInUse(int);
static int FindHitKey(HIPoint, CGRect *, CGPoint *);
static pascal OSStatus KeyWindowEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus KeyLayoutEventHandler(EventHandlerCallRef, EventRef, void *);

static KeyboardLayout	keys[] =
{
	{ KS, 		KS, 	0x35, 	"esc"    },
	{ KS, 		KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x7a, 	"F1"     },
	{ KS, 		KS, 	0x78, 	"F2"     },
	{ KS, 		KS, 	0x63, 	"F3"     },
	{ KS, 		KS, 	0x76, 	"F4"     },
	{ KS/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x60, 	"F5"     },
	{ KS, 		KS, 	0x61, 	"F6"     },
	{ KS, 		KS, 	0x62, 	"F7"     },
	{ KS, 		KS, 	0x64, 	"F8"     },
	{ KS/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x65, 	"F9"     },
	{ KS, 		KS, 	0x6d, 	"F10"    },
	{ KS, 		KS, 	0x67, 	"F11"    },
	{ KS, 		KS, 	0x6f, 	"F12"    },
	{ KS/2, 	KS, 	0x00,	nil      },
	{ KS, 		KS, 	0x69, 	"F13"    },
	{ KS, 		KS, 	0x6b, 	"F14"    },
	{ KS, 		KS, 	0x71, 	"F15"    },
	{ 0,  		0,  	0x00, 	nil      },

	{ 0,  		0,  	0x00, 	nil      },

	{ KS, 		KS, 	0x32, 	"`"      },
	{ KS, 		KS, 	0x12, 	"1"      },
	{ KS, 		KS, 	0x13, 	"2"      },
	{ KS, 		KS, 	0x14, 	"3"      },
	{ KS, 		KS, 	0x15, 	"4"      },
	{ KS, 		KS, 	0x17, 	"5"      },
	{ KS, 		KS, 	0x16, 	"6"      },
	{ KS, 		KS, 	0x1a, 	"7"      },
	{ KS, 		KS, 	0x1c, 	"8"      },
	{ KS, 		KS, 	0x19, 	"9"      },
	{ KS, 		KS, 	0x1d, 	"0"      },
	{ KS, 		KS, 	0x1b, 	"-"      },
	{ KS, 		KS, 	0x18, 	"="      },
	{ KS*2, 	KS, 	0x33, 	"delete" },
	{ KS/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x72, 	"ins"    },
	{ KS, 		KS, 	0x73, 	"hom"    },
	{ KS, 		KS, 	0x74, 	"pgu"    },
	{ KS/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x47, 	"clr"    },
	{ KS, 		KS, 	0x51, 	"="      },
	{ KS, 		KS, 	0x4b, 	"/"      },
	{ KS, 		KS, 	0x43, 	"*"      },
	{ 0,  		0,  	0x00, 	nil      },

	{ KS*3/2, 	KS, 	0x30, 	"tab"    },
	{ KS, 		KS, 	0x0c, 	"Q"      },
	{ KS, 		KS, 	0x0d, 	"W"      },
	{ KS, 		KS, 	0x0e, 	"E"      },
	{ KS, 		KS, 	0x0f, 	"R"      },
	{ KS, 		KS, 	0x11, 	"T"      },
	{ KS, 		KS, 	0x10, 	"Y"      },
	{ KS, 		KS, 	0x20, 	"U"      },
	{ KS, 		KS, 	0x22, 	"I"      },
	{ KS, 		KS, 	0x1f, 	"O"      },
	{ KS, 		KS, 	0x23, 	"P"      },
	{ KS, 		KS, 	0x21, 	"["      },
	{ KS, 		KS, 	0x1e, 	"]"      },
	{ KS*3/2, 	KS, 	0x2a, 	"\\"     },
	{ KS/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x75, 	"del"    },
	{ KS, 		KS, 	0x77, 	"end"    },
	{ KS, 		KS, 	0x79, 	"pgd"    },
	{ KS/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x59, 	"7"      },
	{ KS, 		KS, 	0x5b, 	"8"      },
	{ KS, 		KS, 	0x5c, 	"9"      },
	{ KS, 		KS, 	0x4e, 	"-"      },
	{ 0,  		0,  	0x00, 	nil      },
	
	{ KS*2, 	KS, 	0x39, 	"caps"   },
	{ KS, 		KS, 	0x00, 	"A"      },
	{ KS, 		KS, 	0x01, 	"S"      },
	{ KS, 		KS, 	0x02, 	"D"      },
	{ KS, 		KS, 	0x03, 	"F"      },
	{ KS, 		KS, 	0x05, 	"G"      },
	{ KS, 		KS, 	0x04, 	"H"      },
	{ KS, 		KS, 	0x26, 	"J"      },
	{ KS, 		KS, 	0x28, 	"K"      },
	{ KS, 		KS, 	0x25, 	"L"      },
	{ KS, 		KS, 	0x29, 	";"      },
	{ KS, 		KS, 	0x27, 	"\xd3"   },
	{ KS*2, 	KS, 	0x24, 	"return" },
	{ KS*4, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x56, 	"4"      },
	{ KS, 		KS, 	0x57, 	"5"      },
	{ KS, 		KS, 	0x58, 	"6"      },
	{ KS, 		KS, 	0x45, 	"+"      },
	{ 0,  		0,  	0x00, 	nil      },
	
	{ KS*5/2, 	KS, 	0x38, 	"shift"  },
	{ KS, 		KS, 	0x06, 	"Z"      },
	{ KS, 		KS, 	0x07,	"X"      },
	{ KS, 		KS, 	0x08, 	"C"      },
	{ KS, 		KS, 	0x09, 	"V"      },
	{ KS, 		KS, 	0x0b, 	"B"      },
	{ KS, 		KS, 	0x2d, 	"N"      },
	{ KS, 		KS, 	0x2e, 	"M"      },
	{ KS, 		KS, 	0x2b, 	","      },
	{ KS, 		KS, 	0x2f, 	"."      },
	{ KS, 		KS, 	0x2c, 	"/"      },
	{ KS*5/2, 	KS, 	0x38, 	"shift"  },
	{ KS*3/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x7e, 	"up"     },
	{ KS*3/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x53, 	"1"      },
	{ KS, 		KS, 	0x54, 	"2"      },
	{ KS, 		KS, 	0x55, 	"3"      },
	{ KS, 		KS*2, 	0x4c, 	"ent"    },
	{ 0,  		0,  	0x00, 	nil      },
	
	{ KS*3/2, 	KS, 	0x3b, 	"ctrl"   },
	{ KS*3/2, 	KS, 	0x3a, 	"opt"    },
	{ KS*3/2, 	KS, 	0x37, 	"cmd"    },
	{ KS*6, 	KS, 	0x31, 	"   "    },
	{ KS*3/2, 	KS, 	0x37, 	"cmd"    },
	{ KS*3/2, 	KS, 	0x3a, 	"opt"    },
	{ KS*3/2, 	KS, 	0x3b, 	"ctrl"   },
	{ KS/2, 	KS, 	0x00, 	nil      },
	{ KS, 		KS, 	0x7b, 	"lt"     },
	{ KS, 		KS, 	0x7d, 	"dn"     },
	{ KS, 		KS, 	0x7c, 	"rt"     },
	{ KS/2, 	KS, 	0x00, 	nil      },
	{ KS*2, 	KS, 	0x52, 	"0"      },
	{ KS, 		KS, 	0x41, 	"."      },
	{ 0,  		0,  	0x00, 	nil      },
};

void ConfigureKeyboard(void)
{
	OSStatus	err;
	IBNibRef	nibRef;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		WindowRef	tWindowRef;

		err = CreateWindowFromNib(nibRef, CFSTR("Keyboard"), &tWindowRef);
		if (err == noErr)
		{
			EventHandlerRef	wref;
			EventHandlerUPP	wUPP;
			EventTypeSpec	wEvents[] = { { kEventClassWindow,   kEventWindowClose        },
										  { kEventClassCommand,  kEventCommandProcess     } },
							cEvents[] = { { kEventClassHIObject, kEventHIObjectConstruct  },
										  { kEventClassHIObject, kEventHIObjectDestruct   },
										  { kEventClassHIObject, kEventHIObjectInitialize },
										  { kEventClassControl,  kEventControlDraw        },
										  { kEventClassControl,  kEventControlHitTest     },
										  { kEventClassControl,  kEventControlTrack       } };
			HIObjectRef		hiObject;
			HIViewRef		contentView;
			HIRect			frame;
			Rect			winBounds;

			UpdateIconPlaceImage();

			keyInDrag = false;
			dragKey = -1;
			dragKeyOfs = CGPointMake(0.0, 0.0);
			dragKeyRect = CGRectMake(0.0, 0.0, 0.0, 0.0);
			mousePos = CGPointMake(0.0, 0.0);

			err = noErr;
			if (theClass == nil)
				err = HIObjectRegisterSubclass(kCustomLayoutViewClassID, kHIViewClassID, 0, KeyLayoutEventHandler, GetEventTypeCount(cEvents), cEvents, nil, &theClass);

			if (err == noErr)
			{
				err = HIObjectCreate(kCustomLayoutViewClassID, nil, &hiObject);
				if (err == noErr)
				{
					GetWindowBounds(tWindowRef, kWindowContentRgn, &winBounds);

					frame.origin.x = 2.0;
					frame.origin.y = 2.0;
					frame.size.width  = (float) (winBounds.right - winBounds.left) - 4.0;
					frame.size.height = (float) kKeyLayoutHeight + 36.0;
					
					ofsx = (float) (((int) frame.size.width  - kKeyLayoutWidth ) >> 1) + 1.0;
					ofsy = (float) (((int) frame.size.height - kKeyLayoutHeight) >> 1) + 1.0;

					customView = (HIViewRef) hiObject;

					HIViewFindByID(HIViewGetRoot(tWindowRef), kHIViewWindowContentID, &contentView);
					HIViewAddSubview(contentView, customView);
					HIViewSetFrame(customView, &frame);
					HIViewSetVisible(customView, true);

					wUPP = NewEventHandlerUPP(KeyWindowEventHandler);
					err = InstallWindowEventHandler(tWindowRef, wUPP, GetEventTypeCount(wEvents), wEvents, (void *) tWindowRef, &wref);
			
					MoveWindowPosition(tWindowRef, kWindowKeyConfig, false);
					ShowWindow(tWindowRef);
					err = RunAppModalLoopForWindow(tWindowRef);
					HideWindow(tWindowRef);
					SaveWindowPosition(tWindowRef, kWindowKeyConfig);

					err = RemoveEventHandler(wref);
					DisposeEventHandlerUPP(wUPP);
				}
			}
		
			ReleaseWindow(tWindowRef);
		}
		
		DisposeNibReference(nibRef);
	}
}

static void CreateIconTableImage(void)
{
	if (systemVersion >= 0x1030)
	{
		OSStatus			err;
		CGContextRef		ctx;
		CGDataProviderRef	prov;
		CGColorSpaceRef		color;
		CGRect				rct;
		
		rct = CGRectMake(0.0, 0.0, (float) kIconSize, (float) kIconSize);
		
		iconTableCGWld = (Ptr) malloc(kIconSize * kKeys * (kIconSize + 1) * 4);
		if (!iconTableCGWld)
			QuitWithFatalError(0, "keyboard 08");
		
		ctx = nil;
		
		color = CGColorSpaceCreateDeviceRGB();
		if (color)
		{
			ctx = CGBitmapContextCreate(iconTableCGWld, kIconSize * kKeys, kIconSize, 8, kIconSize * kKeys * 4, color, kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrderDefault : 0));
			CGColorSpaceRelease(color);
		}
		
		if (!ctx)
			QuitWithFatalError(0, "keyboard 09");
		
		CGContextTranslateCTM(ctx, 0.0, (float) kIconSize);
		CGContextScaleCTM(ctx, 1.0, -1.0);
		
		for (int i = 0; i < kKeys; i++)
		{
			IconSuiteRef		iSuite;
			IconFamilyHandle	iFamily;
			IconRef				iRef;
			
			err = GetIconSuite(&iSuite, 800 + i, kSelectorSmall8Bit | kSelectorSmall1Bit);
			err = IconSuiteToIconFamily(iSuite, kSelectorSmall8Bit | kSelectorSmall1Bit, &iFamily);
			HLock((Handle) iFamily);
			err = GetIconRefFromIconFamilyPtr(*iFamily, GetHandleSize((Handle) iFamily), &iRef);
			HUnlock((Handle) iFamily);
			err = PlotIconRefInContext(ctx, &rct, kAlignAbsoluteCenter, kTransformNone, nil, kPlotIconRefNormalFlags, iRef);
			err = ReleaseIconRef(iRef);
			DisposeHandle((Handle) iFamily);
			DisposeIconSuite(iSuite, true);
			
			rct = CGRectOffset(rct, kIconSize, 0);
		}
		
		CGContextRelease(ctx);
		
		iconTableImage = nil;
		
		prov = CGDataProviderCreateWithData(nil, iconTableCGWld, kIconSize * kKeys * kIconSize * 4, nil);
		if (prov)
		{
			color = CGColorSpaceCreateDeviceRGB();
			if (color)
			{
				iconTableImage = CGImageCreate(kIconSize * kKeys, kIconSize, 8, 32, kIconSize * kKeys * 4, color, kCGImageAlphaNoneSkipFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrderDefault : 0), prov, nil, 0, kCGRenderingIntentDefault);
				CGColorSpaceRelease(color);
			}
			
			CGDataProviderRelease(prov);
		}
		
		if (!iconTableImage)
			QuitWithFatalError(0, "keyboard 10");
	}
#ifdef MAC_JAGUAR_SUPPORT
	else
	{
		Rect	bounds, icnrct;

		SetRect(&icnrct, 0, 0, kIconSize, kIconSize);
		SetRect(&bounds, 0, 0, kIconSize * kKeys, kIconSize + 1);

		InitGWorld(&iconTableWorld, &bounds, 32);
		PrepareForGDrawing(iconTableWorld);
		
		for (int i = 0; i < kKeys; i++)
		{
			IconSuiteRef	icon;

			GetIconSuite(&icon, 800 + i, kSelectorSmall8Bit | kSelectorSmall1Bit);
			PlotIconSuite(&icnrct, kAlignAbsoluteCenter, kTransformNone, icon);
			DisposeIconSuite(icon, true);

			OffsetRect(&icnrct, kIconSize, 0);
		}

		FinishGDrawing(iconTableWorld);

		iconTableImage = CreateCGImageFromGWorld(iconTableWorld);
		if (!iconTableImage)
			QuitWithFatalError(0, "keyboard 01");
	}
#endif
}

static void ReleaseIconTableImage(void)
{
	CGImageRelease(iconTableImage);
	if (systemVersion >= 0x1030)
		free(iconTableCGWld);
#ifdef MAC_JAGUAR_SUPPORT	
	else
		DisposeGWorld(iconTableWorld);
#endif
}

static void CreateKeyLayoutImage(void)
{
	CGContextRef		ctx;
	CGDataProviderRef	prov;
	CGColorSpaceRef		color;
	CGAffineTransform	flipMatrix;
	CGRect				rct, r;
	int					index, scancode;

	rct = CGRectMake(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 0x80; i++)
		keyRect[i][0] = keyRect[i][1] = rct;

	keyLayoutWorld = (Ptr) malloc(kKeyLayoutWidth * (kKeyLayoutHeight + 1) * 4);
	if (!keyLayoutWorld)
		QuitWithFatalError(0, "keyboard 02");
	
	ctx = nil;

	color = CGColorSpaceCreateDeviceRGB();
	if (color)
	{
		ctx = CGBitmapContextCreate(keyLayoutWorld, kKeyLayoutWidth, kKeyLayoutHeight, 8, kKeyLayoutWidth * 4, color, kCGImageAlphaPremultipliedFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrderDefault : 0));	
		CGColorSpaceRelease(color);
	}

	if (!ctx)
		QuitWithFatalError(0, "keyboard 04");

	CGContextSetLineJoin(ctx, kCGLineJoinMiter);

	flipMatrix = CGAffineTransformMake(1.0, 0.0, 0.0, -1.0, 0.0, 0.0);
	CGContextSelectFont(ctx, "Helvetica", 10.0, kCGEncodingMacRoman);
	CGContextSetTextDrawingMode(ctx, kCGTextFill); 
	CGContextSetTextMatrix(ctx, flipMatrix);

	rct = CGRectMake(0.0, 0.0, (float) kKeyLayoutWidth, (float) kKeyLayoutHeight);
	CGContextClearRect(ctx, rct);

	index = 0;
	rct = CGRectMake(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < 7; i++)
	{	
		while (keys[index].keyWidth)
		{
			rct.size.width = (float) keys[index].keyWidth;
			
			if (keys[index].keyLabel)
			{
				rct.size.height = (float) keys[index].keyHeight;
				scancode = keys[index].scancode;
				
				if (keyRect[scancode][0].size.height < 1.0)
					keyRect[scancode][0] = rct;
				else
					keyRect[scancode][1] = rct;
				
				r = rct;
				
				r.origin.x += 1.0;
				r.origin.y += 1.0;
				r.size.width  -= 1.0;
				r.size.height -= 1.0;

				CGContextSetRGBStrokeColor(ctx, 0.1, 0.1, 0.1, 1.0);
				CGContextStrokeRect(ctx, r);
				
				float	h, f, p;
				
				CGRectInset(r, 2.0, 2.0);
				h = r.size.height;
				for (f = h; f >= 1.0; f -= 1.0)
				{
					p = (155.0 + (h - f)) / 180.0;
					CGContextSetRGBFillColor(ctx, p, p, p, 1.0);
					CGContextFillRect(ctx, r);
					r.size.height -= 1.0;
				}

				CGContextSetRGBFillColor(ctx, 0.1, 0.1, 0.1, 1.0);
				CGContextShowTextAtPoint(ctx, rct.origin.x + 3.0, rct.origin.y + rct.size.height - 3.0, keys[index].keyLabel, strlen(keys[index].keyLabel));
			}
			
			rct.origin.x += rct.size.width;
			index++;
		}
		
		rct.origin.y += kKeySize;
		rct.origin.x  = rct.size.width = 0;
		index++;
	}

	CGContextRelease(ctx);

	keyLayoutImage = nil;

	prov = CGDataProviderCreateWithData(nil, keyLayoutWorld, kKeyLayoutWidth * kKeyLayoutHeight * 4, nil);
	if (prov)
	{
		color = CGColorSpaceCreateDeviceRGB();
		if (color)
		{
			keyLayoutImage = CGImageCreate(kKeyLayoutWidth, kKeyLayoutHeight, 8, 32, kKeyLayoutWidth * 4, color, kCGImageAlphaPremultipliedFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrderDefault : 0), prov, nil, 0, kCGRenderingIntentDefault);
			CGColorSpaceRelease(color);
		}

		CGDataProviderRelease(prov);
	}

	if (!keyLayoutImage)
		QuitWithFatalError(0, "keyboard 05");
}

static void ReleaseKeyLayoutImage(void)
{
	CGImageRelease(keyLayoutImage);
	free(keyLayoutWorld);
}

static void CreateIconPlaceImage(void)
{	
	iconPlaceWorld = (Ptr) malloc(kKeyLayoutWidth * (kKeyLayoutHeight + 1) * 4);
	if (!iconPlaceWorld)
		QuitWithFatalError(0, "keyboard 06");

	iconPlaceImage = nil;

	UpdateIconPlaceImage();
}

static void UpdateIconPlaceImage(void)
{
	CGContextRef		ctx;
	CGDataProviderRef	prov;
	CGColorSpaceRef		color;
	CGRect				rct;
	
	if (iconPlaceImage)
		CGImageRelease(iconPlaceImage);

	iconPlaceImage = nil;

	color = CGColorSpaceCreateDeviceRGB();
	if (color)
	{
		ctx = CGBitmapContextCreate(iconPlaceWorld, kKeyLayoutWidth, kKeyLayoutHeight, 8, kKeyLayoutWidth * 4, color, kCGImageAlphaPremultipliedFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrderDefault : 0));	
		if (ctx)
		{
			rct = CGRectMake(0.0, 0.0, (float) kKeyLayoutWidth, (float) kKeyLayoutHeight);
			CGContextDrawImage(ctx, rct, keyLayoutImage);

			for (int i = 0; i < kKeys; i++)
				DrawPlacedIcon(ctx, i);

			CGContextRelease(ctx);
		}

		prov = CGDataProviderCreateWithData(nil, iconPlaceWorld, kKeyLayoutWidth * kKeyLayoutHeight * 4, nil);
		if (prov)
		{
			iconPlaceImage = CGImageCreate(kKeyLayoutWidth, kKeyLayoutHeight, 8, 32, kKeyLayoutWidth * 4, color, kCGImageAlphaPremultipliedFirst | ((systemVersion >= 0x1040) ? kCGBitmapByteOrderDefault : 0), prov, nil, 0, kCGRenderingIntentDefault);
			CGDataProviderRelease(prov);
		}
		
		CGColorSpaceRelease(color);
	}

	if (!iconPlaceImage)
		QuitWithFatalError(0, "keyboard 07");
}

static void ReleaseIconPlaceImage(void)
{
	CGImageRelease(iconPlaceImage);
	free(iconPlaceWorld);
}

void InitKeyboard(void)
{
	theClass = nil;
	
	memcpy(defaultKeys, keyCode, sizeof(keyCode));
	
	CreateIconTableImage();
	CreateKeyLayoutImage();
	CreateIconPlaceImage();
}

void DeinitKeyboard(void)
{
	ReleaseIconPlaceImage();
	ReleaseKeyLayoutImage();
	ReleaseIconTableImage();
}

static void DrawPlacedIcon(CGContextRef ctx, int which)
{
	CGRect	keyBounds, srcRect, dstRect;
	
	CGContextSaveGState(ctx);

	CGContextSetRGBFillColor(ctx, 0.40, 0.40, 0.65, 0.5);

	for (int each = 0; each <= 1; each++)
	{
		keyBounds = keyRect[keyCode[which]][each];

		if (keyBounds.size.height > 1.0)
		{
			keyBounds.origin.x += 1.0;
			keyBounds.origin.y += 1.0;
			keyBounds.size.width  -= 1.0;
			keyBounds.size.height -= 1.0;
			
			CGContextFillRect(ctx, keyBounds);
			
			keyBounds.origin.x -= 1.0;
			keyBounds.origin.y -= 1.0;
			keyBounds.size.width  += 1.0;
			keyBounds.size.height += 1.0;

			srcRect.origin.x = (float) (which * kIconSize);
			srcRect.origin.y = 0.0;
			srcRect.size.width  = (float) kIconSize;
			srcRect.size.height = (float) kIconSize;
			
			dstRect.origin.x = keyBounds.origin.x + (keyBounds.size.width  - kIconSize) / 2.0;
			dstRect.origin.y = keyBounds.origin.y + (keyBounds.size.height - kIconSize) / 2.0;
			dstRect.size.width  = (float) kIconSize;
			dstRect.size.height = (float) kIconSize;
			
			DrawSubCGImage(ctx, iconTableImage, srcRect, dstRect);
		}
	}

	CGContextRestoreGState(ctx);
}

static void DrawDraggedIcon(CGContextRef ctx, int which, CGPoint *offset)
{
	CGRect	srcRect, dstRect;

	CGContextSaveGState(ctx);
	
	srcRect.origin.x = (float) (which * kIconSize);
	srcRect.origin.y = 0.0;
	srcRect.size.width  = (float) kIconSize;
	srcRect.size.height = (float) kIconSize;

	dstRect.origin.x = mousePos.x + offset->x;
	dstRect.origin.y = mousePos.y + offset->y;
	dstRect.size.width  = (float) kIconSize;
	dstRect.size.height = (float) kIconSize;

	CGContextSetAlpha(ctx, 0.5);
	DrawSubCGImage(ctx, iconTableImage, srcRect, dstRect);
	
	CGContextRestoreGState(ctx);
}

static Boolean KeyCodeInUse(int code)
{
	for (int i = 0; i < kKeys; i++)
		if (keyCode[i] == code)
			return true;

	return false;
}

static int FindHitKey(HIPoint where, CGRect *keybounds, CGPoint *offset)
{
	int	hit;
	
	hit = -1;
	*offset = CGPointMake(0.0, 0.0);
	*keybounds = CGRectMake(0.0, 0.0, 0.0, 0.0);

	for (int which = 0; which < kKeys; which++)
	{
		for (int each = 0; each <= 1; each++)
		{
			if (CGRectContainsPoint(keyRect[keyCode[which]][each], where))
			{
				hit = which;

				*keybounds = keyRect[keyCode[which]][each];
				offset->x = keybounds->origin.x + (keybounds->size.width  - kIconSize) / 2.0 - where.x + 18.0;
				offset->y = keybounds->origin.y + (keybounds->size.height - kIconSize) / 2.0 - where.y + 18.0;
			}
		}
	}
	
	return hit;
}

static pascal OSStatus KeyWindowEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
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
					
					err = GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, nil, sizeof(HICommand), nil, &tHICommand);
					if (err == noErr)
					{
						if (tHICommand.commandID == 'DFLT')
						{
							memcpy(keyCode, defaultKeys, sizeof(keyCode));
							UpdateIconPlaceImage();
							HIViewSetNeedsDisplay(customView, true);
							result = noErr;
						}
					}
			}
	}

	return result;
}

static pascal OSStatus KeyLayoutEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
{
	OSStatus		err, result = eventNotHandledErr;
	CustomViewData	*data = (CustomViewData *) inUserData;

	switch (GetEventClass(inEvent))
	{
		case kEventClassHIObject:
			switch (GetEventKind(inEvent))
			{
				case kEventHIObjectConstruct:
					data = (CustomViewData *) calloc(1, sizeof(CustomViewData));
					if (data)
					{		
						HIViewRef	epView;

						err = GetEventParameter(inEvent, kEventParamHIObjectInstance, typeHIObjectRef, nil, sizeof(epView), nil, &epView);
						if (err == noErr)
						{
							data->view = epView;
							result = SetEventParameter(inEvent, kEventParamHIObjectInstance, typeVoidPtr, sizeof(data), &data);
						}
					}

					break;
					
				case kEventHIObjectDestruct:
					if (data)
						free(data);
					
					result = noErr;
					break;
				
				case kEventHIObjectInitialize:
					result = CallNextEventHandler(inHandlerRef, inEvent);
			}

			break;

		case kEventClassControl:
			switch (GetEventKind(inEvent))
			{
				case kEventControlDraw:
					CGContextRef	ctx;

					err = GetEventParameter(inEvent, kEventParamCGContextRef, typeCGContextRef, nil, sizeof(ctx), nil, &ctx);
					if (err == noErr)
					{
						HIRect	bounds, srcRect, dstRect;

						HIViewGetBounds(customView, &bounds);
						srcRect = CGRectMake(0, 0, kKeyLayoutWidth, kKeyLayoutHeight);
						
						dstRect.origin.x = (float) (((int) bounds.size.width  - kKeyLayoutWidth ) >> 1);
						dstRect.origin.y = (float) (((int) bounds.size.height - kKeyLayoutHeight) >> 1);
						dstRect.size.width  = (float) kKeyLayoutWidth;
						dstRect.size.height = (float) kKeyLayoutHeight;
						
						DrawSubCGImage(ctx, iconPlaceImage, srcRect, dstRect);
						if (keyInDrag && (dragKey != -1))
							DrawDraggedIcon(ctx, dragKey, &dragKeyOfs);
					}
					
					result = noErr;
					break;
				
				case kEventControlHitTest: 
					ControlPartCode	part;

					part = kControlButtonPart;
					result = SetEventParameter(inEvent, kEventParamControlPart, typeControlPartCode, sizeof(part), &part);

					break;

      			case kEventControlTrack:
                    MouseTrackingResult	trackResult;
					WindowRef			window;
					CGrafPtr			oldPort;
					HIViewRef			contentView;
					HIPoint				hipt;
					Point				qdpt;
					Boolean				portChanged;
					
 					dragKey = -1;
					dragKeyOfs = CGPointMake(0.0, 0.0);
					dragKeyRect = CGRectMake(0.0, 0.0, 0.0, 0.0);
					mousePos = CGPointMake(0.0, 0.0);
					trackResult = kMouseTrackingMouseDown;
					
					window = GetControlOwner(customView);
					HIViewFindByID(HIViewGetRoot(window), kHIViewWindowContentID, &contentView);

					portChanged = QDSwapPort(GetWindowPort(window), &oldPort);

					err = GetEventParameter(inEvent, kEventParamMouseLocation, typeHIPoint, nil, sizeof(hipt), nil, &hipt);
					if (err == noErr)
					{
						hipt.x -= ofsx;
						hipt.y -= ofsy;
						
						dragKey = FindHitKey(hipt, &dragKeyRect, &dragKeyOfs);
						if (dragKey != -1)
						{
							keyInDrag = true;
							
							while (trackResult != kMouseTrackingMouseUp)
	                        {
	                            if (CGPointEqualToPoint(mousePos, hipt) == 0)
								{
									mousePos = hipt;
									HIViewSetNeedsDisplay(customView, true);
	                            }
	
								TrackMouseLocation(nil, &qdpt, &trackResult);

								hipt.x = qdpt.h - ofsx;
								hipt.y = qdpt.v - ofsy;
	        					HIViewConvertPoint(&hipt, contentView, customView);
							}

							keyInDrag = false;
							
							for (int code = 0; code < 0x80; code++)
							{
								for (int each = 0; each <= 1; each++)
								{
									if (CGRectContainsPoint(keyRect[code][each], mousePos))
									{
										if (!KeyCodeInUse(code))
										{
											keyCode[dragKey] = code;
											UpdateIconPlaceImage();
										}
									}
								}
							}
							
							HIViewSetNeedsDisplay(customView, true);
						}
					}
					
					if (portChanged)
						QDSwapPort(oldPort, nil);

					result = noErr;
			}
	}

	return result;
}
