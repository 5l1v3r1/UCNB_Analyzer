// File: ROOTTreeFile.cpp
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Handles ROOT TTree file with waveforms
//
// Revision History:
// 2015/5/8:  LJB  Created
// 2015/5/10: LJB  Writes ROOT files with Tree of NI_event
 

#ifndef ROOT_TREE_FILE_CPP__
#define ROOT_TREE_FILE_CPP__

#include "ROOTTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
ROOTTreeFile::ROOTTreeFile() {
  RootFile = 0;
  RootTree = 0;
  createmode = false;
  setup = false;
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
ROOTTreeFile::~ROOTTreeFile() {
  Close();
}

/*************************************************************************/
//                               Close   
/*************************************************************************/
void ROOTTreeFile::Close() {
  if (RootFile != 0) {
    if (RootFile->IsOpen())
      RootFile->Close();
    delete RootFile;
  }
  RootFile = 0;
  RootTree = 0;
  createmode = false;
  setup = false;
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool ROOTTreeFile::Open(std::string path, std::string name){
  Close();
  std::string filename = path;
  filename.append("/");
  filename.append(name);
  if (gSystem->AccessPathName(filename.c_str())) {
    return false;
  }
  RootFile = new TFile(filename.c_str());
  if (!RootFile->IsOpen()) {
    Close();
    return false;
  }
  createmode = false;

  RootTree = (TTree*)RootFile->Get("t");
  RootTree->SetBranchAddress("timestamp",&NI_event.timestamp);
  //How to do this part: read file somehow
  RootTree->GetEntry(0);
  return true;
}

bool ROOTTreeFile::Open(std::string filename){
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Open(path, name);
  }
  else { //no path in filename
    //1. Check local directory
    //2. Check Files/ directory
    //3. Check INPUT_PATH defined in UCNConfig.hh
    //4. Check environmental variables (getenv("WNR_RAW_DATA"))
    std::string name = filename; 
    const int ntrypath = 6;
    std::string trypath[] = {".","file","File","files","Files",
			      INPUT_PATH/*,getenv("WNR_RAW_DATA")*/};
    int tp = 0;
    bool success = false;
    do {
      success = Open(trypath[tp],name);
    }while (++tp<ntrypath && !success);
    return success;
  }
}

bool ROOTTreeFile::Open(int filenum){
  char tempstr[255];
  sprintf(tempstr,"run%05d.root",filenum);
  std::string filename = tempstr;
  return Open(filename);
}

/*************************************************************************/
//                                IsOpen 
/*************************************************************************/
bool ROOTTreeFile::IsOpen() {
  if (RootFile == 0) {
    return false;
  }
  if (!RootFile->IsOpen()) {
    return false;
  }
  if (RootTree == 0) {
    return false;
  }
  return true;
}

#if !defined (__CINT__)
/*************************************************************************/
//                               Create  
/*************************************************************************/
bool ROOTTreeFile::Create(std::string path, std::string name) {
  Close();
  std::string filename = path;
  if (gSystem->AccessPathName(filename.c_str())) {
    return false;
  }
  filename.append("/");
  filename.append(name);
  RootFile = new TFile(filename.c_str(),"RECREATE");
  if (!RootFile->IsOpen()) {
    cout << "Failed to create ROOT TFile" << endl;
    Close();
    return false;
  }

  createmode = true;
  return true;
}

bool ROOTTreeFile::Create(std::string filename) {
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Create(path, name);
  }
  else { //no path in filename
    //1. Use ROOT_PATH defined in LocalCFG.hh
    //2. Use environmental variables (getenv("WNR_RAW_DATA"))
    //3. Use Files/ directory
    //4. Use local directory
    std::string name = filename; 
    const int ntrypath = 6;
    std::string trypath[] = {ROOT_PATH,/*getenv("WNR_RAW_DATA"),*/
			     "file","File","files","Files","."};
    int tp = 0;
    bool success = false;
    do {
      success = Create(trypath[tp],name);
    }while (++tp<ntrypath && !success);
    return success;
  }
}

bool ROOTTreeFile::Create(int filenum){
  char tempstr[255];
  sprintf(tempstr,"run%05d.root",filenum);
  std::string filename = tempstr;
  return Create(filename);
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                              CalcMax  
/*************************************************************************/
/*
UShort_t ROOTTreeFile::CalcMax(std::vector<Int_t> data) {
  Int_t max = 0;
  for (int i=0;i<data.size();i++) {
    if (max < data[i])
      max = data[i];
  }
  return static_cast<UShort_t>(max);
}
*/

/*************************************************************************/
//                              CalcMin  
/*************************************************************************/
/*
UShort_t ROOTTreeFile::CalcMin(std::vector<Int_t> data) {
  Int_t min = pow(2,12)-1;
  for (int i=0;i<data.size();i++) {
    if (min > data[i])
      min = data[i];
  }
  return static_cast<UShort_t>(min);
}
*/

/*************************************************************************/
//                              CalcPed
/*************************************************************************/
/*
UShort_t ROOTTreeFile::CalcPed(std::vector<Int_t> data) {
  if (data.size() < 15)
    return 0;
  Int_t ped = 0;
  for (int i=0;i<data.size() && i<15;i++) {
    ped += data[i];
  }
  ped = ped/15.;
  return static_cast<UShort_t>(ped);
}
*/

/*************************************************************************/
//                             CalcZero
/*************************************************************************/
/*
UShort_t ROOTTreeFile::CalcZero(std::vector<Int_t> data) {
  if (data.size() == 0)
    return 1;
  Int_t zero = 0;
  for (int i=0;(i*4+3<data.size()) && i<6;i++) {
    zero += data[i*4] + data[i*4 + 1] + data[i*4 + 2] + data[i*4 + 3];
  }
  return static_cast<UShort_t>(zero/data.size());
}
*/


/*************************************************************************/
//                             SetupTree 
/*************************************************************************/
void ROOTTreeFile::SetupTree(int numch){
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
  }
  setup = true;
}

/*************************************************************************/
//                              FillTree 
/*************************************************************************/
void ROOTTreeFile::FillTree(){
  if (!RootFile->IsOpen()) {
    std::cout << "File not open" << std::endl;
    return;
  }
  if (RootTree == 0) {
    std::cout << "Tree does not exist" << std::endl;
    return;    
  }
  if (!createmode) {
    std::cout << "Create new file first" << std::endl;
    return;    
  }
  if (!setup) {
    std::cout << "Set up tree first" << std::endl;
    return;
  }
  RootFile->cd();
  RootTree->Fill();
}

/*************************************************************************/
//                                Write  
/*************************************************************************/
void ROOTTreeFile::Write(){
  if (!RootFile->IsOpen()) {
    std::cout << "File not open" << std::endl;
    return;
  }
  if (!createmode) {
    std::cout << "Create new file first" << std::endl;
    return;    
  }
  if (!setup) {
    std::cout << "Set up tree first" << std::endl;
    return;
  }
  RootFile->cd();
  RootFile->Write();
  Close();
}


#endif // ROOT_TREE_FILE_CPP__

