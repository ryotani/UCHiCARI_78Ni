const int nn = 4;
int Ene[nn]={5293, 3183, 5243, 3703};
int colornum[6]={1,9,8,6,2,4};
//int Eve[nn]={57600, 27400, 49400, 35700}, bin=5, ymax=990; char *output="fig/p2p_78Ni_100";
int life4=0;
int Eve[nn]={765, 364, 655, 473}, bin=40, ymax=82; char *output=Form("fig/gg_widerange_p2p_78Ni_10d_Sep2019_%ips",life4);
//int life4=0; int Eve[nn]={1000,1000,1000,1000}, bin=20, ymax=42; char *output=Form("fig/p2p_78Ni_1000eve_Aug2019_%ips",life4);
int sumeve=0;
int expeve=(int)(1.02/(1.36e-3)*9./6.); //6day ->10day with 90% livetime
double fraction[3]={1.,5.2/9.4,4.2/9.4}

gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
//gStyle->SetOptTitle(0);
gStyle->SetTextFont(43);
gStyle->SetLegendFont(43);

TFile *f[nn];
char *hist[3]={"egamegamABdc", "MBegamMBegamABdc", "GRegamMBegamABdc"};
//int gatemin[3]={553, 1023, 2500};int gatemax[3]={613, 1183, 2700};
int gatemin[3]={500, 1000, 2300};int gatemax[3]={650, 1250, 2700}, gateene[3]={583,1103,2600};
//TF1 *expf=new TF1("expf","TMath::Exp(-1.36e-3*x)",0,4000);
TF1 *expf=new TF1("expf","4.69115*TMath::Exp(-2.877e-3*x) + 2.21611*TMath::Exp(-3.0347e-4*x)",0,4000);
TH1F *h[3][nn+1], *hcomp[3];
TPad *p[5];
TLegend *l;
TCanvas *c = new TCanvas("c","c",1000,500);
void plot(void);
void plotggp2p(void);
void drawhist(int hn);

void plotggp2p(void){
  plot();
}

void plot(void){
  for(int i=0;i<nn;i++){sumeve+=Eve[i];}
  for(int i=0;i<nn;i++) f[i] = new TFile(Form("hist/p2p_%ikeV_%ievt_%ips.root",Ene[i],Eve[i],life4));
  c->Divide(3,1);
  for(int i=0;i<3;i++) drawhist(i);

  //comparehist();
  c->SaveAs(Form("%s.pdf",output));
  c->SaveAs(Form("%s.png",output));
  c->SaveAs(Form("%s.root",output));
  c->SaveAs(Form("%s.C",output));

  //for(int i=0;i<3;i++)  h[i][nn]->SaveAs(Form("%s_%s.root",output,hist[i]));
}

void drawhist(int hn){
  c->cd(hn+1);
  p[hn] = new TPad(Form("p%i",hn),Form("p%i",hn),0,0,1,1);
  p[hn]->SetTopMargin(0.005);
  p[hn]->SetLeftMargin(0.11);
  p[hn]->SetBottomMargin(0.1);
  p[hn]->SetRightMargin(0.005);
  p[hn]->Draw();
  //p[hn]->SetLogy();
  p[hn]->cd();
  
  //  //hist2=egamegamABdc->ProjectionY("2600_py",egamegamABdc->GetXaxis()->FindBin(2600-200),egamegamABdc->GetXaxis()->FindBin(2600+200))

  for(int i=0;i<nn;i++){
    TH2F* h_tmp = (TH2F*)(f[i]->Get(hist[0]));// hist[hn=0] <-> egamgamABdc
    if(i==0){
      cout<<h_tmp->GetXaxis()->FindBin(gatemin[hn])<<", "<<h_tmp->GetXaxis()->FindBin(gatemax[hn])<<endl;
      h[hn][nn] = (TH1F*)(h_tmp->ProjectionY(Form("proj_%i",(gatemin[hn]+gatemax[hn])/2),h_tmp->GetXaxis()->FindBin(gatemin[hn]),h_tmp->GetXaxis()->FindBin(gatemax[hn])))->Clone(); 
    }
    else{
      h[hn][nn]->Add((TH1F*)(h_tmp->ProjectionY(Form("proj_%i_%i",(gatemin[hn]+gatemax[hn])/2,i),h_tmp->GetXaxis()->FindBin(gatemin[hn]),h_tmp->GetXaxis()->FindBin(gatemax[hn])))); 
    //h[hn][nn]->Add((TH1F*)f[i]->Get(hist[hn]));
    //h[hn][i] = (TH1F*)f[i]->Get(hist[hn])->Clone();

    //h[hn][i]->SetName(Form("hist%i_%s",i,hist[hn]));
    //cout<<Form("hist%i_%s",i,hist[hn])<<endl;
    }
    h[hn][nn]->Add((TH1F*)(h_tmp->ProjectionX(Form("projx_%i_%i",(gatemin[hn]+gatemax[hn])/2,i),h_tmp->GetYaxis()->FindBin(gatemin[hn]),h_tmp->GetYaxis()->FindBin(gatemax[hn])))); 
  }
  h[hn][nn]->SetStats(0);
  h[hn][nn]->SetTitleFont(43);
  //  h[hn][nn]->SetTitleOffset(3);
  h[hn][nn]->GetXaxis()->SetTitleFont(43);
  h[hn][nn]->GetXaxis()->SetLabelFont(43);
  h[hn][nn]->GetYaxis()->SetTitleFont(43);
  h[hn][nn]->GetYaxis()->SetLabelFont(43);
  //  h[hn][nn]->FillRandom("expf", (int)((double)expeve*fraction[hn]));
  h[hn][nn]->SetTitle(Form("Gate on %i keV",(gateene[hn])));
  h[hn][nn]->Rebin(bin/4);
  h[hn][nn]->GetYaxis()->SetTitle(Form("counts / %i keV",bin));
  h[hn][nn]->GetYaxis()->SetTitleSize(15);
  h[hn][nn]->GetYaxis()->SetTitleOffset(1.8);
  h[hn][nn]->GetYaxis()->SetLabelSize(15);
  h[hn][nn]->GetYaxis()->SetLabelOffset(0.01);

  h[hn][nn]->GetXaxis()->SetTitle("E_{#gamma} (keV)");
  h[hn][nn]->GetXaxis()->SetTitleSize(15);
  h[hn][nn]->GetXaxis()->SetTitleOffset(1.5);
  h[hn][nn]->GetXaxis()->SetLabelSize(15);
  h[hn][nn]->GetXaxis()->SetLabelOffset(0.013);

  h[hn][nn]->GetXaxis()->SetRangeUser(80,2800);
  //  h[hn][nn]->GetYaxis()->SetRangeUser(0,ymax/(hn==2?3.:1.));

  h[hn][nn]->SetLineColor(colornum[0]);
  //h[hn][nn]->SetLineWidth(2);
  h[hn][nn]->Draw();
  /*
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
  */
}


void comparehist(void){
  int hn=4;
  c->cd(hn);
  p[hn] = new TPad(Form("p%i",hn),Form("p%i",hn),0,0,1,1);
  p[hn]->SetTopMargin(0.01);
  p[hn]->SetLeftMargin(0.08);
  p[hn]->SetBottomMargin(0.11);
  p[hn]->SetRightMargin(0.01);
  p[hn]->Draw();
  //p[hn]->SetLogy();
  p[hn]->cd();

  //  l = new TLegend(0.68, 0.72, 0.98, 0.97);
  for(int i=0;i<3;i++){
    hcomp[i]= (TH1F*)h[i][nn]->Clone();
    hcomp[i]->SetTitle("Compare total spectra");
    //hcomp[i]->SetLineColor(i%5+2);
    hcomp[i]->SetLineColor(colornum[i]);
    hcomp[i]->SetLineStyle(i/5);
    i==0?hcomp[i]->Draw(""):hcomp[i]->Draw("same");
    //
    l->AddEntry(hcomp[i], hist[i], "L");
  }
  /*
  l->SetTextSize(0.04);
  l->SetBorderSize(0);
  l->SetFillColor(0);
  l->Draw();
  */
}
