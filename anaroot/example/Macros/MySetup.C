void MySetup(){

  TArtBigRIPSParameters *bsetup = TArtBigRIPSParameters::Instance();
  //  bsetup->SetFocusPosOffset(8,148.5);

  cout << "setting up rips" << endl;
  TArtRIPSPara *rips3to5 = new TArtRIPSPara(3,5,"matrix/mat1.mat",7.942); // f3 - f5
  bsetup->AddRIPSPara(rips3to5);

  cout << "setting up tof" << endl;
  TArtTOFPara * tof7to13  = new TArtTOFPara("F7pl","F13pl-1",167.96);
  bsetup->AddTOFPara(tof7to13);

  cout << "setting up beam id devices" << endl;
  TArtBeamPara * beam = new TArtBeamPara(rips3to5,tof7to13,"ICB");
  bsetup->AddBeamPara(beam);

}
