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

/*
   XF86 code (xf86.cpp, this file) (c) Copyright 1998 Maciej Babinski,
		use restricted to above terms.
   Question/comments regarding this file only: maciej@imsa.edu
 */

#ifdef __linux
//#define USE_XF86VIDMODE

#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/ioctl.h>

#include <fcntl.h>
#include <sys/types.h>

#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include "snes9x.h"
#include "memmap.h"
#include "debug.h"
#include "ppu.h"
#include "snapshot.h"
#include "gfx.h"
#include "display.h"
#include "apu.h"
#include <asm/io.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "snes9x.h"
#include "memmap.h"
#include "debug.h"
#include "ppu.h"
#include "snapshot.h"
#include "gfx.h"
#include "display.h"
#include "apu.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <X11/extensions/xf86dga.h>

#ifdef USE_XF86VIDMODE

/* GH: Added to allow to be compiled using C++ compiler */
#if defined (__cplusplus) || defined (c_plusplus)
#define private c_private
#endif
#   include <X11/extensions/xf86vmode.h>

static XF86VidModeModeInfo	*orig_mode = NULL;
static XF86VidModeModeInfo	*mod320x240 = NULL;
bool cmpModeInfo (XF86VidModeModeInfo, XF86VidModeModeInfo);
XF86VidModeModeInfo *ModeLine2ModeInfo (XF86VidModeModeLine, int);
#endif


void quit (int sig)
{
	printf ("Received signal %d, quitting\n", sig);
	S9xExit ();
}


struct vidinfo {
	int bitdepth;
	int screendepth;
	int width;
	int height;
	int banksize;
	int memsize;
	char *vidMemBegin;
	char *scrnBegin;
};
struct vidinfo	ourvideo;

static Display		*ourdisp = NULL;
/*
   These three vars are "constants" for a particular display, who's value we
   calculate in S9xInitDisplay, so that we don't have to do it on the fly in
   S9xPutImage.
	mEtoE = distance between end of once line of image and beginning of
	        next one (units differ between bit-depths).
*/
static int		mWide, mHigh, mEtoE; 

static Colormap		cmap;
static XColor		colors[256];
static int		ourscreen;
static bool		is_graphics=FALSE;
static Window		inputwin;
static Window		rootWindow;
static int		is32or24 = 32;

extern int		joypads[5];


void S9xInitDisplay (int argc, char **argv)
{
	Screen			*scrn;
	XSetWindowAttributes	xattr;

	signal (SIGINT, quit);
	signal (SIGTERM, quit);

	if ((ourdisp = XOpenDisplay(NULL)) == NULL) {
		printf ("Can't connect to X server!\n");
		S9xExit ();
	}


	ourscreen = DefaultScreen (ourdisp);
	scrn = DefaultScreenOfDisplay (ourdisp);
	ourvideo.bitdepth = DefaultDepth (ourdisp, ourscreen);
	ourvideo.screendepth = ourvideo.bitdepth / 8;
	rootWindow = RootWindowOfScreen (scrn);

	xattr.override_redirect = True;
	inputwin = XCreateWindow (ourdisp, RootWindowOfScreen(scrn),
	                          10, 10, 100, 100, 0, 0, InputOutput,
	                          DefaultVisualOfScreen(scrn),
	                          CWOverrideRedirect, &xattr);
	XMapWindow (ourdisp, inputwin);

	ourvideo.height = HeightOfScreen (scrn);



	switch (ourvideo.bitdepth) {
		case  8:
			Settings.SixteenBit = FALSE;
			Settings.Transparency = FALSE;
			cmap = XCreateColormap (ourdisp, rootWindow,
			                        DefaultVisualOfScreen (scrn),
						AllocAll);
			XSetWindowColormap (ourdisp, inputwin, cmap);
			for (int i=0; i<256; i++) {
				colors[i].pixel = i;
				colors[i].flags = DoRed | DoGreen | DoBlue;
			}
			break;
		case 16:
			Settings.SixteenBit = TRUE;
			if (!Settings.ForceNoTransparency)
				Settings.Transparency = TRUE;
			break;
		case 24:
			if (is32or24 == 32) {
				ourvideo.bitdepth = 32;
				ourvideo.screendepth = 4;
			}
			Settings.SixteenBit = TRUE;
			if (!Settings.ForceNoTransparency)
				Settings.Transparency = TRUE;
			break;
		default:
			printf ("Color depth %d not supported!\n");
			S9xExit ();
			break;
	}
	printf ("Found %d bit display\n", ourvideo.bitdepth);

	XFree(scrn);

	XF86DGAGetVideo (ourdisp, ourscreen, &(ourvideo.vidMemBegin),
          	       &(ourvideo.width), &(ourvideo.banksize),
          	       &(ourvideo.memsize));

#ifdef USE_XF86VIDMODE
   {
	XF86VidModeModeInfo	**all_modes;
	int			mode_count;
	XF86VidModeModeLine	mod_tmp;
	int			dotclock_tmp;
	int			x;
	
	XF86VidModeGetModeLine (ourdisp, ourscreen, &dotclock_tmp, &mod_tmp);

	orig_mode = ModeLine2ModeInfo (mod_tmp, dotclock_tmp);
	if (orig_mode->hdisplay == 320 && orig_mode->vdisplay == 240) {
		mod320x240 = orig_mode;
	} else {
		XF86VidModeGetAllModeLines (ourdisp,ourscreen, &mode_count,
		                            &all_modes);
		for (x = 0; x < mode_count; x++)
			if (all_modes[x]->hdisplay == 320 &&
			    all_modes[x]->vdisplay == 240) {
				mod320x240 = (XF86VidModeModeInfo *) malloc (
				               sizeof(XF86VidModeModeInfo));
				*mod320x240 = *(all_modes[x]);
				break;
			} else
				XFree (all_modes[x]->c_private);

		if (mod320x240 == NULL) {
			printf ("No 320x240 mode available!\n");
			S9xExit ();
		}

	}
	XFree(all_modes);

   }
#endif

	GFX.Pitch = IMAGE_WIDTH * (Settings.SixteenBit ? 2 : 1);
	GFX.Screen = (uint8 *) malloc (IMAGE_HEIGHT * GFX.Pitch);

	if (Settings.Transparency)
		GFX.SubScreen = (uint8 *) malloc (IMAGE_HEIGHT * GFX.Pitch);

	switch (ourvideo.bitdepth) {
		case  8:
			mWide = IMAGE_WIDTH / 4;
			mHigh = IMAGE_HEIGHT;
			mEtoE = (ourvideo.width - IMAGE_WIDTH) /4;
			break;
		case 16:
			mWide = IMAGE_WIDTH / 2;
			mHigh = IMAGE_HEIGHT;
			mEtoE = (ourvideo.width - IMAGE_WIDTH) / 2;
			break;
		case 24:
			mWide = IMAGE_WIDTH;
			mHigh = IMAGE_HEIGHT;
			mEtoE = (ourvideo.width - IMAGE_WIDTH) * 3;
			break;
		case 32:
			mWide = IMAGE_WIDTH;
			mHigh = IMAGE_HEIGHT;
			mEtoE = (ourvideo.width - IMAGE_WIDTH) * 4;
		default:
			break;
	}
	ourvideo.scrnBegin = ourvideo.vidMemBegin + (320 - IMAGE_WIDTH) * ourvideo.screendepth / 2 + ourvideo.width * ourvideo.screendepth * 8;
}

void S9xGraphicsMode ()
{
	if (is_graphics)
		return;

	XSelectInput (ourdisp, inputwin, KeyPressMask | KeyReleaseMask);
	XSetInputFocus (ourdisp, inputwin, RevertToNone, CurrentTime);
	XGrabKeyboard (ourdisp, inputwin, True, GrabModeAsync, GrabModeAsync,
	               CurrentTime);
	XGrabPointer (ourdisp, inputwin, True, 0, 
	              GrabModeAsync, GrabModeAsync, inputwin, None, CurrentTime);
	
	XFlush(ourdisp);

	XAutoRepeatOff (ourdisp);

#ifdef USE_XF86VIDMODE
	if (orig_mode != mod320x240) {
		XF86VidModeSwitchToMode (ourdisp,ourscreen, mod320x240);
	}
	XFlush (ourdisp);
	XF86VidModeSetViewPort (ourdisp, ourscreen, 0, 0);
	XFlush (ourdisp);
#endif
	
	XF86DGADirectVideo (ourdisp, ourscreen, XF86DGADirectGraphics);
	memset ((void *) ourvideo.vidMemBegin, 0, ourvideo.width * 240 *
	        ourvideo.screendepth);
	is_graphics = TRUE;
	XFlush (ourdisp);
}

void S9xTextMode () 
{
	if (!is_graphics)
		return;

	if (ourdisp == NULL)
		return;
	

	XF86DGADirectVideo (ourdisp, ourscreen, 0);
#ifdef USE_XF86VIDMODE
	if (orig_mode != mod320x240) {
		XF86VidModeSwitchToMode (ourdisp, ourscreen, orig_mode);
	}
#endif
	XAutoRepeatOn (ourdisp);

	XUngrabKeyboard (ourdisp, CurrentTime);
	XUngrabPointer (ourdisp, CurrentTime);
	XSelectInput (ourdisp, inputwin, 0);
	XSync (ourdisp, True);

	is_graphics = FALSE;
} 

void S9xDeinitDisplay ()
{
	S9xTextMode ();

	if (ourdisp == NULL)
		return;


	XDestroyWindow (ourdisp, inputwin);

	free (GFX.Screen);
	free (GFX.SubScreen);
#ifdef USE_XF86VIDMODE
	free (orig_mode);
	free (mod320x240);
	if (orig_mode->privsize)
		XFree (orig_mode->c_private);
	if (mod320x240->privsize)
		XFree (orig_mode->c_private);
#endif
	XFree (ourdisp);
}

void S9xSetPalette () 
{
	int		Brightness;
	
	if (ourvideo.bitdepth != 8)
		return;

	Brightness = IPPU.MaxBrightness *138;

	for (int i = 0; i < 256; i++) {
		colors[i].red = ((PPU.CGDATA[i] >> 0) & 0x1F) * Brightness;
		colors[i].green = ((PPU.CGDATA[i] >> 5) & 0x1F) * Brightness;
		colors[i].blue = ((PPU.CGDATA[i] >> 10) & 0x1F) * Brightness;
	}

	XStoreColors (ourdisp, cmap, colors, 256);
	XF86DGAInstallColormap (ourdisp, ourscreen, cmap);
}

#define RED(r)		((uint8) (((r >> 11) & 31) * 256 / 32))
#define GREEN(g)	((uint8) (((g >> 5) & 63) * 256 / 64))
#define BLUE(b)		((uint8) (((b & 31)) * 256 / 32))

void S9xPutImage (int width, int height)
{
	register int x,y;
	

	switch (ourvideo.bitdepth) {
		case  8:
		case 16:
			{
			long int	*p, *q;
			p = (long int *) GFX.Screen;
			q = (long int *) ourvideo.scrnBegin;

			for (y=0;y<mHigh;y++) {
				for (x=0;x<mWide;x++) {
					*q = *p;
					q++, p++;
					}
				q += mEtoE;
				}
			}
			break;
/*
		case 24:
			{
			uint16		*p;
			uint8	  	*q;

			p = (uint16 *) GFX.Screen;
			q = (uint8 *) ourvideo.scrnBegin;

			for (y=0;y<mHigh;y++) {
				for (x=0;x<mWide;x++) {
					*q = BLUE(*p);
					*(q+1) = GREEN(*p);
					*(q+2) = RED(*p);
					q+=3, p++;
					}
				q += mEtoE;
				}
			}
			break;
*/
		case 24:
			{
			uint16		*p;
			uint8	  	*q;

			p = (uint16 *) GFX.Screen;
			q = (uint8 *) ourvideo.scrnBegin;

			for (y=0;y<mHigh;y++) {
				for (x=0;x<mWide;x++) {
					*q = BLUE(*p);
					*(q+1) = GREEN(*p);
					*(q+2) = RED(*p);
					q+=3, p++;
					}
				q += mEtoE;
				}
			}
			break;
		case 32:
			{
			uint16		*p;
			uint8	  	*q;

			p = (uint16 *) GFX.Screen;
			q = (uint8 *) ourvideo.scrnBegin;

			for (y=0;y<mHigh;y++) {
				for (x=0;x<mWide;x++) {
					*q = BLUE(*p);
					*(q+1) = GREEN(*p);
					*(q+2) = RED(*p);
					q+=4, p++;
					}
				q += mEtoE;
				}
			}
		default:
			break;
	}
}


const char *S9xSelectFilename (const char *def, const char *dir,
				const char *ext1, const char *title)
{
	return (NULL);
}


void S9xSetTitle (const char *title)
{
}

void S9xParseDisplayArg (char **argv, int &ind, int)
{
	if (!strncmp (argv[ind], "-32", 3))
		is32or24 = 32;
	else if  (!strncmp (argv[ind], "-24", 3))
		is32or24 = 24;
	else
		S9xUsage ();
}

S9xMinCommandLineArgs ()
{
	return (2);
}

void S9xExtraUsage ()
{
	printf ("\
-24/-32			On 24/32 bit displays specifies which one to use\n\
			(I can't figure out how to tell how deep the screen\n\
			is, only how many bits per color it uses)\n\n\
");
}

bool8 S9xReadMousePosition (int which1_0_to_1, int &x, int &y, uint32 &buttons)
{
	return (FALSE);
}

bool8 S9xReadSuperScopePosition (int &x, int &y, uint32 &buttons)
{
	return (FALSE);
}

void S9xMessage (int type, int number, const char *message)
{
	fprintf (stderr, "%s\n", message);
}

void S9xProcessEvents (bool8 block)
{
	static fd_set			fds;
	static struct timeval	tv = { 0, 0 };
/*
	if (joypads[0] & SNES_TL_MASK && joypads[0] & SNES_TR_MASK)
		S9xExit();
*/

	if (!is_graphics) {
		while (True) {
		char c;
		if (read (0, &c, 1) >= 0)
			if (c == '\n')  {
				S9xGraphicsMode ();
				Settings.Paused ^= 1;
				return;
			}
		}
	}
		
    while (block || XPending (ourdisp))
    {
	XEvent event;
	XNextEvent (ourdisp, &event);
	block = FALSE;

	uint8 byte1 = 0;
	uint8 byte2 = 0;
	uint8 byte3 = 0;
	uint8 byte4 = 0;
	
	switch (event.type)
	{
	case KeyPress:
	case KeyRelease:
	{
	    int key;
	    switch (key = XKeycodeToKeysym (ourdisp, event.xkey.keycode, 0))
	    {
	    case XK_k:
	    case XK_Right:	byte2 = 1;	break;
	    case XK_h:
	    case XK_Left:	byte2 = 2;	break;
	    case XK_j:
	    case XK_n:
	    case XK_Down:	byte2 = 4;	break;
	    case XK_u:
	    case XK_Up:		byte2 = 8;	break;

	    case XK_Return:	byte2 = 16;	break; // Start
	    case XK_space:	byte2 = 32;	break; // Select

	    case XK_period:
	    case XK_t:
	    case XK_d:		byte1 = 128;	break; // A

	    case XK_slash:
	    case XK_y:
	    case XK_c:		byte2 = 128;	break; // B

	    case XK_m:
	    case XK_e:
	    case XK_s:		byte1 = 64;	break; // X

	    case XK_comma:
	    case XK_r:
	    case XK_x:		byte2 = 64;	break; // Y

	    case XK_v:
	    case XK_q:
	    case XK_a:		byte1 = 32;	break; // TL

	    case XK_b:
	    case XK_w:
	    case XK_z:		byte1 = 16;	break; // TR

	    case XK_KP_4:	byte4 = 1;	break;
	    case XK_KP_6:	byte4 = 2;	break;
	    case XK_KP_2:	byte4 = 4;	break;
	    case XK_KP_8:	byte4 = 8;	break;
	    
	    case XK_KP_Enter:	byte4 = 16;	break; // Start
	    case XK_KP_Add:     byte4 = 32;	break; // Select
	    case XK_Prior:	byte3 = 128;	break; // A
	    case XK_Next:	byte4 = 128;	break; // B
	    case XK_Home:	byte3 = 64;	break; // X
	    case XK_End:	byte4 = 64;	break; // Y
	    case XK_Insert:	byte3 = 32;	break; // TL
	    case XK_Delete:	byte3 = 16;	break; // TR

	    case XK_Escape:	S9xExit ();	break;

	    case XK_0:
		if (event.type == KeyPress)
		    Settings.DisableHDMA = !Settings.DisableHDMA;
		break;
	    case XK_1:
		if (event.type == KeyPress)
		    PPU.BG_Forced ^= 1;
		break;
	    case XK_2:
		if (event.type == KeyPress)
		    PPU.BG_Forced ^= 2;
		break;
	    case XK_3:
		if (event.type == KeyPress)
		    PPU.BG_Forced ^= 4;
		break;
	    case XK_4:
		if (event.type == KeyPress)
		    PPU.BG_Forced ^= 8;
		break;
	    case XK_5:
		if (event.type == KeyPress)
		    PPU.BG_Forced ^= 16;
		break;
	    case XK_6:
		if (event.type == KeyPress)
		    Settings.SwapJoypads = !Settings.SwapJoypads;
		break;
	    case XK_9:
		if (event.type == KeyPress)
		    if (Settings.SixteenBit)
			Settings.Transparency = !Settings.Transparency;
		break;
	    case XK_8:
		if (event.type == KeyPress)
		    Settings.BGLayering = !Settings.BGLayering;
		break;
	    case XK_7:
		if (event.type == KeyPress)
		    S9xNextController ();
		break;

	    case XK_minus:
		if (event.type == KeyPress)
		{
		    if (Settings.SkipFrames <= 1)
			Settings.SkipFrames = AUTO_FRAMERATE;
		    else
		    if (Settings.SkipFrames != AUTO_FRAMERATE)
			Settings.SkipFrames--;
		}
		break;

	    case XK_equal:
	    case XK_plus:
		if (event.type == KeyPress)
		{
		    if (Settings.SkipFrames == AUTO_FRAMERATE)
			Settings.SkipFrames = 1;
		    else
		    if (Settings.SkipFrames < 10)
			Settings.SkipFrames++;
		}
		break;

	    case XK_BackSpace:
		if (event.type == KeyPress)
		{
		    Settings.DisableGraphicWindows = !Settings.DisableGraphicWindows;
		}
		break;
	    case XK_Scroll_Lock:
	    case XK_Pause:
	    case XK_Break:
		if (event.type == KeyPress) {
			if (Settings.Paused)	
				S9xGraphicsMode ();
			else
				S9xTextMode ();
		    Settings.Paused ^= 1;
		}
		break;

/*
	    case XK_Tab:
		if (event.type == KeyPress)
		    superscope_turbo = !superscope_turbo;
		break;

	    case XK_grave:
		superscope_pause = event.type == KeyPress;
		break;
*/
	    case XK_F1:
#ifdef DEBUGGER
		if (event.type == KeyPress && (event.xkey.state & Mod1Mask))
		{
		    CPU.Flags |= DEBUG_MODE_FLAG;
		    break;
		}
#endif
		// Fall...
	    case XK_F2:
		if (event.type == KeyPress && (event.xkey.state & Mod1Mask))
		{
		    S9xLoadSnapshot (S9xChooseFilename (TRUE));
		    break;
		}
		// Fall...
	    case XK_F3:
		if (event.type == KeyPress && (event.xkey.state & Mod1Mask))
		{
		    Snapshot (S9xChooseFilename (FALSE));
		    break;
		}
		// Fall...
	    case XK_F4:
	    case XK_F5:
	    case XK_F6:
	    case XK_F7:
	    case XK_F8:
	    case XK_F9:
	    case XK_F10:
	    case XK_F11:
	    case XK_F12:
		if (event.type == KeyPress)
		{
		    if (!(event.xkey.state & (ShiftMask | Mod1Mask)))
		    {
			if (key == XK_F11)
			{
			    S9xLoadSnapshot (S9xChooseFilename (TRUE));
			    break;
			}
			else if (key == XK_F12)
			{
			    Snapshot (S9xChooseFilename (FALSE));
			    break;
			}
			char def [PATH_MAX];
			char filename [PATH_MAX];
			char drive [_MAX_DRIVE];
			char dir [_MAX_DIR];
			char ext [_MAX_EXT];

			_splitpath (Memory.ROMFilename, drive, dir, def, ext);
			sprintf (filename, "%s%s%s.%03d",
				 S9xGetSnapshotDirectory (), SLASH_STR, def,
				 key - XK_F1);
			S9xLoadSnapshot (filename);
		    }
		    else
		    if (event.xkey.state & Mod1Mask)
		    {
			if (key >= XK_F4)
			    S9xToggleSoundChannel (key - XK_F4);
		    }
		    else
		    {
			char def [PATH_MAX];
			char filename [PATH_MAX];
			char drive [_MAX_DRIVE];
			char dir [_MAX_DIR];
			char ext [_MAX_EXT];

			_splitpath (Memory.ROMFilename, drive, dir, def, ext);
			sprintf (filename, "%s%s%s.%03d",
				 S9xGetSnapshotDirectory (), SLASH_STR, def,
				 key - XK_F1);
			Snapshot (filename);
		    }
		}
		break;
	    
	    }
	    if (event.type == KeyPress)
	    {
		joypads [0] |= byte1;
		joypads [0] |= (byte2 << 8);
		joypads [1] |= byte3;
		joypads [1] |= (byte4 << 8);
	    }
	    else
	    {
		joypads [0] &= ~byte1;
		joypads [0] &= ~(byte2 << 8);
		joypads [1] &= ~byte3;
		joypads [1] &= ~(byte4 << 8);
	    }
	    break;
	}
#if 0
	case ButtonPress:
	    mouse_buttons = (event.xbutton.state | (1 << event.xbutton.button)) & 0x1f;
	    break;
	case ButtonRelease:
	    mouse_buttons = (event.xbutton.state & ~(1 << event.xbutton.button)) & 0x1f;
	    break;
#endif
	}
    }

}

#ifdef USE_XF86VIDMODE
XF86VidModeModeInfo *ModeLine2ModeInfo (XF86VidModeModeLine modeline, int clock)
{
	XF86VidModeModeInfo	*tmp;
	tmp = (XF86VidModeModeInfo *) malloc (sizeof(XF86VidModeModeInfo));
	
	tmp->dotclock = clock;
	tmp->hdisplay = modeline.hdisplay;
	tmp->hsyncstart = modeline.hsyncstart;
	tmp->hsyncend = modeline.hsyncend;
	tmp->htotal = modeline.htotal;
	tmp->vdisplay = modeline.vdisplay;
	tmp->vsyncstart = modeline.vsyncstart;
	tmp->vsyncend = modeline.vsyncend;
	tmp->vtotal = modeline.vtotal;
	tmp->flags = modeline.flags;
	tmp->c_private = modeline.c_private;
	return (tmp);
}

bool cmpModeInfo (XF86VidModeModeInfo a, XF86VidModeModeInfo b)
{
	if (!memcmp ((void *) &a, (void *) &b, sizeof (XF86VidModeModeInfo)))
		return (TRUE);
	else 
		return (FALSE);
}


#endif /* USE_XF86VIDMODE */

/* Scammed from x11.cpp */

int select (int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
	    struct timeval *timeval)
{
    int ret = 0;
    if (readfds)
    {
	if (readfds->fds_bits [0])
	{
	    int i = ffs (readfds->fds_bits [0]) - 1;
	    int arg = 0;
	    if (ioctl (i, FIONREAD, &arg) == 0 && arg)
		ret = 1;
	}
	else
	    readfds->fds_bits [0] = 0;
    }
    if (writefds && writefds->fds_bits [0])
	ret = 1;
    return (ret);
}
#endif /* __linux */

