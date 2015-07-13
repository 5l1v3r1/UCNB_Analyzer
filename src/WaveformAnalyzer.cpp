// File: WaveformAnalyzer.cpp
// Name: Leah Broussard
// Date: 2013/4/11
// Purpose: Analyzes waveforms for energy and timing
//
// Revision History:
// 2013/4/11: LJB  Adapt from Ne19 code
// 2015/5/11: LJB  Adapt to NI format

#ifndef WAVEFORM_ANALYZER_CPP__
#define WAVEFORM_ANALYZER_CPP__

#include "WaveformAnalyzer.hh"
#include "TVectorD.h"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
WaveformAnalyzer::WaveformAnalyzer() {
  g = 0; 
  ds = 0;
  f = 0;
  h = 0;
  ch = 0;
  decaytime = 247;
  risetime = 250;
  risetop = 280;
  pretrigger = 200;
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
//                            GetTriggers
/*************************************************************************/
void WaveformAnalyzer::GetTriggers(Long64_t thresh, vector<Double_t> &E, vector<Double_t> &T)
{
  Short_t up = 0;
  Short_t down = 0;

  E.resize(0);
  T.resize(0);
  if (!setup) return;

  for (int smp = 0; smp < wavelen; smp++) {
    if(s_trap[smp]>=thresh && s_trap[smp-1]<thresh)
      {
	up=smp;
	smp++;
	while(s_trap[smp]>=thresh && smp<wavelen)
	  smp++;
	down=smp;
	if(down != 0) {
	  E.push_back((double)(s_trap[(up+down)/2.]));
	  T.push_back((double)((up+down)/2.)-risetime);//-risetop/2.));
	}
      }
  }

}

/*************************************************************************/
//                            GetTriggers
/*************************************************************************/
void WaveformAnalyzer::GetTriggers(Long64_t thresh, vector<trigger_t> &triglist) {
  vector<Double_t> E; 
  vector<Double_t> T;
  GetTriggers(thresh, E, T);
  triglist.resize(E.size());
  for (int i=0;i<triglist.size();i++) {
    triglist[i].E = E[i];
    triglist[i].T = T[i];
  }
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
  g->Draw("AP");
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
  gTrap->Draw("P");
}

/*************************************************************************/
//                              DelaySubtract
/*************************************************************************/
Double_t WaveformAnalyzer::DelaySubtract(int index)
{
  if(index<risetime) // too early in waveform
    return (double) (wave[index]);
  else if(index<risetop)  // delta
    return (double) (wave[index]-wave[index-risetime]);
  else if(index<(risetime+risetop)) // 
    return (double) (wave[index]-wave[index-risetime]-wave[index-risetop]);
  else
    return (double) (wave[index]-wave[index-risetime]-wave[index-risetop]+wave[index-risetime-risetop]);
}
	    
#endif // WAVEFORM_ANALYZER_CPP__
