/* cmdvme for CAENLIB
   
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "libbbcaenvme.h"

#define TIMEOUT 0x20000
#define OPADDR   0x102e
#define HSADDR   0x1030
#define OPWOK    0x01
#define OPROK    0x02

enum mode {WR, LR, WW, LW, OR, OW, ON, QR, QW, QN, AM, BD, NONE};
typedef struct {
  enum mode md;
  const char mdchar[];
}stchkmode;

stchkmode chkmode[] = {
  {WR, "-wr"},
  {LR, "-lr"},
  {WW, "-ww"},
  {LW, "-lw"},
  {OR, "-or"},
  {OW, "-ow"},
  {ON, "-on"},
  {QR, "-qr"},
  {QW, "-qw"},
  {QN, "-qn"},
  {AM, "-am"},
  {BD, "-bd"},
  {NONE, "  "}
};

enum board {V1718, V2718, NOBOARD};
char *boardchar[] = {"V1718", "V2718"};

enum mode getmode(char *opt){
  int i=0;
  enum mode ret = NONE;
  while(chkmode[i].md != ret){
    if(!strcmp(opt, chkmode[i].mdchar)){
      return chkmode[i].md;
    }
    i++;
  }
  
  return ret;
}

void help(void){
    printf("cmdvme -[wr/lr/ww/lw/or/ow/on/qr/qw/qn] [-am AM] [-bd V1718/V2718] addr [val]\n");
}

int main(int argc, char *argv[]){
  unsigned int addr,lval,i,j;
  unsigned int am = 0x09;
  unsigned short sval,wval,ro=0,wo=0;
  enum mode md = NONE, rmd = NONE;
  enum board bd = V2718;
  char caddr[64]={0}, cval[64]={0}, cwval[64]={0};

  if(argc < 3){
    help();
    exit(0);
  }

  for(i=1;i<argc;i++){
    rmd = getmode(argv[i]);
    switch(rmd){
    case NONE:
      if(strlen(caddr) == 0){
	strcpy(caddr, argv[i]);
      }else if(strlen(cval) == 0){
	strcpy(cval, argv[i]);
      }else{
	strcpy(cwval, argv[i]);
      }
      break;
    case AM:
      am = strtol(argv[i+1],NULL,0);
      i++;
      break;
    case BD:
      if(!strcmp(boardchar[0], argv[i+1])){
	bd = V1718;
      }else if(!strcmp(boardchar[1], argv[i+1])){
	bd = V2718;
      }else{
	bd = NOBOARD;
      }
      break;
    default:
      md = rmd;
      break;
    }
  }

  if(bd == NOBOARD){
    printf("Invalid board, should be V1728 or V2718\n");
    exit(0);
  }

  if(md == NONE){
    printf("Invalid mode");
    help();
    exit(0);
  }

  if(bd == V1718){
    init_caen_v1718();
  }else{
    init_caen_v2718();
  }

  addr = strtol(caddr, NULL, 0);
  lval = strtol(cval, NULL, 0);
  sval = lval & 0x0000ffff;
  wval = strtol(cwval, NULL, 0);

  vme_amsr(am);

  switch(md){
  case WR:
    sval = vme_read16(addr);
    printf("%04x / %u\n",sval, sval);
    break;
  case LR:
    lval = vme_read32(addr);
    printf("%08x / %u\n",lval, lval);
    break;
  case WW:
    vme_write16(addr, sval);
    break;
  case LW:
    vme_write32(addr,lval);
    break;
  case QR:
    if(wval == 0 || wval > 8){
      wval = 1;
    }
    i=0;
    while(wo !=OPWOK && i<TIMEOUT){
      wo = vme_read16(addr+HSADDR);
      i++;
    }
    if(i<TIMEOUT){
      usleep(10);
      vme_write16(addr+OPADDR,sval);
      for(j=0;j<wval;j++){
	i=0;
	while(ro !=OPROK && i<TIMEOUT){
	  ro = vme_read16(addr+HSADDR);
	  i++;
	}
	if(i<TIMEOUT){
	  usleep(10);
	  sval = vme_read16(addr+OPADDR);
	  printf("%x\n",sval);
	}else{
	  printf("Time out in read ok.\n");
	}
      }
    }else{
      printf("Time out in write ok.\n");
    }
    break;
  case QW:
    i=0;
    while(wo !=OPWOK && i<TIMEOUT){
      wo = vme_read16(addr+HSADDR);
      i++;
    }
    if(i<TIMEOUT){
      usleep(10);
      vme_write16(addr+OPADDR,sval);
      i=0;
      while(wo !=OPWOK && i<TIMEOUT){
	wo = vme_read16(addr+HSADDR);
	i++;
      }
      if(i<TIMEOUT){
	usleep(10);
	vme_write16(addr+OPADDR,wval);
	printf("%x\n",sval);
      }else{
	printf("Time out in write ok.\n");
      }
    }else{
      printf("Time out in write ok.\n");
    }
    break;
  case QN:
    i=0;
    while(wo !=OPWOK && i<TIMEOUT){
      wo = vme_read16(addr+HSADDR);
      i++;
    }
    if(i<TIMEOUT){
      usleep(10);
      vme_write16(addr+OPADDR,sval);
    }else{
      printf("Time out in write ok.\n");
    }
    break;
  case OR:
    if(wval == 0 || wval > 8){
      wval = 1;
    }
    i=0;
    while(wo !=0x02 && i<TIMEOUT){
      wo = vme_read16(addr+0x50);
      i++;
    }
    if(i<TIMEOUT){
      usleep(10);
      vme_write16(addr+0x52,sval);
      for(j=0;j<wval;j++){
	i=0;
	while(ro !=0x01 && i<TIMEOUT){
	  ro = vme_read16(addr+0x50);
	  i++;
	}
	if(i<TIMEOUT){
	  usleep(10);
	  sval = vme_read16(addr+0x52);
	  printf("%x\n",sval);
	}else{
	  printf("Time out in read ok.\n");
	}
      }
    }else{
      printf("Time out in write ok.\n");
    }
    break;
  case OW:
    i=0;
    while(wo !=0x02 && i<TIMEOUT){
      wo = vme_read16(addr+0x50);
      i++;
    }
    if(i<TIMEOUT){
      usleep(10);
      vme_write16(addr+0x52,sval);
      i=0;
      while(wo !=0x02 && i<TIMEOUT){
	wo = vme_read16(addr+0x50);
	i++;
      }
      if(i<TIMEOUT){
	usleep(10);
	vme_write16(addr+0x52,wval);
	printf("%x\n",sval);
      }else{
	printf("Time out in write ok.\n");
      }
    }else{
      printf("Time out in write ok.\n");
    }
    break;
  case ON:
    addr = strtol(argv[2],NULL,0);
    sval = strtol(argv[3],NULL,0);
    i=0;
    while(wo !=0x02 && i<TIMEOUT){
      wo = vme_read16(addr+0x50);
      i++;
    }
    if(i<TIMEOUT){
      usleep(10);
      vme_write16(addr+0x52,sval);
    }else{
      printf("Time out in write ok.\n");
    }
    break;
  default:
    help();
    break;
  }
  release_caen();


  return 0;
}
