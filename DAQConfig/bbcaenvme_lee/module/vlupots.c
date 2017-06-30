/* VLUPO Timestamp for libbabies + CAENLIB */

#include "libbabies.h"
#include "libbbcaenvme.h"
#include "vlupots.h"

int vlupots_segts(unsigned int maddr){
  unsigned int tsl, tsh;
  unsigned long long int ts;

  vread32(maddr+VLUPO_TS_DATA32L, (int *)&tsl);
  vread32(maddr+VLUPO_TS_DATA32H, (int *)&tsh);
  ts = tsh;
  ts = tsl | (ts << 32);

  babies_segdata((char *)&ts, 6);
  babies_copyts(ts);
  
  return 1;
}

int vlupots_segtsfly(unsigned int maddr){
  vread32(maddr+VLUPO_TS_DATAFLY, (int *)(babies_segpt32()));

  return 1;
}

void vlupots_intdelay_map(unsigned int maddr, short val){

  vwrite16(maddr+VLUPO_TS_INTDELAY, &val);
}

void vlupots_pulse_map(unsigned int maddr, short val){

  vwrite16(maddr+VLUPO_TS_PULSE, &val);
}

void vlupots_pulsewidth_map(unsigned int maddr, short val){

  vwrite16(maddr+VLUPO_TS_PULSEWIDTH, &val);
}

void vlupots_clear(unsigned int maddr){
  short val = 1;

  vread16(maddr+VLUPO_TS_CLEAR, &val);
}

void vlupots_clear_counter(unsigned int maddr){
  short val = 1;

  vread16(maddr+VLUPO_TS_CLEARCOUNTER, &val);
}

int vlupots_segfifocnt(unsigned int maddr){
  int lval;
   
  vread32(maddr+VLUPO_TS_FIFOCNT, &lval);
  babies_segdata((char *)&lval, 2);

  return 1;
}

int vlupots_segtrgcnt(unsigned int maddr){
   
  vread32(maddr+VLUPO_TS_TRGCOUNTER, (int *)(babies_segpt32()));

  return 1;
}

void vlupots_clkmode_map(unsigned int maddr, short val){

  vwrite16(maddr+VLUPO_TS_CLKMODE, &val);
}


void vlupots_enable_interrupt(unsigned int maddr){
  short val = 1;

  vwrite16(maddr+VLUPO_TS_INTENABLE, &val);

}

void vlupots_disable_interrupt(unsigned int maddr){
  short val = 1;

  vwrite16(maddr+VLUPO_TS_INTDISABLE, &val);

}
