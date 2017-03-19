/*
  DCConfMan.cc

  2004/6/6   T.Takahashi

*/

#include "DCConfMan.hh"
#include "DCGeomRecord.hh"
#include "TArtCore.hh"

#include <string>
#include <cstdio>
#include <cstring>

DCConfMan *DCConfMan::confMan_=0;

DCConfMan::DCConfMan()
  : HODid_(91),  FDC1id_(101),  FDC2id_(102)
{}

DCConfMan::~DCConfMan()
{}


DCConfMan & DCConfMan::GetInstance( void )
{

  if( !confMan_ ){
    TArtCore::Info(__FILE__,"create instance");
    confMan_ = new DCConfMan();
  }
  return *confMan_;
}


double DCConfMan::GetLocalZ( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::GetLocalZ(int)]"; 
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo->length_;
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

double DCConfMan::GetResolution( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::GetResolution(int)]"; 
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo->resol_;
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

double DCConfMan::GetTiltAngle( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::GetTiltAngle(int)]";
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo->tiltAngle_;
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

double DCConfMan::GetRotAngle1( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::GetRotAngle1(int)]";
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo->rotAngle1_;
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

double DCConfMan::GetRotAngle2( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::GetRotAngle2(int)]";
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo->rotAngle2_;
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

const TVector3 & DCConfMan::GetGlobalPosition( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::GetGlobalPosition(int)]";
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo->pos_;
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

TVector3 DCConfMan::NormalVector( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::NormalVector(int)]";
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo->NormalVector();
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

TVector3 DCConfMan::UnitVector( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::UnitVector(int)]";
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo->UnitVector();
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

const DCGeomRecord * DCConfMan::GetRecord( int lnum ) const
{
  static const std::string funcname = "[DCConfMan::GetRecord(int)]";
  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( pGeo ) return pGeo;
  else{
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }
}

void DCConfMan::clearElements( void )
{
  //  for_each( geomRecord_.begin(), geomRecord_.end(), DeleteObject() );
  std::map <int, DCGeomRecord *>::iterator itr;
  for( itr=geomRecord_.begin(); itr!=geomRecord_.end(); ++itr ){
    delete itr->second;
  }
  geomRecord_.clear();
  HODid_=91;
  FDC1id_=101;
  FDC2id_=102;
}

bool DCConfMan::Initialize( void )
{
  const int MaxChar = 200;
  static const std::string funcname = "[DCConfMan::Initialize]";
  char str[MaxChar];
  char cname[MaxChar];
  int id;
  double xs, ys, zs, ta, ra1, ra2, l, res;

  TArtCore::Info(__FILE__,"loading DC geom file: %s",filename_.c_str());

  FILE *fp;

  if( ( fp = fopen( filename_.c_str(), "r" ) ) == 0 ){
    TArtCore::Error(__FILE__,"%s: dc geom file open fail",filename_.c_str());
    throw;
  }

  clearElements();

  while( fgets( str, MaxChar, fp ) != 0 ){
    if( str[0]!='#' ){
      if( sscanf( str, "%d %s %lf %lf %lf %lf %lf %lf %lf %lf",
		  &id, cname, &xs, &ys, &zs, &ta, &ra1, &ra2, &l, &res )
	  == 10 ){
	TArtCore::Info(__FILE__,"PlaneID:%d, Name:%s, GlobalX:%0.02f, GlobalY:%0.02f, GlobalZ:%0.02f, TiltAngle:%0.02f, RA:%0.02f, RA2:%0.02f, L:%0.02f, Resolution:%0.02f",id,cname,xs,ys,zs,ta,ra1,ra2,l,res);

	DCGeomRecord *pRec = 
	  new DCGeomRecord( id, cname, xs, ys, zs, ta, ra1, ra2, l, res );
	DCGeomRecord *pOld = geomRecord_[id];
	geomRecord_[id] = pRec;

	if(strcmp(cname,"HOD")==0) {
	  HODid_=id;
	}
	if(strcmp(cname,"FDC1Center")==0) {
	  FDC1id_=id;
	}
	if(strcmp(cname,"FDC2Center")==0) {
	  FDC2id_=id;
	}

	if( pOld ){
	  //	  TArtCore::Warning(__FILE__,"%s: duplicated id number. following record is deleted.\nId=%d %f ) ...",funcname.c_str(),pOld->id_,pOld->pos_);
	  TArtCore::Warning(__FILE__,"%s: duplicated id number. following record is deleted.\nId=%d) ...",funcname.c_str(),pOld->id_);
	  delete pOld;
	}
      }
      else {
	std::string strtemp=str;
	TArtCore::Error(__FILE__,"%s: Invalid format",strtemp.c_str());
      }
    }
  }

  fclose(fp);

  return true;
}

std::vector <int> DCConfMan::GetDetectorIDList( void ) const
{
  std::vector<int> vlist;
  vlist.reserve(geomRecord_.size());
  std::map <int, DCGeomRecord *>::const_iterator 
    itr=geomRecord_.begin(), end=geomRecord_.end();

  for(; itr!=end; ++itr ){
    vlist.push_back( itr->first );
  }

  return vlist;
}

TVector3 DCConfMan::Local2GlobalPos( int lnum, 
					const TVector3 &in ) const
{
  static const std::string funcname = 
    "[DCConfMan::Local2GlobalPos(ThreeVecor &)]";

  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( !pGeo ) {
    TArtCore::Error(__FILE__,"%s: No record",funcname.c_str());
    throw;
  }

  double x = pGeo->dxds_*in.x() + pGeo->dxdt_*in.y()
    + pGeo->dxdu_*in.z() + pGeo->pos_.x();
  double y = pGeo->dyds_*in.x() + pGeo->dydt_*in.y()
    + pGeo->dydu_*in.z() + pGeo->pos_.y();
  double z = pGeo->dzds_*in.x() + pGeo->dzdt_*in.y()
    + pGeo->dzdu_*in.z() + pGeo->pos_.z();

  return TVector3( x, y, z );
}

TVector3 DCConfMan::Global2LocalPos( int lnum,
					const TVector3 &in ) const
{
  static const std::string funcname = 
    "[DCConfMan::Global2LocalPos(ThreeVecor &)]";

  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( !pGeo ){
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }

  double x 
    = pGeo->dsdx_*(in.x()-pGeo->pos_.x())
    + pGeo->dsdy_*(in.y()-pGeo->pos_.y())
    + pGeo->dsdz_*(in.z()-pGeo->pos_.z());
  double y 
    = pGeo->dtdx_*(in.x()-pGeo->pos_.x())
    + pGeo->dtdy_*(in.y()-pGeo->pos_.y())
    + pGeo->dtdz_*(in.z()-pGeo->pos_.z());
  double z 
    = pGeo->dudx_*(in.x()-pGeo->pos_.x())
    + pGeo->dudy_*(in.y()-pGeo->pos_.y())
    + pGeo->dudz_*(in.z()-pGeo->pos_.z());

  return TVector3( x, y, z );
}

TVector3 DCConfMan::Local2GlobalDir( int lnum,
					const TVector3 &in ) const
{
  static const std::string funcname = 
    "[DCConfMan::Local2GlobalDir(ThreeVecor &)]";

  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( !pGeo ){
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }

  double x = pGeo->dxds_*in.x() + pGeo->dxdt_*in.y()
    + pGeo->dxdu_*in.z();
  double y = pGeo->dyds_*in.x() + pGeo->dydt_*in.y()
    + pGeo->dydu_*in.z();
  double z = pGeo->dzds_*in.x() + pGeo->dzdt_*in.y()
    + pGeo->dzdu_*in.z();

  return TVector3( x, y, z );
}

TVector3 DCConfMan::Global2LocalDir( int lnum,
					const TVector3 &in ) const
{
  static const std::string funcname = 
    "[DCConfMan::Global2LocalDir(ThreeVecor &)]";

  DCGeomRecord *pGeo = geomRecord_[lnum];
  if( !pGeo ){
    TArtCore::Error(__FILE__,"%s: No record. Layer#=%d",funcname.c_str(),lnum);
    throw;
  }

  double x = pGeo->dsdx_*in.x() + pGeo->dsdy_*in.y()+ pGeo->dsdz_*in.z();
  double y = pGeo->dtdx_*in.x() + pGeo->dtdy_*in.y()+ pGeo->dtdz_*in.z();
  double z = pGeo->dudx_*in.x() + pGeo->dudy_*in.y()+ pGeo->dudz_*in.z();

  return TVector3( x, y, z );
}

