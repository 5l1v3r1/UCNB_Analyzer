// File: Sorter.hh
// Name: Leah Broussard
// Date: 2015/6/21
// Purpose: Sorts events in time order
//
// Revision History:
// 2016/1/2:   LJB  Created
 
#ifndef SORTER_HH__
#define SORTER_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>

//#include "TSystem.h"
#include "Rtypes.h"
#include "TMath.h"

using std::list;
using std::cout;
using std::endl;

#if !defined (__CINT__)
#endif

class Sorter
{
private:
  struct orderedlist_t{
	int ev;
	ULong64_t timestamp;
  };
  list<orderedlist_t> orderedlist;
  list<orderedlist_t>::iterator olit;
public:
  Sorter();
  ~Sorter();
  void Reset();
  void InsertEv(ULong64_t timestamp, int ev);
  int GetNextEvent(bool first = false);
};

#endif // SORTER_HH__
