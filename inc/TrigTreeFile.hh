// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: TrigTreeFile.hh
// Purpose: Handles ROOT TTree file with E,t from .trig file
 
#ifndef TRIG_TREE_FILE_HH__
#define TRIG_TREE_FILE_HH__

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

class TrigTreeFile : public TreeFile
{
private:
  struct event_t{
    Double_t E;
    Double_t t;
    Int_t ch;
  }; 
public:
  event_t Trig_event;
  TrigTreeFile();
  ~TrigTreeFile();
  using TreeFile::Sort;
  void Sort(TreeFile& origfile);
private:
  void SetNameStr() {sprintf(namestr,"trg%%05d.root");};
  void SetBranches();
  void MakeBranches();
};

#endif // TRIG_TREE_FILE_HH__
