/* VLUPO TS R1.0 */
int vlupots_segts(unsigned int maddr);
int vlupots_segtsfly(unsigned int maddr);
void vlupots_intdelay_map(unsigned int maddr, short val);
void vlupots_pulse_map(unsigned int maddr, short val);
void vlupots_pulsewidth_map(unsigned int maddr, short val);
void vlupots_clear(unsigned int maddr);
void vlupots_clear_counter(unsigned int maddr);
int vlupots_segfifocnt(unsigned int maddr);
int vlupots_segtrgcnt(unsigned int maddr);
void vlupots_clkmode_map(unsigned int maddr, short val);
void vlupots_enable_interrupt(unsigned int maddr);
void vlupots_disable_interrupt(unsigned int maddr);


#define VLUPO_TS_DATA24L      0x00   // R   D32
#define VLUPO_TS_DATA24H      0x04   // R   D32
#define VLUPO_TS_DATA32L      0x08   // R   D32
#define VLUPO_TS_DATA32H      0x0c   // R   D32
#define VLUPO_TS_TRGCOUNTER   0x10   // R   D32
#define VLUPO_TS_FIFOCNT      0x14   // R   D32
#define VLUPO_TS_DATAFLY      0x18   // R   D32
#define VLUPO_TS_PULSEWIDTH   0x40   // RW D16
#define VLUPO_TS_INTDELAY     0x42   // RW D16
#define VLUPO_TS_CLEAR        0x90   // R  D16
#define VLUPO_TS_RESETTS      0x92   // R  D16
#define VLUPO_TS_CLEARCOUNTER 0x94   // R  D16
#define VLUPO_TS_INTDISABLE   0x80   //  W D16
#define VLUPO_TS_INTENABLE    0xa0   //  W D16
#define VLUPO_TS_LEVEL        0x00   //  W D16
#define VLUPO_TS_PULSE        0x10   //  W D16
#define VLUPO_TS_CLKMODE      0X60   //  W D16
