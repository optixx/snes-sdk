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
#include "ppu.h"
#include "gfx.h"
#include "soundux.h"

#include <QuickTime/QuickTime.h>

#include "mac-prefix.h"
#include "mac-gworld.h"
#include "mac-os.h"
#include "mac-screenshot.h"
#include "mac-quicktime.h"

#define	kMovDoubleSize		(1 << 0)
#define	kMovExtendedHeight	(1 << 1)

static void CheckError(OSStatus, int);
static void MacQTOpenVideoComponent(ComponentInstance *);
static void MacQTCloseVideoComponent(ComponentInstance);

typedef struct
{
	Movie					movie;
	Track					vTrack, sTrack;
	Media					vMedia, sMedia;
	ComponentInstance 		vci;
	ImageDescriptionHandle	imageDesc;
	SoundDescriptionHandle  soundDesc;
	DataHandler				dataHandler;
	Handle					compressedData;
	Handle					soundBuffer;
	Handle					dataRef;
	OSType					dataRefType;
	GWorldPtr				gw, bw;
	Rect					frame;
	long					keyFrame, keyFrameCount;
	long					frameSkip, frameSkipCount;
	int						pw, ph;
	int						soundBufferSize;
	int						samplesPerSec;
	Boolean					firstFrame;
}	MacQTState;

static MacQTState	sqt;

static void CheckError(OSStatus err, int n)
{
	if (err != noErr)
	{
		char	mes[32];

		sprintf(mes, "quicktime %02d", n);
		QuitWithFatalError(err, mes);
	}
}

static void MacQTOpenVideoComponent(ComponentInstance *rci)
{	
	OSStatus			err;
	ComponentInstance	ci;
	
	ci = OpenDefaultComponent(StandardCompressionType, StandardCompressionSubType);
	CheckError((ci == nil), 01);
	
	CFDataRef	data;

	data = (CFDataRef) CFPreferencesCopyAppValue(CFSTR("QTVideoSetting"), kCFPreferencesCurrentApplication);
	if (data)
	{
		CFIndex	len;
		Handle	hdl;
		
		len = CFDataGetLength(data);
		hdl = NewHandleClear((Size) len);
		if (MemError() == noErr)
		{	
			HLock(hdl);
			CFDataGetBytes(data, CFRangeMake(0, len), (unsigned char *) *hdl);
			err = SCSetInfo(ci, scSettingsStateType, &hdl);
		
			DisposeHandle(hdl);
		}

		CFRelease(data);
	}
	else
	{
		SCSpatialSettings	ss;
		SCTemporalSettings	ts;
		
		ss.codecType       = kAnimationCodecType;
		ss.codec           = 0;
		ss.depth           = 16;
		ss.spatialQuality  = codecMaxQuality;
		err = SCSetInfo(ci, scSpatialSettingsType, &ss);
		
		ts.frameRate       = FixRatio(Memory.ROMFramesPerSecond, 1);
		ts.keyFrameRate    = Memory.ROMFramesPerSecond;
		ts.temporalQuality = codecMaxQuality;
		err = SCSetInfo(ci, scTemporalSettingsType, &ts);
	}

	*rci = ci;
}

static void MacQTCloseVideoComponent(ComponentInstance ci)
{
	OSStatus	err;

	err = CloseComponent(ci);
	CheckError(err, 11);
}

void MacQTVideoConfig(WindowRef parent)
{
	OSStatus			err;
	ComponentInstance	ci;
	
	if (running)
		return;

	MacQTOpenVideoComponent(&ci);
	
	long	flag;

	flag = scListEveryCodec | scAllowZeroKeyFrameRate | scDisableFrameRateItem;
	err = SCSetInfo(ci, scPreferenceFlagsType, &flag);
	
	PicHandle   pict;
	Rect		rct;
	int			width, height;

	width  = IPPU.RenderedScreenWidth;
	height = IPPU.RenderedScreenHeight;
	pict = GetScreenAsPicHandle(width, height, width, height);
	HNoPurge((Handle) pict);
	rct.left   = (width  - scTestImageWidth)  >> 1;
	rct.top    = (height - scTestImageHeight) >> 1;
	rct.right  = rct.left + scTestImageWidth;
	rct.bottom = rct.top  + scTestImageHeight;
	err = SCSetTestImagePictHandle(ci, pict, &rct, scPreferCropping);
	
	SCWindowSettings	ws;
	
	ws.size          = sizeof(SCWindowSettings);
	ws.windowRefKind = scWindowRefKindCarbon;
	ws.parentWindow  = parent;
	
	err = SCSetInfo(ci, scWindowOptionsType, &ws);

	err = SCRequestSequenceSettings(ci);
	if (err == noErr)
	{
		CFDataRef	data;
		Handle		hdl;

		err = SCGetInfo(ci, scSettingsStateType, &hdl);
		if (err == noErr)
		{
			HLock(hdl);
			data = CFDataCreate(kCFAllocatorDefault, (unsigned char *) *hdl, GetHandleSize(hdl));
			if (data)
			{
				CFPreferencesSetAppValue(CFSTR("QTVideoSetting"), data, kCFPreferencesCurrentApplication);
				CFRelease(data);
			}
			
			DisposeHandle(hdl);
		}
	}
	
	MacQTCloseVideoComponent(ci);
	
	KillPicture(pict);
}

void MacQTStartRecording(char *path)
{
	OSStatus	err;
	CFStringRef str;
	CFURLRef	url;

	memset(&sqt, 0, sizeof(sqt));

	// storage

	str = CFStringCreateWithCString(kCFAllocatorDefault, path, MAC_PATH_ENCODING);
	url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, str, kCFURLPOSIXPathStyle, false);
	err = QTNewDataReferenceFromCFURL(url, 0, &(sqt.dataRef), &(sqt.dataRefType));
	CheckError(err, 21);
	CFRelease(url);
	CFRelease(str);
	
	err = CreateMovieStorage(sqt.dataRef, sqt.dataRefType, 'TVOD', smSystemScript, createMovieFileDeleteCurFile | newMovieActive, &(sqt.dataHandler), &(sqt.movie));
	CheckError(err, 22);
	
	// video

	MacQTOpenVideoComponent(&(sqt.vci));
	
	SCTemporalSettings	ts;

	err = SCGetInfo(sqt.vci, scTemporalSettingsType, &ts);
	ts.frameRate = FixRatio(Memory.ROMFramesPerSecond, 1);
	if (ts.keyFrameRate < 1)
		ts.keyFrameRate = Memory.ROMFramesPerSecond;
	sqt.keyFrame  = sqt.keyFrameCount  = ts.keyFrameRate;
	sqt.frameSkip = sqt.frameSkipCount = (macQTMovFlag & 0xFF00) >> 8;
	err = SCSetInfo(sqt.vci, scTemporalSettingsType, &ts);

	sqt.frame.top    = 0;
	sqt.frame.left   = 0;
	sqt.frame.right  = ((macQTMovFlag & kMovDoubleSize) ? 2 : 1) * SNES_WIDTH;
	sqt.frame.bottom = ((macQTMovFlag & kMovDoubleSize) ? 2 : 1) * ((macQTMovFlag & kMovExtendedHeight) ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT);

	sqt.pw = sqt.ph = 0;
	sqt.firstFrame = true;
	
	Rect	rct;

	SetRect(&rct, 0, 0, SNES_WIDTH * 2, SNES_HEIGHT_EXTENDED * 2 + 4);
	InitGWorld(&(sqt.bw), &rct, 16);

	sqt.vTrack = NewMovieTrack(sqt.movie, FixRatio(sqt.frame.right, 1), FixRatio(sqt.frame.bottom, 1), kNoVolume);
	CheckError(GetMoviesError(), 23);
	
	sqt.vMedia = NewTrackMedia(sqt.vTrack, VideoMediaType, Memory.ROMFramesPerSecond, nil, 0);
	CheckError(GetMoviesError(), 24);
	
	err = BeginMediaEdits(sqt.vMedia);
	CheckError(err, 25);
	
	// sound
	
	sqt.soundDesc = (SoundDescriptionHandle) NewHandleClear(sizeof(SoundDescription));
	CheckError(MemError(), 26);

	(**sqt.soundDesc).descSize    = sizeof(SoundDescription);
#ifdef __BIG_ENDIAN__
	(**sqt.soundDesc).dataFormat  = Settings.SixteenBitSound ? k16BitBigEndianFormat    : k8BitOffsetBinaryFormat;
#else
	(**sqt.soundDesc).dataFormat  = Settings.SixteenBitSound ? k16BitLittleEndianFormat : k8BitOffsetBinaryFormat;
#endif
	(**sqt.soundDesc).numChannels = Settings.Stereo ? 2 : 1;
	(**sqt.soundDesc).sampleSize  = Settings.SixteenBitSound ? 16 : 8;
	(**sqt.soundDesc).sampleRate  = (UnsignedFixed) FixRatio(Settings.SoundPlaybackRate, 1);
	
	sqt.samplesPerSec = Settings.SoundPlaybackRate / Memory.ROMFramesPerSecond;
	
	sqt.soundBufferSize = sqt.samplesPerSec;
	if (Settings.SixteenBitSound)
		sqt.soundBufferSize <<= 1;
	if (Settings.Stereo)
		sqt.soundBufferSize <<= 1;

	sqt.soundBuffer = NewHandleClear(sqt.soundBufferSize);
	CheckError(MemError(), 27);
	HLock(sqt.soundBuffer);
	
	sqt.sTrack = NewMovieTrack(sqt.movie, 0, 0, kFullVolume);
	CheckError(GetMoviesError(), 28);

	sqt.sMedia = NewTrackMedia(sqt.sTrack, SoundMediaType, Settings.SoundPlaybackRate, nil, 0);
	CheckError(GetMoviesError(), 29);

	err = BeginMediaEdits(sqt.sMedia);
	CheckError(err, 30);
}

void MacQTRecordFrame(int width, int height)
{
	OSStatus	err;

	// video
	
	Rect	rct;
	
	SetRect(&rct, 0, 0, width, height);

	if (((sqt.pw != width) || (sqt.ph != height)) || (((drawingMethod == kDrawingDirect) || (drawingMethod == kDrawingBlitGL)) && multiprocessor))
	{
		if (sqt.gw)
		{
			DisposeGWorld(sqt.gw);
			sqt.gw = nil;
		}

		int	pitch = Settings.OpenGLEnable ? (width << 1) : (SNES_WIDTH << 2);

#ifdef __BIG_ENDIAN__
		err = QTNewGWorldFromPtr(&(sqt.gw), k16BE555PixelFormat, &rct, nil, nil, 0, GFX.Screen, pitch);
#else
		err = QTNewGWorldFromPtr(&(sqt.gw), k16LE555PixelFormat, &rct, nil, nil, 0, GFX.Screen, pitch);
#endif
		CheckError(err, 41);
		
		sqt.pw = width;
		sqt.ph = height;
	}

	if (sqt.frameSkipCount == sqt.frameSkip)
	{
		Rect	src, dst;

		src = rct;

		if ((!(height % SNES_HEIGHT_EXTENDED)) && (!(macQTMovFlag & kMovExtendedHeight)))
			src.bottom = SNES_HEIGHT << ((height > 256) ? 1 : 0);
			
		if ((sqt.frame.bottom << 1) % src.bottom)
		{
			Rect	b1, b2;

			b1.left  = b2.left  = dst.left  = 0;
			b1.right = b2.right = dst.right = sqt.frame.right;

			int	x = (macQTMovFlag & kMovDoubleSize) ? 1 : 0;

			if (drawoverscan)
			{
				b1.top    = 0;
				b1.bottom = 0;

				b2.top    = SNES_HEIGHT << x;
				b2.bottom = SNES_HEIGHT_EXTENDED << x;
			}
			else
			{
				b1.top    = 0;
				b1.bottom = (SNES_HEIGHT_EXTENDED - SNES_HEIGHT) >> (1 - x);
				
				b2.top    = b1.bottom + (SNES_HEIGHT << x);
				b2.bottom = SNES_HEIGHT_EXTENDED << x;
			}
			
			dst.top    = b1.bottom;
			dst.bottom = b2.top;
			
			PrepareForGDrawing(sqt.bw);
			PaintRect(&b1);
			PaintRect(&b2);
			FinishGDrawing(sqt.bw);
		}
		else
			dst = sqt.frame;
		
		CopyBits(GetPortBitMapForCopyBits(sqt.gw), GetPortBitMapForCopyBits(sqt.bw), &src, &dst, srcCopy, nil);
	}

	if (sqt.firstFrame)
	{
		sqt.firstFrame = false;

		err = SCCompressSequenceBegin(sqt.vci, GetPortPixMap(sqt.bw), &(sqt.frame), &(sqt.imageDesc));
		CheckError(err, 42);
	}
	
	if (sqt.frameSkipCount == sqt.frameSkip)
	{
		long	dataSize;
		short	flag = (sqt.keyFrameCount == sqt.keyFrame) ? 0 : mediaSampleNotSync;
	
		err = SCCompressSequenceFrame(sqt.vci, GetPortPixMap(sqt.bw), &(sqt.frame), &(sqt.compressedData), &dataSize, &flag);
		CheckError(err, 43);
	
		err = AddMediaSample(sqt.vMedia, sqt.compressedData, 0, dataSize, 1 + sqt.frameSkip, (SampleDescriptionHandle) (sqt.imageDesc),	1, flag, nil);
		CheckError(err, 44);
		
		sqt.keyFrameCount--;
		if (sqt.keyFrameCount <= 0)
			sqt.keyFrameCount = sqt.keyFrame;
	}

	sqt.frameSkipCount--;
	if (sqt.frameSkipCount < 0)
		sqt.frameSkipCount = sqt.frameSkip;

	// sound

	int	sample_count = sqt.soundBufferSize;
	if (Settings.SixteenBitSound)
		sample_count >>= 1;
		
	S9xMixSamples((uint8 *) *(sqt.soundBuffer), sample_count);
	
	err = AddMediaSample(sqt.sMedia, sqt.soundBuffer, 0, sqt.soundBufferSize, 1, (SampleDescriptionHandle) sqt.soundDesc, sqt.samplesPerSec, mediaSampleNotSync, nil);
	CheckError(err, 45);
	
}

void MacQTStopRecording(void)
{
	OSStatus   err;
	
	// video
	
	err = SCCompressSequenceEnd(sqt.vci);
	CheckError(err, 51);

	err = EndMediaEdits(sqt.vMedia);
	CheckError(err, 52);

	err = InsertMediaIntoTrack(sqt.vTrack, 0, 0, GetMediaDuration(sqt.vMedia), (Fixed) 0x10000L);
	CheckError(err, 53);

	DisposeGWorld(sqt.bw);
	DisposeGWorld(sqt.gw);

	// sound
	
	err = EndMediaEdits(sqt.sMedia);
	CheckError(err, 54);

	err = InsertMediaIntoTrack(sqt.sTrack, 0, 0, GetMediaDuration(sqt.sMedia), (Fixed) 0x10000L);
	CheckError(err, 55);
	
	DisposeHandle(sqt.soundBuffer);
	DisposeHandle((Handle) sqt.soundDesc);

	// storage

	err = AddMovieToStorage(sqt.movie, sqt.dataHandler);
	CheckError(err, 56);
	
	MacQTCloseVideoComponent(sqt.vci);
	
	err = CloseMovieStorage(sqt.dataHandler);
	CheckError(err, 57);
	
	DisposeHandle(sqt.dataRef);
	DisposeMovie(sqt.movie);
}
