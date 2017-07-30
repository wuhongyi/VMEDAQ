// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME MainDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "TGShapedMain.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_TGShapedMain(void *p);
   static void deleteArray_TGShapedMain(void *p);
   static void destruct_TGShapedMain(void *p);
   static void streamer_TGShapedMain(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TGShapedMain*)
   {
      ::TGShapedMain *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TGShapedMain >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TGShapedMain", ::TGShapedMain::Class_Version(), "TGShapedMain.hh", 22,
                  typeid(::TGShapedMain), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TGShapedMain::Dictionary, isa_proxy, 16,
                  sizeof(::TGShapedMain) );
      instance.SetDelete(&delete_TGShapedMain);
      instance.SetDeleteArray(&deleteArray_TGShapedMain);
      instance.SetDestructor(&destruct_TGShapedMain);
      instance.SetStreamerFunc(&streamer_TGShapedMain);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TGShapedMain*)
   {
      return GenerateInitInstanceLocal((::TGShapedMain*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TGShapedMain*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TGShapedMain::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TGShapedMain::Class_Name()
{
   return "TGShapedMain";
}

//______________________________________________________________________________
const char *TGShapedMain::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TGShapedMain*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TGShapedMain::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TGShapedMain*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TGShapedMain::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TGShapedMain*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TGShapedMain::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TGShapedMain*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TGShapedMain::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGShapedMain.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TGShapedMain(void *p) {
      delete ((::TGShapedMain*)p);
   }
   static void deleteArray_TGShapedMain(void *p) {
      delete [] ((::TGShapedMain*)p);
   }
   static void destruct_TGShapedMain(void *p) {
      typedef ::TGShapedMain current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TGShapedMain(TBuffer &buf, void *obj) {
      ((::TGShapedMain*)obj)->::TGShapedMain::Streamer(buf);
   }
} // end of namespace ROOT for class ::TGShapedMain

namespace {
  void TriggerDictionaryInitialization_MainDict_Impl() {
    static const char* headers[] = {
"TGShapedMain.hh",
0
    };
    static const char* includePaths[] = {
"/opt/root60606/include",
"/home/wuhongyi/VMEDAQ/statistics/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "MainDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$TGShapedMain.hh")))  TGShapedMain;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "MainDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TGShapedMain.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TGShapedMain", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MainDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MainDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MainDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MainDict() {
  TriggerDictionaryInitialization_MainDict_Impl();
}
