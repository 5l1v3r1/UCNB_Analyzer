// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ReplayFile.cpp
// Purpose: Analysis task: runs all replay tasks on range of files

#ifndef REPLAY_FILE_CPP__
#define REPLAY_FILE_CPP__

#include "ReplayFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
ReplayFile::ReplayFile() {
}

ReplayFile::ReplayFile(int file1, int file2) {
	Set(file1,file2);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
ReplayFile::~ReplayFile() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void ReplayFile::Set(int file1, int file2) {
	fstart = file1;
	fstop = file2;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void ReplayFile::Go() {
	for (int file = fstart; file <= fstop; file++) {
		for (int task=0; task<tasklist.size(); task++)
			tasklist[task]->Go(file); //mpi-ify
	}
}





#endif // REPLAY_FILE_CPP__

