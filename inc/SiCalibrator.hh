// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: SiCalibrator.hh
// Purpose: Calibrates silicon detectors using known sources
 
#ifndef SI_CALIBRATOR_HH__
#define SI_CALIBRATOR_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "TSystem.h"
#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"

#define MAXPIX 48

using std::cout;
using std::endl;
using std::vector;

class SiCalibrator
{
private:
	struct SourceLine_t {
		double E;
		double Edet; //after foil/deadlayer
		double branch;
	};
	struct CalibSource_t {
		std::string name;
		int type;
		vector<SourceLine_t> betas;
		vector<SourceLine_t> xrays;
	};
	vector<CalibSource_t> sourcelist;
	struct SourceData_t {
		TH1D* hSource;
		vector<TF1*> fits;
	};
	struct CalibData_t {
		vector<SourceData_t> sourcedata;
		TF1* fpol1;
		TF1* fpol2;
	};
	vector<CalibData_t> ch;
	struct RunLog_t {
		int type;
		int run;
	};
	vector<RunLog_t> runlist;
public:
	SiCalibrator();
	~SiCalibrator();
	void DefineSources();
	void DefineRunLog(vector<int> runlist, vector<int> typelist) {};
  
};

#endif // SI_CALIBRATOR_HH__
