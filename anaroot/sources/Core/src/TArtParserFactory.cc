/*
 * @file TArtParserFactory.cc
 * @date  Created : 2008/10/17 18:04:39 JST<BR>
 *  Last Modified : 2008/10/17 18:28:20 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA<ota@ribf.riken.jp>
 */
#include "TArtParserFactory.hh"
#include "TArtParserRDF.hh"
#include "TArtParserRIDF.hh"

#include "TArtCore.hh"
TArtParser* TArtParserFactory::Create(TArtDataSource* source) {

  // return Parser for RIDF file.
  // Reading header of date will cause the problem in the case of Streaming mode.
  // Parser for RDF is not supported as of 2013 Sep.,24th.
  // Tadaaki Isobe
  return new TArtParserRIDF();

   // check second word
   unsigned int header[2];
   const int size = sizeof(int)*2;
   if (!source->Read((char*)header,size,0)) return (TArtParser*)0;
   source->Seek(0,0);
   switch (header[1]) {
   case 0:
     TArtCore::Info("TArtParserFactory","Format = RDF");
     return new TArtParserRDF();
      break;
   default:
     TArtCore::Info("TArtParserFactory","Format = RIDF");
     return new TArtParserRIDF();
   }
}

TArtParserFactory::TArtParserFactory()
{
}
TArtParserFactory::~TArtParserFactory()
{
}
