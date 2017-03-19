/* $Id: TArtFileDataSource.h 4 2009-09-02 15:59:29Z sweetbluemoon $ */
/**
 * @file   TArtFileDataSource.hh
 * @date   Created : Mar 29, 2008 16:29:03 JST
 *   Last Modified : 2008/10/30 20:09:46 JST
 * @author RIPS Taro <rips@entrance.ripsnet>
 *  
 *  
 *    Copyright (C)2008
 */
#ifndef TARTFILEDATASOURCE_H
#define TARTFILEDATASOURCE_H
#include <zlib.h>
#include "TArtDataSource.hh"
#include <iostream>
#include <sys/stat.h>
using namespace std;

class TArtFileDataSource  : public TArtDataSource {
public:

   TArtFileDataSource();
   ~TArtFileDataSource();
   int GetRunNumber(){return fRunNumber;}
   void SetRunNumber(int n){fRunNumber = n;}
   bool FileOpen(const char* sourceName = 0);
   void Close();
   int Read(char *buf, const int &size, const int &offset = 0);
   virtual int Seek(long offset, int origin);

private:
  //   FILE   *fFile;
   gzFile   fFile;
   struct stat fFileStat;
   int fRunNumber;
};
#endif // end of #ifdef TARTFILEDATASOURCE_H
