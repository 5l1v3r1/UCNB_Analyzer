// File: ROOTTreeFileJune.cpp
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Handles ROOT TTree file with waveforms
//
// Revision History:
// 2015/5/8:  LJB  Created
// 2015/5/11: LJB  Read/write ROOT files with Tree of NI_event
// 2015/7/16: LJB  Specialized to June data format
 

#ifndef ROOT_TREE_FILE_JUNE_CPP__
#define ROOT_TREE_FILE_JUNE_CPP__

#include "ROOTTreeFileJune.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
ROOTTreeFileJune::ROOTTreeFileJune() {
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
ROOTTreeFileJune::~ROOTTreeFileJune() {
}

/*************************************************************************/
//                             SetBranches 
/*************************************************************************/
void ROOTTreeFileJune::SetBranches() {
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

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool ROOTTreeFileJune::Open(std::string path, std::string name){
  if (!TreeFile::Open(path,name))
    return false;
  SetBranches();
  return true;
}

bool ROOTTreeFileJune::Open(std::string filename){
  if (!TreeFile::Open(filename))
    return false;
  SetBranches();
  return true;
}

bool ROOTTreeFileJune::Open(int filenum){
  char tempstr[255];
  sprintf(tempstr,"run%05d.root",filenum);
  std::string filename = tempstr;
  if (pathset)
    return Open(mypath,filename);
  else
    return Open(filename);
}

#if !defined (__CINT__)

/*************************************************************************/
//                             MakeBranches 
/*************************************************************************/
void ROOTTreeFileJune::MakeBranches() {
  RootTree->Branch("timestamp",&NI_event.timestamp,"timestamp/l");
  RootTree->Branch("board",&NI_event.board,"board/I");
  RootTree->Branch("channel",&NI_event.channel,"channel/I");
  RootTree->Branch("ch",&NI_event.ch,"ch/I");
  RootTree->Branch("eventID",&NI_event.eventID,"eventID/I");
  RootTree->Branch("result",&NI_event.result,"result/I");
  RootTree->Branch("length",&NI_event.length,"length/I");
  RootTree->Branch("wave",&NI_event.wave[0],"wave[length]/S");
}

/*************************************************************************/
//                               Create  
/*************************************************************************/
bool ROOTTreeFileJune::Create(std::string path, std::string name) {
  if (!TreeFile::Create(path,name))
    return false;
  MakeBranches();
  return true;
}

bool ROOTTreeFileJune::Create(std::string filename) {
  if (!TreeFile::Create(filename))
    return false;
  MakeBranches();
  return true;
}

bool ROOTTreeFileJune::Create(int filenum){
  char tempstr[255];
  sprintf(tempstr,"run%05d.root",filenum);
  std::string filename = tempstr;
  return Create(filename);
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                             FillEvent
/*************************************************************************/
void ROOTTreeFileJune::FillEvent(BinFile::BinEv_t& BinEv){
  NIJune2015BinFile::JuneBinEv_t* JuneBinEv = dynamic_cast<NIJune2015BinFile::JuneBinEv_t*>(&BinEv);
  if (!JuneBinEv) {
    cout << "ROOTTreeFileJune::FillEvent requires JuneBinEv_t types" << endl;
    return;
  }

  NI_event.timestamp = JuneBinEv->timestamp;
  NI_event.board = JuneBinEv->board;
  NI_event.channel = JuneBinEv->channel;
  NI_event.ch = NI_event.board*MAXCH + NI_event.channel;
  NI_event.eventID = JuneBinEv->eventID;
  NI_event.result = JuneBinEv->result;
  NI_event.length = JuneBinEv->wave.size();
  if (NI_event.length > MAXWAVE) {
    NI_event.length = MAXWAVE;
    cout << "Error, wavelength greater than defined MAXWAVE: " << JuneBinEv->wave.size() << " > " << MAXWAVE << endl;
  }
  std::copy(JuneBinEv->wave.begin(),JuneBinEv->wave.begin()+NI_event.length,NI_event.wave);
  for (int i=0;i<NI_event.length;i++) { //data fix
    if (NI_event.wave[i] > 8192) {
      NI_event.wave[i] -= 16384;
    }
  }
}

#endif // ROOT_TREE_FILE_JUNE_CPP__

