// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: TrigTreeFile.cpp
// Purpose: Handles ROOT TTree file with E,t from .trig file
 

#ifndef TRIG_TREE_FILE_CPP__
#define TRIG_TREE_FILE_CPP__

#include "TrigTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TrigTreeFile::TrigTreeFile() {
  SetNameStr();
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
TrigTreeFile::~TrigTreeFile() {
}

/*************************************************************************/
//                             SetBranches 
/*************************************************************************/
void TrigTreeFile::SetBranches() {
  RootTree->SetBranchAddress("E",&Trig_event.E);
  RootTree->SetBranchAddress("t",&Trig_event.t);
  RootTree->SetBranchAddress("ch",&Trig_event.ch);

  RootTree->GetEntry(0);
}


#if !defined (__CINT__)

/*************************************************************************/
//                             MakeBranches 
/*************************************************************************/
void TrigTreeFile::MakeBranches() {
  RootTree->Branch("E",&Trig_event.E,"E/D");
  RootTree->Branch("t",&Trig_event.t,"t/D");
  RootTree->Branch("ch",&Trig_event.ch,"ch/I");
}

/*************************************************************************/
//                                 Sort
/*************************************************************************/

void TrigTreeFile::Sort(TreeFile& origfile){
	Sort(origfile,"t");
}


#endif // !defined (__CINT__)

#endif // TRIG_TREE_FILE_CPP__

