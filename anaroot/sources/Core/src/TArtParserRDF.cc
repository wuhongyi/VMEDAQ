/*
 * @file TArtParserRDF.cc
 * @date  Created : 2008/10/19 11:07:34 JST<BR>
 *  Last Modified : 2008/10/30 21:50:15 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA<ota@ribf.riken.jp>
 */
#include "TArtCore.hh"
#include "TArtParserRDF.hh"

TArtParserRDF::TArtParserRDF()
{
}
TArtParserRDF::~TArtParserRDF()
{
}

void TArtParserRDF::GetNextBlock(TArtDataSource *source)
{
  TArtCore::Debug("TArtParserRDF","GetNextBlock");

  return;
}
bool TArtParserRDF::GetNextEvent(TArtRawEventObject *rawdata)
{
  TArtCore::Debug("TArtParserRDF","GetNextEvent");

  return true;
}
int TArtParserRDF::CopyPrevEvent(char *dest)
{
  return 0;
}
