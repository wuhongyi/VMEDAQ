/*
  MathTools.hh
*/


#ifndef MathTools_h

#define MathTools_h 1

namespace MathTools {
  bool GaussElim( double **a, int n, double *b, int *indx, int *ipiv );
  bool GaussJordan( double **a, int n, double *b, 
		    int *indxc, int *indxd, int *ipiv );
  bool InterpolateRatio( int n, const double *xa, const double *ya, 
			 double *w1, double *w2, 
			 double x, double &y, double &dy );
  bool InterpolatePol( int n, const double *xa, const double *ya, 
		       double *w1, double *w2, 
		       double x, double &y, double &dy );
  bool SVDksb( double **u, const double *w, double **v, 
	       int m, int n, const double *b, double *x, double *wv ); 
  bool SVDcmp( double **a, int m, int n, double *w, double **v,
	       double *wv );
}

#endif
