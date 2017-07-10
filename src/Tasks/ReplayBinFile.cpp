// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ReplayBinFile.cpp
// Purpose: Analysis task: replay a bin file into a raw tree file

#ifndef REPLAY_BIN_FILE_CPP__
#define REPLAY_BIN_FILE_CPP__

#include "ReplayBinFile.hh"
#include "NIFeb2015BinFile.hh"
#include "NIJune2015BinFile.hh"
#include "NIMay2016BinFile.hh"
#include "NIMay2017BinFile.hh"
#include "RawTreeFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
ReplayBinFile::ReplayBinFile() {
	dataformat = 3; //default
}

ReplayBinFile::ReplayBinFile(int format, std::string pin, std::string pout) {
	Set(format, pin, pout);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
ReplayBinFile::~ReplayBinFile() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void ReplayBinFile::Set(int format, std::string pin, std::string pout) {
	dataformat = format;
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void ReplayBinFile::Go(int filenum) {
	//-----Open input/output files
	int nfiles = (dataformat == 0) ? MAXRIO : 1;
	vector<BinFile*> InputFile(nfiles);
	vector<BinFile::BinEv_t*> InputEvent(nfiles);
	for (int rio=0;rio<nfiles;rio++) { 
		if (dataformat == 0) {
			InputEvent[rio] = new NIFeb2015BinFile::FebBinEv_t;
			InputFile[rio] = new NIFeb2015BinFile();
		}
		else if (dataformat == 1) {
			InputEvent[rio] = new NIJune2015BinFile::JuneBinEv_t;
			InputFile[rio] = new NIJune2015BinFile();
		}
		else if (dataformat == 2) {
			InputEvent[rio] = new NIMay2016BinFile::MayBinEv_t;
			InputFile[rio] = new NIMay2016BinFile();
		}
		else if (dataformat == 3) {
			InputEvent[rio] = new NIMay2017BinFile::MayBinEv_t;
			InputFile[rio] = new NIMay2017BinFile();
		}
		if (pathin.compare("") != 0) { 
			InputFile[rio]->SetPath(pathin);
		}
		InputFile[rio]->Open(filenum, rio);
		if (!InputFile[rio]->IsOpen()) {
			cout << "Input file not open!" << endl;
			delete InputFile[rio];
			delete InputEvent[rio];
			return;
		}	
	}
	//-----Store in ROOT file
	cout << "Processing raw file " << filenum << endl;
	RawTreeFile RootFile;
	if (pathout.compare("") != 0) { 
		RootFile.SetPath(pathout);
	}
	if (!RootFile.Create(filenum)) {
		cout << "Input file not open!" << endl;
		for (int rio=0;rio<nfiles;rio++) {
			InputFile[rio]->Close();  
			delete InputFile[rio];
			delete InputEvent[rio];  
		}
		return;
	}
	bool goodevent = true;
	int cnt = 1000; // Event buffer not flushed at beginning of run
	while (InputFile[0]->ReadNextEvent(*InputEvent[0]) && goodevent) {
		float ev = InputFile[0]->GetPosition();
		if (ev ==-1) {
			cout << endl << "Warning: early file termination (check file format?)" << endl;
			break;
		}
		float nentries = InputFile[0]->GetLength();
		printf("Working....%3e/%3e  (%0.1lf %%)\r",ev,nentries,100*ev/nentries);
		for (int rio=1;rio<nfiles;rio++)
			InputFile[rio]->ReadNextEvent(*InputEvent[rio]);
		if (cnt > 0)
			cnt--;
		else
			goodevent = RootFile.FillEvent(InputEvent);
	}
	for (int rio=0;rio<nfiles;rio++) {
		InputFile[rio]->Close();  
		delete InputFile[rio];
		delete InputEvent[rio];  
	}
	RootFile.Write();
	RootFile.Close();
}





#endif // REPLAY_BIN_FILE_CPP__

