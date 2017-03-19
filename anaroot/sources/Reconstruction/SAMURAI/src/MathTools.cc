/*
  MathTools.cc

*/

#include "MathTools.hh"

#include <string>
#include <iostream>
#include <iomanip>

#include <cmath>
#include <limits>

//#define ERROROUT 1 
//#define DebugPrint 1

const double Infinity = std::numeric_limits<double>::infinity();
const double TINY = std::numeric_limits<double>::epsilon();

bool MathTools::GaussElim( double **a, int n, double *b, int *indx, int *ipiv )
{
  static const std::string funcname = "MathTools::GaussElim";
  double big,c,pivinv,sum,c2;
  int js,irow=0;

  for(int i=0; i<n; ++i) ipiv[i]=0;
  for(int i=0; i<n; ++i){
    big=0.0;
    for(int j=0; j<n; ++j){
      if(!ipiv[j]){
        if((c=fabs(a[j][i]))>=big){ big=c; irow=j; }
      }
      else if(ipiv[j]>1){
#ifdef ERROROUT
	std::cerr << "[" << funcname << "]: Singular Matrix" << std::endl;
#endif
        return false;
      }
    }
    ++(ipiv[irow]); indx[i]=irow;
    if(a[irow][i]==0.0){
#ifdef ERROROUT
      std::cerr << "[" << funcname << "]: Singular Matrix" << std::endl;
#endif
      return false;
    }
    pivinv=1.0/a[irow][i];
    
    for(int j=0; j<n; ++j){
      if(ipiv[j]==0){
        c=a[j][i]; a[j][i]=0.0;
        for(int k=i+1; k<n; ++k)
          a[j][k]-=a[irow][k]*pivinv*c;
        b[j]-=b[irow]*pivinv*c;
      }
    }
  }        
          
  b[indx[n-1]]/=a[indx[n-1]][n-1];
  for(int i=n-2; i>=0; --i){
    sum=b[indx[i]];
    for(int j=i+1; j<n; ++j)
      sum-=a[indx[i]][j]*b[indx[j]];
    b[indx[i]]=sum/a[indx[i]][i];
  }
  for(int i=0; i<n; ++i){
    if(indx[i]!=i){
      c2=b[i];
      for(int j=indx[i]; indx[j]!=j; j=js){
        c=b[j]; b[j]=c2; c2=c; js=indx[j]; indx[j]=j;
      }
    }
  }
  return true;
}

bool MathTools::GaussJordan( double **a, int n, double *b, 
			     int *indxr, int *indxc, int *ipiv )
{
  static const std::string funcname = "MathTools::GaussJordan";

  for( int j=0; j<n; ++j ) ipiv[j]=0;
  for( int i=0; i<n; ++i ){
    double big=0.0;
    int irow=-1, icol=-1;
    for( int j=0; j<n; ++j )
      if( ipiv[j]!=1 )
	for( int k=0; k<n; ++k ){
	  if( ipiv[k]==0 ){
	    if( fabs(a[j][k])>=big ){
	      big=fabs(a[j][k]);
	      irow=j; icol=k;
	    }
	  }
	  else if( ipiv[k]>1 ){
#ifdef ERROROUT
	    std::cerr << "[" << funcname << "]: Singular Matrix" 
		      << std::endl;
#endif
	    return false;
	  }
	}
    ++(ipiv[icol]);
    
    if( irow!=icol ){
      for( int k=0; k<n; ++k ){
	double ta=a[irow][k];
	a[irow][k]=a[icol][k];
	a[icol][k]=ta;
      }
      double tb=b[irow];
      b[irow]=b[icol];
      b[icol]=tb;
    }
    
    indxr[i]=irow; indxc[i]=icol;

    if(a[icol][icol]==0.0){
#ifdef ERROROUT
      std::cerr << "[" << funcname << "]: Singular Matrix"  << std::endl;
#endif
      return false;
    }
    double pivinv=1./a[icol][icol];
    a[icol][icol]=1.;
    for(int k=0; k<n; ++k) a[icol][k]*=pivinv;
    b[icol]*=pivinv;
    for( int k=0; k<n; ++k ){
      if(k!=icol){
	double d=a[k][icol];
	a[k][icol]=0.;
	for( int l=0; l<n; ++l ) a[k][l] -= a[icol][l]*d;
	b[k] -= b[icol]*d;
      }
    }
  }

  for(int l=n-1; l>=0; --l){
    if( indxr[l]!=indxc[l] ){
      for(int k=0; k<n; ++k ){
	double t=a[k][indxr[l]];
	a[k][indxr[l]]=a[k][indxc[l]];
	a[k][indxc[l]]=t;
      }
    }
  }	  
  return true;
}


bool MathTools::InterpolateRatio( int n, const double *xa, const double *ya, 
				  double *w1, double *w2,
				  double x, double &y, double &dy )
{
  static const std::string funcname = "MathTools::InterpolateRatio";
  int i, m, ns=1;
  double w, t, hh, h, dd;

  hh=fabs(x-xa[0]);
  for(i=1; i<=n; ++i){
    h=fabs(x-xa[i-1]);
    if(h==0.0) { y=ya[i-1]; dy=0.0; return true; }
    else if(h<hh){ ns=i; hh=h; } 
    w1[i-1]=ya[i-1]; w2[i-1]=ya[i-1]*(1.+TINY);
  }
  y=ya[ns-1]; ns--;
  for(m=1; m<n; ++m){
    for(i=1; i<=n-m; ++i){
      w=w1[i]-w2[i-1]; h=xa[i+m-1]-x;
      t=(xa[i-1]-x)*w2[i-1]/h; dd=t-w1[i];
      if(dd==0.0){
#ifdef ERROROUT
        std::cerr << "[" << funcname << "]: Error" << std::endl;
#endif
        y=Infinity; dy=Infinity; return false;
      }
      dd=w/dd; w2[i-1]=w1[i]*dd; w1[i-1]=t*dd;
    }
    if( 2*ns < (n-m) ) dy=w1[ns];
    else { dy=w2[ns-1]; ns--; }
    y+=dy;
  }
#if 0
  std::cout << "[" << funcname << "]: x=" << std::setw(10) << x 
	    << " y=" << std::setw(10) << y << std::endl; 
#endif
  return true;
}

bool MathTools::InterpolatePol( int n, const double *xa, const double *ya,
				double *w1, double *w2, 
				double x, double &y, double &dy )
{
  static const std::string funcname = "MathTools::InterpolatePol";
  int i, m, ns=1;
  double den, dif, dift, ho, hp, w;

  dif=fabs(x-xa[0]);
  for(i=1; i<=n; ++i){
    if( (dift=fabs(x-xa[i-1]))<dif ){ ns=i; dif=dift; }
    w1[i-1]=w2[i-1]=ya[i-1];
  }
  y=ya[ns-1]; --ns;
  for(m=1; m<n; ++m){
    for(i=1; i<=n-m; ++i){
      ho=xa[i-1]-x; hp=xa[i+m-1]-x;
      w=w1[i]-w2[i-1];
      den=ho-hp;
      if(den==0.0){
#ifdef ERROROUT
	std::cerr << "[" << funcname << "]: Error" << std::endl;
#endif
	y=Infinity; dy=Infinity; return false;
      }
      den=w/den;
      w2[i-1]=hp*den; w1[i-1]=ho*den;
    }
    if( 2*ns<(n-m) ) dy=w1[ns];
    else           { dy=w2[ns-1]; --ns; }
    y+=dy;
  }
  return false;
}


bool MathTools::SVDksb( double **u, const double *w, double **v,
			int m, int n, const double *b, double *x, double *wv )
{
  static const std::string funcname = "MathTools::SVDksb";

  for( int j=0; j<n; ++j ){
    double s=0.0;
    if( w[j]!=0.0 ){
      for( int i=0; i<m; ++i )
	s += u[i][j]*b[i];
      s /= w[j];
    }
    wv[j]=s;
  }
  for( int i=0; i<n; ++i ){
    double s=0.0;
    for( int j=0; j<n; ++j )
      s += v[i][j]*wv[j];
    x[i]=s;
  }
  return true;
}


inline double pythag( double a, double b )
{
  double aa=fabs(a), ab=fabs(b);
  if( aa>ab ) 
    return aa*sqrt(1.+(ab/aa)*(ab/aa));
  else if( ab!=0. )
    return ab*sqrt(1.+(aa/ab)*(aa/ab));
  else
    return 0.0;
}
 
 

bool MathTools::SVDcmp( double **a, int m, int n, double *w, 
			double **v, double *wv )
{
  static const std::string funcname = "MathTools::SVDcmp";

  double g=0.0, scale=0.0, anorm=0.0;
  double s, f, h, c;
  int nm;

#ifdef DebugPrint
  for( int i=0; i<n; ++i ) {
    w[i]=wv[i]=0.0;
    for( int j=0; j<n; ++j ) v[j][i]=0.0;
  }

  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::scientific ); 
    std::cout.precision(3); 
    std::cout << "[" << funcname << "]: A in SVDcmp 1" <<  std::endl;
    for( int ii=0; ii<m; ++ii ){
      for( int ij=0; ij<n; ++ij ){
	std::cout << std::setw(12) << a[ii][ij]; 
	if( ij!=n-1 ) std::cout << ",";
      }
      std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: V in SVDcmp 1" << std::endl;
    for( int ii=0; ii<n; ++ii ){
     for( int ij=0; ij<n; ++ij ){
       std::cout << std::setw(12) << v[ii][ij];
       if( ij!=n-1 ) std::cout << ",";
     }
     std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: W in SVDcmp 1" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << w[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    
    std::cout << "[" << funcname << "]: WV in SVDcmp 1" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << wv[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    std::cout << std::endl;
   
    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

  // Householder method
  for( int i=0; i<n; ++i ){
    wv[i]=scale*g;
    g = scale = 0.0;
    if( i<m ){
      for( int k=i; k<m; ++k ) scale += fabs(a[k][i]);
      if( scale!=0. ){
	s = 0;
	for( int k=i; k<m; ++k ){
	  a[k][i] /= scale;
	  s += a[k][i]*a[k][i];
	}
	f = a[i][i];
	g = ( (f>0.0) ? -sqrt(s) : sqrt(s) );
	h = f*g-s;
	a[i][i] = f-g;
	for( int j=i+1; j<n; ++j ){
	  s = 0.0;
	  for( int k=i; k<m; ++k ) s += a[k][i]*a[k][j];
	  f = s/h;
	  for( int k=i; k<m; ++k )  a[k][j] += f*a[k][i];
	}
	for( int k=i; k<m; ++k ) a[k][i] *= scale;
      }
    }     /* if( i<m ) */
    w[i] = scale*g;
    g = s = scale = 0.0;

    if( i<m && i!=n-1 ){
      for( int k=i+1; k<n; ++k ) scale += fabs(a[i][k]);
      if( scale!=0.0 ){
	for( int k=i+1; k<n; ++k ){
	  a[i][k] /= scale;
	  s += a[i][k]*a[i][k];
	}
	f = a[i][i+1];
	g = ( (f>0.0) ? -sqrt(s) : sqrt(s) );
	h = f*g-s;
	a[i][i+1] = f-g;
	for( int k=i+1; k<n; ++k ) wv[k] = a[i][k]/h;
	for( int j=i+1; j<m; ++j ){
	  s = 0.0;
	  for( int k=i+1; k<n; ++k ) s += a[j][k]*a[i][k];
	  for( int k=i+1; k<n; ++k ) a[j][k] += s*wv[k];
	}
	for( int k=i+1; k<n; ++k ) a[i][k] *= scale;
      }
    }   /* if( i<m && i!=n-1 ) */
    double tmp=fabs(w[i])+fabs(wv[i]);
    if( tmp>anorm ) anorm = tmp;
  }     /* for( int i ... ) */

#if DebugPrint
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::scientific ); 
    std::cout.precision(3); 
    std::cout << "[" << funcname << "]: A in SVDcmp 2" <<  std::endl;
    for( int ii=0; ii<m; ++ii ){
      for( int ij=0; ij<n; ++ij ){
	std::cout << std::setw(12) << a[ii][ij]; 
	if( ij!=n-1 ) std::cout << ",";
      }
      std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: V in SVDcmp 2" << std::endl;
    for( int ii=0; ii<n; ++ii ){
     for( int ij=0; ij<n; ++ij ){
       std::cout << std::setw(12) << v[ii][ij];
       if( ij!=n-1 ) std::cout << ",";
     }
     std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: W in SVDcmp 2" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << w[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    
    std::cout << "[" << funcname << "]: WV in SVDcmp 2" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << wv[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    std::cout << std::endl;
   
    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

  for( int i=n-1; i>=0; --i ){
    if( i<n-1 ){
      if( g!=0.0 ){
	for( int j=i+1; j<n; ++j ) v[j][i] = (a[i][j]/a[i][i+1])/g;
	for( int j=i+1; j<n; ++j ){
	  s = 0.0;
	  for( int k=i+1; k<n; ++k ) s += a[i][k]*v[k][j];
	  for( int k=i+1; k<n; ++k ) v[k][j] += s*v[k][i];
	}
      }
      for( int j=i+1; j<n; ++j )
	v[i][j] = v[j][i] = 0.0;
    }
    v[i][i]=1.0;  g=wv[i];
  }   /* for( int i= ... ) */

#ifdef DebugPrint
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::scientific ); 
    std::cout.precision(3); 
    std::cout << "[" << funcname << "]: A in SVDcmp 3" <<  std::endl;
    for( int ii=0; ii<m; ++ii ){
      for( int ij=0; ij<n; ++ij ){
	std::cout << std::setw(12) << a[ii][ij]; 
	if( ij!=n-1 ) std::cout << ",";
      }
      std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: V in SVDcmp 3" << std::endl;
    for( int ii=0; ii<n; ++ii ){
     for( int ij=0; ij<n; ++ij ){
       std::cout << std::setw(12) << v[ii][ij];
       if( ij!=n-1 ) std::cout << ",";
     }
     std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: W in SVDcmp 3" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << w[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    
    std::cout << "[" << funcname << "]: WV in SVDcmp 3" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << wv[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    std::cout << std::endl;
   
    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

  int mn = ( (m<n) ? m : n );

  for( int i=mn-1; i>=0; --i ){
    g=w[i];
    for( int j=i+1; j<n; ++j ) a[i][j]=0.0;
    if( g!=0.0 ){
      g = 1./g;
      for( int j=i+1; j<n; ++j ){
	s = 0.0;
	for( int k=i+1; k<m; ++k ) s += a[k][i]*a[k][j];
	f = (s/a[i][i])*g;
	for( int k=i; k<m; ++k ) a[k][j] += f*a[k][i];
      }
      for( int j=i; j<m; ++j ) a[j][i] *= g;
    }
    else 
      for( int j=i; j<m; ++j ) a[j][i] = 0.0;

    a[i][i] += 1.0;
  }   /* for( int i= ... ) */

#ifdef DebugPrint
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::scientific ); 
    std::cout.precision(3); 
    std::cout << "[" << funcname << "]: A in SVDcmp 4" <<  std::endl;
    for( int ii=0; ii<m; ++ii ){
      for( int ij=0; ij<n; ++ij ){
	std::cout << std::setw(12) << a[ii][ij]; 
	if( ij!=n-1 ) std::cout << ",";
      }
      std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: V in SVDcmp 4" << std::endl;
    for( int ii=0; ii<n; ++ii ){
     for( int ij=0; ij<n; ++ij ){
       std::cout << std::setw(12) << v[ii][ij];
       if( ij!=n-1 ) std::cout << ",";
     }
     std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: W in SVDcmp 4" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << w[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    
    std::cout << "[" << funcname << "]: WV in SVDcmp 4" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << wv[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    std::cout << std::endl;
   
    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif


  int ll=1;

  for( int k=n-1; k>=0; --k ){
    for( int its=1; its<=30; ++its ){
      int flag=1; nm=ll;
      for( ll=k; ll>=0; --ll ){
	nm=ll-1;
	if( fabs(wv[ll])+anorm == anorm ){
	  flag=0; break;
	}
	if( fabs(w[nm])+anorm == anorm )
	  break;
      }

      if(flag){
	c=0.0; s=1.0;
	for( int i=ll; i<=k; ++i ){
	  f = s*wv[i]; wv[i] *= c;
	  if( fabs(f)+anorm == anorm )
	    break;
	  g=w[i]; h=pythag(f,g); w[i]=h; 
	  h=1./h; c=g*h; s=-f*h;
	  for( int j=0; j<m; ++j ){
	    double y=a[j][nm], z=a[j][i];
	    a[j][nm]=y*c+z*s; a[j][i]=z*c-y*s;
	  }
	}
      }   /* if(flag) */

      double z = w[k];
      if( ll==k ){
	if( z<0. ){
	  w[k]=-z;
	  for( int j=0; j<n; ++j ) v[j][k]=-v[j][k];
	}
	break;
      }
#ifdef ERROROUT
      if( its==30 ){
	std::cerr << "[" << funcname 
		  << "]: -- no convergence in 30 dvdcmp iterations --"
		  << std::endl;
      }
#endif
      nm=k-1;
      double x=w[ll], y=w[nm];
      g=wv[nm]; h=wv[k];
      f=((y-z)*(y+z)+(g-h)*(g+h))/(2.*h*y);
      g=pythag(f,1.0);
      double gtmp = ( (f>0.) ? g : -g );
      f=((x-z)*(x+z)+h*((y/(f+gtmp))-h))/x;
      c=s=1.0;
      for( int j=ll; j<=nm; ++j ){
	g=wv[j+1]; y=w[j+1]; h=s*g; g=c*g;
	z=pythag(f,h);
	wv[j]=z; c=f/z; s=h/z;
	f=x*c+g*s; g=g*c-x*s;
	h=y*s; y=y*c;
	for( int jj=0; jj<n; ++jj ){
	  x=v[jj][j]; z=v[jj][j+1];
	  v[jj][j]=x*c+z*s; v[jj][j+1]=z*c-x*s;
	}
	z=pythag(f,h);
	w[j]=z;
	if( z!=0.0 ){ z=1./z; c=f*z; s=h*z; }
	f=c*g+s*y; x=c*y-s*g;
	for( int jj=0; jj<m; ++jj ){
	  y=a[jj][j]; z=a[jj][j+1];
	  a[jj][j]=y*c+z*s; a[jj][j+1]=z*c-y*s;
	}
      }
      wv[ll]=0.0; wv[k]=f; w[k]=x;
    }   /* for( int its ... ) */
  }     /* for( int k= ... ) */


#ifdef DebugPrint 
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::scientific ); 
    std::cout.precision(3); 
    std::cout << "[" << funcname << "]: A in SVDcmp 5" <<  std::endl;
    for( int ii=0; ii<m; ++ii ){
      for( int ij=0; ij<n; ++ij ){
	std::cout << std::setw(12) << a[ii][ij]; 
	if( ij!=n-1 ) std::cout << ",";
      }
      std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: V in SVDcmp 5" << std::endl;
    for( int ii=0; ii<n; ++ii ){
     for( int ij=0; ij<n; ++ij ){
       std::cout << std::setw(12) << v[ii][ij];
       if( ij!=n-1 ) std::cout << ",";
     }
     std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: W in SVDcmp 5" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << w[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    
    std::cout << "[" << funcname << "]: WV in SVDcmp 5" << std::endl;
    for( int ij=0; ij<n; ++ij ){
      std::cout << std::setw(12) << wv[ij];
      if( ij!=n-1 ) std::cout << ",";
    }
    std::cout << std::endl;
    std::cout << std::endl;
   
    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

  return true;
}

