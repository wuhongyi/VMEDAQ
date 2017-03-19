/*
 * show_module.cc, (compiled by Makefile)
 *
 *   written by T. Isobe
 */


void ShowModule()
//int main(int argc, char *argv[])
{

  gSystem->Load("libanacore.so");

  TArtEventStore *estore = new TArtEventStore();
  TArtRawEventObject *rawevent = estore->GetRawEventObject();
  estore->Open();

  TCanvas *c1 = new TCanvas("c1","ADC",700,500);
  c1->Draw();
  TH2F* h1 = new TH2F("module","module",50,-0.5,49.5,50,-0.5,49.5);
  h1->Draw();
  c1->Modified();
  c1->Update();
  int neve = 0;

  while(estore->GetNextEvent()){
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int detid = seg->GetDetector();
      int modid = seg->GetModule();
      h1->Fill(detid,modid);
    }

    rawevent->Clear();
    neve ++;

    if(neve%100==0){
      h1->Draw();
      c1->Update();
    }
  }
  
}
