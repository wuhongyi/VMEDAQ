const double angle_in = 30.;
const double lc2target = 3872.3;
const double lc2fdc1 = 2723.;

const double angle_out = 90+59.930;
const double lc2fdc2 = 4125.23;
const double offsetfdc2 = 170.350;
const double lc2hod = 5042;

const int nfdc1l = 14;
const double fdc1lpos[nfdc1l] = {65,55,45,35,25,15,5,
				 -5,-15,-25,-35,-45,-55,-65};
//const double fdc1ta[nfdc1l] = {0,0,-30.,-30.,30,30,0,0,-30.,-30.,30,30,0,0};
const double fdc1ta[nfdc1l] = {0,0,30.,30.,-30,-30,0,0,30.,30.,-30,-30,0,0};

const int nfdc2l = 14;
const double fdc2lpos[nfdc1l] = {-308.66,-291.34,-208.66,-191.34,-108.66,-91.34,-8.66,
				 8.66,91.34,108.66,191.34,208.66,291.34,308.66};
//const double fdc2ta[nfdc1l] = {0,0,30.,30.,-30,-30,0,0,30.,30.,-30,-30,0,0};
const double fdc2ta[nfdc1l] = {0,0,-30.,-30.,30,30,0,0,-30.,-30.,30,30,0,0};

void PrintParam(int id, char* name, double l, double angle, double ta, double preso, double o=0){
  cout << id << " " << name << " " ;
  cout << l*cos(TMath::Pi()/180.*angle)-o*sin(TMath::Pi()/180.*angle) << " "; 
  cout << l*sin(TMath::Pi()/180.*angle)+o*cos(TMath::Pi()/180.*angle) << " " 
       << 0.0 << " " 
       << ta << " " ;
  cout << "90.0" << " ";
  if(angle<90){
    cout << angle-90. << " ";
    cout << lc2target-l << " ";
  }
  else{
    cout << angle+90. << " ";
    cout << lc2target+l << " ";
  }
  cout << preso << " ";
  cout << endl;
}

void PrintParam(int id, char* name, double l){
  cout << id << " " << name << " " ;
  cout << l << " "; 
  cout << 0.0 << " " 
       << 0.0 << " " 
       << 0.0 << " " ;
  cout << "90.0" << " ";
  cout << -90. << " ";
  cout << 0 << " ";
  cout << "0.25" << " ";
  cout << endl;
}

void MakeSAMURAIGeomParam(){

  Double_t l,angle;
  TString name;

  l = lc2target;
  angle = angle_in;

  PrintParam(0, "Target  ", l, angle, 0.0, 0.25);

  for(int i=0;i<nfdc1l;++i){
    name = "FDC1-";
    if(fdc1ta[i]==0)name+="X";
    else if(fdc1ta[i]<0)name+="U";
    else if(fdc1ta[i]>0)name+="V";
    name += (int)(i/6)+1;
    name += i%2==0 ? " " : "p";

    l = lc2fdc1+fdc1lpos[i];
    PrintParam(41+i, name.Data(), l, angle, fdc1ta[i], 0.2);
  }

  PrintParam(55, "MagnetIn", 1750);
  PrintParam(56, "Center0", 1250);
  PrintParam(57, "Center1", 750);
  PrintParam(58, "Center", 0);
  PrintParam(59, "Center2", -750);
  PrintParam(60, "MagnetOut", -1750);

  angle = angle_out;
  for(int i=0;i<nfdc2l;++i){
    name = "FDC2-";
    if(fdc2ta[i]==0)name+="X";
    else if(fdc2ta[i]<0)name+="U";
    else if(fdc2ta[i]>0)name+="V";
    name += (int)(i/6)+1;
    name += i%2==0 ? " " : "p";

    l = lc2fdc2+fdc2lpos[i];
    PrintParam(61+i, name.Data(), l, angle, fdc2ta[i], 0.1, offsetfdc2);
  }

  l = lc2hod;
  PrintParam(91, "HOD  ", l, angle, 0.0, 0.25);
  PrintParam(101, "FDC1c  ", lc2fdc1, angle_in, 0.0, 0.25);
  PrintParam(102, "FDC2c  ", lc2fdc2, angle_out, 0.0, 0.25, offsetfdc2);

}

