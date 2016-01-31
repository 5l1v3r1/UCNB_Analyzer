// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: SiCalibrator.cpp
// Purpose: Calibrates silicon detectors using known sources
 

#ifndef SI_CALIBRATOR_CPP__
#define SI_CALIBRATOR_CPP__

#include "SiCalibrator.hh"
#include "TError.h"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
SiCalibrator::SiCalibrator() {
	maxtype = 0;
	detector.resize(MAXPIX);
	DefineSources();
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
SiCalibrator::~SiCalibrator() {
}

/*************************************************************************/
//                             DefineSources
/*************************************************************************/
void SiCalibrator::DefineSources() {
	//www.nndc.bnl.gov
	int cnt = 0;
	CalibSource_t Bi207;
	Bi207.name = "Bi207";
	Bi207.type = cnt++;
	SourceLine_t line;
	line.E = 7.97; line.branch = 54.4; Bi207.betas.push_back(line);
	line.E = 56.7; line.branch = 2.9; Bi207.betas.push_back(line);
	line.E = 481.6935; line.branch = 1.537; Bi207.betas.push_back(line);
	line.E = 553.8372; line.branch = 0.442; Bi207.betas.push_back(line);
	line.E = 565.8473; line.branch = 0.111; Bi207.betas.push_back(line);
	line.E = 975.651; line.branch = 7.08; Bi207.betas.push_back(line);
	line.E = 1047.795; line.branch = 1.84; Bi207.betas.push_back(line);
	line.E = 1059.805; line.branch = 0.44; Bi207.betas.push_back(line);
	sourcelist.push_back(Bi207);
	CalibSource_t Bi207x;
	Bi207x.name = "Bi207x";
	Bi207x.type = cnt++;
	line.E = 10.6; line.branch = 33.2; Bi207x.xrays.push_back(line);
	line.E = 72.805; line.branch = 21.4; Bi207x.xrays.push_back(line);
	line.E = 74.969; line.branch = 35.7; Bi207x.xrays.push_back(line);
	sourcelist.push_back(Bi207x);
	CalibSource_t Sn113;
	Sn113.name = "Sn113";
	Sn113.type = cnt++;
	line.E = 20.1; line.branch = 17.0; Sn113.betas.push_back(line);
	line.E = 363.758; line.branch = 28.8; Sn113.betas.push_back(line);
	line.E = 387.461; line.branch = 5.60; Sn113.betas.push_back(line);
	line.E = 390.872; line.branch = 1.137; Sn113.betas.push_back(line);
	sourcelist.push_back(Sn113);
	CalibSource_t Sn113x;
	Sn113x.name = "Sn113x";
	Sn113x.type = cnt++;
	line.E = 24.002; line.branch = 28.0; Sn113x.xrays.push_back(line);
	line.E = 24.21; line.branch = 51.8; Sn113x.xrays.push_back(line);
	line.E = 27.238; line.branch = 4.66; Sn113x.xrays.push_back(line);
	sourcelist.push_back(Sn113x);
	CalibSource_t Ce139;
	Ce139.name = "Ce139";
	Ce139.type = cnt++;
	line.E = 27.4; line.branch = 8.3; Ce139.betas.push_back(line);
	line.E = 126.9329; line.branch = 17.15; Ce139.betas.push_back(line);
	line.E = 159.5912; line.branch = 2.298; Ce139.betas.push_back(line);
	line.E = 164.4962; line.branch = 0.4751; Ce139.betas.push_back(line);
	sourcelist.push_back(Ce139);
	CalibSource_t Ce139x;
	Ce139x.name = "Ce139x";
	Ce139x.type = cnt++;
	line.E = 33.034; line.branch = 22.5; Ce139x.betas.push_back(line);
	line.E = 33.442; line.branch = 41.0; Ce139x.betas.push_back(line);
	sourcelist.push_back(Ce139x);
	maxtype = cnt;
	
	CalData.resize(maxtype);
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
	TFile* myfile = new TFile("SumCalData.root","RECREATE");
	for (int run=0;run<runlist.size();run++) {
		myfile->cd();
		for (int det=0;det<2;det++) {
			int src = runlist[run].type[det];
			if (src >= 0 && src < CalData.size()) {
				if (CalData[src].hSource == 0) {
					char name[255];
					//name.append(std::to_string(ch));
					sprintf(name,"h%s",sourcelist[src].name.c_str());
					CalData[src].hSource = new TH2D(name,name,MAXPIX,0,MAXPIX,MAXADCVAL,0,MAXADCVAL);
				}
			}
		}
		if (!trapf.Open(runlist[run].filenum,200,300,300)) {
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
}

/*************************************************************************/
//                              FindPeaks
/*************************************************************************/
void SiCalibrator::FindPeaks() {
	TFile* myfile = new TFile("SumCalData.root");
		for (int src=0; src < CalData.size(); src++) {
			char name[255];
			sprintf(name,"h%s",sourcelist[src].name.c_str());
			CalData[src].hSource = (TH2D*)myfile->Get(name);
			CalData[src].sourcedata.resize(MAXPIX);
		}
	int iterate = 25;
	int nbins;
	TSpectrum* spec = new TSpectrum(10);	
	for (int src=0; src < CalData.size(); src++) {
		if (CalData[src].hSource != 0) {
			for (int ch=0; ch<CalData[src].sourcedata.size(); ch++) {
				TH1D* hbi = CalData[src].hSource->ProjectionY("hb",ch+1,ch+1);
				nbins = hbi->GetNbinsX();
				char name[255];
				sprintf(name,"h%sCh%d",sourcelist[src].name.c_str(),ch);
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
				double sigma = max/500; // 2 keV
				gErrorIgnoreLevel = kError;
				int npeaks = spec->Search(hbi,sigma,"nodraw ",0.001);
				gErrorIgnoreLevel = kWarning;
				TList *functions = hbi->GetListOfFunctions();
				if (functions != 0) {
					TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
					if (pm != 0) {
						double* xbi;
						if (pm->GetN() > 0)
							xbi = pm->GetX();
						for (int i=0;i<pm->GetN();i++) {
							CalData[src].sourcedata[ch].points.push_back(xbi[i]);
						}
					}
				}
			}
			//Sanity check
			int ch = 0;
			cout << "Found in " << sourcelist[src].name << endl;
			if (ch < CalData[src].sourcedata.size()) {
				cout << CalData[src].sourcedata[ch].points.size() << endl;
				for (int i=0;i<CalData[src].sourcedata[ch].points.size();i++) {
					cout << CalData[src].sourcedata[ch].points[i] << endl;
				}
			}
		}
	}
	
}

/*************************************************************************/
//                             MatchPeaks
/*************************************************************************/
void SiCalibrator::MatchPeaks() {
	int ch = 0; int src = 0;
	// Build fake spectrum fit function
	// Attempt fit of background subtracted spectrum to fake fit function
	
}



#endif // SI_CALIBRATOR_CPP__

