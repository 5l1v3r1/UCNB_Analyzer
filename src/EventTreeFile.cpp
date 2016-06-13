// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: EventTreeFile.cpp
// Purpose: Handles ROOT TTree file with events E[numch],t
 

#ifndef EVENT_TREE_FILE_CPP__
#define EVENT_TREE_FILE_CPP__
#include "EventTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
EventTreeFile::EventTreeFile() {
  SetNameStr();
  for (int i=0;i<MAXCH*MAXRIO;i++)
    myEvent.E[i] = 0;
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
EventTreeFile::~EventTreeFile() {
}

/*************************************************************************/
//                             SetBranches 
/*************************************************************************/
void EventTreeFile::SetBranches() {
  RootTree->SetBranchAddress("E",&myEvent.E[0]);
  RootTree->SetBranchAddress("t",&myEvent.t);
  RootTree->SetBranchAddress("Esum",&myEvent.Esum);
  RootTree->SetBranchAddress("Eprev",&myEvent.Eprev);
  RootTree->SetBranchAddress("tprev",&myEvent.tprev);
  RootTree->SetBranchAddress("numch",&myEvent.numch);
  RootTree->SetBranchAddress("run",&myEvent.run);
  RootTree->SetBranchAddress("wavefile",&myEvent.wavefile);
  RootTree->SetBranchAddress("waveev",&myEvent.waveev);
  
  RootTree->GetEntry(0);
}

#if !defined (__CINT__)
/*************************************************************************/
//                             MakeBranches 
/*************************************************************************/
void EventTreeFile::MakeBranches() {
  TString Estr = "E[";
  Estr += MAXCH*MAXRIO;
  Estr += "]/D";
  RootTree->Branch("E",&myEvent.E[0],Estr);
  RootTree->Branch("t",&myEvent.t,"t/D");
  RootTree->Branch("Esum",&myEvent.Esum,"Esum/D");
  RootTree->Branch("Eprev",&myEvent.Eprev,"Eprev/D");
  RootTree->Branch("tprev",&myEvent.tprev,"tprev/D");
  RootTree->Branch("numch",&myEvent.numch,"numch/I");
  RootTree->Branch("run",&myEvent.run,"run/I");
  RootTree->Branch("wavefile",&myEvent.wavefile,"wavefile/I");
  RootTree->Branch("waveev",&myEvent.waveev,"waveev/I");

}


#endif // !defined (__CINT__)
#endif // EVENT_TREE_FILE_CPP__

