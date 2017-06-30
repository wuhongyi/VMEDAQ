/*
 * libbabies
 * H.B. RIKEN
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "libbabies.h"

#ifdef DEBUG
#define DB(x) x
#else
#define DB(X)
#endif

#ifdef HUGEBUFF
#define EB_EFBLOCK_SIZE 0x800000   ///< Usual max size of block data = 8MB
#else
#ifdef LARGEBUFF
#define EB_EFBLOCK_SIZE 0x200000   ///< Usual max size of block data = 2MB 
#else
#define EB_EFBLOCK_SIZE 0x20000   ///< Usual max size of block data = 256kB
#endif 
#endif
#define EB_EFBLOCK_BUFFSIZE EB_EFBLOCK_SIZE * 2
#define BABIRL_COM_SIZE    1024*10 ///< Buffer size for babirl tcp commands 

/** babies tcp commands */
#define ES_SET_CONFIG    1
#define ES_GET_CONFIG    2
#define ES_RUN_START     3
#define ES_RUN_NSSTA     4
#define ES_RUN_STOP      5
#define ES_RELOAD_DRV    6
#define ES_GET_EVTN      8    // Get last event number
#define ES_CON_EFR      11    // Connect EFS to EFR
#define ES_DIS_EFR      12    // Disconnect EFS to EFR
#define ES_QUIT         99
/** babies tcp arguments */
#define ES_EF_ON    0x00000000    // Join event build
#define ES_EF_OFF   0x80000000    // Not join event build
#define WHOAREYOU        98
/* Macros */
#define RIDF_MKHD1(a,b,c) ((a&0x3)<<28|(b&0x3f)<<22|(c&0xffffff))
#define RIBF_MKHD2(d)      (d & 0xffffffff)
#define RIDF_LY(x) ((x & 0x30000000) >> 28) // Layer
#define RIDF_CI(x) ((x & 0x0fc00000) >> 22) // Class ID
#define RIDF_SZ(x)  (x & 0x003fffff)        // Block Size
#define RIDF_EF(x)  (x & 0xffffffff)        // Event Fragment Number


/* Bits */
#define RIDF_LY0               0
#define RIDF_LY1               1
#define RIDF_LY2               2
#define RIDF_LY3               3


/* Class ID */
#define RIDF_EF_BLOCK          0
#define RIDF_EA_BLOCK          1
#define RIDF_EAEF_BLOCK        2
#define RIDF_EAF_BLOCK         2
#define RIDF_EVENT             3
#define RIDF_SEGMENT           4
#define RIDF_COMMENT           5
#define RIDF_EVENT_TS          6
#define RIDF_BLOCK_NUMBER      8
#define RIDF_END_BLOCK         9
#define RIDF_SCALER            11
#define RIDF_NCSCALER          11
#define RIDF_CSCALER           12
#define RIDF_NCSCALER32        13
#define RIDF_TIMESTAMP         16
#define RIDF_STATUS            21

/* Comment ID */
#define RIDF_COMMENT_TEXT        0
#define RIDF_COMMENT_RUNINFO_ASC 1
#define RIDF_COMMENT_EFINFO_ASC  2
#define RIDF_COMMENT_DAQINFO_BIN 3
#define RIDF_COMMENT_RUNINFO_BIN 4

/* Comment ID */
#define RIDF_STATUS_START_XML    11
#define RIDF_STATUS_STOP_XML     12

#define SIZEOFINT 4
#define SIZEOFTS  8

#define RIDF_HD_SZ             4
#define RIDF_HDEVT_SZ          6
#define RIDF_HDSEG_SZ          6
#define RIDF_HDSCR_SZ          8
#define RIDF_HDEVTTS_SZ        10

// definitions
struct stefrc{
  int efid;          ///< EFID
  char runname[80];  ///< Run name (RIDF file name)
  int runnumber;     ///< Run number
  short erport;      ///< ER port
  short comport;     ///< ES communication port
  char erhost[80];   ///< ER hostname
  int hd1;           ///< Flag for HDD1
  char hd1dir[80];   ///< Path of HDD1
  int hd2;           ///< Flag for HDD2
  char hd2dir[80];   ///< Path of HDD2
  int mt;            ///< Flag for MT
  char mtdir[80];    ///< Path of MT
  int connect;       ///< Connectivity of ER (0=disconnected., 1=connected)
};


/* static globals */
static char name[80] = "libbabies\0";
static babies_func start   = NULL;
static babies_func nssta   = NULL;
static babies_func stop    = NULL;
static babies_func quit    = NULL;
static babies_func reload  = NULL;
static babies_func evtloop = NULL;
static struct stefrc efrc;
static unsigned short *data, *datapt[2];
static int comfd = 0, fstart, efsock = 0, finit = 0;
volatile static int sflag, flast = 0;
static pthread_t evtthread;
static int fevtthread = 0;

static volatile int eventhmp,eventsize,segmenthmp,segmentsize;
static volatile int mp = 0;
static unsigned int evtn = 0;
static volatile int scrhmp, scrsize;
static volatile unsigned long long int timestamp; 

static volatile int idx = 0, dbuff = 0;

static int pfd[2] = {0}, fseg = 0, fevt = 0, fscr = 0;

static pthread_mutex_t dbuffmutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t waitmutex = PTHREAD_MUTEX_INITIALIZER;

/* prototype */
int mktcpsock(unsigned short port);
int mktcpsend(char *host, unsigned short port);
int efr_connect(void);
int efr_disconnect(void);
int read_efrc(void);
int update_efrc(void);
void babie_init_block(void);


void babies_flush(void){
  char w;
  volatile int d;
  DB(printf("libbabies : babies_flush\n"));

  babies_end_block();

  w = idx;

  if(pfd[1]){
    pthread_mutex_lock(&dbuffmutex);
    dbuff += 1;
    pthread_mutex_unlock(&dbuffmutex);
    write(pfd[1], &w, 1);
    if(idx == 0){
      idx = 1;
    }else{
      idx = 0;
    }
  }

  pthread_mutex_lock(&dbuffmutex);
  d = dbuff;
  pthread_mutex_unlock(&dbuffmutex);
  while(d > 1){
    DB(printf("libbabies: **** double buffer full ****\n"));
    DB(sleep(1));
    pthread_mutex_lock(&dbuffmutex);
    d = dbuff;
    pthread_mutex_unlock(&dbuffmutex);
    usleep(1);
  }

  mp = 0;
}

void datasend(char id){
  int x, len;

  x = (int)id;

  if(!efrc.connect){
    efr_connect();
  }
  if(efrc.connect){
    memcpy((char *)&len, (char *)datapt[x], sizeof(len));
    len = RIDF_SZ(len) * 2;  //get buff length from data
    DB(printf("libbabies: send data[%d] to EFR size=%d\n", x, len));
    send(efsock, (char *)&len, sizeof(len), 0); //send the length of this block
    send(efsock, (char *)datapt[x], len, 0);  //send the data contents
  }

  pthread_mutex_lock(&dbuffmutex);
  dbuff -= 1;
  if(dbuff < 0){
    dbuff = 0;
  }
  pthread_mutex_unlock(&dbuffmutex);

}

int babies_status(void){
  int ret;

  pthread_mutex_lock(&waitmutex);
  ret = sflag;
  pthread_mutex_unlock(&waitmutex);

  if(ret == STAT_RUN_IDLE || ret == STAT_RUN_WAITSTOP){
    usleep(1);
  }
  return ret;
}

void checkfunc(char *n, void *f){
  printf("%-10s : ", n);
  if(f){
    printf("Yes");
  }else{
    printf("No");
  }
  printf("\n");
}


void babies_check(void){
  if(efrc.efid){
    printf("%-10s : %d\n", "EFN", efrc.efid);
  }
  checkfunc("start", start);
  checkfunc("nssta", nssta);
  checkfunc("stop",  stop);
  checkfunc("reload",  reload);
  checkfunc("evtloop",  evtloop);
  printf("%-10s : %s\n", "name", name);
}

void babies_name(char *tname){
  strncpy(name, tname, sizeof(name));
}

void babies_evtloop(babies_func tevtloop){
  evtloop = tevtloop;
  pthread_create(&evtthread, NULL, (void *)evtloop, NULL);
  fevtthread = 1;
}

void wrap_quit(void){
  pthread_mutex_lock(&waitmutex);
  sflag = -1;
  pthread_mutex_unlock(&waitmutex);


  if(fevtthread) pthread_join(evtthread, NULL);

  if(comfd) close(comfd);
  if(efsock) close(efsock);
  if(data) free(data);
  if(pfd[0]) close(pfd[0]);
  if(pfd[1]) close(pfd[1]);

  DB(printf("libbabies : going to quit\n"));
  if(quit){
    quit();
  }
  exit(0);
}

void babies_quit(babies_func tquit){
  quit = tquit;
  signal(SIGINT, (void *)wrap_quit);
}

void babies_start(babies_func tstart){
  start = tstart;
}

void babies_nssta(babies_func tnssta){
  nssta = tnssta;
}

void babies_stop(babies_func tstop){
  stop = tstop;
}

void babies_reload(babies_func treload){
  reload = treload;
}

int babies_init(int tefn){
  int flag;

  if(tefn < 1 || tefn > 255){
    printf("0 < EFN < 255\n");
    return 0;
  }

  // efrc
  memset((char *)&efrc, 0, sizeof(efrc));
  efrc.efid = tefn;
  efrc.runnumber = -1;

  efrc.erport = ERRCVPORT;
  efrc.comport = ESCOMPORT + efrc.efid;
  memset(efrc.erhost, 0, sizeof(efrc.erhost));
  strcpy(efrc.erhost, "localhost\0");
  efrc.hd1 = 0;
  strcpy(efrc.hd1dir, "./\0");
  efrc.hd2 = 0;
  strcpy(efrc.hd2dir, "./\0");
  efrc.mt = 0;
  strcpy(efrc.mtdir, "./\0");
  strcpy(efrc.runname, "\0");
  efrc.connect = 0;

  read_efrc();

  data = malloc((size_t)EB_EFBLOCK_BUFFSIZE);
  if(data == NULL){
    printf("Cannot malloc data buffer size=%d\n", EB_EFBLOCK_BUFFSIZE);
    return 0;
  }
  datapt[0] = data;
  datapt[1] = data + EB_EFBLOCK_SIZE/2;

  memset((char *)data, 0, EB_EFBLOCK_BUFFSIZE);
  
  /* Make command port */
  if(!(comfd = mktcpsock(ESCOMPORT + efrc.efid))){
    printf("Cannot make tcp socket\n");
    return 0;
  }

  pthread_mutex_init(&dbuffmutex, NULL);
  pthread_mutex_init(&waitmutex, NULL);

  pthread_mutex_lock(&waitmutex);
  sflag = STAT_RUN_IDLE;
  pthread_mutex_unlock(&waitmutex);


  // self pipe
  pipe(pfd);
  flag = fcntl(pfd[0], F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(pfd[0], F_SETFL, flag);
  flag = fcntl(pfd[1], F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(pfd[1], F_SETFL, flag);

  finit = 1;

  return 1;
}


// command from DAQ server
int commain(void){
  struct sockaddr_in caddr;
  char buff[BABIRL_COM_SIZE], tdir[80]={0}, f[80]={0};
  int sock, clen, len;
  int com, ret, arg;
  
  clen = sizeof(caddr);
  if((sock = accept(comfd, (struct sockaddr *)&caddr, (socklen_t *)&clen)) < 0){
    perror("Error in accept commain\n");
    return 0;
  }
  
  memset(buff, 0, sizeof(buff));
  recv(sock, (char *)&len, sizeof(len), MSG_WAITALL);
  recv(sock, buff, len, MSG_WAITALL);
  
  /* babies command */
  memcpy((char *)&com, buff, sizeof(com));
  
  switch(com){
  case ES_SET_CONFIG:
    DB(printf("libbabies: ES_SET_CONFIG\n"));
    memset(tdir, 0, sizeof(tdir));
    strcpy(tdir, efrc.mtdir);
    
    memcpy((char *)&efrc, buff+sizeof(com), sizeof(efrc));
    memset(buff, 0, sizeof(buff));
    ret = 1;
    memcpy(buff, (char *)&ret, sizeof(ret));
    len = sizeof(ret);
    send(sock, (char *)&len, sizeof(len),0);
    send(sock, buff, len, 0);
    efr_disconnect();

    update_efrc();

    break;
  case ES_GET_CONFIG:
    DB(printf("libbabies: ES_GET_CONFIG\n"));
    memset(buff, 0, sizeof(buff));
    memcpy(buff, (char *)&efrc, sizeof(efrc));
    len = sizeof(efrc);
    send(sock, (char *)&len, sizeof(len), 0);
    send(sock, buff, len, 0);
    break;
  case ES_RELOAD_DRV:
    DB(printf("libbabies: ES_RELOAD_DRV\n"));
    // user load driver
    if(reload) reload();
    break;
  case ES_RUN_START:
    DB(printf("libbabies: ES_RUN_START\n"));
    memcpy((char *)&arg, buff+sizeof(com), sizeof(arg));
    if(arg == ES_EF_OFF && !fstart){
      DB(printf("libbabies: Run started, but not joining event build\n"));
      break;
    }
    
    // event number = 0
    evtn = 0;


    // User start function
    if(start) start();

    // when start ready, return start ack
    ret = 1;
    len = sizeof(ret);
    send(sock, (char *)&len, sizeof(len), 0);
    send(sock, (char *)&ret, len, 0);
    pthread_mutex_lock(&waitmutex);
    sflag = STAT_RUN_START;
    pthread_mutex_unlock(&waitmutex);


    break;
  case ES_RUN_NSSTA:
    // run start with no save mode
    DB(printf("libbabies: ES_NSRUN_START\n"));
    memcpy((char *)&arg, buff+sizeof(com), sizeof(arg));
    if(arg == ES_EF_OFF && !fstart){
      DB(printf("libbabies: Run started, but not joining event build\n"));
      break;
    }

    // event number = 0
    evtn = 0;

    // User start function
    if(nssta){
      nssta();
    }else{
      if(start) start();
    }

    // when start ready, return start ack
    ret = 1;
    len = sizeof(ret);
    send(sock, (char *)&len, sizeof(len), 0);
    send(sock, (char *)&ret, len, 0);

    pthread_mutex_lock(&waitmutex);
    sflag = STAT_RUN_NSSTA;
    pthread_mutex_unlock(&waitmutex);

    break;
  case ES_RUN_STOP:
    DB(printf("libbabies: ES_RUN_STOP\n"));
    // return command ack
    ret = 1;
    len = sizeof(ret);
    send(sock, (char *)&len, sizeof(len), 0);
    send(sock, (char *)&ret, len, 0);
    sflag = STAT_RUN_WAITSTOP;
    
    break;
  case ES_GET_EVTN:
    // return the event number
    ret = evtn;
    len = sizeof(ret);
    send(sock, (char *)&len, sizeof(len), 0);
    send(sock, (char *)&ret, len, 0);
    break;
  case WHOAREYOU:
    DB(printf("libbabies : WHOAREYOU\n"));
    memset(f, 0, sizeof(f));
    len = sprintf(f, name);
    send(sock, (char *)&len, sizeof(len), 0);
    send(sock, f, len, 0);
    break;
  case ES_QUIT:
    DB(printf("libbabies: ES_QUIT\n"));
    if(!sflag) wrap_quit();
    break;
  case ES_CON_EFR:
    DB(printf("libbabies: ES_CON_EFR\n"));
    if(!efr_connect()){
      DB(printf("libbabies: EFR connection faild\n"));
    }
    break;
  case ES_DIS_EFR:
    DB(printf("libbabies: ES_DIS_EFR\n"));
    if(!efr_disconnect()){
      DB(printf("libbabies: EFR disconnection faild\n"));
    }
  }

  close(sock);
  return 1;
}


void run_ending(void){
  int len;
  if(sflag == STAT_RUN_WAITSTOP){
    if(!efrc.connect){
      efr_connect();
    }
    if(efrc.connect){
      // User stop function
      if(stop) stop();
      DB(printf("libbabies: evtn=%d\n", evtn));
      
      // return stop ack (-1) to the server
      len = -1;
      send(efsock, (char *)&len, sizeof(len), 0);
    }
    pthread_mutex_lock(&waitmutex);
    sflag = STAT_RUN_IDLE;
    pthread_mutex_unlock(&waitmutex);
    DB(printf("libbabies: run ended\n"));
  }
}

void babies_main(void){
  int sn = 0;
  char pbuff;
  fd_set fdset;

  if(!finit){
    printf("ERROR : babies_init(int efn) is required before babies_main\n");
    return;
  }

  sn = comfd;
  if(pfd[0] > sn){
    sn = pfd[0];
  }

  // main loop for babies
  while(1){
//    printf("********************\n");
    FD_ZERO(&fdset);
    FD_SET(comfd, &fdset);
    FD_SET(pfd[0], &fdset);
    if(select(sn+1, &fdset, NULL, NULL, NULL) != 0){
      if(FD_ISSET(comfd, &fdset)){
	DB(printf("libbabies : goto commain\n"));
	commain();
      }else if(FD_ISSET(pfd[0], &fdset)){
	read(pfd[0], &pbuff, 1);
	DB(printf("libbabies : data send command %d\n", pbuff));
	if(pbuff == 10){
	  run_ending();
	}else{
	  datasend(pbuff);
	}
      }else{
//    printf("********************\n");
	usleep(1);
      }
    }
  }

}



// from babirl libs
int mktcpsock(unsigned short port){
  int sock = 0;
  int sockopt = 1;
  struct sockaddr_in saddr;

  memset((char *)&saddr,0,sizeof(saddr)); 
  if((sock = socket(PF_INET,SOCK_STREAM,0)) < 0){ 
    perror("bi-tcp.mktcpsock: Can't make socket.\n"); 
    return 0;
  }
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
       &sockopt, sizeof(sockopt));

  saddr.sin_family = AF_INET; 
  saddr.sin_addr.s_addr = INADDR_ANY; 
  saddr.sin_port = htons(port); 
  if(bind(sock,(struct sockaddr *)&saddr,sizeof(saddr)) < 0){ 
    perror("bi-tcp.mktcpsock: Can't bind socket.\n"); 
    return 0;
  }
  if(listen(sock,100) < 0){
    perror("bi-tcp.mktcpsock: Can't listen socket.");
    return 0;
  }

  return sock;
}

int efr_disconnect(void){
  if(efrc.connect){
    close(efsock);
    efrc.connect = 0;
  }else{
    DB(printf("libbabies: EFR not connected\n"));
    return 0;
  }
  return 1;
}

int efr_connect(void){
  int ret;
  
  ret = 0;
  if(!efrc.connect){
    /* Make data port */
    if(!(efsock = mktcpsend(efrc.erhost, efrc.erport))){
      DB(printf("libbabies: Can't connect %s:%d\n", efrc.erhost, efrc.erport));
      return 0;
    }
    send(efsock, (char *)&efrc.efid, sizeof(efrc.efid), 0);
    recv(efsock, (char *)&ret, sizeof(ret), MSG_WAITALL);
    efrc.connect = 1;
    DB(printf("libbabies: EFR return = %d\n", ret));
  }else{
    DB(printf("libbabies: EFR already connected\n"));
    return 0;
  }
  
  return 1;
}

int mktcpsend(char *host, unsigned short port){
  int sock = 0;
  struct hostent *hp;
  struct sockaddr_in saddr;

  if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0){
    perror("bi-tcp.mktcpsend: Can't make socket.\n");
    return 0;
  }

  memset((char *)&saddr,0,sizeof(saddr));

  if((hp = gethostbyname(host)) == NULL){
    printf("bi-tcp.mktcpsend : No such host (%s)\n", host);
    return 0;
  }

  memcpy(&saddr.sin_addr,hp->h_addr,hp->h_length);
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);

  //DB(printf("host : %s  port : %d\n",host,port);)

  if(connect(sock,(struct sockaddr *)&saddr,sizeof(saddr)) < 0){
    perror("bi-tcp.mktcpsend: Error in tcp connect.\n");
    close(sock);
    return 0;
  }

  return sock;
}

int update_efrc(void){
  FILE *fd;

  if((fd = fopen(BABIESRC, "w"))){
    DB(printf("libbabies: paramter write to file\n"));
    fprintf(fd, "%s\n", efrc.erhost);
    fprintf(fd, "%d\n", efrc.hd1);
    fprintf(fd, "%s\n", efrc.hd1dir);
    fprintf(fd, "%d\n", efrc.hd2);
    fprintf(fd, "%s\n", efrc.hd2dir);
    fprintf(fd, "%d\n", efrc.mt);
    fprintf(fd, "%s\n", efrc.mtdir);
    fclose(fd);
  }else{
    DB(printf("libbaies: Can't open rcfile\n"));
    return 0;
  }

  return 1;
}


int read_efrc(void){
  FILE *fd;

  if((fd = fopen(BABIESRC, "r"))){
    DB(printf("libbabies: parameter from file\n"));
    fscanf(fd, "%s", efrc.erhost);
    fscanf(fd, "%d", &efrc.hd1);
    fscanf(fd, "%s", efrc.hd1dir);
    fscanf(fd, "%d", &efrc.hd2);
    fscanf(fd, "%s", efrc.hd2dir);
    fscanf(fd, "%d", &efrc.mt);
    fscanf(fd, "%s", efrc.mtdir);
    fclose(fd);
    return 1;
  }else{
    DB(printf("libbabies: no %s file\n", BABIESRC));
    return 0;
  }
}

/** Initialize data block
 */
void babies_init_block(void){
  eventsize = 0;
  mp = RIDF_HD_SZ;

}

/** Initialize evtn
 *  This function should be call from startup.c just once.
 */
void babies_init_evtn(void){
  evtn = 0;
}

/** Get evtn
 *  @return evtn event number
 */
int babies_get_evtn(void){
  return evtn;
}

/** Initialize event block
 */
void babies_init_event(void){
  if(mp == 0){
    babies_init_block();
  }

  evtn++;                               // Count up event number
  eventhmp = mp;                        // Store mp for event header
  eventsize = RIDF_HDEVT_SZ;            // Initialize event size
  mp += RIDF_HDEVT_SZ;

  // copy evtn
  memcpy((char *)(datapt[idx]+eventhmp)+SIZEOFINT*2, (char *)&evtn, SIZEOFINT);

  fevt = 1;
}

void babies_init_eventts(void){
  if(mp == 0){
    babies_init_block();
  }

  evtn++;                               // Count up event number
  eventhmp = mp;                        // Store mp for event header
  eventsize = RIDF_HDEVTTS_SZ;            // Initialize event size
  mp += RIDF_HDEVTTS_SZ;

  // copy evtn
  memcpy((char *)(datapt[idx]+eventhmp)+SIZEOFINT*2, (char *)&evtn, SIZEOFINT);

  fevt = 2;
}
 

/** Initialize segment block
 */
void babies_init_segment(int segid){

  segmenthmp = mp;                      // Store mp for segment header
  segmentsize = RIDF_HDSEG_SZ;          // Initialize segment size
  mp += RIDF_HDSEG_SZ;


  if(fevt == 0){
    babies_init_event();
  }
  // copy segment id
  memcpy((char *)(datapt[idx]+segmenthmp)+SIZEOFINT*2, (char *)&segid, SIZEOFINT);

  fseg = 1;
}

/** End of data block */
int babies_end_block(void){
  int thd;

  if(fseg){
    babies_end_segment();
  }
  if(fevt == 1){
    babies_end_event();
  }else if(fevt == 2){
    babies_end_eventts();
  }
  if(fscr == 1){
    babies_end_scaler();
  }else if(fscr == 2){
    babies_end_ncscaler32();
  }


  thd = RIDF_MKHD1(RIDF_LY0, RIDF_EF_BLOCK, mp);  // make header 1
  memcpy((char *)datapt[idx], (char *)&thd, SIZEOFINT);  // copy header 1 to buff
  thd = efrc.efid;                                      // make header 2
  memcpy((char *)(datapt[idx])+SIZEOFINT, (char *)&thd, SIZEOFINT); // copy 
  
  return mp;
}

int babies_chk_block(int maxbuff){
  if(mp >= maxbuff){
    return mp;
  }

  return 0;
}

int babies_last_block(void){
  char w = 10;
  int tsflag;
  write(pfd[1], &w, 1);

  usleep(100);
  pthread_mutex_lock(&waitmutex);
  tsflag = sflag;
  pthread_mutex_unlock(&waitmutex);
  while(tsflag){
    pthread_mutex_lock(&waitmutex);
    tsflag = sflag;
    pthread_mutex_unlock(&waitmutex);
    usleep(100);
  }
  
  return 0;
}


/** End of event block */
int babies_end_event(void){
  int thd;

  if(fseg){
    babies_end_segment();
  }

  // make header 1
  thd = RIDF_MKHD1(RIDF_LY1, RIDF_EVENT, eventsize);
  // copy header 1
  memcpy((char *)(datapt[idx]+eventhmp), (char *)&thd, SIZEOFINT);
  thd = efrc.efid;
  // copy header 2
  memcpy((char *)(datapt[idx]+eventhmp)+SIZEOFINT, (char *)&thd, SIZEOFINT);

  eventsize = 0;
  fevt = 0;

  return mp;
}

/** End of event block with Time Stamp*/
int babies_end_eventts(void){
  int thd;

  if(fseg){
    babies_end_segment();
  }

  // make header 1
  thd = RIDF_MKHD1(RIDF_LY1, RIDF_EVENT_TS, eventsize);
  // copy header 1
  memcpy((char *)(datapt[idx]+eventhmp), (char *)&thd, SIZEOFINT);
  thd = efrc.efid;
  // copy header 2
  memcpy((char *)(datapt[idx]+eventhmp)+SIZEOFINT, (char *)&thd, SIZEOFINT);
  // copy timestamp
  memcpy((char *)(datapt[idx]+eventhmp)+SIZEOFINT*3, (char *)&timestamp, SIZEOFTS);

  eventsize = 0;
  fevt = 0;

  return mp;
}   

/** End of segment block */
int babies_end_segment(void){
  int thd;

  if(!fseg) return 0;

  // make header 1
  thd = RIDF_MKHD1(RIDF_LY2, RIDF_SEGMENT, segmentsize);
  // copy header 1
  memcpy((char *)(datapt[idx]+segmenthmp), (char *)&thd, SIZEOFINT);
  // make header 2
  thd = efrc.efid;
  // copy header 2
  memcpy((char *)(datapt[idx]+segmenthmp)+SIZEOFINT, (char *)&thd, SIZEOFINT);
  
  eventsize += segmentsize;                /* Count up event size */

  fseg = 0;

  return mp;
}


void babies_init_scaler(int scrid){
  //struct timeval t;
  time_t t;
  int tim;

  if(mp == 0){
    babies_init_block();
  }

  scrhmp = mp;                        // Store mp for event header
  scrsize = RIDF_HDSCR_SZ;            // Initialize event size
  mp += RIDF_HDSCR_SZ;

  //do_gettimeofday(&t);
  //tim = (int)t.tv_sec;
  time(&t);
  tim = t;

  // copy date
  memcpy((char *)(datapt[idx]+scrhmp)+SIZEOFINT*2, (char *)&tim, SIZEOFINT);
  // copy scrid
  memcpy((char *)(datapt[idx]+scrhmp)+SIZEOFINT*3, (char *)&scrid, SIZEOFINT);

  if(eventsize > 0) eventsize += scrsize;

  fscr = 1;
}

void babies_init_ncscaler(int scrid){
  babies_init_scaler(scrid);
  fscr = 2;
}

/** End of scaler (clear type) */
int babies_end_scaler(void){
  int thd;

  // make header 1
  if(eventsize > 0){
    // Layer = 2 (Scaler in event)
    thd = RIDF_MKHD1(RIDF_LY2, RIDF_CSCALER, scrsize);
  }else{
    // Layer = 1
    thd = RIDF_MKHD1(RIDF_LY1, RIDF_CSCALER, scrsize);
  }
  // copy header 1
  memcpy((char *)(datapt[idx]+scrhmp), (char *)&thd, SIZEOFINT);
  thd = efrc.efid;
  // copy header 2
  memcpy((char *)(datapt[idx]+scrhmp)+SIZEOFINT, (char *)&thd, SIZEOFINT);

  scrsize = 0;
  fscr = 0;

  return mp;
}

/** End of non clear scaler */
int babies_end_ncscaler(void){
  int thd;

  // make header 1
  if(eventsize > 0){
    // Layer = 2 (Scaler in event)
    thd = RIDF_MKHD1(RIDF_LY2, RIDF_SCALER, scrsize);
  }else{
    // Layer = 1
    thd = RIDF_MKHD1(RIDF_LY1, RIDF_SCALER, scrsize);
  }
  // copy header 1
  memcpy((char *)(datapt[idx]+scrhmp), (char *)&thd, SIZEOFINT);

  thd = efrc.efid;
  // copy header 2
  memcpy((char *)(datapt[idx]+scrhmp)+SIZEOFINT, (char *)&thd, SIZEOFINT);

  scrsize = 0;
  fscr = 0;

  return mp;
}

/** End of non clear scaler */
int babies_end_ncscaler32(void){
  volatile int thd;

  // make header 1
  if(eventsize > 0){
    // Layer = 2 (Scaler in event)
    thd = RIDF_MKHD1(RIDF_LY2, RIDF_NCSCALER32, scrsize);
  }else{
    // Layer = 1
    thd = RIDF_MKHD1(RIDF_LY1, RIDF_NCSCALER32, scrsize);
  }

  // copy header 1
  memcpy((char *)(datapt[idx]+scrhmp), (char *)&thd, SIZEOFINT);

  thd = efrc.efid;
  // copy header 2
  memcpy((char *)(datapt[idx]+scrhmp)+SIZEOFINT, (char *)&thd, SIZEOFINT);

  scrsize = 0;
  fscr = 0;

  return mp;
}

// size in character
int babies_segdata(char *buff, int size){
  if(fseg){
    memcpy((char *)(datapt[idx]+mp), buff, size);
    segmentsize += size/2;

    mp += size/2;
  }
  return mp;
}

int babies_scrdata(char *buff, int size){
  if(fscr){
    memcpy((char *)(datapt[idx]+mp), buff, size);
    scrsize += size/2;
    
    mp += size/2;
  }
  return mp;
}

char *babies_segpt16(void){
  char *ret = (char *)(datapt[idx]+mp);

  mp += 1;
  segmentsize += 1;

  return ret;
}

char *babies_segpt32(void){
  char *ret = (char *)(datapt[idx]+mp);

  mp += 2;
  segmentsize += 2;

  return ret;
}

char *babies_segpt48(void){
  char *ret = (char *)(datapt[idx]+mp);

  mp += 3;
  segmentsize += 3;

  return ret;
}

char *babies_segpt64(void){
  char *ret = (char *)(datapt[idx]+mp);

  mp += 4;
  segmentsize += 4;

  return ret;
}

char *babies_pt(void){
  return (char *)(datapt[idx]+mp);
}

//size in character workd
char *babies_segptfx(int sz){
  char *ret = (char *)(datapt[idx]+mp);

  mp += sz/2;
  segmentsize += sz/2;

  return ret;
}

void babies_copyts(unsigned long long int ts){
  timestamp = ts;
}
