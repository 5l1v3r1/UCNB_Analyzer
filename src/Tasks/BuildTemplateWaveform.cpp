// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: BuildTemplateWaveform.cpp
// Purpose: Analysis task: create pulse template from average of waveforms

#ifndef BUILD_TEMPLATE_WAVEFORM_CPP__
#define BUILD_TEMPLATE_WAVEFORM_CPP__

#include <math.h>

#include "BuildTemplateWaveform.hh"
#include "RawTreeFile.hh"
#include "TrapTreeFile.hh"
#include "WaveformAverage.hh" //move this into this task

/*************************************************************************/
//                           Constructors
/*************************************************************************/
BuildTemplateWaveform::BuildTemplateWaveform() {
}

BuildTemplateWaveform::BuildTemplateWaveform(int decay, int shaping, int top, std::string pin, std::string pout) {
	Set(decay, shaping, top, pin, pout);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
BuildTemplateWaveform::~BuildTemplateWaveform() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void BuildTemplateWaveform::Set(int d, int s, int t, std::string pin, std::string pout) {
	decay = d;
	shaping = s;
	top = t;
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void BuildTemplateWaveform::Go(int filenum) {
	cout << "Building average waveforms" << endl;
	//-----Open input/output files
	RawTreeFile RootFile;
	if (pathin.compare("") != 0) { 
		RootFile.SetPath(pathin);
	}
	if (!RootFile.Open(filenum)) {
		cout << "Input file not open!" << endl;
		return;
	}
	TrapTreeFile TrapFile;
	if (pathin.compare("") != 0) { 
		TrapFile.SetPath(pathin);
	}
	if (!TrapFile.Open(filenum, decay, shaping, top)) {
		cout << "Trap file Not Open!" << endl;
		//todo Go ahead and run it
		return;
	}
	int nentries = TrapFile.GetNumEvents();
	vector<vector<WaveformAverage> > wavelist;
	int NumAmp = 5, MaxAmp = 1000, PreSmpBins = 100, SmpBins = 1000, NegAmpBins = 500, AmpBins = 3000;
	wavelist.resize(NumAmp);
	for (int i=0;i<wavelist.size();i++) {
		wavelist[i].resize(MAXCH*MAXRIO);
		for (int j=0;j<wavelist[i].size();j++) {
			wavelist[i][j].SetBins(PreSmpBins,SmpBins,NegAmpBins,AmpBins);
			char tempstr[255];
			sprintf(tempstr,"ch%dE%d",j,i);
			std::string hname = tempstr;
			wavelist[i][j].SetHistName(hname);
		}
	}
	
	//-----Build distribution of up/down/down-up to guess good range
	vector<vector<double> > updist(MAXCH*MAXRIO,vector<double>(RootFile.NI_event.length,0));
	vector<vector<double> > downdist(MAXCH*MAXRIO,vector<double>(RootFile.NI_event.length,0));
	vector<vector<double> > downupdist(MAXCH*MAXRIO,vector<double>(RootFile.NI_event.length,0));
	vector<int> evcnt(MAXCH*MAXRIO,0);
	for (int ev=0;ev<nentries;ev++) {
		TrapFile.GetEvent(ev);
		if (TrapFile.Trap_event.AveE > 200 && TrapFile.Trap_event.down < RootFile.NI_event.length) {
			evcnt[TrapFile.Trap_event.ch]++;
			updist[TrapFile.Trap_event.ch][TrapFile.Trap_event.up]++;
			downdist[TrapFile.Trap_event.ch][TrapFile.Trap_event.down]++;
			downupdist[TrapFile.Trap_event.ch][TrapFile.Trap_event.down-TrapFile.Trap_event.up]++;
		}
	}
	vector<double> maxup(MAXCH*MAXRIO,0);
	vector<double> hwhmup(MAXCH*MAXRIO,0);
	vector<double> maxdown(MAXCH*MAXRIO,0);
	vector<double> hwhmdown(MAXCH*MAXRIO,0);
	vector<double> maxdownup(MAXCH*MAXRIO,0);
	vector<double> hwhmdownup(MAXCH*MAXRIO,0);
	for (int ch=0;ch<maxup.size();ch++) {
		double maxu = 0, maxd = 0, maxdu = 0;
		if (evcnt[ch] > 30) {
			for (int smp=0;smp<RootFile.NI_event.length;smp++) {
				if (maxu < updist[ch][smp]) {
					maxu = updist[ch][smp];
					maxup[ch] = smp;
				}
				if (maxd < downdist[ch][smp]) {
					maxd = downdist[ch][smp];
					maxdown[ch] = smp;
				}
				if (maxdu < downupdist[ch][smp]) {
					maxdu = downupdist[ch][smp];
					maxdownup[ch] = smp;
				}
			}
			double valR, valL;
			do {
				hwhmup[ch]++;
				valR = updist[ch][maxup[ch]+hwhmup[ch]];
				valL = updist[ch][maxup[ch]-hwhmup[ch]];
			}while(valR>maxu/2 && valL>maxu/2);
			do {
				hwhmdown[ch]++;
				valR = downdist[ch][maxdown[ch]+hwhmdown[ch]];
					valL = downdist[ch][maxdown[ch]-hwhmdown[ch]];
			}while(valR>maxd/2 && valL>maxd/2);
			do {
				hwhmdownup[ch]++;
				valR = downupdist[ch][maxdownup[ch]+hwhmdownup[ch]];
				valL = downupdist[ch][maxdownup[ch]-hwhmdownup[ch]];
			}while(valR>maxdu/2 && valL>maxdu/2);
		}
	}
	
	//-----Build distribution of waveforms
	for (int ev=0;ev<nentries/100;ev++) {
		printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
		TrapFile.GetEvent(ev);
		RootFile.GetEvent(TrapFile.Trap_event.waveev);
		double T0 = (TrapFile.Trap_event.up + TrapFile.Trap_event.down)/2 - top/2 - shaping; 
		int ch = TrapFile.Trap_event.ch;
		if (hwhmup[ch] > 0 && hwhmdown[ch] > 0 && hwhmdownup[ch] > 0 && TrapFile.Trap_event.up>maxup[ch]-hwhmup[ch] && TrapFile.Trap_event.up<maxup[ch]+hwhmup[ch] && TrapFile.Trap_event.down>maxdown[ch]-hwhmdown[ch] && TrapFile.Trap_event.down<maxdown[ch]+hwhmdown[ch] && (TrapFile.Trap_event.down-TrapFile.Trap_event.up) > maxdownup[ch]-hwhmdownup[ch] && (TrapFile.Trap_event.down-TrapFile.Trap_event.up) < maxdownup[ch]+hwhmdownup[ch] && TrapFile.Trap_event.AveE > 0 && TrapFile.Trap_event.AveE < MaxAmp && T0 - PreSmpBins >= 0 && T0 + SmpBins < RootFile.NI_event.length) {
			int whichamp = NumAmp*TrapFile.Trap_event.AveE / MaxAmp;
			vector<short> wave(RootFile.NI_event.wave,RootFile.NI_event.wave+RootFile.NI_event.length);
			wavelist[whichamp][ch].AddToAve(wave,T0,1./TrapFile.Trap_event.AveE);
		}
	}
	RootFile.Close();
	TrapFile.Close();
	
	//-----Write to file
	for (int ch=0;ch<MAXCH*MAXRIO;ch++) {
		if (hwhmup[ch] > 0 && hwhmdown[ch] > 0 && hwhmdownup[ch] > 0) {
			char tempstr[255];
			sprintf(tempstr,"Avech%d.root",ch);
			std::string filename = pathout;
			filename.append("/");
			filename.append(tempstr);
			cout << "Writing " << filename << endl;
			TFile* newfile = new TFile(filename.c_str(),"RECREATE");
			vector<TH2D*> hDist(wavelist.size(),0);
			for (int i=0;i<wavelist.size();i++) {
				wavelist[i][ch].GetDist(hDist[i]);
			}	
			newfile->Write();
			newfile->Close();
		}
	}
}





#endif //ifndef BUILD_TEMPLATE_WAVEFORM_CPP__

