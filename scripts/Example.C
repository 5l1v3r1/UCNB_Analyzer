// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// Example script for use in ROOT with typical analysis routines that haven't made their way into the main program. Warning: may contain lazy and inefficient programming; viewer discretion is advised.

FitTreeFile ttf;
TrapTreeFile trapf;
WaveformAnalyzer wf;
RawTreeFile rtf;

int shaping = 300, top = 300, decay = 200;
double tsmp = 4.e-9;
int abslo = 350; int abshi = 6000;

//Plot a waveform and pulse fit function
void PlotWave(int ev, int run) {
	rtf.SetPath("Files/Sources");
	rtf.Open(run); 
	rtf.GetEvent(ev);
	wf.MakeTrap(rtf.NI_event.length,rtf.NI_event.wave);
	wf.Plot();
	double thresh = 50;
	vector<trigger_t> triglist; 
	wf.FitWave(thresh,triglist);
	//wf.GetTriggers(thresh,triglist);
	for (int i=0;i<triglist.size();i++) {
		cout << "E,T = " << triglist[i].TrapE << ", " << triglist[i].TrapT << endl;
	}
	wf.PlotTrap();
	wf.wavefit->Draw("same");
	double amp, mean, tau, integ, chi2;
	wf.GetFitVals(amp, mean, tau, integ, chi2);
	cout << "parameters: " << endl;
	cout << amp << endl << mean << endl << tau << endl << integ << endl << chi2 << endl;
}

//Plot a waveform given trigger event
void PlotEventWave(int ev, int run) {
	ttf.Open(run);
	ttf.GetEvent(ev);
	cout << "E,T = " << ttf.Fit_event.trapE << ", " << ttf.Fit_event.trapT << endl;
	PlotWave(ttf.Fit_event.waveev, run);
}

//Fit a peak in an energy spectrum
void FitPeak(int run, int decay, int shaping, int top) {
	TH1D* hCe = (TH1D*) gROOT->FindObject("hCe");
	if (hCe != 0) delete hCe;
	hCe = new TH1D("hCe","hCe",3000,0,6000);

	trapf.SetPath("Files/Dec15/Fixed");
	if (!trapf.Open(run,decay,shaping,top)) return;
	for (int i = 0; i < trapf.GetNumEvents(); i++) {
		trapf.GetEvent(i);
		if (trapf.Trap_event.ch == 9 && trapf.Trap_event.AveE > 0) {
			hCe->Fill(trapf.Trap_event.AveE);
		}
	}
	gROOT->cd();
	hCe->GetXaxis()->SetRangeUser(abslo,abshi); //avoid low energy stuffs
	hCe->Draw();
	double max = hCe->GetMaximum();
	int binmax = hCe->GetMaximumBin();
	double xmax = hCe->GetXaxis()->GetBinCenter(binmax);
	TF1* floo = (TF1*) gROOT->FindObject("floo");
	if (floo != 0) delete floo;
	floo = new TF1("floo", floof, 0, 6000, 7);
	floo->SetParameters(max,xmax,10,max*0.1,50,20,5);
	floo->SetParLimits(0,0,2*max);
	floo->SetParLimits(3,0,2*max);
	floo->SetParLimits(4,0,500);
	floo->SetParName(2,"sigma");
	hCe->Fit(floo,"Q","",1000,xmax+60);
	double mean, fwhm;
	double lo, hi;
	GetFWHM(floo,mean,fwhm);
	lo = mean-8.5*fwhm; hi = mean+3*fwhm;
	if (lo < abslo) lo = abslo;
	ReFit(lo,hi);
	GetFWHM(floo,mean,fwhm);
	lo = mean-8.5*fwhm; hi = mean+3*fwhm;
	if (lo < abslo) lo = abslo;
	ReFit(lo,hi);
	GetFWHM(floo,mean,fwhm);
	lo = mean-8.5*fwhm; hi = mean+3*fwhm;
	if (lo < abslo) lo = abslo;
	ReFit(lo,hi);
	GetFWHM(floo,mean,fwhm);
	//draw parts
	TF1* part1 = (TF1*) gROOT->FindObject("part1");
	if (part1 != 0) delete part1;
	part1 = new TF1("part1", gaussian, 0, 6000, 3);
	for (int i=0;i<3;i++)
		part1->SetParameter(i,floo->GetParameter(i));
	part1->SetLineColor(kViolet);
	part1->Draw("same");
	TF1* part2 = (TF1*) gROOT->FindObject("part2");
	if (part2 != 0) delete part2;
	part2 = new TF1("part2", hypermet, 0, 6000, 4);
	part2->SetParameter(0,floo->GetParameter(3));
	part2->SetParameter(1,floo->GetParameter(1));
	part2->SetParameter(2,floo->GetParameter(2));
	part2->SetParameter(3,floo->GetParameter(4));
	part2->SetLineColor(kBlue);
	part2->Draw("same");
	TF1* part3 = (TF1*) gROOT->FindObject("part3");
	if (part3!= 0) delete part3;
	part3 = new TF1("part3", stepbkgd, 0, 6000, 4);
	part3->SetParameter(0,floo->GetParameter(5));
	part3->SetParameter(1,floo->GetParameter(6));
	part3->SetParameter(2,floo->GetParameter(1));
	part3->SetParameter(3,floo->GetParameter(2));
	part3->SetLineColor(kGreen);
	part3->Draw("same");
	fgt->SetLineColor(kBlack);
	fgt->Draw("same");
}
void GetFWHM(TF1* floo, double& mean, double& fwhm) {
	TF1* fgt = (TF1*) gROOT->FindObject("fgt");
	if (fgt != 0) delete fgt;
	fgt = new TF1("fgt", gaustail, 0, 6000, 5);
	for (int i=0;i<5;i++)
		fgt->SetParameter(i,floo->GetParameter(i));
	double mean = fgt->GetMaximumX();
	double max = fgt->GetMaximum();
	double left = fgt->GetX(max/2.,0,mean);
	double right = fgt->GetX(max/2.,mean,6000);
	fwhm = right - left;
	cout << "Found " << mean << " " << fwhm << endl;
}
void ReFit(int lo, int hi) {
	TF1* floo = (TF1*) gROOT->FindObject("floo");
	if (floo == 0) return;
	TH1D* hCe = (TH1D*) gROOT->FindObject("hCe");
	if (hCe == 0) return;
	hCe->GetXaxis()->SetRangeUser(lo,hi);
	double max = hCe->GetMaximum();
	int binmax = hCe->GetMaximumBin();
	double xmax = hCe->GetXaxis()->GetBinCenter(binmax);
	floo->SetParameters(max,xmax,10,max*0.1,50,20,5);
	hCe->Fit(floo,"Q","",lo,hi);
}

//nice fit function: NIM B 129 (1997) 297-299
double floof(Double_t* x, Double_t* par) {
	Double_t xval = *x;
	Double_t amp = par[0];
	Double_t mean = par[1];
	Double_t sigma = par[2];
	Double_t gaussian = amp * TMath::Exp(-(xval-mean)*(xval-mean)/2./sigma/sigma);
	Double_t amp2 = par[3];
	Double_t tau = par[4];
	Double_t s2 = TMath::Sqrt(2.);
	Double_t hypermet = amp2 * TMath::Exp(((xval-mean)/tau) + sigma*sigma/2./tau/tau)*TMath::Erfc(((xval-mean)/s2/sigma)+sigma/s2/tau);
	Double_t A = par[5];
	Double_t B = par[6];
	Double_t stepbkgd = (A/2.)*TMath::Erfc((xval-mean)/s2/sigma) + B;
	return gaussian + hypermet + stepbkgd;
}
double gaussian(Double_t* x, Double_t* par) {
	Double_t xval = *x;
	Double_t amp = par[0];
	Double_t mean = par[1];
	Double_t sigma = par[2];
	Double_t gaussian = amp * TMath::Exp(-(xval-mean)*(xval-mean)/2./sigma/sigma);
	return gaussian;
}
double hypermet(Double_t* x, Double_t* par) {
	Double_t xval = *x;
	Double_t amp2 = par[0];
	Double_t mean = par[1];
	Double_t sigma = par[2];
	Double_t tau = par[3];
	Double_t s2 = TMath::Sqrt(2.);
	Double_t hypermet = amp2 * TMath::Exp(((xval-mean)/tau) + sigma*sigma/2./tau/tau)*TMath::Erfc(((xval-mean)/s2/sigma)+sigma/s2/tau);
	return hypermet;
}
double stepbkgd(Double_t* x, Double_t* par) {
	Double_t xval = *x;
	Double_t A = par[0];
	Double_t B = par[1];
	Double_t mean = par[2];
	Double_t sigma = par[3];
	Double_t s2 = TMath::Sqrt(2.);
	Double_t stepbkgd = (A/2.)*TMath::Erfc((xval-mean)/s2/sigma) + B;
	return stepbkgd;
}
double gaustail(Double_t* x, Double_t* par) {
	Double_t xval = *x;
	Double_t amp = par[0];
	Double_t mean = par[1];
	Double_t sigma = par[2];
	Double_t gaussian = amp * TMath::Exp(-(xval-mean)*(xval-mean)/2./sigma/sigma);
	Double_t amp2 = par[3];
	Double_t tau = par[4];
	Double_t s2 = TMath::Sqrt(2.);
	Double_t hypermet = amp2 * TMath::Exp(((xval-mean)/tau) + sigma*sigma/2./tau/tau)*TMath::Erfc(((xval-mean)/s2/sigma)+sigma/s2/tau);
	return gaussian + hypermet;
}

//Plot coincidence time and amplitude of proton-like events
void TTN(int run) {
	TH1D* hPE = (TH1D*) gROOT->FindObject("hPE");
	if (hPE != 0) delete hPE;
	hPE = new TH1D("hPE","hPE",300,0,15000);
	TH1D* ht = (TH1D*) gROOT->FindObject("ht");
	if (ht != 0) delete ht;
	TH1D* ht = new TH1D("ht","ht",400,-5000,15000);
	ttf.SetPath("Files/Dec15/Fixed");
	ttf.Open(run);
	ttf.GetEvent(0);
	double lasttime = ttf.Fit_event.t;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		double time = ttf.Fit_event.t;
		if (ttf.Fit_event.E>0 && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240) {
			double diff = time - lasttime;
			if (diff < 15000) {
				ht->Fill(diff);
				if (diff > 400)
					hPE->Fill(ttf.Fit_event.trapE);
			}
			lasttime = time;
		}
	
	}
	gROOT->cd();
	ht->SetLineColor(kBlack);
	ht->Draw();
	hPE->SetLineColor(kBlue);
	hPE->Draw("same");
}

// Compare energy spectra/coincidences under different experimental conditions 
void CompareRuns(){
	TCanvas* c1 = new TCanvas();
	TCanvas* c2 = new TCanvas();
	TCanvas* c3 = new TCanvas();
	double thresh = 35;
	double dmin = 400, dmax = 6000; //6000=24us;
	//add 100 us
	//dmin += 25000; dmax += 25000;
	double E0 = 5.37873, E1 = 0.157501; //calibration
	double emax = 6000;
	//HV + UCN on
	c1->cd();
	TH1D* hP = new TH1D("hP","hP",emax/4.,0,emax);
	TH2D* hch = new TH2D("hch","hch",16,0,16,16,0,16);
	TH1D* ht = (TH1D*) gROOT->FindObject("ht");
	if (ht != 0) delete ht;
	TH1D* ht = new TH1D("ht","ht",300,0,15000);
	TH2D* hPT = new TH2D("hPT","hPT",500,0,50,emax/4.,0,emax);
	ttf.SetPath("Files/Dec15/Fixed");
	ttf.Open(27); double endtime = 4*500.;
	ttf.GetEvent(0); double lasttime = ttf.Fit_event.t; int lastch = ttf.Fit_event.chan;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		double time = ttf.Fit_event.t + ttf.Fit_event.trapT;
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {	
			double diff = time - lasttime;
			if (diff < dmax) {
				ht->Fill(diff);
				hPT->Fill(diff*0.004,ttf.Fit_event.trapE*E1 + E0);
			}
			if (diff > dmin && diff < dmax) {//4 to 24 us
				hP->Fill(ttf.Fit_event.trapE*E1 + E0);
				hch->Fill(ttf.Fit_event.chan,lastch);
			}
			lasttime = time;
			lastch = ttf.Fit_event.chan;
		}
	}
	ttf.Open(30); double endtime = 4*510.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t; lastch = ttf.Fit_event.chan;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		double time = ttf.Fit_event.t + ttf.Fit_event.trapT;
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff < dmax){
				ht->Fill(diff);
				hPT->Fill(diff*0.004,ttf.Fit_event.trapE*E1 + E0);
			}
			if (diff > dmin && diff < dmax) {//4 to 24 us
				hP->Fill(ttf.Fit_event.trapE*E1 + E0);
				hch->Fill(ttf.Fit_event.chan,lastch);
			}
			lasttime = time;
			lastch = ttf.Fit_event.chan;
		}
	}
	ttf.Open(33); double endtime = 2250.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t; lastch = ttf.Fit_event.chan;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff < dmax){
				ht->Fill(diff);
				hPT->Fill(diff*0.004,ttf.Fit_event.trapE*E1 + E0);
			}
			if (diff > dmin && diff < dmax) {//4 to 24 us
				hP->Fill(ttf.Fit_event.trapE*E1 + E0);
				hch->Fill(ttf.Fit_event.chan,lastch);
			}
			lasttime = time;
			lastch = ttf.Fit_event.chan;
		}
	}
	ttf.Open(35); double endtime = 1000.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t; lastch = ttf.Fit_event.chan;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff < dmax){
				ht->Fill(diff);
				hPT->Fill(diff*0.004,ttf.Fit_event.trapE*E1 + E0);
			}
			if (diff > dmin && diff < dmax) {//4 to 24 us
				hP->Fill(ttf.Fit_event.trapE*E1 + E0);
				hch->Fill(ttf.Fit_event.chan,lastch);
			}
			lasttime = time;
			lastch = ttf.Fit_event.chan;
		}
	}
	ttf.Open(36); double endtime = 900.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t; lastch = ttf.Fit_event.chan;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff < dmax){
				ht->Fill(diff);
				hPT->Fill(diff*0.004,ttf.Fit_event.trapE*E1 + E0);
			}
			if (diff > dmin && diff < dmax) {//4 to 24 us
				hP->Fill(ttf.Fit_event.trapE*E1 + E0);
				hch->Fill(ttf.Fit_event.chan,lastch);
			}
			lasttime = time;
			lastch = ttf.Fit_event.chan;
		}
	}
	ttf.Open(38); double endtime = 1800.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t; lastch = ttf.Fit_event.chan;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff < dmax){
				ht->Fill(diff);
				hPT->Fill(diff,ttf.Fit_event.trapE*E1 + E0);
			}
			if (diff > dmin && diff < dmax) {//4 to 24 us
				hP->Fill(ttf.Fit_event.trapE*E1 + E0);
				hch->Fill(ttf.Fit_event.chan,lastch);
			}
			lasttime = time;
			lastch = ttf.Fit_event.chan;
		}
	}
	gROOT->cd();
	c3->cd();
	hPT->Draw("COLZ");
	c2->cd();
	hch->Draw("COLZ");
	c1->cd();
	endtime = 4*(500.+510.) + 2250. + 1000. + 900. + 1800.;
	hP->Scale(1./endtime);
	hP->SetLineColor(kRed);
	hP->Draw();
	//HV off, UCN on
	TH1D* hB = new TH1D("hB","hB",emax/4.,0,emax);
	ttf.Open(26); endtime = 4*220.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff > dmin && diff < dmax) //4 to 24 us
				hB->Fill(ttf.Fit_event.trapE*E1 + E0);
			lasttime = time;
		}
	}
	ttf.Open(31); endtime = 4*230.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff > dmin && diff < dmax) //4 to 24 us
				hB->Fill(ttf.Fit_event.trapE*E1 + E0);
			lasttime = time;
		}
	}
	ttf.Open(32); endtime = 4*100.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff > dmin && diff < dmax) //4 to 24 us
				hB->Fill(ttf.Fit_event.trapE*E1 + E0);
			lasttime = time;
		}
	}
	ttf.Open(37); double endtime = 1300.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t; lastch = ttf.Fit_event.chan;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff > dmin && diff < dmax) //4 to 24 us
				hB->Fill(ttf.Fit_event.trapE*E1 + E0);
			lasttime = time;
		}
	}
	gROOT->cd();
	endtime = 4*(220.+230.+100.)+1300.;
	hB->Scale(1./endtime);
	hB->SetLineColor(kGreen);
	hB->Draw("same");
	//HV on, UCN off
	TH1D* hO = new TH1D("hO","hO",emax/4,0,emax);
	ttf.Open(29); endtime = 4*165.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff > dmin && diff < dmax) //4 to 24 us
				hO->Fill(ttf.Fit_event.trapE*E1 + E0);
			lasttime = time;
		}
	}
	ttf.Open(34); endtime = 540.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff > dmin && diff < dmax) //4 to 24 us
				hO->Fill(ttf.Fit_event.trapE*E1 + E0);
			lasttime = time;
		}
	}
	ttf.Open(39); endtime = 2500.;
	ttf.GetEvent(0); lasttime = ttf.Fit_event.t;
	for (int i = 0; i < ttf.GetNumEvents(); i++) {
		ttf.GetEvent(i);
		
		if (ttf.Fit_event.E > thresh && ttf.Fit_event.shaping > 180 && ttf.Fit_event.shaping < 240 && ttf.Fit_event.t*tsmp < endtime && ttf.Fit_event.chan != 2 && ttf.Fit_event.chan < 16) {
			double diff = time - lasttime;
			if (diff > dmin && diff < dmax) //4 to 24 us
				hO->Fill(ttf.Fit_event.trapE*E1 + E0);
			lasttime = time;
		}
	}
	gROOT->cd();
	endtime = 4*(165.)+540+2500;
	hO->Scale(1./endtime);
	hO->SetLineColor(kBlack);
	hO->Draw("same");
}

