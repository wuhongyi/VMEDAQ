#include "TArtRecoFragment.hh" 



//__________________________________________________________
TArtRecoFragment::TArtRecoFragment() : fFragmentArray(NULL), 
				       hod_array(NULL), 
				       pla_tzero(NULL),
				       BDCs_dist(999.32),
				       BDC1_Tgt_dist(2016.62),
				       Tgt_FDC1_dist(1188.38),
				       BDC2X0(0), BDC2Y0(0),
				       QtoZ_a(0), QtoZ_b(0),
				       clight(299.79246),
				       mnucleon(931.494028),
				       fMatReady(false), 
				       fRKtraceReady(true),
				       fMultDimReady(false)
{

  // default value from samurai dayone //////
  // do not edit following parameter!! //////
  // use SetCenterBrho function. Aki //////// 
  center_brho = 7.2751;
  l_pla2target = 2824.; // mm
  ////////////////////////


  TArtCore::Info(__FILE__,"Creating the Reco Fragment objects...");
  sman = TArtStoreManager::Instance();

  fFragmentArray = new TClonesArray("TArtFragment",4);
  fFragmentArray->SetName("SAMURAIFragment");
  fFragmentArray->SetOwner();
  sman->AddDataContainer(fFragmentArray);

  TString mat0thfile = std::getenv("SAMURAI_MATRIX0TH_FILE");
  TString mat1stfile = std::getenv("SAMURAI_MATRIX1ST_FILE");
  TString magfieldfile = std::getenv("SAMURAI_BFIELD_FILE");
  TString dcgeomfile = std::getenv("SAMURAI_DCGEOM_FILE");

  if(!dcgeomfile.IsNull() && !magfieldfile.IsNull()){
    if(!(DCConfMan::GetInstance().Initialize(dcgeomfile.Data())))
      fRKtraceReady = false;
    if(!(FieldMan::GetInstance().Initialize(magfieldfile.Data(),1.0)))
      fRKtraceReady = false;
  } else {
    fRKtraceReady = false;
  }

  
  if(!mat0thfile.IsNull() && !mat1stfile.IsNull()){

    std::ifstream mat0thin(mat0thfile.Data());
    std::ifstream mat1stin(mat1stfile.Data());

    if(mat0thin.is_open() && mat1stin.is_open()){

      mat0.ResizeTo(2,1);
      mat0thin >> mat0(0,0) >> mat0(1,0);

      Double_t mat[3][3];
      mat1stin >> mat[0][0] >>  mat[0][1] >> mat[0][2];
      mat1stin >> mat[1][0] >>  mat[1][1] >> mat[1][2];
      mat1stin >> mat[2][0] >>  mat[2][1] >> mat[2][2];

      mat1.ResizeTo(2,1);
      mat1(0,0) = mat[0][0];
      mat1(1,0) = mat[1][0];
      mat2.ResizeTo(2,2);
      mat2(0,0) = mat[0][1];
      mat2(1,0) = mat[1][1];
      mat2(0,1) = mat[0][2];
      mat2(1,1) = mat[1][2];
      inv_mat2.ResizeTo(2,2);
      inv_mat2=mat2.Invert();

      fMatReady = true;

    }
    else if(!(mat0thin.is_open())){
      TArtCore::Error(__FILE__,"fail to read 0th mat file: %s",mat0thfile.Data());
      fMatReady = false;
    }
    else if(!(mat1stin.is_open())){
      TArtCore::Error(__FILE__,"fail to read 1st mat file: %s",mat1stfile.Data());
      fMatReady = false;
    }

    mat0thin.close();
    mat1stin.close();
  }



  if(fRKtraceReady){
    TClonesArray * pla_array = (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
    if(pla_array){
      pla_tzero = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F13pl-2");
      if(!pla_tzero){
	TArtCore::Error(__FILE__,"Failed to find plastic F13pl-2");
      }
    } else {
      fRKtraceReady = false;
      TArtCore::Error(__FILE__,"Failed to find plastic F13pl-2");     
    }
    hod_array = (TClonesArray *)sman->FindDataContainer("SAMURAIHODPla");
    if(!hod_array){
      fRKtraceReady = false;
      TArtCore::Error(__FILE__,"Failed to find SAMURAIHODPla");     
    }
  }

  fMultDimReady = ReadParameterMultiDimFit();
  if(fMultDimReady){
     TClonesArray * pla_array = (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
    if(pla_array){
      pla_tzero = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F13pl-2");
      if(!pla_tzero){
	TArtCore::Error(__FILE__,"Failed to find plastic F13pl-2");
      }
    } else {
      fRKtraceReady = false;
      TArtCore::Error(__FILE__,"Failed to find plastic F13pl-2");     
    }
     hod_array = (TClonesArray *)sman->FindDataContainer("SAMURAIHODPla");
    if(hod_array){
      TArtCore::Info(__FILE__,"Found HODO plastic array");
    } else {
      fMultDimReady = false;
      TArtCore::Error(__FILE__,"Failed to find plastic SAMURAIHODPla for MultiDimFit");     
    }

  }
}

//__________________________________________________________
TArtRecoFragment::~TArtRecoFragment()  {
  ClearData();
}

//__________________________________________________________
void TArtRecoFragment::ClearData()   {
  fFragmentArray->Clear();
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtRecoFragment::ReconstructData() { 

  if((!fMatReady) && (!fRKtraceReady) && (!fMultDimReady))return;

  TClonesArray * fdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
  TClonesArray * fdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");

  if(!(fdc1trks && fdc2trks)) return;

  Int_t ntr_fdc1 = fdc1trks->GetEntriesFast();
  Int_t ntr_fdc2 = fdc2trks->GetEntriesFast();
  if(!(ntr_fdc1>0 && ntr_fdc2>0)){
    fReconstructed = true;
    return;
  }
  
  TClonesArray * bdc1trks = (TClonesArray*) NULL;
  TClonesArray * bdc2trks = (TClonesArray*) NULL;
  Double_t bdc1tr[] = {
    -9999,-9999
  };
  Double_t bdc2tr[] = {
    -9999,-9999
  };

  Double_t tgttr[] = {
    -9999,-9999
  };
  

  if(fMultDimReady){
    bdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
    bdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");
  
    if(!(bdc1trks && bdc2trks)) return;
    
    Double_t chi2x1 = DBL_MAX,chi2x2 = DBL_MAX ;
    Double_t chi2y1 = DBL_MAX,chi2y2 = DBL_MAX ;
    TArtDCTrack *trk1, *trk2 ;
    Int_t bdc1n = bdc1trks->GetEntriesFast();
    Int_t bdc2n = bdc2trks->GetEntriesFast();
    Bool_t bdc1_X_ok=kFALSE, bdc1_Y_ok=kFALSE, bdc2_X_ok=kFALSE, bdc2_Y_ok=kFALSE;

    //    std::cout << bdc1n << " " << bdc2n << std::endl;
    for(int i=0;i<TMath::Max(bdc1n,bdc2n);i++){
      if(i<bdc1n){
	trk1 = (TArtDCTrack *)bdc1trks->At(i);
	if(trk1){
	  if(trk1->GetPosition(0)>-5000){
	    if(trk1->GetChi2()/(Double_t)trk1->GetNDF() < chi2x1 && trk1->GetChi2()>0){
	      chi2x1 = trk1->GetChi2()/(Double_t)trk1->GetNDF();
	      bdc1tr[0]  = trk1->GetPosition(0);
	      bdc1_X_ok = kTRUE;
	    };
	  }
	  if(trk1->GetPosition(1)>-5000){
	    if(trk1->GetChi2()/(Double_t)trk1->GetNDF() < chi2y1 && trk1->GetChi2()>0){
	      chi2y1 = trk1->GetChi2()/(Double_t)trk1->GetNDF();
	      bdc1tr[1]  = trk1->GetPosition(1);
	      bdc1_Y_ok = kTRUE;
	    };
	  }
	}
      }
      if(i<bdc2n){
	trk2 = (TArtDCTrack *)bdc2trks->At(i);
	if(trk2){
	  if(trk2->GetPosition(0)>-5000){
	    if(trk2->GetChi2()/(Double_t)trk2->GetNDF() < chi2x2 && trk2->GetChi2()>0){
	      chi2x2 = trk2->GetChi2()/(Double_t)trk2->GetNDF();
	      bdc2tr[0]  = trk2->GetPosition(0) - BDC2X0;
	      bdc2_X_ok = kTRUE;
	    };
	  }
	  if(trk2->GetPosition(1)>-5000){
	    if(trk2->GetChi2()/(Double_t)trk2->GetNDF() < chi2y2 && trk2->GetChi2()>0){
	      chi2y2 = trk2->GetChi2()/(Double_t)trk2->GetNDF();
	      bdc2tr[1]  = trk2->GetPosition(1) - BDC2Y0;
	      bdc2_Y_ok = kTRUE;
	    };
	  };
	}
      }
    }
    
    //    std::cout << bdc1_X_ok << " " <<  bdc1_Y_ok << " " <<  bdc2_X_ok << " " <<  bdc2_Y_ok << std::endl;
    if(!(bdc1_X_ok && bdc1_Y_ok && bdc2_X_ok && bdc2_Y_ok)){
      fReconstructed = true;
      return;
    }
    
    tgttr[0] = bdc1tr[0] + (bdc2tr[0]-bdc1tr[0])/(BDCs_dist)*BDC1_Tgt_dist;
    tgttr[1] = bdc1tr[1] + (bdc2tr[1]-bdc1tr[1])/(BDCs_dist)*BDC1_Tgt_dist;

  }

  // as of 2013/10/10 one charged particle is assumed

  TArtDCTrack * fdc1tr = (TArtDCTrack*)fdc1trks->At(0); // chi2 minimum track
  TArtDCTrack * fdc2tr = (TArtDCTrack*)fdc2trks->At(0); // chi2 minimum track
  TArtCore::Debug(__FILE__,"fdcnhit: %d, %d",fdc1tr->GetNumHitLayer(),fdc2tr->GetNumHitLayer());
  TArtCore::Debug(__FILE__,"fdcchi2: %f, %f",fdc1tr->GetChi2(),fdc2tr->GetChi2());
  TArtCore::Debug(__FILE__,"fdc1x: %f, fdc1a: %f",fdc1tr->GetPosition(0),fdc1tr->GetAngle(0));
  TArtCore::Debug(__FILE__,"fdc2x: %f, fdc2a: %f",fdc2tr->GetPosition(0),fdc2tr->GetAngle(0));

  new ((*fFragmentArray)[0]) TArtFragment();
  TArtFragment * frag = (TArtFragment *)fFragmentArray->At(0);
  Double_t toflength = -1 ;
  Double_t brho = -1;

  TArtCore::Debug(__FILE__,"fMultDimReady ",fMultDimReady);
  if(fRKtraceReady || fMultDimReady){
    if(fRKtraceReady) {
      //  TArtCalcGlobalTrack gtr(fdc1tr,fdc2tr,center_brho*(1.+rvec(1,0))*0.3);
      TArtCalcGlobalTrack gtr(fdc1tr,fdc2tr,center_brho*0.3);
      gtr.doFit();
      frag->SetRKtraceStatus(gtr.Status());
      frag->SetChi2(gtr.chisqr());
      frag->SetNHit(gtr.GetNHits());
      frag->SetNIteration(gtr.Niteration());
      frag->SetPrimaryPosition(gtr.PrimaryPosition());
      frag->SetPrimaryMomentum(gtr.PrimaryMomentum());
      toflength = gtr.PathLengthToTOF() + l_pla2target; // mm;
      frag->SetPathLengthToHOD(toflength); // target - HOD path length
      for(int i=0;i<100;i++) frag->SetDiff(i,gtr.diff(i));
    }
    TArtCore::Debug(__FILE__,"fMultDimReady ",fMultDimReady);
    if(fMultDimReady) {
      TArtCore::Debug(__FILE__,"Start procedure");
      frag->SetRKtraceStatus(false); // for compatibility
      frag->SetChi2(1);		    // for compatibility
      frag->SetNHit(1);		    // for compatibility
      frag->SetNIteration(1);       // for compatibility
      frag->SetPrimaryPosition(TVector3(fdc1tr->GetPosition(0),fdc1tr->GetPosition(1),0));
      
      TArtDCTrack * bdc1tr_v = (TArtDCTrack*)bdc1trks->At(0); // chi2 minimum track
      TArtDCTrack * bdc2tr_v = (TArtDCTrack*)bdc2trks->At(0); // chi2 minimum track
      TArtCore::Debug(__FILE__,"bdcnhit: %d, %d",bdc1tr_v->GetNumHitLayer(),bdc2tr_v->GetNumHitLayer());
      TArtCore::Debug(__FILE__,"bdcchi2: %f, %f",bdc1tr_v->GetChi2(),bdc2tr_v->GetChi2());
      TArtCore::Debug(__FILE__,"bdc1x: %f, bdc1a: %f",bdc1tr_v->GetPosition(0),bdc1tr_v->GetAngle(0));
      TArtCore::Debug(__FILE__,"bdc2x: %f, bdc2a: %f",bdc2tr_v->GetPosition(0),bdc2tr_v->GetAngle(0));



      Double_t xin[6];
      xin[0] = fdc1tr->GetPosition(0);
      xin[1] = -fdc1tr->GetPosition(1);// still inverted ?
      xin[2] = TMath::ATan2(fdc1tr->GetPosition(0)-tgttr[0],Tgt_FDC1_dist); // from BDCs + Target 
      xin[3] = TMath::ATan2(-/*?*/fdc1tr->GetPosition(1)-tgttr[1],Tgt_FDC1_dist); // from BDCs + Target 
      xin[4] = fdc2tr->GetPosition(0);
      xin[5] = TMath::ATan(fdc2tr->GetAngle(0));

//      std::cout << BDCs_dist << " " << BDC1_Tgt_dist << " " << Tgt_FDC1_dist <<  std::endl;
//      std::cout << tgttr[0] << " " << tgttr[1] << std::endl;
//      std::cout << bdc1tr[0] << " " << bdc1tr[1] << std::endl;
//      std::cout << bdc2tr[0] << " " << bdc2tr[1] << std::endl;
//      for(int i=0;i<6;i++){
//	std::cout << " (" << i << ") " << xin[i] ;
//      }
//      std::cout << std::endl;

      toflength = GetPathLengthFitResult(xin);
      frag->SetPathLengthToHOD(toflength);
      
      brho = GetRigidityFitResult(xin);
      frag->SetBrho(brho);
      //    frag->SetPrimaryMomentum(gtr.PrimaryMomentum());
      TArtCore::Debug(__FILE__,"Brho: %f",brho);
    }

    if(hod_array && pla_tzero && (fRKtraceReady||fMultDimReady)){
      if(hod_array->GetEntries()>0){
	TArtHODPla *pla = (TArtHODPla *)hod_array->At(0); // pickup qmax-hod	
	Double_t tzero = pla_tzero->GetTime();
	frag->SetTzero(tzero);
	frag->SetTzeroQ(pla_tzero->GetQAveRaw());
	Double_t hodt = pla->GetTime();
	//	std::cout << "HID: " << pla->GetID() << std::endl;
	frag->SetHODTime(hodt);
	frag->SetHODQ(pla->GetQAveCal());
	frag->SetZ(QtoZ_a*pla->GetQAveCal()+QtoZ_b);
	Double_t tof = hodt-tzero + tof_offset[pla->GetID()];
	//TArtCore::Debug(__FILE__,"tzero: %f , hodt: %f",tzero,hodt);
	frag->SetTOF(tof);
	Double_t beta = toflength/tof/clight;
	frag->SetBeta(beta);
	Double_t gamma = 1/TMath::Sqrt(1-beta*beta);
	if(brho<0){
	  brho = frag->GetBrho();
	}
	Double_t aoq = brho * clight / mnucleon / beta / gamma;
	frag->SetAoQ(aoq);
	//std::cout << aoq << std::endl;
	//	TArtCore::Info(__FILE__,"beta: %f , aoq: %f",beta,aoq);
      }
    } else {
      TArtCore::Info(__FILE__,"cannot find pla %x %x",hod_array,pla_tzero);
    }
  } else if(fMatReady) {
    TMatrixD outxvec(2,1);
    outxvec(0,0) = fdc2tr->GetPosition(0);
    outxvec(1,0) = fdc2tr->GetAngle(0);

    TMatrixD rvec = inv_mat2 * (outxvec - fdc1tr->GetPosition(0) * mat1);
    frag->SetDelta(rvec(1,0));
    TArtCore::Debug(__FILE__,"Delta: %f",rvec(1,0));
  }

  fReconstructed = true;
  return;

}


void TArtRecoFragment::ReadParameters(std::string var, std::ifstream &is, std::vector<Double_t> &res){
  std::string line;
  is.clear();
  is.seekg(0,std::ios::beg);
  //  std::cout << "-------------------------------"  << std::endl; 
  //  std::cout << "Look for : " << var << std::endl; 
  while (is.good() ){	
    getline (is,line);    
    if(TString(line).Contains(var+"[]")){

      while(1){
	getline (is,line);
	TString data(line);
	//	std::cout << "Line : " << line << std::endl; 
	data.ReplaceAll(",","") ;
	data(TRegexp("//.*")) = 0;
	//	std::cout << "Clean : " << data << std::endl;
	if(data.IsNull()){
	  break;
	}
	std::istringstream ss(data.Data());
	Double_t tmp ;
	//      std::cout << "Results : " ;
	while(ss >> tmp){;
	  //	std::cout << "[" << tmp << "] " ;
	  res.push_back(tmp);
	}
	//      std::cout << std::endl << std::endl;
      }
    }
  }
  is.seekg(0);
  return ;
}

void TArtRecoFragment::ReadParameters(std::string var, std::ifstream &is, std::vector<Int_t> &res){
  std::vector<Double_t> tmp ;
  ReadParameters(var,is,tmp);
  for(std::vector<Double_t>::size_type i = 0; i != tmp.size(); i++) {
    res.push_back(tmp[i]);
  }
  return ;
}

void TArtRecoFragment::ReadParameters(std::string var, std::ifstream &is, Double_t &res){
  std::string line;
  is.clear();
  is.seekg(0,std::ios::beg);
  //  std::cout << "-------------------------------"  << std::endl; 
  //  std::cout << "Look for : " << var << std::endl; 
  while (is.good() ){	
    getline (is,line);    
    if(TString(line).Contains(var)){
      TString data(line);
      //      std::cout << "Line : " << line << std::endl; 
      data.ReplaceAll(",","") ;
      data(TRegexp("//.*")) = "";
      data(TRegexp(".*"+var+".*=")) = "";
      data(TRegexp(";")) = "";
      //      std::cout << "Clean : " << data << std::endl;

      std::istringstream ss(data.Data());
      Double_t tmp ;
      //      std::cout << "Results : " ;
      while(ss >> tmp){;
	//	std::cout << "[" << tmp << "] " ;	
      }
      //      std::cout << std::endl << std::endl;
      res = tmp;
      is.seekg(0);
      return ;
    }
  }
}

void TArtRecoFragment::ReadParameters(std::string var, std::ifstream &is, Int_t &res){
  Double_t tmp ;
  ReadParameters(var, is, tmp);
  res = tmp ;
  return ;
}

void TArtRecoFragment::PrintParameters(std::string var, std::vector<Double_t> res){
  TString msg("\e[32m") ;
  msg += var ;
  msg += ": \e[0m" ;
  msg += res[0];
  if(res.size()>1){
    msg += ", ";
    msg += res[1];
    if(res.size()>4){     
      msg += ", ... ";
    } else if(res.size()>2){
      msg += ", ";
    }
    if(res.size()>3){
      msg += res[res.size()-2];
      msg += ", ";
    }
    if(res.size()>2){
      msg += res[res.size()-1];
    }
  }
  msg += " [";
  msg += res.size() ;
  msg += " parameter(s)]";
  
  TArtCore::Info(__FILE__,msg.Data());
}
void TArtRecoFragment::PrintParameters(std::string var, std::vector<Int_t> res){
  std::vector<Double_t> v_float(res.begin(), res.end());
  PrintParameters(var,v_float);
}

Bool_t TArtRecoFragment::ReadParameterMultiDimFit(){


  // rigidity 

  TString multidimfile = std::getenv("SAMURAI_MULTIDIM_FILE_RIG");

  if(!multidimfile.IsNull()){
    std::ifstream multidimstream(multidimfile.Data());
        
    if (multidimstream.is_open()){
      ReadParameters("gNVariables"   ,multidimstream, r_gNVariables    );
      ReadParameters("gNCoefficients",multidimstream, r_gNCoefficients );
      ReadParameters("gDMean"        ,multidimstream, r_gDMean         );
      ReadParameters("gXMin",multidimstream,r_gXMin);
      ReadParameters("gXMax",multidimstream,r_gXMax);
      ReadParameters("gCoefficient",multidimstream,r_gCoefficient);
      //      ReadParameters("gCoefficientRMS",multidimstream,r_gCoefficientRMS);
      ReadParameters("gPower",multidimstream,r_gPower);      
      multidimstream.close();
    }
    
    else {
      TArtCore::Error(__FILE__, "Unable to open file %s ",multidimfile.Data());
      return kFALSE;
    }

    if(
       (   (UInt_t)r_gNCoefficients != r_gCoefficient.size() )
       || ((UInt_t)r_gNVariables    != r_gXMin.size() )
       || ((UInt_t)r_gNVariables    != r_gXMax.size() )
       || ((UInt_t)r_gNVariables*r_gNCoefficients    != r_gPower.size())){

      TArtCore::Info(__FILE__,"\e[31m\e[1mMultDimFit parameters for rigidity:\e[0m");
      TArtCore::Info(__FILE__,"\e[31min file:\e[0m  %s",multidimfile.Data());
      TArtCore::Info(__FILE__,"r_gNCoefficients,r_gNVariables %d,%d",r_gNCoefficients,r_gNVariables);
      PrintParameters("gXMin",r_gXMin);
      PrintParameters("gXMax",r_gXMax);
      PrintParameters("gCoefficient",r_gCoefficient);
      PrintParameters("gPower",r_gPower);      
      TArtCore::Error(__FILE__,"Error reading multdimfit file %s",multidimfile.Data());

      return kFALSE;
    }

  } else {
    return kFALSE;
  }

  TArtCore::Info(__FILE__,"\e[32m\e[1mMultDimFit parameters for rigidity:\e[0m");
  TArtCore::Info(__FILE__,"\e[32min file:\e[0m  %s",multidimfile.Data());
  PrintParameters("gXMin",r_gXMin);
  PrintParameters("gXMax",r_gXMax);
  PrintParameters("gCoefficient",r_gCoefficient);
  PrintParameters("gPower",r_gPower);      

  // path length

  multidimfile = std::getenv("SAMURAI_MULTIDIM_FILE_LEN");

  if(!multidimfile.IsNull()){
    std::ifstream multidimstream(multidimfile.Data());
        
    if (multidimstream.is_open()){
      ReadParameters("gNVariables"   ,multidimstream, l_gNVariables    );
      ReadParameters("gNCoefficients",multidimstream, l_gNCoefficients );
      ReadParameters("gDMean"        ,multidimstream, l_gDMean         );
      ReadParameters("gXMin",multidimstream,l_gXMin);
      ReadParameters("gXMax",multidimstream,l_gXMax);
      ReadParameters("gCoefficient",multidimstream,l_gCoefficient);
      //      ReadParameters("gCoefficientRMS",multidimstream,l_gCoefficientRMS);
      ReadParameters("gPower",multidimstream,l_gPower);      
      multidimstream.close();
    }
    
    else {
      TArtCore::Error(__FILE__, "Unable to open file %s ",multidimfile.Data());
      return kFALSE;
    }

    if(
       (   (UInt_t)l_gNCoefficients != l_gCoefficient.size() )
       || ((UInt_t)l_gNVariables    != l_gXMin.size() )
       || ((UInt_t)l_gNVariables    != l_gXMax.size() )
       || ((UInt_t)l_gNVariables*l_gNCoefficients    != l_gPower.size())){
      TArtCore::Info(__FILE__,"\e[31m\e[1mMultDimFit parameters for length:\e[0m");
      TArtCore::Info(__FILE__,"\e[31min file:\e[0m  %s",multidimfile.Data());
      TArtCore::Info(__FILE__,"l_gNCoefficients,l_gNVariables %d,%d",l_gNCoefficients,l_gNVariables);
      PrintParameters("gXMin",l_gXMin);
      PrintParameters("gXMax",l_gXMax);
      PrintParameters("gCoefficient",l_gCoefficient);
      PrintParameters("gPower",l_gPower);      
      TArtCore::Error(__FILE__,"Error reading multdimfit file %s",multidimfile.Data());
      return kFALSE;
    }

  } else {
    return kFALSE;
  }

  TArtCore::Info(__FILE__,"\e[32m\e[1mMultDimFit parameters for path length:\e[0m");
  TArtCore::Info(__FILE__,"\e[32min file:\e[0m %s",multidimfile.Data());
  PrintParameters("gXMin",l_gXMin);
  PrintParameters("gXMax",l_gXMax);
  PrintParameters("gCoefficient",l_gCoefficient);
  PrintParameters("gPower",l_gPower);      

  return kTRUE;
}


// -------- WARNING: this a macro not a function ------------------------
#define __GetFitResult__Macro__(t)					\
  Double_t returnValue = t ## _gDMean;					\
  Int_t  i = 0, j = 0, k = 0;						\
  for (i = 0; i < t ## _gNCoefficients ; i++) {				\
    Double_t term = t ## _gCoefficient[i];				\
    for (j = 0; j < t ## _gNVariables;  j++) {				\
      Int_t power = t ## _gPower[t ## _gNVariables * i + j];		\
      Double_t p1 = 1, p2 = 0, p3 = 0, r = 0;				\
      Double_t v =  1 + 2. / (t ## _gXMax[j] - t ## _gXMin[j]) * (x[j] - t ## _gXMax[j]); \
      switch(power) {							\
      case 1: r = 1;  break;						\
      case 2: r = v;  break;						\
      default:								\
	p2 = v;								\
	for (k = 3; k <= power; k++) {					\
	  p3 = p2 * v;							\
	  p3 = 2 * v * p2 - p1;						\
	  p1 = p2; p2 = p3;						\
	}								\
	r = p3;								\
      }									\
      term *= r;							\
    }									\
    returnValue += term;						\
  }									\
  return returnValue; 
// ----------------------------------------------------------------------

Double_t TArtRecoFragment::GetRigidityFitResult(Double_t *x) {
  __GetFitResult__Macro__(r)
    }

Double_t TArtRecoFragment::GetPathLengthFitResult(Double_t *x) {
  __GetFitResult__Macro__(l)
    }

Bool_t TArtRecoFragment::LoadParameter(const char *xmlfile)
{
  TArtCore::Info(__FILE__,"\e[32mLoad parameter from %s\e[0m", xmlfile);
  TDOMParser domParser;
  domParser.SetValidate(false);
  Int_t parsecode = domParser.ParseFile(xmlfile);
  if(parsecode < 0){
    std::cerr << domParser.GetParseCodeMessage(parsecode) << std::endl;
    return false;
  }
  TXMLNode* node = domParser.GetXMLDocument()->GetRootNode();

  if(strcmp(node->GetNodeName(), "dataroot")) return false;
  node = node->GetChildren();
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node    
    ParseParaList(node->GetChildren());
  }
  return true;
}

//__________________________________________________________
void TArtRecoFragment::ParseParaList(TXMLNode *node)
{
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node
    if(strcmp(node->GetNodeName(), "HODO") == 0){
      TXMLNode *subnode = node->GetChildren();
      UInt_t   id = -1;
      Double_t dt = 0.;
      for(; subnode; subnode = subnode->GetNextNode()){
	if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; 
 	if(strcmp(subnode->GetNodeName(), "ID")==0){
	  id = atoi(subnode->GetText()) ;
	}
 	if(strcmp(subnode->GetNodeName(), "TOFFSET")==0){
	  dt = atof(subnode->GetText()) ;
	  tof_offset[id] = dt ;
	}
      }
    } else if(strcmp(node->GetNodeName(), "BDCs_dist") == 0){
      BDCs_dist = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "BDC1_Tgt_dist") == 0){
      BDC1_Tgt_dist = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "Tgt_FDC1_dist") == 0){
      Tgt_FDC1_dist = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "BDC2X0") == 0){
      BDC2X0 = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "BDC2Y0") == 0){
      BDC2Y0 = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "QtoZ_a") == 0){
      QtoZ_a = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "QtoZ_b") == 0){
      QtoZ_b = atof(node->GetText());
    }

  }
}



ClassImp(TArtRecoFragment)
