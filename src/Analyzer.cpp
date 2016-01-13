// File: Analyzer.cpp
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Analyze data from NI DAQ
//
// Revision History:
// 2015/5/6:   LJB  Created 
// 2015/5/11:  LJB  Converts raw files into ROOT files
// 2015/5/14:  LJB  Applies trap trigger
// 2015/7/8:   LJB  Sorts charge-sharing coincidences into events
// 2015/7/15:  LJB  Handle file format versions
// 2015/11/21: LJB  Handle file versions with polymorphism, standardized
//                  ROOT file format
// 2015/1/2:   LJB  Added sort routine

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
#include "EventTreeFile.hh"
#include "WaveformAnalyzer.hh"
#include "TriggerList.hh"
#include "Sorter.hh"

void Usage(std::string program);
void DoRaw(int filenum);
void DoSort(int filenum);
void DoAve(int filenum, int thresh);
void DoTrap(int filenum, int thresh);
void DoColl(int filenum, int smp);

std::string path;
int dataformat; // 0 = feb, 1 = june
const int maxformat = 2; // only 2 formats so far


/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
  using std::cout;
  using std::endl;

  cout << "Welcome to the NI DAQ Analyzer " << endl;

  bool doraw = false, dotrap = false, docoll = false, doave = false, dosort = false;
  bool fileok = false;
  int i=1, filenum1, filenum2, avethresh, thresh, smpcoll;
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
    if (strcmp(argv[i],"-f")==0) {
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
    else if (strcmp(argv[i],"-sort")==0) {
		dosort = true;
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
      thresh = atoi(argv[i]);
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
  if (!fileok || (!doraw && !dotrap && !docoll &&!doave && !dosort)) {
    if (!fileok)
      cout << "No file indicated" << endl;
    if (!doraw && !dotrap && !docoll && !doave && !dosort)
      cout << "What do you want to do? " << endl;
    Usage(argv[0]);
    return 1;
  }

  //-----Process data
  for (int filenum = filenum1; filenum <= filenum2; filenum++) {
	if (dosort)
		DoSort(filenum);
    if (doraw)
		DoRaw(filenum);
    if (doave)
		DoAve(filenum, avethresh);
    if (dotrap)
		DoTrap(filenum, thresh);
    if (docoll)
		DoColl(filenum, smpcoll);
  }

  cout << "Done." << endl;
  return 0; 

}

void Usage(std::string program) {
  cout << "Usage: " << program  << " -f #1 [#2] -p path" << endl;
  cout << "-raw to convert .bin files to .root" << endl;
  cout << "-trap <threshold> to filter waveforms" << endl;
  cout << "-coll <time in smp (250smp = 1us)> to collect single-event coincidences" << endl;
  cout << "-format <format (0=feb,1=june)> to set data format (default 1)" << endl;
}

void DoRaw(int filenum) {
	//-----Open input/output files
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

void DoSort(int filenum) {
	//-----Open input/output files
	RawTreeFile RootFile;
	RootFile.SetPath(path);
	if (!RootFile.Open(filenum)) {
		cout << "Input file not open!" << endl;
		return;
	}
	std::string newpath = path;
	newpath.append("/Fixed/");
	RawTreeFile NewFile;
	NewFile.SetPath(newpath);
	NewFile.Create(filenum);
	Sorter sortedlist;
	sortedlist.Reset();
	//-----Add to event_time-ordered list
	int nentries = RootFile.GetNumEvents();
	for (int ev=0;ev<nentries;ev++) {
		printf("Working....%d/%d  (%d %%)\r", ev,nentries, 100*ev/nentries);
		RootFile.GetEvent(ev);
		sortedlist.InsertEv(RootFile.NI_event.timestamp, ev);
	}
	//-----Write to tree
	int ev = sortedlist.GetNextEvent(true);
	int cnt = 0;
	while (ev != -1) {
		printf("Writing....%d/%d  (%d %%)\r", cnt,nentries, 100*ev/nentries);
		RootFile.GetEvent(ev);
		NewFile.FillRawEvent(RootFile.NI_event);
		ev = sortedlist.GetNextEvent();
		cnt++;
	}
	NewFile.Write();
	NewFile.Close();
	RootFile.Close();
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

void DoTrap(int filenum, int thresh) {
	//-----Open input/output files
	RawTreeFile RootFile;
	RootFile.SetPath(path);
	if (!RootFile.Open(filenum)) {
		cout << "Input file not open!" << endl;
		return;
	}
	TrigTreeFile TrigFile;
	TrigFile.Create(filenum);
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
			if (RootFile.NI_event.result == 1)
				TrigFile.FillTree();
		}
	}//ev < NumEvents
	TrigFile.Write();
	cout << "Done" << endl;
	TrigFile.Close();
	RootFile.Close();	
}

void DoColl(int filenum, int smp) {
  //Not yet working
  cout << "Collecting single-event coincidences in file " << filenum << endl;
  //-----Open input/output files
  TrigTreeFile TrigFile;
  if (!TrigFile.Open(filenum)) {
    cout << "Trig file Not Open!" << endl;
    return;
  }
  EventTreeFile EventFile;
  EventFile.Create(filenum);
  int numch = MAXCH*NRIO;
  EventFile.SetupTree(numch);
  int nentries = TrigFile.GetNumEvents();
  int StartEv = 0;
  if (nentries == 0){
    EventFile.Write();
    cout << "No triggers: done" << endl;
    EventFile.Close();
    TrigFile.Close();
    return;
  }
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


#endif // __ANALYZER_CPP__
