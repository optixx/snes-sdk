#include "snesc.h"

extern void* __nmi_handler;
extern void __snesc_vblank(void);

void snesc_init(void)
{
  int i;
  __nmi_handler = __snesc_vblank;	/* register vblank handler */
  *((unsigned char*)0x4200) = 0x81; /* dammit, forgot what that does */
  snesc_timer_enabled = snesc_do_copy = snesc_controllers[0] = 0;

  /* snesc sprite init stuff */
  for(i = 0; i < 256; i+=2) {
    snesc_oam_table1[i] = 0xe800;
    snesc_oam_table1[i+1] = 0;
  }
  for(i = 0; i < 32; i+=2) {
    snesc_oam_table2[i] = 0;
  }

  /* asm code used to do cli, wai here; does not seem to be necessary */
}
