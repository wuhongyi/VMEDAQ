#ifndef TARTEXEC_HH
#define TARTEXEC_HH

#include "TObject.h"

class TArtExec
{
public:
  static Double_t GXValue();
  static Double_t GYValue();
  static void GXYValue(Double_t& x, Double_t& y);

private:
  enum EExecState{kInitial, kRunning, kEnd};

private:
  //main eventloop.
  template<class T>
  static void EventLoop(T& fpt);

  //get x value by graphical method
  class FGXValue
  {
  public:
    FGXValue(){;}
    ~FGXValue(){;}

    void operator()(EExecState execstate);
    Double_t GetX() const {return fX;}

  private:
    Double_t fX;
  };

  //get y value by graphical method
  class FGYValue
  {
  public:
    FGYValue(){;}
    ~FGYValue(){;}

    void operator()(EExecState execstate);
    Double_t GetY() const {return fY;}

  private:
    Double_t fY;
  };

  class FGXYValue
  {
  public:
    FGXYValue(){;}
    ~FGXYValue(){;}

    void operator()(EExecState execstate);
    Double_t GetX() const {return fX;}
    Double_t GetY() const {return fY;}

  private:
    Double_t fX, fY;
  };
};

#endif
