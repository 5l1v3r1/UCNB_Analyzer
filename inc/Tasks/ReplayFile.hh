// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ReplayFile.hh
// Purpose: Analysis task: runs all replay tasks on range of files
 
#ifndef REPLAY_FILE_HH__
#define REPLAY_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class ReplayFile : public Task
{
private:
	int fstart;
	int fstop;
	vector<std::shared_ptr<Task>> tasklist;
public:
	ReplayFile();
	ReplayFile(int file1, int file2);
	~ReplayFile();
	void Set(int file1, int file2);
	void AddTask(std::shared_ptr<Task> mytask) {tasklist.push_back(mytask);};
	void Go();
};

#endif // REPLAY_FILE_HH__
