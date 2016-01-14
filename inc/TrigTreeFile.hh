// File: TrigTreeFile.hh
// Name: Leah Broussard
// Date: 2015/5/14
// Purpose: Handles ROOT TTree file with E,t
//
// Revision History:
// 2015/5/14:  LJB  Created
// 2016/1/13:  LJB  Move base functions to TreeFile
 
#ifndef TRIG_TREE_FILE_HH__
#define TRIG_TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "TreeFile.hh"

using std::cout;
using std::endl;
using std::vector;

class TrigTreeFile : public TreeFile
{
private:
  struct event_t{
    Double_t E;
    Double_t t;
    Double_t shaping;
    Double_t integ;
    Double_t chi2;
    Double_t trapE;
    Double_t trapT;
    Int_t rio;
    Int_t rio_ch;
    Int_t chan;
    Int_t waveev;
  }; 
public:
  event_t Trig_event;
  TrigTreeFile();
  ~TrigTreeFile();
private:
  void SetNameStr() {sprintf(namestr,"trig%%05d.root");};
  void SetBranches();
  void MakeBranches();
};

#endif // TRIG_TREE_FILE_HH__
