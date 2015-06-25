// File: ROOTTreeFile.hh
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Handles ROOT TTree file with waveforms
//
// Revision History:
// 2015/5/8:  LJB  Create
// 2015/5/11: LJB  Read/write ROOT files with Tree of NI_event
 
#ifndef ROOT_TREE_FILE_HH__
#define ROOT_TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

//#include <TROOT.h>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "LocalCFG.hh"
#include "EvType.hh"
#if !defined (__CINT__)
#endif

using std::cout;
using std::endl;
using std::vector;

class ROOTTreeFile
{
private:
  TFile* RootFile;
  TTree* RootTree;
  bool createmode;
  Int_t nrio;
  Int_t nch;
  bool setup;  
public:
  ev_t NI_event;
  ROOTTreeFile();
  ~ROOTTreeFile();
  void Close();
  bool Open(std::string path, std::string name);
  bool Open(std::string filename);
  bool Open(int filenum);
  bool IsOpen();
#if !defined (__CINT__)
  bool Create(std::string path, std::string name);
  bool Create(std::string filename);
  bool Create(int filenum);
#endif // !defined (__CINT__)
  void SetupTree(int numch);
  void FillTree();
  void Write();
  Int_t GetNumEvents(){return RootTree->GetEntries();};
  void GetEvent(Int_t ev);
};

#endif // ROOT_TREE_FILE_HH__