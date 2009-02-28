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

#ifdef __linux
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <stdlib.h>
#include <signal.h>

#include <vga.h>
#include <vgagl.h>
#include <vgakeyboard.h>

#include "snes9x.h"
#include "memmap.h"
#include "ppu.h"
#include "gfx.h"
#include "display.h"
#include "soundux.h"
#include "controls.h"
#include "movie.h"

#define COUNT(a) (sizeof(a) / sizeof(a[0]))

static bool8 planar;
static int screen_pitch;
static int screen_width;
static int screen_height;
static int mode = -1;
static bool8 stretch = FALSE;
static bool8 text_mode = TRUE;
static bool8 interpolation = FALSE;
static char prev_keystate [128] = "";
static bool8 restore_modex = FALSE;
static uint8 *DeltaScreen = NULL;
static vga_modeinfo *info = NULL;
static uint32 video_page_size = 64 * 1024;

uint8 snes9x_clear_change_log = 0;

#define ATTRCON_ADDR	0x3c0
#define MISC_ADDR	0x3c2
#define VGAENABLE_ADDR	0x3c3
#define SEQ_ADDR	0x3c4
#define GRACON_ADDR	0x3ce
#define CRTC_ADDR	0x3d4
#define STATUS_ADDR	0x3da

typedef struct
{
    unsigned port;
    unsigned char index;
    unsigned char value;
} Register;

typedef Register *RegisterPtr;
void outRegArray (Register *r, int n);
void _S9xSVGAKeyboardHandler(int scancode, int press);

Register scr256x256[] =
{
     { 0x3c2, 0x00, 0xe3},{ 0x3d4, 0x00, 0x5f},{ 0x3d4, 0x01, 0x3f},
     { 0x3d4, 0x02, 0x40},{ 0x3d4, 0x03, 0x82},{ 0x3d4, 0x04, 0x4A},
     { 0x3d4, 0x05, 0x9A},{ 0x3d4, 0x06, 0x23},{ 0x3d4, 0x07, 0xb2},
     { 0x3d4, 0x08, 0x00},{ 0x3d4, 0x09, 0x61},{ 0x3d4, 0x10, 0x0a},
     { 0x3d4, 0x11, 0xac},{ 0x3d4, 0x12, 0xff},{ 0x3d4, 0x13, 0x20},
     { 0x3d4, 0x14, 0x40},{ 0x3d4, 0x15, 0x07},{ 0x3d4, 0x16, 0x1a},
     { 0x3d4, 0x17, 0xa3},{ 0x3c4, 0x01, 0x01},{ 0x3c4, 0x04, 0x0e},
     { 0x3ce, 0x05, 0x40},{ 0x3ce, 0x06, 0x05},{ 0x3c0, 0x10, 0x41},
     { 0x3c0, 0x13, 0x00}
};

typedef struct
{
    int width;
    int height;
    int mode;
} Mode;

static Mode modes [] = {
    {320, 240, G320x240x256}, // 0
    {320, 200, G320x200x256}, // 1
    {256, 256, G320x200x256}, // 2
    {640, 480, G640x480x256}, // 3
    {800, 600, G800x600x256}, // 4
    {320, 200, G320x200x64K}, // 5
    {640, 480, G640x480x64K}, // 6
    {800, 600, G800x600x64K}, // 7
};

int S9xMinCommandLineArgs ()
{
    return (2);
}

void S9xGraphicsMode ()
{
    if (text_mode)
    {
	screen_width = modes [mode].width;
	screen_height = modes [mode].height;
	int ret = vga_setmode (modes [mode].mode);

	if (ret < 0)
	{
	    fprintf (stderr, "Unable to switch to requested screen mode/resolution:\n");
	    S9xExit ();
	}

	if (vga_setlinearaddressing () < 0)
	{
	    if (info->flags & EXT_INFO_AVAILABLE)
		video_page_size = info->aperture_size;
	    else
		video_page_size = 64 * 1024;
	}
	else
	    video_page_size = ~0;

	if (modes [mode].mode == G320x200x256 && screen_width == 256)
	{
	    iopl(3);
	    outRegArray (scr256x256, sizeof (scr256x256) / sizeof (Register));
	    screen_pitch = screen_width;
	}
    
	gl_setcontextvga (modes [mode].mode);
	if (keyboard_init ())
	{
	    fprintf (stdout, "Keyboard initialisation failed.\n");
	    S9xExit ();
	}
        keyboard_seteventhandler(&_S9xSVGAKeyboardHandler);
	text_mode = FALSE;
	if (DeltaScreen)
	    memset (DeltaScreen, 0xff, GFX.Pitch * IMAGE_HEIGHT);
    }
}

void S9xTextMode ()
{
//    if (!text_mode)
    {
	keyboard_close ();
	vga_setmode (TEXT);
	text_mode = TRUE;
    }
}

static struct sigaction sig1handler;
static struct sigaction oldsig1handler;
static struct sigaction sig2handler;
static struct sigaction oldsig2handler;

void Sig1HandlerFunction(int)
{
    extern void StopTimer ();
    StopTimer ();

    sigaction(SIGUSR2, &sig2handler, NULL);
    sigaction(SIGUSR1, &oldsig1handler, NULL);
    sigsetmask (0);
    raise(SIGUSR1);
}

void Sig2HandlerFunction(int)
{
    restore_modex = TRUE;

    sigaction(SIGUSR1, &sig1handler, NULL);
    sigaction(SIGUSR2, &oldsig2handler, NULL);
    sigsetmask (0);
    raise(SIGUSR2);
}

void S9xInitDisplay (int /*argc*/, char ** /*argv*/)
{
    if (vga_init() < 0)
    {
	fprintf (stdout, "Unable to initialise vga.\n");
	S9xExit ();
    }
    S9xTextMode ();

    if (mode < 0)
    {
	if (Settings.SixteenBit)
	    mode = 6;
	else
	    mode = 2;
    }
    info = vga_getmodeinfo (modes [mode].mode);
    if (info->flags & IS_MODEX)
	planar = 1;

    if (info->flags & CAPABLE_LINEAR)
	video_page_size = ~0;
    else
    if (info->flags & EXT_INFO_AVAILABLE)
	video_page_size = info->aperture_size;
    else
	video_page_size = 64 * 1024;

    if (!screen_pitch)
	screen_pitch = info->linewidth;
	
    if (info->bytesperpixel > 1)
    {
	Settings.Transparency = TRUE;
	Settings.SixteenBit = TRUE;
    }
    else
    {
	Settings.Transparency = FALSE;
	Settings.SixteenBit = FALSE;
    }

    if (info->width >= 512 && info->height >= 578)
	Settings.SupportHiRes = TRUE;

    if (!Settings.SixteenBit || info->width < 512 || info->height < 240)
	interpolation = FALSE;

    if (interpolation)
    {
	GFX.Pitch = (IMAGE_WIDTH + 2) * 2;
	GFX.Screen = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	GFX.SubScreen = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	DeltaScreen = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	GFX.ZBuffer = (uint8 *) malloc ((GFX.Pitch >> 1) * IMAGE_HEIGHT);
	GFX.SubZBuffer = (uint8 *) malloc ((GFX.Pitch >> 1) * IMAGE_HEIGHT);

	if (!GFX.Screen || !GFX.SubScreen || !DeltaScreen || 
	    !GFX.ZBuffer || !GFX.SubZBuffer)
	{
	    fprintf (stdout, "Cannot allocate screen buffer.\n");
	    S9xExit ();
	}
    }
    else
    if (Settings.SixteenBit)
    {
	GFX.Pitch = IMAGE_WIDTH * 2;
	GFX.Screen = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	GFX.SubScreen = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	GFX.ZBuffer = (uint8 *) malloc ((GFX.Pitch >> 1) * IMAGE_HEIGHT);
	GFX.SubZBuffer = (uint8 *) malloc ((GFX.Pitch >> 1) * IMAGE_HEIGHT);

	if (!GFX.Screen || !GFX.SubScreen)
	{
	    fprintf (stdout, "Cannot allocate screen buffer.\n");
	    S9xExit ();
	}
    }
    else
    {
	GFX.Pitch = IMAGE_WIDTH;
	GFX.Screen = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	if (!GFX.Screen)
	{
	    fprintf (stdout, "Cannot allocate screen buffer.\n");
	    S9xExit ();
	}
	GFX.SubScreen = NULL;
	DeltaScreen = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	if (!DeltaScreen)
	{
	    fprintf (stdout, "Cannot allocate shadow screen buffer.\n");
	    S9xExit ();
        }
	GFX.ZBuffer = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
	GFX.SubZBuffer = NULL;
    }
    ZeroMemory (GFX.Screen, GFX.Pitch * IMAGE_HEIGHT);
    if (GFX.SubScreen)
	ZeroMemory (GFX.SubScreen, GFX.Pitch * IMAGE_HEIGHT);
    if (DeltaScreen)
	ZeroMemory (DeltaScreen, GFX.Pitch * IMAGE_HEIGHT);

    sig1handler.sa_handler = Sig1HandlerFunction;
    sigemptyset (&sig1handler.sa_mask);
    sig1handler.sa_flags = 0;
    sig2handler.sa_handler = Sig2HandlerFunction;
    sigemptyset (&sig2handler.sa_mask);
    sig2handler.sa_flags = 0;
    sigaction (SIGUSR1, &sig1handler, &oldsig1handler);
    sigaction (SIGUSR2, &sig2handler, &oldsig2handler);
}

void S9xDeinitDisplay ()
{
    S9xTextMode ();
    if (GFX.Screen)
	free ((char *) GFX.Screen);
    if (GFX.SubScreen)
	free ((char *) GFX.SubScreen);
    if (DeltaScreen)
	free ((char *) DeltaScreen);
    if (GFX.ZBuffer)
	free ((char *) GFX.ZBuffer);
    if (GFX.SubZBuffer)
	free ((char *) GFX.SubZBuffer);
    GFX.Screen = NULL;
    GFX.SubScreen = NULL;
    DeltaScreen = NULL;
    GFX.ZBuffer = NULL;
    GFX.SubZBuffer = NULL;
}

void S9xSetPalette ()
{
    uint16 Brightness = IPPU.MaxBrightness * 138;
    for (int i = 0; i < 256; i++)
	vga_setpalette (i, 
			(((PPU.CGDATA [i] >> 0) & 0x1F) * Brightness) >> 10,
			(((PPU.CGDATA [i] >> 5) & 0x1F) * Brightness) >> 10,
			(((PPU.CGDATA [i] >> 10) & 0x1F) * Brightness) >> 10);
}

static uint8 mods=0;
static uint8 getMod(int code){
    if(code==SCANCODE_LEFTSHIFT || code==SCANCODE_RIGHTSHIFT) return 1;
    if(code==SCANCODE_LEFTCONTROL || code==SCANCODE_RIGHTCONTROL) return 2;
    if(code==SCANCODE_LEFTALT || code==SCANCODE_RIGHTALT) return 8;
    if(code==SCANCODE_LEFTWIN || code==SCANCODE_RIGHTWIN) return 64;
    return 0;
}

void _S9xSVGAKeyboardHandler(int scancode, int press) {
    int i;

    i=getMod(scancode);
    if(i){
        if(press==KEY_EVENTRELEASE){
            mods&=~i;
        } else {
            mods|=i;
        }
        return;
    }
    S9xReportButton((mods<<8)|(scancode&0xff), press!=KEY_EVENTRELEASE);
}

void S9xProcessEvents (bool8 block)
{
    if (restore_modex)
    {
	restore_modex = FALSE;

	ZeroMemory (prev_keystate, 128);
	if (!text_mode && modes [mode].mode == G320x200x256 && screen_width == 256)
	{
	    iopl(3);
	    outRegArray (scr256x256, sizeof (scr256x256) / sizeof (Register));
	}
	extern void InitTimer ();
	InitTimer ();
    }
    if (block)
    {
//	keyboard_waitforupdate ();
	usleep (10000);
	keyboard_update ();
    }
    else
	keyboard_update ();
}

void S9xSetTitle (const char * /*title*/)
{
}

void S9xPutImage (int width, int height)
{
    int y_buff;
    int y_start;
    int y_end;
    int x_start = (screen_width - width) >> 1;
    if (screen_height >= height)
    {
	y_start = (screen_height - height) >> 1;
	y_end = y_start + height;
	y_buff = 0;
    }
    else
    {
	y_start = 0;
	y_end = screen_height;
	y_buff = (height - screen_height) >> 1;
    }
	
    if (planar)
	vga_copytoplanar256 (GFX.Screen + y_buff * GFX.Pitch,
			     IMAGE_WIDTH,
			     y_start * screen_pitch + x_start / 4,
			     screen_pitch, width, y_end - y_start);
    else
    {
	if (screen_pitch == width && screen_height >= height)
	{
#if 0
	    memcpy (vga_getgraphmem () + screen_pitch * y_start,
		    GFX.Screen, width * height);
#else
	    register uint32 *s = (uint32 *) (vga_getgraphmem () + screen_pitch * y_start);
	    register uint32 *o = (uint32 *) DeltaScreen;
	    register uint32 *n = (uint32 *) GFX.Screen;
	    uint32 *end = (uint32 *) (GFX.Screen + width * height);
	    do
	    {
		if (*n != *o)
		    *o = *s = *n;

		o++;
		s++;
		n++;
	    } while (n < end);
#endif
	}
	else
	{
	    if (stretch && screen_width != width)
	    {
		register uint32 x_error;
		register uint32 x_fraction;
		uint32 y_error = 0;
		uint32 y_fraction;
		int yy = 0;
		
		x_fraction = (SNES_WIDTH * 0x10000) / width;
		y_fraction = (SNES_HEIGHT_EXTENDED * 0x10000) / height;
		
		for (int y = 0; y < height; y++)
		{
		    register uint8 *d = (uint8 *) vga_getgraphmem () + y * screen_pitch;
		    register uint8 *s = GFX.Screen + yy * GFX.Pitch;
		    y_error += y_fraction;
		    while (y_error >= 0x10000)
		    {
			yy++;
			y_error -= 0x10000;
		    }
		    x_error = 0;
		    for (register int x = 0; x < width; x++)
		    {
			*d++ = *s;
			x_error += x_fraction;

			while (x_error >= 0x10000)
			{
			    s++;
			    x_error -= 0x10000;
			}
		    }
		}
	    }
	    else
	    {
#if 0
		uint8 *s = GFX.Screen + GFX.Pitch * y_buff;
		uint8 *p = vga_getgraphmem () + screen_pitch * y_start +
			  x_start;
		for (int y = y_start; y < y_end; y++, s += GFX.Pitch, p += screen_pitch)
		    memcpy (p, s, width);
#else
		gl_putbox (0, 0, width * 2, height * 2, GFX.Screen);
#endif
	    }
	}
    }
}

const char *S9xChooseMovieFilename(bool8 read_only)
{
    char def [PATH_MAX + 1];
    char title [PATH_MAX + 1];
    char drive [_MAX_DRIVE + 1];
    char dir [_MAX_DIR + 1];
    char ext [_MAX_EXT + 1];

    _splitpath (Memory.ROMFilename, drive, dir, def, ext);
    strcat (def, ".smv");
    sprintf (title, "Choose movie %s filename", read_only ? "playback" : "record");
    const char *filename;

    S9xSetSoundMute (TRUE);
    filename = S9xSelectFilename (def, S9xGetSnapshotDirectory (), "smv", title);
    S9xSetSoundMute (FALSE);
    return (filename);
}

const char *S9xSelectFilename (const char *def, const char *dir1,
			    const char *ext1, const char *title)
{
    static char path [PATH_MAX];
    char buffer [PATH_MAX];
    
    S9xTextMode ();
    printf ("\n%s (default: %s): ", title, def);
    fflush (stdout);
    if (fgets (buffer, sizeof (buffer) - 1, stdin))
    {
	char *p = buffer;
	while (isspace (*p) || *p == '\n')
	    p++;
	if (!*p)
	{
	    strcpy (buffer, def);
	    p = buffer;
	}

	char *q = strrchr (p, '\n');
	if (q)
	    *q = 0;

	char fname [PATH_MAX];
	char drive [_MAX_DRIVE];
	char dir [_MAX_DIR];
	char ext [_MAX_EXT];

	_splitpath (p, drive, dir, fname, ext);
	_makepath (path, drive, *dir ? dir : dir1, fname, *ext ? ext : ext1);
	S9xGraphicsMode ();
	return (path);
    }
    S9xGraphicsMode ();
    return (NULL);
}

void outReg(Register r)
{
    switch (r.port)
    {
	/* First handle special cases: */

	case ATTRCON_ADDR:
	    /* reset read/write flip-flop */
	    inb (STATUS_ADDR);
	    /* ensure VGA output is enabled */
	    outb (r.index | 0x20, ATTRCON_ADDR);
	    outb (r.value, ATTRCON_ADDR);
	    break;

	case MISC_ADDR:
	case VGAENABLE_ADDR:
	    /*	directly to the port */
	    outb (r.value, r.port);
	    break;

	case SEQ_ADDR:
	case GRACON_ADDR:
	case CRTC_ADDR:
	default:
	    /*	index to port			   */
	    outb (r.index, r.port);
	    /*	value to port+1 		   */
	    outb (r.value, r.port + 1);
	    break;
    }
}

/*
    readyVgaRegs() does the initialization to make the VGA ready to
    accept any combination of configuration register settings.

    This involves enabling writes to index 0 to 7 of the CRT controller
    (port 0x3d4), by clearing the most significant bit (bit 7) of index
    0x11.
*/

void readyVgaRegs (void)
{
    int v;

    outb (0x11, 0x3d4);
    v = inb (0x3d5) & 0x7f;
    outb (0x11, 0x3d4);
    outb (v, 0x3d5);
}
/*
	outRegArray sets n registers according to the array pointed to by r.
	First, indexes 0-7 of the CRT controller are enabled for writing.
*/

void outRegArray (Register *r, int n)
{
    readyVgaRegs ();
    while (n--)
	outReg (*r++);
}

void S9xExtraDisplayUsage ()
{
    S9xMessage(S9X_INFO, S9X_USAGE, "-mode or -m <num>               Use mode <num>");
    S9xMessage(S9X_INFO, S9X_USAGE, "\
                                  0 - 320x240 (modex, slower)\n\
                                  1 - 320x200 (faster but clipped)\n\
                                  2 - 256x256 (faster but non-standard)\n\
                                  3 - 640x480\n\
                                  4 - 800x600");
    S9xMessage(S9X_INFO, S9X_USAGE, "-scale or -sc                   Scale image to screen");
    S9xMessage(S9X_INFO, S9X_USAGE, "-interpolation or -y            Enable image interpolation");
    S9xMessage(S9X_INFO, S9X_USAGE, "");
}

void S9xParseDisplayArg (char **argv, int &ind, int)
{
    if ((strcmp (argv [ind], "-m") == 0 ||
	 strcasecmp (argv [ind], "-mode") == 0) && argv [ind + 1])
    {
	mode = atoi (argv [++ind]);
	if (mode >= (int) (sizeof (modes) / sizeof (modes [0])))
	    mode = 0;
    }
    else
    if (strcasecmp (argv [ind], "-scale") == 0 ||
	strcasecmp (argv [ind], "-sc") == 0)
	stretch = TRUE;
    else
    if (strcasecmp (argv [ind], "-y") == 0 ||
	strcasecmp (argv [ind], "-interpolation") == 0)
    {
	interpolation = TRUE;
	Settings.SixteenBit = TRUE;
	Settings.SupportHiRes = TRUE;
	Settings.Transparency = TRUE;
    }
    else
	S9xUsage ();
}

void S9xMessage (int /* type */, int /* number */, const char *message)
{
    fprintf (stderr, "%s\n", message);
}


bool S9xDisplayPollButton(uint32 id, bool *pressed){
    return false;
}
bool S9xDisplayPollAxis(uint32 id, int16 *value){
    return false;
}
bool S9xDisplayPollPointer(uint32 id, int16 *x, int16 *y){
    return false;
}


s9xcommand_t S9xGetDisplayCommandT(const char *n){
    s9xcommand_t cmd;
    cmd.type=S9xBadMapping;
    cmd.multi_press=0;
    cmd.button_norpt=0;
//    if(!strcmp(n, "")){
//        cmd.type=S9xButtonPort;
//        cmd.port[0]=0;
//    }
    return cmd;
}

char *S9xGetDisplayCommandName(s9xcommand_t cmd){
    return strdup("None");
}

void S9xHandleDisplayCommand(s9xcommand_t cmd, int16 data1, int16 data2){
    if(cmd.type==S9xButtonPort){
        if(data1){
            switch(cmd.port[0]){
            }
        }
    }
}

#include "unix/svga_keynames.h"

static uint8 StringToScancode(const char *s){
    int i;
    if(s[0]=='K' && isdigit(s[1]) && isdigit(s[2]) && isdigit(s[3]) && s[4]=='\0'){
        i=(s[1]-'0')*100+(s[2]-'0')*10+s[3]-'0';
        if(i<128) return i;
    }
    for(i=0; i<128; i++){
        if(svga_keynames[i]==NULL) continue;
        if(!strcasecmp(svga_keynames[i], s)) return i;
    }
    return 255;
}

static char *ScancodeToString(uint8 scancode){
    if(scancode>127) return NULL;
    if(svga_keynames[scancode]!=NULL) return strdup(svga_keynames[scancode]);
    char buf[5];
    sprintf(buf, "K%03u", scancode);
    return strdup(buf);
}

bool8 S9xMapDisplayInput(const char *n, s9xcommand_t *cmd){
    int i, j, d;
    char *c;

    if(!isdigit(n[1]) || !isdigit(n[2]) || n[3]!=':') goto unrecog;
    d=((n[1]-'0')*10+(n[2]-'0'))<<24;
    
    switch(n[0]){
      case 'K':
        {
            d|=0x00000000;
            for(i=4; n[i]!='\0' && n[i]!='+'; i++);
            if(n[i]=='\0' || i==4){
                i=4;
            } else {
                for(i=4; n[i]!='+'; i++){
                    switch(n[i]){
                      case 'S': d|=1<<8; break;
                      case 'C': d|=2<<8; break;
                      case 'A': d|=8<<8; break;
                      case 'M': d|=64<<8; break;
                      default: goto unrecog;
                    }
                }
                i++;
            }
            if((i=StringToScancode(n+i))==255) goto unrecog;
            d|=i&0xff;
        }
        return S9xMapButton(d, *cmd, false);

      default:
        break;
    }
unrecog:
    char *err=new char [strlen(n)+34];
    sprintf(err, "Unrecognized input device name '%s'", n);
    S9xMessage(S9X_CONFIG_INFO, S9X_ERROR, err);
    delete [] err;
    return false;
}

#include "conffile.h"
typedef std::pair<std::string,std::string> strpair_t;
extern std::vector<strpair_t> keymaps;

char *S9xParseDisplayConfig(ConfigFile &conf, int pass){
    if(pass!=1) return "Unix/SVGA";
    if(!conf.GetBool("Controls::ClearAll", false)){
        keymaps.push_back(strpair_t("K00:K", "Joypad1 Right"));
        keymaps.push_back(strpair_t("K00:CursorBlockRight", "Joypad1 Right"));
        keymaps.push_back(strpair_t("K00:H", "Joypad1 Left"));
        keymaps.push_back(strpair_t("K00:CursorBlockLeft", "Joypad1 Left"));
        keymaps.push_back(strpair_t("K00:J", "Joypad1 Down"));
        keymaps.push_back(strpair_t("K00:N", "Joypad1 Down"));
        keymaps.push_back(strpair_t("K00:CursorBlockDown", "Joypad1 Down"));
        keymaps.push_back(strpair_t("K00:U", "Joypad1 Up"));
        keymaps.push_back(strpair_t("K00:CursorBlockUp", "Joypad1 Up"));
        keymaps.push_back(strpair_t("K00:Enter", "Joypad1 Start"));
        keymaps.push_back(strpair_t("K00:Space", "Joypad1 Select"));
        keymaps.push_back(strpair_t("K00:S+D", "Joypad1 ToggleTurbo A"));
        keymaps.push_back(strpair_t("K00:C+D", "Joypad1 ToggleSticky A"));
        keymaps.push_back(strpair_t("K00:D", "Joypad1 A"));
        keymaps.push_back(strpair_t("K00:S+C", "Joypad1 ToggleTurbo B"));
        keymaps.push_back(strpair_t("K00:C+C", "Joypad1 ToggleSticky B"));
        keymaps.push_back(strpair_t("K00:C", "Joypad1 B"));
        keymaps.push_back(strpair_t("K00:S+S", "Joypad1 ToggleTurbo X"));
        keymaps.push_back(strpair_t("K00:C+S", "Joypad1 ToggleSticky X"));
        keymaps.push_back(strpair_t("K00:S", "Joypad1 X"));
        keymaps.push_back(strpair_t("K00:S+X", "Joypad1 ToggleTurbo Y"));
        keymaps.push_back(strpair_t("K00:C+X", "Joypad1 ToggleSticky Y"));
        keymaps.push_back(strpair_t("K00:X", "Joypad1 Y"));
        keymaps.push_back(strpair_t("K00:S+A", "Joypad1 ToggleTurbo L"));
        keymaps.push_back(strpair_t("K00:S+V", "Joypad1 ToggleTurbo L"));
        keymaps.push_back(strpair_t("K00:C+A", "Joypad1 ToggleSticky L"));
        keymaps.push_back(strpair_t("K00:C+V", "Joypad1 ToggleSticky L"));
        keymaps.push_back(strpair_t("K00:A", "Joypad1 L"));
        keymaps.push_back(strpair_t("K00:V", "Joypad1 L"));
        keymaps.push_back(strpair_t("K00:S+Z", "Joypad1 ToggleTurbo R"));
        keymaps.push_back(strpair_t("K00:C+Z", "Joypad1 ToggleSticky R"));
        keymaps.push_back(strpair_t("K00:Z", "Joypad1 R"));

        keymaps.push_back(strpair_t("K00:CursorLeft", "Joypad2 Left"));
        keymaps.push_back(strpair_t("K00:CursorRight", "Joypad2 Right"));
        keymaps.push_back(strpair_t("K00:CursorUp", "Joypad2 Up"));
        keymaps.push_back(strpair_t("K00:CursorDown", "Joypad2 Down"));
        keymaps.push_back(strpair_t("K00:KeyPadEnter", "Joypad2 Start"));
        keymaps.push_back(strpair_t("K00:KeyPadPlus", "Joypad2 Select"));
        keymaps.push_back(strpair_t("K00:PageUp", "Joypad2 A"));
        keymaps.push_back(strpair_t("K00:PageDown", "Joypad2 B"));
        keymaps.push_back(strpair_t("K00:Home", "Joypad2 X"));
        keymaps.push_back(strpair_t("K00:End", "Joypad2 Y"));
        keymaps.push_back(strpair_t("K00:Insert", "Joypad2 L"));
        keymaps.push_back(strpair_t("K00:Remove", "Joypad2 R"));

        keymaps.push_back(strpair_t("K00:A+Escape", "Debugger"));
        keymaps.push_back(strpair_t("K00:CS+Escape", "Reset"));
        keymaps.push_back(strpair_t("K00:S+Escape", "SoftReset"));
        keymaps.push_back(strpair_t("K00:Escape", "ExitEmu"));
        keymaps.push_back(strpair_t("K00:Tab", "EmuTurbo"));
        keymaps.push_back(strpair_t("K00:S+Tab", "ToggleEmuTurbo"));
        keymaps.push_back(strpair_t("K00:ScrollLock", "Pause"));
        keymaps.push_back(strpair_t("K00:Break", "Pause"));
        keymaps.push_back(strpair_t("K00:Break_alternative", "Pause"));

        keymaps.push_back(strpair_t("K00:A+Printscreen", "DumpSPC7110Log"));
        keymaps.push_back(strpair_t("K00:A+Break", "DumpSPC7110Log"));
        keymaps.push_back(strpair_t("K00:A+Break_alternative", "DumpSPC7110Log"));
        keymaps.push_back(strpair_t("K00:S+1", "BeginRecordingMovie"));
        keymaps.push_back(strpair_t("K00:S+2", "EndRecordingMovie"));
        keymaps.push_back(strpair_t("K00:S+3", "LoadMovie"));

        keymaps.push_back(strpair_t("K00:A+F1", "SaveSPC"));
        keymaps.push_back(strpair_t("K00:C+F1", "SaveSPC"));
        keymaps.push_back(strpair_t("K00:A+F2", "LoadFreezeFile"));
        keymaps.push_back(strpair_t("K00:C+F2", "LoadFreezeFile"));
        keymaps.push_back(strpair_t("K00:A+F3", "SaveFreezeFile"));
        keymaps.push_back(strpair_t("K00:C+F3", "SaveFreezeFile"));
        keymaps.push_back(strpair_t("K00:F1", "QuickLoad000"));
        keymaps.push_back(strpair_t("K00:F2", "QuickLoad001"));
        keymaps.push_back(strpair_t("K00:F3", "QuickLoad002"));
        keymaps.push_back(strpair_t("K00:F4", "QuickLoad003"));
        keymaps.push_back(strpair_t("K00:F5", "QuickLoad004"));
        keymaps.push_back(strpair_t("K00:F6", "QuickLoad005"));
        keymaps.push_back(strpair_t("K00:F7", "QuickLoad006"));
        keymaps.push_back(strpair_t("K00:F8", "QuickLoad007"));
        keymaps.push_back(strpair_t("K00:F9", "QuickLoad008"));
        keymaps.push_back(strpair_t("K00:F10", "LoadOopsFile"));
        keymaps.push_back(strpair_t("K00:F11", "LoadFreezeFile"));
        keymaps.push_back(strpair_t("K00:F12", "SaveFreezeFile"));
        keymaps.push_back(strpair_t("K00:S+F1", "QuickSave000"));
        keymaps.push_back(strpair_t("K00:S+F2", "QuickSave001"));
        keymaps.push_back(strpair_t("K00:S+F3", "QuickSave002"));
        keymaps.push_back(strpair_t("K00:S+F4", "QuickSave003"));
        keymaps.push_back(strpair_t("K00:S+F5", "QuickSave004"));
        keymaps.push_back(strpair_t("K00:S+F6", "QuickSave005"));
        keymaps.push_back(strpair_t("K00:S+F7", "QuickSave006"));
        keymaps.push_back(strpair_t("K00:S+F8", "QuickSave007"));
        keymaps.push_back(strpair_t("K00:S+F9", "QuickSave008"));

        keymaps.push_back(strpair_t("K00:0", "ToggleHDMA"));
        keymaps.push_back(strpair_t("K00:1", "ToggleBG0"));
        keymaps.push_back(strpair_t("K00:2", "ToggleBG1"));
        keymaps.push_back(strpair_t("K00:3", "ToggleBG2"));
        keymaps.push_back(strpair_t("K00:4", "ToggleBG3"));
        keymaps.push_back(strpair_t("K00:5", "ToggleSprites"));
        keymaps.push_back(strpair_t("K00:6", "SwapJoypads"));
        keymaps.push_back(strpair_t("K00:8", "BGLayeringHack"));
        keymaps.push_back(strpair_t("K00:S+9", "Mode7Interpolate"));
        keymaps.push_back(strpair_t("K00:9", "ToggleTransparency"));
        keymaps.push_back(strpair_t("K00:Minus", "DecFrameRate"));
        keymaps.push_back(strpair_t("K00:S+Minus", "DecFrameTime"));
        keymaps.push_back(strpair_t("K00:C+Minus", "DecTurboSpeed"));
        keymaps.push_back(strpair_t("K00:A+Minus", "DecEmuTurbo"));
        keymaps.push_back(strpair_t("K00:Equal", "IncFrameRate"));
        keymaps.push_back(strpair_t("K00:S+Equal", "IncFrameTime"));
        keymaps.push_back(strpair_t("K00:C+Equal", "IncTurboSpeed"));
        keymaps.push_back(strpair_t("K00:A+Equal", "IncEmuTurbo"));
        keymaps.push_back(strpair_t("K00:BackSpace", "ClipWindows"));
        keymaps.push_back(strpair_t("K00:Printscreen", "Screenshot"));

        keymaps.push_back(strpair_t("K00:Bracket_left", "InterpolateSound"));
        keymaps.push_back(strpair_t("K00:Bracket_right", "SynchronizeSound"));
        keymaps.push_back(strpair_t("K00:A+F4", "SoundChannel0"));
        keymaps.push_back(strpair_t("K00:C+F4", "SoundChannel0"));
        keymaps.push_back(strpair_t("K00:A+F5", "SoundChannel1"));
        keymaps.push_back(strpair_t("K00:C+F5", "SoundChannel1"));
        keymaps.push_back(strpair_t("K00:A+F6", "SoundChannel2"));
        keymaps.push_back(strpair_t("K00:C+F6", "SoundChannel2"));
        keymaps.push_back(strpair_t("K00:A+F7", "SoundChannel3"));
        keymaps.push_back(strpair_t("K00:C+F7", "SoundChannel3"));
        keymaps.push_back(strpair_t("K00:A+F8", "SoundChannel4"));
        keymaps.push_back(strpair_t("K00:C+F8", "SoundChannel4"));
        keymaps.push_back(strpair_t("K00:A+F9", "SoundChannel5"));
        keymaps.push_back(strpair_t("K00:C+F9", "SoundChannel5"));
        keymaps.push_back(strpair_t("K00:A+F10", "SoundChannel6"));
        keymaps.push_back(strpair_t("K00:C+F10", "SoundChannel6"));
        keymaps.push_back(strpair_t("K00:A+F11", "SoundChannel7"));
        keymaps.push_back(strpair_t("K00:C+F11", "SoundChannel7"));
        keymaps.push_back(strpair_t("K00:A+F12", "SoundChannelsOn"));
        keymaps.push_back(strpair_t("K00:C+F12", "SoundChannelsOn"));

//        keymaps.push_back(strpair_t("M00:B0", "{Mouse1 L,Superscope Fire,Justifier1 Trigger}"));
//        keymaps.push_back(strpair_t("M00:B1", "{Justifier1 AimOffscreen Trigger,Superscope AimOffscreen}"));
//        keymaps.push_back(strpair_t("M00:B2", "{Mouse1 R,Superscope Cursor,Justifier1 Start}"));
//        keymaps.push_back(strpair_t("M00:Pointer", "Pointer C=1 White/Black Mouse1+Superscope+Justifier1"));
//        keymaps.push_back(strpair_t("K00:grave", "Superscope ToggleTurbo"));
//        keymaps.push_back(strpair_t("K00:slash", "Superscope Pause"));
    }

    mode = conf.GetUInt("Unix/SVGA::Mode", 0);
    if (mode >= (int) (sizeof (modes) / sizeof (modes [0])))
        mode = 0;
    stretch = conf.GetBool("Unix/SVGA::Scale", false);
    if((interpolation = conf.GetBool("Unix/SVGA::Interpolation", false))){
        Settings.SixteenBit = TRUE;
        Settings.SupportHiRes = TRUE;
        Settings.Transparency = TRUE;
    }

    return "Unix/SVGA";
}

#endif

