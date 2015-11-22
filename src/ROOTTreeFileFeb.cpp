// File: ROOTTreeFileFeb.cpp
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Handles ROOT TTree file with waveforms
//
// Revision History:
// 2015/5/8:  LJB  Created
// 2015/5/11: LJB  Read/write ROOT files with Tree of NI_event
// 2015/7/16: LJB  Specialized to February data format
 

#ifndef ROOT_TREE_FILE_FEB_CPP__
#define ROOT_TREE_FILE_FEB_CPP__

#include "ROOTTreeFileFeb.hh"
#include "TObjArray.h"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
ROOTTreeFileFeb::ROOTTreeFileFeb() {
  //  TreeFile();
  setup = false;
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
ROOTTreeFileFeb::~ROOTTreeFileFeb() {
  Close();
}

/*************************************************************************/
//                               Close   
/*************************************************************************/
void ROOTTreeFileFeb::Close() {
  //  TreeFile::Close();
  //Call TreeFile Close()
  setup = false;
}

/*************************************************************************/
//                             SetBranches 
/*************************************************************************/
void ROOTTreeFileFeb::SetBranches() {
  //Hack--Get numch from last branch
  TObjArray *BranchList = (TObjArray*)RootTree->GetListOfBranches()->Clone();
  BranchList->SetOwner(kFALSE);
  TString lastbranch = BranchList->At(BranchList->GetEntries()-1)->GetName();
  lastbranch.Remove(0,1);
  NI_event.numch = lastbranch.Atoi() + 1;
  RootTree->SetBranchAddress("timestamp",&NI_event.timestamp);
  for (int ch=0;ch<NI_event.numch;ch++) {
    TString wavelen = "wavelen";
    wavelen += ch;
    RootTree->SetBranchAddress(wavelen,&NI_event.ch[ch].wavelen);
    TString wave = "wave";
    wave += ch;
    RootTree->SetBranchAddress(wave,&NI_event.ch[ch].wave[0]);
    TString rio = "rio";
    rio += ch;
    RootTree->SetBranchAddress(rio,&NI_event.ch[ch].rio);
    TString chan = "ch";
    chan += ch;
    RootTree->SetBranchAddress(chan,&NI_event.ch[ch].ch);
    TString Edaq = "E";
    Edaq += ch;
    RootTree->SetBranchAddress(Edaq,&NI_event.ch[ch].Edaq);
  }
  RootTree->GetEntry(0);
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool ROOTTreeFileFeb::Open(std::string path, std::string name){
  if (!TreeFile::Open(path,name))
    return false;
  SetBranches(); // can parent function call child inherited version? LJB
  return true;
}

bool ROOTTreeFileFeb::Open(std::string filename){
  if (!TreeFile::Open(filename))
    return false;
  SetBranches();
  return true;
}

bool ROOTTreeFileFeb::Open(int filenum){
  char tempstr[255];
  sprintf(tempstr,"run%05d.root",filenum);
  std::string filename = tempstr;
  return Open(filename);
}

#if !defined (__CINT__)

bool ROOTTreeFileFeb::Create(int filenum){
  char tempstr[255];
  sprintf(tempstr,"run%05d.root",filenum);
  std::string filename = tempstr;
  return TreeFile::Create(filename);
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                             SetupTree 
/*************************************************************************/
void ROOTTreeFileFeb::SetupTree(int numch){
  setup = false;
  if (!RootFile->IsOpen()) {
    std::cout << "File not open" << std::endl;
    return;
  }
  if (RootTree != 0) {
    delete RootTree;
  }
  if (!createmode) {
    std::cout << "Create new file first" << std::endl;
    return;    
  }
  NI_event.numch = numch;
  RootTree = new TTree("t","t");
  RootTree->Branch("timestamp",&NI_event.timestamp,"timestamp/L");
  for (int ch=0;ch<numch;ch++) {
    TString wavelen = "wavelen";
    wavelen += ch;
    TString wavelenstr = wavelen;
    wavelenstr += "/I";
    RootTree->Branch(wavelen,&NI_event.ch[ch].wavelen,wavelenstr);
    TString wave = "wave";
    wave += ch;
    TString wavestr = wave;
    wavestr += "[";
    wavestr += wavelen;
    wavestr += "]/S";
    RootTree->Branch(wave,&NI_event.ch[ch].wave[0],wavestr);
    TString rio = "rio";
    rio += ch;
    TString riostr = rio;
    riostr += "/S";
    RootTree->Branch(rio,&NI_event.ch[ch].rio,riostr);
    TString chan = "ch";
    chan += ch;
    TString chanstr = chan;
    chanstr += "/S";
    RootTree->Branch(chan,&NI_event.ch[ch].ch,chanstr);
    TString Edaq = "E";
    Edaq += ch;
    TString Edaqstr = Edaq;
    Edaqstr += "/S";
    RootTree->Branch(Edaq,&NI_event.ch[ch].Edaq,Edaqstr);
  }
  setup = true;
}

/*************************************************************************/
//                              FillTree 
/*************************************************************************/
void ROOTTreeFileFeb::FillTree(){
  if (!setup) {
    std::cout << "Set up tree first" << std::endl;
    return;
  }
  TreeFile::FillTree();
}

/*************************************************************************/
//                                Write  
/*************************************************************************/
void ROOTTreeFileFeb::Write(){
  if (!setup) {
    std::cout << "Set up tree first" << std::endl;
    return;
  }
  TreeFile::Write();
}


/*************************************************************************/
//                             FillEvent
/*************************************************************************/
void ROOTTreeFileFeb::FillEvent(BinFile::BinEv_t& BinEv){
  NIFeb2015BinFile::FebBinEv_t* FebBinEv = dynamic_cast<NIFeb2015BinFile::FebBinEv_t*>(&BinEv);
  if (!FebBinEv) {
    cout << "ROOTTreeFileFeb::FillEvent requires FebBinEv_t types" << endl;
    return;
  }
  NI_event.timestamp = FebBinEv->timestamp;
  NI_event.numch = FebBinEv->ch.size();
  for (int ch=0;ch<numch;ch++) {
    NI_event.ch[ch].wavelen = FebBinEv->ch[ch].wavelen;
    if (NI_event.ch[ch].wavelen > MAXWAVE) {
      NI_event.ch[ch].wavelen = MAXWAVE;
      cout << "Error, wavelength greater than defined MAXWAVE: " << FebBinEv->wave.size() << " > " << MAXWAVE << endl;
    }
    std::copy(FebBinEv->wave.begin(),FebBinEv->ch[ch].wave.begin()+NI_event.ch[ch].wavelen,NI_event.ch[ch].wave);
    NI_event.ch[ch].Edaq = FebBinEv->ch[ch].Edaq;
    //keep?
    NI_event.ch[ch].rio = -1;
    NI_event.ch[ch].ch = -1;
  }
  


  /*
  Int_t wavelen, numch;
  for (int rio=0;rio<NRIO;rio++) {
    streampos pos = RIO[rio].fFileStream.tellg();
    if (pos >= RIO[rio].filelength) {
      return false;
    }
    RIO[rio].fFileStream.read(reinterpret_cast<char *>(&ts[rio]),sizeof(Long64_t));
    RIO[rio].fFileStream.read(reinterpret_cast<char *>(&numch),sizeof(Int_t));
    if (numch != RIO[rio].numch) {
      cout << "numch change for board " << rio << endl;
      return false;
    }
    NI_event.numch += numch;
    for (int ch=0;ch<RIO[rio].numch;ch++) {
      int thech = rio*RIO[rio].numch + ch;
      NI_event.ch[thech].rio = rio;
      NI_event.ch[thech].ch = ch;
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&NI_event.ch[thech].Edaq),sizeof(Short_t));
    }
    RIO[rio].fFileStream.read(reinterpret_cast<char *>(&wavelen),sizeof(Int_t));
    wavelen = wavelen/8.;
    if (wavelen != RIO[rio].wavelen) {
      cout << "wavelen change for board " << rio << endl;
      cout << wavelen << " != " << RIO[rio].wavelen << endl;
      return false;
    }
    for (int ch=0;ch<RIO[rio].numch;ch++) {
      int thech = rio*RIO[rio].numch + ch;
      NI_event.ch[thech].wavelen = wavelen;
      if (wavelen > MAXWAVE) {
	cout << "error, wavelen > MAXWAVE" << endl;
	cout << wavelen << " > " << MAXWAVE << endl;
	return false;
      }
      //NI_event.ch[thech].wave.resize(wavelen);
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&NI_event.ch[thech].wave[0]),wavelen*sizeof(Short_t));
    }
  }
  //For now, assume timestamps will match
  for (int rio=0;rio<NRIO-1;rio++) {
    if (rio < NRIO-1) {
      if (ts[rio] != ts[rio+1]) {
	cout << "Timestamp mismatch, " << ts[rio] << " != " << ts[rio+1] << endl;
	return false;
      }
    }
  }
  NI_event.timestamp = ts[0];
*/
}

#endif // ROOT_TREE_FILE_FEB_CPP__

