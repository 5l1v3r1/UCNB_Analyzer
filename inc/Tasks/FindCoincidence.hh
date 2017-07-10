// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: FindCoincidence.hh
// Purpose: Analysis task: find coincidences within time window
 
#ifndef FIND_COINCIDENCE_HH__
#define FIND_COINCIDENCE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class FindCoincidence : public Task
{
private:
	int smpcoinc;
	std::string calibfile;
	std::string pathin;
	std::string pathout;
public:
	FindCoincidence();
	FindCoincidence(int smpcoinc, std::string calibfile, std::string pin, std::string pout);
	~FindCoincidence();
	void Set(int smpcoinc, std::string calibfile, std::string pin, std::string pout);
	void Go(int filenum);
};

#endif //ifndef FIND_COINCIDENCE_HH__
