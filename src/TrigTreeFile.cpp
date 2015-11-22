// File: TrigTreeFile.cpp
// Name: Leah Broussard
// Date: 2015/5/14
// Purpose: Handles ROOT TTree file with E,t
//
// Revision History:
// 2015/5/14:  LJB  Created
 

#ifndef TRIG_TREE_FILE_CPP__
#define TRIG_TREE_FILE_CPP__

#include "TrigTreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TrigTreeFile::TrigTreeFile() {
  RootFile = 0;
  RootTree = 0;
  createmode = false;
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
TrigTreeFile::~TrigTreeFile() {
  Close();
}

/*************************************************************************/
//                               Close   
/*************************************************************************/
void TrigTreeFile::Close() {
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
bool TrigTreeFile::Open(std::string path, std::string name){
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

  RootTree->SetBranchAddress("E",&Trig_event.E);
  RootTree->SetBranchAddress("t",&Trig_event.t);
  RootTree->SetBranchAddress("shaping",&Trig_event.shaping);
  RootTree->SetBranchAddress("integ",&Trig_event.integ);
  RootTree->SetBranchAddress("trapE",&Trig_event.trapE);
  RootTree->SetBranchAddress("trapT",&Trig_event.trapT);
  RootTree->SetBranchAddress("chi2",&Trig_event.chi2);
  RootTree->SetBranchAddress("rio",&Trig_event.rio);
  RootTree->SetBranchAddress("rio_ch",&Trig_event.rio_ch);
  RootTree->SetBranchAddress("chan",&Trig_event.chan);
  RootTree->SetBranchAddress("waveev",&Trig_event.waveev);

  RootTree->GetEntry(0);
  return true;
}

bool TrigTreeFile::Open(std::string filename){
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Open(path, name);
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

bool TrigTreeFile::Open(int filenum){
  char tempstr[255];
  sprintf(tempstr,"trig%05d.root",filenum);
  std::string filename = tempstr;
  return Open(filename);
}

/*************************************************************************/
//                                IsOpen 
/*************************************************************************/
bool TrigTreeFile::IsOpen() {
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
bool TrigTreeFile::Create(std::string path, std::string name) {
  Close();
  std::string filename = path;
  if (gSystem->AccessPathName(filename.c_str())) {
    return false;
  }
  filename.append("/");
  filename.append(name);
  RootFile = new TFile(filename.c_str(),"RECREATE");
  if (!RootFile->IsOpen()) {
    cout << "Failed to create Trig TFile" << endl;
    Close();
    return false;
  }

  RootTree = new TTree("t","t");
  RootTree->Branch("E",&Trig_event.E,"E/D");
  RootTree->Branch("t",&Trig_event.t,"t/D");
  RootTree->Branch("shaping",&Trig_event.shaping,"shaping/D");
  RootTree->Branch("integ",&Trig_event.integ,"integ/D");
  RootTree->Branch("chi2",&Trig_event.chi2,"chi2/D");
  RootTree->Branch("trapE",&Trig_event.trapE,"trapE/D");
  RootTree->Branch("trapT",&Trig_event.trapT,"trapT/D");
  RootTree->Branch("rio",&Trig_event.rio,"rio/I");
  RootTree->Branch("rio_ch",&Trig_event.rio_ch,"rio_ch/I");
  RootTree->Branch("chan",&Trig_event.chan,"chan/I");
  RootTree->Branch("waveev",&Trig_event.waveev,"waveev/I");

  createmode = true;
  return true;
}

bool TrigTreeFile::Create(std::string filename) {
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Create(path, name);
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

bool TrigTreeFile::Create(int filenum){
  char tempstr[255];
  sprintf(tempstr,"trig%05d.root",filenum);
  std::string filename = tempstr;
  return Create(filename);
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                              FillTree 
/*************************************************************************/
void TrigTreeFile::FillTree(){
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
void TrigTreeFile::Write(){
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
void TrigTreeFile::GetEvent(Int_t ev){
  if (!RootFile->IsOpen()) {
    std::cout << "File not open" << std::endl;
    return;
  }
  RootTree->GetEntry(ev);
}

#endif // TRIG_TREE_FILE_CPP__

