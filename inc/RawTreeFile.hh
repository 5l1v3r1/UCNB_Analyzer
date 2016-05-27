// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: RawTreeFile.hh
// Purpose: Handles raw TTree file with waveforms
 
#ifndef RAW_TREE_FILE_HH__
#define RAW_TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "NIMay2016BinFile.hh"
#include "NIJune2015BinFile.hh"
#include "NIFeb2015BinFile.hh"
#include "TreeFile.hh"

using std::cout;
using std::endl;
using std::vector;

class RawTreeFile : public TreeFile
{
public:
  struct RawEv_t {
    ULong64_t timestamp;
    Int_t board;
    Int_t channel;
    Int_t ch;
    Int_t eventID;
    Int_t result;
    Int_t length;
    Short_t wave[MAXWAVE]; //ROOT TTree hates vectors
  };
  RawEv_t NI_event;
  RawTreeFile();
  ~RawTreeFile();
  bool FillEvent(vector<BinFile::BinEv_t*> &BinEv); 
  bool FillFebEvent(vector<NIFeb2015BinFile::FebBinEv_t*> &FebBinEv);
  bool FillJuneEvent(vector<NIJune2015BinFile::JuneBinEv_t*> &JuneBinEv);
  bool FillMayEvent(vector<NIMay2016BinFile::MayBinEv_t*> &MayBinEv);
  void FillRawEvent(RawEv_t& event);
  using TreeFile::Sort;
  void Sort(TreeFile& origfile);
private:
  void SetNameStr() {sprintf(namestr,"run%%05d.root");};
  void SetBranches();
  void MakeBranches();
};

#endif // RAW_TREE_FILE_HH__
