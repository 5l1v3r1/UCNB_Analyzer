// File: TriggerList.cpp
// Name: Leah Broussard
// Date: 2015/6/21
// Purpose: Handles list of triggers in an event
//
// Revision History:
// 2015/6/21:  LJB  Created
// 2015/6/25:  LJB  Working OrderTriggerList routine
 
#ifndef TRIGGERLIST_CPP__
#define TRIGGERLIST_CPP__

#include "TriggerList.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TriggerList::TriggerList() {
  nch = 0;
  setch = false;
  ordered = false;
  tottrig = 0;
}

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TriggerList::TriggerList(int numch) {
  SetNumch(numch);
}

/*************************************************************************/
//                             Destructor
/*************************************************************************/
TriggerList::~TriggerList() {
}

/*************************************************************************/
//                             SetupTrigs
/*************************************************************************/
void TriggerList::SetNumch(int numch) {
  nch = numch;
  triglist.resize(numch);
  for (int i=0;i<nch;i++) {
    triglist[i].E.resize(0);
    triglist[i].T.resize(0);
  }
  setch = true;
  ordered = false;
  tottrig = 0;
}

/*************************************************************************/
//                             SetTriggerList
/*************************************************************************/
void TriggerList::SetTriggerList(int ch, vector<Double_t> E, vector<Double_t> T) {
  if (!setch) {cout << "Call SetNumch first" << endl; return;}
  if (ch >= nch) {cout << "Out of range: " << ch << " >= " << nch << endl; return;}
  triglist[ch].E.swap(E);
  triglist[ch].T.swap(T);
}

/*************************************************************************/
//                            OrderTriggerList
/*************************************************************************/
void TriggerList::OrderTriggerList() {
  if (!setch) {cout << "Call SetNumch and use SetTriggerList first" << endl; return;}
  tottrig = 0;
  for (int i=0;i<nch;i++) {
    tottrig += triglist[i].E.size();
  }
  trigorderedlist.resize(tottrig);
  
  int cnt = 0;
  if (tottrig > 0) {
    for (int ch=0;ch<nch;ch++){
      for (int j=0;j<triglist[ch].E.size();j++) {
	  trigorderedlist[cnt].ch = ch;
	  trigorderedlist[cnt].myE = &triglist[ch].E[j];
	  trigorderedlist[cnt].myT = &triglist[ch].T[j];
	  trigorderedlist[cnt].prev = NULL;
	  trigorderedlist[cnt].next = NULL;
	  if (cnt > 0) {
	    trigorderedlist_t* check = &trigorderedlist[0];
	    if (*trigorderedlist[cnt].myT > *check->myT) {
	      while (*trigorderedlist[cnt].myT > *check->myT && check->next != NULL) {
		check = check->next;
	      }
	      if (*trigorderedlist[cnt].myT <= *check->myT) {
		trigorderedlist[cnt].next = check;
		trigorderedlist[cnt].prev = check->prev;
		check->prev = &trigorderedlist[cnt];
		if (trigorderedlist[cnt].prev != NULL)
		  trigorderedlist[cnt].prev->next = &trigorderedlist[cnt];
	      }
	      else{
		trigorderedlist[cnt].prev = check;
		trigorderedlist[cnt].next = check->next;
		check->next = &trigorderedlist[cnt];
		if (trigorderedlist[cnt].next != NULL)
		  trigorderedlist[cnt].next->prev = &trigorderedlist[cnt];
	      }
	    }
	    else {
	      while (*trigorderedlist[cnt].myT < *check->myT && check->prev != NULL) {
		check = check->prev;
	      }
	      if (*trigorderedlist[cnt].myT >= *check->myT) {
		trigorderedlist[cnt].prev = check;
		trigorderedlist[cnt].next = check->next;
		check->next = &trigorderedlist[cnt];
		if (trigorderedlist[cnt].next != NULL)
		  trigorderedlist[cnt].next->prev = &trigorderedlist[cnt];
	      }
	      else {
		trigorderedlist[cnt].next = check;
		trigorderedlist[cnt].prev = check->prev;
		check->prev = &trigorderedlist[cnt];
		if (trigorderedlist[cnt].prev != NULL)
		  trigorderedlist[cnt].prev->next = &trigorderedlist[cnt];
	      }
	    }// if !(*trigorderedlist[cnt].myT > *check->myT)
	  }// if (cnt > 0)
	  cnt++;
      }//j < size
    }//ch < numch
  }//tottrig > 0
  ordered = true;
  check = &trigorderedlist[0];
  while (check->prev != NULL)
    check = check->prev;
}

/*************************************************************************/
//                            GetNextTrigger
/*************************************************************************/
bool TriggerList::GetTrigger(Int_t& ch, Double_t& E, Double_t& T) {
  if (!ordered) {cout << "Call OrderTriggerList first" << endl; return false;}
  if (check == NULL) return false;
  ch = check->ch;
  E = *check->myE;
  T = *check->myT;
  check = check->next;
  return true;
}

#endif // TRIGGERLIST_CPP__
