// File: EventTreeFile.cpp
// Name: Leah Broussard
// Date: 2015/6/29
// Purpose: Handles ROOT TTree file with events E[numch],t
//
// Revision History:
// 2015/6/29:  LJB  Created
// 2015/7/8:   LJB  TTree fix: added SetupTree method
// 2016/1/13:  LJB  Move base functions to TreeFile
 

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
//  RootTree->SetBranchAddress("numch",&myEvent.numch);
  RootTree->SetBranchAddress("E",&myEvent.E[0]);
  RootTree->SetBranchAddress("t",&myEvent.t);
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
//  RootTree->Branch("numch",&myEvent.numch,"numch/I");
  RootTree->Branch("E",&myEvent.E[0],Estr);
  RootTree->Branch("t",&myEvent.t,"t/D");
  RootTree->Branch("waveev",&myEvent.waveev,"waveev/I");

}


#endif // !defined (__CINT__)
#endif // EVENT_TREE_FILE_CPP__

