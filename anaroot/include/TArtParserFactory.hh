/*
 * @file TArtParserFactory.h
 * @date  Created : 2008/10/17 18:03:35 JST<BR>
 *  Last Modified : 2008/10/17 18:05:00 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTPARSERFACTORY_H
#define TARTPARSERFACTORY_H
#include "TArtParser.hh"
#include "TArtDataSource.hh"

class TArtParserFactory  {

public:
   static TArtParser* Create(TArtDataSource* source);
   ~TArtParserFactory();

private:
   TArtParserFactory();

};
#endif // end of #ifdef TARTPARSERFACTORY_H
