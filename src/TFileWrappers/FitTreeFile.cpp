// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: FitTreeFile.cpp
// Purpose: Handles ROOT TTree file with E,t
 

#ifndef FIT_TREE_FILE_CPP__
#define FIT_TREE_FILE_CPP__

#include "FitTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
FitTreeFile::FitTreeFile() {
  SetNameStr();
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
FitTreeFile::~FitTreeFile() {
}

/*************************************************************************/
//                             SetBranches 
/*************************************************************************/
void FitTreeFile::SetBranches() {
  RootTree->SetBranchAddress("E",&Fit_event.E);
  RootTree->SetBranchAddress("t",&Fit_event.t);
  RootTree->SetBranchAddress("shaping",&Fit_event.shaping);
  RootTree->SetBranchAddress("integ",&Fit_event.integ);
  RootTree->SetBranchAddress("chi2",&Fit_event.chi2);
  RootTree->SetBranchAddress("ch",&Fit_event.ch);
  RootTree->SetBranchAddress("wavefile",&Fit_event.wavefile);
  RootTree->SetBranchAddress("waveev",&Fit_event.waveev);

  RootTree->GetEntry(0);
}


#if !defined (__CINT__)

/*************************************************************************/
//                             MakeBranches 
/*************************************************************************/
void FitTreeFile::MakeBranches() {
  RootTree->Branch("E",&Fit_event.E,"E/D");
  RootTree->Branch("t",&Fit_event.t,"t/D");
  RootTree->Branch("shaping",&Fit_event.shaping,"shaping/D");
  RootTree->Branch("integ",&Fit_event.integ,"integ/D");
  RootTree->Branch("chi2",&Fit_event.chi2,"chi2/D");
  RootTree->Branch("ch",&Fit_event.ch,"ch/I");
  RootTree->Branch("wavefile",&Fit_event.wavefile,"wavefile/I");
  RootTree->Branch("waveev",&Fit_event.waveev,"waveev/I");
}

#endif // !defined (__CINT__)

#endif // FIT_TREE_FILE_CPP__

