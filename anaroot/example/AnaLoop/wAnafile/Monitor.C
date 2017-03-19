void Monitor(char * anafile="samurai.ana"){

  Int_t interval_nevent = 20;

  clear();
  book(anafile);
  push();
  start();
  gSystem->Sleep(500);

  TCanvas *cmon = new TCanvas("cmon","cmon",1800,1800);
  int x=6;
  int y=6;

  TLatex tex; tex.SetTextSize(0.2);
  TAxis *ax, *ay;

  Int_t numeve = 0;
  while(1){
    zone(x,y);
    for(int i=0;i<x*y;i++){
      if(i==0){
	hht(i+1); 
      }else{
	ht(i+1); 
      }
      ax = gH1->GetXaxis(); ay = gH1->GetYaxis();
      tex.DrawText(ax->GetXmin()+(ax->GetXmax()-ax->GetXmin())*0.2,
		 ay->GetXmin()+(ay->GetXmax()-ay->GetXmin())*0.8,
		 gH1->GetTitle());
    }
    cmon->Update();

    ++numeve;
    if(numeve%interval_nevent == 0){
      cout << "erase histograms" << endl; 
      erase();
      numeve = 0;
      clear();
      book(anafile);
      push();
      start();
      gSystem->Sleep(500);
    }
    start();
    gSystem->Sleep(2000);
    stop();
  }
}
