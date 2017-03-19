/*
 *    TArtSharedMemory.cc
 *    Created       : Jan 19 2007 14:41:56
 *    Last Modified : Mar 30, 2008 03:59:38 JST
 *--------------------------------------------------------
 *    Comment : manage shared memory and control semaphore
 *    
 *--------------------------------------------------------
 *    Copyright (C)2007 by ota
 */
#include "TArtCore.hh"
#include "TArtSharedMemory.hh"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
/** 
 * Constructor will create a shared memory
 * @param shmkey key of shared memory
 * @param size   shared memory size
 */
TArtSharedMemory::TArtSharedMemory(int shmkey, int size)
   : fID(0), fKey(shmkey), fSize(size), fIsValid(false), fSharedMemory(0)
{
   if ((fID = shmget(shmkey, size, IPC_CREAT | 0777)) == -1) {
     TArtCore::Error(__FILE__,"Cannot create shared memory errno: %d", errno);
     fIsValid = false;
     return;
   }
   fSharedMemory = (char*)shmat(fID,0,0);
   fIsValid = true;
   /* bzero(fSharedMemory,0x4000); */
}

/**
 * Default destructor will delete the shared memory if its exist
 */
TArtSharedMemory::~TArtSharedMemory()
{
   if (fSharedMemory) Delete();
}

/**
 * Read shread memory with offset and size. The data will be copied to buf.
 * @param buf output data buffer
 * @param size read size
 * @param offset data from offset will be copy
 */
#include <stdlib.h>
#include <unistd.h>
int TArtSharedMemory::Read(char *buf, int size, int offset)
{
   if (fSize < offset+size) {
      printf("Error: <TArtSharedMemory::Read> bound check required offset+size = %d for fSize(%d) \n",offset+size,fSize);
      return -1;
   }
   memcpy(buf,fSharedMemory+offset, size);
   return size;
}

/** 
 * Delete shared memory
 * @return int returns 0 if there is an error, else 1
 */
int TArtSharedMemory::Delete()
{
   shmdt(fSharedMemory);
   if (shmctl(fID, IPC_RMID, 0) < 0) return 0; // error
   fSharedMemory = 0;
   return 1;
}
