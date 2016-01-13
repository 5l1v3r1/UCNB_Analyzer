// File: Replay.cpp
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Replay data from NI DAQ
//
// Revision History:
// 2015/5/6:   LJB  Created 
// 2015/5/11:  LJB  Converts raw files into ROOT files
// 2015/11/21: LJB  Handle file versions with polymorphism, standardized
//                  ROOT file format

#ifndef REPLAY_CPP__
#define REPLAY_CPP__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstring>

#include "NIFeb2015BinFile.hh"
#include "NIJune2015BinFile.hh"
#include "RawTreeFile.hh"

void Usage(std::string program);
void DoRaw(int filenum);

std::string path;
//const std::string name = "Run0000_RIO0.bin";
//const std::string name = "Run_3.bin";
int dataformat; // 0 = feb, 1 = june
const int maxformat = 2; // only 2 formats so far

/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
  using std::cout;
  using std::endl;

  cout << "Welcome to the NI DAQ Replay " << endl;

  bool fileok = false;
  int i=1, filenum1, filenum2, avethresh, thresh, smpcoll;
  dataformat = 1;

  //Parse parameters
  while (i+1 <= argc) {
    if (strcmp(argv[i],"-p")==0) {
		i++;
		if (i+1 > argc) {
			cout << "Missing argument for -p" << endl;
			Usage(argv[0]);
			return 1;
		}
		if (argv[i][0] == '-') {
			cout << "Missing valid argument for -p" << endl;
			Usage(argv[0]);
			return 1;
		}
		path += argv[i];
		i++;
		if (gSystem->AccessPathName(path.c_str())) {
			cout << "bad path: " << path << " not found " << endl;
			return 1;
		}
	}
    else if (strcmp(argv[i],"-f")==0) {
      i++;
      if (i+1 > argc) {
		cout << "Missing argument for -f" << endl;
		Usage(argv[0]);
		return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
		cout << "Missing valid argument for -f" << endl;
		Usage(argv[0]);
		return 1;
      }
      filenum1 = atoi(argv[i]);
      filenum2 = filenum1;
      i++;
      if (i+1 <= argc) {
		if (argv[i][0] >= '0' && argv[i][0] <= '9') {
		filenum2 = atoi(argv[i]);
		i++;
		}
      }
      fileok = true;
    }
    else if (strcmp(argv[i],"-format")==0) {
      i++;
      if (i+1 > argc) {
		cout << "Missing argument for -format" << endl;
		Usage(argv[0]);
		return 1;
      }
      dataformat = atoi(argv[i]);
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
		cout << "Missing valid argument for -format" << endl;
		Usage(argv[0]);
		return 1;
      }
      if (!(dataformat >= 0 && dataformat < maxformat)) {
		cout << "Missing valid argument for -format" << endl;
		Usage(argv[0]);
		return 1;
      }
      i++;
    }
    else {
      cout << "Unrecognized parameter" << endl;
      Usage(argv[0]);
      return 1;
    }
  }
  if (!fileok) {
    cout << "No file indicated" << endl;
    Usage(argv[0]);
    return 1;
  }
  for (int filenum = filenum1; filenum <= filenum2; filenum++) {
    DoRaw(filenum);
  }

  cout << "Done." << endl;
  return 0; 
}

void Usage(std::string program) {
  cout << "Usage: " << program  << " -f #1 [#2]" << endl;
  cout << "-format <format (0=feb,1=june)> to set data format (default 1)" << endl;
}

void DoRaw(int filenum) {
	//-----Open input files
	int nfiles = (dataformat == 0) ? NRIO : 1; // may need nfiles as parameter in future?
	vector<BinFile*> InputFile(nfiles);
	vector<BinFile::BinEv_t*> InputEvent(nfiles);
	for (int rio=0;rio<nfiles;rio++) { 
		if (dataformat == 0) {
			InputEvent[rio] = new NIFeb2015BinFile::FebBinEv_t;
			InputFile[rio] = new NIFeb2015BinFile();
		}
		else if (dataformat == 1) {
			InputEvent[0] = new NIJune2015BinFile::JuneBinEv_t;
			InputFile[0] = new NIJune2015BinFile();
		}
		InputFile[rio]->SetPath(path);
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
	RootFile.SetPath(path);
	RootFile.Create(filenum);
	while (InputFile[0]->ReadNextEvent(*InputEvent[0])) {
		float ev = InputFile[0]->GetPosition();
		float nentries = InputFile[0]->GetLength();
		printf("Working....%3e/%3e  (%0.1lf %%)\r",ev,nentries,100*ev/nentries);
		for (int rio=1;rio<nfiles;rio++)
			InputFile[rio]->ReadNextEvent(*InputEvent[rio]);
		RootFile.FillEvent(InputEvent);
	}
	RootFile.Write();
	RootFile.Close();
	for (int rio=0;rio<nfiles;rio++) {
		InputFile[rio]->Close();  
		delete InputFile[rio];
		delete InputEvent[rio];  
	}
}


#endif // __REPLAY_CPP__