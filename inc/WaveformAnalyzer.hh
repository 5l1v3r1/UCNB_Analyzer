// File: WaveformAnalyzer.hh
// Name: Leah Broussard
// Date: 2013/4/11
// Purpose: Analyzes waveforms for energy and timing
//
// Revision History:
// 2013/4/11: LJB  Adapt from Ne19 code
// 2015/5/11: LJB  Adapt to NI format

#ifndef WAVEFORM_ANALYZER_HH__
#define WAVEFORM_ANALYZER_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "ROOTTreeFile.hh"
#include "EvType.hh"
#include "TGraph.h"
#include "Rtypes.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

#define FITPOLY 5       //order+1 of fit polynomial

using std::cout;
using std::endl;

class WaveformAnalyzer
{
private:
  bool dodraw, setup;
  Int_t ch;
  Int_t wavelen;
  vector<Double_t> wave;
  vector<Double_t> s_trap;
  vector<Double_t> p_trap;
  Int_t decaytime, risetime, risetop;
  Int_t pretrigger;
  Double_t baseline;

public:

  TGraph* g;
  TGraph* ds;
  TGraph* gTrap;
  TF1* f;
  TH1D* h;

  WaveformAnalyzer();
  WaveformAnalyzer(int filenum);
  ~WaveformAnalyzer();
  void SetPars(Int_t dt, Int_t rt, Int_t top){decaytime = dt; risetime = rt; risetop = top;};
  void MakeTrap(Int_t wavelen, Short_t* wave);
  void BaselineShift();
  void Plot();
  void PlotDS();
  void PlotTrap();
  Double_t DelaySubtract(int index);
  void GetTriggers(Long64_t thresh, vector<Double_t> &E, vector<Double_t> &T);
  //Bool_t IsGoodWaveform();
};

#endif // WAVEFORM_ANALYZER_HH__
