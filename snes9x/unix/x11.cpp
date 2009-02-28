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

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "snes9x.h"
#include "memmap.h"
#include "ppu.h"
#include "gfx.h"
#include "display.h"
#include "soundux.h"
#include "x11.h"
#include "controls.h"
#include "movie.h"

#if 0
#define QT_CLEAN_NAMESPACE
#include <qapplication.h>
#include "snes9x_gui.h"
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

#ifdef USE_GLIDE
bool8 S9xGlideEnable (bool8);
void S9xGlidePutImage (int, int);
void S9xSwitchToGlideMode (bool8);
#endif

#ifdef USE_AIDO
#include "aido.h"
#endif

#ifdef __linux
// Select seems to be broken in 2.x.x kernels - if a signal interrupts a
// select system call with a zero timeout, the select call is restarted but
// with an infinite timeout! The call will block until data arrives on the
// selected fd(s).
//
// The workaround is to stop the X library calling select in the first
// place! Replace XPending - which polls for data from the X server using 
// select - with an ioctl call to poll for data and then only call the blocking
// XNextEvent if data is waiting.

#define SELECT_BROKEN_FOR_SIGNALS
#endif

#ifdef USE_DGA_EXTENSION
#include <X11/extensions/xf86dga.h>

void CreateFullScreenWindow ();
void S9xSwitchToFullScreen (bool8 enable);

#ifdef USE_VIDMODE_EXTENSION
#include <X11/extensions/xf86vmode.h>

#define ALL_DEVICE_EVENTS 0
#endif

typedef struct 
{
    bool8		full_screen_available;
    bool8		is_full_screen;
    bool8		scale;
    char		*vram;
    int			line_width;
    int			bank_size;
    int			size;
    int			window_width;
    int			window_height;
    int			saved_window_width;
    int			saved_window_height;
    bool8		saved_image_needs_scaling;
    Window		fs_window;

#ifdef USE_VIDMODE_EXTENSION
    bool8		switch_video_mode;
    XF86VidModeModeInfo	**all_modes;
    int			num_modes;
    XF86VidModeModeInfo orig;
    XF86VidModeModeInfo *best;
    bool8		no_mode_switch;
    bool8		start_full_screen;
#endif
} XF86Data;

static XF86Data XF86;
#endif

GUIData GUI;

#if 0
QApplication *app;
Snes9xGUI *gui;
#endif

void Scale8 (int width, int height);
void Scale16 (int width, int height);
void Convert8To16 (int width, int height);
void Convert16To8 (int width, int height);
void Convert8To24 (int width, int height);
void Convert8To24Packed (int width, int height);
void Convert16To24 (int width, int height);
void Convert16To24Packed (int width, int height);
void SetupImage ();
int ErrorHandler (Display *, XErrorEvent *);
void TVMode (int width, int height);

void Init_2xSaI (uint32);
void Super2xSaI (uint8 *srcPtr, uint32 srcPitch,
		 uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch,
		 int width, int height);
void SuperEagle (uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, 
		 uint8 *dstPtr, uint32 dstPitch, int width, int height);
void _2xSaI (uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, 
	     uint8 *dstPtr, uint32 dstPitch, int width, int height);
void Scale_2xSaI (uint8 *srcPtr, uint32 srcPitch, uint8 * /* deltaPtr */,
		  uint8 *dstPtr, uint32 dstPitch, 
		  uint32 dstWidth, uint32 dstHeight, int width, int height);

START_EXTERN_C
uint8 snes9x_clear_change_log = 0;
END_EXTERN_C

static inline void SetXRepeat (bool state){
    if(GUI.no_repeat) return;
    if(state) XAutoRepeatOn(GUI.display);
    else XAutoRepeatOff(GUI.display);
}

void S9xDeinitDisplay ()
{
#ifdef USE_GLIDE
    S9xGlideEnable (FALSE);
#elif USE_AIDO
    if (Settings.AIDOShmId)
        return;
#endif

    S9xTextMode ();
    uint32 i;

    for (i = 0; i < sizeof (GUI.to_free) / sizeof (GUI.to_free [0]); i++)
	if (GUI.to_free [i])
	{
	    free (GUI.to_free [i]);
	    GUI.to_free [i] = NULL;
	}

    if (GUI.image)
    {
#ifdef MITSHM
	if (GUI.use_shared_memory)
	{
	    XShmDetach (GUI.display, &GUI.sm_info);
	    GUI.image->data = NULL;
	    XDestroyImage (GUI.image);
	    if (GUI.sm_info.shmaddr)
		shmdt (GUI.sm_info.shmaddr);
	    if (GUI.sm_info.shmid >= 0)
		shmctl (GUI.sm_info.shmid, IPC_RMID, 0);
	    GUI.image = NULL;
        }
	else
#endif
	{
	    XDestroyImage (GUI.image);
	    GUI.image = NULL;
	}
    }
    XSync (GUI.display, False);
    XCloseDisplay (GUI.display);
}

void S9xInitDisplay (int, char **)
{
#ifdef USE_AIDO
    if (Settings.AIDOShmId)
    {
        S9xAIDOInit ();
        return;
    }
#endif

    if (!(GUI.display = XOpenDisplay (NULL)))
    {
	fprintf (stderr, "Failed to connect to X server.\n");
	exit (1);
    }
    GUI.screen = DefaultScreenOfDisplay (GUI.display);
    GUI.screen_num = XScreenNumberOfScreen (GUI.screen);
    GUI.visual = DefaultVisualOfScreen (GUI.screen);
    GUI.window_width = IMAGE_WIDTH;
    GUI.window_height = IMAGE_HEIGHT;

#ifdef USE_OPENGL
    // XXX:
    Settings.OpenGLEnable = TRUE;

    if (Settings.OpenGLEnable)
	S9xOpenGLInit ();
#endif

    XVisualInfo plate;
    XVisualInfo *matches;
    int count;

    plate.visualid = XVisualIDFromVisual (GUI.visual);
    matches = XGetVisualInfo (GUI.display, VisualIDMask, &plate, &count);

    if (!count)
    {
	fprintf (stderr, "Your X Window System server is unwell!\n");
	exit (1);
    }
    GUI.depth = matches[0].depth;
    if ((GUI.depth != 8 && GUI.depth != 15 && GUI.depth != 16 && GUI.depth != 24) ||
	(matches[0].c_class != PseudoColor && matches[0].c_class != TrueColor &&
	 matches[0].c_class != GrayScale))
    {
	fprintf (stderr, "\
Snes9x needs an X Window System server set to 8, 15, 16, 24 or 32-bit colour GUI.depth\n\
supporting PseudoColor, TrueColor or GrayScale.\n");
	exit (1);
    }

    if (GUI.depth >= 15 && !Settings.ForceNoTransparency)
    {
	Settings.Transparency = TRUE;
    }

    GUI.pseudo = matches[0].c_class == PseudoColor ||
	     matches[0].c_class == GrayScale;
    GUI.grayscale = matches[0].c_class == GrayScale;

    if (GUI.depth != 8 || !GUI.pseudo)
    {
	GUI.red_shift = ffs (matches[0].red_mask) - 1;
	GUI.green_shift = ffs (matches[0].green_mask) - 1;
	GUI.blue_shift = ffs (matches[0].blue_mask) - 1;
	GUI.red_size = matches[0].red_mask >> GUI.red_shift;
	GUI.green_size = matches[0].green_mask >> GUI.green_shift;
	GUI.blue_size = matches[0].blue_mask >> GUI.blue_shift;
	if (GUI.depth == 16 && GUI.green_size == 63)
	    GUI.green_shift++;

#ifdef GFX_MULTI_FORMAT
	switch (GUI.depth)
	{
	default:
	case 24:
	case 8:
	    S9xSetRenderPixelFormat (RGB565);
	    Init_2xSaI (565);
	    break;
	case 16:
	    if (GUI.red_size != GUI.green_size || GUI.blue_size != GUI.green_size)
	    {
		// 565 format
		if (GUI.green_shift > GUI.blue_shift && GUI.green_shift > GUI.red_shift)
		    S9xSetRenderPixelFormat (GBR565);
		else
		if (GUI.red_shift > GUI.blue_shift)
		    S9xSetRenderPixelFormat (RGB565);
		else
		    S9xSetRenderPixelFormat (BGR565);

		Init_2xSaI (565);
		break;
	    }
	    /* FALL ... */
	case 15:
	    if (GUI.green_shift > GUI.blue_shift && GUI.green_shift > GUI.red_shift)
		S9xSetRenderPixelFormat (GBR555);
	    else
	    if (GUI.red_shift > GUI.blue_shift)
		S9xSetRenderPixelFormat (RGB555);
	    else
		S9xSetRenderPixelFormat (BGR555);

	    Init_2xSaI (555);
	    break;
	}
#endif	
    }
    XFree ((char *) matches);

#if defined(USE_OPENGL) || defined(USE_AIDO)
    if (Settings.OpenGLEnable || Settings.AIDOShmId)
    {
	S9xSetRenderPixelFormat (RGB555);
        Settings.Transparency = TRUE;
    }
#endif

    int l = 0;
    int i;

    for (i = 0; i < 6; i++)
    {
	int r = (i * 31) / (6 - 1);
	for (int j = 0; j < 6; j++)
	{
	    int g = (j * 31) / (6 - 1);
	    for (int k = 0; k < 6; k++)
	    { 
		int b = (k * 31) / (6 - 1);

		GUI.fixed_colours [l].red = r;
		GUI.fixed_colours [l].green = g;
		GUI.fixed_colours [l++].blue = b;
	    }
	}
    }

    int *color_diff = new int [0x10000];
    int diffr, diffg, diffb, maxdiff = 0, won = 0, lost;
    int r, d = 8;
    for (r = 0; r <= (int) MAX_RED; r++)
    {
	int cr, g, q;
      
	int k = 6 - 1;
	cr = (r * k) / MAX_RED;
	q  = (r * k) % MAX_RED;
	if (q > d && cr < k) 
	    cr++;
	diffr = abs (cr * k - r);
	for (g = 0; g <= (int) MAX_GREEN; g++)
	{
	    int cg, b;
	  
	    k  = 6 - 1;
	    cg = (g * k) / MAX_GREEN;
	    q  = (g * k) % MAX_GREEN;
	    if(q > d && cg < k)
		cg++;
	    diffg = abs (cg * k - g);
	    for (b = 0; b <= (int) MAX_BLUE; b++) 
	    {
		int cb;
		int rgb = BUILD_PIXEL2(r, g, b);

		k  = 6 - 1;
		cb = (b * k) / MAX_BLUE;
		q  = (b * k) % MAX_BLUE;
		if (q > d && cb < k)
		    cb++;
		diffb = abs (cb * k - b);
		GUI.palette[rgb] = (cr * 6 + cg) * 6 + cb;
		color_diff[rgb] = diffr + diffg + diffb;
		if (color_diff[rgb] > maxdiff)
		    maxdiff = color_diff[rgb];
	    }
	}
    }

    while (maxdiff > 0 && l < 256)
    {
	int newmaxdiff = 0;
	lost = 0; won++;
	for (r = MAX_RED; r >= 0; r--)
	{
	    int g;
      
	    for (g = MAX_GREEN; g >= 0; g--)
	    {
		int b;
	  
		for (b = MAX_BLUE; b >= 0; b--) 
		{
		    int rgb = BUILD_PIXEL2(r, g, b);

		    if (color_diff[rgb] == maxdiff)
		    {
			if (l >= 256)
			    lost++;
			else
			{
			    GUI.fixed_colours [l].red = r;
			    GUI.fixed_colours [l].green = g;
			    GUI.fixed_colours [l].blue = b;
			    GUI.palette [rgb] = l;
			    l++;
			}
			color_diff[rgb] = 0;
		    }
		    else
			if (color_diff[rgb] > newmaxdiff)
			    newmaxdiff = color_diff[rgb];
		    
		}
	    }
	}
	maxdiff = newmaxdiff;
    }
    delete[] color_diff;

    XSetWindowAttributes attrib;

    attrib.background_pixel = BlackPixelOfScreen (GUI.screen);
    GUI.window = XCreateWindow (GUI.display, RootWindowOfScreen (GUI.screen),
				(WidthOfScreen(GUI.screen) - GUI.window_width) / 2,
				(HeightOfScreen(GUI.screen) - GUI.window_height) / 2,
				GUI.window_width, GUI.window_height, 0, 
				GUI.depth, InputOutput, GUI.visual, 
				CWBackPixel, &attrib);

#ifdef USE_DGA_EXTENSION
    CreateFullScreenWindow ();
#endif

    static XColor bg;
    static XColor fg;
    static char data [8] = { 0,0,0,0,0,0,0,0 };

    Pixmap bitmap = XCreateBitmapFromData (GUI.display, GUI.window, data, 8, 8);
    GUI.point_cursor = XCreatePixmapCursor (GUI.display, bitmap, bitmap, &fg, &bg, 0, 0);
    XDefineCursor (GUI.display, GUI.window, GUI.point_cursor);
#ifdef USE_DGA_EXTENSION
    if (XF86.full_screen_available)
	XDefineCursor (GUI.display, XF86.fs_window, GUI.point_cursor);
#endif

    GUI.cross_hair_cursor = XCreateFontCursor (GUI.display, XC_crosshair);
    GUI.gc = DefaultGCOfScreen (GUI.screen);
    {
        XSizeHints Hints;
	XWMHints WMHints;

	memset ((void *) &Hints, 0, sizeof (XSizeHints));
	memset ((void *) &WMHints, 0, sizeof (XWMHints));

	Hints.flags = PSize | PMinSize;
	Hints.min_width = Hints.base_width = SNES_WIDTH;
	Hints.min_height = Hints.base_height = SNES_HEIGHT_EXTENDED;
	WMHints.input = True;
	WMHints.flags = InputHint;
	XSetWMHints (GUI.display, GUI.window, &WMHints);
	XSetWMNormalHints (GUI.display, GUI.window, &Hints);
    }
    XSelectInput (GUI.display, GUI.window, FocusChangeMask | ExposureMask |
		  KeyPressMask | KeyReleaseMask | StructureNotifyMask |
		  ButtonPressMask | ButtonReleaseMask);
#ifdef USE_DGA_EXTENSION
    if (XF86.full_screen_available)
	XSelectInput (GUI.display, XF86.fs_window, FocusChangeMask | ExposureMask |
		      KeyPressMask | KeyReleaseMask | StructureNotifyMask |
		      ButtonPressMask | ButtonReleaseMask);
#endif

    if (GUI.pseudo)
    {
	GUI.cmap = XCreateColormap (GUI.display, GUI.window, GUI.visual, True);
	XSetWindowColormap (GUI.display, GUI.window, GUI.cmap);
	for (i = 0; i < 256; i++)
	{
	    GUI.colors[i].red = GUI.colors[i].green = GUI.colors[i].blue = 0;
	    GUI.colors[i].pixel = i;
	    GUI.colors[i].flags = DoRed | DoGreen | DoBlue;
	}
	XStoreColors (GUI.display, GUI.cmap, GUI.colors, 256);
    }
    XMapRaised (GUI.display, GUI.window);
    XClearWindow (GUI.display, GUI.window);
    SetupImage ();

    switch (GUI.depth)
    {
    case 8:
	GUI.bytes_per_pixel = 1;
	break;

    case 15:
    case 16:
	GUI.bytes_per_pixel = 2;
	break;

    case 24:
	if (GUI.image->bits_per_pixel == 24)
	    GUI.bytes_per_pixel = 3;
	else
	    GUI.bytes_per_pixel = 4;
	break;

    case 32:
	GUI.bytes_per_pixel = 4;
	break;
    }
#if 0
    app = new QApplication (GUI.display);
    gui = new Snes9xGUI ();
    app->setMainWidget (gui);
    gui->show ();
#endif

#ifdef USE_OPENGL
    if (Settings.OpenGLEnable)
	S9xOpenGLInit2 ();
#endif

#ifdef USE_GLIDE
    putenv("FX_GLIDE_NO_SPLASH=");
    S9xSwitchToGlideMode (TRUE);
#endif
}

void SetupImage ()
{
    int tf = 0;
    int image_width = GUI.window_width;
    int image_height = GUI.window_height;

    if (image_width < IMAGE_WIDTH)
	image_width = IMAGE_WIDTH;
    if (image_height < IMAGE_HEIGHT)
	image_height = IMAGE_HEIGHT;

    if (GUI.interpolate)
    {
	if (image_width < 512)
	    image_width = 512;
	if (image_height < 239 * 2)
	    image_height = 239 * 2;
	GUI.image_needs_scaling = (GUI.window_width != 512 || 
				   GUI.window_height != 239 * 2) &&
				   GUI.interpolate != 5;
    }
    else
    {
	GUI.image_needs_scaling = GUI.window_width != IMAGE_WIDTH ||
				  GUI.window_height != IMAGE_HEIGHT ||
				  GUI.scale
#ifdef USE_DGA_EXTENSION
				  || (XF86.is_full_screen && XF86.scale)
#endif
				  ;
    }

    uint32 i;

    for (i = 0; i < sizeof (GUI.to_free) / sizeof (GUI.to_free [0]); i++)
	if (GUI.to_free [i])
	{
	    free (GUI.to_free [i]);
	    GUI.to_free [i] = NULL;
	}

    if (GUI.image)
    {
#ifdef MITSHM
	if (GUI.use_shared_memory)
	{
	    XShmDetach (GUI.display, &GUI.sm_info);
	    GUI.image->data = NULL;
	    XDestroyImage (GUI.image);
	    if (GUI.sm_info.shmaddr)
		shmdt (GUI.sm_info.shmaddr);
	    if (GUI.sm_info.shmid >= 0)
		shmctl (GUI.sm_info.shmid, IPC_RMID, 0);
	    GUI.image = NULL;
        }
	else
#endif
	{
	    XDestroyImage (GUI.image);
	    GUI.image = NULL;
	}
    }

#ifdef MITSHM
    GUI.use_shared_memory = 1;

    int major, minor;
    Bool shared;
    if (!XShmQueryVersion (GUI.display, &major, &minor, &shared) || !shared)
	GUI.image = NULL;
    else
	GUI.image = XShmCreateImage (GUI.display, GUI.visual, GUI.depth, ZPixmap, NULL, &GUI.sm_info,
				     image_width, image_height);
    if (!GUI.image)
    {
	fprintf (stderr, "XShmCreateImage failed, switching to XPutImage\n");
	GUI.use_shared_memory = 0;
    }
    else
    {
	GUI.sm_info.shmid = shmget (IPC_PRIVATE, 
				    GUI.image->bytes_per_line * GUI.image->height,
				    IPC_CREAT | 0777);
	if (GUI.sm_info.shmid < 0)
	{
	    fprintf (stderr, "shmget failed, switching to XPutImage\n");
	    XDestroyImage (GUI.image);
	    GUI.use_shared_memory = 0;
	}
	else
	{
	    GUI.image->data = GUI.sm_info.shmaddr = (char *) shmat (GUI.sm_info.shmid, 0, 0);
	    if (!GUI.image->data)
	    {
		fprintf (stderr, "shmat failed, switching to XPutImage\n");
		XDestroyImage (GUI.image);
		shmctl (GUI.sm_info.shmid, IPC_RMID, 0);
		GUI.use_shared_memory = 0;
	    }
	    else
	    {
		GUI.sm_info.readOnly = False;

		XErrorHandler error_handler = XSetErrorHandler (ErrorHandler);
		XShmAttach (GUI.display, &GUI.sm_info);
		XSync (GUI.display, False);
		(void) XSetErrorHandler (error_handler);

		// X Error handler might clear GUI.use_shared_memory if XShmAttach failed
		if (!GUI.use_shared_memory)
		{
		    fprintf (stderr, "XShmAttach failed, switching to XPutImage\n");
		    XDestroyImage (GUI.image);
		    shmdt (GUI.sm_info.shmaddr);
		    shmctl (GUI.sm_info.shmid, IPC_RMID, 0);
		}
	    }
	}
    }

    if (!GUI.use_shared_memory)
    {
#endif
	GUI.image = XCreateImage (GUI.display, GUI.visual, GUI.depth, ZPixmap, 0,
				  (char *) NULL, image_width, image_height,
				  BitmapUnit (GUI.display), 0);
	GUI.image->data = (char *) malloc (image_height *
					   GUI.image->bytes_per_line);
#ifdef LSB_FIRST
	GUI.image->byte_order = LSBFirst;
#else
	GUI.image->byte_order = MSBFirst;
#endif

#ifdef MITSHM
    }
#endif

    int h = IMAGE_HEIGHT;

    if (GUI.depth == 8)
    {
	if (GUI.interpolate)
	{
	    GFX.Pitch = (IMAGE_WIDTH + 4) * 2;
	    h += 2;
	}
	else
	    GFX.Pitch = IMAGE_WIDTH * 2;
	GFX.Screen = (uint16 *) (GUI.to_free [tf++] = malloc (GFX.Pitch * h));
	if (GUI.interpolate)
	{
	    GUI.interpolated_screen = (uint8 *) (GUI.to_free [tf++] = malloc (512 * 478 * 2));
	    GUI.delta_screen = (uint8 *) (GUI.to_free [tf++] = malloc (GFX.Pitch * h));
	}
    }
    else
    {
	if ((GUI.depth != 15 && GUI.depth != 16) || GUI.interpolate)
	{
	    if (GUI.interpolate)
	    {
		GFX.Pitch = (IMAGE_WIDTH + 4) * 2;
		h += 2;
	    }
	    else
		GFX.Pitch = IMAGE_WIDTH * 2;
	    GFX.Screen = (uint16 *) (GUI.to_free [tf++] = malloc (GFX.Pitch * h));
	    if (GUI.interpolate)
	    {
		if (GUI.image_needs_scaling || (GUI.depth != 15 && GUI.depth != 16)
#ifdef USE_DGA_EXTENSION
		    || XF86.scale
#endif
)
		GUI.interpolated_screen = (uint8 *) (GUI.to_free [tf++] = malloc (512 * 478 * 2));
		GUI.delta_screen = (uint8 *) (GUI.to_free [tf++] = malloc (GFX.Pitch * h));
	    }
	}
	else
	{
	    GFX.Screen = (uint16 *) GUI.image->data;
	    GFX.Pitch = GUI.image->bytes_per_line;
	}
    }
    ZeroMemory (GFX.Screen, GFX.Pitch * h);
    if ((uint16 *) GUI.image->data != GFX.Screen)
	ZeroMemory (GUI.image->data, GUI.image->bytes_per_line * GUI.image->height);
    if (GUI.delta_screen)
	memset (GUI.delta_screen, 0xff, GFX.Pitch * h);
    if (GUI.interpolated_screen)
	ZeroMemory (GUI.interpolated_screen, 512 * 478 * 2);
    if (GUI.interpolate)
    {
	// Offset the rendering of the SNES image by at least one pixel because
	// Kreed's interpolation routines read one pixel beyond the bounds of
	// the source image buffer.
	GFX.Screen += GFX.Pitch/2 + 2;
    }
    GUI.image_date = (uint8 *) GUI.image->data;
    GUI.bytes_per_line = GUI.image->bytes_per_line;
}

int ErrorHandler (Display *, XErrorEvent *)
{
#ifdef MITSHM
    GUI.use_shared_memory = 0;
#endif
    return (0);
}

void S9xSetTitle (const char *string)
{
#ifdef USE_AIDO
    if (Settings.AIDOShmId)
        return;
#endif

    XStoreName (GUI.display, GUI.window, string);
    XFlush (GUI.display);
}
    
#ifdef SELECT_BROKEN_FOR_SIGNALS
#include <sys/ioctl.h>
#endif

static bool8 CheckForPendingXEvents (Display *display)
{
#ifdef SELECT_BROKEN_FOR_SIGNALS
    int arg = 0;

    return (XEventsQueued (display, QueuedAlready) ||
	    ioctl (ConnectionNumber (display), FIONREAD, &arg) == 0 && arg);
#else
    return (XPending (display));
#endif
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
    filename = S9xSelectFilename (def, S9xGetDirectory (SPC_DIR), "smv", title);
    S9xSetSoundMute (FALSE);
    return (filename);
}

void S9xProcessEvents (bool8 block)
{
#ifdef USE_AIDO
    if (Settings.AIDOShmId)
    {
        S9xAIDOProcessEvents(block);
        return;
    }
#endif

    while (block || CheckForPendingXEvents (GUI.display))
    {
        XEvent event;

        XNextEvent (GUI.display, &event);
        block = FALSE;

#if 0
        if (event.xany.window != window)
        {
            app->x11ProcessEvent (&event);
            continue;
        }
#endif
        switch (event.type)
        {
        case KeyPress:
        case KeyRelease:
          S9xReportButton(((event.xkey.state&(ShiftMask|Mod1Mask|ControlMask|Mod4Mask))<<8)|event.xkey.keycode, event.type==KeyPress);
#define STUPID_X_KEYSYM_HACK
#ifdef STUPID_X_KEYSYM_HACK
          {
              KeyCode kc=XKeysymToKeycode(GUI.display, XKeycodeToKeysym(GUI.display, event.xkey.keycode, 0));
              if(event.xkey.keycode!=kc)
                  S9xReportButton(((event.xkey.state&(ShiftMask|Mod1Mask|ControlMask|Mod4Mask))<<8)|kc, event.type==KeyPress);
          }
#endif
          break;
        case FocusIn:
            SetXRepeat (false);
            XFlush (GUI.display);
            //Settings.Paused &= ~2;
            break;
        case FocusOut:
            SetXRepeat (true);
            XFlush (GUI.display);
            //Settings.Paused |= 2;
            break;
        case ConfigureNotify:
            if (event.xany.window == GUI.window &&
                (GUI.window_width != event.xconfigure.width ||
                 GUI.window_height != event.xconfigure.height))
            {
                GUI.window_width = event.xconfigure.width;
                GUI.window_height = event.xconfigure.height;
                IPPU.RenderThisFrame = TRUE;
                IPPU.FrameSkip = Settings.SkipFrames;
                SetupImage ();
            }
#ifdef USE_DGA_EXTENSION
            if (XF86.start_full_screen)
            {
                XF86.start_full_screen = FALSE;
                S9xSwitchToFullScreen (TRUE);
            }
#endif
            break;

        case ButtonPress:
        case ButtonRelease:
            S9xReportButton(0x40000000|(event.xbutton.button-1), event.type==ButtonPress);
            break;
        }
    }
}

void S9xPutImage (int snes_width, int snes_height)
{
#ifdef USE_GLIDE
    if (Settings.GlideEnable)
	S9xGlidePutImage (snes_width, snes_height);
    else
#elif USE_OPENGL
    if (Settings.OpenGLEnable)
	S9xOpenGLPutImage (snes_width, snes_height);
    else
#elif USE_AIDO
    if (Settings.AIDOShmId)
        S9xAIDOPutImage (snes_width, snes_height);
    else
#endif

    {
    bool8 done = FALSE;
    int width, height, cheight;

    width = snes_width;
    height = snes_height;
    cheight = (height>SNES_HEIGHT_EXTENDED)?SNES_HEIGHT_EXTENDED*2:SNES_HEIGHT_EXTENDED;
    
    if (GUI.interpolate)
    {
	if (snes_width == 512 && snes_height > 240 && GUI.interpolate != 5)
	{
	    GUI.output_screen = (uint8*)GFX.Screen;
	    GUI.output_pitch = GFX.Pitch;

#ifdef USE_DGA_EXTENSION
	    if (XF86.is_full_screen)
	    {
		if (XF86.scale)
		    GUI.image_date = (uint8 *) XF86.vram;
		else
		{
		    GUI.box.x = (XF86.window_width - width) / 2;
		    GUI.box.y = (XF86.window_height - cheight) / 2;
		    GUI.image_date = (uint8 *) XF86.vram +
			    GUI.box.x * GUI.bytes_per_pixel +
			    GUI.box.y * GUI.bytes_per_line;
		}
	    }
#endif
	    if (!GUI.image_needs_scaling)
	    {
		for (int y = 0; y < snes_height; y++)
		{
		    memmove (GUI.image_date + y * GUI.bytes_per_line,
			     GFX.Screen + GFX.Pitch/2 * y,
			     snes_width * GUI.bytes_per_pixel);
		}
		done = TRUE;
	    }
	}
	else
	{
	    if (GUI.interpolate != 5)
	    {
		width = 512;
		if (snes_height < 240)
		    height = snes_height << 1;
		else
		    height = snes_height;
                cheight = SNES_HEIGHT_EXTENDED << 1;
	    }
	    else
	    {
		width = GUI.window_width;
		cheight = height = GUI.window_height;
	    }
	    if (GUI.image_needs_scaling || GUI.interpolate == 5)
	    {
		GUI.box.x = 0;
		GUI.box.y = 0;
	    }
	    else
	    {
		GUI.box.x = (GUI.window_width - width) / 2;
		GUI.box.y = (GUI.window_height - cheight) / 2;
	    }

	    // Kreed's bi-linear image filter scales as well
	    if ((GUI.image_needs_scaling && GUI.interpolate != 5) ||
		(GUI.depth != 15 && GUI.depth != 16))
	    {
		GUI.output_screen = GUI.interpolated_screen;
		GUI.output_pitch = 512 * 2;
	    }
	    else
	    {
#ifdef USE_DGA_EXTENSION
		if (XF86.is_full_screen)
		{
		    if (XF86.scale)
			GUI.image_date = (uint8 *) XF86.vram;
		    else
		    {
			GUI.box.x = (XF86.window_width - width) / 2;
			GUI.box.y = (XF86.window_height - cheight) / 2;
			GUI.image_date = (uint8 *) XF86.vram +
				GUI.box.x * GUI.bytes_per_pixel +
				GUI.box.y * GUI.bytes_per_line;
		    }
		    GUI.output_screen = GUI.image_date;
		    GUI.output_pitch = GUI.bytes_per_line;
		    done = TRUE;
		}
		else
#endif
		{
		    GUI.output_screen = (uint8 *) GUI.image->data;
		    GUI.output_pitch = GUI.image->bytes_per_line;
		}
	    }

	    if (snes_width != GUI.last_snes_width ||
		snes_height != GUI.last_snes_height)
	    {
		memset (GUI.delta_screen, 255, GFX.Pitch * snes_height);
	    }
	    TVMode (snes_width, snes_height);
	}
    }
    else
    {
	GUI.output_screen = (uint8*)GFX.Screen;
	GUI.output_pitch = GFX.Pitch;
	width = snes_width;
	height = snes_height;
        cheight = (height>SNES_HEIGHT_EXTENDED)?SNES_HEIGHT_EXTENDED<<1:SNES_HEIGHT_EXTENDED;

	if (GUI.image_needs_scaling)
	{
	    GUI.box.x = 0;
	    GUI.box.y = 0;
	}
	else
	{
	    GUI.box.x = (GUI.window_width - width) / 2;
	    GUI.box.y = (GUI.window_height - cheight) / 2;
	}
    }

    if ((GUI.depth != 15 && GUI.depth != 16) ||
	(GUI.image_needs_scaling && GUI.interpolate != 5))
    {
	done = TRUE;
	switch (GUI.depth)
	{
          case 8:
            Convert16To8 (width, height);
	    break;

          case 15:
          case 16:
            Scale16 (width, height);
	    break;

          case 32:
          case 24:
            if (GUI.image->bits_per_pixel == 32)
                Convert16To24 (width, height);
            else
                Convert16To24Packed (width, height);
            break;
	}
    }
    if (GUI.image_needs_scaling)
    {
	GUI.box.width = GUI.window_width;
	GUI.box.height = GUI.window_height;
    }
    else
    {
	GUI.box.width = width;
	GUI.box.height = height;
    }

#ifdef USE_DGA_EXTENSION
    if (XF86.is_full_screen && !done)
    {
	if (XF86.scale)
	    GUI.image_date = (uint8 *) XF86.vram;
	else
	    GUI.image_date = (uint8 *) XF86.vram +
		    ((XF86.window_width - width) / 2) * GUI.bytes_per_pixel +
		    ((XF86.window_height - cheight) / 2) * GUI.bytes_per_line;

	for (int y = 0; y < snes_height; y++)
	{
	    memmove (GUI.image_date + y * GUI.bytes_per_line,
		     GFX.Screen + GFX.Pitch/2 * y,
		     snes_width * GUI.bytes_per_pixel);
	}
    }
#endif

#ifdef USE_DGA_EXTENSION
    if (!XF86.is_full_screen)
    {
#endif
#ifdef MITSHM
	if(GUI.use_shared_memory)
	{
	    XShmPutImage (GUI.display, GUI.window, GUI.gc, GUI.image,
			  0, 0, 
			  GUI.box.x, GUI.box.y,
			  GUI.box.width, GUI.box.height,
			  False);
	    XSync (GUI.display, False);
	}
	else
#endif
	    XPutImage (GUI.display, GUI.window, GUI.gc, GUI.image,
		       0, 0, 
		       GUI.box.x, GUI.box.y,
		       GUI.box.width, GUI.box.height);
#ifdef USE_DGA_EXTENSION
    }
#endif

    GUI.last_snes_width = snes_width;
    GUI.last_snes_height = snes_height;

    if (GUI.box.x != GUI.old_box.x || GUI.box.y != GUI.old_box.y ||
	GUI.box.width != GUI.old_box.width || GUI.box.height != GUI.old_box.height)
    {
	// If the rendered image has changed size/position clear any areas of the
	// screen that should now be border
	Region old_box = XCreateRegion ();
	Region new_box = XCreateRegion ();

	XUnionRectWithRegion (&GUI.old_box, old_box, old_box);
	XUnionRectWithRegion (&GUI.box, new_box, new_box);
	XSubtractRegion (old_box, new_box, old_box);
	if (!XEmptyRegion (old_box))
	{
	    Window window = GUI.window;
	    XRectangle clip;

	    XClipBox (old_box, &clip);
	    XSetRegion (GUI.display, GUI.gc, old_box);
	    XSetForeground (GUI.display, GUI.gc, GUI.depth == 8 ? 0 :
			    BlackPixelOfScreen (GUI.screen));
#ifdef USE_DGA_EXTENSION
	    if (XF86.full_screen_available)
		window = XF86.fs_window;
#endif
	    XFillRectangle (GUI.display, window, GUI.gc, 
			    clip.x, clip.y, clip.width, clip.height);
	    XSetClipMask (GUI.display, GUI.gc, None);
	}
	
	XDestroyRegion (new_box);
	XDestroyRegion (old_box);
	GUI.old_box = GUI.box;
    }

    Window root, child;
    int root_x, root_y;
    int x, y;
    unsigned int mask;

    // Use QueryPointer to sync X server and as a side effect also gets
    // current pointer position for SNES mouse emulation.
    XQueryPointer (GUI.display, GUI.window, &root, &child, &root_x, &root_y,
		   &x, &y, &mask);

    if (x >= 0 && y >= 0 && x < GUI.window_width && y < GUI.window_height)
    {
        GUI.mouse_x = x * 256 / GUI.window_width;  /* Scale to SNES's. */
        GUI.mouse_y = y * SNES_HEIGHT_EXTENDED / GUI.window_height;
	if (mask & Mod1Mask)
	{
	    if (!GUI.mod1_pressed)
	    {
		GUI.mod1_pressed = TRUE;
		XDefineCursor (GUI.display, GUI.window, GUI.cross_hair_cursor);
#ifdef USE_DGA_EXTENSION
		if (XF86.is_full_screen)
		    XDefineCursor (GUI.display, GUI.window, GUI.cross_hair_cursor);
#endif
	    }
	}
	else
	if (GUI.mod1_pressed)
	{
	    GUI.mod1_pressed = FALSE;
            XDefineCursor (GUI.display, GUI.window, GUI.point_cursor);
#ifdef USE_DGA_EXTENSION
            if (XF86.is_full_screen)
                XDefineCursor (GUI.display, GUI.window, GUI.point_cursor);
#endif
        }
    }
    }
}

void S9xSetPalette ()
{
#if defined(USE_GLIDE) || defined(USE_AIDO)
    if (Settings.GlideEnable || Settings.AIDOShmId)
	return;
#endif

    int i;

    if (GUI.grayscale)
    {
	uint16 Brightness = IPPU.MaxBrightness;
	    
	for (i = 0; i < 256; i++)
	{
	    GUI.colors[i].flags = DoRed | DoGreen | DoBlue;
	    GUI.colors[i].red = GUI.colors[i].green = GUI.colors[i].blue = 
		(uint16)(((((PPU.CGDATA[i] >> 0) & 0x1F) * Brightness * 50) +
		        (((PPU.CGDATA[i] >> 5) & 0x1F) * Brightness * 69) +
			(((PPU.CGDATA[i] >> 10) & 0x1F) * Brightness * 21)) * 1.40935);
	}
	XStoreColors (GUI.display, GUI.cmap, GUI.colors, 256);
    }
    else
    if (GUI.pseudo)
    {
        for (i = 0; i < 256; i++)
        {
            GUI.colors[i].flags = DoRed | DoGreen | DoBlue;
            GUI.colors[i].red = GUI.fixed_colours[i].red << 11;
            GUI.colors[i].green = GUI.fixed_colours[i].green << 11;
            GUI.colors[i].blue = GUI.fixed_colours[i].blue << 11;
        }
	XStoreColors (GUI.display, GUI.cmap, GUI.colors, 256);
    }
}

const char *S9xSelectFilename (const char *def, const char *dir1,
			    const char *ext1, const char *title)
{
    static char path [PATH_MAX];
    char buffer [PATH_MAX];
    
    SetXRepeat (true);

    printf ("\n%s (default: %s): ", title, def);
    fflush (stdout);
    if (fgets (buffer, sizeof (buffer) - 1, stdin))
    {
        SetXRepeat (false);

	char *p = buffer;
	while (isspace (*p))
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
	return (path);
    }

    SetXRepeat (false);

    return (NULL);
}

void Scale8 (int width, int height)
{
    register uint32 x_error;
    register uint32 x_fraction;
    uint32 y_error = 0;
    uint32 y_fraction;
    int yy = height - 1;
    
    x_fraction = (width * 0x10000) / GUI.window_width;
    y_fraction = (height * 0x10000) / GUI.window_height;
    
    for (int y = GUI.window_height - 1; y >= 0; y--)
    {
	register uint8 *d = (uint8 *) GUI.image_date + y * GUI.bytes_per_line +
			   GUI.window_width - 1;
	register uint8 *s = GUI.output_screen + yy * GUI.output_pitch + width - 1;
	y_error += y_fraction;
	while (y_error >= 0x10000)
	{
	    yy--;
	    y_error -= 0x10000;
	}
	x_error = 0;
	for (register int x = GUI.window_width - 1; x >= 0; x--)
	{
	    *d-- = *s;
	    x_error += x_fraction;

	    while (x_error >= 0x10000)
	    {
		s--;
		x_error -= 0x10000;
	    }
	}
    }
}

void Scale16 (int width, int height)
{
    register uint32 x_error;
    register uint32 x_fraction;
    uint32 y_error = 0;
    uint32 y_fraction;
    int yy = height - 1;
    
    x_fraction = (width * 0x10000) / GUI.window_width;
    y_fraction = (height * 0x10000) / GUI.window_height;
    
    for (int y = GUI.window_height - 1; y >= 0; y--)
    {
	register uint16 *d = (uint16 *) (GUI.image_date + y * GUI.bytes_per_line) +
					 GUI.window_width - 1;
	register uint16 *s = (uint16 *) (GUI.output_screen + yy * GUI.output_pitch) + width - 1;
	y_error += y_fraction;
	while (y_error >= 0x10000)
	{
	    yy--;
	    y_error -= 0x10000;
	}
	x_error = 0;
	for (register int x = GUI.window_width - 1; x >= 0; x--)
	{
	    *d-- = *s;
	    x_error += x_fraction;

	    while (x_error >= 0x10000)
	    {
		s--;
		x_error -= 0x10000;
	    }
	}
    }
}

void Convert8To24 (int width, int height)
{
    uint32 brightness = IPPU.MaxBrightness >> 1;

    if (!GUI.image_needs_scaling)
    {
	// Convert
	for (register int y = 0; y < height; y++)
	{
	    register uint32 *d = (uint32 *) (GUI.image_date +
					     y * GUI.bytes_per_line);
	    register uint8 *s = GUI.output_screen + y * GUI.output_pitch;

	    for (register int x = 0; x < width; x++)
	    {
		uint32 pixel = PPU.CGDATA [*s++];
		*d++ = (((pixel & 0x1f) * brightness) << GUI.red_shift) |
		       ((((pixel >> 5) & 0x1f) * brightness) << GUI.green_shift) |
		       ((((pixel >> 10) & 0x1f) * brightness) << GUI.blue_shift);
	    }
	}
    }
    else
    {
	// Scale and convert
	register uint32 x_error;
	register uint32 x_fraction;
	uint32 y_error = 0;
	uint32 y_fraction;
	int yy = 0;
	
	x_fraction = (width * 0x10000) / GUI.window_width;
	y_fraction = (height * 0x10000) / GUI.window_height;
	
	for (int y = 0; y < GUI.window_height; y++)
	{
	    register uint32 *d = (uint32 *) (GUI.image_date +
					   y * GUI.bytes_per_line);
	    register uint8 *s = GUI.output_screen + yy * GUI.output_pitch;
	    y_error += y_fraction;
	    while (y_error >= 0x10000)
	    {
		yy++;
		y_error -= 0x10000;
	    }
	    x_error = 0;
	    for (register int x = 0; x < GUI.window_width; x++)
	    {
		uint32 pixel = PPU.CGDATA [*s];
		*d++ = (((pixel & 0x1f) * brightness) << GUI.red_shift) |
		       ((((pixel >> 5) & 0x1f) * brightness) << GUI.green_shift) |
		       ((((pixel >> 10) & 0x1f) * brightness) << GUI.blue_shift);
		       
		x_error += x_fraction;
		while (x_error >= 0x10000)
		{
		    s++;
		    x_error -= 0x10000;
		}
	    }
	}
    }
}

void Convert16To24 (int width, int height)
{
    if (!GUI.image_needs_scaling)
    {
	// Convert
	for (register int y = 0; y < height; y++)
	{
	    register uint32 *d = (uint32 *) (GUI.image_date +
					     y * GUI.bytes_per_line);
	    register uint16 *s = (uint16 *) (GUI.output_screen + y * GUI.output_pitch);

	    for (register int x = 0; x < width; x++)
	    {
		uint32 pixel = *s++;
		*d++ = (((pixel >> 11) & 0x1f) << (GUI.red_shift + 3)) |
		       (((pixel >> 6) & 0x1f) << (GUI.green_shift + 3)) |
		       ((pixel & 0x1f) << (GUI.blue_shift + 3));
	    }
	}
    }
    else
    {
	// Scale and convert
	register uint32 x_error;
	register uint32 x_fraction;
	uint32 y_error = 0;
	uint32 y_fraction;
	int yy = 0;
	
	x_fraction = (width * 0x10000) / GUI.window_width;
	y_fraction = (height * 0x10000) / GUI.window_height;
	
	for (int y = 0; y < GUI.window_height; y++)
	{
	    register uint32 *d = (uint32 *) (GUI.image_date +
					     y * GUI.bytes_per_line);
	    register uint16 *s = (uint16 *) (GUI.output_screen + yy * GUI.output_pitch);
	    y_error += y_fraction;
	    while (y_error >= 0x10000)
	    {
		yy++;
		y_error -= 0x10000;
	    }
	    x_error = 0;
	    for (register int x = 0; x < GUI.window_width; x++)
	    {
		uint32 pixel = *s;
		*d++ = (((pixel >> 11) & 0x1f) << (GUI.red_shift + 3)) |
		       (((pixel >> 6) & 0x1f) << (GUI.green_shift + 3)) |
		       ((pixel & 0x1f) << (GUI.blue_shift + 3));
		       
		x_error += x_fraction;
		while (x_error >= 0x10000)
		{
		    s++;
		    x_error -= 0x10000;
		}
	    }
	}
    }
}

void Convert8To24Packed (int width, int height)
{
    uint32 brightness = IPPU.MaxBrightness >> 1;
    uint8 levels [32];

    for (int l = 0; l < 32; l++)
	levels [l] = l * brightness;
	
    if (!GUI.image_needs_scaling)
    {
	// Convert
	for (register int y = 0; y < height; y++)
	{
	    register uint8 *d = (uint8 *) (GUI.image_date + y * GUI.bytes_per_line);
	    register uint8 *s = GUI.output_screen + y * GUI.output_pitch;

#ifdef LSB_FIRST
	    if (GUI.red_shift < GUI.blue_shift)
#else	    
	    if (GUI.red_shift > GUI.blue_shift)
#endif
	    {
		// Order is RGB
		for (register int x = 0; x < width; x++)
		{
		    uint16 pixel = PPU.CGDATA [*s++];
		    *d++ = levels [(pixel & 0x1f)];
		    *d++ = levels [((pixel >> 5) & 0x1f)];
		    *d++ = levels [((pixel >> 10) & 0x1f)];
		}
	    }
	    else
	    {
		// Order is BGR
		for (register int x = 0; x < width; x++)
		{
		    uint16 pixel = PPU.CGDATA [*s++];
		    *d++ = levels [((pixel >> 10) & 0x1f)];
		    *d++ = levels [((pixel >> 5) & 0x1f)];
		    *d++ = levels [(pixel & 0x1f)];
		}
	    }
	}
    }
    else
    {
	// Scale and convert
	register uint32 x_error;
	register uint32 x_fraction;
	uint32 y_error = 0;
	uint32 y_fraction;
	int yy = 0;
	
	x_fraction = (width * 0x10000) / GUI.window_width;
	y_fraction = (height * 0x10000) / GUI.window_height;
	
	for (int y = 0; y < GUI.window_height; y++)
	{
	    register uint8 *d = (uint8 *) (GUI.image_date +
					 y * GUI.bytes_per_line);
	    register uint8 *s = GUI.output_screen + yy * GUI.output_pitch;
	    y_error += y_fraction;
	    while (y_error >= 0x10000)
	    {
		yy++;
		y_error -= 0x10000;
	    }
	    x_error = 0;
#ifdef LSB_FIRST
	    if (GUI.red_shift < GUI.blue_shift)
#else
	    if (GUI.red_shift > GUI.blue_shift)
#endif
	    {
		// Order is RGB
		for (register int x = 0; x < GUI.window_width; x++)
		{
		    uint16 pixel = PPU.CGDATA [*s];
		    *d++ = levels [(pixel & 0x1f)];
		    *d++ = levels [((pixel >> 5) & 0x1f)];
		    *d++ = levels [((pixel >> 10) & 0x1f)];
		       
		    x_error += x_fraction;
		    while (x_error >= 0x10000)
		    {
			s++;
			x_error -= 0x10000;
		    }
		}
	    }
	    else
	    {
		// Order is BGR
		for (register int x = 0; x < GUI.window_width; x++)
		{
		    uint16 pixel = PPU.CGDATA [*s];
		    *d++ = levels [((pixel >> 10) & 0x1f)];
		    *d++ = levels [((pixel >> 5) & 0x1f)];
		    *d++ = levels [(pixel & 0x1f)];
		       
		    x_error += x_fraction;
		    while (x_error >= 0x10000)
		    {
			s++;
			x_error -= 0x10000;
		    }
		}
	    }
	}
    }
}

void Convert16To24Packed (int width, int height)
{
    if (!GUI.image_needs_scaling)
    {
	// Convert
	for (register int y = 0; y < height; y++)
	{
	    register uint8 *d = (uint8 *) (GUI.image_date +
					 y * GUI.bytes_per_line);
	    register uint16 *s = (uint16 *) (GUI.output_screen + y * GUI.output_pitch);

#ifdef LSB_FIRST
	    if (GUI.red_shift < GUI.blue_shift)
#else	    
	    if (GUI.red_shift > GUI.blue_shift)
#endif
	    {
		// Order is RGB
		for (register int x = 0; x < width; x++)
		{
		    uint32 pixel = *s++;
		    *d++ = (pixel >> (11 - 3)) & 0xf8;
		    *d++ = (pixel >> (6 - 3)) & 0xf8;
		    *d++ = (pixel & 0x1f) << 3;
		}
	    }
	    else
	    {
		// Order is BGR
		for (register int x = 0; x < width; x++)
		{
		    uint32 pixel = *s++;
		    *d++ = (pixel & 0x1f) << 3;
		    *d++ = (pixel >> (6 - 3)) & 0xf8;
		    *d++ = (pixel >> (11 - 3)) & 0xf8;
		}
	    }
	}
    }
    else
    {
	// Scale and convert
	register uint32 x_error;
	register uint32 x_fraction;
	uint32 y_error = 0;
	uint32 y_fraction;
	int yy = 0;
	
	x_fraction = (width * 0x10000) / GUI.window_width;
	y_fraction = (height * 0x10000) / GUI.window_height;
	
	for (int y = 0; y < GUI.window_height; y++)
	{
	    register uint8 *d = (uint8 *) (GUI.image_date +
					 y * GUI.bytes_per_line);
	    register uint16 *s = (uint16 *) (GUI.output_screen + yy * GUI.output_pitch);
	    y_error += y_fraction;
	    while (y_error >= 0x10000)
	    {
		yy++;
		y_error -= 0x10000;
	    }
	    x_error = 0;
#ifdef LSB_FIRST
	    if (GUI.red_shift < GUI.blue_shift)
#else
	    if (GUI.red_shift > GUI.blue_shift)
#endif
	    {
		// Order is RGB
		for (register int x = 0; x < GUI.window_width; x++)
		{
		    uint32 pixel = *s;
		    *d++ = (pixel >> (11 - 3)) & 0xf8;
		    *d++ = (pixel >> (6 - 3)) & 0xf8;
		    *d++ = (pixel & 0x1f) << 3;
		       
		    x_error += x_fraction;
		    while (x_error >= 0x10000)
		    {
			s++;
			x_error -= 0x10000;
		    }
		}
	    }
	    else
	    {
		// Order is BGR
		for (register int x = 0; x < GUI.window_width; x++)
		{
		    uint32 pixel = *s;
		    *d++ = (pixel & 0x1f) << 3;
		    *d++ = (pixel >> (6 - 3)) & 0xf8;
		    *d++ = (pixel >> (11 - 3)) & 0xf8;
		       
		    x_error += x_fraction;
		    while (x_error >= 0x10000)
		    {
			s++;
			x_error -= 0x10000;
		    }
		}
	    }
	}
    }
}

void Convert16To8 (int width, int height)
{
    if (!GUI.image_needs_scaling)
    {
	// Convert
	for (register int y = 0; y < height; y++)
	{
	    register uint8 *d = (uint8 *) GUI.image_date + y * GUI.bytes_per_line;
	    register uint16 *s = (uint16 *) (GUI.output_screen + y * GUI.output_pitch);

	    for (register int x = 0; x < width; x++)
		*d++ = GUI.palette [*s++];
	}
    }
    else
    {
	// Scale and convert
	register uint32 x_error;
	register uint32 x_fraction;
	uint32 y_error = 0;
	uint32 y_fraction;
	int yy = 0;
	
	x_fraction = (width * 0x10000) / GUI.window_width;
	y_fraction = (height * 0x10000) / GUI.window_height;
	
	for (int y = 0; y < GUI.window_height; y++)
	{
	    register uint8 *d = (uint8 *) GUI.image_date + y * GUI.bytes_per_line;
	    register uint16 *s = (uint16 *) (GUI.output_screen + yy * GUI.output_pitch);
	    y_error += y_fraction;
	    while (y_error >= 0x10000)
	    {
		yy++;
		y_error -= 0x10000;
	    }
	    x_error = 0;
	    for (register int x = 0; x < GUI.window_width; x++)
	    {
		*d++ = GUI.palette [*s];
		       
		x_error += x_fraction;
		while (x_error >= 0x10000)
		{
		    s++;
		    x_error -= 0x10000;
		}
	    }
	}
    }
}

void Convert8To16 (int width, int height)
{
    uint32 levels [32];

    for (int l = 0; l < 32; l++)
	levels [l] = (l * IPPU.MaxBrightness) >> 4;
	
    if (!GUI.image_needs_scaling)
    {
	// Convert
	for (register int y = 0; y < height; y++)
	{
	    register uint16 *d = (uint16 *) (GUI.image_date + y * GUI.bytes_per_line);
	    register uint8 *s = GUI.output_screen + y * GUI.output_pitch;

	    for (register int x = 0; x < width; x++)
	    {
		uint32 pixel = PPU.CGDATA [*s++];
		*d++ = (levels [pixel & 0x1f] << GUI.red_shift) |
		       (levels [(pixel >> 5) & 0x1f] << GUI.green_shift) |
		       (levels [(pixel >> 10) & 0x1f] << GUI.blue_shift);
	    }
	}
    }
    else
    {
	// Scale and convert
	register uint32 x_error;
	register uint32 x_fraction;
	uint32 y_error = 0;
	uint32 y_fraction;
	int yy = 0;
	
	x_fraction = (width * 0x10000) / GUI.window_width;
	y_fraction = (height * 0x10000) / GUI.window_height;
	
	for (int y = 0; y < GUI.window_height; y++)
	{
	    register uint16 *d = (uint16 *) (GUI.image_date +
					   y * GUI.bytes_per_line);
	    register uint8 *s = GUI.output_screen + yy * GUI.output_pitch;
	    y_error += y_fraction;
	    while (y_error >= 0x10000)
	    {
		yy++;
		y_error -= 0x10000;
	    }
	    x_error = 0;
	    for (register int x = 0; x < GUI.window_width; x++)
	    {
		uint32 pixel = PPU.CGDATA [*s];
		*d++ = (levels [pixel & 0x1f] << GUI.red_shift) |
		       (levels [(pixel >> 5) & 0x1f] << GUI.green_shift) |
		       (levels [(pixel >> 10) & 0x1f] << GUI.blue_shift);
		       
		x_error += x_fraction;
		while (x_error >= 0x10000)
		{
		    s++;
		    x_error -= 0x10000;
		}
	    }
	}
    }
}

void S9xTextMode ()
{
#ifdef USE_AIDO
    if (Settings.AIDOShmId)
        return;
#endif
#ifdef USE_DGA_EXTENSION
    if (XF86.full_screen_available && XF86.is_full_screen)
    {
	XF86DGADirectVideo (GUI.display, GUI.screen_num, 0);
#ifdef USE_VIDMODE_EXTENSION
	if (XF86.switch_video_mode)
	    XF86VidModeSwitchToMode (GUI.display, GUI.screen_num, &XF86.orig);
#endif
	XUngrabKeyboard (GUI.display, CurrentTime);
	XUngrabPointer (GUI.display, CurrentTime);
	XUnmapWindow (GUI.display, XF86.fs_window);
	XWarpPointer (GUI.display, None, GUI.window, 0, 0, 0, 0, 0, 0);
	XSync (GUI.display, False);
    }
#endif
    SetXRepeat (true);
}

void S9xGraphicsMode ()
{
#ifdef USE_AIDO
    if (Settings.AIDOShmId)
        return;
#endif
#ifdef USE_DGA_EXTENSION
    if (XF86.full_screen_available && XF86.is_full_screen)
    {
	XMapRaised (GUI.display, XF86.fs_window);
	XClearWindow (GUI.display, XF86.fs_window);
	XGrabKeyboard (GUI.display, GUI.window, False, GrabModeAsync, GrabModeAsync,
		       CurrentTime);
	XGrabPointer (GUI.display, GUI.window, False, ALL_DEVICE_EVENTS,
		      GrabModeAsync, GrabModeAsync, GUI.window, GUI.point_cursor,
		      CurrentTime);

	XWarpPointer (GUI.display, None, RootWindowOfScreen (GUI.screen),
		      0, 0, 0, 0, 0, 0);
	XSync (GUI.display, False);

#ifdef USE_VIDMODE_EXTENSION
	if (XF86.switch_video_mode)
	{
	    XF86VidModeSwitchToMode (GUI.display, GUI.screen_num, XF86.best);
	    XF86DGAGetVideo (GUI.display, GUI.screen_num, &XF86.vram,
			     &XF86.line_width, &XF86.bank_size,
			     &XF86.size);
	    XF86VidModeSetViewPort (GUI.display, GUI.screen_num, 0, 0);
	    XSync (GUI.display, False);
	}
#endif
	XF86DGADirectVideo (GUI.display, GUI.screen_num, XF86DGADirectGraphics);
	XF86VidModeSetViewPort (GUI.display, GUI.screen_num, 0, 0);
	XSync (GUI.display, False);

	//memset (XF86.vram, 0, XF86.size * 1024);
    }
#endif
    SetXRepeat (false);
}

void S9xExtraDisplayUsage ()
{
    S9xMessage(S9X_INFO, S9X_USAGE, "-set-repeat or -no-set-repeat   Whether to allow altering keyboard auto-repeat");
    S9xMessage(S9X_INFO, S9X_USAGE, "-y or -y1                       Enable 'TV mode' (implies -16 -hires -tr)");
    S9xMessage(S9X_INFO, S9X_USAGE, "-y2                             Enable Kreed's Super 2xSaI image processing");
    S9xMessage(S9X_INFO, S9X_USAGE, "-y3                             Enable Kreed's Super Eagle image processing");
    S9xMessage(S9X_INFO, S9X_USAGE, "-y4                             Enable Kreed's 2xSaI image processing");
    S9xMessage(S9X_INFO, S9X_USAGE, "-y5                             Enable Kreed's software bi-linear filtering");
    S9xMessage(S9X_INFO, S9X_USAGE, "-GUI.interpolate<num>           Same as -y<num>");
    S9xMessage(S9X_INFO, S9X_USAGE, "-scale or -sc                   Scale image to fit window");
#ifdef USE_DGA_EXTENSION
#ifdef USE_VIDMODE_EXTENSION
    S9xMessage(S9X_INFO, S9X_USAGE, "-nomodeswitch or -nms           Don't switch modes in fullscreen mode");
#endif
    S9xMessage(S9X_INFO, S9X_USAGE, "-fullscreen or -fs              Begin in fullscreen mode");
#endif
    S9xMessage(S9X_INFO, S9X_USAGE, "");
}

void S9xParseDisplayArg (char **argv, int &ind, int)
{
    if (strcasecmp (argv [ind], "-set-repeat") == 0)
    {
        GUI.no_repeat=FALSE;
    }
    else
    if (strcasecmp (argv [ind], "-no-set-repeat") == 0)
    {
        GUI.no_repeat=TRUE;
    }
    else
    if (strncasecmp (argv [ind], "-y", 2) == 0)
    {
        Settings.SupportHiRes = TRUE;
        Settings.ForceTransparency = TRUE;
	switch (argv[ind][2])
	{
	case 0:	    GUI.interpolate = TRUE;	break;
	case '1':   GUI.interpolate = TRUE;	break;
	case '2':   GUI.interpolate = 2;	break;
	case '3':   GUI.interpolate = 3;	break;
	case '4':   GUI.interpolate = 4;	break;
	case '5':   GUI.interpolate = 5;	break;
	}
    }
    else
    if (strncasecmp (argv [ind], "-GUI.interpolate", 12) == 0)
    {
        Settings.SupportHiRes = TRUE;
        Settings.ForceTransparency = TRUE;
	switch (argv[ind][12])
	{
	case 0:	    GUI.interpolate = TRUE;	break;
	case '1':   GUI.interpolate = TRUE;	break;
	case '2':   GUI.interpolate = 2;	break;
	case '3':   GUI.interpolate = 3;	break;
	case '4':   GUI.interpolate = 4;	break;
	case '5':   GUI.interpolate = 5;	break;
	}
    }
    else
    if (strcasecmp (argv [ind], "-scale") == 0 ||
	strcasecmp (argv [ind], "-sc") == 0)
    {
#ifdef USE_DGA_EXTENSION
	XF86.scale = TRUE;
#endif
	GUI.scale = TRUE;
    }
#ifdef USE_DGA_EXTENSION
#ifdef USE_VIDMODE_EXTENSION
    else
    if (strcasecmp (argv [ind], "-nms") == 0 ||
	strcasecmp (argv [ind], "-nomodeswitch") == 0)
	XF86.no_mode_switch = TRUE;
#endif
    else
    if (strcasecmp (argv [ind], "-fs") == 0 ||
	strcasecmp (argv [ind], "-fullscreen") == 0)
	XF86.start_full_screen = TRUE;
#endif
    else
	S9xUsage ();
}

int S9xMinCommandLineArgs ()
{
    return (2);
}

void S9xMessage (int /*type*/, int /*number*/, const char *message)
{
#define MAX_MESSAGE_LEN (36 * 3)

    static char buffer [MAX_MESSAGE_LEN + 1];

    fprintf (stdout, "%s\n", message);
    strncpy (buffer, message, MAX_MESSAGE_LEN);
    buffer [MAX_MESSAGE_LEN] = 0;
    S9xSetInfoString (buffer);
}

void TVMode (int width, int height)
{
    switch (width != 256 && GUI.interpolate != 5 ? 1 : GUI.interpolate)
    {
    case 2:
	Super2xSaI ((uint8*)GFX.Screen, GFX.Pitch, GUI.delta_screen, GUI.output_screen,
		    GUI.output_pitch, width, height);
	break;
    case 3:
	SuperEagle ((uint8*)GFX.Screen, GFX.Pitch, GUI.delta_screen, GUI.output_screen,
		    GUI.output_pitch, width, height);
	break;
    case 4:
	_2xSaI ((uint8*)GFX.Screen, GFX.Pitch, GUI.delta_screen, GUI.output_screen,
		GUI.output_pitch, width, height);
	break;
    case 5:
#ifdef USE_DGA_EXTENSION
	if (XF86.is_full_screen && !XF86.scale)
	{
	    Scale_2xSaI ((uint8*)GFX.Screen, GFX.Pitch, GUI.delta_screen, GUI.output_screen,
			 GUI.output_pitch, IMAGE_WIDTH, IMAGE_HEIGHT, width, height);
	}
	else
#endif
	Scale_2xSaI ((uint8*)GFX.Screen, GFX.Pitch, GUI.delta_screen, GUI.output_screen,
		     GUI.output_pitch, GUI.window_width, GUI.window_height, width, height);
	break;
    case 1:
    {
	uint8 *nextLine, *srcPtr, *deltaPtr, *finish;
	uint8 *dstPtr;
	uint32 colorMask = ~(RGB_LOW_BITS_MASK | (RGB_LOW_BITS_MASK << 16));
	uint32 lowPixelMask = RGB_LOW_BITS_MASK;

	srcPtr = (uint8*)GFX.Screen;
	deltaPtr = GUI.delta_screen;
	dstPtr = GUI.output_screen;
	nextLine = GUI.output_screen + GUI.output_pitch;

	if (width == 256)
	{
	    do
	    {
		uint32 *bP = (uint32 *) srcPtr;
		uint32 *xP = (uint32 *) deltaPtr;
		uint32 *dP = (uint32 *) dstPtr;
		uint32 *nL = (uint32 *) nextLine;
		uint32 currentPixel;
		uint32 nextPixel;
		uint32 currentDelta;
		uint32 nextDelta;

		finish = (uint8 *) bP + ((width + 2) << 1);
		nextPixel = *bP++;
		nextDelta = *xP++;

		do
		{
		    currentPixel = nextPixel;
		    currentDelta = nextDelta;
		    nextPixel = *bP++;
		    nextDelta = *xP++;

		    if ((nextPixel != nextDelta) || (currentPixel != currentDelta))
		    {
			uint32 colorA, colorB, product, darkened;

			*(xP - 2) = currentPixel;
#ifdef LSB_FIRST
			colorA = currentPixel & 0xffff;
#else
			colorA = (currentPixel & 0xffff0000) >> 16;
#endif

#ifdef LSB_FIRST
			colorB = (currentPixel & 0xffff0000) >> 16;
			*(dP) = product = colorA |
					  ((((colorA & colorMask) >> 1) +
					    ((colorB & colorMask) >> 1) +
					    (colorA & colorB & lowPixelMask)) << 16);
#else
			colorB = currentPixel & 0xffff;
			*(dP) = product = (colorA << 16) | 
					  (((colorA & colorMask) >> 1) +
					   ((colorB & colorMask) >> 1) +
					   (colorA & colorB & lowPixelMask));
#endif
                        if(IPPU.RenderedScreenHeight<=SNES_HEIGHT_EXTENDED){
                            darkened = (product = ((product & colorMask) >> 1));
                            darkened += (product = ((product & colorMask) >> 1));
                            darkened += (product & colorMask) >> 1;
                            *(nL) = darkened;
                        }

#ifdef LSB_FIRST
			colorA = nextPixel & 0xffff;
			*(dP + 1) = product = colorB |
					      ((((colorA & colorMask) >> 1) +
						((colorB & colorMask) >> 1) +
						(colorA & colorB & lowPixelMask)) << 16);
#else
			colorA = (nextPixel & 0xffff0000) >> 16;
			*(dP + 1) = product = (colorB << 16) | 
					       (((colorA & colorMask) >> 1) +
						((colorB & colorMask) >> 1) + 
						(colorA & colorB & lowPixelMask));
#endif
                        if(IPPU.RenderedScreenHeight<=SNES_HEIGHT_EXTENDED){
                            darkened = (product = ((product & colorMask) >> 1));
                            darkened += (product = ((product & colorMask) >> 1));
                            darkened += (product & colorMask) >> 1;
                            *(nL + 1) = darkened;
                        }
		    }

		    dP += 2;
		    nL += 2;
		}
		while ((uint8 *) bP < finish);

		deltaPtr += GFX.Pitch;
		srcPtr += GFX.Pitch;
                if(IPPU.RenderedScreenHeight<=SNES_HEIGHT_EXTENDED){
                    dstPtr += GUI.output_pitch * 2;
                    nextLine += GUI.output_pitch * 2;
                } else {
                    dstPtr += GUI.output_pitch;
                    nextLine += GUI.output_pitch;
                }
	    }
	    while (--height);
	}
	else
	{
	    do
	    {
		uint32 *bP = (uint32 *) srcPtr;
		uint32 *xP = (uint32 *) deltaPtr;
		uint32 *dP = (uint32 *) dstPtr;
		uint32 currentPixel;

		finish = (uint8 *) bP + ((width + 2) << 1);

		do
		{
		    currentPixel = *bP++;

		    if (currentPixel != *xP++)
		    {
			uint32 product, darkened;

			*(xP - 1) = currentPixel;
			*dP = currentPixel;
                        if(IPPU.RenderedScreenHeight<=SNES_HEIGHT_EXTENDED){
                            darkened = (product = ((currentPixel & colorMask) >> 1));
                            darkened += (product = ((product & colorMask) >> 1));
                            darkened += (product & colorMask) >> 1;
                            *(uint32 *) ((uint8 *) dP + GUI.output_pitch) = darkened;
                        }
		    }

		    dP++;
		}
		while ((uint8 *) bP < finish);

		deltaPtr += GFX.Pitch;
		srcPtr += GFX.Pitch;
                if(IPPU.RenderedScreenHeight<=SNES_HEIGHT_EXTENDED){
                    dstPtr += GUI.output_pitch * 2;
                } else {
                    dstPtr += GUI.output_pitch;
                }
	    }
	    while (--height);
	}
    }
    }
}

#ifdef USE_DGA_EXTENSION
void CreateFullScreenWindow ()
{
    int major, minor;

    XF86.full_screen_available = False;

    if (XF86DGAQueryVersion (GUI.display, &major, &minor))
    {
	int fd;

	// Need to test for access to /dev/mem here because XF86DGAGetVideo
	// just calls exit if it can't access this device.
	if ((fd = open ("/dev/mem", O_RDWR) < 0))
	{
	    perror ("Can't open \"/dev/mem\", full screen mode not available");
	    return;
	}
	else
	    close (fd);

	XF86DGAGetVideo (GUI.display, GUI.screen_num, &XF86.vram,
			 &XF86.line_width, &XF86.bank_size,
			 &XF86.size);

	XF86.full_screen_available = True;

	XSetWindowAttributes attributes;

	attributes.override_redirect = True;
	attributes.background_pixel = BlackPixelOfScreen (GUI.screen);
	XF86.fs_window = XCreateWindow (GUI.display, RootWindowOfScreen (GUI.screen),
				   0, 0, WidthOfScreen (GUI.screen), 
				   HeightOfScreen (GUI.screen),
				   0, GUI.depth,
				   InputOutput, GUI.visual, 
				   CWOverrideRedirect | CWBackPixel,
				   &attributes);

	XF86.window_width = WidthOfScreen (GUI.screen);
	XF86.window_height = HeightOfScreen (GUI.screen);

#ifdef USE_VIDMODE_EXTENSION
	XF86VidModeModeLine current;
	int dot_clock;

	if (!XF86.no_mode_switch &&
	    XF86VidModeGetAllModeLines (GUI.display, GUI.screen_num,
					&XF86.num_modes,
					&XF86.all_modes) &&
	    XF86VidModeGetModeLine (GUI.display, GUI.screen_num,
				    &dot_clock, &current))
	{
	    int i;

	    XF86.orig.dotclock = dot_clock;
	    XF86.orig.hdisplay = current.hdisplay;
	    XF86.orig.hdisplay = current.hdisplay;
	    XF86.orig.hsyncstart = current.hsyncstart;
	    XF86.orig.hsyncend = current.hsyncend;
	    XF86.orig.htotal = current.htotal;
	    XF86.orig.vdisplay = current.vdisplay;
	    XF86.orig.vsyncstart = current.vsyncstart;
	    XF86.orig.vsyncend = current.vsyncend;
	    XF86.orig.vtotal = current.vtotal;
	    XF86.orig.flags = current.flags;
	    XF86.orig.c_private = current.c_private;
	    XF86.orig.privsize = current.privsize;

	    int best_width_so_far = current.hdisplay;
	    int best_height_so_far = current.vdisplay;
	    XF86.best = NULL;
	    XF86.switch_video_mode = False;
	    
	    for (i = 0; i < XF86.num_modes; i++)
	    {
		if (XF86.all_modes [i]->hdisplay >= IMAGE_WIDTH &&
		    XF86.all_modes [i]->hdisplay <= best_width_so_far &&
		    XF86.all_modes [i]->vdisplay >= IMAGE_HEIGHT &&
		    XF86.all_modes [i]->vdisplay <= best_height_so_far &&
		    (XF86.all_modes [i]->hdisplay != current.hdisplay ||
		     XF86.all_modes [i]->vdisplay != current.vdisplay))
		{
		    best_width_so_far = XF86.all_modes [i]->hdisplay;
		    best_height_so_far = XF86.all_modes [i]->vdisplay;
		    XF86.best = XF86.all_modes [i];
		}
	    }
	    if (XF86.best)
		XF86.switch_video_mode = True;
	    else
		XF86.best = &XF86.orig;

	    XF86.window_width = XF86.best->hdisplay;
	    XF86.window_height = XF86.best->vdisplay;
	}
#endif
    }
}

void S9xSwitchToFullScreen (bool8 enable)
{
    if (XF86.full_screen_available && enable != XF86.is_full_screen)
    {
	S9xTextMode ();
	XF86.is_full_screen = enable;
	if (GUI.delta_screen)
	{
	    uint32 *p = (uint32 *) GUI.delta_screen;
	    uint32 *q = (uint32 *) (GUI.delta_screen + GFX.Pitch * 478);
	    while (p < q)
		*p++ ^= ~0;
	}
	S9xGraphicsMode ();

	if (enable)
	{
	    XF86.saved_image_needs_scaling = GUI.image_needs_scaling;
	    XF86.saved_window_width = GUI.window_width;
	    XF86.saved_window_height = GUI.window_height;
	    GUI.bytes_per_line = XF86.line_width * GUI.bytes_per_pixel;

	    if (XF86.scale)
	    {
		GUI.image_date = (uint8 *) XF86.vram;
		GUI.window_width = XF86.window_width;
		GUI.window_height = XF86.window_height;
		GUI.image_needs_scaling = TRUE;
	    }
	    else
	    {
		// Centre image in available width/height
		GUI.image_date = (uint8 *) XF86.vram +
			     ((XF86.window_width - IMAGE_WIDTH) / 2) * GUI.bytes_per_pixel +
			     ((XF86.window_height - IMAGE_HEIGHT) / 2) * GUI.bytes_per_line;
		GUI.image_needs_scaling = FALSE;
	    }
	}
	else
	{
	    GUI.image_needs_scaling = XF86.saved_image_needs_scaling;
	    GUI.window_width = XF86.saved_window_width;
	    GUI.window_height = XF86.saved_window_height;
	    GUI.image_date = (uint8 *) GUI.image->data;
	    GUI.bytes_per_line = GUI.image->bytes_per_line;
	}
    }
}
#endif

#ifdef USE_GLIDE
void S9xSwitchToGlideMode (bool8 enable)
{
    S9xGlideEnable (enable);
    if (Settings.GlideEnable)
	XGrabKeyboard (GUI.display, GUI.window, True, GrabModeAsync, GrabModeAsync,
		       CurrentTime);
    else
	XUngrabKeyboard (GUI.display, CurrentTime);
}
#endif

bool S9xDisplayPollButton(uint32 id, bool *pressed){
    return false;
}
bool S9xDisplayPollAxis(uint32 id, int16 *value){
    return false;
}
bool S9xDisplayPollPointer(uint32 id, int16 *x, int16 *y){
    int d, n;
    
    if((id&0xc0008000)!=0x40008000) return false;
    d=(id>>24)&0x3f;
    n=id&0x7fff;
    if(d!=0 || n!=0) return false;
    *x=GUI.mouse_x;
    *y=GUI.mouse_y;
    return true;
}


s9xcommand_t S9xGetDisplayCommandT(const char *n){
    s9xcommand_t cmd;
    cmd.type=S9xBadMapping;
    cmd.multi_press=0;
    cmd.button_norpt=0;
    cmd.port[0]=0xff;
    cmd.port[1]=0;
    cmd.port[2]=0;
    cmd.port[3]=0;
    if(!strcmp(n, "OpenGLCube")){
        cmd.type=S9xButtonPort;
        cmd.port[1]=0;
    } else if(!strcmp(n, "FullscreenToggle")){
        cmd.type=S9xButtonPort;
        cmd.port[1]=1;
    } else if(!strcmp(n, "FullscreenOn")){
        cmd.type=S9xButtonPort;
        cmd.port[1]=2;
    } else if(!strcmp(n, "FullscreenOff")){
        cmd.type=S9xButtonPort;
        cmd.port[1]=3;
    }
    return cmd;
}

char *S9xGetDisplayCommandName(s9xcommand_t cmd){
    switch(cmd.type){
      case S9xButtonPort:
        if(cmd.port[0]!=0xff) break;
        switch(cmd.port[1]){
          case 0:
            return strdup("OpenGLCube");
          case 1:
            return strdup("FullscreenToggle");
          case 2:
            return strdup("FullscreenOn");
          case 3:
            return strdup("FullscreenOff");
        }
        break;
      case S9xAxisPort:
        break;
      case S9xPointerPort:
        break;
    }
    return strdup("None");
}

void S9xHandleDisplayCommand(s9xcommand_t cmd, int16 data1, int16 data2){
    if(cmd.type==S9xButtonPort){
        if(cmd.port[0]!=0xff) return;
        if(data1){
            switch(cmd.port[1]){
              case 0:  /* OpenGLCube */
#ifdef USE_OPENGL
                OpenGL.draw_cube ^= TRUE;
#endif
                break;
#ifdef USE_DGA_EXTENSION
              case 1:  /* FullscreenToggle */
                S9xSwitchToFullScreen (TRUE);
                break;
              case 2:  /* FullscreenOn */
                S9xSwitchToFullScreen (FALSE);
                break;
              case 3:  /* FullscreenOff */
                S9xSwitchToFullScreen (!XF86.is_full_screen);
                break;
#endif
            }
        }
    }
}

bool8 S9xMapDisplayInput(const char *n, s9xcommand_t *cmd){
    int i, j, d;
    char *c;

    if(!isdigit(n[1]) || !isdigit(n[2]) || n[3]!=':') goto unrecog;
    d=((n[1]-'0')*10+(n[2]-'0'))<<24;
    
    switch(n[0]){
      case 'K':
        {
            KeyCode kc;
            KeySym ks;
            d|=0x00000000;
            for(i=4; n[i]!='\0' && n[i]!='+'; i++);
            if(n[i]=='\0' || i==4){
                i=4;
            } else {
                for(i=4; n[i]!='+'; i++){
                    switch(n[i]){
                      case 'S': d|=ShiftMask<<8; break;
                      case 'C': d|=ControlMask<<8; break;
                      case 'A': d|=Mod1Mask<<8; break;
                      case 'M': d|=Mod4Mask<<8; break;
                      default: goto unrecog;
                    }
                }
                i++;
            }
            if((ks=XStringToKeysym(n+i))==NoSymbol) goto unrecog;
            if((kc=XKeysymToKeycode(GUI.display, ks))==0) goto unrecog;
            d|=kc&0xff;
        }
        return S9xMapButton(d, *cmd, false);

      case 'M':
        d|=0x40000000;
        if(!strncmp(n+4, "Pointer", 7)){
            d|=0x8000;
            if(n[11]=='\0') return S9xMapPointer(d, *cmd, true);
            i=11;
        } else if(n[4]=='B'){
            i=5;
        } else goto unrecog;
        d|=j=strtol(n+i, &c, 10);
        if(c!=NULL && *c!='\0' || j>0x7fff) goto unrecog;
        if(d&0x8000) return S9xMapPointer(d, *cmd, true);
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
    if(pass!=1) return "Unix/X11";
    if(!conf.GetBool("Controls::ClearAll", false)){
        keymaps.push_back(strpair_t("K00:k", "Joypad1 Right"));
        keymaps.push_back(strpair_t("K00:Right", "Joypad1 Right"));
        keymaps.push_back(strpair_t("K00:h", "Joypad1 Left"));
        keymaps.push_back(strpair_t("K00:Left", "Joypad1 Left"));
        keymaps.push_back(strpair_t("K00:j", "Joypad1 Down"));
        keymaps.push_back(strpair_t("K00:n", "Joypad1 Down"));
        keymaps.push_back(strpair_t("K00:Down", "Joypad1 Down"));
        keymaps.push_back(strpair_t("K00:u", "Joypad1 Up"));
        keymaps.push_back(strpair_t("K00:Up", "Joypad1 Up"));
        keymaps.push_back(strpair_t("K00:Return", "Joypad1 Start"));
        keymaps.push_back(strpair_t("K00:space", "Joypad1 Select"));
        keymaps.push_back(strpair_t("K00:S+d", "Joypad1 ToggleTurbo A"));
        keymaps.push_back(strpair_t("K00:C+d", "Joypad1 ToggleSticky A"));
        keymaps.push_back(strpair_t("K00:d", "Joypad1 A"));
        keymaps.push_back(strpair_t("K00:S+c", "Joypad1 ToggleTurbo B"));
        keymaps.push_back(strpair_t("K00:C+c", "Joypad1 ToggleSticky B"));
        keymaps.push_back(strpair_t("K00:c", "Joypad1 B"));
        keymaps.push_back(strpair_t("K00:S+s", "Joypad1 ToggleTurbo X"));
        keymaps.push_back(strpair_t("K00:C+s", "Joypad1 ToggleSticky X"));
        keymaps.push_back(strpair_t("K00:s", "Joypad1 X"));
        keymaps.push_back(strpair_t("K00:S+x", "Joypad1 ToggleTurbo Y"));
        keymaps.push_back(strpair_t("K00:C+x", "Joypad1 ToggleSticky Y"));
        keymaps.push_back(strpair_t("K00:x", "Joypad1 Y"));
        keymaps.push_back(strpair_t("K00:S+a", "Joypad1 ToggleTurbo L"));
        keymaps.push_back(strpair_t("K00:S+v", "Joypad1 ToggleTurbo L"));
        keymaps.push_back(strpair_t("K00:C+a", "Joypad1 ToggleSticky L"));
        keymaps.push_back(strpair_t("K00:C+v", "Joypad1 ToggleSticky L"));
        keymaps.push_back(strpair_t("K00:a", "Joypad1 L"));
        keymaps.push_back(strpair_t("K00:v", "Joypad1 L"));
        keymaps.push_back(strpair_t("K00:S+z", "Joypad1 ToggleTurbo R"));
        keymaps.push_back(strpair_t("K00:C+z", "Joypad1 ToggleSticky R"));
        keymaps.push_back(strpair_t("K00:z", "Joypad1 R"));

        keymaps.push_back(strpair_t("K00:KP_Left", "Joypad2 Left"));
        keymaps.push_back(strpair_t("K00:KP_Right", "Joypad2 Right"));
        keymaps.push_back(strpair_t("K00:KP_Up", "Joypad2 Up"));
        keymaps.push_back(strpair_t("K00:KP_Down", "Joypad2 Down"));
        keymaps.push_back(strpair_t("K00:KP_Enter", "Joypad2 Start"));
        keymaps.push_back(strpair_t("K00:KP_Add", "Joypad2 Select"));
        keymaps.push_back(strpair_t("K00:Prior", "Joypad2 A"));
        keymaps.push_back(strpair_t("K00:Next", "Joypad2 B"));
        keymaps.push_back(strpair_t("K00:Home", "Joypad2 X"));
        keymaps.push_back(strpair_t("K00:End", "Joypad2 Y"));
        keymaps.push_back(strpair_t("K00:Insert", "Joypad2 L"));
        keymaps.push_back(strpair_t("K00:Delete", "Joypad2 R"));

        keymaps.push_back(strpair_t("K00:A+Escape", "Debugger"));
        keymaps.push_back(strpair_t("K00:CS+Escape", "Reset"));
        keymaps.push_back(strpair_t("K00:S+Escape", "SoftReset"));
        keymaps.push_back(strpair_t("K00:Escape", "ExitEmu"));
        keymaps.push_back(strpair_t("K00:Tab", "EmuTurbo"));
        keymaps.push_back(strpair_t("K00:S+Tab", "ToggleEmuTurbo"));
        keymaps.push_back(strpair_t("K00:Scroll_Lock", "Pause"));
        keymaps.push_back(strpair_t("K00:Pause", "Pause"));

        keymaps.push_back(strpair_t("K00:A+Pause", "DumpSPC7110Log"));
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
        keymaps.push_back(strpair_t("K00:S+6", "OpenGLCube"));
        keymaps.push_back(strpair_t("K00:8", "BGLayeringHack"));
        keymaps.push_back(strpair_t("K00:S+9", "Mode7Interpolate"));
        keymaps.push_back(strpair_t("K00:9", "ToggleTransparency"));
        keymaps.push_back(strpair_t("K00:minus", "DecFrameRate"));
        keymaps.push_back(strpair_t("K00:S+minus", "DecFrameTime"));
        keymaps.push_back(strpair_t("K00:C+minus", "DecTurboSpeed"));
        keymaps.push_back(strpair_t("K00:A+minus", "DecEmuTurbo"));
        keymaps.push_back(strpair_t("K00:equal", "IncFrameRate"));
        keymaps.push_back(strpair_t("K00:S+equal", "IncFrameTime"));
        keymaps.push_back(strpair_t("K00:C+equal", "IncTurboSpeed"));
        keymaps.push_back(strpair_t("K00:A+equal", "IncEmuTurbo"));
        keymaps.push_back(strpair_t("K00:BackSpace", "ClipWindows"));
        keymaps.push_back(strpair_t("K00:Print", "Screenshot"));
        keymaps.push_back(strpair_t("K00:A+f", "FullscreenToggle"));

        keymaps.push_back(strpair_t("K00:bracketleft", "InterpolateSound"));
        keymaps.push_back(strpair_t("K00:bracketright", "SynchronizeSound"));
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

        keymaps.push_back(strpair_t("M00:B0", "{Mouse1 L,Superscope Fire,Justifier1 Trigger}"));
        keymaps.push_back(strpair_t("M00:B1", "{Justifier1 AimOffscreen Trigger,Superscope AimOffscreen}"));
        keymaps.push_back(strpair_t("M00:B2", "{Mouse1 R,Superscope Cursor,Justifier1 Start}"));
        keymaps.push_back(strpair_t("M00:Pointer", "Pointer Mouse1+Superscope+Justifier1"));
        keymaps.push_back(strpair_t("K00:grave", "Superscope ToggleTurbo"));
        keymaps.push_back(strpair_t("K00:slash", "Superscope Pause"));
    }

    GUI.no_repeat=!conf.GetBool("Unix/X11::SetKeyRepeat", true);
    if(conf.Exists("Unix/X11::Interpolate")){
        GUI.interpolate=conf.GetUInt("Unix/X11::Interpolate", 1000);
        if(GUI.interpolate>5) GUI.interpolate=1;
        Settings.SupportHiRes = TRUE;
        Settings.ForceTransparency = TRUE;
    }
    GUI.scale=conf.GetBool("Unix/X11::Scale", false);
#ifdef USE_DGA_EXTENSION
    XF86.scale = GUI.scale;
#ifdef USE_VIDMODE_EXTENSION
    XF86.no_mode_switch=!conf.GetBool("Unix/X11::DGAModeSwitch", true);
#endif
    XF86.start_full_screen=conf.GetBool("Unix/X11::StartFullscreen", false);
#endif

    return "Unix/X11";
}
