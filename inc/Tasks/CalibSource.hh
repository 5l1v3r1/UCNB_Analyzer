// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: CalibSource.hh
// Purpose: List of calibration sources
 
#ifndef CALIBSOURCE_HH__
#define CALIBSOURCE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

using std::vector;

class CalibSource
{
public:
	struct SourceLine_t {
		double E;
		double Edet; //after foil/deadlayer
		double branch;
	};
	struct CalibSource_t {
		std::string name;
		int type; //don't need this?
		vector<SourceLine_t> betas;
		vector<SourceLine_t> xrays;
	};
	const static vector<CalibSource_t> sourcelist;
};


#endif // CALIBSOURCE_HH__