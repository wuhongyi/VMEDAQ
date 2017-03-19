/**
 * @file   TArtStreamingDataSource.hh
 * @author isobe tadaaki
 *  
 *    Copyright (C)2011
 */
#ifndef TARTSTREAMINGDATASOURCE_H
#define TARTSTREAMINGDATASOURCE_H

#include "TArtDataSource.hh"

class TSocket;

class TArtStreamingDataSource  : public TArtDataSource {
public:
   TArtStreamingDataSource();
   ~TArtStreamingDataSource();
   bool SocketOpen(const char * hostname, const int port = 10080, const bool fromlast = true);
   int Read(char *buf, const int &size, const int &offset = 0);
   virtual int GetRunNumber(){return 0;}
   virtual void Close();
   virtual int Seek(long offset, int origin);
   virtual int IsPrepared();

private:

   TSocket *sock;

   ClassDef(TArtStreamingDataSource,1);

};
#endif // end of #ifdef TARTSTREAMINGDATASOURCE_H
