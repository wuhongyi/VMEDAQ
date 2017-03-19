void Monitor(){

  clear();book(new TAlEncExample,"ana/shift.ana");push();start();
  gSystem->Sleep(500);

  TCanvas *cmon = new TCanvas("cmon","cmon",600,1000);
  zone(1,4);

  Int_t numeve = 0;
  TLatex tex; tex.SetTextSize(0.2);
  TAxis *ax, *ay;

  while(1){

    if(numeve%50 == 0){
      cout << "erase histograms" << endl; 
      erase();
      clear();book(new TAlEncExample,"ana/shift.ana");push();start();
      gSystem->Sleep(500);
    }
    gSystem->Sleep(5000);

    for(int i=0;i<4;i++){
      hn(); 
      ax = gH1->GetXaxis(); ay = gH1->GetYaxis();
      tex.DrawText(ax->GetXmin()+(ax->GetXmax()-ax->GetXmin())*0.2,
		 ay->GetXmin()+(ay->GetXmax()-ay->GetXmin())*0.8,
		 gH1->GetTitle());
    }

    cmon->Update();
    numeve ++;

  }


}
