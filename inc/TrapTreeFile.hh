// File: TrapTreeFile.hh
// Name: Leah Broussard
// Date: 2015/5/14
// Purpose: Handles ROOT TTree file with E,t from linear trap filter
//
// Revision History:
// 2015/5/14:  LJB  Created
 
#ifndef TRAP_TREE_FILE_HH__
#define TRAP_TREE_FILE_HH__

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

class TrapTreeFile
{
private:
  struct event_t{
    Double_t MaxE;
    Double_t MidE;
    Double_t AveE;
    Double_t t;
    Double_t Flat0;
    Double_t Flat1;
    Int_t up;
    Int_t down;
    Int_t ch;
  };
  TFile* RootFile;
  TTree* RootTree;
  bool createmode;
  std::string mypath;
  bool pathset;
  Int_t decay;
  Int_t shaping;
  Int_t top;
  bool setup;  
public:
  event_t Trap_event;  //Do the thing!
  TrapTreeFile();
  ~TrapTreeFile();
  void Close();
  bool Open(std::string path, std::string name);
  bool Open(std::string filename);
  bool Open(int filenum, int decay, int shape, int top);
  bool IsOpen();
#if !defined (__CINT__)
  bool Create(std::string path, std::string name);
  bool Create(std::string filename);
  bool Create(int filenum, int decay, int shape, int top);
#endif // !defined (__CINT__)
  void FillTree();
  void Write();
  Int_t GetNumEvents(){return RootTree->GetEntries();};
  void GetEvent(Int_t ev);
  void SetPath(std::string newpath) {mypath = newpath; pathset = true;}
};

#endif // TRAP_TREE_FILE_HH__
