void beta_p2p();
void beta_p3p();
TFile *f;

Double_t parabola(Double_t *x, Double_t *p){
  return p[0]*x[0]*x[0] + p[1]*x[0] + p[2];
}
Double_t inverted(Double_t *x, Double_t *p){
  return ( -p[1] - sqrt(p[1]*p[1] - 4*p[0]*(p[2]-x[0])) ) / (2*p[0]);
}
void beta(){
  //TFile *f = new TFile("root/p2p_2600keV_100000evt_0ps.root");
  //beta_p2p();
  f = new TFile("root/p3p_2910keV_100000evt_0ps.root");beta_p3p();
}
void beta_p2p(){
  TCanvas *c = new TCanvas();
  TTree* tr = (TTree*)f->Get("caltr");

  tr->Draw("minos.fbetare:minos.fpos.fZ>>h(150,-270,-120,200,0.5,0.65)","","colz");
  TH2F* h = (TH2F*)gROOT->FindObject("h");
  TProfile* p = (TProfile*)h->ProfileX("hp");
  p->Draw("same");
  //h->ProfileY()->Draw();
  TF1* fu = new TF1("fu",inverted,-300,-40,3);
  fu->SetParameters(-3000,2500,-500);
  p->Fit("fu");
  fu->Draw("same");
  c->SaveAs("fig/MINOSbeta_p2p.png");
  c->SaveAs("fig/MINOSbeta_p2p.C");
  c->SaveAs("fig/MINOSbeta_p2p.pdf");
  c->SaveAs("fig/MINOSbeta_p2p.root");
  /*
  c->SaveAs("fig/MINOSbeta_p3p.png");
  c->SaveAs("fig/MINOSbeta_p3p.C");
  c->SaveAs("fig/MINOSbeta_p3p.pdf");
  c->SaveAs("fig/MINOSbeta_p3p.root");
*/
    }
void beta_p3p(){
  TCanvas *c = new TCanvas();
  TTree* tr = (TTree*)f->Get("caltr");

  tr->Draw("minos.fbetare:minos.fpos.fZ>>h(150,-270,-120,200,0.5,0.65)","","colz");
  TH2F* h = (TH2F*)gROOT->FindObject("h");
  TProfile* p = (TProfile*)h->ProfileX("hp");
  p->Draw("same");
  //h->ProfileY()->Draw();
  TF1* fu = new TF1("fu",inverted,-300,-40,3);
  fu->SetParameters(-3000,2500,-500);
  p->Fit("fu");
  fu->Draw("same");
  c->SaveAs("fig/MINOSbeta_p3p.png");
  c->SaveAs("fig/MINOSbeta_p3p.C");
  c->SaveAs("fig/MINOSbeta_p3p.pdf");
  c->SaveAs("fig/MINOSbeta_p3p.root");
}
void testbeta(){
  TTree* tr = (TTree*)f->Get("caltr");

  tr->Draw("minos.fbetare:minos.fpos.fZ>>h(150,-270,-120,200,0.5,0.65)","","colz");
  //  TEnv* set = new TEnv("minos.set");
  //TEnv* set = new TEnv("input/p2pana.set");
  TEnv* set = new TEnv("input/p3pana.set");
  double fMINOSBetaCoeff[3];
  TF1* fMINOSZett = new TF1("MINOSZett",inverted, -300,  -50, 3);
  for(int i=0;i<3;i++){
    fMINOSBetaCoeff[i] = set->GetValue(Form("MINOS.Beta.Coefficient.%d",i),0.0);
    cout << fMINOSBetaCoeff[i] << endl;
    fMINOSZett->SetParameter(i,fMINOSBetaCoeff[i]);
  }

  fMINOSZett->Draw("same");
}
