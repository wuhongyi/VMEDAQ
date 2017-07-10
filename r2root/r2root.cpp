#include "r2root.hh"

r2root::r2root() : nevt(0)
{
   estore=new TArtEventStore(); 
}

Bool_t r2root::Open(const char *filename)
{
   if(estore==0) estore=new TArtEventStore();
   bool flag=0;
   flag=estore->Open(filename);
   rawevent=estore->GetRawEventObject();
   return flag;
}

Bool_t r2root::BuildEvent(TTree *tree,Int_t maxnevt)
{
  if(tree==NULL) return false;
  BranchTree(tree);
  while(estore->GetNextEvent())
  {
    Clear();
    for(int i=0;i<rawevent->GetNumSeg();i++)
    {
      seg = rawevent->GetSegment(i);//get segment
      DecodeRaw();
    }
    tree->Fill();
    nevt++;
    //cout<<"nevt:    "<<nevt<<endl;
    if(nevt%10000==0) cout<<nevt<<endl;
    if(maxnevt>0&&nevt>=maxnevt) break;

  }
  return 1;
}

void r2root::DecodeRaw()
{
  for(int j=0;j<seg->GetNumData();j++)
    {
      TArtRawDataObject *d = seg->GetData(j);//get rawdata
      int geo = d->GetGeo();
      //cout<<"geo: "<<geo<<endl;
      
      if(geo==30)
	{
	  int ch = d->GetCh();
	  Long64_t val = d->GetVal();
	  smod[shit]=0;
	  sch[shit]=ch;
	  sval[shit]=val;
	  sdc[ch]=val;
	  shit++;
	}
      
      if(geo<=9&&geo>=0)
	{
	  int ch = d->GetCh();
	  int val = d->GetVal();
	  if(ch>-1)
	    {
	      amod[ahit]=geo;
	      ach[ahit]=ch;
	      aval[ahit]=val;
	      adc[geo][ch]=val;
	      ahit++;
	    }
	  else if(ch==-1)
	    {
	      acnt[geo]=d->GetEvtcnt();
	    }
	}
	   
      if(geo>=10&&geo<=20)
	{
	  int ch=d->GetCh();
	  int val=d->GetVal();
	  if(ch>-1)
	    {
	      mmod[mhit]=geo-10;
	      mch[mhit]=ch;
	      mval[mhit]=val;
	      madc[geo-10][ch]=val;
	      mhit++;
	    }
	  else if(ch==-1)
	    {
	      mcnt[geo-10]=d->GetEvtcnt();
	    }
	}
	    
      /*
	if(geo==31)
	{
	int ch = d->GetCh();
	int val = d->GetVal();
	if(ch>-1)
	{
	qmod[qhit]=geo;
	qch[qhit]=ch;
	qval[qhit]=val;
	qdc[ch]=val;
	qhit++;
	}
	else qcnt=d->GetEvtcnt();
	}
      */
      if(geo==20)
	{
	  int ch = d->GetCh();
	  int val = d->GetVal();
	  if(ch>-1)
	    {
	      if(gmulti[0][ch]>5)
		{
		  cout<<"gmulti 0 ch:"<<ch<<" is more than 20"<<endl; 
		  continue;
		}
	      gmod[ghit]=0;
	      gch[ghit]=ch;
	      gval[ghit]=val;
	      gdc[0][ch][gmulti[0][ch]]=val;
	      gmulti[0][ch]++;
	      ghit++;
	      if(ghit>=ghitmax) 
		{
		  cout<<"*************************************"<<endl;
		  cout<<"* Too Much hits for v1190!!!"<<endl;
		  cout<<"* Pls modify const int ghitmax"<<endl;
		  cout<<"*************************************"<<endl;
		}
	    }
	  else gcnt[0]=d->GetEvtcnt();
	}

      // if(geo==21)
      // 	{
      // 	  int ch = d->GetCh();
      // 	  int val = d->GetVal();
      // 	  if(ch>-1)
      // 	    {
      // 	      if(gmulti[1][ch]>5) {cout<<"gmulti 1 ch:"<<ch<<" is more than 5"<<endl; continue;}
      // 	      gmod[ghit]=1;
      // 	      gch[ghit]=ch;
      // 	      gval[ghit]=val;
      // 	      gdc[1][ch][gmulti[1][ch]]=val;
      // 	      gmulti[1][ch]++;
      // 	      ghit++;
      // 	      if(ghit>=ghitmax) 
      // 		{
      // 		  cout<<"*************************************"<<endl;
      // 		  cout<<"* Too Much hits for v1190!!!"<<endl;
      // 		  cout<<"* Pls modify const int ghitmax"<<endl;
      // 		  cout<<"*************************************"<<endl;
      // 		}
      // 	    }
      // 	  else gcnt[1]=d->GetEvtcnt();
      // 	}
	    
    }
    
}
void r2root::Clear()
{
    ahit=0;
    mhit=0;
    ghit=0;
    shit=0;
    
    //adc
    memset(adc,0,sizeof(adc));
    memset(acnt,0,sizeof(acnt));

    //madc
    memset(madc,0,sizeof(madc));
    memset(mcnt,0,sizeof(mcnt));

    //gdc
    memset(gdc,0,sizeof(gdc));
    memset(gcnt,0,sizeof(gcnt));
    memset(gmulti,0,sizeof(gmulti));

    //qdc
    //memset(qdc,0,sizeof(qdc));

    //sca
    memset(sdc,0,sizeof(sdc));

}

Bool_t r2root::BranchTree(TTree *tree)
{
  if(tree==NULL) return 0;
    //Branches of adc
  tree->Branch("ahit",&ahit,"ahit/I");
  tree->Branch("amod",&amod,"amod[ahit]/I");
  tree->Branch("ach",&ach,"ach[ahit]/I");
  tree->Branch("aval",&aval,"aval[ahit]/I");
  tree->Branch("adc",&adc,TString::Format("adc[%d][32]/I",adcnum).Data());
  tree->Branch("acnt",&acnt,TString::Format("acnt[%d]/I",adcnum).Data());

    //Branches of madc
  
  tree->Branch("mhit",&mhit,"mhit/I");
  tree->Branch("mmod",&mmod,"mmod[mhit]/I");
  tree->Branch("mch",&mch,"mch[mhit]/I");
  tree->Branch("mval",&mval,"mval[mhit]/I");
  tree->Branch("madc",&madc,TString::Format("madc[%d][32]/I",madcnum).Data());
  tree->Branch("mcnt",&mcnt,TString::Format("mcnt[%d]/I",madcnum).Data());
  
/*
  //Branches of qdc
  tree->Branch("qhit",&qhit,"qhit/I");
  tree->Branch("qmod",&qmod,"qmod[qhit]/I");
  tree->Branch("qch",&qch,"qch[qhit]/I");
  tree->Branch("qval",&qval,"qval[qhit]/I");
  tree->Branch("qdc",&qdc,"qdc[32]/I");
  tree->Branch("qcnt",&qcnt,"qcnt/I");
*/

  //Branches of gdc
  tree->Branch("ghit",&ghit,"ghit/I");
  tree->Branch("gmod",&gmod,"gmod[ghit]/I");
  tree->Branch("gch",&gch,"gch[ghit]/I");
  tree->Branch("gval",&gval,"gval[ghit]/I");
  tree->Branch("gdc",&gdc,"gdc[2][128][5]/I");
  tree->Branch("gmulti",&gmulti,"gmulti[2][128]/I");
  tree->Branch("gcnt",&gcnt,"gcnt[2]/I");

  //Branches of scalar
  tree->Branch("shit",&shit,"shit/I");
  tree->Branch("smod",&smod,"smod[shit]/I");
  tree->Branch("sch",&sch,"sch[shit]/I");
  tree->Branch("sval",&sval,"sval[shit]/l");
  tree->Branch("sdc",&sdc,"sdc[32]/l");

  //Branches of evt num
  tree->Branch("nevt",&nevt,"nevt/L");

  return 1;
}
