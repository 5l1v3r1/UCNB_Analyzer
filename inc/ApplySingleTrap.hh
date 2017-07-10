// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ApplySingleTrap.hh
// Purpose: Analysis task: replay a trig file into a trig tree file
 
#ifndef REPLAY_TRAP_FILE_HH__
#define REPLAY_TRAP_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class ApplySingleTrap : public Task
{
private:
	int thresh;
	int decay;
	int shaping;
	int top;
	std::string pathin;
	std::string pathout;
public:
	ApplySingleTrap();
	ApplySingleTrap(int thresh, int decay, int shaping, int top, std::string pin, std::string pout);
	~ApplySingleTrap();
	void Set(int thresh, int decay, int shaping, int top, std::string pin, std::string pout);
	void Go(int filenum);
};

#endif //ifndef REPLAY_TRAP_FILE_HH__
