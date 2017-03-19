/*
  SAMURAIFieldMap.cc
made from
  SksFieldMap.cc
  2007/4  K.Shirotori
*/ 

#include "SAMURAIFieldMap.hh"

#include "TArtCore.hh"

#include "TMath.h"

#include <string>
#include <iostream>
#include <fstream>

SAMURAIFieldMap::SAMURAIFieldMap( const char *filename, double ScaleFactor )
  : magfile(filename), ScaleFactor_(ScaleFactor)
{
  tesla = 0.001;
  cm = 10.0;
}

SAMURAIFieldMap::~SAMURAIFieldMap()
{
}

bool SAMURAIFieldMap::Initialize( void )
{
  static const std::string funcname = "SAMURAIFieldMap::Initialize";
  char buffer[256];
  //TString magfile = std::getenv("SAMURAI_MAGFIELD_FILE");
  if(magfile.EndsWith("bin")){ // if it is binary.
    std::ifstream magin(magfile.Data(),std::ios::in|std::ios::binary);
    if(magin.is_open()){
      TArtCore::Info(__FILE__,"loading magnetic field data from: %s",magfile.Data());
      /* ANAROOT or SAMURAI geant4 coordinate axis */
      magin.read((char *)Bx,sizeof(Bx));
      magin.read((char *)By,sizeof(By));
      magin.read((char *)Bz,sizeof(Bz));
      magin.close();
    }
    else{
      TArtCore::Error(__FILE__,"fail to get magnetic field data from: %s",magfile.Data());
      magin.close();
    }
  }
  else if(magfile.EndsWith("table")){ // supposed to be ususal ascii file

    for(int i=0;i<301;i++)
      for(int j=0;j<81;j++)
	for(int k=0;k<301;k++){
	  Bx[i][j][k] = 0;
	  By[i][j][k] = 0;
	  Bz[i][j][k] = 0;
	}

    std::cout << "opening: " << magfile.Data() << std::endl;
    std::ifstream magfin(magfile.Data()); 
    for(int i=0;i<8;i++)  magfin.getline(buffer,256);

    Double_t v[7];
    
    while(magfin>>v[0]>>v[1]>>v[2]>>v[3]>>v[4]>>v[5]){
      v[1] += 400;
      //v[1] = 400 - v[1];
      //      cout << " x " << (v[0]/10.) << " y " << (v[1]/10.) << " z " << (v[2]/10.) << endl;
      /* ANAROOT or SAMURAI geant4 coordinate axis */
      Bx[(int)(v[0]/10.)][(int)(v[1]/10.)][(int)(v[2]/10.)] = TMath::Abs(v[3]) > 0.0001 ? v[3]*tesla : 0;
      By[(int)(v[0]/10.)][(int)(v[1]/10.)][(int)(v[2]/10.)] = TMath::Abs(v[4]) > 0.0001 ? v[4]*tesla : 0;
      Bz[(int)(v[0]/10.)][(int)(v[1]/10.)][(int)(v[2]/10.)] = TMath::Abs(v[5]) > 0.0001 ? v[5]*tesla : 0;

    }
    magfin.close();

    /*
    for(int i=0;i<301;i++)
      for(int j=0;j<81;j++)
	for(int k=0;k<301;k++){
	  std::cout << i << " " 
		    << j << " " 
		    << k << " " 
		    << Bx[i][j][k] << " "
		    << By[i][j][k] << " "
		    << Bz[i][j][k] << std::endl;
	}
    */

    magfile.Remove(magfile.Length()-2);
    magfile.Replace(magfile.Length()-3,3,"bin");
    std::cout << "making new binary file: " << magfile.Data() << std::endl;
    std::ofstream magout(magfile.Data(),std::ios::out|std::ios::binary);
    // ANAROOT or SAMURAI geant4 coordinate axis
    magout.write((char *)Bx,sizeof(Bx));
    magout.write((char *)By,sizeof(By));
    magout.write((char *)Bz,sizeof(Bz));

    magout.close();

  }
  else {
    TArtCore::Error(__FILE__,"can not identify bfield file type: %s",magfile.Data());
  }

  IsLoaded = true;

  TArtCore::Info(__FILE__,"Finish to read B Field Map ");

  return true;
}

bool SAMURAIFieldMap::GetFieldValue( const Double_t Pos[3],
				 Double_t *B ) const
{
  static const std::string funcname = "SAMURAIFieldMap::GetFieldValue";

  //  std::cout << "B x:" << Pos[0] << " y:" << Pos[1] << " z:" << Pos[2] << std::endl;

  if(!IsLoaded){ 
    TArtCore::Error(__FILE__,"load the magnetic field map!!");
  }
  // af[*] is correction for symmetric axis. 

  Double_t absx[3], x[3], d[3], af[3], dsqt[3], mdsq[3], Btmp[3];
  Int_t n[3];

  /* Pos[], B[]: SKS coordinate axis */
  /* x[], Btmp[]: ANAROOT or SAMURAI geant4 coordinate axis */

  x[0] = -1*Pos[1];
  x[1] = Pos[2];
  x[2] = -1*Pos[0];

  for(int i=0;i<3;i++){
    af[i] = x[i] < 0 ? -1 : 1;
    absx[i] = fabs(x[i]);
    B[i]=0;
    Btmp[i]=0;
  }

  // out of magnetic field
  if(!( absx[0]<300*cm && absx[1]<40*cm && absx[2]<300*cm ))
    return true;

  for(int i=0;i<3;i++){
    n[i] = i == 1 ? (int)((x[i]+40*cm)/cm) : (int)(absx[i]/cm);
    d[i] = i == 1 ? (x[i]+40*cm)/cm - n[i]: absx[i]/cm - n[i];
    //    dsqt[i] = d[i] * d[i];
    //    mdsq[i] = 1 - d[i] * d[i];
    if(0 == d[i]){
      mdsq[i] = 1;
      dsqt[i] = 0;
    }
    else if(1 == d[i]){
      mdsq[i] = 0;
      dsqt[i] = 1;
    }
    else{
      Double_t sum = 1/d[i] + 1/(1.-d[i]);
      mdsq[i] = 1/d[i]/sum;
      dsqt[i] = 1 - mdsq[i];
    }

  }

  Btmp[0] = mdsq[0]*mdsq[1]*mdsq[2]*Bx[n[0]  ][n[1]  ][n[2]  ] +
    dsqt[0]*mdsq[1]*mdsq[2]*Bx[n[0]+1][n[1]  ][n[2]  ] +
    mdsq[0]*dsqt[1]*mdsq[2]*Bx[n[0]  ][n[1]+1][n[2]  ] +
    mdsq[0]*mdsq[1]*dsqt[2]*Bx[n[0]  ][n[1]  ][n[2]+1] +
    dsqt[0]*dsqt[1]*mdsq[2]*Bx[n[0]+1][n[1]+1][n[2]  ] +
    dsqt[0]*mdsq[1]*dsqt[2]*Bx[n[0]+1][n[1]  ][n[2]+1] +
    mdsq[0]*dsqt[1]*dsqt[2]*Bx[n[0]  ][n[1]+1][n[2]+1] +
    dsqt[0]*dsqt[1]*dsqt[2]*Bx[n[0]+1][n[1]+1][n[2]+1];

  Btmp[1] = mdsq[0]*mdsq[1]*mdsq[2]*By[n[0]  ][n[1]  ][n[2]  ] +
    dsqt[0]*mdsq[1]*mdsq[2]*By[n[0]+1][n[1]  ][n[2]  ] +
    mdsq[0]*dsqt[1]*mdsq[2]*By[n[0]  ][n[1]+1][n[2]  ] +
    mdsq[0]*mdsq[1]*dsqt[2]*By[n[0]  ][n[1]  ][n[2]+1] +
    dsqt[0]*dsqt[1]*mdsq[2]*By[n[0]+1][n[1]+1][n[2]  ] +
    dsqt[0]*mdsq[1]*dsqt[2]*By[n[0]+1][n[1]  ][n[2]+1] +
    mdsq[0]*dsqt[1]*dsqt[2]*By[n[0]  ][n[1]+1][n[2]+1] +
    dsqt[0]*dsqt[1]*dsqt[2]*By[n[0]+1][n[1]+1][n[2]+1];

  Btmp[2] = mdsq[0]*mdsq[1]*mdsq[2]*Bz[n[0]  ][n[1]  ][n[2]  ] +
    dsqt[0]*mdsq[1]*mdsq[2]*Bz[n[0]+1][n[1]  ][n[2]  ] +
    mdsq[0]*dsqt[1]*mdsq[2]*Bz[n[0]  ][n[1]+1][n[2]  ] +
    mdsq[0]*mdsq[1]*dsqt[2]*Bz[n[0]  ][n[1]  ][n[2]+1] +
    dsqt[0]*dsqt[1]*mdsq[2]*Bz[n[0]+1][n[1]+1][n[2]  ] +
    dsqt[0]*mdsq[1]*dsqt[2]*Bz[n[0]+1][n[1]  ][n[2]+1] +
    mdsq[0]*dsqt[1]*dsqt[2]*Bz[n[0]  ][n[1]+1][n[2]+1] +
    dsqt[0]*dsqt[1]*dsqt[2]*Bz[n[0]+1][n[1]+1][n[2]+1];

  Btmp[0] *= af[0];// * af[1]; y-axis is not symmetric now
  Btmp[2] *= af[2];// * af[1]; y-axis is not symmetric now

  B[1] = Btmp[0]/tesla;
  B[2] = Btmp[1]/tesla;
  B[0] = Btmp[2]/tesla;

  /*
  std::cout << "n :" << n[0]<< " " << n[1] << " " << n[2]<< std::endl;
  std::cout << "Bx:" << Bx[n[0]  ][n[1]  ][n[2]  ] << std::endl;
  std::cout << "By:" << By[n[0]  ][n[1]  ][n[2]  ] << std::endl;
  std::cout << "Bz:" << Bz[n[0]  ][n[1]  ][n[2]  ] << std::endl;
  */
  //  std::cout << "MyPos: " << Pos[0] << " " << Pos[1] << " " << Pos[2] << " B: " << Btmp[0] << " " << Btmp[1] << " " << Btmp[2] << std::endl;



  return true;
}

