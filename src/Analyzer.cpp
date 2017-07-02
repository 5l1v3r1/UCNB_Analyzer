// Copyright 2016.  Los Alamos National Security, LLC.
// Copyright 2016.  UT-Battelle, LLC.
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

#include "NIDec2015TrigBinFile.hh"
#include "NIMay2016TrigBinFile.hh"
#include "NIFeb2015BinFile.hh"
#include "NIJune2015BinFile.hh"
#include "NIMay2016BinFile.hh"
#include "RawTreeFile.hh"
#include "TrigTreeFile.hh"
#include "FitTreeFile.hh"
#include "TrapTreeFile.hh"
#include "EventTreeFile.hh"
#include "WaveformAnalyzer.hh"
#include "WaveformAverage.hh"
#include "SiCalibrator.hh"

#include "TApplication.h"
#include "TRint.h"
#include "TStyle.h"

using std::cin;  
using std::cout;
using std::endl;
using std::ifstream;

void Usage(std::string program);
void DoRaw(int filenum);
void DoTrig(int filenum);
void DoTrap(int filenum, int thresh, int decay, int shaping, int top);
void DoFit(int filenum, int thresh);
void DoColl(int filenum, int smp, std::string calibfile);
void DoAve(int filenum, int decay, int shaping, int top);
void DoCalib(int thresh, int decay, int shaping, int top);
void DoShapeScan(int scansrc);
double ceb(double*x, double* par);
double cex(double*x, double* par);
double snb(double*x, double* par);
double snx(double*x, double* par);
double bib(double*x, double* par);
double invx(double* x, double* par);
double enc2(double* x, double* par);

std::string path;
int dataformat; // 0 = feb15, 1 = june15, 2 = may16, 3 = may17
const int maxformat = 4; // 4 formats so far...
const bool dodraw = true;

/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  

  cout << "Welcome to UCNB_Analyzer v1.2.3" << endl;

  bool doraw = false, dotrig = false, dotrap = false, dofit = false, docoll = false, doave = false, docal = false, doshapescan = false;
  bool fileok = false;
  int i=1, filenum1, filenum2, fitthresh=-1, trapthresh=-1, decay=-1, shaping=-1, top=-1, smpcoll=-1, avethresh=-1, scansrc=-1;
  dataformat = 3;
  path = "";
  std::string calibfile = "";

  //-----Parse parameters
  while (i+1 <= argc) {
    if ((strcmp(argv[i],"-p")==0)||(strcmp(argv[i],"-path")==0)) {
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
    else if (strcmp(argv[i],"-calibfile")==0) {
		i++;
		if (i+1 > argc) {
			cout << "Missing argument for -cailbfile" << endl;
			Usage(argv[0]);
			return 1;
		}
		if (argv[i][0] == '-') {
			cout << "Missing valid argument for -calibfile" << endl;
			Usage(argv[0]);
			return 1;
		}
		calibfile += argv[i];
		i++;
		if (gSystem->AccessPathName(calibfile.c_str())) {
			cout << "bad file: " << path << " not found " << endl;
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
    else if (strcmp(argv[i],"-raw")==0) {
      doraw = true;
      i++;
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
    else if (strcmp(argv[i],"-trig")==0) {
      dotrig = true;
      i++;
    }
    else if (strcmp(argv[i],"-fit")==0) {
      dofit = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -fit" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -fit" << endl;
	Usage(argv[0]);
	return 1;
      }
      fitthresh = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-trap")==0) {
      dotrap = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -trap" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -trap" << endl;
	Usage(argv[0]);
	return 1;
      }
      trapthresh = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-decay")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -decay" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -decay" << endl;
	Usage(argv[0]);
	return 1;
      }
      decay = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-shaping")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -shaping" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -shaping" << endl;
	Usage(argv[0]);
	return 1;
      }
      shaping = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-top")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -top" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -top" << endl;
	Usage(argv[0]);
	return 1;
      }
      top = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-coll")==0) {
      docoll = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -coll" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -coll" << endl;
	Usage(argv[0]);
	return 1;
      }
      smpcoll = atoi(argv[i]);
      i++;      
    }
    else if (strcmp(argv[i],"-ave")==0) {
      doave = true;
      i++;
    }
    else if (strcmp(argv[i],"-cal")==0) {
      docal = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -cal" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -cal" << endl;
	Usage(argv[0]);
	return 1;
      }
      trapthresh = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-shapescan")==0) {
      doshapescan = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -shapescan" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -shapescan" << endl;
	Usage(argv[0]);
	return 1;
      }
      scansrc = atoi(argv[i]);
      i++;
    }
    else {
      cout << "Unrecognized parameter " << argv[i] << endl;
      Usage(argv[0]);
      return 1;
    }
  }
  if (!docal && !doshapescan && (!fileok || (!doraw && !dotrig && !dotrap && !dofit && !docoll &&!doave))) {
    if (!fileok)
      cout << "No file indicated" << endl;
    if (!doraw && !dotrig && !dotrap && !dofit && !docoll && !doave)
      cout << "What do you want to do? " << endl;
    Usage(argv[0]);
    return 1;
  }
  if ((dotrap || docal) && (decay == -1 || shaping == -1 || top == -1)) {
	  cout << "Specify trap parameters decay/shaping/top" << endl;
    Usage(argv[0]);
    return 1;
  }
  if (doave && !dotrap) {
	cout << "Use average with trap filter" << endl;
	Usage(argv[0]);
    return 1;
  }
  
  
	TApplication* myapp = 0;
  //-----Process data
  if (fileok) {
	for (int filenum = filenum1; filenum <= filenum2; filenum++) {
		if (doraw)
			DoRaw(filenum);
		if (dotrig)
			DoTrig(filenum);
		if (dotrap && !doave)
			DoTrap(filenum, trapthresh, decay, shaping, top);
		if (dofit)
			DoFit(filenum, fitthresh);
		if (docoll)
			DoColl(filenum, smpcoll, calibfile);
		if (doave && dotrap)
			DoAve(filenum, decay, shaping, top);
	}
  }
  if (docal) {
	  DoCalib(trapthresh, decay, shaping, top);
  }
  if (doshapescan) {
	  if (dodraw)
		myapp = new TApplication("myapp",0,0);
	  DoShapeScan(scansrc);
  }
  cout << "Done." << endl;
  if (dodraw)
	if (myapp != 0)
		myapp->Run();
  return 0; 

}

void Usage(std::string program) {
  cout << "Usage:   " << program  << " -f #1 [#2] -p path" << endl;
  cout << "-raw to convert .bin files to .root" << endl;
  cout << "      -format <format (0=feb,1=june,2=may)> to set data format (default 2)" << endl;
  cout << "-trig to convert .trig files to .root" << endl;
  cout << "-trap <threshold> to filter waveforms for events using linear trapezoid" << endl;
  cout << "      -decay <smp> to set linear trap decay constant" << endl;
  cout << "      -shaping <smp> to set linear trap shaping time" << endl;
  cout << "      -top <smp> to set linear trap flat top length" << endl;
  cout << "-ave (with -trap) to build average waveform" << endl;
  cout << "-fit <threshold> to filter waveforms for events using pulse fitting" << endl;
  cout << "-coll <time in smp (250smp = 1us)> to collect single-event coincidences" << endl;
  cout << "-cal <threshold> to perform calibration" << endl;
  cout << "-shapescan <src> o perform shaping scan on 207Bi (src=1), 113Sn (src=2-3), or 139Ce (src=4-5) betas or x-rays" << endl;
}

void DoRaw(int filenum) {
	//-----Open input/output files
	int nfiles = (dataformat == 0) ? MAXRIO : 1; // may need nfiles as parameter in future?
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
		if (path.compare("") != 0) { 
			InputFile[rio]->SetPath(path);
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
	if (path.compare("") != 0) { 
		RootFile.SetPath(path);
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

void DoTrig(int filenum) {
	//-----Open input/output files
	BinFile* InputFile;
	BinFile::BinEv_t* InputEvent;
	if (dataformat == 0 || dataformat == 1) {
		InputFile = new NIDec2015TrigBinFile();
		InputEvent = new NIDec2015TrigBinFile::DecTrigBinEv_t;
	}
	else if (dataformat == 2) {
		InputFile = new NIMay2016TrigBinFile();
		InputEvent = new NIMay2016TrigBinFile::MayTrigBinEv_t;
	}
	if (path.compare("") != 0) { 
		InputFile->SetPath(path);
	}
	InputFile->Open(filenum);
	if (!InputFile->IsOpen()) {
		cout << "Input file not open!" << endl;
		delete InputFile;
		delete InputEvent;
		return;
	}	
	TrigTreeFile TrigFile;
	if (path.compare("") != 0) { 
		TrigFile.SetPath(path);
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
	if (path.compare("") != 0) { 
		NewFile.SetPath(path);
	}
	if (!NewFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
	NewFile.Sort(TrigFile);
	NewFile.Write();
	NewFile.Close();
	std::string tmpname = path;
	tmpname.append("/");
	tmpname.append(TrigFile.GetName());
	TrigFile.Close();
	remove(tmpname.c_str());
}

void DoTrap(int filenum, int thresh, int decay, int shaping, int top) {
	//-----Open input/output files
	RawTreeFile RootFile;
	if (path.compare("") != 0) { 
		RootFile.SetPath(path);
	}
	if (!RootFile.Open(filenum)) {
		cout << "File Not Open!" << endl;
		return;
	}
	TrapTreeFile TrapFile;
	if (path.compare("") != 0) { 
		TrapFile.SetPath(path);
	}
	TrapFile.SetTmp();
	if (!TrapFile.Create(filenum,decay,shaping,top)) {
		cout << "File Not Open!" << endl;
		return;
	}
	WaveformAnalyzer WA;
	WA.SetTrapPars(decay,shaping,top);
	cout << "Applying trap filter (decay/rise/top = " << decay <<"/" << shaping << "/" << top << ") to file " << filenum << endl;
	int filecount = 0;
	//Shaping time scan hack, used for 1 set of runs
	//int dlow = 250, dmid = 1200, dhi = 2000;
	//int decaylist[] = {dlow,dlow,dlow,dlow,dlow, 			dmid,dmid,dlow,dmid,dlow,dhi,dlow,dlow,dhi,dhi,					dlow,dlow,dlow,dlow,dlow};
	do {
		int nentries = RootFile.GetNumEvents();
		//-----Find triggers
		for (int ev=0;ev<nentries;ev++) {
			printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
			RootFile.GetEvent(ev);
			//Shaping time scan hack, used for 1 set of runs
			//if (RootFile.NI_event.ch > 20) 
			//	WA.SetTrapPars(dlow,shaping,top);
			//else
			//	WA.SetTrapPars(decaylist[RootFile.NI_event.ch],shaping,top);
			WA.MakeTrap(RootFile.NI_event.length, RootFile.NI_event.wave);
			vector<trigger_t> triglist; 
			//maybe add veto for events too early in file
			WA.GetTriggers(thresh,triglist);
			for (int t=0;t<triglist.size();t++) {
				TrapFile.Trap_event.MaxE = triglist[t].TrapE;
				TrapFile.Trap_event.AveE = triglist[t].AveTrapE;
				TrapFile.Trap_event.MidE = triglist[t].MidTrapE;
				TrapFile.Trap_event.t = triglist[t].TrapT + (double)RootFile.NI_event.	timestamp;
				TrapFile.Trap_event.Flat0 = triglist[t].Flat0;
				TrapFile.Trap_event.Flat1 = triglist[t].Flat1;
				TrapFile.Trap_event.up = triglist[t].up;
				TrapFile.Trap_event.down = triglist[t].down;
				TrapFile.Trap_event.ch = RootFile.NI_event.ch;
				TrapFile.Trap_event.wavefile = filecount;
				TrapFile.Trap_event.waveev = ev;
				TrapFile.FillTree();
			}
		}//ev < NumEvents
		RootFile.Close();
		if (!RootFile.Open(filenum,++filecount))
			filecount = -1;
	} while (filecount != -1);
	//-----Sort by timestamp
	TrapTreeFile NewFile;
	if (path.compare("") != 0) { 
		NewFile.SetPath(path);
	}
	if (!NewFile.Create(filenum,decay,shaping,top)) {
		cout << "Output file not open!" << endl;
		return;
	}
	NewFile.Sort(TrapFile);
	NewFile.Write();
	NewFile.Close();
	std::string tmpname = path;
	tmpname.append("/");
	tmpname.append(TrapFile.GetName());
	TrapFile.Close();
	remove(tmpname.c_str());
}

void DoFit(int filenum, int thresh) {	
	//-----Open input/output files
	RawTreeFile RootFile;
	if (path.compare("") != 0) { 
		RootFile.SetPath(path);
	}
	if (!RootFile.Open(filenum)) {
		cout << "Input file not open!" << endl;
		return;
	}
	FitTreeFile FitFile;
	if (path.compare("") != 0) { 
		FitFile.SetPath(path);
	}
	if (!FitFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
	WaveformAnalyzer WA;
	cout << "Fitting waveforms in file " << filenum << endl;
	int filecount = 0;
	do {
		int nentries = RootFile.GetNumEvents();
		//-----Find triggers
		for (int ev=0;ev<nentries;ev++) {
			printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
			RootFile.GetEvent(ev);
			WA.MakeTrap(RootFile.NI_event.length, RootFile.NI_event.wave);
			vector<trigger_t> triglist; 
			WA.FitWave(thresh,triglist);
			for (int t=0;t<triglist.size();t++) {
				FitFile.Fit_event.E = triglist[t].E;
				FitFile.Fit_event.t = triglist[t].T + (double)RootFile.NI_event.timestamp;
				FitFile.Fit_event.shaping = triglist[t].Shaping;
				FitFile.Fit_event.integ = triglist[t].Integration;
				FitFile.Fit_event.chi2 = triglist[t].Chi2;
				FitFile.Fit_event.ch = RootFile.NI_event.ch;
				FitFile.Fit_event.wavefile= filecount;
				FitFile.Fit_event.waveev = ev;
				FitFile.FillTree();
			}
		}//ev < NumEvents
		RootFile.Close();	
		if (!RootFile.Open(filenum,++filecount))
			filecount = -1;
	} while (filecount != -1);
	FitFile.Write();
	cout << "Done" << endl;
	FitFile.Close();
	RootFile.Close();	
}

void DoColl(int filenum, int smp, std::string calibfile) {
  cout << "Collecting single-event coincidences in file " << filenum << endl;
  //-----Open input/output files
  TrigTreeFile InputFile;  //expand to trig/trap/fit/coll? at some point
  //TrapTreeFile InputFile;  //expand to trig/trap/fit/coll? at some point
  if (path.compare("") != 0) { 
	InputFile.SetPath(path);
  }
  //  if (!InputFile.Open(filenum, 200, 150,150)) {
  if (!InputFile.Open(filenum)) {
    cout << "Input file Not Open!" << endl;
    return;
  }
  EventTreeFile EventFile;
  if (path.compare("") != 0) { 
	EventFile.SetPath(path);
  }
  if (!EventFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
  int nentries = InputFile.GetNumEvents();
  int StartEv = 0;
  if (nentries == 0){
    EventFile.Write();
    cout << "No triggers: done" << endl;
    EventFile.Close();
    InputFile.Close();
    return;
  }
  
  //-----Open calibration file
  double q0[48],q1[48],q2[48];
  for(int i=0; i<48; i++) {
	  q0[i]=0;q1[i]=1;q2[i]=0;
  }
  if (calibfile.compare("") != 0) { 
	ifstream cal(calibfile.c_str());
	for(int i=0; i<48; i++) {
      cal>>q2[i]>>q1[i]>>q0[i];
    }
  }
  int numch = MAXCH*MAXRIO;
  double tlast = -1, Elast = -1;
  do {
    //-----Get start trigger of event
    printf("Working....%d/%d  (%d %%)\r",StartEv,nentries,100*StartEv/nentries);
    for (int ch=0;ch<numch;ch++)
      EventFile.myEvent.E[ch] = 0;
    InputFile.GetEvent(StartEv);
	//    EventFile.myEvent.t = InputFile.Trap_event.t;
    EventFile.myEvent.t = InputFile.Trig_event.t;
	EventFile.myEvent.tprev = (tlast == -1)? -1 : (EventFile.myEvent.t - tlast);
	EventFile.myEvent.Eprev = (Elast == -1)? -1 : Elast;
	EventFile.myEvent.numch = 0;
    //-----Get triggers within window
    int ev = StartEv;
    do {
		//	  double Ecal = q2[InputFile.Trap_event.ch]*InputFile.Trap_event.AveE*InputFile.Trap_event.AveE/16. + q1[InputFile.Trap_event.ch]*InputFile.Trap_event.AveE/4. + q0[InputFile.Trap_event.ch];
	  double Ecal = q2[InputFile.Trig_event.ch]*InputFile.Trig_event.E*InputFile.Trig_event.E/16. + q1[InputFile.Trig_event.ch]*InputFile.Trig_event.E/4. + q0[InputFile.Trig_event.ch];
	  if (Ecal > 5)
		EventFile.myEvent.E[InputFile.Trig_event.ch] += Ecal;
//		EventFile.myEvent.E[InputFile.Trap_event.ch] += Ecal;
      ev++;
      if (ev < nentries)
		InputFile.GetEvent(ev);
      }while (ev < nentries && (InputFile.Trig_event.t - EventFile.myEvent.t)<smp);
	  //      }while (ev < nentries && (InputFile.Trap_event.t - EventFile.myEvent.t)<smp);
	EventFile.myEvent.Esum = 0;
	for (int ch = 0; ch < MAXCH*MAXRIO; ch++) {
		if (EventFile.myEvent.E[ch] > 5) {
			EventFile.myEvent.numch++;
			EventFile.myEvent.Esum += EventFile.myEvent.E[ch];
		}
	}
	EventFile.myEvent.run = filenum;
	EventFile.myEvent.wavefile = -1;
	EventFile.myEvent.waveev = -1;
	if (EventFile.myEvent.Esum > 5) {
		EventFile.FillTree();
		Elast = EventFile.myEvent.Esum;
		tlast = EventFile.myEvent.t;
	}
    StartEv = ev;
  } while (StartEv < nentries);
  EventFile.Write();
  cout << "Done" << endl;
  EventFile.Close();
  InputFile.Close();
}

void DoAve(int filenum, int decay, int shaping, int top) {
	//-----Open input/output files
	RawTreeFile RootFile;
	if (path.compare("") != 0) { 
		RootFile.SetPath(path);
	}
	if (!RootFile.Open(filenum)) {
		cout << "Input file not open!" << endl;
		return;
	}
	TrapTreeFile TrapFile;
	if (path.compare("") != 0) { 
		TrapFile.SetPath(path);
	}
	if (!TrapFile.Open(filenum, decay, shaping, top)) {
		cout << "Trap file Not Open!" << endl;
		//todo Go ahead and run it
		return;
	}
	int nentries = TrapFile.GetNumEvents();
	WaveformAnalyzer WA;
	vector<vector<WaveformAverage> > wavelist;
	int NumAmp = 5, MaxAmp = 1000, PreSmpBins = 100, SmpBins = 1000, NegAmpBins = 500, AmpBins = 3000;
	wavelist.resize(NumAmp);
	for (int i=0;i<wavelist.size();i++) {
		wavelist[i].resize(MAXCH*MAXRIO);
		for (int j=0;j<wavelist[i].size();j++) {
			wavelist[i][j].SetBins(PreSmpBins,SmpBins,NegAmpBins,AmpBins);
			char tempstr[255];
			sprintf(tempstr,"ch%dE%d",j,i);
			std::string hname = tempstr;
			wavelist[i][j].SetHistName(hname);
		}
	}
	
	//-----Build distribution of up/down/down-up to guess good range
	vector<vector<double> > updist(MAXCH*MAXRIO,vector<double>(RootFile.NI_event.length,0));
	vector<vector<double> > downdist(MAXCH*MAXRIO,vector<double>(RootFile.NI_event.length,0));
	vector<vector<double> > downupdist(MAXCH*MAXRIO,vector<double>(RootFile.NI_event.length,0));
	vector<int> evcnt(MAXCH*MAXRIO,0);
	for (int ev=0;ev<nentries;ev++) {
		TrapFile.GetEvent(ev);
		if (TrapFile.Trap_event.AveE > 200 && TrapFile.Trap_event.down < RootFile.NI_event.length) {
			evcnt[TrapFile.Trap_event.ch]++;
			updist[TrapFile.Trap_event.ch][TrapFile.Trap_event.up]++;
			downdist[TrapFile.Trap_event.ch][TrapFile.Trap_event.down]++;
			downupdist[TrapFile.Trap_event.ch][TrapFile.Trap_event.down-TrapFile.Trap_event.up]++;
		}
	}
	vector<double> maxup(MAXCH*MAXRIO,0);
	vector<double> hwhmup(MAXCH*MAXRIO,0);
	vector<double> maxdown(MAXCH*MAXRIO,0);
	vector<double> hwhmdown(MAXCH*MAXRIO,0);
	vector<double> maxdownup(MAXCH*MAXRIO,0);
	vector<double> hwhmdownup(MAXCH*MAXRIO,0);
	for (int ch=0;ch<maxup.size();ch++) {
		double maxu = 0, maxd = 0, maxdu = 0;
		if (evcnt[ch] > 30) {
			for (int smp=0;smp<RootFile.NI_event.length;smp++) {
				if (maxu < updist[ch][smp]) {
					maxu = updist[ch][smp];
					maxup[ch] = smp;
				}
				if (maxd < downdist[ch][smp]) {
					maxd = downdist[ch][smp];
					maxdown[ch] = smp;
				}
				if (maxdu < downupdist[ch][smp]) {
					maxdu = downupdist[ch][smp];
					maxdownup[ch] = smp;
				}
			}
			double valR, valL;
			do {
				hwhmup[ch]++;
				valR = updist[ch][maxup[ch]+hwhmup[ch]];
				valL = updist[ch][maxup[ch]-hwhmup[ch]];
			}while(valR>maxu/2 && valL>maxu/2);
			do {
				hwhmdown[ch]++;
				valR = downdist[ch][maxdown[ch]+hwhmdown[ch]];
					valL = downdist[ch][maxdown[ch]-hwhmdown[ch]];
			}while(valR>maxd/2 && valL>maxd/2);
			do {
				hwhmdownup[ch]++;
				valR = downupdist[ch][maxdownup[ch]+hwhmdownup[ch]];
				valL = downupdist[ch][maxdownup[ch]-hwhmdownup[ch]];
			}while(valR>maxdu/2 && valL>maxdu/2);
		}
	}
	
	//-----Build distribution of waveforms
	for (int ev=0;ev<nentries/100;ev++) {
		printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
		TrapFile.GetEvent(ev);
		RootFile.GetEvent(TrapFile.Trap_event.waveev);
		double T0 = (TrapFile.Trap_event.up + TrapFile.Trap_event.down)/2 - top/2 - shaping; 
		int ch = TrapFile.Trap_event.ch;
		if (hwhmup[ch] > 0 && hwhmdown[ch] > 0 && hwhmdownup[ch] > 0 && TrapFile.Trap_event.up>maxup[ch]-hwhmup[ch] && TrapFile.Trap_event.up<maxup[ch]+hwhmup[ch] && TrapFile.Trap_event.down>maxdown[ch]-hwhmdown[ch] && TrapFile.Trap_event.down<maxdown[ch]+hwhmdown[ch] && (TrapFile.Trap_event.down-TrapFile.Trap_event.up) > maxdownup[ch]-hwhmdownup[ch] && (TrapFile.Trap_event.down-TrapFile.Trap_event.up) < maxdownup[ch]+hwhmdownup[ch] && TrapFile.Trap_event.AveE > 0 && TrapFile.Trap_event.AveE < MaxAmp && T0 - PreSmpBins >= 0 && T0 + SmpBins < RootFile.NI_event.length) {
			int whichamp = NumAmp*TrapFile.Trap_event.AveE / MaxAmp;
			vector<short> wave(RootFile.NI_event.wave,RootFile.NI_event.wave+RootFile.NI_event.length);
			wavelist[whichamp][ch].AddToAve(wave,T0,1./TrapFile.Trap_event.AveE);
		}
	}
	RootFile.Close();
	TrapFile.Close();
	
	//-----Write to file
	for (int ch=0;ch<MAXCH*MAXRIO;ch++) {
		if (hwhmup[ch] > 0 && hwhmdown[ch] > 0 && hwhmdownup[ch] > 0) {
			char tempstr[255];
			sprintf(tempstr,"Avech%d.root",ch);
			std::string filename = path;
			filename.append("/");
			filename.append(tempstr);
			cout << "Writing " << filename << endl;
			TFile* newfile = new TFile(filename.c_str(),"RECREATE");
			vector<TH2D*> hDist(wavelist.size(),0);
			for (int i=0;i<wavelist.size();i++) {
				wavelist[i][ch].GetDist(hDist[i]);
			}	
			newfile->Write();
			newfile->Close();
		}
	}
}

void DoCalib(int thresh, int decay, int shaping, int top) {
	SiCalibrator calib(thresh, decay, shaping, top);
	if (path.compare("") != 0) { 
		calib.SetPath(path);
	}
	//-----To do: move detector channel mapping to new object
	//east = 0, west = 1
	int det[MAXPIX];
	for (int i=0;i<8;i++)
		det[i] = 0;
	for (int i=8;i<32;i++)
		det[i] = 1;
	for (int i=32;i<48;i++)
		det[i] = 0;
	vector<int> detector;
	detector.assign(&det[0],&det[0]+MAXPIX);
	calib.DefineDetector(detector);
	//-----To do: upload run log by file
	int rl[38] = {10,11,12,13,14,15,16,17,18,19,20,21,30,31,32,33,34,36,37,38,39,40,41,42,43,44,45,50,51,52,53,54,55,56,57,58,59,60};
	vector<int> runlist;
	runlist.assign(&rl[0],&rl[0]+38);
	int typ1[38], typ2[38];
	for (int i=0;i<=11;i++) {
		typ1[i] = 3; //Sn113x
		typ2[i] = 1; //Bi207x
	}
	for (int i=12;i<=26;i++) {
		typ1[i] = 1; //Bi207x
		typ2[i] = 5; //Ce139x
	}
	for (int i=27;i<38;i++) {
		typ1[i] = 5; //Ce139x
		typ2[i] = 3; //Sn113x
	}
	vector<int> type1, type2;
	type1.assign(&typ1[0],&typ1[0]+38);
	type2.assign(&typ2[0],&typ2[0]+38);
	calib.DefineRunLog(runlist,type1,type2);
	//-----Build histograms
	cout << "Building histograms.." << endl;
	TrapTreeFile trapfile;
	if (path.compare("") != 0) { 
		trapfile.SetPath(path);
	}
	//-----To Do:  Check for exising files
	calib.BuildHists(trapfile);
	//calib.FindPeaks();
	cout << "Fitting spectra.."<< endl;
	calib.MatchPeaks();
	//cout << "Hit Ctrl+C to stop viewing plot and quit program." << endl;
}

void DoShapeScan(int src) {
	if (src < 0 || src > 5 || src == 1) {
		cout << "Shaping Scan aborted. Source " << src << " not yet handled." << endl;
		return;
	}
	//Hard-coded parameters, maybe use config file eventually
	int thresh = 35; int decay = 200; int top = 300;
	const int num = 14;
	//int sscan[] = {50,60,70,100,150,200,250,300,350,400,450};
	int sscan[] = {35,50,60,70,100,150,200,250,300,350,400,450,500,600};
	//double sscan[] = {35,50,60,70,100,150,200,250,300,350,400,450,500,600};
	//3.62 at 300K, 3.72 at 80K
	double eVtoENC = 3.7;
	//-----Create output file
	std::string filename = path;
	filename.append("/");
	if(src==0)
		filename.append("ShapingScanBib.root");
	if(src==2)
		filename.append("ShapingScanSnb.root");
	if(src==3)
		filename.append("ShapingScanSnx.root");
	if(src==4)
		filename.append("ShapingScanCeb.root");
	if(src==5)
		filename.append("ShapingScanCex.root");
	TFile* myfile = new TFile(filename.c_str(),"RECREATE");
	SiCalibrator calib;
	if (path.compare("") != 0) { 
		calib.SetPath(path);
	}
	TF1* fenc = new TF1("fenc",enc2,10,1000,3);
	//-----Shaping scans
	double sigma[num];
	double ENC2[num];
	double scan[num];
	double fwhm[num];
	double chi2[num];
	TH1D* hh1 = new TH1D("h1","h1",48,0,48);
	TH1D* hh2 = new TH1D("h2","h2",48,0,48);
	TH1D* hh3 = new TH1D("h3","h3",48,0,48);
	TH1D* hminsig = new TH1D("hminsig","hminsig",48,0,48);
	int cnt;
	double Elo, Ehi;
	TCanvas* canv = new TCanvas();
	gStyle->SetOptFit(11);
	bool first = true;
	std::string pdfname;
	for (int ch=0; ch< 48; ch++) {
		cnt = 0;
		printf("Channel %d/%d\r",ch,48);fflush(stdout);
		double snpars[6] = {1,1,3,20,1,0};
		double cepars[6] = {1,1,3,25,2,0};
		double bibpars[5] = {1,1,3,0,0};
		double snbpars[5] = {1,1,3,0,0};
		double cebpars[6] = {1,1,3,10,0,0};
		for (int i=0;i<num;i++) {
			int shape = sscan[i]; //top = shape;
			calib.SetPars(thresh, decay, shape, top);
			if (!calib.Load()) {
				delete fenc;
				delete hh1;
				delete hh2;
				delete hh3;
				delete hminsig;
				delete canv;
				myfile->Close();
				return;
			}
			TH1D* hp;
			if (src%2 == 1)
				hp = calib.GetHist(src,ch);
			else
				hp = calib.GetHist(src+1,ch);
			int rebin = 2;
			hp->Rebin(rebin);
			if (hp!= 0) {
				if (src == 0) {
					Elo = 1000; 
					//Follow source window with "gaussian" movement
					Ehi = 2500 + 2000*TMath::Exp(-1.*(350-shape)*(350-shape)/2./150./150.);
				}
				if (src == 2) {
					Elo = 900; 
					Ehi = 2500;
				}
				if (src == 3) {
					Elo = 10; 
					Ehi = 500;
				}
				if (src == 4) {
					Elo = 350; 
					Ehi = 1000;
				}
				if (src == 5) {
					Elo = 100; 
					Ehi = 300;
				}
				char hname[25];
				sprintf(hname,"hch%dsh%d",ch,shape);
				hp->SetTitle(hname);
				hp->GetXaxis()->SetRangeUser(Elo,Ehi);
				if (hp->Integral(Elo,Ehi) > 0) {
					TF1* fitf;
					if (src==0) {
						fitf = new TF1("fitf",bib,Elo,Ehi,5);
						fitf->SetParameters(&bibpars[0]);
						fitf->SetParLimits(0,0.1,0.5);
						pdfname = "Snbscan.pdf";
					}
					if (src==2) {
						fitf = new TF1("fitf",snb,Elo,Ehi,5);
						fitf->SetParameters(&snbpars[0]);
						fitf->SetParLimits(0,0.1,0.5);
						pdfname = "Snbscan.pdf";
					}
					if (src==3) {
						fitf = new TF1("fitf",snx,Elo,Ehi,6);
						fitf->SetParameters(&snpars[0]);
						fitf->SetParLimits(0,0.1,0.3);
						fitf->SetParLimits(3,1,30);
						pdfname = "Snxscan.pdf";
					}
					if (src==4) {						
						fitf = new TF1("fitf",ceb,Elo,Ehi,6);
						fitf->SetParameters(&cebpars[0]);
						fitf->SetParLimits(0,0.15,0.5);
						pdfname = "Cebscan.pdf";
					}
					if (src==5) {						
						fitf = new TF1("fitf",cex,Elo,Ehi,6);
						fitf->SetParameters(&cepars[0]);
						fitf->SetParLimits(0,0.15,0.3);
						fitf->SetParLimits(3,10,30);
						pdfname = "Cexscan.pdf";
					}
					int maxbin = hp->GetMaximumBin();
					int max = maxbin*rebin;
					double calib;
					if (src==0)
						calib = 482./(double)max;
					if (src==2)
						calib = 364./(double)max;
					if (src==3)
						calib = 24./(double)max;
					if (src==4)
						calib = 127./(double)max;
					if (src==5)
						calib = 33.5/(double)max;
					fitf->SetParameter(0,calib);
					double scale = hp->GetBinContent(maxbin)/fitf->Eval(max);
					fitf->SetParameter(1,scale);
					fitf->SetParName(2,"sigma");
					fitf->SetParLimits(2,1,10);
					fitf->SetNpx(1000);
					if (src != 2) {
						fitf->SetParLimits(4,0,10);
						fitf->FixParameter(5,0);
					}
					int fitresult = hp->Fit(fitf,"QN","",Elo,Ehi);
					fitf->ReleaseParameter(5);
					fitresult = hp->Fit(fitf,"QN","",Elo,Ehi);
					double chi2ndf = fitf->GetChisquare()/fitf->GetNDF();
					if (chi2ndf > 2.5) {
						fitf->SetNpx(2000);
						if (src==3)	fitf->SetParameters(&snpars[0]);
						if (src==5)	fitf->SetParameters(&cepars[0]);
						fitresult = hp->Fit(fitf,"QN","",Elo,Ehi);
					}
					chi2ndf = fitf->GetChisquare()/fitf->GetNDF();
					sigma[cnt] = fitf->GetParameter(2);
					/*
					if (dodraw && chi2ndf > 2.5) {
						cout << "ch " << ch << " shape: " << shape << " : " << sigma[cnt];
						cout << ", x^2 " << chi2ndf << endl;
						cout << "calib " << 33.5/fitf->GetParameter(0) << endl;
						for (int i=0;i<6;i++)
							cout << "p" << i << "=" << fitf->GetParameter(i) << "; ";
						cout << endl;
					}
					*/
					if (dodraw) {
						fitresult = hp->Fit(fitf,"Q","",Elo,Ehi);
						//hp->Draw(); fitf->Draw("same"); 
						canv->Update();
						if (first) {
							pdfname.append("(");
							first = false;
						}
						canv->Print(pdfname.c_str(),"pdf");
						//char dummy;
						//cin >> dummy;
					}
					if (chi2ndf < 5 && (fitresult == 0 || fitresult == 4)) {
						fwhm[cnt] = sigma[cnt]*2.35;
						ENC2[cnt] = sigma[cnt]*sigma[cnt]*1.e3*1.e3/eVtoENC/eVtoENC;
						scan[cnt] = sscan[i]*4.e-3; //us
						chi2[cnt] = chi2ndf;
						fitf->GetParameters(&cepars[0]);
						cnt++;
					}
					delete fitf;
				}
				delete hp;
			}
		}
		if (cnt > 0) {
			myfile->cd();
			TGraph* g = new TGraph(cnt,scan,ENC2); //fwhm);
			g->SetMarkerStyle(20);
			char gname[25];
			sprintf(gname,"g%d",ch);
			g->SetName(gname);
			g->Fit(fenc,"QN","",10*4.e-3,1000*4.e-3);
			myfile->cd();
			g->Write();
			hh1->SetBinContent(ch+1,fenc->GetParameter(0));
			hh2->SetBinContent(ch+1,fenc->GetParameter(1));
			hh3->SetBinContent(ch+1,fenc->GetParameter(2));
			double msig = TMath::Sqrt(fenc->GetMinimum(10*4.e-3,1000*4.e-3))*eVtoENC/1.e3;
			if (!TMath::IsNaN(msig))
				hminsig->SetBinContent(ch+1,msig);
		}
	}
	pdfname.append(")");
	canv->Print(pdfname.c_str(),"pdf"); //close out
	delete canv;
	hh1->Write();
	hh2->Write();
	hh3->Write();
	hminsig->Write();
	cout << endl;
	myfile->Close();
}

//fit function for 139Ce betas
double ceb(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double A = par[3];
	double B = par[4];
	double calib0 = par[5];
	xval = xval*calib1+calib0;
	double loE = 100;
	double hiE = 200;
	double value = 0;
	Double_t s2 = TMath::Sqrt(2.);
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 5; //139Ce
	for (int i=0; i<CalibSource::sourcelist[src].betas.size(); i++) {
		double amp = CalibSource::sourcelist[src].betas[i].branch;
		double adc = CalibSource::sourcelist[src].betas[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
		Double_t stepbkgd = (A/2.)*TMath::Erfc((xval-adc)/s2/sigma) + B;
		value += stepbkgd;
	}
	return value*scale;
}
//fit function for 139Ce x-rays
double cex(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double exptail = par[3];
	double expamp = par[4];
	double calib0 = par[5];
	xval = xval*calib1+calib0;
	double loE = 25;
	double hiE = 50;
	double value = 0;
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 5; //139Ce
	for (int i=0; i<CalibSource::sourcelist[src].xrays.size(); i++) {
		double amp = CalibSource::sourcelist[src].xrays[i].branch;
		double adc = CalibSource::sourcelist[src].xrays[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	//to do: include 27 keV beta instead of approximating as exponential background?
	value += expamp*scale*TMath::Exp(-1.*xval/exptail);
	return value*scale;
}

//fit function for 113Sn betas
double snb(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double bkgd = par[3];
	double calib0 = par[4];
	xval = xval*calib1+calib0;
	double loE = 330;
	double hiE = 425;
	double value = 0;
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 3; //113Sn
	value = bkgd;
	for (int i=0; i<CalibSource::sourcelist[src].betas.size(); i++) {
		double amp = CalibSource::sourcelist[src].betas[i].branch;
		double adc = CalibSource::sourcelist[src].betas[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	return value*scale;
}

//fit function for 113Sn x-rays
double snx(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double exptail = par[3];
	double expamp = par[4];
	double calib0 = par[5];
	xval = xval*calib1+calib0;
	double loE = 15;
	double hiE = 50;
	double value = 0;
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 3; //113Sn
	for (int i=0; i<CalibSource::sourcelist[src].xrays.size(); i++) {
		double amp = CalibSource::sourcelist[src].xrays[i].branch;
		double adc = CalibSource::sourcelist[src].xrays[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	value += expamp*scale*TMath::Exp(-1.*xval/exptail);
	return value*scale;
}

//fit function for 207Bi betas
double bib(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double bkgd = par[3];
	double calib0 = par[4];
	xval = xval*calib1+calib0;
	double loE = 450;
	double hiE = 600;
	double value = 0;
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 1; //207Bi
	value = bkgd;
	for (int i=0; i<CalibSource::sourcelist[src].betas.size(); i++) {
		double amp = CalibSource::sourcelist[src].betas[i].branch;
		double adc = CalibSource::sourcelist[src].betas[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	return value*scale;
}

//fit function 
double invx(double* x, double* par) {
	double xval = *x;
	return par[0]/xval + par[1];
}

double enc2(double* x, double* par) {
	double xval = *x;
	return par[0]/xval + par[1] + par[2]*xval;
}
#endif // __ANALYZER_CPP__
