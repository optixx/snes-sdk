/* graphics */

void settiles(unsigned int b, unsigned char *p1, unsigned int size);
void setmap(unsigned int b, unsigned char *p1);
void setpalette(unsigned char *pal);
void enablescreen();
void screenmode(unsigned char m);
void setsprite(unsigned int s, unsigned char x, unsigned char y, unsigned char t, unsigned char p);
void waitforvsync();
void delay(unsigned int d);
void resettimer();
void sync(unsigned int d);

/* init */
void snesc_init(void);

/* input */
#define TR_BUTTON		0x0010
#define TL_BUTTON		0x0020
#define Y_BUTTON		0x0040
#define X_BUTTON		0x0080
#define RIGHT_BUTTON	0x0100
#define LEFT_BUTTON	0x0200
#define DOWN_BUTTON	0x0400
#define UP_BUTTON		0x0800
#define START_BUTTON	0x1000
#define SELECT_BUTTON	0x2000
#define B_BUTTON		0x4000
#define A_BUTTON		0x8000
unsigned int getjoystatus(unsigned int j);
void clearjoy(unsigned int j);

/* string */
void addbcd(long long *bcd, unsigned int n);
void writebcd(long long bcd, unsigned int *map, unsigned int p, unsigned int offset);
void writestring(unsigned char *st, unsigned int *map, unsigned int p, unsigned int offset);

extern unsigned char snesc_timer_enabled;
extern unsigned int snesc_timers[16];
extern unsigned int snesc_controllers[4];
extern unsigned char snesc_do_copy;

struct oam_table1 {
  unsigned char x;
  unsigned char y;
  unsigned char t;
  unsigned char p;  
};

extern struct oam_table1 snesc_oam_table1[128];
extern unsigned int snesc_oam_table2[32];
extern unsigned char snesc_palette[512];

struct dma_transfer {
  union {
    struct {
      unsigned short addr; /* 0 + 1 */
      unsigned char bank; /* 2 */
      unsigned char type;
    } c;
    unsigned char *ptr;
  } src;
  unsigned short dest; /* 3 + 4 */
  unsigned short size; /* 5 + 6 */
};

extern struct dma_transfer snesc_dma_transfers[64];
