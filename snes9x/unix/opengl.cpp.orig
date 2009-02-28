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
#include "snes9x.h"
#include "gfx.h"
#include "3d.h"
#include "x11.h"

#ifdef USE_OPENGL
static int dbdepat [] = {GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16, None};
//static int sbdepat [] = {GLX_RGBA,GLX_DEPTH_SIZE, 16, None};
//static int dbnodepat [] = {GLX_RGBA, GLX_DOUBLEBUFFER, None};
//static int sbnodepat [] = {GLX_RGBA, None};

OpenGLData OpenGL;

uint8 basetexbuffer [512 * 512 * 3];
static double xrot, yrot, zrot;

#define DRAW_CUBE 0

bool8 S9xOpenGLInit ()
{
    XVisualInfo *vi;
    vi = glXChooseVisual (GUI.display, 
                          XScreenNumberOfScreen (GUI.screen), dbdepat);

    if (!vi)
        return (FALSE);

    GUI.visual = vi->visual;
    GUI.depth = vi->depth;

    GUI.glcontext = glXCreateContext (GUI.display, vi, 0, GL_TRUE);
    if (!GUI.glcontext)
        return (FALSE);

    OpenGL.draw_cube = FALSE;

    return (TRUE);
}

bool8 S9xOpenGLInit2 ()
{
    glXMakeCurrent (GUI.display, GUI.window, GUI.glcontext);

    glGetIntegerv (GL_MAX_TEXTURE_SIZE, &OpenGL.max_texture_size);

    if (OpenGL.max_texture_size >= 512)
    {
        OpenGL.texture_size = 512;
        OpenGL.num_textures = 2;
    }
    else
    {
        OpenGL.texture_size = OpenGL.max_texture_size;
        OpenGL.num_textures = 1;
    }

    Settings.OpenGLEnable = TRUE;

    const char *ext = (const char *) glGetString (GL_EXTENSIONS);

    if (ext && strstr (ext, "EXT_packed_pixels") != NULL)
        OpenGL.packed_pixels_extension_present = TRUE;

    const char *version = (const char *) glGetString (GL_VERSION);

    if (version && strlen (version) < 100)
    {
	char ver [100];
	strcpy (ver,  version);

	// Strip dots from the version string
	char *ptr;
	while (ptr = strchr (ver, '.'))
	    memmove (ptr, ptr + 1, strlen (ptr + 1) + 1);
	
	// Pad the string with zeros to 4 digits
	while (strlen (ver) < 4)
	    strcat (ver, "0");

	OpenGL.version = atoi (ver);
    }
    else
	OpenGL.version = 1100;

#ifndef GL_UNSIGNED_SHORT_5_5_5_1_EXT
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT     0x8034
#endif

    if (OpenGL.version >= 1200)
    {
        OpenGL.internal_format = GL_RGB5_A1;
        OpenGL.format = GL_RGBA;
        OpenGL.type = GL_UNSIGNED_SHORT_5_5_5_1_EXT;
	S9xSetRenderPixelFormat (RGB5551);
    }
    else
    if (OpenGL.packed_pixels_extension_present)
    {
        OpenGL.internal_format = GL_RGB5_A1;
        OpenGL.format = GL_RGBA;
        OpenGL.type = GL_UNSIGNED_SHORT_5_5_5_1_EXT;
	S9xSetRenderPixelFormat (RGB5551);
    }
    else
    {
        OpenGL.internal_format = GL_RGB;
        OpenGL.format = GL_RGB;
        OpenGL.type = GL_UNSIGNED_BYTE;
    }

    glGenTextures (OpenGL.num_textures, OpenGL.textures);

    if (OpenGL.num_textures == 2)
    {
        glBindTexture (GL_TEXTURE_2D, OpenGL.textures [1]);
        glTexImage2D (GL_TEXTURE_2D, 0, OpenGL.internal_format, 256, 256, 0,
                      OpenGL.format, OpenGL.type, NULL);

        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    glBindTexture (GL_TEXTURE_2D, OpenGL.textures [0]);
    glTexImage2D (GL_TEXTURE_2D, 0, OpenGL.internal_format, 
                  OpenGL.texture_size, OpenGL.texture_size, 0, 
                  OpenGL.format, OpenGL.type, NULL);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glPolygonMode (GL_FRONT, GL_FILL);
    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);

    glEnable (GL_DITHER);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);

    glEnable (GL_POINT_SMOOTH);
    glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable (GL_TEXTURE_2D);

    glShadeModel (GL_SMOOTH);
    glEnable (GL_POLYGON_SMOOTH);
    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth (1.0f);
    glDisable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glMatrixMode (GL_MODELVIEW);

    glClearColor (0, 0, 0, 1);
    glClear (GL_COLOR_BUFFER_BIT);
    glFlush ();

    return (TRUE);
}

void S9xOpenGLDeinit ()
{
    if (GUI.glcontext)
    {
        glXDestroyContext (GUI.display, GUI.glcontext);
        GUI.glcontext = NULL;
    }
}

#define RED(color) ((color&0x7C00) >> 7)
#define GREEN(color) ((color&0x03E0) >> 2)
#define BLUE(color) ((color&0x001F) << 3)

void S9xOpenGLPutImage (int snes_width, int snes_height)
{
    glViewport (0, 0, GUI.window_width, GUI.window_height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    glFrustum (-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 100.0f);
    glMatrixMode (GL_MODELVIEW);

    glDisable (GL_DEPTH_TEST);
    glDisable (GL_LIGHTING);
    glEnable (GL_TEXTURE_2D);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glDisable (GL_BLEND);

    uint8 *data = (uint8 *) GFX.Screen;

    uint32 swidth = snes_width > 256 ? 512 : 256;
    int startl = 0;

    int texture_size;

    if (OpenGL.num_textures == 2 && snes_width == 256)
        texture_size = 256;
    else
        texture_size = OpenGL.texture_size;

    uint32 pheight = snes_height % texture_size;
    if (pheight == 0)
        pheight = texture_size;
    
    if (OpenGL.draw_cube)
    {
        xrot += 0.3f;
        yrot += 0.2f;
        zrot += 0.4f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    for (int starty = snes_height - pheight; starty >= 0; 
         startl += pheight, pheight = texture_size, starty -= texture_size)
    {
        for (int startx = 0; startx < (int) swidth; startx += texture_size)
        {
            uint32 pwidth;
            if ((pwidth = snes_width - startx) > (uint32) texture_size)
                pwidth = texture_size;

            float fx1 = (float) startx / snes_width;
            float fy1 = (float) starty / snes_height;
            float fx2 = fx1 + (float) pwidth / snes_width;
            float fy2 = fy1 + (float) pheight / snes_height;

            if (!OpenGL.packed_pixels_extension_present)
            {
                int i = 0;
                for (uint32 y = 0; y < pheight; y++)
                {
                    uint16 *ptr = (uint16 *) (GFX.Screen + (y + startl) * GFX.Pitch2) + startx;
                    for (uint32 x = 0; x < pwidth; x++)
                    {
                        int color = *ptr++;
                        basetexbuffer [i  ] = RED(color);
                        basetexbuffer [i+1] = GREEN(color);
                        basetexbuffer [i+2] = BLUE(color);
                        i += 3;
                    }
                }
                data = basetexbuffer;
            }
	    else
	    if (snes_width > texture_size)
	    {
		for (uint32 y = 0; y < pheight; y++)
		{
		    memmove (basetexbuffer + sizeof (uint16) * texture_size * y,
			     (GFX.Screen + (y + startl) * GFX.Pitch2) + startx,
			     sizeof (uint16) * texture_size);
		}
		data = basetexbuffer;
	    }

            if (OpenGL.num_textures == 2 && snes_width == 256)
                glBindTexture (GL_TEXTURE_2D, OpenGL.textures [1]);
            else
                glBindTexture (GL_TEXTURE_2D, OpenGL.textures [0]);

            glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, pwidth, pheight,
                             OpenGL.format, OpenGL.type, data);

            float maxx = (float) pwidth / texture_size;
            float maxy = (float) pheight / texture_size;

            if (OpenGL.draw_cube)
            {
                glLoadIdentity();
                glTranslatef (0.0f, 0.0f, -3.5f);
                
                glRotatef (xrot, 1.0f, 0.0f, 0.0f);
                glRotatef (yrot, 0.0f, 1.0f, 0.0f);
                glRotatef (zrot, 0.0f, 0.0f, 1.0f);
                
                glBegin(GL_QUADS);
                {
                    // Front Face
                    glNormal3f (0.0f, 0.0f, 1.0f);
                    glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
                    glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
                    glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f, 1.0f);
                    glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f, 1.0f);
                    // Back Face
                    glNormal3f (0.0f, 0.0f, -1.0f);
                    glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f,-1.0f);
                    glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
                    glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
                    glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f,-1.0f);
                    // Top Face
                    glNormal3f (0.0f, 1.0f, 0.0f);
                    glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
                    glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f, 1.0f, 1.0f);
                    glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f, 1.0f, 1.0f);
                    glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
                    // Bottom Face
                    glNormal3f (0.0f, -1.0f, 0.0f);
                    glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f,-1.0f,-1.0f);
                    glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f,-1.0f,-1.0f);
                    glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
                    glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
                    // Right face
                    glNormal3f (1.0f, 0.0f, 0.0f);
                    glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f,-1.0f,-1.0f);
                    glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
                    glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f, 1.0f, 1.0f);
                    glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
                    // Left Face
                    glNormal3f (-1.0f, 0.0f, 0.0f);
                    glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f,-1.0f,-1.0f);
                    glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
                    glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f, 1.0f, 1.0f);
                    glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
                }
                glEnd();
            }
            else
            {
                glLoadIdentity ();
                glTranslatef (-0.5, -0.5, -1.0);
                glDisable (GL_DEPTH_TEST);
                
                glBegin(GL_TRIANGLE_STRIP);
                {
                    glTexCoord2f (0.0, maxy);
                    glVertex3f (fx1, fy1, 0.0);
                    
                    glTexCoord2f (maxx, maxy);
                    glVertex3f (fx2, fy1, 0.0);
                    
                    glTexCoord2f (0.0, 0.0);
                    glVertex3f (fx1, fy2, 0.0);
                    
                    glTexCoord2f (maxx, 0.0);
                    glVertex3f (fx2, fy2, 0.0);
                }
                glEnd();
            }
        }
    }
    glXSwapBuffers (GUI.display, GUI.window);
}
#endif

