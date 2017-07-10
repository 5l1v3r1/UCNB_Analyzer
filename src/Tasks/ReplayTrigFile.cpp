// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ReplayTrigFile.cpp
// Purpose: Analysis task: replay a trig file into a trig tree file

#ifndef REPLAY_TRIG_FILE_CPP__
#define REPLAY_TRIG_FILE_CPP__

#include <math.h>

#include "ReplayTrigFile.hh"
#include "NIDec2015TrigBinFile.hh"
#include "NIMay2016TrigBinFile.hh"
#include "TrigTreeFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
ReplayTrigFile::ReplayTrigFile() {
	dataformat = 3; //default
}

ReplayTrigFile::ReplayTrigFile(int format, std::string pin, std::string pout) {
	Set(format, pin, pout);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
ReplayTrigFile::~ReplayTrigFile() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void ReplayTrigFile::Set(int format, std::string pin, std::string pout) {
	dataformat = format;
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void ReplayTrigFile::Go(int filenum) {
	//-----Open input/output files
	BinFile* InputFile;
	BinFile::BinEv_t* InputEvent;
	if (dataformat == 0 || dataformat == 1) {
		InputFile = new NIDec2015TrigBinFile();
		InputEvent = new NIDec2015TrigBinFile::DecTrigBinEv_t;
	}
	else if (dataformat == 2 || dataformat == 3) {
		InputFile = new NIMay2016TrigBinFile();
		InputEvent = new NIMay2016TrigBinFile::MayTrigBinEv_t;
	}
	if (pathin.compare("") != 0) { 
		InputFile->SetPath(pathin);
	}
	InputFile->Open(filenum);
	if (!InputFile->IsOpen()) {
		cout << "Input file not open!" << endl;
		delete InputFile;
		delete InputEvent;
		return;
	}	
	TrigTreeFile TrigFile;
	if (pathout.compare("") != 0) { 
		TrigFile.SetPath(pathout);
	}
	TrigFile.SetTmp();
	if (!TrigFile.Create(filenum)) {
		cout << "TrigFile Not Open!" << endl;
		InputFile->Close();  
		delete InputFile;
		delete InputEvent;  
		return;
	}
	while (InputFile->ReadNextEvent(*InputEvent)) {
		float ev = InputFile->GetPosition();
		float nentries = InputFile->GetLength();
		if (ev ==-1) {
			cout << endl << "Warning: early file termination (check file format?)" << endl;
			break;
		}
		printf("Working....%3e/%3e  (%0.1lf %%)\r",ev,nentries,100*ev/nentries);
		TrigFile.FillEvent(InputEvent);
	}
	InputFile->Close();
	delete InputFile;
	//-----Sort by timestamp
	TrigTreeFile NewFile;
	if (pathout.compare("") != 0) { 
		NewFile.SetPath(pathout);
	}
	if (!NewFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
	NewFile.Sort(TrigFile);
	NewFile.Write();
	NewFile.Close();
	std::string tmpname = pathout;
	tmpname.append("/");
	tmpname.append(TrigFile.GetName());
	TrigFile.Close();
	remove(tmpname.c_str());
}





#endif //ifndef REPLAY_TRIG_FILE_CPP__

