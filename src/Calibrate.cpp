// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: Calibrate.cpp
// Purpose: Analysis task: perform automatic calibration using UCNB Jan2016 Data.  Work in progress.

#ifndef CALIBRATE_CPP__
#define CALIBRATE_CPP__

#include <math.h>

#include "Calibrate.hh"
#include "SiCalibrator.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
Calibrate::Calibrate() {
}

Calibrate::Calibrate(int thresh, int decay, int shaping, int top, std::string pin, std::string pout) {
	Set(thresh, decay, shaping, top, pin, pout);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
Calibrate::~Calibrate() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void Calibrate::Set(int th, int d, int s, int t, std::string pin, std::string pout) {
	thresh = th;
	decay = d;
	shaping = s;
	top = t;
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void Calibrate::Go() {
	SiCalibrator calib(thresh, decay, shaping, top);
	if (pathin.compare("") != 0) { 
		calib.SetPath(pathin);
	}
	//-----To do: move detector channel mapping to new object
	//east = 0, west = 1
	int det[MAXPIX];
	for (int i=0;i<8;i++)
		det[i] = 0;
	for (int i=8;i<32;i++)
		det[i] = 1;
	for (int i=32;i<48;i++)
		det[i] = 0;
	vector<int> detector;
	detector.assign(&det[0],&det[0]+MAXPIX);
	calib.DefineDetector(detector);
	//-----To do: upload run log by file
	int rl[38] = {10,11,12,13,14,15,16,17,18,19,20,21,30,31,32,33,34,36,37,38,39,40,41,42,43,44,45,50,51,52,53,54,55,56,57,58,59,60};
	vector<int> runlist;
	runlist.assign(&rl[0],&rl[0]+38);
	int typ1[38], typ2[38];
	for (int i=0;i<=11;i++) {
		typ1[i] = 3; //Sn113x
		typ2[i] = 1; //Bi207x
	}
	for (int i=12;i<=26;i++) {
		typ1[i] = 1; //Bi207x
		typ2[i] = 5; //Ce139x
	}
	for (int i=27;i<38;i++) {
		typ1[i] = 5; //Ce139x
		typ2[i] = 3; //Sn113x
	}
	vector<int> type1, type2;
	type1.assign(&typ1[0],&typ1[0]+38);
	type2.assign(&typ2[0],&typ2[0]+38);
	calib.DefineRunLog(runlist,type1,type2);
	//-----Build histograms
	cout << "Building histograms.." << endl;
	TrapTreeFile trapfile;
	if (pathin.compare("") != 0) { 
		trapfile.SetPath(pathin);
	}
	//-----To Do:  Check for exising files
	calib.BuildHists(trapfile);
	//calib.FindPeaks();
	cout << "Fitting spectra.."<< endl;
	calib.MatchPeaks();
	//cout << "Hit Ctrl+C to stop viewing plot and quit program." << endl;
}





#endif //ifndef CALIBRATE_CPP__

