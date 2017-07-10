// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ReplayTrigFile.hh
// Purpose: Analysis task: replay a trig file into a trig tree file
 
#ifndef REPLAY_TRIG_FILE_HH__
#define REPLAY_TRIG_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class ReplayTrigFile : public Task
{
private:
	int dataformat;
	std::string pathin;
	std::string pathout;
public:
	ReplayTrigFile();
	ReplayTrigFile(int dataformat, std::string pin, std::string pout);
	~ReplayTrigFile();
	void Set(int dataformat, std::string pin, std::string pout);
	void Go(int filenum);
};

#endif //ifndef REPLAY_TRIG_FILE_HH__
