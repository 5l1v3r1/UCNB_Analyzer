// File: TrigTreeFile.cpp
// Name: Leah Broussard
// Date: 2015/5/14
// Purpose: Handles ROOT TTree file with E,t
//
// Revision History:
// 2015/5/14:  LJB  Created
// 2016/1/13:  LJB  Move base functions to TreeFile
 

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
  RootTree->SetBranchAddress("shaping",&Trig_event.shaping);
  RootTree->SetBranchAddress("integ",&Trig_event.integ);
  RootTree->SetBranchAddress("trapE",&Trig_event.trapE);
  RootTree->SetBranchAddress("trapT",&Trig_event.trapT);
  RootTree->SetBranchAddress("chi2",&Trig_event.chi2);
  RootTree->SetBranchAddress("rio",&Trig_event.rio);
  RootTree->SetBranchAddress("rio_ch",&Trig_event.rio_ch);
  RootTree->SetBranchAddress("chan",&Trig_event.chan);
  RootTree->SetBranchAddress("waveev",&Trig_event.waveev);

  RootTree->GetEntry(0);
}


#if !defined (__CINT__)

/*************************************************************************/
//                             MakeBranches 
/*************************************************************************/
void TrigTreeFile::MakeBranches() {
  RootTree->Branch("E",&Trig_event.E,"E/D");
  RootTree->Branch("t",&Trig_event.t,"t/D");
  RootTree->Branch("shaping",&Trig_event.shaping,"shaping/D");
  RootTree->Branch("integ",&Trig_event.integ,"integ/D");
  RootTree->Branch("chi2",&Trig_event.chi2,"chi2/D");
  RootTree->Branch("trapE",&Trig_event.trapE,"trapE/D");
  RootTree->Branch("trapT",&Trig_event.trapT,"trapT/D");
  RootTree->Branch("rio",&Trig_event.rio,"rio/I");
  RootTree->Branch("rio_ch",&Trig_event.rio_ch,"rio_ch/I");
  RootTree->Branch("chan",&Trig_event.chan,"chan/I");
  RootTree->Branch("waveev",&Trig_event.waveev,"waveev/I");
}

#endif // !defined (__CINT__)

#endif // TRIG_TREE_FILE_CPP__

