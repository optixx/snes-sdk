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

/* Snes9x libggi target was written by Bisqwit (http://iki.fi/bisqwit/)
 * Fri Nov 19 1999 17:41
 */

#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/stat.h>

#include <ggi/ggi.h>
#include <ggi/keyboard.h>

#include "snes9x.h"
#include "memmap.h"
#include "debug.h"
#include "ppu.h"
#include "snapshot.h"
#include "gfx.h"
#include "display.h"
#include "apu.h"

#include "pixform.h"

static bool8 stretch = FALSE;
static bool8 text_mode = TRUE;
static bool8 pause_mode = FALSE;
static bool8 interpolate = FALSE;

static int dihdah;
static int kbdtrans_enabled;
static int saved_fcntl;

extern uint32 joypads [5];

static ggi_visual_t vis;
static ggi_mode     vis_mode;

static ggi_color init_pal16[16];

START_EXTERN_C
uint8 snes9x_clear_change_log = 0;
END_EXTERN_C

#define DEFINE_DEST(type, k) static type Buf[k*SNES_WIDTH];register type *d = Buf
static void Convert16to15(void)
{
  for (register int y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    DEFINE_DEST(uint16, 1);
    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register int x = 0; x < SNES_WIDTH; x++) {
      register uint16 pixel = *s++;
      
      *d++ = (((pixel/RED_LOW_BIT_MASK  ) & MAX_RED  ) * (0x20/(MAX_RED  +1)))
           | (((pixel/GREEN_LOW_BIT_MASK) & MAX_GREEN) * (0x400/(MAX_GREEN+1)))
           | (((pixel/BLUE_LOW_BIT_MASK ) & MAX_BLUE ) * (0x8000/(MAX_BLUE +1)));
    }
    ggiPutHLine(vis, 0,y, SNES_WIDTH, Buf);
  }
}
static void Convert16to16(void)
{
  for (register int y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    DEFINE_DEST(uint16, 1);
    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register int x = 0; x < SNES_WIDTH; x++)
      *d++ = *s++;
    ggiPutHLine(vis, 0,y, SNES_WIDTH, Buf);
  }
}
static void Convert16to24(void)
{
  for (register int y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    DEFINE_DEST(uint8, 3);
    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register int x = 0; x < SNES_WIDTH; x++) {
      register uint16 pixel = *s++;
      #if 0
      *d++ = (pixel & 0x1F) << 3;
      *d++ = ((pixel >> 5) & 0x3f) << 2;
      *d++ = ((pixel >> 11) & 0x1f) << 3;
      #else
      *d++ = (pixel & MAX_RED) * (256/(MAX_RED+1));
      *d++ = ((pixel / (MAX_RED+1)) & MAX_GREEN) * (256/(MAX_GREEN+1));
      *d++ = ((pixel / ((MAX_RED+1)*(MAX_GREEN+1))) & MAX_BLUE) * (256/(MAX_BLUE+1));
      #endif
    }
    ggiPutHLine(vis, 0,y, SNES_WIDTH, Buf);
  }
}
static void Convert16to32(void)
{
  for (register int y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    DEFINE_DEST(uint32, 1);
    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register int x = 0; x < SNES_WIDTH; x++) {
      register uint16 pixel = *s++;
      #if 0
      *d++ = ((pixel & 0x1f) << 3)
           | ((((pixel >> 5) & 0x3f) << 2) << 8)
           | ((((pixel >> 11) & 0x1f) << 3) << 16);
      #else
      #if 0
      *d++ = (x*256/SNES_WIDTH) << 8;
      #else
#if __BYTE_ORDER == __BIG_ENDIAN
      *d++ = (((pixel/RED_LOW_BIT_MASK  ) & MAX_RED  ) * (0x100    /(MAX_RED  +1)))
           | (((pixel/GREEN_LOW_BIT_MASK) & MAX_GREEN) * (0x10000  /(MAX_GREEN+1)))
           | (((pixel/BLUE_LOW_BIT_MASK ) & MAX_BLUE ) * (0x1000000/(MAX_BLUE +1)))
#else
      *d++ = (((pixel/RED_LOW_BIT_MASK  ) & MAX_RED  ) * (0x1000000/(MAX_RED  +1)))
           | (((pixel/GREEN_LOW_BIT_MASK) & MAX_GREEN) * (0x10000  /(MAX_GREEN+1)))
           | (((pixel/BLUE_LOW_BIT_MASK ) & MAX_BLUE ) * (0x100    /(MAX_BLUE +1)))
#endif
        ;
      #endif
      #endif
    }
    ggiPutHLine(vis, 0,y, SNES_WIDTH, Buf);
  }
}
static void Convert8to8(void)
{
  for (register int y = 0; y < SNES_HEIGHT_EXTENDED; y++)
  {
    ggiPutHLine(vis, 0,y, SNES_WIDTH, (GFX.Screen + y * GFX.RealPitch));
  }
}
static void Convert16to8(void)
{
  fprintf(stderr, "16 -> 8 bit conversion not implemented yet.\n");
}
#undef DEFINE_DEST

int S9xMinCommandLineArgs ()
{
    return 3;
}

static unsigned Bitness(void)
{
    unsigned depth = GT_DEPTH(vis_mode.graphtype);
    unsigned size = GT_SIZE(vis_mode.graphtype);
    if(size==32)depth=32;
    return depth;
}

void S9xGraphicsMode ()
{
    if (!vis)
    {
        if(!(vis = ggiOpen(NULL)))
        {
            fprintf(stderr, "ggiOpen() failed.\n");
            S9xExit();
        }
    }
    
    if(text_mode)
    {
        fprintf(stderr, "%d: ", __LINE__); ggiFPrintMode(stderr, &vis_mode); fprintf(stderr, "\n");
        
        ggiCheckMode(vis, &vis_mode);
        fprintf(stderr, "%d: ", __LINE__); ggiFPrintMode(stderr, &vis_mode); fprintf(stderr, "\n");
//      S9xExit();

        if(ggiSetMode(vis, &vis_mode) < 0)
        {
            fprintf(stderr, "ggiSetMode() failed.\n");
            S9xExit();
        }
        
        if(Bitness() == 8)
            ggiGetPalette(vis, 0,16, init_pal16);
        
        S9xSetPalette();
        
        ggiSetFlags(vis, GGIFLAG_ASYNC);
        
        text_mode = FALSE;
    }
}

void S9xTextMode ()
{
    if (text_mode == FALSE)
    {
        ggiClose(vis);
        text_mode = TRUE;
        vis = 0;
    }
}

static void CheckFirst(void)
{
    static int has_been_initialized = 0;
    if(!has_been_initialized)
    {
        has_been_initialized = 1;
        ggiParseMode("", &vis_mode);
    }
}

void S9xInitDisplay (int argc, char **argv)
{
    argc=argc, argv=argv;
    
    if(ggiInit() != 0)
    {
        fprintf(stderr, "ggiInit() failed.\n");
        S9xExit();
    }
    
    CheckFirst();
    
    if(!GT_DEPTH(vis_mode.graphtype))
        GT_SETDEPTH(vis_mode.graphtype, GT_8BIT);

    Settings.Transparency = GT_DEPTH(vis_mode.graphtype) != 8;
    Settings.SixteenBit   = GT_DEPTH(vis_mode.graphtype) != 8;
    Settings.SupportHiRes = 0; //interpolate;

    fprintf(stderr, "%d: ", __LINE__); ggiFPrintMode(stderr, &vis_mode); fprintf(stderr, "\n");
        
    GFX.Pitch =  IMAGE_WIDTH * (GT_DEPTH(vis_mode.graphtype) == 8 ? 1 : 2);
    GFX.Screen    = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
    GFX.SubScreen = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
    GFX.ZBuffer    = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
    GFX.SubZBuffer = (uint8 *) malloc (GFX.Pitch * IMAGE_HEIGHT);
}

void S9xDeinitDisplay ()
{
    S9xTextMode();
    ggiExit();
    
    if(GFX.Screen)    { free (GFX.Screen);    GFX.Screen = NULL; }
    if(GFX.SubScreen) { free (GFX.SubScreen); GFX.SubScreen = NULL; }
    if(GFX.ZBuffer)   { free (GFX.ZBuffer);   GFX.ZBuffer = NULL; }
    if(GFX.SubZBuffer){ free (GFX.SubZBuffer);GFX.SubZBuffer = NULL; }
}

void S9xSetPalette ()
{
    if(text_mode == FALSE && Bitness() == 8)
    {
        uint16 Brightness = IPPU.MaxBrightness * 138;
        ggi_color pal[256];
    
        for (int i = 0; i < 256; i++)
        {
            pal[i].r = ((PPU.CGDATA [i] >> 0) & 0x1F) * Brightness;
            pal[i].g = ((PPU.CGDATA [i] >> 5) & 0x1F) * Brightness;
            pal[i].b = ((PPU.CGDATA [i] >>10) & 0x1F) * Brightness;
        }

        if(dihdah)
        {
            putchar('P');
            fwrite(&pal, 1, sizeof pal, stdout);
            fflush(stdout);
        }

        ggiSetPalette(vis, 0,256, pal);
    }
}

static void DrawSpeedIndicator(void)
{
    if(stretch || text_mode)return;

    int fx, fy;
    ggiGetCharSize(vis, &fx, &fy);
    
    if(vis_mode.visible.y <= 256-fy)return;
    
    ggiSetGCForeground(vis, GT_DEPTH(vis_mode.graphtype)==8 ? 15 : 0x7FFF);
    ggiSetGCBackground(vis, 0);

    char Buf[64];
    sprintf(Buf, "%dmspf", Settings.FrameTime);    
    sprintf(strchr(Buf,0), " (%.1f fps)", 1E6/Settings.FrameTime);    
    sprintf(strchr(Buf,0), " Skip %d      ", Settings.SkipFrames);
    ggiPuts(vis, 0,256-fy, Buf);
}

void S9xProcessEvents (bool8 block)
{
    struct timeval tv = {0, 0};

    /* block is used when the game is paused       */
    /* does not necessarily need to block, however */
    if(block)tv.tv_usec = 10000;
    
    for(;;)
    {
        ggi_event event;
        
        if(pause_mode)
        {
            int c = getchar();
            if(c == EOF)break;
            
            fprintf(stderr, "Resume by key %d (%c)\n", c, c);
            
            fcntl(STDIN_FILENO, F_SETFL, saved_fcntl);

            pause_mode = FALSE;
            
            if(text_mode)
            {
                fprintf(stderr, "back: ggiInit: %d\n", ggiInit());
                vis = 0;

                S9xGraphicsMode();
            }
            else
            {
                // FIXME: Keyboard back to the mode ggi wants.
            }
            
            event.any.type = evCommand;
            event.cmd.code = GGICMD_HALT_ON_UNMAP;
            ggiEventSend(vis, &event);
            break;
        }
        
        if(!ggiEventPoll(vis, emAll, &tv))
            break;
            
        ggiEventRead(vis, &event, emAll);
        
        if(event.any.type != evKeyPress
        && event.any.type != evKeyRelease)continue;
        
        int down = event.any.type == evKeyPress;
        
        #define TEST_KEY(which, what, how) \
                if(event.key.label==which) \
                      if(down)joypads[what]|=how;else \
                              joypads[what]&=~how

        // Joypad 1:
        TEST_KEY('K',         0, SNES_RIGHT_MASK);
        TEST_KEY(GIIK_Right,  0, SNES_RIGHT_MASK);

        TEST_KEY('H',         0, SNES_LEFT_MASK);
        TEST_KEY(GIIK_Left,   0, SNES_LEFT_MASK);
        
        TEST_KEY('N',         0, SNES_DOWN_MASK);
        TEST_KEY('J',         0, SNES_DOWN_MASK);
        TEST_KEY(GIIK_Down,   0, SNES_DOWN_MASK);
        
        TEST_KEY('U',         0, SNES_UP_MASK);
        TEST_KEY(GIIK_Up,     0, SNES_UP_MASK);
        
        TEST_KEY(GIIUC_Return,0, SNES_START_MASK);
        TEST_KEY(GIIUC_Space, 0, SNES_SELECT_MASK);
        
        TEST_KEY('A',         0, SNES_TL_MASK);
        TEST_KEY('V',         0, SNES_TL_MASK);
        TEST_KEY('Q',         0, SNES_TL_MASK);
        
        TEST_KEY('Z',         0, SNES_TR_MASK);
        TEST_KEY('B',         0, SNES_TR_MASK);
        TEST_KEY('W',         0, SNES_TR_MASK);
        
        TEST_KEY('S',         0, SNES_X_MASK);
        TEST_KEY('M',         0, SNES_X_MASK);
        TEST_KEY('E',         0, SNES_X_MASK);
        
        TEST_KEY('X',         0, SNES_Y_MASK);
        TEST_KEY(GIIUC_Comma, 0, SNES_Y_MASK);
        TEST_KEY('R',         0, SNES_Y_MASK);
        
        TEST_KEY('D',         0, SNES_A_MASK);
        TEST_KEY(GIIUC_Period,0, SNES_A_MASK);
        TEST_KEY('T',         0, SNES_A_MASK);
        
        TEST_KEY('C',         0, SNES_B_MASK);
        TEST_KEY(GIIUC_Slash, 0, SNES_B_MASK);
        TEST_KEY('Y',         0, SNES_B_MASK);
        
        // Joypad 2:
        TEST_KEY(GIIK_P8,     1, SNES_UP_MASK);
        TEST_KEY(GIIK_P2,     1, SNES_DOWN_MASK);
        TEST_KEY(GIIK_P6,     1, SNES_RIGHT_MASK);
        TEST_KEY(GIIK_P4,     1, SNES_LEFT_MASK);
        TEST_KEY(GIIK_PEnter, 1, SNES_START_MASK);
        TEST_KEY(GIIK_PPlus,  1, SNES_SELECT_MASK);
        TEST_KEY(GIIK_P0,     1, SNES_TL_MASK);
        TEST_KEY(GIIK_PSeparator,1,SNES_TR_MASK);
        TEST_KEY(GIIK_P7,     1, SNES_X_MASK);
        TEST_KEY(GIIK_P1,     1, SNES_Y_MASK);
        TEST_KEY(GIIK_P9,     1, SNES_A_MASK);
        TEST_KEY(GIIK_P3,     1, SNES_B_MASK);
        
        if(down)
        {
            if(event.key.label == GIIUC_Escape)S9xExit();
                
            if(event.key.label == '1')PPU.BG_Forced ^= 1;
            if(event.key.label == '2')PPU.BG_Forced ^= 2;
            if(event.key.label == '3')PPU.BG_Forced ^= 4;
            if(event.key.label == '4')PPU.BG_Forced ^= 8;
            if(event.key.label == '5')PPU.BG_Forced ^= 16;
        
            if(event.key.label == '6')Settings.SwapJoypads ^= TRUE;
            if(event.key.label == '7')Settings.Transparency ^= TRUE;
            if(event.key.label == '8')Settings.BGLayering ^= TRUE;
            if(event.key.label == '9')Settings.InterpolatedSound ^= TRUE;
            if(event.key.label == '0')Settings.DisableHDMA ^= TRUE;
            
            if(event.key.label == GIIK_Break)Settings.Paused ^= TRUE;
                
            if(event.key.label == GIIUC_BackSpace)
                Settings.DisableGraphicWindows = !Settings.DisableGraphicWindows;
                
            if(event.key.label == GIIK_P5)kbdtrans_enabled ^= 1;
        
            int fn = -1;
            
            if(event.key.label == GIIK_F1)fn = 1;
            if(event.key.label == GIIK_F2)fn = 2;
            if(event.key.label == GIIK_F3)fn = 3;
            if(event.key.label == GIIK_F4)fn = 4;
            if(event.key.label == GIIK_F5)fn = 5;
            if(event.key.label == GIIK_F6)fn = 6;
            if(event.key.label == GIIK_F7)fn = 7;
            if(event.key.label == GIIK_F8)fn = 8;
            if(event.key.label == GIIK_F9)fn = 9;
            if(event.key.label == GIIK_F10)fn = 10;
            if(event.key.label == GIIK_F11)fn = 11;
            if(event.key.label == GIIK_F12)fn = 12;
            
            int alt     = event.key.modifiers & GII_MOD_ALT;
            int shift   = event.key.modifiers & GII_MOD_SHIFT;
            int control = event.key.modifiers & GII_MOD_CTRL;
            
            if(!alt && !shift && !control)
            {
                if(fn == 11)
                    S9xLoadSnapshot (S9xChooseFilename (TRUE));
                else if(fn > 0)
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
            else if(alt || control)
            {
                if (fn >= 4)
                    S9xToggleSoundChannel (fn - 4);
#ifdef DEBUGGER
                else if (fn == 1)
                    CPU.Flags |= DEBUG_MODE_FLAG;
#endif
                else if (fn == 1)
                    S9xSPCDump(S9xChooseFilename(2));
                else if (fn == 2)
                    S9xLoadSnapshot(S9xChooseFilename (TRUE));
                else if (fn == 3)
                    Snapshot(S9xChooseFilename(FALSE));
            }
            else if(fn > 0)
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
            if(event.key.button == 12)
            {
                if(shift)
                {
                    if (Settings.FrameTime > 1000)
                        Settings.FrameTime -= 1000;
                    else if(Settings.FrameTime > 100)
                        Settings.FrameTime -= 100;
                }
                else
                {
                    if (Settings.SkipFrames <= 1)
                        Settings.SkipFrames = AUTO_FRAMERATE;
                    else if (Settings.SkipFrames != AUTO_FRAMERATE)
                        Settings.SkipFrames--;
                }
                DrawSpeedIndicator();
            }
            
            if(event.key.button == 13)
            {
                if(shift)
                {
                    if(Settings.FrameTime < 600)
                        Settings.FrameTime += 100;
                    else
                        Settings.FrameTime += 1000;
                }
                else
                {
                    if (Settings.SkipFrames == AUTO_FRAMERATE)
                        Settings.SkipFrames = 1;
                    else if (Settings.SkipFrames < 20)
                        Settings.SkipFrames++;
                }
                DrawSpeedIndicator();
            }
            if(event.key.label=='P')
            {
                if(alt==0)
                {
                    text_mode = TRUE;
                    if(vis)ggiClose(vis);
                    fprintf(stderr, "P: ggiExit: %d\n", ggiExit());
                    
                    fprintf(stderr,
                        "The game is now running in videoless mode.\r\n"
                        "Press ENTER to re-enable graphics.\r\n");
                }
                else
                {
                    fprintf(stderr, "Alt: %d, Shift: %d, Ctrl: %d\n",
                            alt,shift,control);

                    event.any.type = evCommand;
                    event.cmd.code = GGICMD_NOHALT_ON_UNMAP;
                    ggiEventSend(vis, &event);
                    
                    // FIXME: Unraw keyboard here!
                }
                
                pause_mode = TRUE;
                fcntl(STDIN_FILENO, F_SETFL, (saved_fcntl = fcntl(STDIN_FILENO, F_GETFL))|O_NONBLOCK);
        
                /* Exit loop, input handling became unstable. */
                break;
            }
        }
    }
}

void S9xSetTitle (const char *title)
{
    title=title;
}

void S9xPutImage (int width, int height)
{
    #if 1
    if(text_mode)return;
    
    if(dihdah)
    {
        int a, k;
        putchar('F');
        fwrite(&width,  1, sizeof(int), stdout);
        fwrite(&height, 1, sizeof(int), stdout);
        fwrite(GFX.Screen, 1, width*height, stdout);
            fflush(stdout);
        fcntl(STDIN_FILENO, F_SETFL, (a=fcntl(STDIN_FILENO, F_GETFL))|O_NONBLOCK);
        k = getchar();
        fcntl(STDIN_FILENO, F_SETFL, a);
        if(k=='K')
        {
            ggi_event e;
            fread(&e, 1, sizeof(e), stdin);
            if(kbdtrans_enabled)ggiEventSend(vis, &e);
        }
    }
    #endif
    
    if(Settings.SixteenBit)
    {
        /* 16 bpp source */
        switch(Bitness())
        {
          case 8:
            Convert16to8();
            break;
          case 15:
            Convert16to15();
            break;
          case 16:
            Convert16to16();
            break;
          case 24:
            Convert16to24();
            break;
          case 32:
            Convert16to32();
            break;
          default:
            break;
        }
    }
    else
    {
        /* 8 bpp source */
        switch(Bitness())
        {
          case 8:
            Convert8to8();
            break;
          default:
            break;
        }
    }
    ggiFlush(vis);
}

const char *S9xSelectFilename (const char *def, const char *dir1,
                            const char *ext1, const char *title)
{
    
    char InputBuf[PATH_MAX];
    strcpy(InputBuf, def);
    char *s = InputBuf;
    int cpos, len=strlen(s);
    int fx, fy;
    
    ggiGetCharSize(vis, &fx, &fy);
    
    int max = vis_mode.visible.x / fx - 4;
    
    ggiSetPalette(vis, 0,16, init_pal16);
    
    ggiSetGCForeground(vis, 0);
    ggiDrawBox(vis, 0,0, vis_mode.visible.x, fy*4);
    
    unsigned bright, gray, blue, cyan;
    
    switch(GT_DEPTH(vis_mode.graphtype))
    {
        case 8:
            blue=1;
            gray=7;
            cyan=11;
            bright=15;
            break;
        case 16:
        case 15:
            /* These don't work anyway */
            break;
        default:
            blue   = 0x0000FF;
            cyan   = 0x00FFFF;
            gray   = 0x7F7F7F;
            bright = 0xFFFFFF;
    }
    
    ggiSetGCForeground(vis, bright);
    ggiSetGCBackground(vis, 0);
    ggiPuts(vis, 0,fy, title);
    
    if(len > max)s[len=max]=0;
    cpos = len;
    
    char *retval = NULL;
    
    for(int x=2; ;)
    {
        int a, tick=0;
        for(;;)
        {
            struct timeval tv = {0,100000};
            for(a=0; a<max; a++)
            {
                int f = tick<6&&a==cpos;
                ggiSetGCForeground(vis, f?blue:cyan);
                ggiSetGCBackground(vis, f?gray:blue);
                ggiPutc(vis, (x+a)*fx, fy+fy, a<len?s[a]:' ');
            }
            ggiFlush(vis);
                if(ggiEventPoll(vis, gii_event_mask(emKeyPress | emKeyRepeat), &tv))break;
            if(++tick==8)tick=4;
        }
        switch(a=ggiGetc(vis))
        {
            case '\n':
            case '\r':
                goto Done;
            case GIIK_Left:
                if(cpos>0)cpos--;
                break;
            case GIIK_Right:
                if(cpos<len)cpos++;
                break;
            case GIIK_Home:
                cpos=0;
                break;
            case GIIK_End:
                cpos=len;
                break;
            case GIIUC_Escape:
                goto Cancelled;
            case GIIUC_Delete:
            case GIIUC_BackSpace:
                if(!cpos)break;
                cpos--;
                /* through */
            /*case del:*/
                if(cpos < len)
                {
                    memmove(s+cpos, s+1+cpos, len-cpos);
                    len--;
                }
                break;
            default:
                if(len<max && a>=' ' && a<=255)
                {
                    memmove(s+cpos+1, s+cpos, len-cpos+1);
                    s[cpos++]=a;
                    len++;
                }
        }
    }
Done: ;
    char fname [PATH_MAX];
    char drive [_MAX_DRIVE];
    char dir [_MAX_DIR];
    char ext [_MAX_EXT];

    _splitpath (InputBuf, drive, dir, fname, ext);
    static char path [PATH_MAX];
    _makepath (path, drive, *dir ? dir : dir1, fname, *ext ? ext : ext1);
    retval = path;
    
Cancelled:
    ggiSetGCBackground(vis, 0);
    ggiSetGCForeground(vis, 0);
    ggiFillscreen(vis);
    S9xSetPalette();

    DrawSpeedIndicator();
    
    return retval;
}

void S9xExtraDisplayUsage ()
{
    S9xMessage(S9X_INFO, S9X_USAGE, "\
-mode or -m modestr       Select screen mode (see libggi manual, specially doc/env.txt)");
    S9xMessage(S9X_INFO, S9X_USAGE, "\
-scale or -sc             Scale SNES screen to fit S-VGA screen (this probably slows a lot!)");
    S9xMessage(S9X_INFO, S9X_USAGE, "\
-y1,-y2,-y3,-y4,-y5       Select interpolating: 1=interpolate, 2=Super 2xSaI, 3=Super Eagle, 4=2xSaI, 5=soft");
    S9xMessage(S9X_INFO, S9X_USAGE, "\
-send or -dihdah          Send palette/screen actions to stdout, read kbd actions from stdin. Can be used for remote controlling.");
}

void S9xParseDisplayArg(char **argv, int &ind, int)
{
    CheckFirst();
    if((!strcmp(argv[ind], "-m")
     || !strcasecmp(argv[ind], "-mode")) && argv[ind + 1])
        ggiParseMode(argv[++ind], &vis_mode);
    else if(!strcasecmp(argv[ind], "-scale")
         || !strcasecmp(argv[ind], "-sc"))
        stretch = TRUE;
    else if (!strncasecmp (argv [ind], "-y", 2))
    {
        Settings.SixteenBit = TRUE;
        Settings.SupportHiRes = TRUE;
        Settings.ForceTransparency = TRUE;
        switch (argv[ind][2])
        {
            case 0:  interpolate = TRUE;  break;
            case '1': interpolate = TRUE; break;
            case '2': interpolate = 2; break;
            case '3': interpolate = 3; break;
            case '4': interpolate = 4; break;
            case '5': interpolate = 5; break;
        }
    }
    else if(!strcasecmp(argv[ind], "-dihdah")
         || !strcasecmp(argv[ind], "-send"))
    {
        dihdah = TRUE;
        kbdtrans_enabled = TRUE;
    }
    else
        S9xUsage();
}

bool8 S9xReadMousePosition (int which1, int &x, int &y, uint32 &buttons)
{
    which1=which1, x=x, y=y, buttons=buttons;
    return FALSE;
}

bool8 S9xReadSuperScopePosition (int &x, int &y, uint32 &buttons)
{
    x=x, y=y, buttons=buttons;
    return FALSE;
}

void S9xMessage (int type, int number, const char *message)
{
    CheckFirst();
    type=type, number=number;
    fprintf (stderr, "%s\n", message);
}

bool JustifierOffscreen()
{
  return false;
}

void JustifierButtons(uint32& justifiers)
{
}



