// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: FitTreeFile.hh
// Purpose: Handles ROOT TTree file with E,t
 
#ifndef FIT_TREE_FILE_HH__
#define FIT_TREE_FILE_HH__

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

class FitTreeFile : public TreeFile
{
private:
  struct event_t{
    Double_t E;
    Double_t t;
    Double_t shaping;
    Double_t integ;
    Double_t chi2;
    Double_t trapE;
    Double_t trapT;
    Int_t rio;
    Int_t rio_ch;
    Int_t chan;
    Int_t waveev;
  }; 
public:
  event_t Fit_event;
  FitTreeFile();
  ~FitTreeFile();
private:
  void SetNameStr() {sprintf(namestr,"fit%%05d.root");};
  void SetBranches();
  void MakeBranches();
};

#endif // FIT_TREE_FILE_HH__
