/* $Id: TArtDataSource.cc 4 2009-09-02 15:59:29Z sweetbluemoon $ */
/**
 * @file   TArtDataSource.cc
 * @date   Created : Mar 29, 2008 14:29:55 JST
 *   Last Modified : 2008/10/30 20:11:43 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2008
 */
#include "TArtDataSource.hh"
//const int TArtDataSource::kMaxBufferSize = 128 * 1024; // 128kByte
const int TArtDataSource::kMaxBufferSize = 8 * 1024* 1024; // 8MByte

TArtDataSource::TArtDataSource(int id)
  : fID(id) {
}
TArtDataSource::~TArtDataSource()
{
}

