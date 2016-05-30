// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: RawTreeFile.cpp
// Purpose: Handles ROOT TTree file with waveforms
 

#ifndef RAW_TREE_FILE_CPP__
#define RAW_TREE_FILE_CPP__

#include "RawTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
RawTreeFile::RawTreeFile() {
  SetNameStr();
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
RawTreeFile::~RawTreeFile() {
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool RawTreeFile::Open(int filenum, int which){
	char tempstr[255];
	sprintf(tempstr,"run%05d_%d.root",filenum,which);
	std::string filename = tempstr;
	if (pathset)
		return Open(mypath,filename);
	else
		return Open(filename);
}

/*************************************************************************/
//                             SetBranches 
/*************************************************************************/
void RawTreeFile::SetBranches() {
  RootTree->SetBranchAddress("timestamp",&NI_event.timestamp);
  RootTree->SetBranchAddress("board",&NI_event.board);
  RootTree->SetBranchAddress("channel",&NI_event.channel);
  RootTree->SetBranchAddress("ch",&NI_event.ch);
  RootTree->SetBranchAddress("eventID",&NI_event.eventID);
  RootTree->SetBranchAddress("result",&NI_event.result);
  RootTree->SetBranchAddress("length",&NI_event.length);
  RootTree->SetBranchAddress("wave",&NI_event.wave[0]);

  RootTree->GetEntry(0);
}

#if !defined (__CINT__)

/*************************************************************************/
//                             MakeBranches 
/*************************************************************************/
void RawTreeFile::MakeBranches() {
  RootTree->Branch("timestamp",&NI_event.timestamp,"timestamp/l");
  RootTree->Branch("board",&NI_event.board,"board/I");
  RootTree->Branch("channel",&NI_event.channel,"channel/I");
  RootTree->Branch("ch",&NI_event.ch,"ch/I");
  RootTree->Branch("eventID",&NI_event.eventID,"eventID/I");
  RootTree->Branch("result",&NI_event.result,"result/I");
  RootTree->Branch("length",&NI_event.length,"length/I");
  RootTree->Branch("wave",&NI_event.wave[0],"wave[length]/S");
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                             FillEvent
/*************************************************************************/

bool RawTreeFile::FillEvent(vector<BinFile::BinEv_t*> &BinEv) {
	if (BinEv.size() == 0) return false;
	
	vector<NIFeb2015BinFile::FebBinEv_t*> FebBinEv;
	vector<NIJune2015BinFile::JuneBinEv_t*> JuneBinEv;
	vector<NIMay2016BinFile::MayBinEv_t*> MayBinEv;
	vector<BinFile::BinEv_t*>::iterator iter;
	for (iter = BinEv.begin(); iter != BinEv.end(); ++iter) {
		FebBinEv.push_back(dynamic_cast<NIFeb2015BinFile::FebBinEv_t*>(*iter));
		JuneBinEv.push_back(dynamic_cast<NIJune2015BinFile::JuneBinEv_t*>(*iter));
		MayBinEv.push_back(dynamic_cast<NIMay2016BinFile::MayBinEv_t*>(*iter));
	}
	if (FebBinEv[0]) {
		return FillFebEvent(FebBinEv);
	}
	else if (JuneBinEv[0]) {
		return FillJuneEvent(JuneBinEv);		
	}
	else if (MayBinEv[0]) {
		return FillMayEvent(MayBinEv);		
	}
	else 
		return false;
}

  
bool RawTreeFile::FillFebEvent(vector<NIFeb2015BinFile::FebBinEv_t*> &FebBinEv){
	NI_event.eventID = 0;
	NI_event.result = 0;
	for (int rio=0;rio<FebBinEv.size();rio++) {
		NI_event.timestamp = static_cast<ULong64_t>(FebBinEv[rio]->timestamp);
		NI_event.board = rio;
		for (int ch=0;ch<FebBinEv[rio]->ch.size();ch++) {
			NI_event.channel = ch;
			NI_event.ch = NI_event.board*MAXCH + NI_event.channel;
			NI_event.length = FebBinEv[rio]->ch[ch].wavelen;
			if (NI_event.length > MAXWAVE) {
				NI_event.length = MAXWAVE;
				cout << "Error, wavelength greater than defined MAXWAVE: " << FebBinEv[rio]->ch[NI_event.channel].wavelen << " > " << MAXWAVE << endl;
				return false;
			}
			std::copy(FebBinEv[rio]->ch[ch].wave.begin(),FebBinEv[rio]->ch[ch].wave.begin()+NI_event.length,NI_event.wave);
			for (int i=0;i<NI_event.length;i++) { //data fix
				if (NI_event.wave[i] > 8192) {
					NI_event.wave[i] -= 16384;
				}
			}
		FillTree();
		}
	}
	return true;
}

bool RawTreeFile::FillJuneEvent(vector<NIJune2015BinFile::JuneBinEv_t*> &JuneBinEv){
	for (int rio=0;rio<JuneBinEv.size();rio++) {
		NI_event.timestamp = JuneBinEv[rio]->timestamp;
		NI_event.board = JuneBinEv[rio]->board;
		NI_event.channel = JuneBinEv[rio]->channel;
		NI_event.ch = NI_event.board*MAXCH + NI_event.channel;
		NI_event.eventID = JuneBinEv[rio]->eventID;
		NI_event.result = JuneBinEv[rio]->result;
		NI_event.length = JuneBinEv[rio]->wave.size();
		if (NI_event.length > MAXWAVE) {
			NI_event.length = MAXWAVE;
			cout << "Error, wavelength greater than defined MAXWAVE: " << JuneBinEv[rio]->wave.size() << " > " << MAXWAVE << endl;
			return false;
		}
		std::copy(JuneBinEv[rio]->wave.begin(),JuneBinEv[rio]->wave.begin()+NI_event.length,NI_event.wave);
		for (int i=0;i<NI_event.length;i++) { //data fix
			if (NI_event.wave[i] > 8192) {
			NI_event.wave[i] -= 16384;
			}
		}
		if (NI_event.result == 1 && NI_event.eventID == 0)
			FillTree();
		else if (NI_event.result == 0) { // DAQ buffer corrupted
			return false;
		}
	}
	return true;
}

bool RawTreeFile::FillMayEvent(vector<NIMay2016BinFile::MayBinEv_t*> &MayBinEv){
	for (int rio=0;rio<MayBinEv.size();rio++) {
		NI_event.timestamp = MayBinEv[rio]->timestamp;
		NI_event.board = MayBinEv[rio]->board;
		NI_event.channel = MayBinEv[rio]->channel;
		NI_event.ch = NI_event.board*MAXCH + NI_event.channel;
		NI_event.eventID = MayBinEv[rio]->eventID;
		NI_event.result = MayBinEv[rio]->result;
		NI_event.length = MayBinEv[rio]->wave.size();
		if (NI_event.length > MAXWAVE) {
			NI_event.length = MAXWAVE;
			cout << "Error, wavelength greater than defined MAXWAVE: " << MayBinEv[rio]->wave.size() << " > " << MAXWAVE << endl;
			return false;
		}
		std::copy(MayBinEv[rio]->wave.begin(),MayBinEv[rio]->wave.begin()+NI_event.length,NI_event.wave);
		for (int i=0;i<NI_event.length;i++) { //data fix
			if (NI_event.wave[i] > 8192) {
			NI_event.wave[i] -= 16384;
			}
		}
		if (NI_event.result == 1 && NI_event.eventID == 0)
			FillTree();
		else if (NI_event.result == 0) { // DAQ buffer corrupted
			return false;
		}
	}
	return true;
}

void RawTreeFile::FillRawEvent(RawEv_t& event){
	NI_event.timestamp = event.timestamp;
	NI_event.board = event.board;
	NI_event.channel = event.channel;
	NI_event.ch = event.ch;
	NI_event.eventID = event.eventID;
	NI_event.result = event.result;
	NI_event.length = event.length;
	for (int i=0;i<event.length;i++)
		NI_event.wave[i] = event.wave[i];
	FillTree();
}

/*************************************************************************/
//                                 Sort
/*************************************************************************/

void RawTreeFile::Sort(TreeFile& origfile){
	Sort(origfile,"timestamp");
}


#endif // RAW_TREE_FILE_CPP__

