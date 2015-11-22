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
 
#ifndef RAW_TREE_FILE_HH__
#define RAW_TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

//#include <TROOT.h>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "NIJune2015BinFile.hh"
#include "NIFeb2015BinFile.hh"
#include "TreeFile.hh"
#include "LocalCFG.hh"

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
public:
  RawTreeFile();
  ~RawTreeFile();
  bool Open(std::string path, std::string name);
  bool Open(std::string filename);
  bool Open(int filenum);
#if !defined (__CINT__)
  bool Create(std::string path, std::string name);
  bool Create(std::string filename);
  bool Create(int filenum);
#endif // !defined (__CINT__)
  void FillEvent(vector<BinFile::BinEv_t*> &BinEv); 
  void FillFebEvent(vector<NIFeb2015BinFile::FebBinEv_t*> &FebBinEv);
  void FillJuneEvent(vector<NIJune2015BinFile::JuneBinEv_t*> &JuneBinEv);
private:
  void SetBranches();
  void MakeBranches();
};

#endif // RAW_TREE_FILE_HH__
