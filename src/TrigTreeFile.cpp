// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: TrigTreeFile.cpp
// Purpose: Handles ROOT TTree file with E,t from .trig file
 

#ifndef TRIG_TREE_FILE_CPP__
#define TRIG_TREE_FILE_CPP__

#include "TrigTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TrigTreeFile::TrigTreeFile() {
  SetNameStr();
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
TrigTreeFile::~TrigTreeFile() {
}

/*************************************************************************/
//                             SetBranches 
/*************************************************************************/
void TrigTreeFile::SetBranches() {
  RootTree->SetBranchAddress("E",&Trig_event.E);
  RootTree->SetBranchAddress("t",&Trig_event.t);
  RootTree->SetBranchAddress("ch",&Trig_event.ch);

  RootTree->GetEntry(0);
}


#if !defined (__CINT__)

/*************************************************************************/
//                             MakeBranches 
/*************************************************************************/
void TrigTreeFile::MakeBranches() {
  RootTree->Branch("E",&Trig_event.E,"E/D");
  RootTree->Branch("t",&Trig_event.t,"t/D");
  RootTree->Branch("ch",&Trig_event.ch,"ch/I");
}

/*************************************************************************/
//                             FillEvent
/*************************************************************************/

bool TrigTreeFile::FillEvent(BinFile::BinEv_t* BinEv) {
	NIDec2015TrigBinFile::DecTrigBinEv_t* DecTrigBinEv;
	DecTrigBinEv = dynamic_cast<NIDec2015TrigBinFile::DecTrigBinEv_t*>(BinEv);
	NIMay2016TrigBinFile::MayTrigBinEv_t* MayTrigBinEv;
	MayTrigBinEv = dynamic_cast<NIMay2016TrigBinFile::MayTrigBinEv_t*>(BinEv);
	if (DecTrigBinEv) {
		return FillDecEvent(DecTrigBinEv);
	}
	else if (MayTrigBinEv) {
		return FillMayEvent(MayTrigBinEv);		
	}
	else 
		return false;
}
  
bool TrigTreeFile::FillDecEvent(NIDec2015TrigBinFile::DecTrigBinEv_t* DecTrigBinEv){
	Trig_event.ch = DecTrigBinEv->board*MAXCH + DecTrigBinEv->channel;
	Trig_event.t = (double)DecTrigBinEv->timestamp;
	Trig_event.E = DecTrigBinEv->adc;
	FillTree();
	return true;
}
  
bool TrigTreeFile::FillMayEvent(NIMay2016TrigBinFile::MayTrigBinEv_t* MayTrigBinEv){
	Trig_event.ch = MayTrigBinEv->board*MAXCH + MayTrigBinEv->channel;
	Trig_event.t = (double)MayTrigBinEv->timestamp;
	Trig_event.E = MayTrigBinEv->adc;
	FillTree();
	return true;
}

/*************************************************************************/
//                                 Sort
/*************************************************************************/

void TrigTreeFile::Sort(TreeFile& origfile){
	Sort(origfile,"t");
}


#endif // !defined (__CINT__)

#endif // TRIG_TREE_FILE_CPP__

