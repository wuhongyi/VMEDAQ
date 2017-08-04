/* madc32 */

#include "libbabies.h"
#include "libbbcaenvme.h"
#include "madc32.h"

void madc32_clear(unsigned int addr){
  short val;
  val = 0;
  vwrite16(addr+MADC32_READOUT_RESET, &val);
}

void madc32_mclear(unsigned int maddr){
   short val;
   val = 0;
   vmwrite16(maddr+MADC32_READOUT_RESET,&val);
}

void madc32_fifo_reset(unsigned int addr){
  short val=1;
  vwrite16(addr+MADC32_FIFO_RESET,&val);
}

void madc32_mfifo_reset(unsigned int maddr){
  short val=1;
  vmwrite16(maddr+MADC32_FIFO_RESET,&val);
}

void madc32_set_mevent(unsigned int maddr,short val){
  vwrite16(maddr+MADC32_MULTIEVENT,&val);
}

void madc32_mset_mevent(unsigned int maddr,short val){
  vmwrite16(maddr+MADC32_MULTIEVENT,&val);
}

void madc32_start_acq(unsigned int addr){
  short val;
  val = 1;
  vwrite16(addr+MADC32_START_ACQ, &val);
}

void madc32_mstart_acq(unsigned int maddr){
  short val;
  val = 1;
  vmwrite16(maddr+MADC32_START_ACQ, &val);
}

void madc32_stop_acq(unsigned int addr){
  short val;
  val = 0;
  vwrite16(addr+MADC32_START_ACQ, &val);
}

void madc32_mstop_acq(unsigned int maddr){
  short val;
  val = 0;
  vmwrite16(maddr+MADC32_START_ACQ, &val);
}

void madc32_irq_level(unsigned int addr, short val){
  vwrite16(addr+MADC32_IRQ_LEVEL, &val);
}

void madc32_mirq_level(unsigned int maddr, short val){
  vmwrite16(maddr+MADC32_IRQ_LEVEL, &val);
}

void madc32_resol_8khires(unsigned int addr){
  short val;

  val = 4;
  vwrite16(addr+MADC32_ADC_RESOLUTION, &val);
}

void madc32_input_range(unsigned int addr, short val){
  vwrite16(addr+MADC32_INPUT_RANGE, &val);
}


void madc32_module_id(unsigned int addr, short val){
  vwrite16(addr+MADC32_MODULE_ID, &val);
}

int madc32_segdata(unsigned int addr){
  volatile short cnt, i;
  int wordcnt = 0;

  vread16(addr + MADC32_BUFFER_DATA_LENGTH, (short *)&cnt);
  cnt = cnt & 0x3fff;

  for(i=0;i<cnt;i++){
    vread32(addr + MADC32_DATA, (int *)(babies_segpt32()));
    wordcnt++;
  }

  return 1;
}

int madc32_dmasegdata(unsigned int maddr, int cnt){
  int sz, rsz;
  sz = cnt*4;
  rsz = dma_vread32(maddr+MADC32_DATA,babies_pt(),sz);
  babies_segptfx(rsz);
  
  return rsz/2;
}

void madc32_reset_ctra_counters(unsigned int addr){
  short val=1;
  vwrite16(addr+MADC32_RESET_COUNTER, &val);
}

void madc32_mreset_ctra_counters(unsigned int maddr){
  short val=1;
  vmwrite16(maddr+MADC32_RESET_COUNTER, &val);
}
#ifdef UNIV
void madc32_map_clear(int n){
  short val;

  val = 1;
  univ_map_write16(MADC32_READOUT_RESET, &val, n);

}

void madc32_map_start_acq(int n){
  short val;

  val = 1;
  univ_map_write16(MADC32_START_ACQ, &val, n);
}

void madc32_map_stop_acq(int n){
  short val;

  val = 0;
  univ_map_write16(MADC32_START_ACQ, &val, n);
}

void madc32_map_irq_level(short val, int n){

  univ_map_write16(MADC32_IRQ_LEVEL, &val, n);
}

void madc32_map_resol_8khires(int n){
  short val;

  val = 4;
  univ_map_write16(MADC32_ADC_RESOLUTION, &val, n);
}

void madc32_map_input_range(short val, int n){
  univ_map_write16(MADC32_INPUT_RANGE, &val, n);
}


void madc32_map_module_id(short val, int n){
  univ_map_write16(MADC32_MODULE_ID, &val, n);
}

int madc32_map_segdata(int n){
  volatile short cnt, i;

  univ_map_read16(MADC32_BUFFER_DATA_LENGTH, (short *)&cnt, n);
  cnt = cnt & 0x3fff;

  for(i=0;i<cnt;i++){
    univ_map_read32(MADC32_DATA, (long *)(data+mp), n);
    mp += 2;
    segmentsize += 2;
  }

  return segmentsize;
}
#endif
