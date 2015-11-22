// File: TrapTreeFile.cpp
// Name: Leah Broussard
// Date: 2015/5/14
// Purpose: Handles ROOT TTree file with E,t from linear trap filter
//
// Revision History:
// 2015/5/14:  LJB  Created
 

#ifndef TRAP_TREE_FILE_CPP__
#define TRAP_TREE_FILE_CPP__

#include "TrapTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TrapTreeFile::TrapTreeFile() {
  RootFile = 0;
  RootTree = 0;
  createmode = false;
  mypath = "";
  pathset = false;
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
TrapTreeFile::~TrapTreeFile() {
  Close();
}

/*************************************************************************/
//                               Close   
/*************************************************************************/
void TrapTreeFile::Close() {
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
bool TrapTreeFile::Open(std::string path, std::string name){
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
  if (RootTree == 0) {
    cout << "Missing TTree??" << endl;
    return false;
  }

  RootTree->SetBranchAddress("MaxE",&Trap_event.MaxE);
  RootTree->SetBranchAddress("AveE",&Trap_event.AveE);
  RootTree->SetBranchAddress("MidE",&Trap_event.MidE);
  RootTree->SetBranchAddress("t",&Trap_event.t);
  RootTree->SetBranchAddress("Flat0",&Trap_event.Flat0);
  RootTree->SetBranchAddress("Flat1",&Trap_event.Flat1);
  RootTree->SetBranchAddress("up",&Trap_event.up);
  RootTree->SetBranchAddress("down",&Trap_event.down);
  RootTree->SetBranchAddress("ch",&Trap_event.ch);

  RootTree->GetEntry(0);
  return true;
}

bool TrapTreeFile::Open(std::string filename){
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Open(path, name);
  }
  if (pathset) {
    Open(mypath,filename);
  }
  else { //no path in filename
    //1. Check local directory
    //2. Check Files/ directory
    //3. Check TRIG_PATH defined in LocalCFG.hh
    //4. Check environmental variables (e.g. getenv("RAW_DATA_DIR"))
    std::string name = filename; 
    const int ntrypath = 6;
    std::string trypath[] = {".","file","File","files","Files",
			      TRIG_PATH/*,getenv("RAW_DATA_DIR")*/};
    int tp = 0;
    bool success = false;
    do {
      success = Open(trypath[tp],name);
    }while (++tp<ntrypath && !success);
    return success;
  }
}

bool TrapTreeFile::Open(int filenum, int decay, int shape, int top) {
  char tempstr[255];
  sprintf(tempstr,"trap%05d_decay%05d_shape%05d_top%05d.root",filenum, decay, shape, top);
  std::string filename = tempstr;
  return Open(filename);
}

/*************************************************************************/
//                                IsOpen 
/*************************************************************************/
bool TrapTreeFile::IsOpen() {
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
bool TrapTreeFile::Create(std::string path, std::string name) {
  Close();
  std::string filename = path;
  if (gSystem->AccessPathName(filename.c_str())) {
    return false;
  }
  filename.append("/");
  filename.append(name);
  RootFile = new TFile(filename.c_str(),"RECREATE");
  if (!RootFile->IsOpen()) {
    cout << "Failed to create Trap TFile" << endl;
    Close();
    return false;
  }

  RootTree = new TTree("t","t");
  RootTree->Branch("MaxE",&Trap_event.MaxE,"MaxE/D");
  RootTree->Branch("AveE",&Trap_event.AveE,"AveE/D");
  RootTree->Branch("MidE",&Trap_event.MidE,"MidE/D");
  RootTree->Branch("t",&Trap_event.t,"t/D");
  RootTree->Branch("Flat0",&Trap_event.Flat0,"Flat0/D");
  RootTree->Branch("Flat1",&Trap_event.Flat1,"Flat1/D");
  RootTree->Branch("up",&Trap_event.up,"up/I");
  RootTree->Branch("down",&Trap_event.down,"down/I");
  RootTree->Branch("ch",&Trap_event.ch,"ch/I");


  createmode = true;
  return true;
}

bool TrapTreeFile::Create(std::string filename) {
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Create(path, name);
  }
  if (pathset) {
    Create(mypath,filename);
  }
  else { //no path in filename
    //1. Use TRIG_PATH defined in LocalCFG.hh
    //2. Use environmental variables (e.g. getenv("RAW_DATA_DIR"))
    //3. Use Files/ directory
    //4. Use local directory
    std::string name = filename; 
    const int ntrypath = 6;
    std::string trypath[] = {TRIG_PATH,/*getenv("RAW_DATA_DIR"),*/
			     "file","File","files","Files","."};
    int tp = 0;
    bool success = false;
    do {
      success = Create(trypath[tp],name);
    }while (++tp<ntrypath && !success);

    return success;
  }
}

bool TrapTreeFile::Create(int filenum, int decay, int shape, int top) {
  char tempstr[255];
  sprintf(tempstr,"trap%05d_decay%05d_shape%05d_top%05d.root",filenum, decay, shape, top);
  std::string filename = tempstr;
  return Create(filename);
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                              FillTree 
/*************************************************************************/
void TrapTreeFile::FillTree(){
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
  //cout << "filling " << Trap_event.up << " and " << Trap_event.down << endl;
  RootFile->cd();
  RootTree->Fill();
}

/*************************************************************************/
//                                Write  
/*************************************************************************/
void TrapTreeFile::Write(){
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
void TrapTreeFile::GetEvent(Int_t ev){
  if (!RootFile->IsOpen()) {
    std::cout << "File not open" << std::endl;
    return;
  }
  RootTree->GetEntry(ev);
}

#endif // TRAP_TREE_FILE_CPP__

