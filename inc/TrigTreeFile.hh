// File: TrigTreeFile.hh
// Name: Leah Broussard
// Date: 2015/5/14
// Purpose: Handles ROOT TTree file with E,t
//
// Revision History:
// 2015/5/14:  LJB  Created
 
#ifndef TRIG_TREE_FILE_HH__
#define TRIG_TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

//#include <TROOT.h>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "LocalCFG.hh"
#if !defined (__CINT__)
#endif

using std::cout;
using std::endl;
using std::vector;

class TrigTreeFile
{
private:
  struct event_t{
    Double_t E;
    Double_t t;
    Int_t rio;
    Int_t rio_ch;
    Int_t chan;
  };
  TFile* RootFile;
  TTree* RootTree;
  bool createmode;
  int privatevariable;
  Int_t nrio;
  Int_t nch;
  bool setup;  
public:
  event_t Trig_event;  //Do the thing!
  TrigTreeFile();
  ~TrigTreeFile();
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
  void FillTree();
  void Write();
  Int_t GetNumEvents(){return RootTree->GetEntries();};
  void GetEvent(Int_t ev);
};

#endif // TRIG_TREE_FILE_HH__
