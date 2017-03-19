#include "TAlEncMINOSExample.hh"

TAlEncMINOSExample::TAlEncMINOSExample()
  : TAlEncSub(), 
    MINOSthresh( 25.  ),
    TimeBinElec( 20.  ),
    VDrift    (   4.0701),
    Tshaping  ( 333.  ),
    DelayTrig (1200.0 ),
    Tgt_Length( 152.0 ),
    Beta      (   0.6 ),
    Delta_Beta(   0.0 ),
    Pos_offset( -7.0 ),
    DALIOffset(  0.0 )
{
  
  ;}

TAlEncMINOSExample::~TAlEncMINOSExample()
{;}

void TAlEncMINOSExample::Construct()
{


  fStoreManager = TArtStoreManager::Instance();

  fMINOSParameters = TArtMINOSParameters::Instance();
  fMINOSParameters->LoadParameters((char*)"db/MINOS.xml");

  fCalibMINOS = new TArtCalibMINOS();

  fAnalyzedMINOS = new TArtAnalyzedMINOS(fCalibMINOS);
  fTrackMINOS = new TArtTrackMINOS();
  fVertexMINOS = new TArtVertexMINOS();
  TArtCore::Info(__FILE__,"\e[36mStart MINOS parameter\e[0m");
  //Config : VDrift (mm/ns), TimeBin (20ns), TrigDelay (ns)
  //  fAnalyzedMINOS->SetConfig(0.047, 20, 1300); // RUN psp140043.ridf


  // Parameters for the MINOS ANALYSIS
  // transpose of "ConfigFileSetup.txt" in XML format
  
  LoadParameters("db/MINOS_Analysis.xml");
  fAnalyzedMINOS->SetConfig(VDrift, TimeBinElec, DelayTrig);

  hpoly1 = new TH2Poly("hA","MINOS: XY/1evt, Charge weighted",-85,85,-85,85);
  instanceCst(hpoly1);

  hpoly1_ok = (TH2Poly*)hpoly1->Clone("hB");
  hpoly1_ok->SetTitle("MINOS: XY/1evt, Charge weighted>0 ");

  hpoly = (TH2Poly*)hpoly1->Clone("hC");
  hpoly->SetTitle("MINOS: Accumulated XY view, Charge weighted");

  gDirectory->Add(hpoly1);
  gDirectory->Add(hpoly1_ok);
  gDirectory->Add(hpoly);


}

void TAlEncMINOSExample::PreCalculate()
{
  // ->ClearData();
}

bool TAlEncMINOSExample::Calculate()
{

  //********************* Clear *****************
  fCalibMINOS->ClearData();
  fAnalyzedMINOS->ClearData();
  fTrackMINOS->ClearData();
  fVertexMINOS->ClearData();
  hpoly1->ClearBinContents();
  //  rawevent->Clear();
  //  htest->Reset();
  using namespace Analyser;
  using namespace WNum::MINOS;  

  //  TArtRawEventObject *rawevent = (TArtRawEventObject *)fStoreManager->FindDataContainer("RawEvent");

  //  Int_t id = 0;
  //  Int_t Refresh = 100;
  //  Int_t eventnum = rawevent->GetEventNumber();

  Double_t xPad=0., yPad=0.;
  Int_t maxHitCount = 0;
  Int_t hitCount[96] = {0};
  Bool_t AsicHIT[96] = {kFALSE};
  Double_t AsicTime_forMaxCharge[96] = {0};
  Double_t PosX_forMaxCharge[96] = {0};
  Double_t PosY_forMaxCharge[96] = {0};
  Double_t FEC_forMaxCharge[96] = {0};
  Double_t MaxCharge[96] = {0};
  UInt_t  Channel_forMaxCharge[96] = {0};
  UInt_t AsicID = 0;
  UInt_t ListAsicID[96] = {0}; // for optimization : 
  UInt_t NAsicFired = 0;       // reduce the number of loops

  fCalibMINOS->ReconstructData();

  TArtCalibMINOSData *minos = new TArtCalibMINOSData();


  //  std::cout<<"Event : "<<eventnum<<" Size : "<<CalibMINOS->GetNumCalibMINOS()<<std::endl;

  //  if(neve%Refresh==0) {hQt1->Reset(); hpoly1->Reset(""); hasic1->Reset();}

  //read all Pads
  for(Int_t i=0;i<fCalibMINOS->GetNumCalibMINOS();i++) {
    minos = fCalibMINOS->GetCalibMINOS(i);
    xPad = minos->GetX();
    yPad = minos->GetY();

    if((minos->GetHitCount()-2)> maxHitCount) {
      maxHitCount = minos->GetHitCount()-2;
    }
	
    AsicID = minos->GetFec()*4+minos->GetAsic();
    hitCount[AsicID] = minos->GetHitCount()-2;
    
    if(!AsicHIT[AsicID]){
      fAnaLoop->Add(MINOS, ID, AsicID+1, AsicID+1);
      AsicHIT[AsicID] = kTRUE;
      ListAsicID[NAsicFired] = AsicID;
      NAsicFired++;
    }

    
    if( !(xPad==0 && yPad==0) ) {

      //      Double_t Ring = sqrt(xPad*xPad + yPad*yPad);
      for(Int_t j=0;j<minos->GetNData();j++) 
	{
	  if(minos->GetCalibValue(j)>MaxCharge[AsicID]) {

	    MaxCharge[AsicID] = minos->GetCalibValue(j);
	    FEC_forMaxCharge[AsicID] = minos->GetFec();
	    Channel_forMaxCharge[AsicID] = minos->GetChannel();
	    PosX_forMaxCharge[AsicID] = xPad;
	    PosY_forMaxCharge[AsicID] = yPad;
	    AsicTime_forMaxCharge[AsicID] = minos->GetCalibTime(j);
	    
	  }
	  //			hrt->Fill(minos->GetCalibTime(j),Ring);
	  //			if(neve%Refresh==0) hQt1->Fill(minos->GetCalibTime(j),minos->GetCalibValue(j));
	}
      
      hpoly->Fill(xPad, yPad, MaxCharge[AsicID]);
      hpoly1->Fill(xPad, yPad, MaxCharge[AsicID]);
      
      //if(neve%Refresh==0) hpoly1->Fill(xPad, yPad, maxCharge);
    }

    //std::cout<<"Fec : "<<minos->GetFec()<<"   Asic :"<<minos->GetAsic()<<" ch : "<<minos->GetChannel()<<std::endl;
  }

  if(hpoly1->Integral()>0){
    for( int iii = 0; iii < hpoly1->GetNumberOfBins(); iii++ ) {      
      hpoly1_ok->SetBinContent(iii+1, hpoly1->GetBinContent(iii+1));
    }
  }

  
  //cerr << "Max=" << htest->GetMaximum() << std::endl;

  //  hasicmax->Fill(maxHitCount);

  for(UInt_t i=0;i<NAsicFired;i++){
    AsicID = ListAsicID[i];
    fAnaLoop->Add(MINOS, Fec , AsicID+1, FEC_forMaxCharge[AsicID]);
    fAnaLoop->Add(MINOS, Xpos, AsicID+1, PosX_forMaxCharge[AsicID]);
    fAnaLoop->Add(MINOS, Ypos, AsicID+1, PosY_forMaxCharge[AsicID]);
    fAnaLoop->Add(MINOS, Qmax, AsicID+1, MaxCharge[AsicID]);
    fAnaLoop->Add(MINOS, Time, AsicID+1, AsicTime_forMaxCharge[AsicID]);
  }


  fAnalyzedMINOS->ReconstructData(); 

  Int_t myID = 101;
  if(fAnalyzedMINOS->GetNumAnalyzedMINOS()>10){
    //Tracks
    fTrackMINOS->ReconstructData();

  
    //Vertex
    //    fVertexMINOS->SetBetaConfig(Tgt_Length, Beta, Delta_Beta, Pos_offset);
    fVertexMINOS->ReconstructData(); // If SetBetaConfig() has been applied, will ReconstructVertex() && ReconstructBeta()
    fAnaLoop->Add(MINOS, Xvertex, myID, fVertexMINOS->GetXv());
    fAnaLoop->Add(MINOS, Yvertex, myID, fVertexMINOS->GetYv());
    fAnaLoop->Add(MINOS, Zvertex, myID, fVertexMINOS->GetZv());
    fAnaLoop->Add(MINOS, Theta,   myID, -1/*fVertexMINOS->GetThetaZ1()*/);
    fAnaLoop->Add(MINOS, Beta,    myID, -1/*fVertexMINOS->GetBetaVertex()*/);

    TClonesArray *fTrackMINOSArray=fTrackMINOS->GetTrackMINOSArray();
    if(fTrackMINOSArray){
      for(int nt=0;nt<fTrackMINOSArray->GetEntries();nt++){
	//	std::cout << nt << std::endl;
      }
    }
  }

  //  neve ++;



  //    fAnaLoop->Add(MINOS, ID,      id, id);
  //    fAnaLoop->Add(MINOS, Row,     id, row);
  //    fAnaLoop->Add(MINOS, Column,  id, column);
  //    fAnaLoop->Add(MINOS, RawADC,  id, adc);
  //    fAnaLoop->Add(MINOS, Energy,  id, ecal);

  return true;
}


Bool_t TAlEncMINOSExample::LoadParameters(const char *xmlfile)
{
  TArtCore::Info(__FILE__,"\e[33mLoad parameter from %s\e[0m", xmlfile);
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
void TAlEncMINOSExample::ParseParaList(TXMLNode *node)
{
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node
    if(strcmp(node->GetNodeName(), "MINOSthresh") == 0){
      MINOSthresh = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "TimeBinElec") == 0){
      TimeBinElec = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "VDrift") == 0){
      VDrift = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "Tshaping") == 0){
      Tshaping = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "DelayTrig") == 0){
      DelayTrig = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "Tgt_Length") == 0){
      Tgt_Length = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "Beta") == 0){
      Beta = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "Delta_Beta") == 0){
      Delta_Beta = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "Pos_offset") == 0){
      Delta_Beta = atof(node->GetText());
    } else if(strcmp(node->GetNodeName(), "DALIOffset") == 0){
      DALIOffset = atof(node->GetText());
    }

  }
}
/*
void TAlEncMINOSExample::instanceProj(TH2Poly *hxypol){

  double PI = TMath::Pi();
  double theta[256], r[18];
  int k=0;
  TGraph *g[4608];
  float anglepad=2.*PI/256.;
        
  for(int i=0; i<18; i++)    // loop on r (y direction in pad numbering)
    {
      for(int j=0; j<256; j++)  // loop on theta (x direction in pad numbering)
	{
	  r[i]=(45.2+(i+0.5)*2.1);
	  theta[j]=(j-31.5)*anglepad + 1.5*PI;
	  double ypoly[4]={(r[i]-1.05)*sin(theta[j]-anglepad/2.),(r[i]-1.05)*sin(theta[j]+anglepad/2.),(r[i]+1.05)*sin(theta[j]+anglepad/2.),(r[i]+1.05)*sin(theta[j]-anglepad/2.)};
	  double xpoly[4]={(r[i]-1.05)*cos(theta[j]-anglepad/2.),(r[i]-1.05)*cos(theta[j]+anglepad/2.),(r[i]+1.05)*cos(theta[j]+anglepad/2.),(r[i]+1.05)*cos(theta[j]-anglepad/2.)};
	  //	  g[k]=new TGraph(4,xpoly,ypoly);
	  //	  hxypol->AddBin(g[k]);
	  for(int ii=0;ii<4;ii++){
	    std::cout << xpoly[ii] << " " << ypoly[ii] << std::endl;
	  }
	  std::cout << std::endl;
	  hxypol->AddBin(4,xpoly,ypoly);
	  k++;
	}
        
    }

}*/
void TAlEncMINOSExample::instanceCst(TH2Poly *hxypol){

  double PI = TMath::Pi();
  double r,thetaperring;
  //  TGraph *g[3604]; 
  float anglepad;
  int start;
  int padsPring[18]={36,38,39,41,43,44,46,48,49,51,53,54,56,57,59,61,62,64};
  int k = 0;

  std::cout << hxypol << std::endl;
  
  for(int i=0; i<18; i++)    // loop on r (y direction in pad numbering)
    {
        
      r = (45.2+(i+0.5)*2.1); 
      anglepad = (2.*PI/(4*padsPring[i]));
      if((padsPring[i])%2==0) start = -(int)(padsPring[i]/2);
      else start = -(int)((padsPring[i]+1)/2);

      for(int j=0; j<(padsPring[i]*4); j++)  // loop on theta (x direction in pad numbering)
	{
	  thetaperring = ((j+start)*anglepad+3*PI/2.); //to be inizialized
	  double ypoly[4]={(r-1.05)*sin(thetaperring-anglepad/2.),(r-1.05)*sin(thetaperring+anglepad/2.),(r+1.05)*sin(thetaperring+anglepad/2.),(r+1.05)*sin(thetaperring-anglepad/2.)};
	  double xpoly[4]={(r-1.05)*cos(thetaperring-anglepad/2.),(r-1.05)*cos(thetaperring+anglepad/2.),(r+1.05)*cos(thetaperring+anglepad/2.),(r+1.05)*cos(thetaperring-anglepad/2.)};
//	  g[k]=new TGraph(4,xpoly,ypoly);
//	  g[k]->Print();
//	  hxypol->AddBin((TObject*)g[k]);
	  //	  for(int ii=0;ii<4;ii++){
	  //	    std::cout << xpoly[ii] << " " << ypoly[ii] << std::endl;
	  //	  }
	  //	  std::cout << std::endl;
	  hxypol->AddBin(4,xpoly,ypoly);
	  k++;
	}
        
    }

}

void TAlEncMINOSExample::Destruct()
{

  delete fMINOSParameters;  fMINOSParameters=0; 
  delete fCalibMINOS;	    fCalibMINOS=0;	   
  delete fAnalyzedMINOS;    fAnalyzedMINOS=0;   
  delete fTrackMINOS;	    fTrackMINOS=0;	   
  delete fVertexMINOS;      fVertexMINOS=0;     

}

const char* TAlEncMINOSExample::ClassName() const
{
  return "TAlEncMINOSExample";
}

TAlEncMINOSExampleFactory::TAlEncMINOSExampleFactory()
  : TAlEncFactory()
{;}

TAlEncMINOSExampleFactory::~TAlEncMINOSExampleFactory()
{;}

TAlEncSub* TAlEncMINOSExampleFactory::CreateEnc()
{
  return new TAlEncMINOSExample;
}

