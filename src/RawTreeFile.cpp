// File: RawTreeFile.cpp
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Handles ROOT TTree file with waveforms
//
// Revision History:
// 2015/5/8:   LJB  Created
// 2015/5/11:  LJB  Read/write ROOT files with Tree of NI_event
// 2015/7/16:  LJB  Specialized to June data format
// 2015/11/20: LJB  Standardized all formats to June format
// 2015/1/2:   LJB  Added event copy routine
 

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

void RawTreeFile::FillEvent(vector<BinFile::BinEv_t*> &BinEv) {
	if (BinEv.size() == 0) return;
	
	vector<NIFeb2015BinFile::FebBinEv_t*> FebBinEv;
	vector<NIJune2015BinFile::JuneBinEv_t*> JuneBinEv;
	vector<BinFile::BinEv_t*>::iterator iter;
	for (iter = BinEv.begin(); iter != BinEv.end(); ++iter) {
		FebBinEv.push_back(dynamic_cast<NIFeb2015BinFile::FebBinEv_t*>(*iter));
		JuneBinEv.push_back(dynamic_cast<NIJune2015BinFile::JuneBinEv_t*>(*iter));
	}
	if (FebBinEv[0]) {
		FillFebEvent(FebBinEv);
	}
	else if (JuneBinEv[0]) {
		FillJuneEvent(JuneBinEv);		
	}
	else return;
}

  
void RawTreeFile::FillFebEvent(vector<NIFeb2015BinFile::FebBinEv_t*> &FebBinEv){
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
}

void RawTreeFile::FillJuneEvent(vector<NIJune2015BinFile::JuneBinEv_t*> &JuneBinEv){
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
		}
		std::copy(JuneBinEv[rio]->wave.begin(),JuneBinEv[rio]->wave.begin()+NI_event.length,NI_event.wave);
		for (int i=0;i<NI_event.length;i++) { //data fix
			if (NI_event.wave[i] > 8192) {
			NI_event.wave[i] -= 16384;
			}
		}
		FillTree();
	}
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
#endif // RAW_TREE_FILE_CPP__

