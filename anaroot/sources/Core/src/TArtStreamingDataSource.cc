/**
 * @file   TArtStreamingDataSource.cc
 * @author T. Isobe
 *  
 *    Copyright (C)2011
 */
#include "TSocket.h"
#include "TArtCore.hh"
#include "TArtStreamingDataSource.hh"

TArtStreamingDataSource::TArtStreamingDataSource() 
  : sock(0) 
{
  TArtCore::Info("TArtStreamingDataSource","Constructor");
}

TArtStreamingDataSource::~TArtStreamingDataSource()
{
  if(sock)
    delete sock;
}

void TArtStreamingDataSource::Close()
{
  if(sock)
    sock->Close();
}

int TArtStreamingDataSource::IsPrepared()
{
  if(sock){
    return sock->IsValid();
  }
  else{
    return false;
  }
}

bool TArtStreamingDataSource::SocketOpen(const char *hostname, const int port, const bool fromlast) {

  int command, ret;

 sock = new TSocket(hostname,port);

  if(sock->IsValid()){

    if(fromlast){
      command = 0; // recv from last event
    }
    else{
      command = 1; // recv from first event
    }
    sock->SendRaw(&command,sizeof(command));
    sock->RecvRaw(&ret,sizeof(ret));

    fDataSourceType = kStream;
    return true;
  }
  else{
    return false;
  }

}

int TArtStreamingDataSource::Seek(long offset, int origin) 
{
  TArtCore::Error(__FILE__,"Do not use Seek function of this class.");
  return 0;
}

int TArtStreamingDataSource::Read(char *buf, const int& size, const int &offset) 
{
  TArtCore::Debug(__FILE__,"Read");

  int len = sock->RecvRaw(buf,size);
  return len;
}

