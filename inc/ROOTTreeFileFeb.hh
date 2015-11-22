// File: ROOTTreeFileFeb.hh
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Handles ROOT TTree file with waveforms
//
// Revision History:
// 2015/5/8:  LJB  Create
// 2015/5/11: LJB  Read/write ROOT files with Tree of NI_event
// 2015/7/16: LJB  Specialized to February data format
 
#ifndef ROOT_TREE_FILE_FEB_HH__
#define ROOT_TREE_FILE_FEB_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

//#include <TROOT.h>
#include "TFile.h"
#include "TTree.h"

#include "TreeFile.hh"
#include "LocalCFG.hh"
#if !defined (__CINT__)
#endif

using std::cout;
using std::endl;
using std::vector;

class ROOTTreeFileFeb : public TreeFile
{
private:
  Int_t nrio;
  Int_t nch;
  bool setup;
public:
  struct Febevch_t {
    Int_t wavelen;
    //std::vector<short> wave;  --LJB  TTree fails?
    Short_t wave[MAXWAVE];
    Short_t rio;
    Short_t ch;
    Short_t Edaq;
  };
  struct FebROOTev_t {
    Long64_t timestamp;
    Int_t numch;
    Febevch_t ch[MAXCH*NRIO];
  };
  FebROOTev_t NI_event;
public:
  ROOTTreeFileFeb();
  ~ROOTTreeFileFeb();
  bool Open(std::string path, std::string name);
  bool Open(std::string filename);
  bool Open(int filenum);
#if !defined (__CINT__)
  bool Create(int filenum);
#endif // !defined (__CINT__)
  void SetupTree(int numch);
  void FillEvent(BinFile::BinEv_t& BinEv);
  void Close();
  void FillTree();
  void Write();
private:
  void SetBranches();
};

#endif // ROOT_TREE_FILE_FEB_HH__
