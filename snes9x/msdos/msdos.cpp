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

#include <allegro.h>
#undef TRUE
#undef FALSE

#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "swinder.h"
#include "sb.h"

#include "scancode.h"

#include <dpmi.h>
#include <go32.h>

#include "snes9x.h"
#include "memmap.h"
#include "debug.h"
#include "ppu.h"
#include "snapshot.h"
#include "gfx.h"
#include "display.h"
#include "apu.h"
#include "soundux.h"

#ifdef NETPLAY_SUPPORT
#include "netplay.h"
#endif

#define COUNT(a) (sizeof(a) / sizeof(a[0]))

#define sti() __asm__ __volatile__ ("sti": : :"memory")
#define cli() __asm__ __volatile__ ("cli": : :"memory")

static int screen_pitch = 0;
uint32 screen_width = 0;
uint32 screen_height = 0;
static int mode = -1;
static bool8 planar = FALSE;
static bool8 in_text_mode = TRUE;
static bool8 stretch = FALSE;
BITMAP *off_screen;
static BITMAP *sub_screen;
static uint8 *Delta = NULL;
static int num_sidewinders = 0;
static bool8 grip_initialised = FALSE;
extern int NumControllers;
static bool8 mouse_installed = FALSE;
static int prev_mouse_x = 0;
static int prev_mouse_y = 0;
static uint32 superscope_turbo = 0;
static uint32 superscope_pause = 0;
static int mouse_offset_x = 0;
static int mouse_offset_y = 0;
static double mouse_scale_h = 1.0;
static double mouse_scale_v = 1.0;
static bool8 wait_for_vsync = FALSE;
static bool8 interpolate = FALSE;

uint32 last_rendered_width = 0;
uint32 last_rendered_height = 0;

#ifdef GRIP_SUPPORT
#include "grip.h"
void InitGrip ();
void ReadGrip ();
#endif

#ifdef SIDEWINDER_SUPPORT
void InitSidewinders ();
int ReadSidewinders ();
extern bool8 alternate_sidewinder_mapping;
#endif

extern uint32 joypads [5];
extern BITMAP *screen;

void SaveScreenshot ();

#ifdef JOYSTICK_SUPPORT
extern bool8 joystick_has_four_buttons;
extern bool8 joystick_has_six_buttons;
extern bool8 joystick_has_eight_buttons;
#endif

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
    {320, 240, GFX_MODEX},	    // 0
    {320, 200, GFX_VGA},	    // 1
    {256, 256, GFX_VGA},	    // 2
    {640, 480, GFX_AUTODETECT},	    // 3
    {640, 480, GFX_VESA1},	    // 4
    {640, 480, GFX_VESA2B},	    // 5
    {640, 480, GFX_VESA2L},	    // 6
    {640, 400, GFX_XTENDED},	    // 7
    {800, 600, GFX_AUTODETECT},	    // 8

    {320, 240, GFX_AUTODETECT},	    // 9
    {640, 480, GFX_AUTODETECT},	    // 10
    {512, 384, GFX_AUTODETECT},	    // 11
    {800, 600, GFX_AUTODETECT}	    // 12
};

void S9xGraphicsMode ()
{
    if (in_text_mode)
    {
	if (mode < 0)
	{
	    if (Settings.SixteenBit)
	    {
		if (Settings.SupportHiRes || interpolate)
		    mode = 10;
		else
		    mode = 9;
	    }
	    else
	    {
		if (Settings.SupportHiRes)
		    mode = 3;
		else
		    mode = 2;
	    }
	}

	int ret;
	do
	{
	    screen_width = modes [mode].width;
	    screen_height = modes [mode].height;

	    set_color_depth (Settings.SixteenBit ? 16: 8);
	    if (modes [mode].mode == GFX_VGA)
		ret = set_gfx_mode (modes [mode].mode, 320, 200, 0, 0);
	    else
		ret = set_gfx_mode (modes [mode].mode, modes [mode].width,
				    modes [mode].height, 0, 0);
	} while (ret < 0 && ++mode < COUNT(modes));

	planar = modes [mode].mode == GFX_MODEX;
	    
	if (ret < 0)
	{
	    fprintf (stderr, "Unable to switch to requested screen mode/resolution:\n%s\n",
		    allegro_error);
	    S9xExit ();
	}

	if (modes [mode].mode == GFX_VGA && screen_width == 256)
	{
	    outRegArray (scr256x256, sizeof (scr256x256) / sizeof (Register));
	    screen->w = screen->cr = 256;
	    screen->h = screen->cb = 256;
	    for (int i = 1; i < 256; i++)
		screen->line[i] = screen->line[i - 1] + 256;
	}

	clear_to_color (screen, 0);
    
	if (install_keyboard ())
	{
	    set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
	    fprintf (stdout, "Keyboard initialisation failed.\n");
	    S9xExit ();
	}
	if (!install_mouse ())
	{
	    mouse_installed = TRUE;
	    set_mouse_range (0, 0, screen_width, screen_height);
	    position_mouse (screen_width / 2, screen_height / 2);
	    prev_mouse_x = mouse_x;
	    prev_mouse_y = mouse_y;
	}
	else
	    mouse_installed = FALSE;
	in_text_mode = FALSE;
    }
}

void S9xTextMode ()
{
    if (!in_text_mode)
    {
	set_color_depth (8);
	remove_keyboard ();
	remove_mouse ();
	set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
	in_text_mode = TRUE;
    }
}

void S9xInitDisplay (int argc, char **argv)
{
    if (allegro_init () < 0)
    {
	fprintf (stdout, "Unable to initialise Allegro library.\n");
	S9xExit ();
    }

    extern int i_love_bill;

    if (mode >= 9 || (mode < 0 && Settings.SixteenBit))
    {
	set_color_depth (16);
	Settings.Transparency = TRUE;
	Settings.SixteenBit = TRUE;
    }
    else
    {
	set_color_depth (8);
	Settings.Transparency = FALSE;
	Settings.SixteenBit = FALSE;
    }
    
    /*i_love_bill = FALSE;*/

    off_screen = create_bitmap (IMAGE_WIDTH + 2, IMAGE_HEIGHT);

    GFX.Screen = (uint8 *) off_screen->dat;
    if (!GFX.Screen)
    {
	fprintf (stdout, "Cannot allocate screen buffer.\n");
	S9xExit ();
    }
    GFX.Pitch = off_screen->w;
    if (Settings.SixteenBit)
    {
	GFX.Pitch *= 2;
	sub_screen = create_bitmap ((IMAGE_WIDTH + 2), IMAGE_HEIGHT);
	Delta = new uint8 [(IMAGE_WIDTH + 2) * IMAGE_HEIGHT * 2];
	if (!sub_screen || !Delta)
	{
	    fprintf (stdout, "Cannot allocate screen buffer.\n");
	    S9xExit ();
	}
	GFX.SubScreen = (uint8 *) sub_screen->dat;
	GFX.ZBuffer = (uint8 *) malloc (off_screen->w * IMAGE_HEIGHT);
	GFX.SubZBuffer = (uint8 *) malloc (off_screen->w * IMAGE_HEIGHT);
    }
    else
    {
	GFX.SubScreen = GFX.Screen;
	GFX.ZBuffer = (uint8 *) malloc (off_screen->w * IMAGE_HEIGHT);
	GFX.SubZBuffer = NULL;
	sub_screen = NULL;
	Delta = NULL;
    }
    screen_pitch = off_screen->w;
}

void S9xDeinitDisplay ()
{
    S9xTextMode ();
#ifdef GRIP_SUPPORT
    if (grip_initialised)
    {
	GrShutdown ();
	GrUnlink ();
    }
#endif
    destroy_bitmap (off_screen);
    if (sub_screen)
	destroy_bitmap (sub_screen);
    
    delete Delta;
    if (GFX.ZBuffer)
	free ((char *) GFX.ZBuffer);
    if (GFX.SubZBuffer)
	free ((char *) GFX.SubZBuffer);
    GFX.ZBuffer = NULL;
    GFX.SubZBuffer = NULL;
}

void S9xInitInputDevices ()
{
    if (Settings.JoystickEnabled)
    {
#ifdef GRIP_SUPPORT
	InitGrip ();
	if (grip_initialised)
	    num_sidewinders = 0;
	else
#endif

#ifdef SIDEWINDER_SUPPORT
	{
#ifdef GRIP_SUPPORT
	    usleep (20000);
#endif
	    InitSidewinders ();
	}
#else
	{ /* empty */ }
#endif

#ifdef JOYSTICK_SUPPORT
	if (joystick_has_four_buttons)
	    joy_type = JOY_TYPE_4BUTTON;
	else
	if (joystick_has_six_buttons)
	    joy_type = JOY_TYPE_6BUTTON;
	else
	if (joystick_has_eight_buttons)
	    joy_type = JOY_TYPE_8BUTTON;
	else
	    joy_type = JOY_TYPE_2PADS;
	if (num_sidewinders == 0 && !grip_initialised)
	    initialise_joystick ();
#endif
    }
}

#ifdef JOYSTICK_SUPPORT
void InitJoysticks ()
{
}

static int old_joy_left = 0;
static int old_joy_right = 0;
static int old_joy_up = 0;
static int old_joy_down = 0;
static int old_joy_b1 = 0;
static int old_joy_b2 = 0;
static int old_joy_b3 = 0;
static int old_joy_b4 = 0;
static int old_joy_b5 = 0;
static int old_joy_b6 = 0;
static int old_joy_b7 = 0;
static int old_joy_b8 = 0;
static int old_joy2_left = 0;
static int old_joy2_right = 0;
static int old_joy2_up = 0;
static int old_joy2_down = 0;
static int old_joy2_b1 = 0;
static int old_joy2_b2 = 0;

void ReadJoysticks ()
{
    if (num_sidewinders == 0 && !grip_initialised)
    {
	poll_joystick ();
	if (!joystick_has_four_buttons && !joystick_has_six_buttons &&
	    !joystick_has_eight_buttons)
	{
	    if (joy_b1 ^ old_joy_b1)
	    {
		if (joy_b1)
		    joypads [0] |= SNES_A_MASK;
		else
		    joypads [0] &= ~SNES_A_MASK;
	    }
	    if (joy_b2 ^ old_joy_b2)
	    {
		if (joy_b2)
		    joypads [0] |= SNES_B_MASK;
		else
		    joypads [0] &= ~SNES_B_MASK;
	    }
	}
	else
	{
	    if (joy_b1 ^ old_joy_b1)
	    {
		if (joy_b1)
		    joypads [0] |= SNES_Y_MASK;
		else
		    joypads [0] &= ~SNES_Y_MASK;
	    }
	    if (joy_b2 ^ old_joy_b2)
	    {
		if (joy_b2)
		    joypads [0] |= SNES_X_MASK;
		else
		    joypads [0] &= ~SNES_X_MASK;
	    }
	}
	if (joy_b3 ^ old_joy_b3)
	{
	    if (joy_b3)
		joypads [0] |= SNES_B_MASK;
	    else
		joypads [0] &= ~SNES_B_MASK;
	}
	if (joy_b4 ^ old_joy_b4)
	{
	    if (joy_b4)
		joypads [0] |= SNES_A_MASK;
	    else
		joypads [0] &= ~SNES_A_MASK;
	}
	if (joy_b5 ^ old_joy_b5)
	{
	    if (joy_b5)
		joypads [0] |= SNES_TL_MASK;
	    else
		joypads [0] &= ~SNES_TL_MASK;
	}
	if (joy_b6 ^ old_joy_b6)
	{
	    if (joy_b6)
		joypads [0] |= SNES_TR_MASK;
	    else
		joypads [0] &= ~SNES_TR_MASK;
	}
	if (joy_b7 ^ old_joy_b7)
	{
	    if (joy_b7)
		joypads [0] |= SNES_SELECT_MASK;
	    else
		joypads [0] &= ~SNES_SELECT_MASK;
	}
	if (joy_b8 ^ old_joy_b8)
	{
	    if (joy_b8)
		joypads [0] |= SNES_START_MASK;
	    else
		joypads [0] &= ~SNES_START_MASK;
	}
	if (joy_left ^ old_joy_left)
	{
	    if (joy_left)
		joypads [0] |= SNES_LEFT_MASK;
	    else
		joypads [0] &= ~SNES_LEFT_MASK;
	}
	if (joy_right ^ old_joy_right)
	{
	    if (joy_right)
		joypads [0] |= SNES_RIGHT_MASK;
	    else
		joypads [0] &= ~SNES_RIGHT_MASK;
	}
	if (joy_up ^ old_joy_up)
	{
	    if (joy_up)
		joypads [0] |= SNES_UP_MASK;
	    else
		joypads [0] &= ~SNES_UP_MASK;
	}
	if (joy_down ^ old_joy_down)
	{
	    if (joy_down)
		joypads [0] |= SNES_DOWN_MASK;
	    else
		joypads [0] &= ~SNES_DOWN_MASK;
	}

	if (!joystick_has_four_buttons && !joystick_has_six_buttons &&
	    !joystick_has_eight_buttons)
	{
	    if (joy2_b1 ^ old_joy2_b1)
	    {
		if (joy2_b1)
		    joypads [1] |= SNES_A_MASK;
		else
		    joypads [1] &= ~SNES_A_MASK;
	    }
	    if (joy2_b2 ^ old_joy2_b2)
	    {
		if (joy2_b2)
		    joypads [1] |= SNES_B_MASK;
		else
		    joypads [1] &= ~SNES_B_MASK;
	    }
	    if (joy2_left ^ old_joy2_left)
	    {
		if (joy2_left)
		    joypads [1] |= SNES_LEFT_MASK;
		else
		    joypads [1] &= ~SNES_LEFT_MASK;
	    }
	    if (joy2_right ^ old_joy2_right)
	    {
		if (joy2_right)
		    joypads [1] |= SNES_RIGHT_MASK;
		else
		    joypads [1] &= ~SNES_RIGHT_MASK;
	    }
	    if (joy2_up ^ old_joy2_up)
	    {
		if (joy2_up)
		    joypads [1] |= SNES_UP_MASK;
		else
		    joypads [1] &= ~SNES_UP_MASK;
	    }
	    if (joy2_down ^ old_joy2_down)
	    {
		if (joy2_down)
		    joypads [1] |= SNES_DOWN_MASK;
		else
		    joypads [1] &= ~SNES_DOWN_MASK;
	    }
	}
	old_joy_left = joy_left;
	old_joy_right = joy_right;
	old_joy_up = joy_up;
	old_joy_down = joy_down;
	old_joy_b1 = joy_b1;
	old_joy_b2 = joy_b2;
	old_joy_b3 = joy_b3;
	old_joy_b4 = joy_b4;
	old_joy_b5 = joy_b5;
	old_joy_b6 = joy_b6;
	old_joy_b7 = joy_b7;
	old_joy_b8 = joy_b8;
	old_joy2_left = joy2_left;
	old_joy2_right = joy2_right;
	old_joy2_up = joy2_up;
	old_joy2_down = joy2_down;
	old_joy2_b1 = joy2_b1;
	old_joy2_b2 = joy2_b2;
    }
}
#endif

void S9xSetPalette ()
{
    uint16 Brightness = IPPU.MaxBrightness * 138;
    PALLETE p;
    for (int i = 0; i < 256; i++)
    {
	p[i].r = (((PPU.CGDATA [i] >> 0) & 0x1F) * Brightness) >> 10;
	p[i].g = (((PPU.CGDATA [i] >> 5) & 0x1F) * Brightness) >> 10;
	p[i].b = (((PPU.CGDATA [i] >> 10) & 0x1F) * Brightness) >> 10;
    }
    set_palette_range (p, 0, 255, FALSE);
}

void S9xProcessEvents (bool8 block)
{
    static char prev_keystate[128];
    extern volatile char key[128];

#ifdef GRIP_SUPPORT
    ReadGrip ();
#endif

#ifdef SIDEWINDER_SUPPORT
    if (num_sidewinders)
	ReadSidewinders ();
#endif

    char key1[128];
    char *keystate = (char *) key1;
    int fn = 0;
    
    memcpy (key1, (char *) key, sizeof (key1));

#undef KEY_DOWN
#define KEY_DOWN(a) (keystate[a])
#undef KEY_PRESS
#define KEY_PRESS(a) (keystate[a] && !prev_keystate[a])
#undef KEY_WASPRESSED
#define KEY_WASPRESSED(a) (prev_keystate[a] && !keystate[a])
#undef PROCESS_KEY
#define PROCESS_KEY(k, b, v)\
if (KEY_PRESS(k)) b |= v;\
if (KEY_WASPRESSED(k)) b &= ~v;

    if (KEY_PRESS (SCANCODE_ESCAPE))
	S9xExit ();

    // Joypad 1:
    PROCESS_KEY(SCANCODE_K,		    joypads [0], SNES_RIGHT_MASK)
    PROCESS_KEY(SCANCODE_CURSORRIGHT,	    joypads [0], SNES_RIGHT_MASK)
    PROCESS_KEY(SCANCODE_H,		    joypads [0], SNES_LEFT_MASK)
    PROCESS_KEY(SCANCODE_CURSORLEFT,	    joypads [0], SNES_LEFT_MASK)
    PROCESS_KEY(SCANCODE_N,		    joypads [0], SNES_DOWN_MASK)
    PROCESS_KEY(SCANCODE_J,		    joypads [0], SNES_DOWN_MASK)
    PROCESS_KEY(SCANCODE_CURSORDOWN,	    joypads [0], SNES_DOWN_MASK)
    PROCESS_KEY(SCANCODE_U,		    joypads [0], SNES_UP_MASK)
    PROCESS_KEY(SCANCODE_CURSORUP,	    joypads [0], SNES_UP_MASK)
    PROCESS_KEY(SCANCODE_ENTER,		    joypads [0], SNES_START_MASK)
    PROCESS_KEY(SCANCODE_SPACE,		    joypads [0], SNES_SELECT_MASK)

    PROCESS_KEY(SCANCODE_A,		    joypads [0], SNES_TL_MASK)
    PROCESS_KEY(SCANCODE_V,		    joypads [0], SNES_TL_MASK)
    PROCESS_KEY(SCANCODE_Q,		    joypads [0], SNES_TL_MASK)

    PROCESS_KEY(SCANCODE_Z,		    joypads [0], SNES_TR_MASK)
    PROCESS_KEY(SCANCODE_B,		    joypads [0], SNES_TR_MASK)
    PROCESS_KEY(SCANCODE_W,		    joypads [0], SNES_TR_MASK)

    PROCESS_KEY(SCANCODE_S,		    joypads [0], SNES_X_MASK)
    PROCESS_KEY(SCANCODE_M,		    joypads [0], SNES_X_MASK)
    PROCESS_KEY(SCANCODE_E,		    joypads [0], SNES_X_MASK)

    PROCESS_KEY(SCANCODE_X,		    joypads [0], SNES_Y_MASK)
    PROCESS_KEY(SCANCODE_COMMA,		    joypads [0], SNES_Y_MASK)
    PROCESS_KEY(SCANCODE_R,		    joypads [0], SNES_Y_MASK)

    PROCESS_KEY(SCANCODE_D,		    joypads [0], SNES_A_MASK)
    PROCESS_KEY(SCANCODE_PERIOD,	    joypads [0], SNES_A_MASK)
    PROCESS_KEY(SCANCODE_T,		    joypads [0], SNES_A_MASK)

    PROCESS_KEY(SCANCODE_C,		    joypads [0], SNES_B_MASK)
    PROCESS_KEY(SCANCODE_SLASH,		    joypads [0], SNES_B_MASK)
    PROCESS_KEY(SCANCODE_Y,		    joypads [0], SNES_B_MASK)
    
    // Joypad 2:
//    PROCESS_KEY(SCANCODE_CURSORRIGHT,	    joypads [1], SNES_RIGHT_MASK)
//    PROCESS_KEY(SCANCODE_CURSORLEFT,	    joypads [1], SNES_LEFT_MASK)
//    PROCESS_KEY(SCANCODE_CURSORDOWN,	    joypads [1], SNES_DOWN_MASK)
//    PROCESS_KEY(SCANCODE_CURSORUP,	    joypads [1], SNES_UP_MASK)
    PROCESS_KEY(SCANCODE_KEYPADENTER,	    joypads [0], SNES_START_MASK)
    PROCESS_KEY(SCANCODE_KEYPADPLUS,	    joypads [0], SNES_SELECT_MASK)
    PROCESS_KEY(SCANCODE_INSERT,	    joypads [0], SNES_X_MASK)
    PROCESS_KEY(SCANCODE_REMOVE,	    joypads [0], SNES_Y_MASK)
    PROCESS_KEY(SCANCODE_HOME,		    joypads [0], SNES_A_MASK)
    PROCESS_KEY(SCANCODE_END,		    joypads [0], SNES_B_MASK)
    PROCESS_KEY(SCANCODE_PAGEUP,	    joypads [0], SNES_TL_MASK)
    PROCESS_KEY(SCANCODE_PAGEDOWN,	    joypads [0], SNES_TR_MASK)
    
    if (KEY_PRESS (SCANCODE_0))
	Settings.DisableHDMA = !Settings.DisableHDMA;
    if (KEY_PRESS (SCANCODE_1))
	PPU.BG_Forced ^= 1;
    if (KEY_PRESS (SCANCODE_2))
	PPU.BG_Forced ^= 2;
    if (KEY_PRESS (SCANCODE_3))
	PPU.BG_Forced ^= 4;
    if (KEY_PRESS (SCANCODE_4))
	PPU.BG_Forced ^= 8;
    if (KEY_PRESS (SCANCODE_5))
	PPU.BG_Forced ^= 16;
    if (KEY_PRESS (SCANCODE_6))
	Settings.SwapJoypads = !Settings.SwapJoypads;
    if (KEY_PRESS (SCANCODE_7))
    {
	if (IPPU.Controller == SNES_SUPERSCOPE)
	    show_mouse (NULL);
	S9xNextController ();
	if (IPPU.Controller == SNES_SUPERSCOPE)
	    show_mouse (screen);
    }
    if (KEY_PRESS (SCANCODE_8))
	Settings.BGLayering = !Settings.BGLayering;
    if (KEY_PRESS (SCANCODE_9))
	if (Settings.SixteenBit)
	    Settings.Transparency = !Settings.Transparency;

    if (KEY_PRESS(SCANCODE_TAB))
	superscope_turbo = !superscope_turbo;
    PROCESS_KEY(SCANCODE_GRAVE, superscope_pause, 1);

    if (KEY_PRESS(SCANCODE_F1))
	fn = 1;
    if (KEY_PRESS(SCANCODE_F2))
	fn = 2;
    if (KEY_PRESS(SCANCODE_F3))
	fn = 3;
    if (KEY_PRESS(SCANCODE_F4))
	fn = 4;
    if (KEY_PRESS(SCANCODE_F5))
	fn = 5;
    if (KEY_PRESS(SCANCODE_F6))
	fn = 6;
    if (KEY_PRESS(SCANCODE_F7))
	fn = 7;
    if (KEY_PRESS(SCANCODE_F8))
	fn = 8;
    if (KEY_PRESS(SCANCODE_F9))
	fn = 9;
    if (KEY_PRESS(SCANCODE_F10))
	fn = 10;
    if (KEY_PRESS(SCANCODE_F11))
	fn = 11;
    if (KEY_PRESS(SCANCODE_F12))
	fn = 12;
	
    if (fn > 0)
    {
	if (!KEY_DOWN(SCANCODE_LEFTALT) && !KEY_DOWN(SCANCODE_LEFTSHIFT))
	{
	    if (fn == 11)
	    {
		S9xLoadSnapshot (S9xChooseFilename (TRUE));
	    }
	    else if (fn == 12)
	    {
		Snapshot (S9xChooseFilename (FALSE));
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
			 fn - 1);
		S9xLoadSnapshot (filename);
	    }
	}
	else if (KEY_DOWN(SCANCODE_LEFTALT))
	{
	    if (fn >= 4)
		S9xToggleSoundChannel (fn - 4);
#ifdef DEBUGGER
	    else if (fn == 1)
		CPU.Flags |= DEBUG_MODE_FLAG;
#endif
	    else if (fn == 2)
		S9xLoadSnapshot (S9xChooseFilename (TRUE));
	    else if (fn == 3)
		Snapshot (S9xChooseFilename (FALSE));
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
		     fn - 1);
	    Snapshot (filename);
	}
    }

    if (KEY_PRESS (SCANCODE_BREAK) || KEY_PRESS (SCANCODE_BREAK_ALTERNATIVE) ||
	KEY_PRESS (SCANCODE_SCROLLLOCK))
	Settings.Paused ^= 1;

    if (KEY_PRESS (SCANCODE_PRINTSCREEN))
	SaveScreenshot ();

    if (KEY_PRESS (SCANCODE_MINUS))
    {
	if (Settings.SkipFrames <= 1)
	    Settings.SkipFrames = AUTO_FRAMERATE;
	else
	if (Settings.SkipFrames != AUTO_FRAMERATE)
	    Settings.SkipFrames--;
    }

    if (KEY_PRESS (SCANCODE_EQUAL))
    {
	if (Settings.SkipFrames == AUTO_FRAMERATE)
	    Settings.SkipFrames = 1;
	else
	if (Settings.SkipFrames < 10)
	    Settings.SkipFrames++;
    }
	
    memcpy (prev_keystate, keystate, sizeof (prev_keystate));

    if (block)
	__dpmi_yield ();
}

void S9xSetTitle (const char *title)
{
}

void S9xPutImage (int width, int height)
{
    //extern volatile char key[128];

    if (IPPU.Controller == SNES_SUPERSCOPE)
	scare_mouse ();

    if (wait_for_vsync)
    {
	vsync ();
	//while ((inportb (0x3da) & 8) && !key [SCANCODE_ESCAPE]);
	//while (!(inportb (0x3da) & 8) && !key [SCANCODE_ESCAPE]);
    }

    if (interpolate && Settings.SixteenBit && screen_width >= 512 &&
	height < 240)
    {
	void TVMode (uint8 *srcPtr, uint32 srcPitch,
		     uint8 *deltaPtr,
		     BITMAP *dstBitmap, int width, int height);

	if (width != last_rendered_width || height != last_rendered_height)
	    memset (Delta, 0xff, (IMAGE_WIDTH + 2) * IMAGE_HEIGHT * 2);
	TVMode (GFX.Screen, GFX.Pitch, Delta, screen, width, height);
    }
    else
    if (stretch)
    {
	stretch_blit (off_screen, screen, 0, 0, width, height,
		      0, 0, screen_width, screen_height);
	mouse_offset_x = 0;
	mouse_offset_y = 0;
	mouse_scale_h = screen_width / (double) width;
	mouse_scale_v = screen_height / (double) height;
    }
    else
    if (Settings.SupportHiRes)
    {
	if (screen_width < width || screen_height < height)
	{
	    stretch_blit (off_screen, screen, 0, 0, width, height,
			  0, 0, screen_width, screen_height);
	    mouse_offset_x = 0;
	    mouse_offset_y = 0;
	    mouse_scale_h = screen_width / (double) width;
	    mouse_scale_v = screen_height / (double) height;
	}
	else
	{
	    int h = height;
	    int w = width;

	    if (h < 240)
		h <<= 1;
	    if (w == 256)
		w = 512;
	    if (w > screen_width)
		w = screen_width;
	    if (h > screen_height)
		h = screen_height;
	    mouse_offset_x = (screen_width - w) >> 1;
	    mouse_offset_y = (screen_height - h) >> 1;
	    stretch_blit (off_screen, screen, 0, 0, width, height,
			  mouse_offset_x, mouse_offset_y, w, h);
	    mouse_scale_h = w / (double) width;
	    mouse_scale_v = h / (double) height;
	}
    }
    else
    {
	if (modes [mode].mode == GFX_VGA && screen_width == 256)
	{
	    int y_buff;
	    int y_start;
	    int y_end;
	    int x_start = (screen_width - width) >> 1;

	    mouse_offset_x = 0;
	    mouse_scale_h = 1.0;
	    mouse_scale_v = 1.0;
	    if (screen_height >= height)
	    {
		mouse_offset_y = y_start = (screen_height - height) >> 1;
		y_end = y_start + height;
		y_buff = 0;
	    }
	    else
	    {
		y_start = 0;
		y_end = screen_height;
		y_buff = (height - screen_height) >> 1;
	    }
	    uint8 *s = GFX.Screen + screen_pitch * y_buff;
	    uint8 *p = screen->line[0] + screen->w * y_start + x_start;
	    for (int y = y_start; y < y_end; y++, s += screen_pitch, p += screen->w)
		movedata (_my_ds(), (unsigned int) s, 
			  (unsigned int) screen->seg, (unsigned int) p, 
			  width);
	}
	else
	{
	    mouse_offset_x = (screen_width - width) >> 1;
	    mouse_offset_y = (screen_height - height) >> 1;
	    mouse_scale_h = 1.0;
	    mouse_scale_v = 1.0;
	    blit (off_screen, screen, 0, 0, mouse_offset_x, mouse_offset_y,
		  width, height);
	}
    }
    if (IPPU.Controller == SNES_SUPERSCOPE)
	unscare_mouse ();

    last_rendered_width = width;
    last_rendered_height = height;
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
	    inportb (STATUS_ADDR);
	    /* ensure VGA output is enabled */
	    outportb (ATTRCON_ADDR, r.index | 0x20);
	    outportb (ATTRCON_ADDR, r.value);
	    break;

	case MISC_ADDR:
	case VGAENABLE_ADDR:
	    /*	directly to the port */
	    outportb (r.port, r.value);
	    break;

	case SEQ_ADDR:
	case GRACON_ADDR:
	case CRTC_ADDR:
	default:
	    /*	index to port			   */
	    outportb (r.port, r.index);
	    /*	value to port+1 		   */
	    outportb (r.port + 1, r.value);
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

    outportb (0x3d4, 0x11);
    v = inportb (0x3d5) & 0x7f;
    outportb (0x3d4, 0x11);
    outportb (0x3d5, v);
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

void S9xExtraUsage ()
{
    printf ("\
-m  num     Screen mode:\n\
            0 - 320x240 (modex, slower), 1 - 320x200 (faster but clipped)\n\
            2 - 256x256 (fastest but non-standard), 3-6 - 640x480, 7 - 640x400\n\
            8 - 800x600, 9 - 320x240 16 bit, 10 - 640x480 16 bit\n");
    printf ("\
-scale      Scale SNES screen to fit S-VGA screen\n");
}

void S9xParseDisplayArg (char **argv, int &ind, int)
{
    if ((strcmp (argv [ind], "-m") == 0 ||
	 strcasecmp (argv [ind], "-mode") == 0 ) && argv [ind + 1])
    {
	mode = atoi (argv [++ind]);
	if (mode >= sizeof (modes) / sizeof (modes [0]))
	    mode = 0;
    }
    else
    if (strcasecmp (argv [ind], "-scale") == 0 ||
	strcasecmp (argv [ind], "-sc") == 0)
	stretch = TRUE;
    else 
    if (strcasecmp (argv [ind], "-w") == 0)
	wait_for_vsync = TRUE;
    else
    if (strcasecmp (argv [ind], "-y") == 0)
    {
	Settings.SixteenBit = TRUE;
	Settings.SupportHiRes = TRUE;
	Settings.Transparency = TRUE;
	interpolate = TRUE;
    }
    else
	S9xUsage ();
}

volatile uint32 FrameTimer = 0;

static void TimerInterrupt (...)
{
    FrameTimer++;
}
static END_OF_FUNCTION (TimerInterrupt);

void InitTimer ()
{
    install_timer ();
    LOCK_VARIABLE (FrameTimer);
    LOCK_FUNCTION (TimerInterrupt);
    if (install_int (TimerInterrupt, Settings.FrameTime / 1000) < 0)
    {
	printf ("Snes9X: Dynamic speed limiting not available\n");
	Settings.SkipFrames = 3;
    }
}

int S9xMinCommandLineArgs ()
{
    return (2);
}

#include "display.h"

static int Rates[8] =
{
    0, 8192, 11025, 16500, 22050, 29300, 36600, 44000
};

bool8 S9xOpenSoundDevice (int mode, bool8 stereo, int buffer_size)
{
    int playback_rate = Rates [mode & 7];
    int sixteen_bit = TRUE;
    int in_stereo = stereo;

    so.mute_sound = TRUE;

    if (sx_sb_init (&in_stereo, &sixteen_bit, &playback_rate,
		    &buffer_size) < 0)
    {
	return (FALSE);
    }
    so.stereo = in_stereo;
    so.sixteen_bit = sixteen_bit;
    so.playback_rate = playback_rate;
    so.buffer_size = buffer_size;
    so.encoded = FALSE;

    atexit (sx_sb_exit);

    /* Lock variables into memory */
    
    void S9xMixSamples_end();
    void S9xMixSamplesO_end();
    void MixStereo (int);
    void MixStereo_end();
    void MixMono (int);
    void MixMono_end();
    void S9xAPUSetEndOfSample (int, Channel *);
    void S9xAPUSetEndOfSample_end();
    void S9xAPUSetEndX (int);
    void S9xAPUSetEndX_end();
    void S9xSetEnvelopeRate_end();
    void S9xSetEnvRate (Channel *, int32, int32);
    void S9xSetEnvRate_end ();
    
    extern int Echo [24000];
    extern int DummyEchoBuffer [MAX_BUFFER_SIZE];
    extern int MixBuffer [MAX_BUFFER_SIZE];
    extern int EchoBuffer [MAX_BUFFER_SIZE];
    extern int FilterTaps [8];
    extern unsigned long FIRIndex;
    extern int Loop [16];

    LOCK_FUNCTION (S9xMixSamples);
    LOCK_FUNCTION (S9xMixSamplesO);
    LOCK_FUNCTION (MixStereo);
    LOCK_FUNCTION (MixMono);
    LOCK_FUNCTION (S9xAPUSetEndX);
    LOCK_FUNCTION (S9xAPUSetEndOfSample);
    LOCK_FUNCTION (S9xSetEnvelopeRate);
    LOCK_FUNCTION (S9xSetEnvRate);
    LOCK_VARIABLE (APU);
    LOCK_VARIABLE (IAPU);
    LOCK_VARIABLE (SoundData);
    LOCK_VARIABLE (so);
    LOCK_VARIABLE (Echo);
    LOCK_VARIABLE (DummyEchoBuffer);
    LOCK_VARIABLE (MixBuffer);
    LOCK_VARIABLE (EchoBuffer);
    LOCK_VARIABLE (FilterTaps);
    LOCK_VARIABLE (FIRIndex);
    LOCK_VARIABLE (Loop);

    _go32_dpmi_lock_data ((void *) IAPU.RAM, 64 * 1024);
    _go32_dpmi_lock_data ((void *) IAPU.ShadowRAM, 64 * 1024);
    _go32_dpmi_lock_data ((void *) IAPU.CachedSamples, 256 * 1024);
    printf ("Rate: %d, Buffer size: %d, 16-bit: %s, Stereo: %s, Encoded: %s\n",
	    so.playback_rate, so.buffer_size, so.sixteen_bit ? "yes" : "no",
	    so.stereo ? "yes" : "no", so.encoded ? "yes" : "no");

    so.mute_sound = FALSE;

    return (TRUE);
}

void S9xGenerateSound ()
{
}

bool8 S9xReadMousePosition (int which1, int &x, int &y, uint32 &buttons)
{
    if (which1 == 0)
    {
	buttons = mouse_b;
	prev_mouse_x += mouse_x - screen_width / 2;
	prev_mouse_y += mouse_y - screen_height / 2;
	position_mouse (screen_width / 2, screen_height / 2);
	x = prev_mouse_x;
	y = prev_mouse_y;
	return (TRUE);
    }
    return (FALSE);
}

bool8 S9xReadSuperScopePosition (int &x, int &y, uint32 &buttons)
{
    buttons = (mouse_b & 3) | (superscope_turbo << 2) |
	      (superscope_pause << 3);
    x = (int) ((mouse_x - mouse_offset_x) / mouse_scale_h);
    y = (int) ((mouse_y - mouse_offset_y) / mouse_scale_v);

    return (TRUE);
}

#ifdef SIDEWINDER_SUPPORT
#define INITIAL_TIMEOUT 200
#define FIRST_STROBE_TIMEOUT 160
#define MIN_FIRST_STOBE_LEN 32
#define TIMEOUT 30
#define INTER_SIDEWINDER_STROBE_LEN 16

#define MODE_A_STROBES 15
#define MODE_B_STROBES 5
#define MAX_STROBES_PER_SIDEWINDER MODE_A_STROBES

#define SW_PORT	    0x201

extern int SidewinderToSNES [];
extern int SidewinderToSNES2 [];
int ReadSidewinders ();

void InitSidewinders ()
{
    num_sidewinders = ReadSidewinders ();
}

int ReadSidewinders ()
{
    static SW_READ_DATA prev_ret = {0};

    int j, i;
    unsigned char b;
    char *t;
    int strobes = 0;
    int start;
    unsigned char data [SW_MAX_SIDEWINDERS * MAX_STROBES_PER_SIDEWINDER * 2];
    unsigned char *p = data;
    unsigned char *pend = p + sizeof (data);
    struct SW_READ_DATA ret;
    unsigned int buttons;
    unsigned int parity;
    
    for (i = 0; i < SW_MAX_SIDEWINDERS; i++)
    {
	ret.buttons [i] = 0;
	ret.valid [i] = 0;
    }
	
    ret.count = 0;
    j = 0;
    
    cli ();
    outportb ((uint16) SW_PORT, (uint8) 0xff);
    b = inportb ((uint16) SW_PORT);
    b = inportb ((uint16) SW_PORT);

    if (!(b & 0x10))
    {
	/* Search for bit 4 going from 0 to 1 */
	do
	{
	    b = inportb ((uint16) SW_PORT);
	    if (++j >= INITIAL_TIMEOUT)
		goto exit;
	} while (!(b & 0x10));
    }
	    
    j = 0;
    do
    {
	b = inportb ((uint16) SW_PORT);
	if (++j > FIRST_STROBE_TIMEOUT)
	    goto exit;
    } while (b & 0x10);
    
    do
    {
	j = 0;
	do
	{
	    b = inportb ((uint16) SW_PORT);
	    if (++j > TIMEOUT)
	    {
		*p = j;
		*(p + 1) = b;
		p += 2;
		goto exit;
	    }
	} while (!(b & 0x10));
	*p = j;
	*(p + 1) = b;
	p += 2;

	j = 0;
	do
	{
	    b = inportb ((uint16) SW_PORT);
	    if (++j > TIMEOUT)
		goto exit;
	} while (b & 0x10);
    } while (p < pend);
	    
exit:
    sti ();
    start = 0;
    strobes = 0;
    for (i = 0; i < p - data; i += 2)
    {
	if (data [i] >= INTER_SIDEWINDER_STROBE_LEN)
	{
	    if (strobes == MODE_A_STROBES || strobes == MODE_B_STROBES)
	    {
		buttons = 0;
		if (strobes == MODE_A_STROBES)
		{
		    for (j = 0; j < MODE_A_STROBES; j++)
			buttons |= (((data [start + 1 + (j << 1)] >> 5) ^ 1) & 1) << j;
		}
		else
		{
		    for (j = 0; j < MODE_B_STROBES; j++)
			buttons |= (((data [start + 1 + (j << 1)] >> 5) ^ 7) & 7) << (j * 3);
		}
		parity = 0;
		for (j = 0; j < SW_NUM_SIDEWINDER_BUTTONS; j++)
		    if (buttons & (1 << j))
			parity++;

		if ((parity & 1) != ((buttons >> SW_NUM_SIDEWINDER_BUTTONS) & 1))
		{
		    /* No partity error, update the joypad button state */
		    ret.valid [ret.count] = 1;
		    ret.buttons [ret.count++] = buttons & ~SW_PARITY_MASK;
		}
		else
		    ret.valid [ret.count++] = 0;
	    }
	    else
		ret.valid [ret.count++] = 0;
	    start = i;
	    strobes = 1;
	}
	else
	    strobes++;
    }
    if (ret.count)
    {
	if (ret.count < 2)
	    NumControllers = 2;
	else
	{
	    if (ret.count > NumControllers)
	    {
		NumControllers = ret.count;
		if (NumControllers > 5)
		    NumControllers = 5;
	    }
	}

	for (int s = 0; s < ret.count && s < 5; s++)
	{
	    if (ret.valid [s])
	    {
		for (int i = 0; i < SW_NUM_SIDEWINDER_BUTTONS; i++)
		{
		    if ((prev_ret.buttons [s] ^ ret.buttons [s]) & (1 << i))
		    {
			if (alternate_sidewinder_mapping)
			{
			    if (ret.buttons [s] & (1 << i))
				joypads [s] |= SidewinderToSNES2 [i];
			    else
				joypads [s] &= ~SidewinderToSNES2 [i];
			}
			else
			{
			    if (ret.buttons [s] & (1 << i))
				joypads [s] |= SidewinderToSNES [i];
			    else
				joypads [s] &= ~SidewinderToSNES [i];
			}
		    }
		}
		prev_ret.buttons [s] = ret.buttons [s];
	    }
	}
    }
    return (ret.count);
}
#endif

#ifdef GRIP_SUPPORT
void InitGrip ()
{
    char buffer [_MAX_PATH];
    FILE *fs = NULL;
    const char *grip_dir = getenv ("GRIP");
    uint8 *grip_library;
    int size;

    if (grip_dir)
    {
	strcpy (buffer, grip_dir);
	strcat (buffer, "\\");
	strcat (buffer, "GRIP.GLL");
	fs = fopen (buffer, "rb");
    }

    if (!fs)
	fs = fopen ("GRIP.GLL", "rb");

    if (!fs)
	fs = fopen ("C:\\GRIP\\GRIP.GLL", "rb");

    if (!fs)
	fs = fopen ("C:\\GRAVIS\\GRIP\\GRIP.GLL", "rb");

    if (fs)
    {
	fseek (fs, 0, SEEK_END);
	size = ftell (fs);
	if (size > 32 * 1024)
	{
	    fclose (fs);
	    return;
	}
	grip_library = new uint8 [size];
	fseek (fs, 0, SEEK_SET);
	if (fread (grip_library, 1, size, fs) != size)
	{
	    delete grip_library;
	    fclose (fs);
	    return;
	}
	fclose (fs);
	if (!GrLink (grip_library, size))
	{
	    delete grip_library;
	    return;
	}
	delete grip_library;
	if (!GrInitialize ())
	{
	    GrUnlink ();
	    return;
	}
	grip_initialised = TRUE;
	printf ("Grip Initialised\n");
    }
}

struct GripData
{
    GRIP_BITFIELD buttons;
    GRIP_BITFIELD direction;
};

static uint32 GamePadProToSNES [10] = {
    SNES_TR_MASK, SNES_TL_MASK, SNES_X_MASK, SNES_A_MASK,
    SNES_Y_MASK, SNES_TL_MASK, SNES_B_MASK, SNES_TR_MASK,
    SNES_START_MASK, SNES_SELECT_MASK
};

static uint32 GamePadProToSNESDir [4] = {
    SNES_RIGHT_MASK, SNES_LEFT_MASK, SNES_UP_MASK, SNES_DOWN_MASK
};

static struct GripData grip_prev_data [5] = { {0, 0} };

void ReadGrip ()
{
    if (grip_initialised)
    {
	GrRefresh (0);
	int count = 0;
	int slot;
	GRIP_BITFIELD connected = GrGetSlotMap ();
	for (slot = 1; slot < 5; slot++)
	{
	    if (connected & (1 << slot))
	    {
		char product [17];
		product [0] = 0;
		GrGetProductName (slot, product);
		if (strcasecmp (product, "GamePad Pro") == 0)
		{
		    GRIP_BITFIELD buttons;
		    buttons = GrGetPackedValues (slot, GRIP_CLASS_BUTTON, 0, 9);
		    int x = GrGetValue (slot, GRIP_CLASS_AXIS, 0);
		    int y = GrGetValue (slot, GRIP_CLASS_AXIS, 1);
		    GRIP_BITFIELD direction = 0;

		    if (x > 1)
			direction |= 1;
		    else
		    if (x < 1)
			direction |= 2;
		    if (y > 1)
			direction |= 4;
		    else
		    if (y < 1)
			direction |= 8;
			
		    int i;
		    for (i = 0; i < 10; i++)
		    {
			if ((grip_prev_data [slot].buttons ^ buttons) & (1 << i))
			{
			    if (buttons & (1 << i))
				joypads [count] |= GamePadProToSNES [i];
			    else
				joypads [count] &= ~GamePadProToSNES [i];
			}
		    }
		    for (i = 0; i < 4; i++)
		    {
			if ((grip_prev_data [slot].direction ^ direction) & (1 << i))
			{
			    if (direction & (1 << i))
				joypads [count] |= GamePadProToSNESDir [i];
			    else
				joypads [count] &= ~GamePadProToSNESDir [i];
			}
		    }
		    
		    grip_prev_data [slot].buttons = buttons;
		    grip_prev_data [slot].direction = direction;
		}
		else
		{
		}
		count++;
	    }
	    if (count < 2)
		NumControllers = 2;
	    else
	    if (count > NumControllers)
		NumControllers = count;
	}
    }
}
#endif

void _makepath (char *path, const char *drive, const char *dir,
		const char *fname, const char *ext)
{
    if (drive && *drive)
    {
	*path = *drive;
	*(path + 1) = ':';
	*(path + 2) = 0;
    }
    else
	*path = 0;
	
    if (dir && *dir)
    {
	strcat (path, dir);
	if (strlen (dir) != 1 || *dir != '\\')
	    strcat (path, SLASH_STR);
    }
	
    strcat (path, fname);
    if (ext && *ext)
    {
        strcat (path, ".");
        strcat (path, ext);
    }
}

void _splitpath (const char *path, char *drive, char *dir, char *fname,
		 char *ext)
{
    if (*path && *(path + 1) == ':')
    {
	*drive = toupper (*path);
	path += 2;
    }
    else
	*drive = 0;

    char *slash = strrchr (path, SLASH_CHAR);
    if (!slash)
	slash = strrchr (path, '/');
    char *dot = strrchr (path, '.');
    if (dot && slash && dot < slash)
	dot = NULL;

    if (!slash)
    {
	if (*drive)
	    strcpy (dir, "\\");
	else
	    strcpy (dir, "");
	strcpy (fname, path);
        if (dot)
        {
	    *(fname + (dot - path)) = 0;
	    strcpy (ext, dot + 1);
        }
	else
	    strcpy (ext, "");
    }
    else
    {
	if (*drive && *path != '\\')
	{
	    strcpy (dir, "\\");
	    strcat (dir, path);
	    *(dir + (slash - path) + 1) = 0;
	}
	else
	{
	    strcpy (dir, path);
	    if (slash - path == 0)
		*(dir + 1) = 0;
	    else
		*(dir + (slash - path)) = 0;
	}

	strcpy (fname, slash + 1);
        if (dot)
	{
	    *(fname + (dot - slash) - 1) = 0;
    	    strcpy (ext, dot + 1);
	}
	else
	    strcpy (ext, "");
    }
}

extern volatile uint32 FrameTimer;

void S9xSyncSpeed ()
{
#ifdef NETPLAY_SUPPORT
    if (Settings.NetPlay)
    {
	// XXX: Send joypad position update to server
	// XXX: Wait for heart beat from server
	S9xNetPlaySendJoypadUpdate (joypads [0]);
	if (!S9xNetPlayCheckForHeartBeat ())
	{
	    do
	    {
		CHECK_SOUND ();
		S9xProcessEvents (FALSE);
		__dpmi_yield ();
	    } while (!S9xNetPlayCheckForHeartBeat ());
	    IPPU.RenderThisFrame = TRUE;
	    IPPU.SkippedFrames = 0;
	}
	else
	{
	    if (IPPU.SkippedFrames < 10)
	    {
		IPPU.SkippedFrames++;
		IPPU.RenderThisFrame = FALSE;
	    }
	    else
	    {
		IPPU.RenderThisFrame = TRUE;
		IPPU.SkippedFrames = 0;
	    }
	}
    }
    else
#endif
    if (Settings.SkipFrames == AUTO_FRAMERATE)
    {
	while (FrameTimer == 0)
	{
	    S9xProcessEvents (FALSE);
	    __dpmi_yield ();
	}

	if (FrameTimer > 10)
	    FrameTimer = 10;
	if (FrameTimer > 1 && IPPU.SkippedFrames < 10)
	{
	    IPPU.SkippedFrames++;
	    IPPU.RenderThisFrame = FALSE;
	}
	else
	{
	    IPPU.RenderThisFrame = TRUE;
	    IPPU.SkippedFrames = 0;
	}
	FrameTimer--;
    }
    else
    {
	if (++IPPU.FrameSkip >= Settings.SkipFrames)
	{
	    IPPU.FrameSkip = 0;
	    IPPU.RenderThisFrame = TRUE;
	    IPPU.SkippedFrames = 0;
	}
	else
	{
	    IPPU.SkippedFrames++;
	    IPPU.RenderThisFrame = FALSE;
	}
    }
}

void S9xMessage (int type, int message_no, const char *str)
{
    switch (type)
    {
    case S9X_ERROR:
    case S9X_FATAL_ERROR:
	fprintf (stderr, "%s\n", str);
	return;
    case S9X_TRACE:
    case S9X_DEBUG:
	printf ("%s\n", str);
	break;

    default:
	switch (message_no)
	{
	case S9X_ROM_INFO:
	case S9X_HEADERS_INFO:
	case S9X_ROM_CONFUSING_FORMAT_INFO:
	case S9X_ROM_INTERLEAVED_INFO:
	case S9X_USAGE:
	    fprintf (stderr, "%s\n", str);
	    return;
	}
    }
}

