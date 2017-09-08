// pedo.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月  8 19:52:09 2017 (+0800)
// Last-Updated: 五 9月  8 23:19:14 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 24
// URL: http://wuhongyi.cn 

#include "pedo.hh"
#include <iostream>
#include <fstream>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pedo::pedo(TString rootfile)
{
  RootFile = rootfile;
  Benchmark = new TBenchmark;


#ifdef v785_pedo 
#if v785num > 1
  
#else

#endif

#endif  



#ifdef v792_pedo 
#if v792num > 1

#else

#endif

#endif  
  

#ifdef madc32_pedo 
#if madc32num > 1

#else

#endif

#endif  
  
}

pedo::~pedo()
{
#ifdef v785_pedo 
#if v785num > 1

#else

#endif

#endif  



#ifdef v792_pedo 
#if v792num > 1

#else

#endif

#endif  
  

#ifdef madc32_pedo 
#if madc32num > 1

#else

#endif

#endif  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pedo::Process()
{
  Benchmark->Start("tree");//计时开始
  
  if(!Init(RootFile)) return;
  cout<<"Start Fill Histogram"<<endl;
  FillHistogram();
  cout<<"Start Fit Histogram"<<endl;
  FitHistogram();
  cout<<"Start Record"<<endl;
  Record();
  RecordText();
  RecordPicture();
  RecordHisto();
  
#ifdef v785_pedo 
#if v785num > 1

#else

#endif

#endif  



#ifdef v792_pedo 
#if v792num > 1

#else

#endif

#endif  
  

#ifdef madc32_pedo 
#if madc32num > 1

#else

#endif

#endif  

  Benchmark->Show("tree");//计时结束并输出时间
}

Bool_t pedo::Init(const char * filename)
{
  // open root file
  file = new TFile(filename,"READ");
  if(!file || !file->IsOpen()) 
    {
      cout<<"ERROR: cannot open file: "<<filename<<endl;
      return 0;
    }
  cout<<"The data file is: "<<filename<<endl;
  tree = (TTree *)file->Get("tree");
  if(tree == NULL) 
    {
      cout<<"cannot find the tree, pls check the tree's name!"<<endl;
      return 0;
    }

  // Create Histogram
  char histoname[64];
#ifdef v785_pedo 
  tree->SetBranchAddress("adc",&adc);
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  sprintf(histoname,"adc_mod%02d_ch%02d",i,j);
	  adc_data[i][j] = new TH1D(histoname,"",v785binmax-v785binmin,v785binmin,v785binmax);
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      sprintf(histoname,"adc_ch%02d",j);
      adc_data[j] = new TH1D(histoname,"",v785binmax-v785binmin,v785binmin,v785binmax);
    }
#endif
#endif  
  
#ifdef v792_pedo
  tree->SetBranchAddress("qdc",&qdc);
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  sprintf(histoname,"qdc_mod%02d_ch%02d",i,j);
	  qdc_data[i][j] = new TH1D(histoname,"",v792binmax-v792binmin,v792binmin,v792binmax);
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      sprintf(histoname,"qdc_ch%02d",j);
      qdc_data[j] = new TH1D(histoname,"",v792binmax-v792binmin,v792binmin,v792binmax);
    }
#endif  
#endif    

#ifdef madc32_pedo
  tree->SetBranchAddress("madc",&madc);
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  sprintf(histoname,"madc32_mod%02d_ch%02d",i,j);
	  madc_data[i][j] = new TH1D(histoname,"",madc32binmax-madc32binmin,madc32binmin,madc32binmax);
	}

    }
#else
  for(int j = 0;j < 32; j++)
    {
      sprintf(histoname,"madc32_mod%02d_ch%02d",i,j);
      madc_data[j] = new TH1D(histoname,"",madc32binmax-madc32binmin,madc32binmin,madc32binmax);
    }
#endif
#endif
 
  return 1;
}
 
 
void pedo::FillHistogram()
{
  for(int ii = 0; ii < tree->GetEntriesFast(); ii++)
    {
      tree->GetEntry(ii);

#ifdef v785_pedo 
#if v785num > 1
      for (int i = 0; i < v792num; ++i)
	{
	  for(int j = 0;j < 32; j++)
	    {
	      adc_data[i][j]->Fill(adc[i][j]);
	    }
	}
#else
      for(int j = 0;j < 32; j++)
	{
	  adc_data[j]->Fill(adc[j]);
	}
#endif
#endif  

      
#ifdef v792_pedo 
#if v792num > 1
      for (int i = 0; i < v792num; ++i)
	{
	  for(int j = 0;j < 32; j++)
	    {
	      qdc_data[i][j]->Fill(qdc[i][j]);
	    }
	}
#else
      for(int j = 0;j < 32; j++)
	{
	  qdc_data[j]->Fill(qdc[j]);
	}
#endif
#endif  

      
#ifdef madc32_pedo 
#if madc32num > 1
      for (int i = 0; i < madc32num; ++i)
	{
	  for(int j = 0;j < 32; j++)
	    {
	      madc_data[i][j]->Fill(madc[i][j]);
	    }
	}
#else
      for(int j = 0;j < 32; j++)
	{
	  madc_data[j]->Fill(madc[j]);
	}
#endif
#endif 
    }
}

void pedo::FitHistogram()
{
#ifdef v785_pedo 
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  if(adc_data[i][j]->Fit("gaus","QL","",adc_data[i][j]->GetMaximumBin()-v785fitrange,adc_data[i][j]->GetMaximumBin()+v785fitrange) != 0)
	    {
	      cout<<"ADC  mod:"<<i<<" ch:"<<j<<"Fit error!!!"<<endl;
	    }
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      if(adc_data[j]->Fit("gaus","QL","",adc_data[j]->GetMaximumBin()-v785fitrange,adc_data[j]->GetMaximumBin()+v785fitrange) != 0)
	{
	  cout<<"ADC  mod:0"<<" ch:"<<j<<"Fit error!!!"<<endl;
	}
    }
#endif
#endif  

#ifdef v792_pedo 
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  if(qdc_data[i][j]->Fit("gaus","QL","",qdc_data[i][j]->GetMaximumBin()-v792fitrange,qdc_data[i][j]->GetMaximumBin()+v792fitrange) != 0)
	    {
	      cout<<"QDC  mod:"<<i<<" ch:"<<j<<"Fit error!!!"<<endl;
	    }
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      if(j == 0) qdc_data[j]->Print();
      if(qdc_data[j]->Fit("gaus","QL","",qdc_data[j]->GetMaximumBin()-v792fitrange,qdc_data[j]->GetMaximumBin()+v792fitrange) != 0)
	{
	  cout<<"QDC  mod:0"<<" ch:"<<j<<"Fit error!!!"<<endl;
	}
    }
#endif
#endif  
  
#ifdef madc32_pedo 
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  if(madc_data[i][j]->Fit("gaus","QL","",madc_data[i][j]->GetMaximumBin()-madc32fitrange,madc_data[i][j]->GetMaximumBin()+madc32fitrange) != 0)
	    {
	      cout<<"MADC  mod:"<<i<<" ch:"<<j<<"Fit error!!!"<<endl;
	    }
	}
    }
#else
      for(int j = 0;j < 32; j++)
	{
	  if(madc_data[j]->Fit("gaus","QL","",madc_data[j]->GetMaximumBin()-madc32fitrange,madc_data[j]->GetMaximumBin()+madc32fitrange) != 0)
	    {
	      cout<<"MADC  mod:0"<<" ch:"<<j<<"Fit error!!!"<<endl;
	    }
	}
#endif
#endif    
}

void pedo::Record()
{
  std::ofstream writesh;//fstream
  char shellfilename[64];
   
#ifdef v785_pedo 
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      sprintf(shellfilename,"v7xx_%d_pedo.sh",i);
      writesh.open(shellfilename);//ios::bin ios::app
      if(!writesh.is_open())
	{
	  std::cout<<"can't open file: "<<shellfilename<<std::endl;
	}
      writesh<<"#!/bin/sh"<<endl;
      writesh<<"cmd=\"../cmdvme/cmdvme\""<<endl;
      writesh<<"BASEADDR=0x1"<<setw(3)<<setfill('0')<<hex<<i<<endl;
      writesh<<endl<<endl;
      writesh<<"####################################"<<endl<<endl;
      writesh<<"# 以下设置阈值 0x00 - 0xFF"<<endl;
      if(v785D2D16 == 2)
	{
	  writesh<<"# 当前配置下，该参数为真实阈值除以2"<<endl;
	}
      else
	{
	  writesh<<"# 当前配置下，该参数为真实阈值除以16"<<endl;
	}
      for(int j = 0;j < 32; j++)
	{
	  writesh<<"${cmd} -ww ${BASEADDR}"<<hex<<j*2+4224<<"\t"<<"0x"<<int((adc_data[i][j]->GetFunction("gaus")->GetParameter(1)+adc_data[i][j]->GetFunction("gaus")->GetParameter(2)*v785msigma)/v785D2D16+1)<<endl;
	}
      writesh.close();
    }
#else
  sprintf(shellfilename,"v7xx_0_pedo.sh");
  writesh.open(shellfilename);//ios::bin ios::app
  if(!writesh.is_open())
    {
      std::cout<<"can't open file: "<<shellfilename<<std::endl;
    }
  writesh<<"#!/bin/sh"<<endl;
  writesh<<"cmd=\"../cmdvme/cmdvme\""<<endl;
  writesh<<"BASEADDR=0x1000"<<endl;
  writesh<<endl<<endl;
  writesh<<"####################################"<<endl<<endl;
  writesh<<"# 以下设置阈值 0x00 - 0xFF"<<endl;
  if(v785D2D16 == 2)
    {
      writesh<<"# 当前配置下，该参数为真实阈值除以2"<<endl;
    }
  else
    {
      writesh<<"# 当前配置下，该参数为真实阈值除以16"<<endl;
    }
  for(int j = 0;j < 32; j++)
    {
      writesh<<"${cmd} -ww ${BASEADDR}"<<hex<<j*2+4224<<"\t"<<"0x"<<int((adc_data[j]->GetFunction("gaus")->GetParameter(1)+adc_data[j]->GetFunction("gaus")->GetParameter(2)*v785msigma)/v785D2D16+1)<<endl;
    }
  writesh.close();
#endif
#endif  

#ifdef v792_pedo 
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
#ifdef v785_pedo
      sprintf(shellfilename,"v7xx_%d_pedo.sh",i+v785num);
#else
      sprintf(shellfilename,"v7xx_%d_pedo.sh",i);
#endif  
      writesh.open(shellfilename);//ios::bin ios::app
      if(!writesh.is_open())
	{
	  std::cout<<"can't open file: "<<shellfilename<<std::endl;
	}
      writesh<<"#!/bin/sh"<<endl;
      writesh<<"cmd=\"../cmdvme/cmdvme\""<<endl;
#ifdef v785_pedo
      writesh<<"BASEADDR=0x1"<<setw(3)<<setfill('0')<<hex<<i+v785num<<endl;
#else
      writesh<<"BASEADDR=0x1"<<setw(3)<<setfill('0')<<hex<<i<<endl;
#endif  
      writesh<<endl<<endl;
      writesh<<"####################################"<<endl<<endl;
      writesh<<"# 以下设置阈值 0x00 - 0xFF"<<endl;
      if(v785D2D16 == 2)
	{
	  writesh<<"# 当前配置下，该参数为真实阈值除以2"<<endl;
	}
      else
	{
	  writesh<<"# 当前配置下，该参数为真实阈值除以16"<<endl;
	}
      for(int j = 0;j < 32; j++)
	{
	  cout<<j<<"  "<<int((qdc_data[i][j]->GetFunction("gaus")->GetParameter(1)+qdc_data[i][j]->GetFunction("gaus")->GetParameter(2)*v792msigma)/v792D2D16+1)<<endl;
	  writesh<<"${cmd} -ww ${BASEADDR}"<<hex<<j*2+4224<<"\t"<<"0x"<<int((qdc_data[i][j]->GetFunction("gaus")->GetParameter(1)+qdc_data[i][j]->GetFunction("gaus")->GetParameter(2)*v792msigma)/v792D2D16+1)<<endl;
	}
      writesh.close();      
    }
#else
#ifdef v785_pedo
      sprintf(shellfilename,"v7xx_%d_pedo.sh",v785num);
#else
      sprintf(shellfilename,"v7xx_0_pedo.sh");
#endif 
  writesh.open(shellfilename);//ios::bin ios::app
  if(!writesh.is_open())
    {
      std::cout<<"can't open file: "<<shellfilename<<std::endl;
    }
  writesh<<"#!/bin/sh"<<endl;
  writesh<<"cmd=\"../cmdvme/cmdvme\""<<endl;
#ifdef v785_pedo
  writesh<<"BASEADDR=0x1"<<setw(3)<<setfill('0')<<hex<<v785num<<endl;
#else
  writesh<<"BASEADDR=0x1000"<<endl;
#endif  
  writesh<<endl<<endl;
  writesh<<"####################################"<<endl<<endl;
  writesh<<"# 以下设置阈值 0x00 - 0xFF"<<endl;
  if(v785D2D16 == 2)
    {
      writesh<<"# 当前配置下，该参数为真实阈值除以2"<<endl;
    }
  else
    {
      writesh<<"# 当前配置下，该参数为真实阈值除以16"<<endl;
    }
  for(int j = 0;j < 32; j++)
    {
      cout<<j<<"  "<<int((qdc_data[j]->GetFunction("gaus")->GetParameter(1)+qdc_data[j]->GetFunction("gaus")->GetParameter(2)*v792msigma)/v792D2D16+1)<<endl;
      writesh<<"${cmd} -ww ${BASEADDR}"<<hex<<j*2+4224<<"\t"<<"0x"<<int((qdc_data[j]->GetFunction("gaus")->GetParameter(1)+qdc_data[j]->GetFunction("gaus")->GetParameter(2)*v792msigma)/v792D2D16+1)<<endl;
    }
  writesh.close();

  
#endif
#endif  
  
#ifdef madc32_pedo 
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      sprintf(shellfilename,"madc_%d_pedo.sh",i);
      writesh.open(shellfilename);//ios::bin ios::app
      if(!writesh.is_open())
	{
	  std::cout<<"can't open file: "<<shellfilename<<std::endl;
	}
      writesh<<"#!/bin/sh"<<endl;
      writesh<<"cmd=\"../cmdvme/cmdvme\""<<endl;
      writesh<<"BASEADDR=0x2"<<setw(3)<<setfill('0')<<hex<<i<<endl;
      writesh<<endl<<endl;
      writesh<<"####################################"<<endl<<endl;
      for(int j = 0;j < 32; j++)
	{
	  writesh<<"${cmd} -ww ${BASEADDR}"<<hex<<j*2+16384<<"\t"<<"0x"<<int(madc_data[i][j]->GetFunction("gaus")->GetParameter(1)+madc_data[i][j]->GetFunction("gaus")->GetParameter(2)*madc32msigma+1)<<endl;
	}
      writesh.close();
    }
#else
  sprintf(shellfilename,"madc_0_pedo.sh");
  writesh.open(shellfilename);//ios::bin ios::app
  if(!writesh.is_open())
    {
      std::cout<<"can't open file: "<<shellfilename<<std::endl;
    }
  writesh<<"#!/bin/sh"<<endl;
  writesh<<"cmd=\"../cmdvme/cmdvme\""<<endl;
  writesh<<"BASEADDR=0x2000"<<endl;
  writesh<<endl<<endl;
  writesh<<"####################################"<<endl<<endl;
  for(int j = 0;j < 32; j++)
    {
      writesh<<"${cmd} -ww ${BASEADDR}"<<hex<<j*2+16384<<"\t"<<"0x"<<int(madc_data[j]->GetFunction("gaus")->GetParameter(1)+madc_data[j]->GetFunction("gaus")->GetParameter(2)*madc32msigma+1)<<endl;
    }
  writesh.close();
#endif
#endif  
}

void pedo::RecordPicture()
{
#ifdef v785_pedo 
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{

	}
    }
#else

#endif
#endif  

#ifdef v792_pedo 
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{

	}
    }
#else

#endif
#endif  
  
#ifdef madc32_pedo 
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{

	}
    }
#else

#endif
#endif  
}
  
void pedo::RecordHisto()
{
#ifdef v785_pedo 
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{

	}
    }
#else

#endif
#endif  

#ifdef v792_pedo 
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{

	}
    }
#else

#endif
#endif  
  
#ifdef madc32_pedo 
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{

	}
    }
#else

#endif
#endif  
}

void pedo::RecordText()
{


}


// 
// pedo.cc ends here
