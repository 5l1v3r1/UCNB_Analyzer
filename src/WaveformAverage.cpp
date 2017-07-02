// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.


// File: WaveformAverage.cpp
// Purpose: Analyzes waveforms for energy and timing

#ifndef WAVEFORM_AVERAGE_CPP__
#define WAVEFORM_AVERAGE_CPP__

#include "WaveformAverage.hh"
#include "TVectorD.h"
#include "TMath.h"

double WaveFit(double* xx, double* par);

/*************************************************************************/
//                            Constructor
/*************************************************************************/
WaveformAverage::WaveformAverage() {
	baselinesmp = 500;
	upsample = 1;
	SetBins(100,1000,500,3000);
}

/*************************************************************************/
//                             Destructor
/*************************************************************************/
WaveformAverage::~WaveformAverage() {
}


/*************************************************************************///                      SetBins
/*************************************************************************/
void WaveformAverage::SetBins(int ps, int s, int na, int a) {
	PreSmpBins = ps; 
	SmpBins = s; 
	NegAmpBins = na; 
	AmpBins = a;
	dist.resize(SmpBins*upsample);
	for (int i=0;i<dist.size();i++) {
		dist[i].resize(AmpBins);
	}
}


/*************************************************************************/
//                              AddToAve
/*************************************************************************/
void WaveformAverage::AddToAve(vector<short> &wave, double TOffset, double ANorm) {
	BaselineShift(wave);
	//Add waveform
	int startsmp = (int)TMath::Floor(TOffset) - PreSmpBins;
	int shift = (int)TMath::Floor((TOffset - TMath::Floor(TOffset))*upsample);
	for (int bin=0;bin<SmpBins;bin++) {
		double wval = wave[bin+startsmp]*ANorm;
		int aveval = wval*(AmpBins-NegAmpBins)*0.5 + NegAmpBins;
		int avesmp = bin*upsample + shift;
		if (avesmp < dist.size() && aveval < dist[0].size())
			dist[avesmp][aveval]++;
	}
}


/*************************************************************************/
//                             Average
/*************************************************************************/
void WaveformAverage::Average(){
	ave.resize(dist.size());
	for (int bin = 0; bin < dist.size(); bin++) {
		ave[bin] = 0;
		double norm = 0;
		for (int val = 0; val < dist[bin].size(); val++) 
			if (dist[bin][val]>0) {
				ave[bin] += val * dist[bin][val];
				norm += dist[bin][val];
			}
		ave[bin] = ave[bin]/norm;
	}
}

/*************************************************************************/
//                            BaselineShift
/*************************************************************************/
void WaveformAverage::BaselineShift(vector<short> &wave) {
  double baseline = 0;
  for (int smp = 0; smp < baselinesmp; smp++) {
    baseline += wave[smp];
  }
  baseline = baseline/baselinesmp;
  for (int smp = 0; smp < wave.size(); smp++) {
    wave[smp] -= baseline;
  }  
}


/*************************************************************************/
//                             GetDist
/*************************************************************************/
void WaveformAverage::GetDist(TH2D* hDist){
	if (hDist!=0) delete hDist;
	string name = "hDist";
	name.append(hname);
	hDist = new TH2D(name.c_str(),name.c_str(),SmpBins,-1.*PreSmpBins,SmpBins-PreSmpBins,AmpBins,-1.*(NegAmpBins/(AmpBins-NegAmpBins)),1.);
	for (int bin = 0; bin < dist.size(); bin++)
		for (int val = 0; val < dist[bin].size(); val++) 
			if (dist[bin][val]>0)
				hDist->SetBinContent(bin+1,val+1,dist[bin][val]);
}

/*************************************************************************/
//                             GetAve
/*************************************************************************/
void WaveformAverage::GetAve(TH1D* hAve){
	if (hAve!=0) delete hAve;
	string name = "hAve";
	name.append(hname);
	hAve = new TH1D(name.c_str(),name.c_str(),SmpBins,-1.*PreSmpBins,SmpBins-PreSmpBins);
	for (int bin = 0; bin < dist.size(); bin++)
		hAve->SetBinContent(bin+1,ave[bin]);
}


#endif // WAVEFORM_AVERAGE_CPP__
