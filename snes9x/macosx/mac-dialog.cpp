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

#include "mac-prefix.h"
#include "mac-joypad.h"
#include "mac-keyboard.h"
#include "mac-os.h"
#include "mac-render.h"
#include "mac-stringtools.h"
#include "mac-dialog.h"

int		autofireLastTabIndex = 1;

static void RomInfoCopyToClipboard(void);
static void RomInfoBuildInfoText(char *);
static void RomInfoGetLicenseeText(char *);
static void RomInfoGetRegionText(char *);
static void AutofireReadAllSettings(int, HIViewRef);
static void AutofireReadSetting(int, uint16, HIViewRef);
static void AutofireWriteAllSettings(int, HIViewRef);
static void AutofireWriteSetting(int, uint16 *, HIViewRef);
static void AutofireSelectTabPane(HIViewRef, SInt16);
static char *StaticRAMBitSize(void);
static OSStatus UpdateTextControlView(HIViewRef);
static pascal void AutofireSliderActionProc(HIViewRef, HIViewPartCode);
static pascal OSStatus RomInfoEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus AutofireTabEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus AutofireWindowEventHandler(EventHandlerCallRef, EventRef, void *);

static int	tabList[]    = { 2, 257, 258 };

static OSStatus UpdateTextControlView(HIViewRef control)
{
	OSStatus			err;
	WindowAttributes	attr;
	
	err = GetWindowAttributes(GetControlOwner(control), &attr);
	if (err == noErr)
	{
		if (attr & kWindowCompositingAttribute)
			err = HIViewSetNeedsDisplay(control, true);
		else
			Draw1Control(control);
	}

	return err;
}

OSStatus GetStaticTextText(HIViewRef control, Str255 text)
{
	OSStatus	err;
	Size		actualSize;
	
	if ((!control) || (!text))
		return paramErr;
		
	err = GetControlData(control, 0, kControlStaticTextTextTag, 255, (Ptr) (text + 1), &actualSize);
	if (err == noErr)
		text[0] = (actualSize > 255) ? 255 : actualSize;
		
	return err;
}

OSStatus SetStaticTextText(HIViewRef control, Str255 text, Boolean draw)
{
	OSStatus	err;
	
	if ((!control) || (!text))
		return paramErr;

	err = SetControlData(control, 0, kControlStaticTextTextTag, text[0], (Ptr) (text + 1));
	if ((err == noErr) && draw)
		err = UpdateTextControlView(control);
	
	return err;
}

OSStatus SetStaticTextTrunc(HIViewRef control, TruncCode mode, Boolean draw)
{
	OSStatus	err;
	TruncCode	trunc;
	Boolean		multiline;
	
	if (!control)
		return paramErr;
		
	trunc = mode;
	multiline = false;

	err = SetControlData(control, 0, kControlStaticTextIsMultilineTag, sizeof(Boolean), &multiline);
	if (err == noErr)
	{
		err = SetControlData(control, 0, kControlStaticTextTruncTag, sizeof(TruncCode), &trunc);
		if ((err == noErr) && draw)
			err = UpdateTextControlView(control);
	}

	return err;
}

OSStatus SetStaticTextCFString(HIViewRef control, CFStringRef text, Boolean draw)
{
	OSStatus	err;
	
	if ((!control) || (!text))
		return paramErr;

	err = SetControlData(control, 0, kControlStaticTextCFStringTag, sizeof(CFStringRef), (Ptr) &text);			
	if ((err == noErr) && draw)
		err = UpdateTextControlView(control);
	
	return err;
}

OSStatus SetEditTextCFString(HIViewRef control, CFStringRef text, Boolean draw)
{
	OSStatus	err;
	
	if ((!control) || (!text))
		return paramErr;

	err = SetControlData(control, 0, kControlEditTextCFStringTag, sizeof(CFStringRef), (Ptr) &text);
	if ((err == noErr) && draw)
		err = UpdateTextControlView(control);
	
	return err;
}

OSStatus CopyEditTextCFString(HIViewRef control, CFStringRef *text)
{
	OSStatus	err;
	Size		actualSize;
	
	if ((!control) || (!text))
		return paramErr;
		
	err = GetControlData(control, 0, kControlEditTextCFStringTag, sizeof(CFStringRef), (Ptr) text, &actualSize);
		
	return err;
}

OSStatus GetEditTextText(HIViewRef control, Str255 text)
{
	OSStatus	err;
	Size		actualSize;
	
	if ((!control) || (!text))
		return paramErr;
		
	err = GetControlData(control, 0, kControlEditTextTextTag, 255, (Ptr) (text + 1), &actualSize);
	if (err == noErr)
		text[0] = (actualSize > 255) ? 255 : actualSize;
		
	return err;
}

OSStatus SetEditTextText(HIViewRef control, Str255 text, Boolean draw)
{
	OSStatus	err;
	
	if ((!control) || (!text))
		return paramErr;

	err = SetControlData(control, 0, kControlEditTextTextTag, text[0], (Ptr) (text + 1));
	if ((err == noErr) && draw)
		err = UpdateTextControlView(control);
	
	return err;
}

OSStatus SetEditTextSelection(HIViewRef control, SInt16 selStart, SInt16 selEnd)
{
	OSStatus					err;
	ControlEditTextSelectionRec	selection;

	if (!control)
		return paramErr;
		
	selection.selStart = selStart;
	selection.selEnd   = selEnd;
	
	err = SetControlData(control, 0, kControlEditTextSelectionTag, sizeof(selection), (Ptr) &selection);	
	if (err == noErr)
		err = UpdateTextControlView(control);
	
	return err;
}

void StartCarbonModalDialog(void)
{
	HiliteMenu(0);
	DisableMenus();
	if (gWindow)
		HideWindow(gWindow);
}

void FinishCarbonModalDialog(void)
{
	EnableMenus();
	if (gWindow)
		ShowWindow(gWindow);
}

void MoveWindowPosition(WindowRef window, int which, Boolean resize)
{
	if (savewindowpos)
	{
		MoveWindow(window, windowPos[which].h, windowPos[which].v, false);
		
		if (resize)
		{
			if ((windowSize[which].width > 0) && (windowSize[which].height > 0))
				SizeWindow(window, (short) windowSize[which].width, (short) windowSize[which].height, false);
		}
	}
	else
		RepositionWindow(window, nil, kWindowAlertPositionOnMainScreen);
}

void SaveWindowPosition(WindowRef window, int which)
{
	Rect	rct;
	
	GetWindowBounds(window, kWindowContentRgn, &rct);
	windowPos[which].h = rct.left;
	windowPos[which].v = rct.top;
	windowSize[which].width  = (float) (rct.right  - rct.left);
	windowSize[which].height = (float) (rct.bottom - rct.top );
}

void AppearanceAlert(AlertType type, int stringID1, int stringID2)
{
	OSStatus		err;
	DialogRef		dialog;
	DialogItemIndex	outItemHit;
	CFStringRef		key1, key2, mes1, mes2;
	char			label1[32], label2[32];

	sprintf(label1, "AlertMes_%02d", stringID1);
	sprintf(label2, "AlertMes_%02d", stringID2);
	
	key1 = CFStringCreateWithCString(kCFAllocatorDefault, label1, CFStringGetSystemEncoding());
	key2 = CFStringCreateWithCString(kCFAllocatorDefault, label2, CFStringGetSystemEncoding());
	
	if (key1) mes1 = CFCopyLocalizedString(key1, "mes1");	else mes1 = nil;
	if (key2) mes2 = CFCopyLocalizedString(key2, "mes2");	else mes2 = nil;
	
	SysBeep(10);
	err = CreateStandardAlert(type, mes1, mes2, nil, &dialog);
	err = RunStandardAlert(dialog, nil, &outItemHit);
	
	if (key1) CFRelease(key1);
	if (key2) CFRelease(key2);
	if (mes1) CFRelease(mes1);
	if (mes2) CFRelease(mes2);
}

void AboutDialog(void)
{
	OSStatus	err;
	IBNibRef	nibRef;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		WindowRef	tWindowRef;
		
		err = CreateWindowFromNib(nibRef, CFSTR("About"), &tWindowRef);
		if (err == noErr)
		{
			EventHandlerRef		eref;
			EventHandlerUPP		eventUPP;
			EventTypeSpec		windowEvents[] = { { kEventClassWindow, kEventWindowClose } };
			ControlFontStyleRec frec;
			HIViewRef			ctl, root;
			HIViewID			cid;
			char				text[32];
			unsigned char		textP[32];
			
			if (systemVersion >= 0x1040)
				frec.font = FMGetFontFromATSFontRef(ATSFontFindFromName(CFSTR("Lucida Grande"), kATSOptionFlagsDefault));
		#ifdef MAC_PANTHER_JAGUAR_SUPPORT
			else
				frec.font = kThemeSystemFont;
		#endif
			frec.just = teCenter;
			
			root = HIViewGetRoot(tWindowRef);
			cid.id = 0;
			
			cid.signature = 'VERS';
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "Version %s (%s)", VERSION, MAC_VERSION);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			frec.flags = kControlUseFontMask | kControlUseSizeMask | kControlUseJustMask;
			frec.size  = 10;
			err = SetControlFontStyle(ctl, &frec);
			
			cid.signature = 'NAME';
			HIViewFindByID(root, cid, &ctl);
			frec.flags = kControlUseFontMask | kControlUseSizeMask | kControlUseFaceMask | kControlUseJustMask;
			frec.size  = 14;
			frec.style = 1;
			err = SetControlFontStyle(ctl, &frec);
			
			eventUPP = NewEventHandlerUPP(DefaultEventHandler);
			err = InstallWindowEventHandler(tWindowRef, eventUPP, GetEventTypeCount(windowEvents), windowEvents, (void *) tWindowRef, &eref);
			
			MoveWindowPosition(tWindowRef, kWindowAbout, false);
			ShowWindow(tWindowRef);
			err = RunAppModalLoopForWindow(tWindowRef);
			HideWindow(tWindowRef);
			SaveWindowPosition(tWindowRef, kWindowAbout);
			
			err = RemoveEventHandler(eref);
			DisposeEventHandlerUPP(eventUPP);
			
			ReleaseWindow(tWindowRef);
		}
		
		DisposeNibReference(nibRef);
	}
}

pascal OSStatus DefaultEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerRef)
	
	OSStatus	result = eventNotHandledErr;
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
	}
	
	return result;
}

void ConfigureAutofire(void)
{
	OSStatus	err;
	IBNibRef	nibRef;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		WindowRef	tWindowRef;
		
		err = CreateWindowFromNib(nibRef, CFSTR("AutoFire"), &tWindowRef);
		if (err == noErr)
		{
			EventHandlerRef		wRef, tRef;
			EventHandlerUPP		wUPP, tUPP;
			EventTypeSpec		wEvent[] = { { kEventClassWindow,  kEventWindowClose }, { kEventClassCommand, kEventCommandProcess } },
								tEvent[] = { { kEventClassControl, kEventControlHit  } };
			ControlActionUPP	actionUPP;
			HIViewRef			ctl, root;
			HIViewID			cid;
			int					player;
			
			root = HIViewGetRoot(tWindowRef);
			
			wUPP = NewEventHandlerUPP(AutofireWindowEventHandler);
			err = InstallWindowEventHandler(tWindowRef, wUPP, GetEventTypeCount(wEvent), wEvent, (void *) tWindowRef, &wRef);
			
			cid.signature = 'Ftab';
			cid.id = 256;
			HIViewFindByID(root, cid, &ctl);
			SetControl32BitValue(ctl, autofireLastTabIndex);
			AutofireSelectTabPane(ctl, autofireLastTabIndex);
			tUPP = NewEventHandlerUPP(AutofireTabEventHandler);
			err = InstallControlEventHandler(ctl, tUPP, GetEventTypeCount(tEvent), tEvent, 0, &tRef);
			
			actionUPP = NewControlActionUPP(AutofireSliderActionProc);

			for (player = 0; player < 2; player++)
			{
				AutofireReadAllSettings(player + 1, root);

				cid.id = player + 1;
				cid.signature = 'Slid';
				HIViewFindByID(root, cid, &ctl);
				SetControlAction(ctl, actionUPP);
			}
			
			MoveWindowPosition(tWindowRef, kWindowAutoFire, false);
			ShowWindow(tWindowRef);
			err = RunAppModalLoopForWindow(tWindowRef);
			HideWindow(tWindowRef);
			SaveWindowPosition(tWindowRef, kWindowAutoFire);
			
			for (player = 0; player < 2; player++)
				AutofireWriteAllSettings(player + 1, root);
			
			autofire = (autofireRec[0].buttonMask || autofireRec[1].buttonMask) ? true : false;
			
			err = RemoveEventHandler(tRef);
			DisposeEventHandlerUPP(tUPP);
			
			err = RemoveEventHandler(wRef);
			DisposeEventHandlerUPP(wUPP);
			
			DisposeControlActionUPP(actionUPP);
			
			ReleaseWindow(tWindowRef);
		}
		
		DisposeNibReference(nibRef);
	}
}

static void AutofireReadAllSettings(int player, HIViewRef parent)
{
	HIViewRef		ctl;
	HIViewID		cid;
	char			num[10];
	unsigned char	numP[10];
	
	AutofireReadSetting(player * 1,    autofireRec[player - 1].buttonMask, parent);
	AutofireReadSetting(player * 11,   autofireRec[player - 1].toggleMask, parent);
	AutofireReadSetting(player * 111,  autofireRec[player - 1].tcMask,     parent);
	AutofireReadSetting(player * 1111, autofireRec[player - 1].invertMask, parent);
	
	cid.id = player;
	
	cid.signature = 'Num_';
	sprintf(num, "%ld", autofireRec[player - 1].frequency);
	ConvertCString(num, numP);
	HIViewFindByID(parent, cid, &ctl);
	SetStaticTextText(ctl, numP, false);
	
	cid.signature = 'Slid';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, autofireRec[player - 1].frequency);
}

static void AutofireReadSetting(int sig, uint16 target, HIViewRef parent)
{
	HIViewRef	ctl;
	HIViewID	cid;
	
	cid.id = sig;
	
	cid.signature = 'AChk';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x0080) ? 1 : 0);
	
	cid.signature = 'BChk';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x8000) ? 1 : 0);
	
	cid.signature = 'XChk';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x0040) ? 1 : 0);
	
	cid.signature = 'YChk';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x4000) ? 1 : 0);
	
	cid.signature = 'LChk';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x0020) ? 1 : 0);
	
	cid.signature = 'RChk';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x0010) ? 1 : 0);
	
	cid.signature = 'Up  ';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x0800) ? 1 : 0);
	
	cid.signature = 'Down';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x0400) ? 1 : 0);
	
	cid.signature = 'Left';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x0200) ? 1 : 0);
	
	cid.signature = 'Righ';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x0100) ? 1 : 0);
	
	cid.signature = 'Star';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x1000) ? 1 : 0);
	
	cid.signature = 'Sele';
	HIViewFindByID(parent, cid, &ctl);
	SetControl32BitValue(ctl, (target & 0x2000) ? 1 : 0);
}

static void AutofireWriteAllSettings(int player, HIViewRef parent)
{
	HIViewRef	ctl;
	HIViewID	cid;
	
	AutofireWriteSetting(player * 1,    &(autofireRec[player - 1].buttonMask), parent);
	AutofireWriteSetting(player * 11,   &(autofireRec[player - 1].toggleMask), parent);
	AutofireWriteSetting(player * 111,  &(autofireRec[player - 1].tcMask),     parent);
	AutofireWriteSetting(player * 1111, &(autofireRec[player - 1].invertMask), parent);
	
	cid.id = player;
	cid.signature = 'Slid';
	HIViewFindByID(parent, cid, &ctl);
	autofireRec[player - 1].frequency = GetControl32BitValue(ctl);
}

static void AutofireWriteSetting(int sig, uint16 *target, HIViewRef parent)
{
	HIViewRef	ctl;
	HIViewID	cid;
	
	cid.id = sig;
	*target = 0x0000;
	
	cid.signature = 'AChk';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x0080;
	
	cid.signature = 'BChk';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x8000;
	
	cid.signature = 'XChk';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x0040;
	
	cid.signature = 'YChk';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x4000;
	
	cid.signature = 'LChk';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x0020;
	
	cid.signature = 'RChk';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x0010;
	
	cid.signature = 'Up  ';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x0800;
	
	cid.signature = 'Down';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x0400;
	
	cid.signature = 'Left';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x0200;
	
	cid.signature = 'Righ';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x0100;
	
	cid.signature = 'Star';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x1000;
	
	cid.signature = 'Sele';
	HIViewFindByID(parent, cid, &ctl);
	if (GetControl32BitValue(ctl))
		(*target) |= 0x2000;
}

static void AutofireSelectTabPane(HIViewRef tabControl, SInt16 index)
{
	HIViewRef	sup, userPane, selectedPane = nil;
	HIViewID	cid;
	
	autofireLastTabIndex = index;

	sup = HIViewGetSuperview(tabControl);
	cid.signature = 'Ftab';
	
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

static pascal OSStatus AutofireTabEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerRef, inUserData)
	
	OSStatus	result = eventNotHandledErr;
	HIViewRef	ctl;
	HIViewID	cid;
	SInt32		value;
	
	GetEventParameter(inEvent, kEventParamDirectObject, typeControlRef, nil, sizeof(ControlRef), nil, &ctl);
	GetControlID(ctl, &cid);
	value = GetControl32BitValue(ctl);
	
	if ((cid.id == 256) && (value != autofireLastTabIndex))
	{
		AutofireSelectTabPane(ctl, value);
		result = noErr;
	}    
	
	return result;
}

pascal OSStatus AutofireWindowEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
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
					HIViewRef	root;
					int			player = -1;

					root = HIViewGetRoot(tWindowRef);

					err = GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, nil, sizeof(HICommand), nil, &tHICommand);
					if (err == noErr)
					{
						switch (tHICommand.commandID)
						{
							case 'DEF1':
								player = 0;
								break;
							
							case 'DEF2':
								player = 1;
								break;
						}
						
						if (player != -1)
						{
							autofireRec[player].buttonMask = 0x0000;
							autofireRec[player].toggleMask = 0xFFF0;
							autofireRec[player].tcMask     = 0x0000;
							autofireRec[player].invertMask = 0x0000;
							autofireRec[player].frequency  = 10;
							AutofireReadAllSettings(player + 1, root);
							
							result = noErr;
						}
					}
			}
	}
	
	return result;
}

static pascal void AutofireSliderActionProc(HIViewRef slider, HIViewPartCode partCode)
{
	#pragma unused (partCode)

	HIViewRef		ctl;
	HIViewID		cid;
	char			num[10];
	unsigned char	numP[10];
	
	GetControlID(slider, &cid);
	cid.signature = 'Num_';
	HIViewFindByID(HIViewGetSuperview(slider), cid, &ctl);
	
	sprintf(num, "%ld", GetControl32BitValue(slider));
	ConvertCString(num, numP);
	SetStaticTextText(ctl, numP, true);
}

void RomInfoDialog(void)
{
	OSStatus	err;
	IBNibRef	nibRef;
	
	if (!cartOpen)
		return;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		WindowRef	tWindowRef;
		
		err = CreateWindowFromNib(nibRef, CFSTR("RomInfo"), &tWindowRef);
		if (err == noErr)
		{
			EventHandlerRef		eref;
			EventHandlerUPP		eventUPP;
			EventTypeSpec		windowEvents[] = { { kEventClassWindow, kEventWindowClose }, { kEventClassCommand, kEventCommandProcess } };
			CFStringRef			sref;
			HIViewRef			ctl, root;
			HIViewID			cid;
			char				text[256];
			unsigned char		textP[256];
			
			eventUPP = NewEventHandlerUPP(RomInfoEventHandler);
			err = InstallWindowEventHandler(tWindowRef, eventUPP, GetEventTypeCount(windowEvents), windowEvents, (void *) tWindowRef, &eref);
			
			root = HIViewGetRoot(tWindowRef);
			cid.id = 0;
			
			cid.signature = 'Name';	// Cart Name
			HIViewFindByID(root, cid, &ctl);
			
			sprintf(text, "%s", Memory.RawROMName);

			sref = CopyFixNameStrings(text, Memory.ROMRegion);
			if (!sref)	
				SetStaticTextCFString(ctl, CFSTR("unknown"), false);
			else
			{
				SetStaticTextCFString(ctl, sref, false);
				CFRelease(sref);
			}

			cid.signature = 'Spee';	// ROM Speed
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "0x%02X/%s", Memory.ROMSpeed, ((Memory.ROMSpeed & 0x10) != 0) ? "FastROM" : "SlowROM");
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			cid.signature = 'Map ';	// ROM Map
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "%s", Memory.HiROM ? "HiROM" : "LoROM");
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			cid.signature = 'Type';	// ROM Type
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "0x%02X", Memory.ROMType);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
						
			cid.signature = 'Cont';	// Contents
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "%s", Memory.KartContents());
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);

			cid.signature = 'SizH';	// ROM Size written in info block
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "%s", Memory.Size());
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			cid.signature = 'SizC';	// Actual ROM Size
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "%dMbits", Memory.CalculatedSize / 0x20000);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			cid.signature = 'SRAM';	// SRAM Size
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "%s%s", Memory.StaticRAMSize(), StaticRAMBitSize());
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			cid.signature = 'SumC';	// Actual checksum
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "0x%04X", Memory.CalculatedChecksum);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			cid.signature = 'SumH';	// Checksum written in info block
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "0x%04X", Memory.ROMChecksum);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			cid.signature = 'ComH';	// Checksum compliment written in info block : SumH + ComH = 0xFFFF
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "0x%04X", Memory.ROMComplementChecksum);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			cid.signature = 'CRC ';	// CRC32
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "0x%08X", Memory.ROMCRC32);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);

			cid.signature = 'Outp';	// Output (NTSC or PAL)
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "%s", (Memory.ROMRegion > 12 || Memory.ROMRegion < 2) ? "NTSC 60Hz" : "PAL 50Hz");
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);

			cid.signature = 'Vers';	// Version
			HIViewFindByID(root, cid, &ctl);
			sprintf(text, "1.%d", Memory.HiROM ? Memory.ROM[0x0FFDB] : Memory.ROM[0x7FDB]);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);

			cid.signature = 'Lice';	// Licensee
			HIViewFindByID(root, cid, &ctl);
			RomInfoGetLicenseeText(text);		
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);

			cid.signature = 'Regi';	// Region
			HIViewFindByID(root, cid, &ctl);
			RomInfoGetRegionText(text);
			ConvertCString(text, textP);
			SetStaticTextText(ctl, textP, false);
			
			MoveWindowPosition(tWindowRef, kWindowRomInfo, false);
			ShowWindow(tWindowRef);
			err = RunAppModalLoopForWindow(tWindowRef);
			HideWindow(tWindowRef);
			SaveWindowPosition(tWindowRef, kWindowRomInfo);

			err = RemoveEventHandler(eref);
			DisposeEventHandlerUPP(eventUPP);
			
			ReleaseWindow(tWindowRef);
		}
		
		DisposeNibReference(nibRef);
	}
}

static void RomInfoCopyToClipboard(void)
{
	OSStatus	err;
	char		text[2048];

	RomInfoBuildInfoText(text);
	
	err = ClearCurrentScrap();
	if (err == noErr)
	{
		ScrapRef	scrap;
		
		err = GetCurrentScrap(&scrap);
		if (err == noErr)
			err = PutScrapFlavor(scrap, kScrapFlavorTypeText, kScrapFlavorMaskNone, strlen(text), text);
	}
}

static void RomInfoBuildInfoText(char *romtext)
{
	char	temp[256];

	sprintf(romtext, "Snes9x version: %s\r", VERSION);
	sprintf(temp,    "Mac port version: %s, OS X\r\r", MAC_VERSION);
	strcat (romtext, temp);

	sprintf(temp,    "Name: %s\r", Memory.ROMName);
	strcat (romtext, temp);
	sprintf(temp,    "Speed: %02X/%s\r", Memory.ROMSpeed, ((Memory.ROMSpeed & 0x10) !=0) ? "FastROM" : "SlowROM");
	strcat (romtext, temp);
	sprintf(temp,    "ROM Map: %s\r", (Memory.HiROM) ? "HiROM" : "LoROM");
	strcat (romtext, temp);
	sprintf(temp,    "Type: %02X\r", Memory.ROMType);
	strcat (romtext, temp);
	strcat (romtext, "Kart contents: ");
	strcat (romtext, Memory.KartContents());
	strcat (romtext, "\rHeader ROM Size: ");
	strcat (romtext, Memory.Size());
	sprintf(temp,    "\rCalculated ROM Size: %dMbits", Memory.CalculatedSize / 0x20000);
	strcat (romtext, temp);
			
	strcat (romtext, "\rSRAM size: ");
	strcat (romtext, Memory.StaticRAMSize());
	strcat (romtext, StaticRAMBitSize());
	strcat (romtext, "\rActual Checksum: ");
	sprintf(temp,    "%04X", Memory.CalculatedChecksum);
	strcat (romtext, temp);
	strcat (romtext, "\rHeader Checksum: ");
	sprintf(temp,    "%04X", Memory.ROMChecksum);
	strcat (romtext, temp);
	strcat (romtext, "\rHeader Checksum Complement: ");
	sprintf(temp,    "%04X", Memory.ROMComplementChecksum);
	strcat (romtext, temp);

	strcat (romtext, "\rOutput: ");
	if (Memory.ROMRegion > 12 || Memory.ROMRegion < 2)
		strcat(romtext, "NTSC 60Hz");
	else
		strcat(romtext, "PAL 50Hz");
			
	sprintf(temp,    "\rCRC32: %08X", Memory.ROMCRC32);
	strcat (romtext, temp);
	
	strcat (romtext, "\rLicensee: ");
	RomInfoGetLicenseeText(temp);
	strcat (romtext, temp);
				
	strcat (romtext, "\rROM Version: ");
	sprintf(temp,    "1.%d", (Memory.HiROM) ? Memory.ROM[0x0FFDB] : Memory.ROM[0x7FDB]);
	strcat (romtext, temp);

	strcat (romtext, "\rRegion: ");
	RomInfoGetRegionText(temp);
	strcat (romtext, temp);
}

#define NOTKNOWN	"Unknown Company "

static void RomInfoGetLicenseeText(char *text)
{
	int	tmp = -1;

	if (Memory.CompanyId[0] == '0')	tmp = 0;
	if (Memory.CompanyId[0] == '1')	tmp = 16;
	if (Memory.CompanyId[0] == '2')	tmp = 32;
	if (Memory.CompanyId[0] == '3')	tmp = 48;
	if (Memory.CompanyId[0] == '4')	tmp = 64;
	if (Memory.CompanyId[0] == '5')	tmp = 80;
	if (Memory.CompanyId[0] == '6')	tmp = 96;
	if (Memory.CompanyId[0] == '7')	tmp = 112;
	if (Memory.CompanyId[0] == '8')	tmp = 128;
	if (Memory.CompanyId[0] == '9')	tmp = 144;
	if (Memory.CompanyId[0] == 'A')	tmp = 160;
	if (Memory.CompanyId[0] == 'B')	tmp = 176;
	if (Memory.CompanyId[0] == 'C')	tmp = 192;
	if (Memory.CompanyId[0] == 'D')	tmp = 208;
	if (Memory.CompanyId[0] == 'E')	tmp = 224;
	if (Memory.CompanyId[0] == 'F')	tmp = 240;
	
	if (tmp != -1)
	{
			 if (Memory.CompanyId[1] == '0')	tmp += 0;
		else if (Memory.CompanyId[1] == '1')	tmp += 1;
		else if (Memory.CompanyId[1] == '2')	tmp += 2;
		else if (Memory.CompanyId[1] == '3')	tmp += 3;
		else if (Memory.CompanyId[1] == '4')	tmp += 4;
		else if (Memory.CompanyId[1] == '5')	tmp += 5;
		else if (Memory.CompanyId[1] == '6')	tmp += 6;
		else if (Memory.CompanyId[1] == '7')	tmp += 7;
		else if (Memory.CompanyId[1] == '8')	tmp += 8;
		else if (Memory.CompanyId[1] == '9')	tmp += 9;
		else if (Memory.CompanyId[1] == 'A')	tmp += 10;
		else if (Memory.CompanyId[1] == 'B')	tmp += 11;
		else if (Memory.CompanyId[1] == 'C')	tmp += 12;
		else if (Memory.CompanyId[1] == 'D')	tmp += 13;
		else if (Memory.CompanyId[1] == 'E')	tmp += 14;
		else if (Memory.CompanyId[1] == 'F')	tmp += 15;
		else 									tmp  = 0;
	}
	else
		tmp = 0;
		
	if (tmp == 0)
		tmp = ((Memory.ROMSpeed & 0x0F) != 0) ? Memory.ROM[0x0FFDA] : Memory.ROM[0x7FDA];

	switch (tmp)
	{
		case 0:		strcpy(text, "INVALID COMPANY");					break;
		case 1:		strcpy(text, "Nintendo");							break;
		case 2:		strcpy(text, "Ajinomoto");							break;
		case 3:		strcpy(text, "Imagineer-Zoom");						break;
		case 4:		strcpy(text, "Chris Gray Enterprises Inc.");		break;
		case 5:		strcpy(text, "Zamuse");								break;
		case 6:		strcpy(text, "Falcom");								break;
		case 7:		strcpy(text, NOTKNOWN "7");							break;
		case 8:		strcpy(text, "Capcom");								break;
		case 9:		strcpy(text, "HOT-B");								break;
		case 10:	strcpy(text, "Jaleco");								break;
		case 11:	strcpy(text, "Coconuts");							break;
		case 12:	strcpy(text, "Rage Software");						break;
		case 13:	strcpy(text, "Micronet");							break;
		case 14:	strcpy(text, "Technos");							break;
		case 15:	strcpy(text, "Mebio Software");						break;
		case 16:	strcpy(text, "SHOUEi System");						break;
		case 17:	strcpy(text, "Starfish");							break;
		case 18:	strcpy(text, "Gremlin Graphics");					break;
		case 19:	strcpy(text, "Electronic Arts");					break;
		case 20:	strcpy(text, "NCS / Masaya");						break;
		case 21:	strcpy(text, "COBRA Team");							break;
		case 22:	strcpy(text, "Human/Field");						break;
		case 23:	strcpy(text, "KOEI");								break;
		case 24:	strcpy(text, "Hudson Soft");						break;
		case 25:	strcpy(text, "Game Village");						break;
		case 26:	strcpy(text, "Yanoman");							break;
		case 27:	strcpy(text, NOTKNOWN "27");						break;
		case 28:	strcpy(text, "Tecmo");								break;
		case 29:	strcpy(text, NOTKNOWN "29");						break;
		case 30:	strcpy(text, "Open System");						break;
		case 31:	strcpy(text, "Virgin Games");						break;
		case 32:	strcpy(text, "KSS");								break;
		case 33:	strcpy(text, "Sunsoft");							break;
		case 34:	strcpy(text, "POW");								break;
		case 35:	strcpy(text, "Micro World");						break;
		case 36:	strcpy(text, NOTKNOWN "36");						break;
		case 37:	strcpy(text, NOTKNOWN "37");						break;
		case 38:	strcpy(text, "Enix");								break;
		case 39:	strcpy(text, "Loriciel/Electro Brain");				break;
		case 40:	strcpy(text, "Kemco");								break;
		case 41:	strcpy(text, "Seta Co.,Ltd.");						break;
		case 42:	strcpy(text, "Culture Brain");						break;
		case 43:	strcpy(text, "Irem Japan");							break;
		case 44:	strcpy(text, "Pal Soft");							break;
		case 45:	strcpy(text, "Visit Co.,Ltd.");						break;
		case 46:	strcpy(text, "INTEC Inc.");							break;
		case 47:	strcpy(text, "System Sacom Corp.");					break;
		case 48:	strcpy(text, "Viacom New Media");					break;
		case 49:	strcpy(text, "Carrozzeria");						break;
		case 50:	strcpy(text, "Dynamic");							break;
		case 51:	strcpy(text, "Nintendo");							break;
		case 52:	strcpy(text, "Magifact");							break;
		case 53:	strcpy(text, "Hect");								break;
		case 54:	strcpy(text, NOTKNOWN "54");						break;
		case 55:	strcpy(text, NOTKNOWN "55");						break;
		case 56:	strcpy(text, "Capcom Europe");						break;
		case 57:	strcpy(text, "Accolade Europe");					break;
		case 58:	strcpy(text, NOTKNOWN "58");						break;
		case 59:	strcpy(text, "Arcade Zone");						break;
		case 60:	strcpy(text, "Empire Software");					break;
		case 61:	strcpy(text, "Loriciel");							break;
		case 62:	strcpy(text, "Gremlin Graphics");					break;
		case 63:	strcpy(text, NOTKNOWN "63");						break;
		case 64:	strcpy(text, "Seika Corp.");						break;
		case 65:	strcpy(text, "UBI Soft");							break;
		case 66:	strcpy(text, NOTKNOWN "66");						break;
		case 67:	strcpy(text, NOTKNOWN "67");						break;
		case 68:	strcpy(text, "LifeFitness Exertainment");			break;
		case 69:	strcpy(text, NOTKNOWN "69");						break;
		case 70:	strcpy(text, "System 3");							break;
		case 71:	strcpy(text, "Spectrum Holobyte");					break;
		case 72:	strcpy(text, NOTKNOWN "72");						break;
		case 73:	strcpy(text, "Irem");								break;
		case 74:	strcpy(text, NOTKNOWN "74");						break;
		case 75:	strcpy(text, "Raya Systems/Sculptured Software");	break;
		case 76:	strcpy(text, "Renovation Products");				break;
		case 77:	strcpy(text, "Malibu Games/Black Pearl");			break;
		case 78:	strcpy(text, NOTKNOWN "78");						break;
		case 79:	strcpy(text, "U.S. Gold");							break;
		case 80:	strcpy(text, "Absolute Entertainment");				break;
		case 81:	strcpy(text, "Acclaim");							break;
		case 82:	strcpy(text, "Activision");							break;
		case 83:	strcpy(text, "American Sammy");						break;
		case 84:	strcpy(text, "GameTek");							break;
		case 85:	strcpy(text, "Hi Tech Expressions");				break;
		case 86:	strcpy(text, "LJN Toys");							break;
		case 87:	strcpy(text, NOTKNOWN "87");						break;
		case 88:	strcpy(text, NOTKNOWN "88");						break;
		case 89:	strcpy(text, NOTKNOWN "89");						break;
		case 90:	strcpy(text, "Mindscape");							break;
		case 91:	strcpy(text, "Romstar, Inc.");						break;
		case 92:	strcpy(text, NOTKNOWN "92");						break;
		case 93:	strcpy(text, "Tradewest");							break;
		case 94:	strcpy(text, NOTKNOWN "94");						break;
		case 95:	strcpy(text, "American Softworks Corp.");			break;
		case 96:	strcpy(text, "Titus");								break;
		case 97:	strcpy(text, "Virgin Interactive Entertainment");	break;
		case 98:	strcpy(text, "Maxis");								break;
		case 99:	strcpy(text, "Origin/FCI/Pony Canyon");				break;
		case 100:	strcpy(text, NOTKNOWN "100");						break;
		case 101:	strcpy(text, NOTKNOWN "101");						break;
		case 102:	strcpy(text, NOTKNOWN "102");						break;
		case 103:	strcpy(text, "Ocean");								break;
		case 104:	strcpy(text, NOTKNOWN "104");						break;
		case 105:	strcpy(text, "Electronic Arts");					break;
		case 106:	strcpy(text, NOTKNOWN "106");						break;
		case 107:	strcpy(text, "Laser Beam");							break;
		case 108:	strcpy(text, NOTKNOWN "108");						break;
		case 109:	strcpy(text, NOTKNOWN "109");						break;
		case 110:	strcpy(text, "Elite");								break;
		case 111:	strcpy(text, "Electro Brain");						break;
		case 112:	strcpy(text, "Infogrames");							break;
		case 113:	strcpy(text, "Interplay");							break;
		case 114:	strcpy(text, "LucasArts");							break;
		case 115:	strcpy(text, "Parker Brothers");					break;
		case 116:	strcpy(text, "Konami");								break;
		case 117:	strcpy(text, "STORM");								break;
		case 118:	strcpy(text, NOTKNOWN "118");						break;
		case 119:	strcpy(text, NOTKNOWN "119");						break;
		case 120:	strcpy(text, "THQ Software");						break;
		case 121:	strcpy(text, "Accolade Inc.");						break;
		case 122:	strcpy(text, "Triffix Entertainment");				break;
		case 123:	strcpy(text, NOTKNOWN "123");						break;
		case 124:	strcpy(text, "Microprose");							break;
		case 125:	strcpy(text, NOTKNOWN "125");						break;
		case 126:	strcpy(text, NOTKNOWN "126");						break;
		case 127:	strcpy(text, "Kemco");								break;
		case 128:	strcpy(text, "Misawa");								break;
		case 129:	strcpy(text, "Teichio");							break;
		case 130:	strcpy(text, "Namco Ltd.");							break;
		case 131:	strcpy(text, "Lozc");								break;
		case 132:	strcpy(text, "Koei");								break;
		case 133:	strcpy(text, NOTKNOWN "133");						break;
		case 134:	strcpy(text, "Tokuma Shoten Intermedia");			break;
		case 135:	strcpy(text, "Tsukuda Original");					break;
		case 136:	strcpy(text, "DATAM-Polystar");						break;
		case 137:	strcpy(text, NOTKNOWN "137");						break;
		case 138:	strcpy(text, NOTKNOWN "138");						break;
		case 139:	strcpy(text, "Bullet-Proof Software");				break;
		case 140:	strcpy(text, "Vic Tokai");							break;
		case 141:	strcpy(text, NOTKNOWN "141");						break;
		case 142:	strcpy(text, "Character Soft");						break;
		case 143:	strcpy(text, "I'Max");								break;
		case 144:	strcpy(text, "Takara");								break;
		case 145:	strcpy(text, "CHUN Soft");							break;
		case 146:	strcpy(text, "Video System Co., Ltd.");				break;
		case 147:	strcpy(text, "BEC");								break;
		case 148:	strcpy(text, NOTKNOWN "148");						break;
		case 149:	strcpy(text, "Varie");								break;
		case 150:	strcpy(text, "Yonezawa / S'Pal Corp.");				break;
		case 151:	strcpy(text, "Kaneco");								break;
		case 152:	strcpy(text, NOTKNOWN "152");						break;
		case 153:	strcpy(text, "Pack in Video");						break;
		case 154:	strcpy(text, "Nichibutsu");							break;
		case 155:	strcpy(text, "TECMO");								break;
		case 156:	strcpy(text, "Imagineer Co.");						break;
		case 157:	strcpy(text, NOTKNOWN "157");						break;
		case 158:	strcpy(text, NOTKNOWN "158");						break;
		case 159:	strcpy(text, NOTKNOWN "159");						break;
		case 160:	strcpy(text, "Telenet");							break;
		case 161:	strcpy(text, "Hori");								break;
		case 162:	strcpy(text, NOTKNOWN "162");						break;
		case 163:	strcpy(text, NOTKNOWN "163");						break;
		case 164:	strcpy(text, "Konami");								break;
		case 165:	strcpy(text, "K.Amusement Leasing Co.");			break;
		case 166:	strcpy(text, NOTKNOWN "166");						break;
		case 167:	strcpy(text, "Takara");								break;
		case 168:	strcpy(text, NOTKNOWN "168");						break;
		case 169:	strcpy(text, "Technos Jap.");						break;
		case 170:	strcpy(text, "JVC");								break;
		case 171:	strcpy(text, NOTKNOWN "171");						break;
		case 172:	strcpy(text, "Toei Animation");						break;
		case 173:	strcpy(text, "Toho");								break;
		case 174:	strcpy(text, NOTKNOWN "174");						break;
		case 175:	strcpy(text, "Namco Ltd.");							break;
		case 176:	strcpy(text, "Media Rings Corp.");					break;
		case 177:	strcpy(text, "ASCII Co. Activison");				break;
		case 178:	strcpy(text, "Bandai");								break;
		case 179:	strcpy(text, NOTKNOWN "179");						break;
		case 180:	strcpy(text, "Enix America");						break;
		case 181:	strcpy(text, NOTKNOWN "181");						break;
		case 182:	strcpy(text, "Halken");								break;
		case 183:	strcpy(text, NOTKNOWN "183");						break;
		case 184:	strcpy(text, NOTKNOWN "184");						break;
		case 185:	strcpy(text, NOTKNOWN "185");						break;
		case 186:	strcpy(text, "Culture Brain");						break;
		case 187:	strcpy(text, "Sunsoft");							break;
		case 188:	strcpy(text, "Toshiba EMI");						break;
		case 189:	strcpy(text, "Sony Imagesoft");						break;
		case 190:	strcpy(text, NOTKNOWN "190");						break;
		case 191:	strcpy(text, "Sammy");								break;
		case 192:	strcpy(text, "Taito");								break;
		case 193:	strcpy(text, NOTKNOWN "193");						break;
		case 194:	strcpy(text, "Kemco");								break;
		case 195:	strcpy(text, "Square");								break;
		case 196:	strcpy(text, "Tokuma Soft");						break;
		case 197:	strcpy(text, "Data East");							break;
		case 198:	strcpy(text, "Tonkin House");						break;
		case 199:	strcpy(text, NOTKNOWN "199");						break;
		case 200:	strcpy(text, "KOEI");								break;
		case 201:	strcpy(text, NOTKNOWN "201");						break;
		case 202:	strcpy(text, "Konami USA");							break;
		case 203:	strcpy(text, "NTVIC");								break;
		case 204:	strcpy(text, NOTKNOWN "204");						break;
		case 205:	strcpy(text, "Meldac");								break;
		case 206:	strcpy(text, "Pony Canyon");						break;
		case 207:	strcpy(text, "Sotsu Agency/Sunrise");				break;
		case 208:	strcpy(text, "Disco/Taito");						break;
		case 209:	strcpy(text, "Sofel");								break;
		case 210:	strcpy(text, "Quest Corp.");						break;
		case 211:	strcpy(text, "Sigma");								break;
		case 212:	strcpy(text, "Ask Kodansha Co., Ltd.");				break;
		case 213:	strcpy(text, NOTKNOWN "213");						break;
		case 214:	strcpy(text, "Naxat");								break;
		case 215:	strcpy(text, NOTKNOWN "215");						break;
		case 216:	strcpy(text, "Capcom Co., Ltd.");					break;
		case 217:	strcpy(text, "Banpresto");							break;
		case 218:	strcpy(text, "Tomy");								break;
		case 219:	strcpy(text, "Acclaim");							break;
		case 220:	strcpy(text, NOTKNOWN "220");						break;
		case 221:	strcpy(text, "NCS");								break;
		case 222:	strcpy(text, "Human Entertainment");				break;
		case 223:	strcpy(text, "Altron");								break;
		case 224:	strcpy(text, "Jaleco");								break;
		case 225:	strcpy(text, NOTKNOWN "225");						break;
		case 226:	strcpy(text, "Yutaka");								break;
		case 227:	strcpy(text, NOTKNOWN "227");						break;
		case 228:	strcpy(text, "T&ESoft");							break;
		case 229:	strcpy(text, "EPOCH Co.,Ltd.");						break;
		case 230:	strcpy(text, NOTKNOWN "230");						break;
		case 231:	strcpy(text, "Athena");								break;
		case 232:	strcpy(text, "Asmik");								break;
		case 233:	strcpy(text, "Natsume");							break;
		case 234:	strcpy(text, "King Records");						break;
		case 235:	strcpy(text, "Atlus");								break;
		case 236:	strcpy(text, "Sony Music Entertainment");			break;
		case 237:	strcpy(text, NOTKNOWN "237");						break;
		case 238:	strcpy(text, "IGS");								break;
		case 239:	strcpy(text, NOTKNOWN "239");						break;
		case 240:	strcpy(text, NOTKNOWN "240");						break;
		case 241:	strcpy(text, "Motown Software");					break;
		case 242:	strcpy(text, "Left Field Entertainment");			break;
		case 243:	strcpy(text, "Beam Software");						break;
		case 244:	strcpy(text, "Tec Magik");							break;
		case 245:	strcpy(text, NOTKNOWN "245");						break;
		case 246:	strcpy(text, NOTKNOWN "246");						break;
		case 247:	strcpy(text, NOTKNOWN "247");						break;
		case 248:	strcpy(text, NOTKNOWN "248");						break;
		case 249:	strcpy(text, "Cybersoft");							break;
		case 250:	strcpy(text, NOTKNOWN "250");						break;
		case 251:	strcpy(text, "Psygnosis");							break;
		case 252:	strcpy(text, NOTKNOWN "252");						break;
		case 253:	strcpy(text, NOTKNOWN "253");						break;
		case 254:	strcpy(text, "Davidson");							break;
		case 255:	strcpy(text, NOTKNOWN "255");						break;
		default:	strcpy(text, NOTKNOWN);
	}
}

static void RomInfoGetRegionText(char *text)
{
	switch (Memory.ROMRegion)
	{
		case 0:		strcpy(text, "Japan");								break;
		case 1:		strcpy(text, "USA/Canada");							break;
		case 2:		strcpy(text, "Oceania, Europe, and Asia");			break;
		case 3:		strcpy(text, "Sweden");								break;
		case 4:		strcpy(text, "Finland");							break;
		case 5:		strcpy(text, "Denmark");							break;
		case 6:		strcpy(text, "France");								break;
		case 7:		strcpy(text, "Holland");							break;
		case 8:		strcpy(text, "Spain");								break;
		case 9:		strcpy(text, "Germany, Austria, and Switzerland");	break;
		case 10:	strcpy(text, "Italy");								break;
		case 11:	strcpy(text, "Hong Kong and China");				break;
		case 12:	strcpy(text, "Indonesia");							break;
		case 13:	strcpy(text, "South Korea");						break;
		case 14:	strcpy(text, "Unknown region 14");					break;
		default:	strcpy(text, "Unknown region 15");
	}
}

static pascal OSStatus RomInfoEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
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
						switch (tHICommand.commandID)
						{
							case 'Clip':
								RomInfoCopyToClipboard();
								result = noErr;
						}
					}
			}
	}
	
	return result;
}

static char *StaticRAMBitSize(void) 
{ 
	static char	tmp[20]; 

	sprintf(tmp, " (%dKbits)", 8 * (Memory.SRAMMask + 1) / 1024);
	
	return tmp;
}

long GetHIToolboxVersion(void)
{
	CFBundleRef	bundle;
	CFStringRef	versStr = nil;

	static unsigned long	version = 0;
  
	if (version != 0)
		return version;

	bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.HIToolbox"));
	if (bundle)
		versStr = (CFStringRef) CFBundleGetValueForInfoDictionaryKey(bundle, CFSTR("CFBundleShortVersionString"));

	if (versStr && (CFGetTypeID(versStr) == CFStringGetTypeID()))
	{
		int		major = 0, minor = 0, bugfix = 0;
		char	sz[20];

		CFStringGetCString(versStr, sz, sizeof(sz), kCFStringEncodingUTF8);
		sscanf(sz, "%d.%d.%d", &major, &minor, &bugfix);
		version = (major << 8) + (minor << 4) + bugfix;
	}

	return version;
}

void RegisterHelpBook(void)
{
	OSStatus	err;
	CFBundleRef	bundleRef;
	CFURLRef	bundleURL;
	FSRef		fref;

	bundleRef = CFBundleGetMainBundle();
	if (bundleRef)
	{
		bundleURL = CFBundleCopyBundleURL(bundleRef);
		if (bundleURL)
		{
			if (CFURLGetFSRef(bundleURL, &fref))
				err = AHRegisterHelpBook(&fref);

			CFRelease(bundleURL);
		}
	}
}

void SetHIViewID(HIViewID *cid, OSType signature, SInt32 value)
{
	// Since HIViewID.id conflicts Objective-C 'id'...

	cid->signature = signature;
	cid->id = value;
}
