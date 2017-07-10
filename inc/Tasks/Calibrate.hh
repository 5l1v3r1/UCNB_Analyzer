// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: Calibrate.hh
// Purpose: Analysis task: perform automatic calibration using UCNB Jan2016 Data.  Work in progress.
 
#ifndef CALIBRATE_HH__
#define CALIBRATE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class Calibrate : public Task
{
private:
	int thresh;
	int decay;
	int shaping;
	int top;
	std::string pathin;
	std::string pathout;
public:
	Calibrate();
	Calibrate(int thresh, int decay, int shaping, int top, std::string pin, std::string pout);
	~Calibrate();
	void Set(int thresh, int decay, int shaping, int top, std::string pin, std::string pout);
	void Go();
};

#endif //ifndef CALIBRATE_HH__
