/*
 *    TArtSemaphore.cc
 *    Created       : Jan 19 2007 15:13:45
 *    Last Modified : Mar 30, 2008 03:02:12 JST
 *--------------------------------------------------------
 *    Comment : implementation of TArtSemaphore class for
 *    managing and controlling semaphore
 *--------------------------------------------------------
 *    Copyright (C)2007 by ota
 */
#include "TArtSemaphore.hh"
#include <stdio.h>
#include <errno.h>
TArtSemaphore::TArtSemaphore(int semkey)
   : fID(0), fKey(semkey), fIsValid(false)
{
   if ((fID = semget((key_t)semkey, 1, IPC_CREAT | 0666)) == -1) {
      perror("Cannot create semaphore\n");
      //  Delete();
      fIsValid = false;
      return;
   }
   errno = 0;
   /*
     fSemUnion.val = 1;
     if (semctl(fID, 0, SETVAL, fSemUnion) == -1) {
     printf("%d %d\n",fID,fKey);
     printf("ERRNO = %d\n",errno);
     perror("Cannot control semaphore.\n");
     Delete();
     fIsValid = false;
     return;
     }
   */
   fIsValid = true;

   fSemaphore.sem_num = 0;
   fSemaphore.sem_flg = SEM_UNDO;

}

TArtSemaphore::~TArtSemaphore()
{
  //   Delete();
}

/**
 * Delete semaphore
int TArtSemaphore::Delete()
{
   if (semctl(fID,0,IPC_RMID,&fSemUnion) < 0) return 0;
   return 1;
}
 **/

/**
 * Lock semaphore
 */
void TArtSemaphore::Lock()
{
   fSemaphore.sem_op = -1;
   semop(fID,&fSemaphore,1);
}

/**
 * Unlock semaphore
 */
void TArtSemaphore::Unlock()
{
   fSemaphore.sem_op = 1;
   semop(fID,&fSemaphore,1);
}
