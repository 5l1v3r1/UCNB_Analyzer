// File: ROOTTreeFileJune.hh
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Handles ROOT TTree file with waveforms
//
// Revision History:
// 2015/5/8:  LJB  Create
// 2015/5/11: LJB  Read/write ROOT files with Tree of NI_event
// 2015/7/16: LJB  Specialized to June data format
 
#ifndef ROOT_TREE_FILE_JUNE_HH__
#define ROOT_TREE_FILE_JUNE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

//#include <TROOT.h>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "NIJune2015BinFile.hh"
#include "TreeFile.hh"
#include "LocalCFG.hh"

using std::cout;
using std::endl;
using std::vector;

class ROOTTreeFileJune : public TreeFile
{
public:
  struct JuneROOTev_t {
    ULong64_t timestamp;
    Int_t board;
    Int_t channel;
    Int_t ch;
    Int_t eventID;
    Int_t result;
    Int_t length;
    Short_t wave[MAXWAVE]; //ROOT TTree hates vectors
  };
  JuneROOTev_t NI_event;
public:
  ROOTTreeFileJune();
  ~ROOTTreeFileJune();
  bool Open(std::string path, std::string name);
  bool Open(std::string filename);
  bool Open(int filenum);
#if !defined (__CINT__)
  bool Create(std::string path, std::string name);
  bool Create(std::string filename);
  bool Create(int filenum);
#endif // !defined (__CINT__)
  void FillEvent(BinFile::BinEv_t& BinEv);
private:
  void SetBranches();
  void MakeBranches();
};

#endif // ROOT_TREE_FILE_JUNE_HH__
