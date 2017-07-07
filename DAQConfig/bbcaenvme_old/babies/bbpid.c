/* last modified : 10/12/16 15:24:38 
 *
 * bbpid.c
 * library for process management
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

/** MakePID file into PID Directory
 *  @param name Process name
 *  @return PID, (0 = Fail)
 */
int mkpid(char *name){
  char path[80];
  struct stat st;
  int pid;
  FILE *fd;

  snprintf(path,  sizeof(path), "%s", name);

  if(!stat(path, &st)){
    printf("bi-pid: PID file is exist %s\n", path);
    return 0;
  }

  pid = (int)getpid();

  if((fd = fopen(path, "w")) == NULL){
    printf("bi-pid: Can't open PID file %s\n", path);
    return 0;
  }
  
  fprintf(fd, "%d\n", pid);
  fclose(fd);

  return pid;
}


/** Remove PID file
 *  @param name Process name
 *  @return OK = 1, Fail = 0
 */
int rmpid(char *name){
  char path[80];

  snprintf(path,  sizeof(path), "%s", name);
  if(unlink(path)){
    return 0;
  }

  return 1;
}


/** Check PID of process
 *  @param name
 *  @return PID (0 = fail)
 */
int chkpid(char *name){
  char path[128];
  int pid;
  FILE *fd;

  snprintf(path,  sizeof(path), "%s", name);
  if((fd = fopen(path, "r")) == NULL){
    return 0;
  }
  fscanf(fd, "%d", &pid);
  fclose(fd);

  return pid;
}

  
/** Kill process
 *  @param name Process name
 *  @return OK = 1, Fail = 0
 */
int killpid(char *name){
  int pid;


  if(!(pid = chkpid(name))){
    return 0;
  }
  if(kill(pid, SIGINT)){
    perror("bbpid: Can't kill.\n");
    return 0;
  }

  return 1;
}


/** pidof
 *  @param name Process name
 *  @param pids pointer for pids
 *  @return the number of pids
 */
int pidof(char *name, int *pids){
  char buff[1024], pexe[256], *tp;
  FILE *pfd;
  int n;

  n = 0;
  sprintf(pexe, "/sbin/pidof %s", name);
  pfd = popen(pexe, "r");
  if(!pfd) return 0;
  fgets(buff, sizeof(buff), pfd);
  pclose(pfd);

  for(tp = strtok(buff, " "); tp; tp=strtok(NULL, " ")){
    pids[n] = strtol(tp, NULL, 0);
    printf("-- %s --\n", tp);
    printf("pid[%d] %d\n", n, pids[n]);
    if(pids[n] > 1){
      n++;
    }
  }

  return n;
}

/** killpidof
 *  send SIGINT by pidof
 *  @param name Process name
 *  @return 1 = OK, 0 = NG
 */
int killpidof(char *name){
  int pids[32], n, i;
  
  n = pidof(name, pids);
  if(!n) return 0;

  for(i=0;i<n;i++){
    kill(pids[i], SIGINT);
  }

  return 1;
}

/** chkpidof
 *  @param name Process name
 *  @return the number of pids
 */
int chkpidof(char *name){
  int n, pids[32];
  n = pidof(name, pids);

  return n;
}


int dirtoname(char *path, char *name){
  char tpath[256], *tp;
  
  if(!path) return 0;
  strncpy(tpath, path, sizeof(tpath));

  for(tp = strtok(tpath, "/"); tp; tp=strtok(NULL, "/")){
    strcpy(name, tp);
  }

  return 1;
}
