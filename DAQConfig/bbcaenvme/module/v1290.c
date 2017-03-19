/* V1290 for libbabies + CAENLIB */

#include "libbabies.h"
#include "libbbcaenvme.h"
#include "v1290.h"

/* Segment data */
int v1290_segdata(unsigned int maddr){
  int wordcnt;

  wordcnt = 0;
  /* Global Header */
  vread32(maddr + V1290_OUTBUFF, (int *)(babies_segpt32()));
  wordcnt ++;

  if((*((int *)(babies_pt()-4)) & V1290_TYPE_MASK) == V1290_GLOBAL_HEADER_BIT){
    while(wordcnt < 256){
      /* TDC Header */
      vread32(maddr + V1290_OUTBUFF, (int *)(babies_segpt32()));
      wordcnt ++;
      if((*((int *)(babies_pt()-4)) & V1290_TYPE_MASK) == V1290_TDC_HEADER_BIT){
	while(wordcnt < 256){
	  vread32(maddr + V1290_OUTBUFF, (int *)(babies_segpt32()));
	  wordcnt ++;
	  if((*((int *)(babies_pt()-4)) & V1290_TYPE_MASK) != V1290_TDC_DATA_BIT){
	    /* TDC Trailer or TDC Error */
	    break;
	  }
	}
      }else{
	break;
	/* Global Trailer */
      }
    }
  }

  return wordcnt;
}

// cnt = word count in 32bit data
int v1290_dmasegdata(unsigned int maddr, int cnt){
  int sz, rsz;

  sz = cnt *4;

  rsz = dma_vread32(maddr, babies_pt(), sz);
  babies_segptfx(rsz);

  return rsz/2;
}



/* Software clear */
void v1290_clear(unsigned int maddr){
  short sval;

  sval = V1290_SOFT_CLEAR_BIT;
  vwrite16(maddr + V1290_SOFT_CLEAR, &sval);
}

/* Set Almost full register */
void v1290_tdcfull(unsigned int maddr, short fl){

  vwrite16(maddr + V1290_ALMOST_FULL, &fl);
}

void v1290_intlevel(unsigned int maddr, short level){
  
  vwrite16(maddr + V1290_INTLEVEL, &level);
}
