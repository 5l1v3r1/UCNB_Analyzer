// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ReplayCaBinFile.cpp
// Purpose: Analysis task: replay a bin file to correct waveforms

#ifndef REPLAY_CA_BIN_FILE_CPP__
#define REPLAY_CA_BIN_FILE_CPP__

#include "ReplayCaBinFile.hh"
#include "NIMay2017BinFile.hh"
#include "RawTreeFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
ReplayCaBinFile::ReplayCaBinFile() {
}

ReplayCaBinFile::ReplayCaBinFile(std::string pin, std::string pout) {
	Set(pin, pout);
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void ReplayCaBinFile::Set(std::string pin, std::string pout) {
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void ReplayCaBinFile::Go(int filenum) {
	//-----Open input/output files
	NIMay2017BinFile InputFile;
	InputFile.Open(pathin,filenum);
	vector<BinFile::BinEv_t*> InputEvent(1); //super dumb legacy reasons
	InputEvent[0] = new NIMay2017BinFile::MayBinEv_t;
	if (!InputFile.IsOpen()) {
		cout << "Input file not open!" << endl;			
		delete InputEvent[0];
		return;
	}
	//To do: convert to agreed-upon standard bin format
	//ReplayedBinFile OutputFile;   
	//OutputFile.Create(pathout,filenum);
	
	//-----Store in ROOT file
	cout << "Processing raw file " << filenum << endl;
	RawTreeFile RootFile;
	if (pathout.compare("") != 0) { 
		RootFile.SetPath(pathout);
	}
	if (!RootFile.Create(filenum)) {
		cout << "Output file not open!" << endl;	
		delete InputEvent[0];
		return;
	}
	bool goodevent = true;
	int cnt = 1000; // Event buffer not flushed at beginning of run
	while (InputFile.ReadNextEvent(*InputEvent[0]) && goodevent) {
		float ev = InputFile.GetPosition();
		if (ev ==-1) {
			cout << endl << "Warning: early file termination (check file format?)" << endl;
			break;
		}
		float nentries = InputFile.GetLength();
		printf("Working....%3e/%3e  (%0.1lf %%)\r",ev,nentries,100*ev/nentries);
		if (cnt > 0)
			cnt--;
		else
			goodevent = RootFile.FillEvent(InputEvent);
	}	
	delete InputEvent[0];
	InputFile.Close();  
	RootFile.Write();
	RootFile.Close();
}





#endif // REPLAY_CA_BIN_FILE_CPP__

