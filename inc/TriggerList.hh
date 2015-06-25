// File: TriggerList.hh
// Name: Leah Broussard
// Date: 2015/6/21
// Purpose: Handles list of triggers in an event
//
// Revision History:
// 2015/6/21:  LJB  Created
// 2015/6/25:  LJB  Working OrderTriggerList routine
 
#ifndef TRIGGERLIST_HH__
#define TRIGGERLIST_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

//#include "TSystem.h"
#include "Rtypes.h"

using std::vector;
using std::cout;
using std::endl;

#if !defined (__CINT__)
#endif

class TriggerList
{
private:
  struct triglist_t{
    vector<Double_t> E;
    vector<Double_t> T;
  };
  vector<triglist_t> triglist;
  struct trigorderedlist_t{
    trigorderedlist_t* prev;
    trigorderedlist_t* next;  
    Double_t *myE;
    Double_t *myT;
    Int_t ch;
  };
  vector<trigorderedlist_t> trigorderedlist;
  bool setch;
  bool ordered;
  int nch;
  int tottrig;
  trigorderedlist_t* check;
public:
  TriggerList();
  TriggerList(int numch);
  ~TriggerList();
  void SetNumch(int numch);
  void SetTriggerList(int ch, vector<Double_t> E, vector<Double_t> T);
  void OrderTriggerList();
  bool GetTrigger(Int_t& ch, Double_t& E, Double_t& T);
};

#endif // TRIGGERLIST_HH__
