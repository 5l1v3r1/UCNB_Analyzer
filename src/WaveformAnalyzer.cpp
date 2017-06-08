// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: WaveformAnalyzer.cpp
// Purpose: Analyzes waveforms for energy and timing

#ifndef WAVEFORM_ANALYZER_CPP__
#define WAVEFORM_ANALYZER_CPP__

#include "WaveformAnalyzer.hh"
#include "TVectorD.h"
#include "TMath.h"

double WaveFit(double* xx, double* par);

/*************************************************************************/
//                            Constructor
/*************************************************************************/
WaveformAnalyzer::WaveformAnalyzer() {
  g = 0; 
  gTrap = 0;
  ds = 0;
  f = 0;
  h = 0;
  ch = 0;
  numavebins = 250;
  numavevals = 3000;
  BinAve.resize(numavebins);
  for (int i=0;i<numavebins;i++)
    BinAve[i].resize(numavevals);
  wavefit = new TF1("wavefit",WaveFit,0,MAXWAVE,4);
  wavefit->SetParLimits(0,-10000,10000);
  wavefit->SetParLimits(1,0,MAXWAVE);
  wavefit->SetParLimits(2,10,3000);
  wavefit->SetParLimits(3,0.1,5.0);
  decaytime = 250;//270;//247; - calibrate
  risetime = 250;
  top = 400;
  risetop = top+risetime;
  pretrigger = 500;
//
  dodraw = true;
  setup = false;
}

/*************************************************************************/
//                             Destructor
/*************************************************************************/
WaveformAnalyzer::~WaveformAnalyzer() {
  if (g!=0) delete g;
  if (ds!=0) delete ds;
  if (h!=0) delete h;
  if (f!=0) delete f;
  if (wavefit!=0) delete wavefit;
}

/*************************************************************************///                              BuildAve
/*************************************************************************/
void WaveformAnalyzer::BuildAve(Int_t wvlen, Short_t* wv) {
  wavelen = wvlen;
  if (wave.size() < wavelen) {
    wave.reserve(wavelen);
  }
  if (wave.size() > wavelen) {
    wave.resize(wavelen);
  }
  for (int smp = 0; smp < wavelen; smp++)
    wave[smp] = wv[smp];
  setup = true;

  BaselineShift();

  for (Int_t bin=0;bin<numavebins;bin++){
    Int_t smp = 750 + bin*2;
    Short_t val = wave[smp];
    Short_t whatval = val + 50;
    if (whatval > 0 && whatval < numavevals)
      BinAve[bin][whatval]++;
  }
}
/*************************************************************************/
//                             ReturnAve
/*************************************************************************/
void WaveformAnalyzer::ReturnAve(vector<Double_t>& average){
  average.resize(numavebins);
  for (Int_t bin = 0; bin < BinAve.size(); bin++) {
    average[bin] = 0;
    double norm = 0;
    for (Int_t val = 0; val < BinAve[bin].size(); val++) 
      if (BinAve[bin][val]>0) {
	average[bin] += val * BinAve[bin][val];
	norm += BinAve[bin][val];
      }
    average[bin] = average[bin]/norm;
    average[bin] = average[bin]-50;
  }
}

/*************************************************************************/
//                              MakeTrap
/*************************************************************************/
void WaveformAnalyzer::MakeTrap(Int_t wvlen, Short_t* wv) {
  wavelen = wvlen;
  if (wave.size() < wavelen) {
    wave.reserve(wavelen);
    s_trap.reserve(wavelen);
    p_trap.reserve(wavelen);
  }
  if (wave.size() > wavelen) {
    wave.resize(wavelen);
    s_trap.resize(wavelen);
    p_trap.resize(wavelen);
  }
  for (int smp = 0; smp < wavelen; smp++)
    wave[smp] = wv[smp];
  setup = true;

  BaselineShift();

  s_trap.resize(wavelen);
  p_trap.resize(wavelen);
  p_trap[0]=0; s_trap[0]=0;
  for (int smp = 1; smp < wavelen; smp++) {
    p_trap[smp]=p_trap[smp-1]+DelaySubtract(smp);
    s_trap[smp]=s_trap[smp-1]+p_trap[smp]+decaytime*DelaySubtract(smp);
  }
  //Renormalize
  for (int smp = 0; smp < wavelen; smp++) {
    s_trap[smp] = s_trap[smp]/(decaytime*risetime);
  }
}

/*************************************************************************/
//                              FitWave 
/*************************************************************************/
void WaveformAnalyzer::FitWave(Long64_t thresh, vector<Double_t> &E, vector<Double_t> &T)
{
  GetTriggers(thresh,E,T);
  if (E.size() ==0 ) return;
  //get largest, ignore rest
  double max = -1;
  int maxi = -1;
  for (int i=0;i<E.size();i++) {
    if (max < TMath::Abs(E[i])) {
      max = TMath::Abs(E[i]);
      maxi = i;
    }
  }
  double maxT = T[maxi];
  E.resize(1); T.resize(1);
  int mean = maxT;
  if (mean < 0) mean = 0;  
  wavefit->SetParameters(wave[mean], mean, 150, 0.8);
  Plot();
  g->Fit(wavefit,"QN","",0,wave.size());
  E[0] = wavefit->GetParameter(0);
  T[0] = wavefit->GetParameter(1);
#if defined (__CINT__)
  wavefit->Draw("same");
#endif
#if defined (__ROOTCLING__)
  wavefit->Draw("same");
#endif
#if defined (__CLING__)
  wavefit->Draw("same");
#endif
}

void WaveformAnalyzer::FitWave(Long64_t thresh, vector<trigger_t> &triglist) {
  triglist.resize(0);
  vector<Double_t> E; 
  vector<Double_t> T;
  GetTriggers(thresh,E,T);
  if (E.size() ==0 ) return;
  //for now get largest, ignore rest
  double max = -1;
  int maxi = -1;
  for (int i=0;i<E.size();i++) {
    if (max < TMath::Abs(E[i])) {
      max = TMath::Abs(E[i]);
      maxi = i;
    }
  }
  double maxT = T[maxi];
  int mean = maxT;
  if (mean < 0) mean = 0;  
  wavefit->SetParameters(wave[mean], mean, 150, 0.8);
  Plot();
  g->Fit(wavefit,"QN","",0,wave.size());

  triglist.resize(1);
  triglist[0].TrapE = max;
  triglist[0].TrapT = maxT;
  triglist[0].E = wavefit->GetParameter(0);
  triglist[0].T = wavefit->GetParameter(1);
  triglist[0].Shaping = wavefit->GetParameter(2);
  triglist[0].Integration = wavefit->GetParameter(3);
  triglist[0].Chi2 = wavefit->GetChisquare();

}

/*************************************************************************/
//                            GetTriggers
/*************************************************************************/
void WaveformAnalyzer::GetTriggers(Long64_t thresh, vector<Double_t> &E, vector<Double_t> &T)
{
	E.resize(0);
	T.resize(0);
	if (!setup) return;
	if (!CheckBaseline()) return;
	double min, max;
	int minloc, maxloc;
	short start=0, stop=0;
	for (int smp = 0; smp < wavelen; smp++) {
		double sign = (s_trap[smp] >= 0) ? 1. : -1.;
		if (s_trap[smp]*sign>thresh && s_trap[smp-1]*sign<=thresh) {
			start=smp;
			maxloc=start;
			max=s_trap[smp]*sign;
			smp++;
			while(s_trap[smp]*sign>=thresh && smp<wavelen) {
				if (max < s_trap[smp]*sign) {
					max = s_trap[smp]*sign;
					maxloc = smp;
				}
				smp++;
			}
			stop=smp;
			if(stop - start > top*0.75) {
				max=max*sign;
				E.push_back(max);
				T.push_back(maxloc);
			}
		}
	}

}

/*************************************************************************/
//                            GetTriggers
/*************************************************************************/
void WaveformAnalyzer::GetTriggers(Long64_t thresh, vector<trigger_t> &triglist) {
	trigger_t thetrig;
  
	triglist.resize(0);
	if (!setup) return;
	if (!CheckBaseline()) return;
	double min, max;
	int minloc, maxloc;
	short start=0, stop=0;
	for (int smp = 0; smp < wavelen; smp++) {
		double sign = (s_trap[smp] >= 0) ? 1. : -1.;
		if (s_trap[smp]*sign>thresh && s_trap[smp-1]*sign<=thresh) {
			start=smp;
			maxloc=start;
			max=s_trap[smp]*sign;
			smp++;
			while(s_trap[smp]*sign>=thresh && smp<wavelen) {
				if (max < s_trap[smp]*sign) {
					max = s_trap[smp]*sign;
					maxloc = smp;
				}
				smp++;
			}
			stop=smp;
			if(stop - start > top*0.75) {
				int mid = start + risetime + top/2.;
				//flat top
				thetrig.Flat0 = 0;
				double cc =0;
				for (int j=mid-top*0.75/2.;j<mid+top*0.75/2.&&j<wavelen;j++) {
					thetrig.Flat0 += s_trap[j] - s_trap[j-1];
					cc++;
				}
				thetrig.Flat0 = thetrig.Flat0/cc;
				//overshoot
				thetrig.Flat1 = 0;
				cc =0;
				for (int j=mid+top/2.+risetime;j<wavelen;j++) {
					thetrig.Flat1 += s_trap[j] - s_trap[j-1];
					cc++;
				}
				thetrig.Flat1 = thetrig.Flat1/cc;
				max=max*sign;
				thetrig.up = start;
				thetrig.down = stop;
				thetrig.E = max;
				thetrig.AveTrapE = 0;
				for (int j=start;j<stop;j++)
					thetrig.AveTrapE += s_trap[j];
				thetrig.AveTrapE = thetrig.AveTrapE/(stop-start);
				thetrig.MidTrapE = s_trap[(stop+start)/2.];
				thetrig.TrapE = max;
				thetrig.T = maxloc;
				thetrig.TrapT = maxloc;
				triglist.push_back(thetrig);
			}
		}
	}
	/*
	short up, down;
	for (int smp = 0; smp < wavelen; smp++) {
		if(s_trap[smp]<=-1.*thresh && s_trap[smp-1]>-1.*thresh) {
			down=smp;
			minloc = down;
			min = s_trap[smp];
			smp++;
			while(s_trap[smp]<=-1.*thresh && smp<wavelen) {
				if (min > s_trap[smp]) {
					min = s_trap[smp];
					minloc = smp;
				}
			smp++;
			}
			up=smp;
			if(up != 0) {
				thetrig.up = up;
				thetrig.down = down;
				thetrig.E = min;
				thetrig.AveTrapE = min;
				thetrig.MidTrapE = min;
				thetrig.TrapE = min;
				thetrig.T = minloc;
				thetrig.TrapT = minloc;
				triglist.push_back(thetrig);
			}
		}
		if(s_trap[smp]>=thresh && s_trap[smp-1]<thresh) {
			up=smp;
			maxloc = up;
			max = s_trap[smp];
			smp++;
			while(s_trap[smp]>=thresh && smp<wavelen) {
				if (max < s_trap[smp]) {
					max = s_trap[smp];
					maxloc = smp;
				}
				smp++;
			}
			down=smp;
			if(down - up > top) { //To Be Tested
				//cout << "Start point " << up << " and " << down << endl;
				thetrig.up = up;
				thetrig.down = down;
				//Find the flat
				double newthresh = 0.75*max;
				short newup = maxloc;
				short newdown = maxloc;
				while(s_trap[newup]>=newthresh && newup > 0)
					newup--;
				while(s_trap[newdown]>=newthresh && newdown<wavelen-1)
					newdown++;
				thetrig.TrapT = (newup+newdown)/2.;
				thetrig.T = thetrig.TrapT;
				newup = thetrig.TrapT - 0.9*top/2.;
				newdown = thetrig.TrapT + 0.9*top/2.;
				double mean = 0;
				for (int i=newup;i<newdown;i++)
					mean += s_trap[i];
				mean = mean/(newdown - newup);
				thetrig.TrapE = max;
				thetrig.AveTrapE = mean;
				thetrig.MidTrapE = s_trap[(newup+newdown)/2];
				PlotTrap();
				if (f != 0) delete f;
				f = new TF1("f","pol1",newup,newdown);
				gTrap->Fit(f,"QN","",newup,newdown);
				thetrig.Flat0 = f->GetParameter(0);
				thetrig.Flat1 = f->GetParameter(1);
#if defined (__CINT__)
				cout << "Found " << newup << ", " << newdown << endl;
				cout << "Ave: " << thetrig.AveTrapE << endl;
				cout << "Mid: " << thetrig.MidTrapE << endl;
				cout << "Max: " << thetrig.TrapE << endl;
#endif
				triglist.push_back(thetrig);
			}
		}
	}
	*/
}

/*************************************************************************/
//                            BaselineShift
/*************************************************************************/
void WaveformAnalyzer::BaselineShift() {
  if (!setup) return;
  double baseline = 0;
  for (int smp = 0; smp < pretrigger; smp++) {
    baseline += wave[smp];
  }
  baseline = baseline/pretrigger;
  for (int smp = 0; smp < wavelen; smp++) {
    wave[smp] -= baseline;
  }  
}

/*************************************************************************/
//                            CheckBaseline
/*************************************************************************/
bool WaveformAnalyzer::CheckBaseline() {
  if (!setup) return false;
  Plot();
  if (f != 0) delete f;
  f = new TF1("f","pol1",0,pretrigger);
  g->Fit(f,"QN","",0,pretrigger);
  double slope = f->GetParameter(1);

#if defined (__CINT__)
  cout << slope << endl;
#endif 
#if defined (__ROOTCLING__)
  cout << slope << endl;
#endif 
#if defined (__CLING__)
  cout << slope << endl;
#endif 
  if (slope > 0.5 || slope < -0.5)
    return false;
  return true;
}

/*************************************************************************/
//                                Plot
/*************************************************************************/
void WaveformAnalyzer::Plot() {
  if (!setup) return;
  TVectorD x(wavelen);
  TVectorD y(wavelen);
  for (int i=0;i<wavelen;i++) {
    x[i] = i;
    y[i] = wave[i];
  }
  if (g!=0)
    delete g;
  g = new TGraph(x,y);
  g->SetMarkerStyle(20);
  g->GetXaxis()->SetLimits(-0.5,wavelen-0.5);
  //g->GetYaxis()->SetRangeUser(2500,4000);
#if defined (__CINT__)
  g->Draw("AP");
#endif
#if defined (__ROOTCLING__)
  g->Draw("AP");
#endif
#if defined (__CLING__)
  g->Draw("AP");
#endif
}

/*************************************************************************/
//                               PlotDS
/*************************************************************************/
void WaveformAnalyzer::PlotDS() {
  if (!setup) return;
  TVectorD x(wavelen);
  TVectorD y(wavelen);
  for (int i=0;i<wavelen;i++) {
    x[i] = i;
    y[i] = (double)DelaySubtract(i);
  }
  if (ds!=0)
    delete ds;
  ds = new TGraph(x,y);
  ds->SetMarkerStyle(20);
  ds->GetXaxis()->SetLimits(-0.5,wavelen-0.5);
  ds->SetLineColor(kBlue);
  //  ds->GetYaxis()->SetRangeUser(0,8192);
  ds->Draw("P");
}

/*************************************************************************/
//                               PlotTrap
/*************************************************************************/
void WaveformAnalyzer::PlotTrap() {
  if (!setup) return;
  TVectorD x(wavelen);
  TVectorD y(wavelen);
  for (int i=0;i<wavelen;i++) {
    x[i] = i;
    y[i] = s_trap[i];
  }
  if (gTrap!=0)
    delete gTrap;
  gTrap = new TGraph(x,y);
  gTrap->SetMarkerStyle(20);
  gTrap->GetXaxis()->SetLimits(-0.5,wavelen-0.5);
  gTrap->SetMarkerColor(kBlue);
#if defined (__CINT__)
  gTrap->Draw("P");
#endif
#if defined (__ROOTCLING__)
  gTrap->Draw("P");
#endif
#if defined (__CLING__)
  gTrap->Draw("P");
#endif
}

/*************************************************************************/
//                              DelaySubtract
/*************************************************************************/
Double_t WaveformAnalyzer::DelaySubtract(int index)
{
  //  if (index == 100) cout << "risetime " << risetime << " top " << risetop << endl;
  if(index<risetime) // too early in waveform
    return (double) (wave[index]);
  else if(index<risetop)  // delta
    return (double) (wave[index]-wave[index-risetime]);
  else if(index<(risetime+risetop)) // 
    return (double) (wave[index]-wave[index-risetime]-wave[index-risetop]);
  else
    return (double) (wave[index]-wave[index-risetime]-wave[index-risetop]+wave[index-risetime-risetop]);
}

/*************************************************************************/
//                               SetTrapPars     
/*************************************************************************/
void WaveformAnalyzer::SetTrapPars(Int_t newdecaytime, Int_t newrisetime, Int_t newtop) {
  decaytime = newdecaytime;
  risetime = newrisetime;
  top = newtop;
  risetop = top+risetime;
  //cout << "risetop is now " << risetop << endl;
}
/*************************************************************************/
//                               GetFitVals
/*************************************************************************/
void WaveformAnalyzer::GetFitVals(Double_t &Amp, Double_t &Mean, Double_t &Tau, Double_t &Integration, Double_t &Chi2) {
  Amp = wavefit->GetParameter(0);
  Mean = wavefit->GetParameter(1);
  Tau = wavefit->GetParameter(2);
  Integration = wavefit->GetParameter(3);
  Chi2 = wavefit->GetChisquare();
}
	    
/*************************************************************************/
//                               FitFunction     
/*************************************************************************/
double WaveFit(double* xx, double* par) {
  double xval = *xx;
  double Amp = par[0];
  double mean = par[1];
  double Tau = par[2];
  double integration = par[3];
  double Shift = mean - integration * Tau;
  xval = xval - Shift;
  double scale = TMath::Power(integration*Tau,integration)*TMath::Exp(-1.*integration);
  if (xval < 0) return 0;
  return (Amp/scale)*TMath::Power(xval,integration)*TMath::Exp(-1.*xval/Tau);
  // y = x^n e-x/t
  //dy/dx = n x^(n-1) e-x/t + x^n 1/t e-x/t = 0
  //n + x/t = 0
}

#endif // WAVEFORM_ANALYZER_CPP__
