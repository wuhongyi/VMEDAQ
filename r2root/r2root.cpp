// r2root.cpp --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 7月 10 21:12:35 2017 (+0800)
// Last-Updated: 一 9月  4 18:47:15 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 23
// URL: http://wuhongyi.cn 

#include "UserDefine.hh"
#include "r2root.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

r2root::r2root(TString rawfile,TString rootfile)
  : nevt(0)
{
  RawFile = rawfile;
  RootFile = rootfile;

  estore = new TArtEventStore();
  Benchmark = new TBenchmark;
}

r2root::~r2root()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Bool_t r2root::Open(const char *filename)
{
  if(estore == 0) estore = new TArtEventStore();
  bool flag = 0;
  flag = estore->Open(filename);
  rawevent = estore->GetRawEventObject();
  return flag;
}

Bool_t r2root::BuildEvent(Int_t maxnevt)
{
  Open(RawFile.Data());
  
  opfile = new TFile(RootFile.Data(),"RECREATE");
  tree = new TTree("tree","PKU VME Data");

  if(tree == NULL) return false;
  BranchTree();

  Benchmark->Start("tree");//计时开始

  while(estore->GetNextEvent())
    {
      Clear();
      for(int i = 0;i < rawevent->GetNumSeg();i++)
	{
	  seg = rawevent->GetSegment(i);//get segment
	  DecodeRaw();
	}
      tree->Fill();
      nevt++;
      //cout<<"nevt:    "<<nevt<<endl;
      if(nevt%10000 == 0) cout<<nevt<<endl;
      if(maxnevt > 0 && nevt >= maxnevt) break;
    }

  tree->Write();
  opfile->Close();
  
  Benchmark->Show("tree");//计时结束并输出时间
  return 1;
}

void r2root::DecodeRaw()
{
  bool flag;
  TArtRawDataObject *d;
  int geo;
  
  for(int j = 0;j < seg->GetNumData();j++)
    {
      d = seg->GetData(j);//get rawdata
      geo = d->GetGeo();
      //cout<<"geo: "<<geo<<endl;

      //scalar
#ifdef v830_r2root
#if v830num > 1
      flag = false;
      for (int i = 0; i < v830num; ++i)
	{
	  if(geo == v830geo[i])
	    {
	      int ch = d->GetCh();
	      Long64_t val = d->GetVal();
	      smod[shit] = geo;
	      sch[shit] = ch;
	      sval[shit] = val;
	      sdc[i][ch] = val;
	      shit++;
	      
	      flag = true;
	      break;
	    }
	}
      if(flag) continue;
#else
      if(geo == v830geo[0])
	{
	  int ch = d->GetCh();
	  Long64_t val = d->GetVal();
	  smod[shit] = geo;
	  sch[shit] = ch;
	  sval[shit] = val;
	  sdc[ch] = val;
	  shit++;

	  continue;
	}
#endif
#endif

      
      //adc
#ifdef v785_r2root
#if v785num > 1
      flag = false;
      for (int i = 0; i < v785num; ++i)
	{
	  if(geo == v785geo[i])
	    {
	      int ch = d->GetCh();
	      int val = d->GetVal();
	      if(ch > -1)
		{
		  amod[ahit] = geo;
		  ach[ahit] = ch;
		  aval[ahit] = val;
		  adc[i][ch] = val;
		  ahit++;
		}
	      else if(ch == -1)
		{
		  acnt[i] = d->GetEvtcnt();
		}

	      flag = true;
	      break;
	    }
	}
      if(flag) continue;
#else
      if(geo == v785geo[0])
	{
      	  int ch = d->GetCh();
      	  int val = d->GetVal();
      	  if(ch > -1)
      	    {
      	      amod[ahit] = geo;
      	      ach[ahit] = ch;
      	      aval[ahit] = val;
      	      adc[ch] = val;
      	      ahit++;
      	    }
      	  else if(ch == -1)
      	    {
      	      acnt = d->GetEvtcnt();
      	    }
	  
	  continue;
	}
#endif      
#endif

      
      //qdc
#ifdef v792_r2root      
#if v792num > 1
      flag = false;
      for (int i = 0; i < v792num; ++i)
      	{
      	  if(geo == v792geo[i])
      	    {
	      int ch = d->GetCh();
	      int val = d->GetVal();
	      if(ch > -1)
		{
		  qmod[qhit] = geo;
		  qch[qhit] = ch;
		  qval[qhit] = val;
		  qdc[i][ch] = val;
		  qhit++;
		}
	      else
		{
		  qcnt[i] = d->GetEvtcnt();
		}
	      
	      flag = true;
	      break;
      	    }
      	}
      if(flag) continue;
#else
      if(geo == v792geo[0])
      	{
	  int ch = d->GetCh();
	  int val = d->GetVal();
	  if(ch > -1)
	    {
	      qmod[qhit] = geo;
	      qch[qhit] = ch;
	      qval[qhit] = val;
	      qdc[ch] = val;
	      qhit++;
	    }
	  else
	    {
	      qcnt = d->GetEvtcnt();
	    }

	  continue;
      	}
#endif
#endif

      
      //tdc
#ifdef v775_r2root
#if v775num > 1
      flag = false;
      for (int i = 0; i < v775num; ++i)
	{
	  if(geo == v775geo[i])
	    {
	      int ch = d->GetCh();
	      int val = d->GetVal();
	      if(ch > -1)
		{
		  tmod[thit] = geo;
		  tch[thit] = ch;
		  tval[thit] = val;
		  tdc[i][ch] = val;
		  thit++;
		}
	      else
		{
		  tcnt[i] = d->GetEvtcnt();
		}
	      
	      flag = true;
      	      break;
	    }
	}
      if(flag) continue;
#else
      if(geo == v775geo[0])
	{
	  int ch = d->GetCh();
	  int val = d->GetVal();
	  if(ch > -1)
	    {
	      tmod[thit] = geo;
	      tch[thit] = ch;
	      tval[thit] = val;
	      tdc[ch] = val;
	      thit++;
	    }
	  else
	    {
	      tcnt = d->GetEvtcnt();
	    }

	  continue;
	}
#endif  
#endif

      
      //gdc
#ifdef v1x90_r2root
#if v1x90num > 1
      flag = false;
      for (int i = 0; i < v1x90num; ++i)
	{
	  if(geo == v1x90geo[i])
	    {
	      int ch = d->GetCh();
	      int val = d->GetVal();
	      int edge = d->GetEdge();//用来判断 前沿/尾沿
	      if(ch > -1)
		{
		  if(gmulti[i][ch] > v1x90multi)
		    {
		      cout<<"gmulti["<<i<<"]["<<ch<<"]"<<" is more than "<<v1x90multi<<endl; 
		      continue;
		    }
		  gmod[ghit] = geo;
		  gch[ghit] = ch;
		  gval[ghit] = val;
		  gedge[ghit] = edge;
		  gdc[i][ch][gmulti[i][ch]] = val;
		  gdcedge[i][ch][gmulti[i][ch]] = edge;
		  gmulti[i][ch]++;
		  ghit++;
		  if(ghit >= v1x90hitmax) 
		    {
		      cout<<"*************************************"<<endl;
		      cout<<"* Too Much hits for v1x90!!!"<<endl;
		      cout<<"* Pls modify const int v1x90hitmax"<<endl;
		      cout<<"*************************************"<<endl;
		    }
		}
	      else
		{
		  gcnt[i] = d->GetEvtcnt();
		}
	      
	      flag = true;
      	      break;
	    }
	}
      if(flag) continue;
#else
      if(geo == v1x90geo[0])
	{
	  int ch = d->GetCh();
	  int val = d->GetVal();
	  int edge = d->GetEdge();//用来判断 前沿/尾沿

	  if(ch > -1)
	    {
	      if(gmulti[ch] > v1x90multi)
		{
		  cout<<"gmulti["<<ch<<"]"<<" is more than "<<v1x90multi<<endl; 
		  continue;
		}
	      gmod[ghit] = geo;
	      gch[ghit] = ch;
	      gval[ghit] = val;
	      gedge[ghit] = edge;
	      gdc[ch][gmulti[ch]] = val;
	      gdcedge[ch][gmulti[ch]] = edge;
	      gmulti[ch]++;
	      ghit++;
	      if(ghit >= v1x90hitmax) 
		{
		  cout<<"*************************************"<<endl;
		  cout<<"* Too Much hits for v1x90!!!"<<endl;
		  cout<<"* Pls modify const int v1x90hitmax"<<endl;
		  cout<<"*************************************"<<endl;
		}
	    }
	  else
	    {
	      gcnt = d->GetEvtcnt();
	    }

	  continue;
	}
#endif
#endif

      
      //madc
#ifdef madc32_r2root
#if madc32num > 1
      flag = false;
      for (int i = 0; i < madc32num; ++i)
	{
	  if(geo == madc32geo[i])
	    {
	      int ch = d->GetCh();
	      int val = d->GetVal();
	      if(ch > -1)
		{
		  mmod[mhit] = geo;
		  mch[mhit] = ch;
		  mval[mhit] = val;
		  madc[i][ch] = val;
		  mhit++;
		}
	      else if(ch == -1)
		{
		  mcnt[i] = d->GetEvtcnt();
		}

	      flag = true;
      	      break;
	    }
	}
      if(flag) continue;
#else
      if(geo == madc32geo[0])
	{
	  int ch = d->GetCh();
	  int val = d->GetVal();
	  if(ch > -1)
	    {
	      mmod[mhit] = geo;
	      mch[mhit] = ch;
	      mval[mhit] = val;
	      madc[ch] = val;
	      mhit++;
	    }
	  else if(ch == -1)
	    {
	      mcnt = d->GetEvtcnt();
	    }
	  
	  continue;
	}
#endif  
#endif
      
    }
    
}
void r2root::Clear()
{
  //sca
#ifdef v830_r2root  
  shit = 0;
  memset(sdc,0,sizeof(sdc));
#endif

  //adc
#ifdef v785_r2root
  ahit = 0;
  memset(adc,0,sizeof(adc));
#if v785num > 1
  memset(acnt,0,sizeof(acnt));
#else
  acnt = 0;
#endif  
#endif

  //qdc
#ifdef v792_r2root
  qhit = 0;
  memset(qdc,0,sizeof(qdc));
#if v792num > 1
  memset(qcnt,0,sizeof(qcnt));
#else
  qcnt = 0;
#endif
#endif

  //tdc
#ifdef v775_r2root
  thit = 0;
  memset(tdc,0,sizeof(tdc));

#if v775num > 1
  memset(tcnt,0,sizeof(tcnt));
#else
  tcnt = 0;
#endif  
#endif

  //gdc
#ifdef v1x90_r2root
  ghit = 0;
  memset(gdc,0,sizeof(gdc));
  memset(gdcedge,0,sizeof(gdcedge));
  memset(gmulti,0,sizeof(gmulti));
#if v1x90num > 1
  memset(gcnt,0,sizeof(gcnt));
#else
  gcnt = 0;
#endif  
#endif

  //madc
#ifdef madc32_r2root
  mhit = 0;
  memset(madc,0,sizeof(madc));
#if madc32num > 1
  memset(mcnt,0,sizeof(mcnt));
#else
  mcnt = 0;
#endif 
#endif
}

Bool_t r2root::BranchTree()
{
  if(tree == NULL) return 0;

#ifdef v830_r2root //ok
  //Branches of scalar
#if v830num > 1
  tree->Branch("sdc",&sdc,TString::Format("sdc[%d][32]/l",v830num).Data());
#else
  tree->Branch("sdc",&sdc,"sdc[32]/l");
#endif
  tree->Branch("shit",&shit,"shit/I");
  tree->Branch("smod",&smod,"smod[shit]/I");
  tree->Branch("sch",&sch,"sch[shit]/I");
  tree->Branch("sval",&sval,"sval[shit]/l");
#endif


#ifdef v785_r2root //ok
  //Branches of adc
#if v785num > 1
  tree->Branch("adc",&adc,TString::Format("adc[%d][32]/I",v785num).Data());
  tree->Branch("acnt",&acnt,TString::Format("acnt[%d]/I",v785num).Data());
#else
  tree->Branch("adc",&adc,"adc[32]/I");
  tree->Branch("acnt",&acnt,"acnt/I");
#endif
  tree->Branch("ahit",&ahit,"ahit/I");
  tree->Branch("amod",&amod,"amod[ahit]/I");
  tree->Branch("ach",&ach,"ach[ahit]/I");
  tree->Branch("aval",&aval,"aval[ahit]/I");
#endif


#ifdef v792_r2root //ok
  //Branches of qdc
#if v792num > 1
  tree->Branch("qdc",&qdc,TString::Format("qdc[%d][32]/I",v792num).Data());
  tree->Branch("qcnt",&qcnt,TString::Format("qcnt[%d]/I",v792num).Data());
#else
  tree->Branch("qdc",&qdc,"qdc[32]/I");
  tree->Branch("qcnt",&qcnt,"qcnt/I");
#endif
  tree->Branch("qhit",&qhit,"qhit/I");
  tree->Branch("qmod",&qmod,"qmod[qhit]/I");
  tree->Branch("qch",&qch,"qch[qhit]/I");
  tree->Branch("qval",&qval,"qval[qhit]/I");
#endif


#ifdef v775_r2root //ok
  //Branches of tdc
#if v775num > 1
  tree->Branch("tdc",&tdc,TString::Format("tdc[%d][32]/I",v775num).Data());
  tree->Branch("tcnt",&tcnt,TString::Format("tcnt[%d]/I",v775num).Data());
#else
  tree->Branch("tdc",&tdc,"tdc[32]/I");
  tree->Branch("tcnt",&tcnt,"tcnt/I");
#endif    
  tree->Branch("thit",&thit,"thit/I");
  tree->Branch("tmod",&tmod,"tmod[thit]/I");
  tree->Branch("tch",&tch,"tch[thit]/I");
  tree->Branch("tval",&tval,"tval[thit]/I");
#endif


#ifdef v1x90_r2root //ok
  //Branches of gdc
#if v1x90num > 1
  tree->Branch("gdc",&gdc,TString::Format("gdc[%d][128][%d]/I",v1x90num,v1x90multi).Data());
  tree->Branch("gdcedge",&gdcedge,TString::Format("gdcedge[%d][128][%d]/I",v1x90num,v1x90multi).Data());
  tree->Branch("gmulti",&gmulti,TString::Format("gmulti[%d][128]/I",v1x90num).Data());
  tree->Branch("gcnt",&gcnt,TString::Format("gcnt[%d]/I",v1x90num).Data());
#else
  tree->Branch("gdc",&gdc,TString::Format("gdc[128][%d]/I",v1x90multi).Data());
  tree->Branch("gdcedge",&gdcedge,TString::Format("gdcedge[128][%d]/I",v1x90multi).Data());
  tree->Branch("gmulti",&gmulti,"gmulti[128]/I");
  tree->Branch("gcnt",&gcnt,"gcnt/I");
#endif
  tree->Branch("ghit",&ghit,"ghit/I");
  tree->Branch("gmod",&gmod,"gmod[ghit]/I");
  tree->Branch("gch",&gch,"gch[ghit]/I");
  tree->Branch("gval",&gval,"gval[ghit]/I");
  tree->Branch("gedge",&gedge,"gedge[ghit]/I");
#endif


#ifdef madc32_r2root //ok
  //Branches of madc
#if madc32num > 1
  tree->Branch("madc",&madc,TString::Format("madc[%d][32]/I",madc32num).Data());
  tree->Branch("mcnt",&mcnt,TString::Format("mcnt[%d]/I",madc32num).Data());
#else
  tree->Branch("madc",&madc,"madc[32]/I");
  tree->Branch("mcnt",&mcnt,"mcnt/I");
#endif  
  tree->Branch("mhit",&mhit,"mhit/I");
  tree->Branch("mmod",&mmod,"mmod[mhit]/I");
  tree->Branch("mch",&mch,"mch[mhit]/I");
  tree->Branch("mval",&mval,"mval[mhit]/I");
#endif


  //Branches of evt num
  tree->Branch("nevt",&nevt,"nevt/L");

  return 1;
}

// 
// r2root.cpp ends here
