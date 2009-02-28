#include "snesc.h"

extern void* __nmi_handler;

unsigned char snesc_do_copy; /* bit 7: do sprite copy, bit 6: do palette copy
                                bits 0-5: DMA transfer count */
unsigned char snesc_timer_enabled; /* each bit one timer */
unsigned int snesc_timers[16];

unsigned int snesc_oam_table1[256];	/* OAM low table */
unsigned int snesc_oam_table2[16];	/* OAM high table */

unsigned char snesc_palette[512];	/* palette */

unsigned int snesc_controllers[4];	/* data from 4 controllers */

unsigned char snesc_dma_transfers[64*8]; /* DMA transfers
                                            0 src address (24 bit)
                                            3 dest address (16 bit)
                                            5 size (16 bit)
                                            7 type (8 bit, 0 == VRAM, 1 == OAM, 2 == CGRAM, >2 == ???) */

void do_dma(unsigned char do_flags)
{
  if (do_flags & 0x80) {	/* copy sprites? */
    *((unsigned short*)0x4300) = 0x400;	/* DMA channel 0 Bus B addr $2104 (OAM write) */
    /* the original code loads $8000, turning on sprite priority and
       messing up the gfx (some emulators don't emulate this
       correctly, which is probably why it has slipped through) */
    *(unsigned short*)0x2102 = 0;	/* OAM address 0 */
    *((void**)0x4302) = snesc_oam_table1; /* DMA source address (24 bits); hairy: writes 32 bits... */
    *(unsigned short*)0x4305 = 0x220;	/* DMA size */
    *(unsigned char*)0x420b = 1;	/* enable DMA 0 */
  }
  if (do_flags & 0x40) {	/* copy palette? */
    *(unsigned short*)0x4300 = 0x2200;	/* DMA channel 0 Bus B addr $2122 (CGRAM write) */
    *((void**)0x4302) = snesc_palette;	/* DMA source address; see above */
    *(unsigned short*)0x4305 = 0x200;	/* DMA size */
    *(unsigned char*)0x2121 = 0;	/* CGRAM address 0 */
    *(unsigned char*)0x420b = 1;	/* enable DMA 0 */
  }
}

void snesc_vblank(void)
{
  /* stuff pending for DMA? */
  unsigned char do_flags = snesc_do_copy;
  if(do_flags) {
    snesc_do_copy = 0;
    do_dma(do_flags);
    do_flags &= 0x3f;	/* mask out palette and sprite flags (used by do_dma) */
    if (do_flags) {
      int x = 0;
      while (do_flags) {
        switch(snesc_dma_transfers[7+x]) {	/* type of transfer */
        case 0:	/* VRAM */
          *((unsigned short*)0x4300) = 0x1801;	/* 2 regs write once, Bus B addr $2118 (VRAM data write) */
          *((unsigned short*)0x2116) = *(unsigned short*)(snesc_dma_transfers + 3+x);	/* VRAM address */
          break;
        case 1:	/* OAM */
          *((unsigned short*)0x4300) = 0x400;	/* DMA OAM write */
          *((unsigned short*)0x2102) = *(unsigned short*)(snesc_dma_transfers + 3+x);	/* OAM address */
          break;
        case 2:	/* CGRAM */
          *((unsigned short*)0x4300) = 0x2200;	/* CGRAM write */
          *((unsigned char*)0x2121) = snesc_dma_transfers[3+x];	/* CGRAM address */
          break;
        }
        *((unsigned short*)0x4302) = *(unsigned short*)(snesc_dma_transfers + x);	/* DMA source address (16-bit) */
        *((unsigned short*)0x4305) = *(unsigned short*)(snesc_dma_transfers + 5 + x);	/* DMA size */
        *((unsigned char*)0x4304) = snesc_dma_transfers[2+x];	/* DMA source address (bank) */
        *((unsigned char*)0x420b) = 1;	/* enable DMA 0 */
        x += 8;
        do_flags--;
      }
    }
  }

#if 0
  /* does not work for unknown reasons; asm code looks OK; could be a timing issue */
  while(*((unsigned char*)0x4212) & 1) {}
#else
  unsigned char* autojoy = (unsigned char*)0x4212;
  while(*autojoy & 1) {}
#endif
  
  /* update input buffers */
  unsigned int pad;
  for(pad = 0; pad < 4 ; pad++) {
    snesc_controllers[pad] |= ((unsigned short*)0x4218)[pad];
  }
  
  /* timer ticks */
  unsigned char timers_enabled = snesc_timer_enabled;
  unsigned int tc = 0;
  while(timers_enabled & 1) {
    snesc_timers[tc]++;
    tc++;
    timers_enabled >>= 1;
  }
}

void snesc_init(void)
{
  int i;
  __nmi_handler = snesc_vblank;	/* register vblank handler */
  *((unsigned char*)0x4200) = 0x81; /* enable NMI, enable autojoy */
  snesc_timer_enabled = snesc_do_copy = snesc_controllers[0] = 0;

  /* snesc sprite init stuff */
  for(i = 0; i < 256; i+=2) {
    snesc_oam_table1[i] = 0xe800;
    snesc_oam_table1[i+1] = 0;
  }
  for(i = 0; i < 32; i+=2) {
    snesc_oam_table2[i] = 0;
  }
}
