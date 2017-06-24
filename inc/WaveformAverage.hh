// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.


// File: WaveformAverage.hh
// Purpose: Analyzes waveforms for energy and timing

#ifndef WAVEFORM_AVERAGE_HH__
#define WAVEFORM_AVERAGE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

class WaveformAverage
{
private:
//  bool dodraw, setup;
	int baselinesmp;
	int upsample;
	int PreSmpBins;
	int SmpBins;
	int NegAmpBins;
	int AmpBins;
	vector<vector<short> > dist;
	string hname;
public:
	TH1D* hAve;
	TH2D* hDist;
	WaveformAverage();
	~WaveformAverage();
	void SetBaselineSmp(int bs) {baselinesmp = bs;};
	void SetBins(int ps, int s, int na, int a);
	void SetHistName(string name) {hname = name;};
	void AddToAve(vector<short> &wave, double TOffset, double ANorm);
	void Average();
	void BaselineShift(vector<short> &wave);
	void Plot();
	void Write();
};

#endif // WAVEFORM_AVERAGE_HH__
