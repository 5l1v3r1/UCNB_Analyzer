TrapTreeFile ttf;
bool setup = false;
TF1* fg;
TF1* fgb;
TF1* ff;

void Quick() {
  //  if (!ttf.Open(20,500)) {cout << "File not found" << endl; return;}
  if (setup) return;
  fg = new TF1("fg","gaus",0,4000);
  fgb = new TF1("fgb","pol0(0)+gaus(1)",0,4000);
  fgb->SetParameters(5,55,3150,15);
  ttf.SetPath("/media/leah/UCNS1/UCNB/JuneData");
  ff = new TF1("ff",CeFit,0,8000,5);
  ff->SetParameters(200,0.001,100,3130,15);
}

void Do(int file, int decay, int shape, int top, int ch) {
  Quick();
  ttf.Open(file,decay,shape,top);
  TString cuts = "AveE>0 && ch==";
  cuts+=ch;
  t->Draw("AveE>>h(1600,0,8000)",cuts);
}

void Fits(int low, int high) {
  h->Fit(ff,"LL","",low,high);
  cout << "sig/mean = " << ff->GetParameter(4)/fgb->GetParameter(3) << endl;
  cout << "res sig = " << 481.7*ff->GetParameter(4)/ff->GetParameter(3) << endl;
  h->GetXaxis()->SetRangeUser(low-200,high+200);
  h->Draw();
}


void Sums(int decay, int shape, int top, int ch) {
  Quick();
  int CNT = 20;
  //  int ch = 16;
  gROOT->cd();
  TH1D* h = (TH1D*)gROOT->FindObject("h");
  if (h!=0) delete h;
  h = new TH1D("h","h",1400,0,7000);
  for (int i=0;i<CNT;i++)
    if ((i<2 || i > 6)) { //&&(i!=13)&&(i!=20)&&(i<22)) {
      ttf.Open(i,decay,shape,top);
      if (ttf.IsOpen()) {
	int entries = ttf.GetNumEvents();
	for (int ev=0;ev<entries;ev++) {
	  ttf.GetEvent(ev);
	  if (ttf.Trap_event.AveE > 0 && ttf.Trap_event.ch == ch) {
	    gROOT->cd();
	    h->Fill(ttf.Trap_event.AveE);
	  }
	}
      }
    }
  gROOT->cd();
  h->Draw();
}


Double_t CeFit(Double_t* x, Double_t* par) {
  Double_t xval = *x;
  Double_t exp1 = par[0];
  Double_t exp2 = par[1];
  Double_t Amp1 = par[2];
  Double_t mean1 = par[3];
  Double_t sigma1 = par[4];

  return exp1 * TMath::Exp(-1 * exp2 * xval) + Amp1*TMath::Exp(-1*(xval - mean1)*(xval-mean1)/(2*(sigma1*sigma1)));
}
