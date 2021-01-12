#include "TStyle.h"
//const int nn=100;//, bin=20;
int Eneresp[8]={583,1103,1540,2110,2600, 1067, 2910};
int Everesp[8]={2000000,2000000,2000000,2000000,2000000,2000000,2000000};

gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
gStyle->SetTextFont(42);
gStyle->SetLegendFont(42);

TFile  *fresp[8]; TFile *f[nn];
int life[3]={0,50,165}, ;
//TF1 *expf=new TF1("expf","TMath::Exp(-1.36e-3*x)",0,4000);
TF1 *expf=new TF1("expf","4.69115*TMath::Exp(-2.877e-3*x) + 2.21611*TMath::Exp(-3.0347e-4*x)",0,4000);
//TH1F *h[3][nn], *hcomp[3];
TH1F* htmp[18], *hresp[8], *hdraw[18];
TGraph *gresp[8];
TF1 *func[8], *fnull[8];
TF1 *whole[2];
TF1 *test;
double chiwhole[2], chinull[8], ndfwhole[2], ndfnull[8], likewhole[2], likenull[8];

double whole0(double *x, double *param);
double whole1(double *x, double *param);
double likelihood(TH1F*h){
  Double_t amin, edm, errdef;
  Int_t nvpar, nparx;
  TVirtualFitter *fitter = TVirtualFitter::Fitter(h);
  fitter->GetStats(amin, edm, errdef, nvpar, nparx);
  return amin; // Return the log-likelihood ratio
}
int loadresp(void){

  for(int i=0; i<7; i++){
    fresp[i] = new TFile(Form("hist/p%ip_%ikeV_%ievt_%ips.root",i<5?2:3,Eneresp[i],Everesp[i],0));
    hresp[i] = (TH1F*)fresp[i]->Get("egamABdc")->Clone();
    hresp[i]->SetStats(0);
    hresp[i]->SetName(Form("resp_%ikeV",Eneresp[i]));
    gresp[i]=new TGraph(hresp[i]);
    //for(int enedum=0;enedum<100;enedum++){ cout<<enedum<<" "<<gresp[i]->Eval(enedum*10.)<<endl;}
    //func[i]= new TF1("func",[&](double*x, double *p){ return p[0]*gresp[i]->Eval(x[0]); }, 0, 5000, 1);
  }
  whole[0]=new TF1("whole0",&whole0 ,0,5000,9);
  whole[1]=new TF1("whole1",&whole1 ,0,5000,7);
  whole[0]->SetNpx(185);
  whole[1]->SetNpx(185);
  return 0;
}

double whole0(double *x, double *param){
  double value=0.;
  for(int i=0;i<5;i++){
    value+=param[i]*gresp[i]->Eval(x[0]);
  }
  value+=param[5]*TMath::Exp(param[6]*x[0])+param[7]*TMath::Exp(param[8]*x[0]);
  return value;
}
double whole1(double *x, double *param){
  double value=0.;
  for(int i=0;i<2;i++){
    value+=param[i]*gresp[i+5]->Eval(x[0]);
  }
  value+=param[2]*gresp[4]->Eval(x[0]); //Add 2600
  value+=  param[3]*TMath::Exp(param[4]*x[0])+param[5]*TMath::Exp(param[6]*x[0]);
  return value;
}
