/* $Id: TArtSharedMemoryDataSource.cc 4 2009-09-02 15:59:29Z sweetbluemoon $ */
/**
 * @file   TArtSharedMemoryDataSource.cc
 * @date   Created : Mar 29, 2008 15:29:01 JST
 *   Last Modified : Dec 04, 2008 22:29:52 JST
 * @author RIPS Taro <rips@entrance.ripsnet>
 *  
 *  
 *    Copyright (C)2008
 */
#include "TArtSharedMemoryDataSource.hh"
#include "TArtCore.hh"
#include <stdio.h>
#include <unistd.h>
#include <iostream>

TArtSharedMemoryDataSource::TArtSharedMemoryDataSource()
: fSharedMemory(0), fSemaphore(0), fNBlock(0), fMemoryPos(0) {
}

TArtSharedMemoryDataSource::~TArtSharedMemoryDataSource()
{
  Close();
}

bool TArtSharedMemoryDataSource::SMOpen(const int sid) {

  Close();
  fRunNumber = 0;

  if(6 == sid)
    fSharedMemory = new TArtSharedMemory(ANSHMKEY+sid*2,(SHM_BIGBUFF_SIZE+4));
  else
    fSharedMemory = new TArtSharedMemory(ANSHMKEY+sid*2,(SHM_BUFF_SIZE+4));
  if (!fSharedMemory->IsValid()) {
    Close();
    fFileStatus = kNOTEXIST;
    return false;
  }
  fSemaphore = new TArtSemaphore(ANSEMKEY+sid*2);
  if (!fSemaphore->IsValid()) {
    Close();
    fFileStatus = kNOTEXIST;
    return false;
  }
  fMemoryPos = 0;
  fNBlock = 0;
  fFileStatus = kEXIST;
  fDataSourceType = kSM;
  return true;
}

void TArtSharedMemoryDataSource::Close() {
  if (fSharedMemory) {
    delete fSharedMemory;
    fSharedMemory = 0;
  }
  if (fSemaphore) {
    delete fSemaphore;
    fSemaphore = 0;
  }
}

int TArtSharedMemoryDataSource::Seek(long offset, int origin) 
{
   switch (origin) {
   case SEEK_SET:
      fMemoryPos = offset;
      break;
   case SEEK_CUR:
      fMemoryPos += offset;
      break;
   case SEEK_END:
      fMemoryPos = fSharedMemory->GetSize() - offset;
      break;
   default:
      return -1;
   }
   return 0;
}
int TArtSharedMemoryDataSource::Read(char *buf, const int& size, const int& offset) 
{
  if (offset) fMemoryPos = offset;
    int len = fSharedMemory->Read(buf,size,fMemoryPos);
   fMemoryPos += size;
   return len;
}

int TArtSharedMemoryDataSource::IsPrepared()
{
  unsigned int nblock;
  if (!fSemaphore) return 0;
  usleep(2);
  fSemaphore->Lock();
  fMemoryPos = 0;
  //  Read((char*)&nblock,sizeof(int),SHM_BUFF_SIZE); // changed to bottom line 2013/03/04 T.I.
  Read((char*)&nblock,sizeof(int),4*4); // Check block number
  if (nblock == fNBlock) {
    TArtCore::Debug(__FILE__,"same nblock: %d", nblock);
    fSemaphore->Unlock();
    return 0;
  }
  fNBlock = nblock;
  fMemoryPos = 0;
  return 1;
}

int TArtSharedMemoryDataSource::Wait() {
  fSemaphore->Unlock();
}

/*
#include <iostream>
using namespace std;
ArtBlockStat_t TArtSharedMemoryDataSource::GetNextBlock() {
   unsigned int nblock;

   if (!fSemaphore) return kERRORBLOCK;
   
   usleep(1);
   fSemaphore->Lock();
   fSharedMemory->Read((char*)&nblock,sizeof(int),SHM_BUFF_SIZE);
   if (nblock==fNBlock) {
      fBlockStatus = kWAIT;
   } else {
//      cout << "<TArtSharedMemoryDataSource::GetNextBlock()> fNBlock=" 
//           << fNBlock << endl;
      fNBlock = nblock;
      //ArtRidfHeader_t header;
      //Read((char*)&header,sizeof(header));
      fMemoryPos = 0;

//      if (header.BIT.fAddress != 0) {
//         fFormat = TArtRIDF::Instance();
//      } else {
//         fFormat = TArtRDF::Instance();
//      }
//      if (fFormat) fBlockStatus = fFormat->GetNextBlock(this);

      fMemoryPos = 0;
   }
   fSemaphore->Unlock();
   return fBlockStatus;
}
*/
