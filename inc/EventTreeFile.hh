// File: EventTreeFile.hh
// Name: Leah Broussard
// Date: 2015/5/29
// Purpose: Handles ROOT TTree file with events E[numch], T
//
// Revision History:
// 2015/5/29:  LJB  Created
 
#ifndef EVENT_TREE_FILE_HH__
#define EVENT_TREE_FILE_HH__

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

class EventTreeFile
{
private:
  struct event_t{
    Int_t numch;
    Double_t E[MAXCH*NRIO];
    Double_t t;
  };
  TFile* RootFile;
  TTree* RootTree;
  bool createmode;
public:
  event_t myEvent;  //Do the thing!
  EventTreeFile();
  ~EventTreeFile();
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

#endif // EVENT_TREE_FILE_HH__
