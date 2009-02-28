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

/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *      By Shawn Hargreaves,
 *      1 Salisbury Road,
 *      Market Drayton,
 *      Shropshire,
 *      England, TF9 1AJ.
 *
 *      Soundblaster driver. Supports DMA driven sample playback (mixing 
 *      up to eight samples at a time) and raw note output to the SB MIDI 
 *      port. The Adlib (FM synth) MIDI playing code is in adlib.c.
 *
 *      Input routines added by Ove Kaaven.
 *
 *      See readme.txt for copyright information.
 */


#ifndef DJGPP
#error This file should only be used by the djgpp version of Allegro
#endif

#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <go32.h>
#include <dpmi.h>
#include <limits.h>
#include <sys/farptr.h>

#include <allegro.h>
#include "sb.h"

#define SB_DMA_BUFFER_SIZE       32

#define DISABLE()   asm volatile ("cli")
#define ENABLE()    asm volatile ("sti")
/* interrupt hander stuff */
#define _map_irq(irq)   (((irq)>7) ? ((irq)+104) : ((irq)+8))
#define _eoi(irq) { outportb(0x20, 0x20); if ((irq)>7) outportb(0xA0, 0x20); }

/* external interface to the digital SB driver */
void sx_sb_exit ();
static int sx_sb_mixer_volume(int volume);
char sx_sb_desc[80] = "not initialised";

/* external interface to the SB midi output driver */

static int sx_sb_in_use = FALSE;             /* is SB being used? */
static int sx_sb_stereo = FALSE;             /* in stereo mode? */
static int sx_sb_16bit = FALSE;              /* in 16 bit mode? */
static int sx_sb_int = -1;                   /* interrupt vector */
static int sx_sb_dsp_ver = -1;               /* SB DSP version */
static int sx_sb_dma8 = -1;                  /* 8-bit DMA channel (SB16) */
static int sx_sb_dma16 = -1;                 /* 16-bit DMA channel (SB16) */
static int sx_sb_hw_dsp_ver = -1;            /* as reported by autodetect */
static int sx_sb_dma_size = -1;              /* size of dma transfer in bytes */
static int sx_sb_dma_mix_size = -1;          /* number of samples to mix */
static int sx_sb_dma_count = 0;              /* need to resync with dma? */
static volatile int sx_sb_semaphore = FALSE; /* reentrant interrupt? */
static int sx_sb_freq = -1;
static int sx_sb_irq = -1;
static int sx_sb_dma = -1;
static int sx_sb_port = -1;
static int sx_sb_mute = 0;
static char * mixbuffer = NULL;

static int sx_sb_sel[2];                     /* selectors for the buffers */
static unsigned long sx_sb_buf[2];           /* pointers to the two buffers */
static int sx_sb_bufnum = 0;                 /* the one currently in use */

static int sx_sb_master_vol;                 /* stored mixer settings */
static int sx_sb_digi_vol;
static int sx_sb_fm_vol;

static void sx_sb_lock_mem();


/* sx_sb_read_dsp:
 *  Reads a byte from the SB DSP chip. Returns -1 if it times out.
 */
static inline volatile int sx_sb_read_dsp()
{
   int x;

   for (x=0; x<0xffff; x++)
      if (inportb(0x0E + sx_sb_port) & 0x80)
	 return inportb(0x0A+sx_sb_port);

   return -1; 
}



/* sx_sb_write_dsp:
 *  Writes a byte to the SB DSP chip. Returns -1 if it times out.
 */
static inline volatile int sx_sb_write_dsp(unsigned char byte)
{
   int x;

   for (x=0; x<0xffff; x++) {
      if (!(inportb(0x0C+sx_sb_port) & 0x80)) {
	 outportb(0x0C+sx_sb_port, byte);
	 return 0;
      }
   }
   return -1; 
}



/* sx_sb_voice:
 *  Turns the SB speaker on or off.
 */
void sx_sb_voice(int state)
{
   if (state) {
      sx_sb_write_dsp(0xD1);

      if (sx_sb_hw_dsp_ver >= 0x300) {       /* set up the mixer */

	 outportb(sx_sb_port+4, 0x22);      /* store master volume */
	 sx_sb_master_vol = inportb(sx_sb_port+5);

	 outportb(sx_sb_port+4, 4);         /* store DAC level */
	 sx_sb_digi_vol = inportb(sx_sb_port+5);

	 outportb(sx_sb_port+4, 0x26);      /* store FM level */
	 sx_sb_fm_vol = inportb(sx_sb_port+5);
      }
   }
   else {
      sx_sb_write_dsp(0xD3);

      if (sx_sb_hw_dsp_ver >= 0x300) {       /* reset previous mixer settings */

	 outportb(sx_sb_port+4, 0x22);      /* restore master volume */
	 outportb(sx_sb_port+5, sx_sb_master_vol);

	 outportb(sx_sb_port+4, 4);         /* restore DAC level */
	 outportb(sx_sb_port+5, sx_sb_digi_vol);

	 outportb(sx_sb_port+4, 0x26);      /* restore FM level */
	 outportb(sx_sb_port+5, sx_sb_fm_vol);
      }
   }
}



/* sx_sb_set_mixer:
 *  Alters the SB-Pro hardware mixer.
 */
int sx_sb_set_mixer(int digi_volume, int midi_volume)
{
   if (sx_sb_hw_dsp_ver < 0x300)
      return -1;

   if (digi_volume >= 0) {                   /* set DAC level */
      outportb(sx_sb_port+4, 4);
      outportb(sx_sb_port+5, (digi_volume & 0xF0) | (digi_volume >> 4));
   }

   if (midi_volume >= 0) {                   /* set FM level */
      outportb(sx_sb_port+4, 0x26);
      outportb(sx_sb_port+5, (midi_volume & 0xF0) | (midi_volume >> 4));
   }

   return 0;
}

/* sx_sb_mixer_volume:
 *  Sets the SB mixer volume for playing digital samples.
 */
static int sx_sb_mixer_volume(int volume)
{
   return sx_sb_set_mixer(volume, -1);
}

/* sx_sb_stereo_mode:
 *  Enables or disables stereo output for SB-Pro.
 */
static void sx_sb_stereo_mode(int enable)
{
   outportb(sx_sb_port+0x04, 0x0E); 
   outportb(sx_sb_port+0x05, (enable ? 2 : 0));
}

/* sx_sb_set_sample_rate:
 *  The parameter is the rate to set in Hz (samples per second).
 */
static void sx_sb_set_sample_rate(unsigned int rate)
{
   if (sx_sb_16bit) {
      sx_sb_write_dsp(0x41);
      sx_sb_write_dsp(rate >> 8);
      sx_sb_write_dsp(rate & 0xff);
   }
   else {
      if (sx_sb_stereo)
	 rate *= 2;

      sx_sb_write_dsp(0x40);
      sx_sb_write_dsp((unsigned char)(256-1000000/rate));
   }
}



/* sx_sb_reset_dsp:
 *  Resets the SB DSP chip, returning -1 on error.
 */
int sx_sb_reset_dsp(int data)
{
   int x;

   outportb(0x06+sx_sb_port, data);

   for (x=0; x<8; x++)
      inportb(0x06+sx_sb_port);

   outportb(0x06+sx_sb_port, 0);

   if (sx_sb_read_dsp() != 0xAA)
      return -1;

   return 0;
}



/* sx_sb_read_dsp_version:
 *  Reads the version number of the SB DSP chip, returning -1 on error.
 */
int sx_sb_read_dsp_version()
{
   int x, y;

   if (sx_sb_hw_dsp_ver > 0)
      return sx_sb_hw_dsp_ver;

   if (sx_sb_port <= 0)
      sx_sb_port = 0x220;

   if (sx_sb_reset_dsp(1) != 0) {
      sx_sb_hw_dsp_ver = -1;
   }
   else {
      sx_sb_write_dsp(0xE1);
      x = sx_sb_read_dsp();
      y = sx_sb_read_dsp();
      sx_sb_hw_dsp_ver = ((x << 8) | y);
   }

   return sx_sb_hw_dsp_ver;
}



/* sx_sb_play_buffer:
 *  Starts a dma transfer of size bytes. On cards capable of it, the
 *  transfer will use auto-initialised dma, so there is no need to call
 *  this routine more than once. On older cards it must be called from
 *  the end-of-buffer handler to switch to the new buffer.
 */
static void sx_sb_play_buffer(int size)
{
   if (sx_sb_dsp_ver <= 0x200) {                /* 8 bit single-shot */
      sx_sb_write_dsp(0x14);
      sx_sb_write_dsp((size-1) & 0xFF);
      sx_sb_write_dsp((size-1) >> 8);
   }
   else if (sx_sb_dsp_ver < 0x400) {            /* 8 bit auto-initialised */
      sx_sb_write_dsp(0x48);
      sx_sb_write_dsp((size-1) & 0xFF);
      sx_sb_write_dsp((size-1) >> 8);
      sx_sb_write_dsp(0x90);
   }
   else {                                    /* 16 bit */
      size /= 2;
      sx_sb_write_dsp(0xB6);
      sx_sb_write_dsp((sx_sb_stereo ? 0x20 : 0x00) + 0x10);
      /*sx_sb_write_dsp(0x20);*/
      sx_sb_write_dsp((size-1) & 0xFF);
      sx_sb_write_dsp((size-1) >> 8);
   }
}

static END_OF_FUNCTION(sx_sb_play_buffer);



/* sx_sb_interrupt:
 *  The SB end-of-buffer interrupt handler. Swaps to the other buffer 
 *  if the card doesn't have auto-initialised dma, and then refills the
 *  buffer that just finished playing.
 */
static int sx_sb_interrupt()
{
   unsigned char isr;

   if (sx_sb_dsp_ver >= 0x400) {
      /* read SB16 ISR mask */
      outportb(sx_sb_port+4, 0x82);
      isr = inportb(sx_sb_port+5) & 7;

      if (isr & 4) {
	 /* MPU-401 interrupt */
	 /*_mpu_poll();*/
	 _eoi(sx_sb_irq);
	 return 0;
      }

      if (!(isr & 3)) {
	 /* unknown interrupt */
	 _eoi(sx_sb_irq);
	 return 0;
      }
   }

   if (sx_sb_dsp_ver <= 0x200) {                /* not auto-initialised */
      _dma_start(sx_sb_dma, sx_sb_buf[1-sx_sb_bufnum], sx_sb_dma_size, FALSE, FALSE);
      sx_sb_play_buffer(sx_sb_dma_size);
   }
   else {                                    /* poll dma position */
      sx_sb_dma_count++;
      if (sx_sb_dma_count > 16) {
	 sx_sb_bufnum = (_dma_todo(sx_sb_dma) > (unsigned)sx_sb_dma_size) ? 1 : 0;
	 sx_sb_dma_count = 0;
      }
   }

   if (!sx_sb_semaphore) {
      sx_sb_semaphore = TRUE;

      ENABLE();                              /* mix some more samples */
      sx_sb_mix_some_samples(sx_sb_buf[sx_sb_bufnum], _dos_ds);
      DISABLE();

      sx_sb_semaphore = FALSE;
   } 

   sx_sb_bufnum = 1 - sx_sb_bufnum; 

   if (sx_sb_16bit)                             /* acknowledge SB */
      inportb(sx_sb_port+0x0F);
   else
      inportb(sx_sb_port+0x0E);

   _eoi(sx_sb_irq);                            /* acknowledge interrupt */
   return 0;
}

static END_OF_FUNCTION(sx_sb_interrupt);

void sx_sb_mix_some_samples(unsigned long buf, unsigned short seg)
{
    int i;

    if (sx_sb_mute) {
       _farsetsel(seg);

       if (sx_sb_16bit) {
          for (i=0; i<sx_sb_dma_mix_size; i++) {
             _farnspokew(buf, 0x0);
             buf += 2;
          }
       }
       else {
          for (i=0; i<sx_sb_dma_mix_size; i++) {
             _farnspokeb(buf, 0x80);
             buf++;
          }
       }
    }
    else {
       extern void S9xMixSamples (unsigned char *, int);
       S9xMixSamples ((unsigned char *) mixbuffer, sx_sb_dma_mix_size);
       
       if (sx_sb_16bit)
           movedata(_my_ds(), (int)mixbuffer, seg, buf, sx_sb_dma_mix_size << 1);
       else
           movedata(_my_ds(), (int)mixbuffer, seg, buf, sx_sb_dma_mix_size);
    }
}

static END_OF_FUNCTION(sx_sb_mix_some_samples);

/* sx_sb_start:
 *  Starts up the sound output.
 */
static void sx_sb_start()
{
   sx_sb_bufnum = 0;

   sx_sb_voice(1);
   sx_sb_set_sample_rate(sx_sb_freq);

   if ((sx_sb_hw_dsp_ver >= 0x300) && (sx_sb_dsp_ver < 0x400))
      sx_sb_stereo_mode(sx_sb_stereo);

   if (sx_sb_dsp_ver <= 0x200)
      _dma_start(sx_sb_dma, sx_sb_buf[0], sx_sb_dma_size, FALSE, FALSE);
   else
      _dma_start(sx_sb_dma, sx_sb_buf[0], sx_sb_dma_size*2, TRUE, FALSE);

   sx_sb_play_buffer(sx_sb_dma_size);
}



/* sx_sb_stop:
 *  Stops the sound output.
 */
static void sx_sb_stop()
{
   /* halt sound output */
   sx_sb_voice(0);

   /* stop dma transfer */
   _dma_stop(sx_sb_dma);

   if (sx_sb_dsp_ver <= 0x0200)
      sx_sb_write_dsp(0xD0); 

   sx_sb_reset_dsp(1);
}



/* sx_sb_detect:
 *  SB detection routine. Uses the BLASTER environment variable,
 *  or 'sensible' guesses if that doesn't exist.
 */
int sx_sb_detect (int *stereo, int *bits16)
{
   char *blaster = getenv("BLASTER");
   char *msg;
   int cmask;
   int max_freq;
   int default_freq;

   /* parse BLASTER env */
   if (blaster) { 
      while (*blaster) {
	 while ((*blaster == ' ') || (*blaster == '\t'))
	    blaster++;

	 if (*blaster) {
	    switch (*blaster) {

	       case 'a': case 'A':
		  if (sx_sb_port < 0)
		     sx_sb_port = strtol(blaster+1, NULL, 16);
		  break;

	       case 'i': case 'I':
		  if (sx_sb_irq < 0)
		     sx_sb_irq = strtol(blaster+1, NULL, 10);
		  break;

	       case 'd': case 'D':
		  sx_sb_dma8 = strtol(blaster+1, NULL, 10);
		  break;

	       case 'h': case 'H':
		  sx_sb_dma16 = strtol(blaster+1, NULL, 10);
		  break;
	    }

	    while ((*blaster) && (*blaster != ' ') && (*blaster != '\t'))
	       blaster++;
	 }
      }
   }

   if (sx_sb_port < 0)
      sx_sb_port = 0x220;

   /* make sure we got a good port address */
   if (sx_sb_reset_dsp(1) != 0) { 
      static int bases[] = { 0x210, 0x220, 0x230, 0x240, 0x250, 0x260, 0 };
      int i;

      for (i=0; bases[i]; i++) {
	 sx_sb_port = bases[i];
	 if (sx_sb_reset_dsp(1) == 0)
	    break;
      }
   }

   /* check if the card really exists */
   sx_sb_read_dsp_version();
   if (sx_sb_hw_dsp_ver < 0) {
      strcpy(allegro_error, "Sound Blaster not found");
      return FALSE;
   }

   sx_sb_dsp_ver = sx_sb_hw_dsp_ver;

   if (sx_sb_dsp_ver >= 0x400) {
      /* read configuration from SB16 card */
      if (sx_sb_irq < 0) {
	 outportb(sx_sb_port+4, 0x80);
	 cmask = inportb(sx_sb_port+5);
	 if (cmask&1) sx_sb_irq = 2; /* or 9? */
	 if (cmask&2) sx_sb_irq = 5;
	 if (cmask&4) sx_sb_irq = 7;
	 if (cmask&8) sx_sb_irq = 10;
      }
      if ((sx_sb_dma8 < 0) || (sx_sb_dma16 < 0)) {
	 outportb(sx_sb_port+4, 0x81);
	 cmask = inportb(sx_sb_port+5);
	 if (sx_sb_dma8 < 0) {
	    if (cmask&1) sx_sb_dma8 = 0;
	    if (cmask&2) sx_sb_dma8 = 1;
	    if (cmask&8) sx_sb_dma8 = 3;
	 }
	 if (sx_sb_dma16 < 0) {
	    sx_sb_dma16 = sx_sb_dma8;
	    if (cmask&0x20) sx_sb_dma16 = 5;
	    if (cmask&0x40) sx_sb_dma16 = 6;
	    if (cmask&0x80) sx_sb_dma16 = 7;
	 }
      }
   }

   /* if nothing else works */
   if (sx_sb_irq < 0)
      sx_sb_irq = 5;

   if (sx_sb_dma8 < 0)
      sx_sb_dma8 = 1;

   if (sx_sb_dma16 < 0)
      sx_sb_dma16 = 5;

   /* figure out the hardware interrupt number */
   sx_sb_int = _map_irq(sx_sb_irq);

   /* what breed of SB? */
   if (sx_sb_dsp_ver >= 0x400) {
      msg = "SB 16";
      max_freq = 45454;
      default_freq = 22727;
   }
   else if (sx_sb_dsp_ver >= 0x300) {
      msg = "SB Pro";
      max_freq = 22727;
      default_freq = 22727;
   }
   else if (sx_sb_dsp_ver >= 0x201) {
      msg = "SB 2.0";
      max_freq = 45454;
      default_freq = 22727;
   }
   else if (sx_sb_dsp_ver >= 0x200) {
      msg = "SB 1.5";
      max_freq = 16129;
      default_freq = 16129;
   }
   else {
      msg = "SB 1.0";
      max_freq = 16129;
      default_freq = 16129;
   }

   /* set up the playback frequency */
   if (sx_sb_freq <= 0)
      sx_sb_freq = default_freq;

#if 0
   if (sx_sb_freq < 15000) {
      sx_sb_freq = 11906;
      sx_sb_dma_size = 128;
   }
   else if (MIN(sx_sb_freq, max_freq) < 20000) {
      sx_sb_freq = 16129;
      sx_sb_dma_size = 128;
   }
   else if (MIN(sx_sb_freq, max_freq) < 40000) {
      sx_sb_freq = 22727;
      sx_sb_dma_size = 256;
   }
   else {
      sx_sb_freq = 45454;
      sx_sb_dma_size = 512;
   }
#endif

   if (sx_sb_dsp_ver <= 0x200)
      sx_sb_dma_size *= 4;

   sx_sb_dma_mix_size = sx_sb_dma_size;

   /* can we handle 16 bit sound? */
   if (sx_sb_dsp_ver >= 0x400) { 
      if (sx_sb_dma < 0)
	 sx_sb_dma = sx_sb_dma16;
      else
	 sx_sb_dma16 = sx_sb_dma;
      sx_sb_16bit = TRUE;
      sx_sb_dma_size <<= 1;
   }
   else { 
      if (sx_sb_dma < 0)
	 sx_sb_dma = sx_sb_dma8;
      else
	 sx_sb_dma8 = sx_sb_dma;
      sx_sb_16bit = FALSE;
   }

   
   /* can we handle stereo? */
   if (sx_sb_dsp_ver >= 0x300 && *stereo) {
      sx_sb_dma_size <<= 1;
      sx_sb_dma_mix_size <<= 1;
      sx_sb_stereo = TRUE;
   }
   else {
      sx_sb_stereo = FALSE;
   }

   *bits16 = sx_sb_16bit;
   *stereo = sx_sb_stereo;
   
   /* set up the card description */
   sprintf(sx_sb_desc, "%s (%d hz) on port %X, using IRQ %d and DMA channel %d",
			msg, sx_sb_freq, sx_sb_port, sx_sb_irq, sx_sb_dma);

   return TRUE;
}



/* sx_sb_init:
 *  SB init routine: returns zero on success, -1 on failure.
 */
int sx_sb_init (int *stereo, int *bits16, int *freq, int *dmamixsize)
{
   sx_sb_dsp_ver = -1;
   
   if (*dmamixsize < SB_DMA_BUFFER_SIZE)
       *dmamixsize = SB_DMA_BUFFER_SIZE;
   else if (*dmamixsize > 8191)
       *dmamixsize = 8192;

   sx_sb_dma_size = sx_sb_dma_mix_size = *dmamixsize;

   if (*freq > 5000 && *freq <= 44100)
       sx_sb_freq = *freq;

   if (!sx_sb_detect (stereo, bits16))
      return -1;

   *dmamixsize = sx_sb_dma_mix_size;
   mixbuffer = calloc(1, sx_sb_dma_mix_size * sizeof (short));
   if (!mixbuffer)
      return -1;
   memset (mixbuffer, 0, sx_sb_dma_mix_size * sizeof (short));

   _go32_dpmi_lock_data(mixbuffer, sx_sb_dma_mix_size*sizeof(short));

   if (sx_sb_dsp_ver <= 0x200) {       /* two conventional mem buffers */
      if ((_dma_allocate_mem(sx_sb_dma_size, &sx_sb_sel[0], &sx_sb_buf[0]) != 0) ||
	  (_dma_allocate_mem(sx_sb_dma_size, &sx_sb_sel[1], &sx_sb_buf[1]) != 0))
	 return -1;
   }
   else {                           /* auto-init dma, one big buffer */
      if (_dma_allocate_mem(sx_sb_dma_size*2, &sx_sb_sel[0], &sx_sb_buf[0]) != 0)
	 return -1;

      sx_sb_sel[1] = sx_sb_sel[0];
      sx_sb_buf[1] = sx_sb_buf[0] + sx_sb_dma_size;
   }

   sx_sb_lock_mem();

#if 0
   if (_mixer_init(sx_sb_dma_mix_size, sx_sb_freq, sx_sb_stereo, sx_sb_16bit, &digi_sb.voices) != 0)
      return -1;
#endif

   sx_sb_mix_some_samples(sx_sb_buf[0], _dos_ds);
   sx_sb_mix_some_samples(sx_sb_buf[1], _dos_ds);

   _enable_irq(sx_sb_irq);
   _install_irq(sx_sb_int, sx_sb_interrupt);

   sx_sb_start();

   sx_sb_in_use = TRUE;
   return 0;
}


/* sx_sb_exit:
 *  SB driver cleanup routine, removes ints, stops dma, frees buffers, etc.
 */
void sx_sb_exit ()
{
   sx_sb_stop();
   _remove_irq(sx_sb_int);
   _restore_irq(sx_sb_irq);

   __dpmi_free_dos_memory(sx_sb_sel[0]);
   if (sx_sb_sel[1] != sx_sb_sel[0])
      __dpmi_free_dos_memory(sx_sb_sel[1]);

    free (mixbuffer);
    mixbuffer = NULL;

   sx_sb_hw_dsp_ver = sx_sb_dsp_ver = -1;
   sx_sb_in_use = FALSE;
}

/* sx_sb_lock_mem:
 *  Locks all the memory touched by parts of the SB code that are executed
 *  in an interrupt context.
 */
static void sx_sb_lock_mem()
{
   LOCK_VARIABLE(sx_sb_freq);
   LOCK_VARIABLE(sx_sb_port);
   LOCK_VARIABLE(sx_sb_dma);
   LOCK_VARIABLE(sx_sb_irq);
   LOCK_VARIABLE(sx_sb_int);
   LOCK_VARIABLE(sx_sb_in_use);
   LOCK_VARIABLE(sx_sb_16bit);
   LOCK_VARIABLE(sx_sb_dsp_ver);
   LOCK_VARIABLE(sx_sb_hw_dsp_ver);
   LOCK_VARIABLE(sx_sb_dma_size);
   LOCK_VARIABLE(sx_sb_dma_mix_size);
   LOCK_VARIABLE(sx_sb_sel);
   LOCK_VARIABLE(sx_sb_buf);
   LOCK_VARIABLE(sx_sb_bufnum);
   LOCK_VARIABLE(sx_sb_dma_count);
   LOCK_VARIABLE(sx_sb_semaphore);
   LOCK_FUNCTION(sx_sb_play_buffer);
   LOCK_FUNCTION(sx_sb_interrupt);
   LOCK_FUNCTION(sx_sb_mix_some_samples);
}


