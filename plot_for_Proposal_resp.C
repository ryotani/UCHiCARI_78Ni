#include "TStyle.h"

const int nn=100, bin=20; //20;
int colornum[6]={1,9,8,6,2,4};
int Eneresp[8]={583,1103,1540,2110,2600, 1067, 2910};
int Everesp[8]={2000000,2000000,2000000,2000000,2000000,2000000,2000000};

gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
gStyle->SetTextFont(42);
gStyle->SetLegendFont(42);

TFile *f[18], *fresp[8];
char *hist[3]={"egamABdc", "MBegamABdc", "GRegamABdc"};
int life[3]={0,50,165}, ;
//TF1 *expf=new TF1("expf","TMath::Exp(-1.36e-3*x)",0,4000);
//TF1 *expf=new TF1("expf","4.69115*TMath::Exp(-2.877e-3*x) + 2.21611*TMath::Exp(-3.0347e-4*x)",0,4000); // not used?
TH1F *h[3][nn], *hcomp[3];
TH1F *htmp[18], *htmp_rebin[18], *hresp[8], *hdraw[18];
TGraph *gresp[8];
TF1 *func[9], *fnull[9];
TF1 *whole[2];
TF1 *test;
double chiwhole[2], chinull[9], ndfwhole[2], ndfnull[9], likewhole[2], likenull[9];
TCanvas *canvas[1];
TPad *p[5];
TLegend *l[2];
//TCanvas *c = new TCanvas("c","c",900,900);
void plot_for_Osaka(void);
int plotp(void);
//void drawhist(int hn);
void drawhist(int numhist, int *index, char**leg, char*output, TCanvas *c);
int loadresp(void);
double whole0(double *x, double *param);
double whole1(double *x, double *param);
double likelihood(TH1F*h){
  Double_t amin, edm, errdef;
  Int_t nvpar, nparx;
  TVirtualFitter *fitter = TVirtualFitter::Fitter(h);
  fitter->GetStats(amin, edm, errdef, nvpar, nparx);
  return amin; // Return the log-likelihood ratio
}
ofstream fout(Form("fig/p2pp3p_proposal_resp_%ikeV.csv",bin));

void plot_for_Proposal_resp(void){
  cout<< plot()<<endl;;
}

int plot(void){
 
  for(int i=0;i<3*3*2;i++){
    f[i] = new TFile(Form("fig/p%ip_78Ni_10d_Sep2019_%ips_%ikeV_%s.root",(int)(i/9)==0?2:3,life[i%3],bin,hist[(i/3)%3]));
  }
  loadresp();
  //return 0;
  for(int i=0;i<1;i++) canvas[i]= new TCanvas(Form("c%i",i),Form("c%i",i),900,900);
  //c->Divide(2,2);
  //for(int i=0;i<3;i++) drawhist(i);
    
  int dummy2[]={0,9};
  char*legend2 []={"^{79}Cu(p,2p)^{78}Ni","^{80}Zn(p,3p)^{78}Ni"};
  char output[500]=Form("p2pp3p_proposal_resp_%ikeV",bin);
  drawhist(2,dummy2,legend2,output, canvas[0]);

  /*
  int dummy3[]={0,1,2};
  char*legend3 []={"0ps","50ps","165ps"};
  drawhist(3,dummy3,legend3,"p2plife", canvas[1]);

  int dummy[]={0,3,6};
  char*legend []={"Total","MINIBALL","Tracking"};
  drawhist(3,dummy,legend,"p2pdetcomp",canvas[2]);
  */

  //
  //for(int i=0;i<3;i++)  h[i][nn]->SaveAs(Form("%s_%s.root",output,hist[i]));
}
///////////////////

int loadresp(void){

  for(int i=0; i<7; i++){
    fresp[i] = new TFile(Form("hist/p%ip_%ikeV_%ievt_%ips.root",i<5?2:3,Eneresp[i],Everesp[i],0));
    hresp[i] = (TH1F*)fresp[i]->Get("egamABdc")->Clone(); // 1keV binning
    hresp[i]->SetStats(0);
    hresp[i]->SetName(Form("resp_%ikeV",Eneresp[i]));
    gresp[i]=new TGraph(hresp[i]);
    //for(int enedum=0;enedum<100;enedum++){ cout<<enedum<<" "<<gresp[i]->Eval(enedum*10.)<<endl;}
    //func[i]= new TF1("func",[&](double*x, double *p){ return p[0]*gresp[i]->Eval(x[0]); }, 0, 5000, 1);
  }
  whole[0]=new TF1("whole0",&whole0 ,0,5000,9);
  whole[1]=new TF1("whole1",&whole1 ,0,5000,8);
  //whole[0]->SetNpx(185);
  //whole[1]->SetNpx(185);
  return 0;
}

///////////////////
void drawhist(int numhist, int *index, char **leg, char*output ,TCanvas* c){

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
    cout<<index[i]<<" p"<<((int)(index[i]/9)==0?2:3)<<"p "<<life[index[i]%3]<<"ps "<<hist[(index[i]/3)%3]<<endl;
    htmp[i] = (TH1F*)f[index[i]]->Get(Form("Hist_%s",hist[(index[i]/3)%3]))->Clone();
    htmp[i]->SetStats(0);
    if(life[index[i]%3]!=0) continue;
    htmp[i]->SetName(Form("h_p%ip_78Ni_10d_Aug2019_%ips_%s_%s",(int)(index[i]/9)==0?2:3,life[index[i]%3],hist[(index[i]/3)%3],output));
    htmp[i]->Draw(); //i==0?"":"same");
    htmp[i]->SetLineColor(1);//colornum[i]);
    htmp[i]->SetLineStyle(i/5);
    htmp[i]->SetLineWidth(3);
    l[i]->AddEntry(htmp[i], leg[i], "L");
  
    htmp[i]->GetXaxis()->SetTitleFont(43);
    htmp[i]->GetXaxis()->SetLabelFont(43);
    htmp[i]->GetYaxis()->SetTitleFont(43);
    htmp[i]->GetYaxis()->SetLabelFont(43);

    htmp[i]->SetTitle("");
    htmp[i]->GetYaxis()->SetTitle(Form("counts / %i keV, %i keV",bin, 2*bin));
    htmp[i]->GetYaxis()->SetTitleSize(20);
    htmp[i]->GetYaxis()->SetTitleOffset(1.7);
    htmp[i]->GetYaxis()->SetLabelSize(20);
    htmp[i]->GetYaxis()->SetLabelOffset(0.01);
    htmp[i]->GetYaxis()->SetRangeUser(0,i==0?49:19.8);
    htmp[i]->GetXaxis()->SetTitle("E_{#gamma} (keV)");
    htmp[i]->GetXaxis()->SetTitleSize(i+1==numhist?20:0);
    htmp[i]->GetXaxis()->SetTitleOffset(2);
    htmp[i]->GetXaxis()->SetLabelSize(i+1==numhist?20:0);
    htmp[i]->GetXaxis()->SetLabelOffset(0.01);
    htmp[i]->GetXaxis()->SetRangeUser(100,3800);
    //
    hdraw[i]=(TH1F*)htmp[i]->Clone();
    htmp_rebin[i] = (TH1F*)htmp[i]->Clone();
    htmp_rebin[i] -> Rebin(2);
    htmp[i]->Fit(whole[i],"","c",100,3800);
    htmp[i]->Fit(whole[i],"WQ","c",100,3800);
    htmp[i]->Fit(whole[i],"L","c",100,3800);
    chiwhole[i]=whole[i]->GetChisquare();
    likewhole[i]=likelihood(htmp[i]);
    ndfwhole[i]=whole[i]->GetNDF();
    cout<<chiwhole[i]<<" likelihood: "<<likewhole[i]<<" NDF"<< ndfwhole[i] <<endl;
    for(int j=(i==0?0:5);j<(i==0?5:9);j++){
      func[j]=new TF1(*whole[i]);
      fnull[j]=new TF1(*whole[i]);
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
      htmp[i]->Fit(fnull[j],"L","",100,3800);
      chinull[j]=fnull[j]->GetChisquare();
      likenull[j]=likelihood(htmp[i]); //was htmp[j]
      ndfnull[j]=fnull[j]->GetNDF();
      cout<<"Func"<<j<<" Chi:"<<chinull[j]<<" likelihood: "<<likenull[j]<<" NDF"<<ndfnull[j]<<endl;
    }
    //
    hdraw[i]->SetLineWidth(1);
    hdraw[i]->SetLineColor(12);
    //hdraw[i]->SetLineStyle(10);
    hdraw[i]->Draw();
    htmp_rebin[i]->Draw("same");
    for(int j=(i==0?0:5);j<(i==0?5:9);j++){
      func[j]->Draw("same");
      //fnull[j]->Draw("same");
    }
    whole[i]->Draw("same");
  }

  for(int i=0; i<2; i++){
    fout<<endl<<"Whole ,"<<chiwhole[i]<<", likelihood, "<<likewhole[i]<<", NDF,"<< ndfwhole[i] <<endl;
    for(int j=(i==0?0:5);j<(i==0?5:9);j++){
      fout<<"Func"<<j<<"Chi,"<<chinull[j]<<", likelihood, "<<likenull[j]<<", NDF,"<<ndfnull[j]<<endl;
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
  c->SaveAs(Form("fig/%s.png",output));
  c->SaveAs(Form("fig/%s.root",output));
  c->SaveAs(Form("fig/%s.C",output));
  
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
  value+=param[2]*gresp[0]->Eval(x[0]); //Add 583
  value+=param[3]*gresp[4]->Eval(x[0]); //Add 2600
  value+=  param[4]*TMath::Exp(param[5]*x[0])+param[6]*TMath::Exp(param[7]*x[0]);
  return value;
}
