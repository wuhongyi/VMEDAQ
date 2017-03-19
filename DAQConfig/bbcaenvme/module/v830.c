#include "libbabies.h"
#include "libbbcaenvme.h"
#include "v830.h"
/* 
 * original by Masaru-san 
 * reviced by S.Ota 6.Nov.2003.
 * Edit by Yoshinaga 29.Oct.2010.
 * Modified by Z.Li for v2718. 29.May.2012.
 * Modified by Li. J./Lee for v2718. 18.Mar.2014.
 */

void v830_initialize(unsigned int maddr) {
  int sval = 0xffffffff;
  vwrite32(maddr+V830_CHANNEL_ENABLE, &sval);
}


void v830_clear_all(unsigned int maddr){
  short sval = 0x0;
  vwrite16(maddr+V830_SOFTWARE_CLEAR, &sval);
 // vwrite16(maddr+V830_BIT_CLEAR_REGISTER,&sval);
}

// nch = number of enabled channel, no header in MEB
// v830 MEB with header , nch = enabled channel numeber + 1 (for header)
int v830_segndata(unsigned int maddr, int nch){
  int i;

  for(i=0;i<nch;i++){
     vread32(maddr+V830_OUTBUFF,(int *)(babies_segpt32()));
  }

  return i;
}


// cnt = number of enabled channel, no header in MEB
// cnt = number of enabled channel + 1, header in MEB
int v830_dmasegdata(unsigned int maddr, int cnt)
{
  int sz, rsz;

  sz = cnt *4;

  rsz = dma_vread32(maddr, babies_pt(), sz);
  babies_segptfx(rsz);

  return rsz/2;
}

// enable BERR ,rsize = max number of words for 1 evt
// for v830, rsize = 35 or bigger, coz when MEB is empty, the BERR
// will stop the read function

int v830_fxdmasegdata(unsigned int maddr, int rsize){
  /* rsize : int word to be read */
  int csize, rsz;

  csize = rsize * 4;  /* long word -> char size */
  rsz = dma_vread32(maddr, babies_pt(), csize);
  babies_segptfx(rsz);


  return rsz/2;
}


void v830_setinhibit(unsigned int maddr){
   int sval = 0x0;
   
   vwrite32(maddr+V830_CHANNEL_ENABLE,&sval);

}

void v830_intlevel(unsigned int maddr,short level){
   vwrite16(maddr+V830_INTERRUPT_LEVEL,&level);
}

/*
int v830_ridf_ncscaler(unsigned int maddr){
   volatile int i;
   
   for(i=0;i<V830_NCH;i++){
      vread32(maddr+V830_OUTBUFF+i*4,(int *)(data+mp));
      mp += 2;
   }

   scrsize += 2 * V830_NCH;
   if(eventsize > 0) eventsize += 2 * V830_NCH;

   return scrsize;
}
*/
