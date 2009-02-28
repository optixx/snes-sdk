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

#include <errno.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <signal.h>

#include <crt0.h>

#include "snes9x.h"
#include "memmap.h"
#include "debug.h"
#include "cpuexec.h"
#include "ppu.h"
#include "snapshot.h"
#include "apu.h"
#include "display.h"
#include "gfx.h"
#include "soundux.h"

#ifdef NETPLAY_SUPPORT
#include "netplay.h"
#endif

uint32 joypads [5] = {0};
int NumControllers = 2;

#ifdef JOYSTICK_SUPPORT
bool8 joystick_has_four_buttons = FALSE;
bool8 joystick_has_six_buttons = FALSE;
bool8 joystick_has_eight_buttons = FALSE;

#ifdef SIDEWINDER_SUPPORT
void InitSidewinders ();
int ReadSidewinders ();
bool8 alternate_sidewinder_mapping = FALSE;
#endif
void InitJoysticks ();
void ReadJoysticks ();
#endif

void InitTimer ();

char *rom_filename = NULL;
char *snapshot_filename = NULL;

void OutOfMemory ()
{
    fprintf (stderr, "\
Snes9X: Memory allocation failure - not enough RAM/virtual memory available.\n\
        S9xExiting...\n");
    Memory.Deinit ();
    S9xDeinitAPU ();
    
    exit (1);
}

void S9xParseArg (char **argv, int &i, int argc)
{
    if (strcmp (argv [i], "-4") == 0 ||
	     strcasecmp (argv [i], "-four") == 0)
    {
	joystick_has_four_buttons = TRUE;
	Settings.JoystickEnabled = 1;
    }
    else if (strcmp (argv [i], "-6") == 0 ||
	     strcasecmp (argv [i], "-six") == 0)
    {
	joystick_has_six_buttons = TRUE;
	Settings.JoystickEnabled = 1;
    }
    else if (strcmp (argv [i], "-8") == 0 ||
	     strcasecmp (argv [i], "-eight") == 0)
    {
	joystick_has_eight_buttons = TRUE;
	Settings.JoystickEnabled = 1;
    }
    else if (strcmp (argv [i], "-j") == 0 ||
	     strcasecmp (argv [i], "-nojoy") == 0)
	Settings.JoystickEnabled = 0;
    else if (strcasecmp (argv [i], "-b") == 0 ||
	     strcasecmp (argv [i], "-bs") == 0 ||
	     strcasecmp (argv [i], "-buffersize") == 0)
    {
	if (i + 1 < argc)
	    Settings.SoundBufferSize = atoi (argv [++i]);
	else
	    S9xUsage ();
    }
    else if (strcmp (argv [i], "-l") == 0 ||
	     strcasecmp (argv [i], "-loadsnapshot") == 0)
    {
	if (i + 1 < argc)
	    snapshot_filename = argv [++i];
	else
	    S9xUsage ();
    }
#ifdef SIDEWINDER_SUPPORT
    else if (strcmp (argv [i], "-a") == 0)
    {
	alternate_sidewinder_mapping = TRUE;
    }
#endif
    else
	S9xParseDisplayArg (argv, i, argc);
}

int main (int argc, char **argv)
{
    _crt0_startup_flags = _CRT0_FLAG_FILL_SBRK_MEMORY;

    if (argc < S9xMinCommandLineArgs ())
	S9xUsage ();
	
    ZeroMemory (&Settings, sizeof (Settings));

#ifdef JOYSTICK_SUPPORT
    Settings.JoystickEnabled = TRUE;
#else
    Settings.JoystickEnabled = FALSE;
#endif

    Settings.SoundPlaybackRate = 4;
    Settings.Stereo = TRUE;
    Settings.SoundBufferSize = 0;
    Settings.CyclesPercentage = 100;
    Settings.DisableSoundEcho = FALSE;
    Settings.APUEnabled = Settings.NextAPUEnabled = TRUE;
    Settings.H_Max = SNES_CYCLES_PER_SCANLINE;
    Settings.SkipFrames = AUTO_FRAMERATE;
    Settings.ShutdownMaster = TRUE;
    Settings.FrameTimePAL = 20000;
    Settings.FrameTimeNTSC = 16667;
    Settings.FrameTime = Settings.FrameTimeNTSC;
    Settings.DisableSampleCaching = FALSE;
    Settings.DisableMasterVolume = FALSE;
    Settings.Mouse = FALSE;
    Settings.SuperScope = FALSE;
    Settings.MultiPlayer5 = FALSE;
    Settings.ControllerOption = SNES_JOYPAD;
    Settings.Transparency = FALSE;
    Settings.SixteenBit = FALSE;
    Settings.SupportHiRes = FALSE;
    Settings.NetPlay = FALSE;
    Settings.ServerName [0] = 0;
    Settings.ThreadSound = FALSE;
    Settings.AutoSaveDelay = 30;
#ifdef _NETPLAY_SUPPORT
    Settings.Port = NP_DEFAULT_PORT;
#endif
    Settings.ApplyCheats = TRUE;
    Settings.TurboMode = FALSE;
    Settings.TurboSkipFrames = 15;
    rom_filename = S9xParseArgs (argv, argc);

    Settings.Transparency = Settings.ForceTransparency;
    if (Settings.ForceNoTransparency)
	Settings.Transparency = FALSE;

    if (Settings.Transparency)
	Settings.SixteenBit = TRUE;

    Settings.HBlankStart = (256 * Settings.H_Max) / SNES_HCOUNTER_MAX;

    rom_filename = S9xParseArgs (argv, argc);

    if (!Memory.Init () || !S9xInitAPU())
	OutOfMemory ();

    uint32 saved_flags = CPU.Flags;

#ifdef GFX_MULTI_FORMAT
    S9xSetRenderPixelFormat (RGB565);
#endif

    if (rom_filename)
    {
	if (!Memory.LoadROM (rom_filename))
	{
	    char dir [_MAX_DIR];
	    char drive [_MAX_DRIVE];
	    char name [_MAX_FNAME];
	    char ext [_MAX_EXT];
	    char fname [_MAX_PATH];

	    _splitpath (rom_filename, drive, dir, name, ext);
	    _makepath (fname, drive, dir, name, ext);

	    strcpy (fname, S9xGetROMDirectory ());
	    strcat (fname, SLASH_STR);
	    strcat (fname, name);
	    if (ext [0])
	    {
		strcat (fname, ".");
		strcat (fname, ext);
	    }
	    _splitpath (fname, drive, dir, name, ext);
	    _makepath (fname, drive, dir, name, ext);
	    if (!Memory.LoadROM (fname))
	    {
		printf ("Error opening: %s\n", rom_filename);
		exit (1);
	    }
	}
	Memory.LoadSRAM (S9xGetSRAMFilename ());
    }
    else
    {
	S9xReset ();
	Settings.Paused |= 2;
    }
    CPU.Flags = saved_flags;

    S9xInitInputDevices ();

    S9xInitDisplay (argc, argv);
    if (!S9xGraphicsInit ())
	OutOfMemory ();
	
    S9xTextMode ();
#ifdef NETPLAY_SUPPORT
    if (strlen (Settings.ServerName) == 0)
    {
	char *server = getenv ("S9XSERVER");
	if (server)
	{
	    strncpy (Settings.ServerName, server, 127);
	    Settings.ServerName [127] = 0;
	}
    }
    char *port = getenv ("S9XPORT");
    if (Settings.Port >= 0 && port)
	Settings.Port = atoi (port);
    else
    if (Settings.Port < 0)
	Settings.Port = -Settings.Port;

    if (Settings.NetPlay)
    {
	int player;

	if (!S9xNetPlayConnectToServer (Settings.ServerName, Settings.Port,
					Memory.ROMName, player))
	{
	    fprintf (stderr, "\
Failed to connected to Snes9x netplay server \"%s\" on port %d.\n",
		 Settings.ServerName, Settings.Port);
	    S9xExit ();
	}
	fprintf (stderr, "\
Connected to \"%s\" on port %d as player #%d playing \"%s\"\n",
		 Settings.ServerName, Settings.Port, player, Memory.ROMName);
    }
    
#endif

    (void) S9xInitSound (Settings.SoundPlaybackRate, Settings.Stereo,
		      Settings.SoundBufferSize);
    if (!Settings.APUEnabled)
	S9xSetSoundMute (TRUE);

    if (snapshot_filename)
    {
	int Flags = CPU.Flags & (DEBUG_MODE_FLAG | TRACE_FLAG);
	if (!S9xLoadSnapshot (snapshot_filename))
	    exit (1);
	CPU.Flags |= Flags;
    }

    S9xGraphicsMode ();

    sprintf (String, "\"%s\" %s: %s", Memory.ROMName, TITLE, VERSION);
    S9xSetTitle (String);
    
#ifdef JOYSTICK_SUPPORT
    uint32 JoypadSkip = 0;
#endif

    InitTimer ();
    if (!Settings.APUEnabled)
	S9xSetSoundMute (FALSE);

#if 0
    {
	FILE *fs = fopen ("test.bin", "r");
	if (fs)
	{
	    int bytes = fread (IAPU.RAM, 1, 1024 * 64, fs);
	    fclose (fs);
	    int len = 0;
	    char buffer [200];
	    for (int i = 0; i < bytes; i += len)
	    {
		len = S9xAPUOPrint (buffer, i);
		printf ("%s\n", buffer);
	    }
	}
    }
#endif

#if 0
    if (Settings.APUEnabled)
    {
	// Needed for Goal
	for (int z = 0; z < 0x2000; z++)
	{
	    APU_EXECUTE1();
	}
    }
#endif

    while (1)
    {
	if (!Settings.Paused
#ifdef DEBUGGER
	    || (CPU.Flags & (DEBUG_MODE_FLAG | SINGLE_STEP_FLAG))
#endif
           )
	    S9xMainLoop ();

	if (Settings.Paused
#ifdef DEBUGGER
	    || (CPU.Flags & DEBUG_MODE_FLAG)
#endif
           )
	{
	    S9xSetSoundMute (TRUE);
	}

#ifdef DEBUGGER
	if (CPU.Flags & DEBUG_MODE_FLAG)
	{
	    S9xDoDebug ();
	}
	else
#endif
	if (Settings.Paused)
	    S9xProcessEvents (TRUE);

#ifdef JOYSTICK_SUPPORT
	if (Settings.JoystickEnabled && (JoypadSkip++ & 1) == 0)
	    ReadJoysticks ();
#endif
	S9xProcessEvents (FALSE);
	
	if (!Settings.Paused
#ifdef DEBUGGER
	    && !(CPU.Flags & DEBUG_MODE_FLAG)
#endif	    
           )
	{
	    S9xSetSoundMute (FALSE);
	}
    }
    return (0);
}

void S9xAutoSaveSRAM ()
{
    Memory.SaveSRAM (S9xGetSRAMFilename ());
}

void S9xExit ()
{
    S9xDeinitDisplay ();
    Memory.SaveSRAM (S9xGetSRAMFilename ());
    Memory.Deinit ();
    S9xDeinitAPU ();

#ifdef NETPLAY_SUPPORT
    if (Settings.NetPlay)
	S9xNetPlayDisconnect ();
#endif
    
    exit (0);
}

#ifdef SIDEWINDER_SUPPORT
int SidewinderToSNES [] =
{
    SNES_UP_MASK, SNES_DOWN_MASK, SNES_RIGHT_MASK, SNES_LEFT_MASK,
    SNES_B_MASK, SNES_A_MASK, SNES_A_MASK, 
    SNES_Y_MASK, SNES_X_MASK, SNES_B_MASK, 
    SNES_TL_MASK, SNES_TR_MASK,
    SNES_START_MASK, SNES_SELECT_MASK
};

int SidewinderToSNES2 [] =
{
    SNES_UP_MASK, SNES_DOWN_MASK, SNES_RIGHT_MASK, SNES_LEFT_MASK,
    SNES_A_MASK, SNES_B_MASK, SNES_A_MASK, 
    SNES_X_MASK, SNES_Y_MASK, SNES_B_MASK, 
    SNES_TL_MASK, SNES_TR_MASK,
    SNES_START_MASK, SNES_SELECT_MASK
};
#endif

const char *GetHomeDirectory ()
{
    return (".");
}

const char *S9xGetSnapshotDirectory ()
{
    static char filename [PATH_MAX];
    const char *snapshot;
    
    if (!(snapshot = getenv ("SNES96_SNAPSHOT_DIR")))
    {
	const char *home = GetHomeDirectory ();
	strcpy (filename, home);
	strcat (filename, SLASH_STR);
	strcat (filename, "snesnaps");
	mkdir (filename, 0777);
    }
    else
	return (snapshot);

    return (filename);
}

const char *S9xGetSRAMFilename ()
{
    static char filename [PATH_MAX];
    char drive [_MAX_DRIVE];
    char dir [_MAX_DIR];
    char fname [_MAX_FNAME];
    char ext [_MAX_EXT];

    _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
    strcpy (filename, S9xGetSnapshotDirectory ());
    strcat (filename, SLASH_STR);
    strcat (filename, fname);
    strcat (filename, ".srm");

    return (filename);
}

const char *S9xGetROMDirectory ()
{
    const char *roms;
    
    if (!(roms = getenv ("SNES96_ROM_DIR")))
	return ("." SLASH_STR "roms");
    else
	return (roms);
}

const char *S9xBasename (const char *f)
{
    const char *p;
    if ((p = strrchr (f, '/')) != NULL || (p = strrchr (f, '\\')) != NULL)
	return (p + 1);

    if (p = strrchr (f, SLASH_CHAR))
	return (p + 1);

    return (f);
}

const char *S9xChooseFilename (bool8 read_only)
{
    char def [PATH_MAX];
    char title [PATH_MAX];
    char drive [_MAX_DRIVE];
    char dir [_MAX_DIR];
    char ext [_MAX_EXT];

    _splitpath (Memory.ROMFilename, drive, dir, def, ext);
    strcat (def, ".s96");
    sprintf (title, "%s snapshot filename",
	    read_only ? "Select load" : "Choose save");
    const char *filename;

    S9xSetSoundMute (TRUE);
    filename = S9xSelectFilename (def, S9xGetSnapshotDirectory (), "s96", title);
    S9xSetSoundMute (FALSE);
    return (filename);
}

bool8 S9xOpenSnapshotFile (const char *fname, bool8 read_only, STREAM *file)
{
    char filename [PATH_MAX];
    char drive [_MAX_DRIVE];
    char dir [_MAX_DIR];
    char ext [_MAX_EXT];

    _splitpath (fname, drive, dir, filename, ext);

    if (*drive || *dir == '/' ||
	*dir == '\\' ||
	(*dir == '.' && (*(dir + 1) == '/'
			 || *(dir + 1) == '\\'
        )))
    {
	strcpy (filename, fname);
	if (!*ext)
	    strcat (filename, ".s96");
    }
    else
    {
	strcpy (filename, S9xGetSnapshotDirectory ());
	strcat (filename, SLASH_STR);
	strcat (filename, fname);
	if (!*ext)
	    strcat (filename, ".s96");
    }
    
#ifdef ZLIB
    if (read_only)
    {
	if ((*file = OPEN_STREAM (filename, "rb")))
	    return (TRUE);
    }
    else
    {
	if ((*file = OPEN_STREAM (filename, "wb")))
	    return (TRUE);
    }
#else
    char command [PATH_MAX];
    
    if (read_only)
    {
	sprintf (command, "gzip -d <\"%s\"", filename);
	if (*file = popen (command, "r"))
	    return (TRUE);
    }
    else
    {
	sprintf (command, "gzip --best >\"%s\"", filename);
	if (*file = popen (command, "wb"))
	    return (TRUE);
    }
#endif
    return (FALSE);
}

void S9xCloseSnapshotFile (STREAM file)
{
#ifdef ZLIB
    CLOSE_STREAM (file);
#else
    pclose (file);
#endif
}

bool8 S9xInitUpdate ()
{
    return (TRUE);
}

bool8 S9xDeinitUpdate (int Width, int Height, bool8 sixteen_bit)
{
    S9xPutImage (Width, Height);

    return (TRUE);
}

static unsigned long now ()
{
    static unsigned long seconds_base = 0;
    struct timeval tp;
    gettimeofday (&tp, NULL);
    if (!seconds_base)
	seconds_base = tp.tv_sec;

    return ((tp.tv_sec - seconds_base) * 1000 + tp.tv_usec / 1000);
}

void OutputFrameRate ()
{
    static int FrameCount = 0;
    static unsigned long then = now ();

    if (++FrameCount % 60 == 0)
    {
	unsigned long here = now ();
//	printf ("\rFrame rate: %.2lfms", (double) (here - then) / 60);
//	fflush (stdout);
	then = here;
    }
}

void S9xToggleSoundChannel (int c)
{
    static int channel_enable = 255;
    if (c == 8)
	channel_enable = 255;
    else
	channel_enable ^= 1 << c;
    S9xSetSoundControl (channel_enable);
}

uint32 S9xReadJoypad (int which1)
{
#ifdef NETPLAY_SUPPORT
    if (Settings.NetPlay)
	return (S9xNetPlayGetJoypad (which1));
#endif
    if (which1 < NumControllers)
	return (0x80000000 | joypads [which1]);
    return (0);
}

