// File: EventTreeFile.hh
// Name: Leah Broussard
// Date: 2015/6/29
// Purpose: Handles ROOT TTree file with events E[numch], T
//
// Revision History:
// 2015/6/29:  LJB  Created
// 2015/7/8:   LJB  TTree fix: added SetupTree method
// 2016/1/14:  LJB  Move base functions to TreeFile
 
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

#include "TreeFile.hh"

using std::cout;
using std::endl;

class EventTreeFile : public TreeFile
{
private:
  struct event_t{
    Double_t E[MAXCH*MAXRIO];
    Double_t t;
    Int_t numch;
    Int_t waveev;
  };
public:
  event_t myEvent;
  EventTreeFile();
  ~EventTreeFile();
private:
  void SetNameStr() {sprintf(namestr,"ev%%05d.root");};
  void SetBranches();
  void MakeBranches();
};

#endif // EVENT_TREE_FILE_HH__
