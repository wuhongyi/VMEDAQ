/*
 *    TArtParser.h
 *    Created       : 2008/09/26 13:54:03 JST
 *    Last Modified : 2008/10/30 21:37:52 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA
 */
#ifndef TARTPARSER_H
#define TARTPARSER_H
#include "TArtDataSource.hh"
#include "TArtRawEventObject.hh"

class TArtParser {
public:
  static const int kMaxBufLen;
  TArtParser();
  virtual ~TArtParser();
  virtual bool Parse(TArtDataSource *source,TArtRawEventObject* raweve);
  virtual int  CopyPrevEvent(char *dest) = 0;
  virtual void SetSkip(bool flag){isSkip = flag;}
  
protected:
  virtual void GetNextBlock(TArtDataSource *source) = 0;
  virtual bool GetNextEvent(TArtRawEventObject *raweve) = 0;
  unsigned char *fBlockBuffer;
  int fOffset;
  int fNext;
  int fPrev;
  int fBlockSize;
  bool fEOB;
  bool isSkip;
  ArtEventStat_t  fEventStatus;

  ClassDef(TArtParser,1);

};
#endif // end of #ifdef TARTPARSER_H
