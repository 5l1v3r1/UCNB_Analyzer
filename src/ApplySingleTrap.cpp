// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ApplySingleTrap.cpp
// Purpose: Analysis task: apply single trap filter over waveform

#ifndef APPLY_SINGLE_TRAP_CPP__
#define APPLY_SINGLE_TRAP_CPP__

#include <math.h>

#include "ApplySingleTrap.hh"
#include "RawTreeFile.hh"
#include "TrapTreeFile.hh"
#include "WaveformAnalyzer.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
ApplySingleTrap::ApplySingleTrap() {
}

ApplySingleTrap::ApplySingleTrap(int thresh, int decay, int shaping, int top, std::string pin, std::string pout) {
	Set(thresh, decay, shaping, top, pin, pout);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
ApplySingleTrap::~ApplySingleTrap() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void ApplySingleTrap::Set(int th, int d, int s, int t, std::string pin, std::string pout) {
	thresh = th;
	decay = d;
	shaping = s;
	top = t;
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void ApplySingleTrap::Go(int filenum) {
	//-----Open input/output files
	RawTreeFile RootFile;
	if (pathin.compare("") != 0) { 
		RootFile.SetPath(pathin);
	}
	if (!RootFile.Open(filenum)) {
		cout << "File Not Open!" << endl;
		return;
	}
	TrapTreeFile TrapFile;
	if (pathout.compare("") != 0) { 
		TrapFile.SetPath(pathout);
	}
	TrapFile.SetTmp();
	if (!TrapFile.Create(filenum,decay,shaping,top)) {
		cout << "File Not Open!" << endl;
		return;
	}
	WaveformAnalyzer WA;
	WA.SetTrapPars(decay,shaping,top);
	cout << "Applying trap filter (decay/rise/top = " << decay <<"/" << shaping << "/" << top << ") to file " << filenum << endl;
	int filecount = 0;
	//Shaping time scan hack, used for 1 set of runs
	//int dlow = 250, dmid = 1200, dhi = 2000;
	//int decaylist[] = {dlow,dlow,dlow,dlow,dlow, 			dmid,dmid,dlow,dmid,dlow,dhi,dlow,dlow,dhi,dhi,					dlow,dlow,dlow,dlow,dlow};
	do {
		int nentries = RootFile.GetNumEvents();
		//-----Find triggers
		for (int ev=0;ev<nentries;ev++) {
			printf("Working....%d/%d  (%d %%)\r",ev,nentries,100*ev/nentries);
			RootFile.GetEvent(ev);
			//Shaping time scan hack, used for 1 set of runs
			//if (RootFile.NI_event.ch > 20) 
			//	WA.SetTrapPars(dlow,shaping,top);
			//else
			//	WA.SetTrapPars(decaylist[RootFile.NI_event.ch],shaping,top);
			WA.MakeTrap(RootFile.NI_event.length, RootFile.NI_event.wave);
			vector<trigger_t> triglist; 
			//maybe add veto for events too early in file
			WA.GetTriggers(thresh,triglist);
			for (int t=0;t<triglist.size();t++) {
				TrapFile.Trap_event.MaxE = triglist[t].TrapE;
				TrapFile.Trap_event.AveE = triglist[t].AveTrapE;
				TrapFile.Trap_event.MidE = triglist[t].MidTrapE;
				TrapFile.Trap_event.t = triglist[t].TrapT + (double)RootFile.NI_event.timestamp;
				TrapFile.Trap_event.Flat0 = triglist[t].Flat0;
				TrapFile.Trap_event.Flat1 = triglist[t].Flat1;
				TrapFile.Trap_event.up = triglist[t].up;
				TrapFile.Trap_event.down = triglist[t].down;
				TrapFile.Trap_event.ch = RootFile.NI_event.ch;
				TrapFile.Trap_event.wavefile = filecount;
				TrapFile.Trap_event.waveev = ev;
				TrapFile.FillTree();
			}
		}//ev < NumEvents
		RootFile.Close();
		if (!RootFile.Open(filenum,++filecount))
			filecount = -1;
	} while (filecount != -1);
	//-----Sort by timestamp
	TrapTreeFile NewFile;
	if (pathout.compare("") != 0) { 
		NewFile.SetPath(pathout);
	}
	if (!NewFile.Create(filenum,decay,shaping,top)) {
		cout << "Output file not open!" << endl;
		return;
	}
	NewFile.Sort(TrapFile);
	NewFile.Write();
	NewFile.Close();
	std::string tmpname = pathout;
	tmpname.append("/");
	tmpname.append(TrapFile.GetName());
	TrapFile.Close();
	remove(tmpname.c_str());
}





#endif //ifndef APPLY_SINGLE_TRAP_CPP__

