// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: FitRCCR.hh
// Purpose: Analysis task: fit waveform to RC-CR^n
 
#ifndef FIT_RCCR_HH__
#define FIT_RCCR_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class FitRCCR : public Task
{
private:
	int thresh;
	std::string pathin;
	std::string pathout;
public:
	FitRCCR();
	FitRCCR(int thresh, std::string pin, std::string pout);
	~FitRCCR();
	void Set(int thresh, std::string pin, std::string pout);
	void Go(int filenum);
};

#endif //ifndef FIT_RCCR_HH__
