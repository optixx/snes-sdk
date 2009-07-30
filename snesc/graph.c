#include "snesc.h"

void enablescreen(void)
{
  *(unsigned char*)0x2105 = 9; /* BG mode 1 */
  *(unsigned char*)0x2109 = 4; /* BG3 map location */
  *(unsigned char*)0x210b = 1; /* BG1/2 tiles location */
  *(unsigned char*)0x210c = 2; /* BG3/4 tiles location */
  *(unsigned char*)0x212c = 0x15; /* enable BG1, BG3, OBJ display */
  *(unsigned char*)0x2100 = 0x80; /* force blank */
  snesc_vblank();	/* flush pending DMA jobs before turning on screen */
  *(unsigned char*)0x2100 = 0xf; /* turn on screen, full brightness */
}

void screenmode(unsigned char m)
{
  *(unsigned char*)0x2105 = m;
}

void waitforvsync(void)
{
  snesc_timer_enabled |= 1;
  snesc_timers[0] = 0;
  while(!snesc_timers[0]) {}
}

void delay(unsigned int d)
{
  snesc_timer_enabled |= 1;
  snesc_timers[0] = 0;
  while (snesc_timers[0] < d) {}
}

void setpalette(unsigned char *pal)
{
  memcpy(snesc_palette, pal, 0x200);
  snesc_do_copy |= 0x40;
}

void setsprite(unsigned int s, unsigned char x, unsigned char y, unsigned char t, unsigned char p)
{
  struct oam_table1* spr = &snesc_oam_table1[s];
  spr->x = x;
  spr->y = y;
  spr->t = t;
  spr->p = p;
  snesc_do_copy |= 0x80;
}

void sync(unsigned int d)
{
  while (snesc_timers[0] < d) {}
}

void resettimer()
{
  snesc_timer_enabled |= 1;
  snesc_timers[0] = 0;
}

void settiles(unsigned int b, unsigned char *p1, unsigned int size)
{
  unsigned int idx = snesc_do_copy & 0x3f;
  struct dma_transfer *tr = &snesc_dma_transfers[idx];
  /* tile data */
  tr->src.ptr = p1;
  tr->src.c.type = 0;	/* src.ptr and type overlap, so type must be set after */
  tr->dest = (b + 1) << 12;
  tr->size = size;

  /* signal the NMI to copy data to VRAM */
  snesc_do_copy++;
}

void setmap(unsigned int b, unsigned char *p1)
{
  struct dma_transfer *tr = &snesc_dma_transfers[snesc_do_copy & 0x3f];
  /* tile data */
  tr->src.ptr = p1;
  tr->src.c.type = 0;
  tr->dest = b << 10;
  tr->size = 0x800;
  
  /* signal the NMI to copy data to VRAM */
  snesc_do_copy++;
}
