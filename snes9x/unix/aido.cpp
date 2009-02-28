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

/* This file is Copyright 2003 Peter Bortas <peter@bortas.org>, 
 *                             Marcus Comstedt <marcus@mc.pp.se> and 
 *                             Per Hedbor <per@hedbor.org>
 *
 *  Permission to use, copy, modify and distribute this file in both binary and
 *  source form, for non-commercial purposes, is hereby granted without fee,
 *  providing that this license information and copyright notice appear with
 *  all copies and any derived work.
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event shall the authors be held liable for any damages
 *  arising from the use of this software.
 */

//#include <math.h>
#include "snes9x.h"
#include "gfx.h"
#include "aido.h"

#include <sys/ipc.h>
#include <sys/shm.h>

#ifdef USE_AIDO

static unsigned char *data=NULL;
static int offset = 0;

bool8 S9xAIDOInit ()
{
  int shi = Settings.AIDOShmId;
  int id = shmget( shi, 1024*1024*4+100, 0 );
  if(id == -1)
  {
    perror("shmerror");
    fprintf(stderr,
            "FATAL: Snes9X: Unable to get SHM memory. (%d,%d)\n", shi, id);
    exit(1);
  }


  data = (unsigned char *)shmat( id, 0, 0 );
  if((int)data == -1)
  {
    fprintf(stderr, 
            "FATAL: Snes9X: Unable to aquire SHM area. (%d,%d)\n", shi, id);
    exit(1);
  }

  S9xSetRenderPixelFormat (RGB5551);
  //  Settings.SixteenBit = TRUE;
  Settings.SupportHiRes = TRUE;
  Settings.Mode7Interpolate = TRUE;
  Settings.AutoSaveDelay = 10;
  //  Settings.Transparency = TRUE;

  GFX.Pitch = IMAGE_WIDTH*2;                          /* Bytes per line */
  GFX.Screen = (uint8 *) malloc( IMAGE_WIDTH * IMAGE_HEIGHT*2 );
  GFX.ZBuffer = (uint8 *) malloc( IMAGE_WIDTH * IMAGE_HEIGHT );
  GFX.SubScreen = (uint8 *) malloc( IMAGE_WIDTH * IMAGE_HEIGHT*2 );
  GFX.SubZBuffer = (uint8 *) malloc( IMAGE_WIDTH * IMAGE_HEIGHT );
  GFX.Delta = (GFX.SubScreen - GFX.Screen) >> 1;

  return (TRUE);
}

void S9xAIDODeinit ()
{
  fprintf(stderr, "Snes9X exited. This shouldn't happen.\n");
}

#define RED(color) ((color&0x7C00) >> 7)
#define GREEN(color) ((color&0x03E0) >> 2)
#define BLUE(color) ((color&0x001F) << 3)

void S9xAIDOPutImage (int snes_width, int snes_height)
{
  int y, size = GFX.Pitch2*snes_height;
  //  fprintf(stderr, "data: %p   offset:%d   size:%d\n", data, offset, size);

  if( offset >  1024*4*1024 - (snes_width * snes_height * 3) )
    offset = 0;

  for(y=0; y<snes_height; y++)
    memcpy(data+offset+snes_width*y*2, GFX.Screen+GFX.Pitch2*y, GFX.Pitch2);

  {
    static int ow, oh;
    if( snes_width != ow )
    {
      printf("mem_w:%d\n", snes_width);
      ow = snes_width;
    }
    if( snes_height != oh )
    {
      printf("mem_h:%d\n", snes_height);
      oh = snes_height;
    }
  }
  
  printf("mem_offset:%d\n", offset);
  fflush(stdout);
  offset += size;
}

extern uint32 joypads [5];

static void handle_controller (char *input)
{
  uint32 controller = input[0];
  int button = input[1];
  int pressed = input[2];

  uint8 byte1 = 0;
  uint8 byte2 = 0;
  uint8 byte3 = 0;
  uint8 byte4 = 0;

  //  fprintf(stderr, "Handling controller %d.\n", controller);

  switch (button)
  {
   case 'p':
     Settings.Paused ^= 1; return;
   case '7':
     Settings.Mode7Interpolate ^= TRUE; return;
   case 't':
     Settings.TurboMode = pressed; return;
   case 'r':
     byte2 = 1;	        break; // Right
   case 'l':
     byte2 = 2;	        break; // Left
   case 'd':
     byte2 = 4;	        break; // Down
   case 'u':
     byte2 = 8;	        break; // Up
   case 's':	
     byte2 = 16;	break; // Start
   case 'c':	
     byte2 = 32;	break; // Select
   case 'a':
     byte1 = 128;	break; // A
   case 'b':
     byte2 = 128;	break; // B
   case 'x':
     byte1 = 64;	break; // X
   case 'y':	
     byte2 = 64;	break; // Y
   case 'v':	
     byte1 = 32;	break; // TL
   case 'h':	
     byte1 = 16;	break; // RT
   default:
     fprintf(stderr, "Snes9X: Unsupported command %d.\n", button);
  }

  if (pressed)
  {
    joypads [0] |= byte1;
    joypads [0] |= (byte2 << 8);
    joypads [1] |= byte3;
    joypads [1] |= (byte4 << 8);
    joypads[controller] |= byte1;
    joypads[controller] |= (byte2 << 8);
  } else {
    joypads [0] &= ~byte1;
    joypads [0] &= ~(byte2 << 8);
    joypads [1] &= ~byte3;
    joypads [1] &= ~(byte4 << 8);
    joypads[controller] &= ~byte1;
    joypads[controller] &= ~(byte2 << 8);
  }
}

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <stropts.h>
#include <poll.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#define NORMAL_DATA 1
#define HIPRI_DATA 2

//FIXME: Check what block should do.
void S9xAIDOProcessEvents (bool8 block)
{
  struct pollfd poll_list[1];
  int ret;
  char input[3];

  poll_list[0].fd = 0;
  poll_list[0].events = POLLIN|POLLHUP|POLLERR|POLLNVAL;
  poll_list[0].revents = 0;

  do {
    ret = poll(poll_list, 1, 1);

    if( ((poll_list[0].revents&POLLHUP) == POLLHUP) ||
        ((poll_list[0].revents&POLLERR) == POLLERR) ||
        ((poll_list[0].revents&POLLNVAL) == POLLNVAL) )
    {
      fprintf(stderr, "Snes9X: Input poll failed. Exiting.\n");
      exit(1);
    }
    
    if(ret < 0)
    {
      if(errno == EBADF) {
        fprintf(stderr, "Snes9X: Bad fd. Exiting.\n");
        exit(1);
      }        
      return;
    }

    if( (poll_list[0].revents&POLLIN) == POLLIN )
    {
      read(0, input, 3);
      handle_controller(input);
    }
  } while( ret > 0 );
}

#endif /* USE_AIDO */
