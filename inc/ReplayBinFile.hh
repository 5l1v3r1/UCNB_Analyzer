// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ReplayBinFile.hh
// Purpose: Analysis task: replay a bin file into a raw tree file
 
#ifndef REPLAY_BIN_FILE_HH__
#define REPLAY_BIN_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class ReplayBinFile : public Task
{
private:
	int dataformat;
	std::string pathin;
	std::string pathout;
public:
	ReplayBinFile();
	ReplayBinFile(int dataformat, std::string pin, std::string pout);
	~ReplayBinFile();
	void Set(int dataformat, std::string pin, std::string pout);
	void Go(int filenum);
};

#endif // REPLAY_BIN_FILE_HH__
