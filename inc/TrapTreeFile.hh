// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: TrapTreeFile.hh
// Purpose: Handles ROOT TTree file with E,t from linear trap filter
 
#ifndef TRAP_TREE_FILE_HH__
#define TRAP_TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "TreeFile.hh"

using std::cout;
using std::endl;
using std::vector;

class TrapTreeFile : public TreeFile
{
private:
  struct event_t{
    Double_t MaxE;
    Double_t MidE;
    Double_t AveE;
    Double_t t;
    Double_t Flat0;
    Double_t Flat1;
    Int_t up;
    Int_t down;
    Int_t ch;
	Int_t wavefile;
    Int_t waveev;
  };
  Int_t decay;
  Int_t shaping;
  Int_t top;
  bool setup;  
public:
  event_t Trap_event;  //Do the thing!
  TrapTreeFile();
  ~TrapTreeFile();
  using TreeFile::Open;
  bool Open(int filenum, int decay, int shape, int top);
#if !defined (__CINT__)
  using TreeFile::Create;
  bool Create(int filenum, int decay, int shape, int top);
#endif // !defined (__CINT__)
  using TreeFile::Sort;
  void Sort(TreeFile& origfile);
private:
  void SetNameStr() {sprintf(namestr,"trap%%05d.root");};
  void SetBranches();
  void MakeBranches();
};

#endif // TRAP_TREE_FILE_HH__
