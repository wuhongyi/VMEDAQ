/* $Id: TArtSharedMemoryDataSource.h 4 2009-09-02 15:59:29Z sweetbluemoon $ */
/**
 * @file   TArtSharedMemoryDataSource.hh
 * @date   Created : Mar 29, 2008 15:29:53 JST
 *   Last Modified : Dec 04, 2008 21:55:48 JST
 * @author RIPS Taro <rips@entrance.ripsnet>
 *  
 *  
 *    Copyright (C)2008
 */
#ifndef TARTSHAREDMEMORYDATASOURCE_H
#define TARTSHAREDMEMORYDATASOURCE_H

#include "TArtSharedMemory.hh"
#include "TArtSemaphore.hh"
#include "TArtDataSource.hh"

#define BABIRL
#ifdef BABIRL
#define SHM_BIGBUFF_SIZE 0x800000 // 8 MByte
#define SHM_BUFF_SIZE 0x80000 // 256 kByte
#define ANSHMKEY 561000
#define ANSEMKEY 561001
#else 
#define SHM_BUFF_SIZE 0x4000 // 16 kByte
#define ANSHMKEY 17001
#define ANSEMKEY 18001
#endif

class TArtSharedMemoryDataSource  : public TArtDataSource {
public:
   TArtSharedMemoryDataSource();
   ~TArtSharedMemoryDataSource();
   int GetRunNumber(){return fRunNumber;}
   bool SMOpen(const int sid = 0);
   void Close();
  //ArtBlockStat_t GetNextBlock();
   int Read(char *buf, const int &size, const int &offset = 0);
   virtual int Seek(long offset, int origin);
   virtual int IsPrepared();
   virtual int Wait();

private:

   TArtSharedMemory *fSharedMemory;
   TArtSemaphore    *fSemaphore;
   unsigned int      fNBlock;
   unsigned int      fMemoryPos;
   int fRunNumber;
};
#endif // end of #ifdef TARTSHAREDMEMORYDATASOURCE_H
