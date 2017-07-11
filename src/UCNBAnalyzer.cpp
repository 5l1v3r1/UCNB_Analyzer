// Copyright 2016.  Los Alamos National Security, LLC.
// Copyright 2016-2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: Analyzer.cpp
// Purpose: Analyze data from NI DAQ for UCNB experiment

#ifndef ANALYZER_CPP__
#define ANALYZER_CPP__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstring>
#include <memory>

#include "CommandParser.hh"

#include "TApplication.h"

using std::cin;  
using std::cout;
using std::endl;
using std::ifstream;
	
	
/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
	cout << endl << "Welcome to UCNB_Analyzer v1.3.0" << endl << endl;
	//-----Interpret arguments and get task list
	CommandParser comm;
	if (!comm.Parse(argc,argv)) return 1;
	vector<std::shared_ptr<Task>> tasklist;
	comm.GetTasks(tasklist);
  
	for (int task=0; task<tasklist.size(); task++)
		tasklist[task]->Go(); //mpi-ify
}

#endif // __ANALYZER_CPP__
