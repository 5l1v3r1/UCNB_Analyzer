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
// 2015/7/15: LJB  Handle file format versions

#ifndef ANALYZER_CPP__
#define ANALYZER_CPP__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstring>

#include "NIFeb2015BinFile.hh"
#include "NIJune2015BinFile.hh"

#include "RawFileFeb.hh"
#include "RawFileJune.hh"
#include "ROOTTreeFileFeb.hh"
#include "ROOTTreeFileJune.hh"
#include "TrigTreeFile.hh"
#include "EventTreeFile.hh"
#include "WaveformAnalyzer.hh"
#include "TriggerList.hh"
//#include "TInterpreter.h"
//#pragma link C++ class vector<Short_t>+;

void Usage(std::string program);
void DoRaw(int filenum);
void DoAve(int filenum, int thresh);
void DoTrap(int filenum, int thresh);
void DoTrapJune(int filenum, int thresh);
void DoColl(int filenum, int smp);

int dataformat; // 0 = feb, 1 = june
const int maxformat = 2; // only 2 formats so far


/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
  using std::cout;
  using std::endl;

  cout << "Welcome to the NI DAQ Analyzer " << endl;

  bool doraw = false, dotrap = false, docoll = false, doave = false;
  bool fileok = false;
  int i=1, filenum1, filenum2, avethresh, thresh, smpcoll;
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
    else if (strcmp(argv[i],"-raw")==0) {
      doraw = true;
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

  if (!fileok || (!doraw && !dotrap && !docoll &&!doave)) {
    if (!fileok)
      cout << "No file indicated" << endl;
    if (!doraw && !dotrap && !docoll && !doave)
      cout << "What do you want to do? " << endl;
    Usage(argv[0]);
    return 1;
  }
  for (int filenum = filenum1; filenum <= filenum2; filenum++) {
    if (doraw)
      DoRaw(filenum);
    if (doave)
      DoAve(filenum, avethresh);
    if (dotrap)
      if (dataformat == 0)
	DoTrap(filenum, thresh);
      else if (dataformat == 1)
	DoTrapJune(filenum, thresh);
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
  cout << "-format <format (0=feb,1=june)> to set data format (default 1)" << endl;
}

void DoRaw(int filenum) {
  //-----Open input/output files
  NIJune2015BinFile* InputFile = 0;// giving up on polymorphism for now
  /*
  if (dataformat == 0) { 
    InputFile = new NIFeb2015BinFile(); //can I avoid dynamic allocation?
    cout << "I don't work yet, don't use me." << endl;
    delete InputFile;
    return;
  }
  else if (dataformat == 1) {
  */
  std::string path = "/media/leah/UCNS1/UCNB/JulyData/LinearityStudies/";
  //std::string path = "/media/leah/UCNS1/UCNB/JuneData/";
  char tempstr[255];
  sprintf(tempstr,"Run_%04d.bin",filenum);
  //sprintf(tempstr,"Run_%04d.bin",filenum);
  std::string name = tempstr;
  InputFile = new NIJune2015BinFile(path,name);
    //  }
  if (!InputFile->IsOpen()) {
    cout << "File Not Open!" << endl;
    return;
  }
  cout << "Processing raw file " << filenum << endl;
  ROOTTreeFileJune RootFile;
  RootFile.Create(filenum);
  NIJune2015BinFile::JuneBinEv_t event;  //Need a polymorphic way to do this
  while (InputFile->ReadNextEvent(event)) {
    float ev = InputFile->GetPosition();
    float nentries = InputFile->GetLength();
    printf("Working....%3e/%3e  (%0.1lf \%)\r",ev,nentries,100*ev/nentries);
    RootFile.FillEvent(event);
    RootFile.FillTree();    
  }
    RootFile.Write();
    RootFile.Close();
    InputFile->Close();

  if (InputFile != 0) delete InputFile;
  
  /*
  if (dataformat == 0) {//replace with inheritance!
    //-----Open input/output files
    RawFileFeb InputFile(filenum);
    if (!InputFile.IsOpen()) {
      cout << "File Not Open!" << endl;
      return;
    }
    cout << "Processing raw file " << filenum << endl;
    ROOTTreeFileFeb RootFile;
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
  else if (dataformat == 1) {
    RawFileJune InputFile(filenum);
    if (!InputFile.IsOpen()) {
      cout << "File Not Open!" << endl;
      return;
    }
    cout << "Processing raw file " << filenum << endl;
    ROOTTreeFileJune RootFile;
    RootFile.Create(filenum);
    //-----Read events
    while (InputFile.ReadEvent(RootFile.NI_event)) {
      RootFile.FillTree();
    }
    RootFile.Write();
    RootFile.Close();
    InputFile.Close();
  }
  */
}

void DoAve(int filenum, int thresh) {
  //-----Open input/output files
  ROOTTreeFileJune RootFile;
  if (!RootFile.Open(filenum)) {
    cout << "File Not Open!" << endl;
    return;
  }
  TrigTreeFile EvFile;
  if (!EvFile.Open(filenum)) {
    cout << "Event file Not Open!" << endl;
    return;
  }
  int nentries = EvFile.GetNumEvents();
  WaveformAnalyzer WA;
  for (int ev=0;ev<nentries;ev++) {
    printf("Working....%d/%d  (%d \%)\r",ev,nentries,100*ev/nentries);
    EvFile.GetEvent(ev);
    if (EvFile.Trig_event.E>thresh && EvFile.Trig_event.integ < 1.5) {
      RootFile.GetEvent(EvFile.Trig_event.waveev);
      WA.BuildAve(RootFile.NI_event.length, RootFile.NI_event.wave);
    }
  }
  RootFile.Close();
  vector<Double_t> average;
  WA.ReturnAve(average);
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

void DoTrapJune(int filenum, int thresh) {
  //-----Open input/output files
  ROOTTreeFileJune RootFile;
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
  //  for (int ev=0;ev<nentries;ev++) {
  int ev = 0;
  int startev = 0;
  while (ev<nentries) {
    printf("Working....%d/%d  (%d \%)\r",ev,nentries,100*ev/nentries);
    TL.Reset();
    ULong64_t timediff, prevtime;
    RootFile.GetEvent(ev);
    startev = ev;
    do {
      //      cout << "ev " << ev << "                           " << endl;
      WA.MakeTrap(RootFile.NI_event.length, RootFile.NI_event.wave);
      vector<trigger_t> triglist; 
      if (RootFile.NI_event.ch == 16)
      {
	WA.GetTriggers(thresh,triglist);
	for (int i=0;i<triglist.size();i++) {
	  triglist[i].TrapE = triglist[i].E;
	  triglist[i].TrapT = triglist[i].T;
	  triglist[i].E = 0;
	  triglist[i].T = 0;
	}
      }
      else 
	WA.FitWave(thresh,triglist);
      //      cout << "fitted" << endl;
      for (int i=0;i<triglist.size();i++)
	triglist[i].ch = RootFile.NI_event.ch;
      TL.AddTriggers(triglist);
      //      cout << "added " << triglist.size() << "          " << endl;
      prevtime = RootFile.NI_event.timestamp;
      if (++ev < nentries) {
	RootFile.GetEvent(ev);
      }
      timediff = RootFile.NI_event.timestamp - prevtime;
    }while (timediff < RootFile.NI_event.length  && ev < nentries);
    //    cout << "here?" << endl;
    TL.OrderTriggers();
    //    cout << "ordered" << endl;
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
    //    cout << "done " << endl;
  }//ev < NumEvents
  TrigFile.Write();
  cout << "Done" << endl;
  TrigFile.Close();
  RootFile.Close();	
}

void DoTrap(int filenum, int thresh) {
  //-----Open input/output files
  ROOTTreeFileFeb RootFile;
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
  /*
  ROOTTreeFileJune RootFile;
  if (!RootFile.Open(filenum)) {
    cout << "Raw ROOT file Not Open!" << endl;
    return;
  }
  RootFile.GetEvent(0);
  int numch = RootFile.NI_event.numch; //Store in trig file?
  RootFile.Close();
  */
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
    printf("Working....%d/%d  (%d \%)\r",StartEv,nentries,100*StartEv/nentries);
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
