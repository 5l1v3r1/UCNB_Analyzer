// File: EventTreeFile.cpp
// Name: Leah Broussard
// Date: 2015/5/29
// Purpose: Handles ROOT TTree file with events E[numch],t
//
// Revision History:
// 2015/5/29:  LJB  Created
 

#ifndef EVENT_TREE_FILE_CPP__
#define EVENT_TREE_FILE_CPP__
#include "EventTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
EventTreeFile::EventTreeFile() {
  RootFile = 0;
  RootTree = 0;
  createmode = false;
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
EventTreeFile::~EventTreeFile() {
  Close();
}

/*************************************************************************/
//                               Close   
/*************************************************************************/
void EventTreeFile::Close() {
  if (RootFile != 0) {
    if (RootFile->IsOpen())
      RootFile->Close();
    delete RootFile;
  }
  RootFile = 0;
  RootTree = 0;
  createmode = false;
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool EventTreeFile::Open(std::string path, std::string name){
  Close();
  std::string filename = path;
  filename.append("/");
  filename.append(name);
  if (gSystem->AccessPathName(filename.c_str())) {
    return false;
  }
  RootFile = new TFile(filename.c_str());
  if (!RootFile->IsOpen()){ 
    Close();
    return false;
  }
  createmode = false;

  RootTree = (TTree*)RootFile->Get("t");
  RootTree->SetBranchAddress("numch",&myEvent.numch);
  RootTree->SetBranchAddress("E",&myEvent.E[0]);
  RootTree->SetBranchAddress("t",&myEvent.t);

  RootTree->GetEntry(0);
  return true;
}

bool EventTreeFile::Open(std::string filename){
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Open(path, name);
  }
  else { //no path in filename
    //1. Check local directory
    //2. Check Files/ directory
    //3. Check EVENT_PATH defined in LocalCFG.hh
    //4. Check environmental variables (e.g. getenv("RAW_DATA_DIR"))
    std::string name = filename; 
    const int ntrypath = 6;
    std::string trypath[] = {".","file","File","files","Files",
			      EVENT_PATH/*,getenv("RAW_DATA_DIR")*/};
    int tp = 0;
    bool success = false;
    do {
      success = Open(trypath[tp],name);
    }while (++tp<ntrypath && !success);
    return success;
  }
}

bool EventTreeFile::Open(int filenum){
  char tempstr[255];
  sprintf(tempstr,"trig%05d.root",filenum);
  std::string filename = tempstr;
  return Open(filename);
}

/*************************************************************************/
//                                IsOpen 
/*************************************************************************/
bool EventTreeFile::IsOpen() {
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
bool EventTreeFile::Create(std::string path, std::string name) {
  Close();
  std::string filename = path;
  if (gSystem->AccessPathName(filename.c_str())) {
    return false;
  }
  filename.append("/");
  filename.append(name);
  RootFile = new TFile(filename.c_str(),"RECREATE");
  if (!RootFile->IsOpen()) {
    cout << "Failed to create Event TFile" << endl;
    Close();
    return false;
  }

  RootTree = new TTree("t","t");
  RootTree->Branch("numch",&myEvent.E,"numch/I");
  RootTree->Branch("E",&myEvent.E[0],"E[numch]/D");
  RootTree->Branch("t",&myEvent.t,"t/D");

  createmode = true;
  return true;
}

bool EventTreeFile::Create(std::string filename) {
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Create(path, name);
  }
  else { //no path in filename
    //1. Use EVENT_PATH defined in LocalCFG.hh
    //2. Use environmental variables (e.g. getenv("RAW_DATA_DIR"))
    //3. Use Files/ directory
    //4. Use local directory
    std::string name = filename; 
    const int ntrypath = 6;
    std::string trypath[] = {EVENT_PATH,/*getenv("RAW_DATA_DIR"),*/
			     "file","File","files","Files","."};
    int tp = 0;
    bool success = false;
    do {
      success = Create(trypath[tp],name);
    }while (++tp<ntrypath && !success);

    return success;
  }
}

bool EventTreeFile::Create(int filenum){
  char tempstr[255];
  sprintf(tempstr,"ev%05d.root",filenum);
  std::string filename = tempstr;
  return Create(filename);
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                              FillTree 
/*************************************************************************/
void EventTreeFile::FillTree(){
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
  RootFile->cd();
  RootTree->Fill();
}

/*************************************************************************/
//                                Write  
/*************************************************************************/
void EventTreeFile::Write(){
  if (!RootFile->IsOpen()) {
    std::cout << "File not open" << std::endl;
    return;
  }
  if (!createmode) {
    std::cout << "Create new file first" << std::endl;
    return;    
  }
  RootFile->cd();
  RootFile->Write();
  Close();
}


/*************************************************************************/
//                               GetEvent  
/*************************************************************************/
void EventTreeFile::GetEvent(Int_t ev){
  if (!RootFile->IsOpen()) {
    std::cout << "File not open" << std::endl;
    return;
  }
  RootTree->GetEntry(ev);
}

#endif // EVENT_TREE_FILE_CPP__

