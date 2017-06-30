#include "libbabies.h"
#include "libbbcaenvme.h"
#include "v1190.h"

//void v1190_clear(unsigned int maddr);
//int v1190_segdata(unsigned int maddr);
//
#define DMASIZE 128

/* Segment data */
int v1190_segdata(unsigned int maddr){
  int wordcnt;

  wordcnt = 0;
  /* Global Header */
  vread32(maddr + V1190_OUTBUFF, (int *)(babies_segpt32()));
  wordcnt ++;

  if((*((int *)(babies_pt()-4)) & V1190_TYPE_MASK_S) == V1190_GLOBAL_HEADER_BIT_S){
    while(wordcnt < 256){
      /* TDC Header */
      vread32(maddr + V1190_OUTBUFF, (int *)(babies_segpt32()));
      wordcnt ++;
      if((*((int *)(babies_pt()-4)) & V1190_TYPE_MASK_S) == V1190_TDC_HEADER_BIT_S){
	while(wordcnt < 256){
	  vread32(maddr + V1190_OUTBUFF, (int *)(babies_segpt32()));
	  wordcnt ++;
	  if((*((int *)(babies_pt()-4)) & V1190_TYPE_MASK_S) != V1190_TDC_DATA_BIT_S){
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

/* Software clear */
void v1190_clear(unsigned int maddr){
  short sval;

  sval = V1190_SOFT_CLEAR_BIT;
  vwrite16(maddr + V1190_SOFT_CLEAR, &sval);
}

/* Set Almost full register */
void v1190_tdcfull(unsigned int maddr, short fl){
  short sval;

  sval = fl;
  vwrite16(maddr + V1190_ALMOST_FULL, &sval);
}

void v1190_intlevel(unsigned int maddr, short level){
  
  vwrite16(maddr + V1190_INTLEVEL, &level);
}

/* DMA readout when enable EVT FIFO (0x1000 8th bit)
First readout the number of valid number of valid data from 
EVT_FIFO
*/

int v1190_fxdmasegdata(unsigned int maddr){

  int dmacnt, ret, vsize;
  vread32(maddr+V1X90_EVT_FIFO,(int *)(&dmacnt));
  dmacnt = (dmacnt & 0x0000ffff) *4;
  if(dmacnt >DMASIZE){
  dmacnt=DMASIZE;
  }

  ret = dma_vread32(maddr, babies_pt(), dmacnt);
  babies_segptfx(ret);
  vsize = 0;
  if(ret >=0 ){
   vsize = ret/2;
  }
 
 return vsize;
}

/* when enable BERR (0x1000 0th bit), DMA can works in flexable length readout
*/

int v1190_dmasegdata(unsigned int maddr, int rsize){ //rsize = maximum number of 32bit data
  int  ret, vsize, csize;
  
  csize = rsize * 4;
  ret = dma_vread32(maddr, babies_pt(), csize);
  vsize = 0;
  if(ret>=0){
    vsize = ret/2;
  }
  
  return vsize;
}
