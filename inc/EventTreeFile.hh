// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: EventTreeFile.hh
// Purpose: Handles ROOT TTree file with events E[numch], T
 
#ifndef EVENT_TREE_FILE_HH__
#define EVENT_TREE_FILE_HH__

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

class EventTreeFile : public TreeFile
{
private:
  struct event_t{
    Double_t E[MAXCH*MAXRIO];
    Double_t t;
    Int_t numch;
    Int_t waveev;
  };
public:
  event_t myEvent;
  EventTreeFile();
  ~EventTreeFile();
private:
  void SetNameStr() {sprintf(namestr,"ev%%05d.root");};
  void SetBranches();
  void MakeBranches();
};

#endif // EVENT_TREE_FILE_HH__
