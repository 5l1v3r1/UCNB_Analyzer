// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: FindCoincidence.cpp
// Purpose: Analysis task: find coincidences within time window

#ifndef FIND_COINCIDENCE_CPP__
#define FIND_COINCIDENCE_CPP__

#include <math.h>

#include "FindCoincidence.hh"
#include "TrigTreeFile.hh"
#include "EventTreeFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
FindCoincidence::FindCoincidence() {
}

FindCoincidence::FindCoincidence(int smpcoinc, std::string calibfile, std::string pin, std::string pout) {
	Set(smpcoinc, calibfile, pin, pout);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
FindCoincidence::~FindCoincidence() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void FindCoincidence::Set(int sc, std::string calf, std::string pin, std::string pout) {
	smpcoinc = sc;
	calibfile = calf;
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void FindCoincidence::Go(int filenum) {
  cout << "Collecting single-event coincidences in file " << filenum << endl;
  //-----Open input/output files
  TrigTreeFile InputFile;  //expand to trig/trap/fit/coll? at some point
  //TrapTreeFile InputFile;  //expand to trig/trap/fit/coll? at some point
  if (pathin.compare("") != 0) { 
	InputFile.SetPath(pathin);
  }
  //  if (!InputFile.Open(filenum, 200, 150,150)) {
  if (!InputFile.Open(filenum)) {
    cout << "Input file Not Open!" << endl;
    return;
  }
  EventTreeFile EventFile;
  if (pathout.compare("") != 0) { 
	EventFile.SetPath(pathout);
  }
  if (!EventFile.Create(filenum)) {
		cout << "Output file not open!" << endl;
		return;
	}
  int nentries = InputFile.GetNumEvents();
  int StartEv = 0;
  if (nentries == 0){
    EventFile.Write();
    cout << "No triggers: done" << endl;
    EventFile.Close();
    InputFile.Close();
    return;
  }
  
  //-----Open calibration file
  double q0[48],q1[48],q2[48];
  for(int i=0; i<48; i++) {
	  q0[i]=0;q1[i]=1;q2[i]=0; //no calibration
  }
  if (calibfile.compare("") != 0) { 
	std::ifstream cal(calibfile.c_str());
	for(int i=0; i<48; i++) {
      cal>>q2[i]>>q1[i]>>q0[i];
    }
  }
  int numch = MAXCH*MAXRIO;
  double tlast = -1, Elast = -1;
  do {
    //-----Get start trigger of event
    printf("Working....%d/%d  (%d %%)\r",StartEv,nentries,100*StartEv/nentries);
    for (int ch=0;ch<numch;ch++)
      EventFile.myEvent.E[ch] = 0;
    InputFile.GetEvent(StartEv);
	//    EventFile.myEvent.t = InputFile.Trap_event.t;
    EventFile.myEvent.t = InputFile.Trig_event.t;
	EventFile.myEvent.tprev = (tlast == -1)? -1 : (EventFile.myEvent.t - tlast);
	EventFile.myEvent.Eprev = (Elast == -1)? -1 : Elast;
	EventFile.myEvent.numch = 0;
    //-----Get triggers within window
    int ev = StartEv;
    do {
		//	  double Ecal = q2[InputFile.Trap_event.ch]*InputFile.Trap_event.AveE*InputFile.Trap_event.AveE/16. + q1[InputFile.Trap_event.ch]*InputFile.Trap_event.AveE/4. + q0[InputFile.Trap_event.ch];
	  double Ecal = q2[InputFile.Trig_event.ch]*InputFile.Trig_event.E*InputFile.Trig_event.E/16. + q1[InputFile.Trig_event.ch]*InputFile.Trig_event.E/4. + q0[InputFile.Trig_event.ch];
	  if (Ecal > 5)
		EventFile.myEvent.E[InputFile.Trig_event.ch] += Ecal;
//		EventFile.myEvent.E[InputFile.Trap_event.ch] += Ecal;
      ev++;
      if (ev < nentries)
		InputFile.GetEvent(ev);
      }while (ev < nentries && (InputFile.Trig_event.t - EventFile.myEvent.t)<smpcoinc);
	  //      }while (ev < nentries && (InputFile.Trap_event.t - EventFile.myEvent.t)<smpcoinc);
	EventFile.myEvent.Esum = 0;
	for (int ch = 0; ch < MAXCH*MAXRIO; ch++) {
		if (EventFile.myEvent.E[ch] > 5) {
			EventFile.myEvent.numch++;
			EventFile.myEvent.Esum += EventFile.myEvent.E[ch];
		}
	}
	EventFile.myEvent.run = filenum;
	EventFile.myEvent.wavefile = -1;
	EventFile.myEvent.waveev = -1;
	if (EventFile.myEvent.Esum > 5) {
		EventFile.FillTree();
		Elast = EventFile.myEvent.Esum;
		tlast = EventFile.myEvent.t;
	}
    StartEv = ev;
  } while (StartEv < nentries);
  EventFile.Write();
  cout << "Done" << endl;
  EventFile.Close();
  InputFile.Close();
}


#endif //ifndef FIND_COINCIDENCE_CPP__

