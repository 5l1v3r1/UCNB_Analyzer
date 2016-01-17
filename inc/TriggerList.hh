// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: TriggerList.hh
// Purpose: Handles list of triggers in an event
 
#ifndef TRIGGERLIST_HH__
#define TRIGGERLIST_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Rtypes.h"
#include "TMath.h"
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
  bool GetMaxTrigger(trigger_t &trig);
};

#endif // TRIGGERLIST_HH__
