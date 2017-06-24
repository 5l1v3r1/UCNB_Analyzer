// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: WaveformAnalyzer.hh
// Purpose: Analyzes waveforms for energy and timing

#ifndef WAVEFORM_ANALYZER_HH__
#define WAVEFORM_ANALYZER_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "TGraph.h"
#include "Rtypes.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "Trigger.hh"
#include "RawTreeFile.hh"

#define FITPOLY 5       //order+1 of fit polynomial

using std::cout;
using std::endl;
using std::vector;

class WaveformAnalyzer
{
private:
  bool dodraw, setup;
  Int_t ch;
  Int_t wavelen;
  vector<Double_t> wave;
  vector<Double_t> s_trap;
  vector<Double_t> p_trap;
  Int_t decaytime, risetime, risetop, top;
  Int_t pretrigger;
  Double_t baseline;
public:
  TGraph* g;
  TGraph* ds;
  TGraph* gTrap;
  TF1* f;
  TH1D* h;
  TF1* wavefit;
  WaveformAnalyzer();
  WaveformAnalyzer(int filenum);
  ~WaveformAnalyzer();
  void SetPars(Int_t dt, Int_t rt, Int_t top){decaytime = dt; risetime = rt; risetop = top+risetime;};
  void MakeTrap(Int_t wavelen, Short_t* wave);
  void FitWave(Long64_t thresh, vector<Double_t> &E, vector<Double_t> &T);
  void FitWave(Long64_t thresh, vector<trigger_t> &triglist);
  void BaselineShift();
  bool CheckBaseline();
  void Plot();
  void PlotDS();
  void PlotTrap();
  Double_t DelaySubtract(int index);
  void GetTriggers(Long64_t thresh, vector<Double_t> &E, vector<Double_t> &T);
  void GetTriggers(Long64_t thresh, vector<trigger_t> &triglist);
  void GetFitVals(Double_t &Amp, Double_t &Mean, Double_t &Tau, Double_t &Integration, Double_t &Chi2);
  void SetTrapPars(Int_t newdecaytime, Int_t newrisetime, Int_t newtop);
};

#endif // WAVEFORM_ANALYZER_HH__
