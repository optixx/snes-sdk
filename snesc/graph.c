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
