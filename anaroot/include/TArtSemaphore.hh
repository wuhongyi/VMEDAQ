/*
 *    TArtSemaphore.hh
 *    Created       : Jan 19 2007 15:05:00
 *    Last Modified : Jan 19 2007 18:09:15
 *--------------------------------------------------------
 *    Comment : Manage and control a semaphore 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2007 by ota
 */
#ifndef TARTSEMAPHORE_H
#define TARTSEMAPHORE_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
#if 1
#else
union semun {
   int              val;
   struct semid_ds* buf;
   unsigned short*  array;
   struct seminfo*  __buf;
};
#endif

class TArtSemaphore  {

public:
   TArtSemaphore(int semkey);
   ~TArtSemaphore();
   //   int  Delete();
   void Lock();
   void Unlock();
   bool IsValid() {return fIsValid; }
private:
   int            fKey;
   int            fID;
   bool           fIsValid;
   //   union  semun   fSemUnion;
   struct sembuf  fSemaphore;
};
#endif // end of #ifdef TARTSEMAPHORE_H
