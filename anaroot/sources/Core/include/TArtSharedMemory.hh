/*
 *    TArtSharedMemory.hh
 *    Created       : Jan 19 2007 12:43:23
 *    Last Modified : 2008/10/17 18:36:01 JST
 *--------------------------------------------------------
 *    Comment : Shared memory manager and controling semaphore
 *    
 *--------------------------------------------------------
 *    Copyright (C)2007 by ota
 */
#ifndef TARTSHAREDMEMORY_H
#define TARTSHAREDMEMORY_H

class TArtSharedMemory {
public:
   TArtSharedMemory(int shmkey, int size);
   ~TArtSharedMemory();
   int GetSize() { return fSize; }
   int Read(char *buf, int size, int offset = 0);
   int  Delete();
   bool IsValid() { return fIsValid; }
private:
   int   fKey;    // shared memmory key
   int   fSize;   // shared memory size
   int   fID;     // shared memory id
   bool  fIsValid;// flag for validity of shared memory
   char* fSharedMemory; // pointer to shared memory
};
#endif // end of #ifdef TARTSHAREDMEMORY_H
