// File: DoTrapFilter.cpp
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Analyze data from NI DAQ
//
// Revision History:
// 2015/5/6:  LJB  Created 
// 2015/5/14: LJB  Applies trap trigger

#ifndef TRAP_FILTER_CPP__
#define TRAP_FILTER_CPP__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstring>
//#include <string>

//#include "ROOTTreeFileFeb.hh"
#include "ROOTTreeFileJune.hh"
#include "TrapTreeFile.hh"
#include "TriggerList.hh"
#include "WaveformAnalyzer.hh"

void Usage(std::string program);
void DoTrap(int filenum, int thresh, int decay, int shaping, int top, std::string path);

int dataformat; // 0 = feb, 1 = june
const int maxformat = 2; // only 2 formats so far


/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
  using std::cout;
  using std::endl;

  cout << "Welcome to the NI DAQ Trap Filter Analyzer " << endl;

  bool fileok = false, dotrap = false;
  std::string path = "";
  int i=1, filenum1, filenum2;
  int thresh = 50;
  int decay1 = 250, decay2 = 250;
  int shaping1 = 250, shaping2 = 250;
  int top1 = 400, top2 = 400;
  dataformat = 1;
  
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
    else if (strcmp(argv[i],"-path")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -path" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (argv[i][0] == '-') {
	cout << "Missing argument for -path" << endl;
	Usage(argv[0]);
	return 1;
      }
      path = argv[i];
      i++;
    }
    else if (strcmp(argv[i],"-thresh")==0) {
      dotrap = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -thresh" << endl;
	Usage(argv[0]);
	return 1;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -thresh" << endl;
	Usage(argv[0]);
	return 1;
      }
      thresh = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-decay")==0) {
      dotrap = true;
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
      decay1 = atoi(argv[i]);
      decay2 = decay1;
      i++;
      if (i+1 <= argc) {
	if (argv[i][0] >= '0' && argv[i][0] <= '9') {
	  decay2 = atoi(argv[i]);
	  i++;
	}
      }
    }
    else if (strcmp(argv[i],"-shaping")==0) {
      dotrap = true;
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
      shaping1 = atoi(argv[i]);
      shaping2 = shaping1;
      i++;
      if (i+1 <= argc) {
	if (argv[i][0] >= '0' && argv[i][0] <= '9') {
	  shaping2 = atoi(argv[i]);
	  i++;
	}
      }
    }
    else if (strcmp(argv[i],"-top")==0) {
      dotrap = true;
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
      top1 = atoi(argv[i]);
      top2 = top1;
      i++;
      if (i+1 <= argc) {
	if (argv[i][0] >= '0' && argv[i][0] <= '9') {
	  top2 = atoi(argv[i]);
	  i++;
	}
      }
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
  for (int filenum = filenum1; filenum <= filenum2; filenum++) 
    for (int decay = decay1; decay <= decay2; decay++)
      for (int shaping = shaping1; shaping <= shaping2; shaping++)
	for (int top = top1; top <= top2; top++)
	  DoTrap(filenum, thresh, decay, shaping, top, path);

  cout << "Done." << endl;
  return 0; 

}

void Usage(std::string program) {
  cout << "Usage: " << program  << " -f #1 [#2]" << endl;
  cout << "-path <file location string>" << endl;
  cout << "-thresh <threshold in smp> " << endl;
  cout << "-decay <decay time in smp> " << endl;
  cout << "-shaping <shaping time in smp> " << endl;
  cout << "-top <trap flat top length in smp> " << endl;
  cout << "-format <format (0=feb,1=june)> to set data format (default 1)" << endl;
}

void DoTrap(int filenum, int thresh, int decay, int shaping, int top, std::string path) {
  //-----Open input/output files
  ROOTTreeFileJune RootFile;
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
  TrapFile.Create(filenum,decay,shaping,top);
  WaveformAnalyzer WA;
  WA.SetTrapPars(decay,shaping,top);
  TriggerList TL;
  cout << "Applying trap filter (decay/rise/top = " << decay <<"/" << shaping << "/" << top << ") to file " << filenum << endl;
  int nentries = RootFile.GetNumEvents();
  for (int ev=0;ev<nentries;ev++) {
    printf("Working....%d/%d  (%d \%)\r",ev,nentries,100*ev/nentries);
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
      TrapFile.Trap_event.t = maxtrig.TrapT;
      TrapFile.Trap_event.up = maxtrig.up;
      TrapFile.Trap_event.down = maxtrig.down;
      TrapFile.Trap_event.Flat0 = maxtrig.Flat0;
      TrapFile.Trap_event.Flat1 = maxtrig.Flat1;
    }
    TrapFile.Trap_event.ch = RootFile.NI_event.ch;
    TrapFile.FillTree();
  }//ev < NumEvents
  TrapFile.Write();
  cout << "Done" << endl;
  TrapFile.Close();
  RootFile.Close();	
}


#endif // __TRAP_FILTER_CPP__
