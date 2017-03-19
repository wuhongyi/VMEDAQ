/*
 *    TArtCore.cc
 *    Created       :  9·î 26 2008 18:09:12
 *    Last Modified : Sep 05, 2009 13:31:50 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA
 */
#include "TArtCore.hh"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
TArtCore::TArtCore()
{
  //isDebug=true;
}
TArtCore::~TArtCore()
{
}

void TArtCore::Info(const char* cname,const char* fmt, ...)
{
   static int nmesseage = 0;

   if(nmesseage>1000) return;

   va_list argv;
   va_start(argv,fmt);
   char msg[1024];
   char omsg[1024];
   sprintf(msg,"  RIBFROOT-Info : [%s] %s\n",cname,fmt);
   vsprintf(omsg,msg,argv);
   va_end(argv);
   std::cout << omsg;

   if(nmesseage==1000)
     printf("  RIBFROOT-Info : too many info. suppress the information.\n");

   nmesseage ++;

}

//
// in the case of Error, the messesage will be red and the processing will stop for debugging
//

void TArtCore::Error(const char* cname,const char* fmt, ...)
{
   va_list argv;
   va_start(argv,fmt);
   char msg[1024];
   char omsg[1024];
   sprintf(msg,"\x1b[31m  RIBFROOT-Error : [%s] %s\n\x1b[39m",cname,fmt);
   vsprintf(omsg,msg,argv);
   va_end(argv);
   std::cout << omsg;

   static bool nomore = false;
   if(!nomore){
     std::cout << " [q]: abort, [n]: do not stop any more, others: continue > "
               << std::flush;
     std::string str;
     std::getline(std::cin, str);
     if(!str.empty() && (str.c_str())[0] =='q'){
       std::cout << " abort anaroot " << std::endl;
       abort();
     }else if(!str.empty() && (str.c_str())[0] =='n'){
       nomore = true;
     }
     std::cout << std::endl;
   }

}

void TArtCore::Warning(const char* cname,const char* fmt, ...)
{
  return; // will be fixed later
  //  if(!isDebug) return;

   va_list argv;
   va_start(argv,fmt);
   char msg[1024];
   char omsg[1024];
   sprintf(msg,"  RIBFROOT-Warn : [%s] %s\n",cname,fmt);
   vsprintf(omsg,msg,argv);
   va_end(argv);
   std::cout << omsg;
}
   
// Debug messeage, but the number of messeage will be too large to follow all of them. 

void TArtCore::Debug(const char* cname,const char* fmt, ...)
{
  return;
  //  if(!isDebug) return;

   va_list argv;
   va_start(argv,fmt);
   char msg[1024];
   char omsg[1024];
   sprintf(msg,"  RIBFROOT-Debug : [%s] %s\n",cname,fmt);
   vsprintf(omsg,msg,argv);
   va_end(argv);
   std::cout << omsg;
}

