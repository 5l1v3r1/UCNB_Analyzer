// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: TreeFile.cpp
// Purpose: Handles ROOT TTree files
 

#ifndef TREE_FILE_CPP__
#define TREE_FILE_CPP__

#include "TreeFile.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
TreeFile::TreeFile() {
  RootFile = 0;
  RootTree = 0;
  createmode = false;
  mypath = "";
  pathset = false;
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
TreeFile::~TreeFile() {
  Close();
}

/*************************************************************************/
//                               Close   
/*************************************************************************/
void TreeFile::Close() {
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
bool TreeFile::Open(std::string path, std::string name){
  Close();
  std::string filename = path;
  filename.append("/");
  filename.append(name);
  if (gSystem->AccessPathName(filename.c_str())) {
    //cout << filename << " not found " << endl;
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
	cout << "Bad TTree?" << endl;
    Close();
    return false;
  }
  SetBranches();
  return true;
}

bool TreeFile::Open(std::string filename){
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Open(path, name);
  }
  if (pathset) {
    return Open(mypath,filename);
  }
  else { //no path in filename
    //1. Check local directory
    //2. Check variants on Files/ directory
    std::string name = filename; 
    const int ntrypath = 5;
    std::string trypath[] = {".","file","File","files","Files"};
    int tp = 0;
    bool success = false;
    do {
      success = Open(trypath[tp],name);
    }while (++tp<ntrypath && !success);
    return success;
  }
}

bool TreeFile::Open(int filenum){
  char tempstr[255];
  sprintf(tempstr,namestr,filenum);
  std::string filename = tempstr;
  if (pathset)
    return Open(mypath,filename);
  else
    return Open(filename);
}


/*************************************************************************/
//                                IsOpen 
/*************************************************************************/
bool TreeFile::IsOpen() {
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
bool TreeFile::Create(std::string path, std::string name) {
	Close();
	std::string filename = path;
	if (gSystem->AccessPathName(filename.c_str())) {
		cout << "Path " << filename.c_str() << " not found" << endl;
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
	std::cout << "Writing to " << filename << endl;

	RootTree = new TTree("t","t");
	if (RootTree == 0) {
		cout << "Failed to create TTree?" << endl;
		Close();
		return false;
	}
    MakeBranches();
	createmode = true;
	return true;
}

bool TreeFile::Create(std::string filename) {
	std::size_t pos = filename.rfind("/");
	if (pos!=std::string::npos) {
		std::string path = filename.substr(0,pos+1);
		std::string name = filename.substr(pos+1,filename.length());
		return Create(path, name);
	}
	if (pathset) {
		return Create(mypath,filename);
	}
	else { //no path in filename
		//1. Check local directory
		//2. Check variants on Files/ directory
		std::string name = filename; 
		const int ntrypath = 5;
		std::string trypath[] = {".","file","File","files","Files"};
		int tp = 0;
		bool success = false;
		do {
			success = Create(trypath[tp],name);
		}while (++tp<ntrypath && !success);
		return success;
	}
}

bool TreeFile::Create(int filenum){
  char tempstr[255];
  sprintf(tempstr,namestr,filenum);
  std::string filename = tempstr;
  if (pathset)
    return Create(mypath,filename);
  else
	return Create(filename);
}

#endif // !defined (__CINT__)

/*************************************************************************/
//                              FillTree 
/*************************************************************************/
void TreeFile::FillTree(){
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
void TreeFile::Write(){
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
void TreeFile::GetEvent(Int_t ev){
  if (!RootFile->IsOpen()) {
    std::cout << "File not open" << std::endl;
    return;
  }
  RootTree->GetEntry(ev);
}

#endif // TREE_FILE_CPP__

