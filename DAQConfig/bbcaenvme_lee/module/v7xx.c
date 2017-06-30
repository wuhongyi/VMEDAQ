/* V7XX for libbabies + CAENLIB */

#include "libbabies.h"
#include "libbbcaenvme.h"
#include "v7xx.h"

int v7xx_segdata(unsigned int maddr){
  int wordcnt;

  wordcnt = 0;
  vread32(maddr+V7XX_OUTBUFF,(int *)(babies_segpt32()));
  wordcnt++;

  if((*((int *)(babies_pt()-4)) & V7XX_TYPE_MASK) == V7XX_HEADER_BIT){
    while(wordcnt < 34){
      vread32(maddr+V7XX_OUTBUFF,(int *)(babies_segpt32()));
      wordcnt++;
      if((*(int *)(babies_pt()-4) & (V7XX_TYPE_MASK)) != V7XX_DATA_BIT){
        break;
      }
    }
  }

  return 1;
}

int v7xx_segdata2(unsigned int maddr){
  int wordcnt;
  int tdata;
  
  wordcnt = 0;
  vread32(maddr+V7XX_OUTBUFF,&tdata);

  if((tdata & V7XX_TYPE_MASK) != V7XX_HEADER_BIT){
    return 0;
  }
  
  babies_segdata((char *)&tdata, sizeof(tdata));
  wordcnt++;

  while(wordcnt < 34){
    vread32(maddr+V7XX_OUTBUFF,(int *)(babies_segpt32()));
    wordcnt++;
    if((*(int *)(babies_pt()-4) & (V7XX_TYPE_MASK)) != V7XX_DATA_BIT){
      break;
    }
  }

  return 1;
}


void v7xx_clear(unsigned int maddr){
  short sval;
 
  sval = 0x04;
 
  vwrite16(maddr + V7XX_BIT_SET2, &sval);
  vwrite16(maddr + V7XX_BIT_CLE2, &sval);
 
}

void v7xx_mclear(unsigned int maddr){
  short sval;
 
  sval = 0x04;
 
  vmwrite16(maddr + V7XX_BIT_SET2, &sval);
  vmwrite16(maddr + V7XX_BIT_CLE2, &sval);
 
}

void v7xx_intlevelmulti(unsigned int maddr, short level, short evtn){
  vwrite16(maddr + V7XX_EVT_TRIG_REG, &evtn);
  vwrite16(maddr + V7XX_INT_REG1, &level);
}

void v7xx_intlevel(unsigned int maddr, short level){
  v7xx_intlevelmulti(maddr, level, 1);
}

// cnt = word count in 32bit data
int v7xx_dmasegdata(unsigned int maddr, int cnt){
  int sz, rsz;

  sz = cnt *4;

  rsz = dma_vread32(maddr, babies_pt(), sz);
  babies_segptfx(rsz);

  return rsz/2;
}

void v7xx_multievtberr(unsigned int maddr){
  short sval = 0x20;

  vwrite16(maddr + V7XX_CTRL_REG1, &sval);
}

void v7xx_singleevtberr(unsigned int maddr){
  short sval = 0x24;

  vwrite16(maddr + V7XX_CTRL_REG1, &sval);
}

void v7xx_noberr(unsigned int maddr){
  short sval = 0x00;

  vwrite16(maddr + V7XX_CTRL_REG1, &sval);
}

void v7xx_rst_counter(unsigned int maddr)
{
   short sval = 0x01;
   
   vwrite16(maddr + V7XX_EVT_CNT_RST, &sval);
}

void v7xx_mrst_counter(unsigned int maddr)
{
   short sval = 0x01;
   
   vmwrite16(maddr + V7XX_EVT_CNT_RST, &sval);
}
