// File: RawTreeFile.hh
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Handles raw TTree file with waveforms
//
// Revision History:
// 2015/5/8:   LJB  Create
// 2015/5/11:  LJB  Read/write ROOT files with Tree of NI_event
// 2015/7/16:  LJB  Specialized to June data format
// 2015/11/20: LJB  Standardized all formats to June format
// 2016/1/2:   LJB  Added event copy routine
// 2016/1/16:  LJB  Added sort routine
 
#ifndef RAW_TREE_FILE_HH__
#define RAW_TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

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
  void FillEvent(vector<BinFile::BinEv_t*> &BinEv); 
  void FillFebEvent(vector<NIFeb2015BinFile::FebBinEv_t*> &FebBinEv);
  void FillJuneEvent(vector<NIJune2015BinFile::JuneBinEv_t*> &JuneBinEv);
  void FillRawEvent(RawEv_t& event);
  void Sort();
  void Sort(RawTreeFile& origfile);
private:
  void SetNameStr() {sprintf(namestr,"run%%05d.root");};
  void SetBranches();
  void MakeBranches();
};

#endif // RAW_TREE_FILE_HH__
