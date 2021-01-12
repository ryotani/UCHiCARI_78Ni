#include "TStyle.h"

const int nn=100, bin=10;
int colornum[6]={1,9,8,6,2,4};

gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
gStyle->SetTextFont(42);
gStyle->SetLegendFont(42);

TFile *f[18];
char *hist[3]={"egamABdc", "MBegamABdc", "GRegamABdc"};
int life[3]={0,50,165}, ;
//TF1 *expf=new TF1("expf","TMath::Exp(-1.36e-3*x)",0,4000);
TF1 *expf=new TF1("expf","4.69115*TMath::Exp(-2.877e-3*x) + 2.21611*TMath::Exp(-3.0347e-4*x)",0,4000);
TH1F *h[3][nn], *hcomp[3];
TH1F* htmp[18];
TPad *p[5];
TLegend *l;
TCanvas *c = new TCanvas("c","c",900,900);
void plot_for_Osaka(void);
void plotp(void);
void drawhist(int hn);
void drawhist(int numhist, int *index, char**leg, char*output);

void plot_for_Osaka(void){
  plot();
}

void plot(void){
 
  for(int i=0;i<3*3*2;i++){
    f[i] = new TFile(Form("fig/p%ip_78Ni_10d_Sep2019_%ips_%s.root",(int)(i/9)==0?2:3,life[i%3],hist[(i/3)%3]));
  }
  //c->Divide(2,2);
  //for(int i=0;i<3;i++) drawhist(i);
    
  int dummy2[]={0,9};
  char*legend2 []={"^{79}Cu(p,2p)^{78}Ni","^{80}Zn(p,3p)^{78}Ni"};
  drawhist(2,dummy2,legend2,"p2pp3p_all");

  int dummy3[]={0,1,2};
  char*legend3 []={"0ps","50ps","165ps"};
  drawhist(3,dummy3,legend3,"p2plife");

  int dummy[]={0,3,6};
  char*legend []={"Total","MINIBALL","Tracking"};
  drawhist(3,dummy,legend,"p2pdetcomp");


  //
  //for(int i=0;i<3;i++)  h[i][nn]->SaveAs(Form("%s_%s.root",output,hist[i]));
}

void drawhist(int numhist, int *index, char **leg, char*output ){

  p[0] = new TPad(Form("p%i",0),Form("p%i",0),0,0,1,1);
  p[0]->SetTopMargin(0.00);
  p[0]->SetLeftMargin(0.08);
  p[0]->SetBottomMargin(0.1);
  p[0]->SetRightMargin(0.0);
  p[0]->Draw();
  l = new TLegend(0.6, 0.8, 0.9,0.9);

  for(int i=0;i<numhist;i++){
    cout<<index[i]<<" p"<<((int)(index[i]/9)==0?2:3)<<"p "<<life[index[i]%3]<<"ps "<<hist[(index[i]/3)%3]<<endl;
    htmp[i] = (TH1F*)f[index[i]]->Get(Form("Hist_%s",hist[(index[i]/3)%3]))->Clone();
    htmp[i]->SetStats(0);
    htmp[i]->SetName(Form("h_p%ip_78Ni_10d_Aug2019_%ips_%s_%s",(int)(index[i]/9)==0?2:3,life[index[i]%3],hist[(index[i]/3)%3],output));
    htmp[i]->Draw(i==0?"":"same");
    htmp[i]->SetLineColor(colornum[i]);
    htmp[i]->SetLineStyle(i/5);
    htmp[i]->SetLineWidth(2);
    l->AddEntry(htmp[i], leg[i], "L");
  }
  htmp[0]->SetTitle("");
  htmp[0]->GetYaxis()->SetTitle(Form("counts / %i keV",bin));
  htmp[0]->GetYaxis()->SetTitleSize(0.03);
  htmp[0]->GetYaxis()->SetTitleOffset(1.5);
  htmp[0]->GetYaxis()->SetLabelSize(0.03);
  htmp[0]->GetYaxis()->SetLabelOffset(0.01);
  htmp[0]->GetXaxis()->SetTitle("E_{#gamma} (keV)");
  htmp[0]->GetXaxis()->SetTitleSize(0.03);
  htmp[0]->GetXaxis()->SetTitleOffset(1.5);
  htmp[0]->GetXaxis()->SetLabelSize(0.03);
  htmp[0]->GetXaxis()->SetLabelOffset(0.01);
  htmp[0]->GetXaxis()->SetRangeUser(50,4500);
  htmp[0]->GetXaxis()->SetTitleFont(42);
  htmp[0]->GetXaxis()->SetLabelFont(42);
  htmp[0]->GetYaxis()->SetTitleFont(42);
  htmp[0]->GetYaxis()->SetLabelFont(42);


  l->SetTextSize(0.025);
  l->SetBorderSize(1);
  l->SetFillColor(0);
  l->Draw();

  c->SaveAs(Form("fig/%s.pdf",output));
  c->SaveAs(Form("fig/%s.png",output));
  c->SaveAs(Form("fig/%s.root",output));
  c->SaveAs(Form("fig/%s.C",output));
   
}

/*
void drawhist(int hn){
  c->cd(hn+1);
  p[hn] = new TPad(Form("p%i",hn),Form("p%i",hn),0,0,1,1);
  p[hn]->SetTopMargin(0.0);
  p[hn]->SetLeftMargin(0.08);
  p[hn]->SetBottomMargin(0.11);
  p[hn]->SetRightMargin(0.01);
  p[hn]->Draw();
  //p[hn]->SetLogy();
  p[hn]->cd();
  
  for(int i=0;i<nn;i++){
    if(i==0){
      h[hn][nn] = (TH1F*)f[i]->Get(hist[hn])->Clone();
      h[hn][nn]->SetName(Form("Hist_%s",hist[hn]));
    }
    else
      h[hn][nn]->Add((TH1F*)f[i]->Get(hist[hn]));
    h[hn][i] = (TH1F*)f[i]->Get(hist[hn])->Clone();
    h[hn][i]->SetName(Form("hist%i_%s",i,hist[hn]));
    //cout<<Form("hist%i_%s",i,hist[hn])<<endl;
  }
  cout<<expeve<<" "<fraction[hn]<<endl;
  h[hn][nn]->FillRandom("expf", (int)((double)expeve*fraction[hn]));
  h[hn][nn]->Rebin(bin);
  h[hn][nn]->GetYaxis()->SetTitle(Form("counts / %i keV",bin));
  h[hn][nn]->GetYaxis()->SetTitleSize(0.045);
  h[hn][nn]->GetYaxis()->SetTitleOffset(0.9);
  h[hn][nn]->GetYaxis()->SetLabelSize(0.035);
  h[hn][nn]->GetYaxis()->SetLabelOffset(0.01);

  h[hn][nn]->GetXaxis()->SetTitle("E_{#gamma} (keV)");
  h[hn][nn]->GetXaxis()->SetTitleSize(0.045);
  h[hn][nn]->GetXaxis()->SetTitleOffset(1.0);
  h[hn][nn]->GetXaxis()->SetLabelSize(0.035);
  h[hn][nn]->GetXaxis()->SetLabelOffset(0.013);

  h[hn][nn]->GetXaxis()->SetRangeUser(50,3200);
  h[hn][nn]->GetYaxis()->SetRangeUser(0.8,ymax);

  h[hn][nn]->GetXaxis()->SetTitleFont(42);
  h[hn][nn]->GetXaxis()->SetLabelFont(42);
  h[hn][nn]->GetYaxis()->SetTitleFont(42);
  h[hn][nn]->GetYaxis()->SetLabelFont(42);
  h[hn][nn]->SetLineColor(colornum[0]);
  //h[hn][nn]->SetLineWidth(2);
  h[hn][nn]->Draw();
  
  l = new TLegend(0.68, 0.72, 0.98, 0.97);
  l->AddEntry(h[hn][nn], "Total spectrum", "L");
  for(int i=0;i<nn;i++){
    h[hn][i]->Rebin(bin);
    //h[hn][i]->SetLineColor(i%5+2);
    h[hn][i]->SetLineColor(colornum[nn-i]);
    h[hn][i]->SetLineStyle(i/5);
    h[hn][i]->Draw("same");
    //cout<<hn<<" "<<i<<endl;
    l->AddEntry(h[hn][i], Form("%i keV state",Ene[i]), "L");
  }
  l->SetTextSize(0.04);
  l->SetBorderSize(0);
  l->SetFillColor(0);
  l->Draw();
  
}
*/
