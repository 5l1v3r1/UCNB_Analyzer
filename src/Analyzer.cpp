// File: Analyzer.cpp
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Analyze data from NI DAQ
//
// Revision History:
// 2015/5/6:  LJB  Created 
// 2015/5/11: LJB  Converts raw files into ROOT files
// 2015/5/14: LJB  Applies trap trigger
// 2015/7/8:  LJB  Sorts charge-sharing coincidences into events

#ifndef ANALYZER_CPP__
#define ANALYZER_CPP__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstring>

#include "RawFile.hh"
#include "ROOTTreeFile.hh"
#include "TrigTreeFile.hh"
#include "EventTreeFile.hh"
#include "WaveformAnalyzer.hh"
#include "TriggerList.hh"
//#include "TInterpreter.h"
//#pragma link C++ class vector<Short_t>+;

void Usage(std::string program);
void DoRaw(int filenum);
void DoTrap(int filenum, int thresh);
void DoColl(int filenum, int smp);


/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
  using std::cout;
  using std::endl;
  
  cout << "Welcome to the NI DAQ Analyzer " << endl;

  bool doraw = false, dotrap = false, docoll = false, fileok = false;
  int i=1, filenum1, filenum2, thresh, smpcoll;

  //Parse parameters
  while (i+1 <= argc) {
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
    else {
      cout << "Unrecognized parameter" << endl;
      Usage(argv[0]);
      return 1;
    }
  }

  if (!fileok || (!doraw && !dotrap && !docoll)) {
    if (!fileok)
      cout << "No file indicated" << endl;
    if (!doraw && !dotrap && !docoll)
      cout << "What do you want to do? " << endl;
    Usage(argv[0]);
    return 1;
  }
  for (int filenum = filenum1; filenum <= filenum2; filenum++) {
    if (doraw)
      DoRaw(filenum);
    if (dotrap)
      DoTrap(filenum, thresh);
    if (docoll)
      DoColl(filenum, smpcoll);
  }

  cout << "Done." << endl;
  return 0; 

}

void Usage(std::string program) {
  cout << "Usage: " << program  << " -f #1 [#2]" << endl;
  cout << "-raw to convert .bin files to .root" << endl;
  cout << "-trap <threshold> to filter waveforms" << endl;
  cout << "-coll <time in smp (250smp = 1us)> to collect single-event coincidences" << endl;
}

void DoRaw(int filenum) {
  //-----Open input/output files
  RawFile InputFile(filenum);
  if (!InputFile.IsOpen()) {
    cout << "File Not Open!" << endl;
    return;
  }
  cout << "Processing raw file " << filenum << endl;
  ROOTTreeFile RootFile;
  RootFile.Create(filenum);
  RootFile.SetupTree(InputFile.GetNumCh());
  //-----Read events
  while (InputFile.ReadEvent(RootFile.NI_event)) {
    RootFile.FillTree();
  }
  RootFile.Write();
  RootFile.Close();
  InputFile.Close();
}

void DoTrap(int filenum, int thresh) {
  //-----Open input/output files
  ROOTTreeFile RootFile;
  if (!RootFile.Open(filenum)) {
    cout << "File Not Open!" << endl;
    return;
  }
  TrigTreeFile TrigFile;
  TrigFile.Create(filenum);
  WaveformAnalyzer WA;
  cout << "IDing triggers in file " << filenum << endl;
  //-----Find and order triggers
  TriggerList TL;
  int nentries = RootFile.GetNumEvents();
  for (int ev=0;ev<nentries;ev++) {
    printf("Working....%d/%d  (%d \%)\r",ev,nentries,100*ev/nentries);
    RootFile.GetEvent(ev);
    TL.Reset();
    for (int ch=0;ch<RootFile.NI_event.numch;ch++) {
      WA.MakeTrap(RootFile.NI_event.ch[ch].wavelen, RootFile.NI_event.ch[ch].wave);
      vector<trigger_t> triglist;
      WA.GetTriggers(thresh,triglist);
      for (int i=0;i<triglist.size();i++)
	triglist[i].ch = ch;
      TL.AddTriggers(triglist);
    }
    TL.OrderTriggers();
    //-----Write to tree
    trigger_t nexttrig;
    while (TL.GetTrigger(nexttrig)) {
      TrigFile.Trig_event.E = nexttrig.E;
      TrigFile.Trig_event.t = nexttrig.T + (double)RootFile.NI_event.timestamp;
      TrigFile.Trig_event.rio = RootFile.NI_event.ch[nexttrig.ch].rio;
      TrigFile.Trig_event.rio_ch = RootFile.NI_event.ch[nexttrig.ch].ch;
      TrigFile.Trig_event.chan = nexttrig.ch;
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
  ROOTTreeFile RootFile;
  if (!RootFile.Open(filenum)) {
    cout << "Raw ROOT file Not Open!" << endl;
    return;
  }
  RootFile.GetEvent(0);
  int numch = RootFile.NI_event.numch; //Store in trig file?
  RootFile.Close();
  TrigTreeFile TrigFile;
  if (!TrigFile.Open(filenum)) {
    cout << "Trig file Not Open!" << endl;
    return;
  }
  EventTreeFile EventFile;
  EventFile.Create(filenum);
  EventFile.SetupTree(numch);
  int nentries = TrigFile.GetNumEvents();
  int StartEv = 0;
  do {
    //-----Get start trigger of event
    printf("Working....%d/%d  (%d \%)\r",StartEv,nentries,100*StartEv/nentries);
    for (int ch=0;ch<numch;ch++)
      EventFile.myEvent.E[ch] = 0;
    TrigFile.GetEvent(StartEv);
    EventFile.myEvent.t = TrigFile.Trig_event.t;
    //-----Get triggers within window
    int ev = StartEv;
    do {
      EventFile.myEvent.E[TrigFile.Trig_event.chan] = TrigFile.Trig_event.E;
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
