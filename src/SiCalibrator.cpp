// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: SiCalibrator.cpp
// Purpose: Calibrates silicon detectors using known sources
 

#ifndef SI_CALIBRATOR_CPP__
#define SI_CALIBRATOR_CPP__

#include "SiCalibrator.hh"
#include "TError.h"

double fitf(double* x, double* par);
double calcchisquare(TF1* f, TH1D* h, int lo, int hi);
double minimize(TF1* f, TH1D* h, int par, double lo, double hi);

/*************************************************************************/
//                            Constructor
/*************************************************************************/
SiCalibrator::SiCalibrator() {
	detector.resize(MAXPIX);
	CalData.resize(CalibSource::CalibSource::sourcelist.size());
}

SiCalibrator::SiCalibrator(int th, int d, int s, int t) {
	detector.resize(MAXPIX);
	CalData.resize(CalibSource::CalibSource::sourcelist.size());
	thresh = th;
	decay = d;
	shaping = s;
	top = t;
}


/*************************************************************************/
//                              Destructor
/*************************************************************************/
SiCalibrator::~SiCalibrator() {
}


/*************************************************************************/
//                             DefineRunLog
/*************************************************************************/
void SiCalibrator::DefineRunLog(vector<int> rlist, vector<int> tlist) {
	DefineRunLog(rlist,tlist,tlist);
}

void SiCalibrator::DefineRunLog(vector<int> rlist, vector<int> tlist1, vector<int> tlist2) {
	runlist.resize(0);
	if (rlist.size() != tlist1.size() || rlist.size() != tlist2.size() ) {
		cout << "Error: size mismatch" << endl;
		return;
	}
	for (int i=0;i<rlist.size();i++) {
		int sourcerun = 0;
		
	}
	runlist.resize(rlist.size());
	for (int i=0;i<rlist.size();i++) {
		runlist[i].filenum = rlist[i];
		runlist[i].type[0] = tlist1[i];
		runlist[i].type[1] = tlist2[i];
	}
}

/*************************************************************************/
//                             BuildHists
/*************************************************************************/
void SiCalibrator::BuildHists(TrapTreeFile &trapf) {
	char fname[255];
	sprintf(fname,"SumCalData_th%dd%ds%dt%d",thresh,decay,shaping,top);
	TFile* myfile = new TFile(fname,"RECREATE");
	for (int run=0;run<runlist.size();run++) {
		myfile->cd();
		for (int det=0;det<2;det++) {
			int src = runlist[run].type[det];
			if (src >= 0 && src < CalData.size()) {
				if (CalData[src].hSource == 0) {
					char name[255];
					//name.append(std::to_string(ch));
					sprintf(name,"h%s",CalibSource::sourcelist[src].name.c_str());
					CalData[src].hSource = new TH2D(name,name,MAXPIX,0,MAXPIX,MAXADCVAL,0,MAXADCVAL);
				}
			}
		}
		if (!trapf.Open(runlist[run].filenum,decay,shaping,top)) {
			//-----To do: replay missing files
			cout << "Error in runlist, no file " << runlist[run].filenum << endl;
			return;
		}
		for (int ev = 0; ev < trapf.GetNumEvents(); ev++ ) {
			trapf.GetEvent(ev);
			int ch = trapf.Trap_event.ch;
			if (ch < detector.size()) {
				int src = runlist[run].type[detector[ch]];
				if (src < CalData.size()) {
					myfile->cd();
					CalData[src].hSource->Fill(ch,trapf.Trap_event.AveE);
				}
			}
		}
	}
	trapf.Close();
	myfile->cd();
	for (int src=0; src<CalData.size(); src++) {
		if (CalData[src].hSource != 0) {
			CalData[src].hSource->Write();
		}
	}
	myfile->Close();
	return;
}

/*************************************************************************/
//                              FindPeaks
/*************************************************************************/
void SiCalibrator::FindPeaks() {
	char fname[255];
	sprintf(fname,"SumCalData_th%dd%ds%dt%d",thresh,decay,shaping,top);
	TFile* myfile = new TFile(fname);
		for (int src=0; src < CalData.size(); src++) {
			char name[255];
			sprintf(name,"h%s",CalibSource::sourcelist[src].name.c_str());
			CalData[src].hSource = (TH2D*)myfile->Get(name);
			CalData[src].sourcedata.resize(MAXPIX);
		}
	int iterate = 25;
	int nbins;
	TSpectrum* spec = new TSpectrum(10);
	avesigma = 0;
	int nsigma = 0;
	for (int src=0; src < CalData.size(); src++) {
		if (CalData[src].hSource != 0) {
			for (int ch=0; ch<CalData[src].sourcedata.size(); ch++) {
				TH1D* hbi = CalData[src].hSource->ProjectionY("hb",ch+1,ch+1);
				nbins = hbi->GetNbinsX();
				char name[255];
				sprintf(name,"h%sCh%d",CalibSource::sourcelist[src].name.c_str(),ch);
				CalData[src].sourcedata[ch].hdata = new TH1D(name,name,nbins,0,nbins);
				TH1D* hbk1 = CalData[src].sourcedata[ch].hdata;
				hbk1 = (TH1D*) spec->Background(hbi,iterate);
				hbi->Add(hbk1,-1);
				//Estimate parameters
				double lim = 5;
				double max = 0;
				double peak = 0;
				int i = nbins - 1;
				while (hbi->GetBinContent(i) < lim && i > 0) {
					max = i;
					i--;
				}
				double sigma = (max/CalibSource::sourcelist[src].betas.back().E)*2; //2 keV
				if (sigma > 1) {
					avesigma += sigma;
					nsigma++;
				}
				gErrorIgnoreLevel = kError;
				int npeaks = spec->Search(hbi,sigma,"nodraw ",0.001);
				float* adc = spec->GetPositionX();
				float* amp = spec->GetPositionY();
				for (int i=0; i<npeaks; i++) {
					CalData[src].sourcedata[ch].ADC.push_back(adc[i]);
					CalData[src].sourcedata[ch].Amp.push_back(amp[i]);
				}
			}
		}
	}
	avesigma = avesigma/nsigma;
}

/*************************************************************************/
//                             MatchPeaks
/*************************************************************************/
void SiCalibrator::MatchPeaks() {
	int ch = 1; int src = 1;

	char fname[255];
	sprintf(fname,"SumCalData_th%dd%ds%dt%d",thresh,decay,shaping,top);
	TFile* myfile = new TFile(fname);
	for (int src=0; src < CalData.size(); src++) {
		char name[255];
		sprintf(name,"h%s",CalibSource::sourcelist[src].name.c_str());
		CalData[src].hSource = (TH2D*)myfile->Get(name);
		CalData[src].sourcedata.resize(MAXPIX);
	}
	int nbins;
	for (int src=0; src < CalData.size(); src++) {
		if (CalData[src].hSource != 0) {
			for (int ch=0; ch<CalData[src].sourcedata.size(); ch++) {
				TH1D* hbi = CalData[src].hSource->ProjectionY("hb",ch+1,ch+1);
				nbins = hbi->GetNbinsX();
				char name[255];
				sprintf(name,"h%sCh%d",CalibSource::sourcelist[src].name.c_str(),ch);
				CalData[src].sourcedata[ch].hdata = new TH1D(name,name,nbins,0,nbins);
				TH1D* hi = CalData[src].sourcedata[ch].hdata;
				TF1* f = new TF1("fitf",fitf,0,16000,4);
				CalData[src].sourcedata[ch].fpol1 = f;
				// Build spectrum fit function
				f->FixParameter(0,src);
				f->FixParameter(1,0);
				//f->SetParLimits(1,-30,30);
				f->SetParameter(2,0.2);
				f->SetParameter(3,1);
				f->SetNpx(1000);
				double scale = hbi->GetMaximum()/f->GetMaximum();
				f->FixParameter(3,scale);
				f->SetParameter(2,minimize(f,hbi,2,0.1,0.25));
				f->ReleaseParameter(2);
				f->ReleaseParameter(1);
				//hbi->Draw();
				//hbi->Fit(f,"LL","",thresh+50,16000);
				//cout << f->GetChisquare()/f->GetNDF() << endl;
				f->FixParameter(1,f->GetParameter(1));
				f->FixParameter(2,f->GetParameter(2));
				f->SetParameter(3,minimize(f,hbi,3,scale*0.1,scale*5));
				f->ReleaseParameter(3);
				f->SetParLimits(3,scale*0.1,scale*5);
				//hbi->Fit(f,"LL","",thresh+50,16000);
				//cout << f->GetChisquare()/f->GetNDF() << endl;
				//hbi->Fit(f,"","",150,16000);
				//f->Draw("same");
			}
		}
	}
	TH1D* hp1 = new TH1D("hp1","hp1",600,0,0.25);
	for (int src=0; src < CalData.size(); src++) {
		if (CalData[src].hSource != 0) {
			for (int ch=0; ch<CalData[src].sourcedata.size(); ch++) {
				TF1* f = CalData[src].sourcedata[ch].fpol1;
				hp1->Fill(f->GetParameter(2));
			}
		}
	}
	hp1->Draw();
}

double fitf(double* x, double* par) {
	double xval = *x;
	double src = par[0];
	double calib0 = par[1];
	double calib1 = par[2];
	double scale = par[3];
	xval = xval*calib1 + calib0;
	int lowE = 10;
	if (src <= 1) lowE = 25;
	if (xval < lowE) {TF1::RejectPoint();}  //don't overthink near threshold
	if (xval > 900) TF1::RejectPoint(); //account for ADC saturation
	double value = 0;
	for (int i=0; i<CalibSource::sourcelist[src].betas.size(); i++) {
		double amp = CalibSource::sourcelist[src].betas[i].branch;
		double adc = CalibSource::sourcelist[src].betas[i].E;
		double sigma = 2;
		if (adc < 40) {
			adc = 0.7*adc; //Grab real numbers from Penelope at some point
			sigma = 4;
		}
		else if (adc < 60){
			adc = 0.9*adc; //Grab real numbers from Penelope at some point
			sigma = 4;
		}
		//else if (adc < 200)
		//	adc = 0.9*adc;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	double eff = 0.6; // Grab real efficiency from NIST at some point
	for (int i=0; i<CalibSource::sourcelist[src].xrays.size(); i++) {
		double amp = CalibSource::sourcelist[src].xrays[i].branch;
		double adc = CalibSource::sourcelist[src].xrays[i].E;
		double sigma = 2;
		if (adc > 50)
			eff = eff*0.2;
		value += eff*amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	if (src <= 1) //add a bkgd for 207Bi
		value += 0.25*scale*TMath::Exp(-1.*xval/50);
	return value*scale;
}

double calcchisquare(TF1* f, TH1D* h, int lo, int hi) {
	double val = 0;
	for (int i=lo;i<=hi;i++) {
		double hc = h->GetBinContent(i);
		double xc = h->GetXaxis()->GetBinCenter(i);
		TF1::RejectPoint(kFALSE);
		double fc = f->Eval(xc);
		double ec = 1;
		if (hc > 0)
			ec = hc;
		if (!TF1::RejectedPoint())
			val += (hc - fc) * (hc - fc) / ec;
	}
	return val;
}

//ROOT won't minimize
double minimize(TF1* f, TH1D* h, int par, double lo, double hi) {
	double chimin = -1; 
	double pmin;
	for (double p=lo;p<hi;p=p+(hi-lo)/300) {
		f->SetParameter(par,p);
		double cs = calcchisquare(f,h,0,16000);
		if (chimin == -1 || chimin > cs) {
			chimin = cs;
			pmin = p;
		}
	}
	return pmin;
}

#endif // SI_CALIBRATOR_CPP__

