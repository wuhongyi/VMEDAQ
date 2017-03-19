/*
 *    TArtEventStore.cc
 *    Created       : Jan 19 2007 02:48:33
 *    Last Modified : 2009/01/19 13:30:56 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2007 by ota
 */
#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtEventInfo.hh"
#include "TArtFileDataSource.hh"
#include "TArtSharedMemoryDataSource.hh"
#include "TArtStreamingDataSource.hh"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include "TSystem.h"

#include "TArtParserFactory.hh"
#include "TArtCore.hh"
//TArtEventStore* TArtEventStore::fgEventStore = 0;

//////////////////////////////////////////////////
//

TArtEventStore::TArtEventStore()
  : fDataSource(0), fParser(0), pInterrupt(NULL)
{
   TArtCore::Info("TArtEventStore","Constructor");
   sman = TArtStoreManager::Instance();

   rawevent = new TArtRawEventObject();
   rawevent->SetName("RawEvent");
   sman->AddDataContainer(rawevent);

   info_array = new TClonesArray("TArtEventInfo",10);
   info_array->SetName("EventInfo");
   sman->AddDataContainer(info_array);
   new ((*info_array)[0]) TArtEventInfo();
}

TArtEventStore::~TArtEventStore()
{
  //  if(fParser){ delete fParser; fParser=NULL; }
}

void TArtEventStore::ClearData() 
{
  TArtEventInfo *ei = (TArtEventInfo *)info_array->At(0);
  if(ei) ei->Clear();
  if(rawevent) rawevent->Clear();
}

bool TArtEventStore::Open()
{
  return Open(0);
}

bool TArtEventStore::Open(const int sid)
{
  fDataSource = new TArtSharedMemoryDataSource();
  TArtCore::Debug("TArtEventStore","Shared Memory Open: %d",sid);
  bool isopened = fDataSource->SMOpen(sid);
  if (!isopened) return false;

  fParser = TArtParserFactory::Create(fDataSource);
  return true;
}

bool TArtEventStore::Open(const char *filename)
{
  fDataSource = new TArtFileDataSource();
  TArtCore::Debug("TArtEventStore","File Open: %s",filename);
  bool fopened = fDataSource->FileOpen(filename);
  if (!fopened){
    TArtCore::Info("TArtEventStore","Failed to open the file : %s",filename);
    return false;
  }

  fParser = TArtParserFactory::Create(fDataSource);
  return true;
}

bool TArtEventStore::Open(const char *hostname, const int port)
{
  fDataSource = new TArtStreamingDataSource();
  TArtCore::Debug("TArtEventStore","Streaming Open: %s:%d",hostname,port);
  bool fopened = fDataSource->SocketOpen(hostname,port,false);
  if (!fopened){
    TArtCore::Info("TArtEventStore","Failed to open the socket : %s:%d",hostname,port);
    return false;
  }

  fParser = TArtParserFactory::Create(fDataSource);
  return true;
}

void TArtEventStore::SkipEvent(int neve)
{
  TArtCore::Info(__FILE__,"Skip %d events",neve);
  fParser->SetSkip(true);
  for(int i=0;i<neve;i++) GetNextEvent();
  fParser->SetSkip(false);
}

bool TArtEventStore::GetNextEvent()
{

  if(!rawevent){
    TArtCore::Info("TArtEventStore","TArtRawEventObject is not defined");
    return false;
  }
  if(fDataSource->GetFileStatus() == kNOTEXIST)
    return false;

  TArtEventInfo *ei = (TArtEventInfo *)info_array->At(0);
  ei->Clear();
  rawevent->Clear();

  while(rawevent->GetNumSeg()==0){ // pooling till segment > 0
    bool getdata = fParser->Parse(fDataSource,rawevent);
    // return false, when data can not read out.
    if(!getdata && fDataSource->GetDataSourceType() != kSM){
      TArtCore::Debug("TArtEventStore","return false, when data can not read out.");
      return false;
    }

    if(pInterrupt){
      if(true == *pInterrupt){
	TArtCore::Info("TArtEventStore","Interrupted... Stop to get data.");
	return false;
      }
    }

    if(fDataSource->GetDataSourceType() == kSM){
      if(gSystem) gSystem->Sleep(10);
    }
  }

  ei->SetRunNumber(rawevent->GetRunNumber());
  ei->SetEventNumber(rawevent->GetEventNumber());
  ei->SetTimeStamp(rawevent->GetTimeStamp());

 return true;
}
