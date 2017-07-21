// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ReplayCaBinFile.hh
// Purpose: Analysis task: replay a bin file to correct waveforms
 
#ifndef REPLAY_CA_BIN_FILE_HH__
#define REPLAY_CA_BIN_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class ReplayCaBinFile : public Task
{
private:
	std::string pathin;
	std::string pathout;
public:
	ReplayCaBinFile();
	ReplayCaBinFile(std::string pin, std::string pout);
	~ReplayCaBinFile() {};
	void Set(std::string pin, std::string pout);
	void Go(int filenum);
};

#endif // REPLAY_CA_BIN_FILE_HH__
