// File: Sorter.cpp
// Name: Leah Broussard
// Date: 2015/6/21
// Purpose: Sorts event in time order
//
// Revision History:
// 2016/1/2:   LJB  Created
 
#ifndef SORTER_CPP__
#define SORTER_CPP__

#include "Sorter.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
Sorter::Sorter() {
  Reset();
}

/*************************************************************************/
//                             Destructor
/*************************************************************************/
Sorter::~Sorter() {
}

/*************************************************************************/
//                                Reset
/*************************************************************************/
void Sorter::Reset() {
  orderedlist.clear();
}

/*************************************************************************/
//                               InsertEv
/*************************************************************************/
void Sorter::InsertEv(ULong64_t timestamp, int ev) {
	orderedlist_t add;
	add.ev = ev;
	add.timestamp = timestamp;
	//cout << "****************************adding " << timestamp << endl;
	
	//cout << "the list:" << endl;
	//for (olit=orderedlist.begin(); olit!=orderedlist.end(); ++olit)
	//cout << olit->ev << ", " << olit->timestamp << endl;
	
	if (orderedlist.empty()){
		orderedlist.push_back(add);
		return;
	}
	olit = orderedlist.end(); olit--;
	while (timestamp < olit->timestamp && olit != orderedlist.begin()) {
		//cout << timestamp << " going before " << olit->timestamp << endl;
		olit--;
	}
	//cout << "stopped at " << olit->timestamp << endl;
	olit++;
	orderedlist.insert(olit,add);
	
		/*
		cout << "the list:" << endl;
		for (olit=orderedlist.begin(); olit!=orderedlist.end(); ++olit)
		cout << olit->ev << ", " << olit->timestamp << endl;
	*/
	
	/*
	if (orderedlist[cnt].timestamp > olptr->timestamp) {
		while (orderedlist[cnt].timestamp > olptr->timestamp && olptr->next != NULL) {
			cout << "need to go ahead past " << olptr->timestamp << endl;
			olptr = olptr->next;
			cout << "examine " << olptr->timestamp << endl;
			cout << "* " << olptr->ev << ", " << olptr->timestamp << endl;
		}
	}
	else {
		while (orderedlist[cnt].timestamp < olptr->timestamp && olptr->prev != NULL) {
			cout << "need to go before past " << olptr->timestamp << endl;
			olptr = olptr->prev;
		}
	}		
	if (orderedlist[cnt].timestamp <= olptr->timestamp) {
		cout << "compare " << orderedlist[cnt].timestamp << " <= " << olptr->timestamp << endl;
		orderedlist[cnt].next = olptr;
		orderedlist[cnt].prev = olptr->prev;
		olptr->prev = &orderedlist[cnt];
		if (orderedlist[cnt].prev != NULL)
			orderedlist[cnt].prev->next = &orderedlist[cnt];
		else //update first
			nextev = &orderedlist[cnt];
			
		cout << "inserting ev " << add.timestamp << " before " << orderedlist[cnt].next->timestamp << endl;
	}
	else{
		cout << "compare " << orderedlist[cnt].timestamp << " > " << olptr->timestamp << endl;
		orderedlist[cnt].prev = olptr;
		orderedlist[cnt].next = olptr->next;
		olptr->next = &orderedlist[cnt];
		if (orderedlist[cnt].next != NULL)
			orderedlist[cnt].next->prev = &orderedlist[cnt];
		cout << "inserting ev " << add.timestamp << " after " << orderedlist[cnt].prev->timestamp << endl;
	}
	*/
}

/*************************************************************************/
//                             GetNextEvent
/*************************************************************************/
int Sorter::GetNextEvent(bool first) {
	if (first)
		olit = orderedlist.begin();
	if (olit == orderedlist.end()) return -1;
	int ev = olit->ev;
	olit++;
		//if (ev > 170 && ev < 180)
		//cout << olit->timestamp << endl;
	return ev;
	/*
	if (orderedlist.size() == 0) return -1;
	if (nextev == NULL) return -1;
	if (first) {
		nextev = &orderedlist[0];
		while (nextev->prev != NULL) {
			nextev = nextev->prev;
		}
	}
	int ev = nextev->ev;
	nextev = nextev->next;
	return ev;	
	*/
}


#endif // SORTER_CPP__
