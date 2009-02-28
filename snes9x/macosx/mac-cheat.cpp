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
#include "cheats.h"

#include "mac-prefix.h"
#include "mac-dialog.h"
#include "mac-os.h"
#include "mac-stringtools.h"
#include "mac-cheat.h"

#define	kDataBrowser	'BRSR'
#define	kButtonsPane	'BTNS'
#define	kCmCheckBox		'CHK_'
#define	kCmAddress		'ADDR'
#define	kCmValue		'VALU'
#define	kCmDescription	'DESC'
#define	kNewButton		'NEW_'
#define	kDelButton		'DEL_'
#define	kAllButton		'ALL_'

extern SCheatData	Cheat;

static void InitCheatItems(void);
static void ImportCheatItems(void);
static void DetachCheatItems(void);
static void AddCheatItem(void);
static void DeleteCheatItem(void);
static void EnableAllCheatItems(void);
static pascal void DBItemNotificationCallBack(ControlRef, DataBrowserItemID, DataBrowserItemNotification);
static pascal Boolean DBCompareCallBack(ControlRef, DataBrowserItemID, DataBrowserItemID, DataBrowserPropertyID);
static pascal OSStatus DBClientDataCallback(ControlRef, DataBrowserItemID, DataBrowserPropertyID, DataBrowserItemDataRef, Boolean);
static pascal OSStatus CheatEventHandler(EventHandlerCallRef, EventRef, void *);

typedef struct
{
	UInt32	id;
	Boolean	valid;
	Boolean	enabled;
	UInt32	address;
	UInt8	value;
	char	description[22];
}	CheatItem;

static ControlRef	dbRef;
static WindowRef	wRef;
static CheatItem	citem[MAX_CHEATS];
static UInt32		numofcheats;

static void InitCheatItems(void)
{
	for (UInt32 i = 0; i < MAX_CHEATS; i++)
	{
		citem[i].id      = i + 1;
		citem[i].valid   = false;
		citem[i].enabled = false;
		citem[i].address = 0;
		citem[i].value   = 0;
		sprintf(citem[i].description, "Cheat %02lu", citem[i].id);
	}
}

static void ImportCheatItems(void)
{
	for (UInt32 i = 0; i < Cheat.num_cheats; i++)
	{
		citem[i].valid   = true;
		citem[i].enabled = Cheat.c[i].enabled;
		citem[i].address = Cheat.c[i].address;
		citem[i].value   = Cheat.c[i].byte;
		strcpy(citem[i].description, Cheat.c[i].name);
	}
}

static void DetachCheatItems(void)
{
	S9xDeleteCheats(); // ->  include Cheat.num_cheats = 0
	
	for (UInt32 i = 0; i < MAX_CHEATS; i++)
	{
		if (citem[i].valid)
		{
			strcpy(Cheat.c[Cheat.num_cheats].name, citem[i].description);
			S9xAddCheat(citem[i].enabled, false, citem[i].address, citem[i].value); // ->  include Cheat.num_cheats++
		}
	}
	
	S9xApplyCheats();
}

void ConfigureCheat(void)
{
	OSStatus	err;
	IBNibRef	nibRef;
	
	if (!cartOpen)
		return;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		WindowRef	uiparts;
		
		err = CreateWindowFromNib(nibRef, CFSTR("CheatEntry"), &uiparts);
		if (err == noErr)
		{
			DataBrowserCallbacks			callbacks;
			DataBrowserListViewColumnDesc	columnDesc;
			EventHandlerRef					eref;
			EventHandlerUPP					eUPP;
			EventTypeSpec					events[] = { { kEventClassCommand, kEventCommandProcess       },
														 { kEventClassWindow,  kEventWindowClose          },
														 { kEventClassWindow,  kEventWindowBoundsChanging } };
			ControlRef						ctl, userpane, contentview;
			ControlID						cid;
			CFStringRef						str;
			HISize							minSize;
			Rect							winBounds, btnBounds, rct;
			WindowAttributes				metal = 0;
			
			cid.id = 0;
			cid.signature = kButtonsPane;
			GetControlByID(uiparts, &cid, &userpane);
			GetWindowBounds(uiparts, kWindowContentRgn, &winBounds);
			
			if (systemVersion >= 0x1030)	// DataBrowser supports compositing
			{
				HIRect	frame;
				
				str = CFCopyLocalizedString(CFSTR("CreateMetalDlg"), "NO");
				if (str)
				{
					if (CFStringCompare(str, CFSTR("YES"), 0) == kCFCompareEqualTo)
						metal = kWindowMetalAttribute;
					
					CFRelease(str);
				}
				
				err = HIViewGetFrame(userpane, &frame);
				MoveControl(userpane, 0, 0);	// since uiparts window is non-compositing
				err = CreateNewWindow(kDocumentWindowClass, kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute | kWindowFullZoomAttribute | kWindowResizableAttribute | kWindowLiveResizeAttribute | kWindowStandardHandlerAttribute | kWindowCompositingAttribute | metal, &winBounds, &wRef);
				err = HIViewFindByID(HIViewGetRoot(wRef), kHIViewWindowContentID, &contentview);
				err = HIViewAddSubview(contentview, userpane);
				err = HIViewSetFrame(userpane, &frame);
			}
		#ifdef MAC_JAGUAR_SUPPORT
			else
			{				
				err = CreateNewWindow(kDocumentWindowClass, kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute | kWindowFullZoomAttribute | kWindowResizableAttribute | kWindowLiveResizeAttribute | kWindowStandardHandlerAttribute, &winBounds, &wRef);
				err = CreateRootControl(wRef, &contentview);
				err = EmbedControl(userpane, contentview);
			}
		#endif
			
			ReleaseWindow(uiparts);
			
			if (!metal)
				err = SetThemeWindowBackground(wRef, kThemeBrushDialogBackgroundActive, false);
			
			str = CFCopyLocalizedString(CFSTR("CheatEntryDlg"), "CheatEntry");
			if (str)
			{
				err = SetWindowTitleWithCFString(wRef, str);
				CFRelease(str);
			}
			
			if (systemVersion >= 0x1030)	// DataBrowser supports compositing
			{
				HILayoutInfo	layoutinfo;
				
				layoutinfo.version = kHILayoutInfoVersionZero;
				err = HIViewGetLayoutInfo(userpane, &layoutinfo);
				
				layoutinfo.binding.top.toView    = contentview;
				layoutinfo.binding.top.kind      = kHILayoutBindTop;
				layoutinfo.binding.bottom.toView = contentview;
				layoutinfo.binding.bottom.kind   = kHILayoutBindNone;
				layoutinfo.binding.left.toView   = contentview;
				layoutinfo.binding.left.kind     = kHILayoutBindNone;
				layoutinfo.binding.right.toView  = contentview;
				layoutinfo.binding.right.kind    = kHILayoutBindRight;
				err = HIViewSetLayoutInfo(userpane, &layoutinfo);
			}
			
			minSize.width  = (float) (winBounds.right  - winBounds.left);
			minSize.height = (float) (winBounds.bottom - winBounds.top );
			err = SetWindowResizeLimits(wRef, &minSize, nil);
			
			MoveWindowPosition(wRef, kWindowCheatEntry, true);
			
			GetWindowBounds(wRef, kWindowContentRgn, &winBounds);
			GetControlBounds(userpane, &btnBounds);
			
		#ifdef MAC_JAGUAR_SUPPORT
			if (systemVersion < 0x1030)
			{
				MoveControl(userpane, (winBounds.right - winBounds.left) - (btnBounds.right - btnBounds.left), btnBounds.top);
				GetControlBounds(userpane, &btnBounds);
			}
		#endif
			
			rct.top = rct.left = 20;
			rct.bottom = (winBounds.bottom - winBounds.top ) - 20;
			rct.right  = (winBounds.right  - winBounds.left) - (btnBounds.right - btnBounds.left) - 8;
			err = CreateDataBrowserControl(wRef, &rct, kDataBrowserListView, &dbRef);
			
			if (systemVersion >= 0x1030)	// DataBrowser supports compositing
			{
				HILayoutInfo	layoutinfo;
				
				layoutinfo.version = kHILayoutInfoVersionZero;
				err = HIViewGetLayoutInfo(dbRef, &layoutinfo);
				
				layoutinfo.binding.top.toView    = contentview;
				layoutinfo.binding.top.kind      = kHILayoutBindTop;
				layoutinfo.binding.bottom.toView = contentview;
				layoutinfo.binding.bottom.kind   = kHILayoutBindBottom;
				layoutinfo.binding.left.toView   = contentview;
				layoutinfo.binding.left.kind     = kHILayoutBindLeft;
				layoutinfo.binding.right.toView  = contentview;
				layoutinfo.binding.right.kind    = kHILayoutBindRight;
				err = HIViewSetLayoutInfo(dbRef, &layoutinfo);
			}
			
			columnDesc.propertyDesc.propertyID = kCmCheckBox;
			columnDesc.propertyDesc.propertyType = kDataBrowserCheckboxType;
			columnDesc.propertyDesc.propertyFlags = kDataBrowserListViewSortableColumn | kDataBrowserListViewSelectionColumn | kDataBrowserPropertyIsMutable | kDataBrowserDefaultPropertyFlags;
			columnDesc.headerBtnDesc.version = kDataBrowserListViewLatestHeaderDesc;
			columnDesc.headerBtnDesc.minimumWidth = 30;
			columnDesc.headerBtnDesc.maximumWidth = 30;
			columnDesc.headerBtnDesc.titleOffset = 0;
			columnDesc.headerBtnDesc.titleString = nil;
			columnDesc.headerBtnDesc.initialOrder = kDataBrowserOrderIncreasing;
			columnDesc.headerBtnDesc.btnFontStyle.flags = kControlUseJustMask;
			columnDesc.headerBtnDesc.btnFontStyle.just = teCenter;
			columnDesc.headerBtnDesc.btnContentInfo.contentType = 0;
			columnDesc.headerBtnDesc.btnContentInfo.u.resID = 0;
			
			err = AddDataBrowserListViewColumn(dbRef, &columnDesc, ULONG_MAX);
			
			columnDesc.propertyDesc.propertyID = kCmAddress;
			columnDesc.propertyDesc.propertyType = kDataBrowserTextType;
			columnDesc.propertyDesc.propertyFlags = kDataBrowserListViewSortableColumn | kDataBrowserListViewSelectionColumn | kDataBrowserPropertyIsMutable | kDataBrowserDefaultPropertyFlags;
			columnDesc.headerBtnDesc.version = kDataBrowserListViewLatestHeaderDesc;
			columnDesc.headerBtnDesc.minimumWidth = 84;
			columnDesc.headerBtnDesc.maximumWidth = 84;
			columnDesc.headerBtnDesc.titleOffset = 0;
			columnDesc.headerBtnDesc.titleString = CFCopyLocalizedString(CFSTR("Address"), "address");
			columnDesc.headerBtnDesc.initialOrder = kDataBrowserOrderIncreasing;
			columnDesc.headerBtnDesc.btnFontStyle.flags = kControlUseJustMask;
			columnDesc.headerBtnDesc.btnFontStyle.just = teCenter;
			columnDesc.headerBtnDesc.btnContentInfo.contentType = 0;
			columnDesc.headerBtnDesc.btnContentInfo.u.resID = 0;
			
			err = AddDataBrowserListViewColumn(dbRef, &columnDesc, ULONG_MAX);
			CFRelease(columnDesc.headerBtnDesc.titleString);
			
			columnDesc.propertyDesc.propertyID = kCmValue;
			columnDesc.propertyDesc.propertyType = kDataBrowserTextType;
			columnDesc.propertyDesc.propertyFlags = kDataBrowserListViewSortableColumn | kDataBrowserListViewSelectionColumn | kDataBrowserPropertyIsMutable | kDataBrowserDefaultPropertyFlags;
			columnDesc.headerBtnDesc.version = kDataBrowserListViewLatestHeaderDesc;
			columnDesc.headerBtnDesc.minimumWidth = 65;
			columnDesc.headerBtnDesc.maximumWidth = 65;
			columnDesc.headerBtnDesc.titleOffset = 0;
			columnDesc.headerBtnDesc.titleString = CFCopyLocalizedString(CFSTR("Value"), "value");
			columnDesc.headerBtnDesc.initialOrder = kDataBrowserOrderIncreasing;
			columnDesc.headerBtnDesc.btnFontStyle.flags = kControlUseJustMask;
			columnDesc.headerBtnDesc.btnFontStyle.just = teCenter;
			columnDesc.headerBtnDesc.btnContentInfo.contentType = 0;
			columnDesc.headerBtnDesc.btnContentInfo.u.resID = 0;
			
			err = AddDataBrowserListViewColumn(dbRef, &columnDesc, ULONG_MAX);
			CFRelease(columnDesc.headerBtnDesc.titleString);
			
			columnDesc.propertyDesc.propertyID = kCmDescription;
			columnDesc.propertyDesc.propertyType = kDataBrowserTextType;
			columnDesc.propertyDesc.propertyFlags = kDataBrowserListViewSortableColumn | kDataBrowserListViewSelectionColumn | kDataBrowserPropertyIsMutable | kDataBrowserDefaultPropertyFlags;
			columnDesc.headerBtnDesc.version = kDataBrowserListViewLatestHeaderDesc;
			columnDesc.headerBtnDesc.minimumWidth = 196;
			columnDesc.headerBtnDesc.maximumWidth = 196;
			columnDesc.headerBtnDesc.titleOffset = 0;
			columnDesc.headerBtnDesc.titleString = CFCopyLocalizedString(CFSTR("Description"), "description");
			columnDesc.headerBtnDesc.initialOrder = kDataBrowserOrderIncreasing;
			columnDesc.headerBtnDesc.btnFontStyle.flags = kControlUseJustMask;
			columnDesc.headerBtnDesc.btnFontStyle.just = teFlushLeft;
			columnDesc.headerBtnDesc.btnContentInfo.contentType = 0;
			columnDesc.headerBtnDesc.btnContentInfo.u.resID = 0;
			
			err = AddDataBrowserListViewColumn(dbRef, &columnDesc, ULONG_MAX);
			CFRelease(columnDesc.headerBtnDesc.titleString);
			
			callbacks.version = kDataBrowserLatestCallbacks;
			err = InitDataBrowserCallbacks(&callbacks);
			callbacks.u.v1.itemDataCallback = NewDataBrowserItemDataUPP(DBClientDataCallback);
			callbacks.u.v1.itemCompareCallback = NewDataBrowserItemCompareUPP(DBCompareCallBack);
			callbacks.u.v1.itemNotificationCallback = NewDataBrowserItemNotificationUPP(DBItemNotificationCallBack);
			err = SetDataBrowserCallbacks(dbRef, &callbacks);
			
			err = SetDataBrowserHasScrollBars(dbRef, false, true);
			err = SetDataBrowserSortOrder(dbRef, kDataBrowserOrderIncreasing);
			err = SetDataBrowserSortProperty(dbRef, kCmAddress);
			err = SetDataBrowserSelectionFlags(dbRef, kDataBrowserDragSelect | kDataBrowserCmdTogglesSelection);
			err = SetDataBrowserTableViewHiliteStyle(dbRef, kDataBrowserTableViewFillHilite);
			
			if (systemVersion >= 0x1040)
				err = DataBrowserChangeAttributes(dbRef, kDataBrowserAttributeListViewAlternatingRowColors, kDataBrowserAttributeNone);
			
			cid.signature = kDataBrowser;
			SetControlID(dbRef, &cid);
			
			InitCheatItems();
			ImportCheatItems();
			
			DataBrowserItemID	*id;
			
			id = new DataBrowserItemID [MAX_CHEATS];
			if (!id)
				QuitWithFatalError(0, "cheat 01");
			
			numofcheats = 0;
			
			for (UInt32 i = 0; i < MAX_CHEATS; i++)
			{
				if (citem[i].valid)
				{
					id[numofcheats] = citem[i].id;
					numofcheats++;
				}
			}
			
			if (numofcheats)
				err = AddDataBrowserItems(dbRef, kDataBrowserNoItem, numofcheats, id, kDataBrowserItemNoProperty);
			
			delete [] id;
			
			cid.signature = kNewButton;
			GetControlByID(wRef, &cid, &ctl);
			if (numofcheats == MAX_CHEATS)
				err = DeactivateControl(ctl);
			else
				err = ActivateControl(ctl);
			
			cid.signature = kAllButton;
			GetControlByID(wRef, &cid, &ctl);
			if (numofcheats == 0)
				err = DeactivateControl(ctl);
			else
				err = ActivateControl(ctl);
			
			cid.signature = kDelButton;
			GetControlByID(wRef, &cid, &ctl);
			err = DeactivateControl(ctl);
			
			eUPP = NewEventHandlerUPP(CheatEventHandler);
			err = InstallWindowEventHandler(wRef, eUPP, GetEventTypeCount(events), events, (void *) wRef, &eref);
			
			err = SetKeyboardFocus(wRef, dbRef, kControlFocusNextPart);
			
			ShowWindow(wRef);
			err = RunAppModalLoopForWindow(wRef);
			HideWindow(wRef);
			
			SaveWindowPosition(wRef, kWindowCheatEntry);
			
			err = RemoveEventHandler(eref);
			DisposeEventHandlerUPP(eUPP);
			
			DisposeDataBrowserItemNotificationUPP(callbacks.u.v1.itemNotificationCallback);
			DisposeDataBrowserItemCompareUPP(callbacks.u.v1.itemCompareCallback);
			DisposeDataBrowserItemDataUPP(callbacks.u.v1.itemDataCallback);
			
			ReleaseWindow(wRef);
			
			DetachCheatItems();
		}
		
		DisposeNibReference(nibRef);
	}
}

static void AddCheatItem(void)
{
	OSStatus			err;
	ControlRef			control;
	ControlID			cid;
	UInt32				i;
	DataBrowserItemID	id[1];

	if (numofcheats == MAX_CHEATS)
		return;
	
	for (i = 0; i < MAX_CHEATS; i++)
		if (citem[i].valid == false)
			break;
			
	if (i == MAX_CHEATS)
		return;
		
	numofcheats++;
	citem[i].valid   = true;
	citem[i].enabled = false;
	citem[i].address = 0;
	citem[i].value   = 0;
	sprintf(citem[i].description, "Cheat %02lu", citem[i].id);
	
	id[0] = citem[i].id;
	err = AddDataBrowserItems(dbRef, kDataBrowserNoItem, 1, id, kDataBrowserItemNoProperty);
	err = RevealDataBrowserItem(dbRef, id[0], kCmAddress, true);
	
	cid.id = 0;
	
	if (numofcheats == MAX_CHEATS)
	{
		cid.signature = kNewButton;		
		GetControlByID(wRef, &cid, &control);
		err = DeactivateControl(control);
	}

	if (numofcheats)
	{
		cid.signature = kAllButton;		
		GetControlByID(wRef, &cid, &control);
		err = ActivateControl(control);
	}
}

static void DeleteCheatItem(void)
{
	OSStatus	err;
	ControlRef	control;
	ControlID	cid;
	Handle		selectedItems;
	UInt32		selectionCount;
	
	selectedItems = NewHandle(0);
	if (!selectedItems)
		return;
		
	err = GetDataBrowserItems(dbRef, kDataBrowserNoItem, true, kDataBrowserItemIsSelected, selectedItems);
	selectionCount = (GetHandleSize(selectedItems) / sizeof(DataBrowserItemID));

	if (selectionCount == 0)
	{
		DisposeHandle(selectedItems);
		return;
	}
	
	err = RemoveDataBrowserItems(dbRef, kDataBrowserNoItem, selectionCount, (DataBrowserItemID *) *selectedItems, kDataBrowserItemNoProperty);
	
	for (UInt32 i = 0; i < selectionCount; i++)
	{
		citem[((DataBrowserItemID *) (*selectedItems))[i] - 1].valid   = false;
		citem[((DataBrowserItemID *) (*selectedItems))[i] - 1].enabled = false;
		numofcheats--;
	}
	
	DisposeHandle(selectedItems);
	
	cid.id = 0;
	
	if (numofcheats < MAX_CHEATS)
	{
		cid.signature = kNewButton;		
		GetControlByID(wRef, &cid, &control);
		err = ActivateControl(control);
	}
	
	if (numofcheats == 0)
	{
		cid.signature = kAllButton;		
		GetControlByID(wRef, &cid, &control);
		err = DeactivateControl(control);
	}
}

static void EnableAllCheatItems(void)
{
	OSStatus	err;
	
	for (int i = 0; i < MAX_CHEATS; i++)
		if (citem[i].valid)
			citem[i].enabled = true;
	
	err = UpdateDataBrowserItems(dbRef, kDataBrowserNoItem, kDataBrowserNoItem, nil, kDataBrowserItemNoProperty, kCmCheckBox);
}	

static pascal OSStatus DBClientDataCallback(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, DataBrowserItemDataRef itemData, Boolean changeValue)
{
	OSStatus 	err, result;
	CFStringRef	str;
	Boolean		r;
	uint32		address;
	uint8		value;
	char		code[256];
	
	result = noErr;
	
	switch (property)
	{
		case kCmCheckBox:
			ThemeButtonValue	buttonValue;

            if (changeValue)
			{
				err = GetDataBrowserItemDataButtonValue(itemData, &buttonValue);
				citem[itemID - 1].enabled = (buttonValue == kThemeButtonOn) ? true : false;
 	        }
			else
				err = SetDataBrowserItemDataButtonValue(itemData, citem[itemID - 1].enabled ? kThemeButtonOn : kThemeButtonOff);
				
			break;

		case kCmAddress:
			if (changeValue)
			{
				err = GetDataBrowserItemDataText(itemData, &str);
				r = CFStringGetCString(str, code, 256, CFStringGetSystemEncoding());
				CFRelease(str);
				if (r)
				{	
					Boolean	translated;
			
					if (S9xProActionReplayToRaw(code, address, value) == nil)
						translated = true;
					else
					if (S9xGameGenieToRaw(code, address, value) == nil)
						translated = true;
					else
					{
						translated = false;
						if (sscanf(code, "%lx", &address) != 1)
							address = 0;
					}
						
					citem[itemID - 1].address = address;
					sprintf(code, "%06lX", address);
					str = CFStringCreateWithCString(kCFAllocatorDefault, code, CFStringGetSystemEncoding());
					err = SetDataBrowserItemDataText(itemData, str);
					CFRelease(str);
					
					if (translated)
					{
						DataBrowserItemID	id[1];
						
						citem[itemID - 1].value = value;
						id[0] = itemID;
						err = UpdateDataBrowserItems(browser, kDataBrowserNoItem, 1, id, kDataBrowserItemNoProperty, kCmValue);
					}
				}
			}	
			else
			{
				sprintf(code, "%06lX", citem[itemID - 1].address);
				str = CFStringCreateWithCString(kCFAllocatorDefault, code, CFStringGetSystemEncoding());
				err = SetDataBrowserItemDataText(itemData, str);
				CFRelease(str);
			}

			break;
			
		case kCmValue:
			if (changeValue)
			{
				err = GetDataBrowserItemDataText(itemData, &str);
				r = CFStringGetCString(str, code, 256, CFStringGetSystemEncoding());
				CFRelease(str);
				if (r)
				{
					UInt32	byte;
					
					if (sscanf(code, "%lx", &byte) == 1)
						citem[itemID - 1].value = (UInt8) byte;
					else
					{
						citem[itemID - 1].value = 0;
						err = SetDataBrowserItemDataText(itemData, CFSTR("00"));
					}
				}
			}	
			else
			{
				sprintf(code, "%02X", citem[itemID - 1].value);
				str = CFStringCreateWithCString(kCFAllocatorDefault, code, CFStringGetSystemEncoding());
				err = SetDataBrowserItemDataText(itemData, str);
				CFRelease(str);
			}

			break;
			
		case kCmDescription:
			if (changeValue)
			{
				code[0] = 0;
				err = GetDataBrowserItemDataText(itemData, &str);
				strcpy(code, GetMultiByteCharacters(str, 19));
				CFRelease(str);
				
				if (code[0] == 0)
				{
					code[0] = ' ';
					code[1] = 0;
				}

				strcpy(citem[itemID - 1].description, code);
			}
			else
			{	
				str = CFStringCreateWithCString(kCFAllocatorDefault, citem[itemID - 1].description, CFStringGetSystemEncoding());
				err = SetDataBrowserItemDataText(itemData, str);
				CFRelease(str);
			}
			
			break;
		
		case kDataBrowserItemIsActiveProperty:
			err = SetDataBrowserItemDataBooleanValue(itemData, true);
			break;
		
		case kDataBrowserItemIsEditableProperty:
			err = SetDataBrowserItemDataBooleanValue(itemData, true);
			break;
		
		default:
			result = errDataBrowserPropertyNotSupported;
	}
	
	return result;
}

static pascal Boolean DBCompareCallBack(ControlRef browser, DataBrowserItemID itemOne, DataBrowserItemID itemTwo, DataBrowserPropertyID sortProperty)
{
	Boolean	result = false;
	
	switch (sortProperty)
	{
		case kCmCheckBox:
			result = (citem[itemOne - 1].enabled && !(citem[itemTwo - 1].enabled)) ? true : false;
			break;
			
		case kCmAddress:
			result = (citem[itemOne - 1].address <    citem[itemTwo - 1].address)  ? true : false;
			break;
			
		case kCmValue:
			result = (citem[itemOne - 1].value   <    citem[itemTwo - 1].value)    ? true : false;
			break;
		
		case kCmDescription:
			result = (strcmp(citem[itemOne - 1].description, citem[itemTwo - 1].description) < 0) ? true : false;
	}
	
	return result;
}

static pascal void DBItemNotificationCallBack(ControlRef browser, DataBrowserItemID itemID, DataBrowserItemNotification message)
{
	OSStatus	err;
	ControlRef	control;
	ControlID	cid = { kDelButton, 0 };
	UInt32		selectionCount;
	
	switch (message)
	{
		case kDataBrowserSelectionSetChanged:
			GetControlByID(wRef, &cid, &control);

			err = GetDataBrowserItemCount(browser, kDataBrowserNoItem, true, kDataBrowserItemIsSelected, &selectionCount);
			if (selectionCount == 0)
				err = DeactivateControl(control);
			else
				err = ActivateControl(control);
	}
}

static pascal OSStatus CheatEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerCallRef)
	
	OSStatus	err, result = eventNotHandledErr;
	WindowRef	tWindowRef;

	tWindowRef = (WindowRef) inUserData;
	
	switch (GetEventClass(inEvent))
	{
		case kEventClassWindow:
			switch (GetEventKind(inEvent))
			{
				case kEventWindowClose:
					QuitAppModalLoopForWindow(tWindowRef);
					result = noErr;
					break;
				
				case kEventWindowBoundsChanging:
				#ifdef MAC_JAGUAR_SUPPORT
					if (systemVersion < 0x1030)
					{
						ControlRef	ctl;
						ControlID	cid;
						Rect		winBounds, btnBounds;
						UInt32		attr;
						short		x, y;
						
						err = GetEventParameter(inEvent, kEventParamAttributes, typeUInt32, nil, sizeof(UInt32), nil, &attr);
						if ((err == noErr) && (attr & kWindowBoundsChangeSizeChanged))
						{
							err = GetEventParameter(inEvent, kEventParamCurrentBounds, typeQDRectangle, nil, sizeof(Rect), nil, &winBounds);
							if (err == noErr)
							{
								cid.id = 0;
								cid.signature = kButtonsPane;
								GetControlByID(tWindowRef, &cid, &ctl);
								GetControlBounds(ctl, &btnBounds);
								y = btnBounds.top;
								x = (winBounds.right - winBounds.left) - (btnBounds.right - btnBounds.left);
								MoveControl(ctl, x, y);

								cid.signature = kDataBrowser;
								GetControlByID(tWindowRef, &cid, &ctl);
								x = (winBounds.right  - winBounds.left) - (btnBounds.right - btnBounds.left) - 28;
								y = (winBounds.bottom - winBounds.top ) - 40;
								SizeControl(ctl, x, y);

								result = noErr;
							}
						}
					}
				#else
					break;
				#endif
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
							case kNewButton:
								AddCheatItem();
								result = noErr;
								break;
								
							case kDelButton:
								DeleteCheatItem();
								result = noErr;
								break;
								
							case kAllButton:
								EnableAllCheatItems();
								result = noErr;
						}
					}
			}
	}
	
	return result;
}
