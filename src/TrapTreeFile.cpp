// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: TrapTreeFile.cpp
// Purpose: Handles ROOT TTree file with E,t from linear trap filter
 

#ifndef TRAP_TREE_FILE_CPP__
#define TRAP_TREE_FILE_CPP__

#include "TrapTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TrapTreeFile::TrapTreeFile() {
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
TrapTreeFile::~TrapTreeFile() {
}

/*************************************************************************/
//                             SetBranches 
/*************************************************************************/
void TrapTreeFile::SetBranches() {
  RootTree->SetBranchAddress("MaxE",&Trap_event.MaxE);
  RootTree->SetBranchAddress("AveE",&Trap_event.AveE);
  RootTree->SetBranchAddress("MidE",&Trap_event.MidE);
  RootTree->SetBranchAddress("t",&Trap_event.t);
  RootTree->SetBranchAddress("Flat0",&Trap_event.Flat0);
  RootTree->SetBranchAddress("Flat1",&Trap_event.Flat1);
  RootTree->SetBranchAddress("up",&Trap_event.up);
  RootTree->SetBranchAddress("down",&Trap_event.down);
  RootTree->SetBranchAddress("ch",&Trap_event.ch);

  RootTree->GetEntry(0);
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool TrapTreeFile::Open(int filenum, int decay, int shape, int top) {
  char tempstr[255];
  sprintf(tempstr,"trap%05d_decay%05d_shape%05d_top%05d.root",filenum, decay, shape, top);
  std::string filename = tempstr;
  if (pathset) {
    return Open(mypath,filename);
  }
  else
    return Open(filename);
}

#if !defined (__CINT__)

/*************************************************************************/
//                             MakeBranches 
/*************************************************************************/
void TrapTreeFile::MakeBranches() {
  RootTree->Branch("MaxE",&Trap_event.MaxE,"MaxE/D");
  RootTree->Branch("AveE",&Trap_event.AveE,"AveE/D");
  RootTree->Branch("MidE",&Trap_event.MidE,"MidE/D");
  RootTree->Branch("t",&Trap_event.t,"t/D");
  RootTree->Branch("Flat0",&Trap_event.Flat0,"Flat0/D");
  RootTree->Branch("Flat1",&Trap_event.Flat1,"Flat1/D");
  RootTree->Branch("up",&Trap_event.up,"up/I");
  RootTree->Branch("down",&Trap_event.down,"down/I");
  RootTree->Branch("ch",&Trap_event.ch,"ch/I");
}

/*************************************************************************/
//                               Create  
/*************************************************************************/
bool TrapTreeFile::Create(int filenum, int decay, int shape, int top) {
  char tempstr[255];
  sprintf(tempstr,"trap%05d_decay%05d_shape%05d_top%05d.root",filenum, decay, shape, top);
  std::string filename = tempstr;
  if (pathset)
    return Create(mypath,filename);
  else
	return Create(filename);
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                                 Sort
/*************************************************************************/

void TrapTreeFile::Sort(TreeFile& origfile){
	Sort(origfile,"t");
}


#endif // TRAP_TREE_FILE_CPP__

