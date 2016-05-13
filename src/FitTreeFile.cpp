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
  RootTree->SetBranchAddress("trapE",&Fit_event.trapE);
  RootTree->SetBranchAddress("trapT",&Fit_event.trapT);
  RootTree->SetBranchAddress("chi2",&Fit_event.chi2);
  RootTree->SetBranchAddress("rio",&Fit_event.rio);
  RootTree->SetBranchAddress("rio_ch",&Fit_event.rio_ch);
  RootTree->SetBranchAddress("chan",&Fit_event.chan);
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
  RootTree->Branch("trapE",&Fit_event.trapE,"trapE/D");
  RootTree->Branch("trapT",&Fit_event.trapT,"trapT/D");
  RootTree->Branch("rio",&Fit_event.rio,"rio/I");
  RootTree->Branch("rio_ch",&Fit_event.rio_ch,"rio_ch/I");
  RootTree->Branch("chan",&Fit_event.chan,"chan/I");
  RootTree->Branch("waveev",&Fit_event.waveev,"waveev/I");
}

#endif // !defined (__CINT__)

#endif // FIT_TREE_FILE_CPP__

