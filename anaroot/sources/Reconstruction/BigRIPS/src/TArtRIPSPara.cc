#include "TArtRIPSPara.hh" 
#include <fstream>

//__________________________________________________________
TArtRIPSPara::TArtRIPSPara()
{
}

//__________________________________________________________
TArtRIPSPara::~TArtRIPSPara()  {
}

//__________________________________________________________
TArtRIPSPara::TArtRIPSPara(Int_t u, Int_t d, char *matfile) {
  up_fpl = u;
  down_fpl = d;
  brho = -1;
  SetMatrix(matfile);
  dipole_name = "";

  char dname[64]; 
  sprintf(dname,"RIPS%dto%d",u,d);
  detname = dname;
}

//__________________________________________________________
TArtRIPSPara::TArtRIPSPara(Int_t u, Int_t d, char *matfile, char *dipolename) {
  up_fpl = u;
  down_fpl = d;
  brho = -1;
  dipole_name = dipolename;
  SetMatrix(matfile);

  char dname[64]; 
  sprintf(dname,"RIPS%dto%d",u,d);
  detname = dname;
}

//__________________________________________________________
TArtRIPSPara::TArtRIPSPara(Int_t u, Int_t d, char *matfile, Double_t br) {
  up_fpl = u;
  down_fpl = d;
  brho = br;
  SetMatrix(matfile);
  dipole_name = "";

  char dname[64]; 
  sprintf(dname,"RIPS%dto%d",u,d);
  detname = dname;
}

//__________________________________________________________
void TArtRIPSPara::SetMatrix(char *matfile) {

  char buffer[256];
  std::ifstream matfin(matfile); 
  matfin.getline(buffer,256);
  matrix.ResizeTo(6,5); 
  double val[6];  
  for(Int_t i=0;i<6;i++){
    matfin>>val[0]>>val[1]>>val[2]>>val[3]>>val[4]>>val[5];
    for(Int_t j=0;j<5;j++)
      matrix(i,j) = val[j];
  }
  matfin.close();

}
