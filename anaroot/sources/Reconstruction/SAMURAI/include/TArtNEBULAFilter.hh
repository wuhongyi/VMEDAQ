
#ifndef _TARTNEBULAFILTER_H_
#define _TARTNEBULAFILTER_H_

#include "TNamed.h"

class TArtCalibNEBULA;

class TArtNEBULAFilter
{
public:
  static void IHitMin(TArtCalibNEBULA *fCalibNEBULA, Int_t ihitmin_n, Int_t ihitmin_v);
  static void Threshold(TArtCalibNEBULA *fCalibNEBULA);
  static void Threshold(TArtCalibNEBULA *fCalibNEBULA, Double_t threshold_n, Double_t threshold_v);
  static void TOF(TArtCalibNEBULA *fCalibNEBULA, Double_t min, Double_t max);
  static void Veto(TArtCalibNEBULA *fCalibNEBULA,  Int_t VetoNum);
  static void HitMinPos(TArtCalibNEBULA *fCalibNEBULA);
  static void HitMinTime(TArtCalibNEBULA *fCalibNEBULA);
  static void HitMinPos2(TArtCalibNEBULA *fCalibNEBULA);

private:
  TArtNEBULAFilter();
  virtual ~TArtNEBULAFilter();

  ClassDef(TArtNEBULAFilter,1);
};

#endif
