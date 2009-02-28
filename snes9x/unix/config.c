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
 *      Configuration routines.
 *
 *      See readme.txt for copyright information.
 */


#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>

#include "snes9x.h"

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

typedef struct CONFIG_ENTRY
{
   char *name;                      /* variable name (NULL if comment) */
   char *data;                      /* variable value */
   struct CONFIG_ENTRY *next;       /* linked list */
} CONFIG_ENTRY;


typedef struct CONFIG
{
   CONFIG_ENTRY *head;              /* linked list of config entries */
   char *filename;                  /* where were we loaded from? */
   int dirty;                       /* has our data changed? */
} CONFIG;


#define MAX_CONFIGS     4

static CONFIG *config[MAX_CONFIGS] = { NULL, NULL, NULL, NULL };
static CONFIG *config_override = NULL;

/* static int config_installed = FALSE; */ /* Dead code */

char *get_filename(char *path)
{
    int pos;
   
    for (pos=0; path[pos]; pos++)
        ; /* do nothing */
	    
    while ((pos>0) && (path[pos-1] != '\\') && (path[pos-1] != '/'))
	pos--;
    return path+pos;
}

long file_size (char *filename)
{
    FILE *fs = fopen (filename, "r");

    if (fs)
    {
	long len;
	
	fseek (fs, 0, SEEK_END);
	len = ftell (fs);
	fclose (fs);

	return (len);
    }
    return (0);
}

/* destroy_config:
 *  Destroys a config structure, writing it out to disk if the contents
 *  have changed.
 */
static void destroy_config(CONFIG *cfg)
{
   CONFIG_ENTRY *pos, *prev;

   if (cfg) {
      if (cfg->filename) {
	 if (cfg->dirty) {
	    /* write changed data to disk */
	    FILE *f = fopen(cfg->filename, "w");

	    if (f) {
	       pos = cfg->head;

	       while (pos) {
		  if (pos->name) {
		     fputs(pos->name, f);
		     if (pos->name[0] != '[')
			fputs(" = ", f);
		  }
		  if (pos->data)
		     fputs(pos->data, f);

		  fputs("\n", f);
		  pos = pos->next;
	       }

	       fclose(f);
	    }
	 }

	 free(cfg->filename);
      }

      /* destroy the variable list */
      pos = cfg->head;

      while (pos) {
	 prev = pos;
	 pos = pos->next;

	 if (prev->name)
	    free(prev->name);

	 if (prev->data)
	    free(prev->data);

	 free(prev);
      }

      free(cfg);
   }
}



#if 0 /* Dead code */
/* config_cleanup:
 *  Called at shutdown time to free memory being used by the config routines,
 *  and write any changed data out to disk.
 */
static void config_cleanup()
{
   int i;

   for (i=0; i<MAX_CONFIGS; i++) {
      if (config[i]) {
	 destroy_config(config[i]);
	 config[i] = NULL;
      }
   }

   if (config_override) {
      destroy_config(config_override);
      config_override = NULL;
   }

/*   _remove_exit_func(config_cleanup);*/
   config_installed = FALSE;
}
#endif /* Dead code */

/* init_config:
 *  Sets up the configuration routines ready for use, also loading the
 *  default config file if the loaddata flag is set and no other config
 *  file is in memory.
 */
static void init_config(int loaddata)
{
#if 0
   char buf[4][1025];
   char *s;
   int i;

   if (!config_installed) {
/*      _add_exit_func(config_cleanup);*/
      config_installed = TRUE;
   }

   if ((loaddata) && (!config[0])) {
      /* look for allegro.cfg in the same directory as the program */
      strcpy(buf[0], __crt0_argv[0]);
      strlwr(buf[0]);
      *get_filename(buf[0]) = 0;
      put_backslash(buf[0]);
      strcat(buf[0], "allegro.cfg");

      /* if that fails, try sound.cfg */
      strcpy(buf[1], __crt0_argv[0]);
      strlwr(buf[1]);
      *get_filename(buf[1]) = 0;
      put_backslash(buf[1]);
      strcat(buf[1], "sound.cfg");

      /* no luck? try the ALLEGRO enviroment variable... */
      s = getenv("ALLEGRO");
      if (s) {
	 strcpy(buf[2], s);
	 strlwr(buf[2]);
	 put_backslash(buf[2]);
	 strcat(buf[2], "allegro.cfg");

	 strcpy(buf[3], s);
	 strlwr(buf[3]);
	 put_backslash(buf[3]);
	 strcat(buf[3], "sound.cfg");
      }
      else {
	 strcpy(buf[2], buf[0]);
	 strcpy(buf[3], buf[1]);
      }
      /* see which of these files actually exist */
      for (i=0; i<4; i++) {
	 if (access (buf[i], R_OK)) {
	    set_config_file(buf[i]);
	    break;
	 }
      }

      if (i >= 4)
	 set_config_file(buf[0]);
   }
#endif   
}

/* get_line: 
 *  Helper for splitting files up into individual lines.
 */
static int get_line(char *data, int length, char *name, char *val)
{
   char buf[256], buf2[256];
   int pos, i, j;

   for (pos=0; (pos<length) && (pos<255); pos++) {
      if ((data[pos] == '\r') || (data[pos] == '\n')) {
	 buf[pos] = 0;
	 if ((pos < length-1) && 
	     (((data[pos] == '\r') && (data[pos+1] == '\n')) ||
	      ((data[pos] == '\n') && (data[pos+1] == '\r')))) {
	    pos++;
	 }
	 pos++;
	 break;
      }

      buf[pos] = data[pos];
   }

   buf[MIN(pos,255)] = 0;

   /* skip leading spaces */
   i = 0;
   while ((buf[i]) && (isspace((int)buf[i])))
      i++;

   /* read name string */
   j = 0;
   while ((buf[i]) && (!isspace((int)buf[i])) && (buf[i] != '=') && (buf[i] != '#'))
      buf2[j++] = buf[i++];

   if (j) {
      /* got a variable */
      buf2[j] = 0;
      strcpy(name, buf2);

      while ((buf[i]) && ((isspace((int)buf[i])) || (buf[i] == '=')))
	 i++;

      strcpy(val, buf+i);

      /* strip trailing spaces */
      i = strlen(val) - 1;
      while ((i >= 0) && (isspace((int)val[i])))
	 val[i--] = 0;
   }
   else {
      /* blank line or comment */
      name[0] = 0;
      strcpy(val, buf);
   }

   return pos;
}



/* set_config:
 *  Does the work of setting up a config structure.
 */
static void set_config(CONFIG **config, char *data, int length, char *filename)
{
   char name[256];
   char val[256];
   CONFIG_ENTRY **prev, *p;
   int pos;

   init_config(FALSE);

   if (*config) {
      destroy_config(*config);
      *config = NULL;
   }

   *config = (CONFIG *) malloc(sizeof(CONFIG));
   (*config)->head = NULL;
   (*config)->dirty = FALSE;

   if (filename) {
      (*config)->filename = (char *) malloc(strlen(filename)+1);
      strcpy((*config)->filename, filename); 
   }
   else
      (*config)->filename = NULL;

   prev = &(*config)->head;
   pos = 0;

   while (pos < length) {
      pos += get_line(data+pos, length-pos, name, val);

      p = (CONFIG_ENTRY *) malloc(sizeof(CONFIG_ENTRY));

      if (name[0]) {
	 p->name = (char *) malloc(strlen(name)+1);
	 strcpy(p->name, name);
      }
      else
	 p->name = NULL;

      p->data = (char *) malloc(strlen(val)+1);
      strcpy(p->data, val);

      p->next = NULL;
      *prev = p;
      prev = &p->next;
   }
}



/* load_config_file:
 *  Does the work of loading a config file.
 */
static void load_config_file(CONFIG **config, char *filename, char *savefile)
{
   int length = file_size(filename);

   if (length > 0) {
      FILE *f = fopen(filename, "r");
      if (f) {
	 char *tmp = (char *) malloc(length);
	 fread(tmp, 1, length, f);
	 fclose(f);
	 set_config(config, tmp, length, savefile);
	 free(tmp);
      }
      else
	 set_config(config, NULL, 0, savefile);
   }
   else
      set_config(config, NULL, 0, savefile);
}



/* set_config_file:
 *  Sets the file to be used for all future configuration operations.
 */
void set_config_file(char *filename)
{
   load_config_file(&config[0], filename, filename);
}



/* set_config_data:
 *  Sets the block of data to be used for all future configuration 
 *  operations.
 */
void set_config_data(char *data, int length)
{
   set_config(&config[0], data, length, NULL);
}



/* override_config_file:
 *  Sets the file that will override all future configuration operations.
 */
void override_config_file(char *filename)
{
   load_config_file(&config_override, filename, NULL);
}



/* override_config_data:
 *  Sets the block of data that will override all future configuration 
 *  operations.
 */
void override_config_data(char *data, int length)
{
   set_config(&config_override, data, length, NULL);
}



/* push_config_state:
 *  Pushes the current config state onto the stack.
 */
void push_config_state()
{
   int i;

   if (config[MAX_CONFIGS-1])
      destroy_config(config[MAX_CONFIGS-1]);

   for (i=MAX_CONFIGS-1; i>0; i--)
      config[i] = config[i-1];

   config[0] = NULL;
}



/* pop_config_state:
 *  Pops the current config state off the stack.
 */
void pop_config_state()
{
   int i;

   if (config[0])
      destroy_config(config[0]);

   for (i=0; i<MAX_CONFIGS-1; i++)
      config[i] = config[i+1];

   config[MAX_CONFIGS-1] = NULL;
}



/* prettify_section_name:
 *  Helper for ensuring that a section name is enclosed by [ ] braces.
 */
static void prettify_section_name(char *in, char *out)
{
   if (in) {
      if (in[0] != '[')
	 strcpy(out, "[");
      else
	 out[0] = 0;

      strcat(out, in);

      if (out[strlen(out)-1] != ']')
	 strcat(out, "]");
   }
   else
      out[0] = 0;
}



/* find_config_string:
 *  Helper for finding an entry in the configuration file.
 */
static CONFIG_ENTRY *find_config_string(CONFIG *config, char *section, char *name, CONFIG_ENTRY **prev)
{
   CONFIG_ENTRY *p;
   int in_section = TRUE;
   char section_name[256];

   prettify_section_name(section, section_name);

   if (config) {
      p = config->head;

      if (prev)
	 *prev = NULL;

      while (p) {
	 if (p->name) {
	    if ((p->name[0] == '[') && (p->name[strlen(p->name)-1] == ']')) {
	       /* change section */
	       in_section = (strcasecmp(section_name, p->name) == 0);
	    }
	    if ((in_section) || (name[0] == '[')) {
	       /* is this the one? */
	       if (strcasecmp(p->name, name) == 0)
		  return p;
	    }
	 }

	 if (prev)
	    *prev = p;

	 p = p->next;
      }
   }

   return NULL;
}



/* get_config_string:
 *  Reads a string from the configuration file.
 */
char *get_config_string(char *section, char *name, char *def)
{
   CONFIG_ENTRY *p;

   init_config(TRUE);

   p = find_config_string(config_override, section, name, NULL);

   if (!p)
      p = find_config_string(config[0], section, name, NULL);

   if (p)
      return (p->data ? p->data : "");
   else
      return def;
}



/* get_config_int:
 *  Reads an integer from the configuration file.
 */
int get_config_int(char *section, char *name, int def)
{
   char *s = get_config_string(section, name, NULL);

   if ((s) && (*s))
      return strtol(s, NULL, 0);

   return def;
}



/* get_config_hex:
 *  Reads a hexadecimal integer from the configuration file.
 */
int get_config_hex(char *section, char *name, int def)
{
   char *s = get_config_string(section, name, NULL);
   int i;

   if ((s) && (*s)) {
      i = strtol(s, NULL, 16);
      if ((i == 0x7FFFFFFF) && (strcasecmp(s, "7FFFFFFF") != 0))
	 i = -1;
      return i;
   }

   return def;
}



/* get_config_float:
 *  Reads a float from the configuration file.
 */
float get_config_float(char *section, char *name, float def)
{
   char *s = get_config_string(section, name, NULL);

   if ((s) && (*s))
      return atof(s);

   return def;
}



/* get_config_argv:
 *  Reads an argc/argv style token list from the configuration file.
 */
char **get_config_argv(char *section, char *name, int *argc)
{
   #define MAX_ARGV  16

   static char buf[256];
   static char *argv[MAX_ARGV];
   int pos, ac;

   char *s = get_config_string(section, name, NULL);

   if (!s) {
      *argc = 0;
      return NULL;
   }

   strcpy(buf, s);
   pos = 0;
   ac = 0;

   while ((ac<MAX_ARGV) && (buf[pos]) && (buf[pos] != '#')) {
      while ((buf[pos]) && (isspace((int)buf[pos])))
	 pos++;

      if ((buf[pos]) && (buf[pos] != '#')) {
	 argv[ac++] = buf+pos;

	 while ((buf[pos]) && (!isspace((int)buf[pos])))
	    pos++;

	 if (buf[pos])
	    buf[pos++] = 0;
      }
   }

   *argc = ac;
   return argv;
}



/* insert_variable:
 *  Helper for inserting a new variable into a configuration file.
 */
static CONFIG_ENTRY *insert_variable(CONFIG_ENTRY *p, char *name, char *data)
{
   CONFIG_ENTRY *n = (CONFIG_ENTRY *) malloc(sizeof(CONFIG_ENTRY));

   if (name) {
      n->name = (char *) malloc(strlen(name)+1);
      strcpy(n->name, name);
   }
   else
      n->name = NULL;

   if (data) {
      n->data = (char *) malloc(strlen(data)+1);
      strcpy(n->data, data);
   }
   else
      n->data = NULL;

   if (p) {
      n->next = p->next;
      p->next = n; 
   }
   else {
      n->next = NULL;
      config[0]->head = n;
   }

   return n;
}



/* set_config_string:
 *  Writes a string to the configuration file.
 */
void set_config_string(char *section, char *name, char *val)
{
   CONFIG_ENTRY *p, *prev;
   char section_name[256];

   init_config(TRUE);

   if (config[0]) {
      p = find_config_string(config[0], section, name, &prev);

      if (p) {
	 if ((val) && (*val)) {
	    /* modify existing variable */
	    if (p->data)
	       free(p->data);

	    p->data = (char *) malloc(strlen(val)+1);
	    strcpy(p->data, val);
	 }
	 else {
	    /* delete variable */
	    if (p->name)
	       free(p->name);

	    if (p->data)
	       free(p->data);

	    if (prev)
	       prev->next = p->next;
	    else
	       config[0]->head = p->next;

	    free(p);
	 }
      }
      else {
	 if ((val) && (*val)) {
	    /* add a new variable */
	    prettify_section_name(section, section_name);

	    if (section_name[0]) {
	       p = find_config_string(config[0], NULL, section_name, &prev);

	       if (!p) {
		  /* create a new section */
		  p = config[0]->head;
		  while ((p) && (p->next))
		     p = p->next;

		  if ((p) && (p->data) && (*p->data))
		     p = insert_variable(p, NULL, NULL);

		  p = insert_variable(p, section_name, NULL);
	       }

	       /* append to the end of the section */
	       while ((p) && (p->next) && 
		      (((p->next->name) && (*p->next->name)) || 
		       ((p->next->data) && (*p->next->data))))
		  p = p->next;

	       p = insert_variable(p, name, val);
	    }
	    else {
	       /* global variable */
	       p = config[0]->head;
	       insert_variable(NULL, name, val);
	       config[0]->head->next = p;
	    }
	 } 
      }

      config[0]->dirty = TRUE;
   }
}



/* set_config_int:
 *  Writes an integer to the configuration file.
 */
void set_config_int(char *section, char *name, int val)
{
   char buf[32];
   sprintf(buf, "%d", val);
   set_config_string(section, name, buf);
}



/* set_config_hex:
 *  Writes a hexadecimal integer to the configuration file.
 */
void set_config_hex(char *section, char *name, int val)
{
   if (val >= 0) {
      char buf[32];
      sprintf(buf, "%X", val);
      set_config_string(section, name, buf);
   }
   else
      set_config_string(section, name, "-1");
}



/* set_config_float:
 *  Writes a float to the configuration file.
 */
void set_config_float(char *section, char *name, float val)
{
   char buf[32];
   sprintf(buf, "%f", val);
   set_config_string(section, name, buf);
}


