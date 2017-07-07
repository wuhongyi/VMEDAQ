/* V792 for libbabies + CAENLIB */

#include "libbabies.h"
#include "libbbcaenvme.h"
#include "v792.h"

int v792_segdata(unsigned int maddr){
  int wordcnt;

  wordcnt = 0;
  vread32(maddr+V792_OUTBUFF,(int *)(babies_segpt32()));
  wordcnt++;

  if((*((int *)(babies_pt()-4)) & V792_TYPE_MASK) == V792_HEADER_BIT){
    while(wordcnt < 34){
      vread32(maddr+V792_OUTBUFF,(int *)(babies_segpt32()));
      wordcnt++;
      if((*(int *)(babies_pt()-4) & (V792_TYPE_MASK)) != V792_DATA_BIT){
        break;
      }
    }
  }

  return 1;
}

int v792_segdata2(unsigned int maddr){
  int wordcnt;
  int tdata;
  
  wordcnt = 0;
  vread32(maddr+V792_OUTBUFF,&tdata);

  if((tdata & V792_TYPE_MASK) != V792_HEADER_BIT){
    return 0;
  }
  
  babies_segdata((char *)&tdata, sizeof(tdata));
  wordcnt++;

  while(wordcnt < 34){
    vread32(maddr+V792_OUTBUFF,(int *)(babies_segpt32()));
    wordcnt++;
    if((*(int *)(babies_pt()-4) & (V792_TYPE_MASK)) != V792_DATA_BIT){
      break;
    }
  }

  return 1;
}


void v792_clear(unsigned int maddr){
  short sval;
 
  sval = 0x04;
 
  vwrite16(maddr + V792_BIT_SET2, &sval);
  vwrite16(maddr + V792_BIT_CLE2, &sval);
 
}

void v792_intlevelmulti(unsigned int maddr, short level, short evtn){
  vwrite16(maddr + V792_EVT_TRIG_REG, &evtn);
  vwrite16(maddr + V792_INT_REG1, &level);
}

void v792_intlevel(unsigned int maddr, short level){
  v792_intlevelmulti(maddr, level, 1);
}

// cnt = word count in 32bit data
int v792_dmasegdata(unsigned int maddr, int cnt){
  int sz, rsz;

  sz = cnt *4;

  rsz = dma_vread32(maddr, babies_pt(), sz);
  babies_segptfx(rsz);

  return rsz/2;
}

void v792_multievtberr(unsigned int maddr){
  short sval = 0x20;

  vwrite16(maddr + V792_CTRL_REG1, &sval);
}

void v792_singleevtberr(unsigned int maddr){
  short sval = 0x24;

  vwrite16(maddr + V792_CTRL_REG1, &sval);
}

void v792_noberr(unsigned int maddr){
  short sval = 0x00;

  vwrite16(maddr + V792_CTRL_REG1, &sval);
}
