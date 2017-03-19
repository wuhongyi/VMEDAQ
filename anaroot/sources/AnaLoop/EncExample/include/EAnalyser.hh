
#ifndef EANALYSER_HH
#define EANALYSER_HH

namespace Analyser{
  enum {
    GLOBAL=0,     //0
    PPAC,         //1
    PLASTIC,      //2
    IC=5,         //5

    MATRIX=10,    //10
    PID,          //11
    DALI,         //12

    NEBULA=21,    //21
    NEBULAHPC,    //22
    SAMURAIHOD,   //23
    SAMURAIPLA,   //24
    SAMURAIDC,    //25
    SAMURAITZero, //26
    SAMURAIIC,    //27
    SAMURAITED,   //28
    SAMURAIPID,   //29

    NEUTRON=31,   //31

    WINDS=40,     //40
    RPTOF,        //41
    RPDC,         //42

    MINOS=51,     //51

    RAWDATA=99,   //99

  };
}

#endif
