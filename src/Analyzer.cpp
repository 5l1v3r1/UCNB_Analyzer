// File: Analyzer.cpp
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Analyze data from NI DAQ
//
// Revision History:
// 2015/5/6:  LJB  Created 
// 2015/5/11: LJB  Converts raw files into ROOT files
// 2015/5/14: LJB  Applies trap trigger

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
#include "WaveformAnalyzer.hh"
#include "TriggerList.hh"
//#include "TInterpreter.h"
//#pragma link C++ class vector<Short_t>+;


struct trigs_t{
  vector<Double_t> E;
  vector<Double_t> T;
};
struct triglist_t{
  triglist_t* prev;
  triglist_t* next;  
  Double_t *myE;
  Double_t *myT;
  Int_t ch;
};

void Usage(std::string program);
void DoRaw(int filenum);
void DoTrap(int filenum, int thresh);


/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
  using std::cout;
  using std::endl;
  
  cout << "Welcome to the NI DAQ Analyzer " << endl;

  bool doraw = false, dotrap = false, fileok = false;
  int i=1, filenum1, filenum2, thresh;

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
    else {
      cout << "Unrecognized parameter" << endl;
      Usage(argv[0]);
      return 1;
    }
  }

  if (!fileok || (!doraw && !dotrap)) {
    if (!fileok)
      cout << "No file indicated" << endl;
    if (!doraw && !dotrap)
      cout << "What do you want to do? " << endl;
    Usage(argv[0]);
    return 1;
  }
  for (int filenum = filenum1; filenum <= filenum2; filenum++) {
    if (doraw)
      DoRaw(filenum);
    if (dotrap)
      DoTrap(filenum, thresh);
  }

  cout << "Done." << endl;
  return 0; 

}

void Usage(std::string program) {
  cout << "Usage: " << program  << " -f #1 [#2]" << endl;
  cout << "-raw to convert .bin files to .root" << endl;
  cout << "-trap <threshold> to filter waveforms" << endl;
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
    //for (int ev=0;ev<10;ev++) {
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
}

#endif // __ANALYZER_CPP__
