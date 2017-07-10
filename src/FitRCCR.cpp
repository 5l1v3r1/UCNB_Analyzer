// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: FitRCCR.cpp
// Purpose: Analysis task: fit waveform to RC-CR^n

#ifndef FIT_RCCR_CPP__
#define FIT_RCCR_CPP__

#include <math.h>

#include "FitRCCR.hh"
#include "RawTreeFile.hh"
#include "FitTreeFile.hh"
#include "WaveformAnalyzer.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
FitRCCR::FitRCCR() {
}

FitRCCR::FitRCCR(int thresh, std::string pin, std::string pout) {
	Set(thresh, pin, pout);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
FitRCCR::~FitRCCR() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void FitRCCR::Set(int th, std::string pin, std::string pout) {
	thresh = th;
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void FitRCCR::Go(int filenum) {
	//-----Open input/output files
	RawTreeFile RootFile;
	if (pathin.compare("") != 0) { 
		RootFile.SetPath(pathin);
	}
	if (!RootFile.Open(filenum)) {
		cout << "Input file not open!" << endl;
		return;
	}
	FitTreeFile FitFile;
	if (pathout.compare("") != 0) { 
		FitFile.SetPath(pathout);
	}
	if (!FitFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
	WaveformAnalyzer WA;
	cout << "Fitting waveforms in file " << filenum << endl;
	int filecount = 0;
	do {
		int nentries = RootFile.GetNumEvents();
		//-----Find triggers
		for (int ev=0;ev<nentries;ev++) {
			printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
			RootFile.GetEvent(ev);
			WA.MakeTrap(RootFile.NI_event.length, RootFile.NI_event.wave);
			vector<trigger_t> triglist; 
			WA.FitWave(thresh,triglist);
			for (int t=0;t<triglist.size();t++) {
				FitFile.Fit_event.E = triglist[t].E;
				FitFile.Fit_event.t = triglist[t].T + (double)RootFile.NI_event.timestamp;
				FitFile.Fit_event.shaping = triglist[t].Shaping;
				FitFile.Fit_event.integ = triglist[t].Integration;
				FitFile.Fit_event.chi2 = triglist[t].Chi2;
				FitFile.Fit_event.ch = RootFile.NI_event.ch;
				FitFile.Fit_event.wavefile= filecount;
				FitFile.Fit_event.waveev = ev;
				FitFile.FillTree();
			}
		}//ev < NumEvents
		RootFile.Close();	
		if (!RootFile.Open(filenum,++filecount))
			filecount = -1;
	} while (filecount != -1);
	FitFile.Write();
	cout << "Done" << endl;
	FitFile.Close();
	RootFile.Close();	
}





#endif //ifndef FIT_RCCR_CPP__

