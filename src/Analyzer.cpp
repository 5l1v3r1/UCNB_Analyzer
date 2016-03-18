// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: Analyzer.cpp
// Purpose: Analyze data from NI DAQ for UCNB experiment

#ifndef ANALYZER_CPP__
#define ANALYZER_CPP__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstring>

#include "NIFeb2015BinFile.hh"
#include "NIJune2015BinFile.hh"
#include "RawTreeFile.hh"
#include "TrigTreeFile.hh"
#include "TrapTreeFile.hh"
#include "EventTreeFile.hh"
#include "WaveformAnalyzer.hh"
#include "TriggerList.hh"
#include "SiCalibrator.hh"

#include "TApplication.h"
#include "TRint.h"

void Usage(std::string program);
void DoRaw(int filenum);
void DoTrap(int filenum, int thresh, int decay, int shaping, int top);
void DoFit(int filenum, int thresh);
void DoColl(int filenum, int smp);
void DoAve(int filenum, int thresh);
void DoCalib(int thresh, int decay, int shaping, int top);

std::string path;
int dataformat; // 0 = feb, 1 = june
const int maxformat = 2; // only 2 formats so far


/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
  using std::cout;
  using std::endl;

  cout << "Welcome to UCNB_Analyzer 1.0.0" << endl;

  bool doraw = false, dotrap = false, dofit = false, docoll = false, doave = false, docal = false;
  bool fileok = false;
  int i=1, filenum1, filenum2, fitthresh=-1, trapthresh=-1, decay=-1, shaping=-1, top=-1, smpcoll=-1, avethresh=-1;
  dataformat = 1;

  //-----Parse parameters
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
      if (i+1 > argc) {
	cout << "Missing argument for -ave" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -ave" << endl;
	Usage(argv[0]);
	return 1;
      }
      avethresh = atoi(argv[i]);
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
    else {
      cout << "Unrecognized parameter" << endl;
      Usage(argv[0]);
      return 1;
    }
  }
  
  if (!docal && (!fileok || (!doraw && !dotrap && !dofit && !docoll &&!doave))) {
    if (!fileok)
      cout << "No file indicated" << endl;
    if (!doraw && !dotrap && !dofit && !docoll && !doave)
      cout << "What do you want to do? " << endl;
    Usage(argv[0]);
    return 1;
  }
  if ((dotrap || docal) && (decay == -1 || shaping == -1 || top == -1)) {
	  cout << "Specify trap parameters decay/shaping/top" << endl;
    Usage(argv[0]);
    return 1;
  }
  
  
	TApplication* myapp = 0;
  //-----Process data
  if (fileok) {
	for (int filenum = filenum1; filenum <= filenum2; filenum++) {
		if (doraw)
			DoRaw(filenum);
		if (dotrap)
			DoTrap(filenum, trapthresh, decay, shaping, top);
		if (dofit)
			DoFit(filenum, fitthresh);
		if (docoll)
			DoColl(filenum, smpcoll);
		if (doave)
			DoAve(filenum, avethresh);
	}
  }
  if (docal) {
	  myapp = new TApplication("myapp",0,0);
	  DoCalib(trapthresh, decay, shaping, top);
  }
  cout << "Done." << endl;
  if (myapp != 0)
	myapp->Run();
  return 0; 

}

void Usage(std::string program) {
  cout << "Usage:   " << program  << " -f #1 [#2] -p path" << endl;
  cout << "-raw to convert .bin files to .root" << endl;
  cout << "      -format <format (0=feb,1=june)> to set data format (default 1)" << endl;
  cout << "-sort to time-order .root file" << endl;
  cout << "-trap <threshold> to filter waveforms for events using linear trapezoid" << endl;
  cout << "      -decay <smp> to set linear trap decay constant" << endl;
  cout << "      -shaping <smp> to set linear trap shaping time" << endl;
  cout << "      -top <smp> to set linear trap flat top length" << endl;
  cout << "-fit <threshold> to filter waveforms for events using pulse fitting" << endl;
  cout << "-coll <time in smp (250smp = 1us)> to collect single-event coincidences" << endl;
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
	RootFile.SetTmp();
	if (!RootFile.Create(filenum)) {
		cout << "Input file not open!" << endl;
		return;
	}
	bool goodevent = true;
	while (InputFile[0]->ReadNextEvent(*InputEvent[0]) && goodevent) {
		float ev = InputFile[0]->GetPosition();
		float nentries = InputFile[0]->GetLength();
		printf("Working....%3e/%3e  (%0.1lf %%)\r",ev,nentries,100*ev/nentries);
		for (int rio=1;rio<nfiles;rio++)
			InputFile[rio]->ReadNextEvent(*InputEvent[rio]);
		goodevent = RootFile.FillEvent(InputEvent);
	}
	for (int rio=0;rio<nfiles;rio++) {
		InputFile[rio]->Close();  
		delete InputFile[rio];
		delete InputEvent[rio];  
	}
	//-----Sort by timestamp
	RawTreeFile NewFile;
	NewFile.SetPath(path);
	if (!NewFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
	NewFile.Sort(RootFile);
	NewFile.Write();
	NewFile.Close();
	std::string tmpname = path;
	tmpname.append("/");
	tmpname.append(RootFile.GetName());
	RootFile.Close();
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
	TriggerList TL;
	cout << "Applying trap filter (decay/rise/top = " << decay <<"/" << shaping << "/" << top << ") to file " << filenum << endl;
	int nentries = RootFile.GetNumEvents();
	//-----Find triggers
	for (int ev=0;ev<nentries;ev++) {
		printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
		RootFile.GetEvent(ev);
		WA.MakeTrap(RootFile.NI_event.length, RootFile.NI_event.wave);
		vector<trigger_t> triglist; 
		WA.GetTriggers(thresh,triglist);
		TL.Reset();
		TL.AddTriggers(triglist);
		trigger_t maxtrig;
		maxtrig.TrapE = 0; maxtrig.AveTrapE = 0; maxtrig.MidTrapE = 0; 
		maxtrig.TrapT = 0; 
		maxtrig.up = 0; maxtrig.down = 0; maxtrig.ch = 0;
		maxtrig.Flat0 = 0; maxtrig.Flat1 = 0; 
		if (TL.GetMaxTrigger(maxtrig)) {
			TrapFile.Trap_event.MaxE = maxtrig.TrapE;
			TrapFile.Trap_event.AveE = maxtrig.AveTrapE;
			TrapFile.Trap_event.MidE = maxtrig.MidTrapE;
			TrapFile.Trap_event.t = maxtrig.TrapT + (double)RootFile.NI_event.timestamp;
			TrapFile.Trap_event.up = maxtrig.up;
			TrapFile.Trap_event.down = maxtrig.down;
			TrapFile.Trap_event.Flat0 = maxtrig.Flat0;
			TrapFile.Trap_event.Flat1 = maxtrig.Flat1;
		}
		TrapFile.Trap_event.ch = RootFile.NI_event.ch;
		TrapFile.FillTree();
	}//ev < NumEvents
	RootFile.Close();	
	//-----Sort by timestamp
	TrapTreeFile NewFile;
	NewFile.SetPath(path);
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
	RootFile.SetPath(path);
	if (!RootFile.Open(filenum)) {
		cout << "Input file not open!" << endl;
		return;
	}
	TrigTreeFile TrigFile;
	TrigFile.SetPath(path);
	
	if (!TrigFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
	WaveformAnalyzer WA;
	cout << "IDing triggers in file " << filenum << endl;
	//-----Find and order triggers
	TriggerList TL;
	int nentries = RootFile.GetNumEvents();
	int ev = 0;
	int startev = 0;
	while (ev<nentries) {
		printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
		TL.Reset();
		ULong64_t timediff, prevtime;
		RootFile.GetEvent(ev);
		startev = ev;
		do {
			WA.MakeTrap(RootFile.NI_event.length, RootFile.NI_event.wave);
			vector<trigger_t> triglist; 
			//WA.GetTriggers(thresh,triglist);
			WA.FitWave(thresh,triglist);
			for (int i=0;i<triglist.size();i++)
				triglist[i].ch = RootFile.NI_event.ch;
			TL.AddTriggers(triglist);
			prevtime = RootFile.NI_event.timestamp;
			if (++ev < nentries) {
				RootFile.GetEvent(ev);
			}
			timediff = RootFile.NI_event.timestamp - prevtime;
		}while (timediff < RootFile.NI_event.length  && ev < nentries);
		TL.OrderTriggers(); //within channel
		//-----Write to tree
		RootFile.GetEvent(ev-1);
		trigger_t nexttrig;
		while (TL.GetTrigger(nexttrig)) {
			TrigFile.Trig_event.E = nexttrig.E;
			TrigFile.Trig_event.t = nexttrig.T + (double)RootFile.NI_event.timestamp;
			TrigFile.Trig_event.shaping = nexttrig.Shaping;
			TrigFile.Trig_event.integ = nexttrig.Integration;
			TrigFile.Trig_event.chi2 = nexttrig.Chi2;
			TrigFile.Trig_event.trapE = nexttrig.TrapE;
			TrigFile.Trig_event.trapT = nexttrig.TrapT;
			TrigFile.Trig_event.rio = RootFile.NI_event.board;
			TrigFile.Trig_event.rio_ch = RootFile.NI_event.channel;
			TrigFile.Trig_event.chan = RootFile.NI_event.ch;
			TrigFile.Trig_event.waveev = startev;
			TrigFile.FillTree();
		}
	}//ev < NumEvents
	TrigFile.Write();
	cout << "Done" << endl;
	TrigFile.Close();
	RootFile.Close();	
}

void DoColl(int filenum, int smp) {
  //In development
  cout << "Collecting single-event coincidences in file " << filenum << endl;
  //-----Open input/output files
  TrigTreeFile TrigFile;
  if (!TrigFile.Open(filenum)) {
    cout << "Trig file Not Open!" << endl;
    return;
  }
  EventTreeFile EventFile;
  if (!EventFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
  int nentries = TrigFile.GetNumEvents();
  int StartEv = 0;
  if (nentries == 0){
    EventFile.Write();
    cout << "No triggers: done" << endl;
    EventFile.Close();
    TrigFile.Close();
    return;
  }
  int numch = MAXCH*MAXRIO;
  do {
    //-----Get start trigger of event
    printf("Working....%d/%d  (%d %%)\r",StartEv,nentries,100*StartEv/nentries);
    for (int ch=0;ch<numch;ch++)
      EventFile.myEvent.E[ch] = 0;
    TrigFile.GetEvent(StartEv);
    EventFile.myEvent.t = TrigFile.Trig_event.t;
    EventFile.myEvent.waveev = TrigFile.Trig_event.waveev;
    //-----Get triggers within window
    int ev = StartEv;
    do {
      EventFile.myEvent.E[TrigFile.Trig_event.chan] += TrigFile.Trig_event.E;
      ev++;
      if (ev < nentries)
	TrigFile.GetEvent(ev);
    }while (ev < nentries && (TrigFile.Trig_event.t - EventFile.myEvent.t)<smp);
    EventFile.FillTree();
    StartEv = ev;
  } while (StartEv < nentries);
  EventFile.Write();
  cout << "Done" << endl;
  EventFile.Close();
  TrigFile.Close();
}

void DoAve(int filenum, int thresh) {
  //-----Open input/output files
  RawTreeFile RootFile;
  if (!RootFile.Open(filenum)) {
    cout << "Input file not open!" << endl;
    return;
  }
  TrigTreeFile EvFile;
  if (!EvFile.Open(filenum)) {
    cout << "Event file Not Open!" << endl;
    return;
  }
  int nentries = EvFile.GetNumEvents();
  WaveformAnalyzer WA;
  //-----Build average waveform
  for (int ev=0;ev<nentries;ev++) {
    printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
    EvFile.GetEvent(ev);
    if (EvFile.Trig_event.E>thresh && EvFile.Trig_event.integ < 1.5) {
      RootFile.GetEvent(EvFile.Trig_event.waveev);
      WA.BuildAve(RootFile.NI_event.length, RootFile.NI_event.wave);
    }
  }
  RootFile.Close();
  vector<Double_t> average;
  WA.ReturnAve(average);  
  //-----Write to file
  TFile* newfile = new TFile("Test.root","RECREATE");
  TH1D* h = new TH1D("aveh","aveh",WA.BinAve.size(),0,WA.BinAve.size());
  for (int i=0;i<average.size();i++)
    h->Fill(i,average[i]);
  TF1* f = new TF1("avef","pol1",0,WA.BinAve.size());
  h->Fit(f,"QN","",0,50);
  Int_t maxbin = h->GetMaximumBin();
  cout << "Max value for run " << filenum << " is " << h->GetBinContent(maxbin) - f->Eval(maxbin) << endl;
  newfile->Write();
  newfile->Close();
}

void DoCalib(int thresh, int decay, int shaping, int top) {
	SiCalibrator calib(thresh, decay, shaping, top);
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
	TrapTreeFile trapfile;
	trapfile.SetPath(path);
	//-----To Do:  Check for exising files
	calib.BuildHists(trapfile);
	//calib.FindPeaks();
	calib.MatchPeaks();
}

#endif // __ANALYZER_CPP__
