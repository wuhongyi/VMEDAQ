/*
 * @file TArtParser.cc
 * @date  Created : 2008/10/19 20:09:04 JST<BR>
 *  Last Modified : 2008/10/30 21:38:20 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtCore.hh"
#include "TArtParser.hh"
#include <iostream>

//const int TArtParser::kMaxBufLen = 0x40000;
const int TArtParser::kMaxBufLen = 0x800000; // 8MB
TArtParser::TArtParser() 
  : fBlockBuffer(0), fOffset(0), fNext(0), fBlockSize(0), fEOB(true), isSkip(false)
{
   fBlockBuffer = new unsigned char[kMaxBufLen];
}

TArtParser::~TArtParser()
{
   delete [] fBlockBuffer;
}

bool TArtParser::Parse(TArtDataSource* source, TArtRawEventObject *raweve) 
{
  TArtCore::Debug("TArtParser","Parse");
   // if data buffer is empty, load data from source
  if (fEOB)  GetNextBlock(source);
  // return false in the case of empty of data
  if (fEOB){
    TArtCore::Debug("TArtParser","end of data, finish to Parse");
    return false;
  }  // if data buffer is ready, decode the data
  GetNextEvent(raweve);

  return source->GetBlockStatus() != kEOF;
}

