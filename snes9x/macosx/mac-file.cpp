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
#include "display.h"
#include "movie.h"

#include "mac-prefix.h"
#include "mac-dialog.h"
#include "mac-os.h"
#include "mac-stringtools.h"
#include "mac-file.h"

extern char	pfold[9];

extern "C" char * osd_GetPackDir(void);

static void AddFolderIcon(FSRef *, const char *);
static OSErr FindSNESFolder(FSRef *, char *, const char *);
static OSErr FindApplicationSupportFolder(FSRef *, char *, const char *);
static int S9xCompareSDD1IndexEntries(const void *, const void *);
static char * S9xGetPackDirectory(void);

static char	gPackFolderPath[PATH_MAX + 1];

static OSErr FindSNESFolder(FSRef *folderRef, char *folderPath, const char *folderName)
{
	OSStatus	err;
	CFURLRef	burl, purl;
	CFStringRef	fstr;
	FSRef		pref;
	UniChar		buffer[PATH_MAX + 1];
	Boolean		r;
	
	fstr = CFStringCreateWithCString(kCFAllocatorDefault, folderName, CFStringGetSystemEncoding());
	CFStringGetCharacters(fstr, CFRangeMake(0, CFStringGetLength(fstr)), buffer);

	burl = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	purl = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, burl);
	r    = CFURLGetFSRef(purl, &pref);
	
	err = FSMakeFSRefUnicode(&pref, CFStringGetLength(fstr), buffer, kTextEncodingUnicodeDefault, folderRef);
 	if (err == dirNFErr || err == fnfErr) 
	{
		err = FSCreateDirectoryUnicode(&pref, CFStringGetLength(fstr), buffer, kFSCatInfoNone, nil, folderRef, nil, nil);
		if (err == noErr)
			AddFolderIcon(folderRef, folderName);
	}
	
	if (err != noErr && !folderWarning)
	{
		AppearanceAlert(kAlertCautionAlert, kFolderFail, kFolderHint);
		folderWarning = true;
	}
	else
		err = FSRefMakePath(folderRef, (unsigned char *) folderPath, PATH_MAX);
		
	CFRelease(purl);
	CFRelease(burl);
	CFRelease(fstr);

	return err;
}

static OSErr FindApplicationSupportFolder(FSRef *folderRef, char *folderPath, const char *folderName)
{
	OSErr		err;
	FSRef		p2ref, p1ref;
	CFStringRef	fstr;
	UniChar		buffer[PATH_MAX + 1];
	UniChar		s9xfolder[6] = { 'S', 'n', 'e', 's', '9', 'x' },
				oldfolder[6] = { 'S', 'N', 'E', 'S', '9', 'X' };
			
	err = FSFindFolder(kUserDomain, kApplicationSupportFolderType, kCreateFolder, &p2ref);
	if (err)
		return (err);

	err = FSMakeFSRefUnicode(&p2ref, 6, s9xfolder, kTextEncodingUnicodeDefault, &p1ref);
	if (err == dirNFErr || err == fnfErr)
	{
		err = FSMakeFSRefUnicode(&p2ref, 6, oldfolder, kTextEncodingUnicodeDefault, &p1ref);
		if (err == dirNFErr || err == fnfErr)
			err = FSCreateDirectoryUnicode(&p2ref, 6, s9xfolder, kFSCatInfoNone, nil, &p1ref, nil, nil);
	}
	if (err)
		return (err);
		
	fstr = CFStringCreateWithCString(kCFAllocatorDefault, folderName, CFStringGetSystemEncoding());
	CFStringGetCharacters(fstr, CFRangeMake(0, CFStringGetLength(fstr)), buffer);

	err = FSMakeFSRefUnicode(&p1ref, CFStringGetLength(fstr), buffer, kTextEncodingUnicodeDefault, folderRef);
 	if (err == dirNFErr || err == fnfErr)
	{
		err = FSCreateDirectoryUnicode(&p1ref, CFStringGetLength(fstr), buffer, kFSCatInfoNone, nil, folderRef, nil, nil);
		if (err == noErr)
			AddFolderIcon(folderRef, folderName);
	}
	
	if (err != noErr && !folderWarning)
	{
		AppearanceAlert(kAlertCautionAlert, kFolderFail, kFolderHint);
		folderWarning = true;
	}
	else
		err = FSRefMakePath(folderRef, (unsigned char *) folderPath, PATH_MAX);

	CFRelease(fstr);
	
	return err;
}

void ChangeTypeAndCreator(const char *path, OSType type, OSType creator)
{
	OSErr	err;
	FSRef	ref;

	err = FSPathMakeRef((unsigned char *) path, &ref, nil);
	if (err == noErr)
	{
		FSCatalogInfo	catinfo;
		
		err = FSGetCatalogInfo(&ref, kFSCatInfoFinderInfo, &catinfo, nil, nil, nil);
		if (err == noErr)
		{
			((FileInfo *) &catinfo.finderInfo)->fileCreator = creator;
			((FileInfo *) &catinfo.finderInfo)->fileType    = type;
		
			err = FSSetCatalogInfo(&ref, kFSCatInfoFinderInfo, &catinfo);
		}
	}
}

static void AddFolderIcon(FSRef *fref, const char *folderName)
{
	OSStatus			err;
	FSCatalogInfo		fcat, icat;
	FSRef				bref, iref;
	CFStringRef			str;
	CFURLRef			url;
	IconFamilyHandle	family;
	IconRef				icon;
	HFSUniStr255		fork;
	Boolean				r;
	SInt16				resf;
	char				name[64];
	UniChar				iconName[5] = { 'I', 'c', 'o', 'n', '\r' };
	
	strcpy(name, "folder_");
	strcat(name, folderName);
	
	str = CFStringCreateWithCString(kCFAllocatorDefault, name, CFStringGetSystemEncoding());
	if (str)
	{ 
		url = CFBundleCopyResourceURL(CFBundleGetMainBundle(), str, CFSTR("icns"), nil);
		if (url)
		{
			r = CFURLGetFSRef(url, &bref);
			if (r)
			{
				err = RegisterIconRefFromFSRef('~9X~', 'TEMP', &bref, &icon);
				if (err == noErr)
				{
					err = FSGetResourceForkName(&fork);
					if (err == noErr)
					{
						err = FSCreateResourceFile(fref, 5, iconName, kFSCatInfoNone, nil, fork.length, fork.unicode, &iref, nil);
						if (err == noErr)
						{
							err = FSOpenResourceFile(&iref, fork.length, fork.unicode, fsWrPerm, &resf);
							if (err == noErr)
							{
								err = IconRefToIconFamily(icon, kSelectorAllAvailableData, &family);
								if (err == noErr)
								{
									AddResource((Handle) family, 'icns', -16455, "\p");
									WriteResource((Handle) family);
									ReleaseResource((Handle) family);
									
									err = FSGetCatalogInfo(&iref, kFSCatInfoFinderInfo, &icat, nil, nil, nil);
									((FileInfo *) &icat.finderInfo)->finderFlags |= kIsInvisible;
									((FileInfo *) &icat.finderInfo)->fileCreator = 'MACS';
									((FileInfo *) &icat.finderInfo)->fileType    = 'icon';
									err = FSSetCatalogInfo(&iref, kFSCatInfoFinderInfo, &icat);
									
									err = FSGetCatalogInfo(fref, kFSCatInfoFinderInfo, &fcat, nil, nil, nil);
									((FolderInfo *) &fcat.finderInfo)->finderFlags |=  kHasCustomIcon;
									((FolderInfo *) &fcat.finderInfo)->finderFlags &= ~kHasBeenInited;
									err = FSSetCatalogInfo(fref, kFSCatInfoFinderInfo, &fcat);
								}
								
								CloseResFile(resf);
							}
						}
					}
					
					err = UnregisterIconRef('~9X~', 'TEMP');
				}
			}
			
			CFRelease(url);
		}
		
		CFRelease(str);
	}
}

Boolean IsLockedMedia(FSVolumeRefNum volume)
{
    OSErr			err;
	FSVolumeInfo	info;
	
	err = FSGetVolumeInfo(volume, 0, nil, kFSVolInfoFlags, &info, nil, nil);
    if (err == noErr)
	{
		if ((info.flags & kFSVolFlagHardwareLockedMask) || (info.flags & kFSVolFlagSoftwareLockedMask))
			return true;
	}
	
	return false;
}

const char * S9xGetFilename(const char *inExt, enum s9x_getdirtype dirtype)
{
	static char	filePath[PATH_MAX + 1];

	OSErr		err;
	FSRef		ref;
	uint32		type;
	char		folderName[16];
	char		drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	const char	*p;
	
	folderName[0] = filePath[0] = 0;

	if (strlen(inExt) < 4)
		return filePath;

	p = inExt + strlen(inExt) - 4;

#ifdef __BIG_ENDIAN__
	type = *(uint32 *) p;
#else
	type = ((uint32) p[0] << 24) + ((uint32) p[1] << 16) + ((uint32) p[2] << 8) + (uint32) p[3];
#endif

	switch (type)
	{
		case '.srm':
		case '.rtc':
			strcpy(folderName, "SRAMs");
			break;
		
		case '.frz':
			strcpy(folderName, "Freezes");
			break;

		case '.spc':
			strcpy(folderName, "SPCs");
			break;
		
		case '.cht':
			strcpy(folderName, "Cheats");
			break;
			
		case '.ips':
			strcpy(folderName, "IPSs");
			break;
			
		case '.png':
			strcpy(folderName, "Screenshots");
			break;
			
		case '.dat':
		case '.out':
			strcpy(folderName, "Logs");
			break;
		
		case '.bio':	// dummy
			strcpy(folderName, "BIOSs");
			break;
			
		case '.pac':	// dummy
			strcpy(folderName, "Packs");
	}

	if (folderName[0] && ((saveInROMFolder != 1) || lockedROMMedia))
	{
		if (saveInROMFolder == 0)
			err = FindSNESFolder(&ref, filePath, folderName);
		else
			err = FindApplicationSupportFolder(&ref, filePath, folderName);
			
		if (err == noErr)
		{			
			_splitpath(Memory.ROMFilename, drive, dir, fname, ext);
			strcat(filePath, MAC_PATH_SEPARATOR);
			strcat(filePath, fname);
			strcat(filePath, inExt);
		}
		else
		{
			_splitpath(Memory.ROMFilename, drive, dir, fname, ext);
			_makepath(filePath, drive, dir, fname, inExt);
		}
	}
	else
	{
	    _splitpath(Memory.ROMFilename, drive, dir, fname, ext);
		_makepath(filePath, drive, dir, fname, inExt);
	}

	return filePath;
}

const char * S9xGetSPCFilename(void)
{
	char	spcExt[16];
	
	sprintf(spcExt, ".%03d.spc", (int) spcFileCount);
	
	spcFileCount++;
	if (spcFileCount == 999)
		spcFileCount = 0;

	return S9xGetFilename(spcExt, SPC_DIR);
}

const char * S9xGetPNGFilename(void)
{
	char	pngExt[16];
	
	sprintf(pngExt, ".%03d.png", (int) pngFileCount);
	
	pngFileCount++;
	if (pngFileCount == 999)
		pngFileCount = 0;

	return S9xGetFilename(pngExt, SCREENSHOT_DIR);
}

const char * S9xGetFreezeFilename(int which)
{
	char	frzExt[16];
	
	sprintf(frzExt, ".%03d.frz", which);
	
	return S9xGetFilename(frzExt, SNAPSHOT_DIR);
}

extern "C" const char * S9xGetFilenameInc(const char *inExt, enum s9x_getdirtype dirtype)
{
	if (strlen(inExt) < 4)
		return nil;
	
	switch (*(long *) (inExt + strlen(inExt) - 4))
	{
		case '.spc':
			return S9xGetSPCFilename();
		
		case '.png':
			return S9xGetPNGFilename();
	}
	
	return nil;
}

const char * S9xChooseFilename(bool8 read_only)
{
	return nil;
}

const char * S9xChooseMovieFilename(bool8 read_only)
{
	return nil;
}

bool8 S9xOpenSnapshotFile(const char *fname, bool8 read_only, STREAM *file)
{
    if (read_only)
    {
		if (0 != (*file = OPEN_STREAM(fname, "rb")))
		    return true;
    }
    else
    {
		if (0 != (*file = OPEN_STREAM(fname, "wb")))
		    return true;
    }

    return false;
}

void S9xCloseSnapshotFile(STREAM file)
{
    CLOSE_STREAM(file);
}

const char * S9xBasename(const char *in)
{
	#pragma unused (in)

	return "";
}

static int S9xCompareSDD1IndexEntries(const void *p1, const void *p2)
{
    return (*(uint32 *) p1 - *(uint32 *) p2);
}

void S9xLoadSDD1Data(void)
{
	FILE	*fs;
	int		len;
	char	path[PATH_MAX + 1], index[PATH_MAX + 1], data[PATH_MAX + 1];
	
    Memory.FreeSDD1Data();
	
	if (macSDD1Pack == 1)
	{
		Settings.SDD1Pack = true;
		printf("Using on-the-fly S-DD1 decompression.\n");
		return;
	}
	
	Settings.SDD1Pack = false;

    strcpy(path, S9xGetPackDirectory());
	
    strcpy(index, path);
    strcat(index, "SDD1GFX.idx");
    strcpy(data,  path);
    strcat(data,  "SDD1GFX.dat");

	len = 0;
	
    fs = fopen(index, "rb");
    if (fs)
    {
        fseek(fs, 0, SEEK_END);
        len = ftell(fs);
        rewind(fs);
        Memory.SDD1Index = (uint8 *) malloc(len);
        fread(Memory.SDD1Index, 1, len, fs);
        fclose(fs);

        Memory.SDD1Entries = len / 12;
		
		fs = fopen(data, "rb");
        if (!fs)
        {
            free((char *) Memory.SDD1Index);
            Memory.SDD1Index = nil;
            Memory.SDD1Entries = 0;
			S9xMessage(0, 0, "Graphics Pack not found!");
			
			if (macSDD1Pack == 2)
			{
				printf("Using on-the-fly S-DD1 decompression.\n");
				Settings.SDD1Pack = true;
			}
		}
        else
        {
 			uint8			*ptr;
			unsigned int	i;
			
			fseek(fs, 0, SEEK_END);
            len = ftell(fs);
            rewind(fs);
            Memory.SDD1Data = (uint8 *) malloc(len);
            fread(Memory.SDD1Data, 1, len, fs);
            fclose(fs);
			
			ptr = Memory.SDD1Index;
			
		#ifdef __BIG_ENDIAN__
          	for (i = 0; i < Memory.SDD1Entries; i++, ptr += 12)
		    {
				SWAP_DWORD((*(uint32 *) (ptr + 0)));
				SWAP_DWORD((*(uint32 *) (ptr + 4)));
				SWAP_DWORD((*(uint32 *) (ptr + 8)));
		    }
		#endif

			qsort(Memory.SDD1Index, Memory.SDD1Entries, 12, S9xCompareSDD1IndexEntries);
        }
    }
	else
	{
		S9xMessage(0, 0, "Graphics Pack not found!");
		if (macSDD1Pack == 2)
		{
			printf("Using on-the-fly S-DD1 decompression.\n");
			Settings.SDD1Pack = true;
		}
	}
}

static char * S9xGetPackDirectory(void)
{
	OSStatus	err;
	FSRef		ref;
	Boolean		isFolder, wasAliased;
	int			n;
	char		drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	static char	path[PATH_MAX + 1];
	static char	dummy[] = "~/";
	
	pfold[0] = 0;	// unused

	gPackFolderPath[0] = 0;

	strcpy(path, S9xGetFilename(".pac", DEFAULT_DIR));	
	n = strlen(path);
	if (n < 4)
		return dummy;
		
	path[n - 4] = 0;
	err = FSPathMakeRef((unsigned char *) path, &ref, nil);
	if (err == noErr)
	{
		err = FSResolveAliasFile(&ref, false, &isFolder, &wasAliased);
		if ((err == noErr) && isFolder)
		{
			err = FSRefMakePath(&ref, (unsigned char *) path, PATH_MAX);
			if (err == noErr)
			{
				strcat(path, MAC_PATH_SEPARATOR);
				strcpy(gPackFolderPath, path);

				return path;
			}
			else
				return dummy;
		}
	}

	char	fold[16];
		
	if (strncmp(Memory.ROMName, "Star Ocean",            10) == 0)
		strcpy(fold, "SOCNSDD1");
	else
	if (strncmp(Memory.ROMName, "STREET FIGHTER ALPHA2", 21) == 0)
	{
		if (Memory.ROMRegion == 1)
			strcpy(fold, "SFA2SDD1");
		else
			strcpy(fold, "SFA2SDD1.E");
	}
	else
	if (strncmp(Memory.ROMName, "STREET FIGHTER ZERO2" , 20) == 0)
		strcpy(fold, "SFZ2SDD1");
	else
	if (strncmp(Memory.ROMName, "SUPER POWER LEAG 4   ", 21) == 0)
		strcpy(fold, "SPL4-SP7");
	else
	if (strncmp(Memory.ROMName, "MOMOTETSU HAPPY      ", 21) == 0)
		strcpy(fold, "SMHT-SP7");
	else
	if (strncmp(Memory.ROMName, "HU TENGAI MAKYO ZERO ", 21) == 0)
		strcpy(fold, "FEOEZSP7");
	else
	if (strncmp(Memory.ROMName, "JUMP TENGAIMAKYO ZERO", 21) == 0)
		strcpy(fold, "SJUMPSP7");
	else
		strcpy(fold, "MISCPACK");
	
	_splitpath(path, drive, dir, fname, ext);
	_makepath(path, drive, dir, fold, "");

	err = FSPathMakeRef((unsigned char *) path, &ref, nil);
	if (err == noErr)
	{
		err = FSResolveAliasFile(&ref, false, &isFolder, &wasAliased);
		if ((err == noErr) && isFolder)
		{
			err = FSRefMakePath(&ref, (unsigned char *) path, PATH_MAX);
			if (err == noErr)
			{
				strcat(path, MAC_PATH_SEPARATOR);
				strcpy(gPackFolderPath, path);

				return path;
			}
			else
				return dummy;
		}
	}
	
	return dummy;
}

const char * S9xGetSPC7110Directory(void)
{
	//printf("Reading SPC7110 Graphics Pack\n");
	return gPackFolderPath;
}

const char * S9xGetDirectory(enum s9x_getdirtype dirtype)
{
	static char	path[PATH_MAX + 1];

	char	inExt[16];
	char	drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	switch (dirtype)
	{
		case SNAPSHOT_DIR:		strcpy(inExt, ".frz");	break;
		case SRAM_DIR:			strcpy(inExt, ".srm");	break;
		case SCREENSHOT_DIR:	strcpy(inExt, ".png");	break;
		case SPC_DIR:			strcpy(inExt, ".spc");	break;
		case PATCH_DIR:			strcpy(inExt, ".cht");	break;
		case BIOS_DIR:			strcpy(inExt, ".bio");	break;
		default:				strcpy(inExt, ".xxx");
	}
	
	_splitpath(S9xGetFilename(inExt, dirtype), drive, dir, fname, ext);
	_makepath(path, drive, dir, "", "");
	
	int	l = strlen(path);
	if (l > 1)
		path[l - 1] = 0;
	
	return path;
}

extern "C" char * osd_GetPackDir(void)
{
	return S9xGetPackDirectory();
}

void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
	short	x;
	
	drive[0] = '\0';
	fname[0] = '\0';
	ext[0]   = '\0';
	dir[0]   = '\0';
	
	if (strlen(path) < 1)
		return;
	
	x = strlen(path) - 1;
	while (x && (path[x] != MAC_PATH_SEP_CHAR))
		x--;
	
	if (x)
	{
		strcpy(dir, path);
		dir[x + 1] = '\0';
		
		strcpy(fname, path + x + 1);
	}
	else
		strcpy(fname, path);
	
	x = strlen(fname);
	while (x && (fname[x] != '.'))
		x--;
	
	if (x)
	{
		strcpy(ext, fname + x);
		fname[x] = '\0';
	}
}

void _makepath(char *path, const char *drive, const char *dir, const char *fname, const char *ext)
{
	#pragma unused (drive)

	path[0] = '\0';

	if (dir && dir[0])
		strcat(path, dir);
	
	if (fname && fname[0])
		strcat(path, fname);
	
	if (ext && ext[0])
	{
		if (ext[0] != '.')
			strcat(path, ".");
		strcat(path, ext);
	}
}
