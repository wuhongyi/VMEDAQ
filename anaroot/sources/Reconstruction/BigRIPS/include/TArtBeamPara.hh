#ifndef _TARTBEAMPARA_H_
#define _TARTBEAMPARA_H_

#include <TString.h>
#include <TNamed.h>

#include "TArtRIPSPara.hh"
#include "TArtTOFPara.hh"

class TArtBeamPara : public TNamed {
  
 public:
  TArtBeamPara(){}
  TArtBeamPara(TArtRIPSPara *r1, TArtRIPSPara *r2, TArtTOFPara *t, char *de){
    nrips = 2;
    upstream_rips_name = *(r1->GetDetectorName());
    downstream_rips_name = *(r2->GetDetectorName());
    tof_name = *(t->GetDetectorName());
    deltae_name = de;
    char dname[128];
    sprintf(dname,"Beam_urips%s_drips%s_tof%s_ic%s",
            r1->GetDetectorName()->Data(),r2->GetDetectorName()->Data(),
            t->GetDetectorName()->Data(),de);
    detname = dname;
  }
  TArtBeamPara(TArtRIPSPara *r, TArtTOFPara *t, char *de){
    nrips = 1;
    upstream_rips_name = *(r->GetDetectorName());
    tof_name = *(t->GetDetectorName());
    deltae_name = de;

    char dname[128];
    sprintf(dname,"Beam_rips%s_tof%s_ic%s",r->GetDetectorName()->Data(),t->GetDetectorName()->Data(),de);
    detname = dname;

  }
  TArtBeamPara(TArtTOFPara *t, char *de){
    nrips = 0;
    tof_name = *(t->GetDetectorName());
    deltae_name = de;

    char dname[128];
    sprintf(dname,"Beam_tof%s_ic%s",t->GetDetectorName()->Data(),de);
    detname = dname;

  }
  ~TArtBeamPara(){}

  void SetNumRIPS           (Int_t v) { nrips = v; }
  void SetDetectorName      (char *v) { detname = v; }
  void SetUpstreamRIPSName  (char *v) { upstream_rips_name = v; }
  void SetDownstreamRIPSName(char *v) { downstream_rips_name = v; }
  void SetTOFName           (char *v) { tof_name = v; }
  void SetDeltaEName        (char *v) { deltae_name = v; }

  const Int_t GetNumRIPS         () const { return nrips; }
  TString * GetDetectorName      () { return &detname; }
  TString * GetUpstreamRIPSName  () { return &upstream_rips_name; }
  TString * GetDownstreamRIPSName() { return &downstream_rips_name; }
  TString * GetTOFName           () { return &tof_name; }
  TString * GetDeltaEName        () { return &deltae_name; }

 private:
  Int_t   nrips;
  TString detname;
  TString upstream_rips_name;
  TString downstream_rips_name;
  TString tof_name;
  TString deltae_name;

  ClassDef(TArtBeamPara,1);

};

#endif
