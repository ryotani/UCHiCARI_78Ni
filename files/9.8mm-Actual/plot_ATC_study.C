#include "TStyle.h"
#include "draw_middle_angles.C"

const int nn=100, bin=20; //20;
char output[500]=Form("p2pp3p_ATC_Study_%ikeV",bin);
ofstream fout(Form("fig/%s.csv",output));
ofstream fout_Nsigma("fig/Nsigma-Summary.csv", std::ios_base::app);

int colornum[6]={1,9,8,6,2,4};
int Eneresp[8]={583,1103,1540,2110,2600, 1067, 2910};
int Everesp[8]={2000000,2000000,2000000,2000000,2000000,2000000,2000000};
//
int Enep2p[]={5293, 3183, 5243, 3703};
int life4=0;
int Evep2p[]={399,190,342,247};
int Enep3p[]={5293, 3977, 2910};
int Evep3p[]={179,228,414};
//

gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
gStyle->SetTextFont(42);
gStyle->SetLegendFont(42);

TFile *f[18], *fresp[8];
//char *hist[3]={"egamABdc", "MBegamABdc", "GRegamABdc"};
int life[3]={0,50,165}, ;
//TF1 *expf=new TF1("expf","TMath::Exp(-1.36e-3*x)",0,4000);
//TF1 *expf=new TF1("expf","4.69115*TMath::Exp(-2.877e-3*x) + 2.21611*TMath::Exp(-3.0347e-4*x)",0,4000); // not used?
TH1F *h[3][nn], *hcomp[3];
//TH1F *hist[18],
TH1F  *hist_rebin[18], *hresp[3][8], *hdraw[18];
TH1F *h_Actual[2]  ;
TH1F *h_AddATC[2] ;
TH1F *h_AddATC_SCout[2] ;
TGraph *gresp[3][8];
TF1 *func[9], *fnull[9];
TF1 *whole[3][2];
TF1 *test;
double chiwhole[3][2], chinull[9], ndfwhole[3][2], ndfnull[9], likewhole[3][2], likenull[9];
TCanvas *canvas[1];
TPad *p[5];
TLegend *l[2];
//TCanvas *c = new TCanvas("c","c",900,900);
int plotp(void);
//void drawhist(int hn);
void drawhist(int numhist, int *index, char**leg, char*output, TCanvas *c, TH1F** hist, int config_index);
int loadresp(void);
int loadsim(void);
double whole0(double *x, double *param);
double whole1(double *x, double *param);
double whole0ATC(double *x, double *param);
double whole1ATC(double *x, double *param);
double whole0ATCwoSC(double *x, double *param);
double whole1ATCwoSC(double *x, double *param);
double likelihood(TH1F*h){
  Double_t amin, edm, errdef;
  Int_t nvpar, nparx;
  TVirtualFitter *fitter = TVirtualFitter::Fitter(h);
  fitter->GetStats(amin, edm, errdef, nvpar, nparx);
  return amin; // Return the log-likelihood ratio
}
void plot_ATC_study(void){
  int dummy2[]={0,9};
  char*legend2 []={"^{79}Cu(X,Xp)^{78}Ni","^{80}Zn(X,X2p)^{78}Ni"};
  for(int i=0;i<1;i++) canvas[i]= new TCanvas(Form("c%i",i),Form("c%i",i),900,900);
  //
  loadsim();
  loadresp();
  //
  canvas[0]->SaveAs(Form("fig/%s.pdf[",output));
  fout_Nsigma << "h_Actual, ";
  drawhist(2,dummy2,legend2,output, canvas[0], h_Actual, 0);
  fout_Nsigma << "h_ATC_minus_MB, ";
  drawhist(2,dummy2,legend2,output, canvas[0], h_AddATC, 1);
  fout_Nsigma << "h_ATC_minus_SC, ";
  drawhist(2,dummy2,legend2,output, canvas[0], h_AddATC_SCout, 2);
  canvas[0]->SaveAs(Form("fig/%s.pdf]",output));
  fout_Nsigma << endl;
}

///////////////////
int loadsim(void){
  TF1 *expf=new TF1("expf","4.69115*TMath::Exp(-1.08e-3*x) + 2.21611*TMath::Exp(-3.0347e-4*x)",0,4000);
  //cout<< plot()<<endl;;
  int numbin = 200;
  h_Actual[0] = new TH1F("h_Actual0", "Actual; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
  h_AddATC[0] = new TH1F("h_AddATC0", "AddATC - MB; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
  h_AddATC_SCout[0] = new TH1F("h_AddATC_SCout0", "ATC - SC; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
  h_Actual[1] = new TH1F("h_Actual1", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
  h_AddATC[1] = new TH1F("h_AddATC1", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
  h_AddATC_SCout[1] = new TH1F("h_AddATC_SCout1", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
  
  //for(int i=0; i<1; i++){ //p2p
  for(int i=0; i<4; i++){ //p2p
    TH1F *htmp1 = new TH1F("htmp1", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
    TH1F *htmp2 = new TH1F("htmp2", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
    TH1F *htmp3 = new TH1F("htmp3", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
  
    draw_comparison(Form("files/9.8mm-Actual/root/p2p_%ikeV_%ievt_0ps.root", Enep2p[i], Evep2p[i]), 0, htmp1, htmp2, htmp3);
    if(i==0){
      /*h_Actual[0]=(TH1F*)(htmp1->Clone());
      h_AddATC[0]=(TH1F*)(htmp2->Clone());
      h_AddATC_SCout[0]=(TH1F*)(htmp3->Clone());
      */
      h_Actual[0] -> FillRandom("expf", (int)(1.02/(1.36e-3)*9./6.*0.5));
      h_AddATC[0] -> FillRandom("expf", (int)(1.02/(1.36e-3)*9./6.*0.5*1.15));
      h_AddATC_SCout[0] -> FillRandom("expf", (int)(1.02/(1.36e-3)*9./6.*0.5*1.15));
    }
      //}else{
      h_Actual[0]->Add(htmp1);
      h_AddATC[0]->Add(htmp2);
      h_AddATC_SCout[0]->Add(htmp3);
      //}
    //htmp1->Clear();
    //htmp2->Clear();
    //htmp3->Clear();
    delete htmp1;
    delete htmp2;
    delete htmp3;
  }
  for(int i=0; i<3; i++){ //p3p
    TH1F *htmp1 = new TH1F("htmp1", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
    TH1F *htmp2 = new TH1F("htmp2", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
    TH1F *htmp3 = new TH1F("htmp3", "; Energy / keV; Counts / 20 keV", numbin, 200, 4200);
    draw_comparison(Form("files/9.8mm-Actual/root/p3p_%ikeV_%ievt_0ps.root", Enep3p[i], Evep3p[i]),0, htmp1, htmp2, htmp3);
    if(i==0){
      /*
      h_Actual[1] -> FillRandom("expf", (int)(1.02*(double)h_Actual[1]->Integral()/446./(1.36e-3)*9./6.*0.5));
      h_AddATC[1] -> FillRandom("expf", (int)(1.02*(double)h_AddATC[1]->Integral()/446./(1.36e-3)*9./6.*0.5));
      h_AddATC_SCout[1] -> FillRandom("expf", (int)(1.02*(double)h_AddATC_SCout[1]->Integral()/446./(1.36e-3)*9./6.*0.5));
      */
      h_Actual[1] -> FillRandom("expf", (int)(1.02*104./446./(1.36e-3)*9./6.*0.5));
      h_AddATC[1] -> FillRandom("expf", (int)(1.02*104./446./(1.36e-3)*9./6.*0.5*1.15));
      h_AddATC_SCout[1] -> FillRandom("expf", (int)(1.02*104./446./(1.36e-3)*9./6.*0.5*1.15));
    }
    //}else{
      h_Actual[1]->Add(htmp1);
      h_AddATC[1]->Add(htmp2);
      h_AddATC_SCout[1]->Add(htmp3);
      //}
    //htmp1->Clear();
    //htmp2->Clear();
    //htmp3->Clear();
    delete htmp1;
    delete htmp2;
    delete htmp3;
    }
  delete expf;
  return 0;
}

int loadresp(void){
  for(int i=0; i<7; i++){
    TH1F *hresptemp[3];
    for(int j =0; j<3; j++){
      hresptemp[j] = new TH1F(Form("hresptmp%i",j), "2910 keV with AB (Black: Actual geometetry, Red: + ATC - 2 MB); Energy / keV; Counts / 20 keV", 200, 200, 4200);
    }
    draw_comparison(Form("files/9.8mm-Actual/root/p%ip_%ikeV_%ievt_%ips.root", i<5?2:3,Eneresp[i],Everesp[i],0), 0, hresptemp[0], hresptemp[1], hresptemp[2]);
    for(int j =0; j<3; j++){
      hresp[j][i] = (TH1F*)hresptemp[j]->Clone();
      hresp[j][i]->SetStats(0);
      hresp[j][i]->SetName(Form("resp_%ikeV_%i",Eneresp[i],j));
      gresp[j][i]=new TGraph(hresp[j][i]);
    }
    delete hresptemp[0];
    delete hresptemp[1];
    delete hresptemp[2];
    cout<<"Loaded: "<<Form("files/9.8mm-Actual/root/p%ip_%ikeV_%ievt_%ips.root", i<5?2:3,Eneresp[i],Everesp[i],0)<<endl;
  }
  whole[0][0]=new TF1("whole0",&whole0 ,0,5000,9);
  whole[0][1]=new TF1("whole1",&whole1 ,0,5000,8);
  whole[1][0]=new TF1("whole0ATC",&whole0ATC ,0,5000,9);
  whole[1][1]=new TF1("whole1ATC",&whole1ATC ,0,5000,8);
  whole[2][0]=new TF1("whole0ATCwoSC",&whole0ATCwoSC ,0,5000,9);
  whole[2][1]=new TF1("whole1ATCwoSC",&whole1ATCwoSC ,0,5000,8);
  //
  return 0;
}

///////////////////
void drawhist(int numhist, int *index, char **leg, char*output ,TCanvas* c, TH1F **hist, int config_index){

  c->cd(0);
  for(int i=0;i<numhist;i++){
    c->cd(0);
    delete p[numhist-i];
    p[numhist-i] = new TPad(Form("p%i",i),Form("p%i",i),0,i==0?0:(0.9*((double)i)/((double)numhist)+0.1),1,(0.9*((double)i+1.)/((double)numhist)+0.1));
    p[numhist-i]->SetTopMargin(0.01);
    p[numhist-i]->SetLeftMargin(0.08);
    p[numhist-i]->SetBottomMargin(i==0?0.2:0);
    p[numhist-i]->SetRightMargin(0.03);
    p[numhist-i]->Draw();
  }
  //
  for(int i=0;i<numhist;i++){
    p[i+1]->cd();
  l[i] = new TLegend(0.8, 0.9, 0.95,.98);
    hist[i]->SetStats(0);
    if(life[index[i]%3]!=0) continue;
    //hist[i]->SetName(Form("h_p%ip_78Ni_10d_Aug2019_%ips_%s_%s",(int)(index[i]/9)==0?2:3,life[index[i]%3],hist[(index[i]/3)%3],output));
    hist[i]->Draw();
    hist[i]->SetLineColor(1);
    hist[i]->SetLineStyle(i/5);
    hist[i]->SetLineWidth(3);
    l[i]->AddEntry(hist[i], leg[i], "L");
  
    hist[i]->GetXaxis()->SetTitleFont(43);
    hist[i]->GetXaxis()->SetLabelFont(43);
    hist[i]->GetYaxis()->SetTitleFont(43);
    hist[i]->GetYaxis()->SetLabelFont(43);

    hist[i]->SetTitle("");
    //hist[i]->GetYaxis()->SetTitle(Form("counts / %i keV, %i keV",bin, 2*bin));
    hist[i]->GetYaxis()->SetTitle(Form("counts / %i keV",bin));
    hist[i]->GetYaxis()->SetTitleSize(20);
    hist[i]->GetYaxis()->SetTitleOffset(1.7);
    hist[i]->GetYaxis()->SetLabelSize(20);
    hist[i]->GetYaxis()->SetLabelOffset(0.01);
    hist[i]->GetYaxis()->SetRangeUser(0,i==0?30.8:10.8);
    hist[i]->GetXaxis()->SetTitle("E_{#gamma} (keV)");
    hist[i]->GetXaxis()->SetTitleSize(i+1==numhist?20:0);
    hist[i]->GetXaxis()->SetTitleOffset(2);
    hist[i]->GetXaxis()->SetLabelSize(i+1==numhist?20:0);
    hist[i]->GetXaxis()->SetLabelOffset(0.01);
    hist[i]->GetXaxis()->SetRangeUser(300,3800);
    //
    hdraw[i]=(TH1F*)hist[i]->Clone();
    //hist_rebin[i] = (TH1F*)hist[i]->Clone();
    //hist_rebin[i] -> Rebin(2);
    hist[i]->Draw("");
    //
    hist[i]->Fit(whole[config_index][i],"","c",300,3800);
    hist[i]->Fit(whole[config_index][i],"WQ","c",300,3800);
    hist[i]->Fit(whole[config_index][i],"L","c",300,3800);
    chiwhole[config_index][i]=whole[config_index][i]->GetChisquare();
    likewhole[config_index][i]=likelihood(hist[i]);
    ndfwhole[config_index][i]=whole[config_index][i]->GetNDF();
    cout<<chiwhole[config_index][i]<<" likelihood: "<<likewhole[config_index][i]<<" NDF"<< ndfwhole[config_index][i] <<endl;
    for(int j=(i==0?0:5);j<(i==0?5:9);j++){
      func[j]=new TF1(*whole[config_index][i]);
      fnull[j]=new TF1(*whole[config_index][i]);
      func[j]->SetName(Form("func%i",j));
      fnull[j]->SetName(Form("fnull%i",j));
      for(int k=0;k<(i==0?9:8);k++){
	if((j-5*i)!=k){
	  func[j]->SetParameter(k,0.);
	}else{
	  fnull[j]->FixParameter(k,0.);
	}
      }
      cout<<endl;
      func[j]->SetLineColor(9);
      func[j]->SetLineWidth(2);
      func[j]->SetLineStyle(9);
      //
      hist[i]->Fit(fnull[j],"L","",300,3800);
      chinull[j]=fnull[j]->GetChisquare();
      likenull[j]=likelihood(hist[i]); //was hist[j]
      ndfnull[j]=fnull[j]->GetNDF();
      cout<<"Func"<<j<<" Chi:"<<chinull[j]<<" likelihood: "<<likenull[j]<<" NDF"<<ndfnull[j]<<endl;
    }
    //
    hdraw[i]->SetLineWidth(1);
    hdraw[i]->SetLineColor(12);
    //hdraw[i]->SetLineStyle(10);
    hdraw[i]->Draw();
    //hist_rebin[i]->Draw("same");
    for(int j=(i==0?0:5);j<(i==0?5:9);j++){
      func[j]->Draw("same");
      //fnull[j]->Draw("same");
    }
    whole[config_index][i]->Draw("same");
  }

  for(int i=0; i<2; i++){
    fout<<endl<<config_index<<" (X,X"<<i+1<<"p) "<<hist[i]->GetTitle()<<endl;
    fout<<"Whole ,"<<chiwhole[config_index][i]<<", likelihood, "<<likewhole[config_index][i]<<", NDF,"<< ndfwhole[config_index][i] <<endl;
    for(int j=(i==0?0:5);j<(i==0?5:9);j++){
      double Nsigma = TMath::NormQuantile(TMath::Prob(2.*(likenull[j]-likewhole[config_index][i]),1))*(-1.0);
      fout<<"Ene, "<<Eneresp[j<7?(j):((j-7)*4)]<<", ";
      fout<<"Func"<<j<<", Chi,"<<chinull[j]<<", likelihood, "<<likenull[j]<<", Nsigma, "<<Nsigma<<", NDF,"<<ndfnull[j]<<endl;
      fout_Nsigma << Nsigma <<", ";
    }
    l[i]->SetTextFont(43);
    l[i]->SetTextSize(20);
    l[i]->SetBorderSize(0);
    l[i]->SetFillColor(0);
    //c->cd(1+i);
    p[i+1]->cd();
    l[i]->Draw("same");
  }
  
  //  cd->(1);
  

  c->SaveAs(Form("fig/%s.pdf",output));
  
  /*
  c->SaveAs(Form("fig/%s.pdf",output));
  c->SaveAs(Form("fig/%s.png",output));
  c->SaveAs(Form("fig/%s.root",output));
  c->SaveAs(Form("fig/%s.C",output));
  */
  
}

double whole0(double *x, double *param){
  double value=0.;
  for(int i=0;i<5;i++){
    value+=param[i]*gresp[0][i]->Eval(x[0]);
  }
  value+=param[5]*TMath::Exp(param[6]*x[0])+param[7]*TMath::Exp(param[8]*x[0]);
  return value;
}
double whole1(double *x, double *param){
  double value=0.;
  for(int i=0;i<2;i++){
    value+=param[i]*gresp[0][i+5]->Eval(x[0]);
  }
  value+=param[2]*gresp[0][0]->Eval(x[0]); //Add 583
  value+=param[3]*gresp[0][4]->Eval(x[0]); //Add 2600
  value+=  param[4]*TMath::Exp(param[5]*x[0])+param[6]*TMath::Exp(param[7]*x[0]);
  return value;
}
//
double whole0ATC(double *x, double *param){
  double value=0.;
  for(int i=0;i<5;i++){
    value+=param[i]*gresp[1][i]->Eval(x[0]);
  }
  value+=param[5]*TMath::Exp(param[6]*x[0])+param[7]*TMath::Exp(param[8]*x[0]);
  return value;
}
double whole1ATC(double *x, double *param){
  double value=0.;
  for(int i=0;i<2;i++){
    value+=param[i]*gresp[1][i+5]->Eval(x[0]);
  }
  value+=param[2]*gresp[1][0]->Eval(x[0]); //Add 583
  value+=param[3]*gresp[1][4]->Eval(x[0]); //Add 2600
  value+=  param[4]*TMath::Exp(param[5]*x[0])+param[6]*TMath::Exp(param[7]*x[0]);
  return value;
}
//
double whole0ATCwoSC(double *x, double *param){
  double value=0.;
  for(int i=0;i<5;i++){
    value+=param[i]*gresp[2][i]->Eval(x[0]);
  }
  value+=param[5]*TMath::Exp(param[6]*x[0])+param[7]*TMath::Exp(param[8]*x[0]);
  return value;
}
double whole1ATCwoSC(double *x, double *param){
  double value=0.;
  for(int i=0;i<2;i++){
    value+=param[i]*gresp[2][i+5]->Eval(x[0]);
  }
  value+=param[2]*gresp[2][0]->Eval(x[0]); //Add 583
  value+=param[3]*gresp[2][4]->Eval(x[0]); //Add 2600
  value+=  param[4]*TMath::Exp(param[5]*x[0])+param[6]*TMath::Exp(param[7]*x[0]);
  return value;
}
