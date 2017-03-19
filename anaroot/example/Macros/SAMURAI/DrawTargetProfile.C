void DrawTargetProfile(){


  TCanvas *cprof = new TCanvas("cprof","cprof",1200,800);
  cprof->Divide(2,2);
  cprof->cd(1);
  sm->Draw("tgta:tgtx>>h1(100,-100,100,100,-20,20)","","colz");

  h1->GetXaxis()->SetTitle("target-x (mm)");
  h1->GetYaxis()->SetTitle("target-a (mrad)");
  cprof->cd(2);
  sm->Draw("tgtb:tgty>>h2(100,-100,100,100,-20,20)","","colz");
  h2->GetXaxis()->SetTitle("target-y (mm)");
  h2->GetYaxis()->SetTitle("target-b (mrad)");

  cprof->cd(3);
  sm->Draw("tgtx>>h3(100,-100,100)");
  h3->GetXaxis()->SetTitle("target-x (mm)");
  h3->Fit("gaus");
  cprof->cd(4);
  sm->Draw("tgty>>h4(100,-100,100)");
  h4->GetXaxis()->SetTitle("target-y (mm)");
  h4->Fit("gaus");



}
