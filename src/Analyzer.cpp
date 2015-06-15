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

  bool doraw = false, dotrap = false, fileok = false;;
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
  trigs_t trig[RootFile.NI_event.numch];
  int nentries = RootFile.GetNumEvents();
  int debugev = -1;
  //for (int ev=0;ev<nentries;ev++) {
  for (int ev=0;ev<nentries;ev++) {
    printf("Working....%d/%d  (%d \%)\r",ev,nentries,100*ev/nentries);
    RootFile.GetEvent(ev);
    for (int ch=0;ch<RootFile.NI_event.numch;ch++) {
      WA.MakeTrap(RootFile.NI_event.ch[ch].wavelen, RootFile.NI_event.ch[ch].wave);
      WA.GetTriggers(thresh,trig[ch].E,trig[ch].T);
    }
    int tottrig = 0;
    for (int ch=0;ch<RootFile.NI_event.numch;ch++)
      tottrig += trig[ch].E.size();
    if (ev == debugev) cout << endl << "tottrig is " << tottrig << endl;
    vector<triglist_t> triglist;
    triglist.resize(tottrig);
    int cnt = 0;
    //order the triggers    
    if (ev == debugev) cout << "ordering" << endl;
    if (tottrig > 0) {
      for (int ch=0;ch<RootFile.NI_event.numch;ch++){
	for (int j=0;j<trig[ch].E.size();j++) {
	  triglist[cnt].ch = ch;
	  triglist[cnt].myE = &trig[ch].E[j];
	  triglist[cnt].myT = &trig[ch].T[j];
	  if (ev == debugev) cout << "Found " << *triglist[cnt].myT << " from ch " << ch << endl;
	  triglist[cnt].prev = NULL;
	  triglist[cnt].next = NULL;
	  if (cnt > 0) {
	    triglist_t* check = &triglist[0];
	    if (ev == debugev) cout << "compare " << *triglist[cnt].myT << " and " << *check->myT << endl;
	    if (*triglist[cnt].myT > *check->myT) {
	      while (*triglist[cnt].myT > *check->myT && check->next != NULL) {
		if (ev == debugev) cout << *triglist[cnt].myT << " > " << *check->myT << ", moving forward" << endl;
		check = check->next;
	      }
	      if (*triglist[cnt].myT <= *check->myT) {
		triglist[cnt].next = check;
		triglist[cnt].prev = check->prev;
		check->prev = &triglist[cnt];
		if (triglist[cnt].prev != NULL)
		  triglist[cnt].prev->next = &triglist[cnt];
		if (ev == debugev) cout << "placing before " << *check->myT << endl;
	      }
	      else{
		triglist[cnt].prev = check;
		triglist[cnt].next = check->next;
		check->next = &triglist[cnt];
		if (triglist[cnt].next != NULL)
		  triglist[cnt].next->prev = &triglist[cnt];
		if (ev == debugev) cout << "placing after " << *check->myT << endl;
	      }
	    }
	    else {
	      while (*triglist[cnt].myT < *check->myT && check->prev != NULL) {
		if (ev == debugev) cout << *triglist[cnt].myT << " < " << *check->myT << ", moving backward" << endl;
		check = check->prev;
	      }
	      if (*triglist[cnt].myT >= *check->myT) {
		triglist[cnt].prev = check;
		triglist[cnt].next = check->next;
		check->next = &triglist[cnt];
		if (triglist[cnt].next != NULL)
		  triglist[cnt].next->prev = &triglist[cnt];
		if (ev == debugev) cout << "placing after " << *check->myT << endl;
	      }
	      else {
		triglist[cnt].next = check;
		triglist[cnt].prev = check->prev;
		check->prev = &triglist[cnt];
		if (triglist[cnt].prev != NULL)
		  triglist[cnt].prev->next = &triglist[cnt];
		if (ev == debugev) cout << "placing before " << *check->myT << endl;
	      }
	    }// if !(*triglist[cnt].myT > *check->myT)
	    if (ev == debugev) {
	      cout << "Current status at cnt " << *check->myT << endl;
	      for (int c=0;c<=cnt;c++) {
		cout << "this " << &triglist[c] << endl;
		cout << "prev " << triglist[c].prev << endl;
		cout << "next " << triglist[c].next << endl;
	      }
	      cout << endl;
	    }
	  }// if (cnt > 0)
	  cnt++;
	}//j < size
      }//ch < numch
      triglist_t* check = &triglist[0];
      while (check->prev != NULL)
	check = check->prev;
      double checkt = *check->myT;
      do{
	if (ev == debugev) cout << "T=" << RootFile.NI_event.timestamp << endl;
	if (ev == debugev) cout <<  "time " << *check->myT << endl;
	if (ev == debugev) cout <<  "energy " << *check->myE << endl;
	TrigFile.Trig_event.E = *check->myE;
	TrigFile.Trig_event.t = *check->myT + (double)RootFile.NI_event.timestamp;
	if (TrigFile.Trig_event.t < checkt ) {
	  cout << "Problem, " << TrigFile.Trig_event.t << " after " << checkt <<", event " << ev << endl;
	  cout << "ch " << check->ch << endl;
	}
	int checkt = TrigFile.Trig_event.t;
	TrigFile.Trig_event.chan = check->ch;
	TrigFile.Trig_event.rio = RootFile.NI_event.ch[check->ch].rio;
	TrigFile.Trig_event.rio_ch = RootFile.NI_event.ch[check->ch].ch;
	//cout << "Ordered " << TrigFile.Trig_event.t << " from ch " << TrigFile.Trig_event.chan << endl;
	TrigFile.FillTree();
      }while ((check = check->next)!=NULL);
    }//tottrig > 0
  }//ev < NumEvents
  TrigFile.Write();
}

#endif // __ANALYZER_CPP__
