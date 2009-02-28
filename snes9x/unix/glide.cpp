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

#include <math.h>
#include <glide.h>
#include <signal.h>
#include <setjmp.h>
#include "snes9x.h"
#include "gfx.h"
#include "3d.h"

GlideData Glide;

#define TEXTURE_SIZE 256

static uint16 *texture_download_buffer = NULL;
static GrTexInfo texture_info;
static int texture_mem_start [4];
void S9xGlideDeinit ();

static void S9xGlideCalcSquare (int snes_width, int snes_height, 
				int x, int y, int width, int height,
				GrVertex *square)
{
    float virtual_height;
    float virtual_width;
    float virtual_x;
    float virtual_y;

#if 0
    float snes_aspect_ratio = snes_width / (float) snes_height;
    float voodoo_aspect_ratio = Glide.voodoo_width / (float) Glide.voodoo_height;

    if (snes_aspect_ratio < voodoo_aspect_ratio)
    {
	virtual_height = (float) Glide.voodoo_height;
	virtual_width = virtual_height * snes_aspect_ratio;
	virtual_x = ((float) Glide.voodoo_width - virtual_width) / 2.0;
	virtual_y = 0.0;
    }
    else
    {
	virtual_width = Glide.voodoo_width;
	virtual_height = virtual_width / snes_aspect_ratio;
	virtual_x = 0;
	virtual_y = (Glide.voodoo_height - virtual_height) / 2.0;
    }
#endif

    virtual_x = 0;
    virtual_y = 0;
    virtual_width = Glide.voodoo_width;
    virtual_height = Glide.voodoo_height;

    for (int i = 0; i < 4; i++)
    {
	square [i].oow = 1.0;
	square [i].ooz = 65535.0;
	square [i].tmuvtx [0].oow = 1.0;
    }

    square [0].tmuvtx [0].sow = 0.0;
    square [0].tmuvtx [0].tow = 0.0;
    square [1].tmuvtx [0].sow = width;
    square [1].tmuvtx [0].tow = 0.0;
    square [2].tmuvtx [0].sow = width;
    square [2].tmuvtx [0].tow = height;
    square [3].tmuvtx [0].sow = 0.0;
    square [3].tmuvtx [0].tow = height;
	
    float texture_width_percent;
    float texture_height_percent;

    if ((texture_width_percent = virtual_width * TEXTURE_SIZE / snes_width) > virtual_width)
	texture_width_percent = virtual_width;
    if ((texture_height_percent = virtual_height * TEXTURE_SIZE / snes_height) > virtual_height)
	texture_height_percent = virtual_height;

    square [0].x = virtual_x + x * texture_width_percent;
    square [0].y = virtual_y + y * texture_height_percent;
    square [1].x = virtual_x + (x + 1) * texture_width_percent;
    square [1].y = virtual_y + y * texture_height_percent;
    square [2].x = virtual_x + (x + 1) * texture_width_percent;
    square [2].y = virtual_y + (y + 1) * texture_height_percent;
    square [3].x = virtual_x + x * texture_width_percent;
    square [3].y = virtual_y + (y + 1) * texture_height_percent;
}

static void S9xGlideInitTextures ()
{
    grColorCombine (GR_COMBINE_FUNCTION_SCALE_OTHER,
		    GR_COMBINE_FACTOR_ONE,
		    GR_COMBINE_LOCAL_NONE,
		    GR_COMBINE_OTHER_TEXTURE,
		    FXFALSE);

    grTexCombine (GR_TMU0,
		  GR_COMBINE_FUNCTION_LOCAL,
		  GR_COMBINE_FACTOR_NONE,
		  GR_COMBINE_FUNCTION_NONE,
		  GR_COMBINE_FACTOR_NONE,
		  FXFALSE, 
		  FXFALSE);

    grTexMipMapMode (GR_TMU0,
		     GR_MIPMAP_DISABLE,
		     FXFALSE);

    grTexClampMode (GR_TMU0,
		    GR_TEXTURECLAMP_CLAMP,
		    GR_TEXTURECLAMP_CLAMP);

    grTexFilterMode (GR_TMU0, 
		     GR_TEXTUREFILTER_BILINEAR,
		     GR_TEXTUREFILTER_BILINEAR);

    /* XXX: trilinear */
    grTexCombine (GR_TMU0,
                  GR_COMBINE_FUNCTION_BLEND_LOCAL,
                  GR_COMBINE_FACTOR_LOD_FRACTION,
                  GR_COMBINE_FUNCTION_BLEND_LOCAL,
                  GR_COMBINE_FACTOR_LOD_FRACTION,
                  FXFALSE,
                  FXFALSE);

    texture_info.smallLod = GR_LOD_256;
    texture_info.largeLod = GR_LOD_256;
    texture_info.aspectRatio = GR_ASPECT_1x1;
    texture_info.format = GR_TEXFMT_RGB_565;

    Glide.texture.smallLod = GR_LOD_8;
    Glide.texture.largeLod = GR_LOD_8;
    Glide.texture.aspectRatio = GR_ASPECT_1x1;
    Glide.texture.format = GR_TEXFMT_RGB_565;
    Glide.texture_mem_size = grTexTextureMemRequired (GR_MIPMAPLEVELMASK_BOTH,
						      &Glide.texture);
    Glide.texture_mem_start = grTexMinAddress (GR_TMU0);

    int texture_mem_size = grTexTextureMemRequired (GR_MIPMAPLEVELMASK_BOTH,
						    &texture_info);
    int address = grTexMinAddress (GR_TMU0);

    // At maximum SNES resolution (512x478) four seperate Voodoo textures will
    // be needed since the maximum texture size on Voodoo cards is 256x256.
    
    for (int t = 0; t < 4; t++)
    {
	texture_mem_start [t] = address;
	address += texture_mem_size;
    }
    
    texture_download_buffer = (uint16 *) malloc (texture_mem_size);

    for (int i = 0; i < 4; i++)
    {
	Glide.sq [i].oow = 1.0;
	Glide.sq [i].ooz = 65535.0;
	Glide.sq [i].tmuvtx [0].oow = 1.0;
    }

    Glide.x_offset = 0.0;
    Glide.y_offset = 0.0;
    // XXX: Do this when the SNES screen resolution is known.
    Glide.x_scale = Glide.voodoo_width * 8.0 / 256;
    Glide.y_scale = Glide.voodoo_height * 8.0 / 224;
}

static jmp_buf restart_point;

static void SegfaultHandler (int)
{
    printf ("\n\
No permission to access 3Dfx hardware or 3Dfx hardware not present.\n");
    Glide.voodoo_present = FALSE;
    longjmp (restart_point, 1);
}

bool8 S9xGlideInit ()
{
    static GrHwConfiguration hwconfig;

    // XXX:
    GrScreenResolution_t resolution = GR_RESOLUTION_640x480;
    GrScreenRefresh_t refresh_rate = GR_REFRESH_100Hz;

    Glide.voodoo_present = TRUE;
    setjmp (restart_point);
    if (!Glide.voodoo_present)
    {
	signal (SIGSEGV, SIG_DFL);
	return (FALSE);
    }

    signal (SIGSEGV, (SIG_PF) SegfaultHandler);
    grGlideInit ();
    if (grSstQueryHardware (&hwconfig))
    {
	grSstSelect (0);
	if (grSstWinOpen (0, resolution, refresh_rate, GR_COLORFORMAT_ABGR,
			  GR_ORIGIN_UPPER_LEFT, 2, 1))
	{
	    grBufferClear (0, 0, GR_ZDEPTHVALUE_FARTHEST);
	    Glide.voodoo_width = grSstScreenWidth ();
	    Glide.voodoo_height = grSstScreenHeight ();
	    S9xGlideInitTextures ();
	    signal (SIGSEGV, (SIG_PF) S9xGlideDeinit);
	    return (TRUE);
	}
    }
    
    Glide.voodoo_present = FALSE;
    signal (SIGSEGV, SIG_DFL);

    return (FALSE);
}

void S9xGlideDeinit ()
{
    free ((char *) texture_download_buffer);
    grGlideShutdown();
}

void S9xGlidePutImage (int snes_width, int snes_height)
{
    GrVertex square [4];

    grBufferClear (0, 0, GR_ZDEPTHVALUE_FARTHEST);

    if (snes_width <= TEXTURE_SIZE && snes_height <= TEXTURE_SIZE)
    {
	// Special case 256x224 and 256x239 SNES resolutions.
	texture_info.data = (void *) GFX.Screen;

	grTexDownloadMipMapLevel (GR_TMU0, texture_mem_start [0],
				  GR_LOD_256, GR_LOD_256, GR_ASPECT_1x1,
				  GR_TEXFMT_RGB_565,
				  GR_MIPMAPLEVELMASK_BOTH,
				  (void *) GFX.Screen);

	grTexSource (GR_TMU0, texture_mem_start [0],
		     GR_MIPMAPLEVELMASK_BOTH, &texture_info);

	S9xGlideCalcSquare (snes_width, snes_height, 0, 0, 
			    snes_width, snes_height, square);

	grDrawTriangle (&square [0], &square [3], &square [2]);
	grDrawTriangle (&square [0], &square [1], &square [2]);
    }
    else
    {
	// Have to chop the rendered SNES screen into a maximum 256x256 size
	// chunks because that's all the Voodoo card can cope with.
	int t = 0;
	int vert_squares = (snes_height + (TEXTURE_SIZE - 1)) / TEXTURE_SIZE;
	int horiz_squares = (snes_width + (TEXTURE_SIZE - 1)) / TEXTURE_SIZE;

	texture_info.data = (void *) texture_download_buffer;

	for (int y = 0; y < vert_squares; y++)
	{
	    for (int x = 0; x < horiz_squares; x++, t++)
	    {
		int square_height = y != vert_squares - 1 ? TEXTURE_SIZE : 
				    snes_height - (TEXTURE_SIZE * y);
		int square_width = x != horiz_squares - 1 ? TEXTURE_SIZE :
				   snes_width - (TEXTURE_SIZE * x);
		int offset = x * TEXTURE_SIZE * sizeof (uint16) +
			     y * TEXTURE_SIZE * GFX.Pitch;

		for (int l = 0; l < square_height; l++)
		{
		    memmove ((void *) (texture_download_buffer + l * 256),
			     GFX.Screen + l * GFX.RealPitch + offset,
			     sizeof (uint16) * 256);
		}
		grTexDownloadMipMapLevel (GR_TMU0, texture_mem_start [t],
					  GR_LOD_256, GR_LOD_256, 
					  GR_ASPECT_1x1,
					  GR_TEXFMT_RGB_565,
					  GR_MIPMAPLEVELMASK_BOTH,
					  (void *) texture_download_buffer);

		grTexSource (GR_TMU0, texture_mem_start [t],
			     GR_MIPMAPLEVELMASK_BOTH, &texture_info);

		S9xGlideCalcSquare (snes_width, snes_height, x, y,
				    square_width, square_height,
				    square);
		grDrawTriangle (&square [0], &square [3], &square [2]);
		grDrawTriangle (&square [0], &square [1], &square [2]);
	    }
	}
    }

    grBufferSwap(1);
}

bool8 S9xGlideEnable (bool8 enable)
{
    static uint32 previous_graphic_format = RGB565;
    static bool8 previous_sixteenbit = FALSE;

    extern uint32 current_graphic_format;

    if (Settings.GlideEnable != enable)
    {
	if (enable)
	{
	    if (S9xGlideInit ())
	    {
		previous_graphic_format = current_graphic_format;
		previous_sixteenbit = Settings.SixteenBit;
		S9xGraphicsDeinit ();
		S9xSetRenderPixelFormat (RGB565);
		Settings.SixteenBit = TRUE;
		Settings.Transparency = TRUE;
		S9xGraphicsInit ();
		Settings.GlideEnable = TRUE;
	    }
	}
	else
	{
	    Settings.GlideEnable = FALSE;
	    S9xGlideDeinit ();
	    S9xGraphicsDeinit ();
	    S9xSetRenderPixelFormat (previous_graphic_format);
	    if (!(Settings.SixteenBit = previous_sixteenbit))
		Settings.Transparency = FALSE;
	    S9xGraphicsInit ();
	}
    }
    return (TRUE);
}

