
#ifndef EWNUM_HH
#define EWNUM_HH

namespace WNum{

  namespace GLOBAL{
    enum {
      // ID 0
      RUNNUM = 1, //1
      EVENTNUM,   //2

      // ID 0:bit, 1-16:true or false
      TRIGGERBIT  //3
    };
  }

  namespace RAWDATA{
    enum {
      DEVICE = 1, // 1
      FPL,        // 2
      DETECTOR,   // 3
      MODULE,     // 4
      GEO,        // 5
      CHANNEL,    // 6
      VALUE       // 7
    };
  }

  // Definitions for BigRIPS/ZeroDegree analysis

  namespace PPAC{
    enum {

      // ID 1: F1PPAC-1
      //    2: F1PPAC-2
      //    3: F2PPAC-1
      //    4: F2PPAC-2
      //    5: F3PPAC-1A
      //    6: F3PPAC-1B
      //    7: F3PPAC-2A
      //    8: F3PPAC-2B
      //    9: F4PPAC
      //   10: F5PPAC-1A
      //   11: F5PPAC-1B
      //   12: F5PPAC-2A
      //   13: F5PPAC-2B
      //   14: F6PPAC
      //   15: F7PPAC-1A
      //   16: F7PPAC-1B
      //   17: F7PPAC-2A
      //   18: F7PPAC-2B
      //   19: F8PPAC-1A
      //   20: F8PPAC-1B
      //   21: F8PPAC-2A
      //   22: F8PPAC-2B
      //   23: F9PPAC-1A
      //   24: F9PPAC-1B
      //   25: F9PPAC-2A
      //   26: F9PPAC-2B
      //   27: F10PPAC-1A
      //   28: F10PPAC-1B
      //   29: F10PPAC-2A
      //   30: F10PPAC-2B
      //   31: F11PPAC-1A
      //   32: F11PPAC-1B
      //   33: F11PPAC-2A
      //   34: F11PPAC-2B

      ID = 1,     // 1
      TX1RAW,     // 2
      TX2RAW,     // 3
      TY1RAW,     // 4
      TY2RAW,     // 5
      TARAW,      // 6
      QX1RAW,     // 7
      QX2RAW,     // 8
      QY1RAW,     // 9
      QY2RAW,     // 10
      QARAW,      // 11
      TX1,        // 12
      TX2,        // 13
      TY1,        // 14
      TY2,        // 15
      TA,         // 16
      QX1,        // 17
      QX2,        // 18
      QY1,        // 19
      QY2,        // 20
      QA,         // 21
      TX1TADIFF,  // 22
      TX2TADIFF,  // 23
      TY1TADIFF,  // 24
      TY2TADIFF,  // 25

      POSX=31,    // 31 (mm)
      POSY,       // 32 (mm)
      TXDIFF,     // 33
      TYDIFF,     // 34
      TXSUM,      // 35
      TYSUM,      // 36
      QXSUM,      // 37
      QYSUM,      // 38

      // ID 101: F1
      //    102: F2
      //    103: F3
      //    104: F4
      //    105: F5
      //    106: F6
      //    107: F7
      //    108: F8
      //    109: F9
      //    110: F10
      //    111: F11
      //    112: F12
      //    113: F13

      X=2,        // 2
      Y,          // 3
      A,          // 4
      B,          // 5
      T,          // 6
      RESX,       // 7
      RESY,       // 8
      AMRAD,      // 9 (mrad)
      BMRAD,      // 10 (mrad)
      R           // 11

    };
  }

  namespace PLASTIC{
    enum {
      // ID 1: F2pl
      //    2: F3pl
      //    3: F5pl-1
      //    4: F5pl-2
      //    5: F7pl
      //    6: F8pl
      //    7: F11pl-1
      //    8: F11pl-2
      //    9: F12pl

      ID = 1,     // 1
      TLRAW,      // 2
      TRRAW,      // 3
      QLRAW,      // 4
      QRRAW,      // 5
      QRAW,       // 6

      TL=12,      // 12
      TR,         // 13
      QL,         // 14
      QR,         // 15

      T,          // 21
      TDIFF,      // 22
      Q,          // 23
      QDIFF,      // 24
      QMEV,       // 25

      X,          // 32

    };
  }

  namespace IC{
    enum {
      // ID 1: F2IC, 2: F3IC, 3: F7IC, 4: F11IC 

      ID = 1,     // 1
      CH1,        // 2
      CH2,        // 3
      CH3,        // 4
      CH4,        // 5
      CH5,        // 6
      CH6,        // 7
      CH7,        // 8
      CH8,        // 9
      SUMAVE,     // 10
      SQTAVE,     // 11
      SUMAVEMEV,  // 12
      SQTAVEMEV   // 13

    };
  }

  namespace MATRIX{
    enum {

      // ID 1: BigRIPS from position    (not analyzed)
      //    2: ZeroDegree from position (not analyzed)
      //   11: F3-F5
      //   12: F5-F7
      //   13: F8-F9
      //   14: F9-F11

      ID = 1,     // 1
      DELTA,      // 2 (%)
      BRHO,       // 3
      D_FL,       // 4
      RECA,       // 5
      RECB        // 6      

    };
  }

  namespace PID{
    enum {

      ID = 1,     // 1

      // ID 1: TOF & Beta

      F3PL_F5PL2_TOF, // 2
      F3PL_F7PL_TOF,  // 3
      F3PL_F8PL_TOF,  // 4
      F5PL2_F7PL_TOF, // 5
      F7PL_F8PL_TOF,  // 6
      F7PL_F11PL_TOF, // 7
      F8PL_F9PPAC_TOF,// 8
      F8PL_F11PL_TOF, // 9

      F3PL_F5PL2_BETA=22, // 22
      F3PL_F7PL_BETA,     // 23
      F3PL_F8PL_BETA,     // 24
      F5PL2_F7PL_BETA,    // 25
      F7PL_F8PL_BETA,     // 26
      F7PL_F11PL_BETA,    // 27
      F8PL_F9PPAC_BETA,   // 28
      F8PL_F11PL_BETA,    // 29

      // ID 2: dE

      F3PL_DE=2, // 2
      F7IC_DE,   // 3
      F7PL_DE,   // 4
      F11IC_DE,  // 5
      F11PL_DE,  // 6
      F11PL2_DE, // 7

      // ID 11: Brho35_rec & dE7(IC) & Beta35_REC
      //    12: Brho57_rec & dE7(IC) & Beta57_REC
      //    13: Brho35_rec & dE7(IC) & Beta37
      //    14: Brho57_rec & dE7(IC) & Beta37
      //    15: Brho89_rec & dE11(IC) & Beta811
      //    16: Brho911_rec & dE11(IC) & Beta811
      //    17: Brho1011_rec & dE11(IC) & Beta811
      //    18: Brho89_rec & dE11(IC) & Beta711
      //    19: Brho911_rec & dE11(IC) & Beta711

      BRHO=2,        // 2
      BETA,          // 3
      Z,             // 4
      AOQ,           // 5
      AOQTIMESZ,     // 6
      AOQMINUS1TIMESZ// 7

    };
  }

  // Definitions for DALI analysis

  namespace DALI{
    enum {
      // ID 1-140:module, 141,142:TRef
      ID=1,     //1
      Layer,    //2
      Theta,    //3
      CosTheta, //4

      RawADC=10,      //10
      Energy,         //11
      DoppCorEnergy,  //12
      // EnergyWithoutT, //13

      RawTDC=30,    //30
      Time,         //31
      TimeOffseted, //32

      // ID 0
      Multiplicity=100,
    };
  }

  // Definitions for SAMURAI analysis

  namespace SAMURAIDC{
    enum {
      // ID 1:BPC, 2:BDC1, 3:BDC2, 4:FDC1, 5:FDC2 10:Image 11-:IDPlane (11,12:BPC  21-28:BDC1  31-38:BDC2  41-54:FDC1  61-74:FDC2)
      ID=1,
      XPos=10,   // for ID=1-5
      XAng,
      YPos,
      YAng,
      Chi2,
      NTr,

      Layer=51, // for ID=11-
      WireID,
      TRaw,
      TCal,
      TPos,
      WirePos,
      Delta,

      SBVX=101,  // for ID=10
      SBVY,
      TGTX,
      TGTY,
      TGTA,
      TGTB,
    };
  }

  namespace SAMURAIPLA{
    enum {
      // ID 1:F3pl, 2:F3pl-40nsec, 3:F7pl, 4:F13pl-1, 5:F13pl-2, 6:SBV, 7:F7pl2,
      ID = 1,     //1

      QLRaw = 10, //10
      QRRaw,      //11
      QAveRaw,    //12
      LogQRaw,    //13

      TLRaw = 30, //30
      TRRaw,      //31
      TAveRaw,    //32
      DTRaw,      //33
      TLCal,      //34
      TRCal,      //35
      TAveCal,    //36
      DTCal,      //37
      TLSlw,      //38
      TRSlw,      //39
      TAveSlw,    //40
      DTSlw,      //41

      // ID 0
      TOF1313 = 50, //50
      TOF1313Cal,   //51
      TOF1313Slw,   //52
    };
  }

  namespace SAMURAIHOD{
    enum {
      // ID 1-16:module
      ID=1,     //1
      CHANNEL,  //2
      HITID,    //3
      HITCH,    //4
      QURaw=10, //10
      QDRaw,    //11
      QAveRaw,  //12
      QUPed,    //13
      QDPed,    //14
      QAvePed,  //15
      QUCal,    //16
      QDCal,    //17
      QAveCal,  //18
      QAveCalTExist, //19
      LogQPed,      //20
      LogQCal,      //21

      TURaw=30, //30
      TDRaw,    //31
      TAveRaw,  //32
      TUCal,    //33
      TDCal,    //34
      TAveCal,  //35
      TUSlw,    //36
      TDSlw,    //37
      TAveSlw,  //38
      DTRaw,    //39
      DTCal,    //40
      DTSlw,    //41

      HODQMAXID=50,//50
      HODQMAX,     //51
      HODQMAXTDC,  //52

      // ID 0
      NHIT=70,        //70
    };
  }

  namespace SAMURAIPID{
    enum {
      // ID 1... : Tracks 
      ID=1,           // 1
      BRHO=2,         // 2
      BETA=3,         // 3
      Z   =4,         // 4 (TODO)
      AOQ =5,         // 5  
      AOQTIMESZ,      // 6 (TODO)
      AOQMINUS1TIMESZ,// 7 (TODO)
      HODQAveCal=18,  //18 
    };
  }

  namespace NEBULA{
    enum {
      // ID 1-144:module
      ID=1,     //1
      Layer,    //2
      SubLayer, //3
      DetPosX,  //4
      DetPosY,  //5
      DetPosZ,  //6
      Hit,      //7

      QURaw=10,     //10
      QDRaw,        //11
      QUPed,        //12
      QDPed,        //13
      QUCal,        //14
      QDCal,        //15
      QAvePed,      //16
      QAveCal,      //17
      LogQPed,      //18
      LogQCal,      //19
      IvSqrtQUPed,  //20
      IvSqrtQDPed,  //21
      IvSqrtQAvePed,//22

      TURaw=30,    //30
      TDRaw,       //31
      TUCal,       //32
      TDCal,       //33
      TUSlw,       //34
      TDSlw,       //35
      DTRaw,       //36
      DTCal,       //37
      DTSlw,       //38
      TAveRaw,     //39
      TAveCal,     //40
      TAveSlw,     //41
      TUCalT0,     //42
      TDCalT0,     //43
      TUSlwT0,     //44
      TDSlwT0,     //45
      TAveCalT0,   //46
      TAveSlwT0,   //47
      TTOFGamma,   //48
      TTOFNeutron, //49

      PosCal=50,//50
      PosSlw,   //51

      PosX=60,      //60
      PosY,         //61
      PosZ,         //62
      FlightLength, //63

      // ID 0
      Multiplicity_N   = 100,  //100
      Multiplicity_N1,         //101
      Multiplicity_N2,         //102
      Multiplicity_V1,         //103
      Multiplicity_V2,         //104
      Multiplicity_N11 = 111,  //111
      Multiplicity_N12,        //112
      Multiplicity_N21 = 121,  //121
      Multiplicity_N22,        //122
    };
  }

  namespace NEBULAHPC{
    enum {
      // ID 0
      Multiplicity=1, //1

      // ID 1-16:module
      ID=1,     //1
      Layer,    //2
      SubLayer, //3
      DetPosX,  //4
      DetPosY,  //5
      DetPosZ,  //6

      TRaw=30,  //30
      TCal      //31
    };
  }

  namespace SAMURAITZero{
    enum {
      // ID 1
      ID=1,     //1
      TZeroCal, //2
      TZeroSlw, //3
    };
  }

  namespace SAMURAIIC{
    enum {
      // ID 1:ICB, 2:ICF, 
      // 11-:IDPlane (11-21:ICB, 31-42:ICF0, 51-62:ICF1...)
      ID=1, //1
      NHIT, //2
      ARawSqSum, //3
      ARawAvSum, //4
      ESqSum, //5
      EAvSum, //6

      ARaw=10, //10
      Energy,  //11
    };
  }

  namespace SAMURAITED{
    enum {
      // ID 1-36:module
      ID=1,     //1
      Row,      //2
      Column,   //3

      RawADC=10,      //10
      Energy,         //11

    };
  }

  namespace NEUTRON{
    enum {
      // ID 0
      NumOfNeutron=1,

      // for two neutron events
      N2_DT,
      N2_Beta21,
      N2_ivBeta21,
      N2_AveMeVee,
      N2_SubMeVee,
      N2_DR,
      N2_Beta10_S_Beta21,
      N2_Beta20_S_Beta21,
      N2_ivBeta10_S_Beta21,
      N2_ivBeta20_S_Beta21,

      // ID 1-"num of neutron"
      ID=1,
      PX,
      PY,
      PZ,
      PAbs,
      Energy,
      Theta,
      Phi,

      Time = 10,
      MeVee,
      PosX,
      PosY,
      PosZ,
      BetaX,
      BetaY,
      BetaZ,
      Gamma
    };
  }

  namespace WINDS{
    enum {
      // ID 1-16:module
      ID=1,     //1
      CHANNEL,  //2
      HITID,    //3
      HITCH,    //4
      Q1Raw=10, //10
      Q2Raw,    //11
      QAveRaw,  //12
      Q1Ped,    //13
      Q2Ped,    //14
      QAvePed,  //15
      Q1Cal,    //16
      Q2Cal,    //17
      QAveCal,  //18
      QAveCalTExist, //19
      LogQPed,      //20
      LogQCal,      //21

      T1Raw=30, //30
      T2Raw,    //31
      TAveRaw,  //32
      T1Cal,    //33
      T2Cal,    //34
      TAveCal,  //35
      T1Slw,    //36
      T2Slw,    //37
      TAveSlw,  //38
      DTRaw,    //39
      DTCal,    //40
      DTSlw,    //41

      WINDSQMAXID=50,//50
      WINDSQMAX,     //51
      WINDSQMAXTDC,  //52

      // ID 0
      NHIT=70,        //70
    };
  }

  namespace RPTOF{
    enum {
      // ID 1-14:module
      ID=1,     //1
      CHANNEL,  //2
      HITID,    //3
      HITCH,    //4
      QULRaw=10, //10
      QDRRaw,    //11
      QAveRaw,  //12
      QULCal,    //16
      QDRCal,    //17
      QAveCal,  //18
      QAveCalTExist, //19

      TULRaw=30, //30
      TDRRaw,    //31
      TAveRaw,  //32
      TULCal,    //33
      TDRCal,    //34
      TAveCal,  //35
      DTRaw,    //39
      DTCal,    //40
      DTSlw,    //41

      PLAQMAXID=50,//50
      PLAQMAX,     //51
      PLAQMAXTDC,  //52

      // ID 0
      NHIT=70,        //70
    };
  }

  namespace RPDC{
    enum {

      // ID 11-18:layer
      ID=1,
      XPos=10,
      XAng,
      YPos,
      YAng,
      Chi2,
      NTr,

      Layer=21, // for ID=11-
      WireID,
      TRaw,
      TCal,
      TPos,
      WirePos,
      Delta,
    };
  }

  namespace MINOS{
    enum {
      // ID 1-96: ASIC number
      ID =  1,
      Fec=  2,
      Qmax= 3, // For Qmax channel within an ASIC
      Xpos=10, // For Qmax channel within an ASIC
      Ypos=12, // For Qmax channel within an ASIC
      Time=31, // For Qmax channel within an ASIC

      // this values are reset every ?? event
      ID_sample=51, // (TODO) Usefull ?
      Fec_sample=52, // (TODO) Usefull ?
      Qmax_sample=53, // (TODO) Usefull ?
      Xpos_sample=60, // (TODO) Usefull ?
      Ypos_sample=62, // (TODO) Usefull ?

      // Calculated values
      // ID 101-... : track by track 
      // 101: track1, 102: track2, etc
      Beta   = 3, // beta = v/c
      Xvertex=10, 
      Yvertex=12,
      Zvertex=14,
      Theta  =17 // theta = 

      
    };
  }

}

#endif
