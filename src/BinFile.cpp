// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: BinFile.cpp
// Purpose: Opens binary files

#ifndef BIN_FILE_CPP__
#define BIN_FILE_CPP__

#include "BinFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
BinFile::BinFile() {
  mypath = "";
  pathset = false;
}

BinFile::BinFile(std::string path, std::string name) {
  mypath = "";
  pathset = false;
  Open(path,name);
}

BinFile::BinFile(std::string filename) {
  mypath = "";
  pathset = false;
  Open(filename);
}

/*************************************************************************/
//                             Destructor
/*************************************************************************/
BinFile::~BinFile() {
  Close();
}

/*************************************************************************/
//                                Open
/*************************************************************************/

bool BinFile::Open(const char* path, const char* name) {
  std::string pth = path;
  std::string nm = name;
  return Open(pth,nm);
}

bool BinFile::Open(std::string path, std::string name) {
  std::string filename = path;
  filename.append("/");
  filename.append(name);
  fFileStream.open(filename.c_str(),std::ios::binary);
  if (fFileStream.is_open()) {
    fFilePath = path;
    fFileName = name;
    fFileStream.seekg(0,fFileStream.end);
    fFileLength = fFileStream.tellg();
    fFileStream.seekg(0,fFileStream.beg);
  }
  else {
    fFilePath = "";
    fFileName = "";
    fFileLength = 0;
  }
  return fFileStream.is_open();
}

bool BinFile::Open(std::string filename) {
  std::size_t slash = filename.rfind("/");
  if (slash!=std::string::npos) {
    std::string path = filename.substr(0,slash+1);
    std::string name = filename.substr(slash+1,filename.length());
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

/*************************************************************************/
//                               Close
/*************************************************************************/
void BinFile::Close() {
  if (fFileStream.is_open()) {
    fFileName = "";
    fFilePath = "";
    fFileStream.close();
  }
}

/*************************************************************************/
//                            CheckLength
/*************************************************************************/
bool BinFile::CheckLength() {
  if (!IsOpen()) return false;
  std::streampos fFilePos = fFileStream.tellg();
  if (fFilePos >= fFileLength)
    return false;
  return true;
}

std::streampos BinFile::GetPosition() {
  if (!IsOpen()) return -1;
  std::streampos fFilePos = fFileStream.tellg();
  return fFilePos;
}

#endif // __BIN_FILE_CPP__

