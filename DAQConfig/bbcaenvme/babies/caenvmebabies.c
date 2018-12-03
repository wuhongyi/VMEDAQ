/* 
 * example to use libbabies + CAENVMELib
 * H.B. RIKEN
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "libbbcaenvme.h"
#include "libbabies.h"
#include "bbpid.h"
#include "segidlist.h"

#include "v7xx.h"
#include "v830.h"
#include "v1290.h"
#include "v1190.h"
#include "v977.h"
#include "madc32.h"

int efn = 0;
unsigned int scrbuff[32] = {0};
char pidpath[] = "/tmp/babies";
//to use babies
//char pidpath[] = "/var/run/babies";

const int intlevel = 1;

// address of VME board
#include "bbmodules.h"
#include "start.c"
#include "evt.c"
#include "clear.c"
#include "stop.c"


void quit(void){
  release_caen();
  rmpid(pidpath);
  printf("Exit\n");
}


void reload(void){
  printf("Reload\n");
}

void sca(void){
  int i;
  //printf("Sca\n");
  for(i=0;i<32;i++){
    scrbuff[i] = scrbuff[i] + 1;
  }
  babies_init_ncscaler(efn);
  babies_scrdata((char *)scrbuff, sizeof(scrbuff));
  babies_end_ncscaler32();
}

// thread
void evtloop(void){
  int status;

  while((status = babies_status()) != -1){
    switch(status){
    case STAT_RUN_IDLE:
      /* noop */
      usleep(1000);
      break;
    case STAT_RUN_START:
    case STAT_RUN_NSSTA:
      //      if(vme_check_interrupt()){
      //	/* continue */
      //	//printf("interrupt ok\n");
      //      }else{
      //	//printf("interrupt non\n");
      //	usleep(10);
      //	break;
      //      }
      //printf("evtloop\n");
      if(vme_wait_interrupt(1000)){
	/* wait 10s for the interrupt, time units: ms*/
      }else {
        /*printf(" no interrupt in 10 sec !\n");*/
        printf(" babies will loop again!");
        break;
      }
      
      evt();

      // babies_chk_block(int maxbuff)
      // if block size is larger than maxbuff,
      //  data should be send to the event builder
      //  i.e., read scaler and flush
      // example : 8000 = 16kB
      if(babies_chk_block(8000)){
      	//	sca();
      	babies_flush();
      }

      // clear module is here (not in clear())
      //      v1290_clear(V1190ADDR0);
      //      v7xx_clear(ADC1ADDR);

      vme_enable_interrupt();
      clear();

      break;
    case STAT_RUN_WAITSTOP:
      // for the last sequense of run
      v2718_set_ioport(3);//硬件BUSY VETO
      //      sca();
      while(vme_check_interrupt()){
	//      while(vme_wait_interrupt(10000)){
        evt();
	//  clear();
        if(babies_chk_block(8000)){
          babies_flush();
        }
      }
      babies_flush();
      babies_last_block();
      printf("STAT_RUN_WATISTOP!\n");
      break;
    default:
      break;
    }
  }

  // write codes to quit safely

}

int main(int argc, char *argv[]){

  if(argc != 2){
    printf("babies EFN\n");
    exit(0);
  }else{
    efn = strtol(argv[1], NULL, 0);
  }

  babies_quit(quit);
  babies_start(start);
  babies_stop(stop);
  babies_reload(reload);
  babies_evtloop(evtloop);
  babies_name("babies");

  babies_init(efn);
  babies_check();

  /* init vme */
  init_caen_v2718();
  vme_amsr(0x09);
  
  v2718_init_ioport(3,0,0);//硬件BUSY VETO
  v2718_set_ioport(3);//硬件BUSY VETO

  //mkpid to use babimo
  //default = /var/run/babies
  //in this example, use /tmp/babies
  mkpid(pidpath);
  //  printf("*****************\n");
  babies_main();

  return 0;
}
