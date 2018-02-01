// pedo.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月  8 19:52:09 2017 (+0800)
// Last-Updated: 四 2月  1 16:36:30 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 44
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
  tree = NULL;
  file = NULL;

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
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  delete adc_data[i][j];
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      delete adc_data[j];
    }
#endif
#endif  

      
#ifdef v792_pedo 
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  delete qdc_data[i][j];
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      delete qdc_data[j];
    }
#endif
#endif  
  

#ifdef madc32_pedo 
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  delete madc_data[i][j];
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      delete madc_data[j];
    }
#endif
#endif  
  file->Close();
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
  RecordHistogram();
  
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
      sprintf(histoname,"madc32_ch%02d",j);
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
      for (int i = 0; i < v785num; ++i)
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
  double left,right;
  
#ifdef v785_pedo 
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  // if(adc_data[i][j]->Fit("gaus","QL","",adc_data[i][j]->GetBinCenter(adc_data[i][j]->GetMaximumBin())-v785fitrange,adc_data[i][j]->GetBinCenter(adc_data[i][j]->GetMaximumBin())+v785fitrange) != 0)
	  GetRangeAbove(adc_data[i][j],v785fitrangeabove,left,right);
	  if(adc_data[i][j]->Fit("gaus","QL","",left,right) != 0)
	    {
	      cout<<"ADC  mod:"<<i<<" ch:"<<j<<"Fit error!!!"<<endl;
	    }
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      // if(adc_data[j]->Fit("gaus","QL","",adc_data[j]->GetBinCenter(adc_data[j]->GetMaximumBin())-v785fitrange,adc_data[j]->GetBinCenter(adc_data[j]->GetMaximumBin())+v785fitrange) != 0)
      GetRangeAbove(adc_data[j],v785fitrangeabove,left,right);
      if(adc_data[j]->Fit("gaus","QL","",left,right) != 0)
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
	  // if(qdc_data[i][j]->Fit("gaus","QL","",qdc_data[i][j]->GetBinCenter(qdc_data[i][j]->GetMaximumBin())-v792fitrange,qdc_data[i][j]->GetBinCenter(qdc_data[i][j]->GetMaximumBin())+v792fitrange) != 0)
	  GetRangeAbove(qdc_data[i][j],v792fitrangeabove,left,right);
	  if(qdc_data[i][j]->Fit("gaus","QL","",left,right) != 0)
	    {
	      cout<<"QDC  mod:"<<i<<" ch:"<<j<<"Fit error!!!"<<endl;
	    }
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      // if(qdc_data[j]->Fit("gaus","QL","",qdc_data[j]->GetBinCenter(qdc_data[j]->GetMaximumBin())-v792fitrange,qdc_data[j]->GetBinCenter(qdc_data[j]->GetMaximumBin())+v792fitrange) != 0)
      GetRangeAbove(qdc_data[j],v792fitrangeabove,left,right);
      if(qdc_data[j]->Fit("gaus","QL","",left,right) != 0)
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
	  // if(madc_data[i][j]->Fit("gaus","QL","",madc_data[i][j]->GetBinCenter(madc_data[i][j]->GetMaximumBin())-madc32fitrange,madc_data[i][j]->GetBinCenter(madc_data[i][j]->GetMaximumBin())+madc32fitrange) != 0)
	  GetRangeAbove(madc_data[i][j],madc32fitrangeabove,left,right);
	  if(madc_data[i][j]->Fit("gaus","QL","",left,right) != 0)
	    {
	      cout<<"MADC  mod:"<<i<<" ch:"<<j<<"Fit error!!!"<<endl;
	    }
	}
    }
#else
      for(int j = 0;j < 32; j++)
	{
	  // if(madc_data[j]->Fit("gaus","QL","",madc_data[j]->GetBinCenter(madc_data[j]->GetMaximumBin())-madc32fitrange,madc_data[j]->GetBinCenter(madc_data[j]->GetMaximumBin())+madc32fitrange) != 0)
	  GetRangeAbove(madc_data[j],madc32fitrangeabove,left,right);
	  if(madc_data[j]->Fit("gaus","QL","",left,right) != 0)
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
  TCanvas *c1 = new TCanvas("c1","",600,400);
  c1->cd();
  c1->Print("save.pdf[");
  
#ifdef v785_pedo 
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  adc_data[i][j]->Draw();
	  c1->Print("save.pdf");
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      adc_data[j]->Draw();
      c1->Print("save.pdf");
    }
#endif
#endif  

#ifdef v792_pedo 
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  qdc_data[i][j]->Draw();
	  c1->Print("save.pdf");
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      qdc_data[j]->Draw();
      c1->Print("save.pdf");
    }
#endif
#endif  
  
#ifdef madc32_pedo 
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  madc_data[i][j]->Draw();
	  c1->Print("save.pdf");
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      madc_data[j]->Draw();
      c1->Print("save.pdf");
    }
#endif
#endif
  c1->Print("save.pdf]");
  delete c1;
}
  

void pedo::RecordText()
{
  std::ofstream writetxt;//fstream
  writetxt.open("save.txt");//ios::bin ios::app
  if(!writetxt.is_open())
    {
      std::cout<<"can't open file save.txt."<<std::endl;
    }
  writetxt<<"mod    ch    mean    sigma"<<endl<<endl;
#ifdef v785_pedo
  writetxt<<"###### ADC ######"<<endl;
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  writetxt<<i<<"  "<<j<<"  "<<adc_data[i][j]->GetFunction("gaus")->GetParameter(1)<<"  "<<adc_data[i][j]->GetFunction("gaus")->GetParameter(2)<<endl;
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      writetxt<<"0"<<"  "<<j<<"  "<<adc_data[j]->GetFunction("gaus")->GetParameter(1)<<"  "<<adc_data[j]->GetFunction("gaus")->GetParameter(2)<<endl;
    }
#endif
  writetxt<<endl<<endl;
#endif  

#ifdef v792_pedo
  writetxt<<"###### QDC ######"<<endl;
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  writetxt<<i<<"  "<<j<<"  "<<qdc_data[i][j]->GetFunction("gaus")->GetParameter(1)<<"  "<<qdc_data[i][j]->GetFunction("gaus")->GetParameter(2)<<endl;
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      writetxt<<"0"<<"  "<<j<<"  "<<qdc_data[j]->GetFunction("gaus")->GetParameter(1)<<"  "<<qdc_data[j]->GetFunction("gaus")->GetParameter(2)<<endl;
    }
#endif
  writetxt<<endl<<endl;
#endif  
  
#ifdef madc32_pedo
   writetxt<<"###### MADC ######"<<endl;
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  writetxt<<i<<"  "<<j<<"  "<<madc_data[i][j]->GetFunction("gaus")->GetParameter(1)<<"  "<<madc_data[i][j]->GetFunction("gaus")->GetParameter(2)<<endl;
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      writetxt<<"0"<<"  "<<j<<"  "<<madc_data[j]->GetFunction("gaus")->GetParameter(1)<<"  "<<madc_data[j]->GetFunction("gaus")->GetParameter(2)<<endl;
    }
#endif
  writetxt<<endl<<endl;
#endif  
  writetxt.close();
}

void pedo::RecordHistogram()
{
  TFile *rootfile = new TFile("save.root","RECREATE");//"RECREATE" "READ"
  if(!rootfile->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
    }
  rootfile->cd();
  
#ifdef v785_pedo 
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  adc_data[i][j]->Write();
	}
    }
#else
    for(int j = 0;j < 32; j++)
      {
	adc_data[j]->Write();
      }
#endif
#endif  

#ifdef v792_pedo 
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  qdc_data[i][j]->Write();
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      qdc_data[j]->Write();
    }
#endif
#endif  
  
#ifdef madc32_pedo 
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      for(int j = 0;j < 32; j++)
	{
	  madc_data[i][j]->Write();
	}
    }
#else
  for(int j = 0;j < 32; j++)
    {
      madc_data[j]->Write();
    }
#endif
#endif  
  // rootfile->ls("");
  // TObject->Write();
  // TH1D *h = (TH1D*)rootfile->Get("name");
  rootfile->Write();
  rootfile->Close();
  
}


void pedo::GetRangeAbove(TH1 *h,double thre,double &left,double &right)
{
  double maxbincontent = h->GetBinContent(h->GetMaximumBin());
  left = h->GetBinCenter(h->FindFirstBinAbove(thre*maxbincontent));
  right = h->GetBinCenter(h->FindLastBinAbove(thre*maxbincontent));
  // std::cout<<"Fit Range: "<<left<<"  "<<right<<std::endl;
}


// 
// pedo.cc ends here
