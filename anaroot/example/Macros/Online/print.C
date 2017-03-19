{
  //  if(gROOT->GetListOfCanvases==NULL){
  //  TCanvas *c = new TCanvas();
  //  }
  for(int i=0;i<5;++i){
    hn();
    zone(3,2);hht(i*6+1);hn();hn();hn();hn();hn();
    //c->Update();
    update();
    //    gSystem->ProcessEvents();
    if(!wait()) break;
    print("temp.ps");
    gROOT->ProcessLine(".! lpr temp.ps");
    //c->WaitPrimitive();
  }
}
