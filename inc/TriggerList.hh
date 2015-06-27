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
#include "Trigger.hh"

using std::vector;
using std::cout;
using std::endl;

#if !defined (__CINT__)
#endif

class TriggerList
{
private:
  vector<trigger_t> triggerlist;
  struct trigorderedlist_t{
    trigorderedlist_t* prev;
    trigorderedlist_t* next;
    trigger_t* mytrig;
  };
  vector<trigorderedlist_t> trigorderedlist;
  bool ordered;
  int tottrig;
  trigorderedlist_t* check;
public:
  TriggerList();
  ~TriggerList();
  void Reset();
  void AddTriggers(vector<trigger_t> addtrigs);
  void OrderTriggers();
  bool GetTrigger(trigger_t &trig);
};

#endif // TRIGGERLIST_HH__
