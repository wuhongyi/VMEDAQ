// This macro setup.C is called in TArtAnalysis constructor
// These settings are used in batch mode.
// When started from GUI, these settings are default, but can be overwritten.

void setup()
{

  TGo4AnalysisStep        * step;
  TGo4StepFactory         * fact;
  TGo4FileStoreParameter  * store;

  Text_t autosave[]="result";   // name of autosave file (.root)
  Text_t outfile[]="tree";      // name of output file (.root)
  Text_t infile[]="tree";       // name of input file (previous output)
  Text_t node[]="nodename";     // name of MBS or event server node
  Int_t  port=-10;                // depends on the remote event server

  go4->SetAutoSaveFile(autosave); // optional
  go4->SetAutoSaveInterval(0);    // after n seconds, 0 = at end of event loop
  go4->SetAutoSave(kTRUE);        // optional

  // Get access to step and factory:
  step = go4->GetAnalysisStep("Unpack");
  fact=(TGo4StepFactory *)step->GetStepFactory();

  // tell the framework the name and class of our event processor:
  fact->DefUserEventSource("TArtEventSource");
  fact->DefInputEvent("RawEvent","TArtRawEvent");
  fact->DefEventProcessor("Proc","TArtProc");
  fact->DefOutputEvent("Dummy","TGo4EventElement");

  //  TGo4UserSourceParameter* source  = new TGo4UserSourceParameter("test.ridf");
  TGo4UserSourceParameter* source  = new TGo4UserSourceParameter("133.63.127.225","",10080);
  step->SetEventSource(source);  // register event source
  step->SetSourceEnabled(kTRUE);
  step->SetProcessEnabled(kTRUE);
  step->SetErrorStopEnabled(kTRUE);

  //--------------------------------------------------
  // optional output (event object needed):
  // 
  // store = new TGo4FileStoreParameter(outfile);
  // step->SetEventStore(store);
  step->SetStoreEnabled(kFALSE);  // en-disable output
  printf("Setup done !!\n");

}
