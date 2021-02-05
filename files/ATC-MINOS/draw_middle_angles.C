void draw_comparison(char caltrname[200], int printout=0, TH1F* h_Actual, TH1F *h_AddATC, TH1F *h_AddATC_SCout);
//void draw_comparison(char caltrname[200], int printout=0, TH1F h_Actual, TH1F h_AddATC, TH1F h_AddATC_SCout);
//TH1F* draw_original(char caltrname[200]);
//TH1F* draw_ATC(char caltrname[200]);

TString outdir = "files/ATC-MINOS/";
Int_t numbin = 400, min = 200, max = 4200;

void draw_middle_angles(){
  
  //TFile *file = new TFile("p3p_2910keV_2000000evt_0ps.root","READ");
  TH1F *h_Actual = new TH1F("h_Actual", Form("2910 keV with AB (Red: ATC only) ; Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_AddATC = new TH1F("h_AddATC", Form("2910 keV with AB ; Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_AddATC_SCout = new TH1F("h_AddATC_SCout", Form("2910 keV with AB; Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  //draw_comparison(Form("%s/root/p2p_5293keV_765evt_0ps.root",outdir.Data()) , 1, h_Actual, h_AddATC, h_AddATC_SCout);
  draw_comparison(Form("%s/root/p3p_2910keV_100000evt_0ps.root",outdir.Data()) , 1, h_Actual, h_AddATC, h_AddATC_SCout);
}

void draw_comparison(char caltrname[200], int printout, TH1F* h_Actual, TH1F *h_AddATC, TH1F *h_AddATC_SCout){
  TCanvas *c;
  if(printout){
    c = new TCanvas();
    c->SaveAs(Form("%s/fig/draw.pdf[",outdir.Data()));
  }
  TChain *caltr = new TChain("caltr");
  caltr->Add(caltrname);
  TProof::Open("");
  caltr->SetProof();
  TH1F *h_GR = new TH1F("h_GR", Form("2910 keV wo AB  ; Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_SC = new TH1F("h_SC", Form("2910 keV wo AB  ; Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_MB = new TH1F("h_MB", Form("2910 keV wo AB  ; Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_GRAB = new TH1F("h_GRAB", Form("2910 keV with AB  ;Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_ATCAB = new TH1F("h_ATCAB", Form("2910 keV with AB; Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_SCAB = new TH1F("h_SCAB", Form("2910 keV with AB  ;Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_MBAB = new TH1F("h_MBAB", Form("2910 keV with AB  ;Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_MBFWDAB = new TH1F("h_MBFWDAB", Form("2910 keV with AB; Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  /*
  TH1F *h_Actual = new TH1F("h_Actual", Form("2910 keV with AB (Black: Actual geometetry, Red: + ATC - 2 MB); Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_AddATC = new TH1F("h_AddATC", Form("2910 keV with AB (Black: Actual geometetry, Red: + ATC - 2 MB); Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  TH1F *h_AddATC_SCout = new TH1F("h_AddATC_SCout", Form("2910 keV with AB (Black: Actual geometetry, Red: + ATC - 2 MB); Energy / keV; Counts / %i keV", (max-min)/numbin), numbin, min, max);
  */
  h_GR -> SetLineColor(2);
  h_SC -> SetLineColor(1);
  h_MB -> SetLineColor(kBlue);
  //h_GRAB -> SetLineColor(2);
  h_ATCAB -> SetLineColor(2);
  h_SCAB -> SetLineColor(1);
  h_MBAB -> SetLineColor(kBlue);
  
  //
  // Cluster id is in cl# of aeuler + 1?
  caltr->Draw("gretinacalc.fhits.fDCen>>h_GR","","");
  //caltr->Draw("gretinacalc.fhits.fDCen>>h_GR","gretinacalc.fhits.fcluster==12&&gretinacalc.fhits.fcrystal<2","");
  //caltr->Draw("miniballcalc.fhits.fDCen>>h_MB","miniballcalc.fhits.fcluster<=208&&miniballcalc.fhits_ab.fcluster>=207","same");
  //caltr->Draw("miniballcalc.fhits.fDCen>>h_SC","miniballcalc.fhits.fcluster>208","same");
  if(printout){
    c->SaveAs(Form("%s/fig/draw.pdf",outdir.Data()));
  }
  caltr->Draw("gretinacalc.fhits_ab.fDCen>>h_ATCAB","gretinacalc.fhits_ab.fcluster==101","");
  //caltr->Draw("gretinacalc.fhits_ab.fDCen>>h_ATCAB","gretinacalc.fhits_ab.fcluster==12&&gretinacalc.fhits_ab.fcrystal<2","");
  //caltr->Draw("miniballcalc.fhits_ab.fDCen>>h_MBAB","miniballcalc.fhits_ab.fcluster<=208&&miniballcalc.fhits_ab.fcluster>=207","same");
  //caltr->Draw("miniballcalc.fhits_ab.fDCen>>h_SCAB","miniballcalc.fhits_ab.fcluster>208","same");
  if(printout){
    //    c->SaveAs(Form("%s/fig/draw.pdf",outdir.Data()));
  }
  //
  
  caltr->Draw("gretinacalc.fhits_ab.fDCen>>h_GRAB","","");
  //caltr->Draw("miniballcalc.fhits_ab.fDCen>>h_MBFWDAB","miniballcalc.fhits_ab.fcluster<207","");
  //
  //h_Actual -> Add(h_SCAB,1.);//= (TH1F*) h_SCAB->Clone();
  h_Actual -> Add(h_GRAB);
  //h_Actual -> Add(h_MBFWDAB, 0.85);
  //h_AddATC -> Add(h_Actual);//->Clone();
  //
  //h_Actual -> Add(h_MBAB, 0.85);
  h_AddATC -> Add(h_ATCAB);
  h_AddATC_SCout -> Add(h_AddATC); //dummy line
  //h_AddATC_SCout -> Add(h_MBAB, 0.85);
  //h_AddATC_SCout -> Add(h_SCAB, -1.);
  //
  h_Actual -> SetLineColor(1);
  h_AddATC -> SetLineColor(2);
  h_AddATC_SCout -> SetLineColor(kBlue);
  //h_AddATC -> SetTitle(Form("2910 keV with AB (Black: Actual geometetry, Red: + ATC - 2 MB, Blue: + ATC - 2SC); Energy / keV; Counts / %i keV", (max-min)/numbin));
  
  if(printout){
    //h_AddATC_SCout -> Draw("same");
    h_Actual -> Draw();
    h_AddATC -> Draw("same");
    c->SaveAs(Form("%s/fig/draw.pdf",outdir.Data()));
    c->SaveAs(Form("%s/fig/draw.pdf]",outdir.Data()));
  }

  delete caltr ;
  delete h_GR;
  delete h_SC ;
  delete h_MB;
  delete h_GRAB;
  delete h_ATCAB;
  delete h_SCAB;
  delete h_MBAB;
  delete h_MBFWDAB;
}
